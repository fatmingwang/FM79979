#include "Frame.h"
#include "../../../Math/Bound.h"
#include "assert.h"
#include "../../../Common/Log/FMLog.h"
#include "../../../Common/Utility.h"
//#include "../GameplayUT/GameApp.h"
namespace FATMING_CORE
{

	// int e_iType:0 for next sibling,1 for first child
	Frame*	GoThoughAllFrameFromaFirstToEndWithClone(Frame*e_pSrcFrame, Frame*e_pDestFrame, int e_iType,bool e_bCallSibling = false)
	{
		if (e_pSrcFrame)
		{
			Frame*l_pCloneFrame = dynamic_cast<Frame*>(e_pSrcFrame->Clone());
			if (e_pDestFrame)
			{
				if (e_iType == 0)
				{
					e_pDestFrame->SetNextSibling(l_pCloneFrame);
				}
				else
				if (e_iType == 1)
				{
					e_pDestFrame->AddChildToLast(l_pCloneFrame);
				}
			}
			Frame*l_pFrame = nullptr;
			if (e_bCallSibling)
			{
				l_pFrame = e_pSrcFrame->GetNextSibling();
				GoThoughAllFrameFromaFirstToEndWithClone(l_pFrame, l_pCloneFrame, 0, e_bCallSibling);
			}
			l_pFrame = e_pSrcFrame->GetFirstChild();
			if (l_pFrame)
			{
				GoThoughAllFrameFromaFirstToEndWithClone(l_pFrame, l_pCloneFrame, 1,true);
			}
			return l_pCloneFrame;
		}
		return nullptr;
	}

	void Frame::DestoryWithChildren()
	{
		Frame*l_pMe = this;
		GoThoughAllFrameFromaLastToFirst(
			[l_pMe](void*e_pData, Frame*e_pFrame)
			{
				if (e_pFrame != l_pMe)
				{
					delete e_pFrame;
				}
			}
		, this, this);
	}

	//-----------------------------------------------------------------------------
	// Name: Frame::Frame()
	//-----------------------------------------------------------------------------
	Frame::Frame()
	{
		m_bVisible = true;
		m_bIgnoreChildrenRender = false;
		m_bIgnoreChildrenUpdate = false;
		m_pParent = nullptr;
		m_pNextSibling = nullptr;
		m_pFirstChild = nullptr;

		m_CachedWorldTransform._11 = FRAME_DIRTY_WORLD_CACHE;
		m_LocalTransform = cMatrix44::Identity;
		//m_bDestroyConnectionWhileDestroy =  true;
		m_pLocalBound = nullptr;
		m_pCachedWorldBound = nullptr;
		m_bAutoUpdateBound = true;
	}

	Frame::Frame(Frame*e_pFrame)
	{
		m_bVisible = e_pFrame->m_bVisible;
		m_bIgnoreChildrenRender = e_pFrame->m_bIgnoreChildrenRender;
		m_bIgnoreChildrenUpdate = e_pFrame->m_bIgnoreChildrenUpdate;
		m_pLocalBound = nullptr;
		m_pCachedWorldBound = nullptr;
		m_pParent = nullptr;
		m_pNextSibling = nullptr;
		m_pFirstChild = nullptr;
		m_bAutoUpdateBound = e_pFrame->m_bAutoUpdateBound;
		m_LocalTransform = e_pFrame->m_LocalTransform;
		if (e_pFrame->m_pLocalBound)
		{
			this->SetLocalBound(e_pFrame->m_pLocalBound);
		}
		m_CachedWorldTransform._11 = FRAME_DIRTY_WORLD_CACHE;
		//m_bDestroyConnectionWhileDestroy =  true;
		SetName(e_pFrame->GetName());
		this->SetLocalTransform(e_pFrame->GetLocalTransform());
	}
	//-----------------------------------------------------------------------------
	// Name: Frame::~Frame()
	//-----------------------------------------------------------------------------
	Frame::~Frame()
	{
		SetParent(nullptr);
		DestoryWithChildren();
		SAFE_DELETE(m_pLocalBound);
		SAFE_DELETE(m_pCachedWorldBound);
	}

