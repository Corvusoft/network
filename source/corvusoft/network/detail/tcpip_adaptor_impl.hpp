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
                
                std::error_code error( const int errnum, bool report = true ) //error_if
                {
                    std::error_code error( errnum, std::system_category( ) );
                    if ( report and error_handler not_eq nullptr ) error_handler( error );
                    return error;
                }
                
                static std::error_code event_monitor( const std::shared_ptr< TCPIPAdaptorImpl > adaptor )
                {
                    if ( adaptor == nullptr ) return make_error_code( std::errc::invalid_argument );
                    if ( adaptor->peer.events == 0 ) return std::error_code( );
                    if ( adaptor->is_closed ) return std::error_code( );
                    
                    static const short open_event_mask  = POLLOUT;
                    static const short read_event_mask  = POLLIN  | POLLPRI;
                    static const short close_event_mask = POLLHUP;
                    static const int SINGLE_FILE_DESCRIPTOR = 1;
                    //static const int WAIT_UNTIL_EVENT_AVAILABLE = -1;
                    static const int TIMEOUT = 1000; //have to keep this until we add a pipe for signaling.
                    const int status = poll( &( adaptor->peer ), SINGLE_FILE_DESCRIPTOR, TIMEOUT );
                    if ( adaptor->is_closed ) return std::error_code( );
                    if ( status == -1 and errno not_eq EINPROGRESS ) adaptor->error( errno );
                    
                    //adaptor->error_if( status == -1 and errno not_eq EINPROGRESS );
                    fprintf( stderr, "[%s] poll events: %d status: %d errno: %d\n", adaptor->key.data( ), adaptor->peer.revents, status, errno );
                    if ( status == 1 )
                    {
                        const short events = adaptor->peer.revents;
                        
                        if ( ( events & POLLERR or events & POLLNVAL ) and adaptor->error_handler not_eq nullptr )
                            adaptor->error_handler( std::error_code( errno, std::system_category( ) ) );
                        else
                        {
                            if ( events & open_event_mask ) //can we not have multiple events on a poll? else if is wrong! order as well!
                            {
                                adaptor->peer.events ^= open_event_mask; //need to reset this on open/listne?
                                if ( adaptor->open_handler not_eq nullptr )
                                {
                                    adaptor->open_handler( );
                                }
                            }
                            
                            if ( events & read_event_mask and adaptor->message_handler not_eq nullptr ) adaptor->message_handler( );
                            
                            if ( events & close_event_mask and adaptor->close_handler not_eq nullptr  ) adaptor->close_handler( );
                        }
                        
                        adaptor->peer.revents = 0; //no! only clear the bit we processed!
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
