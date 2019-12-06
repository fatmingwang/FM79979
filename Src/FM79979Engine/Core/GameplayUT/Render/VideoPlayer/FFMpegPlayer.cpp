#include "../stdafx.h"

#ifdef WIN32

#include "ffmpegPreinclude.h"
#include "FFMpegPlayer.h"
#include "../XML/StringToStructure.h"
#include "../GameplayUT/GameApp.h"
#ifdef ANDROID
#include "unistd.h"
#endif
#ifdef AV_NOPTS_VALUE
#undef AV_NOPTS_VALUE
#endif

#define AV_NOPTS_VALUE          0x8000000000000000LL
extern void			stream_seek(VideoState *is, int64_t pos, int rel);
extern double			get_master_clock(VideoState *is);
extern void			decode_thread(size_t _workParameter, size_t _pUri);
extern void			video_refresh_timer();

cVideoPlayer::sVideoData::sVideoData()
{
	pFrame = 0;
	pRGBFrame = 0;
	pCodecContext = 0;
	pImageConveteContext = 0;
	iStreamIndex = -1;
	pRGBFrameBuffer = 0;
	pTexture = 0;
}

cVideoPlayer::sVideoData::~sVideoData()
{
	if( pFrame )
	{
		av_free(pFrame);
		pFrame = 0;
	}
	if( pRGBFrame )
	{
		av_free(pRGBFrame);
		pRGBFrame = 0;
	}
	if( pCodecContext )
	{
		avcodec_close(pCodecContext);
		pCodecContext = 0;
	}
	if( pImageConveteContext )
	{
		sws_freeContext(pImageConveteContext);
		pImageConveteContext = 0;
	}
	if( pRGBFrameBuffer )
	{
		SAFE_DELETE(pRGBFrameBuffer);
	}
	SAFE_DELETE(pTexture);
}

cVideoPlayer::sAudioData::sAudioData()
{
	iStreamIndex = -1;
	pStream = 0;
	pCodecContext = 0;;
	pFrame = 0;
	for( int i=0;i<NUM_AUDIO_BUFFERS;++i )
		uiBuffers[i] = -1;
	uiSource = -1;
	bInit = false;
	iBufferNumber = 0;
}

cVideoPlayer::sAudioData::~sAudioData()
{
	if( pCodecContext )
	{
		avcodec_close(pCodecContext);
		pCodecContext = 0;
	}
	if( pFrame )
	{
		av_free(pFrame);
		pFrame = 0;
	}
	
	ALint val;
	alGetSourcei(uiSource, AL_BUFFERS_PROCESSED, &val);
	while(val--)
	{
		ALuint buffer;
		alSourceUnqueueBuffers(uiSource, 1, &buffer);
	}

	if( uiBuffers[0] != -1)
		alDeleteBuffers(NUM_AUDIO_BUFFERS,uiBuffers);
	if( uiSource != -1 )
		alDeleteSources(1,&uiSource);
}

cVideoPlayer::cVideoPlayer()
{
	m_pflush_pkt = new AVPacket();
	m_pAudioData = 0;
	m_pVideoData = 0;
	m_FUThreadPool.Spawn(1);
	m_fCurrentime = 0.f;
	m_fFPS = 30.f;
	m_fCurrentTimeToDetectedTimeForRender = 0.f;
	m_bRender = false;
	m_pVideoState = 0;
	static bool	l_b = false;
	if( !l_b )
	{
		av_register_all();
		l_b = true;
	}
	//===========for single thread
	m_bPlayOver = false;
	m_pFormatCtx = 0;
}

cVideoPlayer::~cVideoPlayer()
{
	if( m_pVideoState )
	{
		DestroyByMultithreadThread();
	}
	else
	{
		DestroyBySingleThread();
	}
	SAFE_DELETE(m_pflush_pkt);
	this->m_FUThreadPool.WaitForIdle();
}

void	cVideoPlayer::Destroy()
{
	DestroyByMultithreadThread();
	DestroyBySingleThread();
}

