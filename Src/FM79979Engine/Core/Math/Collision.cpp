#include "../stdafx.h"
#include "Collision.h"
namespace UT
{
	//http://www.gamedev.net/community/forums/topic.asp?topic_id=501718
	//float fTimeDelta = 0.016;
	//
	//object1.SetPosition(object1.GetPosition().x + int(float(object1.GetVelocity().x) * fTimeDelta),
	//		    object1.GetPosition().y + int(float(object1.GetVelocity().y) * fTimeDetla));
	//
	//POINT ptMovement = { object1.GetPosition().x - object1.GetPreviousPosition().x,
	//		     object1.GetPosition().y - object1.GetPreviousPosition().y };
	//
	//POINT ptPenetration;
	//
	//if(Collision(object1.GetBounds(), object2.GetBounds(), &ptPenetration, ptMovement))
	//{
	//	object1.SetPosition(object1.GetPosition().x + ptPenetration.x,
	//			    object1.GetPosition().y + ptPenetration.y);
	//
	//	// TODO: fire collision response after unseparating - apply forces, play sounds, etc.
	//}
	//

	bool 			RectCollideRectWithNewPosition(const RECT& rcObject1, const RECT& rcObject2, POINT* pptOutPenetration, const POINT* pptMovement)
	{
		// First, find out if they overlap

		if(rcObject1.left > rcObject2.right) return false;

		if(rcObject1.right < rcObject2.left) return false;

		if(rcObject1.top > rcObject2.bottom) return false;

		if(rcObject1.bottom < rcObject2.top) return false;

		// If they do, calculate penetration if specified

		if(pptOutPenetration)
		{
			// Calculate horizontal penetration with respect to rcObject1

			if(rcObject2.left >= rcObject1.left &&
			   rcObject2.left <= rcObject1.right)
			{
				pptOutPenetration->x = rcObject2.left - rcObject1.right;
			}
			else if(rcObject1.left >= rcObject2.left &&
				rcObject1.left <= rcObject2.right)
			{
				pptOutPenetration->x = rcObject2.right - rcObject1.left;
			}

			// Calculate vertical penetration with respect to rcObject1

			if(rcObject2.top >= rcObject1.top &&
			   rcObject2.top <= rcObject1.bottom)
			{
				pptOutPenetration->y = rcObject2.top - rcObject1.bottom;
			}
			else if(rcObject1.top >= rcObject2.top &&
				rcObject1.top <= rcObject2.bottom)
			{
				pptOutPenetration->y = rcObject2.bottom - rcObject1.top;
			}

			if(pptMovement)
			{
				// If given movement vector, weight penetration vector in movement direction
				// This makes sure that when boxes collide, the collider doesn't get
				// pushed out of the wrong side with resulting separation vector

				float fMovementDistance = sqrtf(float(pptMovement->x * pptMovement->x +
									  pptMovement->y * pptMovement->y));

				pptOutPenetration->x = int(float(pptOutPenetration->x) *
							fabsf(float(pptMovement->x) / fMovementDistance));

				pptOutPenetration->y = int(float(pptOutPenetration->y) *
							fabsf(float(pptMovement->y) / fMovementDistance));
			}
			else
			{
				// Choose the smallest absolute penetration axis as separation

				if(abs(pptOutPenetration->x) > abs(pptOutPenetration->y))
					pptOutPenetration->x = 0;
				else
					pptOutPenetration->y = 0;
			}
		}

		return true;
	}
	bool		SphereCollidePoint(Vector3 e_vPoint,sSphere e_Sphere)
	{
		Vector3	l_vLength = e_Sphere.vCenter-e_vPoint;
		float	l_fLength = l_vLength.x * l_vLength.x + l_vLength.y * l_vLength.y + l_vLength.z * l_vLength.z;
		if( l_fLength*l_fLength <= e_Sphere.fRadius*e_Sphere.fRadius )
			return true;
		return false;
	}
	//http://www.gamedev.net/community/forums/topic.asp?topic_id=91617
	//first assume the rectangle is axis aligned and centred on the origin. If it isn't then you can apply a rotation and translation to get it there,
	//so work these out and apply them to the coordinates of the Sphere.
	//
	//The rectangle now is defined by
	//
	//-a < x < a
	//-b < y < b
	//
	//How you proceed depends on where the centre of the Sphere is. If the Sphere is at (X, Y) and has radius r then there are four possibilities
	//
	//1) if
	//
	//-a < X < a and -b < Y < b
	//
	//the Sphere centre is inside the rectangle, and so it is/has collided
	//
	//2) if
	//-a < X < a 
	//
	//the Sphere is above or below the rectangle (assuming the x axis is horizontal). Then it is nearest the top or bottom edge, and so collides if
	//
	//Y < b + r or Y > -b - r
	//
	//3) if
	//-b < Y < b
	//
	//the Sphere is to the side of the rectangle, is nearest the left or right edge, and so collides if
	//
	//X < a + r or X > -a - r
	//
	//4) finally if none of the above are true it is nearest a corner. Which one depends on the sign of X and Y; e.g. if X and Y are both positive it is nearest (a, b), and so it collides if
	//here have to test four corner
	//(X - a)^2 + (Y - b)^2 < r^2
	//
	//The above is based on the geometry of the Voronoi regions of the rectangle - worth mentioning as Voronoi regions can be generated for shapes other than rectangles and in 3D as well as 2D,
	//giving a straightforward way to generalise this to other shapes and higher dimensions.

