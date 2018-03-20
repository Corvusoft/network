/*
 * Copyright 2013-2018, Corvusoft Ltd, All Rights Reserved.
 */

#ifndef _CORVUSOFT_NETWORK_DNS_H
#define _CORVUSOFT_NETWORK_DNS_H 1

//System Includes
#include <string>

//Project Includes
#include <corvusoft/network/adaptor.hpp>

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
        
        class DNS final
        {
            public:
                //Friends
                
                //Definitions
                
                //Constructors
                
                //Functionality
                static std::string lookup( const std::string& domain_name );
                
                //Getters
                
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
                DNS( void ) = delete;
                
                DNS( const DNS& original ) = delete;
                
                //Functionality
                
                //Getters
                
                //Setters
                
                //Operators
                DNS& operator =( const DNS& value ) = delete;
                
                //Properties
        };
    }
}

#endif  /* _CORVUSOFT_NETWORK_DNS_H */
