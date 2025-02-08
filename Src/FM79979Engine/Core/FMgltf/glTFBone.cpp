
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
                srt.translation.x, srt.translation.y, srt.translation.z, 1
            };
            cMatrix44 transMat(l_fTransArray);
            mat = transMat;
        }
        if (srt.iSRTFlag & SRT_ROTATION_FLAG)
        {
            // Construct rotation matrix from quaternion
            float x = srt.rotation.x, y = srt.rotation.y, z = srt.rotation.z, w = srt.rotation.w;
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
                srt.scale.x, 0, 0, 0,
                0, srt.scale.y, 0, 0,
                0, 0, srt.scale.z, 0,
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
    m_PreviousSRT = { m_FormKeyFrames.begin()->first,m_FormKeyFrames.begin()->second };
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

//
//void cBone::EvaluateLocalXForm(float e_fTime, bool e_bSetChildBonesDirty)
//{
//    //return EvaluateLocalXForm2(e_fTime,e_bSetChildBonesDirty);
//    if (m_FormKeyFrames.empty())
//    {
//        return;
//    }
//
//    // Find the keyframes surrounding the current time
//    auto it = m_FormKeyFrames.lower_bound(e_fTime);
//    if (it == m_FormKeyFrames.end())
//    {
//        it = std::prev(m_FormKeyFrames.end());
//    }
//
//    auto nextIt = it;
//
//    // Calculate the interpolation factor
//    float prevTime = m_PreviousSRT.first;
//    float nextTime = nextIt->first;
//    float factor = (e_fTime - prevTime) / (nextTime - prevTime);
//    // Apply the interpolated transformations
//    cMatrix44 localTransform = cMatrix44::Identity;
//
//
//    // Interpolate SRT values
//    const SRT& prevSRT = m_PreviousSRT.second;
//    const SRT& nextSRT = nextIt->second;
//    SRT l_CurrentSRT;
//    if (m_PreviousSRT.second.iSRTFlag & SRT_TRANSLATION_FLAG) // Translation
//    {
//        Vector3 translation = prevSRT.translation * (1.0f - factor) + nextSRT.translation * factor;
//        l_CurrentSRT.translation = translation;
//        l_CurrentSRT.iSRTFlag |= SRT_TRANSLATION_FLAG;
//    }
//    if (m_PreviousSRT.second.iSRTFlag & SRT_ROTATION_FLAG)
//    {
//        Quaternion rotation;
//        rotation.x = prevSRT.rotation.x * (1.0f - factor) + nextSRT.rotation.x * factor;
//        rotation.y = prevSRT.rotation.y * (1.0f - factor) + nextSRT.rotation.y * factor;
//        rotation.z = prevSRT.rotation.z * (1.0f - factor) + nextSRT.rotation.z * factor;
//        rotation.w = prevSRT.rotation.w * (1.0f - factor) + nextSRT.rotation.w * factor;
//        rotation = rotation.Normalize();  // Ensure the quaternion remains unit length
//        l_CurrentSRT.rotation = rotation;
//        l_CurrentSRT.iSRTFlag |= SRT_ROTATION_FLAG;
//    }
//    if (m_PreviousSRT.second.iSRTFlag & SRT_SCALE_FLAG)
//    {
//        Vector3 scale = prevSRT.scale * (1.0f - factor) + nextSRT.scale * factor;
//        l_CurrentSRT.scale = scale;
//        l_CurrentSRT.iSRTFlag |= SRT_SCALE_FLAG;
//    }
//    m_PreviousSRT = { e_fTime,l_CurrentSRT };
//    ApplySRT(m_PreviousSRT.second, e_bSetChildBonesDirty);
//}



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
        m_PreviousSRT = *it;
        ApplySRT(m_PreviousSRT.second, e_bSetChildBonesDirty);
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
        m_PreviousSRT = *prevIt;
        ApplySRT(m_PreviousSRT.second, e_bSetChildBonesDirty);
        return;
    }

    // Calculate the interpolation factor (clamped between 0 and 1)
    float factor = (e_fTime - prevTime) / (nextTime - prevTime);
    factor = std::clamp(factor, 0.0f, 1.0f);

    // Interpolate SRT values
    const SRT& prevSRT = prevIt->second;
    const SRT& nextSRT = nextIt->second;
    SRT l_CurrentSRT;

    // Translation Interpolation
    if (prevSRT.iSRTFlag & SRT_TRANSLATION_FLAG)
    {
        l_CurrentSRT.translation = prevSRT.translation * (1.0f - factor) + nextSRT.translation * factor;
        l_CurrentSRT.iSRTFlag |= SRT_TRANSLATION_FLAG;
    }

    // Rotation Interpolation using SLERP
    if (prevSRT.iSRTFlag & SRT_ROTATION_FLAG)
    {
        //l_CurrentSRT.rotation = Quaternion::Slerp(prevSRT.rotation,nextSRT.rotation, factor);
        l_CurrentSRT.rotation = prevSRT.rotation;
        l_CurrentSRT.iSRTFlag |= SRT_ROTATION_FLAG;
    }

    // Scale Interpolation
    if (prevSRT.iSRTFlag & SRT_SCALE_FLAG)
    {
        l_CurrentSRT.scale = prevSRT.scale * (1.0f - factor) + nextSRT.scale * factor;
        l_CurrentSRT.iSRTFlag |= SRT_SCALE_FLAG;
    }

    // Store the updated transform
    m_PreviousSRT = { e_fTime, l_CurrentSRT };

    // Apply the interpolated SRT
    ApplySRT(l_CurrentSRT, e_bSetChildBonesDirty);
}


