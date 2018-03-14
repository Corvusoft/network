/*
 * Copyright 2013-2018, Corvusoft Ltd, All Rights Reserved.
 */

#ifndef _CORVUSOFT_NETWORK_DETAIL_TCPIP_IMPL_H
#define _CORVUSOFT_NETWORK_DETAIL_TCPIP_IMPL_H 1

//System Includes
#include <poll.h>
#include <string>
#include <memory>
#include <functional>
#include <sys/types.h>
#include <system_error>

//Project Includes

//External Includes
#include <corvusoft/core/byte.hpp>
#include <corvusoft/core/run_loop.hpp>

//System Namespaces

//Project Namespaces

//External Namespaces

namespace corvusoft
{
    //Forward Declarations
    
    namespace network
    {
        //Forward Declarations
        
        namespace detail
        {
            //Forward Declarations
            static const std::string RUNLOOP_KEY = "corvusoft::network::adaptor::tcpip";
            
            struct TCPIPImpl
            {
                int socket = -1;
                
                struct sockaddr_in endpoint { };
                
                std::shared_ptr< core::RunLoop > runloop = nullptr;
                
                const std::function< std::error_code ( const std::shared_ptr< Adaptor >, short, const std::function< std::error_code ( const std::shared_ptr< Adaptor >, const std::error_code ) > ) > socket_task = [ this ]( auto adaptor, auto events, auto completion_handler )
                {
                    struct pollfd peer;
                    peer.fd = socket;
                    peer.revents = 0;
                    peer.events = events | POLLERR | POLLHUP | POLLNVAL;
                    
                    static const int TIMEOUT = 0;
                    static const int SINGLE_FILE_DESCRIPTOR = 1;
                    const int status = poll( &peer, SINGLE_FILE_DESCRIPTOR, TIMEOUT );
                    if ( status == -1 and errno not_eq EINPROGRESS )
                        return completion_handler( adaptor, std::error_code( errno, std::system_category( ) ) );
                    else if ( status == 1 )
                        if ( peer.revents & POLLHUP )
                            return completion_handler( adaptor, std::make_error_code( std::errc::connection_reset ) );
                        else if ( peer.revents & POLLERR or peer.revents & POLLNVAL )
                        {
                            int error = 0;
                            socklen_t length = sizeof( error );
                            ::getsockopt( socket, SOL_SOCKET, SO_ERROR, ( void* ) &error, &length );
                            return completion_handler( adaptor, std::error_code( error, std::system_category( ) ) );
                        }
                        else if ( peer.revents & events ) return completion_handler( adaptor, std::error_code( ) );
                        
                    runloop->launch( std::bind( socket_task, adaptor, POLLOUT, completion_handler ), RUNLOOP_KEY );
                    return std::error_code( );
                };
                
                const std::function< std::error_code ( const std::shared_ptr< Adaptor >, const std::error_code, const std::function< std::error_code ( const std::shared_ptr< Adaptor >, const core::Bytes, const std::error_code ) > ) > consume = [ this ]( auto adaptor, auto status, auto completion_handler )
                {
                    core::Bytes buffer { };
                    if ( status ) return completion_handler( adaptor, buffer, status );
                    
                    const std::size_t length = 1024;
                    core::Byte data[ length + 1 ];
                    
                    memset( &data, 0, length + 1 );
                    static const int FLAGS = 0;
                    ssize_t size = ::recv( socket, data, length, FLAGS );
                    if ( size == -1 and errno not_eq EAGAIN )
                        return completion_handler( adaptor, buffer, std::error_code( errno, std::system_category( ) ) );
                        
                    if ( size == -1 and errno == EAGAIN )
                    {
                        const std::function< std::error_code ( const std::shared_ptr< Adaptor >, const std::error_code ) > consumption_handler = std::bind( consume, std::placeholders::_1, std::placeholders::_2, completion_handler );
                        runloop->launch( std::bind( socket_task, adaptor, POLLIN | POLLPRI, consumption_handler ), detail::RUNLOOP_KEY );
                        return std::error_code( );//return eagain and have the runloop rescheulde us; that removes the 2 lines above!
                    }
                    
                    if ( size == 0 )
                        return completion_handler( adaptor, buffer, std::make_error_code( std::errc::connection_reset ) );
                        
                    buffer.insert( buffer.end( ), &data[ 0 ], &data[ size ] );
                    return completion_handler( adaptor, buffer, std::error_code( ) );
                };
                
                const std::function< std::error_code ( const std::shared_ptr< Adaptor >, const core::Bytes, const std::function< std::error_code ( const std::shared_ptr< Adaptor >, const std::size_t, const std::error_code ) > ) > produce = [ this ]( auto adaptor, auto data, auto completion_handler )
                {
                    static const int FLAGS = 0;
                    ssize_t size = ::send( socket, data.data( ), data.size( ), FLAGS );
                    if ( size == -1 and errno not_eq EAGAIN )
                        return completion_handler( adaptor, 0, std::error_code( errno, std::system_category( ) ) );
                        
                    if ( size == -1 and errno == EAGAIN )
                    {
                        runloop->launch( std::bind( produce, adaptor, data, completion_handler ), detail::RUNLOOP_KEY );
                        return std::error_code( ); //return eagain and have the runloop rescheulde us; that removes the 1 line above!
                    }
                    
                    return completion_handler( adaptor, size, std::error_code( ) );
                };
            };
        }
    }
}

#endif  /* _CORVUSOFT_NETWORK_DETAIL_TCPIP_IMPL_H */
