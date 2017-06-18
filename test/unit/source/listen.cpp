//System Includes
#include <string>
#include <memory>
#include <system_error>

//Project Includes
#include "corvusoft/network/tcpip_adaptor.hpp"

//External Includes
#include <catch.hpp>
#include <corvusoft/core/settings.hpp>

//System Namespaces
using std::string;
using std::error_code;
using std::shared_ptr;
using std::make_shared;

//Project Namespaces
using corvusoft::network::TCPIPAdaptor;

//External Namespaces
using corvusoft::core::Settings;

TEST_CASE( "Listen" )
{
    auto adaptor = TCPIPAdaptor::create( );
    auto status = adaptor->listen( );
    REQUIRE( status == error_code( ) );
    
    adaptor = TCPIPAdaptor::create( );
    status = adaptor->listen( nullptr );
    REQUIRE( status == error_code( ) );
    
    auto settings = make_shared< Settings >( );
    adaptor = TCPIPAdaptor::create( );
    status = adaptor->listen( settings );
    REQUIRE( status == error_code( ) );
    
    adaptor = TCPIPAdaptor::create( );
    status = adaptor->listen( );
    REQUIRE( status == error_code( ) );
    status = adaptor->listen( );
    REQUIRE( status == std::errc::already_connected );
    
    settings->set( "port", -1 );
    adaptor = TCPIPAdaptor::create( );
    status = adaptor->listen( settings );
    REQUIRE( status == error_code( ) );
    
    settings->set( "port", 1984 );
    settings->set( "backlog", -342 );
    adaptor = TCPIPAdaptor::create( );
    status = adaptor->listen( settings );
    REQUIRE( status == error_code( ) );
}
