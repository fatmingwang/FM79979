#include "../stdafx.h"
#include "jni.h"
#include "JNIUtil.h"
#include "../GameplayUT/GameApp.h"
namespace FATMING_CORE
{
	sJNIUtilData*g_pMainThreadJNIUtilData = 0;
	sJNIUtilData::sJNIUtilData(JNIEnv*e_pJNIEnv)
	{
		pJNIEnv = e_pJNIEnv;
		ThreadThis = 0;
		activityClass = 0;
		getClassLoader = 0;
		cls = 0;
		classLoader = 0;
		findClass = 0;
	}

	sJNIUtilData::~sJNIUtilData()
	{
		if(activityClass)
			pJNIEnv->DeleteLocalRef(activityClass);
		if( classLoader )
			pJNIEnv->DeleteLocalRef(classLoader);
		if(cls)
			pJNIEnv->DeleteLocalRef(cls);
		if( ThreadThis && ThreadThis != *cGameApp::m_spAppThreadThis )
		{
			pJNIEnv->DeleteLocalRef(ThreadThis );
		}
	}

	bool	sJNIUtilData::Init()
	{
		activityClass = pJNIEnv->FindClass("android/app/NativeActivity");
		EXCEPTION_RETURN(pJNIEnv);
		getClassLoader = pJNIEnv->GetMethodID(activityClass,"getClassLoader", "()Ljava/lang/ClassLoader;");
		EXCEPTION_RETURN(pJNIEnv);
		ThreadThis  = pJNIEnv->NewGlobalRef(cGameApp::m_spANativeActivity->clazz);
		cls = pJNIEnv->CallObjectMethod(ThreadThis , getClassLoader);
		EXCEPTION_RETURN(pJNIEnv);
		classLoader = pJNIEnv->FindClass("java/lang/ClassLoader");
		EXCEPTION_RETURN(pJNIEnv);
		findClass = pJNIEnv->GetMethodID(classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
		EXCEPTION_RETURN(pJNIEnv);
		return true;
	}

	bool	CallLaunchActivityVoidFunction(char*e_strFunction,char*e_strFunctionParameters)
	{
		jclass thisClass = cGameApp::m_spThreadEnv->GetObjectClass(*cGameApp::m_spAppThreadThis);
		EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
		jmethodID l_FunctionMethodID = cGameApp::m_spThreadEnv->GetMethodID(thisClass,e_strFunction,e_strFunctionParameters);
		EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
		cGameApp::m_spThreadEnv->CallVoidMethod(*cGameApp::m_spAppThreadThis,l_FunctionMethodID);
		return true;
	}

	bool	CallStaticVoidMethod1(char*e_strClassName,char*e_strMethodName,char*e_strFunctionParameter,sJNIUtilData*e_pJNIUtilData,bool e_bCustomClass)
	{
		jclass l_Class;
		if( e_bCustomClass )
			 l_Class = GetCustomJavaClass(e_strClassName,e_pJNIUtilData);
		else
			 l_Class = e_pJNIUtilData->pJNIEnv->FindClass(e_strClassName);
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);
		jmethodID l_Method = e_pJNIUtilData->pJNIEnv->GetStaticMethodID(l_Class,e_strMethodName,e_strFunctionParameter);
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);
		e_pJNIUtilData->pJNIEnv->CallStaticVoidMethod(l_Class, l_Method);
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);
		e_pJNIUtilData->pJNIEnv->DeleteLocalRef(l_Class);
		return true;
	}

	bool	CallStaticVoidMethod2(char*e_strClassName,char*e_strMethodName,char*e_strFunctionParameter,void*e_pInputData,sJNIUtilData*e_pJNIUtilData,bool e_bCustomClass)
	{
		jclass l_Class;
		if( e_bCustomClass )
			 l_Class = GetCustomJavaClass(e_strClassName,e_pJNIUtilData);
		else
			 l_Class = e_pJNIUtilData->pJNIEnv->FindClass(e_strClassName);
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);
		jmethodID l_Method = e_pJNIUtilData->pJNIEnv->GetStaticMethodID(l_Class,e_strMethodName,e_strFunctionParameter);
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);
		e_pJNIUtilData->pJNIEnv->CallStaticVoidMethod(l_Class, l_Method,e_pInputData);
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);
		e_pJNIUtilData->pJNIEnv->DeleteLocalRef(l_Class);
		return true;
	}

	bool	CallStaticVoidMethod3(char*e_strClassName,char*e_strMethodName,char*e_strFunctionParameter,void*e_pInputData1,void*e_pInputData2,sJNIUtilData*e_pJNIUtilData,bool e_bCustomClass)
	{
		jclass l_Class;
		if( e_bCustomClass )
			 l_Class = GetCustomJavaClass(e_strClassName,e_pJNIUtilData);
		else
			 l_Class = e_pJNIUtilData->pJNIEnv->FindClass(e_strClassName);
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);
		jmethodID l_Method = e_pJNIUtilData->pJNIEnv->GetStaticMethodID(l_Class,e_strMethodName,e_strFunctionParameter);
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);
		e_pJNIUtilData->pJNIEnv->CallStaticVoidMethod(l_Class, l_Method,e_pInputData1,e_pInputData2);
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);
		e_pJNIUtilData->pJNIEnv->DeleteLocalRef(l_Class);
		return true;
	}

	bool	CallStaticVoidMethod4(char*e_strClassName,char*e_strMethodName,char*e_strFunctionParameter,void*e_pInputData1,void*e_pInputData2,void*e_pInputData3,sJNIUtilData*e_pJNIUtilData,bool e_bCustomClass)
	{
		jclass l_Class;
		if( e_bCustomClass )
			 l_Class = GetCustomJavaClass(e_strClassName,e_pJNIUtilData);
		else
			 l_Class = e_pJNIUtilData->pJNIEnv->FindClass(e_strClassName);
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);
		jmethodID l_Method = e_pJNIUtilData->pJNIEnv->GetStaticMethodID(l_Class,e_strMethodName,e_strFunctionParameter);
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);
		e_pJNIUtilData->pJNIEnv->CallStaticVoidMethod(l_Class, l_Method,e_pInputData1,e_pInputData2,e_pInputData3);
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);
		e_pJNIUtilData->pJNIEnv->DeleteLocalRef(l_Class);
		return true;	
	}

	//java/lang/ClassLoader make a while,so the best way is make a Init function to keep java/lang/ClassLoader alive
	bool	RegisterNativeFunction(const char*e_strPackageAndClassName,const char*e_strJAVAFunctionName,const char*e_strSignature,void*FunctionPointer,sJNIUtilData*e_pJNIUtilData)
	{
		JNINativeMethod nm[1];
		nm[0].name = e_strJAVAFunctionName;
		nm[0].signature = e_strSignature;
		nm[0].fnPtr = (void*)FunctionPointer;
		try
		{
			jstring strClassName = e_pJNIUtilData->pJNIEnv->NewStringUTF(e_strPackageAndClassName);
			EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);
			jclass classIWant = (jclass)e_pJNIUtilData->pJNIEnv->CallObjectMethod(e_pJNIUtilData->cls, e_pJNIUtilData->findClass, strClassName);
			EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);
			////Register methods with env->RegisterNatives.
			int	l_iResult = e_pJNIUtilData->pJNIEnv->RegisterNatives(classIWant, nm, 1);
			//https://www.java.net/node/676773
			e_pJNIUtilData->pJNIEnv->DeleteLocalRef(strClassName);
			e_pJNIUtilData->pJNIEnv->DeleteLocalRef(classIWant);
			return l_iResult==0?true:false;
		}
		catch(const std::exception& ex)
		{
			const char*l_str = ex.what();
			int a=0;
		}
		return false;
	}

	jclass	GetCustomJavaClass(const char*e_strPackageAndClassName,sJNIUtilData*e_pJNIUtilData)
	{
		jstring strClassName = e_pJNIUtilData->pJNIEnv->NewStringUTF(e_strPackageAndClassName);
		EXCEPTION_RETURN_NULL(e_pJNIUtilData->pJNIEnv);
		jclass classIWant = (jclass)e_pJNIUtilData->pJNIEnv->CallObjectMethod(e_pJNIUtilData->cls, e_pJNIUtilData->findClass, strClassName);
		e_pJNIUtilData->pJNIEnv->DeleteLocalRef(strClassName);
		return classIWant;
	}

	std::wstring	GetDeviceID()
	{
		std::wstring	l_str;
		jclass l_Class = GetCustomJavaClass("util/Define");
		jmethodID l_GetDeviceIDMethod = g_pMainThreadJNIUtilData->pJNIEnv->GetStaticMethodID(l_Class, "GetDeviceID", "()Ljava/lang/String;");
		jstring l_strDeviceID = (jstring)g_pMainThreadJNIUtilData->pJNIEnv->CallStaticObjectMethod(l_Class,l_GetDeviceIDMethod);
		l_str = JavaToWchar(g_pMainThreadJNIUtilData->pJNIEnv,l_strDeviceID);
		cGameApp::m_spJNIUtilData->pJNIEnv->DeleteLocalRef(l_Class);
		return  l_str;
	}

	int	GetDeviceIDToNumber()
	{
		int	l_iMagicID = 79979;
		std::wstring l_str = GetDeviceID();
		for(size_t i=0;i<l_str.length();++i)
		{
			l_iMagicID += l_str[i];
		}
		return l_iMagicID;
	}

	std::wstring JavaToWchar(JNIEnv *env, jstring string)
	{
		std::wstring value;

		const jchar *raw = env->GetStringChars(string, 0);
		jsize len = env->GetStringLength(string);
		//const jchar *temp = raw;

		value.assign(raw, raw + len);

		env->ReleaseStringChars(string, raw);
		return value;
	}

	jstring WcharToJava(JNIEnv *env,const WCHAR*e_str)
	{
		jchar	l_strTemp[MAX_PATH];
		int l_iLength = wcslen(e_str);
		if( l_iLength < 1 )
			return  0;
		for(int i=0;i<l_iLength;++i  )
		{
			l_strTemp[i] = e_str[i];
		}
		l_strTemp[l_iLength] = 0;
		return env->NewString(l_strTemp,l_iLength);
	}

	jbyteArray unsigned_array_to_jbyteArray(unsigned char* buf, int len,JNIEnv*e_pJNIEnv)
	{
		
		jbyteArray array = e_pJNIEnv->NewByteArray(len);
		e_pJNIEnv->SetByteArrayRegion(array, 0, len, reinterpret_cast<jbyte*>(buf));
		return array;
	}

	unsigned char* jbyteArray_to_unsigned_char_array(jbyteArray array, JNIEnv*e_pJNIEnv)
	{
		int len = e_pJNIEnv->GetArrayLength(array);
		unsigned char* buf = new unsigned char[len];
		e_pJNIEnv->GetByteArrayRegion(array, 0, len, reinterpret_cast<jbyte*>(buf));
		return buf;
	}
	//from nvidia TADP sample
	bool	OpenURL(const char*e_strURL,sJNIUtilData*e_pJNIUtilData)
	{
		// JNI is running the equivalent of the following Java code
		// Uri uri = Uri.parse(urlString);  
		// Intent launchBrowser = new Intent(Intent.ACTION_VIEW, uri);  
		// startActivity(launchBrowser);  

		// Convert the C string to a JNI string
		jstring urlText = e_pJNIUtilData->pJNIEnv->NewStringUTF(e_strURL);
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);

		// Find the URI class
		jclass uriClass = e_pJNIUtilData->pJNIEnv->FindClass("android/net/Uri");
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);

		// Find the ID of the URI-from-string parse function (static)
		jmethodID uriParse = e_pJNIUtilData->pJNIEnv->GetStaticMethodID(uriClass, "parse", 
			"(Ljava/lang/String;)Landroid/net/Uri;");
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);

		// Call: Uri uri = Uri.parse(urlString);  
		jobject uri = e_pJNIUtilData->pJNIEnv->CallStaticObjectMethod(uriClass, uriParse, urlText);
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);

		// Find the Intent class
		jclass intentClass = e_pJNIUtilData->pJNIEnv->FindClass("android/content/Intent");
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);

		// Get the static string field Intent.ACTION_VIEW
		jfieldID ACTION_VIEW_id = e_pJNIUtilData->pJNIEnv->GetStaticFieldID(intentClass, "ACTION_VIEW", 
			"Ljava/lang/String;");
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);

		jobject ACTION_VIEW = e_pJNIUtilData->pJNIEnv->GetStaticObjectField(intentClass, ACTION_VIEW_id);
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);

		// Get the initializer/constructor for Uri
		jmethodID newIntent = e_pJNIUtilData->pJNIEnv->GetMethodID(intentClass, "<init>", 
			"()V");
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);

		// Call Intent intent = new Intent(Intent.ACTION_VIEW, uri);  
		// Three stages: alloc object, then call init on it (init is the constructor)
		// then call the set functions
		// We SHOULD be able to set URI and Action in <init>; it works on HC and ICS,
		// but GB fails.  So this longer code is safer on GB.
		jobject intent = e_pJNIUtilData->pJNIEnv->AllocObject(intentClass); 
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);

		e_pJNIUtilData->pJNIEnv->CallVoidMethod(intent, newIntent);
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);

		// Find the function to set the URL
		jmethodID setURI = e_pJNIUtilData->pJNIEnv->GetMethodID(intentClass, "setData", 
			"(Landroid/net/Uri;)Landroid/content/Intent;");
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);

		// Set the URL
		e_pJNIUtilData->pJNIEnv->CallObjectMethod(intent, setURI, uri);
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);

		// Find the function to set the Action
		jmethodID setAction = e_pJNIUtilData->pJNIEnv->GetMethodID(intentClass, "setAction", 
			"(Ljava/lang/String;)Landroid/content/Intent;");
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);

		// Set the Action
		e_pJNIUtilData->pJNIEnv->CallObjectMethod(intent, setAction, ACTION_VIEW);
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);

		// startActivity(launchBrowser);  
		jclass activityClass = e_pJNIUtilData->pJNIEnv->FindClass("android/app/Activity");
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);

		jmethodID startActivity = e_pJNIUtilData->pJNIEnv->GetMethodID(activityClass, 
			"startActivity", "(Landroid/content/Intent;)V");
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);

		e_pJNIUtilData->pJNIEnv->CallVoidMethod(e_pJNIUtilData->ThreadThis, startActivity, intent);
		EXCEPTION_RETURN(e_pJNIUtilData->pJNIEnv);

		return true;
	}
	//http://www.dynadream.com/ddweb/index.php/Special_Blog?id=20

	bool	ShowAd(char*e_strADMobID,int e_iWidth,int e_iHeight,Vector2 e_vshowPos)
	{
		jclass	l_Class = GetCustomJavaClass("util/AdMobUtility");
		//jclass	l_Class = g_pMainThreadJNIUtilData->pJNIEnv->FindClass("util/AdMobUtility");
		jmethodID l_Method = g_pMainThreadJNIUtilData->pJNIEnv->GetStaticMethodID(l_Class,"Show","(Ljava/lang/String;II)V");
		jstring l_strADMobID = g_pMainThreadJNIUtilData->pJNIEnv->NewStringUTF(e_strADMobID);
		g_pMainThreadJNIUtilData->pJNIEnv->CallStaticVoidMethod(l_Class, l_Method,l_strADMobID,e_iWidth,e_iHeight);
		EXCEPTION_RETURN(g_pMainThreadJNIUtilData->pJNIEnv);
		g_pMainThreadJNIUtilData->pJNIEnv->DeleteLocalRef(l_Class);
		return true;
	}

	bool	ShowIntersitialAd(char*e_strADMobID)
	{
		jclass	l_Class = GetCustomJavaClass("util/AdMobUtility");
		//jclass	l_Class = g_pMainThreadJNIUtilData->pJNIEnv->FindClass("util/AdMobUtility");
		jmethodID l_Method = g_pMainThreadJNIUtilData->pJNIEnv->GetStaticMethodID(l_Class,"showInterstitialAd","(Ljava/lang/String;)V");
		jstring l_strADMobID = g_pMainThreadJNIUtilData->pJNIEnv->NewStringUTF(e_strADMobID);
		g_pMainThreadJNIUtilData->pJNIEnv->CallStaticVoidMethod(l_Class, l_Method,l_strADMobID);
		EXCEPTION_RETURN(g_pMainThreadJNIUtilData->pJNIEnv);
		g_pMainThreadJNIUtilData->pJNIEnv->DeleteLocalRef(l_Class);	
		return true;
	}

	bool	AndroidShowDialog(const char* title, const char *contents)
	{
		jstring jniTitle = g_pMainThreadJNIUtilData->pJNIEnv->NewStringUTF(title);
		EXCEPTION_RETURN(g_pMainThreadJNIUtilData->pJNIEnv);
		jstring jniContents = g_pMainThreadJNIUtilData->pJNIEnv->NewStringUTF(contents);
		EXCEPTION_RETURN(g_pMainThreadJNIUtilData->pJNIEnv);
		CallStaticVoidMethod3("util/Define", "showToastAlert", "(Ljava/lang/String;Ljava/lang/String;)V", jniTitle, jniContents, g_pMainThreadJNIUtilData, true);
		return true;
	}
}