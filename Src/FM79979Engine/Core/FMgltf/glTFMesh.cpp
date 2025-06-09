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

TYPDE_DEFINE_MARCO(cMesh);

cMesh::cMesh()
{
}

cMesh::cMesh(cMesh* e_pMesh)
{
    m_vMinBounds = e_pMesh->m_vMinBounds;
    m_vMaxBounds = e_pMesh->m_vMaxBounds;
    m_Material = e_pMesh->m_Material;
    m_SubMeshesVector = e_pMesh->m_SubMeshesVector;
}

cMesh::~cMesh()
{
    m_Material = nullptr;
}

void cMesh::ApplyMorphUniformData(sSubMesh* e_pSubMesh)
{
    if (m_CurrentAnimationMorphPrimitiveWeightsVector.size() && e_pSubMesh->m_spFVFAndVertexDataMorphTargetMap)
    {//setup how many primitive and weights data
        auto l_uiUniform = glGetUniformLocation(e_pSubMesh->m_iShaderProgram,"uMorphWeights");
        auto l_uiSize = e_pSubMesh->m_spFVFAndVertexDataMorphTargetMap->size();
        LAZY_DO_GL_COMMAND_AND_GET_ERROR(glUniform1fv((GLsizei)l_uiUniform, (GLsizei)l_uiSize, m_CurrentAnimationMorphPrimitiveWeightsVector.data()));
    }
}

void cMesh::ApplyMaterial()
{
    if (this->m_Material)
    {
        this->m_Material->Apply();
    }
}

void cMesh::GenerateNormalAttribute(const tinygltf::Model& e_Model,const tinygltf::Primitive& primitive, sSubMesh* e_pSubMesh)
{
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
            uint32_t index0 = e_pSubMesh->m_IndexBuffer[i];
            uint32_t index1 = e_pSubMesh->m_IndexBuffer[i + 1];
            uint32_t index2 = e_pSubMesh->m_IndexBuffer[i + 2];

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
    // Bind the vertex array and draw the sub-mesh
    glBindVertexArray(e_pSubMesh->m_uiVAO);
    EnableVertexAttributes(e_pSubMesh->m_i64FVFFlag);
    MY_GLDRAW_ELEMENTS(GL_TRIANGLES, (GLsizei)e_pSubMesh->m_IndexBuffer.size(), GL_UNSIGNED_INT, 0);
}

