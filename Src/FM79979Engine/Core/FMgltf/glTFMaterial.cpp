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

bool cMaterial::BindTecture(shared_ptr<cTexture> e_spTexture, GLuint& e_uiTextureUnit,const char*e_strTextureName)
{
    if (e_spTexture)
    {
        GLuint occlusionLoc = glGetUniformLocation(m_uiShaderProgrameID, e_strTextureName);
        if (occlusionLoc != GL_INVALID_INDEX)
        {
            e_spTexture->ApplyImageWithActiveTextureID(e_uiTextureUnit);
            glUniform1i(occlusionLoc, e_uiTextureUnit);
            e_uiTextureUnit++;
        }
        else
        {
            FMLOG("not use in shader code");
        }
    }
    else
    {
        glActiveTexture(GL_TEXTURE0 + e_uiTextureUnit);
        glBindTexture(GL_TEXTURE_2D, 0);
        ++e_uiTextureUnit;
    }
    return false;
}

cMaterial::cMaterial()
{
    m_baseColorFactor[0] = m_baseColorFactor[1] = m_baseColorFactor[2] = m_baseColorFactor[3] = 1.0f;
    m_metallicFactor = 1.0f;
    m_roughnessFactor = 1.0f;
    m_occlusionStrength = 1.0f;
    m_emissiveFactor[0] = m_emissiveFactor[1] = m_emissiveFactor[2] = 0.0f;
    m_specularFactor = 1.0f;
    m_spSpecularTexture = nullptr;
    m_spSpecularColorTexture = nullptr;
    m_specularColorFactor[0] = m_specularColorFactor[1] = m_specularColorFactor[2] = 1.0f;
}

cMaterial::~cMaterial()
{
}



