#pragma once
#include "gltfMesh.h"
#include <map>
#include <string>

struct SRT
{
    Vector3 scale;
    Quaternion rotation;
    Vector3 translation;
    int     iSRTFlag = 0;//S 1<<1 R 1<< 2 T 1<<3
};
#define SRT_SCALE_FLAG          (1<<1)
#define SRT_ROTATION_FLAG       (1<<2)
#define SRT_TRANSLATION_FLAG    (1<<3)

using FloatToSRTMap = std::map<float, SRT>;
typedef std::map<float, cMatrix44> FloatTocMatrix44Map;

struct cBone : public Frame
{
    void ConvertSRTMapToMatrixMap(const FloatToSRTMap& srtMap, FloatTocMatrix44Map& matrixMap);
    std::pair <float,SRT>     m_PreviousSRT;
    int     m_iJointIndex = -1;
    int     m_iNodeIndex;
    float m_fMinKeyTime;
    float m_fMaxKeyTime;
    FloatToSRTMap               m_FormKeyFrames;
    FloatTocMatrix44Map         m_MatrixKeyFrames;
    cMatrix44 m_matInvBindPose;
    std::wstring m_strSID;

    cBone(const WCHAR* e_strName,int e_iJointIndex);
    ~cBone();
    cBone* FinChildByName(const wchar_t* e_strBoneName);
    void SetFormKeyFrames(FloatToSRTMap e_FormKeyFrames);
    void SetFormKeyFrames(FloatTocMatrix44Map e_FormKeyFrames);
    void EvaluateLocalXForm(float e_fTime, bool e_bSetChildBonesDirty = true);
    void EvaluateLocalXForm2(float e_fTime, bool e_bSetChildBonesDirty = true);
    void ApplySRT(const SRT& srt, bool e_bSetChildBonesDirty);
    cMatrix44   m_StartWorldTransform;
    cMatrix44   m_StartLocalTransform;
    SRT         m_StartSRT;
};
