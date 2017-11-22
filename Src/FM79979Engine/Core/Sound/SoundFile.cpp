#include "../stdafx.h"
#include "SoundFile.h"

#include "../GameplayUT/BinaryFile.h"
#include "../GameplayUT/GameApp.h"
#include "OpenAlOgg.h"
#ifndef WIN32
#include "ogg/vorbis/vorbisenc.h"
#endif
const char*	g_strFMTFormat = "fmt ";
const char*	g_strWAVHeader = "RIFF";
const char*	g_strWAVFormat = "WAVE";
const char*	g_strWAVData = "data";

My_WAVFileHdr_Struct GetWAVFileHdr_Struct(int e_iChunkSize)
{
	My_WAVFileHdr_Struct l_My_WAVFileHdr_Struct;
	memcpy(&l_My_WAVFileHdr_Struct.Id,g_strWAVHeader,4);
	l_My_WAVFileHdr_Struct.Size = e_iChunkSize;;
	memcpy(&l_My_WAVFileHdr_Struct.Type,g_strWAVFormat,4);
	return l_My_WAVFileHdr_Struct;
}

My_WAVFmtHdr_Struct GetWAVFmtHdr_Struct(unsigned short e_usFormat,unsigned short e_usChannels,
										unsigned int   e_uiSampleRate,unsigned int   e_uiBytesRate,
										unsigned short e_usBlockAlign,unsigned short e_usBitsPerSample)
{
	My_WAVFmtHdr_Struct l_My_WAVFmtHdr_Struct;
	l_My_WAVFmtHdr_Struct.Format = e_usFormat;
	l_My_WAVFmtHdr_Struct.Channels = e_usChannels;
	l_My_WAVFmtHdr_Struct.SampleRate = e_uiSampleRate;
	l_My_WAVFmtHdr_Struct.BytesRate = e_uiBytesRate;
	l_My_WAVFmtHdr_Struct.BlockAlign = e_usBlockAlign;
	l_My_WAVFmtHdr_Struct.BitsPerSample = e_usBitsPerSample;
	return l_My_WAVFmtHdr_Struct;
}

My_WAVChunkHdr_Struct GetWAVChunkHdr_Struct(unsigned int   Size,const char*e_ChunkID)
{
	My_WAVChunkHdr_Struct l_My_WAVChunkHdr_Struct;
	l_My_WAVChunkHdr_Struct.Size = Size;
	memcpy(&l_My_WAVChunkHdr_Struct.Id,e_ChunkID,strlen(e_ChunkID)*sizeof(char));
	return l_My_WAVChunkHdr_Struct;
}
namespace FATMING_CORE
{
	cSoundFile::cSoundFile()
	{
		m_pWriteFile = nullptr;
		m_pSoundData = nullptr;
		m_Format = 0;
		m_iSoundDataSize = 0;
		m_iFreq = 0;
		m_fTime = 0.f;
		m_iSampleCount = 0;
		m_uiDataSize = 0;
		m_iWriteChannel = 1;
		m_iBitPerSample = 0;
	}

	cSoundFile::~cSoundFile()
	{
		DELETE_VECTOR(m_AllChannelData,char*);
		SAFE_DELETE(m_pSoundData);
		SAFE_DELETE(m_pWriteFile);
	}



	void	cSoundFile::AssignChannelData()
	{

	}


