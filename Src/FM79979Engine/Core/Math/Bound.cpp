#include "../stdafx.h"
#include "Bound.h"
#include "Quaternion.h"

const EnumStringMap cBound::BoundType_StringMap[] =
{
    { cBound::No_Bound, L"nullptr" },
    { cBound::Sphere_Bound, L"SPHERE" },
    { cBound::Frustum_Bound, L"FRUSTUM" },
    { cBound::OBB_Bound, L"OBB" },
    { cBound::AABB_Bound, L"AABB" },
    { 0, nullptr },
};


//-----------------------------------------------------------------------------
// Name: cBound::operator*
// Desc: transforms the bound by the current matrix
//-----------------------------------------------------------------------------
cBound cBound::operator*( cMatrix44 World ) const
{
    //$OPTIMIZE: store matrix decomposed    
    Vector4 Translation = ToVector4(World.r[3]);
    float Scale = ToVector3(World.r[2]).Length();
	Quaternion	l_Quaternion(World);
	l_Quaternion.NormalizeIt();
    //Vector4 Rotation = XMQuaternionNormalize( XMQuaternionRotationMatrix( World ) );
	Vector4 Rotation(l_Quaternion.x,l_Quaternion.y,l_Quaternion.z,l_Quaternion.w);

    // switch based off this bounds type and call the correct
    // bound transform function
    switch( m_Type )
    {
        case cBound::Sphere_Bound:
        {
            sSphere WorldSphere = GetSphere();
            TransformSphere( &WorldSphere,
                             &WorldSphere,
                             Scale,
                             Rotation,
                             Translation );
            return cBound( WorldSphere );
        }
        case cBound::Frustum_Bound:
        {
            sFrustum WorldFrustum = GetFrustum();
            TransformFrustum( &WorldFrustum,
                              &WorldFrustum,
                              Scale,
                              Rotation,
                              Translation );
            return cBound( WorldFrustum );
        }
        case cBound::OBB_Bound:
        {
            sOrientedBox WorldObb = GetObb();
            TransformOrientedBox( &WorldObb,
                                  &WorldObb,
                                  Scale,
                                  Rotation,
                                  Translation );
            return cBound( WorldObb );
        }
        case cBound::AABB_Bound:
        {
            sAxisAlignedBox WorldAabb = GetAabb();
            TransformAxisAlignedBox( &WorldAabb,
                                     &WorldAabb,
                                     Scale,
                                     Rotation,
                                     Translation );
            return cBound( WorldAabb );
        }
		case cBound::RECT2D_BOUND:
        {
			RECT l_Rect = GetRect();
            TransformRect( &l_Rect,&l_Rect,Scale,Rotation,Translation );
            return cBound( l_Rect );
        }
        case cBound::No_Bound:
            return cBound();
    }

    return cBound();
}


//-----------------------------------------------------------------------------
// Name: cBound::Collide
// Desc: collides this bound with an obb
//-----------------------------------------------------------------------------
bool cBound::Collide( const sOrientedBox& Obb ) const
{
    // switch on bound type and call the correct intersection function
    switch( m_Type )
    {
        case cBound::Sphere_Bound:
            return IntersectSphereOrientedBox( &GetSphere(), &Obb );
        case cBound::Frustum_Bound:
			return IntersectOrientedBoxFrustum( &Obb, &GetFrustum() )?true:false;
        case cBound::OBB_Bound:
            return IntersectOrientedBoxOrientedBox( &GetObb(), &Obb );
        case cBound::AABB_Bound:
            return IntersectAxisAlignedBoxOrientedBox( &GetAabb(), &Obb );
        case cBound::No_Bound:
            return true;
    }

    return false;
}

bool	cBound::Collide( const int e_iPosX,const int e_iPosY ) const
{
	return RectCollidePoint(this->GetRect(),e_iPosX,e_iPosY);
}

bool	cBound::Collide( const RECT& e_rect ) const
{
	assert(this->m_Type == cBound::RECT2D_BOUND&& "sorry 2d only support 2d collide");
	return RectCollideRectWithNewPosition(this->GetRect(),e_rect);
	
}
//-----------------------------------------------------------------------------
// Name: cBound::Collide
// Desc: collides this bound with a sphere
//-----------------------------------------------------------------------------
bool cBound::Collide( const sSphere& Sphere ) const
{
    switch( m_Type )
    {
        case cBound::Sphere_Bound:
            return IntersectSphereSphere( &GetSphere(), &Sphere );
        case cBound::Frustum_Bound:
			return IntersectSphereFrustum( &Sphere, &GetFrustum() )?true:false;
        case cBound::OBB_Bound:
            return IntersectSphereOrientedBox( &Sphere, &GetObb() );
        case cBound::AABB_Bound:
            return IntersectSphereAxisAlignedBox( &Sphere, &GetAabb() );
        case cBound::No_Bound:
            return true;
    }

    return false;
}


