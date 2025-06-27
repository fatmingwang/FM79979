#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "glTFMesh.h"

#include <algorithm>
#include <cstring>
#include <unordered_set>
#include <set>
#include "glTFLight.h"
#include"glTFCamera.h"
#include "glTFNode.h"

TYPDE_DEFINE_MARCO(cMesh);

cMesh::cMesh()
{
}

cMesh::cMesh(cMesh* e_pMesh)
{
    SetName(e_pMesh->GetName());
    m_vMinBounds = e_pMesh->m_vMinBounds;
    m_vMaxBounds = e_pMesh->m_vMaxBounds;
    m_SubMeshesVector = e_pMesh->m_SubMeshesVector;
}

cMesh::~cMesh()
{
}

void cMesh::ApplyMorphUniformData(sSubMesh* e_pSubMesh)
{
    FORCE_CHECK_GL_ERROR("");
    if (m_CurrentAnimationMorphPrimitiveWeightsVector.size() && e_pSubMesh->m_spFVFAndVertexDataMorphTargetMap)
    {//setup how many primitive and weights data
        auto l_uiUniform = glGetUniformLocation(e_pSubMesh->m_iShaderProgramID,"uMorphWeights");
        if (l_uiUniform != GL_INVALID_INDEX)
        {
            auto l_uiSize = e_pSubMesh->m_spFVFAndVertexDataMorphTargetMap->size();
            LAZY_DO_GL_COMMAND_AND_GET_ERROR(glUniform1fv((GLsizei)l_uiUniform, (GLsizei)l_uiSize, m_CurrentAnimationMorphPrimitiveWeightsVector.data()));
        }
    }
}

void cMesh::GenerateNormalAttribute(const tinygltf::Model& e_Model,const tinygltf::Primitive& primitive, sSubMesh* e_pSubMesh)
{
    FORCE_CHECK_GL_ERROR("");
    // Generate normals if they are not present
    if (primitive.attributes.find("NORMAL") == primitive.attributes.end())
    {
        int l_iFVFIndex = FVF_NORMAL;
        const tinygltf::Accessor& accessor = e_Model.accessors[primitive.attributes.at("POSITION")];
        const tinygltf::BufferView& bufferView = e_Model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer& buffer = e_Model.buffers[bufferView.buffer];

        size_t byteOffset = accessor.byteOffset + bufferView.byteOffset;
        const unsigned char* dataPtr = buffer.data.data() + byteOffset;

        e_pSubMesh->m_i64FVFFlag |= 1LL << l_iFVFIndex;
        size_t dataSize = accessor.count * accessor.ByteStride(bufferView);

        std::vector<Vector3> normals;
        normals.resize(accessor.count);
        Vector3* l_pPositionData = (Vector3*)dataPtr;
        for (size_t i = 0; i < e_pSubMesh->m_IndexBuffer.size(); i += 3)
        {
            GLuint index0 = e_pSubMesh->m_IndexBuffer[i];
            GLuint index1 = e_pSubMesh->m_IndexBuffer[i + 1];
            GLuint index2 = e_pSubMesh->m_IndexBuffer[i + 2];

            Vector3 v0 = l_pPositionData[index0];
            Vector3 v1 = l_pPositionData[index1];
            Vector3 v2 = l_pPositionData[index2];

            Vector3 edge1 = v1 - v0;
            Vector3 edge2 = v2 - v0;
            Vector3 normal = edge1 ^ edge2;            
            //do normalize? normal.NormalizeIt();

            normals[index0] += normal;
            normals[index1] += normal;
            normals[index2] += normal;
        }

        for (auto& normal : normals)
        {
            normal.Normalize();
        }

        // Upload generated normals to GPU
        glGenBuffers(1, &e_pSubMesh->m_iVBOArray[l_iFVFIndex]);
        glBindBuffer(GL_ARRAY_BUFFER, e_pSubMesh->m_iVBOArray[l_iFVFIndex]);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vector3), normals.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(l_iFVFIndex);
        glVertexAttribPointer(l_iFVFIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), nullptr);
        //FMLOG("SubMesh attribute name:%s VBO ID: %d, FVF Index: %d DataSize: %d Type:%d", "Normal", e_pSubMesh->m_iVBOArray[l_iFVFIndex], l_iFVFIndex, dataSize, accessor.componentType);

    }
}




