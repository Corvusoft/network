//System Includes
#include <memory>

//Project Includes
#include "corvusoft/network/tcpip.hpp"

//External Includes
#include <catch.hpp>
#include <corvusoft/mock/run_loop.hpp>

//System Namespaces
using std::make_shared;

//Project Namespaces
using corvusoft::network::TCPIP;

//External Namespaces
using corvusoft::mock::RunLoop;

TEST_CASE( "Read local endpoint of an inactive adaptor." )
{
    auto runloop = make_shared< RunLoop >( );
    auto adaptor = make_shared< TCPIP >( runloop );
    REQUIRE( adaptor->get_local_endpoint( ) == "" );
}
