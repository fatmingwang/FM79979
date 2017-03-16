#pragma once

//http://soundfile.sapp.org/doc/WaveFormat/

//https://my.oschina.net/liusicong/blog/323090

//http://mirlab.org/jang/books/audiosignalprocessing/audioIntro.asp?title=3-1%20Introduction%20to%20Audio%20Signals%20(%AD%B5%B0T%B0%F2%A5%BB%A4%B6%B2%D0)&language=chinese
#ifdef IOS
#include "ogg/ogg/ogg.h"
#include "ogg/vorbis/vorbisfile.h"
#elif defined(ANDROID)
#include "../../ogg/ogg/ogg.h"
#include "../../ogg/vorbis/vorbisfile.h"
#elif defined(WIN32)
#include "../../../Include/ogg/ogg.h"
#include "../../../Include/vorbis/vorbisfile.h"
#include "../../../Include/vorbis/vorbisenc.h"
#else//linux
//#include "ogg.h"
//#include "vorbisfile.h"
#endif

#include "BasicSound.h"
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
	unsigned short Channels			;//1 or 2,
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
namespace FATMING_CORE
{
	class cBinaryFile;
	class cSoundFile
	{
		//for ogg
		ogg_stream_state os;
		ogg_page         og;
		ogg_packet       op;
		vorbis_info      vi;
		vorbis_comment   vc;
		vorbis_dsp_state vd;
		vorbis_block     vb;
	private:
		//for write
		cBinaryFile*					m_pWriteFile;
		size_t							m_uiDataSize;
		int								m_iWriteChannel;
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
		int								m_iFreq;//OneSceondSampleCount,sampleRate / sampleCount
		int								m_iChannel;
		float							m_fTime;//this->m_fTime = sound data length/(float)m_WAVFmtHdr_Struct.BytesRate;
		int								m_iSampleCount;//m_iSampleCount = this->m_iSoundDataSize/m_WAVFmtHdr_Struct.BlockAlign;
		int								m_iBitPerSample;//if channel is 2,each value should divide 2
		//
		My_WAVFileHdr_Struct			m_WAVFileHdr_Struct;
		My_WAVFmtHdr_Struct				m_WAVFmtHdr_Struct;
		My_WAVFmtExHdr_Struct			m_WAVFmtExHdr_Struct;
		My_WAVSmplHdr_Struct			m_WAVSmplHdr_Struct;
		//My_WAVChunkHdr_Struct			m_FMT_And_Data_Header[2];
		bool							OpenWavFile(const char*e_strFileName);
		bool							OpenOggFile(const char*e_strFileName);
	public:
		cSoundFile();
		~cSoundFile();
		//
		bool							OpenFile(const char*e_strFileName);
		int								GetSampleIndexByTime(float e_fTime);
		unsigned char*					GetSample(int e_iSampleIndex);
		unsigned char*					GetChannelData(int e_iChannelIndex);
		//int								GetOneChannelBitPerSample();
		size_t							GetChannelCount(){ return m_AllChannelData.size(); }
		void							SetWAVFmtHdr(unsigned short e_usFormat,unsigned short e_usChannels,
											unsigned int   e_uiSampleRate,unsigned int   e_uiBytesRate,
											unsigned short e_usBlockAlign,unsigned short e_usBitsPerSample);
		//
		bool							StartWriteWavFile(const char*e_strFileName);
		bool							WriteWavData(size_t e_uiSize,unsigned char*e_pusData);
		bool							EndWriteWavFile();
		//
		bool							StartWriteOggData(const char*e_strFileName,int e_iSampleRate,int e_iChannel = 2,float e_fQuality = 0.4f);
		//fuck ogg format is not allow unsigned,it spend my whole night!
		bool							WriteOggData(size_t e_uiSize,char*e_pusData,int e_iInChannel);
		bool							EndWriteOggData();
		//
		bool							WavToOggFile(const char*e_strFileName,const char*e_strOutputFileName,float e_fQuality = 0.4f,int e_iOutChannel = 2);
		bool							OggToWavFile(const char*e_strFileName,const char*e_strOutputFileName,int e_iOutChannel = 2);
	};
//end namespace FATMING_CORE
}