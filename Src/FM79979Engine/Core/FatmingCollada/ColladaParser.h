#ifndef COLLADA_PARSER_H
#define COLLADA_PARSER_H

#include "ColladaGeometries.h"
#include "ColladaImages.h"
#include "ColladaEffects.h"
#include "ColladaMaterials.h"
#include "ColladaControllers.h"
#include "ColladaVisualScene.h"
#include "ColladaAnimations.h"
#include "Mesh.h"

	enum	eColladaMainType
	{
		eCMT_EFFECTS = 0,
		eCMT_MATERIALS,
		eCMT_GEOMETRIES,
		eCMT_LIGHTS,
		eCMT_IMAGES,
		eCMT_CAMERAS,
		eCMT_CONTROLLERS,
		eCMT_VISUAL_SCENE,
		eCMT_ANIMATION,
		eCMT_MAX,
	};
	static const WCHAR*g_sColladaMainType[eCMT_MAX] = 
	{
		L"library_effects",
		L"library_materials",
		L"library_geometries",
		L"library_lights",
		L"library_images",
		L"library_cameras",
		L"library_controllers",
		L"library_visual_scenes",
		L"library_animations",
	};
	//================
	//Geometry->Trianle->Material ID
	//Material ID->Effect ID
	//EffectID->Technique->Texture
	//================
	class	cColladaParser:public cNodeISAX,public NamedTypedObject
	{
		eColladaMainType	m_CurrenteColladaMainType;
		void	ProcessEffects();
		void	ProcessMaterials();
		void	ProcessGeometries();//c cpp
		void	ProcessLights();
		void	ProcessImages();
		void	ProcessCameras();
		void	ProcessVisualSceneas();
		void	ProcessAnimations();
		void	ProcessController();
		//after processed geometry and controller we have that Geometry's mesh generatting weight and blending bone indices,if the controller data exist
		void	ProcessWeightAndBlendingBoneIndicesMergeToMesh();
		cLibraryGeometries		m_LibraryGeometries;
		cLibraryImages			m_LibraryImages;
		cLibraryEffects			m_LibraryEffects;
		cLibraryMaterials		m_LibraryMaterials;
		cLibraryVisualScenes	m_LibraryVisualScenes;
		cLibraryControllers		m_LibraryControllers;
		cLibraryAnimation		m_LibraryAnimation;
	public:
		cColladaParser();
		~cColladaParser();
		std::vector<std::string>	m_strParsingDataList;
		virtual	bool	ParseDataIntoXMLNode(const char*e_strFileName,bool e_bStroeInfo = false);
		cNamedTypedObjectVector<cMesh>*	m_pAllMesh;
		cNamedTypedObjectVector<cAnimationMesh>*	m_pAllAnimationMesh;
		std::vector<cCurve>	m_CurveList;
		void	ClearData();//care about image
		bool	m_bXMLParseFlag[eCMT_MAX];
		cLibraryVisualScenes*	GetLibraryVisualScenes(){return &m_LibraryVisualScenes;}
	};

#endif