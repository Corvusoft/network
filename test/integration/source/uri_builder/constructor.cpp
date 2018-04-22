//System Includes
#include <map>
#include <memory>
#include <string>
#include <cstdint>

//Project Includes
#include "corvusoft/network/uri_builder.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::string;
using std::uint16_t;
using std::multimap;
using std::make_shared;

//Project Namespaces
using corvusoft::network::URIBuilder;

//External Namespaces

TEST_CASE( "Construct new instance." )
{
    auto builder = make_shared< URIBuilder >( "http://bcrowhurst:secret123@www.corvusoft.co.uk:123/resource/1?name=value&q=123#sec" );
    REQUIRE( builder->get_scheme( ) == "http" );
    REQUIRE( builder->get_username( ) == "bcrowhurst" );
    REQUIRE( builder->get_password( ) == "secret123" );
    REQUIRE( builder->get_authority( ) == "www.corvusoft.co.uk" );
    REQUIRE( builder->get_port( ) == 123 );
    REQUIRE( builder->get_path( ) == "/resource/1" );
    REQUIRE( builder->get_query( ) == "name=value&q=123" );
    REQUIRE( builder->get_fragment( ) == "sec" );
    
    multimap< string, string > values = { { "name", "value" }, { "q", "123" } };
    REQUIRE( builder->get_parameters( ) == values );
}
