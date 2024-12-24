#include "stdafx.h"
#include "../imgui/MyGui/MyGui.h"
#include "../imgui/MyGui/MyGUIUtilities.h"
#include "CURLUI.h"
#include <fstream>
#include "CURLForGameData.h"
using json = nlohmann::json;
cCURLUI::cCURLUI()
{
	CurlInit();
	ImGui_ImplOpenGL3_Init(cGameApp::m_spOpenGLRender->m_Handle, nullptr);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
	ImGui::GetIO().IniFilename = nullptr;
	ImGui::GetIO().LogFilename = nullptr;
	ImGui::GetIO().FontGlobalScale = 1.5f;
	//
	cImGuiNode::m_sfParseFunction = std::bind(&cCURLUI::ParseUIFile, this, std::placeholders::_1);
	m_pMainUIRoot = GetMyGuiObjWithType<cMyGuiRootNode>();
	if (this->m_pMainUIRoot)
	{
		std::ifstream l_JsonStream("CURLUI.json");
		auto l_JsonData = json::parse(l_JsonStream);
		l_JsonStream.close();
		auto l_strKEy = l_JsonData.begin().key();
		SAFE_DELETE(this->m_pMainUIRoot);
		this->m_pMainUIRoot = (cMyGuiRootNode*)cImGuiNode::DoUnSerialize(l_JsonData);
		auto l_ChildVector = this->m_pMainUIRoot->GetChildNodeVector();
		//m_pMyGuiForm = (cMyGuiForm*)l_ChildVector[0];
	}

}

cCURLUI::~cCURLUI()
{
	SAFE_DELETE(m_pMainUIRoot);
	ImGui_ImplOpenGL3_Shutdown();
	CurlExit();
}

//"{
//""Token"": ""hhuWRcPdsAvBdPPMtuONqnYrkrAPkVreWYyOSDQqDqzJon"",
//""GameInfo"" : {
//	""GameCode"": ""10099"",
//		""GameName"" : ""Squid Game : Mukunghwaasd"",
//		""LineType"" : ""Line"",
//		""Provider"" : ""AP"",
//		""ReleaseDate"" : ""2020 - 01 - 01"",
//		""IsEgret"" : false,
//		""HasJackpot"" : false,
//		""HasFreeGame"" : true,
//		""HasFreeGameCustom"" : false,
//		""HasCollection"" : false,
//		""BF"" : [
//			""50""
//		] ,
//		""DC"" : [
//			""1.2""
//		] ,
//		""GameCategoryIds"" : [
//			""1""
//		] ,
//		""LanguageInfo"" : [
//	{
//		""Language"": ""cn"",
//			""Name"" : ""Squid Game : Mukunghwa""
//	},
//			{
//				""Language"": ""en"",
//				""Name"" : ""Squid Game : Mukunghwa""
//			},
//			{
//				""Language"": ""hi"",
//				""Name"" : ""Squid Game : Mukunghwa""
//			},
//			{
//				""Language"": ""id"",
//				""Name"" : ""Squid Game : Mukunghwa""
//			},
//			{
//				""Language"": ""ja"",
//				""Name"" : ""Squid Game : Mukunghwa""
//			},
//			{
//				""Language"": ""kh"",
//				""Name"" : ""Squid Game : Mukunghwa""
//			},
//			{
//				""Language"": ""kr"",
//				""Name"" : ""Squid Game : Mukunghwa""
//			},
//			{
//				""Language"": ""my"",
//				""Name"" : ""Squid Game : Mukunghwa""
//			},
//			{
//				""Language"": ""th"",
//				""Name"" : ""Squid Game : Mukunghwa""
//			},
//			{
//				""Language"": ""vi"",
//				""Name"" : ""Squid Game : Mukunghwa""
//			}
//		],
//		""SpecialPlatformRtp"": ""97.01 % ""
//}
//}"
void cCURLUI::JsonCompose()
{
	auto l_strJson = R"(
	{
		"Token": "hhuWRcPdsAvBdPPMtuONqnYrkrAPkVreWYyOSDQqDqzJon",
		"GameInfo" : 
		{
			"GameCode": "10099",
			"GameName" : "Squid Game : Mukunghwaasd",
			"LineType" : "Line",
			"Provider" : "AP",
			"ReleaseDate" : "2020 - 01 - 01",
			"IsEgret" : false,
			"HasJackpot" : false,
			"HasFreeGame" : true,
			"HasFreeGameCustom" : false,
			"HasCollection" : false,
			"BF" : 
			[
			] ,
			"DC" :
			[
				"1.2"
			],
			"GameCategoryIds" : 
			[
				"1"
			],
			"LanguageInfo":
			[
				{
					"Language": "cn",
					"Name" : "Squid Game : Mukunghwa"
				}
			],
			"SpecialPlatformRtp": "97.01%"
		}
	})";
	auto l_JsonObject = nlohmann::json::parse(l_strJson);
	json&l_GameInfo = l_JsonObject["GameInfo"];
	l_GameInfo["GameCode"] = m_strGameCode;
	l_GameInfo["GameName"] = m_strGameCode;
	m_pJsonEditBox->m_pData->m_strText = l_JsonObject.dump(4);
	//""Token"": ""hhuWRcPdsAvBdPPMtuONqnYrkrAPkVreWYyOSDQqDqzJon"",
	//std::string				m_strGameCode;
	//std::string				m_strGameName;
	//std::string				m_strLineType;
	//std::string				m_strProvider = "AP";
	//std::string				m_strReleaseDate = "01-10-2025";
	//bool					m_bJP;
	//bool					m_bFreeGamePickUpFeature;
	//bool					m_bCollectionFeature;
	//std::vector<int>		m_BuyFeatureRateVector;
	//float					m_fDoubleChanceRate = 1.2f;
	//eAPGameType				m_eGameCategoryIds;
	//std::string				m_strSpecialPlatformRtp = "";

}

