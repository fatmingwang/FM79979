#include "stdafx.h"
#include "CodeReport.h"
#include "QRCodeDecode.h"
#include <time.h>
#include <ctime>
#include "../../../Core/GameplayUT/ClickBehavior/DefaultRenderClickBehavior.h"
#define	DO_REPORT				L"DoReport"
#define	REPORT_OK				L"ReportOk"
#define	REPORT_FAILED			L"ReportFailed"



#define	REPORT_OK_INFO_POS					Vector3(450,85,0)

#define	REAL_TIME_INFO_POS					Vector3(450,745,0)


#define	WAIT_SERVER_RESPOND_POS				Vector3(210,550,0)

namespace UT
{
	extern bool mkpath(std::string path);
}

bool g_bDoShare = false;
bool g_bDoCapture = false;
std::string g_strFileName = "/sdcard/GameApp/qoo.txt";
std::wstring g_strPickupPhotoFileName = L"";

extern cCodeReport*g_pCodeReport;
//extern void	QRCodeResult(JNIEnv *env, jobject obj, jstring e_strResult, int e_iLength);
void	PickPhoto(JNIEnv *env, jobject obj, jstring e_strResult)
{
	std::wstring l_strResult = JavaToWchar(env, e_strResult);
	g_strPickupPhotoFileName = l_strResult;
}

std::string	GetFileFullPath(const char* e_strFileName)
{
	//http://blog.sephiroth.it/2010/10/24/reading-resource-files-from-native-code/
	//http://androgeek.info/?p=275
	//try external sd card first
	NvFile*fp = nullptr;
	if (cGameApp::m_spExternalSDDirectory)
	{
		std::string l_strFileName = *cGameApp::m_spExternalSDDirectory;
		l_strFileName += *cGameApp::m_psstrGameAppName;
		l_strFileName += "/";
		l_strFileName += e_strFileName;
		mkpath(l_strFileName);
		fp = NvFOpen(l_strFileName.c_str(), "w");
		if (fp)
		{
			NvFClose(fp);
			return l_strFileName;
		}
	}
	//try write file into
	std::string l_strFileName = "/sdcard/";
	l_strFileName += *cGameApp::m_psstrGameAppName;
	l_strFileName += "/";
	l_strFileName += e_strFileName;
	//I am lazy to do recursive mkdir,so here may not working
	mkpath(l_strFileName);
	fp = NvFOpen(l_strFileName.c_str(), "w");
	if (!fp)
	{//write into internal memory
		l_strFileName = cGameApp::m_spInternalDirectory->c_str();
		l_strFileName += "/";
		l_strFileName += *cGameApp::m_psstrGameAppName;
		l_strFileName += "/";
		l_strFileName += e_strFileName;
		mkpath(l_strFileName);
		fp = NvFOpen(l_strFileName.c_str(), "w");
	}
	if(fp)
		NvFClose(fp);
	return l_strFileName;
}


cCodeReport::cCodeReport()
{
	int l_iData = sizeof(sReportInfo);
	g_pCodeReport = this;
	m_pBluetoothSinglton = cBluetoothSinglton::GetInstance();
	m_pBluetoothSinglton->CreateAsClient(L"FMWin7");
	m_pChangeQRCodePhotoToDecode = nullptr;
	bool l_bResult = RegisterNativeFunction("com/FM79979/EmptyAndroidProject/EmptyAndroidProject", "PickPhoto", "(Ljava/lang/String;)V", (void*)PickPhoto);
	m_pBG = nullptr;
	m_bReportResult = false;
	cGameApp::m_svBGColor = Vector4::Zero;
	m_bWaitServerRequestReportResult = false;
	memset(&m_ReportInfo, 0, sizeof(m_ReportInfo));
	memset(&m_ReportSuccessInfo, 0, sizeof(m_ReportSuccessInfo));
	m_FiGameReporterParameterManager.ParseWithMyParse("FiGameReporterParameterManager.xml");
}

cCodeReport::~cCodeReport()
{
	g_pCodeReport = nullptr;
	SAFE_DELETE(m_pChangeQRCodePhotoToDecode);
	cBluetoothSinglton::DestroyInstance();
	//Frame::DestoryWithChildren(&m_UINode);
}

void	ScreenshotFuck()
{
	if (!g_bDoCapture)
		return;
	g_strFileName = GetFileFullPath(g_strFileName.c_str());
	SaveCurrentBufferToImage(g_strFileName.c_str());
	g_bDoShare = true;
	g_bDoCapture = false;
}


