//System Includes
#include <string>
#include <memory>
#include <cstddef>
#include <system_error>

//Project Includes
#include "corvusoft/network/tcpip.hpp"

//External Includes
#include <catch.hpp>
#include <corvusoft/core/byte.hpp>
#include <corvusoft/core/run_loop.hpp>
#include <corvusoft/core/settings.hpp>

//System Namespaces
using std::size_t;
using std::string;
using std::error_code;
using std::shared_ptr;
using std::make_shared;

//Project Namespaces
using corvusoft::network::TCPIP;
using corvusoft::network::Adaptor;

//External Namespaces
using corvusoft::core::Bytes;
using corvusoft::core::RunLoop;
using corvusoft::core::Settings;
using corvusoft::core::make_bytes;

#include <thread> //remove me.

TEST_CASE( "Client implementation." )
{
    auto runloop = make_shared< RunLoop >( );
    auto adaptor = make_shared< TCPIP >( runloop );
    
    auto status = adaptor->setup( );
    REQUIRE( status == error_code( ) );
    
    auto settings = make_shared< Settings >( );
    settings->set( "port", 80 );
    settings->set( "address", "216.58.203.99" );
    
    bool open_called = false;
    bool produce_called = false;
    bool consume_called = false;
    
    adaptor->open( settings, [ &open_called, &produce_called, &consume_called ] ( const shared_ptr< Adaptor > adaptor, const error_code status )
    {
        REQUIRE( adaptor not_eq nullptr );
        REQUIRE( status == error_code( ) );
        
        open_called = true;
        const auto buffer = make_bytes( "GET / HTTP/1.1\r\nHost: www.google.com.au\r\nConnection: close\r\n\r\n" );
        adaptor->produce( buffer, [ buffer, &produce_called, &consume_called ]( const shared_ptr< Adaptor > adaptor, const size_t length, const error_code status )
        {
            REQUIRE( adaptor not_eq nullptr );
            REQUIRE( status == error_code( ) );
            REQUIRE( length == buffer.size( ) );
            
            produce_called = true;
            adaptor->consume( [ &consume_called ]( const shared_ptr< Adaptor > adaptor, const Bytes data, const error_code status )
            {
                REQUIRE( not data.empty( ) );
                REQUIRE( adaptor not_eq nullptr );
                REQUIRE( status == error_code( ) );
                
                consume_called = true;
                fprintf( stderr, "====== Data ======\n%.*s", data.size( ), data.data( ) );
                return error_code( );
            } );
            
            return error_code( );
        } );
        
        return error_code( );
    } );
    
    //adaptor->close( ); //add completion handler.
    
    status = adaptor->teardown( );
    REQUIRE( status == error_code( ) );
    
    status = runloop->start( );
    REQUIRE( status == error_code( ) );
    
    status = runloop->wait( );
    REQUIRE( status == error_code( ) );
    
    std::this_thread::sleep_for( std::chrono::seconds( 5 ) ); //force wait to hang until all tasks completed.
    
    status = runloop->stop( );
    REQUIRE( status == error_code( ) );
    
    REQUIRE( open_called == true );
    REQUIRE( produce_called == true );
    REQUIRE( consume_called == true );
}
