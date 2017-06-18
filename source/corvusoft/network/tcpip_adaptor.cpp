/*
 * Copyright 2013-2017, Corvusoft Ltd, All Rights Reserved.
 */

//System Includes
#include <poll.h>
#include <string>
#include <cstring>
#include <cassert>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

//Project Includes
#include "corvusoft/network/tcpip_adaptor.hpp"
#include "corvusoft/network/detail/tcpip_adaptor_impl.hpp"

//External Includes
#include <corvusoft/core/settings.hpp>
#include <corvusoft/core/run_loop.hpp>

//System Namespaces
using std::size_t;
using std::string;
using std::memset;
using std::function;
using std::to_string;
using std::bad_alloc;
using std::error_code;
using std::unique_ptr;
using std::shared_ptr;
using std::make_shared;
using std::make_unique;
using std::placeholders::_1;

//Project Namespaces
using corvusoft::network::detail::TCPIPAdaptorImpl;

//External Namespaces
using corvusoft::core::Byte;
using corvusoft::core::Bytes;
using corvusoft::core::make_bytes;
using corvusoft::core::Settings;
using corvusoft::core::RunLoop;
using corvusoft::network::Adaptor;

namespace corvusoft
{
    namespace network
    {
        shared_ptr< Adaptor > TCPIPAdaptor::create( const string& key )
        {
            return shared_ptr< TCPIPAdaptor >( new TCPIPAdaptor( key ) );
        }
        
        TCPIPAdaptor::~TCPIPAdaptor( void )
        {
            try
            {
                close( );
            }
            catch ( ... )
            {
                return;
            }
        }
        
        error_code TCPIPAdaptor::teardown( void )
        {
            m_pimpl->is_in_use = false;
            close( );
            return error_code( );
        }
        
        error_code TCPIPAdaptor::setup( const shared_ptr< const Settings >& settings )
        {
            return setup( nullptr, settings );
        }
        
        error_code TCPIPAdaptor::setup( const shared_ptr< RunLoop >& runloop, const shared_ptr< const Settings >& )
        {
            m_pimpl->runloop = runloop;
            return error_code( );
        }
        
        error_code TCPIPAdaptor::close( void )
        {
            m_pimpl->is_closed = true;
            m_pimpl->is_in_use = false;
            
            const int status = ::close( m_pimpl->peer.fd );
            
            if ( m_pimpl->runloop not_eq nullptr )
            {
                m_pimpl->runloop->cancel( m_pimpl->key );
                
                if ( m_pimpl->close_handler not_eq nullptr )
                    m_pimpl->runloop->launch( [ this ]( )
                {
                    m_pimpl->close_handler( );
                    return error_code( );
                } );
            }
            
            return ( status == -1 ) ? m_pimpl->error( errno, false ) : error_code( );
        }
        
        //document port will be truncated if negative.
        error_code TCPIPAdaptor::open( const shared_ptr< const Settings >& settings )
        {
            if ( m_pimpl->is_in_use ) return make_error_code( std::errc::already_connected ); //message required
            if ( settings == nullptr ) return make_error_code( std::errc::invalid_argument ); //message required
            
            if ( not settings->has( "port" ) ) return make_error_code( std::errc::invalid_argument ); //"address and/or port is malformed."
            if ( not settings->has( "address" ) ) return make_error_code( std::errc::invalid_argument ); //"address is malformed, expected IPv4/6 string."
            
            if ( m_pimpl->runloop == nullptr ) m_pimpl->runloop = make_shared< RunLoop >( );
            
            const string address = settings->get( "address" );
            const short port = settings->get( "port", 0 );
            //socket timeout.
            
            struct pollfd peer;
            peer.revents = 0;
            peer.events = POLLIN | POLLPRI | POLLOUT | POLLERR | POLLHUP | POLLNVAL;
            peer.fd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
            if ( peer.fd < 0 ) return m_pimpl->error( errno );
            
            const socklen_t length = sizeof( struct sockaddr_in );
            
            struct sockaddr_in endpoint;
            memset( &endpoint, 0, length );
            endpoint.sin_family = PF_INET;
            endpoint.sin_port = htons( port );
            
            int status = inet_pton( PF_INET, address.data( ), &( endpoint.sin_addr.s_addr ) );
            if ( status == 0 ) return m_pimpl->error( errno );
            
            status = connect( peer.fd, reinterpret_cast< struct sockaddr* >( &endpoint ), length );
            if ( status == -1 ) return m_pimpl->error( errno );
            
            int val = 1;
            setsockopt( peer.fd, IPPROTO_TCP, SO_KEEPALIVE, &val, sizeof val );
            
            status = fcntl( peer.fd, F_SETFL, fcntl( peer.fd, F_GETFL, 0 ) | O_NONBLOCK ); //remove inner fcntl
            if ( status < 0 ) return m_pimpl->error( errno );
            
            //if null leave null
            const auto message_handler = m_pimpl->original_message_handler;
            m_pimpl->message_handler = [ this, message_handler ]
            {
                message_handler( shared_from_this( ) );
            };
            
            m_pimpl->buffer = make_bytes( );
            m_pimpl->peer = peer;
            m_pimpl->endpoint = endpoint;
            m_pimpl->runloop->launch( bind( TCPIPAdaptorImpl::event_monitor, m_pimpl ), m_pimpl->key );
            
            return error_code( );
        }
        