void	cVideoPlayer::DestroyByMultithreadThread()
{
	if( !m_pVideoState )
		return;
	SetCurrentTime(this->GetDuration());
	//while( m_pVideoState->quit!=1 )
	//{
	//	this->Render();
	//	Sleep(1);
	//}
	if( m_pVideoState->quit != 1 )
	{
#ifndef WIN32
		sleep(2);
#else
		Sleep(2);
#endif
		cFUSynchronizedHold l_FUSynchronizedHold(&m_pVideoState->pMutex);
		m_pVideoState->quit = 1;
		while(m_pVideoState->bVideoThreadEnd != true)
			m_pVideoState->pMutex.WaitCondition();
	}
	m_pVideoState->Destroy();
	SAFE_DELETE(m_pVideoState);
	//av_free( m_pVideoState );
	av_free_packet(m_pflush_pkt);
	m_pVideoState = 0;
}

void	cVideoPlayer::DestroyBySingleThread()
{
	SAFE_DELETE(m_pVideoData);
	SAFE_DELETE(m_pAudioData);
	if( m_pFormatCtx )
	{
		avformat_close_input(&m_pFormatCtx);
		m_pFormatCtx = 0;
	}
}

void decode_thread_Done(size_t _workParameter, size_t _pUri)
{

}

bool	cVideoPlayer::OpenFile(const char*e_strFileName)
{
	Destroy();
	if( !UT::IsFileExists(e_strFileName) )
		return false;
	m_bPlayOver = false;
	m_pVideoState = new VideoState();
	m_pVideoState->Init();
	sprintf(m_pVideoState->filename,"%s\0",e_strFileName);
	m_pVideoState->av_sync_type = DEFAULT_AV_SYNC_TYPE;
	
	FUStaticFunctor2<size_t, size_t, void>* workFunctor = new FUStaticFunctor2<size_t, size_t, void>(&decode_thread);
	FUStaticFunctor2<size_t, size_t, void>* doneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&decode_thread_Done);
	m_FUThreadPool.ExecuteWork(workFunctor,doneFunctor,(size_t)m_pVideoState,0);
	{
		cFUSynchronizedHold l_FUSynchronizedHold(&m_pVideoState->pMutex);
		while( m_pVideoState->bFileOpen == false  )
		{
			m_pVideoState->pMutex.WaitCondition();
		}
	}
	m_fVideoLength = (float)m_pVideoState->pFormatCtx->duration/AV_TIME_BASE;;
	av_init_packet(m_pflush_pkt);
	m_pflush_pkt->data = (uint8_t*)"FLUSH";
	return true;
}

float    cVideoPlayer::GetDuration()
{
	if(!m_pVideoState)
	{
		return -1.f;
	}
	return (float)m_pVideoState->pFormatCtx->duration/AV_TIME_BASE;
}

void  cVideoPlayer::SetCurrentTime(float e_fCurrentime)
{
	if( m_pVideoState )//multi thread.
	{
		float	l_fStep = e_fCurrentime-m_fCurrentime;
		double l_dPos = get_master_clock(m_pVideoState);
		l_dPos += l_fStep;
		stream_seek(m_pVideoState, (int64_t)(l_dPos * AV_TIME_BASE), (int)l_fStep);
	}
	else//singnal thread
	if( m_pFormatCtx )
	{
		 int flgs = AVSEEK_FLAG_ANY;
		 int64 seek_ts = (int64)(e_fCurrentime*(m_pFormatCtx->streams[this->m_pVideoData->iStreamIndex]->time_base.den))/(m_pFormatCtx->streams[this->m_pVideoData->iStreamIndex]->time_base.num);
		 if(av_seek_frame(m_pFormatCtx,m_pVideoData->iStreamIndex,seek_ts, flgs) < 0)
		 {

		 }
	}
	m_fCurrentime = e_fCurrentime;
}

void  cVideoPlayer::Update(float e_fElpaseTime)
{
	if( this->m_pVideoState )
	{
		m_fCurrentime += e_fElpaseTime;
		float l_fDestTime = 1/m_fFPS;
		m_fCurrentTimeToDetectedTimeForRender += e_fElpaseTime;
		if(m_fCurrentTimeToDetectedTimeForRender >= l_fDestTime)
		{
			m_fCurrentTimeToDetectedTimeForRender -= l_fDestTime;
			m_bRender = true;
			video_refresh_timer();
		}
		else
			m_bRender = false;
		if( m_fCurrentime >= this->m_fVideoLength )
			m_bPlayOver = true;
		else
			m_bPlayOver = false;
	}
	else
		UpdateBySingleThread(e_fElpaseTime);
}

