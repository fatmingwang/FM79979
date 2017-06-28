#ifndef _RENDER_OBJECT_BEHAVIOR_H
#define _RENDER_OBJECT_BEHAVIOR_H

#include "../Math/Frame.h"
//#include "../SmartPointer.h"

#include <functional>
namespace FATMING_CORE
{
	class	cRenderObject:public virtual Frame//,virtual public cSmartObject
	{
		//just a rotiton reference
		Vector3			m_vRotation;
		//while set transform is called we might need some data update.ex:rotation
		//if warning is appear override as below
		//virtual	void	SetTransformInternalData(){ cRenderObject::SetTransformInternalData(); }
		void	RenderObjectGoThoughAllFrameFromaFirstToEnd(std::function<void(Frame*)> e_Function,Frame*e_pFrame);
		void	DoRenderObjectGoThoughAllFrameFromaFirstToEndForgetVisible(std::function<void(Frame*)> e_Function,Frame*e_pFrame);
		//void	RenderObjectGoThoughAllFrameFromaEndToFirst(std::function<void(Frame*)> e_Function,Frame*e_pFrame);
	protected:
		virtual	void	SetTransformInternalData();
		GET_SET_DEC(bool,m_bUpdateRotation,IsUpdateRotation,SetUpdateRotation);
		//local position
		Vector3*		m_pvPos;
		virtual	void	ChildUpdate(float e_fElpaseTime);
	public:
		cRenderObject();
		cRenderObject(cRenderObject*e_pRenderObjectBehavior);
		virtual ~cRenderObject();
		//to avoid warning 4436
		virtual NamedTypedObject*	Clone(){ return new cRenderObject(this); }
		Vector3			GetPos();
		void			SetPos(Vector3 e_vPos);
		void			SetPos(Vector2 e_vPos);
		//radian
		float			GetAngle();
		//degree
		float			GetDegreeAngle();
		//radian not degree.
		Vector3			GetRotation(){ return m_vRotation; }
		void			SetAngle(float e_fAngle);
		void			SetAngle(Vector3 e_vAngle);
		virtual	void	Init(){}
		virtual	void	Update(float e_fElpaseTime){}
		virtual	void	Render(){}
		virtual	void	DebugRender(){}
		//
		virtual	void	UpdateNodes(float e_fElpaseTime);
		virtual	void	RenderNodes();
		virtual	void	InitNodes();
		virtual	void	DebugRenderNodes();
		//
		virtual	void	Destroy(){}
		virtual	void	SetColor(Vector4 e_vColor){}
		virtual const cBound*	GenerateBound(){ return nullptr; }
		//virtual	bool	IsDone(){ return false; }
		//virtual	int		GetWidth() = 0;
		//virtual	int		GetHeight() = 0;
	};

//end namespace
}

#endif