void cCURLUI::ParseUIFile(cImGuiNode* e_pNode)
{
	if (e_pNode->Type() == cMyGuiEditBox::TypeID)
	{
		cMyGuiEditBox*l_pMyGuiEditBox = (cMyGuiEditBox*)e_pNode;
		if (l_pMyGuiEditBox->m_pData->m_strImGuiName == "JsonResultEditobx")
		{
			m_pJsonEditBox = l_pMyGuiEditBox;
		}
		
		l_pMyGuiEditBox->m_fContentChangedFunction = [this, l_pMyGuiEditBox]
		(std::string e_strText)
		{
			if (l_pMyGuiEditBox->m_pData->m_strImGuiName == "double chance")
			{
				m_fDoubleChanceRate = GetFloat(e_strText);
			}
			else
			if (l_pMyGuiEditBox->m_pData->m_strImGuiName == "buy feature")
			{
				m_BuyFeatureRateVector = GetIntegerListByCommaDivide(e_strText.c_str(), 5);
			}
			else
			if (l_pMyGuiEditBox->m_pData->m_strImGuiName == "SpecialPlatformRtp")
			{
				m_strSpecialPlatformRtp = e_strText;
			}
			else
			if (l_pMyGuiEditBox->m_pData->m_strImGuiName == "JsonResultEditobx")
			{

			}
			JsonCompose();
		};
	}
	else
	if (e_pNode->Type() == cMyGuiToogle::TypeID)
	{
		cMyGuiToogle* l_pMyGuiToogle = (cMyGuiToogle*)e_pNode;
		l_pMyGuiToogle->m_fToogleChangedFunction = [this, l_pMyGuiToogle]
		(bool e_bChecked)
		{
			if (l_pMyGuiToogle->m_pData->m_strText == "JP")
			{
				m_bJP = e_bChecked;
			}
			else
			if (l_pMyGuiToogle->m_pData->m_strText == "FG Pickup")
			{
				m_bFreeGamePickUpFeature = e_bChecked;
			}
			else
			if (l_pMyGuiToogle->m_pData->m_strText == "Collection")
			{
				m_bCollectionFeature = e_bChecked;
			}
		};
	}
	else
	if (e_pNode->Type() == cMyGuiComboBox::TypeID)
	{
		cMyGuiComboBox*l_pcMyGuiComboBox = (cMyGuiComboBox*)e_pNode;
		l_pcMyGuiComboBox->m_fOnSelectFunction = [this, l_pcMyGuiComboBox](int e_iIndex, std::string e_strSelectedItem)
		{
			if (l_pcMyGuiComboBox->m_pData->m_strText == "GameCode")
			{
				m_strGameCode = e_strSelectedItem;
			}
			else
			if (l_pcMyGuiComboBox->m_pData->m_strText == "LineType")
			{
				m_strLineType = e_strSelectedItem;
			}
			else
			if (l_pcMyGuiComboBox->m_pData->m_strText == "Game Type")
			{
				m_eGameCategoryIds = (eAPGameType)e_iIndex;
			}
				
		};
	}
	else
	if (e_pNode->Type() == cMyGuiDatePicker::TypeID)
	{
		cMyGuiDatePicker*l_pMyGuiDatePicker = (cMyGuiDatePicker*)e_pNode;
		l_pMyGuiDatePicker->m_fDateChangedFunction = [this](std::string e_strDateString)
		{
			m_strReleaseDate = e_strDateString;
		};
	}
}

void cCURLUI::ParseData(const char* e_strFileName)
{
}

void cCURLUI::Init()
{
}

void cCURLUI::Update(float e_fElpaseTime)
{
}

void cCURLUI::Render()
{
	float l_fTargetGameResolution[2] = { 1920.f, 1080.f };
	ImGui_StartFrame();
	if (this->m_pMainUIRoot)
	{
		m_pMainUIRoot->Render();
	}
	ImGui_EndFrame();
	GLRender::RenderRectangle(l_fTargetGameResolution[0], l_fTargetGameResolution[1], cMatrix44::Identity, Vector4::Red);
}