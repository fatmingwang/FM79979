#pragma once

#include "tiny_gltf.h"

struct sTectureAndTexCoordinateIndex
{
    int m_iBaseColorTextureCoordinateIndex = 0;
    int m_iNormalTextureCoordinateIndex = 0;
    int m_iOocclusionTextureCoordinateIndex = 0;
    int m_iEmissiveTextureCoordinateIndex = 0;
    int m_iMetallicRoughnessTextureCoordinateIndex = 0;
    bool m_bUsePBR = false;
    std::string m_strAlphaMode = "OPAQUE";
    float m_fAlphaCutoff = 0.5f;
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
    //Metallic - roughness maps
    // PBR uniform factors
    float m_baseColorFactor[4];    // RGBA
    float m_metallicFactor;
    float m_roughnessFactor;
    float m_occlusionStrength;
    float m_emissiveFactor[3];     // RGB
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
};
//const tinygltf::Material& material
//m_pTexture = new cTexture(this,e_pPixelsData,e_iWidth,e_iHeight,e_strName,true,false,e_iDataFormat);