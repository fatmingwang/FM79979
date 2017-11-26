#ifdef WIN32
#include "../stdafx.h"
#include "ffmpegPreinclude.h"
#include "FFMpegPlayer.h"
#ifdef ANDROID
#include "unistd.h"
#include "libavutil/time.h"
#endif
#include "time.h"

void			packet_queue_init(PacketQueue *q) ;
int				packet_queue_put(PacketQueue *q, AVPacket *pkt);
int				packet_queue_get(PacketQueue *q, AVPacket *pkt);
void			packet_queue_flush(PacketQueue *q);

double			get_audio_clock(VideoState *is);
double			get_video_clock(VideoState *is);
double			get_external_clock(VideoState *is);
double			get_master_clock(VideoState *is);

int				synchronize_audio(VideoState *is, short *samples,int samples_size, double pts);
int				audio_decode_frame(VideoState *is,double *pts_ptr) ;

void			audio_thread(size_t _workParameter, size_t _pUri);

void			video_display(VideoState *is) ;
void			video_refresh_timer();
void			video_thread(size_t _workParameter, size_t _pUri);
void			video_threadDone(size_t _workParameter, size_t _pUri){}
int				queue_picture(VideoState *is, AVFrame *pFrame, double pts,AVFrame *pFrameRGB);
double			synchronize_video(VideoState *is, AVFrame *src_frame, double pts);


int				our_get_buffer(struct AVCodecContext *c, AVFrame *pic);
void			our_release_buffer(struct AVCodecContext *c, AVFrame *pic);

int				stream_component_open(VideoState *is, int stream_index);
void			decode_thread(size_t _workParameter, size_t _pUri);


void			stream_seek(VideoState *is, int64_t pos, int rel);


bool	g_bOpenSound = false;

AVPacket	g_flush_pkt;
VideoState *global_video_state = 0;
SwsContext *g_pImg_convert_ctx = 0;

PacketQueue::PacketQueue()
{
	first_pkt = 0; last_pkt = 0;
	nb_packets = 0;
	size = 0;
}

void	PacketQueue::Destroy()
{
	if(this->first_pkt)
	{
		packet_queue_flush(this);
	}
}

PacketQueue::~PacketQueue()
{
	this->Destroy();
	assert(!last_pkt);
	assert(!first_pkt);
}

VideoPicture::VideoPicture()
{
	bmp = 0;
	pts = 0;
	width = height = 0;
}
void	VideoPicture::Destroy()
{
	SAFE_DELETE(bmp);
}
VideoPicture::~VideoPicture()
{
	Destroy();
}


void	VideoState::Init()
{
	m_FUThreadPool.Spawn(2);
	m_pPixelData = 0;
	bFetchPixelData = false;
	m_pSound = 0;	
	pFormatCtx = 0;
	videoStream = 0;
	audioStream = 0;
	av_sync_type = 0;
	external_clock = 0;
	external_clock_time = 0;
	seek_req = 0;
	seek_flags = 0;
	seek_pos = 0;
	audio_clock = 0;
	audio_st = 0;
	audio_buf_size = 0;
	audio_buf_index = 0;
	av_init_packet(&audio_pkt);
	audio_pkt_data = 0;
	audio_pkt_size = 0;
	audio_hw_buf_size;  
	audio_diff_cum = 0;
	audio_diff_avg_coef = 0;
	audio_diff_threshold = 0;
	audio_diff_avg_count = 0;
	frame_timer = 0;
	frame_last_pts = 0;
	frame_last_delay = 0;
	video_clock = 0;
	video_current_pts = 0;
	video_current_pts_time = 0;
	pictq_windex = 0;
	pictq_rindex = 0;
	pictq_size = 0;
	m_pSound = 0;
	global_video_state = this;
	bVideoThreadEnd = false;
	//pVideoEndMutex = 0;
	//pVideoEndCond = 0;
	//pFrame = 0;
	audio_st = 0;
	video_st = 0;
	bFileOpen = false;
	quit = 0;
	pFormatCtx = 0;
	audio_pkt_data = 0;
	iVideoCodecIndex = -1;
	iAudioCodecIndex = -1;
}
void	VideoState::Destroy()
{
	SAFE_DELETE(m_pSound);
	if( iVideoCodecIndex!=-1 )
	{
		avcodec_close(pFormatCtx->streams[iVideoCodecIndex]->codec);
		iVideoCodecIndex = -1;
	}
	if( iAudioCodecIndex!=-1 )
	{
		avcodec_close(pFormatCtx->streams[iAudioCodecIndex]->codec);
		iAudioCodecIndex = -1;
	}
	if( pFormatCtx )
	{
#ifdef ANDROID
		av_close_input_file(pFormatCtx);
#else
		avformat_close_input(&pFormatCtx);
#endif
		pFormatCtx = 0;
	}
	//if( pFrame )
	//{
	//	av_free(pFrame);
	//	pFrame = 0;
	//}
	av_free_packet(&audio_pkt);
	if( g_pImg_convert_ctx )
	{
		sws_freeContext(g_pImg_convert_ctx);
		g_pImg_convert_ctx = 0;
	}
	this->m_FUThreadPool.WaitForIdle();
}