	//advance topic http://www.gamedev.net/community/forums/topic.asp?topic_id=497303

	//this one for none rotation
	//return value,0 for non collide,1for inside,2 for top,3 for bottom,,4 for left,5 for right,6 for corrnor
	int				SphereCollideRect( RECT e_rc,POINT e_CenterPoint,float e_fRadius)
	{
		//1
		if( e_CenterPoint.x>=e_rc.left&&e_CenterPoint.x<=e_rc.right)
		{
			if( e_CenterPoint.y>=e_rc.top&&e_CenterPoint.y<=e_rc.bottom )//inside the rectangle
				return 1;
			//2
			if(e_CenterPoint.y<=e_rc.top&&e_CenterPoint.y+e_fRadius>=e_rc.top)//top collided
				return 2;
			if(e_CenterPoint.y>=e_rc.bottom&&e_CenterPoint.y-e_fRadius<=e_rc.bottom)//down collided
				return 3;
		}
		else
		//3
		if( e_CenterPoint.y>=e_rc.top&&e_CenterPoint.y<=e_rc.bottom )
		{
			if(e_CenterPoint.x<=e_rc.left&&e_CenterPoint.x+e_fRadius>=e_rc.left)//left collided
				return 4;
			if(e_CenterPoint.x>=e_rc.right&&e_CenterPoint.x-e_fRadius<=e_rc.right)//right collided
				return 5;
		}
		//4
		else
		{	//leftup,rightup leftdown right diwn
			POINT	l_fCorner[4] = {{e_rc.left,e_rc.top},{e_rc.right,e_rc.top},{e_rc.left,e_rc.bottom},{e_rc.right,e_rc.bottom}};
			for(int i=0;i<4;++i)
			{
				POINT	l_Dis = { e_CenterPoint.x-l_fCorner[i].x,e_CenterPoint.y-l_fCorner[i].y };
				if( (l_Dis.x*l_Dis.x)+(l_Dis.y*l_Dis.y)<(e_fRadius*e_fRadius) )
					return 6+i;
			}
		}
		return 0;
	}

