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
		m_i64TextureFVFFlag |= 1LL << FVF_HAS_PBR_TEXTURE;
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
        m_i64TextureFVFFlag |= 1LL << FVF_HAS_PBR_TEXTURE;
    }
}

void cMaterial::Apply()
{
    // Bind textures
    for (size_t i = 0; i < m_uiBaseColorTextureVector.size(); ++i)
    {
        m_uiBaseColorTextureVector[i]->ApplyImageWithActiveTextureID((GLuint)i);
        //glActiveTexture(GL_TEXTURE0 + (GLenum)i);
        //glBindTexture(GL_TEXTURE_2D, m_uiTextureIDVector[i]);
    }
    GLuint texture1Loc = glGetUniformLocation(m_uiShadeProgrameID, "texture1");
    glUniform1i(texture1Loc, 0);

    // Bind normal map texture if available
    if (!m_uiNormalTextureVector.empty())
    {
        //
        m_uiNormalTextureVector[0]->ApplyImageWithActiveTextureID((GLuint)m_uiBaseColorTextureVector.size());
        //glActiveTexture(GL_TEXTURE0 + (GLenum)m_uiTextureIDVector.size());
        //glBindTexture(GL_TEXTURE_2D, m_uiNormalTextureIDVector[0]);
        GLuint normalMapLoc = glGetUniformLocation(m_uiShadeProgrameID, "normalMap");
        glUniform1i(normalMapLoc, (GLint)m_uiBaseColorTextureVector.size());
    }

}


bool cMaterial::ApplyUnriforms()
{
	return false;
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
