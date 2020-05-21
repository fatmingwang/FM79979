#include "stdafx.h"
#include "MorphingAnimation.h"

cEditor_MorphingAnimation::sVertexIndexAndPositionAndTimeVector::sVertexIndexAndPositionAndTimeVector()
{
	pPos = nullptr;
}

Vector3 cEditor_MorphingAnimation::sVertexIndexAndPositionAndTimeVector::UpdateAnimationByGlobalTime(float e_fGlobalTime)
{
	if (m_FormKeyFrames.size() == 0 || !pPos)
		return Vector3::Zero;
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

bool cEditor_MorphingAnimation::sVertexIndexAndPositionAndTimeVector::AddKey(float e_fTime)
{
	if (!pPos)
		return false;
	auto l_Iterator = m_FormKeyFrames.find(e_fTime);
	if (l_Iterator != m_FormKeyFrames.end())
	{
		return false;
	}
	l_Iterator = m_FormKeyFrames.lower_bound(e_fTime);
	if (l_Iterator == m_FormKeyFrames.end())
	{
		m_FormKeyFrames[e_fTime] = *pPos;
	}
	else
	{
		m_FormKeyFrames[e_fTime] = l_Iterator->second;
	}
	return true;
}

bool cEditor_MorphingAnimation::sVertexIndexAndPositionAndTimeVector::AssignKeyVector(std::vector<float>& e_fTimeVector)
{
	if (!pPos)
		return false;
	m_FormKeyFrames.clear();
	for (auto l_fTime : e_fTimeVector)
	{
		m_FormKeyFrames[l_fTime] = *this->pPos;
	}
	return true;
}

bool	cEditor_MorphingAnimation::sVertexIndexAndPositionAndTimeVector::DeleteKey(float e_fTime)
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

bool cEditor_MorphingAnimation::sVertexIndexAndPositionAndTimeVector::KeyTime0PositionChange(Vector3 e_vPos)
{
	//find position offset.
	auto l_Iterator = this->m_FormKeyFrames.find(0);
	if (l_Iterator != m_FormKeyFrames.end())
	{
		auto l_vPos = m_FormKeyFrames[0];
		Vector3 l_vOffset = e_vPos-l_vPos;
		for (auto l_ForIterator : m_FormKeyFrames)
		{
			l_ForIterator.second += l_vOffset;
		}
		return true;
	}
	return false;
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

void cEditor_MorphingAnimation::sVertexIndexAndPositionAndTimeVector::RenderPointByTime(float e_fTime, Vector4 e_vColor, float e_fPointSize)
{
	auto l_Iterator = this->m_FormKeyFrames.find(e_fTime);
	if (l_Iterator == m_FormKeyFrames.end())
	{
		GLRender::RenderPoint(l_Iterator->second, e_fPointSize,e_vColor);
	}
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

void cEditor_MorphingAnimation::ApplyVertexIndexChangeForMorphingAnimation(std::map<int, int>&e_ChangedIndexMap)
{
	vMorphingPosVector = this->vRenderPosVector = this->m_pTarget->vPosVector;
	//assert(m_pTarget->vPosVector.size() == e_ChangedIndexMap.size()&&"vertex count shoudl be same!");
	std::vector<sVertexIndexAndPositionAndTimeVector>	l_NewVertexAnimationVector;
	l_NewVertexAnimationVector.resize(e_ChangedIndexMap.size());
	int l_iSizeDiff = (int)e_ChangedIndexMap.size()-(int)m_VertexAnimationVector.size();
	//add dummy data because new points is added
	for(int i=0;i< l_iSizeDiff;++i)
	{
		sVertexIndexAndPositionAndTimeVector l_sVertexIndexAndPositionAndTimeVector;
		l_sVertexIndexAndPositionAndTimeVector.pPos = &this->m_pTarget->vPosVector[m_VertexAnimationVector.size()];
		l_sVertexIndexAndPositionAndTimeVector.AssignKeyVector(this->m_fListboxTimeVector);
		m_VertexAnimationVector.push_back(l_sVertexIndexAndPositionAndTimeVector);
	}
	for (auto l_Iterator : e_ChangedIndexMap)
	{
		if (l_Iterator.second == -1)
		{//deleted vertex

		}
		else
		{
			l_NewVertexAnimationVector[l_Iterator.second] = m_VertexAnimationVector[l_Iterator.first];
		}
	}
	m_VertexAnimationVector = l_NewVertexAnimationVector;
	ReassignRenderPosVectorAfterVertexIndexChange();
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


bool cEditor_MorphingAnimation::ChangeData(int e_iVertexIndex, Vector3 e_vPos, float e_fTime)
{
	if (e_fTime < 0.f)
		return false;
	if(m_VertexAnimationVector.size() > e_iVertexIndex)
		return m_VertexAnimationVector[e_iVertexIndex].ChangeData(e_vPos,e_fTime);
	return false;
}

bool cEditor_MorphingAnimation::ChangeKeyTime0Data(int e_iVertexIndex, Vector3 e_vPos)
{
	if (m_VertexAnimationVector.size() > e_iVertexIndex && e_iVertexIndex > 0)
	{
		return m_VertexAnimationVector[e_iVertexIndex].KeyTime0PositionChange(e_vPos);
	}
	return false;
}

bool cEditor_MorphingAnimation::IsTimeAvaliable(int e_iVertexIndex, float e_fTime)
{
	if (m_VertexAnimationVector.size() > e_iVertexIndex)
		return m_VertexAnimationVector[e_iVertexIndex].IsTimeAvaliable(e_fTime);
	return false;
}

bool cEditor_MorphingAnimation::ApplyEmptyAnimationData()
{
	if (this->m_pTarget)
	{
		bool l_bSameData = false;
		vRenderPosVector = vMorphingPosVector = m_pTarget->vPosVector;
		if (m_VertexAnimationVector.size() != m_pTarget->vPosVector.size())
		{
			m_VertexAnimationVector.clear();
			for (size_t i = 0; i < vRenderPosVector.size(); ++i)
			{
				sVertexIndexAndPositionAndTimeVector l_Data;
				l_Data.pPos = &vRenderPosVector[i];
				for (auto l_fTime : m_fListboxTimeVector)
				{
					l_Data.AddKey(l_fTime);
				}
				m_VertexAnimationVector.push_back(l_Data);
			}
		}
		for(size_t i=0;i< vRenderPosVector.size();++i)
		{
			m_VertexAnimationVector[i].pPos = &vRenderPosVector[i];
		}
		return true;
	}
	return false;
}

void cEditor_MorphingAnimation::UpdateAnimationByGlobalTime(float e_fElpaseTime)
{
	for (auto l_Data : m_VertexAnimationVector)
	{
		if(l_Data.m_FormKeyFrames.size())
			l_Data.UpdateAnimationByGlobalTime(e_fElpaseTime);
	}
}

void cEditor_MorphingAnimation::Render(cMatrix44 e_Mat, cBaseImage * e_pImage)
{
	if (this->m_pTarget)
	{
		e_pImage->ApplyImage();
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
		e_pImage->ApplyImage();
		auto l_iSize = (GLsizei)m_pTarget->vIndexVector.size();
		if (l_iSize)
		{
			int l_iColorSize = (int)m_pTarget->vColorVector.size();
			for (size_t i=0;i< m_VertexAnimationVector.size();++i)
			{
				auto l_Data = m_VertexAnimationVector[i];
				if (l_Data.m_FormKeyFrames.size())
				{
					auto l_Itrtator = l_Data.m_FormKeyFrames.find(e_fElpaseTime);
					if (l_Itrtator == l_Data.m_FormKeyFrames.end())
					{
						UT::ErrorMsg("cEditor_MorphingAnimation::RenderByTimeForHint no such time!!","Error!!");
						Sleep(10);
						return;
					}
					else
					{
						vRenderPosVector[i] = l_Itrtator->second;
					}
				}
			}
			for (size_t i = 0; i < l_iColorSize; ++i)
			{
				m_pTarget->vColorVector[i] = e_vColor;
			}
			RenderVertexByIndexBuffer(e_Mat, 3, (float*)&vRenderPosVector[0], (float*)&this->m_pTarget->vUVVector[0], (float*)&this->m_pTarget->vColorVector[0], (float*)&this->m_pTarget->vIndexVector[0], (int)l_iSize);
			for (size_t i = 0; i < l_iColorSize; ++i)
			{
				m_pTarget->vColorVector[i] = Vector4::One;
			}
			//https://learnopengl-cn.readthedocs.io/zh/latest/04%20Advanced%20OpenGL/08%20Advanced%20GLSL/
			GLRender::RenderPoints(&vRenderPosVector[0], (int)vRenderPosVector.size(), HINT_VERTEX_POINT_SIZE, Vector4::Red);
		}
	}
}

void cEditor_MorphingAnimation::RearrangeTime(float e_fTargetTime)
{
	if (m_fListboxTimeVector.size())
	{
		float l_fEndTime = m_fListboxTimeVector.back();
		float l_fScale = e_fTargetTime / l_fEndTime;
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
	AddKeyTime(e_fTime);
	return true;
}

bool cEditor_MorphingAnimation::DeleteListboxTime(int e_iIndex)
{
	if (m_fListboxTimeVector.size() > e_iIndex)
	{
		auto l_fTime = m_fListboxTimeVector[e_iIndex];
		RemoveKeyTime(l_fTime);
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
	if (m_fListboxTimeVector.size() > e_iIndex)
	{
		m_fCurrentMorphingAnimationTime = m_fListboxTimeVector[e_iIndex];
		return false;
	}
	return false;
}

void cEditor_MorphingAnimation::AddKeyTime(float e_fTime)
{
	for (sVertexIndexAndPositionAndTimeVector&l_Data : m_VertexAnimationVector)
	{
		l_Data.AddKey(e_fTime);
	}
}

void cEditor_MorphingAnimation::RemoveKeyTime(float e_fTime)
{
	for (sVertexIndexAndPositionAndTimeVector&l_Data : m_VertexAnimationVector)
	{
		l_Data.DeleteKey(e_fTime);
	}
}

void cEditor_MorphingAnimation::ReassignRenderPosVectorAfterVertexIndexChange()
{
	for (size_t i = 0; i < vRenderPosVector.size(); ++i)
	{
		m_VertexAnimationVector[i].pPos = &vRenderPosVector[i];
	}
}

float cEditor_MorphingAnimation::GetEndTime()
{
	if (m_fListboxTimeVector.size())
	{
		return m_fListboxTimeVector.back();
	}
	return 0.0f;
}

int cEditor_MorphingAnimation::FinClosestVertexIndex(Vector3 e_vPos)
{
	std::vector<Vector3>l_vCurrentVerticesPosVector;
	for (sVertexIndexAndPositionAndTimeVector&l_Data : this->m_VertexAnimationVector)
	{
		auto l_Iterator = l_Data.m_FormKeyFrames.find(m_fCurrentMorphingAnimationTime);
		if (l_Iterator == l_Data.m_FormKeyFrames.end())
			return -1;
		l_vCurrentVerticesPosVector.push_back(l_Iterator->second);
	}
	return GetClosestPointIndex(e_vPos, &l_vCurrentVerticesPosVector);
}

void cEditor_MorphingAnimation::RenderVertexPointByVertexIndex(int e_iVertexIndex, Vector4 e_vColor, float e_fPointSize)
{
	if (e_iVertexIndex != -1 && m_fCurrentMorphingAnimationTime>0.f)
	{
		if (m_VertexAnimationVector.size() > e_iVertexIndex)
		{
			m_VertexAnimationVector[e_iVertexIndex].RenderPointByTime(this->m_fCurrentMorphingAnimationTime,e_vColor,e_fPointSize);
		}
	}
}

void cEditor_MorphingAnimation::DataCleanUp()
{
	vMorphingPosVector.clear();
	vRenderPosVector.clear();
	m_VertexAnimationVector.clear();
	m_fListboxTimeVector.clear();;
}