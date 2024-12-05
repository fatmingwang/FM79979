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
	GenerateToolBox();
	ParseEnvData("Deploy.json");
	m_pRoot = GetMyGuiObjWithType<cMyGuiRootNode>();
	ImVec2 l_vSize(1920,1080);
	cMyGuiForm* l_pMyGuiForm = GetMyGuiObjWithType<cMyGuiForm>();
	l_pMyGuiForm->SetFormFlag(ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);
	l_pMyGuiForm->SetOnlyApplyPositionOnceForDragMoving(true);
	//auto l_ExtraFunction = std::bind(&cGUIForFileTransfer::RenderMenu, this,std::placeholders::_1);
	//l_pMyGuiForm->SetExtraRenderFunction(l_ExtraFunction);
	m_pRoot->AddChild(l_pMyGuiForm);
	l_pMyGuiForm->SetSize(l_vSize);
	l_pMyGuiForm->SetLocalPosition(ImVec2(0, 0));

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
	()
	{
		//cGameApp::ShowInfoOnScreen(L"Clicked");
		m_pRoot->ShowYesNoDialog([this]
		(bool e_bResult)
		{
			if (e_bResult)
			{
				m_pRoot->ShowFullScreenBlackText("wait for download ");
				//m_pMyGuiButton->SetEnable(false);
				std::vector<eEnv> l_Vector = { eEnv::eE_DEV };
				DownloadFileOrDirectory(g_strRileFileName, g_strRileFileName, l_Vector, [this](std::string e_strResult)
				{
						m_pRoot->ShowFullScreenBlackText(nullptr);
						//m_pMyGuiButton->SetEnable(true);
						m_pRoot->ShowConfirmDialog(e_strResult.c_str());
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
		l_pMyGuiForm->AddChild(l_pMyGuiPanel);	
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
	SAFE_DELETE(m_pRoot);
	SAFE_DELETE(m_pToolBoxRoot);
	ImGui_ImplOpenGL3_Shutdown();
}

void cGUIForFileTransfer::GenerateToolBox()
{
	if(!m_pToolBoxRoot)
	{
		m_pToolBoxRoot = GetMyGuiObjWithType<cMyGuiRootNode>();
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
	if (this->m_pRoot)
	{
		m_pRoot->Render();
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
	ImGui::SetNextWindowPos(ImVec2(0,0));
	ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(FLT_MAX, -1));
	ImGui::SetNextWindowSize(ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.00f, 5.00f));
	ImGui::Begin("Sidebar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize| ImGuiWindowFlags_MenuBar);
	RenderMenu();
	ImGui::PushClipRect(ImVec2(-99999, -99999), ImVec2(9999, 9999), false);
	/// content-sidebar
	{

		ImGui::TextDisabled("NOTE");
		ImGui::SameLine(); HelpMarker
		("THESE ARE NOT THE ONLY WIDGETS IMGUI PROVIDES!\n"
		 "You can find out more in the Dear ImGui Demo "
		 "(Tools > Demo Window) and imgui/imgui_demo.cpp");
		ImGui::Separator();

		//ANCHOR SIDEBAR.PRIMITIVES
		ImGui::Text("Primitives");
		ImGui::Separator();

		if (ImGui::Button("Window"))
		{
			//bw.state = true;
		}

		if (ImGui::Button("Button"))
		{
			//bw.create("button");
		}
			

		if (ImGui::Button("Radio Button"))
		{
			//bw.create("radio");
		}			

		if (ImGui::Button("Checkbox"))
		{
			//bw.create("checkbox");
		}
			

		if (ImGui::Button("Text"))
		{

		}
		//bw.create("text");

		if (ImGui::Button("Bullet"))
		{

		}
			//bw.create("bullet");

		if (ImGui::Button("Arrow"))
		{

		}
			//bw.create("arrow");

		if (ImGui::Button("Combo"))
		{

		}
			//bw.create("combo");

		if (ImGui::Button("Listbox"))
		{

		}
			//bw.create("listbox");

		ImGui::Separator();

		//ANCHOR SIDEBAR.DATAINPUTS
		ImGui::Text("Data Inputs");
		ImGui::Separator();

		if (ImGui::Button("Input Text"))
		{

		}
			//bw.create("textinput");

		if (ImGui::Button("Input Int"))
		{

		}
			//bw.create("inputint");

		ImGui::SameLine(); HelpMarker
		("You can apply arithmetic operators +,*,/ on numerical values.\n"
		 "  e.g. [ 100 ], input \'*2\', result becomes [ 200 ]\n"
		 "Use +- to subtract.");

		if (ImGui::Button("Input Float"))
		{

		}
			//bw.create("inputfloat");

		if (ImGui::Button("Input Double"))
		{

		}
			//bw.create("inputdouble");

		if (ImGui::Button("Input Scientific"))
		{

		}
			//bw.create("inputscientific");

		ImGui::SameLine(); HelpMarker
		("You can input value using the scientific notation,\n"
		 "  e.g. \"1e+8\" becomes \"100000000\".");

		if (ImGui::Button("Input Float3"))
		{

		}
			//bw.create("inputfloat3");

		if (ImGui::Button("Drag Int"))
		{

		}
			//bw.create("dragint");

		ImGui::SameLine(); HelpMarker
		("Click and drag to edit value.\n"
		 "Hold SHIFT/ALT for faster/slower edit.\n"
		 "Double-click or CTRL+click to input value.");

		if (ImGui::Button("Drag Int %"))
		{

		}
			//bw.create("dragint100");

		if (ImGui::Button("Drag Float"))
		{

		}
			//bw.create("dragfloat");

		if (ImGui::Button("Drag Float Small"))
		{

		}
			//bw.create("dragfloatsmall");

		if (ImGui::Button("Slider Int"))
		{

		}
			//bw.create("sliderint");

		ImGui::SameLine(); HelpMarker("CTRL+click to input value.");

		if (ImGui::Button("Slider Float"))
		{

		}
			//bw.create("sliderfloat");

		if (ImGui::Button("Slider Float Log"))
		{

		}
			//bw.create("sliderfloatlog");

		if (ImGui::Button("Slider Angle"))
		{

		}
			//bw.create("sliderangle");

		ImGui::Separator();

		ImGui::Text("Color Pickers");
		ImGui::Separator();

		if (ImGui::Button("Color 1"))
		{

		}
			//bw.create("color1");

		if (ImGui::Button("Color 2"))
		{

		}
			//bw.create("color2");

		if (ImGui::Button("Color 3"))
		{

		}
			//bw.create("color3");

		ImGui::Separator();

		//ANCHOR SIDEBAR.OTHERS
		ImGui::Text("Misc");
		ImGui::Separator();

		//if (bw.open_child)
		if (1)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 1.000f, 0.110f, 1.000f));
			ImGui::Button("BeginChild");
			ImGui::PopStyleColor(1);
		}
		else
		{
			if (ImGui::Button("BeginChild"))
			{
				//bw.create("child");
				//bw.open_child_id = bw.idgen;
				//bw.open_child = true;
			}
		}
		ImGui::SameLine(); HelpMarker
		("Green = Open (Ready to add items). Calling EndChild will close it, "
		 "and you can't add items to it unless you manually re-open it.");

		if (ImGui::Button("EndChild"))
		{
			//if ((bw.getbaseobj(bw.open_child_id)))
			if (1)
			{
				//bw.getobj(bw.open_child_id)->child.open = false;
				//bw.open_child_id = -1;
			}
			//bw.open_child = false;
		}

		ImGui::BeginDisabled(true);
		if (ImGui::Button("BeginGroup"))
		{
			//
		}
		ImGui::EndDisabled();
		ImGui::SameLine(); HelpMarker
		("Groups are not a feature of ImStudio, but you can probably use "
		 "a child (without borders) to reproduce similar behavior.");

		if (ImGui::Button("<< Same Line"))
		{
			//bw.create("sameline");
		}

		if (ImGui::Button("New Line"))
		{
			//bw.create("newline");
		}

		if (ImGui::Button("Separator"))
		{
			//bw.create("separator");
		}

		if (ImGui::Button("Progress Bar"))
		{
			//bw.create("progressbar");
		}
		ImGui::Separator();

		//ImGui::Checkbox("Static Mode", &bw.staticlayout);

		if ((ImGui::GetIO().KeyAlt) && (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F4))))
		{
			//state = false;
		}

	}
	ImGui::PopClipRect();
	ImGui::End();
	ImGui::PopStyleVar(1);
}

void cGUIForFileTransfer::RenderMenu()
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
}

void cGUIForFileTransfer::Update(float e_fElpaseTime)
{

}

void cGUIForFileTransfer::Render(float* e_pfMatrix, float* e_pfGameResolutoinSize)
{
	ImGui_StartFrame(e_pfGameResolutoinSize);
	//RenderToolBox();
	RenderMainUI();
	ImGui_EndFrame(e_pfMatrix, e_pfGameResolutoinSize);

	//cMatrix44	l_matProjection;
	float l_f[2] = { 1920.f, 1080.f };
	//glhOrthof2((float*)l_matProjection.m, 0, 1920, 1080, 0, -10000, 10000);
	//ImGui_StartFrame();
	ImGui_StartFrame(l_f,1);
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
