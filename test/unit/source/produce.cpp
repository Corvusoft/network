//System Includes
#include <string>
#include <memory>
#include <system_error>

//Project Includes
#include "corvusoft/network/tcpip_adaptor.hpp"

//External Includes
#include <catch.hpp>
#include <corvusoft/core/byte.hpp>
#include <corvusoft/core/settings.hpp>

//System Namespaces
using std::string;
using std::error_code;
using std::shared_ptr;

//Project Namespaces
using corvusoft::network::TCPIPAdaptor;

//External Namespaces
using corvusoft::core::Bytes;
using corvusoft::core::make_bytes;

TEST_CASE( "Produce" )
{
    Bytes data { };
    size_t size = 0;
    error_code status;
    auto adaptor = TCPIPAdaptor::create( );
    REQUIRE_NOTHROW( { size = adaptor->produce( data, status ); } );
    REQUIRE( size == 0 );
    REQUIRE( data.empty( ) == true );
    REQUIRE( status == std::errc::bad_file_descriptor );
    
    data = make_bytes( "test data" );
    size = adaptor->produce( data, status );
    REQUIRE( size == 0 );
    REQUIRE( data == make_bytes( "test data" ) );
    REQUIRE( status == std::errc::bad_file_descriptor );
}
