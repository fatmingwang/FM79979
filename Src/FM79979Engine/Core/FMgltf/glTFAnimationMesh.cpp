#include "tiny_gltf.h"
#include "glTFAnimationMesh.h"


cBone::cBone(const WCHAR* e_strName)
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

cAnimationMeshFromglTF::cAnimationMeshFromglTF()
    : m_pMainRootBone(nullptr), m_fMinKeyTime(0.0f), m_fMaxKeyTime(0.0f), m_fCurrentTime(0.0f), m_fStartTime(0.0f), m_fEndTime(0.0f), m_pAllBonesMatrixForSkinned(nullptr)
{
}

cAnimationMeshFromglTF::~cAnimationMeshFromglTF()
{
    m_SkinningBoneVector.SetFromResource(true);
    int l_iCount = m_AllBoneVector.Count();
    for (int i = 0; i < l_iCount; i++)
    {
        auto l_pData = m_AllBoneVector[i];
        if (l_pData)
        {
            l_pData->SetParent(nullptr);
        }
    }
    m_AllBoneVector.Destroy();
    SAFE_DELETE(m_pAllBonesMatrixForSkinned);
}

void cAnimationMeshFromglTF::LoadAnimation(const tinygltf::Model& model, const tinygltf::Animation& animation)
{
    for (const auto& channel : animation.channels)
    {
        const auto& sampler = animation.samplers[channel.sampler];
        const auto& targetNode = model.nodes[channel.target_node];

        cBone* bone = m_AllBoneVector[targetNode.name.c_str()];
        if (!bone)
        {
            continue;
        }

        FloatTocMatrix44Map keyframes;

        // Load keyframe times
        const auto& inputAccessor = model.accessors[sampler.input];
        const auto& inputBufferView = model.bufferViews[inputAccessor.bufferView];
        const auto& inputBuffer = model.buffers[inputBufferView.buffer];
        const float* inputData = reinterpret_cast<const float*>(inputBuffer.data.data() + inputBufferView.byteOffset + inputAccessor.byteOffset);

        // Load keyframe transforms
        const auto& outputAccessor = model.accessors[sampler.output];
        const auto& outputBufferView = model.bufferViews[outputAccessor.bufferView];
        const auto& outputBuffer = model.buffers[outputBufferView.buffer];
        const float* outputData = reinterpret_cast<const float*>(outputBuffer.data.data() + outputBufferView.byteOffset + outputAccessor.byteOffset);

        for (size_t i = 0; i < inputAccessor.count; ++i)
        {
            cMatrix44 transform;
            memcpy(&transform, outputData + i * 16, sizeof(cMatrix44));
            keyframes[inputData[i]] = transform;
        }

        bone->SetFormKeyFrames(keyframes);
    }
}

void cAnimationMeshFromglTF::UpdateAnimation(float deltaTime)
{
    m_fCurrentTime += deltaTime;
    auto l_Vector = m_AllBoneVector.GetList();
    for (auto& bone : *l_Vector)
    {
        bone->EvaluateLocalXForm(m_fCurrentTime);
    }
}

void cAnimationMeshFromglTF::RefreshAnimationData()
{
    cBone** l_ppBone = &(*m_SkinningBoneVector.GetList())[0];
    m_fMinKeyTime = FLT_MAX;
    m_fMaxKeyTime = FLT_MIN;
    int l_iNum = this->m_SkinningBoneVector.Count();
    for (int i = 0; i < l_iNum; ++i)
    {
        cBone* l_pBone = l_ppBone[i];
        m_fMinKeyTime = min(m_fMinKeyTime, l_pBone->m_fMinKeyTime);
        m_fMaxKeyTime = max(m_fMaxKeyTime, l_pBone->m_fMaxKeyTime);
    }
    if (m_fMinKeyTime != FLT_MAX)
    {
        m_fStartTime = m_fMinKeyTime;
        m_fEndTime = m_fMaxKeyTime;
    }
    else
    {
        m_fMinKeyTime = 0;
        m_fMaxKeyTime = 0;
    }
    if (m_fStartTime < 0)
        m_fStartTime = 0;
    SAFE_DELETE(m_pAllBonesMatrixForSkinned);
    m_pAllBonesMatrixForSkinned = new cMatrix44[l_iNum];
}

void cAnimationMeshFromglTF::UpdateNodes(float e_fTimeValue)
{
    assert((e_fTimeValue >= m_fMinKeyTime) && (e_fTimeValue <= m_fMaxKeyTime));
    int l_iNum = this->m_SkinningBoneVector.Count();
    for (int i = 0; i < l_iNum; ++i)
    {
        m_SkinningBoneVector[i]->EvaluateLocalXForm(e_fTimeValue, i == 0 ? true : false);
    }
}

void cAnimationMeshFromglTF::UpdateNode(cBone* e_pBone, float e_fTime)
{
    e_pBone->EvaluateLocalXForm(e_fTime);
    cBone* l_pBone = (cBone*)e_pBone->GetFirstChild();
    if (l_pBone != nullptr)
    {
        UpdateNode(l_pBone, e_fTime);
    }
    l_pBone = (cBone*)e_pBone->GetNextSibling();
    if (l_pBone != nullptr)
    {
        UpdateNode(l_pBone, e_fTime);
    }
}

