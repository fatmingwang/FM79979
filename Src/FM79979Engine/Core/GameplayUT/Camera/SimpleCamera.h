#pragma once
#include "../Render/CommonRender/RenderObject.h"
namespace FATMING_CORE
{
	//-----------------------------------------------------------------------------
	// Name: Projection
	// Desc: simple projection class to handle various projection types.
	//-----------------------------------------------------------------------------
	class Projection
	{
	public:
	    
		enum ProjectionType
		{
			Perspective,
			Orthographic,

			ForceDWORD = 0x7fffffff
		};      

		// set projection
		void                SetFovXFovY( float FovX, float FovY, float ZNear, float ZFar );
		void                SetFovYAspect( float FovY, float Aspect, float ZNear, float ZFar );
		void                SetFovXAspect( float FovX, float Aspect, float ZNear, float ZFar );
		void                SetOrthographic( float Width, float Height, float ZNear, float ZFar );

		// get projection data
		cMatrix44           GetMatrix() const;
		float				GetAspect(){ return m_fAspect; }
		sFrustum			GetFrustum() const;
		float               GetFovX() const;
		float               GetFovY() const;
		ProjectionType      GetType() const { return m_Type; }
		float               GetZNear() const { return m_fZNear; }
		float               GetZFar() const { return m_fZFar; }
		float               GetWidth() const { return m_fWidth; }
		float               GetHeight() const { return m_fHeight; }
	private:
		ProjectionType      m_Type;
		float               m_fZNear;
		float               m_fZFar;
		float               m_fWidth;
		float               m_fHeight;
		float				m_fAspect;
	};

	//==========
	//
	//==========
	class cCamera:public NamedTypedObject
	{	
		GET_SET_DEC(Vector3,m_vViewDir,GetDirection,SetDirection);
		GET_SET_DEC(Vector3,m_vRightVector,GetRight,SetRight);
		GET_SET_DEC(Vector3,m_vUpVector,GetUp,SetUp);
		GET_SET_DEC(Vector3,m_vPosition,GetPosition,SetPosition);

		GET_SET_DEC(GLfloat,m_fRotatedX,GetRotatedX,SetRotatedX);
		GET_SET_DEC(GLfloat,m_fRotatedY,GetRotatedY,SetRotatedY);
		GET_SET_DEC(GLfloat,m_fRotatedZ,GetRotatedZ,SetRotatedZ);
	public:
		DEFINE_TYPE_INFO()
		cCamera();				//inits the values (Position: (0|0|0) Target: (0|0|-1) )
		void 				Render ( void );	//executes some glRotates and a glTranslate command
								//Note: You should call glLoadIdentity before using Render
		void 				Move( Vector3 Direction );
		void 				RotateX( GLfloat Angle );
		void 				RotateY( GLfloat Angle );
		void 				RotateZ( GLfloat Angle );

		void 				MoveForward( GLfloat Distance );
		void 				MoveUpward( GLfloat Distance );
		void 				MoveRight( GLfloat Distance );

		void 				LookAtPoint(Vector3 e_vPoint);
	};
	//==========
	//
	//==========
	class	cFrameCamera:public Frame
	{
		Projection			m_Projection;
	public:
		cFrameCamera(int e_iViewWidth = 1920,int e_iViewHeight = 1080,float e_fNearPlane = 1000.0f * 1e-4f,float e_fFarPlane = 20000.f);
		cFrameCamera(cFrameCamera*e_pFrameCamera);
		CLONE_MYSELF(cFrameCamera);
		virtual ~cFrameCamera(){}
		DEFINE_TYPE_INFO()
		// Projection
		const Projection&    GetProjection() const { return m_Projection; }
		void                 SetProjection( const Projection& Proj ) { m_Projection = Proj; }
		// view matrices
		cMatrix44            GetLocalView() const { return GetLocalTransform().Inverted(); }
		cMatrix44            GetWorldView() { return GetWorldTransform().Inverted(); }
		cMatrix44            GetWorldViewProjection() { return m_Projection.GetMatrix()*GetWorldTransform().Inverted(); }
		cMatrix44            GetLocalInvView() const { return GetLocalTransform(); }
		cMatrix44            GetWorldInvView() { return GetWorldTransform(); }
		virtual void		Render()override;
		void				 DisableRender();
	};
}