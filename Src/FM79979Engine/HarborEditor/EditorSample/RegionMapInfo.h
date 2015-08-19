#ifndef _REGION_HARBOR_INFO_H_
#define _REGION_HARBOR_INFO_H_

#include "MapPointInfo.h"

class cRegionMapPointInfoManager:public cNamedTypedObjectVector<cMapPointInfo>,public cNodeISAX
{
	virtual	bool	MyParse(TiXmlElement*e_pRoot);
	//
	cMapPointInfo*	m_pEditorMouseMoveHarborInfo;
	GET_SET_DEC(cMapPointInfo*,m_pCurrentSelectedHarborInfo,GetCurrentSelectedHarborInfo,SetCurrentSelectedHarborInfo);
	//
	cMapPointInfo*	GetMapPointInfoByPos(int e_iPosX,int e_iPosY);
	POINT			m_MouseDownPos;
	std::vector<Vector2>		m_OriginalMouseDownPos;
public:
	//for debug
	bool	m_bRenderDebugInfo;
	bool	m_bAllHarborMove;
	std::vector<int>			m_SelectedIndices;
	//
	cRegionMapPointInfoManager();
	~cRegionMapPointInfoManager();
	void	DebugRender();
	void	MouseDown(int e_iPosX,int e_iPosY);
	void	MouseMove(int e_iPosX,int e_iPosY);
	void	MouseUp(int e_iPosX,int e_iPosY);

	void	EditorMouseDown(int e_iPosX,int e_iPosY);
	void	EditorMouseMove(int e_iPosX,int e_iPosY,bool e_bClicked);
	void	EditorMouseUp(int e_iPosX,int e_iPosY);
	bool	Export(const char*e_strFileName,bool e_bBinary = false);
};

extern cRegionMapPointInfoManager*g_pRegionMapPointInfoManager;

#endif