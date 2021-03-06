//System Includes
#include <memory>
#include <system_error>

//Project Includes
#include "corvusoft/network/tcpip.hpp"

//External Includes
#include <catch.hpp>
#include <corvusoft/stub/run_loop.hpp>

//System Namespaces
using std::error_code;
using std::make_shared;

//Project Namespaces
using corvusoft::network::TCPIP;

//External Namespaces
using corvusoft::stub::RunLoop;

TEST_CASE( "Consume inactive adaptor." )
{
    auto runloop = make_shared< RunLoop >( );
    auto adaptor = make_shared< TCPIP >( runloop );
    REQUIRE_NOTHROW( adaptor->consume( nullptr ) );
    REQUIRE_NOTHROW( adaptor->consume( [ ]( auto, auto, auto )
    {
        return error_code( );
    } ) );
}
