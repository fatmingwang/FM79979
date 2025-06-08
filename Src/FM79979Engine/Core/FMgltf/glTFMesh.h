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

class cMesh:public FATMING_CORE::cRenderObject
{
    typedef std::vector<std::map<int, float*>> sMorphTargetVector;
protected:
    struct sSubMesh
    {
        int64                       m_i64FVFFlag = 0;
        GLuint                      m_iShaderProgram;
        size_t                      m_i64VertexCount = 0;
        GLuint                      m_iVBOArray[TOTAL_FVF];
        std::vector<uint32_t>       m_IndexBuffer;
        GLuint                      m_uiVAO;
        GLuint                      m_uiEBO;
        Vector3                     m_vMinBounds;
        Vector3                     m_vMaxBounds;
        //for debug
        shared_ptr<sMorphTargetVector>    m_spFVFAndVertexDataMorphTargetMap;
        int 					    m_iNumMorphTarget = 0;
        void                        GetProperCameraPosition(cMatrix44& e_CameraMatrix);
        void                        ClearOpenGLData();
    };
    //
    //useless?
    std::vector<double>             m_MorphWeights; // Store blending weights per target
    //from sAnimationData,as many as m_SubMeshesVector.size(),all submesh has same count
	std::vector<float>			    m_CurrentAnimationMorphPrimitiveWeightsVector;
    void                            ApplyMorphUniformData(struct sSubMesh*e_pSubMesh);
    virtual void					SetSubMeshCommonUniformData(sSubMesh*e_pMesh,cMatrix44&e_mat);
    void					        CallOpenGLDraw(sSubMesh* e_pMesh);
protected:
    shared_ptr<cMaterial>m_Material;
    void    ApplyMaterial();
    friend class cglTFModel;
    friend class cAnimationClip;
public:
    Vector3 m_vMinBounds;
    Vector3 m_vMaxBounds;
protected:
    std::vector<sSubMesh*>  m_SubMeshesVector;  // Store different primitives
    void                    GenerateNormalAttribute(const tinygltf::Model& e_Model,const tinygltf::Primitive& primitive, sSubMesh*e_pSubMesh);
public:
    DEFINE_TYPE_INFO();
    cMesh();
    virtual ~cMesh();

    //void InitBuffer();  // Initialize and bind buffers
    virtual void Render();        // Draw the mesh

    // Function to load vertex attributes and indices
    //void LoadAttributes(const tinygltf::Model& model, const tinygltf::Primitive& primitive, bool calculateBinormal);
    void            LoadMaterial(const tinygltf::Model& model, const tinygltf::Material& material,unsigned int e_uiShaderProgram);
    void            LoadAttributesAndInitBuffer(const tinygltf::Model& model, const tinygltf::Primitive& primitive, bool calculateBinormal);
    void            LoadMorphingAttributes(sSubMesh*e_pSubMesh,const tinygltf::Model& model, const tinygltf::Primitive& primitive, bool calculateBinormal);
    void            SetMorphingWeights(const std::vector<double>&e_Weights);
    void            logFVFFlags();
};