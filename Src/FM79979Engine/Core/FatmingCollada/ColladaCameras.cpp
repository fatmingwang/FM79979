#include "StdAfx.h"
#include "ColladaCameras.h"

//
//
//
void	cLibraryCameras::Parsecamera(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"id"))
		{
//			l_strValue;
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
		if(!wcscmp(l_strName,g_sLibraryCamerasElement[eLCELcamera_optics]))
		{
			Parsecamera_optics( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryCamerasElement[eLCELcamera_extra]))
		{
			Parsecamera_extra( l_pChildTiXmlElement );
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
void	cLibraryCameras::Parsecamera_optics(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryCamerasElement[eLCELcamera_optics_technique_common]))
		{
			Parsecamera_optics_technique_common( l_pChildTiXmlElement );
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
void	cLibraryCameras::Parsecamera_optics_technique_common(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryCamerasElement[eLCELcamera_optics_technique_common_perspective]))
		{
			Parsecamera_optics_technique_common_perspective( l_pChildTiXmlElement );
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
void	cLibraryCameras::Parsecamera_optics_technique_common_perspective(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryCamerasElement[eLCELcamera_optics_technique_common_perspective_xfov]))
		{
			Parsecamera_optics_technique_common_perspective_xfov( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryCamerasElement[eLCELcamera_optics_technique_common_perspective_aspect_ratio]))
		{
			Parsecamera_optics_technique_common_perspective_aspect_ratio( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryCamerasElement[eLCELcamera_optics_technique_common_perspective_znear]))
		{
			Parsecamera_optics_technique_common_perspective_znear( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryCamerasElement[eLCELcamera_optics_technique_common_perspective_zfar]))
		{
			Parsecamera_optics_technique_common_perspective_zfar( l_pChildTiXmlElement );
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
void	cLibraryCameras::Parsecamera_optics_technique_common_perspective_xfov(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);

}
//
//
//
void	cLibraryCameras::Parsecamera_optics_technique_common_perspective_aspect_ratio(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);

}
//
//
//
void	cLibraryCameras::Parsecamera_optics_technique_common_perspective_znear(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);

}
//
//
//
void	cLibraryCameras::Parsecamera_optics_technique_common_perspective_zfar(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);

}
//
//
//
void	cLibraryCameras::Parsecamera_extra(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryCamerasElement[eLCELcamera_extra_technique]))
		{
			Parsecamera_extra_technique( l_pChildTiXmlElement );
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
void	cLibraryCameras::Parsecamera_extra_technique(TiXmlElement*e_pTiXmlElement)
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
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryCamerasElement[eLCELcamera_extra_technique_target]))
		{
			Parsecamera_extra_technique_target( l_pChildTiXmlElement );
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
void	cLibraryCameras::Parsecamera_extra_technique_target(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);

}
