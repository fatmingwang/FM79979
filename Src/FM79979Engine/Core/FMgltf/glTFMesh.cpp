#include <vector>
#include <string>
#include <map>
#include "tiny_gltf.h"
#include <iostream>
#include "glTFMesh.h"

#include <algorithm>
#include <cstring>

cMesh::cMesh()
{
}

cMesh::~cMesh()
{
    for (auto& subMesh : subMeshes)
    {
        glDeleteBuffers(1, &subMesh.vbo);
        glDeleteBuffers(1, &subMesh.ebo);
        glDeleteVertexArrays(1, &subMesh.vao);
    }
}

void cMesh::InitBuffer()
{
    for (auto& subMesh : subMeshes)
    {
        glGenVertexArrays(1, &subMesh.vao);
        glGenBuffers(1, &subMesh.vbo);
        glGenBuffers(1, &subMesh.ebo);

        glBindVertexArray(subMesh.vao);

        // Upload vertex data
        glBindBuffer(GL_ARRAY_BUFFER, subMesh.vbo);
        glBufferData(GL_ARRAY_BUFFER, subMesh.vertexBuffer.size() * sizeof(float), subMesh.vertexBuffer.data(), GL_STATIC_DRAW);

        // Upload index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMesh.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, subMesh.indexBuffer.size() * sizeof(uint32_t), subMesh.indexBuffer.data(), GL_STATIC_DRAW);

        // Define vertex attribute pointers dynamically based on FVF flags
        auto l_StrideWithSize = subMesh.m_uiVertexStride * sizeof(float);
        size_t offset = 0;
        for (int i = 0; i < TOTAL_FVF; ++i)
        {
            if (subMesh.fvfFlags & (1 << i))
            {
                glEnableVertexAttribArray(i);
                myVertexAttribPointer(i, g_iFVF_DataStride[i], g_iFVF_DataType[i], GL_FALSE, (GLsizei)l_StrideWithSize, (void*)offset);
                offset += g_iFVF_DataSize[i];
            }
        }
    }
    glBindVertexArray(0);

    // Iterate through each subMesh to update the bounds
    for (const auto& subMesh : subMeshes)
    {
        m_vMinBounds.x = min(m_vMinBounds.x, subMesh.m_vMinBounds.x);
        m_vMinBounds.y = min(m_vMinBounds.y, subMesh.m_vMinBounds.y);
        m_vMinBounds.z = min(m_vMinBounds.z, subMesh.m_vMinBounds.z);

        m_vMaxBounds.x = max(m_vMaxBounds.x, subMesh.m_vMaxBounds.x);
        m_vMaxBounds.y = max(m_vMaxBounds.y, subMesh.m_vMaxBounds.y);
        m_vMaxBounds.z = max(m_vMaxBounds.z, subMesh.m_vMaxBounds.z);
    }
}


void EnableVertexAttributes(unsigned int fvfFlags)
{
    for (int i = 0; i < TOTAL_FVF; ++i)
    {
        if (fvfFlags & (1 << i))
        {
            glEnableVertexAttribArray(i);
        }
    }
}


void cMesh::SubMesh::GetProperCameraPosition(cMatrix44& e_CameraMatrix)
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


