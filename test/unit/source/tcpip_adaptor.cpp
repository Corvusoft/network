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

bool is_default_address( const string& address )
{
    return address == "0.0.0.0:0" or address == "[::1]:0" or address == "127.0.0.1:0";
}

TEST_CASE( "Create" )
{
    REQUIRE_NOTHROW( TCPIPAdaptor::create( ) );
    
    auto adaptor = TCPIPAdaptor::create( );
    REQUIRE( adaptor->get_key( ) == "TCPIP" );
    REQUIRE( is_default_address( adaptor->get_local_endpoint( ) ) );
    // REQUIRE( is_default_address( adaptor->get_remote_endpoint( ) ) );
    
    adaptor = TCPIPAdaptor::create( "Test Socket" );
    REQUIRE( adaptor->get_key( ) == "Test Socket" );
    REQUIRE( is_default_address( adaptor->get_local_endpoint( ) ) );
    // REQUIRE( is_default_address( adaptor->get_remote_endpoint( ) ) );
}

TEST_CASE( "Setup" )
{
    auto adaptor = TCPIPAdaptor::create( );
    
    auto status = adaptor->setup( );
    REQUIRE( status == error_code( ) );
    REQUIRE( adaptor->get_key( ) == "TCPIP" );
    REQUIRE( is_default_address( adaptor->get_local_endpoint( ) ) );
    //REQUIRE( is_default_address( adaptor->get_remote_endpoint( ) ) );
    
    const auto runloop = make_shared< RunLoop >( );
    status = adaptor->setup( runloop );
    REQUIRE( status == error_code( ) );
    REQUIRE( adaptor->get_key( ) == "TCPIP" );
    REQUIRE( is_default_address( adaptor->get_local_endpoint( ) ) );
    //REQUIRE( is_default_address( adaptor->get_remote_endpoint( ) ) );
    
    const auto settings = make_shared< Settings >( );
    status = adaptor->setup( settings );
    REQUIRE( status == error_code( ) );
    REQUIRE( adaptor->get_key( ) == "TCPIP" );
    REQUIRE( is_default_address( adaptor->get_local_endpoint( ) ) );
    //REQUIRE( is_default_address( adaptor->get_remote_endpoint( ) ) );
    
    status = adaptor->setup( runloop, settings );
    REQUIRE( status == error_code( ) );
    REQUIRE( adaptor->get_key( ) == "TCPIP" );
    REQUIRE( is_default_address( adaptor->get_local_endpoint( ) ) );
    //REQUIRE( is_default_address( adaptor->get_remote_endpoint( ) ) );
    
    status = adaptor->setup( nullptr, nullptr );
    REQUIRE( status == error_code( ) );
    REQUIRE( adaptor->get_key( ) == "TCPIP" );
    REQUIRE( is_default_address( adaptor->get_local_endpoint( ) ) );
    //REQUIRE( is_default_address( adaptor->get_remote_endpoint( ) ) );
}

TEST_CASE( "Clean-up" )
{
    auto adaptor = TCPIPAdaptor::create( );
    REQUIRE_NOTHROW( adaptor->teardown( ) );
    REQUIRE_NOTHROW( adaptor->teardown( ) );
    REQUIRE_NOTHROW( adaptor.reset( ) );
    
    adaptor = TCPIPAdaptor::create( );
    const auto status = adaptor->teardown( );
    REQUIRE( status == error_code( ) );
}

TEST_CASE( "Close" )
{
    auto adaptor = TCPIPAdaptor::create( );
    auto status = adaptor->close( );
    REQUIRE( status == std::errc::bad_file_descriptor );
    
    status = adaptor->close( );
    REQUIRE( status == std::errc::bad_file_descriptor  );
}

