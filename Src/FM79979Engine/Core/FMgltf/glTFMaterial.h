#pragma once

#include "tiny_gltf.h"

class cMaterial:public NamedTypedObject
{
	int64        m_i64TextureFVFFlag = 0;
    unsigned int m_uiShadeProgrameID;
	tinygltf::Material material;
     //m_pTexture;
    std::vector<shared_ptr<cTexture>> m_uiBaseColorTextureVector;
    std::vector<shared_ptr<cTexture>> m_uiNormalTextureVector;  // Normal maps
    std::vector<shared_ptr<cTexture>> m_uiOocclusionTextureVector;  // Occlusion maps
    std::vector<shared_ptr<cTexture>> m_uiEmissiveTextureIDVector; // Emissive maps (if needed)
    std::vector<shared_ptr<cTexture>> m_uiMetallicRoughnessTextureVector; // 
    //Metallic - roughness maps
    // PBR uniform factors
    float m_baseColorFactor[4];    // RGBA
    float m_metallicFactor;
    float m_roughnessFactor;
    float m_occlusionStrength;
    float m_emissiveFactor[3];     // RGB
	bool	ApplyUnriforms();
    shared_ptr<cTexture> GetTexture(const tinygltf::Image& e_Image,const tinygltf::Sampler*);
public:
    DEFINE_TYPE_INFO();
    cMaterial(unsigned int e_uiShadeProgrameID);
    virtual ~cMaterial();
    void    LoadMaterials(const tinygltf::Model& model, const tinygltf::Material& material);
	void	Apply();
};
//const tinygltf::Material& material
//m_pTexture = new cTexture(this,e_pPixelsData,e_iWidth,e_iHeight,e_strName,true,false,e_iDataFormat);