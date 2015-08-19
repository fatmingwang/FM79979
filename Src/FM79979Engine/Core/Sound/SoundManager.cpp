#include "../stdafx.h"
#include "SoundManager.h"
#include "../XML/AtgXmlWriter.h"

namespace FATMING_CORE
{
	const wchar_t*     cSoundParser::TypeID( L"cSoundParser" );

	ALuint*	cSoundParser::m_psuiSourceID = 0;//whole source ID
	ALuint*	cSoundParser::m_psuiSourceUsingIDIndex = 0;	//as many as m_siNumSoundManager
	int		cSoundParser::m_siNumSourceID = 0;
	int		cSoundParser::m_siNumSoundManager = 0;
	int		cSoundParser::m_siReserveBGSourceCount = 3;
	bool*	cSoundParser::m_spbUsedBGSourceData = 0;
	void    cSoundParser::ActiveOpenAL()
	{
#ifdef WIN32
		alutInit (0,0);
#endif

#if defined(ANDROID) || defined(IOS)
		// Initialization
		//http://pielot.org/2010/12/14/openal-on-android/#comment-1160
		//http://pielot.org/2010/12/14/openal-on-android/#comment-1160
		m_pDevice = alcOpenDevice(0); // select the "preferred device"

		assert(m_pDevice);
		//for android?
		const ALint context_attribs[] = { ALC_FREQUENCY, 22050, 0 };
		if (m_pDevice)
		{
			// use the device to make a context
			m_pContext = alcCreateContext( m_pDevice,nullptr);
			//m_pContext = alcCreateContext( m_pDevice,context_attribs);
			// set my context to the currently active one
			alcMakeContextCurrent(m_pContext);
		}
#endif
		std::vector<ALuint>	AllSourceID;
		bool	l_bGo = true;
		while(l_bGo)
		{
			ALuint	l_SourceID = 0;
			alGenSources (1, &l_SourceID);
			if( alGetError() != AL_NO_ERROR )
			{
				l_bGo = false;
			}
			else
			{
				AllSourceID.push_back(l_SourceID);
			}
			if( AllSourceID.size()>128 )//assume maxiuma sound source is 128(depend on hardware)
			{
				l_bGo = false;
			}
		}
		UINT	l_uiNum = (UINT)AllSourceID.size();
		m_psuiSourceID = new ALuint[l_uiNum];
		m_psuiSourceUsingIDIndex = new ALuint[l_uiNum];
		m_siNumSourceID = l_uiNum;
		for( UINT i=0;i<l_uiNum;++i )
		{
			m_psuiSourceID[i] = AllSourceID[i];
			m_psuiSourceUsingIDIndex[i] = -1;
		}
		m_spbUsedBGSourceData = new bool[cSoundParser::m_siReserveBGSourceCount];
		for( int i=0;i<cSoundParser::m_siReserveBGSourceCount;++i )
			m_spbUsedBGSourceData[i] = false;
		alGetError();
	}
	cSoundParser::cSoundParser()
	{
		m_fBGMVolume = 1.f;
		m_fSoundEffectVolume = 1.f;
		m_siNumSoundManager++;
		m_fVolume = 1.f;
	#if defined(ANDROID) || defined(IOS)
		assert(!m_psuiSourceID&&"iPhone openal just need one sound manager,because I am lazy to do more");
	#endif
		if( !m_psuiSourceID )
		{
            ActiveOpenAL();
			this->SetVolume(1.f);
		}
	}

	bool	cSoundParser::RemoveObject(int e_iIndex)
	{
		cBasicSound*l_p = this->GetObject(e_iIndex);
		RemoveObjectWithoutDelete(e_iIndex);
		if( l_p )
		{
			l_p->Release(this);
			return true;
		}
		return false;
	}

