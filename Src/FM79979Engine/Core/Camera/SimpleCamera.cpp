#include "../stdafx.h"
#include "SimpleCamera.h"
#include "../GLSL/Shader.h"
#include "../GameplayUT/GameApp.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cFrameCamera);
	TYPDE_DEFINE_MARCO(cCamera);

	#define XMASSERT_LINE_STRING_SIZE 16

	void XMAssert
	(
		const char* pExpression,
		const char* pFileName,
		UINT        LineNumber
	)
	{
	#ifdef WIN32
	#ifdef DEBUG
		char        aLineString[XMASSERT_LINE_STRING_SIZE];
		char*       pLineString;
		UINT        Line;

		aLineString[XMASSERT_LINE_STRING_SIZE - 2] = '0';
		aLineString[XMASSERT_LINE_STRING_SIZE - 1] = '\0';
		for (Line = LineNumber, pLineString = aLineString + XMASSERT_LINE_STRING_SIZE - 2;
			 Line != 0 && pLineString >= aLineString;
			 Line /= 10, pLineString--)
		{
			*pLineString = (CHAR)('0' + (Line % 10));
		}

	#ifndef NO_OUTPUT_DEBUG_STRING
		cGameApp::OutputDebugInfoString("Assertion failed: ");
		cGameApp::OutputDebugInfoString(pExpression);
		cGameApp::OutputDebugInfoString(", file ");
		cGameApp::OutputDebugInfoString(pFileName);
		cGameApp::OutputDebugInfoString(", line ");
		cGameApp::OutputDebugInfoString(pLineString + 1);
		cGameApp::OutputDebugInfoString("\r\n");
	#else
		DbgPrint("Assertion failed: %s, file %s, line %d\r\n", pExpression, pFileName, LineNumber);
	#endif

		__debugbreak();
	#endif
	#endif
	}


	#ifdef WIN32
	#ifdef DEBUG
	#define XMASSERT(Expression) ((void)(Expression || (XMAssert(#Expression, __FILE__, __LINE__), 0)))
	#else // !DEBUG
	#define XMASSERT(Expression) ((void)0)
	#endif // !DEBUG
	#else
	#define XMASSERT(Expression)  ((void)0)
	#endif

	bool XMScalarNearEqual
	(
		float S1,
		float S2,
		float Epsilon
	)
	{
		float Delta = S1 - S2;
		UINT  AbsDelta = *(UINT*)&Delta & 0x7FFFFFFF;
		return (*(FLOAT*)&AbsDelta <= Epsilon);
	}

	cMatrix44 cMatrix44PerspectiveRH
	(
		FLOAT ViewWidth, 
		FLOAT ViewHeight, 
		FLOAT NearZ, 
		FLOAT FarZ
	)
	{
		cMatrix44 M;
		float    TwoNearZ;

		XMASSERT(!XMScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
		XMASSERT(!XMScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
		XMASSERT(!XMScalarNearEqual(FarZ, NearZ, 0.00001f));

		TwoNearZ = NearZ + NearZ;

		M.r[0] = VECTOR4Set(TwoNearZ / ViewWidth, 0.0f, 0.0f, 0.0f);
		//y up should change to z up
		M.r[1] = VECTOR4Set(0.0f, TwoNearZ / ViewHeight, 0.0f, 0.0f);
		M.r[2] = VECTOR4Set(0.0f, 0.0f, FarZ / (NearZ - FarZ), -1.0f);
		M.r[3] = VECTOR4Set(0.0f, 0.0f, M.r[2].v[2] * NearZ, 0.0f);

		return M;
	}

	cMatrix44 cMatrix44PerspectiveLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
	{

		cMatrix44 M;
		float    TwoNearZ;

		XMASSERT(!XMScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
		XMASSERT(!XMScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
		XMASSERT(!XMScalarNearEqual(FarZ, NearZ, 0.00001f));

		TwoNearZ = NearZ + NearZ;

		M.r[0] = VECTOR4Set(TwoNearZ / ViewWidth, 0.0f, 0.0f, 0.0f);
		//y up should change to z up
		M.r[1] = VECTOR4Set(0.0f, TwoNearZ / ViewHeight, 0.0f, 0.0f);
		M.r[2] = VECTOR4Set(0.0f, 0.0f, FarZ / (FarZ - NearZ), 1.0f);
		M.r[3] = VECTOR4Set(0.0f, 0.0f, -M.r[2].v[2] * NearZ, 0.0f);

		return M; 
	}

	cMatrix44 cMatrix44OrthographicLH
	(
		float ViewWidth, 
		float ViewHeight, 
		float NearZ, 
		float FarZ
	)
	{
		cMatrix44 M;

		XMASSERT(!XMScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
		XMASSERT(!XMScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
		XMASSERT(!XMScalarNearEqual(FarZ, NearZ, 0.00001f));

		M.r[0] = VECTOR4Set(2.0f / ViewWidth, 0.0f, 0.0f, 0.0f);
		//y up should change to z up
		M.r[1] = VECTOR4Set(0.0f, 2.0f / ViewHeight, 0.0f, 0.0f);
		M.r[2] = VECTOR4Set(0.0f, 0.0f, 1.0f / (FarZ - NearZ), 0.0f);
		M.r[3] = VECTOR4Set(0.0f, 0.0f, -M.r[2].v[2] * NearZ, 1.0f);

		return M;
	}
	cMatrix44 cMatrix44OrthographicRH
	(
		float ViewWidth, 
		float ViewHeight, 
		float NearZ, 
		float FarZ
	)
	{
		cMatrix44 M;

		XMASSERT(!XMScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
		XMASSERT(!XMScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
		XMASSERT(!XMScalarNearEqual(FarZ, NearZ, 0.00001f));

		M.r[0] = VECTOR4Set(2.0f / ViewWidth, 0.0f, 0.0f, 0.0f);
		M.r[1] = VECTOR4Set(0.0f, 2.0f / ViewHeight, 0.0f, 0.0f);
		M.r[2] = VECTOR4Set(0.0f, 0.0f, 1.0f / (NearZ - FarZ), 0.0f);
		M.r[3] = VECTOR4Set(0.0f, 0.0f, M.r[2].v[2] * NearZ, 1.0f);

		return M;
	}
	//-----------------------------------------------------------------------------
	// Name: Projection::GetMatrix
	//-----------------------------------------------------------------------------
	cMatrix44 Projection::GetMatrix() const
	{
		cMatrix44 Proj;
		if( m_Type == Projection::Perspective )
		{
			Proj = cMatrix44PerspectiveRH( m_fWidth, m_fHeight, m_fZNear, m_fZFar );
		}
		else if( m_Type == Projection::Orthographic )
		{
			glhOrthof2(Proj,-m_fWidth/2,m_fWidth/2,m_fHeight/2,-m_fHeight/2,m_fZNear, m_fZFar);
			//Proj = cMatrix44OrthographicRH( m_fWidth, m_fHeight, m_fZNear, m_fZFar );
		}
		else
			assert( 0 );

		return Proj;
	}

	sFrustum Projection::GetFrustum() const
	{
		sFrustum Temp;
		cMatrix44 Proj = GetMatrix();
		ComputeFrustumFromProjection( &Temp, &Proj );
		return Temp;
	}

	//-----------------------------------------------------------------------------
	// Name: Projection::SetFovXFovY
	//-----------------------------------------------------------------------------
	void Projection::SetFovXFovY( float FovX, float FovY, float ZNear, float ZFar )
	{
		m_fZNear = ZNear;
		m_fZFar = ZFar;
		m_Type = Projection::Perspective;
	    
		m_fWidth = (2.0f * m_fZNear) * tanf(FovX/2.0f );
		m_fHeight = (2.0f * m_fZNear) * tanf(FovY/2.0f );
	}


	//-----------------------------------------------------------------------------
	// Name: Projection::SetFovYAspect
	//-----------------------------------------------------------------------------
	void Projection::SetFovYAspect( float FovY, float Aspect, float ZNear, float ZFar )
	{
		m_fZNear = ZNear;
		m_fZFar = ZFar;
		m_Type = Projection::Perspective;


		m_fHeight = 2.0f * m_fZNear * tanf(FovY/2.f);
		m_fWidth = m_fHeight * Aspect;
		m_fAspect = Aspect;
	}


	//-----------------------------------------------------------------------------
	// Name: Projection::SetFovXAspect
	//-----------------------------------------------------------------------------
	void Projection::SetFovXAspect( float FovX, float Aspect, float ZNear, float ZFar )
	{
		m_fZNear = ZNear;
		m_fZFar = ZFar;
		m_Type = Projection::Perspective;


		m_fWidth = 2.0f * m_fZNear * tanf(FovX/2.f);
		m_fHeight = m_fWidth / Aspect;
		m_fAspect = Aspect;
	}


	//-----------------------------------------------------------------------------
	// Name: Projection::SetOrthographic
	//-----------------------------------------------------------------------------
	void Projection::SetOrthographic( float Width, float Height, float ZNear, float ZFar )
	{
		m_fZNear = ZNear;
		m_fZFar = ZFar;
		m_Type = Projection::Orthographic;
		m_fWidth = Width;
		m_fHeight = Height;
	}


	//-----------------------------------------------------------------------------
	// Name: Projection::GetFovX
	//-----------------------------------------------------------------------------
	float Projection::GetFovX() const
	{
		assert( m_Type == Projection::Perspective );
		return atanf( m_fWidth / (2.0f * m_fZNear) ) * 2.0f;
	}


	//----------------------------------------------------------------------------
	// Name: Projection::GetFovY
	//-----------------------------------------------------------------------------
	float Projection::GetFovY() const
	{
		assert( m_Type == Projection::Perspective );
		return atanf( m_fHeight / (2.0f * m_fZNear) ) * 2.0f;
	}



	cCamera::cCamera()
	{
		//Init with standard OGL values:
		m_vPosition = Vector3 (0.0, 0.0,	0.0);
		m_vViewDir = Vector3( 0.0, 1.0, 0.0);
		m_vRightVector = Vector3 (1.0, 0.0, 0.0);
		m_vUpVector = Vector3 (0.0, 0.0, 1.0);

		//Only to be sure:
		m_fRotatedX = m_fRotatedY = m_fRotatedZ = 0.0;
	}

	void cCamera::Move (Vector3 Direction)
	{
		m_vPosition = m_vPosition + Direction;
	}

	void cCamera::RotateX (GLfloat Angle)
	{
		m_fRotatedX += Angle;
		
		//Rotate viewdir around the right vector:
		m_vViewDir = (m_vViewDir*cos(Angle*D3DX_PI/180.f)
									+ m_vUpVector*sin(Angle*D3DX_PI/180.f));
		m_vViewDir.NormalizeIt();
		//now compute the new UpVector (by cross product)
		m_vUpVector = m_vViewDir^m_vRightVector;
		m_vUpVector*=-1;

		
	}

	void cCamera::RotateY (GLfloat Angle)
	{
		m_fRotatedY += Angle;
		
		//Rotate viewdir around the up vector:
		m_vViewDir = m_vViewDir*cos(Angle*D3DX_PI/180.f)
									- m_vRightVector*sin(Angle*D3DX_PI/180.f);
		m_vViewDir.NormalizeIt();
		//now compute the new RightVector (by cross product)
		m_vRightVector = m_vViewDir^m_vUpVector;
	}

	void cCamera::RotateZ (GLfloat Angle)
	{
		m_fRotatedZ += Angle;
		
		//Rotate viewdir around the right vector:
		m_vRightVector = m_vRightVector*cos(Angle*D3DX_PI/180.f)
									+ m_vUpVector*sin(Angle*D3DX_PI/180.f);
		m_vRightVector.NormalizeIt();
		//now compute the new UpVector (by cross product)
		m_vUpVector = m_vViewDir^m_vRightVector;
		m_vUpVector*=-1;
	}

	void cCamera::Render( void )
	{

		//The point at which the camera looks:
		Vector3 ViewPoint = m_vPosition+m_vViewDir;

		//as we know the up vector, we can easily use gluLookAt:
		//gluLookAt	is equivalent to glMultMatrixf(M);
		//	glTranslated (-eyex, -eyey, -eyez);
		//SEE ALSO
		//glFrustum, gluPerspective
		//http://www.gamedev.net/community/forums/topic.asp?topic_id=421529
		//here is what gluLookAt done
		//This is a vector camera and might do the trick that you are looking for.
		//U is the unit vector that points to the right of the camera position.
		//V is the unit vector that points upward from the camera position.
		//N is the unit vector that points in the opposite direction of the view.
		//For example, if the camera was looking down the negative z-axis,
		//then the unit N would be pointing in the positive z direction.
		//The upper 3x3 matrix then describes the orientation of the camera.
		//The translation portion of the matrix maps the camera to the origin.
		//void SetLookAt( const Vector & vEye,
		//				const Vector & vLookat,
		//				const Vector & vUp )
		//{
		//	// determine the new n
		//	Vector vN = vEye - vLookat;
		//	// determine the new u by crossing with the up vector
		//	Vector vU = vUp ^ vN;
		//	// normalize both the u and n vectors
		//	vU.Normalize(); vN.Normalize();
		//	// determine v by crossing n and u
		//	Vector vV = vN ^ vU;

		//	// create a model view matrix
		//	float fModelView[16] =
		//	{
		//	   vU.fX,        vV.fX,        vN.fX,        0.0f,
		//	   vU.fY,        vV.fY,        vN.fY,        0.0f,
		//	   vU.fZ,        vV.fZ,        vN.fZ,        0.0f,
		//	   -(vEye * vU), -(vEye * vV), -(vEye * vN), 1.0f
		//	}

		//	// load the model view matrix
		//	// the model view matrix should already be active
		//	glLoadMatrixf(fModelView);
		//}
		//glLoadIdentity();
		//Glu32.lib
		//gluLookAt(	m_vPosition.x,m_vPosition.y,m_vPosition.z,
		//			ViewPoint.x,ViewPoint.y,ViewPoint.z,
		//			m_vUpVector.x,m_vUpVector.y,m_vUpVector.z);
		cMatrix44	l_Mat = cMatrix44::LookAtMatrix(m_vPosition,ViewPoint,m_vUpVector);
		SetupShaderViewProjectionMatrix(l_Mat,true);
	}

	void cCamera::MoveForward( GLfloat Distance )
	{
		m_vPosition = m_vPosition + (m_vViewDir*-Distance);
	}

	void cCamera::MoveRight ( GLfloat Distance )
	{
		m_vPosition = m_vPosition + (m_vRightVector*Distance);
	}

	void cCamera::MoveUpward( GLfloat Distance )
	{
		m_vPosition = m_vPosition + (m_vUpVector*Distance);
	}

	void cCamera::LookAtPoint(Vector3 e_vPoint)
	{
		//get up offset angle
		float	l_fCameraAngle = GetDirectionOffset(this->m_vViewDir);
		float	l_fDestAngle = GetDirectionOffset(e_vPoint);
		float	l_fAngleRotationY = l_fDestAngle - l_fCameraAngle;
		this->RotateY(l_fAngleRotationY);
		//get Viewdir offset angle
		float	l_fAngleRotationX = GetUpOffset(this->m_vUpVector);;
		this->RotateX(l_fAngleRotationX);		
	}

	cFrameCamera::cFrameCamera(int e_iViewWidth,int e_iViewHeight,float e_fNearPlane,float e_fFarPlane)
	{
		m_Projection.SetFovYAspect( XM_PIDIV4 ,(float)e_iViewWidth/(float)e_iViewHeight,e_fNearPlane,e_fFarPlane);
	}

	cFrameCamera::cFrameCamera(cFrameCamera*e_pFrameCamera):Frame(e_pFrameCamera)
	{
		m_Projection = e_pFrameCamera->m_Projection;
	}

	void	cFrameCamera::Render()
	{
		//MyGLEnable(GL_DEPTH_TEST);
		FATMING_CORE::SetupShaderViewProjectionMatrix(m_Projection.GetMatrix()*this->GetWorldView(),true);
	}

	void	cFrameCamera::DisableRender()
	{
		FATMING_CORE::DisableShaderProgram();
	}

}