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

TEST_CASE( "Flush" )
{
    size_t size = 0;
    error_code status;
    auto adaptor = TCPIPAdaptor::create( );
    size = adaptor->flush( 0, status );
    REQUIRE( size == 0 );
    REQUIRE( status == std::errc::bad_file_descriptor );
    
    size = adaptor->flush( 99999, status );
    REQUIRE( size == 0 );
    REQUIRE( status == std::errc::bad_file_descriptor );
}
