#include "stdafx.h"
#include "GUIForFileTransfer.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include "../imgui/MyGui/MyGui.h"
#include "SFTPTransfer.h"




const char* g_strEnvJSONKey_LocalSource = "LocalSource";
const char* g_strEnvJSONKey_BackupSource = "BackupSource";
const char* g_strEnvJSONKey_TargetSourceIP = "TargetIP";
const char* g_strEnvJSONKey_DeployEnvironmentData = "Deploy";
const char* g_strEnvJSONKey_SFTPDirectory = "SFTPDirectory";
const char* g_strEnvJSONKey_TargetIP = "TargetIP";
const char* g_strEnvJSONKey_UserName = "user";
const char* g_strEnvJSONKey_Password = "pw";
const char* g_strEnvJSONKey_Port = "port";
const char* g_strEnvJSONKey_TargetFolder = "TargetFolder";
const char* g_strEnvJSONKey_GoogleSheetName = "GoogleSheetName";
const char* g_strEnvJSONKey_VersionFileDirectory = "VersionFileDirectory";
const char* g_strEnvJSONKey_UploadGoogleSheetExeFileDirectory = "UploadGoogleSheetExeFileDirectory";


std::string	g_strLocalFolder;
std::string	g_strTargetFolder;
std::string	g_strGoogleSheetName;
std::string	g_strUploadGoogleSheetExeFileDirectory;
std::string	g_strVersionFileDirectory;


std::string	g_strPullCommand;
std::string	g_strPushCommand;

const char* g_strThisPullFileName = "ThisIsPull.bat";
const char* g_strThisPushFileName = "ThisIsCommitAndPush.bat";

namespace fs = std::filesystem;



const char* g_strRileFileName = "version/Rule.json";

