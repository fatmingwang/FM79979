// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once
// TODO: reference additional headers your program requires here




#include "../Core/AllCoreInclude.h"
#include "../Core//Particle/AllParticleInclude.h"
#include "../Core//FMEvent/AllEventInclude.h"


class MyClass
{
public:
	MyClass(){}
	~MyClass(){}

	virtual	void	Init() = 0;
	virtual	void	Destroy() = 0;
	virtual	void	Update(float e_fElpaseTime) = 0;
	virtual	void	Render() = 0;
    virtual	void	MouseDown(int e_iPosX,int e_iPosY) = 0;
    virtual	void	MouseMove(int e_iPosX,int e_iPosY) = 0;
    virtual	void	MouseUp(int e_iPosX,int e_iPosY) = 0;
};