void cMesh::Draw()
{
    static float angle = 0.0f;
    static float lightAngle = 0.0f;
    static float l_fCameraZPosition = -6;
    lightAngle += 0.01f;
    angle += 0.01f;
    cBaseShader* l_pShader = GetCurrentShader();
    if (l_pShader)
    {
        l_pShader->Unuse();
    }
    UseShaderProgram(L"qoo79979");
    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    auto l_vPos = this->GetLocalPosition();
    for (auto& subMesh : subMeshes)
    {
        // Use the shader program specific to this sub-mesh
        glUseProgram(subMesh.shaderProgram);

        // Set model, view, projection matrices
        GLuint modelLoc = glGetUniformLocation(subMesh.shaderProgram, "inMat4Model");
        GLuint viewLoc = glGetUniformLocation(subMesh.shaderProgram, "inMat4View");
        GLuint projLoc = glGetUniformLocation(subMesh.shaderProgram, "inMat4Projection");

        cMatrix44 modelMatrix = cMatrix44::TranslationMatrix(l_vPos);
        cMatrix44 viewMatrix = cMatrix44::LookAtMatrix(Vector3(0, -0, l_fCameraZPosition), Vector3(0, 0, 0), Vector3(0, 1, 0));
        subMesh.GetProperCameraPosition(viewMatrix);

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
        GLuint lightColorLoc = glGetUniformLocation(subMesh.shaderProgram, "inVec3LightColor");
        GLuint lightPosLoc = glGetUniformLocation(subMesh.shaderProgram, "inVec3LightPosition");
        GLuint viewPosLoc = glGetUniformLocation(subMesh.shaderProgram, "inVec3ViewPosition");

        Vector3 lightColor(1.0f, 1.0f, 1.0f);
        Vector3 lightPos(100.0f * cos(lightAngle), 0.0f, 100.0f * sin(lightAngle));
        Vector3 viewPos(0.0f, 0.0f, 30.0f);

        glUniform3fv(lightColorLoc, 1, lightColor);
        glUniform3fv(lightPosLoc, 1, lightPos);
        glUniform3fv(viewPosLoc, 1, viewPos);

        //lightAngle += (0.001f/ subMeshes.size());
        //angle += (0.001f / subMeshes.size());
        // Bind textures
        for (size_t i = 0; i < m_uiTextureIDVector.size(); ++i)
        {
            glActiveTexture(GL_TEXTURE0 + (GLenum)i);
            glBindTexture(GL_TEXTURE_2D, m_uiTextureIDVector[i]);
        }
        GLuint texture1Loc = glGetUniformLocation(subMesh.shaderProgram, "texture1");
        glUniform1i(texture1Loc, 0);

        // Bind normal map texture if available
        if (!m_uiNormalTextureIDVector.empty())
        {
            glActiveTexture(GL_TEXTURE0 + (GLenum)m_uiTextureIDVector.size());
            glBindTexture(GL_TEXTURE_2D, m_uiNormalTextureIDVector[0]);
            GLuint normalMapLoc = glGetUniformLocation(subMesh.shaderProgram, "normalMap");
            glUniform1i(normalMapLoc, (GLint)m_uiTextureIDVector.size());
        }

        // Bind the vertex array and draw the sub-mesh
        glBindVertexArray(subMesh.vao);
        EnableVertexAttributes(subMesh.fvfFlags);
        MY_GLDRAW_ELEMENTS(GL_TRIANGLES, (GLsizei)subMesh.indexBuffer.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    glDisable(GL_CULL_FACE);
    glUseProgram(0);
}

void cMesh::LoadAttributes(const tinygltf::Model& model, const tinygltf::Primitive& primitive, bool calculateBinormal)
{
    SubMesh subMesh;
    subMesh.fvfFlags = 0;

    const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
    const tinygltf::BufferView& bufferView = model.bufferViews[indexAccessor.bufferView];
    const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

    const unsigned char* dataPtr = buffer.data.data() + bufferView.byteOffset + indexAccessor.byteOffset;

    if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
    {
        const uint16_t* shortIndices = reinterpret_cast<const uint16_t*>(dataPtr);
        subMesh.indexBuffer.resize(indexAccessor.count);
        std::transform(shortIndices, shortIndices + indexAccessor.count, subMesh.indexBuffer.begin(),
                       [](uint16_t val)
                       {
                           return static_cast<uint32_t>(val);
                       });
    }
    else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
    {
        subMesh.indexBuffer.resize(indexAccessor.count);
        memcpy(subMesh.indexBuffer.data(), dataPtr, indexAccessor.count * sizeof(uint32_t));
    }
    else
    {
        std::cerr << "Unsupported index component type: " << indexAccessor.componentType << std::endl;
        return;
    }

    bool hasPosition = primitive.attributes.find("POSITION") != primitive.attributes.end();
    bool hasNormal = primitive.attributes.find("NORMAL") != primitive.attributes.end();
    bool hasTexCoord = primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end();
    bool hasTangent = primitive.attributes.find("TANGENT") != primitive.attributes.end();
    bool hasColor = primitive.attributes.find("COLOR_0") != primitive.attributes.end();
    bool hasNormalMap = primitive.attributes.find("NORMAL_MAP") != primitive.attributes.end();
    bool hasBinormal = calculateBinormal && hasPosition && hasNormal && hasTexCoord;

    size_t vertexCount = 0;
    if (hasPosition)
    {
        const tinygltf::Accessor& positionAccessor = model.accessors[primitive.attributes.at("POSITION")];
        vertexCount = positionAccessor.count;
    }
    subMesh.m_uiVertexStride = 3 + (hasNormal ? 3 : 0) + (hasTexCoord ? 2 : 0) + (hasTangent ? 3 : 0) + (hasColor ? 4 : 0) + (hasBinormal ? 3 : 0);
    subMesh.vertexBuffer.resize(vertexCount * subMesh.m_uiVertexStride);

    auto LoadAttribute = [&](const std::string& name) -> const float*
        {
            auto it = primitive.attributes.find(name);
            if (it == primitive.attributes.end())
            {
                FMLOG("%s can't find data", name.c_str());
                return nullptr;
            }
            const tinygltf::Accessor& accessor = model.accessors[it->second];
            const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
            const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
            size_t byteOffset = accessor.byteOffset + bufferView.byteOffset;
            size_t count = accessor.count;
            size_t stride = accessor.ByteStride(bufferView);
            const unsigned char* dataPtr = buffer.data.data() + byteOffset;
            if (name == "POSITION")
            {
                subMesh.m_vMinBounds = Vector3((float)accessor.minValues[0], (float)accessor.minValues[1], (float)accessor.minValues[2]);
                subMesh.m_vMaxBounds = Vector3((float)accessor.maxValues[0], (float)accessor.maxValues[1], (float)accessor.maxValues[2]);
            }
#ifdef DEBUG
            accessor.maxValues;
            accessor.minValues;
            if (accessor.componentType != 5126)
            {
                //5126 GL_FLOAT(No conversion needed)
                //5123 GL_UNSIGNED_SHORT(Divide by 65535)
                //5121 GL_UNSIGNED_BYTE(Divide by 255)
                int a = 0;
            }
#endif
            return reinterpret_cast<const float*>(dataPtr);
        };

    const float* positionData = LoadAttribute("POSITION");
    const float* normalData = LoadAttribute("NORMAL");
    const float* texCoordData = LoadAttribute("TEXCOORD_0");
    const float* tangentData = LoadAttribute("TANGENT");
    const float* colorData = LoadAttribute("COLOR_0");

    for (size_t i = 0; i < vertexCount; ++i)
    {
        size_t offset = 0;
        int l_iArrtibuteIndex = 0;
        auto l_iCurrentVertexIndex = i * subMesh.m_uiVertexStride;
        if (positionData)
        {
            l_iArrtibuteIndex = FVF_POS;
            Vector3 vertex(positionData[i * 3], positionData[i * 3 + 1], positionData[i * 3 + 2]);
            memcpy(&subMesh.vertexBuffer[l_iCurrentVertexIndex + offset], positionData + i * 3, g_iFVF_DataSize[l_iArrtibuteIndex]);
            offset += g_iFVF_DataStride[l_iArrtibuteIndex];
        }
        if (normalData)
        {
            l_iArrtibuteIndex = FVF_NORMAL;
            memcpy(&subMesh.vertexBuffer[l_iCurrentVertexIndex + offset], normalData + i * 3, g_iFVF_DataSize[l_iArrtibuteIndex]);
            offset += g_iFVF_DataStride[l_iArrtibuteIndex];
        }
        if (colorData)
        {
            l_iArrtibuteIndex = FVF_DIFFUSE;
            memcpy(&subMesh.vertexBuffer[l_iCurrentVertexIndex + offset], colorData + i * 4, g_iFVF_DataSize[l_iArrtibuteIndex]);
            offset += g_iFVF_DataStride[l_iArrtibuteIndex];
        }
        if (tangentData)
        {
            l_iArrtibuteIndex = FVF_TANGENT;
            memcpy(&subMesh.vertexBuffer[l_iCurrentVertexIndex + offset], tangentData + i * 3, g_iFVF_DataSize[l_iArrtibuteIndex]);
            offset += g_iFVF_DataStride[l_iArrtibuteIndex];
        }
        if (hasBinormal)
        {
            l_iArrtibuteIndex = FVF_TANGENT;
            offset += g_iFVF_DataStride[l_iArrtibuteIndex];
        }
        if (texCoordData)
        {
            l_iArrtibuteIndex = FVF_TEX0;
            const float* l_fpUVData = texCoordData + i * 2;
            Vector2 l_vUV(l_fpUVData[0], l_fpUVData[1]);
            memcpy(&subMesh.vertexBuffer[l_iCurrentVertexIndex + offset], &l_vUV, g_iFVF_DataSize[l_iArrtibuteIndex]);
            offset += g_iFVF_DataStride[l_iArrtibuteIndex];
        }
    }

    if (hasPosition) subMesh.fvfFlags |= FVF_POS_FLAG;
    if (hasNormal) subMesh.fvfFlags |= FVF_NORMAL_FLAG;
    if (hasTexCoord) subMesh.fvfFlags |= FVF_TEX0_FLAG;
    if (hasTangent) subMesh.fvfFlags |= FVF_TANGENT_FLAG;
    if (hasColor) subMesh.fvfFlags |= FVF_DIFFUSE_FLAG;
    if (hasBinormal) subMesh.fvfFlags |= FVF_BITAGENT_FLAG;
    if (hasNormalMap) subMesh.fvfFlags |= FVF_NORMAL_MAP_TEXTURE_FLAG;

    subMeshes.push_back(subMesh);
    logFVFFlags();
}

void cMesh::LoadTextures(const tinygltf::Model& model, const tinygltf::Material& material)
{
    // Load base color texture
    if (material.pbrMetallicRoughness.baseColorTexture.index >= 0)
    {
        const tinygltf::Texture& texture = model.textures[material.pbrMetallicRoughness.baseColorTexture.index];
        const tinygltf::Image& image = model.images[texture.source];

        GLuint textureID = 0;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        if (image.component == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.image.data());
        }
        else if (image.component == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.image.data());
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        m_uiTextureIDVector.push_back(textureID);
    }

    // Load normal map texture (if available)
    if (material.normalTexture.index >= 0)
    {
        const tinygltf::Texture& texture = model.textures[material.normalTexture.index];
        const tinygltf::Image& image = model.images[texture.source];

        GLuint textureID = 0;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        if (image.component == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.image.data());
        }
        else if (image.component == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.image.data());
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        m_uiNormalTextureIDVector.push_back(textureID);
    }

    // Load occlusion map texture (if available)
    if (material.occlusionTexture.index >= 0)
    {
        const tinygltf::Texture& texture = model.textures[material.occlusionTexture.index];
        const tinygltf::Image& image = model.images[texture.source];

        GLuint textureID = 0;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        if (image.component == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.image.data());
        }
        else if (image.component == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.image.data());
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        m_uiOocclusionTextureIDVector.push_back(textureID);
    }
}

void cMesh::logFVFFlags()
{
    std::cout << "FVF Flags: " << std::endl;
    for (const auto& subMesh : subMeshes)
    {
        if (subMesh.fvfFlags & FVF_POS_FLAG) std::cout << "Position is present." << std::endl;
        if (subMesh.fvfFlags & FVF_NORMAL_FLAG) std::cout << "Normal is present." << std::endl;
        if (subMesh.fvfFlags & FVF_DIFFUSE_FLAG) std::cout << "Color is present." << std::endl;
        if (subMesh.fvfFlags & FVF_TEX0_FLAG) std::cout << "Texture coordinates are present." << std::endl;
        if (subMesh.fvfFlags & FVF_TANGENT_FLAG) std::cout << "Tangent is present." << std::endl;
        if (subMesh.fvfFlags & FVF_BITAGENT_FLAG) std::cout << "Binormal is present." << std::endl;
        if (subMesh.fvfFlags & FVF_NORMAL_MAP_TEXTURE_FLAG) std::cout << "Normal map is present." << std::endl;
    }
}