cGUIForFileTransfer::cGUIForFileTransfer()
{
	m_pToolBoxRoot = nullptr;
	m_pTreeView = new cMyTreeView();
	GenerateToolBox();
	ParseEnvData("Deploy.json");
	m_pMainUIRoot = GetMyGuiObjWithType<cMyGuiRootNode>();
	ImVec2 l_vSize(1920,1080);
	m_pMyGuiForm = GetMyGuiObjWithType<cMyGuiForm>();
	m_pMyGuiForm->SetFormFlag(ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);
	m_pMyGuiForm->SetOnlyApplyPositionOnceForDragMoving(true);
	auto l_ExtraFunction = std::bind(&cGUIForFileTransfer::RenderMenu, this,std::placeholders::_1);
	//m_pMyGuiForm->SetExtraRenderFunction(l_ExtraFunction);
	m_pMainUIRoot->AddChild(m_pMyGuiForm);
	m_pMyGuiForm->SetSize(l_vSize);
	m_pMyGuiForm->SetLocalPosition(ImVec2(0, 0));

	m_pMyGuiListBox = GetMyGuiObjWithType<cMyGuiListBox>();
	m_pMyGuiComboBox = GetMyGuiObjWithType<cMyGuiComboBox>();
	m_pMyGuiButton = GetMyGuiObjWithType<cMyGuiButton>();
	m_pMyGuiButton->SetText("MyButton\nQoo\nQoo\nQoo\nQoo\nQoo\nQoo");
	cMyGuiEditBox* l_pMyGuiEditBox = GetMyGuiObjWithType<cMyGuiEditBox>();
	l_pMyGuiEditBox->SetMultiLines(true);
	l_pMyGuiEditBox->SetHint("");
	l_pMyGuiEditBox->SetLocalPosition(500, 100);
	//m_pMyGuiButton->SetEnable(false);
	//m_pMyGuiListBox->SetEnable(false);
	//m_pMyGuiComboBox->SetEnable(false);
	m_pMyGuiButton->m_fOnClickFunction = [this]
	(cMyGuiButton* e_pMyGuiButton)
	{
		//cGameApp::ShowInfoOnScreen(L"Clicked");
		m_pMainUIRoot->ShowYesNoDialog([this]
		(bool e_bResult)
		{
			if (e_bResult)
			{
				m_pMainUIRoot->ShowFullScreenBlackText("wait for download ");
				//m_pMyGuiButton->SetEnable(false);
				std::vector<eEnv> l_Vector = { eEnv::eE_DEV };
				DownloadFileOrDirectory(g_strRileFileName, g_strRileFileName, l_Vector, [this](std::string e_strResult)
				{
						m_pMainUIRoot->ShowFullScreenBlackText(nullptr);
						//m_pMyGuiButton->SetEnable(true);
						m_pMainUIRoot->ShowConfirmDialog(e_strResult.c_str());
				});
				
				//m_pMyGuiButton->SetVisible(e_bResult);
				
			}
		},"download Rule.json file?");
	};


	std::vector<std::string> l_strEnvNameVector;
	for (int i = eE_DEV; i < eE_MAX; ++i)
	{
		l_strEnvNameVector.push_back(GetEnvName((eEnv)i));
		
	}
	m_pMyGuiComboBox->SetDataVector(l_strEnvNameVector);
	m_pMyGuiComboBox->m_fOnSelectFunction = 
	[](int e_iIndex)
	{
			int a = 0;
	};

	m_pMyGuiListBox->SetDataVector(l_strEnvNameVector);
	m_pMyGuiListBox->m_fOnSelectFunction =
	[](int e_iIndex)
	{
		int a = 0;
	};

	{
		cMyGuiScroller*l_pMyGuiScroller = GetMyGuiObjWithType<cMyGuiScroller>();
		l_pMyGuiScroller->SetLocalPosition(ImVec2(600, 300));
		cMyGuiPanel* l_pMyGuiPanel = GetMyGuiObjWithType<cMyGuiPanel>();
		l_pMyGuiPanel->SetBorder(false);
		l_pMyGuiPanel->SetLocalPosition(ImVec2(0, 0));
		l_pMyGuiPanel->SetSize(ImVec2(1920/2, 1080/2));
		//l_pMiddleNode->AddChild(l_pMyGuiPanel);
		m_pMyGuiButton->SetName(L"1");
		m_pMyGuiListBox->SetName(L"2");
		m_pMyGuiComboBox->SetName(L"3");
		l_pMyGuiEditBox->SetName(L"4");
		l_pMyGuiPanel->AddChild(l_pMyGuiScroller);
		l_pMyGuiPanel->AddChild(m_pMyGuiButton);
		l_pMyGuiPanel->AddChild(m_pMyGuiListBox);
		l_pMyGuiPanel->AddChild(m_pMyGuiComboBox);
		l_pMyGuiPanel->AddChild(l_pMyGuiEditBox);
		m_pMyGuiForm->AddChild(l_pMyGuiPanel);
	}

	m_pMyGuiButton->SetLocalPosition(ImVec2(100, 100));
	m_pMyGuiComboBox->SetLocalPosition(ImVec2(100, 200));
	m_pMyGuiListBox->SetLocalPosition(ImVec2(200, 300));
	// l_pMyGuiForm->AddChild(l_pMiddleNode);

	
	//l_pMyGuiForm->AddChild(m_pMyGuiButton);
	//l_pMyGuiForm->AddChild(m_pMyGuiListBox);
	//l_pMyGuiForm->AddChild(m_pMyGuiComboBox);
	//m_pMyGuiButton->SetSize(ImVec2(600,500));
	
}

cGUIForFileTransfer::~cGUIForFileTransfer()
{
	SAFE_DELETE(m_pMainUIRoot);
	SAFE_DELETE(m_pToolBoxRoot);
	ImGui_ImplOpenGL3_Shutdown();
}

void cGUIForFileTransfer::GenerateRenderData()
{

}

