/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/* This is a JNI example where we use native methods to play sounds
 * using OpenSL ES. See the corresponding Java source file located at:
 *
 *   src/com/example/nativeaudio/NativeAudio/NativeAudio.java
 */
//https://audioprograming.wordpress.com/2012/03/03/android-audio-streaming-with-opensl-es-and-the-ndk/
#include "../../../stdafx.h"
#include <assert.h>
#include <jni.h>
#include <string.h>
#include <pthread.h>
// for __android_log_print(ANDROID_LOG_INFO, "YourApp", "formatted message");
// #include <android/log.h>
// for native audio
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
// for native asset manager
#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <functional>
#include "../SoundCapture.h"
#include "../../GameplayUT/GameApp/GameApp.h"
// engine interfaces
static SLObjectItf engineObject = NULL;
static SLEngineItf engineEngine;
// a mutext to guard against re-entrance to record & playback
// as well as make recording and playing back to be mutually exclusive
// this is to avoid crash at situations like:
//    recording is in session [not finished]
//    user presses record button and another recording coming in
// The action: when recording/playing back is not finished, ignore the new request
// recorder interfaces
static SLObjectItf g_pRecorderObjectItf = NULL;
static SLRecordItf g_pRecorderRecordItf;
static SLAndroidSimpleBufferQueueItf recorderBufferQueue;
#define RECORD_BUFFER_COUNT	1
char*	g_pRecorderBuffer[RECORD_BUFFER_COUNT];
int     g_iCurrentRecorderBuffer = 0;
size_t	g_uiRecordBufferSize = 0;

void bqRecorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context);

namespace	FATMING_CORE
{
	extern cSoundCapture*g_pSoundCapture;
}
//https://github.com/googlesamples/android-ndk/blob/master/native-audio/app/src/main/cpp/native-audio-jni.c
//https://github.com/libretro/ppsspp-native/blob/master/android/native-audio-so.cpp
//https://stackoverflow.com/questions/18444354/save-recorded-audio-to-file-opensl-es-android
void	RecordingThread(size_t e_ui, size_t _pUri)
{

}
void	RecordingDoneThread(size_t _workParameter, size_t _pUri)
{
}

void	AndroidRecordPause(bool e_bPause)
{
	if( g_pRecorderRecordItf )
	{
		if( e_bPause )
			(*g_pRecorderRecordItf)->SetRecordState(g_pRecorderRecordItf, SL_RECORDSTATE_PAUSED);
		else
			(*g_pRecorderRecordItf)->SetRecordState(g_pRecorderRecordItf, SL_RECORDSTATE_RECORDING);
	}
}

void AndroidRecordStop()
{
	if( g_pRecorderRecordItf )
		(*g_pRecorderRecordItf)->SetRecordState(g_pRecorderRecordItf, SL_RECORDSTATE_STOPPED);
}

// create the engine and output mix objects
void CreateAndroidAudioEngine()
{
    SLresult result;

    // create engine
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // realize the engine
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // get the engine interface, which is needed in order to create other objects
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

}


//typedef struct SLDataFormat_PCM_ {
//	SLuint32 		formatType;
//	SLuint32 		numChannels;
//	SLuint32 		samplesPerSec;
//	SLuint32 		bitsPerSample;
//	SLuint32 		containerSize;
//	SLuint32 		channelMask;
//	SLuint32		endianness;
//} SLDataFormat_PCM;


