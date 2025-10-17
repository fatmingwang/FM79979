#include "../AllCoreInclude.h"
#include "glTFMaterial.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include "../../imgui/imgui.h"
#include "ShadowMap.h"
#include "glTFLight.h"

namespace FATMING_CORE
{
    extern GLuint	g_uiMAG_FILTERValue;
    extern GLuint	g_uiMIN_FILTERValue;
    extern GLuint	g_uiTEXTURE_WRAP_S;
    extern GLuint	g_uiTEXTURE_WRAP_T;
}

TYPDE_DEFINE_MARCO(cMaterial);

#define SET_UNIFORM(PROGRAM, NAME, GLFUNC, VALUE) \
    success &= SetUniform(PROGRAM, NAME, GLFUNC, VALUE);


namespace
{
    // For float vector uniforms (e.g., glUniform4fv, glUniform3fv, glUniform2fv)
    template<typename T>
    bool SetUniform(GLuint program, const char* name, void(*glFunc)(GLint, GLsizei, const GLfloat*), const T& value, GLsizei count = 1)
    {
        GLint loc = glGetUniformLocation(program, name);
        if (loc != -1)
        {
            glFunc(loc, count, reinterpret_cast<const GLfloat*>(&value));
            return true;
        }
        return false;
    }
    // For float scalar uniforms (e.g., glUniform1f)
    bool SetUniform(GLuint program, const char* name, void(*glFunc)(GLint, GLfloat), GLfloat value)
    {
        GLint loc = glGetUniformLocation(program, name);
        if (loc != -1)
        {
            glFunc(loc, value);
            return true;
        }
        return false;
    }
    // For int scalar uniforms (e.g., glUniform1i)
    bool SetUniform(GLuint program, const char* name, void(*glFunc)(GLint, GLint), GLint value)
    {
        GLint loc = glGetUniformLocation(program, name);
        if (loc != -1)
        {
            glFunc(loc, value);
            return true;
        }
        return false;
    }
}

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
    m_vBaseColorFactor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    m_metallicFactor = 1.0f;
    m_roughnessFactor = 1.0f;
    m_occlusionStrength = 1.0f;
    m_vEmissiveFactor = Vector3(0.0f, 0.0f, 0.0f);
    m_specularFactor = 1.0f;
    m_spSpecularTexture = nullptr;
    m_spSpecularColorTexture = nullptr;
    m_vSpecularColorFactor = Vector3(1.0f, 1.0f, 1.0f);
    m_vAttenuationColor = Vector3(1.0f, 1.0f, 1.0f);
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
    m_pSpecularExtension = nullptr;
    if (material.extensions.count("KHR_materials_specular")) {
        m_pSpecularExtension = std::make_unique<KHRSpecularExtension>();
        const auto& ext = material.extensions.at("KHR_materials_specular");
        if (ext.Has("specularFactor"))
            m_pSpecularExtension->specularFactor = (float)ext.Get("specularFactor").GetNumberAsDouble();
        if (ext.Has("specularTexture")) {
            const auto& specTex = ext.Get("specularTexture");
            int texIndex = specTex.Get("index").GetNumberAsInt();
            int texCoord = 0;
            if (specTex.Has("texCoord"))
                texCoord = specTex.Get("texCoord").GetNumberAsInt();
            m_TectureAndTexCoordinateIndex.m_iSpecularTextureCoordinateIndex = texCoord;
            if (texIndex >= 0 && texIndex < (int)model.textures.size()) {
                const tinygltf::Texture& texture = model.textures[texIndex];
                tinygltf::Image image = model.images[texture.source];
                const tinygltf::Sampler* l_pSampler = nullptr;
                if (texture.sampler != -1)
                    l_pSampler = &model.samplers[texture.sampler];
                if (image.name.empty())
                    image.name = material.name + "_SpecularTexture_" + std::to_string(texIndex);
                m_pSpecularExtension->specularTexture = GetTexture(image, l_pSampler);
                m_TectureAndTexCoordinateIndex.m_bUseSpecular = true;
            }
        }
        if (ext.Has("specularColorFactor")) {
            const auto& arr = ext.Get("specularColorFactor");
            for (int i = 0; i < 3; ++i)
            {
                m_pSpecularExtension->specularColorFactor[i] = (float)arr.Get(i).GetNumberAsDouble();
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
                m_pSpecularExtension->specularColorTexture = GetTexture(image, l_pSampler);
            }
        }
    }
    // --- KHR_materials_transmission ---
    m_pTransmissionExtension = nullptr;
    if (material.extensions.count("KHR_materials_transmission")) {
        m_pTransmissionExtension = std::make_unique<KHRTransmissionExtension>();
        const auto& ext = material.extensions.at("KHR_materials_transmission");
        if (ext.Has("transmissionFactor"))
            m_pTransmissionExtension->transmissionFactor = (float)ext.Get("transmissionFactor").GetNumberAsDouble();
        if (ext.Has("transmissionTexture")) {
            const auto& transTex = ext.Get("transmissionTexture");
            int texIndex = transTex.Get("index").GetNumberAsInt();
            int texCoord = 0;
            if (transTex.Has("texCoord"))
                texCoord = transTex.Get("texCoord").GetNumberAsInt();
            if (texIndex >= 0 && texIndex < (int)model.textures.size()) {
                const tinygltf::Texture& texture = model.textures[texIndex];
                tinygltf::Image image = model.images[texture.source];
                const tinygltf::Sampler* l_pSampler = nullptr;
                if (texture.sampler != -1)
                    l_pSampler = &model.samplers[texture.sampler];
                if (image.name.empty())
                    image.name = material.name + "_TransmissionTexture_" + std::to_string(texIndex);
                m_pTransmissionExtension->transmissionTexture = GetTexture(image, l_pSampler);
            }
        }
    }
    // --- KHR_materials_ior ---
    m_pIORExtension = nullptr;
    if (material.extensions.count("KHR_materials_ior")) {
        m_pIORExtension = std::make_unique<KHRIorExtension>();
        const auto& ext = material.extensions.at("KHR_materials_ior");
        if (ext.Has("ior"))
            m_pIORExtension->ior = (float)ext.Get("ior").GetNumberAsDouble();
    }
    // --- KHR_materials_volume ---
    m_pVolumeExtension = nullptr;
    if (material.extensions.count("KHR_materials_volume")) {
        m_pVolumeExtension = std::make_unique<KHRVolumeExtension>();
        const auto& ext = material.extensions.at("KHR_materials_volume");
        if (ext.Has("thicknessFactor"))
            m_pVolumeExtension->thicknessFactor = (float)ext.Get("thicknessFactor").GetNumberAsDouble();
        if (ext.Has("attenuationDistance"))
            m_pVolumeExtension->attenuationDistance = (float)ext.Get("attenuationDistance").GetNumberAsDouble();
        if (ext.Has("attenuationColor")) {
            const auto& arr = ext.Get("attenuationColor");
            for (int i = 0; i < 3; ++i)
                m_pVolumeExtension->attenuationColor[i] = (float)arr.Get(i).GetNumberAsDouble();
        }
    }
    // --- KHR_materials_clearcoat ---
    m_pClearcoatExtension = nullptr;
    if (material.extensions.count("KHR_materials_clearcoat")) {
        m_pClearcoatExtension = std::make_unique<KHRClearcoatExtension>();
        const auto& ext = material.extensions.at("KHR_materials_clearcoat");
        if (ext.Has("clearcoatFactor"))
            m_pClearcoatExtension->clearcoatFactor = (float)ext.Get("clearcoatFactor").GetNumberAsDouble();
        if (ext.Has("clearcoatTexture")) {
            const auto& ccTex = ext.Get("clearcoatTexture");
            int texIndex = ccTex.Get("index").GetNumberAsInt();
            int texCoord = 0;
            if (ccTex.Has("texCoord"))
                texCoord = ccTex.Get("texCoord").GetNumberAsInt();
            if (texIndex >= 0 && texIndex < (int)model.textures.size()) {
                const tinygltf::Texture& texture = model.textures[texIndex];
                tinygltf::Image image = model.images[texture.source];
                const tinygltf::Sampler* l_pSampler = nullptr;
                if (texture.sampler != -1)
                    l_pSampler = &model.samplers[texture.sampler];
                if (image.name.empty())
                    image.name = material.name + "_ClearcoatTexture_" + std::to_string(texIndex);
                m_pClearcoatExtension->clearcoatTexture = GetTexture(image, l_pSampler);
            }
        }
        if (ext.Has("clearcoatRoughnessFactor"))
            m_pClearcoatExtension->clearcoatRoughnessFactor = (float)ext.Get("clearcoatRoughnessFactor").GetNumberAsDouble();
        if (ext.Has("clearcoatRoughnessTexture")) {
            const auto& ccRoughTex = ext.Get("clearcoatRoughnessTexture");
            int texIndex = ccRoughTex.Get("index").GetNumberAsInt();
            int texCoord = 0;
            if (ccRoughTex.Has("texCoord"))
                texCoord = ccRoughTex.Get("texCoord").GetNumberAsInt();
            if (texIndex >= 0 && texIndex < (int)model.textures.size()) {
                const tinygltf::Texture& texture = model.textures[texIndex];
                tinygltf::Image image = model.images[texture.source];
                const tinygltf::Sampler* l_pSampler = nullptr;
                if (texture.sampler != -1)
                    l_pSampler = &model.samplers[texture.sampler];
                if (image.name.empty())
                    image.name = material.name + "_ClearcoatRoughnessTexture_" + std::to_string(texIndex);
                m_pClearcoatExtension->clearcoatRoughnessTexture = GetTexture(image, l_pSampler);
            }
        }
    }
    // --- KHR_materials_emissive_strength ---
    m_pEmissiveStrengthExtension = nullptr;
    if (material.extensions.count("KHR_materials_emissive_strength")) {
        m_pEmissiveStrengthExtension = std::make_unique<KHREmissiveStrengthExtension>();
        const auto& ext = material.extensions.at("KHR_materials_emissive_strength");
        if (ext.Has("emissiveStrength"))
            m_pEmissiveStrengthExtension->emissiveStrength = (float)ext.Get("emissiveStrength").GetNumberAsDouble();
    }
    // --- KHR_materials_iridescence ---
    m_pIridescenceExtension = nullptr;
    if (material.extensions.count("KHR_materials_iridescence")) {
        m_pIridescenceExtension = std::make_unique<KHRIridescenceExtension>();
        const auto& ext = material.extensions.at("KHR_materials_iridescence");
        if (ext.Has("iridescenceFactor"))
            m_pIridescenceExtension->iridescenceFactor = (float)ext.Get("iridescenceFactor").GetNumberAsDouble();
        if (ext.Has("iridescenceTexture")) {
            const auto& irTex = ext.Get("iridescenceTexture");
            int texIndex = irTex.Get("index").GetNumberAsInt();
            int texCoord = 0;
            if (irTex.Has("texCoord"))
                texCoord = irTex.Get("texCoord").GetNumberAsInt();
            if (texIndex >= 0 && texIndex < (int)model.textures.size()) {
                const tinygltf::Texture& texture = model.textures[texIndex];
                tinygltf::Image image = model.images[texture.source];
                const tinygltf::Sampler* l_pSampler = nullptr;
                if (texture.sampler != -1)
                    l_pSampler = &model.samplers[texture.sampler];
                if (image.name.empty())
                    image.name = material.name + "_IridescenceTexture_" + std::to_string(texIndex);
                m_pIridescenceExtension->iridescenceTexture = GetTexture(image, l_pSampler);
            }
        }
        if (ext.Has("iridescenceIor"))
            m_pIridescenceExtension->iridescenceIor = (float)ext.Get("iridescenceIor").GetNumberAsDouble();
        if (ext.Has("iridescenceThicknessMinimum"))
            m_pIridescenceExtension->iridescenceThicknessMinimum = (float)ext.Get("iridescenceThicknessMinimum").GetNumberAsDouble();
        if (ext.Has("iridescenceThicknessMaximum"))
            m_pIridescenceExtension->iridescenceThicknessMaximum = (float)ext.Get("iridescenceThicknessMaximum").GetNumberAsDouble();
        if (ext.Has("iridescenceThicknessTexture")) {
            const auto& irThickTex = ext.Get("iridescenceThicknessTexture");
            int texIndex = irThickTex.Get("index").GetNumberAsInt();
            int texCoord = 0;
            if (irThickTex.Has("texCoord"))
                texCoord = irThickTex.Get("texCoord").GetNumberAsInt();
            if (texIndex >= 0 && texIndex < (int)model.textures.size()) {
                const tinygltf::Texture& texture = model.textures[texIndex];
                tinygltf::Image image = model.images[texture.source];
                const tinygltf::Sampler* l_pSampler = nullptr;
                if (texture.sampler != -1)
                    l_pSampler = &model.samplers[texture.sampler];
                if (image.name.empty())
                    image.name = material.name + "_IridescenceThicknessTexture_" + std::to_string(texIndex);
                m_pIridescenceExtension->iridescenceThicknessTexture = GetTexture(image, l_pSampler);
            }
        }
    }
    // --- KHR_materials_variants ---
    m_pVariantsExtension = nullptr;
    if (material.extensions.count("KHR_materials_variants")) {
        m_pVariantsExtension = std::make_unique<KHRVariantsExtension>();
        const auto& ext = material.extensions.at("KHR_materials_variants");
        if (ext.Has("variants")) {
            const auto& arr = ext.Get("variants");
            for (size_t i = 0; i < arr.ArrayLen(); ++i) {
                const auto& variant = arr.Get(i);
                if (variant.Has("name") && variant.Get("name").IsString()) {
                    m_pVariantsExtension->variantNames.push_back(variant.Get("name").Get<std::string>());
                } else {
                    m_pVariantsExtension->variantNames.push_back("");
                }
                if (variant.Has("material")) {
                    m_pVariantsExtension->materialIndices.push_back(variant.Get("material").GetNumberAsInt());
                } else {
                    m_pVariantsExtension->materialIndices.push_back(-1);
                }
            }
        }
    }
    // --- KHR_texture_transform ---
    m_pTextureTransformExtension = nullptr;
    if (material.extensions.count("KHR_texture_transform")) {
        m_pTextureTransformExtension = std::make_unique<KHRTextureTransformExtension>();
        const auto& ext = material.extensions.at("KHR_texture_transform");
        if (ext.Has("offset")) {
            const auto& arr = ext.Get("offset");
            m_pTextureTransformExtension->offset[0] = (float)arr.Get(0).GetNumberAsDouble();
            m_pTextureTransformExtension->offset[1] = (float)arr.Get(1).GetNumberAsDouble();
        }
        if (ext.Has("rotation"))
            m_pTextureTransformExtension->rotation = (float)ext.Get("rotation").GetNumberAsDouble();
        if (ext.Has("scale")) {
            const auto& arr = ext.Get("scale");
            m_pTextureTransformExtension->scale[0] = (float)arr.Get(0).GetNumberAsDouble();
            m_pTextureTransformExtension->scale[1] = (float)arr.Get(1).GetNumberAsDouble();
        }
        if (ext.Has("texCoord"))
            m_pTextureTransformExtension->texCoord = ext.Get("texCoord").GetNumberAsInt();
    }

    for (size_t i = 0; i < 3; ++i)
    {
        m_vEmissiveFactor[i] = static_cast<float>(material.emissiveFactor[i]);
    }
    m_metallicFactor = static_cast<float>(material.pbrMetallicRoughness.metallicFactor);
    m_roughnessFactor = static_cast<float>(material.pbrMetallicRoughness.roughnessFactor);
    m_occlusionStrength = static_cast<float>(material.occlusionTexture.strength);
    for (size_t i = 0; i < 4; ++i)
    {
        m_vBaseColorFactor[i] = static_cast<float>(material.pbrMetallicRoughness.baseColorFactor[i]);
    }
    if (m_vBaseColorFactor[3] < 1.f)
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

