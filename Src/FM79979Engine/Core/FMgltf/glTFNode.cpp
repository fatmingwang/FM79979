
#include "tiny_gltf.h"
#include "glTFNode.h"
#include "glTFAnimationMesh.h"
#include "glTFModel.h"
#include "glTFAnimation.h"


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
cglTFNodeData::cglTFNodeData(const tinygltf::Node& e_Node, int e_iNodeIndex)
{
    SetName(e_Node.name.c_str());
    m_iNodeIndex = e_iNodeIndex;
}

cglTFNodeData::~cglTFNodeData()
{
    // Destructor implementation
}

cglTFNodeData* cglTFNodeData::FinChildByName(const wchar_t* e_strBoneName)
{
    if (!wcscmp(e_strBoneName, GetName()))
        return this;
    if (this->GetNextSibling() != nullptr)
    {
        return ((cglTFNodeData*)(GetNextSibling()))->FinChildByName(e_strBoneName);
    }
    if (GetFirstChild() != nullptr)
    {
        return ((cglTFNodeData*)(GetFirstChild()))->FinChildByName(e_strBoneName);
    }
    return nullptr;
}




void cglTFNodeData::ApplySRT(const sSRT& srt, bool e_bSetChildBonesDirty)
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

void cglTFNodeData::Update(float e_fElpaseTime)
{
}

void cglTFNodeData::Render()
{
}
