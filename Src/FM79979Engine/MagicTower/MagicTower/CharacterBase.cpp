#include "stdafx.h"
#include "CharacterBase.h"


cCharacterBase::cCharacterBase(TiXmlElement*e_pTiXmlElement):cFAIAnimationCharacter(e_pTiXmlElement)
{

}
cCharacterBase::~cCharacterBase()
{

}
void	cCharacterBase::RenderStatusInfo()
{
	if( this->m_pAICharacterStatus )
	{
		Vector3	l_vPos = this->GetWorldTransform().GetTranslation();
		std::wstring	l_str = UT::ComposeMsgByFormat(L"HP%d\nStrength%d\nDefence%d",m_pAICharacterStatus->m_iHP,m_pAICharacterStatus->m_iStrgeth,m_pAICharacterStatus->m_iDefence);
		cGameApp::RenderFont(l_vPos.x,l_vPos.y-50,l_str.c_str());
	    //m_iAgile;
	    //m_iDexterous;
        //m_iLucky;
	}
}