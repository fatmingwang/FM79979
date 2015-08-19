#ifndef COLLADA_CAMERA_H
#define COLLADA_CAMERA_H

enum eLCameraEnumList
{
	eLCEL_camera = 0,
	eLCELcamera_optics,
	eLCELcamera_optics_technique_common,
	eLCELcamera_optics_technique_common_perspective,
	eLCELcamera_optics_technique_common_perspective_xfov,
	eLCELcamera_optics_technique_common_perspective_aspect_ratio,
	eLCELcamera_optics_technique_common_perspective_znear,
	eLCELcamera_optics_technique_common_perspective_zfar,
	eLCELcamera_extra,
	eLCELcamera_extra_technique,
	eLCELcamera_extra_technique_target,
	eLCELMAX,
};
static const WCHAR*g_sLibraryCamerasElement[eLCELMAX] =
{
	L"camera",
	L"optics",
	L"technique_common",
	L"perspective",
	L"xfov",
	L"aspect_ratio",
	L"znear",
	L"zfar",
	L"extra",
	L"technique",
	L"target",
};
class cLibraryCameras :public NamedTypedObject
{
//
	void	Parsecamera(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecamera_optics(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecamera_optics_technique_common(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecamera_optics_technique_common_perspective(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecamera_optics_technique_common_perspective_xfov(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecamera_optics_technique_common_perspective_aspect_ratio(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecamera_optics_technique_common_perspective_znear(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecamera_optics_technique_common_perspective_zfar(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecamera_extra(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecamera_extra_technique(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecamera_extra_technique_target(TiXmlElement*e_pTiXmlElement);
public:
        DEFINE_TYPE_INFO();
        void	ParseData(TiXmlElement*e_pTiXmlElement);
};


#endif