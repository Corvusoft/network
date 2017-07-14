//System Includes
#include <string>
#include <memory>
#include <system_error>

//Project Includes
#include "corvusoft/network/tcpip_adaptor.hpp"

//External Includes
#include <catch.hpp>
#include <corvusoft/core/byte.hpp>
#include <corvusoft/core/run_loop.hpp>

//System Namespaces
using std::string;
using std::error_code;
using std::shared_ptr;

//Project Namespaces
using corvusoft::network::TCPIPAdaptor;

//External Namespaces
using corvusoft::core::Bytes;

bool is_default_address( const string& address )
{
    return address == "0.0.0.0:0" or address == "[::1]:0" or address == "127.0.0.1:0";
}

TEST_CASE( "Create" )
{
    REQUIRE_NOTHROW( TCPIPAdaptor::create( ) );
    
    auto adaptor = TCPIPAdaptor::create( );
    REQUIRE( adaptor->get_key( ) == "TCPIP" );
    REQUIRE( is_default_address( adaptor->get_local_endpoint( ) ) );
    REQUIRE( is_default_address( adaptor->get_remote_endpoint( ) ) );
    
    adaptor = TCPIPAdaptor::create( "Test Socket" );
    REQUIRE( adaptor->get_key( ) == "Test Socket" );
    REQUIRE( is_default_address( adaptor->get_local_endpoint( ) ) );
    REQUIRE( is_default_address( adaptor->get_remote_endpoint( ) ) );
}
