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

using FloatToSRTMap = std::map<float, SRT>;

typedef std::map<float, cMatrix44> FloatTocMatrix44Map;

struct cBone : public Frame
{
    int     m_iJointIndex;
    float m_fMinKeyTime;
    float m_fMaxKeyTime;
    FloatToSRTMap m_FormKeyFrames;
    cMatrix44 m_matInvBindPose;
    bool m_bAnimation;
    std::wstring m_strSID;

    cBone(const WCHAR* e_strName,int e_iJointIndex);
    ~cBone();
    cBone* FinChildByName(const wchar_t* e_strBoneName);
    void SetFormKeyFrames(FloatToSRTMap e_FormKeyFrames);
    void EvaluateLocalXForm(float e_fTime, bool e_bSetChildBonesDirty = true);
};
