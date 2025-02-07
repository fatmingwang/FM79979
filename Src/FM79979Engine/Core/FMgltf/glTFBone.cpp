
#include "tiny_gltf.h"
#include "glTFBone.h"
cBone::cBone(const WCHAR* e_strName, int e_iJointIndex):m_matInvBindPose(cMatrix44::Identity)
{
    SetName(e_strName);
    m_iJointIndex = e_iJointIndex;
    m_fMinKeyTime = 0;
    m_fMaxKeyTime = 0;
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

void cBone::SetFormKeyFrames(FloatToSRTMap e_FormKeyFrames)
{
    m_FormKeyFrames = e_FormKeyFrames;
    size_t l_uiSize = m_FormKeyFrames.size();
    if (l_uiSize == 0)
    {
        return;
    }
    float l_fMinKeyTime(FLT_MAX), l_fMaxKeyTime(FLT_MIN), l_fKeyTime;
    for (FloatToSRTMap::iterator l_Iterator = m_FormKeyFrames.begin(); l_Iterator != m_FormKeyFrames.end(); ++l_Iterator)
    {
        l_fKeyTime = l_Iterator->first;
        l_fMinKeyTime = min(l_fMinKeyTime, l_fKeyTime);
        l_fMaxKeyTime = max(l_fMaxKeyTime, l_fKeyTime);
    }
    m_fMaxKeyTime = l_fMaxKeyTime;
    m_fMinKeyTime = l_fMinKeyTime;
    m_PreviousSRT = std::make_pair( m_FormKeyFrames.begin()->first,m_FormKeyFrames.begin()->second );
}


void cBone::EvaluateLocalXForm(float e_fTime, bool e_bSetChildBonesDirty)
{
    if (m_FormKeyFrames.empty())
    {
        return;
    }

    // Find the keyframes surrounding the current time
    auto it = m_FormKeyFrames.lower_bound(e_fTime);
    if (it == m_FormKeyFrames.end())
    {
        it = std::prev(m_FormKeyFrames.end());
    }

    auto nextIt = it;

    // Calculate the interpolation factor
    float prevTime = m_PreviousSRT.first;
    float nextTime = nextIt->first;
    float factor = (e_fTime - prevTime) / (nextTime - prevTime);
    // Apply the interpolated transformations
    cMatrix44 localTransform = cMatrix44::Identity;


    // Interpolate SRT values
    const SRT& prevSRT = m_PreviousSRT.second;
    const SRT& nextSRT = nextIt->second;
    SRT l_CurrentSRT;
    if (m_PreviousSRT.second.iSRTFlag & SRT_SCALE_FLAG)
    {
        Vector3 scale = prevSRT.scale * (1.0f - factor) + nextSRT.scale * factor;
        l_CurrentSRT.scale = scale;
        l_CurrentSRT.iSRTFlag |= SRT_SCALE_FLAG;
    }
    if (m_PreviousSRT.second.iSRTFlag & SRT_ROTATION_FLAG)
    {
        Quaternion rotation;
        rotation.x = prevSRT.rotation.x * (1.0f - factor) + nextSRT.rotation.x * factor;
        rotation.y = prevSRT.rotation.y * (1.0f - factor) + nextSRT.rotation.y * factor;
        rotation.z = prevSRT.rotation.z * (1.0f - factor) + nextSRT.rotation.z * factor;
        rotation.w = prevSRT.rotation.w * (1.0f - factor) + nextSRT.rotation.w * factor;
        rotation = rotation.Normalize();  // Ensure the quaternion remains unit length
        l_CurrentSRT.rotation = rotation;
        l_CurrentSRT.iSRTFlag |= SRT_ROTATION_FLAG;
    }
    if (m_PreviousSRT.second.iSRTFlag & SRT_TRANSLATION_FLAG) // Translation
    {
        Vector3 translation = prevSRT.translation * (1.0f - factor) + nextSRT.translation * factor;
        l_CurrentSRT.translation = translation;
        l_CurrentSRT.iSRTFlag |= SRT_TRANSLATION_FLAG;
    }        
    m_PreviousSRT = { e_fTime,l_CurrentSRT };
    ApplySRT(m_PreviousSRT.second, e_bSetChildBonesDirty);
}

void cBone::ApplySRT(const SRT& srt, bool e_bSetChildBonesDirty)
{
    cMatrix44 localTransform = cMatrix44::Identity;
    // Apply transformations based on the SRT flag
    if (srt.iSRTFlag & SRT_SCALE_FLAG) // Scale
    {
        localTransform = cMatrix44::ScaleMatrix(srt.scale);
    }
    if (srt.iSRTFlag & SRT_ROTATION_FLAG) // Rotation
    {
        localTransform *= srt.rotation.ToMatrix();
    }
    if (srt.iSRTFlag & SRT_TRANSLATION_FLAG) // Translation
    {
        localTransform *= cMatrix44::TranslationMatrix(srt.translation);
    }
    SetLocalTransform(localTransform, e_bSetChildBonesDirty);
}
