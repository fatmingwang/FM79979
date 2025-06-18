#include <fstream>
#include <iostream>
#include <filesystem>
#include "AnimationTexture.h"
#include "glTFAnimationMesh.h"

#include "../zlib/zlib.h"
#include <vector>
#include <stdexcept>
namespace fs = std::filesystem;
// Utility function to get next power of two
unsigned int NextPowerOfTwo(unsigned int v)
{
    if (v == 0) return 1;
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}


// Compress m_pvTextureData into a memory buffer
std::vector<Bytef> CompressTextureData(Vector4* m_pvTextureData, int textureSquareSize)
{
    // Calculate total data size
    uLong sourceLen = textureSquareSize * textureSquareSize * sizeof(Vector4);

    // Compress with zlib
    uLong destLen = compressBound(sourceLen);
    std::vector<Bytef> compressedData(destLen);
    if (compress(compressedData.data(), &destLen, reinterpret_cast<Bytef*>(m_pvTextureData), sourceLen) != Z_OK)
    {
        throw std::runtime_error("Compression failed");
    }

    // Resize to actual compressed size
    compressedData.resize(destLen);
    return compressedData;
}

// Decompress back into m_pvTextureData
Vector4* DecompressTextureData(const std::vector<Bytef>& compressedData, int textureSquareSize)
{
    // Allocate output buffer
    uLong decompressedSize = textureSquareSize * textureSquareSize * sizeof(Vector4);
    Vector4* m_pvTextureData = new Vector4[textureSquareSize * textureSquareSize];

    // Decompress
    if (uncompress(reinterpret_cast<Bytef*>(m_pvTextureData), &decompressedSize, compressedData.data(), compressedData.size()) != Z_OK)
    {
        delete[] m_pvTextureData;
        throw std::runtime_error("Decompression failed");
    }

    return m_pvTextureData;
}

cAnimTexture::cAnimTexture(cAnimationClip& e_AnimationClip, const char* e_strAnimationName, cSkinningMesh* e_pSkinningMesh)
{
    m_pvTextureData = nullptr;
    m_TextureSquareSize = 0;
    e_AnimationClip.SetAnimation(e_strAnimationName, true, 0.0f);
    auto l_pCurrentAnimationData = e_AnimationClip.GetCurrentAnimationData();
    if (l_pCurrentAnimationData)
    {
        fs::path filePath(e_AnimationClip.GetCharName());
        std::string filenameWithoutExtension = filePath.stem().string();
        std::string l_strPath = filenameWithoutExtension + "/" + std::string(e_strAnimationName) + ".AnimTexture";
        if (this->Load(l_strPath.c_str()))
        {
            return;
        }
        // 30 fps
        auto l_pAllBoneVector = e_pSkinningMesh->GetSkinningBoneVector();
        auto l_NodeInversePoseMatrixVector = e_pSkinningMesh->GetNodeInversePoseMatrixVector();
        bool l_bDoLazyAlign = false;
        float l_fStepTime = ANIMATION_FRAME_STEP_TIME;
        int l_iNumFrames = (int)(l_pCurrentAnimationData->m_fEndTime / l_fStepTime) + 1;
        int l_iNumBones = (int)l_pAllBoneVector->size();
        assert(l_iNumBones <= 100 && "now shader is 100");
        const int l_iNumRowsPerMatrix = 4; // mat4 has 4 rows
        int l_TotalMatrices = l_iNumFrames * l_iNumBones;
        int l_TotalTexels = l_TotalMatrices * l_iNumRowsPerMatrix;
        int l_SQRT = (int)sqrt(l_TotalTexels) + 1;
        int l_iTextureSize = NextPowerOfTwo(l_SQRT);
        if (l_bDoLazyAlign)
        {
            l_iTextureSize = 1024;
        }
        this->Resize(l_iTextureSize);
        int l_iMatrixSize = sizeof(cMatrix44);
        for (int l_iFrameIndex = 0; l_iFrameIndex < l_iNumFrames; ++l_iFrameIndex)
        {
            if (l_iFrameIndex == 81)
            {
                int a = 0;
            }
            e_AnimationClip.Update(ANIMATION_FRAME_STEP_TIME);
            for (int l_iJointIndex = 0; l_iJointIndex < l_iNumBones; ++l_iJointIndex)
            {
                cglTFNodeData* l_pBone = (*l_pAllBoneVector)[l_iJointIndex];
                cMatrix44 l_mat = l_pBone->GetWorldTransform();
                l_mat *= (*l_NodeInversePoseMatrixVector)[l_iJointIndex];
                // Store each row of the matrix as a vec4
                if (l_bDoLazyAlign)
                {
                    const int l_ci4VectorCount = 4;
                    int l_iX = l_iJointIndex * l_ci4VectorCount;
                    int l_iY = (l_iFrameIndex * l_iTextureSize);
                    int l_iDataIndex = l_iX + l_iY;
                    memcpy(&m_pvTextureData[l_iDataIndex], l_mat, sizeof(cMatrix44));
                }
                else
                {
                    int l_iX = (l_iJointIndex * l_iNumRowsPerMatrix);
                    int l_iY = (l_iFrameIndex * l_iNumBones * l_iNumRowsPerMatrix);
                    int l_iDataIndex = l_iX + l_iY;
                    {
                        for (int row = 0; row < 4; ++row)
                        {
                            int x0 = (l_iDataIndex +row) % l_iTextureSize;
                            int y0 = (l_iDataIndex + row) / l_iTextureSize;
                            int a = 0;
                        }
                    }
                    memcpy(&m_pvTextureData[l_iDataIndex], l_mat, l_iMatrixSize);
                }
            }
        }
        glGenTextures(1, &m_uiOpenglTecxtureID);
        UploadTextureDataToGPU();
        Save(l_strPath.c_str());
    }
}