//-----------------------------------------------------------------------------
// Name: cBound::Collide
// Desc: collides this bound with an axis aligned box
//-----------------------------------------------------------------------------
bool cBound::Collide( const sAxisAlignedBox& Aabb ) const
{
    switch( m_Type )
    {
        case cBound::Sphere_Bound:
            return IntersectSphereAxisAlignedBox( &GetSphere(), &Aabb );
        case cBound::Frustum_Bound:
			return IntersectAxisAlignedBoxFrustum( &Aabb, &GetFrustum() )?true:false;
        case cBound::OBB_Bound:
            return IntersectAxisAlignedBoxOrientedBox( &Aabb, &GetObb() );
        case cBound::AABB_Bound:
            return IntersectAxisAlignedBoxAxisAlignedBox( &GetAabb(), &Aabb );
        case cBound::No_Bound:
            return true;
    }

    return false;
}


//-----------------------------------------------------------------------------
// Name: cBound::Collide
// Desc: collides this bound with a frustum
//-----------------------------------------------------------------------------
bool cBound::Collide( const sFrustum& Frustum ) const
{
    switch( m_Type )
    {
        case cBound::Sphere_Bound:
			return IntersectSphereFrustum( &GetSphere(), &Frustum )?true:false;
        case cBound::Frustum_Bound:
            return IntersectFrustumFrustum( &GetFrustum(), &Frustum )?true:false;
        case cBound::OBB_Bound:
            return IntersectOrientedBoxFrustum( &GetObb(), &Frustum )?true:false;
        case cBound::AABB_Bound:
            return IntersectAxisAlignedBoxFrustum( &GetAabb(), &Frustum )?true:false;
        case cBound::No_Bound:
            return true;
    }

    return false;
}


//-----------------------------------------------------------------------------
// Name: cBound::Collide
// Desc: collides this bound with another bound
//-----------------------------------------------------------------------------
bool cBound::Collide( const cBound& Other ) const
{
    switch( Other.m_Type )
    {
        case cBound::Sphere_Bound:
            return Collide( Other.GetSphere() );
        case cBound::Frustum_Bound:
            return Collide( Other.GetFrustum() );
        case cBound::OBB_Bound:
            return Collide( Other.GetObb() );
        case cBound::AABB_Bound:
            return Collide( Other.GetAabb() );
        case cBound::No_Bound:
            return true;
    }

    return false;
}


void cBound::Merge( const cBound& Other )
{
    sSphere OtherSphere;
	OtherSphere.vCenter = Other.GetCenter();
    OtherSphere.fRadius = Other.GetMaxRadius();

    if( m_Type == cBound::No_Bound )
    {
        SetSphere( OtherSphere );
        return;
    }

    sSphere ThisSphere;
    if( m_Type != cBound::Sphere_Bound )
    {
        // convert this bound into a sphere
        ThisSphere.vCenter = GetCenter();
        ThisSphere.fRadius = GetMaxRadius();
    }
    else
    {
        ThisSphere = GetSphere();
    }

    Vector3 vThisCenter = ThisSphere.vCenter;
    Vector3 vOtherCenter = OtherSphere.vCenter ;
    Vector3 vThisToOther = vOtherCenter-vThisCenter ;
    Vector3 vDistance = Vector3LengthEst( vThisToOther );

    float fCombinedDiameter = vDistance.x + ThisSphere.fRadius + OtherSphere.fRadius;
    if( fCombinedDiameter <= ( ThisSphere.fRadius * 2 ) )
    {
        SetSphere( ThisSphere );
        return;
    }
    if( fCombinedDiameter <= ( OtherSphere.fRadius * 2 ) )
    {
        SetSphere( OtherSphere );
        return;
    }

	Vector3 vDirectionNorm = vThisToOther.Normalize();

    Vector4 vRadius( ThisSphere.fRadius, OtherSphere.fRadius, 0, 0 );
    Vector4 vThisRadius( vRadius.x,vRadius.x,vRadius.x,vRadius.x );
    Vector4 vOtherRadius( vRadius.y,vRadius.y,vRadius.y,vRadius.y );
    Vector4 vCombinedDiameter = vThisRadius + Vector4(vDistance.x,vDistance.y,vDistance.z,0) + vOtherRadius;
    Vector4 vMaxDiameter = VectorMax( vCombinedDiameter, vThisRadius * 2 );
    vMaxDiameter = VectorMax( vMaxDiameter, vOtherRadius * 2 );
    Vector4 vMaxRadius = vMaxDiameter * 0.5f;
    ThisSphere.fRadius = vMaxRadius.x;
    vMaxRadius -= vThisRadius;
    Vector4 vCombinedCenter = Vector4(vThisCenter,0.f) + (Vector4Multiply(vMaxRadius,Vector4(vDirectionNorm,0.f)));
    ThisSphere.vCenter = Vector3(vCombinedCenter);
    SetSphere( ThisSphere );
}