	bool	cSoundFile::OpenWavFile(const char*e_strFileName)
	{
		memset(&m_WAVFileHdr_Struct,0,sizeof(My_WAVFileHdr_Struct ));
		memset(&m_WAVFmtHdr_Struct,0,sizeof(My_WAVFmtHdr_Struct));
		memset(&m_WAVFmtExHdr_Struct,0,sizeof(My_WAVFmtExHdr_Struct ));
		memset(&m_WAVSmplHdr_Struct,0,sizeof(My_WAVSmplHdr_Struct ));
		My_WAVChunkHdr_Struct l_WAVChunkHdr_Struct;
		memset(&l_WAVChunkHdr_Struct,0,sizeof(My_WAVChunkHdr_Struct ));
		NvFile *l_pFile = nullptr;
		SAFE_DELETE(m_pSoundData);

		this->m_iSampleCount = 0;
		this->m_iBitPerSample  = 0;
		this->m_Format = AL_FORMAT_MONO16;
		this->m_fTime = 0.f;
		this->m_iSoundDataSize = 0;
		this->m_iFreq = 0;
		bool l_bLoop = AL_FALSE;
		if (e_strFileName)
		{
			l_pFile = MyFileOpen((const char*)e_strFileName, "rb" );
			if (l_pFile)
			{
					//test code
					//size_t l_uiFileSize = UT::GetFileSize(l_pFile);
					//int l_iStartPos = 3;
					//NvFSeek(l_pFile,l_iStartPos,SEEK_CUR);
					//char mmm[99];
					//char mmm2[99];
					//memset(mmm,0,sizeof(char)*99);
					//memset(mmm2,0,sizeof(char)*99);
					//NvFRead(&mmm,1,3,l_pFile);
					//NvFSeek(l_pFile,2,SEEK_CUR);
					//NvFRead(&mmm2,1,5,l_pFile);
				size_t l_uiMy_WAVFileHdr_StructSize = sizeof(My_WAVFileHdr_Struct);
				NvFRead(&m_WAVFileHdr_Struct,1,l_uiMy_WAVFileHdr_StructSize,l_pFile);
				m_WAVFileHdr_Struct.Size=((m_WAVFileHdr_Struct.Size+1)&~1)-4;
				size_t l_uiMy_WAVChunkHdr_StructSize = sizeof(My_WAVChunkHdr_Struct);
				while ((m_WAVFileHdr_Struct.Size!=0)&&(NvFRead(&l_WAVChunkHdr_Struct,1,l_uiMy_WAVChunkHdr_StructSize,l_pFile)))
				{
					if (!memcmp(l_WAVChunkHdr_Struct.Id,g_strFMTFormat,4))
					{
						//m_FMT_And_Data_Header[0] = l_WAVChunkHdr_Struct;
						size_t l_uiMy_WAVFmtHdr_StructSize = sizeof(My_WAVFmtHdr_Struct);
						NvFRead(&m_WAVFmtHdr_Struct,1,l_uiMy_WAVFmtHdr_StructSize,l_pFile);
						m_iBitPerSample = m_WAVFmtHdr_Struct.BitsPerSample;
						//1,PCM
						if ((m_WAVFmtHdr_Struct.Format==0x0001)||(m_WAVFmtHdr_Struct.Format==0xFFFE))
						{
							this->m_Format = cBasicSound::PerSampleBitAndChannelToOpenALFormat(m_WAVFmtHdr_Struct.BitsPerSample,m_WAVFmtHdr_Struct.Channels);
							//if (m_WAVFmtHdr_Struct.Channels==1)
							//	this->m_Format = (m_WAVFmtHdr_Struct.BitsPerSample==4?alGetEnumValue("AL_FORMAT_MONO_IMA4"):(m_WAVFmtHdr_Struct.BitsPerSample==8?AL_FORMAT_MONO8:AL_FORMAT_MONO16));
							//else if (m_WAVFmtHdr_Struct.Channels==2)
							//	this->m_Format = (m_WAVFmtHdr_Struct.BitsPerSample==4?alGetEnumValue("AL_FORMAT_STEREO_IMA4"):(m_WAVFmtHdr_Struct.BitsPerSample==8?AL_FORMAT_STEREO8:AL_FORMAT_STEREO16));
							this->m_iFreq = m_WAVFmtHdr_Struct.SampleRate;
							this->m_iChannel = m_WAVFmtHdr_Struct.Channels;
							//why need this?
							long l_uiStep = l_WAVChunkHdr_Struct.Size-sizeof(My_WAVFmtHdr_Struct);
							NvFSeek(l_pFile,l_uiStep,SEEK_CUR);
						}					
						else //17
						if (m_WAVFmtHdr_Struct.Format==0x0011)
						{
							if (m_WAVFmtHdr_Struct.Channels==1)
								this->m_Format = alGetEnumValue("AL_FORMAT_MONO_IMA4");
							else if (m_WAVFmtHdr_Struct.Channels==2)
								this->m_Format = alGetEnumValue("AL_FORMAT_STEREO_IMA4");
							this->m_iFreq = m_WAVFmtHdr_Struct.SampleRate;
							size_t l_uiStep = l_WAVChunkHdr_Struct.Size-sizeof(My_WAVFmtHdr_Struct);
							NvFSeek(l_pFile,l_WAVChunkHdr_Struct.Size-sizeof(My_WAVFmtHdr_Struct),SEEK_CUR);
						}
						else 
						if (m_WAVFmtHdr_Struct.Format==0x0055)
						{
							this->m_Format = alGetEnumValue("AL_FORMAT_MP3");
							this->m_iFreq = m_WAVFmtHdr_Struct.SampleRate;
							size_t l_uiStep = l_WAVChunkHdr_Struct.Size-sizeof(My_WAVFmtHdr_Struct);
							NvFSeek(l_pFile,l_WAVChunkHdr_Struct.Size-sizeof(My_WAVFmtHdr_Struct),SEEK_CUR);
						}
						else
						{
							//aLaw
							if (m_WAVFmtHdr_Struct.Format==0x0006)
							{
							}//* ima4 adpcm */
							else////aLaw
							if (m_WAVFmtHdr_Struct.Format==0x0007)
							{
							}else
							{
							
							}
							UT::ErrorMsgByFormat("not support wav format %d!?",m_WAVFmtHdr_Struct.Format);
							NvFRead(&m_WAVFmtExHdr_Struct,1,sizeof(My_WAVFmtExHdr_Struct),l_pFile);
							NvFSeek(l_pFile,l_WAVChunkHdr_Struct.Size-sizeof(My_WAVFmtHdr_Struct)-sizeof(My_WAVFmtExHdr_Struct),SEEK_CUR);
						}
					}
					else if (!memcmp(l_WAVChunkHdr_Struct.Id,g_strWAVData,4))
					{
						//m_FMT_And_Data_Header[1] = l_WAVChunkHdr_Struct;
						this->m_fTime = l_WAVChunkHdr_Struct.Size/(float)m_WAVFmtHdr_Struct.BytesRate;
						this->m_iSoundDataSize = l_WAVChunkHdr_Struct.Size;
						//whye here need more 31 byte? for empty sound!?
						int	l_iEndSounddForEmpty_DataLength = 0;
						m_pSoundData = new char[l_WAVChunkHdr_Struct.Size+l_iEndSounddForEmpty_DataLength];
						NvFRead(m_pSoundData,1,l_WAVChunkHdr_Struct.Size,l_pFile);
						//if( l_iEndSounddForEmpty_DataLength > 0 )
							//memset(m_pSoundData+l_WAVChunkHdr_Struct.Size,0,l_iEndSounddForEmpty_DataLength);
						//it should be done here
						//NvFSeek(l_pFile,ChunkHdr.Size&1,SEEK_CUR);
						//int	l_Size = (((ChunkHdr.Size+1)&~1)+8);
						//m_WAVFileHdr_Struct.Size-= l_Size;
						//if(m_WAVFileHdr_Struct.Size == 0)
						//	return true;
						//return false;
					}
					else if (!memcmp(l_WAVChunkHdr_Struct.Id,"smpl",4))
					{
						NvFRead(&m_WAVSmplHdr_Struct,1,sizeof(My_WAVSmplHdr_Struct),l_pFile);
						l_bLoop = (m_WAVSmplHdr_Struct.Loops ? AL_TRUE : AL_FALSE);
						size_t l_uiStep = l_WAVChunkHdr_Struct.Size-sizeof(My_WAVFmtHdr_Struct);
						NvFSeek(l_pFile,l_WAVChunkHdr_Struct.Size-sizeof(My_WAVSmplHdr_Struct),SEEK_CUR);
					}
					else 
					{
						NvFSeek(l_pFile,l_WAVChunkHdr_Struct.Size,SEEK_CUR);
					}
					//why need this one
					int	l_Size = l_WAVChunkHdr_Struct.Size&1;
					NvFSeek(l_pFile,l_Size,SEEK_CUR);
					l_Size = (((l_WAVChunkHdr_Struct.Size+1)&~1)+8);
					m_WAVFileHdr_Struct.Size-= l_Size;
				}
				NvFClose(l_pFile);
			}
		}
		//how many sample
		//one block align contain N channel
		if( m_iSoundDataSize == 0 )
		{
			UT::ErrorMsg("this is now wav file!",e_strFileName);
			return false;
		}
		m_iSampleCount = this->m_iSoundDataSize/m_WAVFmtHdr_Struct.BlockAlign;
		int	l_SoundBlock = m_WAVFmtHdr_Struct.BitsPerSample/8;
		if( m_WAVFmtHdr_Struct.BitsPerSample < 8 )
		{
			UT::ErrorMsg("wav file bit per sample can not smaller than 7",e_strFileName);
			return false;
		}
		return true;
		//below is separate channels,we dont need it right now.
		int l_iOneSampleLength = m_WAVFmtHdr_Struct.BlockAlign;
		assert(m_iSampleCount*m_WAVFmtHdr_Struct.BlockAlign == m_iSoundDataSize&&"this is not a common wav file");
		std::vector<char>*l_pChannelDataPointer = nullptr;
		int l_iSampleStep = l_iOneSampleLength/m_WAVFmtHdr_Struct.Channels;
		for(int i=0;i<this->m_WAVFmtHdr_Struct.Channels;++i)
		{
			int	l_iCurrentSampleIndex = i*m_WAVFmtHdr_Struct.BlockAlign;
			char*l_pCurrentChannelData = new char[m_iSampleCount*m_WAVFmtHdr_Struct.BlockAlign/this->m_WAVFmtHdr_Struct.Channels];
			//int
			int l_iHowManyBytesCopy = 0;
			for(int k=0;k<m_iSampleCount;++k)
			{
				int l_iSrcIndex = (l_iOneSampleLength*k)+l_iSampleStep*i;
				int l_iTargetIndex = l_iSampleStep*k;
				char*l_CurrentChannelData = m_pSoundData+l_iSrcIndex;
				char*l_pCopyData = l_pCurrentChannelData+l_iTargetIndex;
				memcpy(l_pCopyData,l_CurrentChannelData,l_iSampleStep);
				l_iHowManyBytesCopy += l_iSampleStep;
			}
			m_AllChannelData.push_back(l_pCurrentChannelData);
		}
		return true;	
	}

