/*
 * Copyright 2013-2017, Corvusoft Ltd, All Rights Reserved.
 */

#ifndef _CORVUSOFT_NETWORK_ADAPTOR_H
#define _CORVUSOFT_NETWORK_ADAPTOR_H 1

//System Includes
#include <string>
#include <memory>
#include <functional>
#include <system_error>

//Project Includes

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
        
        class Adaptor : public std::enable_shared_from_this< Adaptor >
        {
            public:
                //Friends
                
                //Definitions
                
                //Constructors
                
                //Functionality
                virtual std::error_code teardown( void ) = 0;
                
                virtual std::error_code setup( const std::shared_ptr< const core::Settings >& settings = nullptr ) = 0;
                
                virtual std::error_code setup( const std::shared_ptr< core::RunLoop >& runloop, const std::shared_ptr< const core::Settings >& settings = nullptr ) = 0;
                
                virtual std::error_code close( void ) = 0;
                
                virtual std::error_code open( const std::shared_ptr< const core::Settings >& settings ) = 0;
                
                virtual std::error_code listen( const std::shared_ptr< const core::Settings >& settings = nullptr ) = 0;
                
                virtual const core::Bytes consume( std::error_code& error ) = 0;
                
                virtual std::size_t purge( const std::size_t length, std::error_code& error ) = 0;
                
                virtual std::size_t produce( const core::Bytes& data, std::error_code& error ) = 0;
                
                virtual std::size_t flush( const std::size_t length, std::error_code& error ) = 0;
                
                //Getters
                virtual std::string get_name( void ) const = 0;
                
                virtual std::string get_local_endpoint( void ) = 0;
                
                virtual std::string get_remote_endpoint( void ) = 0;
                
                virtual std::shared_ptr< core::RunLoop > get_runloop( void ) = 0;
                
                //Setters
                virtual void set_open_handler( const std::function< void ( const std::shared_ptr< Adaptor > ) >& ) = 0;
                
                virtual void set_close_handler( const std::function< void ( const std::shared_ptr< Adaptor > ) >& ) = 0;
                
                virtual void set_message_handler( const std::function< void ( const std::shared_ptr< Adaptor > ) >& ) = 0;
                
                virtual void set_error_handler( const std::function< void ( const std::shared_ptr< Adaptor >, const std::error_code ) >& ) = 0;
                
                //Operators
                
                //Properties
                
            protected:
                //Friends
                
                //Definitions
                
                //Constructors
                Adaptor( const std::string& )
                {
                    return;
                };
                
                virtual ~Adaptor( void ) = default;
                
                //Functionality
                
                //Getters
                
                //Setters
                
                //Operators
                
                //Properties
                
            private:
                //Friends
                
                //Definitions
                
                //Constructors
                Adaptor( const Adaptor& original ) = delete;
                
                //Functionality
                
                //Getters
                
                //Setters
                
                //Operators
                Adaptor& operator =( const Adaptor& value ) = delete;
                
                //Properties
        };
    }
}

#endif  /* _CORVUSOFT_NETWORK_ADAPTOR_H */