	Frame*	Frame::CloneFrameWithHierarchy()
	{
		Frame*l_pRoot = GoThoughAllFrameFromaFirstToEndWithClone(this, nullptr, -1);
		return l_pRoot;
	}

	//Frame*l_pFrameTest = new Frame();
	//l_pFrameTest->SetName(L"Root");
	//Frame*l_pFrameTest7 = new Frame();
	//l_pFrameTest7->SetName(L"_1");
	//Frame*l_pFrameTest1 = new Frame();
	//l_pFrameTest1->SetName(L"_2");
	//l_pFrameTest->AddChild(l_pFrameTest7);
	//l_pFrameTest->AddChild(l_pFrameTest1);
	//Frame*l_pFrameTest2 = new Frame();
	//l_pFrameTest2->SetName(L"_1_1");
	//Frame*l_pFrameTest3 = new Frame();
	//l_pFrameTest3->SetName(L"_1_2");
	//l_pFrameTest7->AddChild(l_pFrameTest2);
	//l_pFrameTest7->AddChild(l_pFrameTest3);
	//Frame*l_pFrameTest4 = new Frame();
	//l_pFrameTest4->SetName(L"_2_1");
	//l_pFrameTest1->AddChild(l_pFrameTest4);
	//Frame*l_pFrameTest5 = new Frame();
	//l_pFrameTest5->SetName(L"_2_1_1");
	//Frame*l_pFrameTest6 = new Frame();
	//l_pFrameTest6->SetName(L"_2_1_2");
	//l_pFrameTest4->AddChild(l_pFrameTest5);
	//l_pFrameTest4->AddChild(l_pFrameTest6);
	//Frame::DestoryWithChildren(l_pFrameTest);
	void	Frame::DestoryWithChildren(Frame*e_pFrame, bool e_bDoNextSibling)
	{
		if (e_pFrame)
		{
			//e_pFrame->m_bDestroyConnectionWhileDestroy = false;
			//should not do this but I am lazy to fix this now
			if (e_bDoNextSibling)
			{
				DestoryWithChildren(e_pFrame->GetNextSibling(), e_bDoNextSibling);
			}
			//??why,because MPDI...MPDI will kill child by itself.
			//if( !e_pFrame->IsIgnoreChildrenUpdate() )
			{
				DestoryWithChildren(e_pFrame->GetFirstChild(),true);
			}
			FMLog::Log(e_pFrame->GetName(),false);
			SAFE_DELETE(e_pFrame);
		}
	}

	void	Frame::SetLocalRotation(Vector3 e_vRotation)
	{
		cMatrix44	l_RotationMatrix = cMatrix44::RotationMatrix(e_vRotation);
		l_RotationMatrix.SetTranslation(this->GetLocalPosition());
		this->SetLocalTransform(l_RotationMatrix);
	}


	Frame*	Frame::FinFrameByName(const wchar_t*e_strName, bool e_bDoNextSibling)
	{
		if (!wcscmp(e_strName, GetName()))
		{
			return this;
		}
		if (e_bDoNextSibling && this->GetNextSibling() != nullptr)
		{
			return GetNextSibling()->FinFrameByName(e_strName, e_bDoNextSibling);
		}

		if (GetFirstChild() != nullptr)
		{
			return GetFirstChild()->FinFrameByName(e_strName);
		}
		return nullptr;
	}
	//-----------------------------------------------------------------------------
	// Name: Frame::AddChild()
	//-----------------------------------------------------------------------------
	void Frame::AddChild(Frame* pChild, bool e_bUpdateRelatedPosition)
	{
		assert(pChild);
		pChild->SetParent(this, e_bUpdateRelatedPosition);
		pChild->SetCachedWorldTransformDirty();
	}

	void Frame::AddChildToLast(Frame* pChild, bool e_bUpdateRelatedPosition)
	{
		assert(pChild);
		Frame*l_pFirstChild = this->GetFirstChild();
		pChild->SetParent(nullptr, false);
		pChild->m_pParent = this;
		if (l_pFirstChild)
		{
			while (l_pFirstChild)
			{
				if (l_pFirstChild->GetNextSibling())
				{
					l_pFirstChild = l_pFirstChild->GetNextSibling();
				}
				else
				{
					l_pFirstChild->m_pNextSibling = pChild;
					break;
				}
			}
		}
		else
		{
			this->m_pFirstChild = pChild;
		}
		pChild->SetCachedWorldTransformDirty();
	}


