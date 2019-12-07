#include "stdafx.h"
#include "Unzip.h"
int	g_iUnzipProgress = 0;

cUnZip::cUnZip()
{
	m_iProgress = 0;
	g_iUnzipProgress = 0;
}

cUnZip::~cUnZip()
{

}

#ifdef ANDROID
//after cosume or preume purchased
void ZipUtilActivity_UnzipProgress(JNIEnv *env,jobject thiz, jint e_iProgress)
{
	g_iUnzipProgress = e_iProgress;
}
#endif

bool	cUnZip::StartUnzipInSystemFolder(const char*e_strFileName,const char*e_strExtractDirectory,const char*e_strPassword)
{
#ifdef ANDROID
	std::string	l_strTargetFileName = e_strFileName;

	std::string	l_strTargetExtractDirectory = "sdcard/";
	l_strTargetExtractDirectory += *cGameApp::m_psstrGameAppName;
	l_strTargetExtractDirectory += "/";
	l_strTargetExtractDirectory += e_strExtractDirectory;

	cGameApp::OutputDebugInfoString(l_strTargetFileName);
	cGameApp::OutputDebugInfoString(l_strTargetExtractDirectory);
	RegisterNativeFunction("util/ZipUtilActivity","UnzipProgress","(I)V",(void*)ZipUtilActivity_UnzipProgress);

	jstring l_jnistrFileName = cGameApp::m_spThreadEnv->NewStringUTF(l_strTargetFileName.c_str());
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	jstring l_jnistrExtractDirectory = cGameApp::m_spThreadEnv->NewStringUTF(l_strTargetExtractDirectory.c_str());
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	jstring l_jnistrPasseord = cGameApp::m_spThreadEnv->NewStringUTF(e_strPassword);
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);

	jclass thisClass = GetCustomJavaClass("util/ZipUtilActivity");
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	jmethodID l_CallUnzipUtilityMethod = cGameApp::m_spThreadEnv->GetStaticMethodID(thisClass,"UnzipWithSystemFolder", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V");
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	cGameApp::m_spThreadEnv->CallVoidMethod(*cGameApp::m_spAppThreadThis, l_CallUnzipUtilityMethod,l_jnistrFileName,l_jnistrExtractDirectory,l_jnistrPasseord,false);
#endif
	return true;
}

bool	cUnZip::StartUnzipInDownloadFolder(const char*e_strFileName,const char*e_strExtractDirectory,const char*e_strPassword)
{
#ifdef ANDROID
	std::string	l_strTargetFileName = "sdcard/Download/";
	l_strTargetFileName += e_strFileName;

	std::string	l_strTargetExtractDirectory = "sdcard/";
	l_strTargetExtractDirectory += *cGameApp::m_psstrGameAppName;
	l_strTargetExtractDirectory += "/";
	l_strTargetExtractDirectory += e_strExtractDirectory;

	cGameApp::OutputDebugInfoString(l_strTargetFileName);
	cGameApp::OutputDebugInfoString(l_strTargetExtractDirectory);
	RegisterNativeFunction("util/ZipUtilActivity","UnzipProgress","(I)V",(void*)ZipUtilActivity_UnzipProgress);
	jstring l_jnistrFileName = cGameApp::m_spThreadEnv->NewStringUTF(l_strTargetFileName.c_str());
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	jstring l_jnistrExtractDirectory = cGameApp::m_spThreadEnv->NewStringUTF(l_strTargetExtractDirectory.c_str());
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	jstring l_jnistrPasseord = cGameApp::m_spThreadEnv->NewStringUTF(e_strPassword);
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);

	jclass thisClass = cGameApp::m_spThreadEnv->GetObjectClass(*cGameApp::m_spAppThreadThis);
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	jmethodID l_CallUnzipUtilityMethod = cGameApp::m_spThreadEnv->GetMethodID(thisClass,"CallUnzipUtility", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	cGameApp::m_spThreadEnv->CallVoidMethod(*cGameApp::m_spAppThreadThis, l_CallUnzipUtilityMethod,l_jnistrFileName,l_jnistrExtractDirectory,l_jnistrPasseord);

   //cGameApp::m_spThreadEnv->ReleaseStringUTFChars(l_jnistrFileName,e_strFileName);
   //cGameApp::m_spThreadEnv->ReleaseStringUTFChars(l_jnistrExtractDirectory,e_strExtractDirectory);
   //cGameApp::m_spThreadEnv->ReleaseStringUTFChars(l_jnistrPasseord,e_strPassword);
#endif
	return true;
}

void	cUnZip::Update(float e_fElpaseTime)
{
	this->m_iProgress = g_iUnzipProgress;
}

void	cUnZip::Render()
{
#ifdef ENGLISH
	WCHAR*	l_str1 = L"UnZip progress:";
#else
	WCHAR	l_str1[] = {35299,22739,32302,36914,24230,0};
#endif
	std::wstring	l_str = l_str1;//L"extract file progress:";
	l_str += ValueToStringW(m_iProgress);
	cGameApp::m_spGlyphFontRender->SetScale(2.f);
	cGameApp::RenderFont(cGameApp::m_spOpenGLRender->m_vGameResolution.x/2-300,cGameApp::m_spOpenGLRender->m_vGameResolution.y/2,l_str);
	cGameApp::m_spGlyphFontRender->SetScale(1.f);
}