void cCodeReport::Init()
{
	if (m_ClickBehaviorDispatcher.Count() == 0)
	{	
		auto l_pMPDIList = cGameApp::GetMPDIListByFileName(L"Info.mpdi");
		m_pBG = l_pMPDIList->GetObject(L"BG");
		if (m_pBG)
			m_pBG->Init();
		auto l_DoReportButtonClick =  [this](int e_iPosX, int e_iPosY, cClickBehavior*e_pButton)
		{
			FMLog::Log(e_pButton->GetName(),false);
			m_bReportResult = false;
			cDefaultRenderClickBehavior*l_pDefaultRenderClickBehavior = (cDefaultRenderClickBehavior*)m_ClickBehaviorDispatcher[DO_REPORT];
			if (l_pDefaultRenderClickBehavior)
				l_pDefaultRenderClickBehavior->SetEnableAndImageStatus(false, Vector4::HalfDark);
			sBaseMessageInfo l_sBaseMessageInfo(eBRCM_SEND_TO_SERVER_REQUEST_REPORT);
			int l_iLength = sizeof(sBaseMessageInfo);
			m_pBluetoothSinglton->SendDataToServer(l_iLength, (char*)&l_sBaseMessageInfo);
			m_bWaitServerRequestReportResult = true;
		};
		auto l_ReportOkButton = [this](int e_iPosX, int e_iPosY, cClickBehavior*e_pButton)
		{
			FMLog::Log(e_pButton->GetName(), false);
			cDefaultRenderClickBehavior*l_pDefaultRenderClickBehavior = (cDefaultRenderClickBehavior*)m_ClickBehaviorDispatcher[DO_REPORT];
			if(l_pDefaultRenderClickBehavior)
				l_pDefaultRenderClickBehavior->SetEnableAndImageStatus(true,Vector4::One);
			((cDefaultRenderClickBehavior*)e_pButton)->SetEnableAndImageStatus(false, Vector4::Zero);
		};

		auto l_CaptureScreenButton = [this](int e_iPosX, int e_iPosY, cClickBehavior*e_pButton)
		{
			if (g_bDoCapture)
				return;
			std::string l_strFileName = ValueToString(m_ReportSuccessInfo.iProductTypeNumber);
			l_strFileName += "_";
			l_strFileName += ValueToString(m_ReportSuccessInfo.iMachineID);
			l_strFileName += "_";
			auto l_CurrentTime = time(0);
			l_strFileName += asctime(std::localtime(&l_CurrentTime));
			l_strFileName += ".jpg";
			g_strFileName = l_strFileName;
			g_bDoCapture = true;
		};

		auto l_QRCodeButtonClick = [this](int e_iPosX, int e_iPosY, cClickBehavior*e_pButton)
		{
			bool l_bUsePhotoToDoDecoding = false;
			if (l_bUsePhotoToDoDecoding)
			{
				CallLaunchActivityVoidFunction("PickUpPhotoByIntent", "()V");
			}
			else
			{
				jclass thisClass = cGameApp::m_spThreadEnv->GetObjectClass(*cGameApp::m_spAppThreadThis);
				jmethodID l_FunctionMethodID = cGameApp::m_spThreadEnv->GetMethodID(thisClass, "GetClipboardString", "()Ljava/lang/String;");
				jstring l_strJString = (jstring)cGameApp::m_spThreadEnv->CallObjectMethod(*cGameApp::m_spAppThreadThis, l_FunctionMethodID);
				auto l_strReturnText = cGameApp::m_spThreadEnv->GetStringUTFChars(l_strJString, NULL);
				std::string l_strResultText(l_strReturnText);
				cGameApp::m_spThreadEnv->ReleaseStringUTFChars(l_strJString, l_strReturnText);
				sReportInfo	l_ReportSuccessInfo;
				bool l_bResult = GetReportInfoByString(l_strResultText.c_str(), l_ReportSuccessInfo);
				if (l_bResult)
				{
					m_ReportSuccessInfo = l_ReportSuccessInfo;
					sSendToServerReportResult l_sSendToServerReportResult;
					char l_strReportCode[100];
					FISG_GAME_PARAMETERIZE::sTwLeadStreamProductData l_TwLeadStreamProductData;
					l_TwLeadStreamProductData.iProductTypeNumber = m_ReportSuccessInfo.iProductTypeNumber;
					l_TwLeadStreamProductData.iMachineID = m_ReportSuccessInfo.iMachineID;
					GenerateReportCode(l_strReportCode, m_ReportSuccessInfo.i64ExchangeIn, m_ReportSuccessInfo.i64ExchangeOut, m_ReportSuccessInfo.iReportCount, l_TwLeadStreamProductData);
					int	l_iLength = CombineReportCodeWithParameterSetting(l_strReportCode, l_sSendToServerReportResult.cCode, &m_FiGameReporterParameterManager, false);
					jstring l_CodeResult = WcharToJava(cGameApp::m_spThreadEnv, ValueToStringW(l_sSendToServerReportResult.cCode).c_str());
					jmethodID l_SetClipboardStringFunctionMethodID = cGameApp::m_spThreadEnv->GetMethodID(thisClass, "SetClipboardString", "(Ljava/lang/String;)V");
					cGameApp::m_spThreadEnv->CallVoidMethod(*cGameApp::m_spAppThreadThis, l_SetClipboardStringFunctionMethodID, l_CodeResult);
					AndroidShowDialog("success", l_sSendToServerReportResult.cCode);
				}
				else
				{
					AndroidShowDialog("format is not incorrect", "Error");
				}
			}
		};

		auto l_pButtonMPDI = l_pMPDIList->GetObject(L"Button");
		auto l_pDoReportButtonSubMPDI = l_pButtonMPDI->GetObject(0);
		auto l_pReportOkButtonSubMPDI = l_pButtonMPDI->GetObject(1);
		auto l_pReportFailedButtonSubMPDI = l_pButtonMPDI->GetObject(2);
		auto l_pSaveButtonSubMPDI = l_pButtonMPDI->GetObject(3);
		auto l_pQRCodeSubMPDI = l_pButtonMPDI->GetObject(4);

		//cCueToStartCurveWithTime*e_pSubMPDI, ClickFunction e_ClickFunction, cBasicSound*e_pBasicSound
		auto l_pDoReportButtonClickObjectTuple  = m_ClickBehaviorDispatcher.AddDefaultRenderClickBehaviorButton(l_pDoReportButtonSubMPDI,l_DoReportButtonClick, nullptr);
		auto l_pReportFailedImageObjectTuple = m_ClickBehaviorDispatcher.AddDefaultRenderClickBehaviorButton(l_pReportFailedButtonSubMPDI, l_ReportOkButton, nullptr);
		auto l_pReportOkImageObjectTuple = m_ClickBehaviorDispatcher.AddDefaultRenderClickBehaviorButton(l_pReportOkButtonSubMPDI, l_ReportOkButton, nullptr);
		auto l_pCaptureObjectTuple = m_ClickBehaviorDispatcher.AddDefaultRenderClickBehaviorButton(l_pSaveButtonSubMPDI,l_CaptureScreenButton, nullptr);
		auto l_pQRObjectTuple = m_ClickBehaviorDispatcher.AddDefaultRenderClickBehaviorButton(l_pQRCodeSubMPDI, l_QRCodeButtonClick, nullptr);

		cDefaultRenderClickBehavior*l_pDoReportClickObject = (cDefaultRenderClickBehavior*)std::get<0>(l_pDoReportButtonClickObjectTuple);
		cDefaultRenderClickBehavior*l_pReportFailedClickObject = (cDefaultRenderClickBehavior*)std::get<0>(l_pReportFailedImageObjectTuple);
		cDefaultRenderClickBehavior*l_pReportOkClickObject = (cDefaultRenderClickBehavior*)std::get<0>(l_pReportOkImageObjectTuple);
		cDefaultRenderClickBehavior*l_pCaptureClickObject = (cDefaultRenderClickBehavior*)std::get<0>(l_pCaptureObjectTuple);

		cRenderObject*l_pDoReportImage = std::get<1>(l_pDoReportButtonClickObjectTuple);
		cRenderObject*l_pReportFailedImage = std::get<1>(l_pReportFailedImageObjectTuple);
		cRenderObject*l_pReportOkImage = std::get<1>(l_pReportOkImageObjectTuple);
		cRenderObject*l_pCaptureImage = std::get<1>(l_pCaptureObjectTuple);
		cRenderObject*l_pQRImage = std::get<1>(l_pQRObjectTuple);

		m_UINode.AddChildToLast(l_pDoReportImage);
		m_UINode.AddChildToLast(l_pReportFailedImage);
		m_UINode.AddChildToLast(l_pReportOkImage);
		m_UINode.AddChildToLast(l_pCaptureImage);
		m_UINode.AddChildToLast(l_pQRImage);


		l_pDoReportClickObject->SetEnableAndImageStatus(false,Vector4::Zero);
		l_pReportFailedClickObject->SetEnableAndImageStatus(false,Vector4::Zero);
		l_pReportOkClickObject->SetEnableAndImageStatus(false, Vector4::Zero);
		//auto l_ClientAddFunction = [this](SDLNet_Socket*e_pInput)
		//{

		//};
		auto l_ClientLostFunction = [this, l_pDoReportClickObject](SDLNet_Socket*e_pInput)
		{
			m_bWaitServerRequestReportResult = false;
			l_pDoReportClickObject->SetEnableAndImageStatus(false, Vector4::HalfDark);

		};
		//auto l_ClientConnectionLost = [this](SDLNet_Socket*e_pInput)
		//{

		//};
		auto l_ClientConnectionConnected = [l_pDoReportClickObject,this](SDLNet_Socket*e_pInput)
		{
			l_pDoReportClickObject->SetEnableAndImageStatus(true, Vector4::One);

		};
		m_pBluetoothSinglton->SetSocketFunction(nullptr, l_ClientLostFunction, nullptr, l_ClientConnectionConnected);
	}
}

