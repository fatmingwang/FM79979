#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "OpenAlWavFile.h"
#include "OpenAlOgg.h"
#include "../XML/XMLLoader.h"
//=============================
//current do not support streaming music
//=============================
//enum	eSoundDataType
//{
//	eSDT_WAV = 0,
//	eSDT_STREAMING,
//	eSDT_NONE
//};
namespace FATMING_CORE
{
	class	cSoundParser:public cNamedTypedObjectVector<cBasicSound>,public ISAXCallback
	{
	    void    ActiveOpenAL();
	#if defined(IOS) || defined(ANDROID)
		ALCdevice*	m_pDevice;
		ALCcontext*	m_pContext;	
	#endif
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
		void			ProcessStreamingData();
		void			ProcessStaticData();
		void			ProcessRootDirectory();
		char			m_strDestDirectory[TEMP_SIZE];
		float			m_fVolume;
	public:
		DEFINE_TYPE_INFO();
		//reserve for BG sound
		static	int		m_siReserveBGSourceCount;
		//how many reserver BG sound ID is used
		static	bool*	m_spbUsedBGSourceData;
		//below for sort source ID,because the source hardearw is limited,so we have to assign the source dymanic
		static	int		m_siNumSourceID;
		static	ALuint*	m_psuiSourceID;				//whole source ID,as many as m_siNumSoundManager
		static	ALuint*	m_psuiSourceUsingIDIndex;	//as many as m_siNumSoundManager
		static	int		m_siNumSoundManager;
		//I have call alutInit (0,0); in the construction while sound manager is first time to create
		cSoundParser();
		virtual ~cSoundParser();
		//the referance should be use cResourceStamp to instead but I am lazy to fix right now
		cBasicSound*	AddStaticSound(NamedTypedObject*e_pRef,const char*e_strFileName);
		cBasicSound*	AddStreamingSound(NamedTypedObject*e_pRef,const char*e_strFileName);
		cBasicSound*	AddSound(NamedTypedObject*e_pRef,const char*e_strFileName);
		cBasicSound*	AddSound(const char*e_strFileName);
		void			Export(char*e_strFileName);
		//for 0			to 1
		void			SetVolume(float e_fVolume);
		float			GetVolume(){ return m_fVolume; }
		float			m_fBGMVolume;
		float			m_fSoundEffectVolume;
		void			Pause(bool e_bPause);	//all sound pause
		void			Stop();					//all sound stop
		void			SetSpeed(float e_fSpeed);
		void			Update(float e_fElpaseTime);
		void			RemoveAllBGM();
		//SimpleGLTexture could't add into cNamedTypedObjectVector
		//but cBasicSound is able to do this,so release got a bit bitter,so I am override RemoveObject
		//for release working decent.
		virtual	bool	RemoveObject(int e_iIndex);
		using			cNamedTypedObjectVector<cBasicSound>::RemoveObject;
	};
}
#endif
//
//OpenAL gain means volume. So you have to change the AL_GAIN state to change the volume of a source (you can't change the volume of a buffer as far as I have seen, and it seems strange why you would).
//
//So
//
//// To set the volume of the source to max
//alSourcef(sourceID, AL_GAIN, 1.0f);
//
//// To set it to min
//alSourcef(sourceID, AL_GAIN, 0.0f);
//
//// And halfway 
//alSourcef(sourceID, AL_GAIN, 0.5f);