bool	cVideoPlayer::AssignVideoData()
{
	if( !m_pVideoData )
		return false;
	AVDictionary    *optionsDict = NULL;
	if( m_pVideoData->iStreamIndex == -1 )
		return false; // Didn't find a video stream

	// Get a pointer to the codec context for the video stream
	m_pVideoData->pCodecContext = m_pFormatCtx->streams[m_pVideoData->iStreamIndex]->codec;
	// Find the decoder for the video stream
	AVCodec*l_pVideoCodec = avcodec_find_decoder(m_pVideoData->pCodecContext->codec_id);
	if( l_pVideoCodec == NULL )
		return false; // Codec not found

	// Open codec
	if( avcodec_open2(m_pVideoData->pCodecContext, l_pVideoCodec,&optionsDict) < 0 )
		return false; // Could not open codec

	// Hack to correct wrong frame rates that seem to be generated by some codecs
	if(m_pVideoData->pCodecContext->time_base.num>1000 && m_pVideoData->pCodecContext->time_base.den==1)
		m_pVideoData->pCodecContext->time_base.den=1000;
	// Allocate video frame
	m_pVideoData->pFrame = av_frame_alloc();
	// Allocate an AVFrame structure
	m_pVideoData->pRGBFrame = av_frame_alloc();
	if( m_pVideoData->pRGBFrame == NULL )
		return false;
	// Determine required buffer size and allocate buffer
	int	l_iNumBytes = avpicture_get_size(PIX_FMT_RGB24, m_pVideoData->pCodecContext->width,m_pVideoData->pCodecContext->height);
	m_pVideoData->pRGBFrameBuffer = new char[l_iNumBytes*sizeof(char)];

	// Assign appropriate parts of buffer to image planes in pFrameRGB
	avpicture_fill((AVPicture *)m_pVideoData->pRGBFrame, (uint8_t*)m_pVideoData->pRGBFrameBuffer, PIX_FMT_RGB24,m_pVideoData->pCodecContext->width, m_pVideoData->pCodecContext->height);	
	// Convert the image into YUV format that SDL uses
	int w = m_pVideoData->pCodecContext->width;
	int h = m_pVideoData->pCodecContext->height;

	m_pVideoData->pImageConveteContext = sws_getContext(w, h, m_pVideoData->pCodecContext->pix_fmt, w, h, PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);					
	if(m_pVideoData->pImageConveteContext == NULL) 
	{
		fprintf(stderr, "Cannot initialize the conversion context!\n");
		exit(1);
	}
	m_pVideoData->pTexture = new FATMING_CORE::cBaseImage((char*)0);
	m_pVideoData->pTexture->GetTexture()->SetupTexture(3,m_pVideoData->pCodecContext->width,m_pVideoData->pCodecContext->height,GL_RGB,GL_UNSIGNED_BYTE,false,0);
	m_pVideoData->pTexture->SetWidth(m_pVideoData->pCodecContext->width);
	m_pVideoData->pTexture->SetHeight(m_pVideoData->pCodecContext->height);
	m_pVideoData->pTexture->SetWidth(720);
	m_pVideoData->pTexture->SetHeight(1280);
	return true;
}
cOpanalWAV*g_pOpanalWAV = 0;
bool	cVideoPlayer::AssignAudioData()
{
	SAFE_DELETE(g_pOpanalWAV);

	if(!m_pAudioData)
		return false;
	m_pAudioData->pStream = this->m_pFormatCtx->streams[this->m_pAudioData->iStreamIndex];
	if (m_pAudioData->pStream == NULL)
    {
        return false;
    }
	m_pAudioData->pCodecContext = m_pAudioData->pStream->codec;
	m_pAudioData->pCodecContext->codec = avcodec_find_decoder(m_pAudioData->pCodecContext->codec_id);
    if (m_pAudioData->pCodecContext->codec == NULL)
    {
        
        return false;
    }
    else
	if(avcodec_open2(m_pAudioData->pCodecContext,m_pAudioData->pCodecContext->codec, NULL) != 0)
    {
        return false;
    }
	//m_pAudioData->pCodecContext->sample_fmt = AV_SAMPLE_FMT_S16;
	//m_pAudioData->pCodecContext->codec_id = AV_CODEC_ID_AAC;
	ALenum	l_Format = AL_FORMAT_STEREO8;
	switch(m_pAudioData->pCodecContext->channels)
    {
    case 1:
        l_Format = AL_FORMAT_MONO8;
        break;
    case 2:
        l_Format = AL_FORMAT_STEREO16;
        break;
    case 4:
		if(alIsExtensionPresent("AL_EXT_MCFORMATS"))
			l_Format =  alGetEnumValue("AL_FORMAT_QUAD16");
        break;
	case 6:
		if(alIsExtensionPresent("AL_EXT_MCFORMATS"))
			l_Format =  alGetEnumValue("AL_FORMAT_51CHN16");
		break;
    }
	m_pAudioData->iFormat = l_Format;
	m_pAudioData->iFormat = AL_FORMAT_STEREO16;
	m_pAudioData->iFrequency = m_pAudioData->pCodecContext->sample_rate;
    //m_pAudioData->iFormat = AL_FORMAT_STEREO16;
	alGenBuffers(NUM_AUDIO_BUFFERS, &m_pAudioData->uiBuffers[0]);
	alGenSources(1, &m_pAudioData->uiSource);
	m_pAudioData->pFrame = av_frame_alloc();

	//g_pOpanalWAV = new cOpanalWAV(this,0,false);
	//int	l_iFormat = g_pOpanalWAV->PerSampleBitAndChannelToOpenALFormat(m_pAudioData->pCodecContext->block_align*8,m_pAudioData->pCodecContext->channels);
	//g_pOpanalWAV->SetFreq(m_pAudioData->pCodecContext->sample_rate);
	//g_pOpanalWAV->SetFormat(l_iFormat);
	return true;
}