void cCodeReport::Update(float e_fElpaseTime)
{
	if (g_pCodeReport && g_strPickupPhotoFileName.length())
	{
		auto l_strPhotoFileName = ValueToString(g_strPickupPhotoFileName);
		g_pCodeReport->CreateChangeQRCodePhotoToDecode(l_strPhotoFileName.c_str());
		g_strPickupPhotoFileName = L"";
	}
	if (m_pChangeQRCodePhotoToDecode)
	{
		m_pChangeQRCodePhotoToDecode->Update(e_fElpaseTime);
		if (m_pChangeQRCodePhotoToDecode->m_bSucess)
		{
			if(m_pChangeQRCodePhotoToDecode->m_strResult.length()>37)
				m_strQRCode = &m_pChangeQRCodePhotoToDecode->m_strResult[37];
			SAFE_DELETE(m_pChangeQRCodePhotoToDecode);
		}
	}
	else
	{
		if (m_pBG)
			m_pBG->Update(e_fElpaseTime);
		m_ClickBehaviorDispatcher.Update(e_fElpaseTime);
		m_UINode.UpdateNodes(e_fElpaseTime);
		m_pBluetoothSinglton->Update(e_fElpaseTime);
		std::vector<sBluetoothPacket*> l_DataVector;
		m_pBluetoothSinglton->GetReceivedData(&l_DataVector);
		if (l_DataVector.size())
		{
			for (size_t i = 0; i < l_DataVector.size(); i++)
			{
				sBluetoothPacket*l_pData = l_DataVector[i];
				sBaseMessageInfo*l_pBaseMessageInfo = (sBaseMessageInfo*)l_pData->pData;
				ProcessMessage(l_pBaseMessageInfo, l_pData->iSize);
				delete l_pData;
			}
		}
		if (g_bDoShare)
		{
			static int l_iWaitForImageFlush = 20;
			--l_iWaitForImageFlush;
			if (l_iWaitForImageFlush == 0)
			{
				l_iWaitForImageFlush = 20;
				g_bDoShare = false;
				jstring l_javaStringFileName = cGameApp::m_spThreadEnv->NewStringUTF(g_strFileName.c_str());
				jclass thisClass = cGameApp::m_spThreadEnv->GetObjectClass(*cGameApp::m_spAppThreadThis);
				//EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
				jmethodID l_FunctionMethodID = cGameApp::m_spThreadEnv->GetMethodID(thisClass, "ShareImageFileToFriend", "(Ljava/lang/String;)V");
				//EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
				cGameApp::m_spThreadEnv->CallVoidMethod(*cGameApp::m_spAppThreadThis, l_FunctionMethodID, l_javaStringFileName);
				//EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
			}
		}
	}
}

