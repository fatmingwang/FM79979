#include "RenderNode.h"

namespace FATMING_CORE
{
	cRenderNode::cRenderNode()
	{
		m_vRotation = Vector3::Zero;
		m_pvPos = Frame::GetLocalPositionPointer();
		m_bUpdateRotation = false;
		m_bAbleToDownThough = true;
	}

	cRenderNode::cRenderNode(cRenderNode*e_pRenderObjectBehavior):Frame(e_pRenderObjectBehavior)
	{
		this->SetName(e_pRenderObjectBehavior->GetName());
		m_vRotation = e_pRenderObjectBehavior->m_vRotation;
		m_bAbleToDownThough = e_pRenderObjectBehavior->m_bAbleToDownThough;
		m_pvPos = this->GetLocalPositionPointer();
		this->SetLocalTransform(e_pRenderObjectBehavior->GetLocalTransform());
		m_bUpdateRotation = e_pRenderObjectBehavior->m_bUpdateRotation;
	}

	cRenderNode::~cRenderNode()
	{
	}

	void	cRenderNode::SetTransformInternalData()
	{
		if( !m_bUpdateRotation )
			return;
		Vector3	l_vScale,l_vTranslation;
		float	l_fInvert;
		this->GetLocalTransform().Decompose(l_vScale,m_vRotation,l_vTranslation,l_fInvert);
	}

	Vector3	cRenderNode::GetPos()
	{
		return this->GetWorldPosition();
	}

	void	cRenderNode::SetPos(Vector2 e_vPos)
	{
		Frame::SetLocalPosition(Vector3(e_vPos.x,e_vPos.y,0.f));
	}

	void	cRenderNode::SetPos(Vector3 e_vPos)
	{
		Frame::SetLocalPosition(e_vPos);
	}

	float	cRenderNode::GetAngle()
	{
		return this->GetRotation().z;
	}

	float	cRenderNode::GetDegreeAngle()
	{
		return D3DXToDegree(GetAngle());
	}

	void	cRenderNode::SetAngle(float e_fAngle)
	{
		m_vRotation.x = e_fAngle;
		SetLocalTransform(cMatrix44::RotationMatrix(Vector3(0.f,0.f,e_fAngle)));
	}

	void	cRenderNode::SetAngle(Vector3 e_vAngle)
	{
		m_vRotation = e_vAngle;
		SetLocalTransform(cMatrix44::RotationMatrix(e_vAngle));
	}


	void	cRenderNode::DoChildUpdate(float e_fElpaseTime)
	{
		cRenderNode*l_pFirstChild = (cRenderNode*)this->GetFirstChild();
		while( l_pFirstChild )
		{
			l_pFirstChild->Update(e_fElpaseTime);
			l_pFirstChild = (cRenderNode*)l_pFirstChild->GetNextSibling();
		}	
	}

	void	cRenderNode::DoChildRender()
	{
		cRenderNode*l_pFirstChild = (cRenderNode*)this->GetFirstChild();
		while( l_pFirstChild )
		{
			l_pFirstChild->Render();
			l_pFirstChild = (cRenderNode*)l_pFirstChild->GetNextSibling();
		}	
	}

	void	cRenderNode::DoChildInit()
	{
		cRenderNode*l_pFirstChild = (cRenderNode*)this->GetFirstChild();
		while( l_pFirstChild )
		{
			l_pFirstChild->Init();
			l_pFirstChild = (cRenderNode*)l_pFirstChild->GetNextSibling();
		}	
	}

	void	cRenderNode::Init()
	{
		if( !m_bAbleToDownThough )
			return;
		InternalInit();
		DoChildInit();
	}

	void	cRenderNode::Update(float e_fElpaseTime)
	{
		InternalUpdate(e_fElpaseTime);
		DoChildUpdate(e_fElpaseTime);
	}

	void	cRenderNode::Render()
	{
		InternalRender();
		DoChildRender();
	}

	void	cRenderNode::Destroy()
	{
		if( !m_bAbleToDownThough )
			return;
		InternalDestroy();
		cRenderNode*l_pFirstChild = (cRenderNode*)this->GetFirstChild();
		while( l_pFirstChild )
		{
			l_pFirstChild->Destroy();
			l_pFirstChild = (cRenderNode*)l_pFirstChild->GetNextSibling();
		}
	}

	void	cRenderNode::DebugRender()
	{
		InternalDebugRender();
		cRenderNode*l_pFirstChild = (cRenderNode*)this->GetFirstChild();
		while( l_pFirstChild )
		{
			l_pFirstChild->DebugRender();
			l_pFirstChild = (cRenderNode*)l_pFirstChild->GetNextSibling();
		}
	}
}