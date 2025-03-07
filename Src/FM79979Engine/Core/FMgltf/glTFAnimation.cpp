#include "../AllCoreInclude.h"
#include "glTFAnimation.h"
#include "glTFModel.h"

bool    cAnimationClip::SampleToTime(float e_fTime, bool e_bAssignToBone, std::vector<sSRT>* e_pSRTVector)
{
    if (!this->m_pCurrentAnimationData)
    {
        return false;
    }
    std::vector<sSRT>* l_pSRTVector = &m_SRTVector;
    if (e_pSRTVector)
    {
        l_pSRTVector = e_pSRTVector;
    }
    //std::vector<cglTFNodeData*>& l_pBoneVector = m_pAnimationMesh->m_SkinningBoneVector;
    //auto l_uiSize = l_pBoneVector.size();
    //if (l_uiSize)
    //{
    //    for (size_t i = 0; i < l_uiSize; ++i)
    //    {
    //        UpdateNode(l_pBoneVector[i], e_fTime, (*l_pSRTVector)[i], e_bAssignToBone);
    //    }
    //}
    //else
    {//not skinning animation
        sSRT l_sSRT;
        for (auto l_IT : m_pCurrentAnimationData->m_BoneIDAndAnimationData)
        {
            UpdateNode(l_IT.first, e_fTime, l_sSRT, true);
        }
    }
    return true;
}

void cAnimationClip::UpdateNode(cglTFNodeData* e_pBone, float e_fTime, sSRT& e_SRT, bool e_bAssignToBone)
{
    FloatToSRTMap* l_pFloatToSRTMap = &this->m_pCurrentAnimationData->m_BoneIDAndAnimationData[e_pBone];
    if (l_pFloatToSRTMap->empty())
    {
        sSRT l_sSRT;
        e_SRT = l_sSRT;
        return;
    }

    // Find the keyframes surrounding the current time
    auto it = l_pFloatToSRTMap->lower_bound(e_fTime);

    if (it == l_pFloatToSRTMap->begin()) // If time is before the first keyframe
    {
        e_SRT = it->second;
        return;
    }
    if (it == l_pFloatToSRTMap->end()) // If time is after the last keyframe
    {
        it = std::prev(l_pFloatToSRTMap->end());
    }

    auto nextIt = it;
    auto prevIt = std::prev(it); // Get the previous keyframe

    float prevTime = prevIt->first;
    float nextTime = nextIt->first;

    if (prevTime == nextTime) // Avoid division by zero
    {
        e_SRT = prevIt->second;
        return;
    }

    // Calculate the interpolation factor (clamped between 0 and 1)
    float factor = (e_fTime - prevTime) / (nextTime - prevTime);
    factor = std::clamp(factor, 0.0f, 1.0f);

    // Interpolate SRT values
    const sSRT& prevSRT = prevIt->second;
    const sSRT& nextSRT = nextIt->second;
    sSRT l_CurrentSRT;

    // Translation Interpolation
    if (prevSRT.iSRTFlag & SRT_TRANSLATION_FLAG)
    {
        l_CurrentSRT.vTranslation = prevSRT.vTranslation * (1.0f - factor) + nextSRT.vTranslation * factor;
        l_CurrentSRT.iSRTFlag |= SRT_TRANSLATION_FLAG;
    }

    // Rotation Interpolation using SLERP
    if (prevSRT.iSRTFlag & SRT_ROTATION_FLAG)
    {
        l_CurrentSRT.qRotation = Quaternion::Slerp(prevSRT.qRotation, nextSRT.qRotation, factor);
        l_CurrentSRT.iSRTFlag |= SRT_ROTATION_FLAG;
    }

    // Scale Interpolation
    if (prevSRT.iSRTFlag & SRT_SCALE_FLAG)
    {
        l_CurrentSRT.vScale = prevSRT.vScale * (1.0f - factor) + nextSRT.vScale * factor;
        l_CurrentSRT.iSRTFlag |= SRT_SCALE_FLAG;
    }
    e_SRT = l_CurrentSRT;
    if (e_bAssignToBone)
    {
        e_pBone->SetLocalTransform(e_SRT.GetMatrix());
    }
}

