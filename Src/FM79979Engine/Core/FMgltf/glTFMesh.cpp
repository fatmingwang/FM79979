
#include "../AllCoreInclude.h"
#include <vector>
#include <string>
#include <map>
#include "tiny_gltf.h"
#include <iostream>
#include "glTFMesh.h"



// Constructor
cMesh::cMesh() : m_uiVAO(0), m_uiVBO(0), m_uiEBO(0), m_uiVertexStride(0), m_uiFVFFlags(0)
{
}

// Destructor
cMesh::~cMesh()
{
    glDeleteBuffers(1, &m_uiVBO);
    glDeleteBuffers(1, &m_uiEBO);
    glDeleteVertexArrays(1, &m_uiVAO);
}

void cMesh::InitBuffer()
{
    if (m_uiFVFFlags == 0)
    {
        return;
    }
    glGenVertexArrays(1, &m_uiVAO);
    glGenBuffers(1, &m_uiVBO);
    glGenBuffers(1, &m_uiEBO);

    glBindVertexArray(m_uiVAO);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);
    glBufferData(GL_ARRAY_BUFFER, m_VertexBufferVector.size() * sizeof(float), m_VertexBufferVector.data(), GL_STATIC_DRAW);

    // Upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IDrawndicesBufferVector.size() * sizeof(uint32_t), m_IDrawndicesBufferVector.data(), GL_STATIC_DRAW);

    // Define vertex attribute pointers dynamically based on FVF flags
    auto l_StrideWithSize = m_uiVertexStride * sizeof(float);
    size_t offset = 0;
	for (int i = 0; i < TOTAL_FVF;++i)
	{
		if (m_uiFVFFlags & (1 << i))
		{ 
            glEnableVertexAttribArray(i);
            myVertexAttribPointer(i, g_iFVF_DataStride[i], g_iFVF_DataType[i], GL_FALSE, (GLsizei)l_StrideWithSize, (void*)offset);
            offset += g_iFVF_DataSize[i];
		}
	}
    glBindVertexArray(0);
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

void cMesh::GetProperCameraPosition(cMatrix44& e_CameraMatrix)
{
    Vector3 center = (m_vMinBounds + m_vMaxBounds) * 0.5f;
    Vector3 size = m_vMaxBounds - m_vMinBounds;
    float radius = size.Length() * 0.5f;
    center.y *= -1;
    // Set the camera position to be a bit further away from the center of the mesh
    Vector3 cameraPosition = center + Vector3(0, 0, radius * 2.0f);

    // Assuming you have a function to set the view matrix in the projectionMatrix
    e_CameraMatrix = cMatrix44::LookAtMatrix(cameraPosition, center, Vector3(0, 1, 0));
}


