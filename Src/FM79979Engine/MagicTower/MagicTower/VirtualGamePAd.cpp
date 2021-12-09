#include "stdafx.h"
#include "VirtualGamePad.h"

cVirtualGamePad::cVirtualGamePad()
{
	m_eVirtualGamePadDirection = eVGPD_MAX;
}

cVirtualGamePad::~cVirtualGamePad()
{

}

eVirtualGamePadDirection	cVirtualGamePad::GetPressDirection()
{
	return this->m_eVirtualGamePadDirection;
}