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
            const int status = ::close( m_pimpl->peer.fd );// shutdown is better?, SHUT_RDWR );
            
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
        
        error_code TCPIPAdaptor::open( const shared_ptr< const Settings >& settings )
        {
            if ( m_pimpl->is_in_use ) return make_error_code( std::errc::already_connected );
            if ( settings == nullptr ) return make_error_code( std::errc::invalid_argument );
            
            if ( m_pimpl->runloop == nullptr ) m_pimpl->runloop = make_shared< RunLoop >( );
            //if ( m_pimpl->peer.events == 0 ) return error_code( );
            
            const string address = settings->get( "address" );
            const unsigned int port = settings->get( "port", 0 );
            
            //m_pimpl->runloop->launch( [ this, address, port ]( )
            //{
            struct pollfd peer;
            peer.revents = 0;
            peer.fd = socket( AF_INET, SOCK_STREAM, 0 );
            if ( peer.fd < 0 ) return m_pimpl->error( errno );
            
            int status = fcntl( peer.fd, F_SETFL, fcntl( peer.fd, F_GETFL, 0 ) | O_NONBLOCK );
            if ( status < 0 ) return m_pimpl->error( errno );
            
            const socklen_t length = sizeof( struct sockaddr_in );
            
            struct sockaddr_in endpoint;
            memset( &endpoint, 0, length );
            endpoint.sin_family = AF_INET;
            endpoint.sin_port = htons( port );
            
            status = inet_pton( AF_INET, address.data( ), &( endpoint.sin_addr.s_addr ) );
            if ( status == 0 ) return m_pimpl->error( errno );
            
            status = connect( peer.fd, reinterpret_cast< struct sockaddr* >( &endpoint ), length );
            if ( status == -1 and errno not_eq EINPROGRESS ) return m_pimpl->error( errno );
            
            m_pimpl->buffer = make_bytes( );
            m_pimpl->peer = peer;
            m_pimpl->endpoint = endpoint;
            m_pimpl->runloop->launch( bind( TCPIPAdaptorImpl::event_monitor, m_pimpl ), m_pimpl->key );
            
            return error_code( );
            //}, m_pimpl->key );
            
            //return error_code( );
        }
        
        error_code TCPIPAdaptor::listen( const shared_ptr< const Settings >& settings )
        {
            if ( m_pimpl->is_in_use ) return make_error_code( std::errc::already_connected );
            if ( m_pimpl->runloop == nullptr ) m_pimpl->runloop = make_shared< RunLoop >( );
            
            shared_ptr< const Settings > options = settings;
            if ( options == nullptr ) options = make_shared< Settings >( );
            
            const unsigned int port = options->get( "port", 0 );
            const int backlog = options->get( "backlog", SOMAXCONN );
            //const string bind_address = options->get( "address", "" );
            
            m_pimpl->message_handler = [ this, settings ]
            {
                static socklen_t length = sizeof( struct sockaddr_in );
                struct sockaddr_in endpoint;
                memset( &endpoint, 0, length );
                
                struct pollfd peer;
                peer.revents = 0;
                peer.events = 0;
                peer.fd = accept( m_pimpl->peer.fd, reinterpret_cast< struct sockaddr* >( &endpoint ), &length );
                if ( peer.fd < 0 ) //test also if eagin or ewouldblock and just return, no error;
                {
                    m_pimpl->error( errno );
                    return;
                }
                
                int status = fcntl( peer.fd, F_SETFL, fcntl( peer.fd, F_GETFL, 0 ) | O_NONBLOCK );
                if ( status < 0 )
                {
                    m_pimpl->error( errno );
                    return;
                }
                
                auto adaptor = shared_ptr< TCPIPAdaptor >( new TCPIPAdaptor( "" ) ); //generate random key? no use address:port
                adaptor->setup( m_pimpl->runloop, settings );
                adaptor->m_pimpl->peer = peer;
                //adaptor->m_pimpl->parent = shared_from_this( );
                //adaptor->m_pimpl->buffer = make_bytes( ); //not required as its setup in the pimpl. is this to clear it????
                adaptor->m_pimpl->endpoint = endpoint;
                //m_pimpl->children.push_back( adaptor );
                
                //the name could be the unique address:port string from get_remote_endpoint
                m_pimpl->runloop->launch( bind( TCPIPAdaptorImpl::event_monitor, adaptor->m_pimpl ), m_pimpl->key );
            };
            
            //m_pimpl->runloop->launch( [ this, port, backlog ]( )
            //{
            m_pimpl->peer.revents = 0;
            m_pimpl->peer.events = POLLIN | POLLPRI | POLLOUT | POLLERR | POLLHUP | POLLNVAL | POLLRDBAND | POLLRDNORM | POLLWRBAND | POLLWRNORM;
            m_pimpl->peer.fd = socket( AF_INET, SOCK_STREAM, 0 );
            if ( m_pimpl->peer.fd < 0 ) return m_pimpl->error( errno );
            
            int status = fcntl( m_pimpl->peer.fd, F_SETFL, fcntl( m_pimpl->peer.fd, F_GETFL, 0 ) | O_NONBLOCK );
            if ( status < 0 ) return m_pimpl->error( errno );
            
            status = 1;
            status = setsockopt( m_pimpl->peer.fd, SOL_SOCKET, SO_REUSEADDR, &status, sizeof( status ) );
            if ( status == -1 ) return m_pimpl->error( errno );
            
            const socklen_t length = sizeof( struct sockaddr_in );
            
            struct sockaddr_in endpoint;
            memset( &endpoint, 0, length );
            endpoint.sin_family = AF_INET;
            endpoint.sin_port = htons( port );
            endpoint.sin_addr.s_addr = INADDR_ANY;
            
            status = bind( m_pimpl->peer.fd, reinterpret_cast< struct sockaddr* >( &endpoint ), length );
            if ( status == -1 ) return m_pimpl->error( errno );
            
            status = ::listen( m_pimpl->peer.fd, backlog );
            if ( status == -1 ) return m_pimpl->error( errno );
            
            m_pimpl->endpoint = endpoint;
            m_pimpl->runloop->launch( bind( TCPIPAdaptorImpl::event_monitor, m_pimpl ), m_pimpl->key );
            
            m_pimpl->is_in_use = true;
            
            if ( m_pimpl->open_handler not_eq nullptr )
                m_pimpl->runloop->launch( [ this ]( )
            {
                m_pimpl->open_handler( );
                return error_code( );
            } ); //remove wrapper? static
            
            return error_code( );
            //}, m_pimpl->key );
            
            //return error_code( );
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
                size = recv( m_pimpl->peer.fd, data_ptr, length, 0 );
                if ( size < 0 )
                {
                    error = m_pimpl->error( errno, ( errno not_eq EAGAIN ) );
                    //size = 0;
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
            ssize_t size = send( m_pimpl->peer.fd, data.data( ), data.size( ), 0 );
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
            
            //m_pimpl->peer.events ^= POLLOUT;
        }
        
        void TCPIPAdaptor::set_close_handler( const function< void ( const shared_ptr< Adaptor > ) >& value )
        {
            if ( value == nullptr ) m_pimpl->close_handler = nullptr;
            //else m_pimpl->close_handler = bind( value, shared_from_this( ) );
            else m_pimpl->close_handler = [ this, value ]
            {
                m_pimpl->is_in_use = false;
                value( shared_from_this( ) );
            };
            
            //m_pimpl->peer.events ^= POLLHUP; //POLLRDHUP | POLLHUP; RDHUP is GNU specific.
        }
        
        void TCPIPAdaptor::set_error_handler( const function< void ( const shared_ptr< Adaptor >, const error_code error ) >& value )
        {
            if ( value == nullptr ) m_pimpl->error_handler = nullptr;
            else m_pimpl->error_handler = bind( value, shared_from_this( ), _1 );
            
            //m_pimpl->peer.events ^= POLLERR | POLLNVAL; //do we not always want to catch this and call our error handler, yes.
        }
        
        void TCPIPAdaptor::set_message_handler( const function< void ( const shared_ptr< Adaptor > ) >& value )
        {
            if ( value == nullptr ) m_pimpl->message_handler = nullptr;
            else m_pimpl->message_handler = [ this, value ]
            {
                //consume( ); //this will have broken something.
                value( shared_from_this( ) );
            };
            
            //m_pimpl->peer.events ^= POLLIN | POLLPRI; //what about when we nullptr the value? need to clear them bits.
        }
        
        TCPIPAdaptor::TCPIPAdaptor( const string& key ) : Adaptor( key ),
            m_pimpl( new TCPIPAdaptorImpl )
        {
            m_pimpl->key = key;
        }
    }
}
