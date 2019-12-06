#ifdef WIN32
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
#endif
#include "../stdafx.h"
#include "../GamePlayUT/GameApp.h"
#include "../zlib/zlib.h"
#include "../Image/jpeg/jpge.h"
#include "../Image/jpeg/jpgd.h"
#include "AndroidVideoPlayer.h"
#ifdef WIN32
int	g_iVideoColorType = GL_BGR;
#else
int	g_iVideoColorType = GL_RGB;
#endif
cVideoPlayer::cVideoPlayer()
{
	m_fTotalPlayTime = 56.f;
	m_fCurrentPlayTime = 0.f;
#ifndef ANDROID
	m_pCvCapture = 0;
#endif
	m_fRestTimeToNextFrame = 0.f;
	m_fFPS = 1/24.f;
	m_pVideoImage = new FATMING_CORE::cBaseImage((const char*)0);
	m_pVideoImage->SetWidth((int)cGameApp::m_svGameResolution.x);
	m_pVideoImage->SetHeight((int)cGameApp::m_svGameResolution.y);
	m_iTotalFrame = 0;
}

cVideoPlayer::~cVideoPlayer()
{
#ifndef ANDROID
	if( m_pCvCapture )
		cvReleaseCapture(&m_pCvCapture);
#endif
	SAFE_DELETE(m_pVideoImage);
}

bool	cVideoPlayer::OpenFile(const char*e_strFileName)
{
#ifndef ANDROID
	//http://www.ymer.org/amir/2007/06/04/getting-the-number-of-frames-of-an-avi-video-file-for-opencv/
	//m_pCvCapture->get(CV_CAP_PROP_FRAME_COUNT);
	m_pCvCapture = cvCreateFileCapture(e_strFileName);
	if( m_pCvCapture )
	{
		m_iTotalFrame = (int) cvGetCaptureProperty( m_pCvCapture , CV_CAP_PROP_FRAME_COUNT );
		m_fFPS = (float) cvGetCaptureProperty( m_pCvCapture , CV_CAP_PROP_FPS );
		m_fTotalPlayTime = m_iTotalFrame/m_fFPS;
		return true;
	}
#endif
	return false;
}
void    cVideoPlayer::Update(float e_fElpaseTime)
{
#ifndef ANDROID
	m_fRestTimeToNextFrame -= e_fElpaseTime;
	if( m_fRestTimeToNextFrame < 0 )
	{
		m_fRestTimeToNextFrame += m_fFPS;
		IplImage*l_pFrame = cvQueryFrame(m_pCvCapture);
#ifdef WIN32		
		m_pVideoImage->GetTexture()->SetupTexture(3,l_pFrame->width,l_pFrame->height,GL_BGR,GL_UNSIGNED_BYTE,false,l_pFrame->imageData);
#else
		m_pVideoImage->GetTexture()->SetupTexture(3,l_pFrame->width,l_pFrame->height,GL_RGB,GL_UNSIGNED_BYTE,false,l_pFrame->imageData);
#endif
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
#ifndef ANDROID
	IplImage*l_pIplImage = (IplImage*)e_pIplImage;
	if( !e_pIplImage || !e_pFile )
		return false;
	int	l_iLength = l_pIplImage->imageSize;
	if( m_iMaximumTampFileLength < l_iLength )
	{
		SAFE_DELETE(m_pTempData);
		m_iMaximumTampFileLength = l_iLength;
		m_pTempData = new char[l_iLength];
	}
	char*l_strBuffer = m_pTempData;
	jpge::params	l_Par;
	l_Par.m_quality = 10;
	if(jpge::compress_image_to_jpeg_file_in_memory(l_strBuffer,l_iLength,l_pIplImage->width,l_pIplImage->height,3,(const jpge::uint8*)l_pIplImage->imageData,l_Par))
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
#ifndef ANDROID
	IplImage*l_pFrame = cvQueryFrame(m_pCvCapture);
	if( l_pFrame )
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
		//const WCHAR*l_strVideoHint = L"FM79979Video";
		NvFWrite((void*)&this->m_iTotalFrame,sizeof(int),1,e_pFile);
		m_uiCurrentFilePos += sizeof(int);
		NvFWrite((void*)&this->m_fTotalPlayTime,sizeof(int),1,e_pFile);
		m_uiCurrentFilePos += sizeof(float);
		NvFWrite((void*)&this->m_fFPS,sizeof(int),1,e_pFile);
		m_uiCurrentFilePos += sizeof(float);
		NvFWrite((void*)&l_pFrame->width,sizeof(int),1,e_pFile);
		m_uiCurrentFilePos += sizeof(int);
		NvFWrite((void*)&l_pFrame->height,sizeof(int),1,e_pFile);
		m_uiCurrentFilePos += sizeof(int);
		m_puiFrameFilePosVector = new UINT[m_iTotalFrame];
		m_puiFrameSizeVector = new UINT[m_iTotalFrame];

		return SaveFrameToFile(0,l_pFrame,e_pFile);
	}
#endif
	return false;
}

