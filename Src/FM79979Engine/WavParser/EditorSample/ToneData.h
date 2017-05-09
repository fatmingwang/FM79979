#pragma once

struct sFrequenceAndAmplitudeAndTimeFinder;

//filter FFT data and original sound file
class cToneData:public NamedTypedObject
{
	std::string	m_strSoundFilePath;
	int			m_iSoundID;
	sFrequenceAndAmplitudeAndTimeFinder*m_pFrequenceAndAmplitudeAndTimeFinder;
public:
	cToneData(TiXmlComment*e_pTiXmlComment);
	~cToneData();
	const sFrequenceAndAmplitudeAndTimeFinder*	GetFrequenceAndAmplitudeAndTimeFinder();
};


class cToneDataVector:public cNamedTypedObjectVector<cToneData>
{
public:
	cToneDataVector();
	~cToneDataVector();
};