void cAnimationMeshFromglTF::JointUpdate(float elapsedTime)
{
    float newTime(m_fCurrentTime);
    if ((m_fCurrentTime + elapsedTime) > m_fEndTime)
    {
        float endMinusStart = m_fEndTime - m_fStartTime;
        if (endMinusStart > 0.001)
        {
            float overTime = (m_fCurrentTime + elapsedTime - m_fEndTime);
            newTime = m_fStartTime + (float)fmod(overTime, endMinusStart);
        }
        else
        {
            newTime = m_fCurrentTime;
        }
    }
    else
    {
        newTime = m_fCurrentTime + elapsedTime;
    }
    m_fCurrentTime = newTime;
    assert(this->m_fMinKeyTime <= this->m_fMaxKeyTime);
    float l_fUpdateTime = 0;
    cBone* l_pBone = m_SkinningBoneVector[0];
    if (m_fCurrentTime > m_fMaxKeyTime)
        UpdateNode(l_pBone, m_fMaxKeyTime);
    else if (m_fCurrentTime < m_fMinKeyTime)
        UpdateNode(l_pBone, m_fMinKeyTime);
    else
        UpdateNode(l_pBone, m_fCurrentTime);
}

void cAnimationMeshFromglTF::Update(float elapsedTime)
{
    float newTime(m_fCurrentTime);
    if ((m_fCurrentTime + elapsedTime) > m_fEndTime)
    {
        float endMinusStart = m_fEndTime - m_fStartTime;
        if (endMinusStart > 0.001)
        {
            float overTime = (m_fCurrentTime + elapsedTime - m_fEndTime);
            newTime = m_fStartTime + (float)fmod(overTime, endMinusStart);
        }
        else
        {
            newTime = m_fCurrentTime;
        }
    }
    else
    {
        newTime = m_fCurrentTime + elapsedTime;
    }
    m_fCurrentTime = newTime;
    SetCurrentAnimationTime(m_fCurrentTime);
    int l_iBoneSize = this->m_SkinningBoneVector.Count();
    cBone** l_ppBone = &(*m_SkinningBoneVector.GetList())[0];
    for (int i = 0; i < l_iBoneSize; ++i)
    {
        cMatrix44 l_WorldTransform = l_ppBone[i]->GetWorldTransform() * l_ppBone[i]->m_matInvBindPose * m_matMeshBindShapePose;
        m_pAllBonesMatrixForSkinned[i] = l_WorldTransform;
    }
}

void cAnimationMeshFromglTF::SetCurrentAnimationTime(float e_fCurrentTime)
{
    this->m_fCurrentTime = e_fCurrentTime;
    if (m_fCurrentTime > m_fEndTime)
        UpdateNodes(m_fEndTime);
    else if (m_fCurrentTime < this->m_fStartTime)
        UpdateNodes(m_fStartTime);
    else
        UpdateNodes(m_fCurrentTime);
}

void cAnimationMeshFromglTF::RenderSkeleton()
{
    int l_iNumBone = 0;
    int l_iBoneSize = this->m_SkinningBoneVector.Count();
    cBone** l_ppBone = &(*m_SkinningBoneVector.GetList())[0];
    Vector3 l_vAllVertices[512];
    cMatrix44 l_mat = cMatrix44::Identity;
    for (int i = 1; i < l_iBoneSize; ++i)
    {
        cBone* l_pMe = l_ppBone[i];
        cBone* l_pParent = (cBone*)l_pMe->GetParent();
        cMatrix44 l_mat = cMatrix44::Identity;
        if (l_pParent)
        {
            l_mat = (l_pParent->GetWorldTransform()) * this->m_matMeshBindShapePose;
            l_vAllVertices[l_iNumBone * 2 + 1] = l_mat.GetTranslation();
            l_mat = (l_pMe->GetWorldTransform()) * this->m_matMeshBindShapePose;
            l_vAllVertices[l_iNumBone * 2] = l_mat.GetTranslation();
            ++l_iNumBone;
        }
    }
    l_mat = this->GetWorldTransform() * cMatrix44::ZupToYUp;
    GLRender::RenderLine((float*)&l_vAllVertices[0], l_iBoneSize * 2, Vector4(0.f, 1.f, 0.5f, 1.f), 3, l_mat);
    GLRender::RenderPoints(l_vAllVertices, l_iBoneSize * 2, 15, Vector4(0.f, 1.f, 1.f, 1.f), l_mat);
}

void cAnimationMeshFromglTF::RenderSkeletonName(float* e_pfProjection, float* e_pfMatMV, int* e_piViewport)
{
    int l_iBoneSize = this->m_SkinningBoneVector.Count();
    cBone** l_ppBone = &(*m_SkinningBoneVector.GetList())[0];
    cMatrix44 l_Root = this->GetWorldTransform() * cMatrix44::ZupToYUp;
    for (int i = 1; i < l_iBoneSize; ++i)
    {
        cBone* l_pMe = l_ppBone[i];
        const WCHAR* l_strName = l_pMe->GetName();
        Vector3 l_vPos = (l_Root * l_pMe->GetWorldTransform() * this->m_matMeshBindShapePose).GetTranslation();
        Vector3 l_vPos2 = WorldToScreen(l_vPos.x, l_vPos.y, l_vPos.z, e_pfProjection, e_pfMatMV, e_piViewport);
        if (cGameApp::m_spGlyphFontRender)
        {
            cGameApp::m_spGlyphFontRender->RenderFont(l_vPos2.x, l_vPos2.y, l_strName);
        }
    }
}

void cAnimationMeshFromglTF::DebugRender()
{
    Render();
    RenderSkeleton();
}