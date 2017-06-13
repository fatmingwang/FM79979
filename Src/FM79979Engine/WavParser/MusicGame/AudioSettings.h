/*********************************************************************************************************
	* Author:	Oso, Oluwafemi Ebenezer
	* Date:		5th of June, 2014
	* Filename:	AudioSettings.h

	A generic audio settings common to all audio file format (samplerate, bitrate, number of channel, etc)
***********************************************************************************************************/
//Frequency
enum SampleRate
{
	SR_8khz = 8000,
	SR_11khz = 11025,
	SR_12khz = 12000,
	SR_16khz = 16000,
	SR_22khz = 22050,
	SR_24khz = 24000,
	SR_32khz = 32000,
	SR_44khz = 44100,
	SR_48khz = 48000
};

enum Channel
{
	Mono = 1,
	Stereo = 2
};

enum Bitrate
{
	BR_UNSET = -1,
	BR_8kbps = 8000,
	BR_16kbps = 16000,
	BR_24kbps = 24000,
	BR_32kbps = 32000,
	BR_40kbps = 40000,
	BR_48kbps = 48000,
	BR_56kbps = 56000,
	BR_64kbps = 64000,
	BR_80kbps = 80000,
	BR_96kbps = 96000,
	BR_112kbps = 112000,
	BR_128kbps = 128000,
	BR_144kbps = 144000,
	BR_160kbps = 160000,
	BR_192kbps = 192000,
	BR_224kbps = 224000,
	BR_256kbps = 256000,
	BR_320kbps = 320000
};

enum Encode_Mode
{
	VBR,
	ABR,
	CBR
};