#include "stdafx.h"
#include "WavFile.h"

#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
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

My_WAVFmtExHdr_Struct GetWAVFmtExHdr_Struct(unsigned short Size,unsigned short SamplesPerBlock)
{
	My_WAVFmtExHdr_Struct l_My_WAVFmtExHdr_Struct;
	return l_My_WAVFmtExHdr_Struct;
}

My_WAVChunkHdr_Struct GetWAVChunkHdr_Struct(unsigned int   Size,const char*e_ChunkID)
{
	My_WAVChunkHdr_Struct l_My_WAVChunkHdr_Struct;
	l_My_WAVChunkHdr_Struct.Size = Size;
	memcpy(&l_My_WAVChunkHdr_Struct.Id,e_ChunkID,strlen(e_ChunkID)*sizeof(char));
	return l_My_WAVChunkHdr_Struct;
}

cWaveFile::cWaveFile()
{
	m_pWriteFile = nullptr;
	m_pSoundData = nullptr;
	m_Format = 0;
	m_iSoundDataSize = 0;
	m_iFreq = 0;
	m_bLoop = false;
	m_fTime = 0.f;
	m_iSampleCount = 0;
	m_uiDataSize = 0;
}

cWaveFile::~cWaveFile()
{
	DELETE_VECTOR(m_AllChannelData,unsigned char*);
	SAFE_DELETE(m_pSoundData);
	SAFE_DELETE(m_pWriteFile);
}



void	cWaveFile::AssignChannelData()
{

}


