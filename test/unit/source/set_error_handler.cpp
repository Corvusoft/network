//System Includes
#include <memory>
#include <system_error>

//Project Includes
#include "corvusoft/network/tcpip_adaptor.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::shared_ptr;
using std::error_code;

//Project Namespaces
using corvusoft::network::Adaptor;
using corvusoft::network::TCPIPAdaptor;

//External Namespaces

TEST_CASE( "set error handler" )
{
    auto adaptor = TCPIPAdaptor::create( );
    REQUIRE_NOTHROW( adaptor->set_error_handler( nullptr ) );
    REQUIRE_NOTHROW( adaptor->set_error_handler( [ ] ( const shared_ptr< Adaptor >, const error_code )
    {
        FAIL( "Adaptor should not invoke error handler when calling setter." );
        return error_code( );
    } ) );
}
