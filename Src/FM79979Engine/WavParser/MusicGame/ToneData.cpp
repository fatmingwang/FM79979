#include "stdafx.h"
#include "FindTimeDomainFrequenceAndAmplitude.h"
#include "ToneData.h"
#include "ToneData.h"
#include "Parameters.h"
TYPDE_DEFINE_MARCO(cToneData);
TYPDE_DEFINE_MARCO(cToneDataVector);

cToneData::cToneData(TiXmlElement*e_pTiXmlElement)
{
	m_pbBlackKey = nullptr;
	m_pNoteFrequencyAndDecibles = nullptr;
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
	if(UT::IsStringContain(l_strTest,FREQUENCY_AND_DEIBELS_EXTENSION_FILE_NAME))
	{
		m_pNoteFrequencyAndDecibles = new cNoteFrequencyAndDecibles(m_strSoundFilePath.c_str());
	}
	else
		m_pFrequenceAndAmplitudeAndTimeFinder = new sFindTimeDomainFrequenceAndAmplitude(m_strSoundFilePath.c_str());
	m_iMatchTime = 0;
	//fuck...PC and IOS 5 is okay but android not work...why have no idea... 
#ifdef ANDROID
	m_iMatchTimeCondition = 3;
#else
	m_iMatchTimeCondition = 7;
#endif
	m_bStartHittedCount = false;
}

