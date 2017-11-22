#include "StdAfx.h"
#include "ColladaAnimations.h"
#include "AnimationMesh.h"

std::map<float, cMatrix44>	sAnimationLibrary::sAnimation::GenerateKeyFrameMatrix(const WCHAR*e_strJointeName)
{
	std::map<float, cMatrix44>	l_MapData;
	size_t	l_uiSize = this->SourceVector.size();
	float*l_pKeyFrame = 0;
	float*l_pMatrix = 0;
	int	l_iCount = 0;
	sSource*l_pAnimationSource[2] = {0,0};
	for(size_t i=0;i<l_uiSize;++i  )
	{
		sSource*l_pSource = SourceVector[i];
		if(!l_pSource->pNameArray)
		{
			bool	l_bSanmeName = true;
			size_t	l_uiLength = wcslen(e_strJointeName);
			const WCHAR*l_strFor_Matrix = &l_pSource->strID[l_uiLength];
			//_matrix
			if( l_strFor_Matrix[0] != L'_'||
				l_strFor_Matrix[1] != L'm'||
				l_strFor_Matrix[2] != L'a'||
				l_strFor_Matrix[3] != L't'||
				l_strFor_Matrix[4] != L'r'||
				l_strFor_Matrix[5] != L'i'||
				l_strFor_Matrix[6] != L'x')
			{
				if( wcslen(l_pSource->strID)- l_uiLength>10 )
				{
					//for old version
					if( l_strFor_Matrix[0] != L'-'||
						l_strFor_Matrix[1] != L't'||
						l_strFor_Matrix[2] != L'r'||
						l_strFor_Matrix[3] != L'a'||
						l_strFor_Matrix[4] != L'n'||
						l_strFor_Matrix[5] != L's'||
						l_strFor_Matrix[6] != L'f'||
						l_strFor_Matrix[7] != L'o'||
						l_strFor_Matrix[8] != L'r'||
						l_strFor_Matrix[9] != L'm')
					continue;
				}
				else
					continue;
			}
			for( size_t j=0;j<l_uiLength;++j )
			{
				if( l_pSource->strID[j] != e_strJointeName[j] )
				{
					l_bSanmeName = false;
					break;
				}
			}
			if( l_bSanmeName )
			{
				if( l_pSource->IsMatrix() )
				{
					l_iCount = l_pSource->iCount/16;
					l_pMatrix = l_pSource->pFloatArray;
				}
				else
				{
					l_pKeyFrame = l_pSource->pFloatArray;
				}
				if( l_pAnimationSource[0] == 0 )
					l_pAnimationSource[0] = SourceVector[i];
				else
				{
					assert( l_pAnimationSource[1] == 0 );
					l_pAnimationSource[1] = SourceVector[i];
					break;
				}
			}
		}
	}
	//assert(l_pAnimationSource[0]);
	//assert(l_pAnimationSource[1]);
	for(int i=0;i<l_iCount;++i )
	{
		cMatrix44	l_mat = *(cMatrix44*)l_pMatrix;
		//l_MapData[*l_pKeyFrame] = l_mat.Transposed();
		l_MapData[*l_pKeyFrame] = l_mat;
		l_pMatrix+=16;
		++l_pKeyFrame;
	}
	//assert(l_MapData.size());
	return l_MapData;
}