//done return 1
bool	cVideoConvert::ConvertToFile(int e_iFrame,NvFile*e_pFile)
{
#ifndef ANDROID
	IplImage*l_pFrame = cvQueryFrame(m_pCvCapture);
	if( !l_pFrame )
		return false;
	int	l_iCompressLength = 0;
	if(SaveFrameToFile(e_iFrame,l_pFrame,e_pFile))
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
	int	l_iLength = NvFWrite((void*)m_puiFrameFilePosVector,sizeof(UINT),m_iTotalFrame,e_pFile);
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
	m_iThreadCount = 2;
	m_FUThreadPool.Spawn(m_iThreadCount);
	m_bUsingMultiThread = true;
	m_iNumPacket = 5;
	m_ppTempPixelDataForSync = new char*[m_iThreadCount];
	for( int i=0;i<m_iThreadCount;++i )
		m_ppTempPixelDataForSync[i] = 0;
	m_bDoPreparePowerOfTwoTexture = false;
	m_pbWaitSingnalForThred = new bool[m_iThreadCount];
	m_pMultiThreadReadFileTempData = new char*[m_iThreadCount];
}

cFMVVideo::~cFMVVideo()
{
	Destroy();
}

void	cFMVVideo::Destroy()
{
	m_FUThreadPool.WaitForIdle();
	m_WaitForProcessData.clear();
	m_iCurrentUpdatePixelIndex = 0;
	m_iCurrentFrame = 0;
	for( int i=0;i<m_iThreadCount;++i )
		m_ppTempPixelDataForSync[i] = 0;
	SAFE_DELETE(m_ppTempPixelDataForSync);
	SAFE_DELETE(m_pbWaitSingnalForThred);
	SAFE_DELETE(m_puiFrameFilePosVector);
	SAFE_DELETE(m_puiFrameSizeVector);
	SAFE_DELETE_ARRAY(m_pNewPixelData);
	SAFE_DELETE_ARRAY(m_pNewPowerOfTwoPixelData);
	SAFE_DELETE_ARRAY(m_pMultiThreadReadFileTempData);
	if( m_pFile )
	{
		NvFClose(m_pFile);
		m_pFile = 0;
	}
}

void	VideoThread(size_t _workParameter, size_t _pUri)
{
	int	l_iThreadIndex = _pUri;
	cFMVVideo*l_pFMVVideo = (cFMVVideo*)_workParameter;
	while( !l_pFMVVideo->IsPlayDone() || !l_pFMVVideo->IsForceDone() )
	{
		if( !l_pFMVVideo->m_pbWaitSingnalForThred[l_iThreadIndex] )
		{
			int	l_iTargetFrameIndex = l_pFMVVideo->m_iCurrentFrame+l_iThreadIndex;
			l_pFMVVideo->MultiThreadDecodeFrame(l_iTargetFrameIndex,l_iThreadIndex);
			l_pFMVVideo->m_pbWaitSingnalForThred[l_iThreadIndex] = true;
		}
	}
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
		int	l_iData = NvFRead((void*)m_puiFrameFilePosVector,sizeof(UINT),m_iTotalFrame,m_pFile);
		int	l_iData2 = NvFRead(m_puiFrameSizeVector,sizeof(UINT),m_iTotalFrame,m_pFile);
		if(!m_pNewPixelData)
		{
			int	l_iPow2Width = UT::power_of_two(m_iVideoWidth);
			int	l_iPow2Height = UT::power_of_two(m_iVideoHeight);
			m_pNewPixelData = new  char*[m_iNumPacket];
			m_pNewPowerOfTwoPixelData = new char*[m_iNumPacket];
			for( int i=0;i<m_iNumPacket;++i )
			{
				m_pNewPixelData[i]  = new  char[l_iPow2Width*l_iPow2Height*3];
				m_pNewPowerOfTwoPixelData[i] = 0;
			}
		}
		//go to stream start
		NvFSeek(m_pFile,4,SEEK_SET);
		if( m_bUsingMultiThread )
		{
			FUStaticFunctor2<size_t, size_t, void>* workFunctor = new FUStaticFunctor2<size_t, size_t, void>(&VideoThread);
			FUStaticFunctor2<size_t, size_t, void>* doneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&VideoThreadDone);
			m_FUThreadPool.ExecuteWork(workFunctor,doneFunctor,(size_t)this,0);
		}
	}
	return true;
}

