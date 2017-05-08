#include "stdafx.h"
#include "SingleSoundCompare.h"

float	cSoundCompareParameter::m_sfTolerateTime = 0.32f;
int		cSoundCompareParameter::m_siAmplitudeOffset = 30;

sFrequenceAndAmplitudeAndTimeFinder::sFrequenceAndAmplitudeAndTimeFinder(TiXmlElement*e_pTiXmlElement)
{//
	//5 minutes
	int l_iAssumeBigEnoughSecond = 60*5;
	auto l_pLastChild = e_pTiXmlElement->LastChild();
	if( l_pLastChild && l_pLastChild->ToElement() )
	{
		sFrequenceAndAmplitudeAndTime l_FrequenceAndAmplitudeAndTime(l_pLastChild->ToElement());
		//l_iAssumeBigEnoughSecond = (int)(l_FrequenceAndAmplitudeAndTime.fStartTime+l_FrequenceAndAmplitudeAndTime.fKeepTime);
		l_iAssumeBigEnoughSecond = (int)l_FrequenceAndAmplitudeAndTime.fStartTime;
		OneScondFrequenceAndAmplitudeAndTimeData.resize(l_iAssumeBigEnoughSecond);
		for( int i=0;i<l_iAssumeBigEnoughSecond;++i )
		{
			OneScondFrequenceAndAmplitudeAndTimeData[i] = nullptr;
		}
	}
	int l_iCurrentSecond = -1;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("FrequenceAndAmplitudeAndTime")
		{
			sFrequenceAndAmplitudeAndTime*l_pFrequenceAndAmplitudeAndTime = new sFrequenceAndAmplitudeAndTime(e_pTiXmlElement);
			int l_iCurrentSecond = (int)l_pFrequenceAndAmplitudeAndTime->fStartTime;
			assert(l_iAssumeBigEnoughSecond>l_iCurrentSecond&&"the total second get wrong...call fatming");
			if(!OneScondFrequenceAndAmplitudeAndTimeData[l_iCurrentSecond])
			{
				OneScondFrequenceAndAmplitudeAndTimeData[l_iCurrentSecond] = new std::vector<sFrequenceAndAmplitudeAndTime*>();
			}
			OneScondFrequenceAndAmplitudeAndTimeData[l_iCurrentSecond]->push_back(l_pFrequenceAndAmplitudeAndTime);
		}
	PARSE_NAME_VALUE_END
}

//sFrequenceAndAmplitudeAndTimeFinder::sFrequenceAndAmplitudeAndTimeFinder(sFrequenceAndAmplitudeAndTimeFinder*e_pFrequenceAndAmplitudeAndTimeFinder)
//{
//	fCurrentTime = 0.f;
//	size_t l_uiSize = e_pFrequenceAndAmplitudeAndTimeFinder->OneScondFrequenceAndAmplitudeAndTimeData.size();
//	for( size_t i=0;i<l_uiSize;++i )
//	{
//		auto l_pData = e_pFrequenceAndAmplitudeAndTimeFinder->OneScondFrequenceAndAmplitudeAndTimeData[i];
//		std::vector<sFrequenceAndAmplitudeAndTime*>*l_pNewVector = new std::vector<sFrequenceAndAmplitudeAndTime*>();
//		for(auto l_pInnerData : *l_pData )
//		{
//			sFrequenceAndAmplitudeAndTime*l_pNewData = new sFrequenceAndAmplitudeAndTime(l_pInnerData);
//			l_pNewVector->push_back(l_pNewData);
//		}
//		this->OneScondFrequenceAndAmplitudeAndTimeData.push_back(l_pNewVector);
//	}
//}
//
//sFrequenceAndAmplitudeAndTimeFinder*	sFrequenceAndAmplitudeAndTimeFinder::Clone()
//{
//	sFrequenceAndAmplitudeAndTimeFinder*l_pFrequenceAndAmplitudeAndTimeFinder = new sFrequenceAndAmplitudeAndTimeFinder(this);
//	return l_pFrequenceAndAmplitudeAndTimeFinder;
//}

sFrequenceAndAmplitudeAndTimeFinder::sFrequenceAndAmplitudeAndTimeFinder()
{
}