	bool	cSoundFile::OpenOggFile(const char*e_strFileName)
	{
		return true;
	}

	//ALvoid GetWAVData(const ALbyte *file,ALenum *format,ALvoid **data,ALsizei *size,ALsizei *freq, ALboolean *loop,float *e_fTotalPlayTime)
	bool	cSoundFile::OpenFile(const char*e_strFileName)
	{

		return OpenWavFile(e_strFileName);
		return false;
	}


	int	cSoundFile::GetSampleIndexByTime(float e_fTime)
	{
		if( m_fTime == 0.f )
			return  -1;
		float	l_fPercentage = e_fTime/m_fTime;
		int	l_iIndex = (int)(l_fPercentage*m_iSampleCount);
		return l_iIndex;
	}

	unsigned char*	cSoundFile::GetSample(int e_iSampleIndex)
	{
		return nullptr;
	}

	//int	cSoundFile::GetOneChannelBitPerSample()
	//{
	//	assert(m_iBitPerSample != 0&&"sorry now only support wav file");
	//	return m_iBitPerSample/this->m_iChannel;
	//}

	char*	cSoundFile::GetChannelData(int e_iChannelIndex)
	{
		if(e_iChannelIndex<(int)m_AllChannelData.size())
		{
			return m_AllChannelData[e_iChannelIndex];
		}
		return nullptr;
	}

