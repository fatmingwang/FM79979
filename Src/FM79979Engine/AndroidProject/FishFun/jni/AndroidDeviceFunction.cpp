#ifndef WIN32
#include <jni.h>
#include "../../../FishingFun/Win32Test/stdafx.h"
#include "../../../FishingFun/Win32Test/GameApp.h"
#include "../../../FishingFun/Win32Test/IABSetup.h"
//#include "../../../BluffingGirl/EngineTest/IABSetup.h"


//Signature	 Java Type
//Z	 boolean
//B	 byte
//C	 char
//S	 short
//I	 int
//J	 long
//F	 float
//D	 double
//L fully-qualified-class ;	 fully-qualified-class
//[ type	 type[]
//( arg-types ) ret-type	 method type

bool LaunchURL(const char* urlString)
{
	// JNI is running the equivalent of the following Java code
	// Uri uri = Uri.parse(urlString);  
	// Intent launchBrowser = new Intent(Intent.ACTION_VIEW, uri);  
	// startActivity(launchBrowser);  

	// Convert the C string to a JNI string
	
    jstring urlText = cGameApp::m_spThreadEnv->NewStringUTF(urlString);
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);

	// Find the URI class
	jclass uriClass = cGameApp::m_spThreadEnv->FindClass("android/net/Uri");
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);

	// Find the ID of the URI-from-string parse function (static)
	jmethodID uriParse = cGameApp::m_spThreadEnv->GetStaticMethodID(uriClass, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);

	// Call: Uri uri = Uri.parse(urlString);  
	jobject uri = cGameApp::m_spThreadEnv->CallStaticObjectMethod(uriClass, uriParse, urlText);
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);

	// Find the Intent class
	jclass intentClass = cGameApp::m_spThreadEnv->FindClass("android/content/Intent");
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);

	// Get the static string field Intent.ACTION_VIEW
	jfieldID ACTION_VIEW_id = cGameApp::m_spThreadEnv->GetStaticFieldID(intentClass, "ACTION_VIEW", 
		"Ljava/lang/String;");
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);

	jobject ACTION_VIEW = cGameApp::m_spThreadEnv->GetStaticObjectField(intentClass, ACTION_VIEW_id);
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);

	// Get the initializer/constructor for Uri
	jmethodID newIntent = cGameApp::m_spThreadEnv->GetMethodID(intentClass, "<init>", 
		"()V");
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);

	// Call Intent intent = new Intent(Intent.ACTION_VIEW, uri);  
	// Three stages: alloc object, then call init on it (init is the constructor)
	// then call the set functions
	// We SHOULD be able to set URI and Action in <init>; it works on HC and ICS,
	// but GB fails.  So this longer code is safer on GB.
	jobject intent = cGameApp::m_spThreadEnv->AllocObject(intentClass); 
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);

	cGameApp::m_spThreadEnv->CallVoidMethod(intent, newIntent);
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);

	// Find the function to set the URL
	jmethodID setURI = cGameApp::m_spThreadEnv->GetMethodID(intentClass, "setData", 
		"(Landroid/net/Uri;)Landroid/content/Intent;");
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);

	// Set the URL
	cGameApp::m_spThreadEnv->CallObjectMethod(intent, setURI, uri);
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);

	// Find the function to set the Action
	jmethodID setAction = cGameApp::m_spThreadEnv->GetMethodID(intentClass, "setAction", 
		"(Ljava/lang/String;)Landroid/content/Intent;");
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);

	// Set the Action
	cGameApp::m_spThreadEnv->CallObjectMethod(intent, setAction, ACTION_VIEW);
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);

	// startActivity(launchBrowser);  
	jclass activityClass = cGameApp::m_spThreadEnv->FindClass("android/app/Activity");
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);

	jmethodID startActivity = cGameApp::m_spThreadEnv->GetMethodID(activityClass, 
		"startActivity", "(Landroid/content/Intent;)V");
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);

	cGameApp::m_spThreadEnv->CallVoidMethod(*cGameApp::m_spAppThreadThis, startActivity, intent);
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);

	return true;
}