void cMesh::Draw()
{
    if (m_uiFVFFlags == 0)
    {
        return;
    }
    static float angle = 0.0f;
    static float lightAngle = 0.0f;
    //static float l_fCameraZPosition = -200;
    static float l_fCameraZPosition = -6;
    //l_fCameraZPosition += 1;
	if (l_fCameraZPosition > 100)
	{
		//l_fCameraZPosition = -300;
	}

    cBaseShader* l_pShader = GetCurrentShader();
    if (l_pShader)
    {
        l_pShader->Unuse();
    }
    UseShaderProgram(L"qoo79979");
    LAZY_DO_GL_COMMAND_AND_GET_ERROR(glUseProgram(m_uiShaderProgram));
    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Set model, view, projection matrices
    GLuint modelLoc = glGetUniformLocation(m_uiShaderProgram, "inMat4Model");
    GLuint viewLoc = glGetUniformLocation(m_uiShaderProgram, "inMat4View");
    GLuint projLoc = glGetUniformLocation(m_uiShaderProgram, "inMat4Projection");
    // Assuming you have these matrices defined somewhere
    cMatrix44 modelMatrix = cMatrix44::Identity;
    cMatrix44 viewMatrix = cMatrix44::LookAtMatrix(Vector3(0, -0, l_fCameraZPosition), Vector3(0, 0, 0), Vector3(0, 1, 0));
    //cMatrix44 viewMatrix;
    GetProperCameraPosition(viewMatrix);

    viewMatrix.GetTranslation().z *= -1;
    Projection projectionMatrix;
    projectionMatrix.SetFovYAspect(XM_PIDIV4, (float)1920 / (float)1080, 0.1f, 10000.0f);

    // Define the conversion matrix to flip the Z-axis
    cMatrix44 conversionMatrix = cMatrix44::Identity;
    conversionMatrix.m[2][2] = -1.0f;

    // Apply the conversion matrix to the model and view matrices
    modelMatrix = conversionMatrix * modelMatrix;

    // Rotate the model dynamically
    cMatrix44 rotationMatrix = cMatrix44::YAxisRotationMatrix(angle);
    modelMatrix = rotationMatrix * modelMatrix;


    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projectionMatrix.GetMatrix());

    // Set light and view position uniforms
    GLuint lightColorLoc = glGetUniformLocation(m_uiShaderProgram, "inVec3LightColor");
    GLuint lightPosLoc = glGetUniformLocation(m_uiShaderProgram, "inVec3LightPosition");
    GLuint viewPosLoc = glGetUniformLocation(m_uiShaderProgram, "inVec3ViewPosition");

    Vector3 lightColor(1.0f, 1.0f, 1.0f); // White light
    Vector3 lightPos(100.0f * cos(lightAngle), 0.0f, 100.0f * sin(lightAngle));
    Vector3 viewPos(0.0f, 0.0f, 30.0f); // Camera position

    glUniform3fv(lightColorLoc, 1, lightColor);
    glUniform3fv(lightPosLoc, 1, lightPos);
    glUniform3fv(viewPosLoc, 1, viewPos);

    lightAngle += 0.01f;
    angle += 0.01f;

    // Bind textures
    for (size_t i = 0; i < m_uiTextureIDVector.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + (GLenum)i);
        glBindTexture(GL_TEXTURE_2D, m_uiTextureIDVector[i]);
    }
    GLuint texture1Loc = glGetUniformLocation(m_uiShaderProgram, "texture1");
    glUniform1i(texture1Loc, 0);
    
    // Bind normal map texture if available
    if (!m_uiNormalTextureIDVector.empty())
    {
        glActiveTexture(GL_TEXTURE0 + (GLenum)m_uiTextureIDVector.size());
        glBindTexture(GL_TEXTURE_2D, m_uiNormalTextureIDVector[0]);
        GLuint normalMapLoc = glGetUniformLocation(m_uiShaderProgram, "normalMap");
        glUniform1i(normalMapLoc, (GLint)m_uiTextureIDVector.size());
    }
    // Bind the vertex array and draw the mesh
    glBindVertexArray(m_uiVAO);
    // Usage
    EnableVertexAttributes(m_uiFVFFlags);
    MY_GLDRAW_ELEMENTS(GL_TRIANGLES, (GLsizei)m_IDrawndicesBufferVector.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDisable(GL_CULL_FACE);
    glUseProgram(m_uiShaderProgram);
}


void LoadTextureUVs(const tinygltf::Model& model, const tinygltf::Primitive& primitive, std::vector<float>& uvs)
{
    auto it = primitive.attributes.find("TEXCOORD_0");
    if (it == primitive.attributes.end())
    {
        std::cerr << "No TEXCOORD_0 found in primitive." << std::endl;
        return;
    }

    int accessorIndex = it->second;
    const tinygltf::Accessor& accessor = model.accessors[accessorIndex];
    const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
    const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

    size_t byteOffset = accessor.byteOffset + bufferView.byteOffset;
    size_t count = accessor.count;
    size_t stride = accessor.ByteStride(bufferView);

    if (stride == 0) stride = sizeof(float) * 2; // Default for vec2

    uvs.resize(count * 2);

    const unsigned char* dataPtr = buffer.data.data() + byteOffset;

    for (size_t i = 0; i < count; i++)
    {
        float u = *reinterpret_cast<const float*>(dataPtr + i * stride);
        float v = *reinterpret_cast<const float*>(dataPtr + i * stride + sizeof(float));
        uvs[i * 2] = u;
        uvs[i * 2 + 1] = v;
    }
}


