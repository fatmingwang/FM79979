#include "Stdafx.h"
#include "FAICharacterInterface.h"
#include "FAIMultiTaskBehavior.h"
#include "FAITranslationBehavior.h"
#include "FAIAnimationBehavior.h"
#include "FAIAnimationCharacter.h"
#include "FAIParser.h"
#include "FAIFiledPathBehavior.h"
#include "FAILeaveFiledBehavior.h"
#include "FAIPathBehavior.h"
#include "FAIWallReflectMovingBehavior.h"
namespace FATMING_AI
{
	cFAIBehaviorParser*g_pFAIBehaviorParser = 0;

	cFAIBehaviorParser::cFAIBehaviorParser()
	{
	
	}

	cFAIBehaviorParser::~cFAIBehaviorParser()
	{
		m_FAICharacterStatusList.Destroy();
		m_FAICharacterInterfaceList.Destroy();
		m_FAIMachineList.Destroy();	
	}
    //const wchar_t*     cFAIBehaviorParser::TypeID( L"cFAIBehaviorParser" );
	void	cFAIBehaviorParser::ProcessFAIMachine(TiXmlElement*e_pTiXmlElement)
	{
		cFAIMachine*l_pFAIMachine = new cFAIMachine(e_pTiXmlElement);
		bool    l_b = m_FAIMachineList.AddObject(l_pFAIMachine);
		assert(l_b&&"AIMachine is exists!.");
	}

	void	cFAIBehaviorParser::ProcessFAICharacterInterface(TiXmlElement*e_pTiXmlElement)
	{
		std::wstring	l_strCharacterName;
		cFAICharacterInterface*l_pFAICharacterInterface = 0;
		cFAICharacterStatus*l_pFAICharacterStatus = 0;
		float	l_fSpeed = 10.f;
		float	l_fRunAwaySpeed = l_fSpeed;
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("Name")
			{
				l_strCharacterName = l_strValue;
			}
			else
			COMPARE_NAME("Type")
			{
				COMPARE_VALUE_WITH_DEFINE(cFAIAnimationCharacter::TypeID)
				{
					l_pFAICharacterInterface = new cFAIAnimationCharacter(e_pTiXmlElement);
				}
				else
				{
					UT::ErrorMsg(L"unsupport type",l_strValue);
				}
			}
			else
			COMPARE_NAME("cFAICharacterStatus")
			{
				l_pFAICharacterStatus = this->m_FAICharacterStatusList.GetObject(l_strValue);
			}
			else
			COMPARE_NAME("Speed")
			{
				//l_fSpeed = VALUE_TO_FLOAT;
			}
			else
			COMPARE_NAME("RunAwaySpeed")
			{
				//l_fSpeed = VALUE_TO_FLOAT;
			}
		PARSE_NAME_VALUE_END
		if( l_pFAICharacterInterface )
		{
			//l_pFAICharacterInterface->SetSpeed(l_fSpeed);
			l_pFAICharacterInterface->SetName(l_strCharacterName.c_str());
			if(m_FAICharacterInterfaceList.AddObject(l_pFAICharacterInterface))
			{
				if( l_pFAICharacterStatus )
				{
					cFAICharacterStatus*l_pFAICharacterStatusClone = dynamic_cast<cFAICharacterStatus*>(l_pFAICharacterStatus->Clone());
					l_pFAICharacterInterface->SetStatus(l_pFAICharacterStatusClone);
				}
			}
			else
			{
				UT::ErrorMsg(L"name exist",l_strCharacterName.c_str());
			}
		}
	}


	//<Status Name="SmallZombie" HP="100" Strgeth="100" Defence="100" Agile="100" Dexterous="100" Lucky="100" />
	void	cFAIBehaviorParser::ProcessFAICharacterStatus(TiXmlElement*e_pTiXmlElement)
	{
        if( e_pTiXmlElement )
        {
            cFAICharacterStatus*l_pAICharacterStatus = new cFAICharacterStatus(e_pTiXmlElement);
            bool	l_b = m_FAICharacterStatusList.AddObject(l_pAICharacterStatus);
			if( !l_b )
			{
				delete l_pAICharacterStatus;
				UT::ErrorMsg(l_pAICharacterStatus->GetName(),L"Exist!!");
			}
        }
	}

