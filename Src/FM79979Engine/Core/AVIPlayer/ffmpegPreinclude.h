#ifndef _FFMPEG_PRE_INCLUDE_H_
#define _FFMPEG_PRE_INCLUDE_H_


#ifdef WIN32

#include "stdint.h"

#define __INT64_C(c)     c ## LL
#define __UINT64_C(c)     c ## ULL

#define __STDC_CONSTANT_MACROS
#  define UINT64_C(c)       __UINT64_C(c)
#ifdef ANDROID
#  define INT64_C(c)       __INT64_C(c)
#endif
using namespace FATMING_CORE;
//#define inline _inline
//#define bool _Bool
//#define true 1
//#define false 0
extern "C"
{
	#include "libavcodec/avcodec.h"
	#include "libavformat/avformat.h"
	#include "libavformat/avio.h"
	#include "libswscale/swscale.h"
	#include "libavutil/avstring.h"
	#include "libavutil/time.h"
	//avcodec
	//avutil
	//swresample
	//swscale
	//avfilter
	//avformat
};
namespace FATMING_CORE
{
	class cOpanalWAV;
};

enum 
{
	AV_SYNC_AUDIO_MASTER,
	AV_SYNC_VIDEO_MASTER,
	AV_SYNC_EXTERNAL_MASTER,
};

#define SDL_AUDIO_BUFFER_SIZE 1024
#define MAX_AUDIOQ_SIZE (5 * 16 * 1024)
#define MAX_VIDEOQ_SIZE (5 * 256 * 1024)
#define AV_SYNC_THRESHOLD 0.01
#define AV_NOSYNC_THRESHOLD 10.0
#define SAMPLE_CORRECTION_PERCENT_MAX 10
#define AUDIO_DIFF_AVG_NB 20
#define FF_ALLOC_EVENT   (SDL_USEREVENT)
#define FF_REFRESH_EVENT (SDL_USEREVENT + 1)
#define FF_QUIT_EVENT (SDL_USEREVENT + 2)
#define VIDEO_PICTURE_QUEUE_SIZE 1
#define DEFAULT_AV_SYNC_TYPE AV_SYNC_VIDEO_MASTER

class PacketQueue
{
public:
	PacketQueue();
	~PacketQueue();
	void			Destroy();
	AVPacketList *	first_pkt, *last_pkt;
	int				nb_packets;
	int				size;
	cFUSynchronized mutex;
};

class VideoPicture
{
public:
	FATMING_CORE::cBaseImage *bmp;
	int width, height; /* source height & width */
	double pts;
	VideoPicture();
	void	Destroy();
	~VideoPicture();
};


class VideoState :public NamedTypedObject
{
public:
	bool			bFetchPixelData;
	char*			m_pPixelData;
	cFUThreadPool	m_FUThreadPool;
	cOpanalWAV*		m_pSound;
	AVFormatContext *pFormatCtx;
	int             videoStream, audioStream;

	int             av_sync_type;
	double          external_clock; /* external clock base */
	int64_t         external_clock_time;
	int             seek_req;
	int             seek_flags;
	int64_t         seek_pos;

	double          audio_clock;
	AVStream*		audio_st;	//pFormatCtx->streams[iAudioCodecIndex]
	PacketQueue		audioq;



	//
	AVFrame         audio_frame;
	uint8_t         audio_buf[(192000 * 3) / 2];


	unsigned int    audio_buf_size;
	unsigned int    audio_buf_index;
	AVPacket       audio_pkt;
	uint8_t*		audio_pkt_data;
	int             audio_pkt_size;
	int             audio_hw_buf_size;  
	double          audio_diff_cum; /* used for AV difference average computation */
	double          audio_diff_avg_coef;
	double          audio_diff_threshold;
	int             audio_diff_avg_count;
	//
	double          frame_timer;
	double          frame_last_pts;
	double          frame_last_delay;
	double          video_clock; ///<pts of last decoded frame / predicted pts of next decoded frame
	double          video_current_pts; ///<current displayed pts (different from video_clock if frame fifos are used)
	int64_t         video_current_pts_time;  ///<time (av_gettime) at which we updated video_current_pts - used to have running video pts
	AVStream        *video_st;//pFormatCtx->streams[iVideoCodecIndex]
	PacketQueue     videoq;
	VideoPicture    pictq[VIDEO_PICTURE_QUEUE_SIZE];
	int             pictq_size,pictq_rindex, pictq_windex;
	cFUSynchronized pictq_mutex;
	//wait for file open and close all thread
	cFUSynchronized pMutex;

	char            filename[1024];
	int             quit;
	//ture if file open
	bool			bFileOpen;
	//index for video and audio
	int				iVideoCodecIndex;
	int				iAudioCodecIndex;
	//for video frame
	//AVFrame*	pFrame;
	bool			bVideoThreadEnd;
	void			Init();
	void			Destroy();
	//AVIOContext     *io_context;
};
//end WIN32
#endif
//end _FFMPEG_PRE_INCLUDE_H_
#endif