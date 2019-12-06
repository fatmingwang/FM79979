#pragma once

#include <functional>
#include "../../../Common/NamedTypedObject.h"
#include "../../../AllMathInclude.h"

//-----------------------------------------------------------------------------
// Name: Frame
// Desc: An named typed object with a frame of reference and a bound
//       Note: X is Right, Y is Up, and Z is back
//-----------------------------------------------------------------------------
const float    FRAME_DIRTY_WORLD_CACHE = 1e10f;
class Frame :virtual  public NamedTypedObject
{
	//while set transform is called we might need some data update.ex:rotation
	virtual	void				SetTransformInternalData(){}
protected:
	void						DestoryWithChildren();
public:
	//please do not use = directly,because the parent and child will copy
	//inline	Frame operator =(Frame e_Frame){	Frame	l_Frame;	l_Frame.SetLocalTransform(e_Frame.GetLocalTransform());	return l_Frame;}
    Frame();
	Frame(Frame*e_pFrame);
    virtual ~Frame();
	Frame*	CloneFrameWithHierarchy();
	//if you afried something wrong want manual to delete object set this->m_bIgnoreChildrenUpdate = true
	//
	//
	static	void				DestoryWithChildren(Frame*e_pFrame);
	Frame*						FinFrameByName(wchar_t*e_strName);
    // frame hierarchy
    virtual	void                AddChild( Frame* pChild ,bool e_bUpdateRelatedPosition = true);
	virtual	void                AddChildToLast( Frame* pChild ,bool e_bUpdateRelatedPosition = true);
    virtual	void                SetParent( Frame* pParent ,bool e_bUpdateRelatedPosition = true);  // Set to nullptr to unparent
	virtual	void	            SetNextSibling(Frame*e_pNextSibling);
    Frame*                      GetFirstChild() const { return m_pFirstChild; }
    Frame*                      GetNextSibling() const { return m_pNextSibling; }
    Frame*                      GetParent() const { return m_pParent; }
    bool                        IsAncestor( Frame* pOtherFrame );
    bool                        IsChild( Frame* pOtherFrame );

    // cloned object support
    void                        DisconnectFromParent();    

    // local position
    Vector3                     GetLocalPosition() const { return *(Vector3*)(&m_LocalTransform.r[3]); }
	Vector3*                    GetLocalPositionPointer() const { return (Vector3*)(&m_LocalTransform.r[3]); }
    void                        SetLocalPosition( const VECTOR4& NewPosition ) { SetLocalPosition(Vector3(NewPosition.x,NewPosition.y,NewPosition.z)); }
	void                        SetLocalPosition( Vector3 e_vPos );
   
    // world position
    Vector3                     GetWorldPosition();
    void                        SetWorldPosition( const VECTOR4& NewPosition );
	void                        SetWorldPosition( Vector3 e_vPos ){ SetWorldPosition(VECTOR4Set(e_vPos.x,e_vPos.y,e_vPos.z,1.f)); }

    // local transform
    cMatrix44                   GetLocalTransform() const { return m_LocalTransform; }
    void                        SetLocalTransform( const cMatrix44& LocalTransform,bool e_bAllChildDirty = true );
	cMatrix44*                  GetLocalTransformPointer();
    // world transform
    cMatrix44                   GetWorldTransform();
    void                        SetWorldTransform( const cMatrix44& WorldTransform );
	//cMatrix44*                  GetWorldTransformPointer();

    // basis vectors
    Vector3                     GetLocalRight() const { return *(Vector3*)(&m_LocalTransform.r[0]); }
    Vector3                     GetLocalUp() const { return *(Vector3*)(&m_LocalTransform.r[1]); }
    Vector3                     GetLocalDirection() const { return *(Vector3*)(&m_LocalTransform.r[2]); }      

    Vector3                     GetWorldRight();
    Vector3                     GetWorldUp();
    Vector3                     GetWorldDirection();

    // bound    
    void						SetLocalBound( const cBound* e_pBound );
    const cBound*				GetLocalBound() const { return m_pLocalBound; }
    const cBound*				GetWorldBound() { UpdateCachedWorldTransformIfNeeded(); return m_pCachedWorldBound; }

	//void						SetDestroyConnectionWhileDestoruction(bool e_bDestroyConnectionWhileDestroy);
	bool						IsAutoUpdateBound();
	void						SetAutoUpdateBound(bool e_bAutoUpdateBound);
	
	
	void                        Forward(float e_fDistance);
	//only for first level not all children
	int							GetNextLevelChildrenCount();
	void						SetLocalRotation(Vector3 e_vRotation);

	virtual	void				Update(float e_fElpaseTime){}
	virtual	void				Render(){}
	//for scissor viewport end or post effect or something else...
	//take a look at cRenderObject::RenderNodes
	virtual	void				EndRender() {}
	//virtual	void				AllUpdate(float e_fElpaseTime);
	//virtual	void				AllRender();

	void						DumpDebugInfo();
	GET_SET(bool,m_bIgnoreChildrenUpdate,IsIgnoreChildrenUpdate,SetIgnoreChildrenUpdate);
	GET_SET(bool,m_bIgnoreChildrenRender,IsIgnoreChildrenRender,SetIgnoreChildrenRender);
	GET_SET_DEC(bool,m_bVisible,IsVisible,SetVisible);
protected:
    virtual	void                UpdateCachedWorldTransformIfNeeded();
    void                        SetCachedWorldTransformDirty();

    cMatrix44                   m_LocalTransform;
    cMatrix44                   m_CachedWorldTransform;

    cBound*						m_pLocalBound;
    cBound*						m_pCachedWorldBound;
    
    Frame*                      m_pParent;
    Frame*                      m_pNextSibling;
    Frame*                      m_pFirstChild;
	//bool						m_bDestroyConnectionWhileDestroy;//when destruction is called,SetParent may not call as ue expected
	bool						m_bIgnoreChildrenUpdate;//some object dont want update children,the child has relationship with parent
	bool						m_bIgnoreChildrenRender;
	bool						m_bAutoUpdateBound;//button dont need this
};
//sibling child then pop up back
void	GoThoughAllFrameFromaLastToFirst(std::function<void(void*, Frame*)> e_Function, Frame*e_pFrame, void*e_pData);
void	GoThoughAllFrameFromaFirstToEnd(std::function<void(void*, Frame*)> e_Function, Frame*e_pFrame, void*e_pData);
//only for first child and its sibling
void	GoThoughFirstChildLevel(std::function<void(void*, Frame*)> e_Function, Frame*e_pFrame, void*e_pData);