#include "stdafx.h"
#include "FBPhase.h"

cFBPhase::cFBPhase()
{
	this->SetName(FB_PHASE_NAME);
}

cFBPhase::~cFBPhase()
{

}

void	cFBPhase::Destroy()
{

}

void	cFBPhase::Update(float e_fElpaseTime)
{
	this->m_bSatisfiedCondition = true;
}

void	cFBPhase::Init()
{

}
							//it would be called if the stage is changed.
void	cFBPhase::Render()
{

}

void	cFBPhase::DebugRender()
{

}

void	cFBPhase::MouseDown(int e_iX,int e_iY)
{

}

void	cFBPhase::MouseUp(int e_iX,int e_iY)
{

}

void	cFBPhase::MouseMove(int e_iX,int e_iY)
{

}

const   WCHAR*	cFBPhase::GetNextPhaseName()
{
	return MAIN_PHASE_NAME;
}