	void	cSoundFile::SetWAVFmtHdr(
		unsigned short e_usFormat,unsigned short e_usChannels,
		unsigned int   e_uiSampleRate,unsigned int   e_uiBytesRate,
		unsigned short e_usBlockAlign,unsigned short e_usBitsPerSample)
	{
		m_WAVFmtHdr_Struct = GetWAVFmtHdr_Struct(e_usFormat,e_usChannels,e_uiSampleRate,e_uiBytesRate,e_usBlockAlign,e_usBitsPerSample);
	}
	//here assumy block align is 16bit 
	std::vector<char>	TwoChannelToOneChannel(vector<char>* e_pBuffer)
	{
		size_t l_uiSize = e_pBuffer->size();
		std::vector<char>	l_OutputVector;
		for( size_t i=2;i<l_uiSize;i+=4 )
		{
			l_OutputVector.push_back((*e_pBuffer)[i]);
			l_OutputVector.push_back((*e_pBuffer)[i+1]);
		}
		return l_OutputVector;
	}

	bool	cSoundFile::OggToWavFile(const char*e_strFileName,const char*e_strOutputFileName,int e_iOutChannel )
	{
		vector<char> buffer;
		ALenum l_eFormat;
		int l_iFreq;
		float l_fTime;
		FATMING_CORE::LoadOGG(e_strFileName,buffer,l_eFormat,l_iFreq,l_fTime);
		size_t l_uiSize = static_cast<ALsizei>(buffer.size());
		if( buffer.size() )
		{
			int l_iChannel = l_eFormat==AL_FORMAT_MONO16?1:2;
			int l_iSampleRate = (int)(l_uiSize/l_iChannel/l_fTime);
			const int l_iOggBitPerSample = 16;
			const int l_iBitToByteCount = 8;
			const int l_iBlockAlign = 8;
			unsigned int   l_uBytesRate = l_iSampleRate*l_iChannel*l_iOggBitPerSample/l_iBitToByteCount;//== SampleRate * NumChannels * BitsPerSample/8

			this->SetWAVFmtHdr(1,l_iChannel,l_iFreq,l_uBytesRate,l_iBlockAlign,l_iOggBitPerSample);
			StartWriteWavFile(e_strOutputFileName);
			WriteWavData(l_uiSize,(unsigned char*)&buffer[0]);
			//test code for 2 channel try write only one channel
			//this->SetWAVFmtHdr(1,1,l_iFreq,l_uBytesRate,l_iBlockAlign,l_iOggBitPerSample);
			//vector<char> l_OneChannel = TwoChannelToOneChannel(&buffer);
			//WriteWavData(l_uiSize/2,(unsigned char*)&l_OneChannel[0]);
			EndWriteWavFile();
			return true;
		}
		return false;
	}