	//-----------------------------------------------------------------------------
	// Name: Frame::SetParent()
	//-----------------------------------------------------------------------------
	void Frame::SetParent(Frame* pParent, bool e_bUpdateRelatedPosition)
	{
		Frame *pSrch, *pLast;

		UpdateCachedWorldTransformIfNeeded();
		if (m_pParent == pParent)
			return;
		if (m_pParent)
		{
			pLast = nullptr;
			for (pSrch = m_pParent->m_pFirstChild; pSrch; pSrch = pSrch->m_pNextSibling)
			{
				if (pSrch == this)
					break;
				pLast = pSrch;
			}

			// If we can't find this frame in the old parent's list, assert
			assert(pSrch);

			// Remove us from the list
			if (pLast)
			{
				pLast->m_pNextSibling = m_pNextSibling;
			}
			else // at the beginning of the list
			{
				m_pParent->m_pFirstChild = m_pNextSibling;
			}

			m_pNextSibling = nullptr;
			m_pParent = nullptr;
		}

		// Add us to the new parent's list               
		if (pParent)
		{
			m_pParent = pParent;
			m_pNextSibling = pParent->m_pFirstChild;
			pParent->m_pFirstChild = this;
		}

		// now we update our local transform to match the old world transform
		// (i.e. move under our new parent's frame with no detectable change)
		//if (e_bUpdateRelatedPosition)
		//{
		//	this->GetWorldTransform();
		//}
		//else
		//{
			SetCachedWorldTransformDirty();
		//}
	}


	void	Frame::SetNextSibling(Frame*e_pNextSibling)
	{
		m_pNextSibling = e_pNextSibling;
		if (m_pNextSibling)
			m_pNextSibling->m_pParent = this->m_pParent;
	}

	//-----------------------------------------------------------------------------
	// Name: Frame::IsAncestor()
	// Desc: Returns TRUE if the specified frame is an ancestor of this frame
	//       ancestor = parent, parent->parent, etc.
	//       Also returns TRUE if specified frame is nullptr.
	//-----------------------------------------------------------------------------
	bool Frame::IsAncestor(Frame* pOtherFrame)
	{
		if (pOtherFrame == nullptr)
			return true;
		if (pOtherFrame == this)
			return false;
		Frame* pFrame = GetParent();
		while (pFrame != nullptr)
		{
			if (pFrame == pOtherFrame)
				return true;
			pFrame = pFrame->GetParent();
		}
		return false;
	}


	//-----------------------------------------------------------------------------
	// Name: Frame::IsChild()
	// Desc: Returns TRUE if the specified frame is a direct child of this frame
	//-----------------------------------------------------------------------------
	bool Frame::IsChild(Frame* pOtherFrame)
	{
		if (pOtherFrame == nullptr)
			return false;
		Frame* pChild = GetFirstChild();
		while (pChild != nullptr)
		{
			if (pChild == pOtherFrame)
			{
				return true;
			}
			pChild = pChild->GetNextSibling();
		}
		return false;
	}


	void    Frame::SetLocalPosition(Vector3 e_vPos)
	{
		SetCachedWorldTransformDirty();
		m_LocalTransform.r[3] = VECTOR4Set(e_vPos.x, e_vPos.y, e_vPos.z, 1.f);
		SetTransformInternalData();
	}

	//-----------------------------------------------------------------------------
	// Name: Frame::GetWorldPosition()
	//-----------------------------------------------------------------------------
	Vector3 Frame::GetWorldPosition()
	{
		UpdateCachedWorldTransformIfNeeded();
		return *(Vector3*)(&m_CachedWorldTransform.r[3]);
	}


