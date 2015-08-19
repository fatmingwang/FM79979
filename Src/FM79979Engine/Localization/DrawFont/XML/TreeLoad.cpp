//-----------------------------------------------------------------------------
// AtgFrame.cpp
//
// Xbox Advanced Technology Group.
// Copyright (C) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "TreeLoad.h"
#include <float.h>

namespace ATG
{

CONST StringID TreeLoad::TypeID( L"Frame" );
CONST FLOAT    FRAME_DIRTY_WORLD_CACHE = 1e10f;


//-----------------------------------------------------------------------------
// Name: Frame::Frame()
//-----------------------------------------------------------------------------
TreeLoad::TreeLoad(int e_iElementSize)
{
    m_pParent = NULL;
    m_pNextSibling = NULL;
    m_pFirstChild = NULL;
	m_ucNumElement = e_iElementSize;
	m_ppElementProperty = new WCHAR*[e_iElementSize];
	if( m_ucNumElement )
	{
		for( int i=0;i<e_iElementSize;++i)
			m_ppElementProperty[i] = 0;
	}
}


//-----------------------------------------------------------------------------
// Name: Frame::Frame()
//-----------------------------------------------------------------------------
TreeLoad::TreeLoad( const StringID& Name,int e_iElementSize)
{
    m_pParent = NULL;
    m_pNextSibling = NULL;
    m_pFirstChild = NULL;
    SetName( Name );
	m_ucNumElement = e_iElementSize;
	if( m_ucNumElement )
	{
		m_ppElementProperty = new WCHAR*[e_iElementSize];
		for( int i=0;i<e_iElementSize;++i)
			m_ppElementProperty[i] = 0;
	}
}


//-----------------------------------------------------------------------------
// Name: Frame::~Frame()
//-----------------------------------------------------------------------------
TreeLoad::~TreeLoad()
{
    SetParent( NULL );
	delete[] m_ppElementProperty;
}


//-----------------------------------------------------------------------------
// Name: Frame::AddChild()
//-----------------------------------------------------------------------------
VOID TreeLoad::AddChild( TreeLoad* pChild )
{
    assert( pChild );    
    pChild->SetParent( this );    
}


//-----------------------------------------------------------------------------
// Name: Frame::SetParent()
//-----------------------------------------------------------------------------
VOID TreeLoad::SetParent( TreeLoad* pParent )
{
    TreeLoad *pSrch, *pLast;

    if( m_pParent )
    {
        pLast = NULL;
        for( pSrch = m_pParent->m_pFirstChild; pSrch; pSrch = pSrch->m_pNextSibling )
        {
            if ( pSrch == this )
                break;
            pLast = pSrch;
        }

        // If we can't find this frame in the old parent's list, assert
        assert( pSrch );

        // Remove us from the list
        if ( pLast ) 
        {
            pLast->m_pNextSibling = m_pNextSibling;
        }
        else // at the beginning of the list
        {
            m_pParent->m_pFirstChild = m_pNextSibling;
        }

        m_pNextSibling = NULL;
        m_pParent = NULL;
    }

    // Add us to the new parent's list               
    if ( pParent )
    {
        m_pParent = pParent;   
        m_pNextSibling = pParent->m_pFirstChild;
        pParent->m_pFirstChild = this;
    }
}


//-----------------------------------------------------------------------------
// Name: Frame::IsAncestor()
// Desc: Returns TRUE if the specified frame is an ancestor of this frame
//       ancestor = parent, parent->parent, etc.
//       Also returns TRUE if specified frame is NULL.
//-----------------------------------------------------------------------------
BOOL TreeLoad::IsAncestor( TreeLoad* pOtherFrame )
{
    if( pOtherFrame == NULL )
        return TRUE;
    if( pOtherFrame == this )
        return FALSE;
    TreeLoad* pFrame = GetParent();
    while( pFrame != NULL )
    {
        if( pFrame == pOtherFrame )
            return TRUE;
        pFrame = pFrame->GetParent();
    }
    return FALSE;
}


//-----------------------------------------------------------------------------
// Name: Frame::IsChild()
// Desc: Returns TRUE if the specified frame is a direct child of this frame
//-----------------------------------------------------------------------------
BOOL TreeLoad::IsChild( TreeLoad* pOtherFrame )
{
    if( pOtherFrame == NULL )
        return FALSE;
    TreeLoad* pChild = GetFirstChild();
    while( pChild != NULL )
    {
        if( pChild == pOtherFrame )
            return TRUE;
        pChild = pChild->GetNextSibling();
    }
    return FALSE;
}

//-----------------------------------------------------------------------------
// Name: Frame::SetCachedWorldTransformDirty()
//-----------------------------------------------------------------------------
VOID TreeLoad::SetCachedWorldTransformDirty()
{
    TreeLoad *pChild;
    
    for( pChild = m_pFirstChild; pChild; pChild = pChild->m_pNextSibling )
    {
        pChild->SetCachedWorldTransformDirty();
    }
}


//-----------------------------------------------------------------------------
// Name: Frame::DisconnectFromParent
// Desc: Removes parent link from the object, and converts world transform
//       to local transform.  This method is only to be used on clones of
//       scene objects, since the parent is not notified of the disconnection.
//-----------------------------------------------------------------------------
VOID TreeLoad::DisconnectFromParent()
{
    // Debug check to make sure the parent really isn't the parent of this clone
#ifdef _DEBUG
    if( m_pParent != NULL )
    {
        assert( !m_pParent->IsChild( this ) );
    }
#endif
    m_pParent = NULL;
}

} // namespace ATG
