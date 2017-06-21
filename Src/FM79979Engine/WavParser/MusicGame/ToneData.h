#pragma once

#define	TONE_DATA_SOUND_SOURCE_FILE_NAME	"SoundSourceFileName"
#define	TONE_DATA_ID						"ID"

#define	TONE_DATA_PICTURE_OFFSET_POS		"PosOffset"

#define	TONE_DATA_FILE_EXTENSION_NAME		"soundFFT"


//here are resources

struct sFindTimeDomainFrequenceAndAmplitude;

//filter FFT data and original sound file
//sFindTimeDomainFrequenceAndAmplitude from soundFFT file
class cToneData:public Frame,public cNodeISAX
{
	//
	bool*			m_pbBlackKey;
	std::string		m_strSoundFilePath;
	//int			m_iSoundID;
	sFindTimeDomainFrequenceAndAmplitude*m_pFrequenceAndAmplitudeAndTimeFinder;
public:
	DEFINE_TYPE_INFO();
	cToneData(TiXmlElement*e_pTiXmlElement);
	~cToneData();
	//const int GetSoundID();
	const sFindTimeDomainFrequenceAndAmplitude*	GetFrequenceAndAmplitudeAndTimeFinder();
	bool			IsBlackKey();
};

//<cToneDataVector>
//	<cToneData SoundSourceFileName="ooxx.wav" ID="0">
//</cToneDataVector>

class cToneDataVector:public cNamedTypedObjectVector<cToneData>,public cNodeISAX//,public Frame
{
	virtual bool MyParse(TiXmlElement*e_pRoot);
public:
	DEFINE_TYPE_INFO();
	cToneDataVector();
	~cToneDataVector();
	//const sFindTimeDomainFrequenceAndAmplitude*GetFrequenceAndAmplitudeAndTimeFinderBySoundID(int e_iSoundID);
	const sFindTimeDomainFrequenceAndAmplitude*GetFrequenceAndAmplitudeAndTimeFinder(const WCHAR*e_strName);
};