	bool	cSoundFile::StartWriteWavFile(const char*e_strFileName)
	{
		m_uiDataSize = 0;
		SAFE_DELETE(m_pWriteFile);
		m_pWriteFile = new cBinaryFile();
		m_pWriteFile->Writefile(e_strFileName,true,false);
		//just give it a empty then write again after wrote data
		WriteHeadData();
		//skip header
		//
		return true;
	}

	bool	cSoundFile::WriteHeadData()
	{
		if( m_pWriteFile )
		{
			m_pWriteFile->SetFilePos(0);
			size_t l_TotalSize = m_uiDataSize;
			l_TotalSize += sizeof(My_WAVFileHdr_Struct);
			l_TotalSize += sizeof(My_WAVChunkHdr_Struct);
			l_TotalSize += sizeof(My_WAVFmtHdr_Struct);
			l_TotalSize += sizeof(My_WAVChunkHdr_Struct);

			My_WAVFileHdr_Struct l_My_WAVFileHdr_Struct = GetWAVFileHdr_Struct((int)m_uiDataSize);
			m_pWriteFile->WriteToFile((char*)&l_My_WAVFileHdr_Struct,sizeof(l_My_WAVFileHdr_Struct));
			//for My_WAVFmtHdr_Struct
			My_WAVChunkHdr_Struct l_My_WAVChunkHdr_Struct = GetWAVChunkHdr_Struct(sizeof(My_WAVFmtHdr_Struct),g_strFMTFormat);
			m_pWriteFile->WriteToFile((char*)&l_My_WAVChunkHdr_Struct,sizeof(l_My_WAVChunkHdr_Struct));
			My_WAVFmtHdr_Struct l_My_WAVFmtHdr_Struct = m_WAVFmtHdr_Struct;
			m_pWriteFile->WriteToFile((char*)&l_My_WAVFmtHdr_Struct,sizeof(My_WAVFmtHdr_Struct));
			//for data
			l_My_WAVChunkHdr_Struct = GetWAVChunkHdr_Struct((int)m_uiDataSize,g_strWAVData);
			m_pWriteFile->WriteToFile((char*)&l_My_WAVChunkHdr_Struct,sizeof(l_My_WAVChunkHdr_Struct));
			return true;
		}
		return false;
	}

