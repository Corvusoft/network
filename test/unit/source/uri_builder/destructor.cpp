//System Includes
#include <memory>

//Project Includes
#include "corvusoft/network/uri_builder.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::make_shared;

//Project Namespaces
using corvusoft::network::URIBuilder;

//External Namespaces

TEST_CASE( "Destruct instance." )
{
    auto builder = new URIBuilder( );
    REQUIRE_NOTHROW( delete builder );
    
    builder = new URIBuilder( "" );
    REQUIRE_NOTHROW( delete builder );
    
    builder = new URIBuilder( "https://corvusoft.com.au" );
    REQUIRE_NOTHROW( delete builder );
}
