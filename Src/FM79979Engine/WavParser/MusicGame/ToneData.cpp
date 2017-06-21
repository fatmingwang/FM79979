#include "stdafx.h"
#include "FindTimeDomainFrequenceAndAmplitude.h"
#include "ToneData.h"
TYPDE_DEFINE_MARCO(cToneData);
TYPDE_DEFINE_MARCO(cToneDataVector);


cToneData::cToneData(TiXmlElement*e_pTiXmlElement)
{
	m_pbBlackKey = nullptr;
	m_pFrequenceAndAmplitudeAndTimeFinder = nullptr;
	const wchar_t*l_strID = CHAR_TO_WCHAR_DEFINE(TONE_DATA_ID);
	const wchar_t*l_strPosOffset = CHAR_TO_WCHAR_DEFINE(TONE_DATA_PICTURE_OFFSET_POS );
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_ASSIGN_CHAR_STRING("SoundSourceFileName",m_strSoundFilePath)
		else
		COMPARE_NAME_WITH_DEFINE( l_strID )
		{
			this->SetName(l_strValue);
		}
		else
		COMPARE_NAME_WITH_DEFINE(l_strPosOffset)
		{
			Vector2 l_vPos = GetVector2(l_strValue);
			this->SetLocalPosition(l_vPos);
		}
	PARSE_NAME_VALUE_END
	const char* l_strTest = m_strSoundFilePath.c_str();
	m_pFrequenceAndAmplitudeAndTimeFinder = new sFindTimeDomainFrequenceAndAmplitude(m_strSoundFilePath.c_str());
}

cToneData::~cToneData()
{
	SAFE_DELETE(m_pbBlackKey);
	SAFE_DELETE(m_pFrequenceAndAmplitudeAndTimeFinder);
}

//const int cToneData::GetSoundID()
//{
//	return this->m_iSoundID;
//}

const sFindTimeDomainFrequenceAndAmplitude*	cToneData::GetFrequenceAndAmplitudeAndTimeFinder()
{
	return this->m_pFrequenceAndAmplitudeAndTimeFinder;
}

bool	cToneData::IsBlackKey()
{
	if(this->m_pbBlackKey == nullptr)
	{
		this->m_pbBlackKey = new bool;
		*this->m_pbBlackKey = false;
		std::wstring l_strName = this->GetName();
		for(size_t i=0;i<l_strName.length();++i)
		{
			if( l_strName[i] == 'm' )
			{
				*this->m_pbBlackKey = true;
				return *this->m_pbBlackKey;
			}
		}
	}
	return *this->m_pbBlackKey;
}

cToneDataVector::cToneDataVector()
{

}

cToneDataVector::~cToneDataVector()
{

}

const sFindTimeDomainFrequenceAndAmplitude*cToneDataVector::GetFrequenceAndAmplitudeAndTimeFinder(const WCHAR*e_strName)
{
	auto l_pData = this->GetObject(e_strName);
	if( l_pData )
	{
		return l_pData->GetFrequenceAndAmplitudeAndTimeFinder();
	}
	return nullptr;
}

//const sFindTimeDomainFrequenceAndAmplitude*cToneDataVector::GetFrequenceAndAmplitudeAndTimeFinderBySoundID(int e_iSoundID)
//{
//	int l_iCount = Count();
//	for( int i=0;i<l_iCount;++i )
//	{
//		if( this->GetObject(i)->GetSoundID() == e_iSoundID)
//			return this->GetObject(i)->GetFrequenceAndAmplitudeAndTimeFinder();
//	}
//	return nullptr;
//}

//<cToneDataVector>
//	<cToneData SoundSourceFileName="ooxx.wav" ID="0">
//</cToneDataVector>

bool cToneDataVector::MyParse(TiXmlElement*e_pRoot)
{
	Vector3	l_vPos = Vector3::Zero;
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pRoot)
		COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pRoot,cToneData::TypeID)
		{
			cToneData*l_pToneData = new cToneData(e_pRoot);
			if(!this->AddObject(l_pToneData))
			{
				SAFE_DELETE(l_pToneData);
				UT::ErrorMsg(l_pToneData->GetName(),L"ToneData Same Name!?");
			}
			else
			{
				l_vPos += l_pToneData->GetLocalPosition();
				l_pToneData->SetLocalPosition(l_vPos);
			}
		}
	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pRoot)
	return true;
}