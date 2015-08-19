#ifndef _ANDROID_JNI_H_
#define _ANDROID_JNI_H_

#include "jni.h"
//JNIEnv*								cGameApp::m_spThreadEnv = 0;


//http://docs.oracle.com/javase/1.5.0/docs/guide/jni/spec/types.html

	//those code are declar in the java side.
	//
	//
	//private static native int naInit(String pFileName); 
	//private static native int[] naGetVideoRes();
	//private static native void naSetSurface(Surface pSurface);
	//private static native int naSetup(int pWidth, int pHeight);
	//private static native void naPlay();
	//private static native void naStop();
	//
	//u could registor it as below in c++.
	//JNIEnv* env;
	//if ((*pVm)->GetEnv(pVm, (void **)&env, JNI_VERSION_1_6) != JNI_OK) {
	//	 return -1;
	//}
	//JNINativeMethod nm[8];
	//nm[0].name = "naInit";
	//nm[0].signature = "(Ljava/lang/String;)I";
	//nm[0].fnPtr = (void*)naInit;

	//nm[1].name = "naSetSurface";
	//nm[1].signature = "(Landroid/view/Surface;)V";
	//nm[1].fnPtr = (void*)naSetSurface;

	//nm[2].name = "naGetVideoRes";
	//nm[2].signature = "()[I";
	//nm[2].fnPtr = (void*)naGetVideoRes;

	//nm[3].name = "naSetup";
	//nm[3].signature = "(II)I";
	//nm[3].fnPtr = (void*)naSetup;

	//nm[4].name = "naPlay";
	//nm[4].signature = "()V";
	//nm[4].fnPtr = (void*)naPlay;

	//nm[5].name = "naStop";
	//nm[5].signature = "()V";
	//nm[5].fnPtr = (void*)naStop;

	//jclass cls = (*env)->FindClass(env, "roman10/tutorial/android_ffmpeg_tutorial02/MainActivity");
	////Register methods with env->RegisterNatives.
	//(*env)->RegisterNatives(env, cls, nm, 6);
	//http://www.cnblogs.com/273809717/archive/2013/02/21/2921058.html
//http://www.wikihow.com/Find-the-IMEI-Number-on-a-Mobile-Phone
//get telephone service
 //JNIEnv &env = ... 
 //  jobject activity = ... (android.app.Activity object) ;
	//TelephonyManager telephony_manager = getSystemService(Context.TELEPHONY_SERVICE); 
 //  jmethodID mid = env.GetMethodID(env.GetObjectClass(activity), "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;"); 
 //  jobject telephony_manager = env.CallObjectMethod(activity, mid, env.NewStringUTF("phone")); 
 //                             //String s_imei = tm.getDeviceId(); 
 //  mid = env.GetMethodID(env.GetObjectClass(telephony_manager), "getDeviceId", "()Ljava/lang/String;"); 
 //  jstring s_imei = (jstring)env.CallObjectMethod(telephony_manager, mid); 
 //  if(env.ExceptionCheck())
 //  { 
 //     env.ExceptionClear(); 
 //     return false; 
 //  } 

#endif