//ALvoid GetWAVData(const ALbyte *file,ALenum *format,ALvoid **data,ALsizei *size,ALsizei *freq, ALboolean *loop,float *e_fTotalPlayTime)
bool	cWaveFile::OpenFile(const char*e_strFileName)
{
	m_iSampleCount = 0;
	memset(&m_WAVFileHdr_Struct,0,sizeof(My_WAVFileHdr_Struct ));
	memset(&m_WAVFmtHdr_Struct,0,sizeof(My_WAVFmtHdr_Struct));
	memset(&m_WAVFmtExHdr_Struct,0,sizeof(My_WAVFmtExHdr_Struct ));
	memset(&m_WAVSmplHdr_Struct,0,sizeof(My_WAVSmplHdr_Struct ));
	memset(&m_WAVChunkHdr_Struct,0,sizeof(My_WAVChunkHdr_Struct ));
	NvFile *l_pFile = nullptr;
	SAFE_DELETE(m_pSoundData);

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
			while ((m_WAVFileHdr_Struct.Size!=0)&&(NvFRead(&m_WAVChunkHdr_Struct,1,l_uiMy_WAVChunkHdr_StructSize,l_pFile)))
			{
				if (!memcmp(m_WAVChunkHdr_Struct.Id,g_strFMTFormat,4))
				{
					m_FMT_And_Data_Header[0] = m_WAVChunkHdr_Struct;
					size_t l_uiMy_WAVFmtHdr_StructSize = sizeof(My_WAVFmtHdr_Struct);
					NvFRead(&m_WAVFmtHdr_Struct,1,l_uiMy_WAVFmtHdr_StructSize,l_pFile);
					//1,PCM
					if ((m_WAVFmtHdr_Struct.Format==0x0001)||(m_WAVFmtHdr_Struct.Format==0xFFFE))
					{
						if (m_WAVFmtHdr_Struct.Channels==1)
							this->m_Format = (m_WAVFmtHdr_Struct.BitsPerSample==4?alGetEnumValue("AL_FORMAT_MONO_IMA4"):(m_WAVFmtHdr_Struct.BitsPerSample==8?AL_FORMAT_MONO8:AL_FORMAT_MONO16));
						else if (m_WAVFmtHdr_Struct.Channels==2)
							this->m_Format = (m_WAVFmtHdr_Struct.BitsPerSample==4?alGetEnumValue("AL_FORMAT_STEREO_IMA4"):(m_WAVFmtHdr_Struct.BitsPerSample==8?AL_FORMAT_STEREO8:AL_FORMAT_STEREO16));
						this->m_iFreq = m_WAVFmtHdr_Struct.SampleRate;
						//why need this?
						size_t l_uiStep = m_WAVChunkHdr_Struct.Size-sizeof(My_WAVFmtHdr_Struct);
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
						size_t l_uiStep = m_WAVChunkHdr_Struct.Size-sizeof(My_WAVFmtHdr_Struct);
						NvFSeek(l_pFile,m_WAVChunkHdr_Struct.Size-sizeof(My_WAVFmtHdr_Struct),SEEK_CUR);
					}
					else 
					if (m_WAVFmtHdr_Struct.Format==0x0055)
					{
						this->m_Format = alGetEnumValue("AL_FORMAT_MP3");
						this->m_iFreq = m_WAVFmtHdr_Struct.SampleRate;
						size_t l_uiStep = m_WAVChunkHdr_Struct.Size-sizeof(My_WAVFmtHdr_Struct);
						NvFSeek(l_pFile,m_WAVChunkHdr_Struct.Size-sizeof(My_WAVFmtHdr_Struct),SEEK_CUR);
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
						NvFSeek(l_pFile,m_WAVChunkHdr_Struct.Size-sizeof(My_WAVFmtHdr_Struct)-sizeof(My_WAVFmtExHdr_Struct),SEEK_CUR);
					}
				}
				else if (!memcmp(m_WAVChunkHdr_Struct.Id,g_strWAVData,4))
				{
					m_FMT_And_Data_Header[1] = m_WAVChunkHdr_Struct;
					this->m_fTime = m_WAVChunkHdr_Struct.Size/(float)m_WAVFmtHdr_Struct.BytesRate;
					this->m_iSoundDataSize = m_WAVChunkHdr_Struct.Size;
					//whye here need more 31 byte? for empty sound!?
					int	l_iEndSounddForEmpty_DataLength = 31;
					m_pSoundData = (unsigned char*)malloc(m_WAVChunkHdr_Struct.Size+l_iEndSounddForEmpty_DataLength);
					NvFRead(m_pSoundData,m_WAVFmtHdr_Struct.BlockAlign,m_WAVChunkHdr_Struct.Size/m_WAVFmtHdr_Struct.BlockAlign,l_pFile);
					if( l_iEndSounddForEmpty_DataLength > 0 )
						memset(m_pSoundData+m_WAVChunkHdr_Struct.Size,0,l_iEndSounddForEmpty_DataLength);
					//it should be done here
					//NvFSeek(l_pFile,ChunkHdr.Size&1,SEEK_CUR);
					//int	l_Size = (((ChunkHdr.Size+1)&~1)+8);
					//m_WAVFileHdr_Struct.Size-= l_Size;
					//if(m_WAVFileHdr_Struct.Size == 0)
					//	return true;
					//return false;
				}
				else if (!memcmp(m_WAVChunkHdr_Struct.Id,"smpl",4))
				{
					NvFRead(&m_WAVSmplHdr_Struct,1,sizeof(My_WAVSmplHdr_Struct),l_pFile);
					l_bLoop = (m_WAVSmplHdr_Struct.Loops ? AL_TRUE : AL_FALSE);
					size_t l_uiStep = m_WAVChunkHdr_Struct.Size-sizeof(My_WAVFmtHdr_Struct);
					NvFSeek(l_pFile,m_WAVChunkHdr_Struct.Size-sizeof(My_WAVSmplHdr_Struct),SEEK_CUR);
				}
				else 
				{
					NvFSeek(l_pFile,m_WAVChunkHdr_Struct.Size,SEEK_CUR);
				}
				//why need this one
				int	l_Size = m_WAVChunkHdr_Struct.Size&1;
				NvFSeek(l_pFile,l_Size,SEEK_CUR);
				l_Size = (((m_WAVChunkHdr_Struct.Size+1)&~1)+8);
				m_WAVFileHdr_Struct.Size-= l_Size;
			}
			NvFClose(l_pFile);
		}
	}
	{
		//write test
		cWaveFile l_cWaveFile;
		l_cWaveFile.SetWAVFmtHdr(m_WAVFmtHdr_Struct.Format,m_WAVFmtHdr_Struct.Channels,m_WAVFmtHdr_Struct.SampleRate,m_WAVFmtHdr_Struct.BytesRate,
			m_WAVFmtHdr_Struct.BlockAlign,m_WAVFmtHdr_Struct.BitsPerSample);
		l_cWaveFile.StartWriteFile("79979.wav");
		l_cWaveFile.WriteData(m_WAVChunkHdr_Struct.Size,m_pSoundData);
		l_cWaveFile.EndWriteFile();
	
	}
	//how many sample
	//one block align contain N channel
	m_iSampleCount = this->m_iSoundDataSize/m_WAVFmtHdr_Struct.BlockAlign;
	int	l_SoundBlock = m_WAVFmtHdr_Struct.BitsPerSample/8;
	if( m_WAVFmtHdr_Struct.BitsPerSample < 8 )
	{
		UT::ErrorMsg("wav file bit per sample can not smaller than 7",e_strFileName);
		return false;
	}
	
	int l_iSampleLength = m_WAVFmtHdr_Struct.BlockAlign;
	assert(m_iSampleCount*m_WAVFmtHdr_Struct.BlockAlign == m_WAVChunkHdr_Struct.Size);
	std::vector<char>*l_pChannelDataPointer = nullptr;
	for(int i=0;i<this->m_WAVFmtHdr_Struct.Channels;++i)
	{
		int	l_iCurrentSampleIndex = i*m_WAVFmtHdr_Struct.BlockAlign;
		unsigned char*l_pCurrentChannelData = new unsigned char[m_iSampleCount*m_WAVFmtHdr_Struct.BlockAlign/this->m_WAVFmtHdr_Struct.Channels];
		int l_iCurrentChannelStep = m_WAVFmtHdr_Struct.BlockAlign/this->m_WAVFmtHdr_Struct.Channels;
		//int
		for(int k=0;k<m_iSampleCount;++k)
		{
			int l_iSrcIndex = (l_iSampleLength*k)+l_iCurrentChannelStep*i;
			int l_iTargetIndex = l_iCurrentChannelStep*k;
			unsigned char*l_CurrentChannelData = m_pSoundData+l_iSrcIndex;
			unsigned char*l_pCopyData = l_pCurrentChannelData+l_iTargetIndex;
			memcpy(l_pCopyData,l_CurrentChannelData,l_iCurrentChannelStep);
		}
		m_AllChannelData.push_back(l_pCurrentChannelData);
	}
	return true;
}


