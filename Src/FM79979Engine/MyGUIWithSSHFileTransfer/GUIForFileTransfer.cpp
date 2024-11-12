#include "stdafx.h"
#include "GUIForFileTransfer.h"
#include "json.hpp"

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


cGUIForFileTransfer::cGUIForFileTransfer()
{
}

cGUIForFileTransfer::~cGUIForFileTransfer()
{
}

void cGUIForFileTransfer::FetchVersionFileList()
{
}

void cGUIForFileTransfer::ParseEnvData(const char* e_strFileName)
{


}

void cGUIForFileTransfer::RenderMainUI()
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

void cGUIForFileTransfer::Init()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
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
