
#include "tiny_gltf.h"
#include "glTFBone.h"
#include "glTFAnimationMesh.h"



cMatrix44 sSRT::GetMatrix()
{
    cMatrix44 l_Mat = cMatrix44::Identity;
    if (iSRTFlag & SRT_TRANSLATION_FLAG) // Translation
    {
        l_Mat *= cMatrix44::TranslationMatrix(vTranslation);
    }
    if (iSRTFlag & SRT_ROTATION_FLAG) // Rotation
    {
        l_Mat *= qRotation.ToMatrix();
    }
    if (iSRTFlag & SRT_SCALE_FLAG) // Scale
    {
        l_Mat *= cMatrix44::ScaleMatrix(vScale);
    }
    return l_Mat;
}

sSRT sSRT::Blend(sSRT& e_Source1, sSRT& e_Source2, float e_fFactor)
{
    Quaternion l_Quaternion = e_Source2.qRotation;
    if (Quaternion::Dot(e_Source1.qRotation, l_Quaternion) < 0.0f)
    {
        l_Quaternion = ~l_Quaternion;
    }
    sSRT l_SRT;
    l_SRT.iSRTFlag = e_Source1.iSRTFlag | e_Source2.iSRTFlag;
    l_SRT.vScale = Vector3Lerp(e_Source1.vScale, e_Source2.vScale, e_fFactor);
    l_SRT.qRotation = Quaternion::Slerp(e_Source1.qRotation, l_Quaternion, e_fFactor);
    l_SRT.vTranslation = Vector3Lerp(e_Source1.vTranslation, e_Source2.vTranslation, e_fFactor);
    return l_SRT;
}

void ConvertSRTMapToMatrixMap(const FloatToSRTMap& srtMap, FloatTocMatrix44Map& matrixMap)
{
    matrixMap.clear();

    for (const auto& [time, srt] : srtMap)
    {
        cMatrix44 mat = cMatrix44::Identity;
        if (srt.iSRTFlag & SRT_TRANSLATION_FLAG)
        {
            // Construct translation matrix
            float l_fTransArray[] =
            {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                srt.vTranslation.x, srt.vTranslation.y, srt.vTranslation.z, 1
            };
            cMatrix44 transMat(l_fTransArray);
            mat = transMat;
        }
        if (srt.iSRTFlag & SRT_ROTATION_FLAG)
        {
            // Construct rotation matrix from quaternion
            float x = srt.qRotation.x, y = srt.qRotation.y, z = srt.qRotation.z, w = srt.qRotation.w;
            float l_fTotationArray[]
            {
                1 - 2 * (y * y + z * z), 2 * (x * y - z * w), 2 * (x * z + y * w), 0,
                2 * (x * y + z * w), 1 - 2 * (x * x + z * z), 2 * (y * z - x * w), 0,
                2 * (x * z - y * w), 2 * (y * z + x * w), 1 - 2 * (x * x + y * y), 0,
                0, 0, 0, 1
            };
            cMatrix44 rotMat(l_fTotationArray);
            mat *= rotMat;
        }
        // Construct scale matrix
        if (srt.iSRTFlag & SRT_SCALE_FLAG)
        {
            float l_fScaleArray[] =
            {
                srt.vScale.x, 0, 0, 0,
                0, srt.vScale.y, 0, 0,
                0, 0, srt.vScale.z, 0,
                0, 0, 0, 1
            };
            cMatrix44 scaleMat(l_fScaleArray);
            mat *= scaleMat;
        }
        matrixMap[time] = mat;
    }
}
cBone::cBone(const wchar_t* e_strName, int e_iJointIndex):m_matInvBindPose(cMatrix44::Identity)
{
    SetName(e_strName);
    m_iNodeIndex = e_iJointIndex;
}

cBone::~cBone()
{
    // Destructor implementation
}

cBone* cBone::FinChildByName(const wchar_t* e_strBoneName)
{
    if (!wcscmp(e_strBoneName, GetName()))
        return this;
    if (this->GetNextSibling() != nullptr)
    {
        return ((cBone*)(GetNextSibling()))->FinChildByName(e_strBoneName);
    }
    if (GetFirstChild() != nullptr)
    {
        return ((cBone*)(GetFirstChild()))->FinChildByName(e_strBoneName);
    }
    return nullptr;
}