//=============================================
//
//=============================================
void packet_queue_init(PacketQueue *q) 
{
	q->first_pkt = 0;
	q->last_pkt = 0;
	q->nb_packets = 0;
	q->size = 0;
	//memset(q, 0, sizeof(PacketQueue));
}

int packet_queue_put(PacketQueue *q, AVPacket *pkt) 
{
	cFUSynchronizedHold l_FUSynchronizedHold(&q->mutex);
	AVPacketList *pkt1;
	if( pkt != &g_flush_pkt && av_dup_packet(pkt) < 0)
	{
		return -1;
	}
	pkt1 = (AVPacketList*)av_malloc(sizeof(AVPacketList));
	if (!pkt1)
		return -1;
	pkt1->pkt = *pkt;
	pkt1->next = NULL;
	if (!q->last_pkt)
		q->first_pkt = pkt1;
	else
		q->last_pkt->next = pkt1;
	q->last_pkt = pkt1;
	q->nb_packets++;
	q->size += pkt1->pkt.size;
	q->mutex.ConditionOk();
	return 0;
}

int packet_queue_get(PacketQueue *q, AVPacket *pkt)
{
	AVPacketList *pkt1;
	int ret;
	cFUSynchronizedHold l_FUSynchronizedHold(&q->mutex);
	for(;;) 
	{
		if( global_video_state->quit )
		{
			ret = -1;
			break;
		}
		if(global_video_state->quit) 
		{
			ret = -1;
			break;
		}
		pkt1 = q->first_pkt;
		if (pkt1)
		{
			q->first_pkt = pkt1->next;
			if (!q->first_pkt)
				q->last_pkt = NULL;
			q->nb_packets--;
			q->size -= pkt1->pkt.size;
			*pkt = pkt1->pkt;
			av_free(pkt1);
			ret = 1;
			break;
		}
		else
		{
			q->mutex.WaitCondition();
		}
	}
	return ret;
}

void packet_queue_flush(PacketQueue *q)
{
	AVPacketList *pkt, *pkt1;
	cFUSynchronizedHold l_FUSynchronizedHold(&q->mutex);
	for(pkt = q->first_pkt; pkt != NULL; pkt = pkt1)
	{
		pkt1 = pkt->next;
		av_free_packet(&pkt->pkt);
		av_freep(&pkt);
	}
	q->last_pkt = NULL;
	q->first_pkt = NULL;
	q->nb_packets = 0;
	q->size = 0;
}

double get_audio_clock(VideoState *is)
{
	double pts;
	int hw_buf_size, bytes_per_sec, n;

	pts = is->audio_clock; /* maintained in the audio thread */
	hw_buf_size = is->audio_buf_size - is->audio_buf_index;
	bytes_per_sec = 0;
	n = is->audio_st->codec->channels * 2;
	if(is->audio_st)
	{
		bytes_per_sec = is->audio_st->codec->sample_rate * n;
	}
	if(bytes_per_sec)
	{
		pts -= (double)hw_buf_size / bytes_per_sec;
	}
	return pts;
}

double get_video_clock(VideoState *is)
{
	double delta;
	delta = (av_gettime() - is->video_current_pts_time) / 1000000.0;
	return is->video_current_pts + delta;
}

