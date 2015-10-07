#ifndef _MAGIC_TOWER_GAME_PAD_H_
#define _MAGIC_TOWER_GAME_PAD_H_
	
	enum	eVirtualGamePadDirection
	{
		eVGPD_LEFT = 0,
		eVGPD_UP,
		eVGPD_RIGHT,
		eVGPD_DOWN,
		eVGPD_COUNT,
		eVGPD_NONE = 5,

	};

	class	cVirtualGamePad
	{
		eVirtualGamePadDirection	m_eVirtualGamePadDirection;
		Vector4	m_vGamePadButtonSize[eVGPD_COUNT];
	public:
		cVirtualGamePad(Vector4	e_vGamePadButtonSize[eVGPD_COUNT]);
		~cVirtualGamePad();
		void		MouseDown(int e_iPosX,int e_iPosY);
		void		MouseHover(int e_iPosX,int e_iPosY);
		void		MouseUp(int e_iPosX,int e_iPosY);
		eVirtualGamePadDirection	GetPressDirection();
		void		Update(float e_fElpasedTime);
		void		Render();
	};

#endif