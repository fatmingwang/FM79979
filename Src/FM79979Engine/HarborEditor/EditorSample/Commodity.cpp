#include "stdafx.h"
#include "Commodity.h"

cCommodityManager*g_pCommodityManager = nullptr;;


const  wchar_t*	cCommodity::TypeID = L"cCommodity";

std::string	ValueToString(eCommodityStandType e_eCommodityStandType)
{
	std::string	l_str;
	switch(e_eCommodityStandType)
	{
		case eCommodityStandType::eCST_NORMAL:
			l_str = "Normal";
		break;
		default:
		//case eCommodityStandType::eCST_MAX:
			l_str = "unsupport";
		break;
	}
	return l_str;
}


cCommodity::cCommodity(int e_iID)
{
	m_iID = e_iID;
	m_iCost = 0;
	m_iAvailableCount = 999;
	m_strIconFileName = "Item1.png";
	m_eCommodityStandType = eCommodityStandType::eCST_NORMAL;
}

cCommodity::cCommodity(TiXmlElement*e_pTiXmlElement)
{
	m_iID = -1;
	m_iCost = 0;
	m_iAvailableCount = 999;
	m_eCommodityStandType = eCommodityStandType::eCST_NORMAL;
	ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cCommodity::TypeID);
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("ID")
		{
			m_iID = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("Cost")
		{
			m_iCost = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("StandType")
		{
			m_eCommodityStandType = (eCommodityStandType)VALUE_TO_INT;
		}
		else
		COMPARE_NAME("ExtraType")
		{
			m_ExtractTypeVector = GetValueListByCommaDivide<int>(l_strValue);
		}
		else
		COMPARE_NAME("Name")
		{
			this->SetName(l_strValue);
		}
		else
		COMPARE_NAME("Count")
		{
			m_iAvailableCount = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("Image")
		{
			m_strIconFileName = ValueToString(l_strValue);
		}
	PARSE_NAME_VALUE_END
}

cCommodity::~cCommodity()
{
	
}

TiXmlElement*cCommodity::ToTiXmlElement()
{
	TiXmlElement*l_pTiXmlElement = new TiXmlElement(cCommodity::TypeID);
	l_pTiXmlElement->SetAttribute(L"Name",this->GetName());
	l_pTiXmlElement->SetAttribute(L"ID",this->m_iID);
	if( m_ExtractTypeVector.size() > 0 )
		l_pTiXmlElement->SetAttribute(L"ExtraType",ValueToStringW<int>(m_ExtractTypeVector).c_str());
	l_pTiXmlElement->SetAttribute(L"StandType",(int)m_eCommodityStandType);
	l_pTiXmlElement->SetAttribute(L"Cost",m_iCost);
	l_pTiXmlElement->SetAttribute(L"Count",this->m_iAvailableCount);
	l_pTiXmlElement->SetAttribute(L"Image",UT::CharToWchar(this->m_strIconFileName.c_str()));
	return l_pTiXmlElement;
}

void	cCommodity::DebugRender(int e_iPosX,int e_iPosY)
{
	wchar_t	l_str[MAX_PATH];
	swprintf(l_str,L"ID:%d,Cost:%d,Type:%s,ExtraType:%s,Count:%d,Image:ls",m_iID,m_iCost,ValueToString(m_eCommodityStandType).c_str(),ValueToString<int>(m_ExtractTypeVector).c_str(),this->m_iAvailableCount,UT::CharToWchar(this->m_strIconFileName).c_str());
	cGameApp::RenderFont(e_iPosX,e_iPosY,l_str);
}

cCommodityManager::cCommodityManager()
{
	g_pCommodityManager = this;
}

cCommodityManager::~cCommodityManager()
{
	while( m_CommoditiesByStandType.size() )
	{
		SAFE_DELETE(m_CommoditiesByStandType.begin()->second);
		m_CommoditiesByStandType.erase(m_CommoditiesByStandType.begin());
	}
}

bool	cCommodityManager::AddObject(cCommodity* e_pCommodity)
{
	if( e_pCommodity == nullptr )
		return false;
	if(GetCommodity(e_pCommodity->GetID(),e_pCommodity->GetCommodityStandType()))
		return false;
	std::vector<cCommodity*>*l_pCommodityVector = nullptr;
	eCommodityStandType	l_eCommodityStandType = e_pCommodity->GetCommodityStandType();
	if( m_CommoditiesByStandType.find(l_eCommodityStandType) == m_CommoditiesByStandType.end())
	{
		l_pCommodityVector = new std::vector<cCommodity*>;
		m_CommoditiesByStandType[l_eCommodityStandType] = l_pCommodityVector;
	}
	else
	{
		l_pCommodityVector = m_CommoditiesByStandType[l_eCommodityStandType];
	}
	l_pCommodityVector->push_back(e_pCommodity);
	return cNamedTypedObjectVector<cCommodity>::AddObjectNeglectExist(e_pCommodity);
}

cCommodity*		cCommodityManager::GetCommodity(int e_iID)
{
	size_t	l_uiSize = m_ObjectList.size();
	for( size_t i=0;i<l_uiSize;++i  )
	{
		if( m_ObjectList[i]->GetID() == e_iID )
			return m_ObjectList[i];
	}
	return nullptr;
}


cCommodity*		cCommodityManager::GetCommodity(int e_iID,eCommodityStandType e_eCommodityStandType)
{
	if( m_CommoditiesByStandType.find(e_eCommodityStandType) == m_CommoditiesByStandType.end())
	{
		return nullptr;
	}
	std::vector<cCommodity*>*l_pCommodityVector = m_CommoditiesByStandType[e_eCommodityStandType];
	size_t	l_uiSize = l_pCommodityVector->size();
	for( size_t i=0;i<l_uiSize;++i  )
	{
		if( (*l_pCommodityVector)[i]->GetID() == e_iID )
			return (*l_pCommodityVector)[i];
	}
	return nullptr;
}

bool	cCommodityManager::MyParse(TiXmlElement*e_pRoot)
{
	e_pRoot = e_pRoot->FirstChildElement();
	while( e_pRoot )
	{
		const WCHAR*l_strValue = e_pRoot->Value();
		COMPARE_VALUE_WITH_DEFINE(cCommodity::TypeID)
		{
			cCommodity*l_pCommodity = new cCommodity(e_pRoot);
			if(!this->AddObject(l_pCommodity))
			{
				UT::ErrorMsg(L"id exists",ValueToStringW(l_pCommodity->GetID()).c_str());
			}
		}
		e_pRoot = e_pRoot->NextSiblingElement();
	}
	return true;
}

bool	cCommodityManager::Export(const char*e_strFileName,bool e_bBinary)
{
#ifdef WIN32
	SAFE_DELETE(m_pDoc);
	m_pDoc = new TiXmlDocument();
	TiXmlElement*l_pRoot = new TiXmlElement(L"Root");
	m_pDoc->LinkFirstChild(l_pRoot);
	std::vector<cCommodity*>*l_pCommodityVector = 0;
	std::map<eCommodityStandType,std::vector<cCommodity*>*>::iterator l_Operator = m_CommoditiesByStandType.begin();
	for(;l_Operator!=m_CommoditiesByStandType.end();++l_Operator )
	{
		l_pCommodityVector = l_Operator->second;
		if( l_pCommodityVector )
		{
			size_t	l_uiSize = l_pCommodityVector->size();
			for(size_t i=0;i<l_uiSize;++i  )
			{
				cCommodity*l_pCommodity = (*l_pCommodityVector)[i];
				TiXmlElement*l_pTiXmlElement = l_pCommodity->ToTiXmlElement();
				l_pRoot->LinkEndChild(l_pTiXmlElement);
			}
		}
	}
	return ISAXCallback::Export(e_strFileName,e_bBinary);
#endif
	return false;
}