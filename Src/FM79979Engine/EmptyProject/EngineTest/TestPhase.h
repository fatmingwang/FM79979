#ifndef _TEST_PHASE_H_
#define _TEST_PHASE_H_

class	cGamePhase:public cSimplePhase	
{
public:
	DEFINE_TYPE_INFO();
	cGamePhase();
	~cGamePhase();
	virtual	void	Init();
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
	virtual	void	DebugRender();
	virtual	void	MouseDown(int e_iX,int e_iY);
	virtual	void	MouseUp(int e_iX,int e_iY);
	virtual	void	MouseMove(int e_iX,int e_iY);
	virtual	void	KeyUp(char e_cKey);
	virtual	const	WCHAR*	GetNextPhaseName();	
};


#endif