#pragma once
#include <Core/Core.hpp>
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

class Rendering
{
public:
	ImFont* m_font = nullptr;


	static auto GetInstance( ) -> Rendering*
	{
		Rendering obj;
		return &obj;
	}

	auto add_text( float x, float y, std::wstring text, ImColor color ) -> void
	{
		auto DrawList = ImGui::GetForegroundDrawList( );
		auto wText = std::string( text.begin( ), text.end( ) );

		auto Size = ImGui::CalcTextSize( wText.c_str( ) );
		x -= Size.x / 2.f;
		x -= Size.y / 2.f;
		
		ImGui::PushFont( m_font );

		DrawList->AddText( ImVec2( x + 1, y + 1 ), ImColor( 0, 0, 0, 255 ), wText.c_str( ) );
		DrawList->AddText( ImVec2( x, y ), color, wText.c_str( ) );

		ImGui::PopFont( );
	}

	auto add_line( float x, float y, float x1, float y1, ImColor color ) -> void
	{
		auto DrawList = ImGui::GetForegroundDrawList( );
		DrawList->AddLine( ImVec2( x, y ), ImVec2( x1, y1 ), color );		
	}

	auto add_rect( float x, float y, float x1, float y1, ImColor color ) -> void
	{
		auto DrawList = ImGui::GetForegroundDrawList( );
		DrawList->AddRect( ImVec2( x, y ), ImVec2( x1, y1 ), color, 0.f, 15.f );
	}

	auto add_box_filled( ImVec2 position, ImVec2 size, ImColor color ) -> void
	{
		auto DrawList = ImGui::GetForegroundDrawList( );
		DrawList->AddRectFilled( ImVec2( position[ 0 ], position[ 1 ] ), ImVec2( position[ 0 ] + size[ 0 ], position[ 1 ] + size[ 1 ] ), color );
	}

	auto add_box( ImVec2 position, ImVec2 size, ImColor color ) -> void
	{
		add_box_filled( position, ImVec2( size[ 0 ], 1 ), color );
		add_box_filled( position, ImVec2( 1, size[ 1 ] ), color );

		add_box_filled( ImVec2( position[ 0 ] + size[ 0 ], position[ 1 ] ), ImVec2( 1, size[ 1 ] ), color );
		add_box_filled( ImVec2( position[ 0 ], position[ 1 ] + size[ 1 ] ), ImVec2( size[ 0 ] + 1, 1 ), color );
	}

	auto add_rect_filled( float x, float y, float x1, float y1, ImColor color ) -> void
	{
		auto DrawList = ImGui::GetForegroundDrawList( );
		DrawList->AddRectFilled( ImVec2( x, y ), ImVec2( x1, y1 ), color, 0.f, 15.f );
	}

	auto add_circle( float x, float y, float radius, Color color ) -> void
	{

	}
};