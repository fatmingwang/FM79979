#include "stdafx.h"
#include "RegionMapInfo.h"

cRegionMapPointInfoManager*g_pRegionMapPointInfoManager = nullptr;
cRegionMapPointInfoManager::cRegionMapPointInfoManager()
{
	g_pRegionMapPointInfoManager = this;
	m_pCurrentSelectedHarborInfo = nullptr;
	m_pEditorMouseMoveHarborInfo = nullptr;
	m_bRenderDebugInfo = false;
	m_bAllHarborMove = false;
}

cRegionMapPointInfoManager::~cRegionMapPointInfoManager()
{

}

bool	cRegionMapPointInfoManager::MyParse(TiXmlElement*e_pRoot)
{
	ELEMENT_VALUE_ASSERT_CHECK(e_pRoot,L"HarborInfoManager");
	e_pRoot = e_pRoot->FirstChildElement();
	while( e_pRoot )
	{
		const WCHAR*l_strValue = e_pRoot->Value();
		COMPARE_VALUE("HarborInfo")
		{
			cMapPointInfo*l_pMapPointInfo = new cMapPointInfo(e_pRoot);
			if(!this->AddObject(l_pMapPointInfo))
			{
				UT::ErrorMsg(l_strValue,L"Harbor data error");
			}
		}
		e_pRoot = e_pRoot->NextSiblingElement();
	}
	return true;
}


void	cRegionMapPointInfoManager::MouseDown(int e_iPosX,int e_iPosY)
{
	int	l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		if(this->m_ObjectList[i]->IsCollided(e_iPosX,e_iPosY))
		{
			this->m_pEditorMouseMoveHarborInfo = m_ObjectList[i];
			return;
		}
	}
	m_pEditorMouseMoveHarborInfo = nullptr;
}

void	cRegionMapPointInfoManager::MouseMove(int e_iPosX,int e_iPosY)
{

}

void	cRegionMapPointInfoManager::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_pEditorMouseMoveHarborInfo == nullptr )
		return;
	int	l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		if(this->m_ObjectList[i]->IsCollided(e_iPosX,e_iPosY))
		{
			if(m_pEditorMouseMoveHarborInfo == m_ObjectList[i])
			{
				this->m_pCurrentSelectedHarborInfo = m_ObjectList[i];
				m_pEditorMouseMoveHarborInfo = nullptr;
				return;
			}
		}
	}
	m_pCurrentSelectedHarborInfo = nullptr;
	m_pEditorMouseMoveHarborInfo = nullptr;
}

cMapPointInfo*	cRegionMapPointInfoManager::GetMapPointInfoByPos(int e_iPosX,int e_iPosY)
{
	int	l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		if(this->m_ObjectList[i]->IsCollided(e_iPosX,e_iPosY))
		{
			return m_ObjectList[i];
		}
	}
	return nullptr;
}

void	cRegionMapPointInfoManager::EditorMouseDown(int e_iPosX,int e_iPosY)
{
	m_pCurrentSelectedHarborInfo = GetMapPointInfoByPos(e_iPosX,e_iPosY);
	m_MouseDownPos.x = e_iPosX;
	m_MouseDownPos.y = e_iPosY;
	m_OriginalMouseDownPos.clear();
	if( m_bAllHarborMove )
	{
		int	l_iCount = (int)m_SelectedIndices.size();
		for (int i = 0; i < l_iCount; i++)
		{
			m_OriginalMouseDownPos.push_back(this->GetObject(m_SelectedIndices[i])->GetMapPointData()->vPos);
		}
	}
}

void	cRegionMapPointInfoManager::EditorMouseMove(int e_iPosX,int e_iPosY,bool e_bClicked)
{
	if( m_bAllHarborMove && e_bClicked )
	{
		int	l_iPosXOffset = m_MouseDownPos.x-e_iPosX;
		int	l_iPosYOffset = m_MouseDownPos.y-e_iPosY;
		int	l_iCount = (int)m_SelectedIndices.size();
		for (int i = 0; i < l_iCount; i++)
		{
			Vector2	l_vPos = m_OriginalMouseDownPos[i];
			l_vPos.x -= l_iPosXOffset;
			l_vPos.y -= l_iPosYOffset;
			this->GetObject(m_SelectedIndices[i])->GetMapPointData()->vPos = l_vPos;
		}
		return;
	}
	if( !e_bClicked )
	{
		m_pEditorMouseMoveHarborInfo = GetMapPointInfoByPos(e_iPosX,e_iPosY);
	}
	else
	if( m_pCurrentSelectedHarborInfo )
	{
		m_pCurrentSelectedHarborInfo->GetMapPointData()->vPos = Vector2(e_iPosX,e_iPosY);
	}
}

void	cRegionMapPointInfoManager::EditorMouseUp(int e_iPosX,int e_iPosY)
{
	m_pCurrentSelectedHarborInfo = nullptr;
	m_pEditorMouseMoveHarborInfo = nullptr;
}

void	cRegionMapPointInfoManager::DebugRender()
{
	if( m_pEditorMouseMoveHarborInfo )
		m_pEditorMouseMoveHarborInfo->DebugRender();
	if( m_bRenderDebugInfo )
	{
		LIST_DO_FOR_ALL_CHILDREN(this,DebugRender());
	}
	else
	{
		int l_iCount = Count();
		for( int i=0;i<l_iCount;++i )
		{
			Vector2	l_vPos = this->GetObject(i)->GetMapPointData()->vPos;
			GLRender::RenderPoint(l_vPos,15,Vector4::Green);
		}
	}
}

bool	cRegionMapPointInfoManager::Export(const char*e_strFileName,bool e_bBinary)
{

#ifdef WIN32
	SAFE_DELETE(m_pDoc);
	m_pDoc = new TiXmlDocument();
	TiXmlElement*l_pRoot = new TiXmlElement(L"HarborInfoManager");
	m_pDoc->LinkFirstChild(l_pRoot);
	int	l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		cMapPointInfo*	l_pHarborInfo	= this->m_ObjectList[i];
		TiXmlElement*	l_pHarborInfoElement = l_pHarborInfo->ToTiXmlElement();
		l_pRoot->LinkEndChild(l_pHarborInfoElement);
	}
	return ISAXCallback::Export(e_strFileName,e_bBinary);
#endif
	return false;
}

//DQMSL
//100791806
//ghDzSn7u6kJE