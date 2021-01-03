#include "gui.hpp"

IDirect3D9* gui::g_pd3d;
IDirect3DDevice9* gui::g_pd3ddevice;
D3DPRESENT_PARAMETERS gui::g_d3dpp;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

LRESULT _stdcall gui::wnd_proc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{
    if ( ImGui_ImplWin32_WndProcHandler( hwnd, message, wparam, lparam ) )
        return true;

    switch ( message )
    {
    case WM_DESTROY:
        PostQuitMessage( 0 );
        return 0;
    }

    return ::DefWindowProc( hwnd, message, wparam, lparam );
}

HWND gui::create_window( HINSTANCE inst )
{
    WNDCLASSEX wc = { sizeof( WNDCLASSEX ), CS_CLASSDC, wnd_proc, 0L, 0L, GetModuleHandle( NULL ), NULL, NULL, NULL, NULL, _T( "hello-ngrok" ), NULL };

    RegisterClassEx( &wc );

    return ::CreateWindow( wc.lpszClassName, _T( "hello-ngrok" ), WS_OVERLAPPEDWINDOW, 100, 100, 405, 720, NULL, NULL, wc.hInstance, NULL );
}

bool gui::create_device_d3d( HWND hwnd )
{
    if ( ( g_pd3d = Direct3DCreate9( D3D_SDK_VERSION ) ) == NULL )
        return false;

    ZeroMemory( &g_d3dpp, sizeof( g_d3dpp ) );
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    if ( g_pd3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3ddevice ) < 0 )
        return false;

    return true;
}

void gui::cleanup_device_d3d( )
{
	if ( g_pd3ddevice ) { g_pd3ddevice->Release( ); g_pd3ddevice = NULL; }
	if ( g_pd3d ) { g_pd3d->Release( ); g_pd3d = NULL; }
}

void gui::setup_imgui_style( )
{
    ImGui::GetStyle( ).FrameRounding = 4.0f;
    ImGui::GetStyle( ).FramePadding = ImVec2( 5, 5 );
    ImGui::GetStyle( ).GrabRounding = 4.0f;
    ImGui::GetStyle( ).ChildRounding = 4.0f;
    ImGui::GetStyle( ).WindowTitleAlign = ImVec2( 0.5f, 0.5f );
    ImGui::StyleColorsDark( );
}