#include "../AllCoreInclude.h"
#include "glTFMaterial.h"

namespace FATMING_CORE
{
    extern GLuint	g_uiMAG_FILTERValue;
    extern GLuint	g_uiMIN_FILTERValue;
    extern GLuint	g_uiTEXTURE_WRAP_S;
    extern GLuint	g_uiTEXTURE_WRAP_T;
}

TYPDE_DEFINE_MARCO(cMaterial);

cMaterial::cMaterial(unsigned int e_uiShadeProgrameID)
{
    m_uiShadeProgrameID = e_uiShadeProgrameID;
    m_baseColorFactor[0] = m_baseColorFactor[1] = m_baseColorFactor[2] = m_baseColorFactor[3] = 1.0f;
    m_metallicFactor = 1.0f;
    m_roughnessFactor = 1.0f;
    m_occlusionStrength = 1.0f;
    m_emissiveFactor[0] = m_emissiveFactor[1] = m_emissiveFactor[2] = 0.0f;
}

cMaterial::~cMaterial()
{
}

void cMaterial::LoadMaterials(const tinygltf::Model& model, const tinygltf::Material& material)
{
    this->SetName(material.name.c_str());
    // Load base color texture
    if (material.pbrMetallicRoughness.baseColorTexture.index >= 0)
    {
        const tinygltf::Texture& texture = model.textures[material.pbrMetallicRoughness.baseColorTexture.index];
        const tinygltf::Image& image = model.images[texture.source];
        const tinygltf::Sampler*l_pSampler = nullptr;
        if (texture.sampler != -1)
        {
            l_pSampler = &model.samplers[texture.sampler];
        }        
        shared_ptr<cTexture>l_pTexture = GetTexture(image, l_pSampler);
        m_uiBaseColorTextureVector.push_back(l_pTexture);
		m_i64TextureFVFFlag |= 1LL << FVF_TEX0_FLAG;
    }
    // Load base color factor
    for (size_t i = 0; i < 4; ++i)
    {
        m_baseColorFactor[i] = static_cast<float>(material.pbrMetallicRoughness.baseColorFactor[i]);
    }

    // Load normal map texture (if available)
    if (material.normalTexture.index >= 0)
    {
        const tinygltf::Texture& texture = model.textures[material.normalTexture.index];
        const tinygltf::Image& image = model.images[texture.source];
        const tinygltf::Sampler* l_pSampler = nullptr;
        if (texture.sampler != -1)
        {
            l_pSampler = &model.samplers[texture.sampler];
        }
        shared_ptr<cTexture>l_pTexture = GetTexture(image, l_pSampler);
        m_uiNormalTextureVector.push_back(l_pTexture);
        m_i64TextureFVFFlag |= 1LL << FVF_HAS_NORMAL_MAP_TEXTURE;
    }
    // Load occlusion map texture (if available)
    if (material.occlusionTexture.index >= 0)
    {
        const tinygltf::Texture& texture = model.textures[material.occlusionTexture.index];
        const tinygltf::Image& image = model.images[texture.source];
        const tinygltf::Sampler* l_pSampler = nullptr;
        if (texture.sampler != -1)
        {
            l_pSampler = &model.samplers[texture.sampler];
        }
        shared_ptr<cTexture> l_pTexture = GetTexture(image, l_pSampler);
        m_uiOocclusionTextureVector.push_back(l_pTexture);
        m_i64TextureFVFFlag |= 1LL << FVF_HAS_FVF_OCCLUSION_TEXTURE;
    }
    // Load occlusion strength
    m_occlusionStrength = static_cast<float>(material.occlusionTexture.strength);

    // Load metallic-roughness texture and factors
    if (material.pbrMetallicRoughness.metallicRoughnessTexture.index >= 0)
    {
        const tinygltf::Texture& texture = model.textures[material.pbrMetallicRoughness.metallicRoughnessTexture.index];
        const tinygltf::Image& image = model.images[texture.source];
        const tinygltf::Sampler* l_pSampler = nullptr;
        if (texture.sampler != -1)
        {
            l_pSampler = &model.samplers[texture.sampler];
        }
        shared_ptr<cTexture> l_pTexture = GetTexture(image, l_pSampler);
        m_uiMetallicRoughnessTextureVector.push_back(l_pTexture);
        m_i64TextureFVFFlag |= 1LL << FVF_HAS_METALLIC_ROUGHNESS_TEXTURE;
    }
    m_metallicFactor = static_cast<float>(material.pbrMetallicRoughness.metallicFactor);
    m_roughnessFactor = static_cast<float>(material.pbrMetallicRoughness.roughnessFactor);

    // Load emissive texture and factor
    if (material.emissiveTexture.index >= 0)
    {
        const tinygltf::Texture& texture = model.textures[material.emissiveTexture.index];
        const tinygltf::Image& image = model.images[texture.source];
        const tinygltf::Sampler* l_pSampler = nullptr;
        if (texture.sampler != -1)
        {
            l_pSampler = &model.samplers[texture.sampler];
        }
        shared_ptr<cTexture> l_pTexture = GetTexture(image, l_pSampler);
        m_uiEmissiveTextureIDVector.push_back(l_pTexture);
        m_i64TextureFVFFlag |= 1LL << FVF_HAS_EMISSIVE_TEXTURE;
    }
    for (size_t i = 0; i < 3; ++i)
    {
        m_emissiveFactor[i] = static_cast<float>(material.emissiveFactor[i]);
    }
}

