
#include "tiny_gltf.h"
#include "glTFBone.h"
cBone::cBone(const WCHAR* e_strName):m_matInvBindPose(cMatrix44::Identity)
{
    SetName(e_strName);
    m_fMinKeyTime = 0;
    m_fMaxKeyTime = 0;
    m_bAnimation = false;
}

cBone::cBone(cBone* e_pBone)
{
    SetName(e_pBone->GetName());
    m_bAnimation = e_pBone->m_bAnimation;
    m_fMinKeyTime = e_pBone->m_fMinKeyTime;
    m_fMaxKeyTime = e_pBone->m_fMaxKeyTime;
    m_FormKeyFrames = e_pBone->m_FormKeyFrames;
    m_matInvBindPose = e_pBone->m_matInvBindPose;
    this->SetLocalTransform(this->GetLocalTransform());
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

void cBone::SetFormKeyFrames(FloatTocMatrix44Map e_FormKeyFrames)
{
    m_FormKeyFrames = e_FormKeyFrames;
    size_t l_uiSize = m_FormKeyFrames.size();
    float l_fMinKeyTime(FLT_MAX), l_fMaxKeyTime(FLT_MIN), l_fKeyTime;
    for (FloatTocMatrix44Map::iterator l_Iterator = m_FormKeyFrames.begin(); l_Iterator != m_FormKeyFrames.end(); ++l_Iterator)
    {
        l_fKeyTime = l_Iterator->first;
        l_fMinKeyTime = min(l_fMinKeyTime, l_fKeyTime);
        l_fMaxKeyTime = max(l_fMaxKeyTime, l_fKeyTime);
    }
    m_fMaxKeyTime = l_fMaxKeyTime;
    m_fMinKeyTime = l_fMinKeyTime;
    if (e_FormKeyFrames.size())
    {
        m_bAnimation = true;
    }
    else
    {
        cGameApp::OutputDebugInfoString(this->GetName());
        cGameApp::OutputDebugInfoString(L"animation frame is zero");
    }
}

void cBone::EvaluateLocalXForm(float timeValue, bool e_bSetChildBonesDirty)
{
    if (m_FormKeyFrames.size() == 0)
        return;

    FloatTocMatrix44Map::iterator prevKey(m_FormKeyFrames.lower_bound(timeValue));
    FloatTocMatrix44Map::iterator nextKey(prevKey);

    if ((prevKey == m_FormKeyFrames.end()) ||
        ((prevKey != m_FormKeyFrames.begin()) && (prevKey->first > timeValue)))
        --prevKey;

    assert(prevKey != m_FormKeyFrames.end());

    if ((prevKey == nextKey) || (nextKey == m_FormKeyFrames.end()))
    {
        this->SetLocalTransform(prevKey->second.Transposed());
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

    mNew = mNew.Transposed();
    this->SetLocalTransform(mNew, e_bSetChildBonesDirty);
}
