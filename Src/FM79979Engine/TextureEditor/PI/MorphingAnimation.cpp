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

bool cEditor_MorphingAnimation::sVertexIndexAndPositionAndTimeVector::ChangeData(int e_iDataIndex, Vector3 e_vPos, float e_fTime)
{
	int l_iSize = (int)m_FormKeyFrames.size();
	auto l_Iterator = m_FormKeyFrames.begin();
	for (int i = 0; i < l_iSize; ++i)
	{
		if (i == e_iDataIndex)
		{
			if (m_FormKeyFrames.find(e_fTime) == m_FormKeyFrames.end())
			{
				m_FormKeyFrames.erase(l_Iterator);
				m_FormKeyFrames[e_fTime] = e_vPos;
				return true;
			}
			break;
		}
		++l_Iterator;
	}
	return false;
}

cEditor_MorphingAnimation::cEditor_MorphingAnimation(sTrianglesToDrawIndicesBuffer * e_pTarget)
{
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
		m_VertexAnimationVector[e_iVertexIndex].ChangeData(0, e_vPos,0.f);
	}
}

void cEditor_MorphingAnimation::AddData(int e_iVertexIndex, Vector3 e_vPos, float e_fTime)
{
	m_VertexAnimationVector[e_iVertexIndex].AddData(e_vPos, e_fTime);
}

bool cEditor_MorphingAnimation::DeleteData(int e_iVertexIndex, float e_fTime)
{
	return m_VertexAnimationVector[e_iVertexIndex].DeleteData(e_fTime);
}

bool cEditor_MorphingAnimation::ChangeData(int e_iVertexIndex, int e_iDataIndex, Vector3 e_vPos, float e_fTime)
{
	return m_VertexAnimationVector[e_iVertexIndex].ChangeData(e_iDataIndex,e_vPos,e_fTime);
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
		RenderVertexByIndexBuffer(e_Mat, 3, (float*)&vRenderPosVector[0], (float*)&this->m_pTarget->vUVVector[0], (float*)&this->m_pTarget->vColorVector[0], (float*)&this->m_pTarget->vIndexVector[0], (int)l_iSize);
	}
}
