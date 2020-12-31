#include "gui.hpp"
#include "ui.hpp"
#include "../ngrok/ngrok.hpp"
#include "../include.hpp"
#include "../util/util.hpp"

#include "../../../dependencies/imgui/imgui_stdlib.h"

inline void load_settings( )
{
    Document doc;
    doc.Parse( util::read_file( "settings.json" ).c_str( ) );

    settings::region = doc[ "ngrok_region" ].GetInt( );
    settings::port = doc[ "last_port" ].GetInt( );
}

void ui::begin( )
{
    load_settings( );

    static bool debug_mode = false;
    static std::string ip{};
    static std::string authtoken{};

    ImGui::SetNextWindowSize( ImVec2( 360, 656 ), ImGuiCond_FirstUseEver );
    ImGui::SetNextWindowPos( ImVec2( 15, 12 ), ImGuiCond_FirstUseEver );
    if ( ImGui::Begin( "hello-ngrok", NULL ) )
    {
        ImGui::Text( "hello-ngrok" );
        ImGui::Separator( );
        ImGui::Text( "Tunnel" );
        ImGui::PushItemWidth( 200 );
        if ( ImGui::InputInt( "Port", &settings::port ) )
        {
            Document doc;
            doc.Parse( util::read_file( "settings.json" ).c_str( ) );
            Value& var = doc[ "last_port" ];
            var.SetInt( settings::port );
            StringBuffer buffer;
            Writer<StringBuffer> writer( buffer );
            doc.Accept( writer );
            util::write_to_file( "settings.json", buffer.GetString( ) );
        }
        ImGui::PopItemWidth( );
        if ( ImGui::Button( "Create tunnel" ) )
        {
            ip.clear( );
            ngrok::create_tunnel( settings::port, settings::region );
        }
        ImGui::SameLine( );
        if ( ImGui::Button( "Close tunnel" ) )
        {
            ip.clear( );
            std::system( "taskkill /f /im ngrok.exe" );
        }
        ImGui::InputText( "IP", &ip );
        if ( ImGui::Button( "Get IP" ) )
        {
            ip.clear( );
            ip = ngrok::get_public_url( );
        }
        ImGui::SameLine( );
        if ( ImGui::Button( "Copy IP" ) )
        {
            HWND hwnd = GetDesktopWindow( );
            util::to_clipboard( hwnd, ngrok::get_public_url( ) );
        }
        ImGui::Separator( );
        ImGui::Text( "Authtoken" );
        ImGui::InputText( "###authtokeninput", &authtoken );
        if ( ImGui::Button( "Set ngrok authtoken" ) )
        {
            std::string commandline = "ngrok authtoken " + authtoken;
            std::system( commandline.c_str( ) );
        }
        ImGui::Separator( );
        ImGui::Text( "Tunnel region" );
        if ( ImGui::Combo( "###tunnelregion", &settings::region, settings::regions, sizeof( settings::regions ) / sizeof( *settings::regions ) ) )
        {
            Document doc;
            doc.Parse( util::read_file( "settings.json" ).c_str( ) );
            Value& var = doc[ "ngrok_region" ];
            var.SetInt( settings::region );
            StringBuffer buffer;
            Writer<StringBuffer> writer( buffer );
            doc.Accept( writer );
            util::write_to_file( "settings.json", buffer.GetString( ) );
        }
        ImGui::Separator( );
        if ( ImGui::CollapsingHeader( "Settings" ) )
        {
            if ( ImGui::Checkbox( "Debug mode", &debug_mode ) )
            {
                ::ShowWindow( ::GetConsoleWindow( ), debug_mode ? SW_SHOW : SW_HIDE );
            }
        }
        ImGui::Separator( );

        ImGui::Text( "github.com/gnireorb/hello-ngrok" );
    }

    ImGui::End( );
}