cAnimTexture::cAnimTexture(const cAnimTexture& other)
{
    m_pvTextureData = nullptr;
    m_TextureSquareSize = 0;
    glGenTextures(1, &m_uiOpenglTecxtureID);
    *this = other;
}

cAnimTexture& cAnimTexture::operator=(const cAnimTexture& other)
{
    if (this == &other)
    {
        return *this;
    }

    m_TextureSquareSize = other.m_TextureSquareSize;
    if (m_pvTextureData != 0)
    {
        delete[] m_pvTextureData;
    }
    m_pvTextureData = new Vector4[m_TextureSquareSize * m_TextureSquareSize];
    memcpy(m_pvTextureData, other.m_pvTextureData, sizeof(Vector4) * (m_TextureSquareSize * m_TextureSquareSize));

    return *this;
}

cAnimTexture::cAnimTexture(const char* path)
{
    m_pvTextureData = nullptr;
    m_TextureSquareSize = 0;
    glGenTextures(1, &m_uiOpenglTecxtureID);
}

cAnimTexture::cAnimTexture(unsigned int size)
{
    glGenTextures(1, &m_uiOpenglTecxtureID);
    m_TextureSquareSize = size;
    m_pvTextureData = new Vector4[size * size];
}

cAnimTexture::~cAnimTexture()
{
    SAFE_DELETE(m_pvTextureData);
    if (m_uiOpenglTecxtureID != -1)
    {
        glDeleteTextures(1, &m_uiOpenglTecxtureID);
    }
}

bool cAnimTexture::Save(const char* path)
{
    // Create parent directories if they don't exist
    fs::path filePath(path);
    fs::path dir = filePath.parent_path();
    if (!dir.empty() && !fs::exists(dir))
    {
        fs::create_directories(dir);
    }

    // Open file in binary mode
    std::ofstream file(path, std::ios::out | std::ios::binary);
    if (!file.is_open())
    {
        FMLOG("Couldn't open %s to write to", path);
        return false;
    }

    // Validate texture size
    if (m_TextureSquareSize <= 0)
    {
        FMLOG("Trying to write Animation Texture With Size 0");
        file.close();
        return false;
    }

    // Write m_TextureSquareSize (binary)
    file.write(reinterpret_cast<const char*>(&m_TextureSquareSize), sizeof(int));

    // Compress m_pvTextureData
    std::vector<Bytef> l_CompressData = CompressTextureData(m_pvTextureData, m_TextureSquareSize);
    int compressedSize = static_cast<int>(l_CompressData.size());

    // Write compressed size and data (binary)
    file.write(reinterpret_cast<const char*>(&compressedSize), sizeof(int));
    file.write(reinterpret_cast<const char*>(l_CompressData.data()), compressedSize);

    // Check for write errors
    if (!file.good())
    {
        FMLOG("Failed to write data to %s", path);
        file.close();
        return false;
    }

    file.close();
    return true;
}