	//-----------------------------------------------------------------------------
	// Name: Frame::SetWorldPosition()
	//-----------------------------------------------------------------------------
	void Frame::SetWorldPosition(const VECTOR4 &NewPosition)
	{
		UpdateCachedWorldTransformIfNeeded();
		cMatrix44 ModifiedWorldTransform = m_CachedWorldTransform;

		ModifiedWorldTransform.r[3] = NewPosition;

		SetWorldTransform(ModifiedWorldTransform);
	}


	//-----------------------------------------------------------------------------
	// Name: Frame::GetWorldTransform()
	//-----------------------------------------------------------------------------
	cMatrix44 Frame::GetWorldTransform()
	{
		UpdateCachedWorldTransformIfNeeded();
		return m_CachedWorldTransform;
	}

	void	Frame::SetLocalTransform(const cMatrix44& LocalTransform, bool e_bAllChildDirty)
	{
		if (e_bAllChildDirty)
		{
			SetCachedWorldTransformDirty();
		}
		m_LocalTransform = LocalTransform;
		SetTransformInternalData();
	}

	cMatrix44*      Frame::GetLocalTransformPointer()
	{
		return &m_LocalTransform;
	}

	//cMatrix44*	Frame::GetWorldTransformPointer()
	//{
	//    UpdateCachedWorldTransformIfNeeded();
	//    return &m_CachedWorldTransform;
	//}

	//-----------------------------------------------------------------------------
	// Name: Frame::GetWorldRight()
	//-----------------------------------------------------------------------------
	Vector3 Frame::GetWorldRight()
	{
		UpdateCachedWorldTransformIfNeeded();
		return *(Vector3*)(&m_CachedWorldTransform.r[0]);
	}


	//-----------------------------------------------------------------------------
	// Name: Frame::GetWorldUp()
	//-----------------------------------------------------------------------------
	Vector3 Frame::GetWorldUp()
	{
		UpdateCachedWorldTransformIfNeeded();
		return *(Vector3*)(&m_CachedWorldTransform.r[1]);
	}


	//-----------------------------------------------------------------------------
	// Name: Frame::GetWorldDirection()
	//-----------------------------------------------------------------------------
	Vector3 Frame::GetWorldDirection()
	{
		UpdateCachedWorldTransformIfNeeded();
		return *(Vector3*)(&m_CachedWorldTransform.r[2]);
	}


	//-----------------------------------------------------------------------------
	// Name: Frame::SetWorldTransform()
	//-----------------------------------------------------------------------------
	void Frame::SetWorldTransform(const cMatrix44& WorldTransform)
	{
		cMatrix44 ParentInverse;
		//Vector4 vDeterminant;

		if (m_pParent)
		{
			m_pParent->UpdateCachedWorldTransformIfNeeded();
			ParentInverse = m_pParent->m_CachedWorldTransform.Inverted();

			//if the transform is not correct check this function,because I am lazy to check it's correction
			//ParentInverse = cMatrix44Inverse( &vDeterminant, m_pParent->m_CachedWorldTransform );
			// Parent's matrix should be invertible
			//assert( vDeterminant.x != 0.0f );

			m_LocalTransform = ParentInverse * WorldTransform;
		}
		else
		{
			m_LocalTransform = WorldTransform;
		}

		SetCachedWorldTransformDirty();
		SetTransformInternalData();
	}

	void Frame::GetAllInverseWorldTransform(std::map<Frame*,cMatrix44>&e_InverseTransformMap)
	{
		auto l_Pair = std::make_pair(this, GetWorldTransform().Inverted());
		e_InverseTransformMap.insert(l_Pair);
		auto l_pFrame = GetFirstChild();
		if (l_pFrame)
		{
			l_pFrame->GetAllInverseWorldTransform(e_InverseTransformMap);
		}
		l_pFrame = GetNextSibling();
		if (l_pFrame)
		{
			l_pFrame->GetAllInverseWorldTransform(e_InverseTransformMap);
		}
	}

	void	Frame::GetAllInverseWorldTransform(std::vector<cMatrix44>& e_InverseTransformVector)
	{
		e_InverseTransformVector.push_back(GetWorldTransform().Inverted());
		auto l_pFrame = GetFirstChild();
		if (l_pFrame)
		{
			l_pFrame->GetAllInverseWorldTransform(e_InverseTransformVector);
		}
		l_pFrame = GetNextSibling();
		if (l_pFrame)
		{
			l_pFrame->GetAllInverseWorldTransform(e_InverseTransformVector);
		}
	}


