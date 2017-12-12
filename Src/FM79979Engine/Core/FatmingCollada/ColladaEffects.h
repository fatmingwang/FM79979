#ifndef COLLADA_EFFECTS_H
#define COLLADA_EFFECTS_H
#include "FatmingEffectData.h"
enum eLEList
{
	eLE_effect = 0,
	eLE_effect_profile_COMMON,
	eLE_effect_profile_COMMON_newparam,
	eLE_effect_profile_COMMON_newparam_surface,
	eLE_effect_profile_COMMON_newparam_surface_init_from,
	eLE_effect_profile_COMMON_newparam_surface_format,
	eLE_effect_profile_COMMON_newparam_sampler2D,
	eLE_effect_profile_COMMON_newparam_sampler2D_source,
	eLE_effect_profile_COMMON_newparam_sampler2D_wrap_s,
	eLE_effect_profile_COMMON_newparam_sampler2D_wrap_t,
	eLE_effect_profile_COMMON_newparam_sampler2D_minfilter,
	eLE_effect_profile_COMMON_newparam_sampler2D_magfilter,
	eLE_effect_profile_COMMON_newparam_sampler2D_mipfilter,
	eLE_effect_profile_COMMON_technique,//all technique
	eLE_effect_profile_COMMON_technique_float,
	eLE_effect_profile_COMMON_technique_color,
	eLE_effect_profile_COMMON_technique_texture,
	eLE_effect_profile_COMMON_technique_texture_extra,
	eLE_effect_profile_COMMON_technique_texture_extra_technique,
	eLE_effect_profile_COMMON_technique_texture_extra_technique_mirrorU,
	eLE_effect_profile_COMMON_technique_texture_extra_technique_mirrorV,
	eLE_effect_profile_COMMON_technique_texture_extra_technique_wrapU,
	eLE_effect_profile_COMMON_technique_texture_extra_technique_wrapV,
	eLE_effect_profile_COMMON_technique_texture_extra_technique_repeatU,
	eLE_effect_profile_COMMON_technique_texture_extra_technique_repeatV,
	eLE_effect_profile_COMMON_technique_texture_extra_technique_offsetU,
	eLE_effect_profile_COMMON_technique_texture_extra_technique_offsetV,
	eLE_effect_profile_COMMON_technique_texture_extra_technique_rotateUV,
	eLE_effect_profile_COMMON_technique_texture_extra_technique_amount,
	eLE_effect_profile_COMMON_technique_extra,
	eLE_effect_extra,
	eLE_effect_extra_technique,
	eLE_effect_extra_technique_faceted,
	eLE_effect_extra_technique_double_sided,
	eLE_effect_extra_technique_wireframe,
	eLE_effect_extra_technique_face_map,
	eLE_MAX
};
static const WCHAR*g_sLibraryEffectsElement[eLE_MAX] =
{
	L"effect",
	L"profile_COMMON",
	L"newparam",
	L"surface",
	L"init_from",
	L"format",
	L"sampler2D",
	L"source",
	L"wrap_s",
	L"wrap_t",
	L"minfilter",
	L"magfilter",
	L"mipfilter",
	L"technique",
	L"float",
	L"color",
	L"texture",
	L"extra",
	L"technique",
	L"mirrorU",
	L"mirrorV",
	L"wrapU",
	L"wrapV",
	L"repeatU",
	L"repeatV",
	L"offsetU",
	L"offsetV",
	L"rotateUV",
	L"amount",
	L"extra",
	L"extra",
	L"technique",
	L"faceted",
	L"double_sided",
	L"wireframe",
	L"face_map",
};
class cLibraryEffects :public NamedTypedObject
{
//
	void	Parseeffect(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_newparam(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_newparam_surface(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_newparam_surface_init_from(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_newparam_surface_format(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_newparam_sampler2D(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_newparam_sampler2D_source(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_newparam_sampler2D_wrap_s(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_newparam_sampler2D_wrap_t(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_newparam_sampler2D_minfilter(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_newparam_sampler2D_magfilter(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_newparam_sampler2D_mipfilter(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_technique(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_technique_type(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_technique_data_Assign(TiXmlElement*e_pTiXmlElement,const WCHAR*e_strShadingName);
//
	void	Parseeffect_profile_COMMON_technique_float(TiXmlElement*e_pTiXmlElement,eTechniqueType e_eTechniqueType);
//
	void	Parseeffect_profile_COMMON_technique_color(TiXmlElement*e_pTiXmlElement,eTechniqueType e_eTechniqueType);
//
	void	Parseeffect_profile_COMMON_technique_texture(TiXmlElement*e_pTiXmlElement,eTechniqueType e_eTechniqueType);
//
	void	Parseeffect_profile_COMMON_technique_texture_extra(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_technique_texture_extra_technique(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_technique_texture_extra_technique_mirrorU(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_technique_texture_extra_technique_mirrorV(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_technique_texture_extra_technique_wrapU(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_technique_texture_extra_technique_wrapV(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_technique_texture_extra_technique_repeatU(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_technique_texture_extra_technique_repeatV(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_technique_texture_extra_technique_offsetU(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_technique_texture_extra_technique_offsetV(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_technique_texture_extra_technique_rotateUV(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_technique_texture_extra_technique_amount(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_profile_COMMON_technique_extra(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_extra(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_extra_technique(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_extra_technique_faceted(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_extra_technique_double_sided(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_extra_technique_wireframe(TiXmlElement*e_pTiXmlElement);
//
	void	Parseeffect_extra_technique_face_map(TiXmlElement*e_pTiXmlElement);
	sEffectData*	m_pCurrentEffectData;
	sEffectData::sComonEffect::sNewParam*		m_pCurrentNewParam;
public:
	DEFINE_TYPE_INFO()
	cLibraryEffects()
	{
		m_pCurrentEffectData = 0;
		m_pCurrentNewParam = 0;
	}
	~cLibraryEffects()
	{
		DELETE_VECTOR(m_AllEffectData,sEffectData*);
	}
	void	ParseData(TiXmlElement*e_pTiXmlElement);
	std::vector<sEffectData*>	m_AllEffectData;
	sEffectData*				GetEffectByID(const WCHAR*e_strID);
};

#endif