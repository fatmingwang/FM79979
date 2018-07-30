#include "stdafx.h"
#include "FishRodAndFishThreadData.h"

cFishRodData::cFishRodData(TiXmlElement*e_pElement)
{
	m_pImage = 0;
	m_pImageElement = 0;
	m_iCurrentLV = 0;
	const WCHAR*l_strName = e_pElement->Attribute(L"Name");
	this->SetName(l_strName);
	e_pElement = e_pElement->FirstChildElement();
	while( e_pElement )
	{
		const WCHAR*l_strValue = e_pElement->Value();
		COMPARE_VALUE("LVData")
		{
			sLVData*l_pData = sLVData::ProcessData(e_pElement);
			if( l_pData )
			{
				m_LeaveDataVector.push_back(l_pData);
			}
		}
		else
		COMPARE_VALUE("Image")
		{
			//m_pImage = GetRenderObjectBehavior(e_pElement->FirstChildElement());
			m_pImageElement = new TiXmlElement(*e_pElement->FirstChildElement());
		}
		else
		COMPARE_VALUE("Fish")
		{
			const WCHAR*l_strName = e_pElement->Attribute(L"Name");
			const WCHAR*l_strLeaveProbability = e_pElement->Attribute(L"LeaveProbability");
			sFishLeaveProbability	l_FishLeaveProbability;
			l_FishLeaveProbability.fLeaveProbability = GetFloat(l_strLeaveProbability);
			l_FishLeaveProbability.strFishName = l_strName;
			m_FishLeaveProbabilityVector.push_back(l_FishLeaveProbability);
		}
		e_pElement = e_pElement->NextSiblingElement();
	}
//Brake="" Time="" 
}

cFishRodData::~cFishRodData()
{
	SAFE_DELETE(m_pImageElement);
	SAFE_DELETE(m_pImage);
	DELETE_VECTOR(m_LeaveDataVector);
}
	//<LVData LV="1" Brake="" Time="" FishLeaveCost="">
	//	<ImageData>
	//		<cMPDI cMPDIList="" cMPDI="" />
	//	</ImageData>
	//	<Fish Name="1" LeaveProbability="0.1"/>
	//</LVData>
cFishRodData::sLVData*	cFishRodData::sLVData::ProcessData(TiXmlElement*e_pElement)
{
	sLVData*l_pLVData = new sLVData();
	l_pLVData->iUpgradeMoneyCost = 100;
	l_pLVData->fMoneyProbability = 0.5;
	l_pLVData->iUpgradeGoldCost = 10;
	l_pLVData->fGoldProbability = 0.9f;
	const WCHAR*l_strLV = e_pElement->Attribute(L"LV");
	const WCHAR*l_strFishLeaveCost = e_pElement->Attribute(L"FishLeaveCost");
	const WCHAR*l_strHookRisePercent = e_pElement->Attribute(L"HookRisePercent");
	const WCHAR*l_strMoneyCost = e_pElement->Attribute(L"MoneyCost");
	const WCHAR*l_strGoldCost = e_pElement->Attribute(L"Gold");
	l_pLVData->iLevelIndex = GetInt(l_strLV);
	l_pLVData->iFishLeaveCost = GetInt(l_strFishLeaveCost);
	l_pLVData->fHookRisePercent = GetFloat(l_strHookRisePercent);
	if( l_strMoneyCost )
	{
		Vector2	l_vData = GetVector2(l_strMoneyCost);
		l_pLVData->iUpgradeMoneyCost = (int)l_vData.x;
		l_pLVData->fMoneyProbability = l_vData.y;
	}
	if( l_strGoldCost )
	{
		Vector2	l_vData = GetVector2(l_strGoldCost);
		l_pLVData->iUpgradeGoldCost = (int)l_vData.x;
		l_pLVData->fGoldProbability = l_vData.y;
	}


	e_pElement = e_pElement->FirstChildElement();
	while( e_pElement )
	{
		const WCHAR*l_strValue = e_pElement->Value();
		COMPARE_VALUE("ImageData")
		{
		
		}
		else
		COMPARE_VALUE("BrakeData")
		{
			l_pLVData->fBrakeSpeed = ElementToFloat(e_pElement,L"BrakeSpeed");
			l_pLVData->fPowerTime = ElementToFloat(e_pElement,L"PowerTime");
		}
		e_pElement = e_pElement->NextSiblingElement();
	}
	return l_pLVData;
}

cFishRodData::sLVData*	cFishRodData::GetLVData(int e_iLeavel)
{
	if( e_iLeavel < 0 || e_iLeavel >= (int)m_LeaveDataVector.size() )
		return 0;
	return m_LeaveDataVector[e_iLeavel];
}