        /*
         * note in documentation, setting a port, etc.. to a negative value will be converted signed int.
         * backlog is limited to 128
         */
        error_code TCPIPAdaptor::listen( const shared_ptr< const Settings >& settings )
        {
            if ( m_pimpl->is_in_use ) return make_error_code( std::errc::already_connected );
            if ( m_pimpl->runloop == nullptr ) m_pimpl->runloop = make_shared< RunLoop >( );
            
            shared_ptr< const Settings > options = settings;
            if ( options == nullptr ) options = make_shared< Settings >( );
            
            const unsigned short port = options->get( "port", 0 );
            const unsigned int backlog = options->get( "backlog", SOMAXCONN );
            //socket timeout.
            
            ///m_pimpl->message_handler = accept_handler;
            const auto message_handler = m_pimpl->original_message_handler;
            m_pimpl->message_handler = [ this, settings, message_handler ]
            {
                fprintf( stderr, "accept handler called\n" );
                static socklen_t length = sizeof( struct sockaddr_in );
                struct sockaddr_in endpoint;
                memset( &endpoint, 0, length );
                
                struct pollfd peer;
                peer.revents = 0;
                peer.events = POLLIN | POLLPRI | POLLOUT | POLLERR | POLLHUP | POLLNVAL;
                peer.fd = accept( m_pimpl->peer.fd, reinterpret_cast< struct sockaddr* >( &endpoint ), &length );
                if ( peer.fd < 0 ) //test also if eagin or ewouldblock and just return, no error;
                {
                    m_pimpl->error( errno );
                    return;
                }
                
                const int status = fcntl( peer.fd, F_SETFL, fcntl( peer.fd, F_GETFL, 0 ) | O_NONBLOCK ); //extract inner fcntl!!!
                if ( status < 0 )
                {
                    m_pimpl->error( errno );
                    return;
                }
                
                int val = 1;
                setsockopt( peer.fd, IPPROTO_TCP, SO_KEEPALIVE, &val, sizeof val );
                
                auto adaptor = shared_ptr< TCPIPAdaptor >( new TCPIPAdaptor( "child" ) ); //generate random key? no use address:port
                adaptor->setup( m_pimpl->runloop, settings );
                adaptor->m_pimpl->peer = peer;
                adaptor->m_pimpl->endpoint = endpoint;
                adaptor->m_pimpl->message_handler = [ adaptor, message_handler ] //bind?
                {
                    message_handler( adaptor );
                };
                adaptor->m_pimpl->error_handler = [ ]( error_code )
                {
                    fprintf( stderr, "[child] error handler called.\n" );
                };
                adaptor->m_pimpl->close_handler = [ ]
                {
                    fprintf( stderr, "[child] close handler called.\n" );
                };
                
                fprintf( stderr, "CHILD WITH FD %d\n", adaptor->m_pimpl->peer.fd );
                
                m_pimpl->runloop->launch( bind( TCPIPAdaptorImpl::event_monitor, adaptor->m_pimpl ), m_pimpl->key );
            };
            
            m_pimpl->peer.revents = 0;
            m_pimpl->peer.events = POLLIN | POLLPRI | POLLOUT | POLLERR | POLLHUP | POLLNVAL;
            m_pimpl->peer.fd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
            if ( m_pimpl->peer.fd < 0 ) return m_pimpl->error( errno );
            
            int status = fcntl( m_pimpl->peer.fd, F_SETFL, fcntl( m_pimpl->peer.fd, F_GETFL, 0 ) | O_NONBLOCK );
            if ( status < 0 ) return m_pimpl->error( errno );
            
            int val = 1;
            setsockopt( m_pimpl->peer.fd, IPPROTO_TCP, SO_KEEPALIVE, &val, sizeof val );
            
            status = 1;
            status = setsockopt( m_pimpl->peer.fd, SOL_SOCKET, SO_REUSEADDR, &status, sizeof( status ) );
            if ( status == -1 ) return m_pimpl->error( errno );
            
            const socklen_t length = sizeof( struct sockaddr_in );
            
            struct sockaddr_in endpoint;
            memset( &endpoint, 0, length );
            endpoint.sin_family = PF_INET;
            endpoint.sin_port = htons( port );
            endpoint.sin_addr.s_addr = INADDR_ANY;
            
            status = bind( m_pimpl->peer.fd, reinterpret_cast< struct sockaddr* >( &endpoint ), length );
            if ( status == -1 ) return m_pimpl->error( errno );
            
            status = ::listen( m_pimpl->peer.fd, backlog );
            if ( status == -1 ) return m_pimpl->error( errno );
            
            m_pimpl->endpoint = endpoint;
            m_pimpl->runloop->launch( bind( TCPIPAdaptorImpl::event_monitor, m_pimpl ), m_pimpl->key );
            
            m_pimpl->is_in_use = true;
            
            if ( m_pimpl->open_handler not_eq nullptr ) //why doesnt the poll tell us this?
                m_pimpl->runloop->launch( [ this ]( )
            {
                m_pimpl->open_handler( );
                return error_code( );
            } ); //remove wrapper? static
            fprintf( stderr, "LISTEN WITH FD %d\n", m_pimpl->peer.fd );
            return error_code( ); //send back a message "Adaptor awaiting connections at...";
        }
        
