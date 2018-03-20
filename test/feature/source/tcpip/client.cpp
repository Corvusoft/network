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

bool open_called = false;
bool close_called = false;
bool produce_called = false;
bool consume_called = false;

TEST_CASE( "Client implementation." )
{
    auto runloop = make_shared< RunLoop >( );
    auto adaptor = make_shared< TCPIP >( runloop );
    auto settings = make_shared< Settings >( );
    settings->set( "port", 80 );
    settings->set( "address", "216.58.203.99" );
    
    auto status = adaptor->setup( );
    REQUIRE( status == error_code( ) );
    
    adaptor->open( settings, [ ]( const shared_ptr< Adaptor > adaptor, const error_code status )
    {
        open_called = true;
        REQUIRE( adaptor not_eq nullptr );
        REQUIRE( status == error_code( ) );
        
        const auto buffer = make_bytes( "GET / HTTP/1.1\r\nHost: www.google.com.au\r\nConnection: close\r\n\r\n" );
        adaptor->produce( buffer, [ buffer ]( const shared_ptr< Adaptor > adaptor, const size_t length, const error_code status )
        {
            produce_called = true;
            REQUIRE( adaptor not_eq nullptr );
            REQUIRE( status == error_code( ) );
            REQUIRE( length == buffer.size( ) );
            
            adaptor->consume( [ ]( const shared_ptr< Adaptor > adaptor, const Bytes data, const error_code status )
            {
                consume_called = true;
                REQUIRE( not data.empty( ) );
                REQUIRE( adaptor not_eq nullptr );
                REQUIRE( status == error_code( ) );
                
                adaptor->close( [ ]( const shared_ptr< Adaptor > adaptor, const error_code status )
                {
                    close_called = true;
                    REQUIRE( adaptor not_eq nullptr );
                    REQUIRE( status == error_code( ) );
                    
                    auto result = adaptor->teardown( );
                    REQUIRE( result == error_code( ) );
                    return result;
                } );
                
                return error_code( );
            } );
            
            return error_code( );
        } );
        
        return error_code( );
    } );
    
    runloop->start( );
    runloop->wait( );
    runloop->stop( );
    
    REQUIRE( open_called == true );
    REQUIRE( close_called == true );
    REQUIRE( produce_called == true );
    REQUIRE( consume_called == true );
}