void	RenderReportInfo(Vector2 e_vPos, sReportInfo&e_ReportInfo)
{

	int l_iHeightGap = 0;
	float l_fHeightGap = 49.f;
	cGameApp::RenderFont(e_vPos.x, e_vPos.y + l_iHeightGap, ValueToStringW(e_ReportInfo.i64ExchangeIn).c_str()); l_iHeightGap += l_fHeightGap;
	cGameApp::RenderFont(e_vPos.x, e_vPos.y + l_iHeightGap, ValueToStringW(e_ReportInfo.i64ExchangeOut).c_str()); l_iHeightGap += l_fHeightGap;
	cGameApp::RenderFont(e_vPos.x, e_vPos.y + l_iHeightGap, ValueToStringW(e_ReportInfo.i64BulletShootCount).c_str()); l_iHeightGap += l_fHeightGap;
	cGameApp::RenderFont(e_vPos.x, e_vPos.y + l_iHeightGap, ValueToStringW(e_ReportInfo.iReportCount).c_str()); l_iHeightGap += l_fHeightGap;
	cGameApp::RenderFont(e_vPos.x, e_vPos.y + l_iHeightGap, ValueToStringW(e_ReportInfo.i64BetMoney).c_str()); l_iHeightGap += l_fHeightGap;
	cGameApp::RenderFont(e_vPos.x, e_vPos.y + l_iHeightGap, ValueToStringW(e_ReportInfo.i64WinMoney).c_str()); l_iHeightGap += l_fHeightGap;
	cGameApp::RenderFont(e_vPos.x, e_vPos.y + l_iHeightGap, ValueToStringW(e_ReportInfo.i64BulletTotalShootCount).c_str()); l_iHeightGap += l_fHeightGap;
	cGameApp::RenderFont(e_vPos.x, e_vPos.y + l_iHeightGap, ValueToStringW(e_ReportInfo.i64OVERCount).c_str()); l_iHeightGap += l_fHeightGap;
	cGameApp::RenderFont(e_vPos.x, e_vPos.y + l_iHeightGap, ValueToStringW(e_ReportInfo.i64OVERCount).c_str()); l_iHeightGap += l_fHeightGap;
	cGameApp::RenderFont(e_vPos.x, e_vPos.y + l_iHeightGap, ValueToStringW(e_ReportInfo.i64GameWater).c_str());
}
void cCodeReport::Render()
{
	if (m_pChangeQRCodePhotoToDecode)
	{
		m_pChangeQRCodePhotoToDecode->Render();
	}
	else
	{
		if (m_pBG)
			m_pBG->Render();
		//if (m_bReportResult)
		{
			RenderReportInfo(Vector2(REPORT_OK_INFO_POS.x, REPORT_OK_INFO_POS.y), m_ReportSuccessInfo);
		}

		cGameApp::RenderFont(15.f, 170.f, ValueToStringW(m_ReportInfo.iProductTypeNumber).c_str());
		cGameApp::RenderFont(15.f, 70.f, ValueToStringW(m_ReportInfo.iMachineID).c_str());

		int l_iHeightGap = 0;
		RenderReportInfo(Vector2(REAL_TIME_INFO_POS.x, REAL_TIME_INFO_POS.y), m_ReportInfo); 
		m_UINode.RenderNodes();
		if (m_pBluetoothSinglton)
			m_pBluetoothSinglton->Render(1100, 10);

		if (m_bWaitServerRequestReportResult)
		{
			static int l_siDotCount = 0;
			std::wstring l_strInfo = L"Wait for scene change to do code report";
			++l_siDotCount;
			if (l_siDotCount > 50)
				l_siDotCount = 0;
			for (size_t i = 0; i < l_siDotCount / 10; i++)
			{
				l_strInfo += L".";
			}
			cGameApp::RenderFont(WAIT_SERVER_RESPOND_POS.x, WAIT_SERVER_RESPOND_POS.y, l_strInfo.c_str());
		}

		auto l_CurrentTime = time(0);
		std::string l_strTime = asctime(std::localtime(&l_CurrentTime));
		cGameApp::RenderFont(220.f, 1240.f, ValueToStringW(l_strTime.c_str()));
		if (g_bDoShare)
		{
			cGameApp::RenderFont(500.f, 700.f, L"wait for process");
		}
		if (m_strQRCode.length())
		{
			cGameApp::RenderFont(220, 1210, ValueToStringW(m_strQRCode).c_str());
		}
		ScreenshotFuck();
	}
}

