#ifndef _TEST_PHASE_H_
#define _TEST_PHASE_H_

class	cGamePhase:public cSimplePhase	
{
public:
	DEFINE_TYPE_INFO()
	cGamePhase();
	~cGamePhase();
	virtual	void	Init()override;
	virtual	void	Update(float e_fElpaseTime)override;
	virtual	void	Render()override;
	virtual	void	DebugRender()override;
	virtual	void	MouseDown(int e_iX,int e_iY)override;
	virtual	void	MouseUp(int e_iX,int e_iY)override;
	virtual	void	MouseMove(int e_iX,int e_iY)override;
	virtual	void	KeyUp(char e_cKey)override;
	virtual	const	WCHAR*	GetNextPhaseName()override;
};


#endif