	cFAIMachine*	cFAIBehaviorParser::GetFAIMachineByFileName(const WCHAR*e_strFileName)
	{
		std::string		l_strFileName = UT::WcharToChar(e_strFileName);
		std::wstring	l_strObjectName = UT::GetFileNameWithoutFullPath(e_strFileName);
		cFAIMachine*l_pFAIMachine = this->m_FAIMachineList.GetObject(l_strObjectName.c_str());
		if( !l_pFAIMachine )
		{
			int	l_iOriginalCount = this->m_FAIMachineList.Count();
			bool	l_b = this->ParseWithMyParse(l_strFileName.c_str());
			if( !l_b )
			{
				if(this->m_bShowErrorMsg)
					UT::ErrorMsg(e_strFileName,L"parse failed");
				return 0;
			}
			l_pFAIMachine = this->m_FAIMachineList.GetObject(l_strObjectName.c_str());
			if( !l_pFAIMachine && l_iOriginalCount != this->m_FAIMachineList.Count() )
			{
				UT::ErrorMsg(L"fai file name with AI machine name is not matched",L"Warning!!");
			}
		}
		return l_pFAIMachine;
	}

    cFAIBaseBehave*  cFAIBehaviorParser::ProcessAIBehavior(TiXmlElement*e_pTiXmlElement)
    {
        const WCHAR*l_strName = e_pTiXmlElement->Value();
		cFAIBaseBehave*l_pBaseAIBehave = 0;
        COMPARE_NAME_WITH_DEFINE(cFAIMPDIAnimationChangeAIBehavior::TypeID)
        {
			l_pBaseAIBehave = new cFAIMPDIAnimationChangeAIBehavior(e_pTiXmlElement);
        }
        else
        COMPARE_NAME_WITH_DEFINE(cFAIMultiAITaskBehavior::TypeID)
        {
            l_pBaseAIBehave = new cFAIMultiAITaskBehavior(e_pTiXmlElement);
        }
        else
        COMPARE_NAME_WITH_DEFINE(cFAIWalkToDestinationAIBehavior::TypeID)
        {
            l_pBaseAIBehave = new cFAIWalkToDestinationAIBehavior(e_pTiXmlElement);
        }
        else
        COMPARE_NAME_WITH_DEFINE(cFAIIdleAIBehavior::TypeID)
        {
            l_pBaseAIBehave = new cFAIIdleAIBehavior(e_pTiXmlElement);
        }
        else
        COMPARE_NAME_WITH_DEFINE(cFAIMPDIAnimationMovingAIBehavior::TypeID)
        {
            l_pBaseAIBehave = new cFAIMPDIAnimationMovingAIBehavior(e_pTiXmlElement);
        }
        else
        COMPARE_NAME_WITH_DEFINE(cFAIFiledPathBehavior::TypeID)
        {
            l_pBaseAIBehave = new cFAIFiledPathBehavior(e_pTiXmlElement);
        }
        else
        COMPARE_NAME_WITH_DEFINE(cFAILeaveFiledBehavior::TypeID)
        {
            l_pBaseAIBehave = new cFAILeaveFiledBehavior(e_pTiXmlElement);
        }
		else
        COMPARE_NAME_WITH_DEFINE(cFAIPathBehavior::TypeID)
        {
            l_pBaseAIBehave = new cFAIPathBehavior(e_pTiXmlElement);
        }
		else
        COMPARE_NAME_WITH_DEFINE(cFAIWallReflectMovingBehavior::TypeID)
        {
            l_pBaseAIBehave = new cFAIWallReflectMovingBehavior(e_pTiXmlElement);
        }
		else
		{
			UT::ErrorMsg(l_strName,L"not support this type");
		}
        return l_pBaseAIBehave;
    }

    bool	cFAIBehaviorParser::MyParse(TiXmlElement*e_pRoot)
    {
		g_pFAIBehaviorParser = this;
		e_pRoot = e_pRoot->FirstChildElement();
		while( e_pRoot )
		{
			const WCHAR*l_strName = e_pRoot->Value();
			if( !wcscmp(l_strName,cFAIMachine::TypeID) )
			{
				ProcessFAIMachine(e_pRoot);
			}
			else
			if( !wcscmp(l_strName,cFAICharacterInterface::TypeID) )
			{
				ProcessFAICharacterInterface(e_pRoot);
			}
			else
			if( !wcscmp(l_strName,cFAICharacterStatus::TypeID) )
			{
				ProcessFAICharacterStatus(e_pRoot);
			}
			e_pRoot = e_pRoot->NextSiblingElement();
		}
        return true;
    }
}