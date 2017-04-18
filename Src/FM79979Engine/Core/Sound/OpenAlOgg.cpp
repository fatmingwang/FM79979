#include "../stdafx.h"
#include "OpenAlOgg.h"
#include "SoundManager.h"
#include "../GameplayUT/GameApp.h"



#define _OGG_

#ifdef IOS
#include "ogg/ogg/ogg.h"
#include "ogg/vorbis/vorbisfile.h"
#elif defined(ANDROID)
#include "../../ogg/ogg/ogg.h"
#include "../../ogg/vorbis/vorbisfile.h"
#elif defined(WIN32)
#include "../../../Include/ogg/ogg.h"
#include "../../../Include/vorbis/vorbisfile.h"
#else//linux
//#include "ogg.h"
//#include "vorbisfile.h"
#endif

namespace FATMING_CORE
{
	bool LoadOGG(const char *fileName, vector<char> &buffer, ALenum &format, ALsizei &freq,float&e_fTotalPlayTime)
	{
#ifdef _OGG_
		int endian = 0;                         // 0 for Little-Endian, 1 for Big-Endian
		int bitStream;
		long bytes;
		char array[OGG_STREAMING_SOUND_BUFFER_SIZE];                // Local fixed size array
		NvFile *f;

		// Open for binary reading
		f = UT::MyFileOpen(fileName, "rb" );

		if (f == nullptr)
		{
			UT::ErrorMsg("can't open Sound file",fileName);
		}
		// end if

		vorbis_info *pInfo;
		OggVorbis_File oggFile;

		// Try opening the given file
		if (ov_open(f, &oggFile, nullptr, 0) != 0)
		{
			UT::ErrorMsg("can't open ogg file",fileName);
		}
		e_fTotalPlayTime = (float)ov_time_total(&oggFile,0);
		// end if

		// Get some information about the OGG file
		pInfo = ov_info(&oggFile, -1);

		// Check the number of channels... always use 16-bit samples
		if (pInfo->channels == 1)
			format = AL_FORMAT_MONO16;
		else
			format = AL_FORMAT_STEREO16;
		// end if

		// The frequency of the sampling rate
		freq = pInfo->rate;

		// Keep reading until all is read
		do
		{
			// Read up to a buffer's worth of decoded sound data
			bytes = ov_read(&oggFile, array, OGG_STREAMING_SOUND_BUFFER_SIZE, endian, 2, 1, &bitStream);

			if (bytes < 0)
			{
				return false;
			}
			// end if

			// Append to end of buffer
			buffer.insert(buffer.end(), array, array + bytes);
		}
		while (bytes > 0);
		//l_fTime = ov_time_total(&oggFile,1);
		// Clean up!
		ov_clear(&oggFile);
	//#else
	//	assert(0&&"apple ogg file........Z_Z");
	return true;
#endif
	}

	TYPDE_DEFINE_MARCO(cOpanalOgg);
	cOpanalOgg::cOpanalOgg(NamedTypedObject*e_pNamedTypedObject,const char*e_strileName,bool e_bStreaming,std::function<void(int e_iCount,char*e_pData,size_t e_iCurrentPCMDataPosIndex)> e_CallbuckFunction,bool e_bPreCache):cBasicSound(e_pNamedTypedObject,e_bStreaming)
	{
		m_bPreCached = e_bPreCache;
		m_uiHowManyDataRead = 0;
		m_UpdteNewBufferCallbackFunction = e_CallbuckFunction;
		m_fTimeToUpdate.SetTargetTime(0.1f);
		m_fTimeToUpdate.SetLoop(true);
		m_bPlayDone = true;
		m_pFile = 0;
		m_pOggFile = 0;
		m_pVorbisInfo = 0;
		m_VorbisComment = 0;
		m_uiStreamBuffers[0] = 0;
		m_uiStreamBuffers[1] = 0;
		m_fCurrentStreamingTime = 0.f;
		m_uiIndex = -1;
		if(OpenFile(e_strileName))
		{
			int	l_iReserveBGSourceCount = cSoundParser::m_siReserveBGSourceCount;
			for(int i=0;i<l_iReserveBGSourceCount;++i)
			{
				if(!cSoundParser::m_spbUsedBGSourceData[i])
				{
					cSoundParser::m_spbUsedBGSourceData[i] = true;
					m_uiIndexFor_cSoundParser_m_psuiSourceUsingIDIndex = i;
					m_uiIndex = i;
					cSoundParser::m_psuiSourceUsingIDIndex[m_uiIndexFor_cSoundParser_m_psuiSourceUsingIDIndex] = m_uiIndex;
					break;
				}
			}
			if( m_uiIndex == -1 )
			{
				cGameApp::OutputDebugInfoString(L"over than 3 BG exists is not allow");
			}
		}
	}

