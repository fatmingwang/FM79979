#ifdef	ANDROID
#include <jni.h>
#include <stdio.h>
#include <android/native_window_jni.h>

#define CURL_STATICLIB


//#include "../../../BluffingGirl/EngineTest/MyJni.h"
//http://docs.oracle.com/javase/1.5.0/docs/guide/jni/spec/types.html
float			g_fSensorValue[3] = {1.f,1.f,1.f};
float			g_fForce = 0.f;


//it's possible still cosume failed but just give player happy lah
//void Java_util_iab_IABManager_ItemNotCosumeButNotPurchasedFillBackToPlayer(JNIEnv *env, jobject obj,jstring e_str)
//{
//	int a=0;
//	int b=a;	
//}
////after cosume or preume purchased
//void Java_util_iab_IABManager_ItemPurchaseSuccess(JNIEnv *env, jobject obj,jstring e_str)
//{
//	const char *nativeString = (*env)->GetStringUTFChars(env, e_str, 0);
//	//ItemPurchaseSuccess(nativeString);
//   (*env)->ReleaseStringUTFChars(env, env, nativeString);
//}
////purchased but cosume failed,reconnect again?
//void Java_util_iab_IABManager_ItemCosumeFailed(JNIEnv *env, jobject obj)
//{
//	int a=0;
//	int b=a;	
//}
////connection failed or something else?
//void Java_util_iab_IABManager_ItemPurchaseFailed(JNIEnv *env, jobject obj)
//{
//	//ItemPurchaseFailed();
//	int a=0;
//	int b=a;
//}
//
////
//void Java_util_iab_IABManager_SetWaitForRespond(JNIEnv *env, jobject obj,jboolean e_b)
//{
//	//SetWaitForRespond(e_b?1:0);
//	int a=0;
//	int b=a;
//}
//
////connection failed or something else?
//void Java_util_iab_IABManager_IABErrorMessage(JNIEnv *env, jobject obj,jstring e_strMessage)
//{
//	const char *nativeString = (*env)->GetStringUTFChars(env, e_strMessage, 0);
//   (*env)->ReleaseStringUTFChars(env, e_strMessage, nativeString);
//	int a=0;
//	int b=a;
//}


void Java_util_Define_SensorChange( JNIEnv* env, jobject obj,jfloat e_fX,jfloat e_fY,jfloat e_fZ)
{
	g_fSensorValue[0] = e_fX;
	g_fSensorValue[1] = e_fY;
	g_fSensorValue[2] = e_fZ;
}

void Java_util_Define_SensorForce( JNIEnv* env, jobject obj,jfloat e_fX)
{
	g_fForce = e_fX;
}


//void Java_util_iab_IabHelper_IABProductInfo(JNIEnv* env,jobject thiz,  jstring e_strSKU, jstring e_strPrice)
//{
//	const char *l_strSKU = (*env)->GetStringUTFChars(env, e_strSKU, 0);
//	const char *l_strPrice = (*env)->GetStringUTFChars(env, e_strPrice, 0);
//
//	(*env)->ReleaseStringUTFChars(env,e_strSKU,l_strSKU);
//	(*env)->ReleaseStringUTFChars(env,e_strPrice,l_strPrice);
//}
//
//jstring Java_util_iab_IABManager_GetBase64EncodedPublicKey(JNIEnv* env,jobject thiz)
//{
//	//for TW
//	//const char*l_strBase64EncodedPublicKey  ="MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA1JzAkyi+vBs8GwwqIEm7rQrw4sB78m5Zjn1u4YJw1KDy+oTEmnKKFKSZTGmUXpEb/g0XI9r6NoHOQpf/lPloOPgZ49/qq8aEF4iKO+5cXcmG5C4vBlzV+ZalU4eRsviY9ghwBMhY/9Dc8/sbrGfIbg1pSIKHJPD/L+DqnXeDKsHaxzctL9LIAhl/RRUASpCyHSL5Bk4kPKQ/B6DK9Nz3SvBlCZX1ttWbpagsf5Faqkbh1kWQU7O6Vn/jEh09M1beUTE+QzwBMvKwtqGrZbw5u8L9HJXIkV4LjbHSMlMZGmud/MQo2g482/5ZVbhvyN1solaCcM3Ljtn8Y4m7+sVHUQIDAQAB";
//	//for EN
//	const char*l_strBase64EncodedPublicKey ="MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA5UBr8+/FLR38FVKgU6zE1GgEESczy7bVlLAgsdz9jglFMoFOk13LNguRL7BhGOr0M9BsEa2534/U+T7AeZZk+muGgitECEk7md2sy91AQBY8+gCq7AYoyzw3JUu8m7hn6wBHvzRPEWIVjFJlBucW9mqqeeiIJqxtMJzv66oN6yGZqhE+fkBga3jVNU46DJRDnumg7ghne2BQAHvASkocseR+bjzuwop9T7sRAS9jPPuR2voxflghns24/yzVuh9MLmgFVwiZAL54ghITDJgBI+iVzJbV36AJaOGGrQ1G04z/h+p0HkGqf9kf+cHqk5OK4hAclBZ0I4v3WAz6OuIE6QIDAQAB";
//	return (*env)->NewStringUTF(env, l_strBase64EncodedPublicKey);
//}
//void Java_util_facebook_FBManager_FBShareOk(JNIEnv* env,jobject thiz)
//{
//	int a=0;
//}


#endif