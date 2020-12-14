#include <string>
#include <algorithm>
#include <fstream>

#include "util.hpp"

namespace hello
{
	bool util::create_process( STARTUPINFO startup_info, PROCESS_INFORMATION process_info, std::string cmd_args )
	{
		try
		{
			memset( &startup_info, 0, sizeof( STARTUPINFO ) );
			startup_info.cb = sizeof( STARTUPINFO );
			memset( &process_info, 0, sizeof( PROCESS_INFORMATION ) );

			// CloseHandle( )
			BOOL proc = ::CreateProcess(
				NULL,
				(char*)cmd_args.c_str( ),
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

	bool util::create_file( std::string file_name )
	{
		std::ofstream file( file_name, std::ios::out | std::ios::trunc );
		file.close( );
		return file.good( );
	}

	bool util::write_to_file( std::string file_name, std::string json )
	{
		std::ofstream file( file_name, std::ios::out | std::ios::trunc );
		std::string str = json;
		file << str;
		file.close( );
		return file.good( );
	}

	std::string util::read_file( std::string file_name )
	{
		std::string output;
		std::ifstream file;
		file.open( file_name, std::ios::in );
		if ( !file.is_open( ) )
		{
			std::string error_message = "Ocorreu uma falha ao abrir " + file_name + ".";
			MessageBox( NULL, error_message.c_str( ), "Error", MB_ICONERROR );
			exit( -1 );
		}
		std::getline( file, output );
		file.close( );
		return output;
	}

	bool util::file_exists( std::string string )
	{
		struct stat buffer;
		return ( stat( string.c_str( ), &buffer ) == 0 );
	}

	void util::to_clipboard( HWND hwnd, const std::string& s )
	{
		OpenClipboard( hwnd );
		EmptyClipboard( );
		HGLOBAL hg = GlobalAlloc( GMEM_MOVEABLE, s.size( ) + 1 );
		if ( !hg ) { CloseClipboard( ); return; }
		memcpy( GlobalLock( hg ), s.c_str( ), s.size( ) + 1 );
		GlobalUnlock( hg );
		SetClipboardData( CF_TEXT, hg );
		CloseClipboard( );
		GlobalFree( hg );
	}

	void util::to_lowercase( std::string& string )
	{
		std::transform( string.begin( ), string.end( ), string.begin( ), ::tolower );
	}

	void util::to_uppercase( std::string& string )
	{
		std::transform( string.begin( ), string.end( ), string.begin( ), ::toupper );
	}
}