	//-----------------------------------------------------------------------------
	// Name: Frame::UpdateCachedWorldTransformIfNeeded()
	//-----------------------------------------------------------------------------
	void Frame::UpdateCachedWorldTransformIfNeeded()
	{
		if (m_CachedWorldTransform._11 == FRAME_DIRTY_WORLD_CACHE)
		{
			if (m_pParent)
			{
				m_pParent->UpdateCachedWorldTransformIfNeeded();
				m_CachedWorldTransform = m_pParent->m_CachedWorldTransform * m_LocalTransform;
				if (m_pLocalBound && m_bAutoUpdateBound)
				{
					*m_pCachedWorldBound = (*m_pLocalBound) * m_CachedWorldTransform;
				}
				TransformChangedInternalData();
			}
			else
			{
				m_CachedWorldTransform = m_LocalTransform;
				if (m_pLocalBound && m_bAutoUpdateBound)
				{
					if (m_pLocalBound->GetType() != 0)
						*m_pCachedWorldBound = (*m_pLocalBound) * m_CachedWorldTransform;
				}
				TransformChangedInternalData();
			}
		}
	}


	//-----------------------------------------------------------------------------
	// Name: Frame::SetCachedWorldTransformDirty()
	//-----------------------------------------------------------------------------
	void Frame::SetCachedWorldTransformDirty()
	{
		//float* l_pAddress = &m_CachedWorldTransform._11;
		if (m_CachedWorldTransform._11 != FRAME_DIRTY_WORLD_CACHE)
		{
			Frame *pChild;
			m_CachedWorldTransform._11 = FRAME_DIRTY_WORLD_CACHE;

			for (pChild = m_pFirstChild; pChild; pChild = pChild->m_pNextSibling)
			{
				pChild->SetCachedWorldTransformDirty();
			}
		}
	}


	//-----------------------------------------------------------------------------
	// Name: Frame::DisconnectFromParent
	// Desc: Removes parent link from the object, and converts world transform
	//       to local transform.  This method is only to be used on clones of
	//       scene objects, since the parent is not notified of the disconnection.
	//-----------------------------------------------------------------------------
	void Frame::DisconnectFromParent()
	{
		// Debug check to make sure the parent really isn't the parent of this clone
#ifdef DEBUG
		if (m_pParent != nullptr)
		{
			assert(!m_pParent->IsChild(this));
		}
#endif

		UpdateCachedWorldTransformIfNeeded();
		m_pParent = nullptr;
		SetWorldTransform(m_CachedWorldTransform);
	}

	void	Frame::SetLocalBound(const cBound* e_pBound)
	{
		if (e_pBound == nullptr)
		{
			SAFE_DELETE(m_pLocalBound);
			SAFE_DELETE(m_pCachedWorldBound);
			return;
		}
		if (!m_pLocalBound)
		{
			m_pLocalBound = new cBound();
			m_pCachedWorldBound = new cBound();
		}
		*m_pLocalBound = *e_pBound;
		*m_pCachedWorldBound = *m_pLocalBound;
		SetCachedWorldTransformDirty();
	}

	//void	Frame::SetDestroyConnectionWhileDestoruction(bool e_bDestroyConnectionWhileDestroy)
	//{
	//	m_bDestroyConnectionWhileDestroy = e_bDestroyConnectionWhileDestroy;
	//}

	bool	Frame::IsAutoUpdateBound()
	{
		return this->m_bAutoUpdateBound;
	}
	void	Frame::SetAutoUpdateBound(bool e_bAutoUpdateBound)
	{
		m_bAutoUpdateBound = e_bAutoUpdateBound;
	}

	void    Frame::Forward(float e_fDistance)
	{
		Vector3 l_vDirection = this->GetLocalDirection();
		this->SetWorldPosition(this->GetWorldPosition() + l_vDirection * e_fDistance);
	}

	int		Frame::GetNextLevelChildrenCount()
	{
		int	l_iCount = 0;
		Frame*l_pFrame = m_pFirstChild;
		while (l_pFrame)
		{
			++l_iCount;
			l_pFrame = l_pFrame->m_pNextSibling;
		}
		return l_iCount;
	}

