#include "StdAfx.h"
#include "ColladaEffects.h"
const wchar_t*           cLibraryEffects::TypeID( L"cLibraryEffects" );

eTechniqueType	GetTechniqueName(const WCHAR*e_stName)
{
	if( !wcscmp(e_stName,L"ambient") )
	{
		return eTL_AMBIENT;
	}
	else
	if( !wcscmp(e_stName,L"diffuse") )
	{
		return eTL_DIFFUSE;
	}
	else
	if( !wcscmp(e_stName,L"specular") )
	{
		return eTL_SPECULAR;
	}
	else
	if( !wcscmp(e_stName,L"bump") )
	{
		return eTL_BUMP;
	}
	else
	if( !wcscmp(e_stName,L"glow") )
	{
		return eTL_GLOW;
	}
	else
	if( !wcscmp(e_stName,L"transparency") )
	{
		return eTL_TRANSPARENT;
	}
	return eTL_NONE;
}


sEffectData*	cLibraryEffects::GetEffectByID(const WCHAR*e_strID)
{
	for(UINT i=0;i<m_AllEffectData.size();++i)
	{
		if(!wcscmp(m_AllEffectData[i]->m_strID,e_strID))
			return m_AllEffectData[i];
	}
	return 0;
}

void	cLibraryEffects::ParseData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		sEffectData*l_pEffectData = new sEffectData();
		m_AllEffectData.push_back(l_pEffectData);
		this->m_pCurrentEffectData = l_pEffectData;
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect]))
		{
			Parseeffect(l_pChildTiXmlElement);
		}
		else
		{
			//////assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}
}
//
//
//
void	cLibraryEffects::Parseeffect(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"name"))
		{
			m_pCurrentEffectData->m_strName = l_strValue;
		}
		else
		if(!wcscmp(l_strName,L"id"))
		{
			m_pCurrentEffectData->m_strID = l_strValue;
		}
		else
		{
			//assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON]))
		{
			Parseeffect_profile_COMMON( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_extra]))
		{
			Parseeffect_extra( l_pChildTiXmlElement );
		}
		else
		{
			//assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_newparam]))
		{
			Parseeffect_profile_COMMON_newparam( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_technique]))
		{
			Parseeffect_profile_COMMON_technique( l_pChildTiXmlElement );
		}
		else
		{
			//assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_newparam(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	sEffectData::sComonEffect::sNewParam*l_pNewParam = new sEffectData::sComonEffect::sNewParam();
	m_pCurrentEffectData->ComonEffect.AllNewParam.push_back(l_pNewParam);
	m_pCurrentNewParam = l_pNewParam;
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"sid"))
		{
			m_pCurrentNewParam->strSid = l_strValue;
		}
		else
		{
			//assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_newparam_surface]))
		{
			Parseeffect_profile_COMMON_newparam_surface( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_newparam_sampler2D]))
		{
			Parseeffect_profile_COMMON_newparam_sampler2D( l_pChildTiXmlElement );
		}
		else
		{
			//assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_newparam_surface(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	m_pCurrentNewParam->eNPT = sEffectData::sComonEffect::sNewParam::eNPT_SURFACE;
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"type"))
		{
			m_pCurrentNewParam->Surface.strType = l_strValue;
		}
		else
		{
			//assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_newparam_surface_init_from]))
		{
			Parseeffect_profile_COMMON_newparam_surface_init_from( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_newparam_surface_format]))
		{
			Parseeffect_profile_COMMON_newparam_surface_format( l_pChildTiXmlElement );
		}
		else
		{
			//assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_newparam_surface_init_from(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	m_pCurrentNewParam->Surface.strInitForm = l_strText;
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_newparam_surface_format(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	m_pCurrentNewParam->Surface.strFormat = l_strText;
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_newparam_sampler2D(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	m_pCurrentNewParam->eNPT = sEffectData::sComonEffect::sNewParam::eNPT_SAMPLE2D;
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_newparam_sampler2D_source]))
		{
			Parseeffect_profile_COMMON_newparam_sampler2D_source( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_newparam_sampler2D_wrap_s]))
		{
			Parseeffect_profile_COMMON_newparam_sampler2D_wrap_s( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_newparam_sampler2D_wrap_t]))
		{
			Parseeffect_profile_COMMON_newparam_sampler2D_wrap_t( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_newparam_sampler2D_minfilter]))
		{
			Parseeffect_profile_COMMON_newparam_sampler2D_minfilter( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_newparam_sampler2D_magfilter]))
		{
			Parseeffect_profile_COMMON_newparam_sampler2D_magfilter( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_newparam_sampler2D_mipfilter]))
		{
			Parseeffect_profile_COMMON_newparam_sampler2D_mipfilter( l_pChildTiXmlElement );
		}
		else
		{
			//assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_newparam_sampler2D_source(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	m_pCurrentNewParam->Sample2D.strSource = l_strText;
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_newparam_sampler2D_wrap_s(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	m_pCurrentNewParam->Sample2D.strWrap_s = l_strText;
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_newparam_sampler2D_wrap_t(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	m_pCurrentNewParam->Sample2D.strWrap_t = l_strText;
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_newparam_sampler2D_minfilter(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	m_pCurrentNewParam->Sample2D.strMinFilter = l_strText;
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_newparam_sampler2D_magfilter(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	m_pCurrentNewParam->Sample2D.strMagFilter = l_strText;
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_newparam_sampler2D_mipfilter(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	m_pCurrentNewParam->Sample2D.strMipfilter = l_strText;
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_technique(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	assert(this->m_pCurrentEffectData->ComonEffect.AllTechniqueAndTextureName.size()==0&&"we should only have one technique in profile common");
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"sid"))
		{
			m_pCurrentEffectData->ComonEffect.strSid = l_strValue;
		}
		else
		if(!wcscmp(l_strName,L"id"))
		{
			m_pCurrentEffectData->ComonEffect.strId = l_strValue;
		}
		else
			assert(0);
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_technique_extra]))
		{//forget this,I do not want those data
			Parseeffect_profile_COMMON_technique_extra( l_pChildTiXmlElement );
		}
		else
		//if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_technique_blinn]))
		{//all technique will into this function
			Parseeffect_profile_COMMON_technique_type( l_pChildTiXmlElement );
		}
		//else
		//{
		//	assert(0);
		//}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_technique_extra(TiXmlElement*e_pTiXmlElement)
{
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		Parseeffect_profile_COMMON_technique_type( l_pChildTiXmlElement );
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}
}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_technique_type(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"sid"))
		{
			m_pCurrentEffectData->ComonEffect.strTechniqueName = l_strValue;
		}
		else
		{
			//assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		Parseeffect_profile_COMMON_technique_data_Assign( l_pChildTiXmlElement,l_strName );
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_technique_data_Assign(TiXmlElement*e_pTiXmlElement,const WCHAR*e_strShadingName)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_technique_color]))
		{
			Parseeffect_profile_COMMON_technique_color( l_pChildTiXmlElement,GetTechniqueName(e_strShadingName) );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_technique_texture]))
		{
			Parseeffect_profile_COMMON_technique_texture( l_pChildTiXmlElement,GetTechniqueName(e_strShadingName));
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_technique_float]))
		{
			Parseeffect_profile_COMMON_technique_float( l_pChildTiXmlElement,GetTechniqueName(e_strShadingName) );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_technique_color(TiXmlElement*e_pTiXmlElement,eTechniqueType e_eTechniqueType)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	sEffectData::sComonEffect::sTechniqueData*l_pTechniqueData  = new sEffectData::sComonEffect::sTechniqueData();
	//eTechniqueList
	l_pTechniqueData->eTL = e_eTechniqueType;
	m_pCurrentEffectData->ComonEffect.AllTechniqueAndTextureName.push_back(l_pTechniqueData);
	l_pTechniqueData->pColor = ParseDataToGenerateIntPointer<float>((WCHAR*)l_strText,4);
	assert(l_strText);
}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_technique_float(TiXmlElement*e_pTiXmlElement,eTechniqueType e_eTechniqueType)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	sEffectData::sComonEffect::sTechniqueData*l_pTechniqueData  = new sEffectData::sComonEffect::sTechniqueData();
	l_pTechniqueData->eTL = e_eTechniqueType;
	m_pCurrentEffectData->ComonEffect.AllTechniqueAndTextureName.push_back(l_pTechniqueData);
	l_pTechniqueData->pfFloatData = new float;
	*l_pTechniqueData->pfFloatData = (float)_wtof(l_strText);
	assert(l_strText);
}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_technique_texture(TiXmlElement*e_pTiXmlElement,eTechniqueType e_eTechniqueType)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	sEffectData::sComonEffect::sTechniqueData*l_pTechniqueData  = new sEffectData::sComonEffect::sTechniqueData();
	m_pCurrentEffectData->ComonEffect.AllTechniqueAndTextureName.push_back(l_pTechniqueData);
	l_pTechniqueData->eTL = e_eTechniqueType;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"texture"))
		{
			l_pTechniqueData->strTextureName = l_strValue;
		}
		else
		if(!wcscmp(l_strName,L"texcoord"))
		{
			l_pTechniqueData->strTexCoord = l_strValue;
		}
		else
		{
			//assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_technique_texture_extra]))
		{
			Parseeffect_profile_COMMON_technique_texture_extra( l_pChildTiXmlElement );
		}
		else
		{
			//assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_technique_texture_extra(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_technique_texture_extra_technique]))
		{
			Parseeffect_profile_COMMON_technique_texture_extra_technique( l_pChildTiXmlElement );
		}
		else
		{
			//assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_technique_texture_extra_technique(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"profile"))
		{
			//l_strValue;
		}
		else
		{
			//assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_technique_texture_extra_technique_mirrorU]))
		{
			Parseeffect_profile_COMMON_technique_texture_extra_technique_mirrorU( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_technique_texture_extra_technique_mirrorV]))
		{
			Parseeffect_profile_COMMON_technique_texture_extra_technique_mirrorV( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_technique_texture_extra_technique_wrapU]))
		{
			Parseeffect_profile_COMMON_technique_texture_extra_technique_wrapU( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_technique_texture_extra_technique_wrapV]))
		{
			Parseeffect_profile_COMMON_technique_texture_extra_technique_wrapV( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_technique_texture_extra_technique_repeatU]))
		{
			Parseeffect_profile_COMMON_technique_texture_extra_technique_repeatU( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_technique_texture_extra_technique_repeatV]))
		{
			Parseeffect_profile_COMMON_technique_texture_extra_technique_repeatV( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_technique_texture_extra_technique_offsetU]))
		{
			Parseeffect_profile_COMMON_technique_texture_extra_technique_offsetU( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_technique_texture_extra_technique_offsetV]))
		{
			Parseeffect_profile_COMMON_technique_texture_extra_technique_offsetV( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_technique_texture_extra_technique_rotateUV]))
		{
			Parseeffect_profile_COMMON_technique_texture_extra_technique_rotateUV( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_profile_COMMON_technique_texture_extra_technique_amount]))
		{
			Parseeffect_profile_COMMON_technique_texture_extra_technique_amount( l_pChildTiXmlElement );
		}
		else
		{
			//assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_technique_texture_extra_technique_mirrorU(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_technique_texture_extra_technique_mirrorV(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_technique_texture_extra_technique_wrapU(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_technique_texture_extra_technique_wrapV(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_technique_texture_extra_technique_repeatU(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_technique_texture_extra_technique_repeatV(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_technique_texture_extra_technique_offsetU(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_technique_texture_extra_technique_offsetV(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_technique_texture_extra_technique_rotateUV(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_profile_COMMON_technique_texture_extra_technique_amount(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_extra(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_extra_technique]))
		{
			Parseeffect_extra_technique( l_pChildTiXmlElement );
		}
		else
		{
			//assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryEffects::Parseeffect_extra_technique(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"profile"))
		{
			//l_strValue;
		}
		else
		{
			//assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_extra_technique_faceted]))
		{
			Parseeffect_extra_technique_faceted( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_extra_technique_double_sided]))
		{
			Parseeffect_extra_technique_double_sided( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_extra_technique_wireframe]))
		{
			Parseeffect_extra_technique_wireframe( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryEffectsElement[eLE_effect_extra_technique_face_map]))
		{
			Parseeffect_extra_technique_face_map( l_pChildTiXmlElement );
		}
		else
		{
			//assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryEffects::Parseeffect_extra_technique_faceted(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_extra_technique_double_sided(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_extra_technique_wireframe(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);

}
//
//
//
void	cLibraryEffects::Parseeffect_extra_technique_face_map(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);

}