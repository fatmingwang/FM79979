#ifndef COLLADA_CONTROLLERS
#define COLLADA_CONTROLLERS
#include "FatmingController.h"
#include "FatmingGeometryData.h"
enum eLCList
{
	eLC_controller = 0,
	eLC_controller_skin,
	eLC_controller_skin_bind_shape_matrix,
	eLC_controller_skin_source,
	eLC_controller_skin_source_Name_array,
	eLC_controller_skin_source_technique_common,
	eLC_controller_skin_source_technique_common_accessor,
	eLC_controller_skin_source_technique_common_accessor_param,
	eLC_controller_skin_source_float_array,
	eLC_controller_skin_joints,
	eLC_controller_skin_joints_input,
	eLC_controller_skin_vertex_weights,
	eLC_controller_skin_vertex_weights_input,
	eLC_controller_skin_vertex_weights_vcount,
	eLC_controller_skin_vertex_weights_v,
	eLC_MAX
};
static const WCHAR*g_sLibraryControllersElement[eLC_MAX] =
{
	L"controller",
	L"skin",
	L"bind_shape_matrix",
	L"source",
	L"Name_array",
	L"technique_common",
	L"accessor",
	L"param",
	L"float_array",
	L"joints",
	L"input",
	L"vertex_weights",
	L"input",
	L"vcount",
	L"v",
};
//=======================
//vcount has same count as vertext
//vcount for how many bones influnce with specific bone
//v data order by vcount
//ex: 1 2 1 1 1 1 
//first bones has 1 influnce bone
//second has 1
//third has 1......
//then we could fetch bone index and weight valuse by <v>
//
//generate weights u have to fetch weight source and calculate influnce bones
//if total invlunce bones is over 4,calculate suitable weight is need and abandon useless
//
//
//
//=======================
class cLibraryControllers :public NamedTypedObject
{
//
	void	Parsecontroller(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecontroller_skin(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecontroller_skin_bind_shape_matrix(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecontroller_skin_source(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecontroller_skin_source_Name_array(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecontroller_skin_source_technique_common(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecontroller_skin_source_technique_common_accessor(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecontroller_skin_source_technique_common_accessor_param(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecontroller_skin_source_float_array(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecontroller_skin_joints(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecontroller_skin_joints_input(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecontroller_skin_vertex_weights(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecontroller_skin_vertex_weights_input(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecontroller_skin_vertex_weights_vcount(TiXmlElement*e_pTiXmlElement);
//
	void	Parsecontroller_skin_vertex_weights_v(TiXmlElement*e_pTiXmlElement);
//internal use
	sController*		m_pCurrentController;
	sController::sSkin::sSource*	m_pCurrentSource;
public:
        DEFINE_TYPE_INFO()
        void	ParseData(TiXmlElement*e_pTiXmlElement);
		std::vector<sController*>	m_ControllerList;
		sController*	GetController(WCHAR*e_strSource)
		{
			for( UINT i=0;i<m_ControllerList.size();++i )
			{
				if(!wcscmp(m_ControllerList[i]->strID,e_strSource))
					return m_ControllerList[i];
			}
			return 0;
		}
		void	ClearData(){ DELETE_VECTOR(m_ControllerList); }
		cLibraryControllers(){}
		~cLibraryControllers()
		{
			ClearData();
		}
};


#endif