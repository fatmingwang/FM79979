#include "../stdafx.h"
#include "OpenAlWavFile.h"

#if defined _MSC_VER
	#pragma pack (push,1) 							/* Turn off alignment */
#elif defined __GNUC__
	#define PADOFF_VAR __attribute__((packed))
#endif

#ifndef PADOFF_VAR
	#define PADOFF_VAR
#endif
namespace FATMING_CORE
{
	typedef struct                                  /* WAV File-header */
	{
	  ALubyte  Id[4]			PADOFF_VAR;
	  ALsizei  Size				PADOFF_VAR;
	  ALubyte  Type[4]			PADOFF_VAR;
	} WAVFileHdr_Struct;

	typedef struct                                  /* WAV Fmt-header */
	{
	  ALushort Format			PADOFF_VAR;
	  ALushort Channels			PADOFF_VAR;
	  ALuint   SamplesPerSec	PADOFF_VAR;
	  ALuint   BytesPerSec		PADOFF_VAR;
	  ALushort BlockAlign		PADOFF_VAR;
	  ALushort BitsPerSample	PADOFF_VAR;
	} WAVFmtHdr_Struct;

	typedef struct									/* WAV FmtEx-header */
	{
	  ALushort Size				PADOFF_VAR;
	  ALushort SamplesPerBlock	PADOFF_VAR;
	} WAVFmtExHdr_Struct;

	typedef struct                                  /* WAV Smpl-header */
	{
	  ALuint   Manufacturer		PADOFF_VAR;
	  ALuint   Product			PADOFF_VAR;
	  ALuint   SamplePeriod		PADOFF_VAR;
	  ALuint   Note				PADOFF_VAR;
	  ALuint   FineTune			PADOFF_VAR;
	  ALuint   SMPTEFormat		PADOFF_VAR;
	  ALuint   SMPTEOffest		PADOFF_VAR;
	  ALuint   Loops			PADOFF_VAR;
	  ALuint   SamplerData		PADOFF_VAR;
	  struct
	  {
		ALuint Identifier		PADOFF_VAR;
		ALuint Type				PADOFF_VAR;
		ALuint Start			PADOFF_VAR;
		ALuint End				PADOFF_VAR;
		ALuint Fraction			PADOFF_VAR;
		ALuint Count			PADOFF_VAR;
	  }      Loop[1]			PADOFF_VAR;
	} WAVSmplHdr_Struct;

	typedef struct                                  /* WAV Chunk-header */
	{
	  ALubyte  Id[4]			PADOFF_VAR;
	  ALuint   Size				PADOFF_VAR;
	} WAVChunkHdr_Struct;


	#ifdef PADOFF_VAR			    				/* Default alignment */
		#undef PADOFF_VAR
	#endif

	#if defined _MSC_VER
		#pragma pack (pop)
	#endif

