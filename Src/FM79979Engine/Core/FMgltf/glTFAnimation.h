#pragma once

#include "glTFNode.h"
#include "Common.h"
class cglTFNodeData;
struct sAnimationData:public NamedTypedObject
{
    sAnimationData()
    {
		m_BoneIDAndAnimationData = std::make_shared<std::map<int, FloatToSRTMap>>();
    }
    std::shared_ptr<std::map<int, FloatToSRTMap>>                m_BoneIDAndAnimationData;
    cNamedTypedObjectVector<cglTFNodeData>*     m_pBoneVector;
    //morphing
    //here only contain weights because position and others are store in SubMesh
    //here only apply weight and sub mesh do it's own morpthing.
    std::map<float, std::vector<float>>         m_TimaAndMorphWeightMap;
    class cMesh*                                m_pTargetMesh = nullptr;
    int                                         m_iTargetNodeIndex = -1;
    bool    m_bLoop;
    float   m_fMinKeyTime;
    float   m_fMaxKeyTime;
    float   m_fCurrentTime;
    float   m_fStartTime;
    float   m_fEndTime;
    void    Update(float e_fElpaseTime);
    std::vector<float> GetInterpolatedWeights(float e_fTime);
    static sAnimationData* CloneFromModel(class cglTFModelRenderNode* e_pglTFModelRenderNode, sAnimationData*e_pSource);
};


class cAnimationClip :public NamedTypedObject
{
    friend class cglTFModel;
    friend class cglTFModelRenderNode;
    //from model
    std::map<int, cglTFNodeData*>*          m_pNodeIndexAndBoneMap;
    std::map<std::string, sAnimationData*>* m_pNameAndAnimationMap;
    cNamedTypedObjectVector<cglTFNodeData>* m_pNodesVector;
    std::vector<sSRT>                       m_SRTVector;
    std::string                             m_strAnimationName;
    sAnimationData*                         m_pCurrentAnimationData = nullptr;
    float                                   m_fBlendingTime = 0;
    //class cglTFModel*                       m_pglTFModel = nullptr;
    bool                                    SampleToTime(float e_fTime, bool e_bAssignToBone, std::vector<sSRT>* e_pSRTVector = nullptr);
    void	                                UpdateNode(cglTFNodeData* e_pBone, float e_fTime, sSRT& e_SRT, bool e_bAssignToBone);
    friend class cAnimTexture;
    friend class cAnimationInstanceManager;
public:
    void	SetBoneAndAnimationData(class cglTFModel* e_pglTFModel);
    void	SetBoneAndAnimationData(class cglTFModelRenderNode* e_pglTFModel);
    bool    SetAnimation(const char* e_strAnimationName, bool e_bLoop, float e_fTargetTime = 0.f);
    void    UpdateToTargetTime(float e_fTime, bool e_bAssignToBone);
    void    BlendClips(float e_fTime, const char* e_strAnimationName1, const char* e_strAnimationName2, bool e_bAssignToBone, bool e_bLoop = true, float e_fTargetFactor = 0.5);
    void    Update(float e_fElpaseTime);
    sAnimationData* GetAnimationData(std::string e_strName);
    sAnimationData* GetCurrentAnimationData()
    {
        return m_pCurrentAnimationData;
    }
};

class cAnimTexture :public NamedTypedObject
{
    friend class cAnimationInstanceManager;
    float* m_pfTextureData;
    unsigned int m_TextureSquareSize = -1;
    unsigned int m_uiOpenglTecxtureID = -1;
public:
    cAnimTexture(cAnimationClip&e_AnimationClip, const char* e_strAnimationName);
    cAnimTexture(const cAnimTexture&);
    cAnimTexture& operator=(const cAnimTexture&);
    cAnimTexture(const char* path);
    cAnimTexture(unsigned int size);
    ~cAnimTexture();

    bool Load(const char* path);
    bool Save(const char* path);
    void UploadTextureDataToGPU();

    unsigned int Size();
    //make it square
    void Resize(unsigned int e_uiWidthAndHeight);
    float* GetData();

    void Set(unsigned int uniformIndex, unsigned int textureIndex);
    void UnSet(unsigned int textureIndex);
    unsigned int GetHandle();
    void        SetTexel(unsigned int x, unsigned int y, const Vector4& e_Value);
    void        SetTexel(unsigned int x, unsigned int y, const Vector3& e_Value);
};
#define ANIMATION_TEXTURE_FPS       30.0f
#define ANIMATION_FRAME_STEP_TIME   (1/ANIMATION_TEXTURE_FPS)

struct sAnimationFrameAndTime :public NamedTypedObject
{
    float m_fNextFrameLerpTimes;
    float m_fCurrentPlayTime;
    int   m_iNextFrame;
    int   m_iCurrentFrame;
    float m_fAnimationEndTime = 0.f;
    bool  m_bLoop = false;
    int   m_iLastFrame = -1;
public:
    sAnimationFrameAndTime(float e_fEndTime);
    void Update(float e_fElpaseTime);
};

struct sAniamationInstanceData :public NamedTypedObject
{
    std::vector<std::shared_ptr<sAnimationFrameAndTime>>    m_AnimationFrameAndTimeVector;
	std::shared_ptr<cAnimTexture>                           m_spAnimTexture;
    struct sFrameIndex
    {
        int iCurrent = 0;
        int iNext = 1;
    };
    std::vector<sFrameIndex>    m_FrameIndexVector;
    std::vector<float>          m_ToNextLerpTime;
    sAniamationInstanceData(cAnimationClip* e_pAnimationClip, const char* e_strAnimationName,int e_iNumInstanceData);
};


class cAnimationInstanceManager :public NamedTypedObject
{
	std::shared_ptr<class cMeshInstance> m_spMeshInstance;
	std::map<std::string,std::shared_ptr<sAniamationInstanceData>> m_AnimationNameAndAniamationInstanceDataMap;
    cAnimationClip*m_pAnimationClip = nullptr;
    void GenerateAnimationNameAndAniamationInstanceDataMap(int e_iNumInstanceData, const char* e_strAnimationName);
    GLuint m_uiProgramID;
public:
    cAnimationInstanceManager(cAnimationClip* e_pAnimationClip, std::shared_ptr<class cMeshInstance>, GLuint e_uiProgramID);
    virtual ~cAnimationInstanceManager();
    void Render(GLuint e_uiProgramID, std::shared_ptr<sAniamationInstanceData>);
    
    std::tuple<std::shared_ptr<sAniamationInstanceData>, GLuint >    GetAnimationInstanceData(const char* e_strAnimationName);
};