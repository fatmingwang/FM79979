#include "StdAfx.h"
#include "LevelLayerGridData.h"
#include "EventDataNode.h"
namespace FATMING_CORE
{
	cLevelLayerGridData::cLevelLayerGridData()
	{
		m_bSelected = false;
		m_pImage = 0;
		m_pTemplateNode = 0;
		m_pEventDataNode = 0;
	}

	cLevelLayerGridData::cLevelLayerGridData(cLevelLayerGridData*e_pLevelLayerGridData)
	{
		m_pTemplateNode = 0;
		m_bSelected = false;
		m_pImage = 0;
		m_pTemplateNode = e_pLevelLayerGridData->m_pTemplateNode;
		m_pEventDataNode = e_pLevelLayerGridData->m_pEventDataNode;
		SetImage(e_pLevelLayerGridData->GetImage());
		SetName(e_pLevelLayerGridData->GetName());
	}

	cLevelLayerGridData::~cLevelLayerGridData()
	{
		SAFE_DELETE(m_pImage);
	}

	void	cLevelLayerGridData::SetImage(cRenderObject*e_pRenderObjectBehavior)
	{
		SAFE_DELETE(m_pImage);
		if(e_pRenderObjectBehavior)
		{
			m_pImage = dynamic_cast<cRenderObject*>(e_pRenderObjectBehavior->Clone());
			m_pImage->SetOwner(e_pRenderObjectBehavior->GetOwner());
			m_pImage->Init();
		}
	}

	cEventDataNode*cLevelLayerGridData::GetTemplateDataNode()
	{
		return m_pTemplateNode;
	}

	void	cLevelLayerGridData::SetTemplateDataNode(cEventDataNode*e_pEventDataNode)
	{
		m_pTemplateNode = e_pEventDataNode;
	}

	cEventDataNode*	cLevelLayerGridData::GetEventDataNode()
	{
		return m_pEventDataNode;
	}

	void	cLevelLayerGridData::SetEventDataNode(cEventDataNode*e_pEventDataNode)
	{
		m_pEventDataNode = e_pEventDataNode;
	}

	void	cLevelLayerGridData::SetPos(Vector3 e_vPos)
	{
		if( m_pImage )
			this->m_pImage->SetPos(e_vPos);
	}

	Vector3	cLevelLayerGridData::GetPos()
	{
		return m_pImage->GetPos();
	}

	void	cLevelLayerGridData::Init()
	{
		if( m_pImage )
			m_pImage->Init();
	}

	void	cLevelLayerGridData::SetSelected(bool e_bSelected)
	{
		m_bSelected = e_bSelected;
		if( m_bSelected )
			m_pImage->SetColor(Vector4(0.5f,0.5f,0.5f,1));
		else
			m_pImage->SetColor(Vector4::One);
	}

	void	cLevelLayerGridData::Update(float e_fElpaseTime)
	{
		if( m_pImage )
		{
			m_pImage->Update(e_fElpaseTime);
		}
	}

	void	cLevelLayerGridData::Render()
	{
		if( m_pImage )
		{
			//if( m_bRenderByCenter )
			//{
			//	Vector3	l_vPos = m_vPos;
			//	l_vPos.x -= m_pImage->GetPuzzleData()->OriginalSize.x/2.f;
			//	l_vPos.y -= m_pImage->GetPuzzleData()->OriginalSize.y/2.f;
			//	m_pImage->SetPos(l_vPos);
			//	m_pImage->Render();
			//}
			//else
			m_pImage->Update(0.016f);
			m_pImage->Render();
		}
		else
		{
			//render a rectangle here
			Vector3	l_vPos = m_pImage->GetPos();
			POINT	l_Pos = {(int)l_vPos.x,(int)l_vPos.y};
			GLRender::RenderRectangle(l_Pos,50,50,Vector4::One);
		}
	}

	void	cLevelLayerGridData::DebugRender(float e_fRandomColor)
	{
		int	l_iWidth = GetWidth();
		int	l_iHeight = GetHeight();
		Vector3	l_vPos = m_pImage->GetPos();
		RenderRectangle(Vector2(l_vPos.x,l_vPos.y),(float)l_iWidth,(float)l_iHeight,Vector4::One*e_fRandomColor+Vector4(0.5,0,0,0),0);	
	}

	bool	cLevelLayerGridData::IsCollide(Vector3 e_vPos)
	{		
		return IsCollide(Vector2(e_vPos.x,e_vPos.y));
	}

	bool	cLevelLayerGridData::IsCollide(Vector2 e_vPos)
	{
		Vector3	l_vPos = m_pImage->GetPos();
		if( e_vPos.x>=l_vPos.x&&e_vPos.y>=l_vPos.y&&
			e_vPos.x<=(l_vPos.x+GetWidth())&&
			e_vPos.y<=(l_vPos.y+GetHeight())
			)
		{
			return true;
		}
		return false;
	}


	int	cLevelLayerGridData::GetWidth()
	{
		if( this->m_pImage->Type() == cBaseImage::TypeID ||
			m_pImage->Type() == cPuzzleImageUnit::TypeID)
		{
			cBaseImage*l_pBaseImage = (cBaseImage*)m_pImage;
			{
				return l_pBaseImage->GetWidth();
			}
		}
		else
		if( this->m_pImage->Type() == cMPDI::TypeID )
		{
			cMPDI*l_pMPDI = dynamic_cast<cMPDI*>(m_pImage);
			return (int)l_pMPDI->GetDrawSize().x;
		}
		else
		{
			assert(0);
		}
		return 0;
	}

	int	cLevelLayerGridData::GetHeight()
	{
		if( this->m_pImage->Type() == cBaseImage::TypeID ||
			m_pImage->Type() == cPuzzleImageUnit::TypeID)
		{
			cBaseImage*l_pBaseImage = (cBaseImage*)m_pImage;
			{
				return l_pBaseImage->GetHeight();
			}
		}
		else
		if( this->m_pImage->Type() == cMPDI::TypeID )
		{
			cMPDI*l_pMPDI = dynamic_cast<cMPDI*>(m_pImage);
			return (int)l_pMPDI->GetDrawSize().y;
		}
		else
		{
			assert(0);
		}
		return 0;	
	}
	//if two data is same return true
	bool	cLevelLayerGridData::DataCompare(cLevelLayerGridData*e_pMap_Cell_ObjectData)
	{
		if(	e_pMap_Cell_ObjectData->GetImage() == m_pImage &&
			e_pMap_Cell_ObjectData->GetPos() == m_pImage->GetPos() &&
			e_pMap_Cell_ObjectData->IsSameName(GetName()) )
			return true;
		return false;
	}
//end namespace
}