double get_external_clock(VideoState *is)
{
	return av_gettime() / 1000000.0;
}

double get_master_clock(VideoState *is)
{
	if(is->av_sync_type == AV_SYNC_VIDEO_MASTER)
	{
		return get_video_clock(is);
	}
	else
	if(is->av_sync_type == AV_SYNC_AUDIO_MASTER)
	{
		return get_audio_clock(is);
	}
	else
	{
		return get_external_clock(is);
	}
}
/* Add or subtract samples to get a better sync, return new
audio buffer size */

int synchronize_audio(VideoState *is, short *samples,int samples_size, double pts)
{
	int n;
	double ref_clock;

	n = 2 * is->audio_st->codec->channels;

	if(is->av_sync_type != AV_SYNC_AUDIO_MASTER)
	{
		double diff, avg_diff;
		int wanted_size, min_size, max_size;

		ref_clock = get_master_clock(is);
		diff = get_audio_clock(is) - ref_clock;

		if(diff < AV_NOSYNC_THRESHOLD)
		{
			// accumulate the diffs
			is->audio_diff_cum = diff + is->audio_diff_avg_coef* is->audio_diff_cum;
			if(is->audio_diff_avg_count < AUDIO_DIFF_AVG_NB)
			{
				is->audio_diff_avg_count++;
			}
			else
			{
				avg_diff = is->audio_diff_cum * (1.0 - is->audio_diff_avg_coef);
				if(fabs(avg_diff) >= is->audio_diff_threshold)
				{
					wanted_size = samples_size + ((int)(diff * is->audio_st->codec->sample_rate) * n);
					min_size = samples_size * ((100 - SAMPLE_CORRECTION_PERCENT_MAX) / 100);
					max_size = samples_size * ((100 + SAMPLE_CORRECTION_PERCENT_MAX) / 100);
					if(wanted_size < min_size)
					{
						wanted_size = min_size;
					}
					else
						if(wanted_size > max_size)
						{
							wanted_size = max_size;
						}
						if(wanted_size < samples_size)
						{
							/* remove samples */
							samples_size = wanted_size;
						}
						else
							if(wanted_size > samples_size)
							{
								uint8_t *samples_end, *q;
								int nb;
								/* add samples by copying final sample*/
								nb = (samples_size - wanted_size);
								samples_end = (uint8_t *)samples + samples_size - n;
								q = samples_end + n;
								while(nb > 0)
								{
									memcpy(q, samples_end, n);
									q += n;
									nb -= n;
								}
								samples_size = wanted_size;
							}
				}
			}
		}
		else
		{
			/* difference is TOO big; reset diff stuff */
			is->audio_diff_avg_count = 0;
			is->audio_diff_cum = 0;
		}
	}
	return samples_size;
}


int audio_decode_frame(VideoState *is, double *pts_ptr)
{
	int len1, data_size, n;
	AVPacket *pkt = &is->audio_pkt;
	double pts;
	for(;;)
	{
		while( is->audio_pkt_size > 0 )
		{
			int got_frame = 0;
#ifdef ANDROID
			//len1 = avcodec_decode_audio2(is->audio_st->codec, &is->audio_frame, &got_frame, pkt);
#else
			len1 = avcodec_decode_audio4(is->audio_st->codec, &is->audio_frame, &got_frame, pkt);
#endif
			if(len1 < 0) 
			{
				/* if error, skip frame */
				is->audio_pkt_size = 0;
				break;
			}
			if (got_frame)
			{
				data_size = av_samples_get_buffer_size(NULL, is->audio_st->codec->channels,is->audio_frame.nb_samples,is->audio_st->codec->sample_fmt,1);
				memcpy(is->audio_buf, is->audio_frame.data[0], data_size);
			}
			is->audio_pkt_data += len1;
			is->audio_pkt_size -= len1;
			if(data_size <= 0) 
			{
				/* No data yet, get more frames */
				continue;
			}
			pts = is->audio_clock;
			*pts_ptr = pts;
			n = 2 * is->audio_st->codec->channels;
			is->audio_clock += (double)data_size /(double)(n * is->audio_st->codec->sample_rate);

			/* We have data, return it and come back for more later */
			return data_size;
		}
		if(pkt->data)
			av_free_packet(pkt);

		if(is->quit) 
		{
			return -1;
		}
		/* next packet */
		if(packet_queue_get(&is->audioq, pkt) < 0) 
		{
			return -1;
		}
		if(pkt->data == g_flush_pkt.data)
		{
			avcodec_flush_buffers(is->audio_st->codec);
			continue;
		}
		is->audio_pkt_data = pkt->data;
		is->audio_pkt_size = pkt->size;
		/* if update, update the audio clock w/pts */
		if(pkt->pts != AV_NOPTS_VALUE) 
		{
			is->audio_clock = av_q2d(is->audio_st->time_base)*pkt->pts;
		}
	}
}


