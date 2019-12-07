#include "stdafx.h"
#include "LinerDataModifer.h"

//template <class T>
cLinerDataModifer::cLinerDataModifer()
{
	m_ePathMode	= ePM_ADD_POINT;
	m_pSrcData = nullptr;
	m_iFocusPoint = -1;
	m_bAddNewPoint = false;
	m_bFixPoint = false;
}

//template <class T>
cLinerDataModifer::~cLinerDataModifer()
{

}


void	cLinerDataModifer::MouseDown(Vector3 e_vPos,MyMath::AXIS e_Axis,float*e_pvProjectionMatrix,float*e_pfViewMatrix,int *e_piViewPort)
{
	m_bAddNewPoint = false;
	e_vPos = SwapPosAsAxisView(e_vPos,e_Axis);
	if( m_ePathMode == ePM_FIX )
	{
		std::vector<Vector3>* l_pvPointList = m_pSrcData->GetLinerDataVector();
		this->m_iFocusPoint = -1;
		Vector3	l_vMousePos = e_vPos;
		float	l_fClosetDistance = 20;
		int	l_iLength = (int)l_pvPointList->size();
		for( int i=0;i<l_iLength;++i )
		{
			Vector3 l_vPos = (*l_pvPointList)[i];
			l_vPos = SwapPosAsAxisView(l_vPos,e_Axis);

			if( e_Axis == MyMath::W&&e_pvProjectionMatrix )//try screen space
			{
				Vector3	l_vp1 = WorldToScreen(l_vPos.x,l_vPos.y,l_vPos.z,e_pvProjectionMatrix,e_pfViewMatrix,e_piViewPort);
				l_vp1.z = 0.f;
				Vector3	l_vp2 = e_vPos;
				float	l_fDis = (l_vp1-l_vp2).Length();
				if( l_fClosetDistance>l_fDis )
				{
					l_fClosetDistance = l_fDis;
					m_iFocusPoint = i;
				}
			}
			else
			{
				Vector3	l_vDis = l_vPos-l_vMousePos;
				float	l_fDis = l_vDis.Length();
				if( l_fClosetDistance>l_fDis )
				{
					l_fClosetDistance = l_fDis;
					m_iFocusPoint = i;
				}
			}
		}
		if( m_iFocusPoint != -1 )
			m_ePathMode = ePM_FIX_WITH_MOUSE_DRAG;
	}
	else
	if( m_ePathMode == ePM_PATH_MOVING_ALL )
	{
		m_ePathMode = ePM_ALL_PATH_WITH_MOUSE_DRAG;
	}
	m_vMouseClickedPosition = e_vPos;
}

void	cLinerDataModifer::MouseMove(Vector3 e_vPos,MyMath::AXIS e_Axis,float*e_pvProjectionMatrix,float*e_pfViewMatrix,int *e_piViewPort)
{
	m_bAddNewPoint = false;
	m_bFixPoint = false;
	if( m_pSrcData == nullptr )
		return;
	e_vPos = SwapPosAsAxisView(e_vPos,e_Axis);
	std::vector<Vector3>* l_pvPointList = m_pSrcData->GetLinerDataVector();
	if( m_ePathMode == ePM_FIX_WITH_MOUSE_DRAG )
	{
		//if( m_iFocusPoint !=-1 )
		{
			Vector3	l_vTargetPos = RetainPosAxisAsAxisView((*l_pvPointList)[m_iFocusPoint],e_vPos,e_Axis);
			m_pSrcData->ChangeData(this->m_iFocusPoint,l_vTargetPos);
			m_fAddTime = *m_pSrcData->GetTime(m_iFocusPoint);
			m_vAddPos = l_vTargetPos;
			m_bFixPoint = true;
		}
	}
	else
	if( m_ePathMode == ePM_ALL_PATH_WITH_MOUSE_DRAG )
	{
		int	l_iNum = (int)m_pSrcData->Count();
		for( int i=0;i<l_iNum;++i )
		{
			Vector3	*l_vPos = m_pSrcData->GetData(i);
			l_vPos->x += (e_vPos.x-m_vMouseClickedPosition.x);
			l_vPos->y += (e_vPos.y-m_vMouseClickedPosition.y);
		}
		m_vMouseClickedPosition = e_vPos;
		m_pSrcData->DoLOD();
	}
}

