#pragma once
#include "tiny_gltf.h"
#include "glTFMesh.h"
#include "glTFAnimationMesh.h"
#include "glTFAnimation.h"
#include "Common.h"

//glTF uses a right-handed coordinate system. glTF defines +Y as up, +Z as forward, and -X as right; the front of a glTF asset faces +Z.
//so camera use right hand too
//!!!!!!!!!!  please do not delete it directly instead use   DestoryWithChildren        !!!!!!!!!!!!!!
class cglTFModel:public FATMING_CORE::cRenderObject
{
	static std::map<std::string, cglTFModel*>               m_sNameAndglTFModelMap;
    static std::map<int64, GLuint>                          m_sFVFAndShaderProgramsMap; // FVF -> Shader Program Map
    //
    std::shared_ptr<class cglTFCamera>                      m_pCamera;
	std::shared_ptr<class cglTFLight>                       m_pLight;
    //
    cNamedTypedObjectVector<cglTFNodeData>                  m_NodesVector;
    std::map<int, cglTFNodeData*>                           m_NodeIndexAndBoneMap;
    std::map<std::string, sAnimationData*>                  m_NameAndAnimationMap;
    std::map<std::string, cMesh*>                           m_NameAndMeshes;
    std::map<std::string, cSkinningMesh*>                   m_AnimationMeshMap;
    std::vector<cglTFNodeData*>                             m_TopNodesVector;
    std::vector<cglTFNodeData*>                             m_ContainMeshglTFNodeDataVector;
    cAnimationClip                                          m_AnimationClip;
	int     										        m_iInstanceValue;
	//if skinning mesh and m_iInstanceValue, then this will be created.
    std::vector<std::shared_ptr<class cAnimationInstanceManager>>        m_sAnimationInstanceManagerVector;
    //
    std::map<std::string, sAnimationData*>                  CloneNameAndAnimationMap(cglTFModelRenderNode*e_pglTFModelRenderNode);
    GLuint                                                  CreateShader(int64 e_i64FVFFlags, int e_iNumMorphTarget,struct sTectureAndTexCoordinateIndex* e_pTectureAndTexCoordinateIndex);
    GLuint                                                  GetShaderProgram(int64 fvfFlags,int64 e_i64extureFVF , int e_iNumMorphTarget, struct sTectureAndTexCoordinateIndex* e_pTectureAndTexCoordinateIndex);  // Returns shader based on FVF
    void                                                    InternalLoadNode(const tinygltf::Node& node, const tinygltf::Model& model, cglTFNodeData* parentBone, std::map<const tinygltf::Node*, cglTFNodeData*>& e_tinyglTFNodeAndJointIndexMap, bool e_bCalculateBiNormal);
    void                                                    LoadNodes(const tinygltf::Model& model, bool e_bCalculateBiNormal);
    void                                                    PopulateUniform(int e_iProgram);
    void                                                    PopulateAttribute(int e_iProgram);
    void                                                    loadAnimations(const tinygltf::Model& model);
    void                                                    AssignMeshAttributes(cMesh*e_pMesh, const tinygltf::Node& node,const  tinygltf::Mesh& e_Mesh, const  tinygltf::Model& e_Model, bool e_bCalculateBiNormal);
    cMesh*                                                  GenerateMesh(const tinygltf::Node& node,const tinygltf::Mesh&e_Mesh, const tinygltf::Model&e_Model, bool e_bCalculateBiNormal);
    cMesh*                                                  GenerateAnimationMesh(const tinygltf::Skin&e_Skin, const tinygltf::Node& node, const tinygltf::Mesh& e_Mesh,const tinygltf::Model& e_Model, bool e_bCalculateBiNormal);
    void                                                    Destory();
    friend class cAnimationClip;
    friend class cSkinningMesh;
    friend class cglTFCamera;
public:
    DEFINE_TYPE_INFO();
    cglTFModel();
    virtual ~cglTFModel();

    bool    LoadFromGLTF(const std::string& filename,bool e_bCalculateBiNormal = false,int e_iInstanceValue = 0);
    void    InitBuffers();
    virtual void    Update(float e_fEpaseTime)override;
    virtual void    Render()override;
    void            RenderNodesShadowPass(const cMatrix44& lightViewProj, GLuint shadowShaderProgram);
    virtual void    DebugRender()override;
    void            RenderImGUI();
    void            RenderAnimationImGUI();
    //
    void    SetCurrentAnimation(const std::string& animationName);
    void    SetCurrentAnimationTime(float e_fCurrentTime);
	bool    IsContainAnimation();
    std::vector<std::shared_ptr<class cAnimationInstanceManager>>& GetAnimationInstanceManagerVector(){return   m_sAnimationInstanceManagerVector; }
    cNamedTypedObjectVector<cglTFNodeData>* GetNodeVector()
    {
        return &m_NodesVector;
    }
    class cglTFModelRenderNode* ToRenderNode();
    static void DeleteCachedFiles();
};


class cglTFModelRenderNode :public FATMING_CORE::cRenderObject
{
    cNamedTypedObjectVector<cglTFNodeData>                  m_NodesVector;
    std::map<std::string, sAnimationData*>                  m_NameAndAnimationMap;
    cAnimationClip                                          m_AnimationClip;
    std::map<std::string, cMesh*>                           m_NameAndMeshes;
    std::vector<cglTFNodeData*>                             m_ContainMeshglTFNodeDataVector;
	cglTFModel*                                             m_pSourceglTFModel;
    std::map<int, cglTFNodeData*>                           m_NodeIndexAndBoneMap;
    friend class cglTFModel;
    friend struct sAnimationData;
    friend class cAnimationClip;
    friend class cSkinningMesh;
public:
	cglTFModelRenderNode()
	{
	}
    virtual ~cglTFModelRenderNode()
    {
        DELETE_MAP(m_NameAndAnimationMap);
        DELETE_MAP(m_NameAndMeshes);
    }
    void    Update(float e_fEpaseTime);
    void    Render();
    void    Destory();
    //
    void    SetCurrentAnimation(const std::string& animationName);
    void    SetCurrentAnimationTime(float e_fCurrentTime);
};

void g_fRenderSkeleton(std::map<std::string, cSkinningMesh*>&e_Map);