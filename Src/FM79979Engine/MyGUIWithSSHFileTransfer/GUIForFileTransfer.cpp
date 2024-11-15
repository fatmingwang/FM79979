#include "stdafx.h"
#include "GUIForFileTransfer.h"
#include "json.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>
#include "../imgui/MyGui/MyGui.h"

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
std::string	g_strPullCommand;
std::string	g_strPushCommand;

const char* g_strThisPullFileName = "ThisIsPull.bat";
const char* g_strThisPushFileName = "ThisIsCommitAndPush.bat";

namespace fs = std::filesystem;
using json = nlohmann::json;

cGUIForFileTransfer::cGUIForFileTransfer()
{
	m_pRoot = new cMyGuiNode();
	ImVec2 l_vSize(cGameApp::m_spOpenGLRender->m_vGameResolution.x, cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	cMyGuiForm*l_pMyGuiForm = new cMyGuiForm();
	l_pMyGuiForm->SetFormFlag(ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);
	l_pMyGuiForm->SetOnlyApplyPositionOnceForDragMoving(true);


	auto l_ExtraFunction = std::bind(&cGUIForFileTransfer::RenderMenu, this,std::placeholders::_1);

	l_pMyGuiForm->SetExtraRenderFunction(l_ExtraFunction);
	m_pRoot->AddChild(l_pMyGuiForm);
	l_pMyGuiForm->SetSize(l_vSize);
	l_pMyGuiForm->SetLocalPosition(ImVec2(100, 100));
	//m_pRoot->
	m_pMyGuiListBox = new cMyGuiListBox();
	m_pMyGuiComboBox = new cMyGuiComboBox();
	m_pMyGuiButton = new cMyGuiButton();




	m_pMyGuiButton->SetLocalPosition(ImVec2(100, 100));
	m_pMyGuiListBox->SetLocalPosition(ImVec2(100, 300));
	m_pMyGuiComboBox->SetLocalPosition(ImVec2(200, 600));
	l_pMyGuiForm->AddChild(m_pMyGuiButton);
	l_pMyGuiForm->AddChild(m_pMyGuiListBox);
	l_pMyGuiForm->AddChild(m_pMyGuiComboBox);
	//m_pMyGuiButton->SetSize(ImVec2(600,500));
	
}

cGUIForFileTransfer::~cGUIForFileTransfer()
{
}

void cGUIForFileTransfer::FetchVersionFileList()
{
}

void cGUIForFileTransfer::ParseEnvData(const char* e_strFileName)
{
	std::ifstream l_Json("Deploy.json");
	json l_JsonData = json::parse(l_Json);
	g_strLocalFolder = l_JsonData[g_strEnvJSONKey_LocalSource];
	l_Json.close();
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