	ALvoid GetWAVData(const ALbyte *file,ALenum *format,ALvoid **data,ALsizei *size,ALsizei *freq, ALboolean *loop,float *e_fTotalPlayTime)
	{
		WAVChunkHdr_Struct ChunkHdr;
		WAVFmtExHdr_Struct FmtExHdr;
		WAVFileHdr_Struct FileHdr;
		WAVSmplHdr_Struct SmplHdr;
		WAVFmtHdr_Struct FmtHdr;
		NvFile *Stream;
		
		*format=AL_FORMAT_MONO16;
		*data=nullptr;
		*size=0;
		*freq=22050;
		*loop=AL_FALSE;
		if (file)
		{
			Stream = MyFileOpen((const char*)file, "rb" );
			if (Stream)
			{
				NvFRead(&FileHdr,1,sizeof(WAVFileHdr_Struct),Stream);
				FileHdr.Size=((FileHdr.Size+1)&~1)-4;
				while ((FileHdr.Size!=0)&&(NvFRead(&ChunkHdr,1,sizeof(WAVChunkHdr_Struct),Stream)))
				{
					if (!memcmp(ChunkHdr.Id,"fmt ",4))
					{
						NvFRead(&FmtHdr,1,sizeof(WAVFmtHdr_Struct),Stream);
						if ((FmtHdr.Format==0x0001)||(FmtHdr.Format==0xFFFE))
						{
							if (FmtHdr.Channels==1)
								*format=(FmtHdr.BitsPerSample==4?alGetEnumValue("AL_FORMAT_MONO_IMA4"):(FmtHdr.BitsPerSample==8?AL_FORMAT_MONO8:AL_FORMAT_MONO16));
							else if (FmtHdr.Channels==2)
								*format=(FmtHdr.BitsPerSample==4?alGetEnumValue("AL_FORMAT_STEREO_IMA4"):(FmtHdr.BitsPerSample==8?AL_FORMAT_STEREO8:AL_FORMAT_STEREO16));
							*freq=FmtHdr.SamplesPerSec;
							NvFSeek(Stream,ChunkHdr.Size-sizeof(WAVFmtHdr_Struct),SEEK_CUR);
						} 
						else if (FmtHdr.Format==0x0011)
						{
							if (FmtHdr.Channels==1)
								*format=alGetEnumValue("AL_FORMAT_MONO_IMA4");
							else if (FmtHdr.Channels==2)
								*format=alGetEnumValue("AL_FORMAT_STEREO_IMA4");
							*freq=FmtHdr.SamplesPerSec;
							NvFSeek(Stream,ChunkHdr.Size-sizeof(WAVFmtHdr_Struct),SEEK_CUR);
						}
						else if (FmtHdr.Format==0x0055)
						{
							*format=alGetEnumValue("AL_FORMAT_MP3");
							*freq=FmtHdr.SamplesPerSec;
							NvFSeek(Stream,ChunkHdr.Size-sizeof(WAVFmtHdr_Struct),SEEK_CUR);
						}
						else
						{
							NvFRead(&FmtExHdr,1,sizeof(WAVFmtExHdr_Struct),Stream);
							NvFSeek(Stream,ChunkHdr.Size-sizeof(WAVFmtHdr_Struct)-sizeof(WAVFmtExHdr_Struct),SEEK_CUR);
						}
					}
					else if (!memcmp(ChunkHdr.Id,"data",4))
					{
						if( e_fTotalPlayTime )//total play time
							*e_fTotalPlayTime = ChunkHdr.Size/(float)FmtHdr.BytesPerSec;
						*size=ChunkHdr.Size;
						*data=malloc(ChunkHdr.Size+31);
						if (*data) NvFRead(*data,FmtHdr.BlockAlign,ChunkHdr.Size/FmtHdr.BlockAlign,Stream);
						memset(((char *)*data)+ChunkHdr.Size,0,31);
						//e_fTotalPlayTime = ;
					}
					else if (!memcmp(ChunkHdr.Id,"smpl",4))
					{
						NvFRead(&SmplHdr,1,sizeof(WAVSmplHdr_Struct),Stream);
						*loop = (SmplHdr.Loops ? AL_TRUE : AL_FALSE);
						NvFSeek(Stream,ChunkHdr.Size-sizeof(WAVSmplHdr_Struct),SEEK_CUR);
					}
					else NvFSeek(Stream,ChunkHdr.Size,SEEK_CUR);
					NvFSeek(Stream,ChunkHdr.Size&1,SEEK_CUR);
					FileHdr.Size-=(((ChunkHdr.Size+1)&~1)+8);
				}
				NvFClose(Stream);
			}
			
		}
	}




	const wchar_t*     cOpanalWAV::TypeID( L"cOpanalWAV" );

	cOpanalWAV::cOpanalWAV(NamedTypedObject*e_pNamedTypedObject,const char*e_strileName,bool e_bStreaming):cBasicSound(e_pNamedTypedObject,e_bStreaming)
	{
		if( e_strileName )
			OpenFile(e_strileName);
	}

	cOpanalWAV::~cOpanalWAV()
	{

	}

	bool	cOpanalWAV::OpenFile(const char*e_strileName)
	{
		assert(m_bStreaming == false&&"cOpanalWAV::OpenFile(const char*e_strileName) only support static sound no streaming!...because fatming is lazy to do this.");
		check();
		ALvoid*l_pData = 0;
		ALboolean l_loop;
		SetName(UT::CharToWchar(UT::GetFileNameWithoutFullPath(e_strileName)).c_str());
		GetWAVData((ALbyte*)(e_strileName ),&m_iFormat,(ALvoid**)(&l_pData),&m_iSize,&m_iFreq,&l_loop,&m_fTimeLength);
		if( l_pData == 0 )
		{
			UT::ErrorMsg(e_strileName,"open file failed");
			SetName(L"");
			return false;
		}
		Destroy();
		//m_uiBuffer = alutCreateBufferFromFile (e_strileName);
		alGenBuffers(1, &m_uiBufferID);
		check();
		AssignBufferData(l_pData);
		char*l_pData2 = (char*)l_pData;
		SAFE_DELETE(l_pData2);
		l_pData = 0;
		check();
		return true;
	}
}