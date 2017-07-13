#pragma once

#define	TONE_DATA_SOUND_SOURCE_FILE_NAME	"SoundSourceFileName"
#define	TONE_DATA_ID						"ID"

#define	TONE_DATA_PICTURE_OFFSET_POS		"PosOffset"

#define	TONE_DATA_FILE_EXTENSION_NAME		"soundFFT"


//here are resources

struct sFindTimeDomainFrequenceAndAmplitude;
class cNoteFrequencyAndDecibles;
class cQuickFFTDataFrequencyFinder;
//filter FFT data and original sound file
//sFindTimeDomainFrequenceAndAmplitude from soundFFT file
class cToneData:public Frame,public cNodeISAX
{
	bool			m_bStartHittedCount;
	int				m_iMatchTime;
	int				m_iMatchTimeCondition;
	//
	bool*			m_pbBlackKey;
	std::string		m_strSoundFilePath;
	//int			m_iSoundID;
	sFindTimeDomainFrequenceAndAmplitude*	m_pFrequenceAndAmplitudeAndTimeFinder;
	cNoteFrequencyAndDecibles*				m_pNoteFrequencyAndDecibles;
public:
	DEFINE_TYPE_INFO();
	cToneData(TiXmlElement*e_pTiXmlElement);
	~cToneData();
	//const int GetSoundID();
	const sFindTimeDomainFrequenceAndAmplitude*	GetFrequenceAndAmplitudeAndTimeFinder();
	const cNoteFrequencyAndDecibles*			GetNoteFrequencyAndDecibles();
	bool										IsBlackKey();

	float										CompareFFTDecibles(cQuickFFTDataFrequencyFinder*e_pQuickFFTDataFrequencyFinder);
};

//<cToneDataVector>
//	<cToneData SoundSourceFileName="ooxx.wav" ID="0">
//</cToneDataVector>

class cToneDataVector:public cNamedTypedObjectVector<cToneData>,public cNodeISAX//,public Frame
{
	virtual bool MyParse(TiXmlElement*e_pRoot);
	std::vector<std::wstring>	m_MatchName;
	std::vector<float>			m_ResultVector;
	//UT::sTimeCounter	m_TempForMatchName;
public:
	DEFINE_TYPE_INFO();
	cToneDataVector();
	~cToneDataVector();
	//const sFindTimeDomainFrequenceAndAmplitude*GetFrequenceAndAmplitudeAndTimeFinderBySoundID(int e_iSoundID);
	const sFindTimeDomainFrequenceAndAmplitude*GetFrequenceAndAmplitudeAndTimeFinder(const WCHAR*e_strName);
	//for test
	void	Update(float e_fElpaseTime,cQuickFFTDataFrequencyFinder*e_pQuickFFTDataFrequencyFinder);
	void	Render();
};