void	audio_thread(size_t _workParameter, size_t _pUri)
{
	VideoState *is = (VideoState *)_workParameter;
	int len1 = 0, frameFinished = 0;
	int	audio_size = 0;
	//AVPacket pkt1;
#ifdef ANDROID
	AVFrame *pFrame = av_frame_alloc();
#else
	AVFrame *pFrame = av_frame_alloc();
#endif
	if( is->quit )
		return;
	double pts;
	for(;;)
	{
		audio_size = audio_decode_frame(is, &pts) ;
	}
	return;
}

void video_display(VideoState *is) 
{
	VideoPicture *vp;
	double aspect_ratio;
	//int w, h, x, y;

	vp = &is->pictq[is->pictq_rindex];
	if(vp->bmp)
	{
		if(is->video_st->codec->sample_aspect_ratio.num == 0)
		{
			aspect_ratio = 0;
		}
		else 
		{
			aspect_ratio = av_q2d(is->video_st->codec->sample_aspect_ratio) *is->video_st->codec->width / is->video_st->codec->height;
		}
		if(aspect_ratio <= 0.0) 
		{
			aspect_ratio = (float)is->video_st->codec->width /(float)is->video_st->codec->height;
		}
		//h = g_pScreen->h;
		//w = ((int)(h * aspect_ratio)) & -3;
		//if(w > g_pScreen->w) 
		//{
		//	w = g_pScreen->w;
		//	h = ((int)(w / aspect_ratio)) & -3;
		//}
		//x = (g_pScreen->w - w) / 2;
		//y = (g_pScreen->h - h) / 2;
	}
	//draw into screen
}

void video_refresh_timer()
{

	VideoState *is = global_video_state;
	VideoPicture *vp;
	double actual_delay, delay, sync_threshold, ref_clock, diff;

	if(is->video_st) 
	{
		if( is->pictq_size == 0 )
		{
		}
		else
		{
			vp = &is->pictq[is->pictq_rindex];

			is->video_current_pts = vp->pts;
			is->video_current_pts_time = av_gettime();

			delay = vp->pts - is->frame_last_pts; /* the pts from last time */
			if(delay <= 0 || delay >= 1.0) 
			{
				/* if incorrect delay, use previous one */
				delay = is->frame_last_delay;
			}
			/* save for next time */
			is->frame_last_delay = delay;
			is->frame_last_pts = vp->pts;

			/* update delay to sync to audio if not master source */
			if(is->av_sync_type != AV_SYNC_VIDEO_MASTER) 
			{
				ref_clock = get_master_clock(is);
				diff = vp->pts - ref_clock;

				/* Skip or repeat the frame. Take delay into account
				FFPlay still doesn't "know if this is the best guess." */
				sync_threshold = (delay > AV_SYNC_THRESHOLD) ? delay : AV_SYNC_THRESHOLD;
				if(fabs(diff) < AV_NOSYNC_THRESHOLD) 
				{
					if(diff <= -sync_threshold) 
					{
						delay = 0;
					}
					else
					if(diff >= sync_threshold) 
					{
						delay = 2 * delay;
					}
				}
			}

			is->frame_timer += delay;
			/* computer the REAL delay */
			actual_delay = is->frame_timer - (av_gettime() / 1000000.0);
			if(actual_delay < 0.010) 
			{
				/* Really it should skip the picture instead */
				actual_delay = 0.010;
			}
			/* show the picture! */
			video_display(is);

			/* update queue for next picture! */
			if(++is->pictq_rindex == VIDEO_PICTURE_QUEUE_SIZE) 
			{
				is->pictq_rindex = 0;
			}
			{
				cFUSynchronizedHold l_FUSynchronizedHold(&is->pictq_mutex);
				is->pictq_size--;
			}
		}
	}
	else 
	{
	}
}