void cMesh::sSubMesh::GetProperCameraPosition(cMatrix44& e_CameraMatrix)
{
    Vector3 center = (m_vMinBounds + m_vMaxBounds) * 0.5f;
    Vector3 l_Extends = m_vMaxBounds - m_vMinBounds;
    float l_fSize = max(l_Extends.x,l_Extends.y);
    l_fSize = max(l_fSize, l_Extends.z);
    //float l_fEffectiveSize = max(l_fSize, 1);
    //// Calculate distance based on FOV
    //float fovY = 45 * (3.14159265358979323846f / 180.0f);
    //float distance = (l_fEffectiveSize / 2.0f) / std::tan(fovY / 2.0f) * l_fSize;
    //// Apply minimum and maximum distance constraints
    //float minDistance = 1.0f;
    //float maxDistance = 1000.0f;
    //distance = std::clamp(distance, minDistance, maxDistance);
    center.y *= -1;
    // Set the camera position to be a bit further away from the center of the mesh
    Vector3 cameraPosition = center + Vector3(0, 0, l_fSize*2);

    // Assuming you have a function to set the view matrix in the projectionMatrix
    e_CameraMatrix = cMatrix44::LookAtMatrix(cameraPosition, center, Vector3(0, 1, 0));
}

void cMesh::sSubMesh::ApplyMaterial()
{
    FORCE_CHECK_GL_ERROR("");
    if (this->m_Material)
    {
        this->m_Material->Apply();
    }
    else
    {
        GLuint metallicFactorLoc = glGetUniformLocation(this->m_iShaderProgramID, "uBaseColorFactor");
        if (metallicFactorLoc != GL_INVALID_INDEX)
        {
            Vector4 l_vbaseColorFactor = Vector4::One;
            glUniform4fv(metallicFactorLoc,1,(float*)&l_vbaseColorFactor);
        }
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

shared_ptr<cMaterial>   cMesh::LoadMaterial(const tinygltf::Model& e_Model, const tinygltf::Material& e_Material, std::shared_ptr<sSubMesh>e_SubMesh)
{
    FORCE_CHECK_GL_ERROR("");
	auto l_IT = m_MaterialNameAndMaterialMap.find(e_Material.name);
    if(l_IT != m_MaterialNameAndMaterialMap.end() && e_Material.name.length() != 0)
    {//same material ignore.
        e_SubMesh->m_Material = l_IT->second;
        return e_SubMesh->m_Material;
	}
    auto l_Material = std::make_shared<cMaterial>();
    l_Material->SetName(e_Material.name.c_str());
    l_Material->LoadMaterials(e_Model, e_Material);
	e_SubMesh->m_Material = l_Material;
    m_MaterialNameAndMaterialMap[e_Material.name] = l_Material;
    m_MaterialVector.push_back(l_Material);
    //FMLOG_FORMAT("Material name:%s",e_Material.name.c_str());
    //FMLOG("Material name:%s", e_Material.name.c_str());
    return e_SubMesh->m_Material;
}

//cMesh::sSubMesh::sSubMesh(sSubMesh* e_psSubMesh)
//{
//    *this = *e_psSubMesh;
//    m_spFVFAndVertexDataMorphTargetMap = e_psSubMesh->m_spFVFAndVertexDataMorphTargetMap;
//    this->m_Material = e_psSubMesh->m_Material;
//}

cMesh::sSubMesh::~sSubMesh()
{
    for (int i = 0; i < TOTAL_FVF; ++i)
    {
        if (i & this->m_i64FVFFlag)
        {
            glDeleteBuffers(1, &m_iVBOArray[i]);
        }
    }
    glDeleteBuffers(1, &m_uiEBO);
    glDeleteVertexArrays(1, &m_uiVAO);
}

void	cMesh::CallOpenGLDraw(sSubMesh* e_pSubMesh)
{
    if (e_pSubMesh->m_IndexBuffer.empty())
    {
        FMLOG("e_pSubMesh m_IndexBuffer.empty()");
        return;
    }
    FORCE_CHECK_GL_ERROR("111");
    glBindVertexArray(e_pSubMesh->m_uiVAO);
    //GLint maxAttribs;
    //glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
    //GLint currentVAO, currentIBO, currentProgram;
    //glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
    //glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &currentIBO);
    //glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    //printf("VAO: %d, IBO: %d, Program: %d\n", currentVAO, currentIBO, currentProgram);
    //GLint maxAttribs;
    //glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
    //for (GLint i = 0; i < maxAttribs; i++)
    //{
    //    GLint enabled;
    //    glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
    //    if (enabled) printf("Attribute %d enabled\n", i);
    //}
    FORCE_CHECK_GL_ERROR("3939889");
    MY_GLDRAW_ELEMENTS(GL_TRIANGLES, (GLsizei)e_pSubMesh->m_IndexBuffer.size(), GL_UNSIGNED_INT, 0);
    FORCE_CHECK_GL_ERROR("333");
    glBindVertexArray(0);
}

void cMesh::SetSubMeshCommonUniformData(sSubMesh* e_pSubMesh, cMatrix44& e_mat)
{
    FORCE_CHECK_GL_ERROR("");
    // Use the shader program specific to this sub-mesh
    glUseProgram(e_pSubMesh->m_iShaderProgramID);
    ApplyMorphUniformData(e_pSubMesh);
    g_fSetLightUniform(e_pSubMesh->m_iShaderProgramID);
    GLuint l_uiMat4ModelLocation = glGetUniformLocation(e_pSubMesh->m_iShaderProgramID, "uMat4Model");
    if (l_uiMat4ModelLocation != GL_INVALID_INDEX)
    {
        glUniformMatrix4fv(l_uiMat4ModelLocation, 1, GL_FALSE, e_mat);
    }
    g_fSetCameraUniform(e_pSubMesh->m_iShaderProgramID);
    e_pSubMesh->ApplyMaterial();
    
}

void cMesh::Render()
{
    auto l_matTransform = this->GetWorldTransform();
    for (auto l_pSubMesh : m_SubMeshesVector)
    {
        SetSubMeshCommonUniformData(l_pSubMesh.get(), l_matTransform);
        CallOpenGLDraw(l_pSubMesh.get());
    }
}

void cMesh::Render(cMeshInstance* e_pMeshInstance)
{
    if (e_pMeshInstance)
    {
        auto l_matTransform = this->GetWorldTransform();
        size_t instanceCount = e_pMeshInstance->GetCount();
        assert(instanceCount > 0);
        if (e_pMeshInstance->GetInstanceVBO() == 0)
        {
            e_pMeshInstance->InitBuffer(m_SubMeshesVector);
        }

        if (e_pMeshInstance->IsBufferDirty())
        {
            e_pMeshInstance->UpdateBuffer();
        }
        for (auto& subMesh : m_SubMeshesVector)
        {
            SetSubMeshCommonUniformData(subMesh.get(), l_matTransform);
            glBindVertexArray(subMesh->m_uiVAO);
            glDrawElementsInstanced(GL_TRIANGLES,
                                    static_cast<GLsizei>(subMesh->m_IndexBuffer.size()),
                                    GL_UNSIGNED_INT, 0,
                                    static_cast<GLsizei>(instanceCount));
        }
    }
}

void cMesh::logFVFFlags()
{
    return;
    FMLOG("FVF Flags")
    for (std::shared_ptr<sSubMesh> l_pSubMesh : m_SubMeshesVector)
    {
        if (l_pSubMesh->m_i64FVFFlag & FVF_POS_FLAG) FMLOG("Position is present.");
        if (l_pSubMesh->m_i64FVFFlag & FVF_NORMAL_FLAG) FMLOG("Normal is present.");
        if (l_pSubMesh->m_i64FVFFlag & FVF_DIFFUSE_FLAG) FMLOG("Color is present.");
        if (l_pSubMesh->m_i64FVFFlag & FVF_SKINNING_WEIGHT_FLAG) FMLOG("weight is present.");
        if (l_pSubMesh->m_i64FVFFlag & FVF_SKINNING_BONE_INDEX_FLAG) FMLOG("joint is present.");
        if (l_pSubMesh->m_i64FVFFlag & FVF_TEX0_FLAG) FMLOG("Texture coordinates are present.");
        if (l_pSubMesh->m_i64FVFFlag & FVF_TANGENT_FLAG) FMLOG("Tangent is present.");
        if (l_pSubMesh->m_i64FVFFlag & FVF_BINORMAL_FLAG) FMLOG("Binormal is present.");
        if (l_pSubMesh->m_i64FVFFlag & FVF_NORMAL_MAP_TEXTURE_FLAG) FMLOG("Normal map is present.");
    }
}

GLuint cMesh::GetSubMeshShaderProgramID(int e_iIndexOfSubMesh)
{
    if (this->m_SubMeshesVector.size() > e_iIndexOfSubMesh)
    {
        return this->m_SubMeshesVector[e_iIndexOfSubMesh]->m_iShaderProgramID;
    }
    
    return -1;
}


void cMesh::LoadAttributesAndInitBuffer(const tinygltf::Model& e_Model, const tinygltf::Primitive& primitive, bool e_bCalculateBinormal)
{
    bool l_bPosition = primitive.attributes.find("POSITION") != primitive.attributes.end();
    if (!l_bPosition)
    {
        return;
    }
    if (primitive.mode != TINYGLTF_MODE_TRIANGLES)
    {
        assert(0 && "only support triangles now");
        return;
    }
    std::shared_ptr<sSubMesh> l_pSubMesh = std::make_shared<sSubMesh>();
    // Load indices
    if (primitive.indices == -1)
    {
        const tinygltf::Accessor& positionAccessor = e_Model.accessors[primitive.attributes.at("POSITION")];
        size_t vertexCount = positionAccessor.count;
        l_pSubMesh->m_IndexBuffer.resize(vertexCount);
        std::iota(l_pSubMesh->m_IndexBuffer.begin(), l_pSubMesh->m_IndexBuffer.end(), 0);
    }
    else
    {
        const tinygltf::Accessor& indexAccessor = e_Model.accessors[primitive.indices];
        const tinygltf::BufferView& bufferView = e_Model.bufferViews[indexAccessor.bufferView];
        const tinygltf::Buffer& buffer = e_Model.buffers[bufferView.buffer];

        const unsigned char* dataPtr = buffer.data.data() + bufferView.byteOffset + indexAccessor.byteOffset;

        if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
        {
            const uint16_t* shortIndices = reinterpret_cast<const uint16_t*>(dataPtr);
            l_pSubMesh->m_IndexBuffer.assign(shortIndices, shortIndices + indexAccessor.count);
        }
        else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
        {
            const GLuint* intIndices = reinterpret_cast<const GLuint*>(dataPtr);
            l_pSubMesh->m_IndexBuffer.assign(intIndices, intIndices + indexAccessor.count);
        }
        else
        {
            std::cerr << "Unsupported index component type: " << indexAccessor.componentType << std::endl;
            l_pSubMesh = nullptr;
            return;
        }
    }

    // Generate and bind VAO
    glGenVertexArrays(1, &l_pSubMesh->m_uiVAO);
    glBindVertexArray(l_pSubMesh->m_uiVAO);
	//FMLOG("SubMesh VAO ID: %d", l_pSubMesh->m_uiVAO);

    // Generate and upload index buffer
    glGenBuffers(1, &l_pSubMesh->m_uiEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, l_pSubMesh->m_uiEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, l_pSubMesh->m_IndexBuffer.size() * sizeof(GLuint), l_pSubMesh->m_IndexBuffer.data(), GL_STATIC_DRAW);
    FORCE_CHECK_GL_ERROR("EBO setup");

    std::map<std::string, int> l_AttributeMap = {
        {"POSITION", FVF_POS},
        {"NORMAL", FVF_NORMAL},
        {"COLOR_0", FVF_DIFFUSE},
        {"TANGENT", FVF_TANGENT},
        {"BITANGENT", FVF_BINORMAL},
        {"WEIGHTS_0", FVF_SKINNING_WEIGHT},
        {"JOINTS_0", FVF_SKINNING_BONE_INDEX},
        {"TEXCOORD_0", FVF_TEX0},
        {"TEXCOORD_1", FVF_TEX1}
    };
    for (const auto& attribute : primitive.attributes)
    {
        auto it = l_AttributeMap.find(attribute.first);
        if (it != l_AttributeMap.end())
        {
            int l_iFVFIndex = it->second;
            const tinygltf::Accessor& accessor = e_Model.accessors[attribute.second];
            const tinygltf::BufferView& bufferView = e_Model.bufferViews[accessor.bufferView];
            const tinygltf::Buffer& buffer = e_Model.buffers[bufferView.buffer];

            size_t byteOffset = accessor.byteOffset + bufferView.byteOffset;
            const unsigned char* dataPtr = buffer.data.data() + byteOffset;

            l_pSubMesh->m_i64FVFFlag |= 1LL << l_iFVFIndex;
            size_t dataSize = accessor.count * accessor.ByteStride(bufferView);

            glGenBuffers(1, &l_pSubMesh->m_iVBOArray[l_iFVFIndex]);
            glBindBuffer(GL_ARRAY_BUFFER, l_pSubMesh->m_iVBOArray[l_iFVFIndex]);
            glBufferData(GL_ARRAY_BUFFER, dataSize, dataPtr, GL_STATIC_DRAW);

            auto    l_ByteStride =  accessor.ByteStride(bufferView);
            int     l_iStride = accessor.type;
            int     l_iGLDataType = accessor.componentType;

            if (accessor.componentType != GL_FLOAT)
            {
                //GL_FLOAT
                //GL_UNSIGNED_BYTE 5121
                //GL_UNSIGNED_SHORT,5123
                //GL_UNSIGNED_INT,5125
                //GL_FLOAT,5126
                glVertexAttribIPointer(l_iFVFIndex, l_iStride, l_iGLDataType, l_ByteStride, nullptr);
            }
            else
            {
                //(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
                glVertexAttribPointer(l_iFVFIndex, l_iStride, l_iGLDataType, GL_FALSE, l_ByteStride, nullptr);
            }
            glEnableVertexAttribArray(l_iFVFIndex);
            //FMLOG("SubMesh attribute name:%s VBO ID: %d,FVF Index: %d,Stride:%d  DataSize: %d Type:%d", attribute.first.c_str(), l_pSubMesh->m_iVBOArray[l_iFVFIndex], l_iFVFIndex, l_iStride, dataSize, accessor.componentType);
            if (attribute.first == "POSITION")
            {
                l_pSubMesh->m_i64VertexCount = accessor.count;
                l_pSubMesh->m_vMinBounds = Vector3((float)accessor.minValues[0], (float)accessor.minValues[1], (float)accessor.minValues[2]);
                l_pSubMesh->m_vMaxBounds = Vector3((float)accessor.maxValues[0], (float)accessor.maxValues[1], (float)accessor.maxValues[2]);
            }
#ifdef DEBUG
            if (attribute.first == "JOINTS_0")
            {
				//std::set<unsigned short> l_tt2;
    //            std::vector<unsigned short>l_TestVector;
    //            int l_iCount = dataSize / sizeof(unsigned short);
    //            unsigned short* l_pJoints = (unsigned short*)dataPtr;
    //            for (int i = 0; i < l_iCount; ++i)
    //            {
    //                l_TestVector.push_back(*l_pJoints);
    //                l_tt2.insert(*l_pJoints);
    //                ++l_pJoints;
    //            }
    //            int a = 0;
            }
#endif
        }
    }
    GenerateNormalAttribute(e_Model, primitive, l_pSubMesh.get());

    m_SubMeshesVector.push_back(l_pSubMesh);
    logFVFFlags();
    // Iterate through each subMesh to update the bounds
    for (std::shared_ptr<sSubMesh> l_pSubMesh : m_SubMeshesVector)
    {
        m_vMinBounds.x = min(m_vMinBounds.x, l_pSubMesh->m_vMinBounds.x);
        m_vMinBounds.y = min(m_vMinBounds.y, l_pSubMesh->m_vMinBounds.y);
        m_vMinBounds.z = min(m_vMinBounds.z, l_pSubMesh->m_vMinBounds.z);

        m_vMaxBounds.x = max(m_vMaxBounds.x, l_pSubMesh->m_vMaxBounds.x);
        m_vMaxBounds.y = max(m_vMaxBounds.y, l_pSubMesh->m_vMaxBounds.y);
        m_vMaxBounds.z = max(m_vMaxBounds.z, l_pSubMesh->m_vMaxBounds.z);
    }
    LoadMorphingAttributes(l_pSubMesh.get(), e_Model, primitive, e_bCalculateBinormal);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void cMesh::LoadMorphingAttributes(sSubMesh* e_pSubMesh,const tinygltf::Model& model, const tinygltf::Primitive& primitive, bool e_bCalculateBiNormal)
{
    if (!e_pSubMesh || primitive.targets.empty())
    {
        return;
    }

    if (e_pSubMesh->m_spFVFAndVertexDataMorphTargetMap)
    {
        FMLOG("morpthing data already parsed!?");
        return;
    }
    shared_ptr<sMorphTargetVector>    l_spMorphTargetVector;
#ifdef DEBUG
    e_pSubMesh->m_spFVFAndVertexDataMorphTargetMap = std::make_shared<sMorphTargetVector>();
    l_spMorphTargetVector = e_pSubMesh->m_spFVFAndVertexDataMorphTargetMap;
#endif
	auto l_MaxMorphingTarget = FVF_MORPHING_TARGET_POS4 - FVF_MORPHING_TARGET_POS1 + 1;
    auto l_uiNumTarget = primitive.targets.size()> l_MaxMorphingTarget? l_MaxMorphingTarget: primitive.targets.size();
    for (size_t i = 0; i < l_uiNumTarget; ++i)
    {
        const auto& target = primitive.targets[i];
        std::map<int, float*>l_FVFAndVertexData;
        // Load POSITION deltas
        if (target.find("POSITION") != target.end())
        {
            ++e_pSubMesh->m_iNumMorphTarget;
            int l_iFVF = (int)(FVF_MORPHING_TARGET_POS1 + i);
            const tinygltf::Accessor& accessor = model.accessors[target.at("POSITION")];
            const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
            const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
            if (accessor.componentType == GL_FLOAT)
            {
                size_t l_uiPosDataSize = accessor.count * accessor.ByteStride(bufferView);
                const float* data = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
                l_FVFAndVertexData = { {l_iFVF, (float*)data} };
                e_pSubMesh->m_i64FVFFlag |= 1LL << (l_iFVF);
                // Upload generated normals to GPU
                glGenBuffers(1, &e_pSubMesh->m_iVBOArray[l_iFVF]);
                glBindBuffer(GL_ARRAY_BUFFER, e_pSubMesh->m_iVBOArray[l_iFVF]);
                glBufferData(GL_ARRAY_BUFFER, l_uiPosDataSize, data, GL_STATIC_DRAW);
                glVertexAttribPointer(l_iFVF, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), nullptr);
                glEnableVertexAttribArray(l_iFVF);
            }
        }
#ifdef DEBUG
        // Load NORMAL deltas
        if (target.find("NORMAL") != target.end())
        {
            const tinygltf::Accessor& accessor = model.accessors[target.at("NORMAL")];
            const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
            const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

            const float* data = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
        }

        // Load TANGENT deltas (Optional)
        if (target.find("TANGENT") != target.end())
        {
            const tinygltf::Accessor& accessor = model.accessors[target.at("TANGENT")];
            const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
            const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
            const float* data = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
        }
        if (l_spMorphTargetVector)
        {
            l_spMorphTargetVector->push_back(l_FVFAndVertexData);
        }
#endif
    }
}

//void cMesh::SetMorphingWeights(const std::vector<double>& e_Weights)
//{
//	m_MorphWeights = e_Weights;
//}


// --- cMeshInstance Implementation ---

cMeshInstance::cMeshInstance()
{
}
cMeshInstance::cMeshInstance(cMeshInstance* e_pMeshInstance)
{
    m_InstanceTransformVector = e_pMeshInstance->m_InstanceTransformVector;
}
cMeshInstance::~cMeshInstance()
{
    if (m_InstanceVBO) glDeleteBuffers(1, &m_InstanceVBO);
}

void cMeshInstance::SetInstanceTransforms(const std::vector<cMatrix44>& transforms)
{
    m_InstanceTransformVector = transforms;
    m_BufferDirty = true;
}

void cMeshInstance::Clear()
{
    m_InstanceTransformVector.clear();
    m_BufferDirty = true;
}

void cMeshInstance::InitBuffer(const std::vector<std::shared_ptr<cMesh::sSubMesh>>& subMeshes)
{
    if (m_InstanceVBO == 0)
    {
        glGenBuffers(1, &m_InstanceVBO);
    }

    for (auto& subMesh : subMeshes)
    {
        glBindVertexArray(subMesh->m_uiVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
        glBufferData(GL_ARRAY_BUFFER, m_InstanceTransformVector.size() * sizeof(cMatrix44),m_InstanceTransformVector.data(), GL_DYNAMIC_DRAW);
        // mat4 occupies 4 attribute locations (e.g., 13, 14, 15, 16)
        GLuint baseLocation = FVF_MORPHING_TARGET_POS1;
        for (GLuint i = 0; i < 4; ++i)
        {
            glEnableVertexAttribArray(baseLocation + i);
            glVertexAttribPointer(
                baseLocation + i,         // attribute location
                4, GL_FLOAT, GL_FALSE,
                sizeof(cMatrix44),
                (void*)(sizeof(float) * i * 4)
            );
            glVertexAttribDivisor(baseLocation + i, 1);
        }

        glBindVertexArray(0);
    }
    //for (auto& subMesh : subMeshes)
    //{
    //    glBindVertexArray(subMesh->m_uiVAO);
    //    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
    //    //glBufferData(GL_ARRAY_BUFFER, m_InstanceTransformVector.size() * sizeof(cMatrix44),
    //    //             m_InstanceTransformVector.data(), GL_DYNAMIC_DRAW);
    //    //GLuint attribLocation = FVF_INSTANCING;
    //    GLuint attribLocation = FVF_MORPHING_TARGET_POS1; // Use locations FVF_INSTANCING_FLAG +1 +2 +3
    //    glEnableVertexAttribArray(attribLocation);
    //    glVertexAttribPointer(attribLocation, 16, GL_FLOAT, GL_FALSE, sizeof(cMatrix44), nullptr);
    //    glVertexAttribDivisor(attribLocation, 1);



    //    //glGenBuffers(1, &e_pSubMesh->m_iVBOArray[l_iFVFIndex]);
    //    //glBindBuffer(GL_ARRAY_BUFFER, e_pSubMesh->m_iVBOArray[l_iFVFIndex]);
    //    //glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vector3), normals.data(), GL_STATIC_DRAW);

    //    //glEnableVertexAttribArray(l_iFVFIndex);
    //    //glVertexAttribPointer(l_iFVFIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), nullptr);
    //    glBindVertexArray(0);
    //}
}

void cMeshInstance::UpdateBuffer()
{
    if (m_InstanceVBO == 0)
    {
        return;
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
    //this is slow chatgpt says
    //glMapBufferRange is typically 2× to 5× faster than glBufferSubData for large or frequent updates — especially
    if (0)
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_InstanceTransformVector.size() * sizeof(cMatrix44), m_InstanceTransformVector.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    else
    {
        // Get size in bytes of all transforms
        GLsizeiptr size = m_InstanceTransformVector.size() * sizeof(cMatrix44);

        // Map the buffer range for writing
        void* ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, size,
                                     GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

        if (ptr)
        {
            // Copy your instance transform data into the mapped buffer
            memcpy(ptr, m_InstanceTransformVector.data(), size);

            // Unmap when done
            glUnmapBuffer(GL_ARRAY_BUFFER);
        }
        else
        {
            // Mapping failed
            //std::cerr << "Error: glMapBufferRange failed!" << std::endl;
            int a = 0;
        }
    }
    m_BufferDirty = false;
}