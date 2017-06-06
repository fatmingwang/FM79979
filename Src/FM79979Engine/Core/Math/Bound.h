#ifndef _BOUND_H
#define _BOUND_H

#include "Collision.h"

	struct EnumStringMap
	{
		unsigned long Value;
		const wchar_t* szName;
	};

    class cBound
    {
    public:
        enum BoundType
        {
            No_Bound,
            Sphere_Bound,
            Frustum_Bound,
            OBB_Bound,
            AABB_Bound,
			RECT2D_BOUND,
            ForceDWORD = 0x7FFFFFFF
        };

        static const EnumStringMap   BoundType_StringMap[];

        // construct/destruct a bound
        cBound() { Clear(); }
        cBound( const sSphere& Sphere ) { SetSphere( Sphere ); }
        cBound( const sFrustum& Frustum ){ SetFrustum( Frustum ); }
        cBound( const sAxisAlignedBox& Aabb ){ SetAabb( Aabb ); }
        cBound( const sOrientedBox& Obb ){ SetObb( Obb ); }
		cBound( const RECT& e_Rect ){ SetRect(e_Rect); }
        cBound( const cBound& Other ){ *this = Other; }
        virtual ~cBound() {}

        void Clear() { m_Type = cBound::No_Bound; }

        // collision with other primitives
        bool					Collide( const cBound& Other ) const;
        bool					Collide( const sFrustum& Frustum ) const;	
        bool					Collide( const sSphere& Sphere ) const;
        bool					Collide( const sAxisAlignedBox& Aabb ) const;
        bool					Collide( const sOrientedBox& pObb ) const;
		bool					Collide( const RECT& e_rect ) const;
		//only for 2D
		bool					Collide( const int e_iPosX,const int e_iPosY ) const;

        // merge with another bound
        void					Merge( const cBound& Other );

        // transformation
        cBound					operator*( const cMatrix44 Matrix ) const;

        // get-sets
        // Note that the get functions will assert if you try to
        // get a type not equal to the bounds current BoundType
        BoundType               GetType() const { return m_Type; }
        const sSphere&          GetSphere() const;
        void                    SetSphere( const sSphere& Sphere );
        const sFrustum&         GetFrustum() const;
        void                    SetFrustum( const sFrustum& Frustum );
        const sAxisAlignedBox&  GetAabb() const;
        void                    SetAabb( const sAxisAlignedBox& Aabb );
        const sOrientedBox&     GetObb() const;
        void                    SetObb( const sOrientedBox& Obb );
		void					SetRect(const RECT& e_Rect);
        // gets the center of the bound. 
        Vector3					GetCenter() const;
		const	RECT&			GetRect() const;

        // gets the bound's maximum radius
        float					GetMaxRadius() const;
		void					DebugRender();

    private:
        BoundType				m_Type;
        // Data must be big enough to hold the union of all the bound types.
        BYTE					m_Data[ sizeof( FLOAT ) * 13 ];

    };

#endif