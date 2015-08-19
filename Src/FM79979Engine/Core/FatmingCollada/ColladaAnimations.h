#ifndef COLLADA_ANIMATION_H
#define COLLADA_ANIMATION_H
#include "FatmingColladaAnimationData.h"
enum eLAList
{
	eLA_animation = 0,
	eLA_animation_source,
	eLA_animation_source_float_array,
	eLA_animation_source_technique_common,
	eLA_animation_source_technique_common_accessor,
	eLA_animation_source_technique_common_accessor_param,
	eLA_animation_source_technique,
	eLA_animation_source_technique_pre_infinity,
	eLA_animation_source_technique_post_infinity,
	eLA_animation_source_Name_array,
	eLA_animation_sampler,
	eLA_animation_sampler_input,
	eLA_animation_channel,
	eLA_MAX
};
static const WCHAR*g_scLibraryAnimationElement[eLA_MAX] =
{
	L"animation",
	L"source",
	L"float_array",
	L"technique_common",
	L"accessor",
	L"param",
	L"technique",
	L"pre_infinity",
	L"post_infinity",
	L"Name_array",
	L"sampler",
	L"input",
	L"channel",
};
class cLibraryAnimation :public NamedTypedObject
{
//
	void	Parseanimation(TiXmlElement*e_pTiXmlElement);
//
	void	Parseanimation_source(TiXmlElement*e_pTiXmlElement);
//
	void	Parseanimation_source_float_array(TiXmlElement*e_pTiXmlElement);
//
	void	Parseanimation_source_technique_common(TiXmlElement*e_pTiXmlElement);
//
	void	Parseanimation_source_technique_common_accessor(TiXmlElement*e_pTiXmlElement);
//
	void	Parseanimation_source_technique_common_accessor_param(TiXmlElement*e_pTiXmlElement);
//
	void	Parseanimation_source_technique(TiXmlElement*e_pTiXmlElement);
//
	void	Parseanimation_source_technique_pre_infinity(TiXmlElement*e_pTiXmlElement);
//
	void	Parseanimation_source_technique_post_infinity(TiXmlElement*e_pTiXmlElement);
//
	void	Parseanimation_source_Name_array(TiXmlElement*e_pTiXmlElement);
//
	void	Parseanimation_sampler(TiXmlElement*e_pTiXmlElement);
//
	void	Parseanimation_sampler_input(TiXmlElement*e_pTiXmlElement);
//
	void	Parseanimation_channel(TiXmlElement*e_pTiXmlElement);
	//internal using
	sAnimationLibrary::sAnimation*m_pCurrentAnimation;
	sAnimationLibrary::sAnimation::sSource*m_pCurrentSource;
	sAnimationLibrary::sAnimation::sSampler*m_pCurrentSampler;
	sAnimationLibrary::sAnimation::sChannel*m_pCurrentChannel;
public:
        //DEFINE_TYPE_INFO();
        void	ParseData(TiXmlElement*e_pTiXmlElement);
		sAnimationLibrary	m_AnimationLibrary;
		void	ClearData(){ m_AnimationLibrary.ClearData(); }
};

#endif