	bool				SphereCollideSphere(sSphere e_Sphere1,sSphere e_Sphere2)
	{
		float	l_fDistance = (e_Sphere1.vCenter-e_Sphere2.vCenter).Length();
		float	l_fRadius = e_Sphere1.fRadius+e_Sphere2.fRadius;
		//squard for value to postive
		return l_fRadius>=l_fDistance?true:false;
	}
	//http://www.3dkingdoms.com/weekly/weekly.php?a=27
	//
	//for orientation and object position,and a angle to determinate it collied or not
	bool 			VecAngleTest( float e_fMaxAngle, Vector3 e_v1, Vector3 e_v2 )
	{
		e_v1.NormalizeIt();
		e_v2.NormalizeIt();
		float l_fAngle = D3DXToDegree(acosf(e_v1*e_v2));
		return ( l_fAngle <= e_fMaxAngle );
	}
	//2 point and one sphere,
	//final result will stroe on e_pfRssult
	//e_pfRssult is a 6float array
	//for the point where collided
	//return value for how many points is collided,(max is2)
	//http://local.wasp.uwa.edu.au/~pbourke/geometry/sphereline/source.cpp
	//http://local.wasp.uwa.edu.au/~pbourke/geometry/sphereline/
	//fuck it does't work
	int 				sphere_line_intersection(
		Vector3	e_vP1,
		Vector3	e_vP2,
		sSphere e_sSphere,
		float*e_pfRssult)
	{
		 // x1,y1,z1  P1 coordinates (point of line)
		 // x2,y2,z2  P2 coordinates (point of line)
		 // x3,y3,z3, r  P3 coordinates and radius (sphere)
		 // x,y,z   intersection coordinates
		 //
		 // This function returns a pointer array which first index indicates
		 // the number of intersection point, followed by coordinate pairs.
		 float x1 = e_vP1.x;float y1 = e_vP1.y;float z1 = e_vP1.z;
		 float x2 = e_vP2.x;float y2 = e_vP2.y;float z2 = e_vP2.z;
		 float x3 = e_sSphere.vCenter.x;float y3 = e_sSphere.vCenter.y;float z3 = e_sSphere.vCenter.z;float r = e_sSphere.fRadius;
		 float a, b, c, mu, i ;

		 a =  square(x2 - x1) + square(y2 - y1) + square(z2 - z1);
		 b =  2* ( (x2 - x1)*(x1 - x3)
			  + (y2 - y1)*(y1 - y3)
			  + (z2 - z1)*(z1 - z3) ) ;
		 c =  square(x3) + square(y3) +
			  square(z3) + square(x1) +
			  square(y1) + square(z1) -
			  2* ( x3*x1 + y3*y1 + z3*z1 ) - square(r) ;
		 i =   b * b - 4 * a * c ;

		 if ( i < 0.0 )
		 {
			// no intersection
			e_pfRssult[0] = 0.0;
			return 0;
		 }
		 if ( i == 0.0 )
		 {
			// one intersection
			e_pfRssult[0] = 1.0;
			mu = -b/(2*a) ;
			e_pfRssult[1] = x1 + mu*(x2-x1);
			e_pfRssult[2] = y1 + mu*(y2-y1);
			e_pfRssult[3] = z1 + mu*(z2-z1);
			return 1;
		 }
		 if ( i > 0.0 )
		 {
			// two intersections
			e_pfRssult[0] = 2.0;

			// first intersection
			mu = (-b + sqrt( square(b) - 4*a*c )) / (2*a);
			e_pfRssult[1] = x1 + mu*(x2-x1);
			e_pfRssult[2] = y1 + mu*(y2-y1);
			e_pfRssult[3] = z1 + mu*(z2-z1);
			// second intersection
			mu = (-b - sqrt(square(b) - 4*a*c )) / (2*a);
			e_pfRssult[4] = x1 + mu*(x2-x1);
			e_pfRssult[5] = y1 + mu*(y2-y1);
			e_pfRssult[6] = z1 + mu*(z2-z1);
			return 2;
		 }
		 return 0;
	}
	//the resource Sphere will adjust to the new position for suitable place where seeing well
	//e_fStillCollidedOffset is the value let 2 object has a small collide zone but for vision will be non-notificationed
	//
	bool				SphereCollideSphereWithNewPosition(sSphere *e_pSphereTarget,sSphere *e_pSphereSource,float e_fStillCollidedOffset)
	{
		Vector3	l_vRelativePos = e_pSphereTarget->vCenter-e_pSphereSource->vCenter;
		float	l_fLength = l_vRelativePos.Length();
		float	l_f2Radius = e_pSphereSource->fRadius+e_pSphereTarget->fRadius-e_fStillCollidedOffset ;
		if( l_fLength <= l_f2Radius )
		{
			e_pSphereSource->vCenter = e_pSphereTarget->vCenter-(l_vRelativePos.Normalize()*(l_f2Radius));
			return true;
		}
		return false;
	}
	
