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

TEST_CASE( "Assert instance is finalised." )
{
    auto builder = make_shared< URIBuilder >( );
    builder->set_scheme( "http" );
    builder->set_authority( "mail.corvusoft.co.nz" );
    REQUIRE( builder->is_finalised( ) == true );
}