        const Bytes TCPIPAdaptor::peek( error_code& error )
        {
            auto& buffer = m_pimpl->buffer;
            static const size_t length = 1024;
            
            unique_ptr< Byte[ ] > data = nullptr;
            try
            {
                data = make_unique< Byte[ ] >( length + 1 );
            }
            catch ( const bad_alloc& ba )
            {
                error = m_pimpl->error( errno, true );
                return buffer;
            }
            
            const auto data_ptr = data.get( );
            
            ssize_t size = 0;
            do
            {
                memset( data_ptr, 0, length + 1 );
                size = read( m_pimpl->peer.fd, data_ptr, length );
                if ( size < 0 )
                {
                    size = 0;
                    if ( errno not_eq EAGAIN )
                        error = m_pimpl->error( errno );
                    break;
                }
                
                buffer.insert( buffer.end( ), data_ptr, data_ptr + size );
            }
            while ( size not_eq 0 );
            
            buffer.shrink_to_fit( );
            return buffer;
        }
        
        const Bytes TCPIPAdaptor::consume( error_code& error )
        {
            const auto data = peek( error );
            m_pimpl->buffer.clear( );
            return data;
        }
        
        size_t TCPIPAdaptor::produce( const Bytes& data, error_code& error )
        {
            ssize_t size = write( m_pimpl->peer.fd, data.data( ), data.size( ) );
            if ( size < 0 )
            {
                size = 0;
                error = m_pimpl->error( errno );
            }
            
            return size;
        }
        
        string TCPIPAdaptor::get_key( void ) const
        {
            return m_pimpl->key;
        }
        
        string TCPIPAdaptor::get_local_endpoint( void )
        {
            const uint16_t port = ntohs( m_pimpl->endpoint.sin_port );
            const string address = inet_ntoa( m_pimpl->endpoint.sin_addr );
            //if version 6 and brackets [::1]:80
            return address + ":" + to_string( port );
        }
        
        string TCPIPAdaptor::get_remote_endpoint( void )
        {
            if ( not m_pimpl->is_in_use ) return get_local_endpoint( );
            
            struct sockaddr_in endpoint;
            socklen_t size = sizeof( endpoint );
            
            getpeername( m_pimpl->peer.fd, reinterpret_cast< struct sockaddr*>( &endpoint ), &size );
            
            const uint16_t port = ntohs( endpoint.sin_port );
            const string address = inet_ntoa( endpoint.sin_addr );
            //if version 6 and brackets [::1]:80
            return address + ":" + to_string( port );
        }
        
        shared_ptr< RunLoop > TCPIPAdaptor::get_runloop( void )
        {
            return m_pimpl->runloop;
        }
        
        void TCPIPAdaptor::set_open_handler( const function< void ( const shared_ptr< Adaptor > ) >& value )
        {
            if ( value == nullptr ) m_pimpl->open_handler = nullptr;
            else m_pimpl->open_handler = bind( value, shared_from_this( ) );
        }
        
        void TCPIPAdaptor::set_close_handler( const function< void ( const shared_ptr< Adaptor > ) >& value )
        {
            if ( value == nullptr ) m_pimpl->close_handler = nullptr;
            else m_pimpl->close_handler = [ this, value ]
            {
                m_pimpl->is_in_use = false;
                value( shared_from_this( ) );
            };
        }
        
        void TCPIPAdaptor::set_error_handler( const function< void ( const shared_ptr< Adaptor >, const error_code error ) >& value )
        {
            if ( value == nullptr ) m_pimpl->error_handler = nullptr;
            else m_pimpl->error_handler = bind( value, shared_from_this( ), _1 );
        }
        
        void TCPIPAdaptor::set_message_handler( const function< void ( const shared_ptr< Adaptor > ) >& value )
        {
            if ( value == nullptr ) m_pimpl->original_message_handler = nullptr;
            else m_pimpl->original_message_handler = value;
        }
        
        TCPIPAdaptor::TCPIPAdaptor( const string& key ) : Adaptor( key ),
            m_pimpl( new TCPIPAdaptorImpl )
        {
            m_pimpl->key = key;
        }
    }
}