void cMesh::LoadAttributes(const tinygltf::Model& model, const tinygltf::Primitive& primitive, bool calculateBinormal)
{
    // Ensure primitive has indices
    if (primitive.indices < 0)
    {
        FMLOG("primitive.indices < 0");
        return;
    }

    m_VertexBufferVector.clear();
    m_IDrawndicesBufferVector.clear();
    m_uiFVFFlags = 0;

    const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
    if (indexAccessor.bufferView == -1)
    {
        FMLOG("indexAccessor.bufferView == -1");
        return;
    }
    const tinygltf::BufferView& bufferView = model.bufferViews[indexAccessor.bufferView];
    const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

    const unsigned char* dataPtr = buffer.data.data() + bufferView.byteOffset + indexAccessor.byteOffset;

    m_IDrawndicesBufferVector.clear();

    if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
    {
        const uint16_t* shortIndices = reinterpret_cast<const uint16_t*>(dataPtr);

        // Reserve space to avoid reallocation
        m_IDrawndicesBufferVector.resize(indexAccessor.count);

        // Use std::copy to convert uint16_t to uint32_t in bulk
        std::transform(shortIndices, shortIndices + indexAccessor.count, m_IDrawndicesBufferVector.begin(),
                       [](uint16_t val)
                       {
                           return static_cast<uint32_t>(val);
                       });
    }
    else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
    {
        // Use uint32_t for indices
        std::vector<uint32_t> tempIndices(indexAccessor.count);
        memcpy(tempIndices.data(), dataPtr, indexAccessor.count * sizeof(uint32_t));

        // Convert from uint32_t to uint16_t if needed (you may want to keep 32-bit indices if the mesh is large)
        m_IDrawndicesBufferVector.assign(tempIndices.begin(), tempIndices.end());
    }
    else
    {
        std::cerr << "Unsupported index component type: " << indexAccessor.componentType << std::endl;
        return;
    }

    // Determine available attributes
    bool hasPosition = primitive.attributes.find("POSITION") != primitive.attributes.end();
    bool hasNormal = primitive.attributes.find("NORMAL") != primitive.attributes.end();
    bool hasTexCoord = primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end();
    bool hasTangent = primitive.attributes.find("TANGENT") != primitive.attributes.end();
    bool hasColor = primitive.attributes.find("COLOR_0") != primitive.attributes.end();
    bool hasNormalMap = primitive.attributes.find("NORMAL_MAP") != primitive.attributes.end();

    // Calculate binormal only if requested and data exists
    bool hasBinormal = calculateBinormal && hasPosition && hasNormal && hasTexCoord;

    // Get the vertex count from the "POSITION" attribute
    size_t vertexCount = 0;
    if (hasPosition)
    {
        const tinygltf::Accessor& positionAccessor = model.accessors[primitive.attributes.at("POSITION")];
        vertexCount = positionAccessor.count;
    }

    // Calculate vertex stride
    m_uiVertexStride = 3 + (hasNormal ? 3 : 0) + (hasTexCoord ? 2 : 0) + (hasTangent ? 3 : 0) + (hasColor ? 4 : 0) + (hasBinormal ? 3 : 0);
    m_VertexBufferVector.resize(vertexCount * m_uiVertexStride); // Resize to the required size

    // Initialize bounding box
    m_vMinBounds = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
    m_vMaxBounds = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    // Lambda function to load an attribute
    auto LoadAttribute = [&](const std::string& name) -> const float*
        {
            auto it = primitive.attributes.find(name);
            if (it == primitive.attributes.end())
            {
                FMLOG("%s can't find data",name.c_str());
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
				m_vMinBounds = Vector3((float)accessor.minValues[0], (float)accessor.minValues[1], (float)accessor.minValues[2]);
                m_vMaxBounds = Vector3((float)accessor.maxValues[0], (float)accessor.maxValues[1], (float)accessor.maxValues[2]);
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

    // Load raw pointers
    const float* positionData = LoadAttribute("POSITION");
    const float* normalData = LoadAttribute("NORMAL");
    const float* texCoordData = LoadAttribute("TEXCOORD_0");
    const float* tangentData = LoadAttribute("TANGENT");
    const float* colorData = LoadAttribute("COLOR_0");

    // Insert data directly into interleavedData and calculate bounding box
    for (size_t i = 0; i < vertexCount; ++i)
    {
        size_t offset = 0;
		int l_iArrtibuteIndex = 0;
        auto l_iCurrentVertexIndex = i * m_uiVertexStride;
        if (positionData)
        {
            l_iArrtibuteIndex = FVF_POS;
            Vector3 vertex(positionData[i * 3], positionData[i * 3 + 1], positionData[i * 3 + 2]);
            memcpy(&m_VertexBufferVector[l_iCurrentVertexIndex + offset], positionData + i * 3, g_iFVF_DataSize[l_iArrtibuteIndex]);
            offset += g_iFVF_DataStride[l_iArrtibuteIndex];

            //// Update bounding box
            //m_vMinBounds.x = min(m_vMinBounds.x, vertex.x);
            //m_vMinBounds.y = min(m_vMinBounds.y, vertex.y);
            //m_vMinBounds.z = min(m_vMinBounds.z, vertex.z);

            //m_vMaxBounds.x = max(m_vMaxBounds.x, vertex.x);
            //m_vMaxBounds.y = max(m_vMaxBounds.y, vertex.y);
            //m_vMaxBounds.z = max(m_vMaxBounds.z, vertex.z);
        }
        if (normalData)
        {
            l_iArrtibuteIndex = FVF_NORMAL;
            memcpy(&m_VertexBufferVector[l_iCurrentVertexIndex + offset], normalData + i * 3, g_iFVF_DataSize[l_iArrtibuteIndex]);
            offset += g_iFVF_DataStride[l_iArrtibuteIndex];
        }
        if (colorData)
        {
            l_iArrtibuteIndex = FVF_DIFFUSE;
            memcpy(&m_VertexBufferVector[l_iCurrentVertexIndex + offset], colorData + i * 4, g_iFVF_DataSize[l_iArrtibuteIndex]);
            offset += g_iFVF_DataStride[l_iArrtibuteIndex];
        }
        if (tangentData)
        {
            l_iArrtibuteIndex = FVF_TANGENT;
            memcpy(&m_VertexBufferVector[l_iCurrentVertexIndex + offset], tangentData + i * 3, g_iFVF_DataSize[l_iArrtibuteIndex]);
            offset += g_iFVF_DataStride[l_iArrtibuteIndex];
        }
        if (hasBinormal)
        {
            l_iArrtibuteIndex = FVF_TANGENT;
            // Binormal data will be calculated later
            offset += g_iFVF_DataStride[l_iArrtibuteIndex];
        }
        if (texCoordData)
        {
            l_iArrtibuteIndex = FVF_TEX0;
            const float* l_fpUVData = texCoordData + i * 2;
            //Vector2 l_vUV(l_fpUVData[0], 1 - l_fpUVData[1]);
            Vector2 l_vUV(l_fpUVData[0], l_fpUVData[1]);
            memcpy(&m_VertexBufferVector[l_iCurrentVertexIndex + offset], &l_vUV, g_iFVF_DataSize[l_iArrtibuteIndex]);
            offset += g_iFVF_DataStride[l_iArrtibuteIndex];
        }
    }
    size_t offset = 3 + (hasNormal ? 3 : 0) + (hasColor ? 4 : 0) + (hasTangent ? 3 : 0);
    // Compute and insert binormals if required
    if (hasBinormal)
    {
        std::vector<float> binormalData(vertexCount * 3); // Allocate based on vertex count

        for (size_t i = 0; i < m_IDrawndicesBufferVector.size(); i += 3) // Iterate per triangle
        {
            Vector3 pos0, pos1, pos2;
            Vector2 uv0, uv1, uv2;

            int i0 = m_IDrawndicesBufferVector[i];
            int i1 = m_IDrawndicesBufferVector[i + 1];
            int i2 = m_IDrawndicesBufferVector[i + 2];

            memcpy(&pos0, positionData + i0 * 3, sizeof(Vector3));
            memcpy(&pos1, positionData + i1 * 3, sizeof(Vector3));
            memcpy(&pos2, positionData + i2 * 3, sizeof(Vector3));

            memcpy(&uv0, texCoordData + i0 * 2, sizeof(Vector2));
            memcpy(&uv1, texCoordData + i1 * 2, sizeof(Vector2));
            memcpy(&uv2, texCoordData + i2 * 2, sizeof(Vector2));

            Vector3 edge1 = pos1 - pos0;
            Vector3 edge2 = pos2 - pos0;
            Vector2 deltaUV1 = uv1 - uv0;
            Vector2 deltaUV2 = uv2 - uv0;

            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
            Vector3 binormal = f * (-deltaUV2.x * edge1 + deltaUV1.x * edge2);
            binormal.NormalizeIt();
            // Store binormal per vertex
            binormalData[i0 * 3 + 0] = binormal.x;
            binormalData[i0 * 3 + 1] = binormal.y;
            binormalData[i0 * 3 + 2] = binormal.z;

            binormalData[i1 * 3 + 0] = binormal.x;
            binormalData[i1 * 3 + 1] = binormal.y;
            binormalData[i1 * 3 + 2] = binormal.z;

            binormalData[i2 * 3 + 0] = binormal.x;
            binormalData[i2 * 3 + 1] = binormal.y;
            binormalData[i2 * 3 + 2] = binormal.z;
        }

        // Interleave binormal data with other vertex attributes
        for (size_t i = 0; i < vertexCount; ++i)
        {
            auto l_iCurrentVertexIndex = i * m_uiVertexStride;;
            memcpy(&m_VertexBufferVector[l_iCurrentVertexIndex + offset], &binormalData[i * 3], g_iFVF_DataSize[FVF_BITAGENT]);
        }
    }

    // Set FVF Flags
    if (hasPosition) m_uiFVFFlags |= FVF_POS_FLAG;
    if (hasNormal) m_uiFVFFlags |= FVF_NORMAL_FLAG;
    if (hasTexCoord) m_uiFVFFlags |= FVF_TEX0_FLAG;
    if (hasTangent) m_uiFVFFlags |= FVF_TANGENT_FLAG;
    if (hasColor) m_uiFVFFlags |= FVF_DIFFUSE_FLAG;
    if (hasBinormal) m_uiFVFFlags |= FVF_BITAGENT_FLAG;
    if (hasNormalMap) m_uiFVFFlags |= FVF_NORMAL_MAP_TEXTURE_FLAG;
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
    if (m_uiFVFFlags & FVF_POS_FLAG) std::cout << "Position is present." << std::endl;
    if (m_uiFVFFlags & FVF_NORMAL_FLAG) std::cout << "Normal is present." << std::endl;
    if (m_uiFVFFlags & FVF_DIFFUSE_FLAG) std::cout << "Color is present." << std::endl;
    if (m_uiFVFFlags & FVF_TEX0_FLAG) std::cout << "Texture coordinates are present." << std::endl;
    if (m_uiFVFFlags & FVF_TANGENT_FLAG) std::cout << "Tangent is present." << std::endl;
    if (m_uiFVFFlags & FVF_BITAGENT_FLAG) std::cout << "Binormal is present." << std::endl;
    if (m_uiFVFFlags & FVF_NORMAL_MAP_TEXTURE_FLAG) std::cout << "Normal map is present." << std::endl;
    //if (m_uiFVFFlags & FVF_METALLIC) std::cout << "Metallic map is present." << std::endl;
    //if (m_uiFVFFlags & FVF_ROUGHNESS) std::cout << "Roughness map is present." << std::endl;
    //if (m_uiFVFFlags & FVF_EMISSIVE) std::cout << "Emissive map is present." << std::endl;
    //if (m_uiFVFFlags & FVF_OCCLUSION) std::cout << "Occlusion map is present." << std::endl;
    //if (m_uiFVFFlags & FVF_BASE_COLOR) std::cout << "Base color map is present." << std::endl;
    //if (m_uiFVFFlags & FVF_PBR) std::cout << "PBR attributes (metallic, roughness, etc.) are present." << std::endl;
}