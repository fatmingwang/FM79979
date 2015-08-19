#include "stdafx.h"
#include "FBFunction.h"
#include "BluffingGirlApp.h"
#include "BluffingGirlPhaseManager.h"
#include "PlayerData.h"

bool	SetupFBInfo()
{
#ifdef ANDROID
	cNodeISAX	l_XmlNode;
	if(l_XmlNode.ParseDataIntoXMLNode("PhaseData/FBLinkInfo.xml"))
	{
		TiXmlElement*l_pRoot = l_XmlNode.GetRootElement();
		const WCHAR*l_strName = l_pRoot->Attribute(L"Name");
		const WCHAR*l_strDescription = l_pRoot->Attribute(L"Description");
		const WCHAR*l_strLink = l_pRoot->Attribute(L"Link");
		const WCHAR*l_strPictureURL = l_pRoot->Attribute(L"PictureURL");
		//const WCHAR*l_strAppID = l_pRoot->Attribute(L"AppID");
		jclass l_Class = GetCustomJavaClass("util/facebook/FBManager");
		EXCEPTION_RETURN(g_pMainThreadJNIUtilData->pJNIEnv);
		//jmethodID l_Method = g_pMainThreadJNIUtilData->pJNIEnv->GetStaticMethodID(l_Class,"SetupFBShareInfo","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
		jmethodID l_Method = g_pMainThreadJNIUtilData->pJNIEnv->GetStaticMethodID(l_Class,"SetupFBShareInfo","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
		EXCEPTION_RETURN(g_pMainThreadJNIUtilData->pJNIEnv);
		jstring l_jstrName = WcharToJava(g_pMainThreadJNIUtilData->pJNIEnv,l_strName);
		jstring l_jstrDescription = WcharToJava(g_pMainThreadJNIUtilData->pJNIEnv,l_strDescription);
		jstring l_jstrURL = WcharToJava(g_pMainThreadJNIUtilData->pJNIEnv,l_strLink);
		jstring l_jstrPictureURL = WcharToJava(g_pMainThreadJNIUtilData->pJNIEnv,l_strPictureURL);
		//jstring l_jstrAppID = WcharToJava(g_pMainThreadJNIUtilData->pJNIEnv,l_strAppID);
		//g_pMainThreadJNIUtilData->pJNIEnv->CallStaticVoidMethod(l_Class, l_Method,l_jstrName,l_jstrDescription,l_jstrURL,l_jstrPictureURL,l_jstrAppID);
		g_pMainThreadJNIUtilData->pJNIEnv->CallStaticVoidMethod(l_Class, l_Method,l_jstrName,l_jstrDescription,l_jstrURL,l_jstrPictureURL);
		EXCEPTION_RETURN(g_pMainThreadJNIUtilData->pJNIEnv);
		g_pMainThreadJNIUtilData->pJNIEnv->DeleteLocalRef(l_Class);
		return true;
	}
#endif
	return false;
}

cFBFunction::cFBFunction()
{
	m_pFBSharePrizeConfirmPopUpMessager = 0;
	//cPhasePopUpMessager*	m_pFBSharePrizeConfirmPopUpMessager;
	cBluffingGirlApp::RegisterPopupSystemMessenger("PhaseData/FBPrize.xml");

	m_pFBSharePrizeConfirmPopUpMessager = cBluffingGirlApp::GetPopupSystemMessenger(L"FBPrize");;
#ifdef ANDROID
	RegisterNativeFunction("util/facebook/FBManager","FBShareOk","()V",(void*)cFBFunction::DoFBSharePrize);
	SetupFBInfo();
	

#endif
}

cFBFunction::~cFBFunction()
{

}

void	cFBFunction::Update(float e_fElpaseTime)
{
	if( m_pFBSharePrizeConfirmPopUpMessager )
	{
		m_pFBSharePrizeConfirmPopUpMessager->Update(e_fElpaseTime);
		if( m_pFBSharePrizeConfirmPopUpMessager->IsSatisfiedCondition() )
		{
			cBluffingGirlApp::m_spPlayerData->HeartAdd(1,false);
		}
	}
}

void	cFBFunction::WriteTimeStamp(char*e_strFileName,int e_iMagicID)
{
	cSkipHeaderBinaryFile*l_pTimeFile = new cSkipHeaderBinaryFile(e_iMagicID);
	l_pTimeFile->Writefile(e_strFileName,true,true);
	if( l_pTimeFile->GetFile() )
	{
		l_pTimeFile->SetFilePos(0);
		timeval	l_Time;
		gettimeofday(&l_Time,0);
		//write current time
		l_pTimeFile->WriteToFile(l_Time.tv_sec);
		l_pTimeFile->WriteToFile(l_Time.tv_usec);
	}
	SAFE_DELETE(l_pTimeFile);
}

void	cFBFunction::DoFBSharePrize()
{
	int		l_iMagixID = 79979;
	bool	l_bWakePopupMessenger = false;
	char*	l_strFBFileName = "log.bin";
	float	l_fTimeDiff = GetTimeDiffByTimeStampFile(l_strFBFileName,l_iMagixID);
	float	l_fOneDatSeconds = 60*60*24;
	if( l_fOneDatSeconds - l_fTimeDiff < 0  )
	{
		//if( m_pFBSharePrizeConfirmPopUpMessager )
		//{
			cBluffingGirlApp::WakePopupSystemMessenger(L"FBPrize",L"FBWinPrize");
			WriteTimeStamp(l_strFBFileName,l_iMagixID);
		//}
	}
}


void	cFBFunction::DoFBLinkInfo()
{

}

float	cFBFunction::GetTimeDiffByTimeStampFile(char*e_strFileName,int e_iMagicID)
{
	cSkipHeaderBinaryFile*l_pTimeFile = new cSkipHeaderBinaryFile(e_iMagicID);
	if(l_pTimeFile->Openfile(e_strFileName,"rb+"))
	{
		char*l_pData = (char*)l_pTimeFile->GetDataFile(0);
		timeval	l_Time;
		memcpy(&l_Time.tv_sec,l_pData,sizeof(long));			l_pData += sizeof(long);
		memcpy(&l_Time.tv_usec,l_pData,sizeof(long));			l_pData += sizeof(long);
		SAFE_DELETE(l_pTimeFile);
		return cGameApp::m_sTimeAndFPS.GetSecondDiff(l_Time);
	}
	SAFE_DELETE(l_pTimeFile);
	return 999999999.f;
}