cToneData::~cToneData()
{
	SAFE_DELETE(m_pNoteFrequencyAndDecibles);
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

cNoteFrequencyAndDecibles*			cToneData::GetNoteFrequencyAndDecibles()
{
	return m_pNoteFrequencyAndDecibles;
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

float	cToneData::CompareFFTDecibles(cQuickFFTDataFrequencyFinder*e_pQuickFFTDataFrequencyFinder)
{
	if( !e_pQuickFFTDataFrequencyFinder || !m_pNoteFrequencyAndDecibles )
		return -2.f;
	int l_iNumMatched = 0;
	size_t l_uiSize = m_pNoteFrequencyAndDecibles->FrequencyVector.size();
	if( l_uiSize == 0 )
		return -1.f;
	//float l_fCurrentProgress = 0.f;
	//float l_fOneStep = 1.f/l_uiSize;
	for(size_t i=0;i<l_uiSize;++i  )
	{
		//l_fCurrentProgress += l_fOneStep;
		//int l_iFrequency = m_pNoteFrequencyAndDecibles->FrequencyVector[i];
		int l_iFFTBinIndex = m_pNoteFrequencyAndDecibles->FrequencyBinIndexVector[i];
		//int l_iTargetDecibels = m_pNoteFrequencyAndDecibles->FrequencyHittedValueVector[i]-13;
		float l_fTargetDecibels = m_pNoteFrequencyAndDecibles->FrequencyHittedValueVector[i]/cSoundCompareParameter::m_sfDecibelsMatchDivideValue;
		if( l_fTargetDecibels < 9 )
		{//too small ignore this and set it as matched
			++l_iNumMatched;
			continue;
		}
		//int l_fTargetDecibels = 8;
		std::vector<int>	l_DeciblesVector = e_pQuickFFTDataFrequencyFinder->GetDecibelsByFFTBinIndex(l_iFFTBinIndex);
		//l_fTargetDecibels -= (1-l_fCurrentProgress)*13.f;
		//l_fTargetDecibels -= 13;
		for( int l_iDecible : l_DeciblesVector )
		{
			if( (float)l_iDecible > l_fTargetDecibels)
			{
				++l_iNumMatched;
				break;
			}
		}
	}
	
	//because some frequency just not we want but I have no idea how to filter this so...
	float l_fPercent = (float)l_iNumMatched/l_uiSize;
	//return l_fPercent;
	//float l_fToomanySampleSoGiveALittleReduce = l_uiSize/800.f;
	//return l_fPercent+l_fToomanySampleSoGiveALittleReduce;
	if(l_fPercent >= 0.7)
	{
		m_bStartHittedCount = true;
	}
	if( m_bStartHittedCount )
	{
		if(l_fPercent >= 0.7)
		{
			++m_iMatchTime;
			if( m_iMatchTime < m_iMatchTimeCondition )
			{
				l_fPercent = 0.f;
			}
		}
		else
		{
			m_bStartHittedCount = false;
		}
	}
	else
	{
		m_iMatchTime = 0;
		l_fPercent = 0.f;
	}
	return l_fPercent;
}

void	cToneData::Render()
{
	if( !m_pNoteFrequencyAndDecibles )
		return;
	//int l_iCount = (int)m_pNoteFrequencyAndDecibles->FrequencyVector.size();
	//float l_fXGap = this->GetWidthGapByPoints(l_iCount);
	//Vector2 l_vShowPos = m_vShowPos;
	//std::vector<Vector2>	l_vBlueLinePos;
	//Vector2 l_vLazyLineTemplate[2048*2];
	//for( int i=0;i<l_iCount;++i )
	//{
	//	l_vLazyLineTemplate[i*2].x = l_vShowPos.x;
	//	l_vLazyLineTemplate[i*2+1].x = l_vShowPos.x;
	//	l_vLazyLineTemplate[i*2].y = l_vShowPos.y;
	//	l_vLazyLineTemplate[i*2+1].y = e_piData[i]*e_fYScale+l_vShowPos.y;
	//	if( i % 100 == 0 )
	//	{
	//		Vector2 l_vPosYIncrease = e_pvPointsData[i*2+1];
	//		l_vPosYIncrease.y += 20;
	//		l_vBlueLinePos.push_back(e_pvPointsData[i*2]);
	//		l_vBlueLinePos.push_back(l_vPosYIncrease);
	//	}
	//	l_vShowPos.x += l_fXGap;
	//}
	//RenderLine((float*)e_pvPointsData,e_iCount*2,e_vColor,2);
	//RenderLine(&l_vBlueLinePos,Vector4::Blue);
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
				UT::ErrorMsg(l_pToneData->GetName(),L"ToneData Same Name!?");
				SAFE_DELETE(l_pToneData);
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

void	cToneDataVector::Update(float e_fElpaseTime,cQuickFFTDataFrequencyFinder*e_pQuickFFTDataFrequencyFinder)
{
	m_MatchName.clear();
	m_ResultVector.clear();
	if( !e_pQuickFFTDataFrequencyFinder )
		return;
	int l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		auto l_pData = this->GetObject(i);
		float l_fResult = l_pData->CompareFFTDecibles(e_pQuickFFTDataFrequencyFinder);
		m_ResultVector.push_back(l_fResult);
		std::wstring l_strResult = l_pData->GetName();
		l_strResult += L":";
		l_strResult += ValueToStringW(l_fResult);
		m_MatchName.push_back(l_strResult);
	}
}

void	cToneDataVector::Render()
{	
	Vector2 l_vShowPos(cGameApp::m_spOpenGLRender->m_vGameResolution.x/2,20);
	cGameApp::m_spGlyphFontRender->SetScale(2);
	//if( !m_TempForMatchName.bTragetTimrReached )
	{
		float l_fMax = 0.8f;
		int l_iMaxIndex = -1;
		for(size_t i=0;i<m_MatchName.size();++i)
		{
			if( m_ResultVector[i] >= 1.f )
				cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::Green);
			else
				cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::Red);
			cGameApp::RenderFont(l_vShowPos,m_MatchName[i].c_str());
			l_vShowPos.y += 70;
			if(cGameApp::m_spOpenGLRender->m_vGameResolution.y-100 <= l_vShowPos.y )
			{
				l_vShowPos.y = 20.f;
				l_vShowPos.x += 600.f;
			}
			if( l_fMax < m_ResultVector[i] )
			{
				l_fMax = m_ResultVector[i];
				l_iMaxIndex = (int)i;
			}
		}
		if( l_iMaxIndex != -1 )
		{
			l_vShowPos = Vector2(cGameApp::m_spOpenGLRender->m_vGameResolution.x/2+600,680);
			cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::One);
			cGameApp::RenderFont(l_vShowPos,m_MatchName[l_iMaxIndex].c_str());
		}
	}
	//else
	//{
	//	static int l_i=0;
	//	++l_i;
	//	std::wstring l_str = L"Detecting";
	//	int l_iSize = wcslen(l_str.c_str());
	//	wchar_t l_str2[200];
	//	memset(l_str2,0,sizeof(200)*sizeof(wchar_t));
	//	for( int i=0;i<l_i;++i )
	//		l_str2[i] = l_str[i];
	//	cGameApp::RenderFont(l_vShowPos,l_str2);
	//	if( l_i >=l_iSize )
	//		l_i = 0;
	//}
	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::One);
	cGameApp::m_spGlyphFontRender->SetScale(1);
}