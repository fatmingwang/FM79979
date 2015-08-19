#include "../stdafx.h"

#include "RenderObject.h"

namespace FATMING_CORE
{
	cRenderObject::cRenderObject()
	{
		m_vRotation = Vector3::Zero;
		m_pvPos = Frame::GetLocalPositionPointer();
		m_bUpdateRotation = false;
	}

	cRenderObject::cRenderObject(cRenderObject*e_pRenderObjectBehavior):Frame(e_pRenderObjectBehavior)
	{
		this->SetName(e_pRenderObjectBehavior->GetName());
		m_vRotation = e_pRenderObjectBehavior->m_vRotation;
		m_pvPos = this->GetLocalPositionPointer();
		this->SetLocalTransform(e_pRenderObjectBehavior->GetLocalTransform());
		m_bUpdateRotation = e_pRenderObjectBehavior->m_bUpdateRotation;
	}

	cRenderObject::~cRenderObject()
	{
	}

	void	cRenderObject::SetTransformInternalData()
	{
		if( !m_bUpdateRotation )
			return;
		Vector3	l_vScale,l_vTranslation;
		float	l_fInvert;
		this->GetLocalTransform().Decompose(l_vScale,m_vRotation,l_vTranslation,l_fInvert);
		//m_pvRotation->x = D3DXToDegree(m_pvRotation->x);
		//m_pvRotation->y = D3DXToDegree(m_pvRotation->y);
		//m_pvRotation->z = D3DXToDegree(m_pvRotation->z);
	}

	Vector3	cRenderObject::GetPos()
	{
		return this->GetWorldPosition();
	}

	void	cRenderObject::ChildUpdate(float e_fElpaseTime)
	{
		Frame*l_pChild = this->GetFirstChild();
		while( l_pChild  )
		{
			cRenderObject*l_p = dynamic_cast<cRenderObject*>(l_pChild);
			l_p->Update(e_fElpaseTime);
			l_p->ChildUpdate(e_fElpaseTime);
			l_p = dynamic_cast<cRenderObject*>(l_p->GetNextSibling());
		}
	}

	void	cRenderObject::SetPos(Vector2 e_vPos)
	{
		Frame::SetLocalPosition(Vector3(e_vPos.x,e_vPos.y,0.f));
	}

	void	cRenderObject::SetPos(Vector3 e_vPos)
	{
		Frame::SetLocalPosition(e_vPos);
	}

	float	cRenderObject::GetAngle()
	{
		return this->GetRotation().z;
	}

	float	cRenderObject::GetDegreeAngle()
	{
		return D3DXToDegree(GetAngle());
	}

	void	cRenderObject::SetAngle(float e_fAngle)
	{
		m_vRotation.x = e_fAngle;
		SetLocalTransform(cMatrix44::RotationMatrix(Vector3(0.f,0.f,e_fAngle)));
	}

	void	cRenderObject::SetAngle(Vector3 e_vAngle)
	{
		m_vRotation = e_vAngle;
		SetLocalTransform(cMatrix44::RotationMatrix(e_vAngle));
	}


	void	cRenderObject::ForAllNodesUpdate(float e_fElpaseTime)
	{
		cRenderObject*l_pNextSibling = dynamic_cast<cRenderObject*>(this->GetNextSibling());
		while( l_pNextSibling )
		{
			l_pNextSibling->Update(e_fElpaseTime);
			l_pNextSibling = dynamic_cast<cRenderObject*>(l_pNextSibling->GetNextSibling());
		}
		cRenderObject*l_pFirstChild = dynamic_cast<cRenderObject*>(this->GetFirstChild());	
		if(l_pFirstChild)
			l_pFirstChild->ForAllNodesUpdate(e_fElpaseTime);
	}

	void	cRenderObject::ForAllNodesRender()
	{
		cRenderObject*l_pNextSibling = dynamic_cast<cRenderObject*>(this->GetNextSibling());
		while( l_pNextSibling )
		{
			l_pNextSibling->Render();
			l_pNextSibling = dynamic_cast<cRenderObject*>(l_pNextSibling->GetNextSibling());
		}
		cRenderObject*l_pFirstChild = dynamic_cast<cRenderObject*>(this->GetFirstChild());	
		if(l_pFirstChild)
			l_pFirstChild->ForAllNodesRender();	
	}

	void	cRenderObject::ForAllNodesDebugRender()
	{
		DebugRender();
		cRenderObject*l_pNextSibling = dynamic_cast<cRenderObject*>(this->GetNextSibling());
		while( l_pNextSibling )
		{
			l_pNextSibling->DebugRender();
			l_pNextSibling = dynamic_cast<cRenderObject*>(l_pNextSibling->GetNextSibling());
		}
		cRenderObject*l_pFirstChild = dynamic_cast<cRenderObject*>(this->GetFirstChild());	
		if(l_pFirstChild)
			l_pFirstChild->ForAllNodesDebugRender();
	}

}