int queue_picture(VideoState *is, AVFrame *pFrame, double pts,AVFrame *pFrameRGB) 
{
	VideoPicture *vp;

	{
		while(is->pictq_size >= VIDEO_PICTURE_QUEUE_SIZE &&	!is->quit)
		{
			cFUSynchronizedHold l_FUSynchronizedHold(&is->pictq_mutex);
			is->pictq_mutex.ConditionOk();
		}
	}
	if(is->quit)
		return -1;

	// windex is set to 0 initially
	vp = &is->pictq[is->pictq_windex];
	/* We have a place to put our picture on the queue */
	/* If we are skipping a frame, do we set this to null
	but still return vp->allocated = 1? */


	if(vp->bmp) 
	{
		if( g_pImg_convert_ctx == NULL) 
		{
			int w = is->video_st->codec->width;
			int h = is->video_st->codec->height;
			g_pImg_convert_ctx = sws_getContext(w,h,is->video_st->codec->pix_fmt,w,h,PIX_FMT_RGB24,SWS_BICUBIC,NULL, NULL, NULL);
			if(g_pImg_convert_ctx == NULL) 
			{
				fprintf(stderr, "Cannot initialize the conversion context!\n");
				exit(1);
			}
		}

		sws_scale(g_pImg_convert_ctx, pFrame->data, pFrame->linesize,0, is->video_st->codec->height, pFrameRGB->data,pFrameRGB->linesize);
		vp->pts = pts;
		{
			cFUSynchronizedHold	l_FUSynchronizedHold(vp->bmp->GetTexture());
			is->m_pPixelData = (char*)pFrameRGB->data[0];
			is->bFetchPixelData = true;
		}
		/* now we inform our display thread that we have a pic ready */
		if(++is->pictq_windex == VIDEO_PICTURE_QUEUE_SIZE) 
		{
			is->pictq_windex = 0;
		}
		{
			cFUSynchronizedHold l_FUSynchronizedHold(&is->pictq_mutex);
			is->pictq_size++;
		}
	}
	return 0;
}


double synchronize_video(VideoState *is, AVFrame *src_frame, double pts) 
{

	double frame_delay;

	if(pts != 0) 
	{
		/* if we have pts, set video clock to it */
		is->video_clock = pts;
	}
	else 
	{
		/* if we aren't given a pts, set it to the clock */
		pts = is->video_clock;
	}
	/* update the video clock */
	frame_delay = av_q2d(is->video_st->codec->time_base);
	/* if we are repeating a frame, adjust clock accordingly */
	frame_delay += src_frame->repeat_pict * (frame_delay * 0.5);
	is->video_clock += frame_delay;
	return pts;
}

/* These are called whenever we allocate a frame
* buffer. We use this to store the global_pts in
* a frame at the time it is allocated.
*/
uint64_t g_uiVideo_pkt_pts = AV_NOPTS_VALUE;
int our_get_buffer(struct AVCodecContext *c, AVFrame *pic) 
{
	int ret = avcodec_default_get_buffer(c, pic);
	uint64_t *pts = (uint64_t *)av_malloc(sizeof(uint64_t));
	*pts = g_uiVideo_pkt_pts;
	pic->opaque = pts;
	return ret;
}

void our_release_buffer(struct AVCodecContext *c, AVFrame *pic) 
{
	if(pic) av_freep(&pic->opaque);
	avcodec_default_release_buffer(c, pic);
}

