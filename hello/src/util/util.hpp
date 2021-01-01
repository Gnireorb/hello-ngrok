#pragma once
#include "../include.hpp"

namespace util
{
	bool create_process( STARTUPINFO startup_info, PROCESS_INFORMATION process_info, std::string cmd_args );
	bool create_file( std::string file_name );
	bool write_to_file( std::string file_name, std::string json );
	std::string read_file( const std::string_view file_path );
	bool file_exists( std::string string );
	void to_clipboard( const char* str );
}