/*
 * Copyright 2013-2018, Corvusoft Ltd, All Rights Reserved.
 */

//System Includes
#include <regex>
#include <sstream>
#include <iostream>
#include <stdexcept>

//Project Includes
#include "corvusoft/network/uri_builder.hpp"
#include "corvusoft/network/detail/uri_builder_impl.hpp"

//External Includes

//System Namespaces
using std::stoi;
using std::regex;
using std::string;
using std::getline;
using std::multimap;
using std::uint16_t;
using std::to_string;
using std::regex_match;
using std::out_of_range;
using std::istringstream;
using std::invalid_argument;

//Project Namespaces
using corvusoft::network::detail::URIBuilderImpl;

//External Namespaces
namespace corvusoft
{
    namespace network
    {
        URIBuilder::URIBuilder( void ) : m_pimpl( new URIBuilderImpl )
        {
            return;
        }
        
        URIBuilder::URIBuilder( const string& value ) : m_pimpl( new URIBuilderImpl )
        {
            m_pimpl->parse( value );
        }
        
        URIBuilder::~URIBuilder( void )
        {
            return;
        }
        
        string URIBuilder::build( void ) const
        {
            std::ostringstream builder;
            if ( not m_pimpl->scheme.empty( ) )
            {
                builder << m_pimpl->scheme;
                if ( m_pimpl->scheme.find( "://" ) == string::npos )
                    builder << "://";
            }
            
            if ( not m_pimpl->username.empty( ) )
                builder << m_pimpl->username;
                
            if ( not m_pimpl->password.empty( ) )
                builder << ":" << m_pimpl->password;
                
            if ( not m_pimpl->username.empty( ) or not m_pimpl->password.empty( ) )
                builder << "@";
                
            builder << m_pimpl->authority;
            
            if ( not m_pimpl->port.empty( ) )
                builder << ":" << m_pimpl->port;
                
            if ( not m_pimpl->path.empty( ) )
            {
                if ( m_pimpl->path.front( ) == '/' )
                    builder << m_pimpl->path;
                else
                    builder << "/" << m_pimpl->path;
            }
            
            if ( not m_pimpl->query.empty( ) )
            {
                if ( m_pimpl->query.front( ) == '?' )
                    builder << m_pimpl->query;
                else
                    builder << "?" << m_pimpl->query;
            }
            
            if ( not m_pimpl->fragment.empty( ) )
            {
                if ( m_pimpl->fragment.front( ) == '#' )
                    builder << m_pimpl->fragment;
                else
                    builder << "#" << m_pimpl->fragment;
            }
            
            return builder.str( );
        }
        
        bool URIBuilder::is_malformed( void )
        {
            return not regex_match( build( ), m_pimpl->pattern );
        }
        
        bool URIBuilder::is_finalised( void )
        {
            return not m_pimpl->scheme.empty( ) and not m_pimpl->authority.empty( );
        }
        
        uint16_t URIBuilder::get_port( void ) const
        try
        {
            return stoi( m_pimpl->port );
        }
        catch ( const invalid_argument )
        {
            return 0;
        }
        catch ( const out_of_range )
        {
            return 0;
        }
        
        string URIBuilder::get_path( void ) const
        {
            return m_pimpl->path;
        }
        
        string URIBuilder::get_query( void ) const
        {
            return m_pimpl->query;
        }
        
        string URIBuilder::get_scheme( void ) const
        {
            return m_pimpl->scheme;
        }
        
        string URIBuilder::get_fragment( void ) const
        {
            return m_pimpl->fragment;
        }
        
        string URIBuilder::get_username( void ) const
        {
            return m_pimpl->username;
        }
        
        string URIBuilder::get_password( void ) const
        {
            return m_pimpl->password;
        }
        
        string URIBuilder::get_authority( void ) const
        {
            return m_pimpl->authority;
        }
        
        multimap< string, string > URIBuilder::get_parameters( void ) const
        {
            multimap< string, string > parameters;
            
            auto query = m_pimpl->query;
            if ( query.front( ) == '?' )
                query.erase( query.begin( ) );
            if ( query.back( ) == '&' )
                query.erase( query.end( ) - 1 );
                
            string parameter = "";
            istringstream stream { query };
            while ( getline( stream, parameter, '&' ) )
            {
                auto index = parameter.find_first_of( '=' );
                auto name = parameter.substr( 0, index );
                auto value = parameter.substr( index + 1, parameter.length( ) );
                parameters.insert( make_pair( name, value ) );
            }
            
            return parameters;
        }
        
        void URIBuilder::set_port( const uint16_t value )
        {
            m_pimpl->port = to_string( value );
        }
        
        void URIBuilder::set_path( const string& value )
        {
            m_pimpl->path = value;
        }
        
        void URIBuilder::set_query( const string& value )
        {
            m_pimpl->query = value;
        }
        
        void URIBuilder::set_scheme( const string& value )
        {
            m_pimpl->scheme = value;
        }
        
        void URIBuilder::set_fragment( const string& value )
        {
            m_pimpl->fragment = value;
        }
        
        void URIBuilder::set_username( const string& value )
        {
            m_pimpl->username = value;
        }
        
        void URIBuilder::set_password( const string& value )
        {
            m_pimpl->password = value;
        }
        
        void URIBuilder::set_authority( const string& value )
        {
            m_pimpl->authority = value;
        }
        
        void URIBuilder::set_parameters( const multimap< string, string >& values )
        {
            string query = "";
            for ( auto parameter : values )
                query += parameter.first + "=" + parameter.second + "&";
                
            if ( query.back( ) == '&' )
                query.erase( query.end( ) - 1 );
                
            m_pimpl->query = query;
        }
    }
}
