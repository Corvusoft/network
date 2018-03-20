//System Includes

//Project Includes
#include "corvusoft/network/tcpip.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces

//Project Namespaces
using corvusoft::network::TCPIP;

//External Namespaces

TEST_CASE( "Listen for incoming connections." )
{
    // auto adaptor = TCPIPAdaptor::create( );
    // auto status = adaptor->listen( );
    // REQUIRE( status == error_code( ) );
    
    // adaptor = TCPIPAdaptor::create( );
    // status = adaptor->listen( nullptr );
    // REQUIRE( status == error_code( ) );
    
    // auto settings = make_shared< Settings >( );
    // adaptor = TCPIPAdaptor::create( );
    // status = adaptor->listen( settings );
    // REQUIRE( status == error_code( ) );
    
    // adaptor = TCPIPAdaptor::create( );
    // status = adaptor->listen( );
    // REQUIRE( status == error_code( ) );
    // status = adaptor->listen( );
    // REQUIRE( status == std::errc::already_connected );
    
    // settings->set( "port", -1 );
    // adaptor = TCPIPAdaptor::create( );
    // status = adaptor->listen( settings );
    // REQUIRE( status == error_code( ) );
    
    // settings->set( "port", 1984 );
    // settings->set( "backlog", -342 );
    // adaptor = TCPIPAdaptor::create( );
    // status = adaptor->listen( settings );
    // REQUIRE( status == error_code( ) );
}
