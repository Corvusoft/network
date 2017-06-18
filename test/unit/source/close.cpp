//System Includes
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

TEST_CASE( "Close" )
{
    auto adaptor = TCPIPAdaptor::create( );
    auto status = adaptor->close( );
    REQUIRE( status == std::errc::bad_file_descriptor );
    
    status = adaptor->close( );
    REQUIRE( status == std::errc::bad_file_descriptor  );
}
