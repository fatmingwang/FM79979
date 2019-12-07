#include "stdafx.h"
#include "DownloadFile.h"
int	g_iDownloadProgress = 0;
#ifdef ANDROID
void NetworkUtil_DownloadProgress( JNIEnv* env,jobject thiz,  jint e_iProgress)
{
	g_iDownloadProgress = (int)e_iProgress;
}
#endif
cDownloadFile::cDownloadFile()
{
	m_iProgress = 0;
	g_iDownloadProgress = 0;
}

cDownloadFile::~cDownloadFile()
{

}

bool	cDownloadFile::StartToDownload(const char*e_strDownloadFileURL,const char*e_strDescription,const char* e_strTitle,const char*e_strDownloadFileName)
{
	m_strDownloadFileName = e_strDownloadFileName;
#ifdef ANDROID
	RegisterNativeFunction("util/NetworkUtil","DownloadProgress","(I)V",(void*)NetworkUtil_DownloadProgress);
	jstring l_jnistrURL = cGameApp::m_spThreadEnv->NewStringUTF(e_strDownloadFileURL);
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	jstring l_jnistrDescription = cGameApp::m_spThreadEnv->NewStringUTF(e_strDescription);
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	jstring l_jnistrTitle = cGameApp::m_spThreadEnv->NewStringUTF(e_strTitle);
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	jstring l_jnistrDownloadFileName = cGameApp::m_spThreadEnv->NewStringUTF(e_strDownloadFileName);
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	jclass thisClass = GetCustomJavaClass("util/NetworkUtil");
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	jmethodID l_DownloadFileFromNetworkMethod = cGameApp::m_spThreadEnv->GetStaticMethodID(thisClass,"DownloadByDownloadManager","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V");
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	//true for delete old file already download,because we do not check file size,so just delete old to avoid accident.
	//false for if old file is exists it will skip download
	//cGameApp::m_spThreadEnv->CallVoidMethod(*cGameApp::m_spAppThreadThis, l_DownloadFileFromNetworkMethod,l_jnistrURL,l_jnistrDescription,l_jnistrTitle,l_jnistrDownloadFileName,false);
	cGameApp::m_spThreadEnv->CallStaticVoidMethod(thisClass, l_DownloadFileFromNetworkMethod,l_jnistrURL,l_jnistrDescription,l_jnistrTitle,l_jnistrDownloadFileName,true);
	cGameApp::m_spThreadEnv->DeleteLocalRef(thisClass);
	
	//Sleep(1);
	//cGameApp::m_spThreadEnv->ReleaseStringUTFChars(l_jnistrURL,e_strDownloadFileURL);
	//cGameApp::m_spThreadEnv->ReleaseStringUTFChars(l_jnistrDescription,e_strDescription);
	//cGameApp::m_spThreadEnv->ReleaseStringUTFChars(l_jnistrTitle,e_strTitle);
	//cGameApp::m_spThreadEnv->ReleaseStringUTFChars(l_jnistrDownloadFileName,e_strDownloadFileName);
	//cGameApp::m_spThreadEnv->ReleaseStringUTFChars(l_jnistrPasseord,e_strZipPassword);
#endif
   return true;

}

void	cDownloadFile::Update(float e_fElpaseTime)
{
	m_iProgress = g_iDownloadProgress;
}

void	cDownloadFile::Render()
{
#ifdef ENGLISH
	const WCHAR l_str1[] = L"download progress:";
#else
	const WCHAR l_str1[] = {19979,36617,36914,24230,0};
#endif
	std::wstring	l_str = l_str1;
	l_str += ValueToStringW(m_iProgress);
	cGameApp::m_spGlyphFontRender->SetScale(2.f);
	cGameApp::RenderFont(cGameApp::m_spOpenGLRender->m_vGameResolution.x/2-300,cGameApp::m_spOpenGLRender->m_vGameResolution.y/2,l_str);
	cGameApp::m_spGlyphFontRender->SetScale(1.f);
}