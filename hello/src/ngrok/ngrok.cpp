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
	std::string proc_name = "ngrok.exe";
	std::string args;

	if ( region >= 4 || region < 0 )
		return false;

	// fmt <3
	switch ( region )
	{
	case 0:
		args = proc_name + " tcp " + std::to_string( port ) + " --region sa";
		break;
	case 1:
		args = proc_name + " tcp " + std::to_string( port ) + " --region us";
		break;
	case 2:
		args = proc_name + " tcp " + std::to_string( port ) + " --region eu";
		break;
	case 3:
		args = proc_name + " tcp " + std::to_string( port ) + " --region ap";
		break;
	case 4:
		args = proc_name + " tcp " + std::to_string( port ) + " --region au";
		break;
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
	const char* json = api_request.c_str( );
	Document d;
	d.Parse( json );
	Value const& tunnels = d[ "tunnels" ];
	for ( auto& tunnel_url : tunnels.GetArray( ) )
	{
		std::string public_url = tunnel_url[ "public_url" ].GetString( );
		public_url.erase( 0, 6 );

		return public_url;
	}

	return "Unknown IP";
}