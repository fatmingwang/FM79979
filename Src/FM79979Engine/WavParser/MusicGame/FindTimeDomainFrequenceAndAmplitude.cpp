#include "stdafx.h"
#include "FindTimeDomainFrequenceAndAmplitude.h"

sFindTimeDomainFrequenceAndAmplitude::sFindTimeDomainFrequenceAndAmplitude(const char*e_strFileName)
{
	cNodeISAX l_NodeISAX;
	if(l_NodeISAX.ParseDataIntoXMLNode(e_strFileName))
	{
		TiXmlElement*l_pTiXmlElement = l_NodeISAX.GetRootElement();
#ifdef PARSE_TEST_SOUND
		{
			const wchar_t*l_strSoundSourceFileName = l_pTiXmlElement->Attribute(CHAR_TO_WCHAR_DEFINE(SOUD_SOURCE_FILE_NAME));
			if( l_strSoundSourceFileName )
			{
				std::wstring l_strSoundFileName = UT::GetFileNameWithoutFullPath(l_strSoundSourceFileName,false);
				std::string l_strDirectory = UT::GetDirectoryWithoutFileName(e_strFileName);
				l_strDirectory += UT::WcharToChar(l_strSoundFileName);
				cGameApp::m_spSoundParser->AddStaticSound(cGameApp::m_spSoundParser,l_strDirectory.c_str());
			}
		}
#endif
		int l_iLastTimeInSecond = SetupTotalSecond(l_pTiXmlElement);
		FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(l_pTiXmlElement)
			COMPARE_TARGET_ELEMENT_VALUE(l_pTiXmlElement,"FrequenceAndAmplitudeAndTime")
			{
				GenerateFrequenceAndAmplitudeAndTime(l_pTiXmlElement,l_iLastTimeInSecond);
			}
		FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(l_pTiXmlElement)
	}
	else
	{
		UT::ErrorMsg("ParseFiled!",e_strFileName);
	}
}

//sFindTimeDomainFrequenceAndAmplitude::sFindTimeDomainFrequenceAndAmplitude(TiXmlElement*e_pTiXmlElement)
//{//
//	//5 minutes
//	int l_iAssumeBigEnoughSecond = 60*5;
//	auto l_pLastChild = e_pTiXmlElement->LastChild();
//	if( l_pLastChild && l_pLastChild->ToElement() )
//	{
//		sFrequenceAndAmplitudeAndTime l_FrequenceAndAmplitudeAndTime(l_pLastChild->ToElement());
//		//l_iAssumeBigEnoughSecond = (int)(l_FrequenceAndAmplitudeAndTime.fStartTime+l_FrequenceAndAmplitudeAndTime.fKeepTime);
//		l_iAssumeBigEnoughSecond = (int)l_FrequenceAndAmplitudeAndTime.fStartTime;
//		OneScondFrequenceAndAmplitudeAndTimeData.resize(l_iAssumeBigEnoughSecond);
//		for( int i=0;i<l_iAssumeBigEnoughSecond;++i )
//		{
//			OneScondFrequenceAndAmplitudeAndTimeData[i] = nullptr;
//		}
//	}
//	int l_iCurrentSecond = -1;
//	PARSE_ELEMENT_START(e_pTiXmlElement)
//		COMPARE_NAME("FrequenceAndAmplitudeAndTime")
//		{
//			sFrequenceAndAmplitudeAndTime*l_pFrequenceAndAmplitudeAndTime = new sFrequenceAndAmplitudeAndTime(e_pTiXmlElement);
//			int l_iCurrentSecond = (int)l_pFrequenceAndAmplitudeAndTime->fStartTime;
//			assert(l_iAssumeBigEnoughSecond>l_iCurrentSecond&&"the total second get wrong...call fatming");
//			if(!OneScondFrequenceAndAmplitudeAndTimeData[l_iCurrentSecond])
//			{
//				OneScondFrequenceAndAmplitudeAndTimeData[l_iCurrentSecond] = new std::vector<sFrequenceAndAmplitudeAndTime*>();
//			}
//			OneScondFrequenceAndAmplitudeAndTimeData[l_iCurrentSecond]->push_back(l_pFrequenceAndAmplitudeAndTime);
//		}
//	PARSE_NAME_VALUE_END
//}

int	sFindTimeDomainFrequenceAndAmplitude::SetupTotalSecond(TiXmlElement*e_pTiXmlElement)
{
	int l_iAssumeBigEnoughSecond = -1;
	auto l_pLastChild = e_pTiXmlElement->LastChild();
	COMPARE_TARGET_ELEMENT_VALUE(l_pLastChild,"FrequenceAndAmplitudeAndTime")
	{
		if( l_pLastChild && l_pLastChild->ToElement() )
		{
			sFrequenceAndAmplitudeAndTime l_FrequenceAndAmplitudeAndTime(l_pLastChild->ToElement());
			l_iAssumeBigEnoughSecond = (int)l_FrequenceAndAmplitudeAndTime.fStartTime+1;
			OneScondFrequenceAndAmplitudeAndTimeData.resize(l_iAssumeBigEnoughSecond);
			for( int i=0;i<l_iAssumeBigEnoughSecond;++i )
			{
				OneScondFrequenceAndAmplitudeAndTimeData[i] = nullptr;
			}
		}
	}
	return l_iAssumeBigEnoughSecond;
}

