#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "glTFMesh.h"

#include <algorithm>
#include <cstring>
#include <unordered_set>
#include <set>

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

        e_pSubMesh->m_iFVFFlag |= 1 << l_iFVFIndex;
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

cMesh::cMesh()
{
}

cMesh::~cMesh()
{
    m_Material = nullptr;
    for (auto l_pSubMesh : m_SubMeshesVector)
    {
        l_pSubMesh->ClearOpenGLData();
    }
    DELETE_VECTOR(m_SubMeshesVector);
}


void cMesh::sSubMesh::GetProperCameraPosition(cMatrix44& e_CameraMatrix)
{
    Vector3 center = (m_vMinBounds + m_vMaxBounds) * 0.5f;
    Vector3 size = m_vMaxBounds - m_vMinBounds;
    float radius = size.Length() * 0.5f;
    center.y *= -1;
    // Set the camera position to be a bit further away from the center of the mesh
    Vector3 cameraPosition = center + Vector3(0, 0, radius * 5.0f);

    // Assuming you have a function to set the view matrix in the projectionMatrix
    e_CameraMatrix = cMatrix44::LookAtMatrix(cameraPosition, center, Vector3(0, 1, 0));
}

void cMesh::sSubMesh::ClearOpenGLData()
{
    for (int i = 0; i < TOTAL_FVF; ++i)
    {
        if (i & this->m_iFVFFlag)
        {
            glDeleteBuffers(1,&m_iVBOArray[i]);
        }
        
    }
    glDeleteBuffers(1, &m_uiEBO);
    glDeleteVertexArrays(1, &m_uiVAO);
}

void cMesh::Render()
{
    static float angle = 0.0f;
    static float lightAngle = 0.0f;
    static float l_fCameraZPosition = -6;
    //lightAngle += 0.01f;
    //angle += 0.01f;

    auto l_matTransform = this->GetWorldTransform();
    for (auto l_pSubMesh : m_SubMeshesVector)
    {
        // Use the shader program specific to this sub-mesh
        glUseProgram(l_pSubMesh->shaderProgram);

        // Set model, view, projection matrices
        GLuint modelLoc = glGetUniformLocation(l_pSubMesh->shaderProgram, "inMat4Model");
        GLuint viewLoc = glGetUniformLocation(l_pSubMesh->shaderProgram, "inMat4View");
        GLuint projLoc = glGetUniformLocation(l_pSubMesh->shaderProgram, "inMat4Projection");

        cMatrix44 modelMatrix = l_matTransform;
        cMatrix44 viewMatrix = cMatrix44::LookAtMatrix(Vector3(0, -0, l_fCameraZPosition), Vector3(0, 0, 0), Vector3(0, 1, 0));
        l_pSubMesh->GetProperCameraPosition(viewMatrix);

        viewMatrix.GetTranslation().z *= -1;
        Projection projectionMatrix;
        projectionMatrix.SetFovYAspect(XM_PIDIV4, (float)1920 / (float)1080, 0.1f, 10000.0f);

        cMatrix44 conversionMatrix = cMatrix44::Identity;
        conversionMatrix.m[2][2] = -1.0f;

        modelMatrix = conversionMatrix * modelMatrix;
        cMatrix44 rotationMatrix = cMatrix44::YAxisRotationMatrix(angle);
        modelMatrix = rotationMatrix * modelMatrix;

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projectionMatrix.GetMatrix());

        // Set light and view position uniforms
        GLuint lightColorLoc = glGetUniformLocation(l_pSubMesh->shaderProgram, "inVec3LightColor");
        GLuint lightPosLoc = glGetUniformLocation(l_pSubMesh->shaderProgram, "inVec3LightPosition");
        GLuint viewPosLoc = glGetUniformLocation(l_pSubMesh->shaderProgram, "inVec3ViewPosition");

        Vector3 lightColor(1.0f, 1.0f, 1.0f);
        Vector3 lightPos(100.0f * cos(lightAngle), 0.0f, 100.0f * sin(lightAngle));
        Vector3 viewPos(0.0f, 0.0f, 30.0f);

        glUniform3fv(lightColorLoc, 1, lightColor);
        glUniform3fv(lightPosLoc, 1, lightPos);
        glUniform3fv(viewPosLoc, 1, viewPos);

        // Set directional light uniforms
        GLuint dirLightDirLoc = glGetUniformLocation(l_pSubMesh->shaderProgram, "dirLightDirection");
        GLuint dirLightColorLoc = glGetUniformLocation(l_pSubMesh->shaderProgram, "dirLightColor");

        Vector3 dirLightDirection(-0.f, -0.f, -1.f);
        Vector3 dirLightColor(1.f, 0.f, 0.f);

        glUniform3fv(dirLightDirLoc, 1, dirLightDirection);
        glUniform3fv(dirLightColorLoc, 1, dirLightColor);
        ApplyMaterial();
        // Bind the vertex array and draw the sub-mesh
        glBindVertexArray(l_pSubMesh->m_uiVAO);
        EnableVertexAttributes(l_pSubMesh->m_iFVFFlag);
        MY_GLDRAW_ELEMENTS(GL_TRIANGLES, (GLsizei)l_pSubMesh->m_IndexBuffer.size(), GL_UNSIGNED_INT, 0);
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
        if (l_pSubMesh->m_iFVFFlag & FVF_POS_FLAG) FMLOG("Position is present.");
        if (l_pSubMesh->m_iFVFFlag & FVF_NORMAL_FLAG) FMLOG("Normal is present.");
        if (l_pSubMesh->m_iFVFFlag & FVF_DIFFUSE_FLAG) FMLOG("Color is present.");
        if (l_pSubMesh->m_iFVFFlag & FVF_SKINNING_WEIGHT_FLAG) FMLOG("weight is present.");
        if (l_pSubMesh->m_iFVFFlag & FVF_SKINNING_BONE_INDEX_FLAG) FMLOG("joint is present.");
        if (l_pSubMesh->m_iFVFFlag & FVF_TEX0_FLAG) FMLOG("Texture coordinates are present.");
        if (l_pSubMesh->m_iFVFFlag & FVF_TANGENT_FLAG) FMLOG("Tangent is present.");
        if (l_pSubMesh->m_iFVFFlag & FVF_BITAGENT_FLAG) FMLOG("Binormal is present.");
        if (l_pSubMesh->m_iFVFFlag & FVF_NORMAL_MAP_TEXTURE_FLAG) FMLOG("Normal map is present.");
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
    sSubMesh* l_pSubMesh = new sSubMesh();

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

            l_pSubMesh->m_iFVFFlag |= 1 << l_iFVFIndex;
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
    GenerateNormalAttribute(model, primitive, l_pSubMesh);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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
}
