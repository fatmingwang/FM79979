if the VM can not start please close program and try again

referende sample C:\NVPACK\TDK_Samples\tegra_android_native_samples_v10p10\sample_apps

lib location

C:\NVPACK\TDK_Samples\tegra_android_native_samples_v10p10\libs\jni\nv_obj\Debug


-landroid

// with nv is Nvidia's easy using libs

-lnv_and_util
-lnv_bitfont
-lnv_egl_util
-lnv_file
-lnv_glesutil
-lnv_hhdds
-lnv_log
-lnv_math
-lnv_shader
-lnv_thread

-lstdc++
-lc
-lm
-llog
-ldl
-lGLESv2
-lEGL
-lOpenSLES


dynamic_cast need command line
--rtti

platformdefine.h

string compress.cpp
__cdecl

lost SmartPointer.h

cBinaryFile

exception enable commandline -fexceptions


openAL porting

bullet porting





pure NDK is slight defferent with java code most function from pool won't be called
ex: Accelerometer


May 23 2014
the JNI 
jclass once is has been assigned must call DeleteLocalRef
or it will occur reference overflow
ex:check AndroidVideoPlayer_OneThread.cpp
	jclass bitmapCls = env->GetObjectClass(jbitmap);
	env->DeleteLocalRef(bitmapCls);



May 27 2014
multi thread in NDK
if oyu need JNI in the multhread u have to call
int	res = cGameApp::m_spANativeActivity->vm->AttachCurrentThread(&g_pMultiThreadEnv,NULL);
and after leave thread call
cGameApp::m_spANativeActivity->vm->DetachCurrentThread();
or it will show multithread problem.
take a see at AndroidVideoPlayer void	VideoThread(size_t _workParameter, size_t _pUri)



Jun 4 2014
sd card
external sd card is different
most device has sd card and it's embeded
the external sd card is the card plugin.

June 25 2014
if the device cannot start to debug check AndroidManifest.xml
the 
android:debuggable="false"
if true cannot start to debug


upload apk the version code must to be change
and debug must be false
and if want to test in google+ coominucation must publish apk