	cOpanalOgg::~cOpanalOgg()
	{
		this->Destroy();
		for(int i=0;i<cSoundParser::m_siReserveBGSourceCount;++i)
		{
			if(cSoundParser::m_spbUsedBGSourceData[i])
			{
				if(m_uiIndexFor_cSoundParser_m_psuiSourceUsingIDIndex == i)
				{
					cSoundParser::m_spbUsedBGSourceData[i] = false;
					m_uiIndex = -1;
					cSoundParser::m_psuiSourceUsingIDIndex[i] = -1;
					return;
				}
			}
		}
	}

	void	cOpanalOgg::Destroy()
	{
		check();
		if(this->m_bStreaming)
		{
			if( m_uiSourceID )
			{
				alSourceStop(this->m_uiSourceID);
				Empty();
				alDeleteSources(1, &m_uiSourceID);
				check();
			}
			if( m_uiStreamBuffers[0 ]!= 0 )
			{
				alDeleteBuffers(2, this->m_uiStreamBuffers);
				check();
			}
			if( m_pOggFile )
			{
				ov_clear(m_pOggFile);
				SAFE_DELETE(m_pOggFile);
			}
			if( m_pFile )
			{
				NvFClose(m_pFile);
				m_pFile = 0;
			}
		}
		else
		{
			cBasicSound::Destroy();
		}
		check();
	}

	bool	cOpanalOgg::OpenFile(const char*e_strileName)
	{
		m_uiHowManyDataRead = 0;
		this->Destroy();
		if(m_bStreaming)
		{
			int result;
			if(!(m_pFile = UT::MyFileOpen(e_strileName, "rb")))
			{
				std::string	l_str = "Could not open Ogg file.";
				l_str += e_strileName;
				cGameApp::OutputDebugInfoString(l_str);
				return false;
			}

			m_pOggFile = new OggVorbis_File();
			if((result = ov_open(m_pFile,m_pOggFile, nullptr, 0)) < 0)
			{
				Destroy();
				std::string	l_str = "Could not open Ogg stream";
				l_str += e_strileName;
				cGameApp::OutputDebugInfoString(l_str);
				return false;
			}
			//fetch by m_pOggFile
			m_pFile = 0;
			SetName(UT::CharToWchar(UT::GetFileNameWithoutFullPath(e_strileName)).c_str());
			m_pVorbisInfo = ov_info(m_pOggFile, -1);
			m_VorbisComment = ov_comment(m_pOggFile, -1);
			if(m_pVorbisInfo->channels == 1)
				this->m_iFormat = AL_FORMAT_MONO16;
			else
			if(m_pVorbisInfo->channels == 2)
				m_iFormat = AL_FORMAT_STEREO16;
			else
			{
				UT::ErrorMsg(L"sorry there is too much channel data",ValueToStringW(m_pVorbisInfo->channels).c_str());
			}
			cBasicSound::SetChannelByFormat(m_iFormat);
			alGenBuffers(2, m_uiStreamBuffers);
			check();
			alGenSources(1, &m_uiSourceID);
			check();
			m_fTimeLength = (float)ov_time_total(m_pOggFile,-1);
			m_iFreq = m_pVorbisInfo->rate;
			//ogg sample bit is 16!.?
			m_iPCMDataSize = cBasicSound::CalculatePCMDataSize(m_pVorbisInfo->channels,m_iFreq,m_fTimeLength,16);
			//
			if( m_bPreCached )
				Playback(false);
			
		}
		else
		{
			alGenBuffers(1, &m_uiBufferID);
			alGenSources(1, &m_uiSourceID);		
			vector<char> buffer;
			LoadOGG(e_strileName,buffer,m_iFormat,m_iFreq,m_fTimeLength);
			m_iPCMDataSize = static_cast<ALsizei>(buffer.size());
			cBasicSound::SetChannelByFormat(m_iFormat);
			//16bit is 2 byte,assume all ogg file is 16 bit...
			assert(abs(cBasicSound::CalculatePCMDataSize(this->m_iChannelCount,m_iFreq,m_fTimeLength,16)-m_iPCMDataSize)<2&&"PCM data size should be same or I get wrong");
			if( buffer.size() )
			{
				AssignBufferData(&buffer[0]);
				SetName(UT::CharToWchar(UT::GetFileNameWithoutFullPath(e_strileName)).c_str());
			}
		}
		return true;
	}


	string cOpanalOgg::ErrorString(int code)
	{
		switch(code)
		{
			case OV_EREAD:
				return string("Read from media.");
			case OV_ENOTVORBIS:
				return string("Not Vorbis data.");
			case OV_EVERSION:
				return string("Vorbis version mismatch.");
			case OV_EBADHEADER:
				return string("Invalid Vorbis header.");
			case OV_EFAULT:
				return string("Internal logic fault (bug or heap/stack corruption.");
			default:
				return string("Unknown Ogg error.");
		}
	}