void cCodeReport::MouseDown(int e_iPosX, int e_iPosY)
{
	if (m_pChangeQRCodePhotoToDecode)
	{
		m_pChangeQRCodePhotoToDecode->MouseDown(e_iPosX,e_iPosY);
	}
	else
	{
		m_ClickBehaviorDispatcher.MouseDown(e_iPosX, e_iPosY);
	}
}

void cCodeReport::MouseMove(int e_iPosX, int e_iPosY)
{
	if (m_pChangeQRCodePhotoToDecode)
	{

	}
	else
		m_ClickBehaviorDispatcher.MouseMove(e_iPosX, e_iPosY);
}

void cCodeReport::MouseUp(int e_iPosX, int e_iPosY)
{
	if (m_pChangeQRCodePhotoToDecode)
	{

	}
	else
		m_ClickBehaviorDispatcher.MouseUp(e_iPosX, e_iPosY);
}

void cCodeReport::CreateChangeQRCodePhotoToDecode(const char*e_strFileName)
{
	SAFE_DELETE(m_pChangeQRCodePhotoToDecode);
	m_pChangeQRCodePhotoToDecode = new cChangeQRCodePhotoToDecode(e_strFileName);
}


void	cCodeReport::ProcessMessage(sBaseMessageInfo*e_pBaseMessageInfo, int e_iPacketSize)
{
	switch (e_pBaseMessageInfo->iMessageID)
	{
		case eBRCM_SENT_TO_CLIENT_REPORT_INFO:
		{
			if (e_iPacketSize == sizeof(sSendToClientReportInfo))
			{
				sSendToClientReportInfo*l_pSendToClientReportInfo = (sSendToClientReportInfo*)e_pBaseMessageInfo;
				memcpy(&m_ReportInfo, &l_pSendToClientReportInfo->ReportInfo, sizeof(sReportInfo));
			}
		}
		break;
		case eBRCM_SENT_TO_CLIENT_REQUEST_REPORT_RESULT:
		{
			sSendToServerReportResult l_sSendToServerReportResult;
			char l_strReportCode[100];
			FISG_GAME_PARAMETERIZE::sTwLeadStreamProductData l_TwLeadStreamProductData;
			l_TwLeadStreamProductData.iProductTypeNumber = m_ReportInfo.iProductTypeNumber;
			l_TwLeadStreamProductData.iMachineID = m_ReportInfo.iMachineID;
			GenerateReportCode(l_strReportCode, m_ReportInfo.i64ExchangeIn, m_ReportInfo.i64ExchangeOut, m_ReportInfo.iReportCount, l_TwLeadStreamProductData);
			int	l_iLength = CombineReportCodeWithParameterSetting(l_strReportCode, l_sSendToServerReportResult.cCode, &m_FiGameReporterParameterManager,false);
			int l_iDataLength = sizeof(sSendToServerReportResult);
			l_sSendToServerReportResult.iCodeLnegth = l_iLength;
			m_pBluetoothSinglton->SendDataToServer(l_iDataLength, (char*)&l_sSendToServerReportResult);
			m_ReportSuccessInfo = m_ReportInfo;
		}
		break;
		case eBRCM_SENT_TO_CLIENT_REPORT_RESULT:
		{
			m_bWaitServerRequestReportResult = false;
			sSendToClientReportResult*l_pSendToClientReportResult = (sSendToClientReportResult*)e_pBaseMessageInfo;
			cDefaultRenderClickBehavior*l_pDefaultRenderClickBehavior = nullptr;
			m_bReportResult = l_pSendToClientReportResult->bResult;
			if (l_pSendToClientReportResult->bResult)
			{
				l_pDefaultRenderClickBehavior = (cDefaultRenderClickBehavior*)m_ClickBehaviorDispatcher[REPORT_OK];
			}
			else
			{
				l_pDefaultRenderClickBehavior = (cDefaultRenderClickBehavior*)m_ClickBehaviorDispatcher[REPORT_FAILED];
			}
			if (l_pDefaultRenderClickBehavior)
			{
				l_pDefaultRenderClickBehavior->SetEnableAndImageStatus(true, Vector4::One);
			}

		}
			break;
			//case eBRCM_SEND_TO_SERVER_REQUEST_REPORT:
			//	break;
			//case eBRCM_SEND_TO_SERVER_REPORT_RESULT:
			//	break;
		default:
			break;
	}
}