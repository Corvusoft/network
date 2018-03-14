/*
 * Copyright 2013-2017, Corvusoft Ltd, All Rights Reserved.
 */

#ifndef _CORVUSOFT_NETWORK_TCPIP_H
#define _CORVUSOFT_NETWORK_TCPIP_H 1

//System Includes
#include <string>
#include <memory>
#include <functional>
#include <system_error>

//Project Includes
#include <corvusoft/network/adaptor.hpp>

//External Includes
#include <corvusoft/core/byte.hpp>

//System Namespaces

//Project Namespaces

//External Namespaces

namespace corvusoft
{
    //Forward Declarations
    namespace core
    {
        class RunLoop;
        class Settings;
    }
    
    namespace network
    {
        //Forward Declarations
        namespace detail
        {
            struct TCPIPImpl;
        }
        
        class TCPIP final : public Adaptor
        {
            public:
                //Friends
                
                //Definitions
                
                //Constructors
                TCPIP( const std::shared_ptr< core::RunLoop >& runloop );
                
                virtual ~TCPIP( void );
                
                //Functionality
                std::error_code setup( const std::shared_ptr< const core::Settings >& settings = nullptr ) override;
                
                std::error_code teardown( void ) override;
                
                void open( const std::shared_ptr< const core::Settings >& settings, const std::function< std::error_code ( const std::shared_ptr< Adaptor >, const std::error_code ) > completion_handler ) override;
                
                void close( const std::function< std::error_code ( const std::shared_ptr< Adaptor >, const std::error_code ) > completion_handler ) override;
                
                void consume( const std::function< std::error_code ( const std::shared_ptr< Adaptor >, const core::Bytes, const std::error_code ) > completion_handler ) override;
                
                void produce( const core::Bytes& data, const std::function< std::error_code ( const std::shared_ptr< Adaptor >, const std::size_t, const std::error_code ) > completion_handler ) override;
                
                void listen( const std::shared_ptr< const core::Settings >& settings, const std::function< std::error_code ( const std::shared_ptr< Adaptor >, const std::error_code ) > completion_handler ) override;
                
                //Getters
                virtual std::string get_local_endpoint( void ) override;
                
                virtual std::string get_remote_endpoint( void ) override;
                
                //Setters
                
                //Operators
                
                //Properties
                
            protected:
                //Friends
                
                //Definitions
                
                //Constructors
                
                //Functionality
                
                //Getters
                
                //Setters
                
                //Operators
                
                //Properties
                
            private:
                //Friends
                
                //Definitions
                
                //Constructors
                TCPIP( void ) = delete;
                
                TCPIP( const TCPIP& original ) = delete;
                
                //Functionality
                
                //Getters
                
                //Setters
                
                //Operators
                TCPIP& operator =( const TCPIP& value ) = delete;
                
                //Properties
                std::shared_ptr< detail::TCPIPImpl > m_pimpl;
        };
    }
}

#endif  /* _CORVUSOFT_NETWORK_TCPIP_H */
