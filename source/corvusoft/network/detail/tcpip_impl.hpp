/*
 * Copyright 2013-2018, Corvusoft Ltd, All Rights Reserved.
 */

#pragma once

//System Includes
#include <poll.h>
#include <string>
#include <memory>
#include <cstring>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <functional>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <system_error>
#include <netinet/tcp.h>

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
                
                int socket_timeout = 0;
                
                struct addrinfo hints { };
                
                struct addrinfo* info = nullptr;
                
                std::shared_ptr< core::RunLoop > runloop = nullptr;
                
                void open( const std::shared_ptr< Adaptor > adaptor, const std::function< std::error_code ( const std::shared_ptr< Adaptor >, const std::error_code ) > completion_handler )
                {
                    if ( info == nullptr )
                    {
                        freeaddrinfo( info );
                        completion_handler( adaptor, std::error_code( errno, std::system_category( ) ) );
                        return;
                    }
                    
                    socket = ::socket( info->ai_family, info->ai_socktype, info->ai_protocol );
                    if ( socket < 0 )
                    {
                        info = info->ai_next;
                        return open( adaptor, completion_handler );
                    }
                    
                    struct timeval timeout { };
                    timeout.tv_sec = socket_timeout;
                    timeout.tv_usec = 0;
                    ::setsockopt( socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof( timeout ) );
                    
                    int value = 1;
                    ::setsockopt( socket, SOL_SOCKET, SO_KEEPALIVE, &value, sizeof( value ) );
                    
                    int status = ::fcntl( socket, F_SETFL, fcntl( socket, F_GETFL, 0 ) | O_NONBLOCK );
                    if ( status < 0 )
                    {
                        completion_handler( adaptor, std::error_code( errno, std::system_category( ) ) );
                        return;
                    }
                    
                    status = ::connect( socket, info->ai_addr, info->ai_addrlen );
                    if ( status == -1 and errno not_eq EINPROGRESS )
                    {
                        ::close( socket );
                        info = info->ai_next;
                        return open( adaptor, completion_handler );
                    }
                    
                    runloop->launch( bind( socket_task, adaptor, POLLOUT, completion_handler ), detail::RUNLOOP_KEY );
                }
                
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
                    {
                        if ( peer.revents & POLLHUP )
                            return completion_handler( adaptor, std::make_error_code( std::errc::connection_reset ) );
                        else if ( peer.revents & POLLERR or peer.revents & POLLNVAL )
                        {
                            int error = 0;
                            socklen_t length = sizeof( error );
                            ::getsockopt( socket, SOL_SOCKET, SO_ERROR, ( void* ) &error, &length );
                            return completion_handler( adaptor, std::error_code( error, std::system_category( ) ) );
                        }
                        else if ( peer.revents & events )
                            return completion_handler( adaptor, std::error_code( ) );
                    }
                    
                    return std::make_error_code( std::errc::resource_unavailable_try_again );
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
                        return std::make_error_code( std::errc::resource_unavailable_try_again );
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
                        return std::make_error_code( std::errc::resource_unavailable_try_again );
                        
                    return completion_handler( adaptor, size, std::error_code( ) );
                };
            };
        }
    }
}
