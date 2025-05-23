#include "MyImGuiUIEditor.h"
#ifdef WIN32
#include "../../Core/AllCoreInclude.h"
#include "../ThirtyParty/FileDialog/ImGuiFileDialog.h"
#include "../ThirtyParty/ImGuiBuilder/additional.h"
#include "../ImGuiRender.h"
#include "../../../include/json.hpp"
#include "MyGUIUtilities.h"

void cMyImGuiUIEditor::OpenSaveFileDialog()
{
	IGFD::FileDialogConfig config;
	config.path = ".";
	ImGuiFileDialog::Instance()->OpenDialog("SaveFile", "Choose File", ".json", config);
	EditboxFocusChangedChangeMouseEnable(true);
}

void cMyImGuiUIEditor::OpenOpenFileDialog()
{
	IGFD::FileDialogConfig config;
	config.path = ".";
	ImGuiFileDialog::Instance()->OpenDialog("OpenFile", "Choose File", ".json", config);
	EditboxFocusChangedChangeMouseEnable(true);
}

cMyImGuiUIEditor::cMyImGuiUIEditor()
{
	m_p2DCamera = new cOrthogonalCamera();
	m_pToolBoxRoot = nullptr;
	cMyGuiEditBox::m_bEditboxDisableInputBecauseIAmLazyToHandlMouseFocus = true;
}

cMyImGuiUIEditor::~cMyImGuiUIEditor()
{
	SAFE_DELETE(m_p2DCamera);
	SAFE_DELETE(m_pMainUIRoot);
	SAFE_DELETE(m_pToolBoxRoot);
	SAFE_DELETE(m_pTreeView);
	ImGui_ImplOpenGL3_Shutdown();
}

