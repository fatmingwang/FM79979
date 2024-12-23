#include "stdafx.h"
#include "GUIForFileTransfer.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include "../imgui/MyGui/MyGui.h"
#include "../imgui/MyGui/MyGUIUtilities.h"
#include "SFTPTransfer.h"
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
std::string	g_strTargetFolder;
std::string	g_strGoogleSheetName;
std::string	g_strUploadGoogleSheetExeFileDirectory;
std::string	g_strVersionFileDirectory;


std::string	g_strPullCommand;
std::string	g_strPushCommand;

const char* g_strThisPullFileName = "ThisIsPull.bat";
const char* g_strThisPushFileName = "ThisIsCommitAndPush.bat";

namespace fs = std::filesystem;



const char* g_strRuleFileName = "version/Rule.json";


bool isNumber(const std::string& str, int* e_pOutValue)
{
	try
	{
		auto l_Value = std::stoi(str); // Try to convert to integer
		if (e_pOutValue)
		{
			*e_pOutValue = l_Value;
		}
		return true;
	}
	catch (const std::invalid_argument& e)
	{
		printf(e.what());
		return false; // Not a valid number
	}
	catch (const std::out_of_range& e)
	{
		printf(e.what());
		return false; // Number out of range
	}
}

cGUIForFileTransfer::cGUIForFileTransfer()
{
	ParseRuleFile(g_strRuleFileName);
	ParseEnvData("Deploy.json");
	m_pMainUIRoot = GetMyGuiObjWithType<cMyGuiRootNode>();
	ImVec2 l_vSize(1920,1080);
	m_pMyGuiForm = GetMyGuiObjWithType<cMyGuiForm>();
	//m_pMyGuiForm->SetFormFlag(ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);
	m_pMyGuiForm->SetFormFlag(ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse| ImGuiWindowFlags_NoTitleBar);
	m_pMyGuiForm->SetShowCloseCutton(false);
	m_pMyGuiForm->m_fFormCloseFunction = [](cMyGuiForm* e_pMyGuiForm)
	{
			int a = 0;
	};
	m_pMyGuiForm->SetOnlyApplyPositionOnceForDragMoving(true);
	m_pMainUIRoot->AddChild(m_pMyGuiForm);
	m_pMyGuiForm->SetSize(l_vSize);
	m_pMyGuiForm->SetLocalPosition(ImVec2(0, 0));

	cMyGuiBasicObj*l_pMyGuiBasicObj = GetMyGuiObjWithType<cMyGuiBasicObj>();
	l_pMyGuiBasicObj->SetExtraRenderFunction([this]
	(cImGuiNode* e_pNode)
	{
		NumericUpDown("Numeric Up/Down", &m_iAllGameSharedCodeVersion, 0,100,1);
		//ImGui::DragInt("Drag Int", &value, 1.0f, -100, 100);
	});
	auto l_pJsonContentCheckButton = GetMyGuiObjWithType<cMyGuiButton>();
	l_pJsonContentCheckButton->SetText("Check Json Content");
	m_pUploadRuleFileButton = GetMyGuiObjWithType<cMyGuiButton>();
	m_pTargetEnvListBox = GetMyGuiObjWithType<cMyGuiListBox>();
	m_pVersionListBox = GetMyGuiObjWithType<cMyGuiListBox>();
	m_pSourceEnvComboBox = GetMyGuiObjWithType<cMyGuiComboBox>();
	m_pFetchRuleFileButton = GetMyGuiObjWithType<cMyGuiButton>();
	m_pFetchRuleFileButton->SetText("Fetch \nRule.json");
	m_pUploadRuleFileButton->SetText("Upload");
	m_pRuleJsonContentEditbox = GetMyGuiObjWithType<cMyGuiEditBox>();
	m_pRuleJsonContentEditbox->SetMultiLines(true);
	m_pRuleJsonContentEditbox->SetHint("");
	m_pRuleJsonContentEditbox->SetSize(ImVec2(1000, 600));


	l_pJsonContentCheckButton->m_fOnClickFunction = [this]
	(cMyGuiButton* e_pMyGuiButton)
	{
			if (CheckEditboxContentJson())
			{

			}
	};

	m_pUploadRuleFileButton->m_fOnClickFunction = [this]
	(cMyGuiButton* e_pMyGuiButton)
	{
		m_pMainUIRoot->ShowYesNoDialog([this]
		(bool e_bResult)
		{
			if (e_bResult)
			{
				const char* l_strTempFile = "version/myRuleTemp.json";
				m_pMainUIRoot->ShowFullScreenBlackText("wait for upload ");
				m_pUploadRuleFileButton->SetEnable(false);
				std::vector<eEnv> l_Vector = { eEnv::eE_DEV };
				if (CheckEditboxContentJson())
				{
					SaveStringToFile(l_strTempFile, m_pRuleJsonContentEditbox->GetText().c_str());
					//UT::SaveTxtToFile("version/myRuleTemp.json", m_pRuleJsonContentEditbox->GetText().c_str());
					auto l_strContent = this->m_pRuleJsonContentEditbox->GetText();
					//UploadFileOrDirectory(l_strTempFile,g_strRuleFileName, l_Vector, [this](std::string e_strResult)
					UploadFileOrDirectory(l_strTempFile, l_strTempFile, l_Vector, [this](std::string e_strResult)
										  //DownloadFileOrDirectory(g_strRuleFileName, g_strRuleFileName, l_Vector, [this](std::string e_strResult)
										  {
											  m_pMainUIRoot->ShowFullScreenBlackText(nullptr);
											  m_pUploadRuleFileButton->SetEnable(true);
											  m_pMainUIRoot->ShowConfirmDialog(e_strResult.c_str());
										  });
				}
				else
				{

				}
			}
		},"upload local Rule.json file?");
	};

	m_pFetchRuleFileButton->m_fOnClickFunction = [this]
	(cMyGuiButton* e_pMyGuiButton)
	{
		m_pMainUIRoot->ShowYesNoDialog([this]
		(bool e_bResult)
		{
			if (e_bResult)
			{
				m_pMainUIRoot->ShowFullScreenBlackText("wait for download ");
				m_pFetchRuleFileButton->SetEnable(false);
				if(1)
				{
					DownloadRuleFileAndGetDirectoryList();
				}
				else
				{
					std::vector<eEnv> l_Vector = { eEnv::eE_DEV };
					ListRemoteDirectory(eEnv::eE_DEV, "", [this](std::vector<std::string>e_DirectoryVector, std::vector<std::string>e_FileVector)
					{
						std::vector<int>l_NumberVector;
						std::vector<std::string>l_strNumberVector;
						for (auto l_IT : e_DirectoryVector)
						{
							int l_iOutValue = 0;
							if (isNumber(l_IT, &l_iOutValue))
							{
								l_NumberVector.push_back(l_iOutValue);
							}
						}
						std::sort(l_NumberVector.begin(), l_NumberVector.end());
						for (const auto& num : l_NumberVector)
						{
							l_strNumberVector.push_back(std::to_string(num));
						}
						m_pVersionListBox->SetItemList(l_strNumberVector);
						m_pMainUIRoot->ShowFullScreenBlackText(nullptr);
						m_pFetchRuleFileButton->SetEnable(true);
						if (l_NumberVector.size())
						{
							m_pVersionListBox->SetImGuiName(L"VersionList");
						}
						else
						{
							m_pVersionListBox->SetImGuiName(L"No Env Version Data");
						}
						int a = 0;
					});

					/*DownloadFileOrDirectory(g_strRuleFileName, g_strRuleFileName, l_Vector, [this](std::string e_strResult)
					{
							m_pMainUIRoot->ShowFullScreenBlackText(nullptr);
							m_pFetchRuleFileButton->SetEnable(true);
							m_pMainUIRoot->ShowConfirmDialog(e_strResult.c_str());
							ParseRuleFile(g_strRuleFileName);
					});*/
				}				
			}
		},"download Rule.json file?");
	};


	std::vector<std::string> l_strEnvNameVector;
	for (int i = eE_DEV; i < eE_MAX; ++i)
	{
		l_strEnvNameVector.push_back(GetEnvName((eEnv)i));
		
	}
	m_pSourceEnvComboBox->SetItemList(l_strEnvNameVector);
	m_pSourceEnvComboBox->m_fOnSelectFunction =
	[](int e_iIndex)
	{
			int a = 0;
	};

	m_pTargetEnvListBox->SetItemList(l_strEnvNameVector);
	m_pTargetEnvListBox->SetMiltiSelecteable(false);
	m_pTargetEnvListBox->m_fOnSelectFunction =
	[](int e_iIndex)
	{
		int a = 0;
	};

	m_pVersionListBox->SetItemList({});



	m_pFetchRuleFileButton->SetImGuiName(L"FetchButton");
	m_pUploadRuleFileButton->SetImGuiName(L"UploadButton");
	m_pTargetEnvListBox->SetImGuiName(L"TargetEnv");
	m_pVersionListBox->SetImGuiName(L"No Env Version Data");
	m_pSourceEnvComboBox->SetImGuiName(L"SourceEnv");
	m_pRuleJsonContentEditbox->SetImGuiName(L"Rule Content");
	m_pMyGuiForm->AddChild(m_pFetchRuleFileButton);
	m_pMyGuiForm->AddChild(m_pTargetEnvListBox);
	m_pMyGuiForm->AddChild(m_pSourceEnvComboBox);
	m_pMyGuiForm->AddChild(m_pRuleJsonContentEditbox);
	m_pMyGuiForm->AddChild(m_pVersionListBox);
	m_pMyGuiForm->AddChild(m_pUploadRuleFileButton);
	m_pMyGuiForm->AddChild(l_pMyGuiBasicObj);
	m_pMyGuiForm->AddChild(l_pJsonContentCheckButton);
	
	l_pJsonContentCheckButton->SetLocalPosition(ImVec2(1400, 400));
	m_pFetchRuleFileButton->SetLocalPosition(ImVec2(100, 200));
	m_pSourceEnvComboBox->SetLocalPosition(ImVec2(100, 100));
	m_pTargetEnvListBox->SetLocalPosition(ImVec2(100, 300));
	m_pVersionListBox->SetLocalPosition(ImVec2(500, 100));
	m_pRuleJsonContentEditbox->SetLocalPosition(350, 400);
	m_pUploadRuleFileButton->SetLocalPosition(100, 600);
	l_pMyGuiBasicObj->SetLocalPosition(300, 20);
	
}

