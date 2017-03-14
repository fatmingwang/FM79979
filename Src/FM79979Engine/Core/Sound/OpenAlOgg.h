#ifndef _OPENAL_OGG_H_
#define _OPENAL_OGG_H_

#include "BasicSound.h"

struct OggVorbis_File;
struct vorbis_info;
struct vorbis_comment;

namespace FATMING_CORE
{
	//now we are assume all ogg file is streaming.
	//so please call update
	//
	class	cOpanalOgg:public cBasicSound
	{
		UT::sTimeCounter	m_fTimeToUpdate;//default is 0.3 second
        NvFile*				m_pFile;
        OggVorbis_File*		m_pOggFile;
        vorbis_info*		m_pVorbisInfo;
        vorbis_comment*		m_VorbisComment;
        ALuint				m_uiStreamBuffers[2];
		void				Empty();
		bool				Playback(bool e_bPlaye);//put data into buffer
		bool				Stream(ALuint buffer);
		bool				m_bPlayDone;
		string				ErrorString(int code);
		std::function<void(int e_iCount,char*e_pData)>	m_UpdteNewBufferCallbackFunction;
	public:
		DEFINE_TYPE_INFO();
		cOpanalOgg(NamedTypedObject*e_pNamedTypedObject,const char*e_strileName,bool e_bStreaming);
		virtual ~cOpanalOgg();
		virtual	NamedTypedObject*	Clone(){ return 0; }
		virtual bool	OpenFile(const char*e_strileName);
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	Destroy();
		virtual bool	GoTo(float e_fTime);
		virtual	void	Play(bool e_bPlay);
		void			SetUpdateNewBufferCallbackFunction(std::function<void(int e_iCount,char*e_pData)> e_CallbuckFunction);
	};
	bool LoadOGG(const char *fileName, vector<char> &buffer, ALenum &format, ALsizei &freq,float&e_fTotalPlayTime);

}
#endif