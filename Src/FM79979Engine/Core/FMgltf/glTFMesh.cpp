
#include "../AllCoreInclude.h"
#include <vector>
#include <string>
#include <map>
#include "tiny_gltf.h"
#include <iostream>
#include "glTFMesh.h"



// Constructor
cMesh::cMesh() : VAO(0), VBO(0), EBO(0), vertexStride(0), fvfFlags(0)
{
}

// Destructor
cMesh::~cMesh()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void cMesh::InitBuffer()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, interleavedData.size() * sizeof(float), interleavedData.data(), GL_STATIC_DRAW);

    // Upload index data (detect type dynamically)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    GLenum indexType = (indices.size() > 0 && sizeof(indices[0]) == 4) ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);


    // Define vertex attribute pointers dynamically based on FVF flags
    size_t offset = 0;

    if (fvfFlags & FVF_POSITION1)
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexStride * sizeof(float), (void*)offset);
        glEnableVertexAttribArray(0);
        offset += 3 * sizeof(float);
    }
    if (fvfFlags & FVF_NORMAL2)
    {
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexStride * sizeof(float), (void*)offset);
        glEnableVertexAttribArray(1);
        offset += 3 * sizeof(float);
    }
    if (fvfFlags & FVF_TEXCOORD4)
    {
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexStride * sizeof(float), (void*)offset);
        glEnableVertexAttribArray(2);
        offset += 2 * sizeof(float);
    }
    if (fvfFlags & FVF_COLOR3)
    {
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, vertexStride * sizeof(float), (void*)offset);
        glEnableVertexAttribArray(3);
        offset += 4 * sizeof(float);
    }
    if (fvfFlags & FVF_BINORMAL5)
    {
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, vertexStride * sizeof(float), (void*)offset);
        glEnableVertexAttribArray(4);
        offset += 3 * sizeof(float);
    }

    glBindVertexArray(0);
}


// Draw the mesh
void cMesh::Draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}

void cMesh::LoadAttributes(const tinygltf::Model& model, const tinygltf::Primitive& primitive, bool calculateBinormal)
{
    // Ensure primitive has indices
    if (primitive.indices < 0)
    {
        return;
    }

    interleavedData.clear();
    indices.clear();
    fvfFlags = 0;

    const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
    const tinygltf::BufferView& bufferView = model.bufferViews[indexAccessor.bufferView];
    const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

    const unsigned char* dataPtr = buffer.data.data() + bufferView.byteOffset + indexAccessor.byteOffset;

    indices.clear();

    if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
    {
        const uint16_t* shortIndices = reinterpret_cast<const uint16_t*>(dataPtr);

        // Reserve space to avoid reallocation
        indices.resize(indexAccessor.count);

        // Use std::copy to convert uint16_t to uint32_t in bulk
        std::transform(shortIndices, shortIndices + indexAccessor.count, indices.begin(),
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
        indices.assign(tempIndices.begin(), tempIndices.end());
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

    // Calculate binormal only if requested and data exists
    bool hasBinormal = calculateBinormal && hasPosition && hasNormal && hasTexCoord;

    // Calculate vertex stride
    vertexStride = 3 + (hasNormal ? 3 : 0) + (hasTexCoord ? 2 : 0) + (hasTangent ? 3 : 0) + (hasColor ? 4 : 0) + (hasBinormal ? 3 : 0);
    interleavedData.reserve(indexAccessor.count * vertexStride); // Reserve space to avoid multiple allocations

    // Lambda function to load an attribute
    auto LoadAttribute = [&](const std::string& name) -> const float*
        {
            if (primitive.attributes.find(name) == primitive.attributes.end()) return nullptr;
            const tinygltf::Accessor& accessor = model.accessors[primitive.attributes.at(name)];
            const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
            return reinterpret_cast<const float*>(&model.buffers[bufferView.buffer].data[accessor.byteOffset]);
        };

    // Load raw pointers
    const float* positionData = LoadAttribute("POSITION");
    const float* normalData = LoadAttribute("NORMAL");
    const float* texCoordData = LoadAttribute("TEXCOORD_0");
    const float* tangentData = LoadAttribute("TANGENT");
    const float* colorData = LoadAttribute("COLOR_0");

    // Insert data directly into interleavedData
    if (positionData) interleavedData.insert(interleavedData.end(), positionData, positionData + indexAccessor.count * 3);
    if (normalData) interleavedData.insert(interleavedData.end(), normalData, normalData + indexAccessor.count * 3);
    if (texCoordData) interleavedData.insert(interleavedData.end(), texCoordData, texCoordData + indexAccessor.count * 2);
    if (tangentData) interleavedData.insert(interleavedData.end(), tangentData, tangentData + indexAccessor.count * 3);
    if (colorData) interleavedData.insert(interleavedData.end(), colorData, colorData + indexAccessor.count * 4);

    // Compute and insert binormals if required
    if (hasBinormal)
    {
        std::vector<float> binormalData(indices.size() * 3); // Allocate based on triangle count

        for (size_t i = 0; i < indices.size(); i += 3) // Iterate per triangle
        {
            Vector3 pos0, pos1, pos2;
            Vector2 uv0, uv1, uv2;

            int i0 = indices[i];
            int i1 = indices[i + 1];
            int i2 = indices[i + 2];

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

            // Store binormal per triangle vertex
            binormalData[i * 3 + 0] = binormal.x;
            binormalData[i * 3 + 1] = binormal.y;
            binormalData[i * 3 + 2] = binormal.z;

            binormalData[i * 3 + 3] = binormal.x;
            binormalData[i * 3 + 4] = binormal.y;
            binormalData[i * 3 + 5] = binormal.z;

            binormalData[i * 3 + 6] = binormal.x;
            binormalData[i * 3 + 7] = binormal.y;
            binormalData[i * 3 + 8] = binormal.z;
        }

        interleavedData.insert(interleavedData.end(), binormalData.begin(), binormalData.end());
    }


    // Set FVF Flags
    if (hasPosition) fvfFlags |= FVF_POSITION1;
    if (hasNormal) fvfFlags |= FVF_NORMAL2;
    if (hasTexCoord) fvfFlags |= FVF_TEXCOORD4;
    if (hasTangent) fvfFlags |= FVF_COLOR3;
    if (hasColor) fvfFlags |= FVF_COLOR3;
    if (hasBinormal) fvfFlags |= FVF_BINORMAL5;
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

        textures.push_back(textureID);
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

        normalTextures.push_back(textureID);
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

        occlusionTextures.push_back(textureID);
    }
}

// Log FVF Flags
void cMesh::logFVFFlags()
{
    std::cout << "FVF Flags: " << std::endl;
    if (fvfFlags & FVF_POSITION1) std::cout << "Position is present." << std::endl;
    if (fvfFlags & FVF_NORMAL2) std::cout << "Normal is present." << std::endl;
    if (fvfFlags & FVF_COLOR3) std::cout << "Color is present." << std::endl;
    if (fvfFlags & FVF_TEXCOORD4) std::cout << "Texture coordinates are present." << std::endl;
    if (fvfFlags & FVF_BINORMAL5) std::cout << "Binormal is present." << std::endl;
}