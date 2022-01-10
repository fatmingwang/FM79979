#pragma once


void	ComputerShaderInit();
void	ComputerShaderUpdate();
void	ComputerShaderRender();
void	ComputerShaderDestroy();


class cComputeShader:public cBaseShader
{
	f_BindGLDataFunction	m_fBindGLDataFunction;
public:
	cComputeShader();
	virtual ~cComputeShader();
	//
	virtual	void				Use(bool e_bUseLastWVPMatrix = true)override;
	virtual	void				Update(float e_fElpaseTime)override;
};