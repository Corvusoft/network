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

TEST_CASE( "Construct new instance." )
{
    REQUIRE_NOTHROW( new URIBuilder( ) );
    REQUIRE_NOTHROW( new URIBuilder( "" ) );
    REQUIRE_NOTHROW( new URIBuilder( "file:///usr/local/bin/peer" ) );
    REQUIRE_NOTHROW( new URIBuilder( "http://www.corvusoft.com.au" ) );
    REQUIRE_NOTHROW( new URIBuilder( "http://www.corvusoft.com.au?query=abcd&page=1,2#debug" ) );
    REQUIRE_NOTHROW( new URIBuilder( "kjfh894q [JFANV 2U1 12G ILBNIUADSHVA0DF8G[93U KN DKJKJ!@$%$@ " ) );
}
