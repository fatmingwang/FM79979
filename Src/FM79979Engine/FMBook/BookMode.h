#ifndef _BOOK_MODE_H_
#define _BOOK_MODE_H_

#include "../Core/AllCoreInclude.h"
#include "../Core/FMEvent/AllEventInclude.h"
#include "stdafx.h"
class	cBookMode:public MyClass
{
	struct	sBookData
	{
		std::wstring	strPageName;
		std::string		strPageFileName;
		Vector2			vResolution;
		sBookData(){ vResolution = Vector2(1920.f,1080.f); }
	};
	std::vector<sBookData>		m_BookDataVector;
	bool						m_bInitOk;
	bool						m_bPickupBooks;
	//
	cScroller*					m_pScroller;
	cEventManager*				m_pEventManager;
	cEventPackageRunner*		m_pEventPackageRunner;
	cTextButton*				m_pBackToPickupBooksButton;
public:
	cBookMode();
	~cBookMode();
	void	Init();
	void	Destroy();
	void	Update(float e_fElpaseTime);
	void	Render();
    void	MouseDown(int e_iPosX,int e_iPosY);
    void	MouseMove(int e_iPosX,int e_iPosY);
    void	MouseUp(int e_iPosX,int e_iPosY);
};


#endif