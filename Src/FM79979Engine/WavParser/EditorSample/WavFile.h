#pragma once

//http://soundfile.sapp.org/doc/WaveFormat/

//https://my.oschina.net/liusicong/blog/323090

//http://mirlab.org/jang/books/audiosignalprocessing/audioIntro.asp?title=3-1%20Introduction%20to%20Audio%20Signals%20(%AD%B5%B0T%B0%F2%A5%BB%A4%B6%B2%D0)&language=chinese

//file  size = total data count*byte rate
//total data count = file size//byte rate
//file size = sample rate*byte rate*total seconds

//ex:byte rate is 16bit(2byte)
//total data count is 15716
//file size = 2*15716 = 31432 byte = 31.4k

//file size = 60seconds*16KHz(sample rate)*2 byte(16bit) = 1920KByte
//data count = 1920*1024/2 = 983040

//first
/* WAV File-header */
//12 bytes
struct       My_WAVFileHdr_Struct
{
	unsigned char  Id[4];			//RIFF
	int  Size;						//chunk size,the size of file beside above and this
	unsigned char  Type[4];			//format WAVE,0x57415645
};
/* WAV Fmt-header */
//third
//16 bytes
//   ByteRate         == SampleRate * NumChannels * BitsPerSample/8
//   BlockAlign       == NumChannels * BitsPerSample/8
//                               The number of bytes for one sample including
//                               all channels. I wonder what happens when
//                               this number isn't an integer?
struct My_WAVFmtHdr_Struct
{
	unsigned short Format			;//fmt ,0x666d7420
	unsigned short Channels			;//must big than 16
	unsigned int   SampleRate		;//m_iSampleCount/m_fTime
	unsigned int   BytesRate		;//== SampleRate * NumChannels * BitsPerSample/8
	unsigned short BlockAlign		;//== NumChannels * BitsPerSample/8,The number of bytes for one sample includingall channels. I wonder what happens when this number isn't an integer?
	unsigned short BitsPerSample	;//8,16...etc
};

//4 bytes
/* WAV FmtEx-header */
struct My_WAVFmtExHdr_Struct
{
	unsigned short Size;
	unsigned short SamplesPerBlock;
};

/* WAV Smpl-header */
struct My_WAVSmplHdr_Struct
{
	unsigned int   Manufacturer		;
	unsigned int   Product			;
	unsigned int   SamplePeriod		;
	unsigned int   Note				;
	unsigned int   FineTune			;
	unsigned int   SMPTEFormat		;
	unsigned int   SMPTEOffest		;
	unsigned int   Loops			;
	unsigned int   SamplerData		;
	struct
	{
		unsigned int Identifier		;
		unsigned int Type			;
		unsigned int Start			;
		unsigned int End			;
		unsigned int Fraction		;
		unsigned int Count			;
	}Loop[1]			;
} ;
//second
/* WAV Chunk-header */
//8 bytes
struct My_WAVChunkHdr_Struct
{
	unsigned char  Id[4]			;//fmt         ,data
	unsigned int   Size				;//16 for PCM  ,sound size
};

class cWaveFile
{
private:
	//for write
	cBinaryFile*					m_pWriteFile;
	size_t							m_uiDataSize;
	//
	std::vector<unsigned char*>		m_AllChannelData;
	void							AssignChannelData();
	//
	bool							WriteHeadData();
public:
	unsigned char*					m_pSoundData;
	//
	//std::vector<char>	m_LeftChannelSoundData;
	//std::vector<char>	m_RigtChannelSoundData;
	ALenum							m_Format;
	int								m_iSoundDataSize;
	int								m_iFreq;
	bool							m_bLoop;
	float							m_fTime;//this->m_fTime = sound data length/(float)m_WAVFmtHdr_Struct.BytesRate;
	int								m_iSampleCount;//m_iSampleCount = this->m_iSoundDataSize/m_WAVFmtHdr_Struct.BlockAlign;
	//
	My_WAVFileHdr_Struct			m_WAVFileHdr_Struct;
	My_WAVFmtHdr_Struct				m_WAVFmtHdr_Struct;
	My_WAVFmtExHdr_Struct			m_WAVFmtExHdr_Struct;
	My_WAVSmplHdr_Struct			m_WAVSmplHdr_Struct;
	My_WAVChunkHdr_Struct			m_WAVChunkHdr_Struct;
	My_WAVChunkHdr_Struct			m_FMT_And_Data_Header[2];
public:
	cWaveFile();
	~cWaveFile();
	//
	bool							OpenFile(const char*e_strFileName);
	int								GetSampleIndexByTime(float e_fTime);
	unsigned char*					GetSample(int e_iSampleIndex);
	unsigned char*					GetChannelData(int e_iChannelIndex);
	size_t							GetChannelCount(){ return m_AllChannelData.size(); }
	void							SetWAVFmtHdr(unsigned short e_usFormat,unsigned short e_usChannels,
										unsigned int   e_uiSampleRate,unsigned int   e_uiBytesRate,
										unsigned short e_usBlockAlign,unsigned short e_usBitsPerSample);
	//
	bool							StartWriteFile(const char*es_trFileName);
	bool							WriteData(size_t e_uiSize,unsigned char*e_pusData);
	bool							EndWriteFile();
};