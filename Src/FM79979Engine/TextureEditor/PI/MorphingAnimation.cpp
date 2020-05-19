#include "stdafx.h"
#include "MorphingAnimation.h"

cEditor_MorphingAnimation::sVertexIndexAndPositionAndTimeVector::sVertexIndexAndPositionAndTimeVector()
{
	pPos = nullptr;
	iVertexIndex = -1;
}

Vector3 cEditor_MorphingAnimation::sVertexIndexAndPositionAndTimeVector::UpdateAnimationByGlobalTime(float e_fGlobalTime)
{
	//find keyframe before and after and do linear interpolation between them
	FloatTocVector3Map::iterator prevKey(m_FormKeyFrames.lower_bound(e_fGlobalTime));
	FloatTocVector3Map::iterator nextKey(prevKey);
	if ((prevKey == m_FormKeyFrames.end()) || ((prevKey != m_FormKeyFrames.begin()) && (prevKey->first > e_fGlobalTime)))
		--prevKey;
	// because m_FormKeyFrames.size() != 0 prevKey should be a valid iterator here
	assert(prevKey != m_FormKeyFrames.end());

	if ((prevKey == nextKey) || (nextKey == m_FormKeyFrames.end()))
	{
		*pPos = prevKey->second;
		return prevKey->second;
	}

	float time0 = prevKey->first;
	float time1 = nextKey->first;
	float timeRange = time1 - time0;
	float l_fTimeDis = (e_fGlobalTime - time0) / timeRange;

	const Vector3& m0 = prevKey->second;
	const Vector3& m1 = nextKey->second;
	auto l_vPos = (m1 - m0)*l_fTimeDis + m0;
	*pPos = l_vPos;
	return l_vPos;
}

void cEditor_MorphingAnimation::sVertexIndexAndPositionAndTimeVector::AssignPositionPointer(Vector3 * e_pAnimationPositionTarget, int e_iVertexIndex)
{
	pPos = e_pAnimationPositionTarget;
	iVertexIndex = e_iVertexIndex;
}

void cEditor_MorphingAnimation::sVertexIndexAndPositionAndTimeVector::AddData(Vector3 e_vPos, float e_fTime)
{
	m_FormKeyFrames[e_fTime] = e_vPos;
}

bool	cEditor_MorphingAnimation::sVertexIndexAndPositionAndTimeVector::DeleteData(float e_fTime)
{
	auto l_Iterator = m_FormKeyFrames.find(e_fTime);
	if (l_Iterator != m_FormKeyFrames.end())
	{
		m_FormKeyFrames.erase(l_Iterator);
		return true;
	}
	return false;
}

bool cEditor_MorphingAnimation::sVertexIndexAndPositionAndTimeVector::ChangeData(Vector3 e_vPos, float e_fTime)
{
	m_FormKeyFrames[e_fTime] = e_vPos;
	return true;
}

bool cEditor_MorphingAnimation::sVertexIndexAndPositionAndTimeVector::IsTimeAvaliable(float e_fTime)
{
	if (m_FormKeyFrames.find(e_fTime) == m_FormKeyFrames.end())
		return false;
	return true;
}

void cEditor_MorphingAnimation::sVertexIndexAndPositionAndTimeVector::RearrangeTime(float e_fScale)
{
	FloatTocVector3Map l_NewFormKeyFrames;
	for (auto l_Data : m_FormKeyFrames)
	{
		l_NewFormKeyFrames[l_Data.first*e_fScale] = l_Data.second;
	}
	m_FormKeyFrames = l_NewFormKeyFrames;
}

void cEditor_MorphingAnimation::RearrangeTimeByScale(float e_fScale)
{
	for (int i = 0; i < (int)m_fListboxTimeVector.size(); ++i)
	{
		m_fListboxTimeVector[i] = e_fScale * m_fListboxTimeVector[i];
	}
	for (auto l_Data : m_VertexAnimationVector)
	{
		l_Data.RearrangeTime(e_fScale);
	}
}

cEditor_MorphingAnimation::cEditor_MorphingAnimation(sTrianglesToDrawIndicesBuffer * e_pTarget)
{
	m_fCurrentMorphingAnimationTime = -1.f;
	m_pTarget = e_pTarget;
}

void cEditor_MorphingAnimation::ReassignVertexIndexData(std::map<int, int>&e_ChangedIndexMap)
{
	std::vector<sVertexIndexAndPositionAndTimeVector>	l_NewVertexAnimationVector;
	l_NewVertexAnimationVector.resize(e_ChangedIndexMap.size());
	for (auto l_Iterator : e_ChangedIndexMap)
	{
		l_NewVertexAnimationVector[l_Iterator.second] = m_VertexAnimationVector[l_Iterator.first];
	}
	m_VertexAnimationVector = l_NewVertexAnimationVector;
}

void cEditor_MorphingAnimation::DeleteVertexIndexData(int e_iVertexIndex)
{
	m_VertexAnimationVector.erase(m_VertexAnimationVector.begin() + e_iVertexIndex);
}

void cEditor_MorphingAnimation::VertexMove(int e_iVertexIndex, Vector3 e_vPos)
{
	vMorphingPosVector[e_iVertexIndex] = e_vPos;
	if (m_VertexAnimationVector[e_iVertexIndex].m_FormKeyFrames.size())
	{
		m_VertexAnimationVector[e_iVertexIndex].ChangeData(e_vPos,0.f);
	}
}

