//System Includes
#include <string>
#include <memory>

//Project Includes
#include "corvusoft/network/uri_builder.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::string;
using std::uint16_t;
using std::make_shared;

//Project Namespaces
using corvusoft::network::URIBuilder;

//External Namespaces

TEST_CASE( "Assert instance builder." )
{
    auto builder = make_shared< URIBuilder >( "http://bcrowhurst:secret123@www.corvusoft.co.uk:123/resource/1?name=value&q=123#sec" );
    REQUIRE( builder->build( ) == "http://bcrowhurst:secret123@www.corvusoft.co.uk:123/resource/1?name=value&q=123#sec" );
    
    builder = make_shared< URIBuilder >( );
    builder->set_port( 80 );
    builder->set_path( "/api/upgrade" );
    builder->set_query( "a=b&c=d" );
    builder->set_scheme( "ftp" );
    builder->set_fragment( "subsection-1" );
    builder->set_username( "bcrowhurst" );
    builder->set_password( "secret-one" );
    builder->set_authority( "tier1.google.com.au" );
    REQUIRE( builder->build( ) == "ftp://bcrowhurst:secret-one@tier1.google.com.au:80/api/upgrade?a=b&c=d#subsection-1" );
    
    builder = make_shared< URIBuilder >( );
    builder->set_port( 123 );
    builder->set_path( "/api/upgrade" );
    builder->set_query( "?a=b&c=d" );
    builder->set_scheme( "ftp://" );
    builder->set_fragment( "#subsection-1" );
    builder->set_username( "bcrowhurst" );
    builder->set_password( "secret-one" );
    builder->set_authority( "tier1.google.com.au" );
    REQUIRE( builder->build( ) == "ftp://bcrowhurst:secret-one@tier1.google.com.au:123/api/upgrade?a=b&c=d#subsection-1" );
    
    builder = make_shared< URIBuilder >( );
    builder->set_path( "/api/upgrade" );
    builder->set_query( "?a=b&c=d" );
    builder->set_scheme( "ftp://" );
    builder->set_fragment( "#subsection-1" );
    builder->set_authority( "tier1.google.com.au" );
    REQUIRE( builder->build( ) == "ftp://tier1.google.com.au/api/upgrade?a=b&c=d#subsection-1" );
    
    builder = make_shared< URIBuilder >( );
    builder->set_port( 9 );
    builder->set_path( "/api/upgrade" );
    builder->set_query( "?a=b&c=d" );
    builder->set_scheme( "ftp://" );
    builder->set_fragment( "#subsection-1" );
    builder->set_username( "bcrowhurst" );
    builder->set_authority( "tier1.google.com.au" );
    REQUIRE( builder->build( ) == "ftp://bcrowhurst@tier1.google.com.au:9/api/upgrade?a=b&c=d#subsection-1" );
}
