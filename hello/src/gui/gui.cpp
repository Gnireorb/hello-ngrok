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
    ImVec4* colors = ImGui::GetStyle( ).Colors;
    colors[ ImGuiCol_Text ] = ImVec4( 0.93f, 0.94f, 0.95f, 1.00f );
    colors[ ImGuiCol_TextDisabled ] = ImVec4( 0.93f, 0.94f, 0.95f, 0.58f );
    colors[ ImGuiCol_WindowBg ] = ImVec4( 0.06f, 0.09f, 0.15f, 0.95f );
    colors[ ImGuiCol_ChildBg ] = ImVec4( 0.12f, 0.18f, 0.29f, 0.58f );
    colors[ ImGuiCol_PopupBg ] = ImVec4( 0.09f, 0.14f, 0.21f, 0.92f );
    colors[ ImGuiCol_Border ] = ImVec4( 0.06f, 0.09f, 0.15f, 0.00f );
    colors[ ImGuiCol_BorderShadow ] = ImVec4( 0.06f, 0.09f, 0.15f, 0.00f );
    colors[ ImGuiCol_FrameBg ] = ImVec4( 0.12f, 0.18f, 0.29f, 1.00f );
    colors[ ImGuiCol_FrameBgHovered ] = ImVec4( 0.12f, 0.18f, 0.29f, 1.00f );
    colors[ ImGuiCol_FrameBgActive ] = ImVec4( 0.12f, 0.18f, 0.29f, 1.00f );
    colors[ ImGuiCol_TitleBg ] = ImVec4( 0.12f, 0.18f, 0.29f, 1.00f );
    colors[ ImGuiCol_TitleBgActive ] = ImVec4( 0.12f, 0.18f, 0.29f, 1.00f );
    colors[ ImGuiCol_TitleBgCollapsed ] = ImVec4( 0.12f, 0.18f, 0.29f, 0.75f );
    colors[ ImGuiCol_MenuBarBg ] = ImVec4( 0.12f, 0.18f, 0.29f, 0.47f );
    colors[ ImGuiCol_ScrollbarBg ] = ImVec4( 0.12f, 0.18f, 0.29f, 1.00f );
    colors[ ImGuiCol_ScrollbarGrab ] = ImVec4( 0.12f, 0.18f, 0.29f, 0.21f );
    colors[ ImGuiCol_ScrollbarGrabHovered ] = ImVec4( 0.12f, 0.18f, 0.29f, 0.78f );
    colors[ ImGuiCol_ScrollbarGrabActive ] = ImVec4( 0.12f, 0.18f, 0.29f, 1.00f );
    colors[ ImGuiCol_CheckMark ] = ImVec4( 1.00f, 1.00f, 1.00f, 0.80f );
    colors[ ImGuiCol_SliderGrab ] = ImVec4( 1.00f, 1.00f, 1.00f, 0.50f );
    colors[ ImGuiCol_SliderGrabActive ] = ImVec4( 1.00f, 1.00f, 1.00f, 1.00f );
    colors[ ImGuiCol_Button ] = ImVec4( 0.12f, 0.18f, 0.29f, 0.50f );
    colors[ ImGuiCol_ButtonHovered ] = ImVec4( 0.12f, 0.18f, 0.29f, 0.86f );
    colors[ ImGuiCol_ButtonActive ] = ImVec4( 0.12f, 0.18f, 0.29f, 1.00f );
    colors[ ImGuiCol_Header ] = ImVec4( 0.12f, 0.18f, 0.29f, 0.76f );
    colors[ ImGuiCol_HeaderHovered ] = ImVec4( 0.12f, 0.18f, 0.29f, 0.86f );
    colors[ ImGuiCol_HeaderActive ] = ImVec4( 0.12f, 0.18f, 0.29f, 1.00f );
    colors[ ImGuiCol_Separator ] = ImVec4( 0.43f, 0.43f, 0.50f, 0.50f );
    colors[ ImGuiCol_SeparatorHovered ] = ImVec4( 0.10f, 0.40f, 0.75f, 0.78f );
    colors[ ImGuiCol_SeparatorActive ] = ImVec4( 0.10f, 0.40f, 0.75f, 1.00f );
    colors[ ImGuiCol_ResizeGrip ] = ImVec4( 0.12f, 0.18f, 0.29f, 0.15f );
    colors[ ImGuiCol_ResizeGripHovered ] = ImVec4( 0.12f, 0.18f, 0.29f, 0.78f );
    colors[ ImGuiCol_ResizeGripActive ] = ImVec4( 0.12f, 0.18f, 0.29f, 1.00f );
    colors[ ImGuiCol_Tab ] = ImVec4( 0.18f, 0.35f, 0.58f, 0.86f );
    colors[ ImGuiCol_TabHovered ] = ImVec4( 0.26f, 0.59f, 0.98f, 0.80f );
    colors[ ImGuiCol_TabActive ] = ImVec4( 0.20f, 0.41f, 0.68f, 1.00f );
    colors[ ImGuiCol_TabUnfocused ] = ImVec4( 0.07f, 0.10f, 0.15f, 0.97f );
    colors[ ImGuiCol_TabUnfocusedActive ] = ImVec4( 0.14f, 0.26f, 0.42f, 1.00f );
    colors[ ImGuiCol_PlotLines ] = ImVec4( 0.93f, 0.94f, 0.95f, 0.63f );
    colors[ ImGuiCol_PlotLinesHovered ] = ImVec4( 0.12f, 0.18f, 0.29f, 1.00f );
    colors[ ImGuiCol_PlotHistogram ] = ImVec4( 0.93f, 0.94f, 0.95f, 0.63f );
    colors[ ImGuiCol_PlotHistogramHovered ] = ImVec4( 0.12f, 0.18f, 0.29f, 1.00f );
    colors[ ImGuiCol_TextSelectedBg ] = ImVec4( 0.12f, 0.18f, 0.29f, 0.43f );
    colors[ ImGuiCol_DragDropTarget ] = ImVec4( 1.00f, 1.00f, 0.00f, 0.90f );
    colors[ ImGuiCol_NavHighlight ] = ImVec4( 0.26f, 0.59f, 0.98f, 1.00f );
    colors[ ImGuiCol_NavWindowingHighlight ] = ImVec4( 1.00f, 1.00f, 1.00f, 0.70f );
    colors[ ImGuiCol_NavWindowingDimBg ] = ImVec4( 0.80f, 0.80f, 0.80f, 0.20f );
    colors[ ImGuiCol_ModalWindowDimBg ] = ImVec4( 0.12f, 0.18f, 0.29f, 0.73f );
}