bool	cVideoPlayer::OpenFileBySingleThread(char*e_strFileName)
{
	Destroy();
#ifdef WIN32
	Sleep(100);
#else
	sleep(100);
#endif
	m_bPlayOver = false;
	// Open video file
	if(avformat_open_input(&m_pFormatCtx, e_strFileName, NULL, 0)!=0)
		return false; // Couldn't open file

	// Retrieve stream information
	if(avformat_find_stream_info(m_pFormatCtx,NULL)<0)
		return false; // Couldn't find stream information

	// Dump information about file onto standard error
	av_dump_format(m_pFormatCtx, 0,e_strFileName, false);
	unsigned int i=0;
	for(i=0; i<m_pFormatCtx->nb_streams; i++)
	{
		AVMediaType l_AVMediaType = m_pFormatCtx->streams[i]->codec->codec_type;
		if( l_AVMediaType == AVMEDIA_TYPE_VIDEO )
		{
			if( !m_pVideoData )
			{
				m_pVideoData = new sVideoData();
				m_pVideoData->iStreamIndex = i;
			}
		}
		else
		if( l_AVMediaType == AVMEDIA_TYPE_AUDIO )
		{
			if( !m_pAudioData )
			{
				m_pAudioData = new sAudioData();
				m_pAudioData->iStreamIndex = i;
			}
			else
			{//more than one sound is possible?.
			
			}
		}
		//we only need video
		else
		{
			avcodec_close(m_pFormatCtx->streams[i]->codec);
		}
	}
	AssignVideoData();
	AssignAudioData();
	if( m_pVideoData )
	{
		m_fFPS = GetFPS(m_pVideoData->pCodecContext);
		//m_fFPS = (float)m_pVideoData->pCodecContext->ticks_per_frame;
		float l_fDestTime = 1/m_fFPS;
		m_fCurrentTimeToDetectedTimeForRender = l_fDestTime;
	}
	m_fVideoLength = (float)m_pFormatCtx->duration/AV_TIME_BASE;;
	return true;
}

float	cVideoPlayer::GetFPS(AVCodecContext*e_pAVCodecContext)
{
	AVRational l_AVR = e_pAVCodecContext->time_base;
	return static_cast<float>(l_AVR.den) / static_cast<float>(l_AVR.num * e_pAVCodecContext->ticks_per_frame);
}