void video_thread(size_t _workParameter, size_t _pUri)
{
	VideoState *is = (VideoState *)_workParameter;
	AVPacket pkt1;
	int len1, frameFinished;
	AVFrame *pFrame = av_frame_alloc();;
	AVFrame *pFrameRGB = av_frame_alloc();
	//m_pVideoData->pCodecContext->width,m_pVideoData->pCodecContext->height
	
	int	l_iWidth = is->pFormatCtx->streams[is->iVideoCodecIndex]->codec->width;
	int	l_iHeight = is->pFormatCtx->streams[is->iVideoCodecIndex]->codec->height;
	int	l_iNumBytes = avpicture_get_size(PIX_FMT_RGB24,l_iWidth,l_iHeight);
	uint8_t*l_pRGBFilledData = (uint8_t*)malloc(l_iNumBytes*sizeof(uint8_t));
	avpicture_fill((AVPicture *)pFrameRGB,l_pRGBFilledData, PIX_FMT_RGB24,l_iWidth,l_iHeight);

	double pts;
	for(;;)
	{
		if(packet_queue_get(&is->videoq, &pkt1) < 0)
		{
			//is->quit = 1;
			// means we quit getting packets
			break;
		}
		if(pkt1.data == g_flush_pkt.data)
		{
			avcodec_flush_buffers(is->video_st->codec);
			continue;
		}
		pts = 0;

		// Save global pts to be stored in pFrame in first call
		g_uiVideo_pkt_pts = pkt1.pts;
		// Decode video frame
		len1 = avcodec_decode_video2(is->video_st->codec, pFrame, &frameFinished, &pkt1);

		//len1 = avcodec_decode_video(is->video_st->codec, is->pFrame, &frameFinished, packet->data, packet->size);

		if(pkt1.dts == AV_NOPTS_VALUE  && pFrame->opaque && *(uint64_t*)pFrame->opaque != AV_NOPTS_VALUE)
		{
			pts = (double)*(uint64_t *)pFrame->opaque;
		}
		else
		if(pkt1.dts != AV_NOPTS_VALUE)
		{
			pts = (double)pkt1.dts;
		}
		else
		{
			pts = 0;
		}
		pts *= av_q2d(is->video_st->time_base);

		// Did we get a video frame?
		if(frameFinished)
		{
			pts = synchronize_video(is, pFrame, pts);
			if( !is->pictq[0].bmp )
			{
				//AVCodecContext*l_pAVCodecContext = is->pFormatCtx->streams[is->iVideoCodecIndex]->codec;
				//for(int i=0;i<VIDEO_PICTURE_QUEUE_SIZE;++i)
				//{
				//	VideoPicture*l_pVideoPicture = &is->pictq[i];
				//	if(!l_pVideoPicture->bmp)
				//	{
				//		l_pVideoPicture->bmp = new cBaseImage((char*)0);
				//		l_pVideoPicture->bmp->GetTexture()->SetupTexture(3,l_pAVCodecContext->width,l_pAVCodecContext->height,GL_RGB,GL_UNSIGNED_BYTE,false,0);
				//		l_pVideoPicture->bmp->SetWidth(l_pAVCodecContext->width);
				//		l_pVideoPicture->bmp->SetHeight(l_pAVCodecContext->height);
				//		l_pVideoPicture->bmp->SetWidth(720);
				//		l_pVideoPicture->bmp->SetHeight(1280);
				//	}
				//}
			}
			if(queue_picture(is, pFrame, pts,pFrameRGB) < 0)
			{
				break;
			}
		}
		av_free_packet(&pkt1);
	}
	av_free(pFrame);
	av_free(pFrameRGB);
	free(l_pRGBFilledData);
	{
		cFUSynchronizedHold l_FUSynchronizedHold(&global_video_state->pMutex);
		is->bVideoThreadEnd = true;
		global_video_state->pMutex.ConditionOk();
	}
	return;
}

//check this out for play sound with openAL
//http://stackoverflow.com/questions/21386135/ffmpeg-openal-playback-streaming-sound-from-video-wont-work
//http://stackoverflow.com/questions/14554409/streaming-audio-with-openal-and-ffmpeg