void cEditor_MorphingAnimation::AddData(int e_iVertexIndex, Vector3 e_vPos, float e_fTime)
{
	m_VertexAnimationVector[e_iVertexIndex].AddData(e_vPos, e_fTime);
}

bool cEditor_MorphingAnimation::DeleteData(int e_iVertexIndex, float e_fTime)
{
	if (e_fTime < 0.f)
		return false;
	return m_VertexAnimationVector[e_iVertexIndex].DeleteData(e_fTime);
}

bool cEditor_MorphingAnimation::ChangeData(int e_iVertexIndex, Vector3 e_vPos, float e_fTime)
{
	if (e_fTime < 0.f)
		return false;
	if(m_VertexAnimationVector.size() > e_iVertexIndex)
		return m_VertexAnimationVector[e_iVertexIndex].ChangeData(e_vPos,e_fTime);
	return false;
}

bool cEditor_MorphingAnimation::IsTimeAvaliable(int e_iVertexIndex, float e_fTime)
{
	if (m_VertexAnimationVector.size() > e_iVertexIndex)
		return m_VertexAnimationVector[e_iVertexIndex].IsTimeAvaliable(e_fTime);
	return false;
}

bool cEditor_MorphingAnimation::ApplyData()
{
	if (this->m_pTarget)
	{
		bool l_bSameData = false;
		vRenderPosVector = vMorphingPosVector = m_pTarget->vPosVector;
		return true;
	}
	return false;
}

void cEditor_MorphingAnimation::UpdateAnimationByGlobalTime(float e_fElpaseTime)
{
	for (auto l_Data : m_VertexAnimationVector)
	{
		l_Data.UpdateAnimationByGlobalTime(e_fElpaseTime);
	}
}

void cEditor_MorphingAnimation::Render(cMatrix44 e_Mat, cBaseImage * e_pImage)
{
	if (this->m_pTarget)
	{
		auto l_iSize = (GLsizei)m_pTarget->vIndexVector.size();
		if (l_iSize)
		{
			RenderVertexByIndexBuffer(e_Mat, 3, (float*)&vRenderPosVector[0], (float*)&this->m_pTarget->vUVVector[0], (float*)&this->m_pTarget->vColorVector[0], (float*)&this->m_pTarget->vIndexVector[0], (int)l_iSize);
		}
	}
}

void cEditor_MorphingAnimation::RenderByTimeForHint(float e_fElpaseTime, Vector4 e_vColor,cMatrix44 e_Mat, cBaseImage * e_pImage)
{
	if (this->m_pTarget)
	{
		auto l_iSize = (GLsizei)m_pTarget->vIndexVector.size();
		if (l_iSize)
		{
			for (auto l_Data : m_VertexAnimationVector)
			{
				l_Data.UpdateAnimationByGlobalTime(e_fElpaseTime);
			}
			for (size_t i = 0; i < l_iSize; ++i)
			{
				m_pTarget->vColorVector[i] = e_vColor;
			}
			RenderVertexByIndexBuffer(e_Mat, 3, (float*)&vRenderPosVector[0], (float*)&this->m_pTarget->vUVVector[0], (float*)&this->m_pTarget->vColorVector[0], (float*)&this->m_pTarget->vIndexVector[0], (int)l_iSize);
			for (size_t i = 0; i < l_iSize; ++i)
			{
				m_pTarget->vColorVector[i] = Vector4::One;
			}
		}
	}
}

void cEditor_MorphingAnimation::RearrangeTime(float e_fTargetTime)
{
	if (m_fListboxTimeVector.size())
	{
		float l_fEndTime = m_fListboxTimeVector.back();
		float l_fScale = e_fTargetTime/l_fEndTime;
		RearrangeTimeByScale(l_fScale);
	}
}

bool cEditor_MorphingAnimation::AddListboxTime(float e_fTime)
{
	if (m_fListboxTimeVector.size())
	{
		if (e_fTime <= m_fListboxTimeVector[m_fListboxTimeVector.size() - 1])
			return false;
	}
	m_fListboxTimeVector.push_back(e_fTime);
	return true;
}

bool cEditor_MorphingAnimation::DeleteListboxTime(int e_iIndex)
{
	if (m_fListboxTimeVector.size() > e_iIndex)
	{
		m_fListboxTimeVector.erase(m_fListboxTimeVector.begin() + e_iIndex);
		return true;
	}
	return false;
}

bool cEditor_MorphingAnimation::ChangeListboxTime(int e_iIndex, float e_fTime)
{
	if (m_fListboxTimeVector.size() > e_iIndex)
	{
		m_fListboxTimeVector[e_iIndex] = e_fTime;
		return true;
	}
	return false;
}

bool cEditor_MorphingAnimation::SetCurrentListboxTime(int e_iIndex)
{
	if (m_fListboxTimeVector.size()> e_iIndex)
	{
		m_fCurrentMorphingAnimationTime = m_fListboxTimeVector[e_iIndex];
		return false;
	}
	return false;
}

float cEditor_MorphingAnimation::GetEndTime()
{
	if (m_fListboxTimeVector.size())
	{
		return m_fListboxTimeVector.back();
	}
	return 0.0f;
}