bool cAnimTexture::Load(const char* path)
{
    // Open file in binary mode
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        FMLOG("Couldn't open %s to read from", path);
        return false;
    }

    // Read m_TextureSquareSize (binary)
    file.read(reinterpret_cast<char*>(&m_TextureSquareSize), sizeof(int));
    if (!file.good() || m_TextureSquareSize <= 0)
    {
        FMLOG("Invalid or zero texture size in %s", path);
        file.close();
        return false;
    }

    // Read compressed size
    int compressedSize;
    file.read(reinterpret_cast<char*>(&compressedSize), sizeof(int));
    if (!file.good() || compressedSize <= 0)
    {
        FMLOG("Invalid compressed size in %s", path);
        file.close();
        return false;
    }

    // Read compressed data
    std::vector<Bytef> l_CompressedData(compressedSize);
    file.read(reinterpret_cast<char*>(l_CompressedData.data()), compressedSize);
    if (!file.good())
    {
        FMLOG("Failed to read compressed data from %s", path);
        file.close();
        return false;
    }

    // Close file before decompression
    file.close();

    // Deallocate existing m_pvTextureData to prevent leaks
    delete[] m_pvTextureData;
    m_pvTextureData = nullptr;

    // Decompress data
    try
    {
        m_pvTextureData = DecompressTextureData(l_CompressedData, m_TextureSquareSize);
    }
    catch (const std::runtime_error& e)
    {
        FMLOG("Decompression failed: %s", e.what());
        return false;
    }

    // Upload to GPU
    UploadTextureDataToGPU();
    return true;
}



void cAnimTexture::UploadTextureDataToGPU()
{
    glBindTexture(GL_TEXTURE_2D, m_uiOpenglTecxtureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_TextureSquareSize, m_TextureSquareSize, 0, GL_RGBA, GL_FLOAT, m_pvTextureData);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int cAnimTexture::Size()
{
    return m_TextureSquareSize;
}

void cAnimTexture::Resize(unsigned int newSize)
{
    if (m_pvTextureData != 0)
    {
        delete[] m_pvTextureData;
    }
    m_TextureSquareSize = newSize;
    m_pvTextureData = new Vector4[m_TextureSquareSize * m_TextureSquareSize];
}

unsigned int cAnimTexture::GetHandle()
{
    return m_uiOpenglTecxtureID;
}

void cAnimTexture::SetTexel(unsigned int x, unsigned int y, const Vector4& e_Value)
{
    unsigned int index = (y * m_TextureSquareSize) + x;

    m_pvTextureData[index] = e_Value;
}

void cAnimTexture::SetTexel(unsigned int x, unsigned int y, const Vector3& e_Value)
{
    unsigned int index = (y * m_TextureSquareSize) + x;
    Vector4 l_vData(e_Value.x, e_Value.y, e_Value.z, 0.f);
    m_pvTextureData[index] = l_vData;
}

void cAnimTexture::Set(unsigned int uniformIndex, unsigned int textureIndex)
{
    glActiveTexture(GL_TEXTURE0 + textureIndex);
    glBindTexture(GL_TEXTURE_2D, m_uiOpenglTecxtureID);
    glUniform1i(uniformIndex, textureIndex); // Bind uniform X to sampler Y
}
void cAnimTexture::UnSet(unsigned int textureIndex)
{
    glActiveTexture(GL_TEXTURE0 + textureIndex);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
}

float* cAnimTexture::GetData()
{
    return (float*)m_pvTextureData;
}