	bool	cSoundFile::WriteWavData(size_t e_uiSize,unsigned char*e_pusData)
	{
		if( m_pWriteFile )
		{
			m_uiDataSize += e_uiSize;
			m_pWriteFile->WriteToFile((char*)e_pusData,e_uiSize);
			return true;
		}
		return false;
	}

	bool	cSoundFile::EndWriteWavFile()
	{
		if(m_pWriteFile)
		{
			WriteHeadData();
			SAFE_DELETE(m_pWriteFile);
			return true;
		}
		return false;
	}
//#define SRROUND_SOUND_TEST
#ifdef SRROUND_SOUND_TEST
//for circle sound data test
float g_fTest = 1.f;
float g_fTest2 = 1.f;
#endif

	bool	cSoundFile::WavToOggFile(const char*e_strFileName,const char*e_strOutputFileName,float e_fQuality,int e_iOutChannel)
	{
		if(m_iSoundDataSize == 0)
			this->OpenFile(e_strFileName);
		if(this->m_iSoundDataSize > 0 )
		{
			this->m_iWriteChannel = e_iOutChannel;
			StartWriteOggData(e_strOutputFileName,this->m_WAVFmtHdr_Struct.SampleRate,m_iWriteChannel,e_fQuality);
			//StartWriteOggData(e_strOutputFileName,this->m_WAVFmtHdr_Struct.SampleRate,2,e_fQuality);
			int l_iWriteSize = 1024*4;//m_iSoundDataSize;//1024*100;//306432
			if( l_iWriteSize >= m_iSoundDataSize)
				l_iWriteSize = m_iSoundDataSize;
			//from byte to float
			int l_iNumData = this->m_iSoundDataSize/l_iWriteSize;
			if(this->m_iSoundDataSize%l_iWriteSize)
			{
				l_iNumData += 1;
			}
			int l_iCurrentDataPos = 0;
			char*l_pData = (char*)this->m_pSoundData;
			for( int i=0;i<l_iNumData;++i )
			{
				int l_iDataLen = l_iWriteSize;
				if((i+1)*l_iWriteSize>m_iSoundDataSize)
				{
					l_iDataLen = m_iSoundDataSize-(i*l_iWriteSize);
				}
#ifdef SRROUND_SOUND_TEST
				g_fTest = sin((float)i/l_iNumData*20.f);
				g_fTest2 = cos((float)i/l_iNumData*20.f);
#endif
				char*l_pucCurrentData = (char*)this->m_pSoundData+l_iCurrentDataPos;
				//make noise here
				//float l_fRandValue = frand(0.1+l_iNumData,2+l_iNumData);
				//for( int j=0;j<l_iDataLen;++j )
				//{
				//	l_pucCurrentData[j] *= l_fRandValue;
				//}
				WriteOggData(l_iDataLen,l_pucCurrentData,m_WAVFmtHdr_Struct.Channels);
				l_iCurrentDataPos += l_iDataLen;
			}
			EndWriteOggData();
			return true;
		}
		return false;
	}

