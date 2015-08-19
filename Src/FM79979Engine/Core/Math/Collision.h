#ifndef COLLISION_H
#define COLLISION_H

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix44.h"
//#include "Bound.h"

struct sAxisAlignedBox
{
    Vector3 			vCenter;            // Center of the box.
    Vector3 			vExtents;           // Distance from the center to each side.
};

struct sOrientedBox
{
    Vector3 			vCenter;            // Center of the box.
    Vector3 			vExtents;           // Distance from the center to each side.
    Vector4 			vOrientation;       // Unit quaternion representing rotation (box -> world).
};

#define XM_PIDIV4           0.785398163f
//=================
//here for 
//=================

struct sFrustum
{
	Vector3 Origin;            // Origin of the sFrustum (and projection).
	Vector4 Orientation;       // Unit quaternion representing rotation.

	float RightSlope;           // Positive X slope (X/Z).
	float LeftSlope;            // Negative X slope.
	float TopSlope;             // Positive Y slope (Y/Z).
	float BottomSlope;          // Negative Y slope.
	float Near, Far;            // Z of the near plane and far plane.
};
//please use Sphere instead Sphere
struct	sSphere
{
	sSphere(){}
	sSphere(Vector3 e_vCenter,float e_fRadius){ fRadius = e_fRadius; vCenter = e_vCenter; }
	//sSphere(){}
	float				fRadius;// Radius of the sphere.
	Vector3				vCenter;// Center of the sphere.
};


//-----------------------------------------------------------------------------
// Bounding volume construction.
//-----------------------------------------------------------------------------
void ComputeBoundingSphereFromPoints( sSphere* pOut,  UINT Count, const Vector3* pPoints,  UINT Stride );
void ComputeBoundingAxisAlignedBoxFromPoints( sAxisAlignedBox* pOut,  UINT Count, const Vector3* pPoints,  UINT Stride );
void ComputeBoundingOrientedBoxFromPoints( sOrientedBox* pOut,  UINT Count, const Vector3* pPoints,  UINT Stride );
void ComputeFrustumFromProjection( sFrustum* pOut,cMatrix44* pProjection );
void ComputePlanesFromFrustum( const sFrustum* pVolume,Vector4* pPlane0,Vector4* pPlane1,Vector4* pPlane2,
                              Vector4* pPlane3,Vector4* pPlane4,Vector4* pPlane5 );



//-----------------------------------------------------------------------------
// Bounding volume transforms.
//-----------------------------------------------------------------------------
void TransformSphere( sSphere* pOut, const sSphere* pIn,float Scale, Vector4 Rotation, Vector4 Translation );
void TransformAxisAlignedBox( sAxisAlignedBox* pOut, const sAxisAlignedBox* pIn,float Scale, Vector4 Rotation,
                              Vector4 Translation );
void TransformOrientedBox( sOrientedBox* pOut, const sOrientedBox* pIn,float Scale, Vector4 Rotation,
                           Vector4 Translation );
void TransformFrustum( sFrustum* pOut, const sFrustum* pIn,float Scale, Vector4 Rotation, Vector4 Translation );



//-----------------------------------------------------------------------------
// Intersection testing routines.
//-----------------------------------------------------------------------------
bool IntersectPointSphere( Vector4 Point, const sSphere* pVolume );
bool IntersectPointAxisAlignedBox( Vector4 Point, const sAxisAlignedBox* pVolume );
bool IntersectPointOrientedBox( Vector4 Point, const sOrientedBox* pVolume );
bool IntersectPointFrustum( Vector4 Point, const sFrustum* pVolume );
bool IntersectRayTriangle( Vector4 Origin, Vector4 Direction, Vector4 V0, Vector4 V1, Vector4 V2,
                          float* pDist );
bool IntersectRaySphere( Vector4 Origin, Vector4 Direction, const sSphere* pVolume,float* pDist );
bool IntersectRayAxisAlignedBox( Vector4 Origin, Vector4 Direction, const sAxisAlignedBox* pVolume,float* pDist );
bool IntersectRayOrientedBox( Vector4 Origin, Vector4 Direction, const sOrientedBox* pVolume,float* pDist );
bool IntersectTriangleTriangle( Vector4 A0, Vector4 A1, Vector4 A2, Vector4 B0, Vector4 B1, Vector4 B2 );
bool IntersectTriangleSphere( Vector4 V0, Vector4 V1, Vector4 V2, const sSphere* pVolume );
bool IntersectTriangleAxisAlignedBox( Vector4 V0, Vector4 V1, Vector4 V2, const sAxisAlignedBox* pVolume );
bool IntersectTriangleOrientedBox( Vector4 V0, Vector4 V1, Vector4 V2, const sOrientedBox* pVolume );
bool IntersectSphereSphere( const sSphere* pVolumeA, const sSphere* pVolumeB );
bool IntersectSphereAxisAlignedBox( const sSphere* pVolumeA, const sAxisAlignedBox* pVolumeB );
bool IntersectSphereOrientedBox( const sSphere* pVolumeA, const sOrientedBox* pVolumeB );
bool IntersectAxisAlignedBoxAxisAlignedBox( const sAxisAlignedBox* pVolumeA, const sAxisAlignedBox* pVolumeB );
bool IntersectAxisAlignedBoxOrientedBox( const sAxisAlignedBox* pVolumeA, const sOrientedBox* pVolumeB );
bool IntersectOrientedBoxOrientedBox( const sOrientedBox* pVolumeA, const sOrientedBox* pVolumeB );