void cAnimationClip::SetBoneAndAnimationData(cglTFModel* e_pglTFModel)
{
    m_pglTFModel = e_pglTFModel;
    m_SRTVector.clear();
    for (auto l_pBone : *e_pglTFModel->m_NodesVector.GetList())
    {
        sSRT l_SRT;
        l_SRT.iSRTFlag = SRT_SCALE_FLAG | SRT_ROTATION_FLAG | SRT_TRANSLATION_FLAG;
        m_SRTVector.push_back(l_SRT);
    }
}

bool cAnimationClip::SetAnimation(const char* e_strAnimationName, bool e_bLoop, float e_fTargetTime)
{
    auto it = m_pglTFModel->m_NameAndAnimationMap.find(e_strAnimationName);
    if (it != m_pglTFModel->m_NameAndAnimationMap.end())
    {
        m_strAnimationName = e_strAnimationName;
        m_pCurrentAnimationData = it->second;
        m_pCurrentAnimationData->m_fCurrentTime = e_fTargetTime;
        m_pCurrentAnimationData->m_bLoop = e_bLoop;
        return true;
    }
    return false;
}

void cAnimationClip::UpdateToTargetTime(float e_fTime, bool e_bAssignToBone)
{
    SampleToTime(e_fTime, true);
}

void    cAnimationClip::BlendClips(float e_fTime, const char* e_strAnimationName1, const char* e_strAnimationName2, bool e_bAssignToBone, bool e_bLoop, float e_fTargetFactor)
{
    float l_fPreviousTime = m_fBlendingTime;
    m_fBlendingTime += e_fTime;
    std::vector<sSRT>   l_SRTVector1 = m_SRTVector;
    std::vector<sSRT>   l_SRTVector2 = m_SRTVector;
    if (!SetAnimation(e_strAnimationName1, e_bLoop, l_fPreviousTime))
    {
        return;
    }
    this->m_pCurrentAnimationData->Update(e_fTime);
    this->SampleToTime(this->m_pCurrentAnimationData->m_fCurrentTime, false, &l_SRTVector1);
    if (!SetAnimation(e_strAnimationName2, e_bLoop, l_fPreviousTime))
    {
        return;
    }
    this->m_pCurrentAnimationData->Update(e_fTime);
    this->SampleToTime(this->m_pCurrentAnimationData->m_fCurrentTime, false, &l_SRTVector2);

    cNamedTypedObjectVector<cglTFNodeData>&l_BoneVector = this->m_pglTFModel->m_NodesVector;
    auto l_iSize = l_BoneVector.Count();
    for (int i = 0; i < l_iSize; ++i)
    {
        m_SRTVector[i] = sSRT::Blend(l_SRTVector1[i], l_SRTVector2[i], e_fTargetFactor);
        l_BoneVector[i]->SetLocalTransform(m_SRTVector[i].GetMatrix());
    }
}

void cAnimationClip::Update(float e_fElpaseTime)
{
    if (this->m_pCurrentAnimationData)
    {
        m_pCurrentAnimationData->Update(e_fElpaseTime);
        this->SampleToTime(this->m_pCurrentAnimationData->m_fCurrentTime, true);
    }
}

void sAnimationData::GenerateNodeVectorFromAnimationData()
{
}

void sAnimationData::Update(float e_fElpaseTime)
{
    float l_fNewTime = m_fCurrentTime + e_fElpaseTime;
    if (l_fNewTime > m_fEndTime)
    {
        if (m_bLoop)
        {
            l_fNewTime = std::fmod(l_fNewTime, m_fEndTime);
        }
        else
        {
            l_fNewTime = m_fEndTime;
        }
    }
    m_fCurrentTime = l_fNewTime;
}