int	cWaveFile::GetSampleIndexByTime(float e_fTime)
{
	if( m_fTime == 0.f )
		return  -1;
	float	l_fPercentage = e_fTime/m_fTime;
	int	l_iIndex = (int)(l_fPercentage*m_iSampleCount);
	return l_iIndex;
}

unsigned char*	cWaveFile::GetSample(int e_iSampleIndex)
{
	return nullptr;
}

unsigned char*	cWaveFile::GetChannelData(int e_iChannelIndex)
{
	if(e_iChannelIndex<(int)m_AllChannelData.size())
	{
		return m_AllChannelData[e_iChannelIndex];
	}
	return nullptr;
}

void	cWaveFile::SetWAVFmtHdr(
	unsigned short e_usFormat,unsigned short e_usChannels,
	unsigned int   e_uiSampleRate,unsigned int   e_uiBytesRate,
	unsigned short e_usBlockAlign,unsigned short e_usBitsPerSample)
{
	m_WAVFmtHdr_Struct = GetWAVFmtHdr_Struct(e_usFormat,e_usChannels,e_uiSampleRate,e_uiBytesRate,e_usBlockAlign,e_usBitsPerSample);
}

bool	cWaveFile::StartWriteFile(const char*es_trFileName)
{
	m_uiDataSize = 0;
	SAFE_DELETE(m_pWriteFile);
	m_pWriteFile = new cBinaryFile();
	m_pWriteFile->Writefile(es_trFileName,true,false);
	//just give it a empty then write again after wrote data
	WriteHeadData();
	//skip header
	//
	return true;
}

bool	cWaveFile::WriteHeadData()
{
	if( m_pWriteFile )
	{
		m_pWriteFile->SetFilePos(0);
		int l_TotalSize = m_uiDataSize;
		l_TotalSize += sizeof(My_WAVFileHdr_Struct);
		l_TotalSize += sizeof(My_WAVChunkHdr_Struct);
		l_TotalSize += sizeof(My_WAVFmtHdr_Struct);
		l_TotalSize += sizeof(My_WAVChunkHdr_Struct);

		My_WAVFileHdr_Struct l_My_WAVFileHdr_Struct = GetWAVFileHdr_Struct(m_uiDataSize);
		m_pWriteFile->WriteToFile((char*)&l_My_WAVFileHdr_Struct,sizeof(l_My_WAVFileHdr_Struct));
		//for My_WAVFmtHdr_Struct
		My_WAVChunkHdr_Struct l_My_WAVChunkHdr_Struct = GetWAVChunkHdr_Struct(sizeof(My_WAVFmtHdr_Struct),g_strFMTFormat);
		m_pWriteFile->WriteToFile((char*)&l_My_WAVChunkHdr_Struct,sizeof(l_My_WAVChunkHdr_Struct));
		My_WAVFmtHdr_Struct l_My_WAVFmtHdr_Struct = m_WAVFmtHdr_Struct;
		m_pWriteFile->WriteToFile((char*)&l_My_WAVFmtHdr_Struct,sizeof(My_WAVFmtHdr_Struct));
		//for data
		l_My_WAVChunkHdr_Struct = GetWAVChunkHdr_Struct(m_uiDataSize,g_strWAVData);
		m_pWriteFile->WriteToFile((char*)&l_My_WAVChunkHdr_Struct,sizeof(l_My_WAVChunkHdr_Struct));
		return true;
	}
	return false;
}

bool	cWaveFile::WriteData(size_t e_uiSize,unsigned char*e_pusData)
{
	if( m_pWriteFile )
	{
		m_uiDataSize += e_uiSize;
		m_pWriteFile->WriteToFile((char*)e_pusData,e_uiSize);
		return true;
	}
	return false;
}

bool	cWaveFile::EndWriteFile()
{
	if(m_pWriteFile)
	{
		WriteHeadData();
		SAFE_DELETE(m_pWriteFile);
		return true;
	}
	return false;
}