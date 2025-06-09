#pragma once
#include "tiny_gltf.h"
#include "glTFMesh.h"
#include "glTFAnimationMesh.h"
#include "glTFAnimation.h"


template<class TYPE>
class cSharedObjectVector:public NamedTypedObject
{
	std::vector<std::shared_ptr<TYPE>> m_ObjectVector;
public:
    cSharedObjectVector(){ }
    virtual ~cSharedObjectVector(){}
    bool Add(std::shared_ptr<TYPE> e_pObject)
    {
        if(!Get(e_pObject->GetName()))
        {
            m_ObjectVector.push_back(e_pObject);
            return true;
        }
        return false;
    }
    size_t Size()
    {
        return m_ObjectVector.size();
	}
    std::shared_ptr<TYPE> Get(const std::wstring e_strName)
    {
        return Get(e_strName.c_str());
	}
    std::shared_ptr<TYPE> Get(const wchar_t* e_strName)
    {
        if (e_strName)
        {
            for (const auto& l_Obj : m_ObjectVector)
            {
                if (l_Obj && wcscmp(l_Obj->GetName(), e_strName) == 0)
                {
                    return l_Obj;
                }
            }
        }
        return nullptr;
    }
    std::vector<std::shared_ptr<TYPE>>& GetVector()
    {
        return m_ObjectVector;
    }
    std::shared_ptr<TYPE> operator [](int e_iIndex)
    {
        return m_ObjectVector[e_iIndex];
    }
};

//glTF uses a right-handed coordinate system. glTF defines +Y as up, +Z as forward, and -X as right; the front of a glTF asset faces +Z.
//so camera use right hand too
class cglTFModel:public FATMING_CORE::cRenderObject
{
	static std::map<std::string, cglTFModel*> m_sNameAndglTFModelMap;
    virtual	void		                    TransformChangedInternalData()override;
    std::shared_ptr<class cglTFCamera>      m_pCamera;
	std::shared_ptr<class cglTFLight>       m_pLight;
    GLuint                                  CreateShader(int64 fvfFlags, int e_iNumMorphTarget);
    std::map<int64, GLuint>                 m_FVFAndShaderProgramsMap; // FVF -> Shader Program Map
    GLuint                                  GetShaderProgram(int64 fvfFlags, int e_iNumMorphTarget);  // Returns shader based on FVF
    //this is useless but now m_SkinningBoneVector's data from here
    cSharedObjectVector<cglTFNodeData>      m_NodesVector;
    std::map<int, cglTFNodeData*>           m_NodeIndexAndBoneMap;
    void                                    InternalLoadNode(const tinygltf::Node& node, const tinygltf::Model& model, cglTFNodeData* parentBone, std::map<const tinygltf::Node*, cglTFNodeData*>& e_tinyglTFNodeAndJointIndexMap, bool e_bCalculateBiNormal);
    void                                    LoadNodes(const tinygltf::Model& model, bool e_bCalculateBiNormal);
    void                                    PopulateUniform(int e_iProgram);
    void                                    PopulateAttribute(int e_iProgram);
    void                                    loadAnimations(const tinygltf::Model& model);
    std::map<std::string,std::shared_ptr<sAnimationData>>  m_NameAndAnimationMap;
    cAnimationClip                          m_AnimationClip;
    void                                    AssignMeshAttributes(cMesh*e_pMesh, const  tinygltf::Mesh& e_Mesh, const  tinygltf::Model& e_Model, bool e_bCalculateBiNormal);
    shared_ptr<cMesh>                       GenerateMesh(const tinygltf::Mesh&e_Mesh, const tinygltf::Model&e_Model, bool e_bCalculateBiNormal);
    shared_ptr<cMesh>                       GenerateAnimationMesh(const tinygltf::Skin&e_Skin, const tinygltf::Mesh& e_Mesh,const tinygltf::Model& e_Model, bool e_bCalculateBiNormal);
    friend class cAnimationClip;
    friend class cSkinningMesh;
    std::map<std::string, shared_ptr<cMesh>>            m_NameAndMeshes;
    std::vector<cglTFNodeData*>                         m_ContainMeshglTFNodeDataVector;
    cglTFNodeData* m_pRoot;
public:
    //wrong but I am Lazy
    std::map<std::string, shared_ptr<cSkinningMesh>>    m_AnimationMeshMap;
    DEFINE_TYPE_INFO();
    cglTFModel(){ }
    virtual ~cglTFModel();

    bool    LoadFromGLTF(const std::string& filename,bool e_bCalculateBiNormal = false);
    void    InitBuffers();
    void    Update(float e_fEpaseTime);
    void    Render();
    void    Destory();
    //

    void    SetCurrentAnimation(const std::string& animationName);
    void    SetCurrentAnimationTime(float e_fCurrentTime);
};


class cglTFModelRenderNode :public FATMING_CORE::cRenderObject
{

};

void g_fRenderSkeleton(cglTFModel*e_pglTFModel);