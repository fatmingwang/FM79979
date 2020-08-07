//#ifdef USE_OLD_OPENCV
#include "AndroidVideoPlayer.h"
#include "../../OpenGL/Glh.h"
#include "../../GameApp/GameApp.h"
#include "../CommonRender/BaseImage.h"
#include "../Texture/jpeg/jpge.h"
#include "../Texture/jpeg/jpgd.h"
#ifdef WIN32
char*g_strCommonVideoFS = "													\
						  uniform sampler2D texSample;								\
						  varying vec2 PSTexcoord;									\
						  varying vec4 PSColor;										\
						  \
						  void main()													\
						  {															\
						  vec4 l_vColor = texture2D(texSample, PSTexcoord)+(PSColor-vec4(1,1,1,1));\
						  gl_FragColor = l_vColor.zyxw;\
}";


#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;

//opencv_calib3d
//opencv_contrib
//opencv_core
//opencv_features2d
//opencv_flann
//opencv_highgui
//opencv_imgproc
//opencv_legacy
//opencv_ml
//opencv_objdetect
//opencv_photo
//opencv_stitching
//opencv_video
//opencv_videostab
#else
const char*g_strCommonVideoFS = "														\
uniform sampler2D texSample;													\
varying lowp vec2 PSTexcoord;													\
varying lowp vec4 PSColor;														\
\
void main()																		\
{																				\
	gl_FragColor = texture2D(texSample, PSTexcoord)+(PSColor-vec4(1,1,1,1));	\
	gl_FragColor.xyzw = gl_FragColor.zyxw;										\
}";
#endif
#ifdef ANDROID
#include <unistd.h>
#include <jni.h>
#include <android/bitmap.h>
JNIEnv*		g_pMultiThreadEnv = 0;
jclass		g_bitmap_factory_class = 0;
jmethodID	g_decode_byte_array_method = 0;
jclass		g_BitmapConfigClass = 0;
jmethodID	g_bmpClsValueOfMid = 0;
jobject		g_jBmpCfg = 0;
//http://docs.oracle.com/javase/1.5.0/docs/guide/jni/spec/types.html
bool	BitmapReduce(jobject e_object,JNIEnv* e_env,bool e_bDeleteLocalRef)
{
	jclass bitmapCls = e_env->GetObjectClass(e_object);
	jmethodID l_RecycleFunction = e_env->GetMethodID(bitmapCls, "recycle", "()V");
	jmethodID l_IsRecycleFunction = e_env->GetMethodID(bitmapCls, "isRecycled", "()Z");

	//if( l_IsRecycleFunction )
	//{
	//	jobject l_bIsRecycled = g_pMultiThreadEnv->CallObjectMethod(bitmapCls,l_IsRecycleFunction);
	//	bool	l_b = (bool)l_bIsRecycled;
	//	if (!l_b && l_RecycleFunction )
	//	{
	//		e_env->CallVoidMethod(bitmapCls, l_RecycleFunction);
	//	}
	//	e_env->DeleteLocalRef(l_bIsRecycled);
	//}
	if( e_bDeleteLocalRef )
	{
		e_env->DeleteLocalRef(e_object);
	}
	e_env->DeleteLocalRef(bitmapCls);
	return true;
}

jobject CopyBitmapToRGBA8888(jobject e_OldBitmap)
{
	jclass bitmapCls = g_pMultiThreadEnv->GetObjectClass(e_OldBitmap);
	jmethodID l_CopyMethod = g_pMultiThreadEnv->GetMethodID(bitmapCls, "copy", "(Landroid/graphics/Bitmap$Config;Z)Landroid/graphics/Bitmap;");
	if( l_CopyMethod )
	{
		jobject l_Newjbitmap = g_pMultiThreadEnv->CallObjectMethod(e_OldBitmap,l_CopyMethod,g_jBmpCfg,0);
		g_pMultiThreadEnv->DeleteLocalRef(bitmapCls);
		if( l_Newjbitmap )
		{
			return l_Newjbitmap;
		}
	}
	g_pMultiThreadEnv->DeleteLocalRef(bitmapCls);
	return  nullptr;
}