TEST_CASE( "Open" )
{
    auto adaptor = TCPIPAdaptor::create( );
    auto status = adaptor->open( nullptr );
    REQUIRE( status == std::errc::invalid_argument );
    
    //auto settings = make_shared< Settings >( );
    //status = adaptor->open( settings );
    //REQUIRE( status == std::errc::invalid_argument );
    // REQUIRE( status.message( ) == "address and/or port is malformed." );
    
    // settings->set( "port", -1 );
    // status = adaptor->open( settings );
    // REQUIRE( status == std::errc::invalid_argument );
    // REQUIRE( status.message( ) == "port is malformed, expected positive integer." );
    
    // settings->set( "address", "kjflks" );
    // status = adaptor->open( settings );
    // REQUIRE( status == std::errc::invalid_argument );
    // REQUIRE( status.message( ) == "address is malformed, expected IPv4/6 string." );
}

TEST_CASE( "Listen" )
{
    auto adaptor = TCPIPAdaptor::create( );
    auto status = adaptor->listen( );
    adaptor->close( );
    REQUIRE( status == error_code( ) );
    
    status = adaptor->listen( nullptr );
    REQUIRE( status == error_code( ) );
    adaptor->close( );
    
    auto settings = make_shared< Settings >( );
    status = adaptor->listen( settings );
    adaptor->close( );
    REQUIRE( status == error_code( ) );
    
    // settings->set( "port", -1 );
    // status = adaptor->listen( settings );
    // REQUIRE( status == std::errc::invalid_argument );
    // REQUIRE( status.message( ) == "port is malformed, expected positive integer." );
    
    // settings->set( "backlog", -1 );
    // status = adaptor->listen( settings );
    // REQUIRE( status == std::errc::invalid_argument );
    // REQUIRE( status.message( ) == "backlog is malformed, expected positive integer." );
}

TEST_CASE( "Peek" )
{
    Bytes data { };
    error_code status;
    auto adaptor = TCPIPAdaptor::create( );
    REQUIRE_NOTHROW( { data = adaptor->peek( status ); } );
    REQUIRE( data.empty( ) == true );
    REQUIRE( status == std::errc::bad_file_descriptor );
    
    REQUIRE_NOTHROW( { data = adaptor->peek( status ); } );
    REQUIRE( data.empty( ) == true );
    REQUIRE( status == std::errc::bad_file_descriptor );
}

TEST_CASE( "Consume" )
{
    Bytes data { };
    error_code status;
    auto adaptor = TCPIPAdaptor::create( );
    REQUIRE_NOTHROW( { data = adaptor->consume( status ); } );
    REQUIRE( data.empty( ) == true );
    REQUIRE( status == std::errc::bad_file_descriptor );
    
    REQUIRE_NOTHROW( { data = adaptor->consume( status ); } );
    REQUIRE( data.empty( ) == true );
    REQUIRE( status == std::errc::bad_file_descriptor );
}

TEST_CASE( "Produce" )
{
    Bytes data { };
    size_t size = 0;
    error_code status;
    auto adaptor = TCPIPAdaptor::create( );
    REQUIRE_NOTHROW( { size = adaptor->produce( data, status ); } );
    REQUIRE( size == 0 );
    REQUIRE( data.empty( ) == true );
    REQUIRE( status == std::errc::bad_file_descriptor );
    
    data = make_bytes( "test data" );
    size = adaptor->produce( data, status );
    REQUIRE( size == 0 );
    REQUIRE( data == make_bytes( "test data" ) );
    REQUIRE( status == std::errc::bad_file_descriptor );
}

TEST_CASE( "Handlers" )
{
    auto adaptor = TCPIPAdaptor::create( );
    REQUIRE_NOTHROW( adaptor->set_open_handler( nullptr ) );
    REQUIRE_NOTHROW( adaptor->set_close_handler( nullptr ) );
    REQUIRE_NOTHROW( adaptor->set_error_handler( nullptr ) );
    REQUIRE_NOTHROW( adaptor->set_message_handler( nullptr ) );
}
