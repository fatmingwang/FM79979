
#include "tiny_gltf.h"
#include "glTFBone.h"

void cBone::ConvertSRTMapToMatrixMap(const FloatToSRTMap& srtMap, FloatTocMatrix44Map& matrixMap)
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
cBone::cBone(const WCHAR* e_strName, int e_iJointIndex):m_matInvBindPose(cMatrix44::Identity)
{
    SetName(e_strName);
    m_iNodeIndex = e_iJointIndex;
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
    //m_PreviousSRT = std::make_pair( m_FormKeyFrames.begin()->first,m_FormKeyFrames.begin()->second );
    ConvertSRTMapToMatrixMap(m_FormKeyFrames, m_MatrixKeyFrames);
}

void cBone::SetFormKeyFrames(FloatTocMatrix44Map e_FormKeyFrames)
{
    m_MatrixKeyFrames = e_FormKeyFrames;
    size_t l_uiSize = m_MatrixKeyFrames.size();
    if (l_uiSize == 0)
    {
        return;
    }
    float l_fMinKeyTime(FLT_MAX), l_fMaxKeyTime(FLT_MIN), l_fKeyTime;
    for (auto l_Iterator = m_MatrixKeyFrames.begin(); l_Iterator != m_MatrixKeyFrames.end(); ++l_Iterator)
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
    //return EvaluateLocalXForm2(e_fTime,e_bSetChildBonesDirty);
    if (m_FormKeyFrames.empty())
    {
        return;
    }

    // Find the keyframes surrounding the current time
    auto it = m_FormKeyFrames.lower_bound(e_fTime);

    if (it == m_FormKeyFrames.begin()) // If time is before the first keyframe
    {
        //m_PreviousSRT = *it;
        //ApplySRT(m_PreviousSRT.second, e_bSetChildBonesDirty);
        ApplySRT(it->second, e_bSetChildBonesDirty);
        return;
    }
    if (it == m_FormKeyFrames.end()) // If time is after the last keyframe
    {
        it = std::prev(m_FormKeyFrames.end());
    }

    auto nextIt = it;
    auto prevIt = std::prev(it); // Get the previous keyframe

    float prevTime = prevIt->first;
    float nextTime = nextIt->first;

    if (prevTime == nextTime) // Avoid division by zero
    {
        //m_PreviousSRT = *prevIt;
        ApplySRT(prevIt->second, e_bSetChildBonesDirty);
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
        l_CurrentSRT.qRotation = Quaternion::Slerp(prevSRT.qRotation,nextSRT.qRotation, factor);
        l_CurrentSRT.iSRTFlag |= SRT_ROTATION_FLAG;
    }

    // Scale Interpolation
    if (prevSRT.iSRTFlag & SRT_SCALE_FLAG)
    {
        l_CurrentSRT.vScale = prevSRT.vScale * (1.0f - factor) + nextSRT.vScale * factor;
        l_CurrentSRT.iSRTFlag |= SRT_SCALE_FLAG;
    }

    // Store the updated transform
    //m_PreviousSRT = { e_fTime, l_CurrentSRT };

    // Apply the interpolated SRT
    ApplySRT(l_CurrentSRT, e_bSetChildBonesDirty);
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


void cBone::EvaluateLocalXForm2(float timeValue, bool e_bSetChildBonesDirty)
{
    if (m_MatrixKeyFrames.size() == 0)
    {
        return;
    }

    FloatTocMatrix44Map::iterator prevKey(m_MatrixKeyFrames.lower_bound(timeValue));
    FloatTocMatrix44Map::iterator nextKey(prevKey);

    if ((prevKey == m_MatrixKeyFrames.end()) ||
        ((prevKey != m_MatrixKeyFrames.begin()) && (prevKey->first > timeValue)))
        --prevKey;

    assert(prevKey != m_MatrixKeyFrames.end());

    if ((prevKey == nextKey) || (nextKey == m_MatrixKeyFrames.end()))
    {
        this->SetLocalTransform(this->m_StartNodeTransform * prevKey->second);
        return;
    }

    float time0 = prevKey->first;
    float time1 = nextKey->first;
    float l_fTimeDis = (timeValue - time0) / (time1 - time0);

    const cMatrix44& m0 = prevKey->second;
    const cMatrix44& m1 = nextKey->second;

    cMatrix44 mNew = m0;

    Vector3 m0x(m0[0]), m0y(m0[1]), m0z(m0[2]);
    Vector3 m1x(m1[0]), m1y(m1[1]), m1z(m1[2]);

#define SCALE_EPSILON 0.02f
    if (((m0x.LengthSquared() - 1.0f) > SCALE_EPSILON) || ((m0y.LengthSquared() - 1.0f) > SCALE_EPSILON) ||
        ((m0z.LengthSquared() - 1.0f) > SCALE_EPSILON) || ((m1x.LengthSquared() - 1.0f) > SCALE_EPSILON) ||
        ((m1y.LengthSquared() - 1.0f) > SCALE_EPSILON) || ((m1z.LengthSquared() - 1.0f) > SCALE_EPSILON))
    {
        // TODO: need to interpolate scaling too
    }
    else
    {
        Vector3 t0 = m0.GetTranslation();
        Vector3 t1 = m1.GetTranslation();

        Vector3 dX(m1x - m0x), dY(m1y - m0y), dZ(m1z - m0z);
        Vector3 axis = (dX ^ dY) + (dY ^ dZ) + (dZ ^ dX);

#define VLENGTH_EPSILON 0.00000001f
        if (axis.Length() >= VLENGTH_EPSILON)
        {
            Vector3 dU = dX;
            Vector3 U = m0x;
            if (dU.Length() < VLENGTH_EPSILON)
            {
                dU = dY;
                U = m0y;
            }
            double angle;
            axis.NormalizeIt();

            Vector3 axisCrossU = (axis ^ U);

            if ((axisCrossU * dU) < 0.0)
                axis = -axis;

            if (axisCrossU.Length() < VLENGTH_EPSILON)
                angle = 0;
            else
            {
                double a = dU.Length() / (2.0 * axisCrossU.Length());
                a = (a < -1.0) ? -1.0 : ((a > 1.0) ? 1.0 : a);
                angle = 2.0 * asin(a);
            }

            double aNew = l_fTimeDis * angle;

            cMatrix44 mRot(cMatrix44::AxisRotationMatrix(axis, (float)aNew));

            mNew = mRot * m0;
        }

        Vector3 tNew = t0 + l_fTimeDis * (t1 - t0);
        mNew.SetTranslation(tNew);
    }
    this->SetLocalTransform(this->m_StartNodeTransform*mNew, e_bSetChildBonesDirty);
}

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
    sSRT l_SRT;
    l_SRT.iSRTFlag = e_Source1.iSRTFlag | e_Source2.iSRTFlag;
    l_SRT.vScale = Vector3Lerp(e_Source1.vScale, e_Source2.vScale,e_fFactor);
    l_SRT.qRotation = Quaternion::Slerp(e_Source1.qRotation, e_Source2.qRotation, e_fFactor);
    l_SRT.vTranslation = Vector3Lerp(e_Source1.vTranslation,e_Source2.vTranslation, e_fFactor);
    return l_SRT;
}