//-----------------------------------------------------------------------------
// sFrustum intersection testing routines.
// Return values: 0 = no intersection, 
//                1 = intersection, 
//                2 = A is completely inside B
//-----------------------------------------------------------------------------
int IntersectTriangleFrustum( Vector4 V0, Vector4 V1, Vector4 V2, const sFrustum* pVolume );
int IntersectSphereFrustum( const sSphere* pVolumeA, const sFrustum* pVolumeB );
int IntersectAxisAlignedBoxFrustum( const sAxisAlignedBox* pVolumeA, const sFrustum* pVolumeB );
int IntersectOrientedBoxFrustum( const sOrientedBox* pVolumeA, const sFrustum* pVolumeB );
int IntersectFrustumFrustum( const sFrustum* pVolumeA, const sFrustum* pVolumeB );




//-----------------------------------------------------------------------------
// Test vs six planes (usually forming a sFrustum) intersection routines.
// The intended use for these routines is for fast culling to a view sFrustum.  
// When the volume being tested against a view sFrustum is small relative to the
// view sFrustum it is usually either inside all six planes of the sFrustum or 
// outside one of the planes of the sFrustum. If neither of these cases is true
// then it may or may not be intersecting the sFrustum. Outside a plane is 
// defined as being on the positive side of the plane (and inside negative).
// Return values: 0 = volume is outside one of the planes (no intersection),
//                1 = not completely inside or completely outside (intersecting),
//                2 = volume is inside all the planes (completely inside)
//-----------------------------------------------------------------------------
int IntersectTriangle6Planes( Vector4 V0, Vector4 V1, Vector4 V2, Vector4 Plane0, Vector4 Plane1,
                              Vector4 Plane2, Vector4 Plane3, Vector4 Plane4, Vector4 Plane5 );
int IntersectSphere6Planes( const sSphere* pVolume, Vector4 Plane0, Vector4 Plane1, Vector4 Plane2,
                            Vector4 Plane3, Vector4 Plane4, Vector4 Plane5 );
int IntersectAxisAlignedBox6Planes( const sAxisAlignedBox* pVolume, Vector4 Plane0, Vector4 Plane1,
                                    Vector4 Plane2, Vector4 Plane3, Vector4 Plane4, Vector4 Plane5 );
int IntersectOrientedBox6Planes( const sOrientedBox* pVolume, Vector4 Plane0, Vector4 Plane1, Vector4 Plane2,
                                 Vector4 Plane3, Vector4 Plane4, Vector4 Plane5 );
int IntersectFrustum6Planes( const sFrustum* pVolume, Vector4 Plane0, Vector4 Plane1, Vector4 Plane2,
                             Vector4 Plane3, Vector4 Plane4, Vector4 Plane5 );


//-----------------------------------------------------------------------------
// Volume vs plane intersection testing routines.
// Return values: 0 = volume is outside the plane (on the positive sideof the plane),
//                1 = volume intersects the plane,
//                2 = volume is inside the plane (on the negative side of the plane) 
//-----------------------------------------------------------------------------
int IntersectTrianglePlane( Vector4 V0, Vector4 V1, Vector4 V2, Vector4 Plane );
int IntersectSpherePlane( const sSphere* pVolume, Vector4 Plane );
int IntersectAxisAlignedBoxPlane( const sAxisAlignedBox* pVolume, Vector4 Plane );
int IntersectOrientedBoxPlane( const sOrientedBox* pVolume, Vector4 Plane );
int IntersectFrustumPlane( const sFrustum* pVolume, Vector4 Plane );



namespace UT
{
	bool 					RectCollideRectWithNewPosition(const RECT& rcObject1, const RECT& rcObject2, POINT* pptOutPenetration = 0, const POINT* pptMovement = 0);
	bool					SphereCollidePoint(Vector3 e_vPoint,sSphere e_Sphere);
	int						SphereCollideRect( RECT e_rc,POINT e_CenterPoint,float e_fRadius);
	bool					SphereCollideSphere(sSphere e_Sphere1,sSphere e_Sphere2);
	bool 					VecAngleTest(float e_fMaxAngle, Vector3 e_v1, Vector3 e_v2 );
	//fuck it does't work
	int 					sphere_line_intersection(Vector3	e_vP1,Vector3	e_vP2,sSphere e_sSphere,float*e_pfRssult);
	bool					SphereCollideSphereWithNewPosition(sSphere *e_pSphereTarget,sSphere *e_pSphereSource,float e_fStillCollidedOffset = 0.f );	
	bool					CollideRectWithTransform(int e_iClickX,int e_iClickY,int e_iImagePosX,int e_iImagePosY,float e_fZRotationAngle,int e_iImageSizeX,int e_iImageSizeY);
	bool					CollideRectWithTransform(int e_iClickX,int e_iClickY,int e_iImagePosX,int e_iImagePosY,Vector3 e_vRotationAngle,int e_iImageSizeX,int e_iImageSizeY);

    inline bool				RectCollidePoint( RECT rc,int e_iPosX,int e_iPosY )	{if(rc.left<=e_iPosX&&rc.right>=e_iPosX)if(rc.bottom>=e_iPosY&&rc.top<=e_iPosY){return true;}return false;}
	inline bool				RectCollidePoint( RECT rc,POINT point )	{if(rc.left<=point.x&&rc.right>=point.x)if(rc.bottom>=point.y&&rc.top<=point.y){return true;}return false;}
	inline bool				RectCollideRect( RECT e_rc1,RECT e_rc2){return RectCollideRectWithNewPosition(e_rc1,e_rc2);}
	inline int				SphereCollideRect( RECT e_rc,sSphere e_Sphere){	POINT	l_Point = {(long)e_Sphere.vCenter.x,(long)e_Sphere.vCenter.y};return SphereCollideRect(e_rc,l_Point,e_Sphere.fRadius);}
	inline float 			square(float f ) { return (f*f) ;};
}
#endif