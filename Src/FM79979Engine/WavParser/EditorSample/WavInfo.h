#pragma once

//http://soundfile.sapp.org/doc/WaveFormat/

//https://my.oschina.net/liusicong/blog/323090

//first
/* WAV File-header */
//12 bytes
struct       My_WAVFileHdr_Struct
{
	unsigned char  Id[4]			;//RIFF
	int  Size				;//chunk size
	unsigned char  Type[4]			;//format WAVE,0x57415645
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
	unsigned short Size				;
	unsigned short SamplesPerBlock	;
} ;
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
} ;

class cWaveInfo
{
	std::vector<unsigned char*>	m_AllChannelData;
	void				AssignChannelData();
public:
	unsigned char *m_pSoundData;
	//
	//std::vector<char>	m_LeftChannelSoundData;
	//std::vector<char>	m_RigtChannelSoundData;
	ALenum				m_Format;
	int					m_iSoundDataSize;
	int					m_iFreq;
	bool				m_bLoop;
	float				m_fTime;//this->m_fTime = sound data length/(float)m_WAVFmtHdr_Struct.BytesRate;
	int					m_iSampleCount;//m_iSampleCount = this->m_iSoundDataSize/m_WAVFmtHdr_Struct.BlockAlign;
	//
	My_WAVFileHdr_Struct m_WAVFileHdr_Struct;
	My_WAVFmtHdr_Struct m_WAVFmtHdr_Struct;
	My_WAVFmtExHdr_Struct m_WAVFmtExHdr_Struct;
	My_WAVSmplHdr_Struct m_WAVSmplHdr_Struct;
	My_WAVChunkHdr_Struct m_WAVChunkHdr_Struct;
	My_WAVChunkHdr_Struct m_FMT_And_Data_Header[2];
public:
	cWaveInfo();
	~cWaveInfo();
	bool				OpenFile(const char*e_strFileName);
	int					GetSampleIndexByTime(float e_fTime);
	unsigned char*		GetSample(int e_iSampleIndex);
	unsigned char*		GetChannelData(int e_iChannelIndex);
	size_t				GetChannelCount(){ return m_AllChannelData.size(); }
private:

};