void cMaterial::LoadMaterials(const tinygltf::Model& model, const tinygltf::Material& material)
{
    this->SetName(material.name.c_str());
    // Standard PBR in glTF 2.0
    bool hasPBR = material.pbrMetallicRoughness.baseColorFactor.size() == 4;
    // Unlit extension disables PBR
    bool isUnlit = material.extensions.count("KHR_materials_unlit") > 0;

    m_TectureAndTexCoordinateIndex.m_bUsePBR = (hasPBR && !isUnlit);
    if (material.alphaMode.empty())
    {
        m_TectureAndTexCoordinateIndex.m_strAlphaMode = "OPAQUE";
    }
    else
    {
        m_TectureAndTexCoordinateIndex.m_strAlphaMode = material.alphaMode;
    }
        
    m_TectureAndTexCoordinateIndex.m_fAlphaCutoff = (float)material.alphaCutoff;
    FMLOG("MaterialName:%s", material.name.c_str());
	m_bDoubleSize = material.doubleSided;
    if (m_bDoubleSize)
    {
        int a = 0;
    }
    // Load base color texture
    if (material.pbrMetallicRoughness.baseColorTexture.index >= 0)
    {
        const tinygltf::Texture& texture = model.textures[material.pbrMetallicRoughness.baseColorTexture.index];
		m_TectureAndTexCoordinateIndex.m_iBaseColorTextureCoordinateIndex = material.pbrMetallicRoughness.baseColorTexture.texCoord;
        tinygltf::Image image = model.images[texture.source];
        const tinygltf::Sampler*l_pSampler = nullptr;
        if (texture.sampler != -1)
        {
            l_pSampler = &model.samplers[texture.sampler];
        }        
        if (image.name.empty())
        {
            image.name = material.name+"_BaceTexture_" + std::to_string(material.pbrMetallicRoughness.baseColorTexture.index);
        }
        shared_ptr<cTexture>l_pTexture = GetTexture(image, l_pSampler);
        m_spBaseColorTexture = l_pTexture;
		m_i64TextureFVFFlag |= FVF_BASE_COLOR_TEXTURE_FLAG;
    }

    // Load normal map texture (if available)
    if (material.normalTexture.index >= 0)
    {
        const tinygltf::Texture& texture = model.textures[material.normalTexture.index];
        m_TectureAndTexCoordinateIndex.m_iNormalTextureCoordinateIndex = material.normalTexture.texCoord;
        tinygltf::Image image = model.images[texture.source];
        const tinygltf::Sampler* l_pSampler = nullptr;
        if (texture.sampler != -1)
        {
            l_pSampler = &model.samplers[texture.sampler];
        }
        if(image.name.empty())
        {
            image.name = material.name + "_NormalTexture_" + std::to_string(material.normalTexture.index);
        }
        shared_ptr<cTexture>l_pTexture = GetTexture(image, l_pSampler);
        m_spNormalTexture = l_pTexture;
        m_i64TextureFVFFlag |= FVF_NORMAL_MAP_TEXTURE_FLAG;
    }
    // Load occlusion map texture (if available)
    if (material.occlusionTexture.index >= 0)
    {
        const tinygltf::Texture& texture = model.textures[material.occlusionTexture.index];
        m_TectureAndTexCoordinateIndex.m_iOocclusionTextureCoordinateIndex = material.occlusionTexture.texCoord;
        tinygltf::Image image = model.images[texture.source];
        const tinygltf::Sampler* l_pSampler = nullptr;
        if (texture.sampler != -1)
        {
            l_pSampler = &model.samplers[texture.sampler];
        }
        if (image.name.empty())
        {
            image.name = material.name + "_OcclusionTexture_" + std::to_string(material.occlusionTexture.index);
        }
        shared_ptr<cTexture> l_pTexture = GetTexture(image, l_pSampler);
        m_spOocclusionTexture = l_pTexture;
        m_i64TextureFVFFlag |=  FVF_FVF_OCCLUSION_TEXTURE_FLAG;
    }

    // Load metallic-roughness texture and factors
    if (material.pbrMetallicRoughness.metallicRoughnessTexture.index >= 0)
    {
        const tinygltf::Texture& texture = model.textures[material.pbrMetallicRoughness.metallicRoughnessTexture.index];
        m_TectureAndTexCoordinateIndex.m_iMetallicRoughnessTextureCoordinateIndex = material.pbrMetallicRoughness.metallicRoughnessTexture.texCoord;
        tinygltf::Image image = model.images[texture.source];
        const tinygltf::Sampler* l_pSampler = nullptr;
        if (texture.sampler != -1)
        {
            l_pSampler = &model.samplers[texture.sampler];
        }
        if (image.name.empty())
        {
            image.name = material.name + "_MetallicRoughnessTexture_" + std::to_string(material.pbrMetallicRoughness.metallicRoughnessTexture.index);
        }
        shared_ptr<cTexture> l_pTexture = GetTexture(image, l_pSampler);
        m_spMetallicRoughnessTexture = l_pTexture;
        m_i64TextureFVFFlag |= FVF_METALLIC_ROUGHNESS_TEXTURE_FLAG;
    }

    // Load emissive texture and factor
    if (material.emissiveTexture.index >= 0)
    {
        const tinygltf::Texture& texture = model.textures[material.emissiveTexture.index];
        m_TectureAndTexCoordinateIndex.m_iEmissiveTextureCoordinateIndex = material.emissiveTexture.texCoord;
        tinygltf::Image image = model.images[texture.source];
        const tinygltf::Sampler* l_pSampler = nullptr;
        if (texture.sampler != -1)
        {
            l_pSampler = &model.samplers[texture.sampler];
        }
        if (image.name.empty())
        {
            image.name = material.name + "_EmissiveTexture_" + std::to_string(material.emissiveTexture.index);
        }
        shared_ptr<cTexture> l_pTexture = GetTexture(image, l_pSampler);
        m_spEmissiveTexture = l_pTexture;
        m_i64TextureFVFFlag |= FVF_EMISSIVE_TEXTURE_FLAG;
    }

    // --- KHR_materials_specular extension ---
    m_specularFactor = 1.0f;
    m_spSpecularTexture = nullptr;
    m_spSpecularColorTexture = nullptr;
    m_specularColorFactor[0] = m_specularColorFactor[1] = m_specularColorFactor[2] = 1.0f;
    if (material.extensions.count("KHR_materials_specular")) {
        const auto& ext = material.extensions.at("KHR_materials_specular");
        if (ext.Has("specularFactor"))
        {
            m_specularFactor = (float)ext.Get("specularFactor").GetNumberAsDouble();
        }
            
        if (ext.Has("specularTexture")) 
        {
            const auto& specTex = ext.Get("specularTexture");
            int texIndex = specTex.Get("index").GetNumberAsInt();
            int texCoord = 0;
            if (specTex.Has("texCoord"))
            { 
                texCoord = specTex.Get("texCoord").GetNumberAsInt();
            }
            m_TectureAndTexCoordinateIndex.m_iSpecularTextureCoordinateIndex = texCoord;
            if (texIndex >= 0 && texIndex < (int)model.textures.size()) {
                const tinygltf::Texture& texture = model.textures[texIndex];
                tinygltf::Image image = model.images[texture.source];
                const tinygltf::Sampler* l_pSampler = nullptr;
                if (texture.sampler != -1)
                {
                    l_pSampler = &model.samplers[texture.sampler];
                }
                    
                if (image.name.empty())
                {
                    image.name = material.name + "_SpecularTexture_" + std::to_string(texIndex);
                }
                m_spSpecularTexture = GetTexture(image, l_pSampler);
                m_TectureAndTexCoordinateIndex.m_bUseSpecular = true;
            }
        }
        if (ext.Has("specularColorFactor")) {
            const auto& arr = ext.Get("specularColorFactor");
            for (int i = 0; i < 3; ++i) {
                m_specularColorFactor[i] = arr.Get(i).GetNumberAsDouble();
            }
        }
        if (ext.Has("specularColorTexture")) {
            const auto& specColTex = ext.Get("specularColorTexture");
            int texIndex = specColTex.Get("index").GetNumberAsInt();
            int texCoord = 0;
            if (specColTex.Has("texCoord"))
                texCoord = specColTex.Get("texCoord").GetNumberAsInt();
            m_TectureAndTexCoordinateIndex.m_iSpecularColorTextureCoordinateIndex = texCoord;
            if (texIndex >= 0 && texIndex < (int)model.textures.size()) {
                const tinygltf::Texture& texture = model.textures[texIndex];
                tinygltf::Image image = model.images[texture.source];
                const tinygltf::Sampler* l_pSampler = nullptr;
                if (texture.sampler != -1)
                    l_pSampler = &model.samplers[texture.sampler];
                if (image.name.empty())
                    image.name = material.name + "_SpecularColorTexture_" + std::to_string(texIndex);
                m_spSpecularColorTexture = GetTexture(image, l_pSampler);
            }
        }
    }

    for (size_t i = 0; i < 3; ++i)
    {
        m_emissiveFactor[i] = static_cast<float>(material.emissiveFactor[i]);
    }
    m_metallicFactor = static_cast<float>(material.pbrMetallicRoughness.metallicFactor);
    m_roughnessFactor = static_cast<float>(material.pbrMetallicRoughness.roughnessFactor);
    // Load occlusion strength
    m_occlusionStrength = static_cast<float>(material.occlusionTexture.strength);
    // Load base color factor
    for (size_t i = 0; i < 4; ++i)
    {
        m_baseColorFactor[i] = static_cast<float>(material.pbrMetallicRoughness.baseColorFactor[i]);
    }
    if (m_baseColorFactor[3] < 1.f)
    {
        int a = 0;
    }
}
#ifdef DEBUG
extern std::string g_strVSForDebug;
extern std::string g_strFSForDebug;
#endif
void  cMaterial::SetShaderProgramID(unsigned int e_uiShaderProgramID)
{
    m_uiShaderProgrameID = e_uiShaderProgramID;
#ifdef DEBUG
    this->m_strFSShaderCode = g_strFSForDebug;
#endif
}


