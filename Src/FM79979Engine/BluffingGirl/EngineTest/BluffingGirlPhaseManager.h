#ifndef _BLUFFING_GIRL_PHASE_MANAGER_H_
#define _BLUFFING_GIRL_PHASE_MANAGER_H_

class	cBluffingGirlPhaseManager:public cPhaseManager
{
public:
	cBluffingGirlPhaseManager();
	~cBluffingGirlPhaseManager();
	virtual	void	Update(float	e_fElpaseTime);
};

#endif