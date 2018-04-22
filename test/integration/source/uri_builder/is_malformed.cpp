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

TEST_CASE( "Assert instance is malformed." )
{
    auto builder = make_shared< URIBuilder >( "http://ftp.corvusoft.co.uk" );
    REQUIRE( builder->is_malformed( ) == false );
    
    builder = make_shared< URIBuilder >( "http://ftp.corvusoft.co.uk?query=asdf" );
    REQUIRE( builder->is_malformed( ) == false );
    
    builder = make_shared< URIBuilder >( "ftp://bcrowhurst@ftp.corvusoft.co.uk?query=asdf" );
    REQUIRE( builder->is_malformed( ) == false );
    
    builder = make_shared< URIBuilder >( "ftp:///usr/local/bin" );
    REQUIRE( builder->is_malformed( ) == false );
    
    builder = make_shared< URIBuilder >( "ftp://bcrowhurst@ftp.corvusoft.co.uk?query=asdf#asdf" );
    REQUIRE( builder->is_malformed( ) == false );
    
    builder = make_shared< URIBuilder >( "ftp://bcrowhurst@ftp.corvusoft.co.uk#asdf" );
    REQUIRE( builder->is_malformed( ) == false );
    
    builder = make_shared< URIBuilder >( "ftp://bcrowhurst@ft#asdf" );
    REQUIRE( builder->is_malformed( ) == false );
    
    builder = make_shared< URIBuilder >( "ftp://bcrowhurst:123455@ft.com/news/article1#asdf" );
    REQUIRE( builder->is_malformed( ) == false );
    
    builder = make_shared< URIBuilder >( "ftpcrowhurst:123455@ft.com/news/article1#asdf" );
    REQUIRE( builder->is_malformed( ) == true );
    
    builder = make_shared< URIBuilder >( "aldksfjao;iuje490437850t92uwgt)(*&^%$£@" );
    REQUIRE( builder->is_malformed( ) == true );
}
