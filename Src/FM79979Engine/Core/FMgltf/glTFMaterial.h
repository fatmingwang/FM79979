#pragma once



class cMaterial:public NamedTypedObject
{
    unsigned int m_uiShadeProgrameID;
	tinygltf::Material material;
     //m_pTexture;
    std::vector<cTexture*> m_uiBaseColorTextureVector;
    std::vector<cTexture*> m_uiNormalTextureVector;  // Normal maps
    std::vector<cTexture*> m_uiOocclusionTextureVector;  // Occlusion maps
    std::vector<cTexture*> m_uiEmissiveTextureIDVector; // Emissive maps (if needed)
	bool	ApplyUnriforms();
    cTexture* GetTexture(const tinygltf::Image& e_Image);
public:
    cMaterial(unsigned int e_uiShadeProgrameID);
    virtual ~cMaterial();
    void    LoadMaterials(const tinygltf::Model& model, const tinygltf::Material& material);
	void	Apply();
};
//const tinygltf::Material& material
//m_pTexture = new cTexture(this,e_pPixelsData,e_iWidth,e_iHeight,e_strName,true,false,e_iDataFormat);