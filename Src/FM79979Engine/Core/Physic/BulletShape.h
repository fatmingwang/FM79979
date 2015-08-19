#ifndef _BULLET_ENGINE_H
#define _BULLET_ENGINE_H
//#if defined USING_BULLET_PHYSICAL_ENGINE || defined( WIN32 )//or iPhone??
//==============
//if u wanna intergrated bullet engine with dot net u have to use below ddeclare
//#pragma unmanaged
//#pragma managed



//The updated Bullet 2.75 release, svn revision 1776 supports btConvex2dShape.
//Check out the updated Bullet/Demos/Box2dDemo showing how to use this btConvex2dShape in combination with a 2d btConvexHullShape.



class	btConvexShape;
class	btTransform;
class	btCollisionShape;
class	btVector3;
namespace FATMING_CORE
{
	extern		bool	g_sbCollisionRender;
	Vector3		GetVector3(btVector3* e_pv);
	cMatrix44	GetMatrix(btTransform* e_btTransform);

	void	InitBT();
	void	ExitBT();

	//very similar btCollisionObject,but not add into the physical world.
	//if collids is not u expect please check 'Z' value
	class	cbtShapeCollision:virtual public NamedTypedObject
	{
	protected:
		btCollisionShape*m_pbtCollisionShape;
		btTransform *m_pmatTransform;
	public:
		DEFINE_TYPE_INFO();
		cbtShapeCollision();
		//static cbtShapeCollision*GetMe(TiXmlElement*e_pElement);
		cbtShapeCollision(cbtShapeCollision*e_pbtShapeCollision);
		CLONE_MYSELF(cbtShapeCollision);
		static cbtShapeCollision*	GetMe(TiXmlElement*e_pElement);
		virtual~cbtShapeCollision();
		void	SetConvexShape(btConvexShape*e_pbtConvexShape);
		btCollisionShape*GetConvexShape(){ return m_pbtCollisionShape; }
		virtual	void	SetTransform(float*e_pfMatrix);
		virtual	void	SetTransform(btTransform*e_pMatrix);
		inline	btTransform	*GetTransform();
		//
		virtual	bool Collide(cbtShapeCollision*e_pbtShapeCollision);
		virtual	void DebugRender(){}
	};
	//=======================
	//
	//=======================
	class	cbtConvexHullShape:public cbtShapeCollision
	{
	public:
		DEFINE_TYPE_INFO();
		//input position(xyz)
		cbtConvexHullShape(float *e_pfVertex,int e_iCount,int e_iStride = sizeof(float)*4);//btVector3 has xyzw.
		cbtConvexHullShape(cbtConvexHullShape*e_pbtConvexHullShape);
		CLONE_MYSELF(cbtConvexHullShape);
		virtual~cbtConvexHullShape();
		//for debug render
		int		m_iVerticesCount;
		Vector3*m_pvPos;
		virtual	void DebugRender();
	};

	class	cbtSphereShape:public cbtShapeCollision
	{
		Vector3	m_vOriginalPos;//to set original position
		float	m_fRadius;
	public:
		DEFINE_TYPE_INFO();
		cbtSphereShape(float e_fRadius,Vector3 e_vPos = Vector3::Zero);
		cbtSphereShape(cbtSphereShape*e_pbtSphereShape);
		virtual ~cbtSphereShape(){}
		CLONE_MYSELF(cbtSphereShape);
		virtual	void	SetTransform(float*e_pfMatrix);
		virtual	void	SetTransform(btTransform*e_pMatrix);
		virtual	void	DebugRender();
	};

	class	cbtConcaveShapeList:public cNamedTypedObjectVector<cbtShapeCollision>,public cbtShapeCollision
	{
		void	ApplyTransformToChild();
	public:
		DEFINE_TYPE_INFO();
		cbtConcaveShapeList();
		cbtConcaveShapeList(cbtConcaveShapeList*e_pbtConcaveShapeList);
		CLONE_MYSELF(cbtConcaveShapeList);
		virtual ~cbtConcaveShapeList();
		virtual	void	SetTransform(float*e_pfMatrix);
		virtual	void	SetTransform(btTransform*e_pMatrix);
		virtual	bool	Collide(cbtShapeCollision*e_pbtShapeCollision);
		void			DebugRender();
	};

	void	Test();
	bool	ConvexShapeCollideSphere(float*e_pfSphereMatrix,float e_fRadius,float*e_pfConvexMatrix,float*e_pfConvexPosition,int e_iNum,int e_iStride = sizeof(Vector3));
	bool	ConvexShapeCollideConverx(float*e_pfConvexMatrix,float*e_pfConvexPosition,int e_iNum,float*e_pfConvexMatrix2,float*e_pfConvexPosition2,int e_iNum2,int e_iStride = sizeof(Vector3));
	bool	SphereCollideSphere(float*e_pfSphereMatrix1,float e_fRadius1,float*e_pfSphereMatrix2,float e_fRadius2);
	bool	btCollisionShapeCollidebtCollisionShape(float*e_pfMatrix1,btCollisionShape*e_pbtCollisionShape1,float*e_pfMatrix2,btCollisionShape*e_pbtCollisionShape2);

//end namespace FATMING_CORE
}
//end #if defined USING_BULLET_PHYSICAL_ENGINE || defined( WIN32 )//or iPhone??
//#endif
#endif