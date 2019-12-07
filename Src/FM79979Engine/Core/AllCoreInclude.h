#pragma once

//if platfor is android command line make u has set (--rtti -fexceptions )
#ifndef ANDROID
//#define ANDROID
//lot of undefined reference to ... errors
//Application.mk  file LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
//http://stackoverflow.com/questions/7209971/ndk-build-undefined-reference-to-errors-when-statically-linking-to-libxml-a
#endif

#if defined(WIN32)
#pragma warning( disable : 4996 )
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <math.h>
#include "XML/AtgXmlWriter.h"
#endif

#include "AllCommonInclude.h"
#include "AllMathInclude.h"
//#ifndef IOS
#include "Sound/AllSoundInclude.h"
//#endif
//#ifndef IOS
//#include "AVIPlayer/AVIPlayer.h"
//#endif
#include "GameplayUT/AllGamePlayUTInclude.h"

#include "Synchronization/AllFUThreadInclude.h"
#ifdef ANDROID
#include "Android/JNIUtil.h"
#endif
using namespace FATMING_CORE;