#pragma once
//#include "gltfMesh.h"
#include <map>
#include <string>
#include "../AllCoreInclude.h"
#include "tiny_gltf.h"

struct sSRT
{
    Vector3             vScale = Vector3::One;
    Quaternion          qRotation = Quaternion::Identity;
    Vector3             vTranslation = Vector3::Zero;
    int                 iSRTFlag = 0;//S 1<<1 R 1<< 2 T 1<<3
    cMatrix44           GetMatrix();
    static sSRT         Blend(sSRT& e_Source1, sSRT& e_Source2,float e_fFactor);
};
#define SRT_SCALE_FLAG          (1<<1)
#define SRT_ROTATION_FLAG       (1<<2)
#define SRT_TRANSLATION_FLAG    (1<<3)

using FloatToSRTMap = std::map<float, sSRT>;
typedef std::map<float, cMatrix44> FloatTocMatrix44Map;
void    ConvertSRTMapToMatrixMap(const FloatToSRTMap& srtMap, FloatTocMatrix44Map& matrixMap);

struct cglTFNodeData : public Frame
{
    //for debug
    int                 m_iJointIndex = -1;
    int                 m_iNodeIndex = -1;
    shared_ptr<class cMesh>        m_pMesh = nullptr;
    //
    cMatrix44   m_StartNodeWorldTransform;
    //rest pose
    cMatrix44   m_StartNodeTransform;
    sSRT        m_StartSRT;

    cglTFNodeData(const tinygltf::Node&e_Node,int e_iNodeIndex);
    virtual ~cglTFNodeData();

    cglTFNodeData* FinChildByName(const wchar_t* e_strBoneName);
    void ApplySRT(const sSRT& srt, bool e_bSetChildBonesDirty);

    virtual	void        Update(float e_fElpaseTime)override;
    virtual	void        Render()override;
};