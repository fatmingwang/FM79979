//#if defined USING_BULLET_PHYSICAL_ENGINE || defined(WIN32)//or iPhone??
#include "../stdafx.h"
#include "BulletShape.h"
#include "2DImageCollisionData.h"
#define USE_GJK

#include "../Bullet/LinearMath/btQuaternion.h"
#include "../Bullet/LinearMath/btTransform.h"
#include "../Bullet/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h"
#include "../Bullet/BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "../Bullet/BulletCollision/CollisionShapes/btCylinderShape.h"
#include "../Bullet/BulletCollision/CollisionShapes/btSphereShape.h"
#include "../Bullet/BulletCollision/CollisionShapes/btCompoundShape.h"

#include "../Bullet/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.h"
#include "../Bullet/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h"
#include "../Bullet/BulletCollision/NarrowPhaseCollision/btPointCollector.h"
#include "../Bullet/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h"
#include "../Bullet/BulletCollision/NarrowPhaseCollision/btConvexPenetrationDepthSolver.h"
#include "../Bullet/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h"

#ifndef USE_GJK
#include "btBulletCollisionCommon.h"
#endif //USE_GJK
#include "../Image/SimplePrimitive.h"
#include "../GameplayUT/GameApp.h"
namespace FATMING_CORE
{
	bool	g_sbCollisionRender = false;
	TYPDE_DEFINE_MARCO(cbtShapeCollision);
	TYPDE_DEFINE_MARCO(cbtConvexHullShape);
	TYPDE_DEFINE_MARCO(cbtSphereShape);
	TYPDE_DEFINE_MARCO(cbtConcaveShapeList);

	btGjkEpaPenetrationDepthSolver* g_pEpa = 0;
	btVoronoiSimplexSolver* g_pGjkSimplexSolver = 0;
	btGjkPairDetector::ClosestPointInput* g_pInput = 0;
	int		g_iInitBTCount = 0;

	void	InitBT()
	{
		++g_iInitBTCount;
		if(!g_pEpa)
		{
			g_pEpa = new btGjkEpaPenetrationDepthSolver;
			g_pGjkSimplexSolver = new btVoronoiSimplexSolver;
			g_pInput = new btGjkPairDetector::ClosestPointInput;
		}
	}

	void	ExitBT()
	{
		--g_iInitBTCount;
		if( g_iInitBTCount == 0 )
		{
			SAFE_DELETE(g_pEpa);
			SAFE_DELETE(g_pGjkSimplexSolver);
			SAFE_DELETE(g_pInput);
		}
	}

	Vector3		GetVector3(btVector3* e_pv)
	{
		return Vector3(e_pv->m_floats[0],e_pv->m_floats[1],e_pv->m_floats[2]);
	}
	cMatrix44	GetMatrix(btTransform* e_btTransform)
	{
		cMatrix44	l_mat;
		e_btTransform->getOpenGLMatrix((float*)&l_mat);
		return l_mat;
	}


	cbtShapeCollision::cbtShapeCollision()
	{
		m_pbtCollisionShape = 0;
		m_pmatTransform = new btTransform;
		m_pmatTransform->setIdentity();
	}

	cbtShapeCollision::cbtShapeCollision(cbtShapeCollision*e_pbtShapeCollision)
	{
		SetName(e_pbtShapeCollision->GetName());
		m_pbtCollisionShape = 0;
		m_pmatTransform = new btTransform;
		m_pmatTransform->setIdentity();
	}

	//cbtShapeCollision*cbtShapeCollision::GetMe(TiXmlElement*e_pElement)
	//{
	//	ELEMENT_VALUE_ASSERT_CHECK(e_pElement,L"Collision");
	//	PARSE_ELEMENT_START
	//		COMPARE_NAME("File")
	//		{
	//		
	//		}
	//	PARSE_NAME_VALUE_END
	//}

	cbtShapeCollision::~cbtShapeCollision()
	{
		SAFE_DELETE(m_pmatTransform);
		SAFE_DELETE(m_pbtCollisionShape);
	}

