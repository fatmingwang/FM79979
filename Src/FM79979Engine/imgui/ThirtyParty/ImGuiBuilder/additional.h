#pragma once
#include "../../imgui.h"
#include <vector>

class cMyGuiBasicObj;

namespace ImGui
{
	void ToggleButton( const char* str_id, bool* v );
	void SetStyle( ImGuiStyle e_Style );
	void PopAllColorsCustom( );
	bool InputTextEx( const char* label, std::string* str, ImGuiInputTextFlags flags );
	//draw selected border
	void DrawObjBorder( ImVec2 obj_pos, ImVec2 obj_size, float distance_thickness = 5.f, ImU32 col = IM_COL32( 255, 255, 0, 255 ) );
	void DrawObjBorder(cMyGuiBasicObj* obj, float distance_thickness = 5.f, ImU32 col = IM_COL32( 255, 255, 0, 255 ) );
}