#include <string>
#include <algorithm>
#include <fstream>

#include "util.hpp"

bool util::create_process( STARTUPINFO startup_info, PROCESS_INFORMATION process_info, std::string cmd_args )
{
	try
	{
		memset( &startup_info, 0, sizeof( STARTUPINFO ) );
		startup_info.cb = sizeof( STARTUPINFO );
		memset( &process_info, 0, sizeof( PROCESS_INFORMATION ) );

		// I'm leaking the handle. CloseHandle( )
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

std::string util::read_file( const std::string_view file_path )
{
	std::string output;
	std::ifstream file( file_path.data(), std::ios::in );
	if ( !file.good( ) )
	{
		::MessageBox( NULL, "Ocorreu uma falha ao abrir um arquivo.", "Error", MB_ICONERROR );
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

void util::to_clipboard( const char* str )
{
	auto size = MultiByteToWideChar( CP_UTF8, NULL, str, -1, NULL, 0 );
	if ( !size )
		return;
	auto hg = GlobalAlloc( GHND | GMEM_SHARE, size * 2 );
	if ( !hg )
		return;
	auto p = GlobalLock( hg );
	MultiByteToWideChar( CP_UTF8, NULL, str, -1, (LPWSTR)p, size );
	GlobalUnlock( hg );
	OpenClipboard( NULL );
	EmptyClipboard( );
	SetClipboardData( CF_UNICODETEXT, hg );
	CloseClipboard( );
}

