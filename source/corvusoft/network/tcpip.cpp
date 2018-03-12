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
#include <system_error>
#include <netinet/tcp.h>

//Project Includes
#include "corvusoft/network/tcpip.hpp"
#include "corvusoft/network/detail/tcpip_impl.hpp"

//External Includes
#include <corvusoft/core/settings.hpp>
#include <corvusoft/core/run_loop.hpp>

//System Namespaces
using std::size_t;
using std::string;
using std::memset;
using std::function;
using std::to_string;
using std::error_code;
using std::shared_ptr;
using std::make_shared;
using std::make_error_code;
using std::system_category;
using std::placeholders::_1;
using std::placeholders::_2;

//Project Namespaces
using corvusoft::network::detail::TCPIPImpl;

//External Namespaces
using corvusoft::core::Bytes;
using corvusoft::core::RunLoop;
using corvusoft::core::Settings;
using corvusoft::network::Adaptor;

namespace corvusoft
{
    namespace network
    {
        TCPIP::TCPIP( const shared_ptr< RunLoop >& runloop ) : Adaptor( ),
            m_pimpl( new TCPIPImpl )
        {
            //assert( runloop == nullptr, "error message");
            m_pimpl->runloop = runloop;
        }
        
        TCPIP::~TCPIP( void )
        {
            return;
        }
        
        error_code TCPIP::setup( const shared_ptr< const Settings >& settings )
        {
            return error_code( );
        }
        
        error_code TCPIP::teardown( void )
        {
            return error_code( );
        }
        
        void TCPIP::open( const shared_ptr< const Settings >& settings, const function< error_code ( const shared_ptr< Adaptor >, const error_code ) > completion_handler )
        {
            if ( settings == nullptr ) completion_handler( shared_from_this( ), make_error_code( std::errc::invalid_argument ) );
            if ( not settings->has( "port" ) ) completion_handler( shared_from_this( ), make_error_code( std::errc::invalid_argument ) );
            if ( not settings->has( "address" ) ) completion_handler( shared_from_this( ), make_error_code( std::errc::invalid_argument ) );
            
            m_pimpl->socket = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
            if ( m_pimpl->socket < 0 )
                completion_handler( shared_from_this( ), error_code( errno, system_category( ) ) );
                
            int status = fcntl( m_pimpl->socket, F_SETFL, fcntl( m_pimpl->socket, F_GETFL, 0 ) | O_NONBLOCK );
            if ( status < 0 )
                completion_handler( shared_from_this( ), error_code( errno, system_category( ) ) );
                
            const uint16_t port = settings->get( "port", 0 );
            const socklen_t length = sizeof( struct sockaddr_in );
            memset( &m_pimpl->endpoint, 0, length );
            m_pimpl->endpoint.sin_family = PF_INET;
            m_pimpl->endpoint.sin_port = htons( port );
            
            const string address = settings->get( "address" );
            status = inet_pton( PF_INET, address.data( ), &( m_pimpl->endpoint.sin_addr.s_addr ) );
            if ( status == 0 )
                completion_handler( shared_from_this( ), error_code( errno, system_category( ) ) );
                
            //if settings->get( "timeout", 30 );
            
            //if settings->get( "keep-alive", true );
            int val = 1;
            setsockopt( m_pimpl->socket, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof val );
            
            status = connect( m_pimpl->socket, reinterpret_cast< struct sockaddr* >( &( m_pimpl->endpoint ) ), length );
            if ( status == -1 and errno not_eq EINPROGRESS )
                completion_handler( shared_from_this( ), error_code( errno, system_category( ) ) );
                
            m_pimpl->runloop->launch( bind( m_pimpl->socket_task, shared_from_this( ), POLLOUT, completion_handler ), detail::RUNLOOP_KEY );
        }
        
        void TCPIP::close( const function< error_code ( const shared_ptr< Adaptor >, const error_code ) > completion_handler )
        {
            m_pimpl->runloop->cancel( detail::RUNLOOP_KEY );
            m_pimpl->runloop->launch( bind( m_pimpl->socket_task, shared_from_this( ), POLLHUP, completion_handler ), detail::RUNLOOP_KEY );
            
            const int status = ::close( m_pimpl->socket );
            if ( status == -1 )
                completion_handler( shared_from_this( ), error_code( errno, system_category( ) ) );
                
            completion_handler( shared_from_this( ), error_code( ) );
        }
        
        void TCPIP::consume( const function< error_code ( const shared_ptr< Adaptor >, const Bytes, const error_code ) > completion_handler )
        {
            const function< error_code ( const shared_ptr< Adaptor >, const error_code ) > consumption_handler = bind( m_pimpl->consume, _1, _2, completion_handler );
            m_pimpl->runloop->launch( bind( m_pimpl->socket_task, shared_from_this( ), POLLIN | POLLPRI, consumption_handler ), detail::RUNLOOP_KEY );
        }
        
        void TCPIP::produce( const Bytes& data, const function< error_code ( const shared_ptr< Adaptor >, const size_t, const error_code ) > completion_handler )
        {
            m_pimpl->runloop->launch( bind( m_pimpl->produce, shared_from_this( ), data, completion_handler ), detail::RUNLOOP_KEY );
        }
        