void cMyImGuiUIEditor::Init()
{
	if (m_p2DCamera)
	{
		RECT clientRect;
		if (GetClientRect((HWND)cGameApp::m_spOpenGLRender->m_Handle, &clientRect))
		{

			m_p2DCamera->SetCameraPos(Vector2(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top) / 2);
		}
		else
		{
			m_p2DCamera->SetCameraPos(Vector2(960, 544));
		}

		//m_p2DCamera->SetScale(1.25);
	}
	ImGui_ImplOpenGL3_Init(cGameApp::m_spOpenGLRender->m_Handle, nullptr, 2);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
	ImGui::GetIO().IniFilename = nullptr;
	ImGui::GetIO().LogFilename = nullptr;
	ImGui::GetIO().FontGlobalScale = 1.5f;
	if (m_p2DCamera)
	{
		auto l_ImGuiCameraPositionConvertFunction = std::bind(&cOrthogonalCamera::GetGLSciccorRect, m_p2DCamera, std::placeholders::_1);
		auto l_ImGuiGetCameraCursorPosition = [this](long& e_PosX, long& e_PosY)
			{
				auto l_vPos = m_p2DCamera->GetMouseWorldPos();
				e_PosX = (long)l_vPos.x;
				e_PosY = (long)l_vPos.y;
			};
		SetImGuiGetCameraCursorPosition(l_ImGuiGetCameraCursorPosition, m_iRootNodeRenderContextIndex);
		SetImGuiCameraPositionConvertFunction(l_ImGuiCameraPositionConvertFunction, m_iRootNodeRenderContextIndex);
	}
	if (!m_pTreeView)
	{
		m_pTreeView = new cMyTreeView();
		m_pMainUIRoot = GetMyGuiObjWithType<cMyGuiRootNode>();
		ImVec2 l_vSize(1920, 1080);
		m_pMyGuiForm = (cMyGuiForm*)GetMyGuiObj(eMIGT_FORM);
		m_pMyGuiForm->SetFormFlag(ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
		//m_pMyGuiForm->SetFormFlag(ImGuiWindowFlags_NoDecoration);
		//m_pMyGuiForm->SetOnlyApplyPositionOnceForDragMoving(true);
		m_pMainUIRoot->AddChild(m_pMyGuiForm);
		m_pMyGuiForm->SetSize(l_vSize);
		m_pMyGuiForm->SetLocalPosition(ImVec2(0, 0));
		GenerateToolBox();
		ImGui::StyleColorsDark(&m_dark_style);
		//ImGui::SetStyle(m_dark_style);
		//ImGui::StyleColorsLight(&m_dark_style);
		//IMGUI_API void          StyleColorsDark(ImGuiStyle * dst = NULL);    // new, recommended style (default)
		//IMGUI_API void          StyleColorsLight(ImGuiStyle * dst = NULL);   // best used with borders and a custom, thicker font
		//IMGUI_API void          StyleColorsClassic(ImGuiStyle * dst = NULL); // classic imgui style
		ImGuiStyle* style = &ImGui::GetStyle();
		*style = m_dark_style;
		for (auto i = 0; i < ImGuiCol_COUNT; i++)
		{
			m_custom_gui_style.Colors[i] = m_dark_style.Colors[i];
		}
	}
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
				OpenSaveFileDialog();
			}

			if (ImGui::MenuItem("Open"))
			{
				OpenOpenFileDialog();
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
	int	l_iOpenGLViewportX = (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Width();
	int	l_iOpenGLViewportY = (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Height();
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

//void cMyImGuiUIEditor::Render1ToolBox()
//{
//	int l_iBuilderWindowHeight = 250;
//	auto width = 1280;
//	ImGui::SetNextWindowSize({ static_cast<float>(width - 16) + 1600, (float)l_iBuilderWindowHeight });
//	ImGui::SetNextWindowPos({ 0, 0 });
//	ImGui::Begin("BUILDER", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);
//	if (ImGui::Button("New Form"))
//	{
//		//create_form();
//	}
//	ImGui::SameLine();
//	if (ImGui::Button("New Child"))
//	{
//		//create_child();
//	}
//	ImGui::SameLine();
//	if (ImGui::Button("New Button"))
//	{
//		//create_obj(eMyImGuiType::eMIGT_BUTTON);
//	}
//	ImGui::SameLine();
//	if (ImGui::Button("New Label"))
//	{
//		//create_obj(eMyImGuiType::eMIGT_LABEL);
//	}
//	ImGui::SameLine();
//	if (ImGui::Button("New Text"))
//	{
//		//create_obj(eMyImGuiType::eMIGT_EDIT);
//	}
//	ImGui::SameLine();
//
//	if (ImGui::Button("New Slider Int"))
//	{
//		//create_obj(eMyImGuiType::eMIGT_SLIDER_I);
//	}
//	ImGui::SameLine();
//	if (ImGui::Button("New Slider float"))
//	{
//		//create_obj(eMyImGuiType::eMIGT_SLIDER_F);
//	}
//
//	ImGui::SameLine();
//	if (ImGui::Button("New CheckBox"))
//	{
//		//create_obj(eMyImGuiType::eMIGT_CHECKBOX);
//	}
//	ImGui::SameLine();
//	if (ImGui::Button("New radio"))
//	{
//		//create_obj(eMyImGuiType::eMIGT_RADIO);
//	}
//	ImGui::SameLine();
//	if (ImGui::Button("New toggle"))
//	{
//		//create_obj(eMyImGuiType::eMIGT_TOOGLE);
//	}
//	//object_property(l_iBuilderWindowHeight);
//	ImGui::PopAllColorsCustom();
//
//	ImGui::SetStyle(m_custom_gui_style);
//
//	//show_form();
//
//	ImGui::PopAllColorsCustom();
//
//	ImGui::End();
//}

void cMyImGuiUIEditor::Render()
{
	if (!m_p2DCamera)
	{
		return;
	}
	auto l_ViewPortSize = Vector2(cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), cGameApp::m_spOpenGLRender->m_vViewPortSize.Height());
	if (m_p2DCamera)
	{
		m_p2DCamera->SetResolution(l_ViewPortSize);
		m_p2DCamera->Render();
		m_p2DCamera->DrawGrid(0.f, 0.f, Vector4(0.5f, 1.f, 0.f, 0.3f), 2.f);
	}
	Vector2 l_vSize = m_p2DCamera->GetScreenViewPortSize();	
	float l_fTargetGameResolution[2] = { 1920.f, 1080.f };

	cGameApp::m_spOpenGLRender->m_vGameResolution.x = cGameApp::m_spOpenGLRender->m_vViewPortSize.Width();
	cGameApp::m_spOpenGLRender->m_vGameResolution.y = cGameApp::m_spOpenGLRender->m_vViewPortSize.Height();

	ImGui_StartFrame(l_fTargetGameResolution, m_iRootNodeRenderContextIndex);
	//ImGui::GetStyle() = m_dark_style;
	RenderMainUI();
	ImGui_EndFrame(m_p2DCamera->GetProjectionMatrix(), l_vSize);
	GLRender::RenderRectangle(l_fTargetGameResolution[0], l_fTargetGameResolution[1], cMatrix44::Identity, Vector4::Red);
	if (m_p2DCamera)
	{
		m_p2DCamera->Render(false, DEFAULT_SHADER);
		m_p2DCamera->DrawGridCoordinateInfo(-80.f, -30.f);
	}
	if (cGameApp::m_sucKeyData[17])
	{
		m_p2DCamera->Render();
		m_p2DCamera->DrawGrid(0.f, 0.f, Vector4(0.5f, 1.f, 0.f, 0.3f), 2.f);
		auto l_vPos = m_p2DCamera->GetMouseWorldPos();
		this->m_pMainUIRoot->Collided((int)l_vPos.x, (int)l_vPos.y);
		this->m_pMainUIRoot->DebugRender();
		auto l_strExtraInfo = UT::ComposeMsgByFormat(L"%d,%d", (int)l_vPos.x, (int)l_vPos.y);
		l_vPos.y -= 50;
		l_vPos.x -= 50;
		cGameApp::RenderFont(l_vPos, l_strExtraInfo.c_str());
	}
	ImGui_StartFrame(l_fTargetGameResolution, m_iToolboxRenderContextIndex);
	//ImGui::GetStyle() = m_dark_style;
	RenderToolBox();
	if (ImGuiFileDialog::Instance()->Display("OpenFile", 0, ImVec2(800, 800), ImVec2(1850, 1000)))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{ // action if OK
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			// action
			this->OpenFile(filePathName.c_str());

		}
		// close
		ImGuiFileDialog::Instance()->Close();
		AllImGuiMouseEnable();
	}
	if (ImGuiFileDialog::Instance()->Display("SaveFile", 0, ImVec2(800, 800), ImVec2(1850, 1000)))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{ // action if OK
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			// action
			this->SaveToFile(filePathName.c_str());
		}
		// close
		ImGuiFileDialog::Instance()->Close();
		AllImGuiMouseEnable();
	}
	ImGui_EndFrame();
}

void cMyImGuiUIEditor::Update(float e_fElpaeeTime)
{
	if (m_p2DCamera)
	{
		if (m_p2DCamera)
		{
			float l_fMoveSpeed = 1.f;
			short l_cMouseWhellDelta = 0;
			if (cGameApp::m_sucKeyData[17] || cGameApp::m_sucKeyData[229])
			{
				l_cMouseWhellDelta = cGameApp::m_sMouseWhellDelta;
				l_fMoveSpeed = 3.f;
			}
			Vector2	l_vViewPort(cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), cGameApp::m_spOpenGLRender->m_vViewPortSize.Height());
			m_p2DCamera->CameraUpdateByMouse(cGameApp::m_sbMouseClickStatus[0], cGameApp::m_sbMouseClickStatus[1], l_cMouseWhellDelta, cGameApp::m_sMousePosition.x, cGameApp::m_sMousePosition.y, l_vViewPort, l_fMoveSpeed);
		}
		m_p2DCamera->Update(e_fElpaeeTime);
#ifdef WIN32
		cGameApp::m_spOpenGLRender->m_vViewPortSize = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize;
#endif
		cGameApp::m_spOpenGLRender->m_vGameResolution.x = cGameApp::m_spOpenGLRender->m_vViewPortSize.Width();
		cGameApp::m_spOpenGLRender->m_vGameResolution.y = cGameApp::m_spOpenGLRender->m_vViewPortSize.Height();
		cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution((int)cGameApp::m_spOpenGLRender->m_vGameResolution.x, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.y, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.x, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	}
	if (cGameApp::m_sbMouseClickStatus[0])
	{
		auto l_vPos = m_p2DCamera->GetMouseWorldPos();
		if (this->m_pMainUIRoot)
		{
			if (m_pCollidedItem)
			{
				auto l_vPositionOffset = l_vPos- m_vMouseClickPos;
				auto l_vLocalPositionOffset = ImVec2(m_vObjectLocalPosition.x + l_vPositionOffset.x, m_vObjectLocalPosition.y + l_vPositionOffset.y);
				m_pCollidedItem->SetLocalPosition(l_vLocalPositionOffset);
			}
			else
			{
				m_pCollidedItem = this->m_pMainUIRoot->Collided((int)l_vPos.x, (int)l_vPos.y);
				if (m_pCollidedItem)
				{
					this->m_pTreeView->SetFocusNode(m_pCollidedItem);
					m_vMouseClickPos = l_vPos;
					m_vObjectLocalPosition = m_pCollidedItem->GetLocalPosition();
				}
			}
		}
	}
	else
	{
		m_pCollidedItem = nullptr;
	}
}
//cImageNode:
//{
//	Name:"",
//	LocalPositionL"",
//		Child:
//		[
//		]
//}


void cMyImGuiUIEditor::SaveToFile(const char* e_strFileName)
{
	if (this->m_pMainUIRoot)
	{
		this->m_pMainUIRoot->ExportJsonFile(e_strFileName);
	}
}

void cMyImGuiUIEditor::OpenFile(const char* e_strFileName)
{
	if (this->m_pMainUIRoot)
	{
		if (m_pTreeView)
		{
			m_pTreeView->SetSelectedNodeNull();
		}
		using json = nlohmann::json;
		std::ifstream l_JsonStream(e_strFileName);
		auto l_JsonData = json::parse(l_JsonStream);
		l_JsonStream.close();
		auto l_strKEy = l_JsonData.begin().key();
		SAFE_DELETE(this->m_pMainUIRoot);
		this->m_pMainUIRoot = (cMyGuiRootNode*)cImGuiNode::DoUnSerialize(l_JsonData);
		auto l_ChildVector =  this->m_pMainUIRoot->GetChildNodeVector();
		m_pMyGuiForm = (cMyGuiForm*)l_ChildVector[0];
	}
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

void cMyImGuiUIEditor::GenerateToolBox()
{
	if (!m_pToolBoxRoot)
	{
		m_pToolBoxRoot = GetMyGuiObjWithType<cMyGuiBasicObj>();
		m_pToolBoxRoot->SetNotApplyPosition(false);
		for (int l_eMyImGuiType = eMyImGuiType::eMIGT_NODE; l_eMyImGuiType < eMyImGuiType::eMIGT_MAX; ++l_eMyImGuiType)
		{
			printf("%d\n", l_eMyImGuiType);
			if (l_eMyImGuiType == eMyImGuiType::eMIGT_NODE || l_eMyImGuiType == eMyImGuiType::eMIGT_FORM ||
				l_eMyImGuiType == eMyImGuiType::eMIGT_ROOT_NODE)
			{
				continue;
			}
			auto l_pMyGuiButton = GetMyGuiObjWithType<cMyGuiButton>();
			l_pMyGuiButton->SetText(GetMyGuiObjLabel((eMyImGuiType)l_eMyImGuiType));
			l_pMyGuiButton->m_fOnClickFunction = [this, l_eMyImGuiType](cMyGuiButton* e_pMyGuiButton)
				{
					cMyGuiBasicObj* l_pObject = GetMyGuiObj((eMyImGuiType)l_eMyImGuiType);
					l_pObject->SetLocalPosition(ImVec2(200, 200));
					if (l_pObject->Type() == cMyGuiEditBox::TypeID)
					{
						l_pObject->SetEnable(false);
					}
					m_pMyGuiForm->AddChild(l_pObject);

				};
			m_pToolBoxRoot->AddChild(l_pMyGuiButton);
			l_pMyGuiButton->SetNotApplyPosition(false);
		}
	}
}

void cMyImGuiUIEditor::RenderMainUI()
{
	if (this->m_pMainUIRoot)
	{
		m_pMainUIRoot->Render();
	}
}

void cMyImGuiUIEditor::RenderToolBox()
{
	static bool l_bDoOnce = true;
	const int l_iWidth = 200;
	auto l_MousePos = this->m_p2DCamera->GetMouseWorldPos();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSizeConstraints(ImVec2(l_iWidth, 100), ImVec2(FLT_MAX, 500));
	if (l_bDoOnce)
	{
		ImGui::SetNextWindowSize(ImVec2(l_iWidth, 1080));
	}
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.00f, 5.00f));
	ImGui::Begin("Sidebar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar);
	auto l_bCollided = m_pTreeView->IsCollided((int)l_MousePos.x, (int)l_MousePos.y);
	if (l_bCollided || CheckMouseAndCurrentWindowCollision())
	{
		SetImGuiMouseEnable(false, m_iRootNodeRenderContextIndex);
	}
	else
	{
		SetImGuiMouseEnable(true, m_iRootNodeRenderContextIndex);
	}
	RenderPopupMenuContext();
	RenderMenu(nullptr);
	{
		//ANCHOR SIDEBAR.PRIMITIVES
		ImGui::Text("Common"); RenderHintLabel
		("common component");
		ImGui::Separator();
		if (m_pToolBoxRoot)
		{
			m_pToolBoxRoot->Render();
		}
	}

	if (l_bDoOnce)
	{
		//ImGui::SetNextWindowPos(ImVec2(1680, 0));
		////ImGui::SetNextWindowSize(ImVec2(l_iWidth, 500));
		//ImGui::SetNextWindowSizeConstraints(ImVec2(l_iWidth + 50, 900), ImVec2(FLT_MAX, 1080));
		l_bDoOnce = false;
		m_pTreeView->m_vPosition = ImVec2(0, 540);
		m_pTreeView->m_vSize = ImVec2(350, 450);
	}
	int l_iRenderFlag = ImGuiWindowFlags_NoTitleBar;
	//ShowTreeViewWindow(this->m_pMainUIRoot->GetChildNodeVector()[0], l_iRenderFlag);
	m_pTreeView->SerRootNode(this->m_pMainUIRoot->GetChildNodeVector()[0]);
	m_pTreeView->Render();
	if (m_pCollidedItem)
	{
		((cMyGuiBasicObj*)m_pCollidedItem)->RenderProperty();
		m_pTreeView->SetSelectedNodeNull();
	}
	else
	{
		cMyGuiBasicObj* l_pSelectedNode = (cMyGuiBasicObj*)m_pTreeView->GetSelectedNode();
		if (l_pSelectedNode)
		{
			l_pSelectedNode->RenderProperty();
		}
	}
	ImGui::End();
	ImGui::PopStyleVar(1);
}

void cMyImGuiUIEditor::RenderMenu(cImGuiNode*e_pNode)
{
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
		//if (ImGui::BeginMenu("Editor"))
		//{
		//	if (ImGui::MenuItem("Color"))
		//	{
		//		//m_color_menu = !m_color_menu;
		//	}

		//	if (ImGui::MenuItem("Style"))
		//	{
		//		//m_style_menu = !m_style_menu;
		//	}

		//	if (ImGui::MenuItem("Font"))
		//	{
		//		//m_font_menu = !m_font_menu;
		//	}

		//	ImGui::EndMenu();
		//}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("ShowInfo"))
			{
				cGameApp::m_sbDebugFunctionWorking = !cGameApp::m_sbDebugFunctionWorking;
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void cMyImGuiUIEditor::RenderPopupMenuContext()
{
	if (ImGui::BeginPopupContextWindow("bwcontextmenu"))
	{
		if (ImGui::BeginMenu("Add"))
		{
			if (ImGui::BeginMenu("Primitives"))
			{
				if (ImGui::MenuItem("Listbox"))
				{

				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Data Inputs"))
			{
				if (ImGui::MenuItem("Slider Angle"))
				{

				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Save"))
		{
			OpenSaveFileDialog();
		}
		if (ImGui::MenuItem("Open"))
		{
			OpenOpenFileDialog();
		}
		ImGui::EndPopup();
	}
}

void cMyImGuiUIEditor::RenderTreeivewPopupMenuContext()
{
	if (ImGui::BeginPopupContextWindow("bwcontextmenu"))
	{
		if (ImGui::BeginMenu("Add"))
		{
			if (ImGui::BeginMenu("Primitives"))
			{
				if (ImGui::MenuItem("Listbox"))
				{

				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Data Inputs"))
			{
				if (ImGui::MenuItem("Slider Angle"))
				{

				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndPopup();
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