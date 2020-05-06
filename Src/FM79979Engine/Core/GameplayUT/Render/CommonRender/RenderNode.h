#pragma once

#include "Frame.h"
#include <functional>
namespace FATMING_CORE
{
	//!!!!!!!!!!!!!!!!
	//don't use this one...
	//!!!!!!!!!!!!!!!!
	class	cRenderNode:public Frame
	{
		//just a rotiton reference
		//Vector3			m_vRotation;
		//while set transform is called we might need some data update.ex:rotation
		//if warning is appear override as below
		//virtual	void	SetTransformInternalData(){ cRenderNode::SetTransformInternalData(); }
	protected:
		virtual	void	SetTransformInternalData();
		//GET_SET_DEC(bool,m_bUpdateRotation,IsUpdateRotation,SetUpdateRotation);
		//if the object dont expect a reinit or destroy,the sequence root will be true to avoid unexpect init or destroy.
		GET_SET_DEC(bool,m_bAbleToDownThough,IsAbleToDownThough,SetAbleToDownThough);
		//local position
		//Vector3*		m_pvPos;
	    //the start condition for inherit class to implement
	    virtual	void	InternalInit() = 0;
	    //if m_bAnimationDone is false and start condition is actived,it will be actived.
	    virtual	void	InternalUpdate(float e_fElpaseTime) = 0;
	    //if m_bAnimationDone is false and start condition is actived,it will be actived.
	    virtual	void	InternalRender() = 0;
		virtual	void	InternalDebugRender(){}
		virtual	void	InternalDestroy(){}
		//not right,but I am lazy to fix
		void			DoChildUpdate(float e_fElpaseTime);
		//not right,but I am lazy to fix
		void			DoChildRender();
		void			DoChildInit();
	public:

		cRenderNode();
		cRenderNode(cRenderNode*e_pRenderObjectBehavior);
		virtual ~cRenderNode();
		Vector3			GetPos();
		void			SetPos(Vector3 e_vPos);
		void			SetPos(Vector2 e_vPos);
		//radian,a 2 d interface.
		float			GetAngle();
		//degree
		float			GetDegreeAngle();
		//radian not degree.
		//Vector3			GetRotation(){ return m_vRotation; }
		void			SetAngle(float e_fAngle);
		void			SetAngle(Vector3 e_vAngle);
		virtual	void	Init();
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	Render();
		virtual	void	Destroy();
		virtual	void	DebugRender();		//
		virtual	void	SetColor(Vector4 e_vColor){}
		//virtual	bool	IsDone(){ return false; }
		//virtual	int		GetWidth() = 0;
		//virtual	int		GetHeight() = 0;
	};

//end namespace
}