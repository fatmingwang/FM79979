#ifndef _FAI_COMBO_ATTACK_BEHAVIOR_H_
#define _FAI_COMBO_ATTACK_BEHAVIOR_H_

struct	sComboAttackData
{
	sComboAttackData(TiXmlElement*e_pTiXmlElement);
	sComboAttackData(sComboAttackData*e_pComboAttackData);
	~sComboAttackData();
	UT::sMinMaxData		ProbabilityToNextCombo;//0-
	UT::sMinMaxData		PowerRange;//
	//the name of this attack
	WCHAR*				strComboName;
	//animation name
	WCHAR*				strAttackAnimationName;
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