void cGUIForFileTransfer::GenerateToolBox()
{
	if(!m_pToolBoxRoot)
	{
		m_pToolBoxRoot = GetMyGuiObjWithType<cMyGuiBasicObj>();
		m_pToolBoxRoot->SetNotApplyPosition(false);
		for (int l_eMyImGuiType = eMyImGuiType::eMIGT_NODE; l_eMyImGuiType<eMyImGuiType::eMIGT_MAX;++l_eMyImGuiType)
		{
			printf("%d\n", l_eMyImGuiType);
			if (l_eMyImGuiType == eMyImGuiType::eMIGT_NODE || l_eMyImGuiType == eMyImGuiType::eMIGT_FORM || 
				l_eMyImGuiType == eMyImGuiType::eMIGT_ROOT_NODE)
			{
				continue;
			}
			auto l_pMyGuiButton = GetMyGuiObjWithType<cMyGuiButton>();
			l_pMyGuiButton->SetText(GetMyGuiObjLabel((eMyImGuiType)l_eMyImGuiType));
			l_pMyGuiButton->m_fOnClickFunction = [this,l_eMyImGuiType](cMyGuiButton*e_pMyGuiButton)
			{
					cMyGuiBasicObj*l_pObject = GetMyGuiObj((eMyImGuiType)l_eMyImGuiType);
					l_pObject->SetLocalPosition(ImVec2(200, 200));
					m_pMyGuiForm->AddChild(l_pObject);
					
			};
			m_pToolBoxRoot->AddChild(l_pMyGuiButton);
			l_pMyGuiButton->SetNotApplyPosition(false);
		}

	}
}

void cGUIForFileTransfer::FetchVersionFileList()
{
}
//{
//	"LocalSource": "..\\Info",
//	"TargetFolder" : "system/Info",
//	"GoogleSheetName" : "InfoPage",
//	"UploadGoogleSheetExeFileDirectory" : "..\\..\\DeployTools\\",
//	"VersionFileDirectory" : "../",
//	"Dev" : {
//	"BackupSource": "\\\\192.168.250.71\\client\\games-DEV1",
//	"Deploy" : {
//	"SFTPDirectory": "/usr/share/nginx/html/games",
//		"TargetIP" : "192.168.250.47",
//		"user" : "root",
//		"pw" : "",
//		"port" : "22"
//	}
//	}
//}

sEnvData::sEnvData(json e_json)
{
	m_strBackupDirectory = e_json[g_strEnvJSONKey_BackupSource];
	auto l_Deploy = e_json[g_strEnvJSONKey_DeployEnvironmentData];
	m_strTargetIP = l_Deploy[g_strEnvJSONKey_TargetSourceIP];
	m_strRemoteUserName = l_Deploy[g_strEnvJSONKey_UserName];
	m_strRemotePassword = l_Deploy[g_strEnvJSONKey_Password];
	m_strRemoteDirectory = l_Deploy[g_strEnvJSONKey_SFTPDirectory];
	std::string l_strPort = l_Deploy[g_strEnvJSONKey_Port];
	m_iPort = GetInt(l_strPort);
	
}

void cGUIForFileTransfer::ParseEnvData(const char* e_strFileName)
{
	if (fs::exists("Deploy.json"))
	{
		std::ifstream l_JsonStream("Deploy.json");
		json l_JsonData = json::parse(l_JsonStream);
		l_JsonStream.close();

		g_strLocalFolder = l_JsonData[g_strEnvJSONKey_LocalSource];
		g_strTargetFolder = l_JsonData[g_strEnvJSONKey_TargetFolder];
		g_strGoogleSheetName = l_JsonData[g_strEnvJSONKey_GoogleSheetName];
		g_strUploadGoogleSheetExeFileDirectory = l_JsonData[g_strEnvJSONKey_UploadGoogleSheetExeFileDirectory];
		g_strVersionFileDirectory = l_JsonData[g_strEnvJSONKey_VersionFileDirectory];
		for (int i = eE_DEV; i < eE_MAX; ++i)
		{

			auto l_Json = l_JsonData[GetEnvName((eEnv)i)];
			if (l_Json.is_object())
			{
				sEnvData l_EnvData(l_Json);
				this->m_EnvDataMap[(eEnv)i] = l_EnvData;
				AssignEnvData((eEnv)i, l_EnvData);
			}
		}
	}
	else
	{
		UT::ErrorMsg("Deploy.json not exists","Error");
	}
}

