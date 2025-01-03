#pragma once

#include "MyGui.h"

class cLazyGUIParser :public NamedTypedObject
{
	class cMyGuiRootNode*			m_pMainUIRoot;
public:
	cLazyGUIParser();
	virtual ~cLazyGUIParser();
	bool	ParseUIFile(const char* e_strFileName);
	void	Init();
	void	Update(float e_fElpaseTime);
	void	Render(float*e_pfMatrix = nullptr,float*e_pfGameResolutoinSize = nullptr);
};