	bool	cSoundFile::WavToOggFileOnlyChannel(const char*e_strFileName,const char*e_strOutputFileName,float e_fQuality,int e_iOutChannel ,int e_iTargetChannelIndex)
	{
		assert( 0 &&"fuck dont use this one, because I am lazy o fix this!");
		if( e_iTargetChannelIndex >1 )
			e_iTargetChannelIndex = 1;
		if(OpenWavFile(e_strFileName))
		{
			this->m_iWriteChannel = 1;
			m_iSoundDataSize /= 2;
			StartWriteOggData(e_strOutputFileName,this->m_WAVFmtHdr_Struct.SampleRate,m_iWriteChannel,0.5);
			//StartWriteOggData(e_strOutputFileName,this->m_WAVFmtHdr_Struct.SampleRate,2,e_fQuality);
			int l_iWriteSize = 1024*4;//m_iSoundDataSize;//1024*100;//306432
			if( l_iWriteSize >= m_iSoundDataSize)
				l_iWriteSize = m_iSoundDataSize;
			//from byte to float
			int l_iNumData = this->m_iSoundDataSize/l_iWriteSize;
			if(this->m_iSoundDataSize%l_iWriteSize)
			{
				l_iNumData += 1;
			}
			char*l_pCurrentSoundData = m_AllChannelData[0];
			int l_iCurrentDataPos = 0;
			for( int i=0;i<l_iNumData;++i )
			{
				int l_iDataLen = l_iWriteSize;
				if((i+1)*l_iWriteSize>m_iSoundDataSize)
				{
					l_iDataLen = m_iSoundDataSize-(i*l_iWriteSize);
				}
#ifdef SRROUND_SOUND_TEST
				g_fTest = sin((float)i/l_iNumData*20.f);
				g_fTest2 = cos((float)i/l_iNumData*20.f);
#endif
				char*l_pucCurrentData = (char*)l_pCurrentSoundData+l_iCurrentDataPos;
				WriteOggData(l_iDataLen,l_pucCurrentData,1);
				l_iCurrentDataPos += l_iDataLen;
			}
			EndWriteOggData();
		}			
		return false;
	}

