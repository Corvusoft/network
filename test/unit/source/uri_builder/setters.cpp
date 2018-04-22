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

TEST_CASE( "Assert instance setters." )
{
    auto builder = make_shared< URIBuilder >( );
    REQUIRE_NOTHROW( builder->set_port( 0 ) );
    REQUIRE_NOTHROW( builder->set_port( 80 ) );
    REQUIRE_NOTHROW( builder->set_port( 65365 ) );
    
    REQUIRE_NOTHROW( builder->set_path( "" ) );
    REQUIRE_NOTHROW( builder->set_path( "/a/b/c" ) );
    REQUIRE_NOTHROW( builder->set_path( ")(*&^%$£@" ) );
    
    REQUIRE_NOTHROW( builder->set_query( "" ) );
    REQUIRE_NOTHROW( builder->set_query( "a=b&c=d" ) );
    REQUIRE_NOTHROW( builder->set_query( "?a=b&c=d" ) );
    REQUIRE_NOTHROW( builder->set_query( ")(*&^%$£@" ) );
    
    REQUIRE_NOTHROW( builder->set_scheme( "" ) );
    REQUIRE_NOTHROW( builder->set_scheme( "tcp" ) );
    REQUIRE_NOTHROW( builder->set_scheme( "http://" ) );
    REQUIRE_NOTHROW( builder->set_scheme( ")(*&^%$" ) );
    
    REQUIRE_NOTHROW( builder->set_fragment( "" ) );
    REQUIRE_NOTHROW( builder->set_fragment( "subsection-1" ) );
    REQUIRE_NOTHROW( builder->set_fragment( "#subsection-1" ) );
    REQUIRE_NOTHROW( builder->set_fragment( ")(*&^%$" ) );
    
    REQUIRE_NOTHROW( builder->set_username( "" ) );
    REQUIRE_NOTHROW( builder->set_username( "bcrowhurst" ) );
    REQUIRE_NOTHROW( builder->set_username( ")(*&^%$" ) );
    
    REQUIRE_NOTHROW( builder->set_username( "" ) );
    REQUIRE_NOTHROW( builder->set_username( "secret-one" ) );
    REQUIRE_NOTHROW( builder->set_username( ")(*&^%$" ) );
    
    REQUIRE_NOTHROW( builder->set_authority( "" ) );
    REQUIRE_NOTHROW( builder->set_authority( "www.corvusoft.com.au" ) );
    REQUIRE_NOTHROW( builder->set_authority( "corvusoft.com.au" ) );
    REQUIRE_NOTHROW( builder->set_authority( ")(*&^%$" ) );
    
    multimap< string, string > values;
    REQUIRE_NOTHROW( builder->set_parameters( values ) );
    values = { { "q", "123" } };
    REQUIRE_NOTHROW( builder->set_parameters( values ) );
}
