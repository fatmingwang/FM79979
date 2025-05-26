#pragma once
#include "tiny_gltf.h"
#include "glTFMesh.h"
#include "glTFAnimationMesh.h"
#include "glTFAnimation.h"

//glTF uses a right-handed coordinate system. glTF defines +Y as up, +Z as forward, and -X as right; the front of a glTF asset faces +Z.
//so camera use right hand too
class cglTFModel:public NamedTypedObject
{
    std::shared_ptr<class cglTFCamera>      m_pCamera;
	std::shared_ptr<class cglTFLight>       m_pLight;
    GLuint                                  CreateShader(int64 fvfFlags, int e_iNumMorphTarget);
    std::map<int64, GLuint>                 m_FVFAndShaderProgramsMap; // FVF -> Shader Program Map
    GLuint                                  GetShaderProgram(int64 fvfFlags, int e_iNumMorphTarget);  // Returns shader based on FVF
    //this is useless but now m_SkinningBoneVector's data from here
    cNamedTypedObjectVector<cglTFNodeData>  m_NodesVector;
    std::map<int, cglTFNodeData*>           m_NodeIndexAndBoneMap;
    void                                    InternalLoadNode(const tinygltf::Node& node, const tinygltf::Model& model, cglTFNodeData* parentBone, std::map<const tinygltf::Node*, cglTFNodeData*>& e_tinyglTFNodeAndJointIndexMap, bool e_bCalculateBiNormal);
    void                                    LoadNodes(const tinygltf::Model& model, bool e_bCalculateBiNormal);
    void                                    PopulateUniform(int e_iProgram);
    void                                    PopulateAttribute(int e_iProgram);
    void                                    loadAnimations(const tinygltf::Model& model);
    std::map<std::string, sAnimationData*>  m_NameAndAnimationMap;
    cAnimationClip                          m_AnimationClip;
    void                                    AssignMeshAttributes(cMesh*e_pMesh, const  tinygltf::Mesh& e_Mesh, const  tinygltf::Model& e_Model, bool e_bCalculateBiNormal);
    shared_ptr<cMesh>                       GenerateMesh(const tinygltf::Mesh&e_Mesh, const tinygltf::Model&e_Model, bool e_bCalculateBiNormal);
    shared_ptr<cMesh>                       GenerateAnimationMesh(const tinygltf::Skin&e_Skin, const tinygltf::Mesh& e_Mesh,const tinygltf::Model& e_Model, bool e_bCalculateBiNormal);
    friend class cAnimationClip;
    friend class cSkinningMesh;
public:
    DEFINE_TYPE_INFO();
    cglTFModel(){ }
    virtual ~cglTFModel();
    std::map<std::string, shared_ptr<cMesh>> m_NameAndMeshes;
    std::map<std::string, shared_ptr<cSkinningMesh>> m_AnimationMeshMap;

    bool    LoadFromGLTF(const std::string& filename,bool e_bCalculateBiNormal = false);
    void    InitBuffers();
    void    Update(float e_fEpaseTime);
    void    Render();
    void    Destory();
    //

    void    SetCurrentAnimation(const std::string& animationName);
    void    SetCurrentAnimationTime(float e_fCurrentTime);
};