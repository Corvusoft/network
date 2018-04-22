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

TEST_CASE( "Assert instance builder." )
{
    auto builder = make_shared< URIBuilder >( );
    REQUIRE( builder->build( ) == "" );
}
