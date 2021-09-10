#include "../stdafx.h"
#include "SoundManager.h"
#ifdef WIN32
#include "../XML/AtgXmlWriter.h"
#endif
#include "../Common/StringToStructure.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cSoundParser);

	ALuint*	cSoundParser::m_psuiSourceID = nullptr;//whole source ID
	ALuint*	cSoundParser::m_psuiSourceUsingIDIndex = nullptr;	//as many as m_siNumSoundManager
	int		cSoundParser::m_siNumSourceID = 0;
	int		cSoundParser::m_siNumSoundManager = 0;
	int		cSoundParser::m_siReserveBGSourceCount = 3;
	bool*	cSoundParser::m_spbUsedBGSourceData = nullptr;
	float	cSoundParser::m_sfBGMVolume = 1.f;
	float	cSoundParser::m_sfSoundEffectVolume = 1.f;

	static void list_audio_devices(const ALCchar* devices)
	{
		const ALCchar* device = devices, * next = devices + 1;
		size_t len = 0;

		fprintf(stdout, "Devices list:\n");
		fprintf(stdout, "----------\n");
		while (device && *device != '\0' && next && *next != '\0') {
			fprintf(stdout, "%s\n", device);
			len = strlen(device);
			device += (len + 1);
			next += (len + 2);
		}
		fprintf(stdout, "----------\n");
	}
	void		displayDevices(const char* type, const char* list)
	{
		ALCchar* ptr, * nptr;

		ptr = (ALCchar*)list;
		printf("list of all available %s devices:\n", type);
		if (!list)
		{
			printf("none\n");
		}
		else
		{
			nptr = ptr;
			while (*(nptr += strlen(ptr) + 1) != 0)
			{
				printf("  %s\n", ptr);
				ptr = nptr;
			}
			printf("  %s\n", ptr);
		}
}

	bool    cSoundParser::ActiveOpenAL()
	{
#if defined(WIN32) || defined(LINUX)
		ALboolean l_bAlutInitResult = alutInit (0,0);
		if( !l_bAlutInitResult )
		{
			UT::ErrorMsg(L"openal create failed!",L"please install oalinst,https://www.openal.org/downloads/");
			return false;
		}
#else
		// Initialization
		//http://pielot.org/2010/12/14/openal-on-android/#comment-1160
		//http://pielot.org/2010/12/14/openal-on-android/#comment-1160
		const ALCchar* s = nullptr;
		//https://stackoverflow.com/questions/28960638/listing-all-devices-open-al-does-not-work
		if(0)
		{
			if (alcIsExtensionPresent(NULL, "ALC_enumeration_EXT") == AL_TRUE)
			{
				//https://github.com/ffainelli/openal-example/blob/master/openal-example.c
				list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

				if (alcIsExtensionPresent(NULL, "ALC_enumerate_all_EXT") == AL_FALSE)
					s = (char*)alcGetString(NULL, ALC_DEVICE_SPECIFIER);
				else
					s = (char*)alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
				displayDevices("output", s);

				s = (char*)alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
				displayDevices("input", s);
			}
		}
		//m_pDevice = alcOpenDevice("bcm2835 HDMI 1, bcm2835 HDMI 1 (CARD=b1,DEV=0)"); // select the "preferred device"
		//m_pDevice = alcOpenDevice("bcm2835 Headphones, bcm2835 Headphones (CARD=Headphones,DEV=0)"); // select the "preferred device"		
		//const char* defname = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);// 
		//m_pDevice = alcOpenDevice(defname); // select the "preferred device"
		m_pDevice = alcOpenDevice(0);

		if (!m_pDevice)
			return false;
		assert(m_pDevice);
		//for android?
		const ALint context_attribs[] = { ALC_FREQUENCY, 22050, 0 };
		if (m_pDevice)
		{
			// use the device to make a context
			m_pContext = alcCreateContext( m_pDevice,nullptr);
			//m_pContext = alcCreateContext( m_pDevice,context_attribs);
			// set my context to the currently active one
			if (m_pContext)
				alcMakeContextCurrent(m_pContext);
			else
				return false;
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
			if( AllSourceID.size() > 64 )//assume maxiuma sound source is 128(depend on hardware)
			{
				l_bGo = false;
			}
		}
		UINT	l_uiNum = (UINT)AllSourceID.size();
		if (l_uiNum == 0)
		{
			FMLog::LogWithFlag(L"OpenAL not actived", CORE_LOG_FLAG,false);
			return false;
		}
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
		return true;
	}
	cSoundParser::cSoundParser()
	{
		m_siNumSoundManager++;
		m_fVolume = 1.f;
#if defined(IOS) || defined(ANDROID) || defined(WASM) || defined(LINUX)
		m_pDevice = nullptr;
		m_pContext = nullptr;
#endif
	#if defined(ANDROID) || defined(IOS)
		assert(!m_psuiSourceID&&"iPhone openal just need one sound manager,because I am lazy to do more");
	#endif
		if( !m_psuiSourceID )
		{
			if (ActiveOpenAL())
			{
				this->SetVolume(1.f);
			}
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
				const wchar_t*l_sterrorCode = 0;
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
	#if defined(IOS) || defined(ANDROID) || defined(WASM) || defined(LINUX)
			// destroy the context
			if(m_pContext)
				alcDestroyContext(m_pContext);
			// close the device
			if(m_pDevice)
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

	cBasicSound*	cSoundParser::AddSound(const char*e_strFileName)
	{
		return this->AddSound(this,e_strFileName);
	}
	NamedTypedObject* cSoundParser::GetObjectByFileName(const char*e_strFileName)
	{
		return AddSound(e_strFileName);
	}

	cBasicSound*	cSoundParser::AddSound(NamedTypedObject*e_pRef,const char*e_strFileName)
	{
		std::string	l_extensionName = UT::GetFileExtensionName(e_strFileName);
		auto l_pSound = this->GetObject(UT::CharToWchar(UT::GetFileNameWithoutFullPath(e_strFileName)).c_str());
		if(l_pSound)
		    return l_pSound;
		if( !memcmp(l_extensionName.c_str(),"wav",sizeof(char)*3) )
		{
			cOpanalWAV*	l_pOpanalWAV = new cOpanalWAV(e_pRef,e_strFileName,false);
			if( wcslen(l_pOpanalWAV->GetName()) != 0 )
			{
				if(!this->AddObject(l_pOpanalWAV))//such sound already existed
				{
					l_pOpanalWAV->Release(e_pRef);
				}
				else
					return l_pOpanalWAV;
			}
			else
			{
				l_pOpanalWAV->Release(e_pRef);
			}
		}
		else
		if( !memcmp(l_extensionName.c_str(),"ogg",sizeof(char)*3) )
		{
			cOpanalOgg*	l_pOpanalOgg = new cOpanalOgg(e_pRef,e_strFileName,true);
			if( wcslen(l_pOpanalOgg->GetName()) != 0 )
			{
				if(!this->AddObject(l_pOpanalOgg))//such sound already existed
				{
					l_pOpanalOgg->Release(e_pRef);
				}
				else
					return l_pOpanalOgg;
			}
			else
			{
				l_pOpanalOgg->Release(e_pRef);
			}
		}
		else
		{
			this->m_strErrorMsg += UT::CharToWchar(e_strFileName);
			this->m_strErrorMsg += L" not support such format\n";
		}
		return nullptr;
	}

	cBasicSound*	cSoundParser::AddStreamingSound(NamedTypedObject*e_pRef,const char*e_strFileName)
	{
		return AddSound(e_pRef,e_strFileName);
	}


	cBasicSound*	cSoundParser::AddStaticSound(NamedTypedObject*e_pRef,const char*e_strFileName)
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
				sprintf(this->m_strDestDirectory,"%s",UT::WcharToChar(l_strValue).c_str());
				//sprintf(this->m_strCurrentDirectory,"%s",l_strValue);
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
				sprintf(l_strSoundName,"%s%s",this->m_strDestDirectory,UT::WcharToChar(l_strValue).c_str());
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


	void	cSoundParser::Export(const char*e_strFileName)
	{
#if defined(WIN32) && !defined(UWP)
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

//	void	cSoundParser::IOSRecordContextSet(bool e_bStartToRecord)
//	{
//#if defined(IOS)
//		if( e_bStartToRecord )
//			alcMakeContextCurrent(nullptr);
//		else
//			alcMakeContextCurrent(m_pContext);
//#endif	
//	}
}