	cSoundParser::~cSoundParser()
	{
		this->Destroy();
		m_siNumSoundManager--;
		if( m_siNumSoundManager == 0 )
		{
			SAFE_DELETE(cSoundParser::m_spbUsedBGSourceData);
			for( int i=0;i<m_siNumSourceID;++i )
			{
				alSourceStop(m_psuiSourceID[i]);
				alDeleteSources(1, &m_psuiSourceID[i]);
				int error = alGetError();
				if(error != AL_NO_ERROR)
				{
					//assert(0);
				}
			}
			SAFE_DELETE_ARRAY(m_psuiSourceID);
			SAFE_DELETE_ARRAY(m_psuiSourceUsingIDIndex);
	#ifdef DEBUG
			int error = alGetError();
			if(error != AL_NO_ERROR)
			{
				wchar_t*l_sterrorCode = 0;
				if( 0xA001 == error )
					l_sterrorCode = L"AL_INVALID_NAME";
				else
				if(error == 0xA002)
					l_sterrorCode = L"AL_INVALID_ENUM  File data type error??";
				else
				if(error == 0xA003)
					l_sterrorCode = L"AL_INVALID_VALUE";
				else
				if(error == 0xA004)
					l_sterrorCode = L"AL_INVALID_OPERATION";
				else
				if(error == 0xA005)
					l_sterrorCode = L"AL_OUT_OF_MEMORY";
				this->m_strErrorMsg += l_sterrorCode;
			}
	#endif
	#if defined(IOS) || defined(ANDROID)
			// destroy the context
			alcDestroyContext(m_pContext);
			// close the device
			alcCloseDevice(m_pDevice);
	#elif defined(WIN32)			
			alutExit();
	#endif
		}
	}

	void	cSoundParser::SetVolume(float e_fVolume)
	{
		if(e_fVolume>1.f)
			e_fVolume = 1.f;
		if( e_fVolume<0.f )
			e_fVolume = 0.f;
		if(m_fVolume != e_fVolume)
		{
			alListenerf(AL_GAIN,e_fVolume);
			m_fVolume = e_fVolume;
			//for( int i=0;i<m_siNumSourceID;++i )
			//{
			//	alSourcef(m_psuiSourceID[i], AL_GAIN, e_fVolume);
			//}
		}
	}

	void	cSoundParser::Pause(bool e_bPause)
	{
		for( int i=0;i<this->Count();++i )
		{
			(*this)[i]->Pause(e_bPause);
		}
	}

	void	cSoundParser::Stop()
	{
		for( int i=0;i<this->Count();++i )
		{
			(*this)[i]->Play(false);
		}
	}

	void	cSoundParser::Update(float e_fElpaseTime)
	{
		for( int i=0;i<this->Count();++i )
		{
			(*this)[i]->Update(e_fElpaseTime);
		}	
	}

