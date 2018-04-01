//S//System Includes
#include <memory>

//Project Includes
#include "corvusoft/network/tcpip.hpp"

//External Includes
#include <catch.hpp>
#include <corvusoft/core/byte.hpp>
#include <corvusoft/stub/run_loop.hpp>

//System Namespaces
using std::make_shared;

//Project Namespaces
using corvusoft::network::TCPIP;

//External Namespaces
using corvusoft::stub::RunLoop;

TEST_CASE( "Produce on an inactive adaptor." )
{
    auto runloop = make_shared< RunLoop >( );
    auto adaptor = make_shared< TCPIP >( runloop );
    REQUIRE_NOTHROW( adaptor->produce( { }, nullptr ) );
}