void    cVideoPlayer::UpdateBySingleThread(float e_fElpaseTime)
{
	m_fCurrentime += e_fElpaseTime;
	float l_fDestTime = 1/m_fFPS;
	m_fCurrentTimeToDetectedTimeForRender += e_fElpaseTime;
	if( m_fCurrentTimeToDetectedTimeForRender >= l_fDestTime )
	{
		m_fCurrentTimeToDetectedTimeForRender -= l_fDestTime;
		m_bRender = true;
	}
	else
		m_bRender = false;
	if( m_bRender )
	{
		bool	l_bFetchVideoFrame = false;
		int		l_iLength = 0;
		bool	l_bFetchAudio = false;
		//while( !l_bFetchVideoFrame && !l_bFetchAudio )
		while( !l_bFetchVideoFrame )
		//while( 1 )
		{
			AVPacket        l_Packet;
			int             l_iFrameFinished;
			av_init_packet(&l_Packet);
			if( av_read_frame(m_pFormatCtx, &l_Packet) >= 0 )
			{
				// Is this a packet from the video stream?
				if(m_pVideoData && l_Packet.stream_index == m_pVideoData->iStreamIndex)
				{
					l_bFetchVideoFrame = true;
					// Decode video frame
					int	l_iRet = avcodec_decode_video2(m_pVideoData->pCodecContext, m_pVideoData->pFrame, &l_iFrameFinished, &l_Packet);
					// Did we get a video frame?
					if(l_iFrameFinished)
					{
						l_iRet = sws_scale(m_pVideoData->pImageConveteContext, m_pVideoData->pFrame->data, m_pVideoData->pFrame->linesize, 0,	m_pVideoData->pCodecContext->height, m_pVideoData->pRGBFrame->data, m_pVideoData->pRGBFrame->linesize);
						if( m_pVideoData->pTexture )
						{
							cFUSynchronizedHold	l_FUSynchronizedHold(m_pVideoData->pTexture->GetTexture());
							m_pVideoData->pTexture->GetTexture()->UpdatePixels(m_pVideoData->pRGBFrame->data[0]);
							m_bRender = true;
							cTexture::m_suiLastUsingImageIndex = -1;
							this->Render();
						}
					}
				}
				else
				if(m_pAudioData && l_Packet.stream_index == this->m_pAudioData->iStreamIndex)
				{
					//if(l_Packet.size > 0)
					while (l_Packet.size > 0)
					{
						avcodec_get_frame_defaults(m_pAudioData->pFrame);
						// Try to decode the packet into a frame
						l_iLength = avcodec_decode_audio4(m_pAudioData->pCodecContext,m_pAudioData->pFrame, &l_iFrameFinished, &l_Packet);
						if( l_iLength >=0 )
						{
							if ( l_iFrameFinished)
							{
								int data_size = av_samples_get_buffer_size(NULL, m_pAudioData->pCodecContext->channels,m_pAudioData->pFrame->nb_samples,m_pAudioData->pCodecContext->sample_fmt,1);
								
								//if(!g_pOpanalWAV->IsPlay())
								//{
								//	g_pOpanalWAV->SetSize(data_size);
								//	g_pOpanalWAV->AssignBufferData(m_pAudioData->pFrame->data[0]);
								//	g_pOpanalWAV->Play(true);
								//}

								if( !m_pAudioData->bInit )
								{
									if (m_pAudioData->iFormat == AL_FORMAT_MONO16)
									{
										alBufferData(m_pAudioData->uiBuffers[m_pAudioData->iBufferNumber], m_pAudioData->iFormat, &m_pAudioData->pFrame->data[0], data_size, m_pAudioData->iFrequency/2);
									}
									else
									{
										alBufferData(m_pAudioData->uiBuffers[m_pAudioData->iBufferNumber], m_pAudioData->iFormat, &m_pAudioData->pFrame->data[0], data_size, m_pAudioData->iFrequency);
									}
									if( m_pAudioData->iBufferNumber == 2 )
									{
										// all buffers queued
										alSourceQueueBuffers(m_pAudioData->uiSource, NUM_AUDIO_BUFFERS, m_pAudioData->uiBuffers);
										// start playback
										alSourcePlay(m_pAudioData->uiSource);
										m_pAudioData->bInit = true;
										l_bFetchAudio = true;
									}
									// update buffer number to fill
									m_pAudioData->iBufferNumber = (m_pAudioData->iBufferNumber+1)%NUM_AUDIO_BUFFERS;
								}
								else
								{
								   // OpenAL is playing in the background.
								   // one new frame with audio data is ready
								   // first wait for openal to release one buffer
								   ALuint buffer;
								   ALint val;
								   do 
								   {
									   alGetSourcei(m_pAudioData->uiSource, AL_BUFFERS_PROCESSED, &val);
								   }
								   while (val <=0);	
								   // fill and requeue the empty buffer
								   alSourceUnqueueBuffers(m_pAudioData->uiSource, 1, &buffer);


									if (m_pAudioData->iFormat == AL_FORMAT_MONO16)
										alBufferData(buffer, m_pAudioData->iFormat,&m_pAudioData->pFrame->data[0], data_size, m_pAudioData->iFrequency/2);
									else
										alBufferData(buffer, m_pAudioData->iFormat,&m_pAudioData->pFrame->data[0], data_size, m_pAudioData->iFrequency);
								   alSourceQueueBuffers(m_pAudioData->uiSource, 1, &buffer);				                 
								   //Restart openal playback if needed
								   alGetSourcei(m_pAudioData->uiSource, AL_SOURCE_STATE, &val);
								   if(val != AL_PLAYING)
								   {
									   alSourcePlay(m_pAudioData->uiSource);								
								   }
								   l_bFetchAudio = true;
								}
							}
						}
						//There may be more than one frame of audio data
						//inside the packet.
						l_Packet.size -= l_iLength;
						l_Packet.data += l_iLength;
					}
				}
				else
				{
				
				}
				// Free the packet that was allocated by av_read_frame
				av_free_packet(&l_Packet);
			}
			else
			{
				m_bPlayOver = true;
				l_bFetchVideoFrame = true;
				//is loop!?
			}
		}
	}
}

