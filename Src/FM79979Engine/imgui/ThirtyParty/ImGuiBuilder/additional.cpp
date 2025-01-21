#include "../../MyGui/MyGui.h"
#include "additional.h"

int InputTextCallback( ImGuiInputTextCallbackData* data )
{
	if ( data->EventFlag == ImGuiInputTextFlags_CallbackResize )
	{
		auto str = static_cast<std::string*>( data->UserData );
		IM_ASSERT( data->Buf == str->c_str( ) );
		str->resize( data->BufTextLen );
		data->Buf = const_cast<char*>( str->c_str( ) );
	}
	return data->BufSize;
}

bool ImGui::InputTextEx( const char* label, std::string* str, ImGuiInputTextFlags flags )
{
	flags |= ImGuiInputTextFlags_CallbackResize;
	return ImGui::InputText( label, const_cast<char*>( str->c_str( ) ), str->capacity( ) + 1, flags, InputTextCallback, static_cast<void*>( str ) );
}

void ImGui::ToggleButton( const char* str_id, bool* v )
{
	const auto p = ImGui::GetCursorScreenPos( );
	auto* draw_list = ImGui::GetWindowDrawList( );

	const auto height = ImGui::GetFrameHeight( );
	const auto width = height * 1.55f;
	const auto radius = height * 0.50f;

	if ( ImGui::InvisibleButton( str_id, ImVec2( width, height ) ) )
		*v = !*v;
	ImU32 col_bg = 0;
	//custom_gui_style.Colors[22].w, custom_gui_style.Colors[22].x, custom_gui_style.Colors[22].y, custom_gui_style.Colors[22].z
	if ( ImGui::IsItemHovered( ) )
		col_bg = *v ? IM_COL32( 145 + 20, 211, 68 + 20, 255 ) : IM_COL32( 218 - 20, 218 - 20, 218 - 20, 255 );
	else
		col_bg = *v ? IM_COL32( 145, 211, 68, 255 ) : IM_COL32( 218, 218, 218, 255 );

	draw_list->AddRectFilled( p, ImVec2( p.x + width, p.y + height ), col_bg, height * 0.5f );

	draw_list->AddCircleFilled( ImVec2( *v ? ( p.x + width - radius ) : ( p.x + radius ), p.y + radius ), radius - 1.5f,
		IM_COL32( 255, 255, 255, 255 ) );
}

void ImGui::SetStyle( ImGuiStyle dark )
{
	auto& style = ImGui::GetStyle( );

	for ( auto i = 0; i < ImGuiCol_COUNT; i++ )
	{
		ImGui::PushStyleColor( static_cast<ImGuiCol_>( i ), dark.Colors[ i ] );
	}
	//Padding
	style.WindowPadding = dark.WindowPadding;
	style.FramePadding = dark.FramePadding;
	style.CellPadding = dark.CellPadding;
	style.ItemSpacing = dark.ItemSpacing;
	style.ItemInnerSpacing = dark.ItemInnerSpacing;
	style.TouchExtraPadding = dark.TouchExtraPadding;
	style.IndentSpacing = dark.IndentSpacing;
	style.ScrollbarSize = dark.ScrollbarSize;
	style.GrabMinSize = dark.GrabMinSize;
	style.WindowBorderSize = dark.WindowBorderSize;
	style.ChildBorderSize = dark.ChildBorderSize;
	style.PopupBorderSize = dark.PopupBorderSize;
	style.FrameBorderSize = dark.FrameBorderSize;
	style.TabBorderSize = dark.TabBorderSize;

	//Rounding
	style.WindowRounding = dark.WindowRounding;
	style.ChildRounding = dark.ChildRounding;
	style.FrameRounding = dark.FrameRounding;
	style.PopupRounding = dark.PopupRounding;
	style.ScrollbarRounding = dark.ScrollbarRounding;
	style.GrabRounding = dark.GrabRounding;
	style.LogSliderDeadzone = dark.LogSliderDeadzone;
	style.TabRounding = dark.TabRounding;

	//Position
	style.WindowTitleAlign = dark.WindowTitleAlign;
	style.WindowMenuButtonPosition = dark.WindowMenuButtonPosition;
	style.ColorButtonPosition = dark.ColorButtonPosition;
	style.ButtonTextAlign = dark.ButtonTextAlign;
	style.SelectableTextAlign = dark.SelectableTextAlign;

	//NANI???
	style.DisplaySafeAreaPadding = dark.DisplaySafeAreaPadding;
}


void ImGui::PopAllColorsCustom( )
{
	ImGui::PopStyleColor( ImGuiCol_COUNT );
}

void ImGui::DrawObjBorder(cMyGuiBasicObj* obj, float distance_thickness, ImU32 col )
{
	DrawObjBorder( obj->GetWorldPosition(), obj->m_vSizeObj, distance_thickness, col);
}

void ImGui::DrawObjBorder( ImVec2 obj_pos, ImVec2 obj_size, float distance_thickness, ImU32 col )
{
	ImVec2 vMin		= { ImGui::GetWindowPos( ).x + obj_pos.x - distance_thickness, ImGui::GetWindowPos( ).y + obj_pos.y - distance_thickness };
	ImVec2 vMax		= { vMin.x + obj_size.x + ( distance_thickness * 2.f ), vMin.y + obj_size.y + ( distance_thickness * 2.f ) };
	//ImGui::GetForegroundDrawList( )->AddRect( vMin, vMax, col ); //will draw on top of everything 
	ImGui::GetWindowDrawList( )->AddRect( vMin, vMax, col, 0.f, 15, 0.01f );
}