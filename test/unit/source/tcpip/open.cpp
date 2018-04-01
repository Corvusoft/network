//System Includes
#include <memory>
#include <system_error>

//Project Includes
#include "corvusoft/network/tcpip.hpp"

//External Includes
#include <catch.hpp>
#include <corvusoft/stub/run_loop.hpp>
#include <corvusoft/core/settings.hpp>

//System Namespaces
using std::error_code;
using std::make_shared;

//Project Namespaces
using corvusoft::network::TCPIP;

//External Namespaces
using corvusoft::stub::RunLoop;
using corvusoft::core::Settings;

TEST_CASE( "Open adaptor with null settings and handler arguments." )
{
    auto runloop = make_shared< RunLoop >( );
    auto settings = make_shared< Settings >( );
    auto adaptor = make_shared< TCPIP >( runloop );
    REQUIRE_NOTHROW( adaptor->open( nullptr, nullptr ) );
}

TEST_CASE( "Open adaptor with null settings argument." )
{
    auto runloop = make_shared< RunLoop >( );
    auto settings = make_shared< Settings >( );
    auto adaptor = make_shared< TCPIP >( runloop );
    
    bool open_called = false;
    adaptor->open( nullptr, [ &open_called ]( auto, auto status )
    {
        open_called = true;
        REQUIRE( status ==  std::errc::invalid_argument );
        return error_code( );
    } );
    REQUIRE( open_called == true );
}

TEST_CASE( "Open adaptor with invalid port and address settings." )
{
    auto runloop = make_shared< RunLoop >( );
    auto settings = make_shared< Settings >( );
    auto adaptor = make_shared< TCPIP >( runloop );
    
    bool open_called = false;
    adaptor->open( settings, [ &open_called ]( auto, auto status )
    {
        open_called = true;
        REQUIRE( status ==  std::errc::invalid_argument );
        return error_code( );
    } );
    REQUIRE( open_called == true );
}