        /*
         * note in documentation, setting a port, etc.. to a negative value will be converted signed int.
         * backlog is limited to 128
         */
        void TCPIP::listen( const shared_ptr< const Settings >& settings, const function< error_code ( const shared_ptr< Adaptor >, const error_code ) > accept_handler )
        {
            // if ( m_pimpl->is_in_use ) return make_error_code( std::errc::already_connected );
            // if ( m_pimpl->runloop == nullptr ) m_pimpl->runloop = make_shared< RunLoop >( );
            
            // shared_ptr< const Settings > options = settings;
            // if ( options == nullptr ) options = make_shared< Settings >( );
            
            // const unsigned short port = options->get( "port", 0 );
            // const unsigned int backlog = options->get( "backlog", SOMAXCONN );
            
            // ///m_pimpl->message_handler = accept_handler;
            // const auto message_handler = m_pimpl->original_message_handler;
            // m_pimpl->message_handler = [ this, settings, message_handler ]
            // {
            //     static socklen_t length = sizeof( struct sockaddr_in );
            //     struct sockaddr_in endpoint;
            //     memset( &endpoint, 0, length );
            
            //     struct pollfd peer;
            //     peer.revents = 0;
            //     peer.events = POLLIN | POLLPRI | POLLOUT | POLLERR | POLLHUP | POLLNVAL;
            //     peer.fd = accept( m_pimpl->peer.fd, reinterpret_cast< struct sockaddr* >( &endpoint ), &length );
            //     if ( peer.fd < 0 ) //test also if eagin or ewouldblock and just return, no error;
            //     {
            //         m_pimpl->error( errno );
            //         return;
            //     }
            
            //     const int status = fcntl( peer.fd, F_SETFL, fcntl( peer.fd, F_GETFL, 0 ) | O_NONBLOCK ); //extract inner fcntl!!!
            //     if ( status < 0 )
            //     {
            //         m_pimpl->error( errno );
            //         return;
            //     }
            
            //     int val = 1;
            //     setsockopt( peer.fd, IPPROTO_TCP, SO_KEEPALIVE, &val, sizeof val );
            
            //     auto adaptor = shared_ptr< TCPIP >( new TCPIP( "child" ) ); //generate random key? no use address:port
            //     adaptor->setup( m_pimpl->runloop, settings );
            //     adaptor->m_pimpl->peer = peer;
            //     adaptor->m_pimpl->endpoint = endpoint;
            //     adaptor->m_pimpl->message_handler = [ adaptor, message_handler ] //bind?
            //     {
            //         message_handler( adaptor );
            //     };
            //     adaptor->m_pimpl->error_handler = [ ]( error_code )
            //     {
            //         fprintf( stderr, "[child] error handler called.\n" );
            //     };
            //     adaptor->m_pimpl->close_handler = [ ]
            //     {
            //         fprintf( stderr, "[child] close handler called.\n" );
            //     };
            
            //     m_pimpl->runloop->launch( bind( TCPIPImpl::event_monitor, adaptor->m_pimpl ), m_pimpl->name );
            // };
            
            // m_pimpl->peer.revents = 0;
            // m_pimpl->peer.events = POLLIN | POLLPRI | POLLOUT | POLLERR | POLLHUP | POLLNVAL;
            // m_pimpl->peer.fd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
            // if ( m_pimpl->peer.fd < 0 ) return m_pimpl->error( errno );
            
            // int status = fcntl( m_pimpl->peer.fd, F_SETFL, fcntl( m_pimpl->peer.fd, F_GETFL, 0 ) | O_NONBLOCK );
            // if ( status < 0 ) return m_pimpl->error( errno );
            
            // int val = 1;
            // setsockopt( m_pimpl->peer.fd, IPPROTO_TCP, SO_KEEPALIVE, &val, sizeof val );
            
            // status = 1;
            // status = setsockopt( m_pimpl->peer.fd, SOL_SOCKET, SO_REUSEADDR, &status, sizeof( status ) );
            // if ( status == -1 ) return m_pimpl->error( errno );
            
            // const socklen_t length = sizeof( struct sockaddr_in );
            
            // struct sockaddr_in endpoint;
            // memset( &endpoint, 0, length );
            // endpoint.sin_family = PF_INET;
            // endpoint.sin_port = htons( port );
            // endpoint.sin_addr.s_addr = INADDR_ANY;
            
            // status = bind( m_pimpl->peer.fd, reinterpret_cast< struct sockaddr* >( &endpoint ), length );
            // if ( status == -1 ) return m_pimpl->error( errno );
            
            // status = ::listen( m_pimpl->peer.fd, backlog );
            // if ( status == -1 ) return m_pimpl->error( errno );
            
            // m_pimpl->endpoint = endpoint;
            // m_pimpl->runloop->launch( bind( TCPIPImpl::event_monitor, m_pimpl ), m_pimpl->name );
            
            // m_pimpl->is_in_use = true;
            
            // if ( m_pimpl->open_handler not_eq nullptr ) //why doesnt the poll tell us this?
            //     m_pimpl->runloop->launch( [ this ]( )
            // {
            //     m_pimpl->open_handler( );
            //     return error_code( );
            // } ); //remove wrapper? static
        }
        
        string TCPIP::get_local_endpoint( void )
        {
            const uint16_t port = ntohs( m_pimpl->endpoint.sin_port );
            const string address = inet_ntoa( m_pimpl->endpoint.sin_addr );
            //if version 6 and brackets [::1]:80
            return address + ":" + to_string( port );
        }
        
        string TCPIP::get_remote_endpoint( void )
        {
            struct sockaddr_in endpoint;
            socklen_t size = sizeof( endpoint );
            
            getpeername( m_pimpl->socket, reinterpret_cast< struct sockaddr*>( &endpoint ), &size );
            
            const uint16_t port = ntohs( endpoint.sin_port );
            const string address = inet_ntoa( endpoint.sin_addr );
            //if version 6 and brackets [::1]:80
            return address + ":" + to_string( port );
        }
    }
}
