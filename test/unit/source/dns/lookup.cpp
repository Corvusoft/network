//System Includes

//Project Includes
#include "corvusoft/network/dns.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces

//Project Namespaces
using corvusoft::network::DNS;

//External Namespaces

TEST_CASE( "DNS lookup empty string." )
{
    REQUIRE( DNS::lookup( "" ) == "" );
}