void cMaterial::RenderImGUI()
{
#ifdef DEBUG
    std::string l_strName = std::string("Material_") + this->GetCharName();
	std::string l_UniqueID = std::to_string(this->GetUniqueID());
    ImGui::PushID(l_UniqueID.c_str());
    if (ImGui::TreeNode(l_strName.c_str()))
    {
        ImGui::PushID(this->GetUniqueID());        
        ImGui::Text("Shader Program ID: %u", m_uiShaderProgrameID);
        ImGui::Checkbox("Double Sided", &m_bDoubleSize);
        ImGui::InputFloat4("Base Color Factor", m_vBaseColorFactor);
        ImGui::InputFloat("Alpha Cutoff", &m_TectureAndTexCoordinateIndex.m_fAlphaCutoff);
        ImGui::InputFloat("Metallic Factor", &m_metallicFactor);
        ImGui::InputFloat("Roughness Factor", &m_roughnessFactor);
        ImGui::InputFloat("Occlusion Strength", &m_occlusionStrength);
        ImGui::InputFloat3("Emissive Factor", m_vEmissiveFactor);
        if (m_pSpecularExtension) {
            ImGui::PushID("KHR_specular");
            if (ImGui::TreeNode("KHR_specular")) {
                ImGui::InputFloat("Specular Factor", &m_pSpecularExtension->specularFactor);
                ImGui::InputFloat3("Specular Color Factor", m_pSpecularExtension->specularColorFactor);
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        if (m_pTransmissionExtension) {
            ImGui::PushID("KHR_transmission");
            if (ImGui::TreeNode("KHR_transmission")) {
                ImGui::InputFloat("Transmission Factor", &m_pTransmissionExtension->transmissionFactor);
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        if (m_pIORExtension) {
            ImGui::PushID("KHR_ior");
            if (ImGui::TreeNode("KHR_ior")) {
                ImGui::InputFloat("IOR", &m_pIORExtension->ior);
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        if (m_pVolumeExtension) {
            ImGui::PushID("KHR_volume");
            if (ImGui::TreeNode("KHR_volume")) {
                ImGui::InputFloat("Thickness Factor", &m_pVolumeExtension->thicknessFactor);
                ImGui::InputFloat("Attenuation Distance", &m_pVolumeExtension->attenuationDistance);
                ImGui::InputFloat3("Attenuation Color", m_pVolumeExtension->attenuationColor);
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        if (m_pClearcoatExtension) {
            ImGui::PushID("KHR_clearcoat");
            if (ImGui::TreeNode("KHR_clearcoat")) {
                ImGui::InputFloat("Clearcoat Factor", &m_pClearcoatExtension->clearcoatFactor);
                ImGui::InputFloat("Clearcoat Roughness Factor", &m_pClearcoatExtension->clearcoatRoughnessFactor);
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        if (m_pEmissiveStrengthExtension) {
            ImGui::PushID("KHR_emissive_strength");
            if (ImGui::TreeNode("KHR_emissive_strength")) {
                ImGui::InputFloat("Emissive Strength", &m_pEmissiveStrengthExtension->emissiveStrength);
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        if (m_pIridescenceExtension) {
            ImGui::PushID("KHR_iridescence");
            if (ImGui::TreeNode("KHR_iridescence")) {
                ImGui::InputFloat("Iridescence Factor", &m_pIridescenceExtension->iridescenceFactor);
                ImGui::InputFloat("Iridescence IOR", &m_pIridescenceExtension->iridescenceIor);
                ImGui::InputFloat("Iridescence Thickness Min", &m_pIridescenceExtension->iridescenceThicknessMinimum);
                ImGui::InputFloat("Iridescence Thickness Max", &m_pIridescenceExtension->iridescenceThicknessMaximum);
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        if (m_pTextureTransformExtension) {
            ImGui::PushID("KHR_texture_transform");
            if (ImGui::TreeNode("KHR_texture_transform")) {
                ImGui::InputFloat2("Offset", m_pTextureTransformExtension->offset);
                ImGui::InputFloat("Rotation", &m_pTextureTransformExtension->rotation);
                ImGui::InputFloat2("Scale", m_pTextureTransformExtension->scale);
                ImGui::InputInt("TexCoord", &m_pTextureTransformExtension->texCoord);
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        ImGui::PopID();
        ImGui::TreePop();
    }
    ImGui::PopID();
#endif
}

void cMaterial::Apply()
{
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
    BindTecture(m_spTransmissionTexture, l_TextureUnit, "uTextureTransmission");
    // --- KHR_materials_clearcoat ---
    if (m_pClearcoatExtension) {
        BindTecture(m_pClearcoatExtension->clearcoatTexture, l_TextureUnit, "uTextureClearcoat");
        BindTecture(m_pClearcoatExtension->clearcoatRoughnessTexture, l_TextureUnit, "uTextureClearcoatRoughness");
    }
    // --- KHR_materials_iridescence ---
    if (m_pIridescenceExtension) {
        BindTecture(m_pIridescenceExtension->iridescenceTexture, l_TextureUnit, "uTextureIridescence");
        BindTecture(m_pIridescenceExtension->iridescenceThicknessTexture, l_TextureUnit, "uTextureIridescenceThickness");
    }
    // --- Shadow Map Setup ---
    const int SHADOW_MAP_TEXTURE_UNIT = l_TextureUnit;
    if (g_pShadowMap) {
        g_pShadowMap->BindForReading(GL_TEXTURE0 + SHADOW_MAP_TEXTURE_UNIT);
        GLuint l_ShadowMap = glGetUniformLocation(m_uiShaderProgrameID, "uShadowMap");
        if (l_ShadowMap != GL_INVALID_INDEX)
        {
            glUniform1i(l_ShadowMap, SHADOW_MAP_TEXTURE_UNIT);
        }
        auto l_EnableShadow = glGetUniformLocation(m_uiShaderProgrameID, "uEnableShadow");
        if (l_EnableShadow != GL_INVALID_INDEX)
        {
            glUniform1i(l_EnableShadow, 1);
        }
        cMatrix44 lightViewProj;
        auto l_Light = cLighController::GetInstance()->GetFirstDirectionLight();
        if (cLighController::GetInstance()->GetDirectionViewProjectionMatrix(l_Light, lightViewProj)) {
            glUniformMatrix4fv(glGetUniformLocation(m_uiShaderProgrameID, "uLightViewProj"), 1, GL_FALSE, lightViewProj.m[0]);
        }
    } else {
        glUniform1i(glGetUniformLocation(m_uiShaderProgrameID, "uEnableShadow"), 0);
    }
    ApplyUnriforms();
}

bool cMaterial::ApplyUnriforms()
{
    bool success = true;
    auto prog = m_uiShaderProgrameID;
    SET_UNIFORM(prog, "uBaseColorFactor", glUniform4fv,m_vBaseColorFactor);
    SET_UNIFORM(prog, "uAlphaCutoff", glUniform1f, m_TectureAndTexCoordinateIndex.m_fAlphaCutoff);
    SET_UNIFORM(prog, "uMetallicFactor", glUniform1f, m_metallicFactor);
    SET_UNIFORM(prog, "uRoughnessFactor", glUniform1f, m_roughnessFactor);
    SET_UNIFORM(prog, "uOcclusionStrength", glUniform1f, m_occlusionStrength);
    SET_UNIFORM(prog, "uEmissiveFactor", glUniform3fv, m_vEmissiveFactor);
    // KHR_materials_specular
    if (m_pSpecularExtension) {
        SET_UNIFORM(prog, "uSpecularFactor", glUniform1f, m_pSpecularExtension->specularFactor);
        SET_UNIFORM(prog, "uSpecularColorFactor", glUniform3fv, m_pSpecularExtension->specularColorFactor);
    }
    // KHR_materials_transmission
    if (m_pTransmissionExtension) {
        SET_UNIFORM(prog, "uTransmissionFactor", glUniform1f, m_pTransmissionExtension->transmissionFactor);
    }
    // KHR_materials_ior
    if (m_pIORExtension) {
        SET_UNIFORM(prog, "uIOR", glUniform1f, m_pIORExtension->ior);
    }
    // KHR_materials_volume
    if (m_pVolumeExtension) {
        SET_UNIFORM(prog, "uThicknessFactor", glUniform1f, m_pVolumeExtension->thicknessFactor);
        SET_UNIFORM(prog, "uAttenuationDistance", glUniform1f, m_pVolumeExtension->attenuationDistance);
        SET_UNIFORM(prog, "uAttenuationColor", glUniform3fv, m_pVolumeExtension->attenuationColor);
    }
    // KHR_materials_clearcoat
    if (m_pClearcoatExtension) {
        SET_UNIFORM(prog, "uClearcoatFactor", glUniform1f, m_pClearcoatExtension->clearcoatFactor);
        SET_UNIFORM(prog, "uClearcoatRoughnessFactor", glUniform1f, m_pClearcoatExtension->clearcoatRoughnessFactor);
    }
    // KHR_materials_emissive_strength
    if (m_pEmissiveStrengthExtension) {
        SET_UNIFORM(prog, "uEmissiveStrength", glUniform1f, m_pEmissiveStrengthExtension->emissiveStrength);
    }
    // KHR_materials_iridescence
    if (m_pIridescenceExtension) {
        SET_UNIFORM(prog, "uIridescenceFactor", glUniform1f, m_pIridescenceExtension->iridescenceFactor);
        SET_UNIFORM(prog, "uIridescenceIor", glUniform1f, m_pIridescenceExtension->iridescenceIor);
        SET_UNIFORM(prog, "uIridescenceThicknessMinimum", glUniform1f, m_pIridescenceExtension->iridescenceThicknessMinimum);
        SET_UNIFORM(prog, "uIridescenceThicknessMaximum", glUniform1f, m_pIridescenceExtension->iridescenceThicknessMaximum);
    }
    // KHR_texture_transform
    if (m_pTextureTransformExtension) {
        SET_UNIFORM(prog, "uTextureTransformOffset", glUniform2fv, m_pTextureTransformExtension->offset);
        SET_UNIFORM(prog, "uTextureTransformRotation", glUniform1f, m_pTextureTransformExtension->rotation);
        SET_UNIFORM(prog, "uTextureTransformScale", glUniform2fv, m_pTextureTransformExtension->scale);
        SET_UNIFORM(prog, "uTextureTransformTexCoord", glUniform1i, m_pTextureTransformExtension->texCoord);
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


#ifdef DEBUG
#include "../../imgui/imgui.h"
using namespace ImGui;
#endif