bool JpegToRawPixelData(char*e_pJpegData,int e_iLength,char*e_pStorePixelData)
{
	jbyteArray byte_array = g_pMultiThreadEnv->NewByteArray(e_iLength);
	g_pMultiThreadEnv->SetByteArrayRegion(byte_array, 0, e_iLength, (jbyte *)e_pJpegData);
	//get the bitmap itself
	jobject l_JpegToBitmapObject = g_pMultiThreadEnv->CallStaticObjectMethod(g_bitmap_factory_class, g_decode_byte_array_method,byte_array, 0, e_iLength);
	g_pMultiThreadEnv->DeleteLocalRef(byte_array);
	AndroidBitmapInfo bitmapInfo;
	int ret;
	if ((ret = AndroidBitmap_getInfo(g_pMultiThreadEnv, l_JpegToBitmapObject, &bitmapInfo)) < 0)
	{
		g_pMultiThreadEnv->DeleteLocalRef(l_JpegToBitmapObject);
		return false;
	}
	if(bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888)
	{
		jobject l_Newjbitmap = CopyBitmapToRGBA8888(l_JpegToBitmapObject);
		if( l_Newjbitmap )
		{
			BitmapReduce(l_JpegToBitmapObject,g_pMultiThreadEnv,true);
			l_JpegToBitmapObject = l_Newjbitmap;
			if ((ret = AndroidBitmap_getInfo(g_pMultiThreadEnv, l_JpegToBitmapObject, &bitmapInfo)) < 0)
			{
				g_pMultiThreadEnv->DeleteLocalRef(l_JpegToBitmapObject);
				return false;
			}
			if(bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888)
			{
				BitmapReduce(l_JpegToBitmapObject,g_pMultiThreadEnv,true);
				l_JpegToBitmapObject = 0;				
			}
		}
		else
		{
			BitmapReduce(l_JpegToBitmapObject,g_pMultiThreadEnv,true);
			l_JpegToBitmapObject = 0;
		}
		if( !l_JpegToBitmapObject )
		{
			return false;
		}
	}
	void* bitmapPixels;
	if ((ret = AndroidBitmap_lockPixels(g_pMultiThreadEnv, l_JpegToBitmapObject, &bitmapPixels)) == 0)
	{
		uint32_t* src = (uint32_t*) bitmapPixels;
		int pixelsCount = bitmapInfo.height * bitmapInfo.width;
		memcpy(e_pStorePixelData, src, sizeof(uint32_t) * pixelsCount);
		AndroidBitmap_unlockPixels(g_pMultiThreadEnv, l_JpegToBitmapObject);
		//recycle bitmap
		BitmapReduce(l_JpegToBitmapObject,g_pMultiThreadEnv,true);;
		return true;
	}
	return false;
}
#endif
#ifdef WIN32
int	g_iVideoColorType = GL_RGB;
int	g_iVideoChannel = 3;
#else
int	g_iVideoColorType = GL_RGBA;
int	g_iVideoChannel = 4;
#endif