void cMaterial::Apply()
{
    GLuint textureUnit = 0;

    // Bind base color texture
    for (size_t i = 0; i < m_uiBaseColorTextureVector.size(); ++i)
    {
        m_uiBaseColorTextureVector[i]->ApplyImageWithActiveTextureID(textureUnit);
        GLuint textureLoc = glGetUniformLocation(m_uiShadeProgrameID, "textureDiffuse");
        if (textureLoc != GL_INVALID_INDEX)
        {
            glUniform1i(textureLoc, textureUnit);
        }
        textureUnit++;
    }

    // Bind normal map texture
    if (!m_uiNormalTextureVector.empty())
    {
        m_uiNormalTextureVector[0]->ApplyImageWithActiveTextureID(textureUnit);
        GLuint normalMapLoc = glGetUniformLocation(m_uiShadeProgrameID, "textureNormal");
        if (normalMapLoc != GL_INVALID_INDEX)
        {
            glUniform1i(normalMapLoc, textureUnit);
        }
        textureUnit++;
    }

    // Bind occlusion texture
    if (!m_uiOocclusionTextureVector.empty())
    {
        m_uiOocclusionTextureVector[0]->ApplyImageWithActiveTextureID(textureUnit);
        GLuint occlusionLoc = glGetUniformLocation(m_uiShadeProgrameID, "textureOcclusion");
        if (occlusionLoc != GL_INVALID_INDEX)
        {
            glUniform1i(occlusionLoc, textureUnit);
        }
        textureUnit++;
    }

    // Bind metallic-roughness texture
    if (!m_uiMetallicRoughnessTextureVector.empty())
    {
        m_uiMetallicRoughnessTextureVector[0]->ApplyImageWithActiveTextureID(textureUnit);
        GLuint metallicRoughnessLoc = glGetUniformLocation(m_uiShadeProgrameID, "textureMetallicRoughness");
        if (metallicRoughnessLoc != GL_INVALID_INDEX)
        {
            glUniform1i(metallicRoughnessLoc, textureUnit);
        }
        textureUnit++;
    }

    // Bind emissive texture
    if (!m_uiEmissiveTextureIDVector.empty())
    {
        m_uiEmissiveTextureIDVector[0]->ApplyImageWithActiveTextureID(textureUnit);
        GLuint emissiveLoc = glGetUniformLocation(m_uiShadeProgrameID, "textureEmissive");
        if (emissiveLoc != GL_INVALID_INDEX)
        {
            glUniform1i(emissiveLoc, textureUnit);
        }
        textureUnit++;
    }
    ApplyUnriforms();
}


