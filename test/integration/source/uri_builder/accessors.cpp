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

TEST_CASE( "Assert instance accessors." )
{
    auto builder = make_shared< URIBuilder >( );
    builder->set_port( 80 );
    REQUIRE( builder->get_port( ) == 80 );
    
    builder->set_path( "/api/upgrade" );
    REQUIRE( builder->get_path( ) == "/api/upgrade" );
    
    builder->set_query( "a=b&c=d" );
    REQUIRE( builder->get_query( ) == "a=b&c=d" );
    multimap< string, string > values = { { "a", "b" }, { "c", "d" } };
    REQUIRE( builder->get_parameters( ) == values );
    
    builder->set_query( "?a=b&c=d" );
    REQUIRE( builder->get_query( ) == "?a=b&c=d" );
    values = { { "a", "b" }, { "c", "d" } };
    REQUIRE( builder->get_parameters( ) == values );
    
    values = { { "a", "b" }, { "c", "d" } };
    builder->set_parameters( values );
    REQUIRE( builder->get_parameters( ) == values );
    REQUIRE( builder->get_query( ) == "a=b&c=d" );
    
    builder->set_scheme( "tcp" );
    REQUIRE( builder->get_scheme( ) == "tcp" );
    builder->set_scheme( "ftp://" );
    REQUIRE( builder->get_scheme( ) == "ftp://" );
    
    builder->set_fragment( "subsection-1" );
    REQUIRE( builder->get_fragment( ) == "subsection-1" );
    builder->set_fragment( "#subsection-2" );
    REQUIRE( builder->get_fragment( ) == "#subsection-2" );
    
    builder->set_username( "bcrowhurst" );
    REQUIRE( builder->get_username( ) == "bcrowhurst" );
    
    builder->set_password( "secret-one" );
    REQUIRE( builder->get_password( ) == "secret-one" );
    
    builder->set_authority( "www.corvusoft.com.au" );
    REQUIRE( builder->get_authority( ) == "www.corvusoft.com.au" );
}
