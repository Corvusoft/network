//System Includes
#include <memory>
#include <system_error>

//Project Includes
#include "corvusoft/network/tcpip.hpp"

//External Includes
#include <catch.hpp>
#include <corvusoft/mock/run_loop.hpp>

//System Namespaces
using std::error_code;
using std::make_shared;

//Project Namespaces
using corvusoft::network::TCPIP;

//External Namespaces
using corvusoft::mock::RunLoop;

TEST_CASE( "Close inactive adaptor." )
{
    auto runloop = make_shared< RunLoop >( );
    auto adaptor = make_shared< TCPIP >( runloop );
    REQUIRE_NOTHROW( adaptor->close( nullptr ) );
    
    bool close_called = false;
    adaptor->close( [ &close_called ]( auto, auto )
    {
        close_called = true;
        return error_code( );
    } );
    REQUIRE( close_called == true );
}