wchar_t*g_strVideoShaderName = L"VideoShader";
cVideoPlayer::cVideoPlayer()
{
	m_fTotalPlayTime = 56.f;
	m_fCurrentPlayTime = 0.f;
#ifndef ANDROID
	m_pCvCapture = new cv::VideoCapture();
#endif
	m_fRestTimeToNextFrame = 0.f;
	m_fFPS = 1/24.f;
	m_pVideoImage = 0;//
	m_pVideoImage = new FATMING_CORE::cBaseImage((const char*)0);
	if( m_pVideoImage )
	{
		m_pVideoImage->SetWidth((int)cGameApp::m_spOpenGLRender->m_vGameResolution.x);
		m_pVideoImage->SetHeight((int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	}
	m_iTotalFrame = 0;
}

cVideoPlayer::~cVideoPlayer()
{
#ifdef WIN32
	SAFE_DELETE(m_pCvCapture);
#endif
	SAFE_DELETE(m_pVideoImage);
}

int		cVideoPlayer::GetCurrentFrame(float e_fTime)
{
	int	l_iFrame = (int)(this->m_iTotalFrame*(m_fCurrentPlayTime/this->m_fTotalPlayTime));;
	if( l_iFrame < 0 )
		l_iFrame = 0;
	if( l_iFrame >= this->m_iTotalFrame )
		l_iFrame = m_iTotalFrame-1;
	return l_iFrame;
}

bool	cVideoPlayer::OpenFile(const char*e_strFileName)
{
#ifdef WIN32
	//http://www.ymer.org/amir/2007/06/04/getting-the-number-of-frames-of-an-avi-video-file-for-opencv/
	//m_pCvCapture->get(CV_CAP_PROP_FRAME_COUNT);
	try
	{
		//if return null please find opencv_ffmpeg249.dll.
		m_pCvCapture->open(e_strFileName);
	}
	catch( cv::Exception& e )
	{
	    const char* err_msg = e.what();
		UT::ErrorMsg(err_msg,"error!");
	}
	if( m_pCvCapture && m_pCvCapture->isOpened())
	{
		m_iTotalFrame = (int)m_pCvCapture->get(cv::CAP_PROP_FRAME_COUNT);
		m_fFPS = (float)m_pCvCapture->get(cv::CAP_PROP_FPS);
		m_fTotalPlayTime = m_iTotalFrame/m_fFPS;
		return true;
	}
#endif
	return false;
}

void    cVideoPlayer::Update(float e_fElpaseTime)
{
#ifdef WIN32
	m_fRestTimeToNextFrame -= e_fElpaseTime;
	if( m_fRestTimeToNextFrame < 0 )
	{
		m_fRestTimeToNextFrame += m_fFPS;
		cv::Mat l_Frame;
		if (m_pCvCapture->read(l_Frame))
		{
			
			auto l_ColorType = GL_RGB;
#ifdef WIN32	
			l_ColorType = GL_BGR;
#endif
			m_pVideoImage->GetTexture()->SetupTexture(3, l_Frame.rows, l_Frame.cols, GL_BGR, GL_UNSIGNED_BYTE, false,
				l_Frame.data, false);
		}
	}
#endif
	return;
}
void    cVideoPlayer::Render()
{
	if( m_pVideoImage )
		m_pVideoImage->Render();
}
bool	cVideoPlayer::IsPlayDone()
{
	return false;
}

cVideoConvert::cVideoConvert()
{
	m_iTargetWidth = -1;
	m_iTargetHeight = -1;
	m_iQuality = 100;
	m_iCurrentFrame = 0;
	m_iMaximumTampFileLength = 0;
	m_pTempData = 0;
	m_puiFrameFilePosVector = 0;
	m_puiFrameSizeVector = 0;
}

cVideoConvert::~cVideoConvert()
{
	SAFE_DELETE(m_pTempData);
	SAFE_DELETE(m_puiFrameFilePosVector);
	SAFE_DELETE(m_puiFrameSizeVector);
}

bool	cVideoConvert::SaveFrameToFile(int e_iFrame,void*e_pIplImage,NvFile*e_pFile)
{
#ifdef WIN32
	if( !e_pIplImage || !e_pFile )
		return false;
	cv::Mat*l_pImageFrame = nullptr;
	cv::Mat*l_pIplImage = (cv::Mat*)e_pIplImage;
	cv::Mat l_ScaleImage;
	if( m_iTargetHeight != -1 && m_iTargetWidth != -1 )
	{
		cv::resize(*l_pIplImage,l_ScaleImage, cv::Size(m_iTargetWidth, m_iTargetHeight), 0, 0, cv::INTER_AREA);
		l_pImageFrame = &l_ScaleImage;
	}
	else
	{
		l_pImageFrame = l_pIplImage;
	}
	cv::Size s = l_pImageFrame->size();
	//3 channel
	int	l_iLength = s.height*s.width*3;
	if( m_iMaximumTampFileLength < l_iLength )
	{
		SAFE_DELETE(m_pTempData);
		m_iMaximumTampFileLength = l_iLength;
		m_pTempData = new char[l_iLength];
	}
	char*l_strBuffer = m_pTempData;
	jpge::params	l_Par;
	l_Par.m_quality = m_iQuality;
	if(jpge::compress_image_to_jpeg_file_in_memory(l_strBuffer,l_iLength,l_pIplImage->rows,l_pIplImage->cols,3,(const jpge::uint8*)l_pIplImage->data,l_Par))
	//if(jpge::compress_image_to_jpeg_file_in_memory(l_strBuffer,l_iLength,l_pIplImage->width,l_pIplImage->height,3,(const jpge::uint8*)l_pIplImage->imageData))
	{
		m_puiFrameFilePosVector[e_iFrame] = m_uiCurrentFilePos;
		m_puiFrameSizeVector[e_iFrame] = l_iLength;
		NvFWrite(l_strBuffer,sizeof(char),l_iLength,e_pFile);
		m_uiCurrentFilePos += l_iLength;
		return true;
	}
#endif
	return false;
}

UINT	cVideoConvert::GetFrameSize(UINT e_iFrame)
{
	if( m_iTotalFrame > (int)e_iFrame )
		return m_puiFrameSizeVector[e_iFrame];
	return 0;
}

bool	cVideoConvert::FirstConvertToFile(NvFile*e_pFile)
{
	SAFE_DELETE(m_puiFrameFilePosVector);
	SAFE_DELETE(m_puiFrameSizeVector);
	//
#ifdef WIN32
	cv::Mat l_Frame;
	if (m_pCvCapture->read(l_Frame))
	{
		//start hint caption is FM79979Video
		//lazy to do this now just give a 100 frame
		//header
		//first 12 bytes are
		//1stframe length
		//2nd total play time
		//3rd fps
		//second for compressed length
		//third for original length
		//const wchar_t*l_strVideoHint = L"FM79979Video";
		NvFWrite((void*)&this->m_iTotalFrame,sizeof(int),1,e_pFile);
		m_uiCurrentFilePos += sizeof(int);
		NvFWrite((void*)&this->m_fTotalPlayTime,sizeof(int),1,e_pFile);
		m_uiCurrentFilePos += sizeof(float);
		NvFWrite((void*)&this->m_fFPS,sizeof(int),1,e_pFile);
		m_uiCurrentFilePos += sizeof(float);
		if( this->m_iTargetWidth != -1 && this->m_iTargetHeight != -1 )
		{
			NvFWrite((void*)&m_iTargetWidth,sizeof(int),1,e_pFile);
			m_uiCurrentFilePos += sizeof(int);
			NvFWrite((void*)&this->m_iTargetHeight,sizeof(int),1,e_pFile);
			m_uiCurrentFilePos += sizeof(int);
		}
		else
		{
			NvFWrite((void*)&l_Frame.rows,sizeof(int),1,e_pFile);
			m_uiCurrentFilePos += sizeof(int);
			NvFWrite((void*)&l_Frame.cols,sizeof(int),1,e_pFile);
			m_uiCurrentFilePos += sizeof(int);
		}
		m_puiFrameFilePosVector = new UINT[m_iTotalFrame];
		m_puiFrameSizeVector = new UINT[m_iTotalFrame];

		return SaveFrameToFile(0,&l_Frame,e_pFile);
	}
#endif
	return false;
}

//done return 1
bool	cVideoConvert::ConvertToFile(int e_iFrame,NvFile*e_pFile)
{
#ifdef WIN32
	cv::Mat l_Frame;
	if (!m_pCvCapture->read(l_Frame))
		return false;
	int	l_iCompressLength = 0;
	if(SaveFrameToFile(e_iFrame,&l_Frame,e_pFile))
	{
		return true;
	}
#endif
	return false;
	//std::vector<char>	l_Data;
	//compress_memory(l_pFrame->imageData,l_pFrame->width*l_pFrame->height*3*sizeof(char),l_Data);
	//unsigned long	l_iReceivedLength = 0;
	//char*l_pReceivedData = new char[l_pFrame->imageSize];
	//unsigned long	l_iReceivedLength = l_pFrame->imageSize;
	//unsigned long	l_iReceivedLength2 = l_pFrame->imageSize;
	////void*l_pReceivedData2;

	//compress2((Bytef*)l_pReceivedData,&l_iReceivedLength,(Bytef*)l_pFrame->imageData,l_pFrame->imageSize,Z_BEST_COMPRESSION);
	//compress2((Bytef*)l_pReceivedData,&l_iReceivedLength2,(Bytef*)l_pFrame->imageData,l_pFrame->imageSize,5);
	//compress2((Bytef*)l_pReceivedData2,&l_iReceivedLength2,(const Bytef*)l_pFrame->imageData,l_pFrame->imageSize,Z_BEST_COMPRESSION);
	//m_pVideoImage->GetTexture()->SetupTexture(3,l_pFrame->width,l_pFrame->height,GL_BGR,GL_UNSIGNED_BYTE,false,l_pFrame->imageData);
}

float	cVideoConvert::PlayCompress(int e_iFrame,NvFile*e_pFile,int e_iFrameSize)
{
	return 1.f;
}

bool	cVideoConvert::StartConvert(NvFile*e_pFile)
{
	m_iCurrentFrame = 0;
	m_uiCurrentFilePos = 0;
	if(FirstConvertToFile(e_pFile))
	{
		m_iCurrentFrame = 1;
		for( ;m_iCurrentFrame<this->m_iTotalFrame;++m_iCurrentFrame )
		{
			if(!ConvertToFile(m_iCurrentFrame,e_pFile))
			{
				return false;
			}
		}
	}
	//UT::GetFileSize
	//rewrite file pos and frame length
	//NvFSeek(e_pFile,sizeof(int),SEEK_SET);
	long l_lCurrentPosition1 = NvFTell( e_pFile );
	size_t	l_iLength = NvFWrite((void*)m_puiFrameFilePosVector,sizeof(UINT),m_iTotalFrame,e_pFile);
	long l_lCurrentPosition2 = NvFTell( e_pFile );
	l_iLength = NvFWrite((void*)m_puiFrameSizeVector,sizeof(UINT),m_iTotalFrame,e_pFile);
	long l_lCurrentPosition3 = NvFTell( e_pFile );
	//last one for info position
	l_iLength = NvFWrite((void*)&l_lCurrentPosition1,sizeof(int),1,e_pFile);
	//now go back start rewrite file pos and size
	return true;
}

float	cVideoConvert::GetProgress()
{
	return (float)m_iCurrentFrame/this->m_iTotalFrame;
}

cFMVVideo::cFMVVideo()
{
	m_pFile = 0;
	m_bForceDone = false;
	m_pNewPixelData = 0;
	m_pNewPowerOfTwoPixelData = 0;
	m_pFUThreadPool = 0;
	m_bUsingMultiThread = true;
	m_bUsingSoundThread = true;
	m_iNumPacket = 5;
	m_bDoPreparePowerOfTwoTexture = false;
	m_bLoop = false;
	m_iCurrentUpdatePixelIndex = 0;
	m_iCurrentFrame = 0;
	m_pVideoSound = 0;
	CreateShader(g_bCommonVSClientState,g_strCommonVS,g_strCommonVideoFS,g_strVideoShaderName);
}

cFMVVideo::~cFMVVideo()
{
	m_bForceDone = true;
	Destroy();
}

void	cFMVVideo::SetTime(float e_fTime)
{
	this->m_fCurrentPlayTime = e_fTime;
	m_iCurrentFrame = cVideoPlayer::GetCurrentFrame(m_fCurrentPlayTime);
}

float	cFMVVideo::GetcurrentTime()
{
	return m_fCurrentPlayTime;
}

int		cFMVVideo::GetCurrentFrame()
{
	return m_iCurrentFrame;
}

cBaseImage*		cFMVVideo::GetLastFrameImage()
{
	if( !m_pFile )
		return 0;
	int	l_iLastFrame = this->m_iTotalFrame-1;
	int	l_iFilePos = m_puiFrameFilePosVector[l_iLastFrame];
	int	l_iFileSize = m_puiFrameSizeVector[l_iLastFrame];
	char*l_pTempData = new char[l_iFileSize];
	NvFSeek(m_pFile,l_iFilePos,SEEK_SET);
	NvFRead(l_pTempData,sizeof(char),l_iFileSize,m_pFile);
	{
		char*l_pNewPixelData = new char[l_iFileSize];
#ifdef ANDROID
		JpegToRawPixelData(l_pTempData,l_iFileSize,l_pNewPixelData);
#else
		jpgd::FM_decompress_jpeg_image_from_memory((unsigned char*)l_pNewPixelData,(const unsigned char*)l_pTempData,l_iFileSize,&m_iVideoWidth,&m_iVideoHeight,&m_iVideoChannal,3);
#endif
		if( l_pNewPixelData )
		{
			cBaseImage*l_pImage = new cBaseImage(0,false);
			cTexture*l_pTexture = new cTexture(l_pImage,l_pNewPixelData,m_iVideoWidth,m_iVideoHeight,L"FMVideoImage",false,false,GL_RGB);
			l_pImage->SetTexture(l_pTexture);
			SAFE_DELETE_ARRAY(l_pNewPixelData);
			return l_pImage;
		}
	}
	return 0;
}

void	cFMVVideo::Destroy()
{
	m_fCurrentPlayTime = 0.f;
	if( m_pFUThreadPool )
		m_pFUThreadPool->WaitForIdle();
	if( m_pVideoSound )
	{
		m_pVideoSound->Play(false);
		if( this->m_bUsingSoundThread )
		{
			SAFE_RELEASE(m_pVideoSound,this);
		}
		else
		{
			cGameApp::m_spSoundParser->RemoveObject(m_pVideoSound->GetName());
		}
	}
	SAFE_DELETE(m_pFUThreadPool);
	m_WaitForProcessData.clear();
	m_iCurrentUpdatePixelIndex = 0;
	m_iCurrentFrame = 0;
	SAFE_DELETE(m_puiFrameFilePosVector);
	SAFE_DELETE(m_puiFrameSizeVector);
	if( m_pNewPixelData )
	{
		for( int i=0;i<this->m_iNumPacket;++i )
		{
			SAFE_DELETE(m_pNewPixelData[i]);
		}
		SAFE_DELETE(m_pNewPixelData);
	}
	if( m_pNewPowerOfTwoPixelData )
	{
		for( int i=0;i<this->m_iNumPacket;++i )
		{
			SAFE_DELETE(m_pNewPowerOfTwoPixelData[i]);
		}
		SAFE_DELETE(m_pNewPowerOfTwoPixelData);
	}
	if( m_pFile )
	{
		NvFClose(m_pFile);
		m_pFile = 0;
	}
	//DeleteShader(g_strVideoShaderName);
}

void	SoundThread(size_t _workParameter, size_t _pUri)
{
	UT::sTimeAndFPS	l_TC;
	cFMVVideo*l_pFMVVideo = (cFMVVideo*)_workParameter;
	if(l_pFMVVideo->GetVideoSound())
		l_pFMVVideo->GetVideoSound()->Play(true);
	l_TC.Update();
	float	l_fElpaseTime = 0.f;
	while( !l_pFMVVideo->IsPlayDone() && !l_pFMVVideo->IsForceDone() )
	{
		if( !cGameApp::m_sbGamePause )
		{
			l_TC.Update();
			l_fElpaseTime += l_TC.fElpaseTime;
			if( l_fElpaseTime >= 0.03f )
			{
				l_pFMVVideo->GetVideoSound()->Update(10.f);
				l_fElpaseTime = 0.f;
			}
		}
		Sleep(1);
	}
}

void	VideoThread(size_t _workParameter, size_t _pUri)
{
#ifdef ANDROID
	int	res = cGameApp::m_spANativeActivity->vm->AttachCurrentThread(&g_pMultiThreadEnv,nullptr);
	if( cGameApp::m_spANativeActivity->vm->GetEnv((void**)&g_pMultiThreadEnv,JNI_VERSION_1_6) != JNI_OK )
	{
		return;
	}
	g_bitmap_factory_class = g_pMultiThreadEnv->FindClass("android/graphics/BitmapFactory");
	g_decode_byte_array_method = g_pMultiThreadEnv->GetStaticMethodID(g_bitmap_factory_class,"decodeByteArray", "([BII)Landroid/graphics/Bitmap;");
	//// Creaing Bitmap Config Class
	g_BitmapConfigClass = g_pMultiThreadEnv->FindClass("android/graphics/Bitmap$Config");

	g_bmpClsValueOfMid = g_pMultiThreadEnv->GetStaticMethodID(g_BitmapConfigClass, "valueOf", "(Ljava/lang/String;)Landroid/graphics/Bitmap$Config;");
	jstring l_strARGB_8888 = g_pMultiThreadEnv->NewStringUTF("ARGB_8888");
	g_jBmpCfg = g_pMultiThreadEnv->CallStaticObjectMethod(g_BitmapConfigClass, g_bmpClsValueOfMid,l_strARGB_8888);
	g_pMultiThreadEnv->DeleteLocalRef(l_strARGB_8888);	
#endif
	UT::sTimeAndFPS	l_TC;
	cFMVVideo*l_pFMVVideo = (cFMVVideo*)_workParameter;
	l_TC.Update();
	while( !l_pFMVVideo->IsPlayDone() && !l_pFMVVideo->IsForceDone() )
	{
		if( cGameApp::m_sbGamePause )
		{
			Sleep(1);
			continue;
		}
		l_TC.Update();
#ifdef DEBUG
		l_pFMVVideo->Update(l_TC.fElpaseTime*cGameApp::m_sfGameSpeedValue);
#else
		l_pFMVVideo->Update(l_TC.fElpaseTime);
#endif
	}
#ifdef ANDROID
	g_pMultiThreadEnv->DeleteLocalRef(g_bitmap_factory_class);
	g_pMultiThreadEnv->DeleteLocalRef(g_BitmapConfigClass);
	g_pMultiThreadEnv->DeleteLocalRef(g_jBmpCfg);
	cGameApp::m_spANativeActivity->vm->DetachCurrentThread();
#endif


}

void	VideoThreadDone(size_t _workParameter, size_t _pUri)
{

}

bool	cFMVVideo::OpenFile(const char*e_strFileName)
{
	Destroy();
	m_pFile = UT::MyFileOpen(e_strFileName,"rb");
	if( m_pFile )
	{
		int	l_iFileLength = UT::GetFileSize(m_pFile);
		int	l_iInfoStart = 0;
		NvFRead(&this->m_iTotalFrame,sizeof(int),1,m_pFile);
		NvFRead(&this->m_fTotalPlayTime,sizeof(float),1,m_pFile);
		NvFRead(&this->m_fFPS,sizeof(float),1,m_pFile);
		NvFRead(&this->m_iVideoWidth,sizeof(int),1,m_pFile);
		NvFRead(&this->m_iVideoHeight,sizeof(int),1,m_pFile);
		m_pVideoImage->GetTexture()->SetupTexture(3,m_iVideoWidth,m_iVideoHeight,g_iVideoColorType,GL_UNSIGNED_BYTE);
		m_puiFrameFilePosVector = new UINT[m_iTotalFrame];
		m_puiFrameSizeVector = new UINT[m_iTotalFrame];
		memset(m_puiFrameFilePosVector,0,sizeof(UINT)*m_iTotalFrame);
		memset(m_puiFrameSizeVector,0,sizeof(UINT)*m_iTotalFrame);
		//go to info position
		NvFSeek(m_pFile,l_iFileLength-sizeof(int),SEEK_SET);
		NvFRead(&l_iInfoStart,sizeof(UINT),1,m_pFile);
		NvFSeek(m_pFile,l_iInfoStart,SEEK_SET);
		size_t	l_iData = NvFRead((void*)m_puiFrameFilePosVector,sizeof(UINT),m_iTotalFrame,m_pFile);
		size_t	l_iData2 = NvFRead(m_puiFrameSizeVector,sizeof(UINT),m_iTotalFrame,m_pFile);
		if(!m_pNewPixelData)
		{
			//int	l_iPow2Width = UT::power_of_two(m_iVideoWidth);
			//int	l_iPow2Height = UT::power_of_two(m_iVideoHeight);
			int	l_iPow2Width = m_iVideoWidth;
			int	l_iPow2Height = m_iVideoHeight;
			m_pNewPixelData = new  char*[m_iNumPacket];
			m_pNewPowerOfTwoPixelData = new char*[m_iNumPacket];
			for( int i=0;i<m_iNumPacket;++i )
			{
				m_pNewPixelData[i]  = new  char[l_iPow2Width*l_iPow2Height*g_iVideoChannel];
				m_pNewPowerOfTwoPixelData[i] = 0;
			}
		}
		//go to stream start
		NvFSeek(m_pFile,4,SEEK_SET);
		std::string	l_strVideoFileName = UT::ChangeFileExtensionName(e_strFileName,"og");
		if( m_bUsingMultiThread )
		{
			m_pFUThreadPool = new cFUThreadPool();
			if( m_bUsingSoundThread )
			{
				m_pVideoSound = new cOpanalOgg(this,l_strVideoFileName.c_str(),true);
				m_pVideoSound->SetOwner(this);
				m_pFUThreadPool->Spawn(2);
				FUStaticFunctor2<size_t, size_t, void>* soundworkFunctor = new FUStaticFunctor2<size_t, size_t, void>(&SoundThread);
				FUStaticFunctor2<size_t, size_t, void>* sounddoneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&VideoThreadDone);
				m_pFUThreadPool->ExecuteWork(soundworkFunctor,sounddoneFunctor,(size_t)this,0);
			}
			else
			{
				m_pFUThreadPool->Spawn(1);
				if(cGameApp::m_spSoundParser->AddStreamingSound(cGameApp::m_spSoundParser,l_strVideoFileName.c_str()))
				{
					m_pVideoSound = cGameApp::m_spSoundParser->GetLastObject();
					m_pVideoSound->Play(true);
				}
			}
			FUStaticFunctor2<size_t, size_t, void>* workFunctor = new FUStaticFunctor2<size_t, size_t, void>(&VideoThread);
			FUStaticFunctor2<size_t, size_t, void>* doneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&VideoThreadDone);
			m_pFUThreadPool->ExecuteWork(workFunctor,doneFunctor,(size_t)this,0);
		}
			return true;
	}
	return false;
}

