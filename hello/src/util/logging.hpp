#pragma once
#include "../include.hpp"

namespace logging
{
	template<typename... Args>
	void info( const std::string_view string, Args... args )
	{
		const auto time = fmt::format( "[{:%X}]: ", fmt::localtime( std::time( nullptr ) ) );
		static auto handle = GetStdHandle( STD_OUTPUT_HANDLE );
		SetConsoleTextAttribute( handle, FOREGROUND_BLUE | FOREGROUND_INTENSITY );
		fmt::print( time );
		std::string append{ string };
		append.append( "\n" );
		fmt::print( append, std::forward<Args>( args )... );
		SetConsoleTextAttribute( handle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED );
	}

	template<typename... Args>
	void error( const std::string_view string, Args... args )
	{
		const auto time = fmt::format( "[{:%X}]: ", fmt::localtime( std::time( nullptr ) ) );
		static auto handle = GetStdHandle( STD_OUTPUT_HANDLE );
		SetConsoleTextAttribute( handle, FOREGROUND_RED | FOREGROUND_INTENSITY );
		fmt::print( time );
		std::string append{ string };
		append.append( "\n" );
		fmt::print( append, std::forward<Args>( args )... );
		SetConsoleTextAttribute( handle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED );
	}

	template<typename... Args>
	void fatal( const std::string_view string, Args... args )
	{
		const auto time = fmt::format( "[{:%X}]: ", fmt::localtime( std::time( nullptr ) ) );
		static auto handle = GetStdHandle( STD_OUTPUT_HANDLE );
		SetConsoleTextAttribute( handle, FOREGROUND_RED );
		fmt::print( time );
		std::string append{ string };
		append.append( "\n" );
		fmt::print( append, std::forward<Args>( args )... );
		SetConsoleTextAttribute( handle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED );
	}
}