jclass GetJavaClass(char*e_strClassPath)
{
	return cGameApp::m_spThreadEnv->FindClass(e_strClassPath);
}
//ex:e_strInputParameter "([Ljava/lang/String;)V"
jmethodID GetJavaClassMethod(jclass e_Class,const char*e_strFunctionName,const char*e_strInputParameter)
{
	return cGameApp::m_spThreadEnv->GetMethodID(e_Class,e_strFunctionName,e_strInputParameter);
}
jmethodID GetJavaClassStaticMethod(jclass e_Class,char*e_strFunctionName,char*e_strInputParameter)
{
	return cGameApp::m_spThreadEnv->GetStaticMethodID(e_Class,e_strFunctionName,e_strInputParameter);
}
//e_strClassPath = "example/jni/InvocationHelloWorld"
void InvokeClassMethod(char*e_strClassPath,const char*e_strFunctionName,const char*e_strInputParameter)
{
	jclass l_Class = GetJavaClass(e_strClassPath);
	jmethodID l_Method = GetJavaClassMethod(l_Class,e_strFunctionName,e_strInputParameter);
	cGameApp::m_spThreadEnv->CallVoidMethod( *cGameApp::m_spAppThreadThis, l_Method);
}
void InvokeClassStaticMethod(char*e_strClassPath,const char*e_strFunctionName,const char*e_strInputParameter)
{
	jclass l_Class = GetJavaClass(e_strClassPath);
	jmethodID l_Method = GetJavaClassMethod(l_Class,e_strFunctionName,e_strInputParameter);
	cGameApp::m_spThreadEnv->CallStaticVoidMethod(l_Class, l_Method);
}

//http://www3.ntu.edu.sg/home/ehchua/programming/java/JavaNativeInterface.html
//I for int
//if data type is I,do not need ; to sperate
//bwlow input data type is
//int,String,String,I, and return value is void
//"(ILjava/lang/String;Ljava/lang/String;I)V")
//
//jmethodID midToString = (*env)->GetMethodID(env, cls, "toString", "()Ljava/lang/String;");
//if (NULL == midToString) return NULL;
//jstring resultStr = (*env)->CallObjectMethod(env, newObj, midToString);
//
//
bool		CallIABRCreate()
{
	jclass thisClass = cGameApp::m_spThreadEnv->GetObjectClass(*cGameApp::m_spAppThreadThis);
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	jmethodID l_IABCreateMethod = cGameApp::m_spThreadEnv->GetMethodID(thisClass,"IABCreate", "()V");
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	cGameApp::m_spThreadEnv->CallVoidMethod(*cGameApp::m_spAppThreadThis, l_IABCreateMethod);
	return true;
}

bool		FBPost()
{
	jclass thisClass = cGameApp::m_spThreadEnv->GetObjectClass(*cGameApp::m_spAppThreadThis);
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	jmethodID l_FBPostMethod = cGameApp::m_spThreadEnv->GetMethodID(thisClass,"FBPost", "()V");
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	cGameApp::m_spThreadEnv->CallVoidMethod(*cGameApp::m_spAppThreadThis, l_FBPostMethod);
	return true;
}

bool		AddSKUData(const char* e_strSKUID,const char* e_strName,int e_iType,bool e_bConsumable)
{
	jclass thisClass = cGameApp::m_spThreadEnv->GetObjectClass(*cGameApp::m_spAppThreadThis);
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	jmethodID l_AddSKUData = cGameApp::m_spThreadEnv->GetMethodID(thisClass,"AddSKUData", "(Ljava/lang/String;Ljava/lang/String;IZ)V");
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	jstring l_strSkuID = cGameApp::m_spThreadEnv->NewStringUTF(e_strSKUID);
	jstring l_strProductName = cGameApp::m_spThreadEnv->NewStringUTF(e_strName);
	cGameApp::m_spThreadEnv->CallVoidMethod(*cGameApp::m_spAppThreadThis, l_AddSKUData,l_strSkuID,l_strProductName,e_iType,(unsigned char)e_bConsumable);
	return true;
}

bool			PurchaseProduct(const char*e_strProductionID,ePurchaseType e_ePurchaseType,char*e_strPayLoadDataForCheck)
{
	jclass thisClass = cGameApp::m_spThreadEnv->GetObjectClass(*cGameApp::m_spAppThreadThis);
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	jmethodID l_DoPurchaseProductsMethod = cGameApp::m_spThreadEnv->GetMethodID(thisClass,"DoPurchaseProducts", "(ILjava/lang/String;)V");
	EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
	jstring l_strProductID = cGameApp::m_spThreadEnv->NewStringUTF(e_strProductionID);
	cGameApp::m_spThreadEnv->CallVoidMethod(*cGameApp::m_spAppThreadThis, l_DoPurchaseProductsMethod,(int)e_ePurchaseType,l_strProductID);
	return true;
}
//
//
//bool		IsProductsPurchased(char*e_strProductionID,ePurchaseType e_ePurchaseType)
//{
//	return false;
//}

bool		IsWaitingForRespond()
{
	return false;
}
//win32
#endif