void cGUIForFileTransfer::RenderMainUI()
{
	//ImVec2 l_vSize(cGameApp::m_spOpenGLRender->m_vGameResolution.x, cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	//ImGui::SetNextWindowSize(l_vSize);
	//ImGui::SetNextWindowPos({ 100, 100 });
	//ImGui::Begin("BUILDER", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);
	//RenderMenu();
	if (this->m_pMainUIRoot)
	{
		m_pMainUIRoot->Render();
	}
}

void HelpMarker(const char* desc)
{
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.92f, 0.92f, 0.92f, 1.00f));
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	ImGui::PopStyleColor(1);
}

void cGUIForFileTransfer::RenderToolBox()
{
	static bool l_bDoOnce = true;
	const int l_iWidth = 200;
	auto l_MousePos = this->m_p2DCamera->GetMouseWorldPos();
	ImGui::SetNextWindowPos(ImVec2(0,0));
	ImGui::SetNextWindowSizeConstraints(ImVec2(l_iWidth, 100), ImVec2(FLT_MAX, 500));
	if (l_bDoOnce)
	{
		ImGui::SetNextWindowSize(ImVec2(l_iWidth, 1080));
	}
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.00f, 5.00f));
	ImGui::Begin("Sidebar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize| ImGuiWindowFlags_MenuBar);
	auto l_bCollided = m_pTreeView->IsCollided(l_MousePos.x, l_MousePos.y);
	if (l_bCollided|| CheckMouseAndCurrentWindowCollision())
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
		ImGui::Text("Common"); HelpMarker
		("common component");
		ImGui::Separator();
		if (m_pToolBoxRoot)
		{
			m_pToolBoxRoot->Render();
		}
	}
	
	if (l_bDoOnce)
	{
		ImGui::SetNextWindowPos(ImVec2(1680, 0));
		//ImGui::SetNextWindowSize(ImVec2(l_iWidth, 500));
		ImGui::SetNextWindowSizeConstraints(ImVec2(l_iWidth+50, 900), ImVec2(FLT_MAX,1080));
		l_bDoOnce = false;
	}
	int l_iRenderFlag = ImGuiWindowFlags_NoTitleBar;
	//ShowTreeViewWindow(this->m_pRoot->GetChildNodeVector()[0], l_iRenderFlag);
	m_pTreeView->m_pRoot = this->m_pMainUIRoot->GetChildNodeVector()[0];
	m_pTreeView->Render();
	ImGui::End();
	ImGui::PopStyleVar(1);
}

void cGUIForFileTransfer::RenderMenu(class cImGuiNode*e_pImGuiNode)
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

void cGUIForFileTransfer::RenderPopupMenuContext()
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

void cGUIForFileTransfer::RenderTreeivewPopupMenuContext()
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

void cGUIForFileTransfer::Init()
{
	ImGui_ImplOpenGL3_Init(cGameApp::m_spOpenGLRender->m_Handle, nullptr, 2);
	m_pToolBoxGuiContext = ImGui::CreateContext();
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
}

void cGUIForFileTransfer::Update(float e_fElpaseTime)
{

}

void cGUIForFileTransfer::Render(float* e_pfMatrix, float* e_pfGameResolutoinSize)
{
	float l_fTargetGameResolution[2] = { 1920.f, 1080.f };
	ImGui_StartFrame(e_pfGameResolutoinSize);
	//RenderToolBox();
	RenderMainUI();
	ImGui_EndFrame(e_pfMatrix, e_pfGameResolutoinSize);
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
		auto l_pItem = this->m_pMainUIRoot->Collided(l_vPos.x, l_vPos.y);
		this->m_pMainUIRoot->DebugRender();
		auto l_strExtraInfo = UT::ComposeMsgByFormat(L"%d,%d", (int)l_vPos.x, (int)l_vPos.y);
		l_vPos.y -= 50;
		l_vPos.x -= 50;
		cGameApp::RenderFont(l_vPos, l_strExtraInfo.c_str());
	}
	//cMatrix44	l_matProjection;
	//glhOrthof2((float*)l_matProjection.m, 0, 1920, 1080, 0, -10000, 10000);
	//ImGui_StartFrame();

	ImGui_StartFrame(l_fTargetGameResolution,1);
	RenderToolBox();
	ImGui_EndFrame();
}

