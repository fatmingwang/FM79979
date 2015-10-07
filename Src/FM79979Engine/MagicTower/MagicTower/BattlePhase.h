#ifndef _MAGIC_TOWER_BATTLE_PHASE_H_
#define _MAGIC_TOWER_BATTLE_PHASE_H_

class	cMonsterBase;
class	cMainRoleData;
class	cBattleBase;
class	cGridBehavior;
class	cBattlePhase:public cSimplePhase
{
	cMonsterBase*		m_pMonsterBase;
	cMainRoleData*		m_pMainRoleData;
	cBattleBase*		m_pBattleAttack;
	cBattleBase*		m_pBattleDefence;
	cGridBehavior*		m_pGridBehavior;
	void				ProcessAttack();
public:
	cBattlePhase(cMainRoleData*e_pMainRoleData,cGridBehavior*e_GridBehavior);
	~cBattlePhase();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Init();
	virtual	void	Render();
	virtual	void	DebugRender();
	virtual	void	MouseDown(int e_iX,int e_iY);
	virtual	void	MouseUp(int e_iX,int e_iY);
	virtual	void	MouseMove(int e_iX,int e_iY);
	virtual	void	KeyUP(char e_cKey);
	virtual	const	WCHAR*	GetNextPhaseName();	

};

#endif