void cBone::ApplySRT(const sSRT& srt, bool e_bSetChildBonesDirty)
{
    cMatrix44 localTransform = cMatrix44::Identity;
    sSRT l_SRT = srt;
    if (l_SRT.iSRTFlag & SRT_TRANSLATION_FLAG) // Translation
    {
        localTransform *= cMatrix44::TranslationMatrix(l_SRT.vTranslation);
    }
    if (l_SRT.iSRTFlag & SRT_ROTATION_FLAG) // Rotation
    {
        localTransform *= l_SRT.qRotation.ToMatrix();
    }
    if (l_SRT.iSRTFlag & SRT_SCALE_FLAG) // Scale
    {
        localTransform *= cMatrix44::ScaleMatrix(l_SRT.vScale);
    }
    //if (l_SRT.iSRTFlag & SRT_SCALE_FLAG) // Scale
    //{
    //    localTransform *= cMatrix44::ScaleMatrix(l_SRT.scale);
    //}
    //if (l_SRT.iSRTFlag & SRT_ROTATION_FLAG) // Rotation
    //{
    //    localTransform *= l_SRT.rotation.ToMatrix();
    //}
    //if (l_SRT.iSRTFlag & SRT_TRANSLATION_FLAG) // Translation
    //{
    //    localTransform *= cMatrix44::TranslationMatrix(l_SRT.translation);
    //}
    //localTransform = this->m_StartNodeTransform* localTransform;
    SetLocalTransform(localTransform , e_bSetChildBonesDirty);
}

bool    cAnimationClip::SampleToTime(float e_fTime, bool e_bAssignToBone,std::vector<sSRT>* e_pSRTVector)
{
    if (!this->m_pCurrentAnimationData)
    {
        return false;
    }
    std::vector<sSRT>*l_pSRTVector = &m_SRTVector;
    if (e_pSRTVector)
    {
        l_pSRTVector = e_pSRTVector;
    }
    std::vector<cBone*>&l_pBoneVector = m_pAnimationMesh->m_SkinningBoneVector;
    auto l_uiSize = l_pBoneVector.size();
    if (l_uiSize)
    {
        for (size_t i = 0; i < l_uiSize; ++i)
        {
            UpdateNode(l_pBoneVector[i], e_fTime, (*l_pSRTVector)[i], e_bAssignToBone);
        }
    }
    else
    {//not skinning animation
        sSRT l_sSRT;
        for (auto l_IT : m_pCurrentAnimationData->m_BoneIDAndAnimationData)
        {
            UpdateNode(l_IT.first, e_fTime, l_sSRT, true);
            if (!this->m_pCurrentAnimationData->m_pNotSkinningMeshBone)
            {
                if (l_IT.first->m_strTargetMeshName.length()>0)
                {
                    this->m_pCurrentAnimationData->m_pNotSkinningMeshBone = l_IT.first;
                }
            }
        }
    }
    return true;
}

void cAnimationClip::UpdateNode(cBone* e_pBone, float e_fTime, sSRT&e_SRT, bool e_bAssignToBone)
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

void cAnimationClip::SetBoneAndAnimationData(cAnimationMesh* e_pAnimationMesh)
{
    m_pAnimationMesh = e_pAnimationMesh;
	m_SRTVector.clear();
	for (auto l_pBone : m_pAnimationMesh->m_SkinningBoneVector)
	{
        sSRT l_SRT;
        l_SRT.iSRTFlag = SRT_SCALE_FLAG | SRT_ROTATION_FLAG | SRT_TRANSLATION_FLAG;
		m_SRTVector.push_back(l_SRT);
	}
}

bool cAnimationClip::SetAnimation(const char* e_strAnimationName, bool e_bLoop, float e_fTargetTime)
{
    auto it = m_pAnimationMesh->m_NameAndAnimationMap.find(e_strAnimationName);
    if (it != m_pAnimationMesh->m_NameAndAnimationMap.end())
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
    SampleToTime(e_fTime,true);
}

void    cAnimationClip::BlendClips(float e_fTime, const char* e_strAnimationName1, const char* e_strAnimationName2, bool e_bAssignToBone, bool e_bLoop, float e_fTargetFactor)
{
    float l_fPreviousTime = m_fBlendingTime;
    m_fBlendingTime += e_fTime;
    std::vector<sSRT>   l_SRTVector1 = m_SRTVector;
    std::vector<sSRT>   l_SRTVector2 = m_SRTVector;
    if(!SetAnimation(e_strAnimationName1, e_bLoop, l_fPreviousTime))
    {
        return;
    }
    this->m_pCurrentAnimationData->Update(e_fTime);
    this->SampleToTime(this->m_pCurrentAnimationData->m_fCurrentTime,false, &l_SRTVector1);
    if (!SetAnimation(e_strAnimationName2, e_bLoop, l_fPreviousTime))
    {
        return;
    }
    this->m_pCurrentAnimationData->Update(e_fTime);
    this->SampleToTime(this->m_pCurrentAnimationData->m_fCurrentTime, false, &l_SRTVector2);
    std::vector<cBone*>& l_pBoneVector = m_pAnimationMesh->m_SkinningBoneVector;
    auto l_uiSize = l_pBoneVector.size();
    for (size_t i = 0; i < l_uiSize; ++i)
    {
        m_SRTVector[i] = sSRT::Blend(l_SRTVector1[i], l_SRTVector2[i], e_fTargetFactor);
        l_pBoneVector[i]->SetLocalTransform(m_SRTVector[i].GetMatrix());
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