void cMaterial::Apply()
{
    if (m_bDoubleSize)
    {
        glDisable(GL_CULL_FACE); // Disable culling for double-sided rendering
    }
    else
    {
        glEnable(GL_CULL_FACE);  // Enable back-face culling
        glCullFace(GL_BACK);     // Cull back faces (default for glTF)
        glFrontFace(GL_CCW);     // glTF uses counter-clockwise winding
    }
    // Handle alpha mode
    if (m_TectureAndTexCoordinateIndex.m_strAlphaMode == "BLEND")
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
    }
    else if (m_TectureAndTexCoordinateIndex.m_strAlphaMode == "MASK")
    {
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    }
    else // OPAQUE
    {
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    }
    GLuint l_TextureUnit = 0;
    BindTecture(m_spBaseColorTexture, l_TextureUnit,"uTextureDiffuse");
    BindTecture(m_spNormalTexture, l_TextureUnit,"uTextureNormal");
    BindTecture(m_spOocclusionTexture, l_TextureUnit,"uTextureOcclusion");
    BindTecture(m_spMetallicRoughnessTexture, l_TextureUnit,"uTextureMetallicRoughness");
    BindTecture(m_spEmissiveTexture, l_TextureUnit,"uTextureEmissive");
    // KHR_materials_specular
    BindTecture(m_spSpecularTexture, l_TextureUnit,"uTextureSpecular");
    BindTecture(m_spSpecularColorTexture, l_TextureUnit,"uTextureSpecularColor");
    ApplyUnriforms();
}


