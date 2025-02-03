#pragma once
#include "gltfMesh.h"
#include <map>
#include <string>

typedef std::map<float, cMatrix44> FloatTocMatrix44Map;

struct cBone : public Frame
{
    int     m_iJointIndex;
    float m_fMinKeyTime;
    float m_fMaxKeyTime;
    FloatTocMatrix44Map m_FormKeyFrames;
    cMatrix44 m_matInvBindPose;
    bool m_bAnimation;
    std::wstring m_strSID;

    cBone(const WCHAR* e_strName,int e_iJointIndex);
    ~cBone();
    cBone* FinChildByName(const wchar_t* e_strBoneName);
    void SetFormKeyFrames(FloatTocMatrix44Map e_FormKeyFrames);
    void EvaluateLocalXForm(float e_fTime, bool e_bSetChildBonesDirty = true);
};
