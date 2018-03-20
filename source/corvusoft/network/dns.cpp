/*
 * Copyright 2013-2018, Corvusoft Ltd, All Rights Reserved.
 */

//System Includes
#include <netdb.h>
#include <arpa/inet.h>

//Project Includes
#include "corvusoft/network/dns.hpp"

//External Includes

//System Namespaces
using std::string;

//Project Namespaces

//External Namespaces

namespace corvusoft
{
    namespace network
    {
        string DNS::lookup( const string& domain_name )
        {
            const hostent* entry = gethostbyname( domain_name.data( ) );
            if ( entry == nullptr ) return "";
            
            const in_addr* address = ( in_addr* )entry->h_addr;
            return inet_ntoa( *address );
        }
    }
}
