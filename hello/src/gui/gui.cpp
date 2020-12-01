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

void gui::imgui_easy_theming( ImVec3 color_for_text, ImVec3 color_for_head, ImVec3 color_for_area, ImVec3 color_for_body, ImVec3 color_for_pops )
{
    ImGuiStyle& style = ImGui::GetStyle( );

    style.Colors[ ImGuiCol_Text ] = ImVec4( color_for_text.x, color_for_text.y, color_for_text.z, 1.00f );
    style.Colors[ ImGuiCol_TextDisabled ] = ImVec4( color_for_text.x, color_for_text.y, color_for_text.z, 0.58f );
    style.Colors[ ImGuiCol_WindowBg ] = ImVec4( color_for_body.x, color_for_body.y, color_for_body.z, 0.95f );
    style.Colors[ ImGuiCol_ChildBg ] = ImVec4( color_for_area.x, color_for_area.y, color_for_area.z, 0.58f );
    style.Colors[ ImGuiCol_Border ] = ImVec4( color_for_body.x, color_for_body.y, color_for_body.z, 0.00f );
    style.Colors[ ImGuiCol_BorderShadow ] = ImVec4( color_for_body.x, color_for_body.y, color_for_body.z, 0.00f );
    style.Colors[ ImGuiCol_FrameBg ] = ImVec4( color_for_area.x, color_for_area.y, color_for_area.z, 1.00f );
    style.Colors[ ImGuiCol_FrameBgHovered ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ ImGuiCol_FrameBgActive ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ ImGuiCol_TitleBg ] = ImVec4( color_for_area.x, color_for_area.y, color_for_area.z, 1.00f );
    style.Colors[ ImGuiCol_TitleBgCollapsed ] = ImVec4( color_for_area.x, color_for_area.y, color_for_area.z, 0.75f );
    style.Colors[ ImGuiCol_TitleBgActive ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ ImGuiCol_MenuBarBg ] = ImVec4( color_for_area.x, color_for_area.y, color_for_area.z, 0.47f );
    style.Colors[ ImGuiCol_ScrollbarBg ] = ImVec4( color_for_area.x, color_for_area.y, color_for_area.z, 1.00f );
    style.Colors[ ImGuiCol_ScrollbarGrab ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.21f );
    style.Colors[ ImGuiCol_ScrollbarGrabHovered ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.78f );
    style.Colors[ ImGuiCol_ScrollbarGrabActive ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ ImGuiCol_CheckMark ] = ImVec4( 1.f, 1.f, 1.f, 0.80f );
    style.Colors[ ImGuiCol_SliderGrab ] = ImVec4( 1.f, 1.f, 1.f, 0.50f );
    style.Colors[ ImGuiCol_SliderGrabActive ] = ImVec4( 1.f, 1.f, 1.f, 1.00f );
    style.Colors[ ImGuiCol_Button ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.50f );
    style.Colors[ ImGuiCol_ButtonHovered ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.86f );
    style.Colors[ ImGuiCol_ButtonActive ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ ImGuiCol_Header ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.76f );
    style.Colors[ ImGuiCol_HeaderHovered ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.86f );
    style.Colors[ ImGuiCol_HeaderActive ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ ImGuiCol_ResizeGrip ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.15f );
    style.Colors[ ImGuiCol_ResizeGripHovered ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.78f );
    style.Colors[ ImGuiCol_ResizeGripActive ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ ImGuiCol_PlotLines ] = ImVec4( color_for_text.x, color_for_text.y, color_for_text.z, 0.63f );
    style.Colors[ ImGuiCol_PlotLinesHovered ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ ImGuiCol_PlotHistogram ] = ImVec4( color_for_text.x, color_for_text.y, color_for_text.z, 0.63f );
    style.Colors[ ImGuiCol_PlotHistogramHovered ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ ImGuiCol_TextSelectedBg ] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.43f );
    style.Colors[ ImGuiCol_PopupBg ] = ImVec4( color_for_pops.x, color_for_pops.y, color_for_pops.z, 0.92f );
    style.Colors[ ImGuiCol_ModalWindowDarkening ] = ImVec4( color_for_area.x, color_for_area.y, color_for_area.z, 0.73f );
}

void gui::setup_imgui_style( )
{
    ImGui::GetStyle( ).FrameRounding = 4.0f;
    ImGui::GetStyle( ).FramePadding = ImVec2( 5, 5 );
    ImGui::GetStyle( ).GrabRounding = 4.0f;
    ImGui::GetStyle( ).ChildRounding = 4.0f;
    ImGui::GetStyle( ).WindowTitleAlign = ImVec2( 0.5f, 0.5f );

    static ImVec3 color_for_text = ImVec3( 236.f / 255.f, 240.f / 255.f, 241.f / 255.f );
    static ImVec3 color_for_head = ImVec3( 30.f / 255.f, 47.f / 255.f, 73.f / 255.f );
    static ImVec3 color_for_area = ImVec3( 30.f / 255.f, 47.f / 255.f, 73.f / 255.f );
    static ImVec3 color_for_body = ImVec3( 15.f / 255.f, 24.f / 255.f, 37.f / 255.f );
    static ImVec3 color_for_pops = ImVec3( 22.f / 255.f, 35.f / 255.f, 53.f / 255.f );
    imgui_easy_theming( color_for_text, color_for_head, color_for_area, color_for_body, color_for_pops );
}