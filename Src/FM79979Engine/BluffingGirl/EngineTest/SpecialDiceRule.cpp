#include "stdafx.h"
#include "SpecialDiceRule.h"
//SpecialValue -1 not working
//<SpecialDiceRule MPDIList="Image/Dice/Dice.mpdi" SpecialValue="0" WorkingSubMPDI="" NoWorkingSubMPDI="" />
cSpecialDiceRule::cSpecialDiceRule(TiXmlElement*e_pElement)
{
	m_iSpecialDicePoint = 0;//-1 for not working
	m_bWorking = true;
	m_pRuleWorking = 0;
	m_pRuleNotWorking = 0;
	cMPDIList*l_pMPDIList = 0;
	cMPDI*l_pMPDI = 0;
	PARSE_ELEMENT_START(e_pElement)
		COMPARE_NAME("MPDIList")
		{
			l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strValue);
		}
		else
		COMPARE_NAME("MPDI")
		{
			if( l_pMPDIList )
				l_pMPDI = l_pMPDIList->GetObject(l_strValue);
		}
		else
		COMPARE_NAME("SpecialValue")
		{
			m_iSpecialDicePoint = VALUE_TO_INT;
			if( m_iSpecialDicePoint == -1 )
				m_bWorking = false;
		}
		else
		COMPARE_NAME("WorkingSubMPDI")
		{
			if( l_pMPDI )
				m_pRuleWorking = l_pMPDI->GetObject(l_strValue);
		}
		else
		COMPARE_NAME("NoWorkingSubMPDI")
		{
			if( l_pMPDI )
				m_pRuleNotWorking = l_pMPDI->GetObject(l_strValue);
		}
	PARSE_NAME_VALUE_END
	if(m_pRuleWorking)
	{
		m_pRuleWorking->Init();
		m_pRuleWorking->Update(0.016f);
	}
	if( m_pRuleNotWorking )
	{
		m_pRuleNotWorking->Init();
		m_pRuleNotWorking->Update(0.016f);
	}
}

cSpecialDiceRule::~cSpecialDiceRule()
{

}

void	cSpecialDiceRule::Render()
{
	if( m_bWorking )
	{
		if( m_pRuleWorking )
			m_pRuleWorking->Render();
	}
	else
	{
		if( m_pRuleNotWorking )
			m_pRuleNotWorking->Render();
	}
}