	bool cOpanalOgg::Stream(ALuint buffer)
	{
		char pcm[OGG_STREAMING_SOUND_BUFFER_SIZE];
		int  size = 0;
		int  section;
		int  result;
		size_t l_uiStartPos = m_uiHowManyDataRead;
		while(size < OGG_STREAMING_SOUND_BUFFER_SIZE)
		{
			result = ov_read(m_pOggFile,pcm+size, OGG_STREAMING_SOUND_BUFFER_SIZE - size, 0, 2, 1, &section);
	    
			if(result > 0)
			{
				size += result;
				m_uiHowManyDataRead += result;
			}
			else
			if(result < 0)
			{
				this->m_bPlayDone = true;
				cGameApp::OutputDebugInfoString(ErrorString(result));
			}
			else
			if(result == 0)
			{
				this->m_bPlayDone = true;
				break;
			}
		}
	    
		if(size == 0)
		{
			return false;
		}
		//this->m_iFreq*this->m_ich;
		//AL_FORMAT_MONO16;
		//AL_FORMAT_MONO8;
		//AL_FORMAT_STEREO8;
		//AL_FORMAT_STEREO16;
		//size;
		//here is possible lock by thread safe synchroization?
		m_fCurrentStreamingTime = (float)m_uiHowManyDataRead/(float)this->m_iPCMDataSize*this->m_fTimeLength;
	    if(m_UpdteNewBufferCallbackFunction)
		{
			//ogg_int64_t l_i64Size = m_pOggFile->end-m_pOggFile->offsets;
			m_UpdteNewBufferCallbackFunction(size,pcm,l_uiStartPos);
		}
		alBufferData(buffer, this->m_iFormat, pcm, size, m_pVorbisInfo->rate);
		check();
	    
		return true;
	}

	bool cOpanalOgg::Playback(bool e_bPlaySound)
	{
		ALenum state;    
		alGetSourcei(this->m_uiSourceID, AL_SOURCE_STATE, &state);    
		if(state == AL_PLAYING)
			return true;
	    for( int i=0;i<2;++i )
		{
			if(!Stream(m_uiStreamBuffers[i]))
			{
				if( m_bLoop )
				{
					ov_pcm_seek(this->m_pOggFile, 0);
					if(!Stream(m_uiStreamBuffers[i]))
						return false;
				}
				else
					return false;
			}
		}
		alSourceQueueBuffers(this->m_uiSourceID, 2, this->m_uiStreamBuffers);
		check();
		if( e_bPlaySound )
			this->Play(true);
		return true;
	}

	void cOpanalOgg::Empty()
	{
		int queued;
	    
		alGetSourcei(m_uiSourceID, AL_BUFFERS_QUEUED, &queued);
	    
		while(queued--)
		{
			ALuint buffer;
	    
			alSourceUnqueueBuffers(m_uiSourceID, 1, &buffer);
			check();
		}
	}

	void cOpanalOgg::Update(float e_fElpaseTime)
	{
		if( m_uiSourceID == 0 )
			return;
		if( m_bPlayDone && this->m_bLoop )
		{
			m_bPlayDone = false;
			//rewind
		}
		if(!this->m_bStreaming  ||this->m_bPause || m_bPlayDone )
		{
			return;
		}
		m_fTimeToUpdate.Update(e_fElpaseTime);
		if( !m_fTimeToUpdate.bTragetTimrReached )
		{
			return;
		}
		int processed;
		bool l_bActive = true;
		alGetSourcei(this->m_uiSourceID, AL_BUFFERS_PROCESSED, &processed);
		while(processed--)
		{
			ALuint buffer;
			alSourceUnqueueBuffers(this->m_uiSourceID, 1, &buffer);
			check();
			l_bActive = Stream(buffer);
			alSourceQueueBuffers(m_uiSourceID, 1, &buffer);
			check();
		}
		if( l_bActive )
		{
			if(!IsPlay())
			{
				this->Play(true);
			}
		}
		if( m_bPlayDone && this->m_bLoop )
		{
			GoTo(0.f);
		}
	}

	void	cOpanalOgg::Play(bool e_bPlay)
	{
		if( !m_bPreCached )
		{
			m_bPreCached = true;
			this->Playback(false);
		}
		cBasicSound::Play(e_bPlay);
		m_bPlayDone = !e_bPlay;
	}

	bool	cOpanalOgg::GoTo(float e_fTime)
	{
		float	l_fPercentage = e_fTime/this->m_fTimeLength;
		if( l_fPercentage >1.f )
			l_fPercentage = 1.f;
		float	l_fTime = l_fPercentage*this->m_fTimeLength;
		m_uiHowManyDataRead = (int)(l_fPercentage*this->m_iPCMDataSize);
		return ov_time_seek(this->m_pOggFile,l_fTime) == 0?true:false;
	}

	void	cOpanalOgg::SetUpdateNewBufferCallbackFunction(std::function<void(int e_iCount,char*e_pData,size_t e_iCurrentPCMDataPosIndex)> e_CallbuckFunction)
	{
		m_UpdteNewBufferCallbackFunction = e_CallbuckFunction;
	}
}