int stream_component_open(VideoState *is, int stream_index)
{
	AVFormatContext *pFormatCtx = is->pFormatCtx;
	AVCodecContext *codecCtx;
	AVCodec *codec;
	AVDictionary    *optionsDict = NULL;
	if(stream_index < 0 || stream_index >= (int)pFormatCtx->nb_streams)
	{
		return 0;
	}

	// Get a pointer to the codec context for the video stream
	codecCtx = pFormatCtx->streams[stream_index]->codec;

	if(codecCtx->codec_type == AVMEDIA_TYPE_AUDIO) 
	{
		if( !is->m_pSound )
		{
			is->m_pSound = new cOpanalWAV(is,(char*)0,false);
			is->m_pSound->SetFormat(AL_FORMAT_MONO16);
			is->m_pSound->SetFreq(codecCtx->sample_rate);
		}

	}
	codec = avcodec_find_decoder(codecCtx->codec_id);
	if(!codec || (avcodec_open2(codecCtx, codec,&optionsDict) < 0)) 
	{
		return 0;
	}

	switch(codecCtx->codec_type) 
	{
	case AVMEDIA_TYPE_AUDIO:
		is->audioStream = stream_index;
		is->audio_st = pFormatCtx->streams[stream_index];
		is->audio_buf_size = 0;
		is->audio_buf_index = 0;

		/* averaging filter for audio sync */
		is->audio_diff_avg_coef = exp(log(0.01 / AUDIO_DIFF_AVG_NB));
		is->audio_diff_avg_count = 0;
		/* Correct audio only if larger error than this */
		is->audio_diff_threshold = 2.0 * SDL_AUDIO_BUFFER_SIZE / codecCtx->sample_rate;
		memset(&is->audio_pkt, 0, sizeof(is->audio_pkt));
		packet_queue_init(&is->audioq);
		{
			//wake up audio thread
			FUStaticFunctor2<size_t, size_t, void>* workFunctor = new FUStaticFunctor2<size_t, size_t, void>(&audio_thread);
			FUStaticFunctor2<size_t, size_t, void>* doneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&video_threadDone);
			is->m_FUThreadPool.ExecuteWork(workFunctor,doneFunctor,(size_t)is,0);
		}
		break;
	case AVMEDIA_TYPE_VIDEO:
		is->videoStream = stream_index;
		is->video_st = pFormatCtx->streams[stream_index];

		is->frame_timer = (double)av_gettime() / 1000000.0;
		is->frame_last_delay = 40e-3;
		is->video_current_pts_time = av_gettime();

		packet_queue_init(&is->videoq);
		{
			FUStaticFunctor2<size_t, size_t, void>* workFunctor = new FUStaticFunctor2<size_t, size_t, void>(&video_thread);
			FUStaticFunctor2<size_t, size_t, void>* doneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&video_threadDone);
			is->m_FUThreadPool.ExecuteWork(workFunctor,doneFunctor,(size_t)is,0);
		}

		codecCtx->get_buffer = our_get_buffer;
		codecCtx->release_buffer = our_release_buffer;
		break;
	default:
		break;
	}

	return 1;
}

//int decode_interrupt_cb()
//{
//	return (global_video_state && global_video_state->quit);
//}