bool cMaterial::ApplyUnriforms()
{
    bool success = true;

    // Set base color factor
    GLuint baseColorFactorLoc = glGetUniformLocation(m_uiShaderProgrameID, "uBaseColorFactor");
    if (baseColorFactorLoc != GL_INVALID_INDEX)
    {
        glUniform4fv(baseColorFactorLoc, 1, m_baseColorFactor);
    }
    else
    {
        success = false;
    }
    GLuint alphaCutoffLoc = glGetUniformLocation(m_uiShaderProgrameID, "uAlphaCutoff");
    if (alphaCutoffLoc != GL_INVALID_INDEX)
    {
        glUniform1f(alphaCutoffLoc,  this->m_TectureAndTexCoordinateIndex.m_fAlphaCutoff);
    }
    // Set metallic factor
    GLuint metallicFactorLoc = glGetUniformLocation(m_uiShaderProgrameID, "uMetallicFactor");
    if (metallicFactorLoc != GL_INVALID_INDEX)
    {
        glUniform1f(metallicFactorLoc, m_metallicFactor);
    }
    else
    {
        success = false;
    }

    // Set roughness factor
    GLuint roughnessFactorLoc = glGetUniformLocation(m_uiShaderProgrameID, "uRoughnessFactor");
    if (roughnessFactorLoc != GL_INVALID_INDEX)
    {
        glUniform1f(roughnessFactorLoc, m_roughnessFactor);
    }
    else
    {
        success = false;
    }

    // Set occlusion strength
    GLuint occlusionStrengthLoc = glGetUniformLocation(m_uiShaderProgrameID, "uOcclusionStrength");
    if (occlusionStrengthLoc != GL_INVALID_INDEX)
    {
        glUniform1f(occlusionStrengthLoc, m_occlusionStrength);
    }
    else
    {
        success = false;
    }

    // Set emissive factor
    GLuint emissiveFactorLoc = glGetUniformLocation(m_uiShaderProgrameID, "uEmissiveFactor");
    if (emissiveFactorLoc != GL_INVALID_INDEX)
    {
        glUniform3fv(emissiveFactorLoc, 1, m_emissiveFactor);
    }
    else
    {
        success = false;
    }

    // --- KHR_materials_specular ---
    GLuint specularFactorLoc = glGetUniformLocation(m_uiShaderProgrameID, "uSpecularFactor");
    if (specularFactorLoc != GL_INVALID_INDEX)
        glUniform1f(specularFactorLoc, m_specularFactor);
    else success = false;

    GLuint specularColorFactorLoc = glGetUniformLocation(m_uiShaderProgrameID, "uSpecularColorFactor");
    if (specularColorFactorLoc != GL_INVALID_INDEX)
        glUniform3fv(specularColorFactorLoc, 1, m_specularColorFactor);
    else success = false;

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
    std::string l_strTextureName = this->GetCharName()+"_"+ e_Image.name;
    shared_ptr<cTexture>l_pTexture = cTextureManager::GetObjectByPixels((void*)e_Image.image.data(), e_Image.width, e_Image.height, ValueToStringW(l_strTextureName).c_str(), (int)l_uiFormat);
    l_pTexture->SetName(e_Image.name.c_str());
    g_uiMAG_FILTERValue = l_OriginalMAG;
    g_uiMIN_FILTERValue = l_OriginalMIN;
    g_uiTEXTURE_WRAP_S = l_OriginalWRAP_S;
    g_uiTEXTURE_WRAP_T = l_OriginalWRAP_T;
    l_pTexture->ApplyImage();
    glGenerateMipmap(GL_TEXTURE_2D);
    l_pTexture->Disable();
    return l_pTexture;
}
