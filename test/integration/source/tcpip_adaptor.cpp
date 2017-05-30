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

TEST_CASE( "Service handler call backs." )
{
    auto settings = make_shared< Settings >( );
    settings->set( "port", 1984 );
    
    bool handler_invoked = false;
    
    auto server = TCPIPAdaptor::create( );
    server->set_open_handler( [ &handler_invoked ]( const shared_ptr< Adaptor > adaptor )
    {
        const auto status = adaptor->close( );
        REQUIRE( status == error_code( ) );
        
        handler_invoked = true;
    } );
    
    server->set_close_handler( [ &handler_invoked ]( const shared_ptr< Adaptor > adaptor )
    {
        adaptor->get_runloop( )->stop( ); //flush
    } );
    
    server->set_message_handler( [ ]( const shared_ptr< Adaptor > )
    {
        FAIL( "Recieved unexpected message handler invocation." );
    } );
    
    server->set_error_handler( [ ]( const shared_ptr< Adaptor >, const error_code code )
    {
        FAIL( "Recieved unexpected error handler invocation: " + code.message( ) );
    } );
    
    const auto status = server->listen( settings );
    REQUIRE( status == error_code( ) );
    
    auto runloop = server->get_runloop( );
    runloop->start( ); //flush
    
    REQUIRE( handler_invoked );
}

TEST_CASE( "Calling listen on active service adaptor." )
{
    auto settings = make_shared< Settings >( );
    settings->set( "port", 1984 );
    
    bool handler_invoked = false;
    
    auto server = TCPIPAdaptor::create( );
    server->set_open_handler( [ &handler_invoked, settings ]( const shared_ptr< Adaptor > adaptor )
    {
        auto status = adaptor->listen( );
        REQUIRE( status == std::errc::already_connected );
        
        status = adaptor->listen( settings );
        REQUIRE( status == std::errc::already_connected );
        
        adaptor->get_runloop( )->stop( ); //flush
        handler_invoked = true;
    } );
    
    server->set_message_handler( [ ]( const shared_ptr< Adaptor > )
    {
        FAIL( "Recieved unexpected message handler invocation." );
    } );
    
    server->set_error_handler( [ ]( const shared_ptr< Adaptor >, const error_code code )
    {
        FAIL( "Recieved unexpected error handler invocation: " + code.message( ) );
    } );
    
    const auto status = server->listen( settings );
    REQUIRE( status == error_code( ) );
    
    auto runloop = server->get_runloop( );
    runloop->start( ); //flush
    
    REQUIRE( handler_invoked );
}

TEST_CASE( "Calling client open on active service adaptor." )
{
    bool handler_invoked = false;
    auto settings = make_shared< Settings >( );
    
    auto server = TCPIPAdaptor::create( );
    server->set_open_handler( [ &handler_invoked, settings ]( const shared_ptr< Adaptor > adaptor )
    {
        auto status = adaptor->open( settings );
        REQUIRE( status == std::errc::already_connected );
        
        adaptor->get_runloop( )->stop( ); //flush
        handler_invoked = true;
    } );
    
    server->set_message_handler( [ ]( const shared_ptr< Adaptor > )
    {
        FAIL( "Recieved unexpected message handler invocation." );
    } );
    
    server->set_error_handler( [ ]( const shared_ptr< Adaptor >, const error_code code )
    {
        FAIL( "Recieved unexpected error handler invocation: " + code.message( ) );
    } );
    
    const auto status = server->listen( settings );
    REQUIRE( status == error_code( ) );
    
    auto runloop = server->get_runloop( );
    runloop->start( ); //flush
    
    REQUIRE( handler_invoked );
}

//call setup after listen/open and attempt to null settings or runloop then call lsiten/open again.
//test with and with out settings
//invalid ports/addresses etc...
//create client/server with no handlers specified... waht happens?
// TEST_CASE( "end-to-end communication" )
// {
//     auto settings = make_shared< Settings >( );
//     settings->set( "port", 1984 );

//     auto server = TCPIPAdaptor::create( );
//     server->set_open_handler( [ ]( const shared_ptr< TCPIPAdaptor > )
//     {
//         //called when socket is up and running.
//         settings->set( "address", "127.0.0.1" );
//         //auto client = TCPIPAdaptor::create( );
//         //client->set_open_handler( );
//         //client->set_message_handler( );
//         //client->open( settings );
//     } );
//     server->set_message_handler( [ ]( const shared_ptr< TCPIPAdaptor > )
//     {
//         //called when new socket connection is made.
//     } );
//     server->listen( settings );
// }

// restless::Client client( runloop )

// client.get( path, headers, body );
// client.get( request );

// client.read( path, headers, body );
// client.read( request );

// client.set_status_handler( [ ]( request, response, error_code ){ } );
// client.set_status_handler( 200, [ ]( request, response, error_code ){ } );
// client.set_status_handler( range< 400, 499 >, [ ]( request, response, error_code ){ } );

// client.observe( request, event, reaction );
// client.observe( request, 500ms, reaction );

// client.fetch( delimiter | size );
// client.close( );