void cBone::ApplySRT(const SRT& srt, bool e_bSetChildBonesDirty)
{
    cMatrix44 localTransform = cMatrix44::Identity;
    SRT l_SRT = srt;
    if (m_StartSRT.iSRTFlag & SRT_TRANSLATION_FLAG) // Translation
    {
        if (l_SRT.iSRTFlag & SRT_TRANSLATION_FLAG)
        {
            l_SRT.translation = m_StartSRT.translation;
        }
        else
        {
            l_SRT.translation = m_StartSRT.translation;
        }
        l_SRT.iSRTFlag |= SRT_TRANSLATION_FLAG;
    }
    if (m_StartSRT.iSRTFlag & SRT_ROTATION_FLAG) // Rotation
    {
        if (l_SRT.iSRTFlag & SRT_ROTATION_FLAG)
        {
            //l_SRT.rotation *= m_StartSRT.rotation;
            l_SRT.rotation.x += m_StartSRT.rotation.x;
            l_SRT.rotation.y += m_StartSRT.rotation.y;
            l_SRT.rotation.z += m_StartSRT.rotation.z;
            l_SRT.rotation.w += m_StartSRT.rotation.w;
            l_SRT.rotation.NormalizeIt();
        }
        else
        {
            l_SRT.rotation = m_StartSRT.rotation;
        }
        l_SRT.iSRTFlag |= SRT_ROTATION_FLAG;
    }
    if (m_StartSRT.iSRTFlag & SRT_SCALE_FLAG) // Scale
    {
        if (l_SRT.iSRTFlag & SRT_SCALE_FLAG)
        {
            l_SRT.scale += m_StartSRT.scale;
        }
        l_SRT.iSRTFlag |= SRT_SCALE_FLAG;
    }
    if (l_SRT.iSRTFlag & SRT_TRANSLATION_FLAG) // Translation
    {
        localTransform *= cMatrix44::TranslationMatrix(l_SRT.translation);
    }
    if (l_SRT.iSRTFlag & SRT_ROTATION_FLAG) // Rotation
    {
        localTransform *= l_SRT.rotation.ToMatrix();
    }
    if (l_SRT.iSRTFlag & SRT_SCALE_FLAG) // Scale
    {
        localTransform *= cMatrix44::ScaleMatrix(l_SRT.scale);
    }
    SetLocalTransform(localTransform, e_bSetChildBonesDirty);
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
        this->SetLocalTransform(prevKey->second);
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
    this->SetLocalTransform(mNew, e_bSetChildBonesDirty);
}