	bool	cSoundFile::StartWriteOggData(const char*e_strFileName,int e_iSampleRate,int e_iChannel,float e_fQuality)
	{
		m_iOggEOSValue = 0;
		m_iWriteChannel = e_iChannel;
		SAFE_DELETE(m_pWriteFile);
		m_pWriteFile = new cBinaryFile();
		if (!m_pWriteFile->Writefile(e_strFileName,true,false))
		{
			//printf("\n\nError : fopen failed : %s\n", strerror (errno)) ;
			UT::ErrorMsg("write file failed",e_strFileName);
			return false;
		}
		int ret;
		/********** Encode setup ************/

		vorbis_info_init (&vi);

		//	//l_ret = vorbis_encode_init_vbr(&l_vi,2,44100,0.1);
		int l_iChannel = e_iChannel;
		float l_fQuality = e_fQuality;
		int l_iSamplerate = e_iSampleRate;
		ret = vorbis_encode_init_vbr (&vi,l_iChannel,l_iSamplerate,l_fQuality);
		//ret = vorbis_encode_init_vbr (&vi,l_iChannel,l_iSamplerate,-1);
		if (ret)
		{
			std::wstring l_strInfo = L"vorbis_encode_init_vbr return ";
			l_strInfo += ValueToStringW(ret);
			cGameApp::OutputDebugInfoString(l_strInfo);
			return false;
		}

		vorbis_comment_init (&vc);
		vorbis_comment_add_tag (&vc,"ENCODER","SoundFile.cpp");
		/* set up the analysis state and auxiliary encoding storage */
		vorbis_analysis_init (&vd,&vi);
		vorbis_block_init (&vd,&vb);

		//ogg_stream_init (&os,rand());
		ogg_stream_init(&os,79979);
		{
			ogg_packet header;
			ogg_packet header_comm;
			ogg_packet header_code;

			vorbis_analysis_headerout (&vd,&vc,&header,&header_comm,&header_code);
			ogg_stream_packetin (&os,&header);

			ogg_stream_packetin (&os,&header_comm);
			ogg_stream_packetin (&os,&header_code);

			/* Ensures the audio data will start on a new page. */
			while (!m_iOggEOSValue)
			{
				int result = ogg_stream_flush (&os,&og);
				if (result == 0)
					break;
				NvFWrite (og.header,1,og.header_len,m_pWriteFile->GetFile());
				NvFWrite (og.body,1,og.body_len,m_pWriteFile->GetFile());
			}

		}
		return true;
	}
	bool	cSoundFile::WriteOggData(size_t e_uiSize,char*e_pusData,int e_iInChannel)
	{
		//byte to float
		int l_iDataCount = (int)e_uiSize;
		if( e_iInChannel == 2 )
			l_iDataCount /= 4;
		else
		if( e_iInChannel == 1 )
			l_iDataCount /= 2;
		/* expose the buffer to submit data */
		float **buffer = vorbis_analysis_buffer (&vd,l_iDataCount);
		int i;
		/* uninterleave samples */
		//for 2 channel
		if(this->m_iWriteChannel == 2)
		{
			if( e_iInChannel == 2 )
			{
				for(i=0;i<l_iDataCount;i++)
				{
					buffer[0][i] = ((e_pusData[i*4+1]<<8)|(0x00ff&(int)e_pusData[i*4]))/32768.f;
					buffer[1][i] = ((e_pusData[i*4+3]<<8)|(0x00ff&(int)e_pusData[i*4+2]))/32768.f;
#ifdef SRROUND_SOUND_TEST
					buffer[0][i] *= g_fTest;
					buffer[1][i] *= g_fTest2;
#endif
				}
			}
			else
			{
			if( e_iInChannel == 1 )
			{
				for(i=0;i<l_iDataCount;i++)
				{
					float l_fValue = ((e_pusData[i*2+1]<<8) | (0x00ff & (int)e_pusData[i*2])) / 32768.f;
					buffer[0][i] = l_fValue;
					buffer[1][i] = l_fValue;
#ifdef SRROUND_SOUND_TEST
					buffer[0][i] *= g_fTest;
					buffer[1][i] *= g_fTest2;
#endif
				}
			}			
			}
		}
		else
		if(this->m_iWriteChannel == 1)
		{
			if( e_iInChannel == 2 )
			{
				for(i = 0; i < l_iDataCount; i++)
				{
					buffer[0][i] = ((((e_pusData[i*4+1]<<8) | (0x00ff & (int)e_pusData[i*4])) / 32768.f) + (((e_pusData[i*4+3]<<8) | (0x00ff & (int)e_pusData[i*4+2])) / 32768.f)) * 0.5f;
#ifdef SRROUND_SOUND_TEST
					buffer[0][i] *= g_fTest;
#endif
				}
			}
			else
			if( e_iInChannel == 1 )
			{
				for(i = 0; i < l_iDataCount; i++)
				{
					buffer[0][i] = ((e_pusData[i*2+1]<<8) | (0x00ff & (int)e_pusData[i*2])) / 32768.f;
#ifdef SRROUND_SOUND_TEST
					buffer[0][i] *= g_fTest;
#endif
				}
			}
		}
		/* tell the library how much we actually submitted */
		vorbis_analysis_wrote (&vd,i);

		while (vorbis_analysis_blockout (&vd,&vb) == 1) 
		{
			vorbis_analysis (&vb,nullptr);
			vorbis_bitrate_addblock (&vb);

			while (vorbis_bitrate_flushpacket (&vd,&op))
			{
				ogg_stream_packetin (&os,&op);

				while (!m_iOggEOSValue)
				{
					int result = ogg_stream_pageout (&os,&og);
					if (result == 0)
						break;
					NvFWrite (og.header,1,og.header_len,m_pWriteFile->GetFile());
					NvFWrite (og.body,1,og.body_len,m_pWriteFile->GetFile());

					if (ogg_page_eos (&og))
						m_iOggEOSValue = 1;
				}
			}
		}
		return true;
	}

	bool	cSoundFile::EndWriteOggData()
	{
		vorbis_analysis_wrote (&vd,0);

		ogg_stream_clear (&os);
		vorbis_block_clear (&vb);
		vorbis_dsp_clear (&vd);
		vorbis_comment_clear (&vc);
		vorbis_info_clear (&vi);

		m_pWriteFile->CloseFile();
		SAFE_DELETE(m_pWriteFile);
		return true;
	}
//end namespace FATMING_CORE
}