     bool			CollideRectWithTransform(int e_iClickX,int e_iClickY,int e_iImagePosX,int e_iImagePosY,float e_fZRotationAngle,int e_iImageSizeX,int e_iImageSizeY)
    {
        int l_iLocalY = e_iClickY-e_iImagePosY;
        int l_iLocalX = e_iClickX-e_iImagePosX;
		cMatrix44	l_matrix;
		Vector3 l_vPos = Vector3(e_iImageSizeX/2.f-l_iLocalY,l_iLocalX-e_iImageSizeY/2.f,0);
		l_matrix = l_matrix.ZAxisRotationMatrix(MyMath::DegToRad(360-e_fZRotationAngle));
		Vector3 l_vFinalPos = l_matrix.TransformVector(l_vPos);
        RECT    l_rc = {e_iImagePosX,e_iImagePosY,e_iImagePosX+e_iImageSizeX,e_iImagePosY+e_iImageSizeY};
        if(UT::RectCollidePoint(l_rc,e_iClickX,e_iClickY))
            return true;
        return false;
    }

    bool			CollideRectWithTransform(int e_iClickX,int e_iClickY,int e_iImagePosX,int e_iImagePosY,Vector3 e_vRotationAngle,int e_iImageSizeX,int e_iImageSizeY)
    {
        int l_iLocalY = e_iClickY-e_iImagePosY;
        int l_iLocalX = e_iClickX-e_iImagePosX;
		cMatrix44	l_matrix;
		Vector3 l_vPos = Vector3(e_iImageSizeX/2.f-l_iLocalY,l_iLocalX-e_iImageSizeY/2.f,0);
		l_matrix = l_matrix.XAxisRotationMatrix(MyMath::DegToRad(360-e_vRotationAngle.x));
		l_matrix *= l_matrix.YAxisRotationMatrix(MyMath::DegToRad(360-e_vRotationAngle.y));
		l_matrix *= l_matrix.ZAxisRotationMatrix(MyMath::DegToRad(360-e_vRotationAngle.z));
		Vector3 l_vFinalPos = l_matrix.TransformVector(l_vPos);
        RECT    l_rc = {e_iImagePosX,e_iImagePosY,e_iImagePosX+e_iImageSizeX,e_iImagePosY+e_iImageSizeY};
        if(UT::RectCollidePoint(l_rc,e_iClickX,e_iClickY))
            return true;
        return false;
    }
//end namespace
}


//-----------------------------------------------------------------------------
// Build a frustum from a persepective projection matrix.  The matrix may only
// contain a projection; any rotation, translation or scale will cause the
// constructed frustum to be incorrect.
//-----------------------------------------------------------------------------
void ComputeBoundingSphereFromPoints( sSphere* pOut,  UINT Count, const Vector3* pPoints,  UINT Stride )
{

}

void ComputeBoundingAxisAlignedBoxFromPoints( sAxisAlignedBox* pOut,  UINT Count, const Vector3* pPoints,  UINT Stride )
{

}

void ComputeBoundingOrientedBoxFromPoints( sOrientedBox* pOut,  UINT Count, const Vector3* pPoints,  UINT Stride )
{

}

void ComputeFrustumFromProjection( sFrustum* pOut, cMatrix44* pProjection )
{
    assert( pOut );
    assert( pProjection );

    // Corners of the projection frustum in homogenous space.
    static Vector4 HomogenousPoints[6] =
    {
        Vector4(1.0f,  0.0f, 1.0f, 1.0f),   // right (at far plane)
        Vector4(-1.0f,  0.0f, 1.0f, 1.0f),   // left
        Vector4(0.0f,  1.0f, 1.0f, 1.0f),   // top
        Vector4(0.0f, -1.0f, 1.0f, 1.0f),   // bottom

        Vector4(0.0f, 0.0f, 0.0f, 1.0f),     // near
        Vector4(0.0f, 0.0f, 1.0f, 1.0f)      // far
    };

    cMatrix44 matInverse = 	pProjection->Inverted();

    // Compute the frustum corners in world space.
    Vector4 Points[6];

    for( INT i = 0; i < 6; i++ )
    {
        // Transform point.
		Points[i] = matInverse.TransformCoordinate(HomogenousPoints[i]);
    }

    pOut->Origin = Vector3( 0.0f, 0.0f, 0.0f );
    pOut->Orientation = Vector4( 0.0f, 0.0f, 0.0f, 1.0f );

    // Compute the slopes.
    Points[0] = Vector4Multiply(Points[0],VectorReciprocal( VectorSplatZ( Points[0] ) ));
    Points[1] = Vector4Multiply(Points[1],VectorReciprocal( VectorSplatZ( Points[1] )) );
    Points[2] = Vector4Multiply(Points[2],VectorReciprocal( VectorSplatZ( Points[2] )) );
    Points[3] = Vector4Multiply(Points[3],VectorReciprocal( VectorSplatZ( Points[3] )) );

    pOut->RightSlope = Points[0].x;
    pOut->LeftSlope = Points[1].x;
    pOut->TopSlope = Points[2].y;
    pOut->BottomSlope = Points[3].y;

    // Compute near and far.
    Points[4] = Vector4Multiply(Points[4],VectorReciprocal( VectorSplatW( Points[4] ) ));
    Points[5] = Vector4Multiply(Points[5],VectorReciprocal( VectorSplatW( Points[5] ) ));

    pOut->Near = Points[4].z;
    pOut->Far = Points[5].z;

    return;
}
void ComputePlanesFromFrustum( const sFrustum* pVolume,Vector4* pPlane0,Vector4* pPlane1,Vector4* pPlane2,
                              Vector4* pPlane3,Vector4* pPlane4,Vector4* pPlane5 )
{

}



