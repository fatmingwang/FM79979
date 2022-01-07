#pragma once
	
	enum	eVirtualGamePadDirection
	{
		eVGPD_LEFT = 0,
		eVGPD_UP,
		eVGPD_RIGHT,
		eVGPD_DOWN,
		eVGPD_COUNT,
		eVGPD_MAX//IDLE
	};
	//<Gamepad>
	//<cMPDI cMPDIListFileName="" cMPDI>
	// <Data Direction="Up">
	//	<cSubMPDI cSubMPDI="">
	// </Data>
	//</Gamepad>
	class	cVirtualGamePad:public cMPDIToGameObject
	{
		eVirtualGamePadDirection	m_eVirtualGamePadDirection;
	public:
		cVirtualGamePad();
		~cVirtualGamePad();
		eVirtualGamePadDirection	GetPressDirection();
	};