void cGUIForFileTransfer::Destory()
{
}



// Draw a 9-sliced texture
void DrawNineSlicedTexture(ImTextureID texture, ImVec2 pos, ImVec2 size, ImVec4 borders, ImVec2 uv_min, ImVec2 uv_max)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	// Texture size in pixels
	float tex_width = size.x;
	float tex_height = size.y;

	// Border sizes
	float left = borders.x;
	float top = borders.y;
	float right = borders.z;
	float bottom = borders.w;

	// Define UVs for each region
	ImVec2 uv_tl = ImVec2(uv_min.x, uv_min.y);                       // Top-left
	ImVec2 uv_tr = ImVec2(uv_max.x, uv_min.y);                       // Top-right
	ImVec2 uv_bl = ImVec2(uv_min.x, uv_max.y);                       // Bottom-left
	ImVec2 uv_br = ImVec2(uv_max.x, uv_max.y);                       // Bottom-right

	ImVec2 uv_center_min = ImVec2(uv_min.x + left / tex_width, uv_min.y + top / tex_height);
	ImVec2 uv_center_max = ImVec2(uv_max.x - right / tex_width, uv_max.y - bottom / tex_height);

	// Draw 9 parts
	// Top-left
	draw_list->AddImage(texture, pos, ImVec2(pos.x + left, pos.y + top), uv_tl, uv_center_min);

	// Top-center
	draw_list->AddImage(texture, ImVec2(pos.x + left, pos.y), ImVec2(pos.x + size.x - right, pos.y + top),
						ImVec2(uv_center_min.x, uv_min.y), ImVec2(uv_center_max.x, uv_center_min.y));

	// Top-right
	draw_list->AddImage(texture, ImVec2(pos.x + size.x - right, pos.y), ImVec2(pos.x + size.x, pos.y + top),
						ImVec2(uv_center_max.x, uv_min.y), uv_tr);

	// Middle-left
	draw_list->AddImage(texture, ImVec2(pos.x, pos.y + top), ImVec2(pos.x + left, pos.y + size.y - bottom),
						ImVec2(uv_min.x, uv_center_min.y), ImVec2(uv_center_min.x, uv_center_max.y));

	// Center
	draw_list->AddImage(texture, ImVec2(pos.x + left, pos.y + top), ImVec2(pos.x + size.x - right, pos.y + size.y - bottom),
						uv_center_min, uv_center_max);

	// Middle-right
	draw_list->AddImage(texture, ImVec2(pos.x + size.x - right, pos.y + top), ImVec2(pos.x + size.x, pos.y + size.y - bottom),
						ImVec2(uv_center_max.x, uv_center_min.y), ImVec2(uv_max.x, uv_center_max.y));

	// Bottom-left
	draw_list->AddImage(texture, ImVec2(pos.x, pos.y + size.y - bottom), ImVec2(pos.x + left, pos.y + size.y),
						uv_bl, ImVec2(uv_center_min.x, uv_max.y));

	// Bottom-center
	draw_list->AddImage(texture, ImVec2(pos.x + left, pos.y + size.y - bottom), ImVec2(pos.x + size.x - right, pos.y + size.y),
						ImVec2(uv_center_min.x, uv_center_max.y), ImVec2(uv_center_max.x, uv_max.y));

	// Bottom-right
	draw_list->AddImage(texture, ImVec2(pos.x + size.x - right, pos.y + size.y - bottom), ImVec2(pos.x + size.x, pos.y + size.y),
						ImVec2(uv_center_max.x, uv_center_max.y), uv_br);
}
