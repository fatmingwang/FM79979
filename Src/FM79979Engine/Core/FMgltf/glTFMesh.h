#pragma once

//Explanation:
//Position(FVF_POSITION1) : Vertex position(typically vec3).
//Normal(FVF_NORMAL) : Normal vector(typically vec3).
//Color(FVF_DIFFUSE) : Color data(typically vec4), useful for vertex colors.
//Texture Coordinates(FVF_TEX0) : Texture UV coordinates(typically vec2).
//Tangent(FVF_TANGENT5) : Tangent vector(typically vec3), useful for normal mapping.
//Binormal(FVF_BITAGENT) : Binormal vector(typically vec3), needed for normal mapping(or TBN matrix).
//Normal Map(FVF_NORMAL_MAP) : Flag to enable the use of normal maps in the fragment shader.
//Metallic(FVF_METALLIC) : Metallic texture, typically part of PBR workflows.
//Roughness(FVF_ROUGHNESS) : Roughness texture, also part of PBR workflows.
//Emissive(FVF_EMISSIVE) : Emissive texture for lighting effects like glow.
//Occlusion(FVF_OCCLUSION) : Occlusion texture, typically used for PBR(ambient occlusion).
//Base Color(FVF_BASE_COLOR) : Base color texture(diffuse map or albedo in PBR).
//PBR(FVF_PBR) : Convenience flag for PBR, which combines all the necessary attributes.
//All(FVF_ALL) : Combines all common attributes like position, normal, texture coordinates, tangent, and binormal.
//
//The FVF flags are used to define which attributes are present in your mesh data.For example, if you want to use a normal map in your fragment shader, you would enable the FVF_NORMAL_MAP flag.
//In your cMesh::Draw or cMesh::LoadAttributes function, you can check these flags and dynamically modify the shaders or buffer binding accordingly.
//
//Let's say you want to load a mesh with positions, normals, and texture coordinates, but you don't want tangents or binormals, and you want to include a normal map for the fragment shader.You could set the FVF flags as follows :
//unsigned int fvfFlags = FVF_POSITION1 | FVF_NORMAL | FVF_TEX0 | FVF_NORMAL_MAP;
//This tells the system to only use position, normal, texture coordinates, and normal map in the shader, and the shaders would be generated accordingly.

#pragma once

#include <vector>
#include <string>
#include <map>

#include "../AllCoreInclude.h"
#include "glTFMaterial.h"

class cMeshInstance;
class cMesh:public FATMING_CORE::cRenderObject
{
    typedef std::vector<std::map<int, float*>> sMorphTargetVector;
    friend class cMeshInstance;
protected:
    struct sSubMesh
    {
        //for debug
        shared_ptr<sMorphTargetVector>    m_spFVFAndVertexDataMorphTargetMap;
        //
        int64                       m_i64FVFFlag = 0;
        GLuint                      m_iShaderProgramID;
        size_t                      m_i64VertexCount = 0;
        GLuint                      m_iVBOArray[TOTAL_FVF];
        std::vector<GLuint>         m_IndexBuffer;
        GLuint                      m_uiVAO = -1;
        GLuint                      m_uiEBO = -1;
        Vector3                     m_vMinBounds;
        Vector3                     m_vMaxBounds;
        int 					    m_iNumMorphTarget = 0;
        void                        GetProperCameraPosition(cMatrix44& e_CameraMatrix);
        //void LoadAttributes(const tinygltf::Model& model, const tinygltf::Primitive& primitive, bool calculateBinormal);
        void                        ApplyMaterial();
        shared_ptr<cMaterial>       m_Material;
        ~sSubMesh();
    };
    //
    //useless?
    //std::vector<double>             m_MorphWeights; // Store blending weights per target
    //from sAnimationData,as many as m_SubMeshesVector.size(),all submesh has same count
	std::vector<float>			    m_CurrentAnimationMorphPrimitiveWeightsVector;
    void                            ApplyMorphUniformData(struct sSubMesh*e_pSubMesh);
    virtual void					SetSubMeshCommonUniformData(sSubMesh*e_pMesh,cMatrix44&e_mat);
    void					        CallOpenGLDraw(sSubMesh* e_pMesh);
protected:
    void    ApplyMaterial();
    friend class cglTFModel;
    friend class cAnimationClip;
public:
    Vector3 m_vMinBounds = Vector3(-999,-999,-999);
    Vector3 m_vMaxBounds = Vector3::Zero;
protected:
    std::vector<std::shared_ptr<sSubMesh>>              m_SubMeshesVector;
    std::vector<std::shared_ptr<cMaterial>>             m_MaterialVector;
    std::map<std::string,shared_ptr<cMaterial>>         m_MaterialNameAndMaterialMap;
    void                    GenerateNormalAttribute(const tinygltf::Model& e_Model,const tinygltf::Primitive& primitive, sSubMesh*e_pSubMesh);
public:
    DEFINE_TYPE_INFO();
    LAZY_CLONE_FUNCTION(cMesh);
    cMesh();
    cMesh(cMesh*e_pMesh);
    virtual ~cMesh();

    //void InitBuffer();  // Initialize and bind buffers
    virtual void Render()override;        // Draw the mesh
    virtual void Render(cMeshInstance*e_pMeshInstance);
    virtual void RenderShadow(const cMatrix44& lightViewProj, const cMatrix44& modelMatrix, GLuint shadowShaderProgram);
    

    // Function to load vertex attributes and indices
    void                            LoadAttributesAndInitBuffer(const tinygltf::Model& model, const tinygltf::Primitive& primitive, bool calculateBinormal);
    void                            LoadMorphingAttributes(sSubMesh*e_pSubMesh,const tinygltf::Model& model, const tinygltf::Primitive& primitive, bool calculateBinormal);
    shared_ptr<cMaterial>           LoadMaterial(const tinygltf::Model& model, const tinygltf::Material& material, std::shared_ptr<sSubMesh>e_SubMesh);
    void                            logFVFFlags();
    virtual void                    AfterCloneSetBoneData(class cglTFModelRenderNode* e_pData){}
    GLuint                          GetSubMeshShaderProgramID(int e_iIndexOfSubMesh = 0);
	void                            RenderImGUI();
};

// Instance manager for cMesh
class cMeshInstance:public NamedTypedObject
{
public:
    cMeshInstance();
    cMeshInstance(cMeshInstance*e_pMeshInstance);
    ~cMeshInstance();

    // Set per-instance transforms
    void SetInstanceTransforms(const std::vector<cMatrix44>& e_TransformVector);

    // Clear all instances
    void Clear();

    // Get instance count
    size_t GetCount() const
    {
        return m_InstanceTransformVector.size();
    }

    // Get instance transforms
    std::vector<cMatrix44>* GetTransforms()
    {
        return &m_InstanceTransformVector;
    }

    // OpenGL buffer management
    void InitBuffer(const std::vector<std::shared_ptr<cMesh::sSubMesh>>& e_SubMesheVector);
    void UpdateBuffer();

    GLuint GetInstanceVBO() const
    {
        return m_InstanceVBO;
    }
    bool IsBufferDirty() const
    {
        return m_BufferDirty;
    }
    void MarkBufferDirty()
    {
        m_BufferDirty = true;
    }
    static std::shared_ptr<cMeshInstance>CreateInstance()
    {
        return std::make_shared<cMeshInstance>();
    }
private:
    std::vector<cMatrix44> m_InstanceTransformVector;
    GLuint m_InstanceVBO = 0;
    bool m_BufferDirty = false;
};