void cAnimationClip::SampleToTime(float e_fTime, bool e_bAssignToBone)
{
    auto l_uiSize = this->m_OrderedAnimationVector.size();
    for (size_t i = 0; i < l_uiSize; ++i)
    {
        auto l_pFloatToSRTMap = m_OrderedAnimationVector[i];
        if (l_pFloatToSRTMap->empty())
        {
            continue;
        }

        // Find the keyframes surrounding the current time
        auto it = l_pFloatToSRTMap->lower_bound(e_fTime);

        if (it == l_pFloatToSRTMap->begin()) // If time is before the first keyframe
        {
            m_SRTVector[i] = it->second;
            continue;
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
            m_SRTVector[i] = prevIt->second;
            continue;
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
		m_SRTVector[i] = l_CurrentSRT;
    }
    if (e_bAssignToBone)
    {
        for (size_t i = 0; i < l_uiSize; ++i)
        {
            (*m_pOrderedBonesVector)[i]->SetLocalTransform(m_SRTVector[i].GetMatrix());
        }
    }
}

void cAnimationClip::SetBoneAndAnimationData(std::vector<cBone*>* e_pBoneVector, sAnimationData* e_pAnimationData)
{
	m_pOrderedBonesVector = e_pBoneVector;
	m_fCurrentTime = e_pAnimationData->m_fCurrentTime;
	m_fEndTime = e_pAnimationData->m_fEndTime;
	m_bLoop = e_pAnimationData->m_fEndTime;
	m_SRTVector.clear();
	for (auto l_pBone : *e_pBoneVector)
	{
        sSRT l_SRT;
        l_SRT.iSRTFlag = SRT_SCALE_FLAG | SRT_ROTATION_FLAG | SRT_TRANSLATION_FLAG;
        auto l_IT = e_pAnimationData->m_BoneIDAndAnimationData.find(l_pBone);
        if (l_IT != e_pAnimationData->m_BoneIDAndAnimationData.end())
        {
            auto l_Data = l_IT->second.find(0);
            l_SRT = l_Data->second;
			m_OrderedAnimationVector.push_back(&l_IT->second);
        }
		m_SRTVector.push_back(l_SRT);
	}
}

void cAnimationClip::UpdateToTargetTime(float e_fTime, bool e_bAssignToBone)
{
    SampleToTime(e_fTime,true);
}

void cAnimationClip::BlendClips(float e_fTime, cAnimationClip* e_pTarget, bool e_bAssignToBone, float e_fTargetFactor)
{
    this->SampleToTime(e_fTime, false);
    e_pTarget->SampleToTime(e_fTime, false);
    auto l_uiSize = this->m_OrderedAnimationVector.size();
    for (size_t i = 0; i < l_uiSize; ++i)
    {
        sSRT l_SRT = sSRT::Blend(this->m_SRTVector[i], e_pTarget->m_SRTVector[i], e_fTargetFactor);
        (*this->m_pOrderedBonesVector)[i]->SetLocalTransform(l_SRT.GetMatrix());
    }
}
