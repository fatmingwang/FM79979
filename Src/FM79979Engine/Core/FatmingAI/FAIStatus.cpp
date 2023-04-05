#include "Stdafx.h"
#include "FAIStatus.h"

namespace FATMING_AI
{

	const wchar_t*         cBattlePowerTargetValue::TypeID( L"cBattlePowerTargetValue" );
	const wchar_t*         cBattlePowerList::TypeID( L"cBattlePowerList" );
	const wchar_t*         cFAICharacterStatus::TypeID( L"cFAICharacterStatus" );

	eCharacterAttribute	GetCharacterAttribute(const wchar_t*e_str)
	{
		for( int i=0;i<eCA_UNKNOW;++i )
		{
			if( !wcscmp(e_str,g_strCharacterAttribute[i]) )
			{
				return (eCharacterAttribute)i;
			}
		}
		return eCA_UNKNOW;
	}

	eFAICharacterBehaviorStatus	GetCharacterBehaviodStatus(const wchar_t*e_strName)
	{
		for( int i=0;i<eFAI_CBS_MAX;++i )
		{
			if(!wcscmp(e_strName,g_strCharacterStatus[i]))
			{
				return (eFAICharacterBehaviorStatus)i;
			}
		}
		return eFAI_CBS_MAX;
	}
//<Monkey	Count="3">
//	<Data Attribute="HP" Value="10"  Posiion="0,0" />
//	<Data Attribute="Agile" Value="10"  Posiion="0,100" />
//	<Data Attribute="Defence" Value="10"  Posiion="0,110" />
//</Monkey>
	std::vector<sCharacterAttributeAndUIInfo>	ParseAnimalStatusAndUIInfo(char*e_strFileName)
	{
		std::vector<sCharacterAttributeAndUIInfo>l_StatueAndUIInfoVector;
		cNodeISAX	l_NodeISAX;
		if(l_NodeISAX.ParseDataIntoXMLNode(e_strFileName))
		{
			TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
			l_pElement = l_pElement->FirstChildElement();
			while( l_pElement )
			{
				const wchar_t*l_strAttribute = l_pElement->Attribute(L"Attribute");
				const wchar_t*l_strValue = l_pElement->Attribute(L"Value");
				const wchar_t*l_strPosiion = l_pElement->Attribute(L"Posiion");
				sCharacterAttributeAndUIInfo l_sStatueAndUIInfo;
				if( l_strAttribute )
					l_sStatueAndUIInfo.CharacterAttribute = GetCharacterAttribute(l_strAttribute);
				if( l_strValue )
					l_sStatueAndUIInfo.fValue = (float)_wtof(l_strValue);
				if( l_strPosiion )
					l_sStatueAndUIInfo.vUIPos = GetVector2(l_strPosiion);
				l_StatueAndUIInfoVector.push_back(l_sStatueAndUIInfo);
				l_pElement = l_pElement->NextSiblingElement();
			}
		}
		return l_StatueAndUIInfoVector;
	}

	sCharacterAttributeAndUIInfo*	GetCharacterAttributeAndUIInfo(std::vector<sCharacterAttributeAndUIInfo>*e_pCharacterAttributeAndUIInfoVector,
		eCharacterAttribute	e_CharacterAttribute,int*e_piReceivedIndex)
	{
		size_t l_uiSize = e_pCharacterAttributeAndUIInfoVector->size();
		for( size_t i=0;i<l_uiSize;++i )
		{
			sCharacterAttributeAndUIInfo*	l_pCharacterAttributeAndUIInfo = &(*e_pCharacterAttributeAndUIInfoVector)[i];
			if(l_pCharacterAttributeAndUIInfo->CharacterAttribute == e_CharacterAttribute)
			{
				if( e_piReceivedIndex )
					*e_piReceivedIndex = (int)i;
				return l_pCharacterAttributeAndUIInfo;
			}
		}
		return 0;
	}

	//<cBattlePowerTargetValue Name="" StartValue="" TargetValue="" />
	cBattlePowerTargetValue::cBattlePowerTargetValue(TiXmlElement*e_pTiXmlElement)
	{
		m_fTargetValue = 1.f;
		m_fValue = 0.f;
		m_pstrDescription = 0;
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("Name")
			{
				this->SetName(l_strValue);
			}
			else
			COMPARE_NAME("TargetValue")
			{
				m_fTargetValue = VALUE_TO_FLOAT;
			}
			else
			COMPARE_NAME("StartValue")
			{
				m_fValue = VALUE_TO_FLOAT;
			}
			else
			COMPARE_NAME("Description")
			{
				SAFE_DELETE(m_pstrDescription);
				m_pstrDescription = new std::wstring;
				*m_pstrDescription = l_strValue;
			}
		PARSE_NAME_VALUE_END
	}

