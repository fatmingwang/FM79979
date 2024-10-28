#include "MyImGuiUIEditor.h"
#ifdef WIN32
#include "../../Core/AllCoreInclude.h"
#include "../ThirtyParty/FileDialog/ImGuiFileDialog.h"

//cursor
struct sMouseCursor
{
	HCURSOR m_arrow_all = nullptr;
	HCURSOR m_arrow_top_or_bottom = nullptr;
	HCURSOR m_arrow_left_or_right = nullptr;
	HCURSOR m_arrow_northwest_and_southeast = nullptr;
	HCURSOR m_arrow_northeast_and_southwest = nullptr;
	HCURSOR m_current_icon = nullptr;
};

void cMyImGuiUIEditor::RenderFileDoalog()
{
	static std::vector<std::string> dialogs_keys = {
		"SaveProjectFileDlgKey",
		"OpenProjectFileDlgKey",
		"GenCodeProjectFileDlgKey",
		"SaveColorsDlgKey",
		"OpenColorsDlgKey",
		"SaveFlagsDlgKey",
		"OpenFlagsDlgKey"
	};

	auto DeleteKey = [&](const std::string& keyname) -> void {
		HKEY hKey;
		RegOpenKeyExA(HKEY_CURRENT_USER, NULL, 0, KEY_SET_VALUE, &hKey);

		RegDeleteKeyExA(hKey, keyname.c_str(), KEY_WOW64_64KEY, NULL);

		RegCloseKey(hKey);
		};

	auto SetValue = [&](const std::string& keyname, const std::string& value) -> void {
		DeleteKey(keyname);

		HKEY hKey;
		RegOpenKeyExA(HKEY_CURRENT_USER, NULL, 0, KEY_SET_VALUE, &hKey);

		auto status = RegSetValueExA(hKey, keyname.c_str(), NULL, REG_SZ, (LPBYTE)value.c_str(), value.size() + 1);

		printf_s("RegSetValueEx: %d\n", status);

		RegCloseKey(hKey);
		};

	auto i = 0;
	for (auto key : dialogs_keys)
	{
		if (ImGuiFileDialog::Instance()->Display(key.c_str(), 32, { 350.f, 300.f }))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string file_full_path = ImGuiFileDialog::Instance()->GetFilePathName();
				std::string full_path = ImGuiFileDialog::Instance()->GetCurrentPath();

				SetValue("ImGuiBuilderPath", full_path);

				switch (i)
				{
				case eFILE_DIALOG_RESULT::eFDR_UI_LAYOUT_SAVE:
				{
//					if (im_config::controls::save(file_full_path, m_forms, m_objs))
						MessageBoxA(nullptr, "Project saved!", "ImGui Builder", MB_OK | MB_ICONINFORMATION);
					break;
				}
				case eFILE_DIALOG_RESULT::eFDR_UI_LAYOUT_OPEN:
				{
					//m_uiID = 0;
					//if (im_config::controls::load(file_full_path, m_forms, m_objs, &l_iID))
						MessageBoxA(nullptr, "Project loaded!", "ImGui Builder", MB_OK | MB_ICONINFORMATION);
					break;
				}
				case eFDR_GENERATE_CODE:
				{
					//if (im_config::controls::create_code(file_full_path, m_forms, m_objs))
						MessageBoxA(nullptr, "Code been generated!", "ImGui Builder", MB_OK | MB_ICONINFORMATION);
					break;
				}
				case eFDR_COLOR_SAVE:
				{
					//if (im_config::color::save(file_full_path, m_custom_gui_style))
						MessageBoxA(nullptr, "Colors saved!", "ImGui Builder", MB_OK | MB_ICONINFORMATION);
					break;
				}
				case eFDR_COLOR_OPEN:
				{
					//if (im_config::color::load(file_full_path, m_custom_gui_style))
						MessageBoxA(nullptr, "Colors loaded!", "ImGui Builder", MB_OK | MB_ICONINFORMATION);
					break;
				}
				case eFDR_FLAG_SAVE:
				{
					//if (im_config::window_flags::save(file_full_path, m_custom_gui_style))
						MessageBoxA(nullptr, "Flags saved!", "ImGui Builder", MB_OK | MB_ICONINFORMATION);
					break;
				}
				case eFDR_FLAG_OPEN:
				{
					//if (im_config::window_flags::load(file_full_path, m_custom_gui_style))
						MessageBoxA(nullptr, "Flags loaded!", "ImGui Builder", MB_OK | MB_ICONINFORMATION);
					break;
				}
				default:
					break;
				}
			}

			ImGuiFileDialog::Instance()->Close();
		}
		++i;
	}
}

