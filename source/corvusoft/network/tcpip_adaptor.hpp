/*
 * Copyright 2013-2017, Corvusoft Ltd, All Rights Reserved.
 */

#ifndef _CORVUSOFT_NETWORK_TCPIP_ADAPTOR_H
#define _CORVUSOFT_NETWORK_TCPIP_ADAPTOR_H 1

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
            struct TCPIPAdaptorImpl;
        }
        
        class TCPIPAdaptor final : public Adaptor
        {
            public:
                //Friends
                
                //Definitions
                
                //Constructors
                virtual ~TCPIPAdaptor( void );
                
                //Functionality
                static std::shared_ptr< Adaptor > create( const std::string& key = "TCPIP" );
                
                virtual std::error_code teardown( void ) override;
                
                virtual std::error_code setup( const std::shared_ptr< const core::Settings >& settings = nullptr ) override;
                
                virtual std::error_code setup( const std::shared_ptr< core::RunLoop >& runloop,
                                               const std::shared_ptr< const core::Settings >& settings = nullptr ) override;
                                               
                virtual std::error_code close( void ) override;
                
                virtual std::error_code open( const std::shared_ptr< const core::Settings >& settings ) override;
                
                virtual std::error_code listen( const std::shared_ptr< const core::Settings >& settings ) override;
                
                virtual const core::Bytes consume( std::error_code& error ) override;
                
                virtual std::size_t purge( const std::size_t length, std::error_code& error ) override;
                
                virtual std::size_t produce( const core::Bytes& data, std::error_code& error ) override;
                
                virtual std::size_t flush( const std::size_t length, std::error_code& error ) override;
                
                //Getters
                virtual std::string get_key( void ) const override;
                
                virtual std::string get_local_endpoint( void ) override;
                
                virtual std::string get_remote_endpoint( void ) override;
                
                virtual std::shared_ptr< core::RunLoop > get_runloop( void ) override;
                
                //Setters
                virtual void set_open_handler( const std::function< void ( const std::shared_ptr< Adaptor > ) >& value ) override;
                
                virtual void set_close_handler( const std::function< void ( const std::shared_ptr< Adaptor > ) >& value ) override;
                
                virtual void set_error_handler( const std::function< void ( const std::shared_ptr< Adaptor >, const std::error_code ) >& value ) override;
                
                virtual void set_message_handler( const std::function< void ( const std::shared_ptr< Adaptor > ) >& value ) override;
                
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
                TCPIPAdaptor( const std::string& key );
                
                TCPIPAdaptor( const TCPIPAdaptor& original ) = delete;
                
                //Functionality
                
                //Getters
                
                //Setters
                
                //Operators
                TCPIPAdaptor& operator =( const TCPIPAdaptor& value ) = delete;
                
                //Properties
                std::shared_ptr< detail::TCPIPAdaptorImpl > m_pimpl;
        };
    }
}

#endif  /* _CORVUSOFT_NETWORK_TCPIP_ADAPTOR_H */
