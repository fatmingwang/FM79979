#include "MyImGuiUIEditor.h"
#ifdef WIN32
#include "../../Core/AllCoreInclude.h"
#include "../ThirtyParty/FileDialog/ImGuiFileDialog.h"
#include "../ThirtyParty/ImGuiBuilder/additional.h"
#include "../ImGuiRender.h"

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

void cMyImGuiUIEditor::RenderMenu()
{
	ImGui::SetNextWindowSize({ static_cast<float>(1280 - 16) + 1000, (float)250 });
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::Begin("BUILDER", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{

		if (ImGui::BeginMenu("Project"))
		{
			if (ImGui::MenuItem("Save"))
			{
				//ImGuiFileDialog::Instance()->OpenDialog("SaveProjectFileDlgKey", "Save File", ".builder", RegeditGetPath("ImGuiBuilderPath"), "project");
			}

			if (ImGui::MenuItem("Open"))
			{
				//ImGuiFileDialog::Instance()->OpenDialog("OpenProjectFileDlgKey", "Open File", ".builder", RegeditGetPath("ImGuiBuilderPath"), "project");
			}

			if (ImGui::MenuItem("Generate Code"))
			{
				//ImGuiFileDialog::Instance()->OpenDialog("GenCodeProjectFileDlgKey", "Open File", ".cpp,.h,.hpp", RegeditGetPath("ImGuiBuilderPath"), "imgui_builder");
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Editor"))
		{
			if (ImGui::MenuItem("Color"))
			{
				//m_color_menu = !m_color_menu;
			}

			if (ImGui::MenuItem("Style"))
			{
				//m_style_menu = !m_style_menu;
			}

			if (ImGui::MenuItem("Font"))
			{
				//m_font_menu = !m_font_menu;
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
}

void cMyImGuiUIEditor::RenderDebugInfo()
{
	int	l_iOpenGLViewportX = cGameApp::m_spOpenGLRender->m_vViewPortSize.Width();
	int	l_iOpenGLViewportY = cGameApp::m_spOpenGLRender->m_vViewPortSize.Height();
	int fb_width = (int)(ImGui::GetDrawData()->DisplaySize.x * ImGui::GetDrawData()->FramebufferScale.x);
	int fb_height = (int)(ImGui::GetDrawData()->DisplaySize.y * ImGui::GetDrawData()->FramebufferScale.y);
#ifdef WASM
	int l_iBrowserWidth = EMSDK::EMSDK_GetBrowserWidth();
	int l_iBrowserHeight = EMSDK::EMSDK_GetBrowserHeight();
	int	l_iViewportWidth = EMSDK::EMSDK_GetViewportWidth();
	int	l_iViewportHeight = EMSDK::EMSDK_GetViewportHeight();
	int	l_iCanvasPosX = EMSDK::EMSDK_GetCanvasPosX();
	int	l_iCanvasPosY = EMSDK::EMSDK_GetCanvasPosY();
	int windowWidth, windowHeight;
	emscripten_get_canvas_element_size("#canvas", &windowWidth, &windowHeight);

	cGameApp::RenderFont(0, 300, UT::ComposeMsgByFormat(L"FBSize:%d,:%d\nBrowserSize:%d,%d\nCavansSize:%d,%d\nCanvasPos:%d,%d\nOpenGLViewport:%d,%d\nEMSCanvans:%d,%d",
		fb_width, fb_height,
		l_iBrowserWidth, l_iBrowserHeight,
		l_iViewportWidth, l_iViewportHeight,
		l_iCanvasPosX, l_iCanvasPosY,
		l_iOpenGLViewportX, l_iOpenGLViewportY,
		windowWidth, windowHeight).c_str());
#endif
	cGameApp::RenderFont(0, 300, UT::ComposeMsgByFormat(L"FBSize:%d,:%d\nOpenGLViewport:%d,%d",
		fb_width, fb_height,
		l_iOpenGLViewportX, l_iOpenGLViewportY).c_str());
}

void cMyImGuiUIEditor::Render1ToolBox()
{
	ImGui::PushAllColorsDark(m_dark_style);
	int l_iBuilderWindowHeight = 250;
	auto width = 1280;
	ImGui::SetNextWindowSize({ static_cast<float>(width - 16) + 1600, (float)l_iBuilderWindowHeight });
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::Begin("BUILDER", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);
	if (ImGui::Button("New Form"))
	{
		//create_form();
	}
	ImGui::SameLine();
	if (ImGui::Button("New Child"))
	{
		//create_child();
	}
	ImGui::SameLine();
	if (ImGui::Button("New Button"))
	{
		//create_obj(eMyImGuiType::eMIGT_BUTTON);
	}
	ImGui::SameLine();
	if (ImGui::Button("New Label"))
	{
		//create_obj(eMyImGuiType::eMIGT_LABEL);
	}
	ImGui::SameLine();
	if (ImGui::Button("New Text"))
	{
		//create_obj(eMyImGuiType::eMIGT_EDIT);
	}
	ImGui::SameLine();

	if (ImGui::Button("New Slider Int"))
	{
		//create_obj(eMyImGuiType::eMIGT_SLIDER_I);
	}
	ImGui::SameLine();
	if (ImGui::Button("New Slider float"))
	{
		//create_obj(eMyImGuiType::eMIGT_SLIDER_F);
	}

	ImGui::SameLine();
	if (ImGui::Button("New CheckBox"))
	{
		//create_obj(eMyImGuiType::eMIGT_CHECKBOX);
	}
	ImGui::SameLine();
	if (ImGui::Button("New radio"))
	{
		//create_obj(eMyImGuiType::eMIGT_RADIO);
	}
	ImGui::SameLine();
	if (ImGui::Button("New toggle"))
	{
		//create_obj(eMyImGuiType::eMIGT_TOOGLE);
	}
	//object_property(l_iBuilderWindowHeight);
	ImGui::PopAllColorsCustom();

	ImGui::PushAllColorsCustom(m_custom_gui_style);

	//show_form();

	ImGui::PopAllColorsCustom();

	ImGui::End();
}

cMyImGuiUIEditor::cMyImGuiUIEditor()
{
	ImGui::StyleColorsDark(&m_dark_style);
	for (auto i = 0; i < ImGuiCol_COUNT; i++)
	{
		m_custom_gui_style.Colors[i] = m_dark_style.Colors[i];
	}
	m_pRoot = new cMyGuiForm();
}

cMyImGuiUIEditor::~cMyImGuiUIEditor()
{
	SAFE_DELETE(m_pRoot);
}

void cMyImGuiUIEditor::Render()
{
#ifdef WIN32
	ImGui_ImplWin32_NewFrame();
#elif defined(WASM)
	ImGui_ImplSDL2_NewFrame();
#endif
	ImGui::NewFrame();
	RenderMenu();
	Render1ToolBox();
	if (m_pRoot)
	{
		m_pRoot->Render();
	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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