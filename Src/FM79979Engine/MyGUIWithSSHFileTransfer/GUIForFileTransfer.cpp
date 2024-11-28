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
	ParseEnvData("Deploy.json");
	m_pRoot = new cMyGuiRootNode();
	ImVec2 l_vSize(cGameApp::m_spOpenGLRender->m_vGameResolution.x, cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	cMyGuiForm*l_pMyGuiForm = new cMyGuiForm();
	l_pMyGuiForm->SetFormFlag(ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);
	l_pMyGuiForm->SetOnlyApplyPositionOnceForDragMoving(true);
	auto l_ExtraFunction = std::bind(&cGUIForFileTransfer::RenderMenu, this,std::placeholders::_1);
	l_pMyGuiForm->SetExtraRenderFunction(l_ExtraFunction);
	m_pRoot->AddChild(l_pMyGuiForm);
	l_pMyGuiForm->SetSize(l_vSize);
	l_pMyGuiForm->SetLocalPosition(ImVec2(0, 0));

	m_pMyGuiListBox = new cMyGuiListBox();
	m_pMyGuiComboBox = new cMyGuiComboBox();
	m_pMyGuiButton = new cMyGuiButton();
	m_pMyGuiButton->SetText("MyButton\nQoo");
	//m_pMyGuiButton->SetEnable(false);
	//m_pMyGuiListBox->SetEnable(false);
	//m_pMyGuiComboBox->SetEnable(false);
	m_pMyGuiButton->m_fOnClickFunction = [this]
	()
	{
		cGameApp::ShowInfoOnScreen(L"Clicked");
		m_pRoot->ShowYesNoDialog([this]
		(bool e_bResult)
		{
			if (e_bResult)
			{
				m_pMyGuiButton->SetEnable(false);
				std::vector<eEnv> l_Vector = { eEnv::eE_DEV };
				DownloadFileOrDirectory(g_strRileFileName, g_strRileFileName, l_Vector, [this](std::string e_strResult)
				{
						m_pMyGuiButton->SetEnable(true);
						m_pRoot->ShowConfirmDialog("download finished");
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
	

	//cMyGuiNode*l_pMiddleNode = new cMyGuiNode();
	//l_pMiddleNode->SetLocalPosition(ImVec2(100, 100));
	{
		cMyGuiPanel* l_pMyGuiPanel = new cMyGuiPanel();
		l_pMyGuiPanel->SetBorder(false);
		l_pMyGuiPanel->SetLocalPosition(ImVec2(100, 100));
		l_pMyGuiPanel->SetSize(ImVec2(1920/2, 1080/2));
		//l_pMiddleNode->AddChild(l_pMyGuiPanel);
		l_pMyGuiPanel->AddChild(m_pMyGuiButton);
		l_pMyGuiPanel->AddChild(m_pMyGuiListBox);
		l_pMyGuiPanel->AddChild(m_pMyGuiComboBox);
		l_pMyGuiForm->AddChild(l_pMyGuiPanel);

		cMyGuiEditBox*l_pMyGuiEditBox = new cMyGuiEditBox();
		l_pMyGuiPanel->AddChild(l_pMyGuiEditBox);
		l_pMyGuiEditBox->SetMultiLines(true);
		l_pMyGuiEditBox->SetHint("");
		l_pMyGuiEditBox->SetLocalPosition(500, 100);
	}

	m_pMyGuiButton->SetLocalPosition(ImVec2(00, 100));
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
}

std::string cGUIForFileTransfer::GetEnvName(eEnv e_eEnv)
{
	if (e_eEnv == eE_DEV)
	{
		return "Dev";
	}
	else
	if (e_eEnv == eE_UAT)
	{
		return "Uat";
	}
	else
	if (e_eEnv == eE_SIT)
	{
		return "Sit";
	}
	else
	if (e_eEnv == eE_FUN)
	{
		return "Funplay";
	}
	else
	if (e_eEnv == eE_PROD)
	{
		return "Prod";
	}
	return std::string("unknown");
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

void cGUIForFileTransfer::RenderMenu(cImGuiNode* e_pNode)
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
}

void cGUIForFileTransfer::Init()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
	ImGui::GetIO().IniFilename = nullptr;
	ImGui::GetIO().LogFilename = nullptr;
}

void cGUIForFileTransfer::Update(float e_fElpaseTime)
{

}

void cGUIForFileTransfer::Render()
{
	ImGui_StartFrame();

	RenderMainUI();

	ImGui_EndFrame();
}

void cGUIForFileTransfer::Destory()
{
}