#ifdef ANDROID

#ifndef _JNI_UTIL_H_
#define _JNI_UTIL_H_
#include "jni.h"
namespace FATMING_CORE
{
	struct sJNIUtilData
	{
		jobject			ThreadThis;
		JNIEnv*			pJNIEnv;
		jclass			activityClass;
		jclass			classLoader;
		jobject			cls;
		jmethodID		getClassLoader;
		jmethodID		findClass;
		sJNIUtilData(JNIEnv*e_pJNIEnv);
		~sJNIUtilData();
		bool	Init();
	};
	extern sJNIUtilData*g_pMainThreadJNIUtilData;
	bool	CallLaunchActivityVoidFunction(char*e_strFunction,char*e_strFunctionParameters);
	//if multithread please change Env
	bool	CallStaticVoidMethod1(char*e_strClassName,char*e_strMethodName,char*e_strFunctionParameter,sJNIUtilData*e_pJNIUtilData = g_pMainThreadJNIUtilData,bool e_bCustomClass = false);
	bool	CallStaticVoidMethod2(char*e_strClassName,char*e_strMethodName,char*e_strFunctionParameter,void*e_pInputData,sJNIUtilData*e_pJNIUtilData = g_pMainThreadJNIUtilData,bool e_bCustomClass = false);
	bool	CallStaticVoidMethod3(char*e_strClassName,char*e_strMethodName,char*e_strFunctionParameter,void*e_pInputData1,void*e_pInputData2,sJNIUtilData*e_pJNIUtilData = g_pMainThreadJNIUtilData,bool e_bCustomClass = false);
	bool	CallStaticVoidMethod4(char*e_strClassName,char*e_strMethodName,char*e_strFunctionParameter,void*e_pInputData1,void*e_pInputData2,void*e_pInputData3,sJNIUtilData*e_pJNIUtilData = g_pMainThreadJNIUtilData,bool e_bCustomClass = false);
	//"util/NetworkUtil","DownloadProgress","(I)V",(void*)NetworkUtil_DownloadProgress
	bool	RegisterNativeFunction(const char*e_strPackageAndClassName,const char*e_strJAVAFunctionName,const char*e_strSignature,void*FunctionPointer,sJNIUtilData*e_pJNIUtilData = g_pMainThreadJNIUtilData);
	//ensure e_pJNIUtilData->pJNIEnv->DeleteLocalRef(classIWant); is called after use.
	jclass	GetCustomJavaClass(const char*e_strPackageAndClassName,sJNIUtilData*e_pJNIUtilData = g_pMainThreadJNIUtilData);
	//http://stackoverflow.com/questions/11643498/android-get-meid-from-jni
	//Mobile Equipment Identifier (MEID)
	std::wstring	GetDeviceID();
	int				GetDeviceIDToNumber();
	//"util/Define","OpenKeyBoard","()V"
	//"util/Define","CloseKeyBoard","()V"
	std::wstring JavaToWchar(JNIEnv *env, jstring string);
	jstring WcharToJava(JNIEnv *env,const WCHAR*e_str);
	//https://stackoverflow.com/questions/16667029/a-correct-way-to-convert-byte-in-java-to-unsigned-char-in-c-and-vice-versa
	jbyteArray unsigned_array_to_jbyteArray(unsigned char* buf, int len,JNIEnv*e_pJNIEnv);
	unsigned char* jbyteArray_to_unsigned_char_array(jbyteArray array, JNIEnv*e_pJNIEnv);


	bool	OpenURL(const char*e_strURL,sJNIUtilData*e_pJNIUtilData = g_pMainThreadJNIUtilData);
	//AdMobUtility.java
	bool	ShowAd(char*e_strADMobID,int e_iWidth,int e_iHeight,Vector2 e_vshowPos);
	bool	ShowIntersitialAd(char*e_strADMobID);

	bool	ShowDialog(const char* title, const char *contents, bool exitApp);
//end namespace FATMING_CORE
}
//end	_JNI_UTIL_H_
#endif
//end	ANDROID
#endif