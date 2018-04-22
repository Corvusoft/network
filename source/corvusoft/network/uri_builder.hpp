/*
 * Copyright 2013-2018, Corvusoft Ltd, All Rights Reserved.
 */

#pragma once

//System Includes
#include <map>
#include <string>
#include <memory>
#include <cstdint>

//Project Includes

//External Includes

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
            struct URIBuilderImpl;
        }
        
        class URIBuilder
        {
            public:
                //Friends
                
                //Definitions
                
                //Constructors
                URIBuilder( void );
                
                URIBuilder( const std::string& value );
                
                virtual ~URIBuilder( void );
                
                //Functionality
                bool is_malformed( void );
                
                bool is_finalised( void );
                
                std::string build( void ) const;
                
                //Getters
                std::uint16_t get_port( void ) const;
                
                std::string get_path( void ) const;
                
                std::string get_query( void ) const;
                
                std::string get_scheme( void ) const;
                
                std::string get_fragment( void ) const;
                
                std::string get_username( void ) const;
                
                std::string get_password( void ) const;
                
                std::string get_authority( void ) const;
                
                std::multimap< std::string, std::string > get_parameters( void ) const;
                
                //Setters
                void set_port( const std::uint16_t value );
                
                void set_path( const std::string& value );
                
                void set_query( const std::string& value );
                
                void set_scheme( const std::string& value );
                
                void set_fragment( const std::string& value );
                
                void set_username( const std::string& value );
                
                void set_password( const std::string& value );
                
                void set_authority( const std::string& value );
                
                void set_parameters( const std::multimap< std::string, std::string >& values );
                
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
                URIBuilder( const URIBuilder& original ) = delete;
                
                //Functionality
                
                //Getters
                
                //Setters
                
                //Operators
                URIBuilder& operator =( const URIBuilder& value ) = delete;
                
                //Properties
                std::shared_ptr< detail::URIBuilderImpl > m_pimpl;
        };
    }
}
