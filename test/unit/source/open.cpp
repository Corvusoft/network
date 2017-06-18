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
using std::make_shared;

//Project Namespaces
using corvusoft::network::Adaptor;
using corvusoft::network::TCPIPAdaptor;

//External Namespaces
using corvusoft::core::Settings;

TEST_CASE( "Open" )
{
    auto adaptor = TCPIPAdaptor::create( );
    auto status = adaptor->open( nullptr );
    REQUIRE( status == std::errc::invalid_argument );
    
    auto settings = make_shared< Settings >( );
    status = adaptor->open( settings );
    REQUIRE( status == std::errc::invalid_argument );
}
