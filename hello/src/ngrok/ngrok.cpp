#include "../../dependencies/HTTPRequest/HTTPRequest.hpp"
#include "../../dependencies/rapidjson/document.h"
#include "../../dependencies/rapidjson/stringbuffer.h"
#include "../../dependencies/rapidjson/writer.h"

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

void ngrok::load_ngrok_settings( )
{
	Document doc;
	doc.Parse( util::read_file( "settings.json" ).c_str( ) );

	settings::region = doc[ "ngrok_region" ].GetInt( );
	settings::port = doc[ "last_port" ].GetInt( );
}

bool ngrok::init( )
{
	try
	{
		if ( !util::file_exists( "ngrok.exe" ) )
			throw std::runtime_error( "O arquivo 'ngrok.exe' eh inexistente, baixe-o em 'ngrok.com'." );

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
	std::string proc_name = "ngrok.exe";
	std::string args;
	std::string cmd_line;

	if ( region >= 4 || region < 0 )
		return false;

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

	cmd_line = args.c_str( );

	try
	{
		STARTUPINFO startup_info;
		PROCESS_INFORMATION process_info;

		memset( &startup_info, 0, sizeof( STARTUPINFO ) );
		startup_info.cb = sizeof( STARTUPINFO );
		memset( &process_info, 0, sizeof( PROCESS_INFORMATION ) );

		BOOL proc = ::CreateProcess(
			NULL,
			(char*)cmd_line.c_str( ),
			NULL,
			NULL,
			FALSE,
			CREATE_NO_WINDOW | CREATE_NEW_PROCESS_GROUP,
			NULL,
			NULL,
			&startup_info,
			&process_info
		);

		if ( proc == FALSE )
			throw std::runtime_error( "Ocorreu uma falha no CreateProcess()." );
	}
	catch ( const std::runtime_error& error )
	{
		::MessageBox( NULL, error.what( ), "Error", MB_ICONERROR );
		return false;
	}

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

	return nullptr;
}