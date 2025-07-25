#pragma once

#include "tiny_gltf.h"

struct sTectureAndTexCoordinateIndex
{
    int m_iBaseColorTextureCoordinateIndex = 0;
    int m_iNormalTextureCoordinateIndex = 0;
    int m_iOocclusionTextureCoordinateIndex = 0;
    int m_iEmissiveTextureCoordinateIndex = 0;
    int m_iMetallicRoughnessTextureCoordinateIndex = 0;
    int m_iSpecularTextureCoordinateIndex = -1; // KHR_materials_specular
    int m_iSpecularColorTextureCoordinateIndex = -1; // KHR_materials_specular
    bool m_bUsePBR = false;
    bool m_bUseSpecular = false;
    std::string m_strAlphaMode = "OPAQUE";
    float m_fAlphaCutoff = 0.5f;
};

struct KHRSpecularExtension {
    float specularFactor = 1.0f;
    shared_ptr<cTexture> specularTexture;
    float specularColorFactor[3] = {1.0f, 1.0f, 1.0f};
    shared_ptr<cTexture> specularColorTexture;
};
struct KHRTransmissionExtension {
    float transmissionFactor = 0.0f;
    shared_ptr<cTexture> transmissionTexture;
};
struct KHRIorExtension {
    float ior = 1.5f;
};
struct KHRVolumeExtension {
    float thicknessFactor = 0.0f;
    float attenuationDistance = 0.0f;
    float attenuationColor[3] = {1.0f, 1.0f, 1.0f};
};

class cMaterial:public NamedTypedObject
{
	int64        m_i64TextureFVFFlag = 0;
    unsigned int m_uiShaderProgrameID = -1;
	tinygltf::Material material;
	bool m_bDoubleSize = false;
     //m_pTexture;
    shared_ptr<cTexture>    m_spBaseColorTexture;
    shared_ptr<cTexture>    m_spNormalTexture;  // Normal maps
    shared_ptr<cTexture>    m_spOocclusionTexture;  // Occlusion maps
    shared_ptr<cTexture>    m_spEmissiveTexture; // Emissive maps (if needed)
    shared_ptr<cTexture>    m_spMetallicRoughnessTexture; // 
    shared_ptr<cTexture>    m_spSpecularTexture; // KHR_materials_specular
    shared_ptr<cTexture>    m_spSpecularColorTexture; // KHR_materials_specular
    // Transmission, IOR, Volume extensions
    float m_transmissionFactor = 0.0f;
    shared_ptr<cTexture> m_spTransmissionTexture;
    float m_ior = 1.5f;
    float m_thicknessFactor = 0.0f;
    float m_attenuationDistance = 0.0f;
    float m_attenuationColor[3] = {1.0f, 1.0f, 1.0f};
    //Metallic - roughness maps
    // PBR uniform factors
    float m_baseColorFactor[4];    // RGBA
    float m_metallicFactor;
    float m_roughnessFactor;
    float m_occlusionStrength;
    float m_emissiveFactor[3];     // RGB
    float m_specularFactor = 1.0f; // KHR_materials_specular
    float m_specularColorFactor[3] = {1.0f, 1.0f, 1.0f}; // KHR_materials_specular
    sTectureAndTexCoordinateIndex   m_TectureAndTexCoordinateIndex;
	bool	ApplyUnriforms();
    shared_ptr<cTexture> GetTexture(const tinygltf::Image& e_Image,const tinygltf::Sampler*);
#ifdef DEBUG
    std::string m_strFSShaderCode;
#endif
    bool BindTecture(shared_ptr<cTexture> e_spTexture, GLuint &e_uiTextureUnit, const char* e_strTextureName);
public:
    DEFINE_TYPE_INFO();
    cMaterial();
    virtual ~cMaterial();
    void    LoadMaterials(const tinygltf::Model& model, const tinygltf::Material& material);
	void	Apply();
    void    SetShaderProgramID(unsigned int e_uiShaderProgramID);
    int64   GetTextureFVFFlag(){return m_i64TextureFVFFlag;}
    sTectureAndTexCoordinateIndex*   GetTectureAndTexCoordinateIndex()
    {
        return &m_TectureAndTexCoordinateIndex;
    }
    std::unique_ptr<KHRSpecularExtension> m_pSpecularExtension;
    std::unique_ptr<KHRTransmissionExtension> m_pTransmissionExtension;
    std::unique_ptr<KHRIorExtension> m_pIORExtension;
    std::unique_ptr<KHRVolumeExtension> m_pVolumeExtension;
};