void	cFMVVideo::DecodeFrame(int e_iFrameIndex)
{
	int	l_iFilePos = m_puiFrameFilePosVector[e_iFrameIndex];
	int	l_iFileSize = m_puiFrameSizeVector[e_iFrameIndex];
	if( m_iMaximumTampFileLength < l_iFileSize )
	{
		SAFE_DELETE(m_pTempData);
		m_iMaximumTampFileLength = l_iFileSize;
		m_pTempData = new char[l_iFileSize];
	}
	m_FPS.Update();
	NvFSeek(m_pFile,l_iFilePos,SEEK_SET);
	NvFRead(m_pTempData,sizeof(char),l_iFileSize,m_pFile);
	{
#ifdef ANDROID
		JpegToRawPixelData(m_pTempData,l_iFileSize,m_pNewPixelData[m_iCurrentUpdatePixelIndex]);
#else
		jpgd::FM_decompress_jpeg_image_from_memory((unsigned char*)m_pNewPixelData[m_iCurrentUpdatePixelIndex],(const unsigned char*)m_pTempData,l_iFileSize,&m_iVideoWidth,&m_iVideoHeight,&m_iVideoChannal,3);
#endif
		{
			cFUSynchronizedHold	l_cFUSynchronizedHold(&m_FUSynchronized);
			if( m_bDoPreparePowerOfTwoTexture )
			{
				m_pNewPowerOfTwoPixelData[m_iCurrentUpdatePixelIndex] = cTexture::GeneratePowerOfTwoPixelData(g_iVideoChannel,m_iVideoWidth,m_iVideoHeight,g_iVideoChannel==3?GL_RGB:GL_RGBA,GL_UNSIGNED_BYTE,m_pNewPixelData[m_iCurrentUpdatePixelIndex]);
				if( m_pNewPowerOfTwoPixelData[m_iCurrentUpdatePixelIndex] )
					m_WaitForProcessData.push_back(m_pNewPowerOfTwoPixelData[m_iCurrentUpdatePixelIndex]);
				else
				{
					int a=0;
				}
			}
			else
				m_WaitForProcessData.push_back(m_pNewPixelData[m_iCurrentUpdatePixelIndex]);
			++m_iCurrentUpdatePixelIndex;
			if( m_iCurrentUpdatePixelIndex >= m_iNumPacket )
				m_iCurrentUpdatePixelIndex = 0;
		}
	}
}

