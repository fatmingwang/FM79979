//-----------------------------------------------------------------------------
// AtgFrame.h
//
// Describes a frame in the scene- a frame is a named typed object
// with a set of bounds and part of a transform heirarchy
//
// $OPTIMIZE: Pool Allocate
//
// Xbox Advanced Technology Group.
// Copyright (C) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#pragma once
#ifndef ATG_FRAME_H
#define ATG_FRAME_H

#include "AtgNamedTypedObject.h"

namespace ATG
{

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class TreeLoad : public NamedTypedObject
{
    DEFINE_TYPE_INFO();

public:    
    TreeLoad(int e_iElementSize = 0);
    TreeLoad( const StringID& Name,int e_iElementSize = 0);
    ~TreeLoad();

    // frame hierarchy
    VOID                        AddChild( TreeLoad* pChild );
    VOID                        SetParent( TreeLoad* pParent );  // Set to NULL to unparent
    
    TreeLoad*                   GetFirstChild() CONST { return m_pFirstChild; }
    TreeLoad*                   GetNextSibling() CONST { return m_pNextSibling; }
    TreeLoad*                   GetParent() CONST { return m_pParent; }
    BOOL                        IsAncestor( TreeLoad* pOtherFrame );
    BOOL                        IsChild( TreeLoad* pOtherFrame );

    // cloned object support
    VOID                        DisconnectFromParent();
	int							NumElement(){ return (int)m_ucNumElement; }
	WCHAR*						GetElementProperty(int e_i){ assert( e_i<=m_ucNumElement&&m_ucNumElement ); return m_ppElementProperty[e_i]; }
	bool						SetElement(int e_i,WCHAR*e_pString){  }
private:
	WCHAR**						m_ppElementProperty;
	BYTE						m_ucNumElement;
    VOID                        SetCachedWorldTransformDirty();
    
    TreeLoad*                   m_pParent;
    TreeLoad*                   m_pNextSibling;
    TreeLoad*                   m_pFirstChild;
};

} // namespace ATG

#endif // ATG_FRAME_H
