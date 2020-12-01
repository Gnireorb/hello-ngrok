#pragma once
#define DIRECTINPUT_VERSION 0x0800

#include <d3d9.h>
#include <dinput.h>
#include <tchar.h>
#pragma comment (lib, "d3d9.lib")

#include "../include.hpp"

struct ImVec3 { float x, y, z; ImVec3( float _x = 0.0f, float _y = 0.0f, float _z = 0.0f ) { x = _x; y = _y; z = _z; } };

namespace gui
{
	extern IDirect3D9* g_pd3d;
	extern IDirect3DDevice9* g_pd3ddevice;
	extern D3DPRESENT_PARAMETERS g_d3dpp;

	LRESULT _stdcall wnd_proc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );

	HWND create_window( HINSTANCE inst );

	bool create_device_d3d( HWND hwnd );
	void cleanup_device_d3d( );

	void setup_imgui_style( );
	void imgui_easy_theming( ImVec3 color_for_text, ImVec3 color_for_head, ImVec3 color_for_area, ImVec3 color_for_body, ImVec3 color_for_pops );
}
