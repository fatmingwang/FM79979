
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
    float l_fMinKeyTime(FLT_MAX), l_fMaxKeyTime(FLT_MIN), l_fKeyTime;
    for (FloatToSRTMap::iterator l_Iterator = m_FormKeyFrames.begin(); l_Iterator != m_FormKeyFrames.end(); ++l_Iterator)
    {
        l_fKeyTime = l_Iterator->first;
        l_fMinKeyTime = min(l_fMinKeyTime, l_fKeyTime);
        l_fMaxKeyTime = max(l_fMaxKeyTime, l_fKeyTime);
    }
    m_fMaxKeyTime = l_fMaxKeyTime;
    m_fMinKeyTime = l_fMinKeyTime;
}


void cBone::EvaluateLocalXForm(float e_fTime, bool e_bSetChildBonesDirty)
{
    if (m_FormKeyFrames.empty())
    {
        return;
    }

    auto it = m_FormKeyFrames.lower_bound(e_fTime);
    if (it == m_FormKeyFrames.end())
    {
        it = std::prev(m_FormKeyFrames.end());
    }

    const SRT& srt = it->second;

    // Default values for SRT components
    Vector3 defaultScale(1.0f, 1.0f, 1.0f);
    Quaternion defaultRotation(1.0f, 0.0f, 0.0f, 0.0f);
    Vector3 defaultTranslation(0.0f, 0.0f, 0.0f);

    // Use provided SRT values or default values if not present
    Vector3 scale = (srt.iSRTFlag & (1 << 1)) ? srt.scale : defaultScale;
    Quaternion rotation = (srt.iSRTFlag & (1 << 2)) ? srt.rotation : defaultRotation;
    Vector3 translation = (srt.iSRTFlag & (1 << 3)) ? srt.translation : defaultTranslation;

    cMatrix44 localTransform = cMatrix44::Identity;

    // Apply transformations based on the SRT flag
    if (srt.iSRTFlag & (1 << 1)) // Scale
    {
        localTransform *= cMatrix44::ScaleMatrix(scale);
    }
    if (srt.iSRTFlag & (1 << 2)) // Rotation
    {
        localTransform *= rotation.ToMatrix();
    }
    if (srt.iSRTFlag & (1 << 3)) // Translation
    {
        localTransform *= cMatrix44::TranslationMatrix(translation);
    }

    SetLocalTransform(localTransform, e_bSetChildBonesDirty);
}