cGUIForFileTransfer::~cGUIForFileTransfer()
{
	SAFE_DELETE(m_pMainUIRoot);
	ImGui_ImplOpenGL3_Shutdown();
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

void cGUIForFileTransfer::ParseRuleFile(const char* e_strFileName)
{
	if (fs::exists(e_strFileName))
	{
		std::ifstream l_JsonStream(e_strFileName);
		json l_JsonData = json::parse(l_JsonStream);
		l_JsonStream.close();
		std::string l_LatestVersion = l_JsonData["LatestVersion"];
		auto l_SpecialGameRule = l_JsonData["SpecialGameRule"];

		//auto specialGameRule = jsonData["SpecialGameRule"];

		// Iterate through the key-value pairs in "SpecialGameRule"
		std::vector<std::string>	l_strVersionVector;
		for (const auto& [key, value] : l_SpecialGameRule.items())
		{
			if (isNumber(key,nullptr))
			{
				std::string l_strkey = key;
				std::string l_strvalue = value;
				l_strVersionVector.push_back(l_strvalue);
			}
			else
			{
				std::string l_strkey = key;
				std::string l_strvalue = value;
				std::cout << "Key: " << key << ", Value: " << value << std::endl;
			}
		}
		isNumber(l_LatestVersion.c_str(), &m_iAllGameSharedCodeVersion);
		if (m_pVersionListBox)
		{
			//m_pVersionListBox->SetItemList(l_strVersionVector);
		}
	}
}

void cGUIForFileTransfer::DownloadRuleFileAndGetDirectoryList()
{
	eEnv l_eEnv = (eEnv)m_pSourceEnvComboBox->GetSelectedIndex();
	sLIBSSH2SocketData* l_pLIBSSH2SocketData = sLIBSSH2SocketData::GetDataByEnv(l_eEnv);
	if (l_pLIBSSH2SocketData)
	{
		std::function<void()> l_Function = [this, l_pLIBSSH2SocketData]()
		{
			m_pMainUIRoot->ShowFullScreenBlackText("connecting...");
			if (l_pLIBSSH2SocketData->DoConnect())
			{
				m_pMainUIRoot->ShowFullScreenBlackText("try do download rule file");
				if (l_pLIBSSH2SocketData->DoDownloadFile(g_strRuleFileName, g_strRuleFileName))
				{
					m_pMainUIRoot->ShowFullScreenBlackText("try do download version list");
					std::string l_strRemotDir = "";
					std::vector<std::string> l_FilesVector;
					std::vector<std::string> l_DirectoriesVector;
					if (l_pLIBSSH2SocketData->DoListRemoteDirectory(l_strRemotDir, l_FilesVector, l_DirectoriesVector))
					{
						m_pUploadRuleFileButton->SetEnable(true);
						m_pMainUIRoot->ShowConfirmDialog("data updated");

						std::vector<std::string>l_strNumberVector;
						std::vector<int>l_NumberVector;
						for (auto l_IT : l_DirectoriesVector)
						{
							int l_iOutValue = 0;
							if (isNumber(l_IT, &l_iOutValue))
							{
								l_NumberVector.push_back(l_iOutValue);
							}
						}
						std::sort(l_NumberVector.begin(), l_NumberVector.end());
						for (const auto& num : l_NumberVector)
						{
							l_strNumberVector.push_back(std::to_string(num));
						}
						m_pVersionListBox->SetItemList(l_strNumberVector);
						m_pFetchRuleFileButton->SetEnable(true);
						if (l_DirectoriesVector.size())
						{
							m_pVersionListBox->SetImGuiName(L"VersionList");
							auto l_str = GetTxtFileContent(g_strRuleFileName);
							m_pRuleJsonContentEditbox->SetText(l_str);
						}
						else
						{
							m_pVersionListBox->SetImGuiName(L"No Env Version Data");
						}
						return;
					}
				}
			}
			else
			{
				UT::ErrorMsgByFormat("connect to %s failed", GetEnvName(eEnv::eE_DEV).c_str());
			}
			m_pMainUIRoot->ShowFullScreenBlackText(nullptr);
			m_pUploadRuleFileButton->SetEnable(true);
			//m_pMainUIRoot->ShowConfirmDialog(e_strResult.c_str());
			m_pMainUIRoot->ShowConfirmDialog("something wrong");
		};
		std::thread t(l_Function);
		t.detach();
	}
}

bool cGUIForFileTransfer::CheckEditboxContentJson()
{
	auto l_strContent = this->m_pRuleJsonContentEditbox->GetText();
	try
	{
		json l_JsonData = json::parse(l_strContent);
	}
	catch (json::exception& e)
	//catch (std::exception& e)
	{
		m_pMainUIRoot->ShowConfirmDialog(e.what());
		return false;
	}
	m_pMainUIRoot->ShowConfirmDialog("ok");
	return true;
}

void cGUIForFileTransfer::Init()
{
	ImGui_ImplOpenGL3_Init(cGameApp::m_spOpenGLRender->m_Handle, nullptr);
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
		SetImGuiGetCameraCursorPosition(l_ImGuiGetCameraCursorPosition, 0);
		SetImGuiCameraPositionConvertFunction(l_ImGuiCameraPositionConvertFunction, 0);
	}
}

void cGUIForFileTransfer::Update(float e_fElpaseTime)
{

}

void cGUIForFileTransfer::Render(float* e_pfMatrix, float* e_pfGameResolutoinSize)
{
	float l_fTargetGameResolution[2] = { 1920.f, 1080.f };
	ImGui_StartFrame(e_pfGameResolutoinSize);
	if (this->m_pMainUIRoot)
	{
		m_pMainUIRoot->Render();
	}
	ImGui_EndFrame(e_pfMatrix, e_pfGameResolutoinSize);
	GLRender::RenderRectangle(l_fTargetGameResolution[0], l_fTargetGameResolution[1], cMatrix44::Identity, Vector4::Red);
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