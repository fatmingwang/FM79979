#ifndef _EDITOR_MODE_H_
#define _EDITOR_MODE_H_

#include "../Core/AllCoreInclude.h"
#include "../Core/FMEvent/AllEventInclude.h"

#include "stdafx.h"
namespace   FMBOOK
{

	class	cFMBOOKManager;
	class	cFMBookPageDataList;
	class	cFMBookReader;
}

struct	sFMBookEditorData
{
	static cEventObjectInstance*					m_spEventObjectInstance;
	static cEventInstance*							m_spEventInstance;
	//for editor current object
	static cEventPackage*							m_spEventPackage;
	static	void									AddEventPackage(cEventPackage*e_pEventPackage);
	static	void									DelEventPackage(const wchar_t*e_strName);
	static	void									SaveFile(const char*e_strName);
	static	bool									OpenFile(const char*e_strName);
	sFMBookEditorData();
	~sFMBookEditorData();
	static	void									SetupResource();
	//while file open call this
	static	void									DestroyResource();
	static	void									DataLegalCheck();
};

class	cEditorMode:public MyClass
{
public:
	static cEventManager*							m_spEventManager;
	static cEventPackageRunner*						m_spEventPackageRunner;
	cEditorMode();
	~cEditorMode();
	void	Init();
	void	Destroy();
	void	Update(float e_fElpaseTime);
	void	Render();
    void	MouseDown(int e_iPosX,int e_iPosY);
    void	MouseMove(int e_iPosX,int e_iPosY);
    void	MouseUp(int e_iPosX,int e_iPosY);
};


#endif