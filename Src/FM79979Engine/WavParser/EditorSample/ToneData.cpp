#include "stdafx.h"
#include "FindTimeDomainFrequenceAndAmplitude.h"
#include "ToneData.h"
TYPDE_DEFINE_MARCO(cToneData);
TYPDE_DEFINE_MARCO(cToneDataVector);

cToneData::cToneData(TiXmlElement*e_pTiXmlElement)
{
	m_pFrequenceAndAmplitudeAndTimeFinder = nullptr;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_ASSIGN_CHAR_STRING("oundFilePath",m_strSoundFilePath)
		else
		COMPARE_ASSIGN_INT("SoundID",m_iSoundID)
	PARSE_NAME_VALUE_END
	this->SetName(ValueToStringW(m_iSoundID));
	m_pFrequenceAndAmplitudeAndTimeFinder = new sFindTimeDomainFrequenceAndAmplitude(m_strSoundFilePath.c_str());
}

cToneData::~cToneData()
{
	SAFE_DELETE(m_pFrequenceAndAmplitudeAndTimeFinder);
}

const int cToneData::GetSoundID()
{
	return this->m_iSoundID;
}

const sFindTimeDomainFrequenceAndAmplitude*	cToneData::GetFrequenceAndAmplitudeAndTimeFinder()
{
	return this->m_pFrequenceAndAmplitudeAndTimeFinder;
}

cToneDataVector::cToneDataVector()
{

}

cToneDataVector::~cToneDataVector()
{

}

const sFindTimeDomainFrequenceAndAmplitude*cToneDataVector::GetFrequenceAndAmplitudeAndTimeFinderBySoundID(int e_iSoundID)
{
	int l_iCount = Count();
	for( int i=0;i<l_iCount;++i )
	{
		if( this->GetObject(i)->GetSoundID() == e_iSoundID)
			return this->GetObject(i)->GetFrequenceAndAmplitudeAndTimeFinder();
	}
	return nullptr;
}

//<cToneDataVector>
//	<cToneData SoundSourceFileName="ooxx.wav" ID="0">
//</cToneDataVector>

bool cToneDataVector::MyParse(TiXmlElement*e_pRoot)
{
	e_pRoot = e_pRoot->FirstChildElement();
	PARSE_ELEMENT_START(e_pRoot)
		COMPARE_VALUE("ToneData")
		{
			cToneData*l_pToneData = new cToneData(e_pRoot);
			if(!this->AddObject(l_pToneData))
			{
				UT::ErrorMsg(l_strValue,L"ToneData Same Name!?");
			}
		}
	PARSE_NAME_VALUE_END
	return true;
}