	//<cbtShapeCollision FileName="Image/Fish/test.DAE" Name="Collision" />
	cbtShapeCollision*cbtShapeCollision::GetMe(TiXmlElement*e_pElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pElement,cbtShapeCollision::TypeID);
		if(cGameApp::m_sp2DImageCollisionDataVector)
		{
			const wchar_t*l_strfileName = e_pElement->Attribute(L"FileName");
			if( l_strfileName )
			{
				c2DImageCollisionData*l_p2DImageCollisionData = cGameApp::m_sp2DImageCollisionDataVector->GetObject(l_strfileName);
				const wchar_t*l_strName = e_pElement->Attribute(L"Name");
				if( !l_p2DImageCollisionData )
				{
					l_p2DImageCollisionData = new c2DImageCollisionData();
					if(l_p2DImageCollisionData->Parse(UT::WcharToChar(l_strfileName).c_str()))
					{
						l_p2DImageCollisionData->SetName(l_strfileName);
						cGameApp::m_sp2DImageCollisionDataVector->AddObjectNeglectExist(l_p2DImageCollisionData);
						return l_p2DImageCollisionData->GetObject(l_strName);
					}
					SAFE_DELETE(l_p2DImageCollisionData);
				}
				return l_p2DImageCollisionData->GetObject(l_strName);
			}
		}
		return 0;
	}

	void	cbtShapeCollision::SetConvexShape(btConvexShape*e_pbtConvexShape)
	{
		SAFE_DELETE(m_pbtCollisionShape);
		m_pbtCollisionShape = e_pbtConvexShape; 
	}
	void	cbtShapeCollision::SetTransform(float*e_pfMatrix)
	{
		m_pmatTransform->setFromOpenGLMatrix(e_pfMatrix); 
	}

	void	cbtShapeCollision::SetTransform(btTransform*e_pMatrix)
	{
		*m_pmatTransform = *e_pMatrix;
	}

	btTransform*cbtShapeCollision::GetTransform()
	{
		return m_pmatTransform; 
	}

	bool	cbtShapeCollision::Collide(cbtShapeCollision*e_pbtShapeCollision)
	{
		if( e_pbtShapeCollision->Type() == cbtConcaveShapeList::TypeID )
		{
			cbtConcaveShapeList*l_pbtConcaveShapeList = (cbtConcaveShapeList*)e_pbtShapeCollision;
			return l_pbtConcaveShapeList->Collide(this);
		}
		btGjkPairDetector	convexConvex((btConvexShape*)m_pbtCollisionShape,(btConvexShape*)e_pbtShapeCollision->GetConvexShape(),g_pGjkSimplexSolver,g_pEpa);

		//btVector3 seperatingAxis(0.00000000f,0.059727669f,0.29259586f);
		//convexConvex->setCachedSeperatingAxis(seperatingAxis);
		g_pInput->m_transformA = *m_pmatTransform;
		g_pInput->m_transformB = *e_pbtShapeCollision->GetTransform();

		btPointCollector l_gjkOutput;
		convexConvex.getClosestPoints(*g_pInput ,l_gjkOutput,0);
		bool	l_bResult = l_gjkOutput.m_hasResult;
		if (l_bResult)
		{
			float	l_fDis = l_gjkOutput.m_distance;
			if( l_fDis<=0.f )
				return true;
		}
		return false;
	}



	cbtConvexHullShape::cbtConvexHullShape(float *e_pfVertex,int e_iCount,int e_iStride)
	{
		assert(sizeof(float)*3>=e_iStride);
		m_pbtCollisionShape = new btConvexHullShape(e_pfVertex,e_iCount,e_iStride);
		m_pvPos = 0;
		m_iVerticesCount = 0;
		if( g_sbCollisionRender )
		{
			m_iVerticesCount = e_iCount;
			m_pvPos = new Vector3[m_iVerticesCount];
			Vector3*l_pMyPos = m_pvPos;
			char*l_pcSrc = (char*)e_pfVertex;
			for( int i=0;i<m_iVerticesCount;++i )
			{
				memcpy(l_pMyPos,l_pcSrc,sizeof(float)*3);
				++l_pMyPos;
				l_pcSrc+=e_iStride;
			}
		}
	}
	cbtConvexHullShape::cbtConvexHullShape(cbtConvexHullShape*e_pbtConvexHullShape)
	{
		btConvexHullShape*l_pbtConvexHullShape = (btConvexHullShape*)e_pbtConvexHullShape->m_pbtCollisionShape;
		int	l_iNumPoints = l_pbtConvexHullShape->getNumPoints();
		m_pbtCollisionShape = new btConvexHullShape((btScalar*)l_pbtConvexHullShape->getUnscaledPoints(),l_iNumPoints,sizeof(btVector3));
		m_pvPos = 0;
		m_iVerticesCount = e_pbtConvexHullShape->m_iVerticesCount;
		if( g_sbCollisionRender )
		{
			m_iVerticesCount = l_iNumPoints;
			m_pvPos = new Vector3[m_iVerticesCount];
			Vector3*l_pMyPos = m_pvPos;
			float*l_pcSrc = (float*)l_pbtConvexHullShape->getUnscaledPoints();
			for( int i=0;i<m_iVerticesCount;++i )
			{
				memcpy(l_pMyPos,l_pcSrc,sizeof(float)*3);
				++l_pMyPos;
				l_pcSrc+=4;
			}
		}
	}

	cbtConvexHullShape::~cbtConvexHullShape()
	{
		SAFE_DELETE(m_pvPos);
	}

	void	cbtConvexHullShape::DebugRender()
	{
		if( m_iVerticesCount>1 && g_sbCollisionRender )
		{
			int	l_iNumPoint = (m_iVerticesCount-1)*2;
			std::vector<Vector3>	l_CurvesPointVector;
			Vector3	l_vPreviousPos = m_pvPos[0];
			l_CurvesPointVector.resize(l_iNumPoint);
			for( int i=0;i<m_iVerticesCount-1;++i )
			{
				l_CurvesPointVector[i*2] = l_vPreviousPos;
				l_CurvesPointVector[i*2+1] = m_pvPos[i+1];
				l_vPreviousPos = m_pvPos[i+1];
			}
			cMatrix44	l_mat = GetMatrix(m_pmatTransform);
			RenderLine(&l_CurvesPointVector,Vector4::One,(float*)&l_mat);
		}
	}

	cbtSphereShape::cbtSphereShape(float e_fRadius,Vector3 e_vPos)
	{
		m_pbtCollisionShape = 0;
		m_fRadius = e_fRadius;
		this->m_vOriginalPos = e_vPos;
		this->m_pmatTransform->setOrigin(btVector3(m_vOriginalPos.x,m_vOriginalPos.y,m_vOriginalPos.z));
		m_pbtCollisionShape = new btSphereShape(e_fRadius);
	}

	cbtSphereShape::cbtSphereShape(cbtSphereShape*e_pbtSphereShape)
	{
		m_pbtCollisionShape = 0;
		m_fRadius = e_pbtSphereShape->m_fRadius;
		btVector3	l_btVector3 = e_pbtSphereShape->m_pmatTransform->getOrigin();
		this->m_pmatTransform->setOrigin(l_btVector3);
		btSphereShape*l_pbtSphereShape = (btSphereShape*)e_pbtSphereShape->m_pbtCollisionShape;
		float	l_fRadius = l_pbtSphereShape->getRadius();
		this->m_vOriginalPos = Vector3(l_btVector3.x(),l_btVector3.y(),l_btVector3.z());
		m_pbtCollisionShape = new btSphereShape(l_fRadius);
	}

	void	cbtSphereShape::SetTransform(float*e_pfMatrix)
	{
		cMatrix44 l_mat = *(cMatrix44*)e_pfMatrix;
		if( m_vOriginalPos.x != 0.f||
			m_vOriginalPos.y != 0.f||
			m_vOriginalPos.z != 0.f)
			l_mat = l_mat*cMatrix44::TranslationMatrix(m_vOriginalPos);
		m_pmatTransform->setFromOpenGLMatrix(l_mat);
	}

	void	cbtSphereShape::SetTransform(btTransform*e_pMatrix)
	{
		btTransform	l_mat = btTransform::getIdentity();
		l_mat.setOrigin(btVector3(this->m_vOriginalPos.x,m_vOriginalPos.y,m_vOriginalPos.z));
		l_mat = *e_pMatrix*l_mat;
		*m_pmatTransform = l_mat;
	}

	void	cbtSphereShape::DebugRender()
	{
		if( g_sbCollisionRender )
		{
			Vector3	l_vPos = GetVector3(&m_pmatTransform->getOrigin());//+m_vOriginalPos;
			cMatrix44	l_mat = GetMatrix(m_pmatTransform);
			RenderSphere(l_mat,m_fRadius);
		}
	}

	cbtConcaveShapeList::cbtConcaveShapeList()
	{

	}

	cbtConcaveShapeList::cbtConcaveShapeList(cbtConcaveShapeList*e_pbtConcaveShapeList):
		cNamedTypedObjectVector<cbtShapeCollision>(e_pbtConcaveShapeList),cbtShapeCollision(e_pbtConcaveShapeList)
	{
	}

	cbtConcaveShapeList::~cbtConcaveShapeList()
	{
	
	}

	void	cbtConcaveShapeList::ApplyTransformToChild()
	{
		int	l_iNum = this->Count();
		for( int i=0;i<l_iNum;++i )
		{
			cbtShapeCollision*l_pbtShapeCollision = (*this)[i];
			l_pbtShapeCollision->SetTransform(m_pmatTransform);
			////do something if need
			//if( l_pbtShapeCollision->Type() == cbtConvexHullShape::TypeID)
			//{
			//	l_pbtShapeCollision->SetTransform(m_pmatTransform);		
			//}
			//else
			//if( l_pbtShapeCollision->Type() == cbtSphereShape::TypeID)
			//{
			//	l_pbtShapeCollision->GetTransform()->setOrigin(
			//		l_pbtShapeCollision->GetTransform()->getOrigin()+this->m_pmatTransform->getOrigin());
			//}
		}		
	}

	void	cbtConcaveShapeList::SetTransform(float*e_pfMatrix)
	{
		m_pmatTransform->setFromOpenGLMatrix(e_pfMatrix);
		ApplyTransformToChild();
	}
	void	cbtConcaveShapeList::SetTransform(btTransform*e_pMatrix)
	{
		*m_pmatTransform = *e_pMatrix;
		ApplyTransformToChild();
	}

	bool cbtConcaveShapeList::Collide(cbtShapeCollision*e_pbtShapeCollision)
	{
		int	l_iNum = this->Count();
		for( int i=0;i<l_iNum;++i )
		{
			cbtShapeCollision*l_pbtShapeCollision = (*this)[i];
			if(l_pbtShapeCollision->Collide(e_pbtShapeCollision))
				return true;
		}
		return false;
	}

	void	cbtConcaveShapeList::DebugRender()
	{
		int	l_iNum = this->Count();
		for( int i=0;i<l_iNum;++i )
		{
			cbtShapeCollision*l_pbtShapeCollision = (*this)[i];
			l_pbtShapeCollision->DebugRender();
		}
	}

	bool	ConvexShapeCollideSphere(float*e_pfSphereMatrix,float e_fRadius,float*e_pfConvexMatrix,float*e_pfConvexPosition,int e_iNum,int e_iStride)
	{
		btSphereShape	l_btSphereShape(e_fRadius);
		btConvexHullShape	l_btConvexHullShape(e_pfConvexPosition,e_iNum,e_iStride);
		btTransform	l_SphereMatrix;l_SphereMatrix.setFromOpenGLMatrix(e_pfSphereMatrix);
		btTransform	l_ConvexMatrix;l_ConvexMatrix.setFromOpenGLMatrix(e_pfConvexMatrix);

		btGjkPairDetector	convexConvex((btConvexShape*)&l_btSphereShape,(btConvexShape*)&l_btConvexHullShape,g_pGjkSimplexSolver,g_pEpa);
		g_pInput->m_transformA = l_SphereMatrix;
		g_pInput->m_transformB = l_ConvexMatrix;

		btPointCollector l_gjkOutput;
		convexConvex.getClosestPoints(*g_pInput ,l_gjkOutput,0);
		bool	l_bResult = l_gjkOutput.m_hasResult;
		if (l_bResult)
		{
			float	l_fDis = l_gjkOutput.m_distance;
			if( l_fDis<=0.f )
				return true;
		}
		return false;
	}

	bool	ConvexShapeCollideConverx(float*e_pfConvexMatrix,float*e_pfConvexPosition,int e_iNum,float*e_pfConvexMatrix2,float*e_pfConvexPosition2,int e_iNum2,int e_iStride)
	{
		btConvexHullShape	l_btConvexHullShape(e_pfConvexPosition,e_iNum,e_iStride);
		btConvexHullShape	l_btConvexHullShape2(e_pfConvexPosition2,e_iNum2,e_iStride);
		btTransform	l_ConvexMatrix;l_ConvexMatrix.setFromOpenGLMatrix(e_pfConvexMatrix);
		btTransform	l_ConvexMatrix2;l_ConvexMatrix.setFromOpenGLMatrix(e_pfConvexMatrix2);

		btGjkPairDetector	convexConvex((btConvexShape*)&l_btConvexHullShape,(btConvexShape*)&l_btConvexHullShape2,g_pGjkSimplexSolver,g_pEpa);
		g_pInput->m_transformA = l_ConvexMatrix;
		g_pInput->m_transformB = l_ConvexMatrix;

		btPointCollector l_gjkOutput;
		convexConvex.getClosestPoints(*g_pInput ,l_gjkOutput,0);
		bool	l_bResult = l_gjkOutput.m_hasResult;
		if (l_bResult)
		{
			float	l_fDis = l_gjkOutput.m_distance;
			if( l_fDis<=0.f )
				return true;
		}
		return false;	
	}

	bool	SphereCollideSphere(float*e_pfSphereMatrix1,float e_fRadius1,float*e_pfSphereMatrix2,float e_fRadius2)
	{
		btSphereShape	l_btSphereShape1(e_fRadius1);
		btSphereShape	l_btSphereShape2(e_fRadius2);
		btTransform	l_SphereMatrix1;l_SphereMatrix1.setFromOpenGLMatrix(e_pfSphereMatrix1);
		btTransform	l_SphereMatrix2;l_SphereMatrix2.setFromOpenGLMatrix(e_pfSphereMatrix2);

		btGjkPairDetector	convexConvex((btConvexShape*)&l_btSphereShape1,(btConvexShape*)&l_btSphereShape2,g_pGjkSimplexSolver,g_pEpa);
		g_pInput->m_transformA = l_SphereMatrix1;
		g_pInput->m_transformB = l_SphereMatrix2;

		btPointCollector l_gjkOutput;
		convexConvex.getClosestPoints(*g_pInput ,l_gjkOutput,0);
		bool	l_bResult = l_gjkOutput.m_hasResult;
		if (l_bResult)
		{
			float	l_fDis = l_gjkOutput.m_distance;
			if( l_fDis<=0.f )
				return true;
		}
		return false;	
	}

	bool	btCollisionShapeCollidebtCollisionShape(float*e_pfMatrix1,btCollisionShape*e_pbtCollisionShape1,float*e_pfMatrix2,btCollisionShape*e_pbtCollisionShape2)
	{
		btTransform	l_Matrix1;l_Matrix1.setFromOpenGLMatrix(e_pfMatrix1);
		btTransform	l_Matrix2;l_Matrix2.setFromOpenGLMatrix(e_pfMatrix2);

		btGjkPairDetector	convexConvex((btConvexShape*)e_pbtCollisionShape1,(btConvexShape*)e_pbtCollisionShape2,g_pGjkSimplexSolver,g_pEpa);
		g_pInput->m_transformA = l_Matrix1;
		g_pInput->m_transformB = l_Matrix2;

		btPointCollector l_gjkOutput;
		convexConvex.getClosestPoints(*g_pInput ,l_gjkOutput,0);
		bool	l_bResult = l_gjkOutput.m_hasResult;
		if (l_bResult)
		{
			float	l_fDis = l_gjkOutput.m_distance;
			if( l_fDis<=0.f )
				return true;
		}
		return false;	
	}


	void	Test()
	{
		InitBT();
		btVector3	l_vPos[6];
		l_vPos[0] = btVector3(0,0,0);
		l_vPos[1] = btVector3(0,2,0);
		l_vPos[2] = btVector3(2,2,0);
		l_vPos[3] = btVector3(4,1,0);
		l_vPos[4] = btVector3(4,0,0);
		l_vPos[5] = btVector3(2,-2,0);
		btVector3	l_vPos1[6];
		l_vPos1[0] = btVector3(5,0,0);
		l_vPos1[1] = btVector3(6,0,0);
		l_vPos1[2] = btVector3(7,0,0);
		l_vPos1[3] = btVector3(7,1,0);
		l_vPos1[4] = btVector3(6,1,0);
		l_vPos1[5] = btVector3(5,1,0);
		//cbtConvexHullShape	*m_pbtConvexHullShape1 = new cbtConvexHullShape((float*)&l_vPos[0].getX(),6);
		//cbtConvexHullShape	*m_pbtConvexHullShape2 = new cbtConvexHullShape((float*)&l_vPos1[0].getX(),6);
		//cbtSphereShape* ball = new cbtSphereShape(1.f);
		//bool	l_b1 = ball->Collide(m_pbtConvexHullShape2);
		//bool	l_b = m_pbtConvexHullShape1->Collide(m_pbtConvexHullShape2);
		

		//c2DImageCollisionData	*l_2DImageCollisionData = new c2DImageCollisionData();
		//l_2DImageCollisionData->OpenFile("C:/Users/ShihMing/Desktop/10.xml");
		//cbtSphereShape	l_btSphereShape(29.f);
		//cbtSphereShape	l_btSphereShape2(5.f);
		//bool	l_b2 = l_2DImageCollisionData->GetShapeCollisionByIndex(0)->Collide((cbtShapeCollision*)&l_btSphereShape);
		//l_b2 = l_2DImageCollisionData->GetShapeCollisionByIndex(0)->Collide((cbtShapeCollision*)&l_btSphereShape2);
		//int		a = 0;
		//delete l_2DImageCollisionData;
		ExitBT();
	}
//end namespace
}