	void	cSoundParser::RemoveAllBGM()
	{
		std::vector<cBasicSound*>	l_BasicSoundVector;
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			if(this->GetObject(i)->Type() == cOpanalOgg::TypeID)
			{
				l_BasicSoundVector.push_back(this->GetObject(i));
			}
		}
		while( l_BasicSoundVector.size() )
		{
			this->RemoveObject(l_BasicSoundVector[0]);
			l_BasicSoundVector.erase(l_BasicSoundVector.begin());
		}
	}
	//The default speed of sound value is 343.3.
	void	cSoundParser::SetSpeed(float e_fSpeed)
	{
		alSpeedOfSound(343.3f*e_fSpeed);  
	}

	bool	cSoundParser::AddSound(NamedTypedObject*e_pRef,const char*e_strFileName)
	{
		std::string	l_extensionName = UT::GetFileExtensionName(e_strFileName);
		if(this->GetObject(UT::CharToWchar(UT::GetFileNameWithoutFullPath(e_strFileName)).c_str()))
		    return false;
		if( !memcmp(l_extensionName.c_str(),"wav",sizeof(char)*3) )
		{
			cOpanalWAV*	l_pOpanalWAV = new cOpanalWAV(0,e_strFileName,false);
			l_pOpanalWAV->AddRef(e_pRef);
			if( wcslen(l_pOpanalWAV->GetName()) != 0 )
			{
				if(!this->AddObject(l_pOpanalWAV))//such sound already existed
				{
					l_pOpanalWAV->Release(e_pRef);
				}
				else
					return true;
			}
			else
			{
				l_pOpanalWAV->Release(e_pRef);
			}
		}
		else
		if( !memcmp(l_extensionName.c_str(),"ogg",sizeof(char)*3) )
		{
			cOpanalOgg*	l_pOpanalOgg = new cOpanalOgg(0,e_strFileName,true);
			l_pOpanalOgg->AddRef(e_pRef);
			if( wcslen(l_pOpanalOgg->GetName()) != 0 )
			{
				if(!this->AddObject(l_pOpanalOgg))//such sound already existed
				{
					l_pOpanalOgg->Release(e_pRef);
				}
				else
					return true;
			}
		}
		else
		{
			this->m_strErrorMsg += UT::CharToWchar(e_strFileName);
			this->m_strErrorMsg += L" not support such format\n";
		}
		return false;
	}

	bool	cSoundParser::AddStreamingSound(NamedTypedObject*e_pRef,const char*e_strFileName)
	{
		return AddSound(e_pRef,e_strFileName);
	}


	bool	cSoundParser::AddStaticSound(NamedTypedObject*e_pRef,const char*e_strFileName)
	{
		return AddSound(e_pRef,e_strFileName);
	}

	void	cSoundParser::HandleElementData(TiXmlElement*e_pTiXmlElement)
	{
		const wchar_t*	l_strName = e_pTiXmlElement->Value();
		if( !wcslen(l_strName) )
			return;
		COMPARE_NAME("Static")
			ProcessStaticData();
		else
		COMPARE_NAME("Streaming")
			ProcessStreamingData();
		else
		COMPARE_NAME("SoundData")
			ProcessRootDirectory();
	}

	void	cSoundParser::ProcessStreamingData()
	{
		PARSE_CURRENT_ELEMENT_START
			COMPARE_NAME("Name")
			{

			}
			else
			COMPARE_NAME("Loop")
			{

			}
		PARSE_NAME_VALUE_END
	}

	void	cSoundParser::ProcessRootDirectory()
	{
		PARSE_CURRENT_ELEMENT_START
			COMPARE_NAME("Root")
			{
				sprintf(this->m_strDestDirectory,"%s\0",UT::WcharToChar(l_strValue).c_str());
				//sprintf(this->m_strCurrentDirectory,"%s\0",l_strValue);
			}
		PARSE_NAME_VALUE_END
	}

	void	cSoundParser::ProcessStaticData()
	{
		char	l_strSoundName[TEMP_SIZE];
		bool	l_bLoop = false;
		int		l_iPiority = 0;
		float	l_fVolume = 1.f;
		PARSE_CURRENT_ELEMENT_START
			COMPARE_NAME("Name")
			{
				sprintf(l_strSoundName,"%s%s\0",this->m_strDestDirectory,UT::WcharToChar(l_strValue).c_str());
			}
			else
			COMPARE_NAME("Loop")
			{
				l_bLoop = VALUE_TO_BOOLEAN;
			}
			else
			COMPARE_NAME("Piority")
			{
				l_iPiority = VALUE_TO_INT;
			}
			else
			COMPARE_NAME("Volume")
			{
				l_fVolume = GetFloat(l_strValue);
			}
		PARSE_NAME_VALUE_END
		if(this->AddStaticSound(this,l_strSoundName))
		{
			if( l_bLoop )
				this->m_ObjectList[m_ObjectList.size()-1]->SetLoop(true);
			this->m_ObjectList[m_ObjectList.size()-1]->SetPiority(l_iPiority);
			if( l_fVolume != 1.f )
			{
				this->m_ObjectList[m_ObjectList.size()-1]->SetVolume(l_fVolume);
			}
		}
	}


	void	cSoundParser::Export(char*e_strFileName)
	{
	#ifdef WIN32
		int	l_iNum = Count();
		ATG::XMLWriter	l_XMLWriter(e_strFileName);
		l_XMLWriter.StartElement("SoundData");
		l_XMLWriter.AddAttribute("Root","Sound/");
			for( int i=0;i<l_iNum;++i )
			{
				cBasicSound*l_pBasicSound = (*this)[i];
				const wchar_t*	l_strName = l_pBasicSound->GetName();
				bool	l_bLoop = l_pBasicSound->IsLoop();
				char	l_strFinalName[TEMP_SIZE];
				if(!wcscmp(l_pBasicSound->Type(),cOpanalOgg::TypeID))
				{
					sprintf(l_strFinalName,"%s.ogg",UT::WcharToChar(l_strName).c_str());
					//cOpanalOgg*l_pOpanalOgg = (cOpanalOgg*)l_pBasicSound;			
				}
				else
				if(!wcscmp(l_pBasicSound->Type(),cOpanalWAV::TypeID))
				{
					sprintf(l_strFinalName,"%s.wav",UT::WcharToChar(l_strName).c_str());
					//cOpanalWAV*l_pOpanalWAV = (cOpanalWAV*)l_pBasicSound;
				}
				l_XMLWriter.StartElement("Static");
				l_XMLWriter.AddAttribute("Name",l_strFinalName);
				l_XMLWriter.AddAttribute("Loop",l_bLoop);
				l_XMLWriter.EndElement();
			}
		//end SoundData
		l_XMLWriter.EndElement();
	#endif
	}
}