void cMyImGuiUIEditor::RenderRootNodeTree()
{
}

cMyImGuiUIEditor::cMyImGuiUIEditor()
{
}

void cMyImGuiUIEditor::Render()
{
}


//Move obj with cursor

uint64_t		g_tick_move = 0;
bool KeyPressed(int e_iK)
{
	auto l_Key = GetAsyncKeyState(e_iK);
	return (GetFocus() == cGameApp::m_spOpenGLRender->m_Handle) && (l_Key & 0x8000);
}
bool HoldingKey(int key)
{
	return (GetFocus() == cGameApp::m_spOpenGLRender->m_Handle) && GetAsyncKeyState(key);
}

bool PressedBindingKeys(int key_1, int key_2)
{
	return (GetFocus() == cGameApp::m_spOpenGLRender->m_Handle) && (GetAsyncKeyState(key_1)) && (GetAsyncKeyState(key_2) & 1);
}

int cMyImGuiUIEditor::GetUniqueID()
{
	return ++m_uiUniqueID;
}
void cMyImGuiUIEditor::move_item(ImVec2& obj_pos, bool& continue_edt)
{
	static ImVec2 old_pos{ };
	if (!HoldingKey(VK_LBUTTON) || !continue_edt)
	{
		old_pos = { 0, 0 };
		m_MovingObj = false;
		continue_edt = false;
		g_tick_move = 0;
		return;
	}
	auto tick_now = GetTickCount64();
	auto current_pos = cGameApp::m_sMousePosition;
	if (g_tick_move == 0)
	{
		old_pos = { current_pos.x - obj_pos.x, current_pos.y - obj_pos.y };
		g_tick_move = tick_now + 50;
		return;
	}
	auto current_win_pos = ImGui::GetWindowPos();
	if (tick_now > g_tick_move)
	{
		m_MovingObj = true;
		const auto x_pos = current_pos.x - old_pos.x;
		const auto y_pos = current_pos.y - old_pos.y;
		obj_pos.x = x_pos;
		obj_pos.y = y_pos;
	}
}

resize_opt GetResizeOptWithObjectSizeAndPos(ImVec2 obj_pos, ImVec2 obj_size, float thickness)
{
	auto current_win_pos = ImGui::GetWindowPos();

	auto control_win_pos = ImVec2(current_win_pos.x + obj_pos.x, current_win_pos.y + obj_pos.y);

	auto pos = cGameApp::m_sMousePosition;

	auto top = (pos.y >= (long)(control_win_pos.y - thickness) && pos.y <= (long)(control_win_pos.y));

	auto bottom = (pos.y >= (long)(control_win_pos.y + obj_size.y) && pos.y <= (long)(control_win_pos.y + obj_size.y + thickness));

	auto left = (pos.x >= (long)(control_win_pos.x - thickness) && pos.x <= (long)(control_win_pos.x));

	auto right = (pos.x >= (long)(control_win_pos.x + obj_size.x) && pos.x <= (long)(control_win_pos.x + obj_size.x + thickness));

	if ((bottom && right) || (top && left))
	{
		return (bottom && right) ? resize_opt::bottom_right : resize_opt::top_left;
	}
	else
	if ((top && right) || (bottom && left))
	{
		return (top && right) ? resize_opt::top_right : resize_opt::bottom_left;
	}
	else
	{
		if (top || bottom)
		{
			return (top) ? resize_opt::top : resize_opt::bottom;
		}
		else
		if (left || right)
		{
			return (left) ? resize_opt::left : resize_opt::right;
		}
	}
	return resize_opt::off;
}


bool IsItemHovered(ImVec2 obj_pos, ImVec2 obj_size, float distance)
{
	auto current_win_pos = ImGui::GetWindowPos();
	auto control_win_pos = ImVec2(current_win_pos.x + (obj_pos.x - distance), current_win_pos.y + (obj_pos.y - distance));
	auto pos = cGameApp::m_sMousePosition;
	return (pos.y >= control_win_pos.y && pos.y <= (control_win_pos.y + (obj_size.y + (distance * 2))) && pos.x >= control_win_pos.x && pos.x <= (control_win_pos.x + obj_size.x + (distance * 2)));
}
bool IsCollides(cImGuiNode* e_pNode, ImVec2 e_vPoint)
{
	auto l_vPos = e_pNode->GetWorldPosition();
	RECT l_Rect = { (long)l_vPos.x,(long)l_vPos.y,(long)l_vPos.x,(long)l_vPos .y};
	POINT l_Point = { (long)e_vPoint .x,(long)e_vPoint .y};
	return RectCollidePoint(l_Rect, l_Point);
}
#endif