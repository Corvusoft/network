//System Includes
#include <string>
#include <memory>
#include <system_error>

//Project Includes
#include "corvusoft/network/tcpip_adaptor.hpp"

//External Includes
#include <catch.hpp>
#include <corvusoft/core/byte.hpp>
#include <corvusoft/core/run_loop.hpp>
#include <corvusoft/core/settings.hpp>

//System Namespaces
using std::string;
using std::error_code;
using std::shared_ptr;
using std::make_shared;

//Project Namespaces
using corvusoft::network::Adaptor;
using corvusoft::network::TCPIPAdaptor;

//External Namespaces
using corvusoft::core::Bytes;
using corvusoft::core::RunLoop;
using corvusoft::core::Settings;
using corvusoft::core::make_bytes;

TEST_CASE( "Uni-directional communiction." )
{
    auto runloop = make_shared< RunLoop >( );
    
    auto client = TCPIPAdaptor::create( "client" );
    client->setup( runloop );
    client->set_open_handler( [ ]( const shared_ptr< Adaptor > client )
    {
        error_code status;
        const auto size = client->produce( make_bytes( "Hello, Server!" ), status );
        REQUIRE( size == 14 );
        REQUIRE( status == error_code( ) );
    } );
    client->set_message_handler( [ ]( const shared_ptr< Adaptor > )
    {
        FAIL( "Recieved unexpected client message handler invocation." );
    } );
    client->set_error_handler( [ ]( const shared_ptr< Adaptor >, const error_code code )
    {
        FAIL( "Recieved unexpected client error handler invocation: " + code.message( ) );
    } );
    
    auto settings = make_shared< Settings >( );
    settings->set( "port", 1985 );
    
    auto server = TCPIPAdaptor::create( "server" );
    server->setup( runloop );
    server->set_open_handler( [ client, settings ]( const shared_ptr< Adaptor > )
    {
        settings->set( "address", "127.0.0.1" );
        
        error_code status = client->open( settings );
        REQUIRE( status == error_code( ) );
    } );
    server->set_message_handler( [ client ]( const shared_ptr< Adaptor > server )
    {
        error_code status;
        auto data = server->consume( status );
        REQUIRE( status == error_code( ) );
        REQUIRE( data == make_bytes( "Hello, Server!" ) );
        
        status = server->close( );
        REQUIRE( status == error_code( ) );
        
        status = client->close( );
        REQUIRE( status == error_code( ) );
        
        server->get_runloop( )->stop( );
    } );
    server->set_error_handler( [ ]( const shared_ptr< Adaptor >, const error_code code )
    {
        FAIL( "Recieved unexpected service error handler invocation: " + code.message( ) );
    } );
    
    const auto status = server->listen( settings );
    REQUIRE( status == error_code( ) );
    
    runloop->start( );
}
