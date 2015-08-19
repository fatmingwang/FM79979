#pragma once


class cLinerDataModifer
{
	GET_SET_DEC(ePathMode,m_ePathMode,GetPathMode,SetPathMode);
	int		m_iFocusPoint;				//which one point is closest point
	//for selected point moving 
	Vector3	m_vMouseClickedPosition;
public:
	cLinerDataProcessor<Vector3>*	m_pSrcData;
	cLinerDataModifer();
	~cLinerDataModifer();
	void	MouseMove(Vector3 e_vPos,MyMath::AXIS e_Axis = MyMath::W,float*e_pvProjectionMatrix = 0,float*e_pfViewMatrix = 0,int *e_piViewPort = 0);//xyz for orthognal,w for pespective
	void	MouseDown(Vector3 e_vPos,MyMath::AXIS e_Axis = MyMath::W,float*e_pvProjectionMatrix = 0,float*e_pfViewMatrix = 0,int *e_piViewPort = 0);//xyz for orthognal,w for pespective
	void	MouseUp(Vector3 e_vPos,MyMath::AXIS e_Axis = MyMath::W,float*e_pvProjectionMatrix = 0,float*e_pfViewMatrix = 0,int *e_piViewPort = 0);//xyz for orthognal,w for pespective
	//
	bool	m_bAddNewPoint;
	float	m_fAddTime;
	Vector3	m_vAddPos;
	//
	bool	m_bFixPoint;
};


cMPDINode*	GetMPDINodeByTreeViewItem(System::Windows::Controls::TreeView^e_pTreeView,System::Windows::Controls::TreeViewItem^e_pItem,cMPDINode*e_pRoot);