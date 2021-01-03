#include "../../../dependencies/HTTPRequest/HTTPRequest.hpp"
#include "../../../dependencies/rapidjson/document.h"
#include "../../../dependencies/rapidjson/stringbuffer.h"
#include "../../../dependencies/rapidjson/writer.h"

#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <tchar.h>
#include <future>

#include "../settings.hpp"
#include "ngrok.hpp"
#include "../util/util.hpp"

using namespace rapidjson;

bool ngrok::init( )
{
	try
	{
#if (!_DEBUG)
		if ( !util::file_exists( "ngrok.exe" ) )
			throw std::runtime_error( "O arquivo 'ngrok.exe' eh inexistente, baixe-o em 'ngrok.com'." );
#endif

		if ( !util::file_exists( "settings.json" ) )
		{
			if ( !util::create_file( "settings.json" ) )
				throw std::runtime_error( "Ocorreu uma falha ao criar o arquivo 'settings.json." );

			if ( !util::file_exists( "settings.json" ) )
				throw std::runtime_error( "O arquivo 'settings.json' eh inexistente." );

			if ( !util::write_to_file( "settings.json", "{\"last_port\":0,\"ngrok_region\":0}" ) )
				throw std::runtime_error( "Ocorreu uma falha ao escrever em 'settings.json'." );
		}
	}
	catch ( const std::runtime_error& error )
	{
		MessageBox( NULL, error.what( ), "Error", MB_ICONERROR );
		exit( 0 );
	}

	return true;
}

bool ngrok::create_tunnel( int port, int region )
{
	STARTUPINFO startup_info{};
	PROCESS_INFORMATION process_info{};
	std::string args;

	if ( region >= 4 || region < 0 )
		return false;

	switch ( region )
	{
	case 0:
	{
		args = fmt::format( "ngrok.exe tcp {} --region sa", port );
		break;
	}
	case 1:
	{
		args = fmt::format( "ngrok.exe tcp {} --region us", port );
		break;
	}
	case 2:
	{
		args = fmt::format( "ngrok.exe tcp {} --region eu", port );
		break;
	}
	case 3:
	{
		args = fmt::format( "ngrok.exe tcp {} --region ap", port );
		break;
	}
	case 4:
	{
		args = fmt::format( "ngrok.exe tcp {} --region au", port );
		break;
	}
	}

	if ( !util::create_process( startup_info, process_info, args ) )
		return false;

	return true;
}

std::string ngrok::get_public_url( )
{
	std::string api_request;
	try
	{
		http::Request request( "http://127.0.0.1:4040/api/tunnels" );
		const http::Response response = request.send( "GET" );
		api_request = std::string( response.body.begin( ), response.body.end( ) );
	}
	catch ( const std::exception& e )
	{
		MessageBox( NULL, e.what( ), "Error", MB_ICONERROR );
	}
	Document d;
	d.Parse( api_request.c_str() );
	Value const& tunnels = d[ "tunnels" ];
	for ( auto& tunnel_url : tunnels.GetArray( ) )
	{
		std::string public_url = tunnel_url[ "public_url" ].GetString( );
		return public_url.erase( 0, 6 );
	}

	return "Unknown IP";
}