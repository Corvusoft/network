/*
 * Copyright 2013-2018, Corvusoft Ltd, All Rights Reserved.
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
                virtual std::error_code setup( const std::shared_ptr< const core::Settings >& settings = nullptr ) = 0;
                
                virtual std::error_code teardown( void ) = 0;
                
                virtual void open( const std::shared_ptr< const core::Settings >& settings, const std::function< std::error_code ( const std::shared_ptr< Adaptor >, const std::error_code ) > completion_handler ) = 0;
                
                virtual void close( const std::function< std::error_code ( const std::shared_ptr< Adaptor >, const std::error_code ) > completion_handler ) = 0;
                
                virtual void consume( const std::function< std::error_code ( const std::shared_ptr< Adaptor >, const core::Bytes, const std::error_code ) > completion_handler ) = 0;
                
                virtual void produce( const core::Bytes& data, const std::function< std::error_code ( const std::shared_ptr< Adaptor >, const std::size_t, const std::error_code ) > completion_handler ) = 0;
                
                virtual void listen( const std::shared_ptr< const core::Settings >& settings, const std::function< std::error_code ( const std::shared_ptr< Adaptor >, const std::error_code ) > connection_handler ) = 0;
                
                //Getters
                
                //Setters
                
                //Operators
                
                //Properties
                
            protected:
                //Friends
                
                //Definitions
                
                //Constructors
                Adaptor( void ) = default;
                
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
