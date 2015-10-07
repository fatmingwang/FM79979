#include "stdafx.h"
#include "VirtualGamePad.h"

cVirtualGamePad::cVirtualGamePad(Vector4	e_vGamePadButtonSize[eVGPD_COUNT])
{
	m_eVirtualGamePadDirection = eVGPD_NONE;
	memcpy(m_vGamePadButtonSize,e_vGamePadButtonSize,sizeof(Vector4)*4);
}

cVirtualGamePad::~cVirtualGamePad()
{

}

void		cVirtualGamePad::MouseDown(int e_iPosX,int e_iPosY)
{
	Vector4	l_v;
	for( int i=0;i<eVGPD_COUNT;++i)
	{
		l_v = this->m_vGamePadButtonSize[i];
		if( l_v.x<=e_iPosX&&
			l_v.y<=e_iPosY&&
			l_v.z>=e_iPosX&&
			l_v.w>=e_iPosY)
		{
			m_eVirtualGamePadDirection = eVirtualGamePadDirection(i);
			return;
		}
	}
	m_eVirtualGamePadDirection = eVGPD_NONE;
}

void		cVirtualGamePad::MouseHover(int e_iPosX,int e_iPosY)
{
	MouseDown(e_iPosX,e_iPosY);
}

void		cVirtualGamePad::MouseUp(int e_iPosX,int e_iPosY)
{
	m_eVirtualGamePadDirection = eVGPD_NONE;
}

eVirtualGamePadDirection	cVirtualGamePad::GetPressDirection()
{
	return this->m_eVirtualGamePadDirection;
}