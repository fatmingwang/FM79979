#ifndef COLLADA_GEOMETRIES_H
#define COLLADA_GEOMETRIES_H
#include "FatmingGeometryData.h"

	//only element not include attribute
	enum eLGList
	{
		eLG_geometry = 0,
		eLG_geometry_mesh,
		eLG_geometry_mesh_source,
		eLG_geometry_mesh_source_float_array,
		eLG_geometry_mesh_source_technique_common,
		eLG_geometry_mesh_source_technique_common_accessor,
		eLG_geometry_mesh_source_technique_common_accessor_param,
		eLG_geometry_mesh_vertices,
		eLG_geometry_mesh_vertices_input,
		eLG_geometry_mesh_triangles,
		eLG_geometry_mesh_triangles_input,
		eLG_geometry_mesh_triangles_p,
		eLG_geometry_spline,
		eLG_geometry_spline_source,
		eLG_geometry_spline_source_float_array,
		eLG_geometry_spline_source_technique_common,
		eLG_geometry_spline_source_technique_common_accessor,
		eLG_geometry_spline_source_technique_common_accessor_param,
		eLG_geometry_spline_source_Name_array,
		eLG_geometry_spline_control_vertices,
		eLG_geometry_spline_control_vertices_input,
		eLG_MAX
	};
	static const WCHAR*g_sLibraryGeometriesElement[eLG_MAX] =
	{
		L"geometry",
		L"mesh",
		L"source",
		L"float_array",
		L"technique_common",
		L"accessor",
		L"param",
		L"vertices",
		L"input",
		L"triangles",
		L"input",
		L"p",
		L"spline",
		L"source",
		L"float_array",
		L"technique_common",
		L"accessor",
		L"param",
		L"Name_array",
		L"control_vertices",
		L"input",
	};
	struct sColladaGeomatries;
	class cLibraryGeometries :public NamedTypedObject
	{
		//for internal using,it will be push into m_ColladaGeomatryList
		sColladaGeomatries*m_pCurrentColladaGeomatry;
		//
		void	Parsegeometry(TiXmlElement*e_pTiXmlElement);
		//
		void	Parsegeometry_mesh(TiXmlElement*e_pTiXmlElement);
		//
		void	Parsegeometry_mesh_source(TiXmlElement*e_pTiXmlElement);
		//
		void	Parsegeometry_mesh_source_float_array(TiXmlElement*e_pTiXmlElement);
		//
		void	Parsegeometry_mesh_source_technique_common(TiXmlElement*e_pTiXmlElement);
		//
		void	Parsegeometry_mesh_source_technique_common_accessor(TiXmlElement*e_pTiXmlElement);
		//
		void	Parsegeometry_mesh_source_technique_common_accessor_param(TiXmlElement*e_pTiXmlElement);
		//
		void	Parsegeometry_mesh_vertices(TiXmlElement*e_pTiXmlElement);
		//
		void	Parsegeometry_mesh_vertices_input(TiXmlElement*e_pTiXmlElement);
		//
		void	Parsegeometry_mesh_triangles(TiXmlElement*e_pTiXmlElement);
		//as set attribute
		//https://collada.org/public_forum/viewtopic.php?p=3037&sid=0973780713238182da5d492866774507
		//You can have multiple sets of texture coordinates in a <mesh>, and you can have multiple textures in an <effect>. The set attribute plays a role in matching up the textures in the <effect> with the texture coordinates in a <mesh>, which happens in <instance_material>. 
		//<mesh>
		//  <input offset="0" semantic="TEXCOORD" source="#texCoords1" set="0"/>
		//  <input offset="1" semantic="TEXCOORD" source="#texCoords2" set="1"/>
		//</mesh>
		//...
		//<effect>
		//  ...
		//  <diffuse>
		//    <texture texture="sampler1" texcoord="diffuseCoords"/>
		//  </diffuse>
		//  <specular
		//    <texture texture="sampler2" texcoord="specularCoords"/>
		//</effect>
		//<node>
		//  <instance_geometry ...>
		//    <bind_material>
		//      <technique_common>
		//        <instance_material ...>
		//          <bind_vertex_input semantic="diffuseCoords" input_semantic="TEXCOORD" input_set="0"/>
		//          <bind_vertex_input semantic="specularCoords" input_semantic="TEXCOORD" input_set="1"/>
		//</node>  
		//To retain as much flexibility as possible, material properties are bound when a geometry element is instantiated,
		//not when it's defined. As shown in the example above,
		//the "set" attribute is used to bind a texture to a particular set of texture coordinates in <bind_vertex_input>. 
		//For additional references, take a look at this thread and the section on texture mapping in the release notes.
		void	Parsegeometry_mesh_triangles_input(TiXmlElement*e_pTiXmlElement);
		//https://collada.org/public_forum/viewtopic.php?p=3037&sid=0973780713238182da5d492866774507
		void	Parsegeometry_mesh_triangles_p(TiXmlElement*e_pTiXmlElement);
		//for spline
		void	Parsegeometry_spline(TiXmlElement*e_pTiXmlElement);
		//
		void	Parsegeometry_spline_source(TiXmlElement*e_pTiXmlElement);
		//
		void	Parsegeometry_spline_source_float_array(TiXmlElement*e_pTiXmlElement);
		//
		void	Parsegeometry_spline_source_technique_common(TiXmlElement*e_pTiXmlElement);
		//
		void	Parsegeometry_spline_source_technique_common_accessor(TiXmlElement*e_pTiXmlElement);
		//
		void	Parsegeometry_spline_source_technique_common_accessor_param(TiXmlElement*e_pTiXmlElement);
		//
		void	Parsegeometry_spline_source_Name_array(TiXmlElement*e_pTiXmlElement);
		//
		void	Parsegeometry_spline_control_vertices(TiXmlElement*e_pTiXmlElement);
		//
		void	Parsegeometry_spline_control_vertices_input(TiXmlElement*e_pTiXmlElement);
	public:
		DEFINE_TYPE_INFO()
		cLibraryGeometries();
		~cLibraryGeometries();
		void	ParseData(TiXmlElement*e_pTiXmlElement);
		std::vector<sColladaGeomatries*>	m_ColladaGeomatryList;
		sColladaGeomatries*					GetGeometryByID(const WCHAR*e_strID);
		sColladaGeomatries*GetColladaGeomatries(const WCHAR*e_strID);
		//please just call me once
		bool	GenerateSkinningTriangleBlendicesWeights(sController*e_pController);
		//please just call me once
		void	GenerateMesh();
		//
		std::vector<cCurve>	ProcessCurveData();
		//
		void	ClearData(){DELETE_VECTOR(m_ColladaGeomatryList,sColladaGeomatries*);}
		
	};

#endif