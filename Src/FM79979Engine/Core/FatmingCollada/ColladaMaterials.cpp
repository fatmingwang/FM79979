#include "StdAfx.h"
#include "ColladaMaterials.h"
const wchar_t*  cLibraryMaterials::TypeID( L"cLibraryMaterials" );

sMaterial*	cLibraryMaterials::GetMaterialByID(const WCHAR*e_strName)
{
	for(UINT i=0;i<AllMaterial.size();++i)
	{
		if(!wcscmp(this->AllMaterial[i]->strID,e_strName))
			return AllMaterial[i];
	}
	return 0;
}
//
//
//
void	cLibraryMaterials::ParseData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_scLibraryMaterialsElement[eLM_material]))
		{
			sMaterial*l_pMaterial = new sMaterial();
			AllMaterial.push_back(l_pMaterial);
			this->m_pCurrentMaterial = l_pMaterial;
			Parsematerial(l_pChildTiXmlElement);
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
void	cLibraryMaterials::Parsematerial(TiXmlElement*e_pTiXmlElement)
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
			m_pCurrentMaterial->strName = l_strValue;
		}
		else
		if(!wcscmp(l_strName,L"id"))
		{
			m_pCurrentMaterial->strID = l_strValue;
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
		if(!wcscmp(l_strName,g_scLibraryMaterialsElement[eLM_material_instance_effect]))
		{
			Parsematerial_instance_effect( l_pChildTiXmlElement );
		}
		else
		{
			cGameApp::OutputDebugInfoString(l_strName);
			cGameApp::OutputDebugInfoString(L"	Fucking Lazy to do this now!!!\n");
			//assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryMaterials::Parsematerial_instance_effect(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"url"))
		{
			m_pCurrentMaterial->strInstanceEffectName = &l_strValue[0];
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}

}
