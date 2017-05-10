#pragma once

//here are resources

struct sFindTimeDomainFrequenceAndAmplitude;

//filter FFT data and original sound file
//sFindTimeDomainFrequenceAndAmplitude from soundFFT file
class cToneData:public NamedTypedObject,public cNodeISAX
{
	std::string	m_strSoundFilePath;
	int			m_iSoundID;
	sFindTimeDomainFrequenceAndAmplitude*m_pFrequenceAndAmplitudeAndTimeFinder;
public:
	DEFINE_TYPE_INFO();
	cToneData(TiXmlElement*e_pTiXmlElement);
	~cToneData();
	const int GetSoundID();
	const sFindTimeDomainFrequenceAndAmplitude*	GetFrequenceAndAmplitudeAndTimeFinder();
};

//<cToneDataVector>
//	<cToneData SoundSourceFileName="ooxx.wav" ID="0">
//</cToneDataVector>

class cToneDataVector:public cNamedTypedObjectVector<cToneData>,public cNodeISAX
{
	virtual bool MyParse(TiXmlElement*e_pRoot);
public:
	DEFINE_TYPE_INFO();
	cToneDataVector();
	~cToneDataVector();
	const sFindTimeDomainFrequenceAndAmplitude*GetFrequenceAndAmplitudeAndTimeFinderBySoundID(int e_iSoundID);
};