bool cMaterial::ApplyUnriforms()
{
    bool success = true;

    // Set base color factor
    GLuint baseColorFactorLoc = glGetUniformLocation(m_uiShadeProgrameID, "baseColorFactor");
    if (baseColorFactorLoc != GL_INVALID_INDEX)
    {
        glUniform4fv(baseColorFactorLoc, 1, m_baseColorFactor);
    }
    else
    {
        success = false;
    }

    // Set metallic factor
    GLuint metallicFactorLoc = glGetUniformLocation(m_uiShadeProgrameID, "metallicFactor");
    if (metallicFactorLoc != GL_INVALID_INDEX)
    {
        glUniform1f(metallicFactorLoc, m_metallicFactor);
    }
    else
    {
        success = false;
    }

    // Set roughness factor
    GLuint roughnessFactorLoc = glGetUniformLocation(m_uiShadeProgrameID, "roughnessFactor");
    if (roughnessFactorLoc != GL_INVALID_INDEX)
    {
        glUniform1f(roughnessFactorLoc, m_roughnessFactor);
    }
    else
    {
        success = false;
    }

    // Set occlusion strength
    GLuint occlusionStrengthLoc = glGetUniformLocation(m_uiShadeProgrameID, "occlusionStrength");
    if (occlusionStrengthLoc != GL_INVALID_INDEX)
    {
        glUniform1f(occlusionStrengthLoc, m_occlusionStrength);
    }
    else
    {
        success = false;
    }

    // Set emissive factor
    GLuint emissiveFactorLoc = glGetUniformLocation(m_uiShadeProgrameID, "emissiveFactor");
    if (emissiveFactorLoc != GL_INVALID_INDEX)
    {
        glUniform3fv(emissiveFactorLoc, 1, m_emissiveFactor);
    }
    else
    {
        success = false;
    }

    return success;
}

shared_ptr<cTexture> cMaterial::GetTexture(const tinygltf::Image& e_Image,const tinygltf::Sampler*e_pSampler)
{
    auto l_OriginalMAG = g_uiMAG_FILTERValue;
    auto l_OriginalMIN = g_uiMIN_FILTERValue;
    auto l_OriginalWRAP_S = g_uiTEXTURE_WRAP_S;
    auto l_OriginalWRAP_T = g_uiTEXTURE_WRAP_T;

    auto l_uiFormat = GL_RGBA;
    if (e_Image.component == 3)
    {
        l_uiFormat = GL_RGB;
    }
    else
    if (e_Image.component == 4)
    {
        l_uiFormat = GL_RGBA;
    }
    if (e_pSampler)
    {
        if (e_pSampler->magFilter != -1 && e_pSampler->minFilter != -1)
        {
            g_uiMAG_FILTERValue = e_pSampler->magFilter;
            g_uiMIN_FILTERValue = e_pSampler->minFilter;
        }
        g_uiTEXTURE_WRAP_S = e_pSampler->wrapS;
        g_uiTEXTURE_WRAP_T = e_pSampler->wrapT;
    }
    shared_ptr<cTexture>l_pTexture = cTextureManager::GetObjectByPixels((void*)e_Image.image.data(), e_Image.width, e_Image.height, ValueToStringW(e_Image.uri).c_str(), (int)l_uiFormat);
    g_uiMAG_FILTERValue = l_OriginalMAG;
    g_uiMIN_FILTERValue = l_OriginalMIN;
    g_uiTEXTURE_WRAP_S = l_OriginalWRAP_S;
    g_uiTEXTURE_WRAP_T = l_OriginalWRAP_T;
    l_pTexture->ApplyImage();
    glGenerateMipmap(GL_TEXTURE_2D);
    l_pTexture->Disable();
    return l_pTexture;
}