void    cVideoPlayer::Render()
{
	if(!this->m_pVideoState)
	{
		cFUSynchronizedHold	l_FUSynchronizedHold(m_pVideoData->pTexture->GetTexture());
		m_pVideoData->pTexture->Render();
	}
	else
	{
		if( m_bRender )
		{
			if( m_pVideoState&&m_pVideoState->bFetchPixelData&&m_pVideoState )
			{
				VideoPicture*l_pVideoPicture = &m_pVideoState->pictq[0];
				cFUSynchronizedHold l_FUSynchronizedHold(l_pVideoPicture->bmp->GetTexture());
				l_pVideoPicture->bmp->GetTexture()->UpdatePixels(m_pVideoState->m_pPixelData);
			}
		}
		if(this->m_pVideoState)
		{
			VideoPicture*l_pVideoPicture = &m_pVideoState->pictq[0];
			if(l_pVideoPicture->bmp)
			{
				l_pVideoPicture->bmp->Render();
			}
			else
			if(m_pVideoState->bFileOpen)
			{
				AVCodecContext*l_pAVCodecContext = m_pVideoState->pFormatCtx->streams[m_pVideoState->iVideoCodecIndex]->codec;
				for(int i=0;i<VIDEO_PICTURE_QUEUE_SIZE;++i)
				{
					VideoPicture*l_pVideoPicture = &m_pVideoState->pictq[i];
					if(!l_pVideoPicture->bmp)
					{
						l_pVideoPicture->bmp = new cBaseImage((char*)0);
						l_pVideoPicture->bmp->GetTexture()->SetupTexture(3,l_pAVCodecContext->width,l_pAVCodecContext->height,GL_RGB,GL_UNSIGNED_BYTE,false,0);
						l_pVideoPicture->bmp->SetWidth(l_pAVCodecContext->width);
						l_pVideoPicture->bmp->SetHeight(l_pAVCodecContext->height);
						l_pVideoPicture->bmp->SetWidth(720);
						l_pVideoPicture->bmp->SetHeight(1280);
					}
				}
			}
		}
	}
}

bool	cVideoPlayer::IsPlayDone()
{
	return m_bPlayOver;
}
//end WIN32
#endif