#ifndef COLLADA_MATERIALS_H
#define COLLADA_MATERIALS_H

struct	sMaterial
{
	const WCHAR*strID;
	const WCHAR*strName;
	const WCHAR*strInstanceEffectName;
};

enum eLMList
{
	eLM_material = 0,
	eLM_material_instance_effect,
	eLM_MAX
};
static const WCHAR*g_scLibraryMaterialsElement[eLM_MAX] =
{
	L"material",
	L"instance_effect",
};
class cLibraryMaterials :public NamedTypedObject
{
//
	void	Parsematerial(TiXmlElement*e_pTiXmlElement);
//
	void	Parsematerial_instance_effect(TiXmlElement*e_pTiXmlElement);
	sMaterial*	m_pCurrentMaterial;
public:
	DEFINE_TYPE_INFO()
	cLibraryMaterials (){}
	~cLibraryMaterials ()
	{
		DELETE_VECTOR(AllMaterial,sMaterial*);
	}
	void	ParseData(TiXmlElement*e_pTiXmlElement);
	std::vector<sMaterial*>	AllMaterial;
	sMaterial*				GetMaterialByID(const WCHAR*e_strName);
};


#endif