void    cFMVVideo::Update(float e_fElpaseTime)
{
	int	l_iOldFrame = m_iCurrentFrame;
	if( l_iOldFrame == 0 )
		l_iOldFrame = -1;
	m_fCurrentPlayTime += e_fElpaseTime;
	m_iCurrentFrame = (int)(this->m_iTotalFrame*(m_fCurrentPlayTime/this->m_fTotalPlayTime));
	if( m_iTotalFrame <= this->m_iCurrentFrame )
	{
		if( m_bLoop && m_iTotalFrame <= this->m_iCurrentFrame )
		{
			m_iCurrentFrame = 0;
			m_fCurrentPlayTime = 0.f;
		}
		else
		{
			m_iCurrentFrame = m_iTotalFrame;
			Sleep(1);
			//this->m_bForceDone = true;
			return;
		}
	}
	if(l_iOldFrame == m_iCurrentFrame)
		return;
	{
		cFUSynchronizedHold	l_cFUSynchronizedHold(&m_FUSynchronized);
		if( m_WaitForProcessData.size() == m_iNumPacket )
		{
			return;
		}
	}
	DecodeFrame(m_iCurrentFrame);
}

void    cFMVVideo::Render()
{
	if( m_pVideoImage )
	{
		if(m_WaitForProcessData.size())
		{
			unsigned char*l_pData = 0;
			{
				cFUSynchronizedHold	l_cFUSynchronizedHold(&m_FUSynchronized);
				l_pData = (unsigned char*)m_WaitForProcessData.front();
				m_WaitForProcessData.pop_front();
			}
			if( m_bDoPreparePowerOfTwoTexture )
			{
				m_pVideoImage->GetTexture()->ForceUpdatePixels(l_pData);
				delete l_pData;
			}
			else
			{
				m_pVideoImage->SetupTexture(g_iVideoChannel,m_iVideoWidth,m_iVideoHeight,g_iVideoColorType,GL_UNSIGNED_BYTE,false,l_pData);
			}
			m_pVideoImage->SetWidth((int)cGameApp::m_spOpenGLRender->m_vGameResolution.x);
			m_pVideoImage->SetHeight((int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
		}
		cBaseShader*l_pVideoShader = GetShader(g_strVideoShaderName);
		if( l_pVideoShader )
			l_pVideoShader->Use();
		m_pVideoImage->Render();
	}
}

bool	cFMVVideo::IsPlayDone()
{
	return m_iTotalFrame <= this->m_iCurrentFrame;
}
//end USE_OLD_OPENCV
//#endif