//System Includes
#include <memory>

//Project Includes
#include "corvusoft/network/tcpip.hpp"

//External Includes
#include <catch.hpp>
#include <corvusoft/stub/run_loop.hpp>

//System Namespaces
using std::make_shared;

//Project Namespaces
using corvusoft::network::TCPIP;

//External Namespaces
using corvusoft::stub::RunLoop;

TEST_CASE( "Construct new instance." )
{
    auto runloop = make_shared< RunLoop >( );
    REQUIRE_NOTHROW( new TCPIP( runloop ) );
}
