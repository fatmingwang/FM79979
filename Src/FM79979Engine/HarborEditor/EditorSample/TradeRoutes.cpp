#include "stdafx.h"
#include "TradeRoutes.h"
#include "RegionMapInfo.h"
const  wchar_t*        cTradeRoutes::TypeID = L"cTradeRoutes";

cTradeRoutes::cTradeRoutes()
{
	m_fSEEffect = 1.f;
	m_fESEffect = 1.f;
	m_fDistance = 1.f;
	m_pStartPoint = nullptr;
	m_pEndPoint = nullptr;
	m_pRoute = new cLinerDataProcessor<Vector3>;
}

cTradeRoutes::cTradeRoutes(cTradeRoutes*e_pTradeRoutes)
{
	m_fSEEffect = e_pTradeRoutes->m_fSEEffect;
	m_fESEffect = e_pTradeRoutes->m_fESEffect;
	m_fDistance = e_pTradeRoutes->m_fDistance;
	m_pStartPoint = e_pTradeRoutes->m_pStartPoint;
	m_pEndPoint = e_pTradeRoutes->m_pEndPoint;
	if( e_pTradeRoutes->m_pRoute )
		m_pRoute = (cLinerDataProcessor<Vector3>*)e_pTradeRoutes->m_pRoute->Clone();
	else
		m_pRoute = new cLinerDataProcessor<Vector3>;
}

cTradeRoutes::~cTradeRoutes()
{
	SAFE_DELETE(m_pRoute);
}

cTradeRoutes::cTradeRoutes(TiXmlElement*e_pTiXmlElement)
{
	m_fSEEffect = 1.f;
	m_fESEffect = 1.f;
	m_fDistance = 1.f;
	m_pStartPoint = nullptr;
	m_pEndPoint = nullptr;
	m_pRoute = nullptr;
	ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cTradeRoutes::TypeID);
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("SEEffect")
		{
			m_fSEEffect = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("ESEffect")
		{
			m_fESEffect = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("Distance")
		{
			m_fDistance = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("Name")
		{
			this->SetName(l_strValue);
			std::vector<std::wstring>l_HarborVector = GetWStringListByCommaDivide(l_strValue);
			assert(l_HarborVector.size() == 2 &&"need 2 harbor name");
			if( l_HarborVector.size() == 2 )
			{
				this->m_pStartPoint = g_pRegionMapPointInfoManager->GetObject(l_HarborVector[0]);
				this->m_pEndPoint = g_pRegionMapPointInfoManager->GetObject(l_HarborVector[1]);
			}
		}
	PARSE_NAME_VALUE_END
	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	if( !wcscmp(e_pTiXmlElement->Value(),L"cLinerDataProcessor") )
	{
		m_pRoute = new cLinerDataProcessor<Vector3>(e_pTiXmlElement);
		size_t	l_uiSize = m_pRoute->GetLinerDataVector()->size();
		if( l_uiSize > 1 )
		{
			m_pRoute->ChangeData(0,m_pStartPoint->GetMapPointData()->vPos);
			m_pRoute->ChangeData(m_pRoute->GetLinerDataVector()->size()-1,this->m_pEndPoint->GetMapPointData()->vPos);
		}

	}
}

TiXmlElement*	cTradeRoutes::ToTiXmlElement()
{
	TiXmlElement*l_pTiXmlElement = new TiXmlElement(cTradeRoutes::TypeID);
	l_pTiXmlElement->SetAttribute(L"SEEffect",m_fSEEffect);
	l_pTiXmlElement->SetDoubleAttribute(L"ESEffect",m_fESEffect);
	if( m_pRoute )
	{
		m_fDistance = this->m_pRoute->GetTotalDistance();
		l_pTiXmlElement->SetAttribute(L"Distance",m_fDistance);
	}
	l_pTiXmlElement->SetAttribute(L"Name",this->GetName());
	if( m_pRoute )
	{
		TiXmlElement*l_pRouteElement = m_pRoute->ToTiXmlElement();
		l_pTiXmlElement->LinkEndChild(l_pRouteElement);
	}
	return l_pTiXmlElement;
}

void	cTradeRoutes::SetRoute(cLinerDataProcessor<Vector3>*e_pData)
{
	SAFE_DELETE(m_pRoute);
	m_pRoute = e_pData;
}

float	cTradeRoutes::GetRouteTraveTime(bool e_StoE,float e_fSpeed)
{
	float	l_fEffect = m_fESEffect;
	if( e_StoE )
		l_fEffect = m_fSEEffect;
	float	l_fTime = m_fDistance/e_fSpeed*l_fEffect;
	return  l_fTime;
}


cTradeRoutesManager::cTradeRoutesManager()
{
	
}

cTradeRoutesManager::~cTradeRoutesManager()
{
	
}

bool	cTradeRoutesManager::MyParse(TiXmlElement*e_pRoot)
{
	e_pRoot = e_pRoot->FirstChildElement();
	while( e_pRoot )
	{
		const WCHAR*l_strValue = e_pRoot->Value();
		COMPARE_VALUE_WITH_DEFINE(cTradeRoutes::TypeID)
		{
			cTradeRoutes*l_pTradeRoutes = new cTradeRoutes(e_pRoot);
			this->AddObjectNeglectExist(l_pTradeRoutes);
		}
		e_pRoot = e_pRoot->NextSiblingElement();
	}
	return true;
}

void	cTradeRoutesManager::DebugRender()
{
	LIST_DO_FOR_ALL_CHILDREN(this,GetRoute()->DebugRender());
}

void	cTradeRoutesManager::Export(char*e_strFileName)
{
#ifdef WIN32
	SAFE_DELETE(m_pDoc);
	m_pDoc = new TiXmlDocument();
	TiXmlElement*l_pRoot = new TiXmlElement(L"cTradeRoutesManager");
	m_pDoc->LinkFirstChild(l_pRoot);
	int	l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		cTradeRoutes*l_pTradeRoutes = this->m_ObjectList[i];
		l_pRoot->LinkEndChild(l_pTradeRoutes->ToTiXmlElement());
	}
	cNodeISAX::Export(e_strFileName);
#endif
}