SLuint32	FrequencyConvert(int e_iFrequency)
{
	if( e_iFrequency <= 8000 )
		return SL_SAMPLINGRATE_8;
	if( e_iFrequency <= 11025 )
		return SL_SAMPLINGRATE_11_025;
	if( e_iFrequency <= 12000 )
		return SL_SAMPLINGRATE_12;
	if( e_iFrequency <= 16000 )
		return SL_SAMPLINGRATE_16;
	if( e_iFrequency <= 22050 )
		return SL_SAMPLINGRATE_22_05;
	if( e_iFrequency <= 24000 )
		return SL_SAMPLINGRATE_24;
	if( e_iFrequency <= 32000 )
		return SL_SAMPLINGRATE_32;
	if( e_iFrequency <= 44100 )
		return SL_SAMPLINGRATE_44_1;
	if( e_iFrequency <= 48000 )
		return SL_SAMPLINGRATE_48;
	if( e_iFrequency <= 64000 )
		return SL_SAMPLINGRATE_64;
	if( e_iFrequency <= 88200 )
		return SL_SAMPLINGRATE_88_2;
	if( e_iFrequency <= 96000 )
		return SL_SAMPLINGRATE_96;
	return SL_SAMPLINGRATE_192;
}


// create audio recorder: recorder is not in fast path
//    like to avoid excessive re-sampling while playing back from Hello & Android clip
unsigned char NativeAudioCreateAudioRecorder(int e_iFrequency,int e_iSampleBitm,int e_iChannel)
{

	g_iCurrentRecorderBuffer = 0;
	for(int i=0;i<RECORD_BUFFER_COUNT;++i)
	{
		g_pRecorderBuffer[i] = nullptr;
	}

    SLresult result;
	SLuint32 l_Freq = FrequencyConvert(e_iFrequency);
    // configure audio source
    SLDataLocator_IODevice loc_dev = {SL_DATALOCATOR_IODEVICE, SL_IODEVICE_AUDIOINPUT,SL_DEFAULTDEVICEID_AUDIOINPUT, NULL};
    SLDataSource audioSrc = {&loc_dev, NULL};

    // configure audio sink
    SLDataLocator_AndroidSimpleBufferQueue loc_bq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, RECORD_BUFFER_COUNT};

	//frequency,int format,int buffersize
    SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, (SLuint32)e_iChannel, (SLuint32)l_Freq,SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN};
    SLDataSink audioSnk = {&loc_bq, &format_pcm};

    // create audio recorder
    // (requires the RECORD_AUDIO permission)
    const SLInterfaceID id[1] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};
    result = (*engineEngine)->CreateAudioRecorder(engineEngine, &g_pRecorderObjectItf, &audioSrc,&audioSnk, 1, id, req);
    if (SL_RESULT_SUCCESS != result) {
        return JNI_FALSE;
    }

    // realize the audio recorder
    result = (*g_pRecorderObjectItf)->Realize(g_pRecorderObjectItf, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result)
	{
        return JNI_FALSE;
    }

    // get the record interface
    result = (*g_pRecorderObjectItf)->GetInterface(g_pRecorderObjectItf, SL_IID_RECORD, &g_pRecorderRecordItf);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // get the buffer queue interface
    result = (*g_pRecorderObjectItf)->GetInterface(g_pRecorderObjectItf, SL_IID_ANDROIDSIMPLEBUFFERQUEUE,&recorderBufferQueue);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // register callback on the buffer queue
    result = (*recorderBufferQueue)->RegisterCallback(recorderBufferQueue, bqRecorderCallback,NULL);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    return JNI_TRUE;
}

void	DoEnqueue()
{
    // enqueue an empty buffer to be filled by the recorder
    // (for streaming recording, we would enqueue at least 2 empty buffers to start things off)
    (*recorderBufferQueue)->Enqueue(recorderBufferQueue, g_pRecorderBuffer[g_iCurrentRecorderBuffer],g_uiRecordBufferSize);
	++g_iCurrentRecorderBuffer;
	if( g_iCurrentRecorderBuffer >= RECORD_BUFFER_COUNT )
		g_iCurrentRecorderBuffer = 0;
}

