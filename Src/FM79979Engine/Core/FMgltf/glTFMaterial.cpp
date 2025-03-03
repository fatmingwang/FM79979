#include "../AllCoreInclude.h"
#include "tiny_gltf.h"
#include "glTFMaterial.h"

namespace FATMING_CORE
{
    extern GLfloat	g_fMAG_FILTERValue;
    extern GLfloat	g_fMIN_FILTERValue;
}

cMaterial::cMaterial(unsigned int e_uiShadeProgrameID)
{
    m_uiShadeProgrameID = e_uiShadeProgrameID;
}

cMaterial::~cMaterial()
{
}

void cMaterial::LoadMaterials(const tinygltf::Model& model, const tinygltf::Material& material)
{
    // Load base color texture
    if (material.pbrMetallicRoughness.baseColorTexture.index >= 0)
    {
        const tinygltf::Texture& texture = model.textures[material.pbrMetallicRoughness.baseColorTexture.index];
        const tinygltf::Image& image = model.images[texture.source];
        cTexture* l_pTexture = GetTexture(image);
        m_uiBaseColorTextureVector.push_back(l_pTexture);
    }

    // Load normal map texture (if available)
    if (material.normalTexture.index >= 0)
    {
        const tinygltf::Texture& texture = model.textures[material.normalTexture.index];
        const tinygltf::Image& image = model.images[texture.source];
        cTexture*l_pTexture = GetTexture(image);
        m_uiNormalTextureVector.push_back(l_pTexture);
    }

    // Load occlusion map texture (if available)
    if (material.occlusionTexture.index >= 0)
    {
        const tinygltf::Texture& texture = model.textures[material.occlusionTexture.index];
        const tinygltf::Image& image = model.images[texture.source];
        cTexture* l_pTexture = GetTexture(image);
        m_uiOocclusionTextureVector.push_back(l_pTexture);
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

cTexture* cMaterial::GetTexture(const tinygltf::Image& e_Image)
{
    auto l_OriginalMAG = g_fMAG_FILTERValue;
    auto l_OriginalMIN = g_fMIN_FILTERValue;

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
    cTexture*l_pTexture = cTextureManager::GetObjectByPixels(this, (void*)e_Image.image.data(), e_Image.width, e_Image.height, ValueToStringW(e_Image.uri).c_str(), (int)l_uiFormat);
    g_fMAG_FILTERValue = l_OriginalMAG;
    g_fMIN_FILTERValue = l_OriginalMIN;
    l_pTexture->ApplyImage();
    glGenerateMipmap(GL_TEXTURE_2D);
    l_pTexture->Disable();
    return l_pTexture;
}
