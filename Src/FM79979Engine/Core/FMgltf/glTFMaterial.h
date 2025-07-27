#pragma once
#include <string>
#include <vector>
#include <memory>
#include "tiny_gltf.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"

struct sTectureAndTexCoordinateIndex {
    int m_iBaseColorTextureCoordinateIndex = 0;
    int m_iNormalTextureCoordinateIndex = 0;
    int m_iOocclusionTextureCoordinateIndex = 0;
    int m_iEmissiveTextureCoordinateIndex = 0;
    int m_iMetallicRoughnessTextureCoordinateIndex = 0;
    int m_iSpecularTextureCoordinateIndex = -1;
    int m_iSpecularColorTextureCoordinateIndex = -1;
    bool m_bUsePBR = false;
    bool m_bUseSpecular = false;
    std::string m_strAlphaMode = "OPAQUE";
    float m_fAlphaCutoff = 0.5f;
};

struct KHRSpecularExtension {
    float specularFactor = 1.0f;
    std::shared_ptr<cTexture> specularTexture;
    Vector3 specularColorFactor{1.0f, 1.0f, 1.0f};
    std::shared_ptr<cTexture> specularColorTexture;
};
struct KHRTransmissionExtension {
    float transmissionFactor = 0.0f;
    std::shared_ptr<cTexture> transmissionTexture;
};
struct KHRIorExtension {
    float ior = 1.5f;
};
struct KHRVolumeExtension {
    float thicknessFactor = 0.0f;
    float attenuationDistance = 0.0f;
    Vector3 attenuationColor{1.0f, 1.0f, 1.0f};
};
struct KHRClearcoatExtension {
    float clearcoatFactor = 0.0f;
    std::shared_ptr<cTexture> clearcoatTexture;
    float clearcoatRoughnessFactor = 0.0f;
    std::shared_ptr<cTexture> clearcoatRoughnessTexture;
};
struct KHREmissiveStrengthExtension {
    float emissiveStrength = 1.0f;
};
struct KHRIridescenceExtension {
    float iridescenceFactor = 0.0f;
    std::shared_ptr<cTexture> iridescenceTexture;
    float iridescenceIor = 1.3f;
    float iridescenceThicknessMinimum = 100.0f;
    float iridescenceThicknessMaximum = 400.0f;
    std::shared_ptr<cTexture> iridescenceThicknessTexture;
};
struct KHRVariantsExtension {
    std::vector<std::string> variantNames;
    std::vector<int> materialIndices;
};
struct KHRTextureTransformExtension {
    Vector2 offset{0.0f, 0.0f};
    float rotation = 0.0f;
    Vector2 scale{1.0f, 1.0f};
    int texCoord = 0;
};

class cMaterial : public NamedTypedObject {
    int64_t m_i64TextureFVFFlag = 0;
    unsigned int m_uiShaderProgrameID = -1;
    tinygltf::Material material;
    bool m_bDoubleSize = false;
    std::shared_ptr<cTexture> m_spBaseColorTexture;
    std::shared_ptr<cTexture> m_spNormalTexture;
    std::shared_ptr<cTexture> m_spOocclusionTexture;
    std::shared_ptr<cTexture> m_spEmissiveTexture;
    std::shared_ptr<cTexture> m_spMetallicRoughnessTexture;
    std::shared_ptr<cTexture> m_spSpecularTexture;
    std::shared_ptr<cTexture> m_spSpecularColorTexture;
    float m_transmissionFactor = 0.0f;
    std::shared_ptr<cTexture> m_spTransmissionTexture;
    float m_ior = 1.5f;
    float m_thicknessFactor = 0.0f;
    float m_attenuationDistance = 0.0f;
    Vector3 m_vAttenuationColor{1.0f, 1.0f, 1.0f};
    Vector4 m_vBaseColorFactor{0.0f, 0.0f, 0.0f, 0.0f};
    float m_metallicFactor = 0.0f;
    float m_roughnessFactor = 0.0f;
    float m_occlusionStrength = 1.0f;
    Vector3 m_vEmissiveFactor{0.0f, 0.0f, 0.0f};
    float m_specularFactor = 1.0f;
    Vector3 m_vSpecularColorFactor{1.0f, 1.0f, 1.0f};
    sTectureAndTexCoordinateIndex m_TectureAndTexCoordinateIndex;
    bool ApplyUnriforms();
    std::shared_ptr<cTexture> GetTexture(const tinygltf::Image& e_Image, const tinygltf::Sampler*);
#ifdef DEBUG
    std::string m_strFSShaderCode;
#endif
    bool BindTecture(std::shared_ptr<cTexture> e_spTexture, GLuint& e_uiTextureUnit, const char* e_strTextureName);
public:
    DEFINE_TYPE_INFO();
    cMaterial();
    virtual ~cMaterial();
    void LoadMaterials(const tinygltf::Model& model, const tinygltf::Material& material);
    void Apply();
    void SetShaderProgramID(unsigned int e_uiShaderProgramID);
    int64_t GetTextureFVFFlag() { return m_i64TextureFVFFlag; }
    sTectureAndTexCoordinateIndex* GetTectureAndTexCoordinateIndex() { return &m_TectureAndTexCoordinateIndex; }
    std::unique_ptr<KHRSpecularExtension> m_pSpecularExtension;
    std::unique_ptr<KHRTransmissionExtension> m_pTransmissionExtension;
    std::unique_ptr<KHRIorExtension> m_pIORExtension;
    std::unique_ptr<KHRVolumeExtension> m_pVolumeExtension;
    std::unique_ptr<KHRClearcoatExtension> m_pClearcoatExtension;
    std::unique_ptr<KHREmissiveStrengthExtension> m_pEmissiveStrengthExtension;
    std::unique_ptr<KHRIridescenceExtension> m_pIridescenceExtension;
    std::unique_ptr<KHRVariantsExtension> m_pVariantsExtension;
    std::unique_ptr<KHRTextureTransformExtension> m_pTextureTransformExtension;
};