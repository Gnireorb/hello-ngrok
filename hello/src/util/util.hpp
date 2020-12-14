#pragma once
#include "../include.hpp"

namespace hello
{
	class util
	{
	public:
		static bool create_process( STARTUPINFO startup_info, PROCESS_INFORMATION process_info, std::string cmd_args );
		static bool create_file( std::string file_name );
		static bool write_to_file( std::string file_name, std::string json );
		static std::string read_file( std::string file_name );
		static bool file_exists( std::string string );
		static void to_clipboard( HWND hwnd, const std::string& s );
		static void to_lowercase( std::string& string );
		static void to_uppercase( std::string& string );
	};
	inline util g_util;
}