// set the recording state for the audio recorder
void StartAndroidRecording(int e_iBufferSize)
{
	g_uiRecordBufferSize = e_iBufferSize;///sizeof(short);
    SLresult result;
    // in case already recording, stop recording and clear buffer queue
    result = (*g_pRecorderRecordItf)->SetRecordState(g_pRecorderRecordItf, SL_RECORDSTATE_STOPPED);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;
    result = (*recorderBufferQueue)->Clear(recorderBufferQueue);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;


	g_iCurrentRecorderBuffer = 0;
	for(int i=0;i<RECORD_BUFFER_COUNT;++i)
	{
		if(g_pRecorderBuffer[i])
		{
			delete g_pRecorderBuffer[i];
		}
		g_pRecorderBuffer[i] = new char[g_uiRecordBufferSize];
	}

	DoEnqueue();
    // the most likely other result is SL_RESULT_BUFFER_INSUFFICIENT,
    // which for this code example would indicate a programming error
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // start recording
    result = (*g_pRecorderRecordItf)->SetRecordState(g_pRecorderRecordItf, SL_RECORDSTATE_RECORDING);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;
}

//1 seond 44100
short g_pAndroidTempRecordingData[44100];
// this callback handler is called every time a buffer finishes recording
UT::sTimeAndFPS g_OPenSLTestTimeCounter;
void bqRecorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
	g_OPenSLTestTimeCounter.Update();
	char*l_str = g_OPenSLTestTimeCounter.GetFPS();
	cGameApp::OutputDebugInfoString(l_str);
    assert(bq == recorderBufferQueue);
    assert(NULL == context);
	//fuck I am lazy
	int l_iGetProperIndex = g_iCurrentRecorderBuffer;
	if( l_iGetProperIndex == 0)
	{
		l_iGetProperIndex = 1;
	}
	else
		l_iGetProperIndex = 0;
//
	//char*l_pData = g_pRecorderBuffer[l_iGetProperIndex];
	//short*l_pData = g_pRecorderBuffer[0];
	//memcpy(g_pAndroidTempRecordingData,g_pRecorderBuffer[l_iGetProperIndex],g_uiRecordBufferSize);
	//memset(g_pAndroidTempRecordingData,0,sizeof(g_pAndroidTempRecordingData));
	memcpy(g_pAndroidTempRecordingData,g_pRecorderBuffer[0],g_uiRecordBufferSize);
	g_pSoundCapture->AddFileSize(g_uiRecordBufferSize);
	int l_iNumCount = g_pSoundCapture->Count();
	auto l_CallbackObjectVector = g_pSoundCapture->GetList();
	for( int i=0;i<l_iNumCount;++i )
	{
		//(*l_CallbackObjectVector)[i]->CaptureSoundNewDataCallBack(g_uiRecordBufferSize,(char*)g_pAndroidTempRecordingData);
		//(*l_CallbackObjectVector)[i]->CaptureSoundNewDataCallBack(g_uiRecordBufferSize,(char*)g_pRecorderBuffer[l_iGetProperIndex]);
		(*l_CallbackObjectVector)[i]->CaptureSoundNewDataCallBack(g_uiRecordBufferSize/sizeof(short),(char*)g_pAndroidTempRecordingData);
	}
	DoEnqueue();
}

// shut down the native audio system
void AndroidRecordShutdown()
{

    // destroy audio recorder object, and invalidate all associated interfaces
    if (g_pRecorderObjectItf != NULL) {
        (*g_pRecorderObjectItf)->Destroy(g_pRecorderObjectItf);
        g_pRecorderObjectItf = NULL;
        g_pRecorderRecordItf = NULL;
        recorderBufferQueue = NULL;
    }


    // destroy engine object, and invalidate all associated interfaces
    if (engineObject != NULL) {
        (*engineObject)->Destroy(engineObject);
        engineObject = NULL;
        engineEngine = NULL;
    }
	for(int i=0;i<RECORD_BUFFER_COUNT;++i)
	{
		if(g_pRecorderBuffer[i])
		{
			delete g_pRecorderBuffer[i];
			g_pRecorderBuffer[i] = nullptr;
		}
	}
}