	cBattlePowerTargetValue::cBattlePowerTargetValue(cBattlePowerTargetValue*e_pPowerTargetValue)
	{
		m_pstrDescription = 0;
		m_fValue = e_pPowerTargetValue->m_fValue;
		m_fTargetValue = e_pPowerTargetValue->m_fTargetValue;
		if( e_pPowerTargetValue->m_pstrDescription )
		{
			m_pstrDescription = new std::wstring;
			*m_pstrDescription = *e_pPowerTargetValue->m_pstrDescription;
		}

	}

	cBattlePowerTargetValue::~cBattlePowerTargetValue()
	{
		SAFE_DELETE(m_pstrDescription);
	}

	const wchar_t*	cBattlePowerTargetValue::GetDescription()
	{
		if( m_pstrDescription )
		{
			return m_pstrDescription->c_str();
		}
		return 0;
	}
	void	cBattlePowerTargetValue::AddValue(float e_fValue){ m_fValue += e_fValue;if(m_fValue>m_fTargetValue)m_fValue = m_fTargetValue; }
	bool	cBattlePowerTargetValue::IsPowerFull(){ return m_fTargetValue<=m_fValue?true:false; }
	float	cBattlePowerTargetValue::GetValue(){  return m_fValue; }
	float	cBattlePowerTargetValue::SetValue(){  return m_fValue; }
	float	cBattlePowerTargetValue::GetTargetValue(){  return m_fValue; }
	void	cBattlePowerTargetValue::SetTargetValue(float e_fTargetValue){  m_fTargetValue = e_fTargetValue; }
	float	cBattlePowerTargetValue::CurrentPercent()
	{
		float	l_fGap = m_fTargetValue-m_fValue;
		if( l_fGap > 0.f && m_fTargetValue != 0.f )
		{
			return  l_fGap/m_fTargetValue;
		}
		return 1.f;
	}
	//<cBattlePowerList>
	//	<cBattlePowerTargetValue Name="" StartValue="" TargetValue="" />
	//	<cBattlePowerTargetValue Name="" StartValue="" TargetValue="" />
	//	<cBattlePowerTargetValue Name="" StartValue="" TargetValue="" />
	//</cBattlePowerList>
	cBattlePowerList::cBattlePowerList(TiXmlElement*e_pTiXmlElement)
	{
		ASSERT_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pTiXmlElement,cBattlePowerList::TypeID);
		e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		while( e_pTiXmlElement )
		{
			if( !wcscmp(e_pTiXmlElement->Value(),cBattlePowerTargetValue::TypeID) )
			{
				cBattlePowerTargetValue*l_pPowerTargetValue = new cBattlePowerTargetValue(e_pTiXmlElement);
				bool	l_b = this->AddObject(l_pPowerTargetValue);
				if( l_b )
				{
					UT::ErrorMsg(l_pPowerTargetValue->GetName(),L"name exists");
				}
			}
			e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
		}

	}

	cBattlePowerList::cBattlePowerList(cBattlePowerList*e_pPowerList):cNamedTypedObjectVector<cBattlePowerTargetValue>(e_pPowerList)
	{
	
	}

	cBattlePowerList::~cBattlePowerList()
	{
	
	}

	bool	cBattlePowerList::IsPowerFull(int e_iIndex)
	{
		cBattlePowerTargetValue*l_pPowerTargetValue = this->GetObject(e_iIndex);
		if( l_pPowerTargetValue )
		{
			l_pPowerTargetValue->IsPowerFull();
		}
		return false;
	}
	//
	cFAICharacterStatus::cFAICharacterStatus()
	{
		m_eFAICharacterBehaviorStatus = eFAI_CBS_MAX;
		m_iDexterous = m_iHP = m_iDefence = m_iStrgeth = m_iLucky = m_iAgile = 100;	
		m_pPowerList = 0;
	}
	//<cFAICharacterStatus Name="" HP="" Strehgth="" Defence="" Agile="" Dexterous="" Lucky="">
	//	<BehavioStatusLock Type="Attack" Lock="1" />
	//	<BehavioStatusLock Type="Hurt" Lock="1" />
	//	<cBattlePowerList>
	//		<cBattlePowerTargetValue Name="" StartValue="" TargetValue="" />
	//		<cBattlePowerTargetValue Name="" StartValue="" TargetValue="" />
	//		<cBattlePowerTargetValue Name="" StartValue="" TargetValue="" />
	//	</cBattlePowerList>
	//</cFAICharacterStatus>
    cFAICharacterStatus::cFAICharacterStatus(TiXmlElement*e_pElement)
    {
		memset(m_bFAICharacterBehaviorStatusLock,0,sizeof(bool)*eFAI_CBS_MAX);
		m_pPowerList = 0;
		m_eFAICharacterBehaviorStatus = eFAI_CBS_MAX;
		m_iDexterous = m_iHP = m_iDefence = m_iStrgeth = m_iLucky = m_iAgile = 100;
		ASSERT_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pElement,cFAICharacterStatus::TypeID);
		TiXmlAttribute*l_pTiXmlAttribute = e_pElement->FirstAttribute();
        const wchar_t*l_strName = 0;
        const wchar_t*l_strValue = 0;
        while( l_pTiXmlAttribute )
        {
            l_strValue = l_pTiXmlAttribute->Value();
            l_strName = l_pTiXmlAttribute->Name();
            COMPARE_NAME("Name")
            {
                this->SetName(l_strValue);
            }
            else
            COMPARE_NAME("HP")
            {
                this->m_iHP = VALUE_TO_INT;
            }
            else
            COMPARE_NAME("Strength")
            {
                this->m_iStrgeth = VALUE_TO_INT;
            }
            else
            COMPARE_NAME("Defence")
            {
                this->m_iDefence = VALUE_TO_INT;
            }
            else
            COMPARE_NAME("Agile")
            {
                this->m_iAgile = VALUE_TO_INT;
            }
            else
            COMPARE_NAME("Dexterous")
            {
                this->m_iDexterous = VALUE_TO_INT;
            }
            else
            COMPARE_NAME("Lucky")
            {
                this->m_iLucky = VALUE_TO_INT;
            }                        
            l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
        }
		e_pElement = e_pElement->FirstChildElement();
		while( e_pElement )
		{
			COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pElement,cBattlePowerList::TypeID)
			{
				SAFE_DELETE(m_pPowerList);
				m_pPowerList = new cBattlePowerList(e_pElement);
			}
			else
			COMPARE_TARGET_ELEMENT_VALUE(e_pElement,"BehavioStatusLock")
			{
				eFAICharacterBehaviorStatus	l_eFAICharacterBehaviorStatus = eFAI_CBS_MAX;
				PARSE_ELEMENT_START(e_pElement)
					COMPARE_NAME("Type")
					{
						l_eFAICharacterBehaviorStatus = GetCharacterBehaviodStatus(l_strValue);
					}
					else
					COMPARE_NAME("Lock")
					{
						this->m_bFAICharacterBehaviorStatusLock[l_eFAICharacterBehaviorStatus] = VALUE_TO_BOOLEAN;
					}
				PARSE_NAME_VALUE_END
			}
			e_pElement = e_pElement->NextSiblingElement();
		}
    }

	cFAICharacterStatus::cFAICharacterStatus(cFAICharacterStatus*e_pStatus)
    {
		memcpy(m_bFAICharacterBehaviorStatusLock,e_pStatus->m_bFAICharacterBehaviorStatusLock,sizeof(bool)*eFAI_CBS_MAX);
		m_pPowerList = 0;
		m_eFAICharacterBehaviorStatus = e_pStatus->m_eFAICharacterBehaviorStatus;
        m_iHP = e_pStatus->m_iHP;
        m_iStrgeth = e_pStatus->m_iStrgeth;
        m_iDefence = e_pStatus->m_iDefence;
        m_iAgile = e_pStatus->m_iAgile;
        m_iDexterous = e_pStatus->m_iDexterous;
        m_iLucky = e_pStatus->m_iLucky;
		if(e_pStatus->m_pPowerList)
		{
			m_pPowerList = dynamic_cast<cBattlePowerList*>(e_pStatus->m_pPowerList->Clone());
		}
    }

	cFAICharacterStatus::~cFAICharacterStatus()
	{
		SAFE_DELETE(m_pPowerList);
	}

	bool		cFAICharacterStatus::SetChracterBehaviorStatus(eFAICharacterBehaviorStatus e_eFAICharacterBehaviorStatus,bool e_bForce)
	{
		if( e_bForce || ! this->IsCharacterBehaviorStatusLock() )
		{
			m_eFAICharacterBehaviorStatus = e_eFAICharacterBehaviorStatus;
			return true;
		}
		return false;
	}

	void	cFAICharacterStatus::RenderStatusInfo(int e_iPosX,int e_iPosY)
	{
			//Vector3	l_vPos = this->GetWorldTransform().GetTranslation();
			std::wstring	l_str = UT::ComposeMsgByFormat(L"HP%d\nStrength%d\nDefence%d",m_iHP,m_iStrgeth,m_iDefence);
			cGameApp::RenderFont(e_iPosX,e_iPosY,l_str.c_str());
			//cGameApp::RenderFont(e_iPosX,l_vPos.y-50,l_str.c_str());
			//m_iAgile;
			//m_iDexterous;
			//m_iLucky;
	}
//end nsmespace
}