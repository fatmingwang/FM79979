#include "stdafx.h"
#include "TimeFrequencyAmplitudeValueCampare.h"

//<FrequenceAndAmplitudeAndTime>
	//<///wewewewewe>
//</FrequenceAndAmplitudeAndTime>

sFrequenceAndAmplitudeAndTimeFinder::sFrequenceAndAmplitudeAndTimeFinder(TiXmlElement*e_pTiXmlElement)
{//
	//5 minutes
	int l_iAssumeBigEnoughSecond = 60*5;
	//OneScondFrequenceAndAmplitudeAndTimeData.resize(l_iAssumeBigEnoughSecond);
	//for( int i=0;i<l_iAssumeBigEnoughSecond;++i )
	//{
	//	OneScondFrequenceAndAmplitudeAndTimeData[i] = nullptr;
	//}
	auto l_pLastChild = e_pTiXmlElement->LastChild();
	if( l_pLastChild )
	{
		sFrequenceAndAmplitudeAndTime l_FrequenceAndAmplitudeAndTime(e_pTiXmlElement);;
		l_iAssumeBigEnoughSecond = (int)(l_FrequenceAndAmplitudeAndTime.fStartTime+l_FrequenceAndAmplitudeAndTime.fKeepTime);
		OneScondFrequenceAndAmplitudeAndTimeData.resize(l_iAssumeBigEnoughSecond);
		for( int i=0;i<l_iAssumeBigEnoughSecond;++i )
		{
			OneScondFrequenceAndAmplitudeAndTimeData[i] = nullptr;
		}
	}
	//l_pLastChild-
	int l_iCurrentSecond = -1;
	//std::vector<sFrequenceAndAmplitudeAndTime*>l_Temp[];
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

bool	cTimeFrequencyAmplitudeValueCompare::MyParse(TiXmlElement*e_pRoot)
{
	e_pRoot;
	return true;
}

void	cTimeFrequencyAmplitudeValueCompare::Update(float e_fElpaseTime)
{

}

void	cTimeFrequencyAmplitudeValueCompare::AddNewData(sFrequencyAndAmplitude e_FrequencyAndAmplitude)
{

}

void	cTimeFrequencyAmplitudeValueCompare::SetupInputDataFFT(int e_iNumBins)
{

}