#pragma once

struct	sComboAttackData
{
	sComboAttackData(TiXmlElement*e_pTiXmlElement);
	sComboAttackData(sComboAttackData*e_pComboAttackData);
	~sComboAttackData();
	UT::sMinMaxData		ProbabilityToNextCombo;//0-
	UT::sMinMaxData		PowerRange;//
	//the name of this attack
	wchar_t*			strComboName;
	//animation name
	wchar_t*			strAttackAnimationName;
};

class	cFAIAttackModeAIBehavior
{
	std::vector<sComboAttackData*>	m_ComboAttackDataVector;
public:
	cFAIAttackModeAIBehavior();
	virtual ~cFAIAttackModeAIBehavior();
	void	Update();
	void	Render();
};


//end _FAI_COMBO_ATTACK_BEHAVIOR_H_
#endif