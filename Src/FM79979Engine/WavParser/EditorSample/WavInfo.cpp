#include "stdafx.h"
#include "WavInfo.h"


cWaveInfo::cWaveInfo()
{
	m_pSoundData = nullptr;
	m_Format = 0;
	m_iSoundDataSize = 0;
	m_iFreq = 0;
	m_bLoop = false;
	m_fTime = 0.f;
	m_iSampleCount = 0;
}

cWaveInfo::~cWaveInfo()
{
	DELETE_VECTOR(m_AllChannelData,unsigned char*);
	SAFE_DELETE(m_pSoundData);
}



void	cWaveInfo::AssignChannelData()
{

}


//ALvoid GetWAVData(const ALbyte *file,ALenum *format,ALvoid **data,ALsizei *size,ALsizei *freq, ALboolean *loop,float *e_fTotalPlayTime)
bool	cWaveInfo::OpenFile(const char*e_strFileName)
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
			NvFRead(&m_WAVFileHdr_Struct,1,sizeof(My_WAVFileHdr_Struct),l_pFile);
			m_WAVFileHdr_Struct.Size=((m_WAVFileHdr_Struct.Size+1)&~1)-4;
			while ((m_WAVFileHdr_Struct.Size!=0)&&(NvFRead(&m_WAVChunkHdr_Struct,1,sizeof(My_WAVChunkHdr_Struct),l_pFile)))
			{
				if (!memcmp(m_WAVChunkHdr_Struct.Id,"fmt ",4))
				{
					m_FMT_And_Data_Header[0] = m_WAVChunkHdr_Struct;
					NvFRead(&m_WAVFmtHdr_Struct,1,sizeof(My_WAVFmtHdr_Struct),l_pFile);
					//1,PCM
					if ((m_WAVFmtHdr_Struct.Format==0x0001)||(m_WAVFmtHdr_Struct.Format==0xFFFE))
					{
						if (m_WAVFmtHdr_Struct.Channels==1)
							this->m_Format = (m_WAVFmtHdr_Struct.BitsPerSample==4?alGetEnumValue("AL_FORMAT_MONO_IMA4"):(m_WAVFmtHdr_Struct.BitsPerSample==8?AL_FORMAT_MONO8:AL_FORMAT_MONO16));
						else if (m_WAVFmtHdr_Struct.Channels==2)
							this->m_Format = (m_WAVFmtHdr_Struct.BitsPerSample==4?alGetEnumValue("AL_FORMAT_STEREO_IMA4"):(m_WAVFmtHdr_Struct.BitsPerSample==8?AL_FORMAT_STEREO8:AL_FORMAT_STEREO16));
						this->m_iFreq = m_WAVFmtHdr_Struct.SampleRate;
						NvFSeek(l_pFile,m_WAVChunkHdr_Struct.Size-sizeof(My_WAVFmtHdr_Struct),SEEK_CUR);
					}					
					else //17
					if (m_WAVFmtHdr_Struct.Format==0x0011)
					{
						if (m_WAVFmtHdr_Struct.Channels==1)
							this->m_Format = alGetEnumValue("AL_FORMAT_MONO_IMA4");
						else if (m_WAVFmtHdr_Struct.Channels==2)
							this->m_Format = alGetEnumValue("AL_FORMAT_STEREO_IMA4");
						this->m_iFreq = m_WAVFmtHdr_Struct.SampleRate;
						NvFSeek(l_pFile,m_WAVChunkHdr_Struct.Size-sizeof(My_WAVFmtHdr_Struct),SEEK_CUR);
					}
					else 
					if (m_WAVFmtHdr_Struct.Format==0x0055)
					{
						this->m_Format = alGetEnumValue("AL_FORMAT_MP3");
						this->m_iFreq = m_WAVFmtHdr_Struct.SampleRate;
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
				else if (!memcmp(m_WAVChunkHdr_Struct.Id,"data",4))
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
					NvFSeek(l_pFile,m_WAVChunkHdr_Struct.Size-sizeof(My_WAVSmplHdr_Struct),SEEK_CUR);
				}
				else 
				{
					NvFSeek(l_pFile,m_WAVChunkHdr_Struct.Size,SEEK_CUR);
				}
				NvFSeek(l_pFile,m_WAVChunkHdr_Struct.Size&1,SEEK_CUR);
				int	l_Size = (((m_WAVChunkHdr_Struct.Size+1)&~1)+8);
				m_WAVFileHdr_Struct.Size-= l_Size;
			}
			NvFClose(l_pFile);
		}
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


int	cWaveInfo::GetSampleIndexByTime(float e_fTime)
{
	if( m_fTime == 0.f )
		return  -1;
	float	l_fPercentage = e_fTime/m_fTime;
	int	l_iIndex = (int)(l_fPercentage*m_iSampleCount);
	return l_iIndex;
}

unsigned char*	cWaveInfo::GetSample(int e_iSampleIndex)
{
	return nullptr;
}

unsigned char*	cWaveInfo::GetChannelData(int e_iChannelIndex)
{
	if(e_iChannelIndex<(int)m_AllChannelData.size())
	{
		return m_AllChannelData[e_iChannelIndex];
	}
	return nullptr;
}