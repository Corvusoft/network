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

TEST_CASE( "Assert instance getters." )
{
    auto builder = make_shared< URIBuilder >( );
    REQUIRE( builder->get_port( ) == 0 );
    REQUIRE( builder->get_path( ) == "" );
    REQUIRE( builder->get_query( ) == "" );
    REQUIRE( builder->get_scheme( ) == "" );
    REQUIRE( builder->get_fragment( ) == "" );
    REQUIRE( builder->get_username( ) == "" );
    REQUIRE( builder->get_password( ) == "" );
    REQUIRE( builder->get_authority( ) == "" );
    REQUIRE( builder->get_parameters( ).empty( ) );
}