void cMesh::SetSubMeshCommonUniformData(sSubMesh* e_pSubMesh, cMatrix44& e_mat)
{
    // Use the shader program specific to this sub-mesh
    glUseProgram(e_pSubMesh->m_iShaderProgram);
    ApplyMorphUniformData(e_pSubMesh);
    g_fSetLightUniform(e_pSubMesh->m_iShaderProgram);
    GLuint modelLoc = glGetUniformLocation(e_pSubMesh->m_iShaderProgram, "inMat4Model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, e_mat);
    g_fSetCameraUniform(e_pSubMesh->m_iShaderProgram);
    ApplyMaterial();
}

void cMesh::Render()
{
    auto l_matTransform1 = this->GetParent()->GetWorldTransform();
    auto l_matTransform = this->GetWorldTransform();
    for (auto l_pSubMesh : m_SubMeshesVector)
    {
        SetSubMeshCommonUniformData(l_pSubMesh.get(), l_matTransform);
        CallOpenGLDraw(l_pSubMesh.get());
    }
}

void cMesh::LoadMaterial(const tinygltf::Model& e_Model, const tinygltf::Material& e_Material, unsigned int e_uiShaderProgram)
{
    m_Material = std::make_shared<cMaterial>(e_uiShaderProgram);
    m_Material->LoadMaterials(e_Model,e_Material);
}

void cMesh::logFVFFlags()
{
    FMLOG("FVF Flags")
    for (const auto l_pSubMesh : m_SubMeshesVector)
    {
        if (l_pSubMesh->m_i64FVFFlag & FVF_POS_FLAG) FMLOG("Position is present.");
        if (l_pSubMesh->m_i64FVFFlag & FVF_NORMAL_FLAG) FMLOG("Normal is present.");
        if (l_pSubMesh->m_i64FVFFlag & FVF_DIFFUSE_FLAG) FMLOG("Color is present.");
        if (l_pSubMesh->m_i64FVFFlag & FVF_SKINNING_WEIGHT_FLAG) FMLOG("weight is present.");
        if (l_pSubMesh->m_i64FVFFlag & FVF_SKINNING_BONE_INDEX_FLAG) FMLOG("joint is present.");
        if (l_pSubMesh->m_i64FVFFlag & FVF_TEX0_FLAG) FMLOG("Texture coordinates are present.");
        if (l_pSubMesh->m_i64FVFFlag & FVF_TANGENT_FLAG) FMLOG("Tangent is present.");
        if (l_pSubMesh->m_i64FVFFlag & FVF_BITAGENT_FLAG) FMLOG("Binormal is present.");
        if (l_pSubMesh->m_i64FVFFlag & FVF_NORMAL_MAP_TEXTURE_FLAG) FMLOG("Normal map is present.");
    }
}


void cMesh::LoadAttributesAndInitBuffer(const tinygltf::Model& model, const tinygltf::Primitive& primitive, bool calculateBinormal)
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
        const tinygltf::Accessor& positionAccessor = model.accessors[primitive.attributes.at("POSITION")];
        size_t vertexCount = positionAccessor.count;
        l_pSubMesh->m_IndexBuffer.resize(vertexCount);
        std::iota(l_pSubMesh->m_IndexBuffer.begin(), l_pSubMesh->m_IndexBuffer.end(), 0);
    }
    else
    {
        const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
        const tinygltf::BufferView& bufferView = model.bufferViews[indexAccessor.bufferView];
        const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

        const unsigned char* dataPtr = buffer.data.data() + bufferView.byteOffset + indexAccessor.byteOffset;

        if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
        {
            const uint16_t* shortIndices = reinterpret_cast<const uint16_t*>(dataPtr);
            l_pSubMesh->m_IndexBuffer.assign(shortIndices, shortIndices + indexAccessor.count);
        }
        else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
        {
            const uint32_t* intIndices = reinterpret_cast<const uint32_t*>(dataPtr);
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

    // Generate and upload index buffer
    glGenBuffers(1, &l_pSubMesh->m_uiEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, l_pSubMesh->m_uiEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, l_pSubMesh->m_IndexBuffer.size() * sizeof(uint32_t), l_pSubMesh->m_IndexBuffer.data(), GL_STATIC_DRAW);

    std::map<std::string, int> l_AttributeMap = {
        {"POSITION", FVF_POS},
        {"NORMAL", FVF_NORMAL},
        {"COLOR_0", FVF_DIFFUSE},
        {"TANGENT", FVF_TANGENT},
        {"BITANGENT", FVF_BITAGENT},
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
            const tinygltf::Accessor& accessor = model.accessors[attribute.second];
            const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
            const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

            size_t byteOffset = accessor.byteOffset + bufferView.byteOffset;
            const unsigned char* dataPtr = buffer.data.data() + byteOffset;

            l_pSubMesh->m_i64FVFFlag |= 1LL << l_iFVFIndex;
            size_t dataSize = accessor.count * accessor.ByteStride(bufferView);

            glGenBuffers(1, &l_pSubMesh->m_iVBOArray[l_iFVFIndex]);
            glBindBuffer(GL_ARRAY_BUFFER, l_pSubMesh->m_iVBOArray[l_iFVFIndex]);
            glBufferData(GL_ARRAY_BUFFER, dataSize, dataPtr, GL_STATIC_DRAW);

            glEnableVertexAttribArray(l_iFVFIndex);
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
                glVertexAttribPointer(l_iFVFIndex, l_iStride, l_iGLDataType, GL_FALSE, l_ByteStride, nullptr);
            }

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
    GenerateNormalAttribute(model, primitive, l_pSubMesh.get());

    m_SubMeshesVector.push_back(l_pSubMesh);
    logFVFFlags();
    // Iterate through each subMesh to update the bounds
    for (const auto l_pSubMesh : m_SubMeshesVector)
    {
        m_vMinBounds.x = min(m_vMinBounds.x, l_pSubMesh->m_vMinBounds.x);
        m_vMinBounds.y = min(m_vMinBounds.y, l_pSubMesh->m_vMinBounds.y);
        m_vMinBounds.z = min(m_vMinBounds.z, l_pSubMesh->m_vMinBounds.z);

        m_vMaxBounds.x = max(m_vMaxBounds.x, l_pSubMesh->m_vMaxBounds.x);
        m_vMaxBounds.y = max(m_vMaxBounds.y, l_pSubMesh->m_vMaxBounds.y);
        m_vMaxBounds.z = max(m_vMaxBounds.z, l_pSubMesh->m_vMaxBounds.z);
    }
    LoadMorphingAttributes(l_pSubMesh.get(), model, primitive, calculateBinormal);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void cMesh::LoadMorphingAttributes(sSubMesh* e_pSubMesh,const tinygltf::Model& model, const tinygltf::Primitive& primitive, bool calculateBinormal)
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
                glEnableVertexAttribArray(l_iFVF);
                glVertexAttribPointer(l_iFVF, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), nullptr);
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