void	cFMVVideo::MultiThreadDecodeFrame(int e_iFrameIndex,int e_iThreadIndex)
{
	int	l_iFilePos = m_puiFrameFilePosVector[e_iFrameIndex];
	int	l_iFileSize = m_puiFrameSizeVector[e_iFrameIndex];
	if( m_iMaximumTampFileLength < l_iFileSize )
	{
		SAFE_DELETE(m_pTempData);
		m_iMaximumTampFileLength = l_iFileSize;
		m_pMultiThreadReadFileTempData[e_iThreadIndex] = new char[l_iFileSize];
	}
	m_FPS.Update();
	{
		cFUSynchronizedHold	l_cFUSynchronizedHold(&m_FUSynchronizedForFileRead);
		NvFSeek(m_pFile,l_iFilePos,SEEK_SET);
		NvFRead(m_pMultiThreadReadFileTempData[e_iThreadIndex],sizeof(char),l_iFileSize,m_pFile);
	}
	{
		//UT::sTimeAndFPS	l_Test;
		//l_Test.Update();
		//int	l_iNumError = 0;
		//for( int i=0;i<60;++i )
		//{
		//	int	l_iFilePos2 = m_puiFrameFilePosVector[i];
		//	int	l_iFileSize2 = m_puiFrameSizeVector[i];
		//	NvFSeek(m_pFile,l_iFilePos2,SEEK_SET);
		//	int	l_iReaded = NvFRead(m_pTempData,sizeof(char),l_iFileSize2,m_pFile);
		//	if(!jpgd::FM_decompress_jpeg_image_from_memory((unsigned char*)m_pNewPixelData[m_iCurrentUpdatePixelIndex],(const unsigned char*)m_pTempData,l_iFileSize2,&m_iVideoWidth,&m_iVideoHeight,&m_iVideoChannal,3))
		//	{
		//		int a=0;
		//		NvFSeek(m_pFile,l_iFilePos2,SEEK_SET);
		//		NvFRead(m_pTempData,sizeof(char),l_iFileSize2,m_pFile);
		//		++l_iNumError;
		//		break;
		//	}
		//}
		//l_Test.Update();
		{
			//if(jpgd::FM_decompress_jpeg_image_from_memory((unsigned char*)m_pNewPixelData[m_iCurrentUpdatePixelIndex],(const unsigned char*)m_pTempData,l_iFileSize,&m_iVideoWidth,&m_iVideoHeight,&m_iVideoChannal,3) == 0)
			jpgd::FM_decompress_jpeg_image_from_memory((unsigned char*)m_pNewPixelData[e_iThreadIndex],(const unsigned char*)m_pMultiThreadReadFileTempData[e_iThreadIndex],l_iFileSize,&m_iVideoWidth,&m_iVideoHeight,&m_iVideoChannal,3);
			{
				if( m_bDoPreparePowerOfTwoTexture )
				{
					m_pNewPowerOfTwoPixelData[e_iThreadIndex] = cTexture::GeneratePowerOfTwoPixelData(3,m_iVideoWidth,m_iVideoHeight,GL_RGB,GL_UNSIGNED_BYTE,m_pNewPixelData[e_iThreadIndex]);
					if( m_pNewPowerOfTwoPixelData[e_iThreadIndex] )
						m_ppTempPixelDataForSync[e_iThreadIndex] = m_pNewPowerOfTwoPixelData[e_iThreadIndex];
					else
					{
						int a=0;
					}
				}
				else
					m_ppTempPixelDataForSync[e_iThreadIndex] = m_pNewPixelData[e_iThreadIndex];
			}
			//else
			//{
			//	::OutputDebugString(L"\nError frame index ");
			//	::OutputDebugString(ValueToStringW(m_iCurrentFrame).c_str());
			//}
		}
	}
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
		//UT::sTimeAndFPS	l_Test;
		//l_Test.Update();
		//int	l_iNumError = 0;
		//for( int i=0;i<60;++i )
		//{
		//	int	l_iFilePos2 = m_puiFrameFilePosVector[i];
		//	int	l_iFileSize2 = m_puiFrameSizeVector[i];
		//	NvFSeek(m_pFile,l_iFilePos2,SEEK_SET);
		//	int	l_iReaded = NvFRead(m_pTempData,sizeof(char),l_iFileSize2,m_pFile);
		//	if(!jpgd::FM_decompress_jpeg_image_from_memory((unsigned char*)m_pNewPixelData[m_iCurrentUpdatePixelIndex],(const unsigned char*)m_pTempData,l_iFileSize2,&m_iVideoWidth,&m_iVideoHeight,&m_iVideoChannal,3))
		//	{
		//		int a=0;
		//		NvFSeek(m_pFile,l_iFilePos2,SEEK_SET);
		//		NvFRead(m_pTempData,sizeof(char),l_iFileSize2,m_pFile);
		//		++l_iNumError;
		//		break;
		//	}
		//}
		//l_Test.Update();
		{
			//if(jpgd::FM_decompress_jpeg_image_from_memory((unsigned char*)m_pNewPixelData[m_iCurrentUpdatePixelIndex],(const unsigned char*)m_pTempData,l_iFileSize,&m_iVideoWidth,&m_iVideoHeight,&m_iVideoChannal,3) == 0)
			jpgd::FM_decompress_jpeg_image_from_memory((unsigned char*)m_pNewPixelData[m_iCurrentUpdatePixelIndex],(const unsigned char*)m_pTempData,l_iFileSize,&m_iVideoWidth,&m_iVideoHeight,&m_iVideoChannal,3);
			{
				cFUSynchronizedHold	l_cFUSynchronizedHold(&m_FUSynchronized);
				if( m_bDoPreparePowerOfTwoTexture )
				{
					m_pNewPowerOfTwoPixelData[m_iCurrentUpdatePixelIndex] = cTexture::GeneratePowerOfTwoPixelData(3,m_iVideoWidth,m_iVideoHeight,GL_RGB,GL_UNSIGNED_BYTE,m_pNewPixelData[m_iCurrentUpdatePixelIndex]);
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
			//else
			//{
			//	::OutputDebugString(L"\nError frame index ");
			//	::OutputDebugString(ValueToStringW(m_iCurrentFrame).c_str());
			//}
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
	//if( m_iTotalFrame <= this->m_iCurrentFrame )
	//	m_iCurrentFrame = 0;
	if( m_iTotalFrame <= this->m_iCurrentFrame || l_iOldFrame == m_iCurrentFrame )
		return;
	if( this->m_bUsingMultiThread )
	{
		int	l_iNumThreadDoneCount = 0;
		for( int i=0;i<m_iThreadCount;++i )
		{
			if(m_pbWaitSingnalForThred[l_iThreadIndex])
				++l_iNumThreadDoneCount;
		}
		if( l_iNumThreadDoneCount == m_iThreadCount )
		{
			int	l_
			m_iCurrentFrame = (int)(this->m_iTotalFrame*(m_fCurrentPlayTime/this->m_fTotalPlayTime));
		}
	}
	else
	{
		//if(IsNewFrame())
		if( m_WaitForProcessData.size() == m_iNumPacket )
		{
			return;
		}
		DecodeFrame(m_iCurrentFrame);
	}
}

void    cFMVVideo::Render()
{
	if( m_pVideoImage  )
	{
		//if( this->IsNewFrame() )
		if(m_WaitForProcessData.size())
		{
			unsigned char*l_pData = 0;
			cFUSynchronizedHold	l_cFUSynchronizedHold(&m_FUSynchronized);
			l_pData = (unsigned char*)m_WaitForProcessData.front();
			m_WaitForProcessData.pop_front();
			if( m_bDoPreparePowerOfTwoTexture )
			{
				m_pVideoImage->GetTexture()->ForceUpdatePixels(l_pData);
				delete l_pData;
			}
			else
			{
				m_pVideoImage->SetupTexture(3,m_iVideoWidth,m_iVideoHeight,g_iVideoColorType,GL_UNSIGNED_BYTE,false,l_pData);
			}
		}
		m_pVideoImage->Render();
	}
	//if(m_WaitForProcessData.size())
	//{
	//	unsigned char*l_pData = 0;
	//	cFUSynchronizedHold	l_cFUSynchronizedHold(&m_FUSynchronized);
	//	l_pData = (unsigned char*)m_WaitForProcessData.front();
	//	m_WaitForProcessData.pop_front();
	//	if( m_bDoPreparePowerOfTwoTexture )
	//	{
	//		cBaseImage*l_pImage = new cBaseImage((const char*)0);
	//		l_pImage->SetupTexture(3,1024,512,g_iVideoColorType,GL_UNSIGNED_BYTE,false,l_pData);
	//		l_pImage->Render();
	//		delete l_pImage;
	//		delete l_pData;
	//	}
	//	SetNewFrame(false);
	//}
}

bool	cFMVVideo::IsPlayDone()
{
	return m_iTotalFrame <= this->m_iCurrentFrame;
}