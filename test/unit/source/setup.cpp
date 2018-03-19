//System Includes
#include <memory>
#include <system_error>

//Project Includes
#include "corvusoft/network/tcpip.hpp"

//External Includes
#include <catch.hpp>
#include <corvusoft/mock/run_loop.hpp>
#include <corvusoft/core/settings.hpp>

//System Namespaces
using std::error_code;
using std::make_shared;

//Project Namespaces
using corvusoft::network::TCPIP;

//External Namespaces
using corvusoft::mock::RunLoop;
using corvusoft::core::Settings;

TEST_CASE( "Setup adaptor." )
{
    auto runloop = make_shared< RunLoop >( );
    auto adaptor = make_shared< TCPIP >( runloop );
    auto status = adaptor->setup( nullptr );
    REQUIRE( status == error_code( ) );
    
    auto settings = make_shared< Settings >( );
    status = adaptor->setup( settings );
    REQUIRE( status == error_code( ) );
}
