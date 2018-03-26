/*
 * Copyright 2013-2018, Corvusoft Ltd, All Rights Reserved.
 */

//System Includes
#include <string>
#include <system_error>

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
            m_pimpl->runloop = runloop;
        }
        
        TCPIP::~TCPIP( void )
        {
            return;
        }
        
        error_code TCPIP::setup( const shared_ptr< const Settings >& )
        {
            return error_code( );
        }
        
        error_code TCPIP::teardown( void )
        {
            return error_code( );
        }
        
        void TCPIP::open( const shared_ptr< const Settings >& settings, const function< error_code ( const shared_ptr< Adaptor >, const error_code ) > completion_handler )
        {
            if ( completion_handler == nullptr ) return;
            
            if ( settings == nullptr or settings->has( "port" ) == false or settings->has( "address" ) == false )
            {
                completion_handler( shared_from_this( ), make_error_code( std::errc::invalid_argument ) );
                return;
            }
            
            string address = settings->get( "address" );
            string port = ::to_string( settings->get( "port", 0 ) );
            m_pimpl->socket_timeout = settings->get( "timeout", 30 );
            
            memset( &m_pimpl->hints, 0, sizeof( m_pimpl->hints ) );
            m_pimpl->hints.ai_family = AF_UNSPEC;
            m_pimpl->hints.ai_socktype = SOCK_STREAM;
            m_pimpl->hints.ai_protocol = IPPROTO_TCP;
            
            int status = getaddrinfo( address.data( ), port.data( ), &m_pimpl->hints, &m_pimpl->info );
            if ( status not_eq 0 )
                completion_handler( shared_from_this( ), error_code( errno, system_category( ) ) );
            else
                m_pimpl->open( shared_from_this( ), completion_handler );
        }
        
        void TCPIP::close( const function< error_code ( const shared_ptr< Adaptor >, const error_code ) > completion_handler )
        {
            if ( completion_handler == nullptr ) return;
            
            m_pimpl->runloop->cancel( detail::RUNLOOP_KEY );
            m_pimpl->runloop->launch( bind( m_pimpl->socket_task, shared_from_this( ), POLLHUP, completion_handler ), detail::RUNLOOP_KEY );
            
            const int status = ::close( m_pimpl->socket );
            if ( status == -1 )
            {
                completion_handler( shared_from_this( ), error_code( errno, system_category( ) ) );
                return;
            }
            
            m_pimpl->socket = -1;
            freeaddrinfo( m_pimpl->info );
            
            completion_handler( shared_from_this( ), error_code( ) );
        }
        
        void TCPIP::consume( const function< error_code ( const shared_ptr< Adaptor >, const Bytes, const error_code ) > completion_handler )
        {
            if ( completion_handler == nullptr ) return;
            
            const function< error_code ( const shared_ptr< Adaptor >, const error_code ) > consumption_handler = bind( m_pimpl->consume, _1, _2, completion_handler );
            m_pimpl->runloop->launch( bind( m_pimpl->socket_task, shared_from_this( ), POLLIN | POLLPRI, consumption_handler ), detail::RUNLOOP_KEY );
        }
        
        void TCPIP::produce( const Bytes& data, const function< error_code ( const shared_ptr< Adaptor >, const size_t, const error_code ) > completion_handler )
        {
            if ( completion_handler == nullptr ) return;
            
            m_pimpl->runloop->launch( bind( m_pimpl->produce, shared_from_this( ), data, completion_handler ), detail::RUNLOOP_KEY );
        }
        
        void TCPIP::listen( const shared_ptr< const Settings >&, const function< error_code ( const shared_ptr< Adaptor >, const error_code ) > )
        {
            return;
        }
    }
}
