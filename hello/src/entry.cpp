#include "include.hpp"
#include "gui/gui.hpp"
#include "gui/ui.hpp"
#include "ngrok/ngrok.hpp"
#include "util/util.hpp"
#include "util/logging.hpp"

int main( HINSTANCE inst ) 
{
	ngrok::init( );
	logging::info( "ngrok initilized." );

	::ShowWindow( ::GetConsoleWindow( ), SW_HIDE );

	auto hwnd = gui::create_window( inst );
	logging::info( "created window." );

	if ( !gui::create_device_d3d( hwnd ) )
		return false;

	::ShowWindow( hwnd, SW_SHOWDEFAULT );
	::UpdateWindow( hwnd );

	ImGui::CreateContext( );

	ImGui_ImplWin32_Init( hwnd );
	ImGui_ImplDX9_Init( gui::g_pd3ddevice );

	gui::setup_imgui_style( );

	ImFont* font = ImGui::GetIO( ).Fonts->AddFontFromMemoryTTF( const_cast<std::uint8_t*>( rubik ), sizeof( rubik ), 20.f, NULL );
	IM_ASSERT( font != NULL );

	ImVec4 clear_color = ImVec4( 0.039f, 0.066f, 0.105f, 1.00f );

	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );
	while ( msg.message != WM_QUIT )
	{
		if ( ::PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			::TranslateMessage( &msg );
			::DispatchMessage( &msg );
			continue;
		}

		ImGui_ImplDX9_NewFrame( );
		ImGui_ImplWin32_NewFrame( );
		ImGui::NewFrame( );
		 
		{
			ui::begin( );
		}

		ImGui::EndFrame( );
		gui::g_pd3ddevice->SetRenderState( D3DRS_ZENABLE, FALSE );
		gui::g_pd3ddevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
		gui::g_pd3ddevice->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );
		D3DCOLOR clear_col_dx = D3DCOLOR_RGBA( ( int ) ( clear_color.x * 255.0f ), ( int ) ( clear_color.y * 255.0f ), ( int ) ( clear_color.z * 255.0f ), ( int ) ( clear_color.w * 255.0f ) );
		gui::g_pd3ddevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0 );
		if ( gui::g_pd3ddevice->BeginScene( ) >= 0 )
		{
			ImGui::Render( );
			ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
			gui::g_pd3ddevice->EndScene( );
		}
		gui::g_pd3ddevice->Present( NULL, NULL, NULL, NULL );
	}

	ImGui_ImplDX9_Shutdown( );
	ImGui_ImplWin32_Shutdown( );
	ImGui::DestroyContext( );

	gui::cleanup_device_d3d( );
	::DestroyWindow( hwnd );

	return false;
}