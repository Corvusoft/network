/*
 * Copyright 2013-2017, Corvusoft Ltd, All Rights Reserved.
 */

#ifndef _CORVUSOFT_NETWORK_DETAIL_TCPIP_ADAPTOR_IMPL_H
#define _CORVUSOFT_NETWORK_DETAIL_TCPIP_ADAPTOR_IMPL_H 1

//System Includes
#include <poll.h>
#include <string>
#include <memory>
#include <functional>
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
            
            struct TCPIPAdaptorImpl
            {
                std::string key = "";
                
                core::Bytes buffer { };
                
                struct pollfd peer
                {
                    -1, 0, 0
                };
                
                struct sockaddr_in endpoint { };
                
                bool is_in_use = false;
                
                bool is_closed = false;
                
                std::shared_ptr< core::RunLoop > runloop = nullptr;
                
                std::function< void ( void ) > open_handler = nullptr;
                
                std::function< void ( void ) > close_handler = nullptr;
                
                std::function< void ( void ) > message_handler = nullptr;
                
                std::function< void ( const std::shared_ptr< network::Adaptor > ) > original_message_handler = nullptr;
                
                std::function< void ( const std::error_code ) > error_handler = nullptr;
                
                std::error_code error( const int errnum, bool report = true )
                {
                    std::error_code error( errnum, std::system_category( ) );
                    if ( report and error_handler not_eq nullptr ) error_handler( error );
                    return error;
                }
                
                static std::error_code event_monitor( const std::shared_ptr< TCPIPAdaptorImpl > adaptor )
                {
                    static const int TIMEOUT = 1000;
                    static const int SINGLE_FILE_DESCRIPTOR = 1;
                    
                    //this is wrong! we launch the event monitor which blocks teh runloop.
                    //nonblocking check a group of peers and then schedule ourselves for a later date.
                    const int status = poll( &( adaptor->peer ), SINGLE_FILE_DESCRIPTOR, TIMEOUT );
                    if ( adaptor->is_closed ) return std::error_code( );
                    if ( status == -1 and errno not_eq EINPROGRESS ) adaptor->error( errno );
                    if ( status == 1 )
                    {
                        const short events = adaptor->peer.revents;
                        
                        if ( ( events & POLLERR or events & POLLNVAL ) and adaptor->error_handler not_eq nullptr )
                            adaptor->error_handler( std::error_code( errno, std::system_category( ) ) );
                        else
                        {
                            if ( events & POLLOUT )
                            {
                                adaptor->peer.events ^= POLLOUT;
                                if ( adaptor->open_handler not_eq nullptr ) adaptor->open_handler( );
                            }
                            
                            if ( events & ( POLLIN | POLLPRI ) and adaptor->message_handler not_eq nullptr ) adaptor->message_handler( );
                            
                            if ( events & POLLHUP and adaptor->close_handler not_eq nullptr  ) adaptor->close_handler( );
                        }
                        
                        adaptor->peer.revents = 0;
                    }
                    
                    //if POLLNVAL don't reschedule the socket is not valid!
                    //EAGAIN, its a core task!
                    adaptor->runloop->launch( bind( TCPIPAdaptorImpl::event_monitor, adaptor ), adaptor->key );
                    return std::error_code( );
                }
            };
        }
    }
}

#endif  /* _CORVUSOFT_NETWORK_DETAIL_TCPIP_ADAPTOR_IMPL_H */
