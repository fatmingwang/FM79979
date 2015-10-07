#ifndef _MAGIC_TOWER_BATTLE_BASE_H_
#define _MAGIC_TOWER_BATTLE_BASE_H_

	enum	eBattleType
	{
		eBT_ATTATK_MOVE_OBJECT = 0,
		eBT_ATTATK_POINT_TOUCH,
		eBT_ATTATK_TOUCH_SEQUENCE,
		eBT_ATTATK_MAX,
		eBT_DEFENCE_1,
		eBT_DEFENCE_2,
		eBT_DEFENCE_3,
		eBT_DEFENCE_MAX,
	};
	//
	//while player click battle object,it will show a curve moving object to main character's body
	//
	//
	struct	s2PositionBecomeARandomCurve
	{
		s2PositionBecomeARandomCurve(cMPDI*e_pPowerMovingToDestinationAnimationMPDI);
		~s2PositionBecomeARandomCurve();
		Vector3	vPos[2];
		cCurveWithTime	Curve;
		//to generate curve
		void	Init();
		void	Update(float e_fElpaseTime);
		void	Render();
		cMPDI*	pPowerMovingToDestinationAnimationMPDI;
	};

	class	cBattleBase:public cClickMouseBehavior
	{
		eBattleType			m_eBattleType;
		int					m_iObjectLV;
	protected:
		float				m_fPower;
		UT::sTimeCounter	m_EnergyRecoveryTime;//while reach battle active or idle
	public:
		cBattleBase(eBattleType	m_eBattleType);
		~cBattleBase();
		static	cBattleBase*	GetBattleBase(eBattleType	e_eBattleType);
		//how this battle perform?
		virtual	float		GetPower(){ return m_fPower; }
		virtual	const		WCHAR*	GetNextPhaseName(){ return 0; }
		virtual void    	Render() = 0;
		virtual void    	DebugRender(){}
		virtual int	    	GetLV(){ return m_iObjectLV; }
	};

#endif