sFrequenceAndAmplitudeAndTimeFinder::~sFrequenceAndAmplitudeAndTimeFinder()
{
	DELETE_VECTOR_VECTORPOINTER(OneScondFrequenceAndAmplitudeAndTimeData,sFrequenceAndAmplitudeAndTime*);
}


bool	sFrequenceAndAmplitudeAndTimeFinder::GetDataByTime(float e_fTime,float e_fTolerateTime,std::vector<sFrequenceAndAmplitudeAndTime*>*e_pOutVector,bool e_bIgnodeSameObject)
{
	if( e_pOutVector == nullptr )
		return false;
	int l_iSecondIndex = (int)e_fTime;
	auto l_pData = OneScondFrequenceAndAmplitudeAndTimeData[l_iSecondIndex];
	if( l_pData )
	{
		//for(int i=0;i<l_pData->iNum;++i)
		//{
		//	auto l_pFrequenceAndAmplitudeAndTime = &l_pData->pData[i];
		for(size_t i=0;i<l_pData->size();++i)
		{
			auto l_pFrequenceAndAmplitudeAndTime = (*l_pData)[i];
			if(abs(e_fTime-l_pFrequenceAndAmplitudeAndTime->fStartTime)<=e_fTolerateTime)
			{
				bool l_bAddToVector = true;
				if( !e_bIgnodeSameObject )
				{
					size_t l_uiVectorSize = e_pOutVector->size();
					for(size_t j=0;j<l_uiVectorSize;++j)
					{
						if((*e_pOutVector)[j] == l_pFrequenceAndAmplitudeAndTime)
						{
							l_bAddToVector = false;
							break;
						}
					}
				}
				if( l_bAddToVector )
					e_pOutVector->push_back(l_pFrequenceAndAmplitudeAndTime);
			}
		}
		if( e_pOutVector->size() )
			return true;
	}
	return false;
}

cSindSoundCompare::cSindSoundCompare(sFrequenceAndAmplitudeAndTimeFinder*e_pData,float e_fCompareTime)
{
	m_fCompareTime = e_fCompareTime;
	m_pFrequenceAndAmplitudeAndTimeFinder = e_pData;
	m_iCurrentMatchedIndex = 0;
	m_iNumMatched = 0;
	m_bMustMatchAfterProior = false;
}

cSindSoundCompare::~cSindSoundCompare()
{

}

bool		cSindSoundCompare::Compare(float e_fCurrentTime,cQuickFFTDataFrequencyFinder*e_pQuickFFTDataFrequencyFinder)
{
	float l_fLocalTime = e_fCurrentTime-this->m_fCompareTime;
	int	l_iAllMatched = 0;
	auto l_pData = this->m_pFrequenceAndAmplitudeAndTimeFinder->OneScondFrequenceAndAmplitudeAndTimeData[this->m_iCurrentMatchedIndex];
	for(auto l_pInnerData :	*l_pData)
	{
		int l_iAmplitude = e_pQuickFFTDataFrequencyFinder->GetAmplitude((int)l_pInnerData->fFrequency);
		if(cSoundCompareParameter::m_siAmplitudeOffset<=abs(l_iAmplitude-l_pInnerData->iAmplitude))
		{//matched
			++l_iAllMatched;
		}			
	}
	if( l_iAllMatched == l_pData->size() )
	{
		++m_iCurrentMatchedIndex;
	}
	if(this->m_iCurrentMatchedIndex >= (int)this->m_pFrequenceAndAmplitudeAndTimeFinder->OneScondFrequenceAndAmplitudeAndTimeData.size())
	{
		//all matched
		return true;
	}
	return this->IsFinish(e_fCurrentTime);
}

bool		cSindSoundCompare::IsFinish(float e_fCurrentTime)
{
	float l_iEndSecond = this->m_pFrequenceAndAmplitudeAndTimeFinder->OneScondFrequenceAndAmplitudeAndTimeData.size()+this->m_fCompareTime;
	if( e_fCurrentTime-l_iEndSecond >= cSoundCompareParameter::m_sfTolerateTime )
	{
		return true;	
	}
	return false;
}