//-----------------------------------------------------------------------------
// Name: cBound::SetSphere
// Desc: sets this bound to a sphere
//-----------------------------------------------------------------------------
void cBound::SetSphere( const sSphere& Sphere )
{
    m_Type = cBound::Sphere_Bound;
    memcpy( m_Data, &Sphere, sizeof( Sphere ) );
}


//-----------------------------------------------------------------------------
// Name: cBound::GetSphere
// Desc: Gets the current bound as a sphere.
//-----------------------------------------------------------------------------
const sSphere& cBound::GetSphere() const
{
    assert( m_Type == cBound::Sphere_Bound );
    return *( sSphere* )m_Data;
}


//-----------------------------------------------------------------------------
// Name: cBound::SetAabb
// Desc: sets this bound to an axis aligned box
//-----------------------------------------------------------------------------
void cBound::SetAabb( const sAxisAlignedBox& Aabb )
{
    m_Type = cBound::AABB_Bound;
    memcpy( m_Data, &Aabb, sizeof( Aabb ) );
}


//-----------------------------------------------------------------------------
// Name: cBound::GetAabb
// Desc: Gets the current bound as an axis aligned box.
//-----------------------------------------------------------------------------
const sAxisAlignedBox& cBound::GetAabb() const
{
    assert( m_Type == cBound::AABB_Bound );
    return *( sAxisAlignedBox* )m_Data;
}


//-----------------------------------------------------------------------------
// Name: cBound::SetObb
// Desc: sets this bound to an oriented box
//-----------------------------------------------------------------------------
void cBound::SetObb( const sOrientedBox& Obb )
{
    m_Type = cBound::OBB_Bound;
    memcpy( m_Data, &Obb, sizeof( Obb ) );
}

void	cBound::SetRect(const RECT& e_Rect)
{
	m_Type = cBound::RECT2D_BOUND;
    memcpy( m_Data, &e_Rect, sizeof( e_Rect ) );
}


//-----------------------------------------------------------------------------
// Name: cBound::GetObb
// Desc: Gets this bound as an oriented box
//-----------------------------------------------------------------------------
const sOrientedBox& cBound::GetObb() const
{
    assert( m_Type == cBound::OBB_Bound );
    return *( sOrientedBox* )m_Data;
}


//-----------------------------------------------------------------------------
// Name: cBound::SetFrustum
// Desc: sets this bound to a frustum
//-----------------------------------------------------------------------------
void cBound::SetFrustum( const sFrustum& Frustum )
{
    m_Type = cBound::Frustum_Bound;
    memcpy( m_Data, &Frustum, sizeof( Frustum ) );
}


//-----------------------------------------------------------------------------
// Name: cBound::GetFrustum
// Desc: Gets this bound as a frustum
//-----------------------------------------------------------------------------
const sFrustum& cBound::GetFrustum() const
{
    assert( m_Type == cBound::Frustum_Bound );
    return *( sFrustum* )m_Data;
}


//-----------------------------------------------------------------------------
// Name: cBound::GetCenter
// Desc: Gets the center of the bound.
//-----------------------------------------------------------------------------
Vector3 cBound::GetCenter() const
{
    switch( m_Type )
    {
        case cBound::Sphere_Bound:
            return GetSphere().vCenter ;
        case cBound::Frustum_Bound:
            return GetFrustum().Origin ;
        case cBound::OBB_Bound:
            return GetObb().vCenter ;
        case cBound::AABB_Bound:
            return GetAabb().vCenter ;
		case cBound::RECT2D_BOUND:
			{
				const RECT l_rect = this->GetRect();
				return Vector3((l_rect.right-l_rect.left)/2.f,(l_rect.bottom-l_rect.top)/2.f,0.f);
			}
        case cBound::No_Bound:
            break;
    }

    return Vector3( 0.0f, 0.0f, 0.0f );
}

const	RECT&	cBound::GetRect() const
{
	assert( m_Type == cBound::RECT2D_BOUND );
    return *( RECT* )m_Data;
}


//-----------------------------------------------------------------------------
// Name: cBound::GetMaxRadius
// Desc: Computes the maximum radius of the bound.
//-----------------------------------------------------------------------------
float cBound::GetMaxRadius() const
{
    switch( m_Type )
    {
        case cBound::Sphere_Bound:
        {
            float Radius = GetSphere().fRadius;
            return Radius;
        }

        case cBound::Frustum_Bound:
        {
            float MaxZ = fabs( GetFrustum().Far - GetFrustum().Near );
            float MaxX = fabs( GetFrustum().LeftSlope * GetFrustum().Far - GetFrustum().RightSlope * GetFrustum().Far );
            float MaxY = fabs( GetFrustum().TopSlope * GetFrustum().Far - GetFrustum().BottomSlope * GetFrustum().Far );
            return max( MaxZ, max( MaxX, MaxY ) );
        }

        case cBound::OBB_Bound:
        {
            return GetObb().vExtents.x;
        }
        case cBound::AABB_Bound:
        {
            return GetAabb().vExtents.x;
        }
        case cBound::No_Bound:
            break;
    }

    return 0.0f;
}