	//void	Frame::AllUpdate(float e_fElpaseTime)
	//{
	//	this->Update(e_fElpaseTime);
	//	if( this->m_pNextSibling )
	//	{
	//		m_pNextSibling->AllUpdate(e_fElpaseTime);
	//	}
	//	if(this->m_pFirstChild)
	//	{
	//		m_pFirstChild->AllUpdate(e_fElpaseTime);
	//	}
	//}
	//
	//void	Frame::AllRender()
	//{
	//	this->Render();
	//	if( this->m_pNextSibling )
	//	{
	//		m_pNextSibling->Render();
	//	}
	//	if(this->m_pFirstChild)
	//	{
	//		m_pFirstChild->Render();
	//	}
	//}

	void	Frame::DumpDebugInfo(bool e_bDoNextSibling,bool e_bRoot)
	{
		Frame*l_pParentNode = GetParent();
		static int	l_siCount = 0;
		++l_siCount;
		std::wstring l_strDebugInfo;
		while (l_pParentNode)
		{
			l_strDebugInfo += L"-----";
			//FMLog::LogWithFlag(L"-----",false,false);
			l_pParentNode = l_pParentNode->GetParent();
		}
		//WCHAR	l_str[MAX_PATH];
		//swprintf(l_str,MAX_PATH,L"Name:%ls\n",GetName());
		//if (this->m_bVisible)
		{
			l_strDebugInfo += GetName();
			//FMLog::LogWithFlag(l_str, CORE_LOG_FLAG);
			FMLog::Log(l_strDebugInfo.c_str(), false);
			if (GetFirstChild())
			{
				GetFirstChild()->DumpDebugInfo(true,false);
			}
		}

		if (e_bDoNextSibling && GetNextSibling())
		{
			GetNextSibling()->DumpDebugInfo(e_bDoNextSibling, false);
		}
		if (e_bRoot)
		{
			FMLOG("Total count:%d", l_siCount);
			l_siCount = 0;
		}
	}

	void	GoThoughAllFrameFromaLastToFirst(std::function<void(void*, Frame*)> e_Function, Frame*e_pFrame, void*e_pData, bool e_bDoNextSibling)
	{
		if (e_pFrame)
		{
			//why? I donno it should be wrong,don't need to check next sibling
			auto l_pFrame = e_pFrame->GetNextSibling();
			if (e_bDoNextSibling && l_pFrame)
			{
				GoThoughAllFrameFromaLastToFirst(e_Function, l_pFrame, e_pData, e_bDoNextSibling);
			}
			l_pFrame = e_pFrame->GetFirstChild();
			if (l_pFrame)
			{
				GoThoughAllFrameFromaLastToFirst(e_Function, l_pFrame, e_pData,true);
			}
			e_Function(e_pData, e_pFrame);
		}
	}

	void	GoThoughAllFrameFromaFirstToEnd(std::function<void(void*, Frame*)> e_Function, Frame*e_pFrame, void*e_pData, bool e_bDoNextSibling)
	{
		if (e_pFrame)
		{
			e_Function(e_pData, e_pFrame);
			auto l_pFrame = e_pFrame->GetNextSibling();
			if (e_bDoNextSibling && l_pFrame)
			{
				GoThoughAllFrameFromaFirstToEnd(e_Function, l_pFrame, e_pData, e_bDoNextSibling);
			}
			l_pFrame = e_pFrame->GetFirstChild();
			if (l_pFrame)
			{
				GoThoughAllFrameFromaFirstToEnd(e_Function, l_pFrame, e_pData, true);
			}
		}
	}

	void GoThoughFirstChildLevel(std::function<void(void*, Frame*)> e_Function, Frame * e_pFrame, void * e_pData)
	{
		if (e_pFrame)
		{
			auto l_pChild = e_pFrame->GetFirstChild();
			while (l_pChild)
			{
				e_Function(e_pData, l_pChild);
				l_pChild = l_pChild->GetNextSibling();
			}
		}
	}
//end namespace FATMING_CORE
}