void	cLinerDataModifer::MouseUp(Vector3 e_vPos,MyMath::AXIS e_Axis,float*e_pvProjectionMatrix,float*e_pfViewMatrix,int *e_piViewPort)
{
	m_bAddNewPoint = false;
	m_bFixPoint = false;
	if( m_ePathMode == ePM_FIX_WITH_MOUSE_DRAG)
		m_ePathMode = ePM_FIX ;
	else
	if( m_ePathMode == ePM_PATH_WITH_MOUSE_DRAG)
		m_ePathMode = ePM_PATH_MOVING;
	else
	if( m_ePathMode == ePM_ALL_PATH_WITH_MOUSE_DRAG)
		m_ePathMode = ePM_PATH_MOVING_ALL;
	else
	if( m_ePathMode == ePM_PATH_ROTATION_DRAG )
		m_ePathMode = ePM_PATH_ROTATION;
	else
	if( m_ePathMode == ePM_ALL_PATH_ROTATION_DRAG )
		m_ePathMode = ePM_ALL_PATH_ROTATION;
	else
	if( m_ePathMode == ePM_ADD_POINT )
	{
		float	l_fTime = 0.f;
		int	l_iCount = m_pSrcData->Count();
		if( l_iCount > 0 )
		{
			l_fTime = *m_pSrcData->GetTime(l_iCount-1)+0.1f;
		}
		e_vPos = VectorToInteger(e_vPos);
		m_pSrcData->AddData(e_vPos,l_fTime);
		m_bAddNewPoint = true;
		this->m_fAddTime = l_fTime;
		this->m_vAddPos = e_vPos;
	}
}

//template class	cLinerDataModifer<Vector2>;
//template class	cLinerDataModifer<Vector3>;
//template class	cLinerDataModifer<Vector4>;


cMPDINode*	GetMPDINodeByTreeViewItem(System::Windows::Controls::TreeView^e_pTreeView,System::Windows::Controls::TreeViewItem^e_pItem,cMPDINode*e_pRoot)
{
	cMPDINode*l_pMPDINode = e_pRoot;
	std::vector<int>	l_Path;
	while( e_pItem )
	{
		int	l_iPath = -1;
		System::Windows::Controls::TreeViewItem^l_pParent = dynamic_cast<System::Windows::Controls::TreeViewItem^>(e_pItem->Parent);
		if( l_pParent != nullptr )
		{
			l_iPath = l_pParent->Items->IndexOf(e_pItem);
			l_Path.push_back(l_iPath);
		}
		if( l_pParent )
			e_pItem = (System::Windows::Controls::TreeViewItem^)e_pItem->Parent;
		else
			e_pItem = nullptr;
	}
	if( l_Path.size() == 0 )
	{
		return e_pRoot;
	}
	while( l_Path.size() )
	{
		int	l_iIndex = l_Path[l_Path.size()-1];
		l_pMPDINode = dynamic_cast<cMPDINode*>(l_pMPDINode->GetFirstChild());
		while( l_iIndex )
		{
			l_pMPDINode = dynamic_cast<cMPDINode*>(l_pMPDINode->GetNextSibling());
			--l_iIndex;
		}
		l_Path.erase(l_Path.begin()+l_Path.size()-1);
	}
	return l_pMPDINode;

	//cMPDINode*l_pMPDINode = e_pRoot;
	//std::vector<int>	l_Path;
	//while( e_pItem )
	//{
	//	int	l_iPath = -1;
	//	System::Windows::Controls::TreeViewItem^l_pParent = dynamic_cast<System::Windows::Controls::TreeViewItem^>(e_pItem->Parent);
	//	if( l_pParent != nullptr )
	//	{
	//		l_iPath = l_pParent->Items->IndexOf(e_pItem);
	//		l_Path.push_back(l_iPath);
	//	}
	//	if( l_pParent )
	//		e_pItem = (System::Windows::Controls::TreeViewItem^)e_pItem->Parent;
	//	else
	//		e_pItem = nullptr;
	//}
	//if( l_Path.size() == 0 )
	//{
	//	return e_pRoot;
	//}
	//while( l_Path.size() )
	//{
	//	int	l_iIndex = l_pMPDINode->GetNextLevelChildrenCount()-l_Path[l_Path.size()-1]-1;
	//	l_pMPDINode = dynamic_cast<cMPDINode*>(l_pMPDINode->GetFirstChild());
	//	while( l_iIndex )
	//	{
	//		l_pMPDINode = dynamic_cast<cMPDINode*>(l_pMPDINode->GetNextSibling());
	//		--l_iIndex;
	//	}
	//	l_Path.erase(l_Path.begin()+l_Path.size()-1);
	//	OutputDebugString(l_pMPDINode->GetName());
	//	OutputDebugString(L"\n");
	//}
	//return l_pMPDINode;
}