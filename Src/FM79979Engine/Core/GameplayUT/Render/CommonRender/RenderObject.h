#pragma once
#include "../../../Common/CommonDefine.h"
#include "../../OpenGL/Glh.h"
#include "../../OpenGL/OpenGLCommon.h"
#include "SimplePrimitive.h"
#include "Frame.h"
//#include "../Common/SmartPointer.h"

#include <functional>
namespace FATMING_CORE
{
	typedef	std::function<void()>								f_RenderFunction;
	typedef	std::function<void(float)>							f_UpdateFunction;

	struct sFVFBase
	{
		int						iNumVertex = 0;
		std::vector<Vector3>	vPosVector;
		std::vector<Vector2>	vUVVector;
		std::vector<Vector4>	vColorVector;
		cTexture*				m_pTexture = nullptr;
		void					AssigeVertexCount(int e_iNumVertex);
		void					AssigeColor(Vector4 e_vColor);
	};

	class	cRenderObject:public virtual Frame//,virtual public cSmartObject
	{
		virtual	void	InternalAssignVertexData() { XMASSERT(0 && "please override me"); }
		cTexture*	InnerTriangulatorRenderDataForBatchRendering(int& e_iOutNumVertex, Vector3* e_pvOutPos, Vector2* e_pvOutUV, Vector4* e_pvOutColor);
		//virtual cTexture*	InnerQuadRenderDataForBatchRendering(int& e_iOutNumVertex, cMatrix44& e_OutMat, Vector3* e_pvOutPos, Vector2* e_pvOutUV, Vector4* e_pvOutColor);
	protected:
		sFVFBase			m_FVFBase;
		//just a rotiton reference
		Vector3				m_vRotation;
		//while set transform is called we might need some data update.ex:rotation
		//if warning is appear override as below
		//virtual	void	SetTransformInternalData(){ cRenderObject::SetTransformInternalData(); }
		void	RenderObjectGoThoughAllFrameFromaFirstToEnd(std::function<void(Frame*)> e_Function,Frame*e_pFrame);
		void	UpdateObjectGoThoughAllFrameFromaFirstToEnd(std::function<void(Frame*)> e_Function, Frame*e_pFrame);
		void	DoRenderObjectGoThoughAllFrameFromaFirstToEndForgetVisible(std::function<void(Frame*)> e_Function,Frame*e_pFrame);
		//void	RenderObjectGoThoughAllFrameFromaEndToFirst(std::function<void(Frame*)> e_Function,Frame*e_pFrame);
		f_RenderFunction	m_RenderFunction;
		f_UpdateFunction	m_UpdateFunction;
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
		virtual				NamedTypedObject*	Clone();
		Vector3				GetPos();
		void				SetPos(Vector3 e_vPos);
		void				SetPos(Vector2 e_vPos);
		//radian
		float				GetAngle();
		//degree
		float				GetDegreeAngle();
		//radian not degree.
		Vector3				GetRotation(){ return m_vRotation; }
		virtual POINT		GetSize(){POINT l_Size = { 0,0 };return l_Size;}
		void				SetAngle(float e_fAngle);
		//because 0 degree is different ont is Y up one us X Right
		void				SetAngleWith180MinusValue(float e_fAngle){ SetAngle(180 - e_fAngle); }
		void				SetAngle(Vector3 e_vAngle);
		virtual	void		Init(){}
		virtual	void		Update(float e_fElpaseTime){ if (m_UpdateFunction)m_UpdateFunction(e_fElpaseTime); }
		virtual	void		Render() { if (m_RenderFunction)m_RenderFunction(); }
		virtual	void		DoRenderCommand(){}
		virtual	void		DebugRender(){}
		//
		virtual	void		UpdateNodes(float e_fElpaseTime);
		virtual	void		RenderNodes();
		virtual	void		InitNodes();
		virtual	void		DebugRenderNodes();
		//
		virtual	void		Destroy(){}
		virtual	void		SetColor(Vector4 e_vColor){ m_CachedWorldTransform._11 = FRAME_DIRTY_WORLD_CACHE; }
		virtual const		cBound*	GenerateBound(){ return nullptr; }
		virtual bool		Collide(int e_iPosX, int e_iPosY);
		void				SetRenderFunction(f_RenderFunction e_f_RenderFunction) { m_RenderFunction = e_f_RenderFunction; }
		void				SetUpdateFunction(f_UpdateFunction e_f_UpdateFunction) { m_UpdateFunction = e_f_UpdateFunction; }
		virtual cTexture*	GetTriangulatorRenderDataForBatchRendering(int& e_iOutNumVertex, Vector3* e_pvOutPos, Vector2* e_pvOutUV, Vector4* e_pvOutColor);
		virtual cTexture*	GetQuadRenderDataForBatchRendering(int& e_iOutNumVertex, cMatrix44& e_OutMat, Vector3* e_pvOutPos, Vector2* e_pvOutUV, Vector4* e_pvOutColor);
		virtual int			GetNumVertexForTwoTriangles() { return TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT; }
		//virtual	bool	IsDone(){ return false; }
		//virtual	int		GetWidth() = 0;
		//virtual	int		GetHeight() = 0;
	};

//end namespace
}