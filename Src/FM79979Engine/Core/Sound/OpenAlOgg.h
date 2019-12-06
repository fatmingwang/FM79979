#pragma once

#include "BasicSound.h"
#include <functional>
struct OggVorbis_File;
struct vorbis_info;
struct vorbis_comment;

namespace FATMING_CORE
{
	//now we are assume all ogg file is streaming.
	//so please call update
	//
	#define	OGG_SOUND_STREAMING_CALL_BACK		std::function<void(int e_iCount,char*e_pfData,size_t e_iCurrentPCMDataPosIndex,int e_iBitPersample)>	
	class	cOpanalOgg:public cBasicSound
	{
		//for streaming get the data has been readed
		size_t				m_uiHowManyDataRead;
		//
		UT::sTimeCounter	m_fTimeToUpdate;//default is 0.3 second
        NvFile*				m_pFile;
        OggVorbis_File*		m_pOggFile;
        vorbis_info*		m_pVorbisInfo;
        vorbis_comment*		m_VorbisComment;
        ALuint				m_uiStreamBuffers[2];
		float				m_fCurrentStreamingTime;//while new data is ready to stream set this time
		void				Empty();
		//bool				Playback(bool e_bPlaye);//put data into buffer
		bool				Stream(ALuint buffer);
		bool				m_bPlayDone;
		bool				Playback(bool e_bPlaye);//put data into buffer
		//if this is true,it will take a little time to cache while cOpanalOgg is trying to new.if you want to cache later set to false,anyway cache need time to process
		bool				m_bPreCached;
		string				ErrorString(int code);
		//bit persample is 4(32bit)
		OGG_SOUND_STREAMING_CALL_BACK	m_UpdteNewBufferCallbackFunction;
	public:
		DEFINE_TYPE_INFO()
		cOpanalOgg(NamedTypedObject*e_pNamedTypedObject,const char*e_strileName,bool e_bStreaming,OGG_SOUND_STREAMING_CALL_BACK e_CallbuckFunction = nullptr,bool e_bPreCache = true);
		virtual ~cOpanalOgg();
		virtual	NamedTypedObject*	Clone()override { return 0; }
		virtual bool	OpenFile(const char*e_strileName)override;
		virtual	void	Update(float e_fElpaseTime)override;
		virtual	void	Destroy()override;
		virtual bool	GoTo(float e_fTime)override;
		virtual	void	Play(bool e_bPlay)override;
		void			SetUpdateNewBufferCallbackFunction(OGG_SOUND_STREAMING_CALL_BACK e_CallbuckFunction);
		float			GetCurrentStreamingTime(){ return m_fCurrentStreamingTime; }
	};
	bool LoadOGG(const char *fileName, vector<char> &buffer, ALenum &format, ALsizei &freq,float&e_fTotalPlayTime);
	#define OGG_STREAMING_SOUND_BUFFER_SIZE     32768//32 KB buffers
}