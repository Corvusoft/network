//System Includes
#include <string>
#include <memory>
#include <system_error>

//Project Includes
#include "corvusoft/network/tcpip_adaptor.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::string;
using std::error_code;
using std::shared_ptr;

//Project Namespaces
using corvusoft::network::TCPIPAdaptor;

//External Namespaces

TEST_CASE( "Purge" )
{
    size_t size = 0;
    error_code status;
    auto adaptor = TCPIPAdaptor::create( );
    size = adaptor->purge( 0, status );
    REQUIRE( size == 0 );
    REQUIRE( status == error_code( ) );
    
    size = adaptor->purge( 99999, status );
    REQUIRE( size == 0 );
    REQUIRE( status == error_code( ) );
}