//-----------------------------------------------------------------------------
// Bounding volume transforms.
//-----------------------------------------------------------------------------
void TransformSphere( sSphere* pOut, const sSphere* pIn,float Scale, Vector4 Rotation, Vector4 Translation )
{

}
void TransformAxisAlignedBox( sAxisAlignedBox* pOut, const sAxisAlignedBox* pIn,float Scale, Vector4 Rotation,
                              Vector4 Translation )
{

}

void TransformOrientedBox( sOrientedBox* pOut, const sOrientedBox* pIn,float Scale, Vector4 Rotation,
                           Vector4 Translation )
{

}

void TransformFrustum( sFrustum* pOut, const sFrustum* pIn,float Scale, Vector4 Rotation, Vector4 Translation )
{

}
//I am lazy so just do a move
void TransformRect( RECT* pOut, const RECT* pIn,float Scale, Vector4 Rotation, Vector4 Translation )
{
	POINT l_Size = {pIn->right-pIn->left,pIn->bottom-pIn->top};
	l_Size.x /= 2;
	l_Size.y /= 2;
	POINT l_NewPos = { (LONG)Translation.x,(LONG)Translation.y };
	pOut->left = l_NewPos.x-l_Size.x;
	pOut->top = l_NewPos.y-l_Size.y;
	pOut->right = l_NewPos.x+l_Size.x;
	pOut->bottom = l_NewPos.y+l_Size.y;
}


//-----------------------------------------------------------------------------
// Intersection testing routines.
//-----------------------------------------------------------------------------
bool IntersectPointSphere( Vector4 Point, const sSphere* pVolume )
{
	return true;
}

bool IntersectPointAxisAlignedBox( Vector4 Point, const sAxisAlignedBox* pVolume )
{
	return true;
}

bool IntersectPointOrientedBox( Vector4 Point, const sOrientedBox* pVolume )
{
	return true;
}

bool IntersectPointFrustum( Vector4 Point, const sFrustum* pVolume )
{
	return true;
}

bool IntersectRayTriangle( Vector4 Origin, Vector4 Direction, Vector4 V0, Vector4 V1, Vector4 V2,
                          float* pDist )
{
	return true;
}

bool IntersectRaySphere( Vector4 Origin, Vector4 Direction, const sSphere* pVolume,float* pDist )
{
	return true;
}

bool IntersectRayAxisAlignedBox( Vector4 Origin, Vector4 Direction, const sAxisAlignedBox* pVolume,float* pDist )
{
	return true;
}

bool IntersectRayOrientedBox( Vector4 Origin, Vector4 Direction, const sOrientedBox* pVolume,float* pDist )
{
	return true;
}

bool IntersectTriangleTriangle( Vector4 A0, Vector4 A1, Vector4 A2, Vector4 B0, Vector4 B1, Vector4 B2 )
{
	return true;
}

bool IntersectTriangleSphere( Vector4 V0, Vector4 V1, Vector4 V2, const sSphere* pVolume )
{
	return true;
}