void	cLibraryAnimation::ParseData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_scLibraryAnimationElement[eLA_animation]))
		{
			Parseanimation(l_pChildTiXmlElement);
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
void	cLibraryAnimation::Parseanimation(TiXmlElement*e_pTiXmlElement)
{
	sAnimationLibrary::sAnimation*l_pAnination = new sAnimationLibrary::sAnimation;
	m_AnimationLibrary.AnimationVector.push_back(l_pAnination);
	m_pCurrentAnimation = l_pAnination;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"id"))
		{
			m_pCurrentAnimation->strID = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_scLibraryAnimationElement[eLA_animation_source]))
		{
			Parseanimation_source( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_scLibraryAnimationElement[eLA_animation_sampler]))
		{
			Parseanimation_sampler( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_scLibraryAnimationElement[eLA_animation_channel]))
		{
			Parseanimation_channel( l_pChildTiXmlElement );
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
void	cLibraryAnimation::Parseanimation_source(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	sAnimationLibrary::sAnimation::sSource*l_pSource = new sAnimationLibrary::sAnimation::sSource;
	m_pCurrentSource = l_pSource;
	m_pCurrentAnimation->SourceVector.push_back(l_pSource);
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"id"))
		{
			l_pSource->strID = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_scLibraryAnimationElement[eLA_animation_source_float_array]))
		{
			Parseanimation_source_float_array( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_scLibraryAnimationElement[eLA_animation_source_technique_common]))
		{
			Parseanimation_source_technique_common( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_scLibraryAnimationElement[eLA_animation_source_Name_array]))
		{
			Parseanimation_source_Name_array( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_scLibraryAnimationElement[eLA_animation_source_technique]))
		{
			Parseanimation_source_technique( l_pChildTiXmlElement );
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
void	cLibraryAnimation::Parseanimation_source_float_array(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"count"))
		{
			m_pCurrentSource->iCount = _wtoi(l_strValue);
		}
		else
		if(!wcscmp(l_strName,L"id"))
		{
			m_pCurrentSource->strArrayID = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	m_pCurrentSource->pFloatArray = ParseDataToGenerateIntPointer<float>(l_strText,m_pCurrentSource->iCount);
	assert(l_strText);

}
//
//
//
void	cLibraryAnimation::Parseanimation_source_technique_common(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	m_pCurrentSource->pTechniqueCommon = new sAnimationLibrary::sAnimation::sSource::sTechniqueCommon();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_scLibraryAnimationElement[eLA_animation_source_technique_common_accessor]))
		{
			Parseanimation_source_technique_common_accessor( l_pChildTiXmlElement );
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
void	cLibraryAnimation::Parseanimation_source_technique_common_accessor(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"count"))
		{
			m_pCurrentSource->pTechniqueCommon->iCount = _wtoi(l_strValue);
		}
		else
		if(!wcscmp(l_strName,L"source"))
		{
			m_pCurrentSource->pTechniqueCommon->strSource = l_strValue;
		}
		else
		if(!wcscmp(l_strName,L"stride"))
		{
			m_pCurrentSource->pTechniqueCommon->iStride = _wtoi(l_strValue);
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_scLibraryAnimationElement[eLA_animation_source_technique_common_accessor_param]))
		{
			Parseanimation_source_technique_common_accessor_param( l_pChildTiXmlElement );
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
void	cLibraryAnimation::Parseanimation_source_technique_common_accessor_param(TiXmlElement*e_pTiXmlElement)
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
			m_pCurrentSource->pTechniqueCommon->strParaName = l_strValue;
		}
		else
		if(!wcscmp(l_strName,L"type"))
		{
			m_pCurrentSource->pTechniqueCommon->strParaType = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}

}
//
//
//
void	cLibraryAnimation::Parseanimation_source_technique(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	m_pCurrentSource->pTechnique = new sAnimationLibrary::sAnimation::sSource::sTechnique();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"profile"))
		{
			m_pCurrentSource->pTechnique->strProfile = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_scLibraryAnimationElement[eLA_animation_source_technique_pre_infinity]))
		{
			Parseanimation_source_technique_pre_infinity( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_scLibraryAnimationElement[eLA_animation_source_technique_post_infinity]))
		{
			Parseanimation_source_technique_post_infinity( l_pChildTiXmlElement );
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
void	cLibraryAnimation::Parseanimation_source_technique_pre_infinity(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	m_pCurrentSource->pTechnique->strPreInfinity = l_strText;
	assert(l_strText);

}
//
//
//
void	cLibraryAnimation::Parseanimation_source_technique_post_infinity(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	m_pCurrentSource->pTechnique->strPostInfinity = l_strText;
	assert(l_strText);

}
//
//
//
void	cLibraryAnimation::Parseanimation_source_Name_array(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"count"))
		{
			m_pCurrentSource->iCount = _wtoi(l_strValue);
		}
		else
		if(!wcscmp(l_strName,L"id"))
		{
			m_pCurrentSource->strID = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	m_pCurrentSource->pNameArray = l_strText;
	assert(l_strText);

}
//
//
//
void	cLibraryAnimation::Parseanimation_sampler(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	sAnimationLibrary::sAnimation::sSampler*l_pSampler = new sAnimationLibrary::sAnimation::sSampler();
	m_pCurrentSampler = l_pSampler;
	m_pCurrentAnimation->SamplerVector.push_back(l_pSampler);
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"id"))
		{
			l_pSampler->strID = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_scLibraryAnimationElement[eLA_animation_sampler_input]))
		{
			Parseanimation_sampler_input( l_pChildTiXmlElement );
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
void	cLibraryAnimation::Parseanimation_sampler_input(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	sAnimationLibrary::sAnimation::sSampler::sInput*l_pInput = new sAnimationLibrary::sAnimation::sSampler::sInput;
	m_pCurrentSampler->InputVector.push_back(l_pInput);
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"semantic"))
		{
			l_pInput->strSemantic = l_strValue;
		}
		else
		if(!wcscmp(l_strName,L"source"))
		{
			l_pInput->strSource = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}

}
//
//
//
void	cLibraryAnimation::Parseanimation_channel(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	sAnimationLibrary::sAnimation::sChannel*l_pChannel = new sAnimationLibrary::sAnimation::sChannel;
	m_pCurrentChannel = l_pChannel;
	this->m_pCurrentAnimation->ChannelVector.push_back(l_pChannel);
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"target"))
		{
			l_pChannel->strTarget = l_strValue;
		}
		else
		if(!wcscmp(l_strName,L"source"))
		{
			l_pChannel->strSource = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}

}