bool		sFindTimeDomainFrequenceAndAmplitude::GenerateFrequenceAndAmplitudeAndTime(TiXmlElement*e_pTiXmlElement,int e_iCurrentTimeInSecond)
{
	sFrequenceAndAmplitudeAndTime*l_pFrequenceAndAmplitudeAndTime = new sFrequenceAndAmplitudeAndTime(e_pTiXmlElement);
	int l_iCurrentSecond = (int)l_pFrequenceAndAmplitudeAndTime->fStartTime;
	if( e_iCurrentTimeInSecond <= l_iCurrentSecond )
	{
		int a=0;
	}
	assert(e_iCurrentTimeInSecond > l_iCurrentSecond&&"the total second get wrong...call fatming");
	if(!OneScondFrequenceAndAmplitudeAndTimeData[l_iCurrentSecond])
	{
		OneScondFrequenceAndAmplitudeAndTimeData[l_iCurrentSecond] = new std::vector<sFrequenceAndAmplitudeAndTime*>();
	}
	OneScondFrequenceAndAmplitudeAndTimeData[l_iCurrentSecond]->push_back(l_pFrequenceAndAmplitudeAndTime);
	return true;
}

sFindTimeDomainFrequenceAndAmplitude::sFindTimeDomainFrequenceAndAmplitude(const sFindTimeDomainFrequenceAndAmplitude*e_pFrequenceAndAmplitudeAndTimeFinder)
{
	size_t l_uiSize = e_pFrequenceAndAmplitudeAndTimeFinder->OneScondFrequenceAndAmplitudeAndTimeData.size();
	for( size_t i=0;i<l_uiSize;++i )
	{
		auto l_pData = e_pFrequenceAndAmplitudeAndTimeFinder->OneScondFrequenceAndAmplitudeAndTimeData[i];
		std::vector<sFrequenceAndAmplitudeAndTime*>*l_pNewVector = new std::vector<sFrequenceAndAmplitudeAndTime*>();
		for(auto l_pInnerData : *l_pData )
		{
			sFrequenceAndAmplitudeAndTime*l_pNewData = new sFrequenceAndAmplitudeAndTime(l_pInnerData);
			l_pNewVector->push_back(l_pNewData);
		}
		this->OneScondFrequenceAndAmplitudeAndTimeData.push_back(l_pNewVector);
	}
}

//sFindTimeDomainFrequenceAndAmplitude*	sFindTimeDomainFrequenceAndAmplitude::Clone()
//{
//	sFindTimeDomainFrequenceAndAmplitude*l_pFrequenceAndAmplitudeAndTimeFinder = new sFindTimeDomainFrequenceAndAmplitude(this);
//	return l_pFrequenceAndAmplitudeAndTimeFinder;
//}

sFindTimeDomainFrequenceAndAmplitude::sFindTimeDomainFrequenceAndAmplitude()
{
}

sFindTimeDomainFrequenceAndAmplitude::~sFindTimeDomainFrequenceAndAmplitude()
{
	DELETE_VECTOR_VECTORPOINTER(OneScondFrequenceAndAmplitudeAndTimeData,sFrequenceAndAmplitudeAndTime*);
}


bool	sFindTimeDomainFrequenceAndAmplitude::GetDataByTime(float e_fTime,float e_fTolerateTime,std::vector<sFrequenceAndAmplitudeAndTime*>*e_pOutVector,bool e_bIgnodeSameObject)
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


cNoteFrequencyAndDecibles::cNoteFrequencyAndDecibles(const char*e_strFileName)
{ 
	cNodeISAX l_NodeISAX;
	if(l_NodeISAX.ParseDataIntoXMLNode(e_strFileName))
	{
		TiXmlElement*l_pTiXmlElement = l_NodeISAX.GetRootElement();
		ProcessData(l_pTiXmlElement);
	}
}

cNoteFrequencyAndDecibles::~cNoteFrequencyAndDecibles()
{
	
}

bool cNoteFrequencyAndDecibles::ProcessData(TiXmlElement*e_pFFTHitCountAndTimeElement)
{
	e_pFFTHitCountAndTimeElement = e_pFFTHitCountAndTimeElement->FirstChildElement();
	PARSE_ELEMENT_START(e_pFFTHitCountAndTimeElement)
		COMPARE_NAME("StartTime")
		{
		}
		else
		COMPARE_NAME("FrequencyVector")
		{
			FrequencyVector = GetValueListByCommaDivide<int>(l_strValue);
		}
		else
		COMPARE_NAME("FrequencyHittedCount")
		{
			FrequencyHittedCountVector = GetValueListByCommaDivide<int>(l_strValue);
		}
		else
		COMPARE_NAME("FrequencyHittedValue")
		{
			 FrequencyHittedValueVector = GetValueListByCommaDivide<int>(l_strValue);
		}
		else
		COMPARE_NAME("Count")
		{//only for check
		}
	PARSE_NAME_VALUE_END
	return true;
}