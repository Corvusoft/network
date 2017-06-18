//System Includes
#include <string>
#include <memory>
#include <system_error>

//Project Includes
#include "corvusoft/network/tcpip_adaptor.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::error_code;
using std::shared_ptr;

//Project Namespaces
using corvusoft::network::TCPIPAdaptor;

//External Namespaces

TEST_CASE( "teardown" )
{
    auto adaptor = TCPIPAdaptor::create( );
    const auto status = adaptor->teardown( );
    REQUIRE( status == error_code( ) );
    
    adaptor = TCPIPAdaptor::create( );
    REQUIRE_NOTHROW( adaptor->teardown( ) );
    REQUIRE_NOTHROW( adaptor->teardown( ) );
    REQUIRE_NOTHROW( adaptor.reset( ) );
}
