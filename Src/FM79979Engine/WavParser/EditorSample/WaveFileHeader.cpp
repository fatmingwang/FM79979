/************************************************************************************
	* Author:	Oso, Oluwafemi Ebenezer
	* Date:		5th of June, 2014
	* Filename:	WaveFileHeader.h

	A struct for the PCM wave header. Can be used to write the header of a wave file
**************************************************************************************/
#include "stdafx.h"
#include "WaveFileHeader.h"

WaveFileHeader::WaveFileHeader()
{
}

//Build wave header
WaveFileHeader::WaveFileHeader(unsigned long SamplingRate, unsigned short BitsPerSample, unsigned short Channels, unsigned long DataSize)
{
	ZeroMemory(this, sizeof(*this));

	// Set Riff-Chunk
	CopyMemory(Riff_ID, "RIFF", 4);
	Riff_Size = DataSize + 44;
	CopyMemory(Riff_Type, "WAVE", 4);

	// Set Fmt-Chunk
	CopyMemory(Fmt_ID, "fmt ", 4);
	Fmt_Length = 16;
	Fmt_Format = WAVE_FORMAT_PCM;
	Fmt_Channels = Channels;
	Fmt_SamplingRate = SamplingRate;
	Fmt_BlockAlign = Channels*BitsPerSample/8;
	Fmt_DataRate = Channels*BitsPerSample*SamplingRate/8;
	Fmt_BitsPerSample = BitsPerSample;

	// Set Data-Chunk
	CopyMemory(Data_ID, "data", 4);
	Data_DataSize = DataSize;
}

/*Use to check if the header is truly a RIFF wave header*/
BOOL WaveFileHeader::IsWAVE(FILE* f_in, WaveFileHeader* wfh)
{
	fread(wfh, 1, 44, f_in); 
	char riff_id[5];
	char riff_type[5];
	char fmt_id[5];
	char data_id[5];
	memcpy(riff_id, wfh->Riff_ID, 4);
	memcpy(riff_type, wfh->Riff_Type, 4);
	memcpy(fmt_id, wfh->Fmt_ID, 4);
	memcpy(data_id, wfh->Data_ID, 4);
	riff_id[4] = '\0';
	riff_type[4] = '\0';
	fmt_id[4] = '\0';
	data_id[4] = '\0';
	
	if(strcmp(riff_id, "RIFF") == 0 && strcmp(riff_type, "WAVE") == 0 && strcmp(fmt_id, "fmt ") == 0 && strcmp(data_id, "data") == 0)
		return TRUE;
	else
		return FALSE;
}