void decode_thread(size_t _workParameter, size_t _pUri)
{
	VideoState *is = (VideoState *)_workParameter;
	AVFormatContext *pFormatCtx;
	AVPacket pkt1, *packet = &pkt1;

	int video_index = -1;
	int audio_index = -1;
	int i;

	is->videoStream=-1;
	is->audioStream=-1;

	// will interrupt blocking functions if we quit!
	//url_set_interrupt_cb(decode_interrupt_cb);

	// Open video file
	if(avformat_open_input(&pFormatCtx, is->filename, NULL, 0)!=0)
	{
		//assert(0&&"openFile failed");
		return ;
	}
	is->pFormatCtx = pFormatCtx;
	//AVDictionary *l_pOptions;
	// Retrieve stream information
	//if(avformat_find_stream_info(pFormatCtx,&l_pOptions)<0)
	if(avformat_find_stream_info(pFormatCtx,0)<0)
		return; // Couldn't find stream information

	// Dump information about file onto standard error
	av_dump_format(pFormatCtx, 0, is->filename, 0);

	// Find the first video stream
	for(i=0; i<(int)pFormatCtx->nb_streams; i++) 
	{
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO &&video_index < 0) 
		{
			video_index=i;
		}
		else
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO &&audio_index < 0)
		{
			audio_index=i;
		}
		else
		{
			avcodec_close(pFormatCtx->streams[i]->codec);
		}
	}
	if(audio_index >= 0) 
	{
		if( g_bOpenSound )
		{
			is->iAudioCodecIndex = audio_index;
			if(stream_component_open(is, audio_index)<1)
			{
				//error
			}
		}
	}
	if(video_index >= 0) 
	{
		is->iVideoCodecIndex = video_index;
		if(stream_component_open(is, video_index) <1)
		{
			//error
		}
	}
	{
		cFUSynchronizedHold l_FUSynchronizedHold(&is->pMutex);
		is->bFileOpen = true;
		is->pMutex.ConditionOk();
	}
	//if(is->videoStream < 0 || is->audioStream < 0) {
	if(is->videoStream < 0 )
	{
		fprintf(stderr, "%s: could not open codecs\n", is->filename);
		return;
	}
	// main decode loop
	for(;;)
	{
		if(is->quit)
		{
			break;
		}
		// seek stuff goes here
		if(is->seek_req)
		{
			int stream_index= -1;
			int64_t seek_target = is->seek_pos;
			if(is->videoStream >= 0)
				stream_index = is->videoStream;
			else
			if(is->audioStream >= 0)
				stream_index = is->audioStream;
			if( stream_index >= 0 )
			{
//#ifdef WIN32
				AVRational	l_prevailing  = {1, AV_TIME_BASE};
				seek_target = av_rescale_q( seek_target,l_prevailing , pFormatCtx->streams[stream_index]->time_base );
//#else
//				seek_target = av_rescale_q( seek_target, AV_TIME_BASE_Q, pFormatCtx->streams[stream_index]->time_base );
//#endif
			}
			if( !av_seek_frame(is->pFormatCtx, stream_index, seek_target, is->seek_flags) )
			{
				//fprintf(stderr, "%s: error while seeking\n", is->pFormatCtx->filename);
			}
			else
			{
				if(is->audioStream >= 0)
				{
					packet_queue_flush(&is->audioq);
					packet_queue_put(&is->audioq, &g_flush_pkt);
				}
				if(is->videoStream >= 0)
				{
					packet_queue_flush(&is->videoq);
					packet_queue_put(&is->videoq, &g_flush_pkt);
				}
			}
			is->seek_req = 0;
		}

		if(is->audioq.size > MAX_AUDIOQ_SIZE ||is->videoq.size > MAX_VIDEOQ_SIZE)
		{
#ifdef WIN32
			Sleep(10);
#else
			sleep(10);
#endif
			continue;
		}
		if(av_read_frame(is->pFormatCtx, packet) < 0 )
		{
			//if(url_ferror(&pFormatCtx->pb) == 0) 
			//{
			//	SDL_Delay(100); /* no error; wait for user input */
			//	continue;
			//}
			//else 
			{
				//break;
			}		
		}
		//else
		{
			// Is this a packet from the video stream?
			if( packet->stream_index == is->videoStream )
			{
				packet_queue_put(&is->videoq, packet);
			}
			else
			if( packet->stream_index == is->audioStream )
			{
				packet_queue_put(&is->audioq, packet);
			}
			else
			{
				av_free_packet(packet);
			}
		}
	}
	/* all done - wait for it */
	while(!is->quit)
	{
#ifdef WIN32
			Sleep(100);
#else
			sleep(100);
#endif
	}
	return;
}

void stream_seek(VideoState *is, int64_t pos, int rel)
{
	if(!is->seek_req)
	{
		is->seek_pos = pos;
		is->seek_flags = rel < 0 ? AVSEEK_FLAG_BACKWARD : 0;
		is->seek_req = 1;
	}
}

void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) 
{
	FILE *pFile;
	char szFilename[32];
	int  y;

	// Open file
	sprintf(szFilename, "frame%d.ppm", iFrame);
	pFile=fopen(szFilename, "wb");
	if(pFile==NULL)
		return;

	// Write header
	fprintf(pFile, "P6\n%d %d\n255\n", width, height);

	// Write pixel data
	for(y=0; y<height; y++)
		fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);

	// Close file
	fclose(pFile);
}
//end WIN32
#endif