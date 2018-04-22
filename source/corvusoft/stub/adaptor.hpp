/*
 * Copyright 2013-2018, Corvusoft Ltd, All Rights Reserved.
 */

#ifndef _CORVUSOFT_STUB_ADAPTOR_H
#define _CORVUSOFT_STUB_ADAPTOR_H 1

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
    
    namespace stub
    {
        //Forward Declarations
        
        class Adaptor final : public network::Adaptor
        {
            public:
                //Friends
                
                //Definitions
                
                //Constructors
                Adaptor( const std::shared_ptr< core::RunLoop >& ) : network::Adaptor( )
                {
                    return;
                }
                
                virtual ~Adaptor( void )
                {
                    return;
                }
                
                //Functionality
                std::error_code setup( const std::shared_ptr< const core::Settings >& settings = nullptr ) override
                {
                    return;
                }
                
                std::error_code teardown( void ) override
                {
                    return;
                }
                
                void open( const std::shared_ptr< const core::Settings >&, const std::function< std::error_code ( const std::shared_ptr< network::Adaptor >, const std::error_code ) > completion_handler ) override
                {
                    completion_handler( std::shared_from_this( ), std::error_code( ) );
                }
                
                void close( const std::function< std::error_code ( const std::shared_ptr< network::Adaptor >, const std::error_code ) > completion_handler ) override
                {
                    completion_handler( std::shared_from_this( ), std::error_code( ) );
                }
                
                void consume( const std::function< std::error_code ( const std::shared_ptr< network::Adaptor >, const core::Bytes, const std::error_code ) > completion_handler ) override
                {
                    completion_handler( std::shared_from_this( ), m_data, std::error_code( ) );
                }
                
                void produce( const core::Bytes& data, const std::function< std::error_code ( const std::shared_ptr< network::Adaptor >, const std::size_t, const std::error_code ) > completion_handler ) override
                {
                    completion_handler( std::shared_from_this( ), data.size( ), std::error_code( ) );
                }
                
                void listen( const std::shared_ptr< const core::Settings >&, const std::function< std::error_code ( const std::shared_ptr< network::Adaptor >, const std::error_code ) > connection_handler ) override
                {
                    connection_handler( std::shared_from_this( ), std::error_code( ) );
                }
                
                //Getters
                
                //Setters
                void set_data( const core::Bytes& value )
                {
                    m_data = value;
                }
                
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
                Adaptor( void ) = delete;
                
                Adaptor( const Adaptor& original ) = delete;
                
                //Functionality
                
                //Getters
                
                //Setters
                
                //Operators
                Adaptor& operator =( const Adaptor& value ) = delete;
                
                //Properties
                core::Bytes m_data { };
        };
    }
}

#endif  /* _CORVUSOFT_STUB_ADAPTOR_H */
