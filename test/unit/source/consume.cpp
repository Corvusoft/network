//System Includes
#include <memory>
#include <system_error>

//Project Includes
#include "corvusoft/network/tcpip_adaptor.hpp"

//External Includes
#include <catch.hpp>
#include <corvusoft/core/byte.hpp>

//System Namespaces
using std::error_code;
using std::shared_ptr;

//Project Namespaces
using corvusoft::network::TCPIPAdaptor;

//External Namespaces
using corvusoft::core::Bytes;

TEST_CASE( "Consume" )
{
    Bytes data { };
    error_code status;
    auto adaptor = TCPIPAdaptor::create( );
    data = adaptor->consume( status );
    REQUIRE( data.empty( ) == true );
    REQUIRE( status == std::errc::bad_file_descriptor );
    
    data = adaptor->consume( status );
    REQUIRE( data.empty( ) == true );
    REQUIRE( status == std::errc::bad_file_descriptor );
}
