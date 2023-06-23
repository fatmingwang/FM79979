#ifndef _LEVEL_DATA_H_
#define _LEVEL_DATA_H_

#include "LevelLayerGridData.h"
#include "LevelLayerData.h"
#include "EventDataNode.h"
namespace FATMING_CORE
{
	//=======================
	//all map data store in here
	//cell could contain cell data,
	//the hirechy Object->Cell
	//======================
	//<LevelData EventDataNode="aabb.xml" ResourceData="aaxx.xml" EventFile="aaxx.xml" >
		// <ImageResource>
		//		<PI FileName="aa.pi" />
		//		<MPDI FileName="aa.mpdi" />
		//	</ImageResource>
		//<Template> 
		//	<Data PI="" Image="" Name="" DataNode="" />
		//	<Data MPDI="" Image="" Name="" DataNode="" />
		//</Template>
	  //<StageData Name="3" LayerCount="2" StartPos="0.00,0.00,-0.00," Size="840.00,600.00" GridSize="60,60">
		 // <StageDataLayer Name="Layer1" Count="3" UserData="Test">
			//<Data TemplateName="Env_Road" Pos="10.00,0.00,0.00," />
			//<Data TemplateName="Env_Road" Pos="20.00,0.00,0.00," />
			//<Data TemplateName="Env_Road" Pos="30.00,0.00,0.00," />
		 // <StageDataLayer/>
		 // <StageDataLayer Name="Layer2" Count="3" UserData="Test">
			//<Data TemplateName="Env_Road" Pos="10.00,10.00,0.00," />
			//<Data TemplateName="Env_Road" Pos="20.00,20.00,0.00," />
			//<Data TemplateName="Env_Road" Pos="30.00,30.00,0.00," />
		 // <StageDataLayer/>
	  //</StageData>
	//</LevelData>
	class	cEventManager;
	class	cLevelData:public cNodeISAX,public cNamedTypedObjectVector<cLevelLayerList>
	{
		//for fast to fetch level laver grid
		cLevelLayerGridData*m_pLastLevelLayerGridData;
		//
		void	ProcessEventDataNodeFile(const WCHAR*e_steEventDataNodeFileName);
		void	ProcessImageResourceFile(const WCHAR*e_steImageResourceFileName);
		void	ProcessEventFile(const WCHAR*e_steEventFileName);
		void	ProcessTemplateFile(const WCHAR*e_steTemplateFileName);
		void	ProcessTemplateDataNodeFile(const WCHAR*e_strTemplateDataNodeFile);

		void	ProcessTemplateWithPI(TiXmlElement*e_pTiXmlElement);
		void	ProcessTemplateWithMPDI(TiXmlElement*e_pTiXmlElement);
		void	ProcessTemplateWithBaseImage(TiXmlElement*e_pTiXmlElement);
		void	ProcessImageResource(TiXmlElement*e_pTiXmlElement);
		void	ProcessPIImage(TiXmlElement*e_pTiXmlElement);
		void	ProcessMPDIImage(TiXmlElement*e_pTiXmlElement);
		void	ProcessNoImage(TiXmlElement*e_pTiXmlElement);
		//
		//
		virtual	bool	MyParse(TiXmlElement*e_pRoot)override;
		void	ProcesssLevelData(TiXmlElement*e_pTiXmlElement);
		void	ProcesssLevelLayerData(TiXmlElement*e_pTiXmlElement,cLevelLayerList*e_pLevelLayerData);
		void	ProcesssLevelLayerGridData(TiXmlElement*e_pTiXmlElement,cLayer*e_pLayer);
	public:
		//in the game we might don't need image file,because we just need name to get correct animation
		//bool	m_bIgnoreParseImageFile;
		//store all templateImage
		cLayer*		m_pTemplateList;
		//
		cEventManager*							m_pEventManager;
		//
		cNamedTypedObjectVector<cEventDataNode>		m_EventDataNodeList;
		cNamedTypedObjectVector<cEventDataNode>		m_TemplateDataNodeList;
		DEFINE_TYPE_INFO();
		cLevelData();
		virtual ~cLevelData();
		cLevelLayerGridData*GetListChildByUniqueID(uint64 e_uiID);
		//virtual	bool	ParseDataIntoXMLNode(char*e_strFileName);
		bool	Export(const char*e_strFileName,bool e_bAlsoExportYInvert = false,bool e_bAlsoExportXInvert = false);
		bool	ExportTemplateFile(const char*e_strFileName);
		bool	ExportResourceFile(const char*e_strFileName);
		bool	ExportEventDataNodeFile(const char*e_strFileName);
		bool	ExportTemplateDataNodeFile(const char*e_strFileName);
	};

//end if _LEVEL_MANAGE_H_
}
#endif