cFishRodData::sLVData*	cFishRodData::GetCurrentLVData()
{
	return GetLVData(this->m_iCurrentLV);
}

bool	cFishRodData::IsFishLeaving(const WCHAR*e_strFishName)
{
	float	l_fProbability = GetLeaveFishProbability(e_strFishName);
	float	l_fLeaveProbabilityValue = frand(0,1);
	if( l_fProbability >= l_fLeaveProbabilityValue )
		return true;
	return false;
}

float	cFishRodData::GetLeaveFishProbability(const WCHAR*e_strFishName)
{
	sLVData*l_pLVData = GetCurrentLVData();
	if( l_pLVData )
	{
		size_t	l_iSize = m_FishLeaveProbabilityVector.size();
		for( size_t i=0;i<l_iSize;++i )
		{
			if(!wcscmp(m_FishLeaveProbabilityVector[i].strFishName.c_str(),e_strFishName))
			{
				float	l_fLeaveProbability = m_FishLeaveProbabilityVector[i].fLeaveProbability;
				float	l_fHookedProbability = 1-l_fLeaveProbability;
				l_fHookedProbability = l_pLVData->fHookRisePercent*l_fHookedProbability+l_fHookedProbability;
				l_fLeaveProbability = 1-l_fHookedProbability;
				if( l_fLeaveProbability < 0.f )
					l_fLeaveProbability = 0.f;
				return l_fLeaveProbability;
			}
		}
	}
	UT::ErrorMsg(e_strFishName,L"Fish data is not in this FishRod!");
	return 0.f;
}


//cFishRodData::cFishProbabilityToLeave*	cFishRodData::GetFishProbabilityToLeaveDataByID(int e_iID)
//{
//	int	l_iCount = m_FishProbabilityToLeaveData.Count();
//	for(int i=0;i<l_iCount;++i)
//	{
//		if(m_FishProbabilityToLeaveData[i]->m_iQuickIndexID == e_iID)
//			return m_FishProbabilityToLeaveData[i];
//	}
//	return 0;
//}
//<FishThread Name="FishThread1" SinkTime="30" SinkSpeed="100" FloatTime="20" FloatSpeed="166"/>
cFishThread::cFishThread(TiXmlElement*e_pElement)
{
	PARSE_ELEMENT_START(e_pElement)
		COMPARE_NAME("Name")
		{
			this->SetName(l_strValue);
		}
		else
		COMPARE_NAME("SinkTime")
		{
			m_fSinkTime = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("SinkSpeed")
		{
			m_fSinkSpeed = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("FloatTime")
		{
			m_fFloatTime = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("FloatSpeed")
		{
			m_fFloatSpeed = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("Energy")
		{
			m_fEnergy = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("Cost")
		{
			m_fCost = VALUE_TO_FLOAT;
		}
	PARSE_NAME_VALUE_END
	if( m_fSinkTime*m_fSinkSpeed != m_fFloatTime*m_fFloatSpeed )
	{
		UT::ErrorMsg(L"total distance is not same",L"cFishThread data error");
	}
}

cFishThread::~cFishThread()
{

}

//cFishRodData::cFishProbabilityToLeave*	cFishRodData::GetFishProbabilityToLeaveDataByID(const WCHAR*e_strName)
//{
//	return m_FishProbabilityToLeaveData.GetObject(e_strName);
//}

cFishRodAndFishThreadData::cFishRodAndFishThreadData()
{
	m_pCurrentSelectedFishRodData = 0;
	m_pCurrentSelectedcFishThread = 0;
}

cFishRodAndFishThreadData::~cFishRodAndFishThreadData()
{

}
//<FishRodAndFishThreadData>
//	<FishRodData Name="" Description="">
//		<Fish Name="" LeaveTime="" LeaveProbability="" />
//	</FishRodData>
//	<FishThread Name="" Description=""/>
//</FishRodAndFishThreadData>
void	cFishRodAndFishThreadData::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("FishRod")
	{
		cFishRodData*l_pFishRodData = new cFishRodData(e_pTiXmlElement);
		if(!m_FishRodDataVector.AddObject(l_pFishRodData))
		{
			UT::ErrorMsg(l_pFishRodData->GetName(),L"FishRod data error!");
			SAFE_DELETE(l_pFishRodData);
		}
	}
	else
	COMPARE_VALUE("FishThread")
	{
		cFishThread*l_pFishThread = new cFishThread(e_pTiXmlElement);
		if(!m_FishThreadVector.AddObject(l_pFishThread))
		{
			UT::ErrorMsg(l_pFishThread->GetName(),L"FishThread data error!");
			SAFE_DELETE(l_pFishThread);
		}
	}
}