bool IntersectTriangleAxisAlignedBox( Vector4 V0, Vector4 V1, Vector4 V2, const sAxisAlignedBox* pVolume )
{
	return true;
}

bool IntersectTriangleOrientedBox( Vector4 V0, Vector4 V1, Vector4 V2, const sOrientedBox* pVolume )
{
	return true;
}

bool IntersectSphereSphere( const sSphere* pVolumeA, const sSphere* pVolumeB )
{
	return true;
}

bool IntersectSphereAxisAlignedBox( const sSphere* pVolumeA, const sAxisAlignedBox* pVolumeB )
{
	return true;
}

bool IntersectSphereOrientedBox( const sSphere* pVolumeA, const sOrientedBox* pVolumeB )
{
	return true;
}

bool IntersectAxisAlignedBoxAxisAlignedBox( const sAxisAlignedBox* pVolumeA, const sAxisAlignedBox* pVolumeB )
{
	return true;
}

bool IntersectAxisAlignedBoxOrientedBox( const sAxisAlignedBox* pVolumeA, const sOrientedBox* pVolumeB )
{
	return true;
}

bool IntersectOrientedBoxOrientedBox( const sOrientedBox* pVolumeA, const sOrientedBox* pVolumeB )
{
	return true;
}




//-----------------------------------------------------------------------------
// sFrustum intersection testing routines.
// Return values: 0 = no intersection, 
//                1 = intersection, 
//                2 = A is completely inside B
//-----------------------------------------------------------------------------
int IntersectTriangleFrustum( Vector4 V0, Vector4 V1, Vector4 V2, const sFrustum* pVolume )
{
	return 1;
}

int IntersectSphereFrustum( const sSphere* pVolumeA, const sFrustum* pVolumeB )
{
	return 1;
}

int IntersectAxisAlignedBoxFrustum( const sAxisAlignedBox* pVolumeA, const sFrustum* pVolumeB )
{
	return 1;
}

int IntersectOrientedBoxFrustum( const sOrientedBox* pVolumeA, const sFrustum* pVolumeB )
{
	return 1;
}

int IntersectFrustumFrustum( const sFrustum* pVolumeA, const sFrustum* pVolumeB )
{
	return 1;
}





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
                              Vector4 Plane2, Vector4 Plane3, Vector4 Plane4, Vector4 Plane5 )
{
	return 1;
}

int IntersectSphere6Planes( const sSphere* pVolume, Vector4 Plane0, Vector4 Plane1, Vector4 Plane2,
                            Vector4 Plane3, Vector4 Plane4, Vector4 Plane5 )
{
	return 1;
}

int IntersectAxisAlignedBox6Planes( const sAxisAlignedBox* pVolume, Vector4 Plane0, Vector4 Plane1,
                                    Vector4 Plane2, Vector4 Plane3, Vector4 Plane4, Vector4 Plane5 )
{
	return 1;
}

int IntersectOrientedBox6Planes( const sOrientedBox* pVolume, Vector4 Plane0, Vector4 Plane1, Vector4 Plane2,
                                 Vector4 Plane3, Vector4 Plane4, Vector4 Plane5 )
{
	return 1;
}

int IntersectFrustum6Planes( const sFrustum* pVolume, Vector4 Plane0, Vector4 Plane1, Vector4 Plane2,
                             Vector4 Plane3, Vector4 Plane4, Vector4 Plane5 )
{
	return 1;
}



//-----------------------------------------------------------------------------
// Volume vs plane intersection testing routines.
// Return values: 0 = volume is outside the plane (on the positive sideof the plane),
//                1 = volume intersects the plane,
//                2 = volume is inside the plane (on the negative side of the plane) 
//-----------------------------------------------------------------------------
int IntersectTrianglePlane( Vector4 V0, Vector4 V1, Vector4 V2, Vector4 Plane )
{
	return 1;
}

int IntersectSpherePlane( const sSphere* pVolume, Vector4 Plane )
{
	return 1;
}

int IntersectAxisAlignedBoxPlane( const sAxisAlignedBox* pVolume, Vector4 Plane )
{
	return 1;
}

int IntersectOrientedBoxPlane( const sOrientedBox* pVolume, Vector4 Plane )
{
	return 1;
}

int IntersectFrustumPlane( const sFrustum* pVolume, Vector4 Plane )
{
	return 1;
}
