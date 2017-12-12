#ifndef COLLADA_VISUAL_SCENE_H
#define COLLADA_VISUAL_SCENE_H
#include "FatmingVisualSceneData.h"
#include "AnimationMesh.h"
enum eLVSList
{
	eLVS_visual_scene = 0,
	eLVS_visual_scene_node,
	eLVS_visual_scene_node_matrix,
	eLVS_visual_scene_node_translate,
	eLVS_visual_scene_node_rotate,
	eLVS_visual_scene_node_scale,
	eLVS_visual_scene_node_instance_controller,//controller and geometry using same instance 
	eLVS_visual_scene_node_instance_geometry,
	eLVS_visual_scene_node_skeleton,
	eLVS_visual_scene_node_instance_bind_material,
	eLVS_visual_scene_node_instance_bind_material_technique_common,
	eLVS_visual_scene_node_instance_bind_material_technique_common_instance_material,
	eLVS_visual_scene_node_instance_bind_material_technique_common_instance_material_bind_vertex_input,
	eLVS_visual_scene_node_instance_light,
	eLVS_visual_scene_node_instance_camera,
	eLVS_visual_scene_extra,
	eLVS_visual_scene_extra_technique,
	eLVS_visual_scene_extra_technique_start_time,
	eLVS_visual_scene_extra_technique_end_time,
	eLVS_visual_scene_extra_technique_frame_rate,
	eLVS_MAX
};
static const WCHAR*g_sLibraryVisualScenesElement[] =
{
	L"visual_scene",
	L"node",
	L"matrix",
	L"translate",
	L"rotate",
	L"scale",
	L"instance_controller",//controller and geometry using same instance 
	L"instance_geometry",
	L"skeleton",
	L"bind_material",
	L"technique_common",
	L"instance_material",
	L"bind_vertex_input",
	L"instance_light",
	L"instance_camera",
	L"extra",
	L"technique",
	L"start_time",
	L"end_time",
	L"frame_rate",
};
class cLibraryVisualScenes :public NamedTypedObject
{
//
	void	Parsevisual_scene(TiXmlElement*e_pTiXmlElement);
//
	void	Parsevisual_scene_node(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode);
//
	void	Parsevisual_scene_node_matrix(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode);
//controller and geometry using same instance
	void	Parsevisual_scene_node_instance(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode);
//
//	void	Parsevisual_scene_node_instance_Geometry(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode);
//
	void	Parsevisual_scene_node_instance_skeleton(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode);
//
	void	Parsevisual_scene_node_instance_bind_material(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode);
//
	void	Parsevisual_scene_node_instance_bind_material_technique_common(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode);
//
	void	Parsevisual_scene_node_instance_bind_material_technique_common_instance_material(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode);
//
	void	Parsevisual_scene_node_instance_bind_material_technique_common_instance_material_bind_vertex_input(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode);
//
	void	Parsevisual_scene_node_instance_camera(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode);
//
	void	Parsevisual_scene_node_instance_light(TiXmlElement*e_pTiXmlElement,sVisualSceneLibrary::sNode*e_pParentNode);
//
	void	Parsevisual_scene_extra(TiXmlElement*e_pTiXmlElement);
//
	void	Parsevisual_scene_extra_technique(TiXmlElement*e_pTiXmlElement);
//
	void	Parsevisual_scene_extra_technique_start_time(TiXmlElement*e_pTiXmlElement);
//
	void	Parsevisual_scene_extra_technique_end_time(TiXmlElement*e_pTiXmlElement);
//
	void	Parsevisual_scene_extra_technique_frame_rate(TiXmlElement*e_pTiXmlElement);
public:
	DEFINE_TYPE_INFO()
	cLibraryVisualScenes()
	{
	}
	~cLibraryVisualScenes()
	{
	}
	sVisualSceneLibrary	m_VisualSceneData;
    void	ParseData(TiXmlElement*e_pTiXmlElement);
};

#endif