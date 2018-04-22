/*
 * Copyright 2013-2018, Corvusoft Ltd, All Rights Reserved.
 */

#pragma once

//System Includes
#include <regex>
#include <string>
#include <cstdint>
#include <iterator>

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
            //Forward Declarations
            
            struct URIBuilderImpl
            {
                std::string port = "";
                
                std::string path = "";
                
                std::string query = "";
                
                std::string scheme = "";
                
                std::string fragment = "";
                
                std::string username = "";
                
                std::string password = "";
                
                std::string authority = "";
                
                static const std::regex pattern;
                
                void parse( const std::string& value )
                {
                    std::smatch matches;
                    if ( std::regex_search( std::begin( value ), std::end( value ), matches, pattern ) )
                    {
                        scheme = matches[ 1 ];
                        username = matches[ 2 ];
                        password = matches[ 3 ];
                        authority = matches[ 4 ];
                        port = matches[ 5 ];
                        path = matches[ 6 ];
                        query = matches[ 7 ];
                        fragment = matches[ 8 ];
                    }
                }
            };
            
            const std::regex URIBuilderImpl::pattern { R"(^(?:([^\:]*)\:\/\/)(?:([^\:\@]*)(?:\:([^\@]*))?\@)?(?:([^\/\:]*))?(?:\:([0-9]*))?([^\?#]*)?(?:\?([^#]*))?(?:#(.*))?$)" };
        }
    }
}
