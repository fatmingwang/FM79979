#include "stdafx.h"
#include "LevelData.h"
#include "EventDataNode.h"
#ifdef WIN32
#include <direct.h>
#endif
namespace FATMING_CORE
{
	const wchar_t*         cLevelData::TypeID( L"cLevelData" );
	cLevelData::cLevelData()
	{
		m_pEventManager = 0;
		m_pTemplateList = new cLayer();
		//SAFE_DELETE(m_pTemplateList);
		m_pLastLevelLayerGridData = 0;
		//m_bIgnoreParseImageFile = false;
	}

	cLevelData::~cLevelData()
	{
		SAFE_DELETE(m_pEventManager);
		SAFE_DELETE(m_pTemplateList);
	}
	//image resource 
	// <ImageResource>
	//		<PI FileName="aa.pi" />
	//		<MPDI FileName="aa.mpdi" />
	//	</ImageResource>

	//user data node
	//<Template> 
	//	<PITemplate PI="" Image="" Name="" DataNode=""/>
	//	<MPDITemplate MPDI="" Image="" Name="" DataNode=""/>
	//</Template>

	//<LevelData EventDataNode="aabb.xml" ResourceData="aaxx.xml" TemplateFile="aa.xml" >
  //<StageData Name="3" LayerCount="2" StartPos="0.00,0.00,-0.00," Size="840.00,600.00" GridSize="60,60">
	 // <StageDataLayer Name="Layer1" Count="3" UserData="Test">
		//<Data TemplateName="Env_Road" Pos="10.00,0.00,0.00," />
		//<Data TemplateName="Env_Road" Pos="20.00,0.00,0.00," />
		//<Data TemplateName="Env_Road" Pos="30.00,0.00,0.00," />
	 // <StageDataLayer/>
	 // <StageDataLayer Name="Layer2" Count="3" EventNode="Test">
		//<Data TemplateName="Env_Road" Pos="10.00,10.00,0.00," />
		//<Data TemplateName="Env_Road" Pos="20.00,20.00,0.00," />
		//<Data TemplateName="Env_Road" Pos="30.00,30.00,0.00," />
	 // <StageDataLayer/>
  //</StageData>

	bool	cLevelData::MyParse(TiXmlElement*e_pRoot)
	{
		m_EventDataNodeList.Destroy();
		m_TemplateDataNodeList.Destroy();
		SAFE_DELETE(m_pTemplateList);
		SAFE_DELETE(m_pEventManager);
		m_pEventManager = new cEventManager();
		m_pTemplateList = new cLayer();
		this->Destroy();
		PARSE_ELEMENT_START(e_pRoot)
			COMPARE_NAME("EventDataNode")
			{
				ProcessEventDataNodeFile(l_strValue);
			}
			else
			COMPARE_NAME("ResourceData")
			{
				ProcessImageResourceFile(l_strValue);
			}
			else
			COMPARE_NAME("Event")
			{
				ProcessEventFile(l_strValue);
			}
			else
			COMPARE_NAME("TemplateFile")
			{
				ProcessTemplateFile(l_strValue);
			}
			else
			COMPARE_NAME("TemplateDataNodeFile")
			{
				ProcessTemplateDataNodeFile(l_strValue);
			}
		PARSE_NAME_VALUE_END
		TiXmlElement*l_pTiXmlElement = e_pRoot->FirstChildElement();
		while( l_pTiXmlElement )
		{
			const WCHAR*l_strName = l_pTiXmlElement->Value();
			COMPARE_NAME("LevelData")
			{
				ProcesssLevelData(l_pTiXmlElement);
			}
			l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
		}
		return true;
	}
	void	cLevelData::ProcessEventDataNodeFile(const WCHAR*e_steEventDataNodeFileName)
	{
		std::string	l_strFileName = this->m_strCurrentDirectory;
		l_strFileName += UT::WcharToChar(e_steEventDataNodeFileName);
		cNodeISAX	l_cNodeISAX;
		if(l_cNodeISAX.ParseDataIntoXMLNode(l_strFileName.c_str()))
		{
			TiXmlElement*l_pTiXmlElement = l_cNodeISAX.GetRootElement();
			l_pTiXmlElement = l_pTiXmlElement->FirstChildElement();
			while( l_pTiXmlElement )
			{
				cEventDataNode*l_pUserDayaNode = new cEventDataNode();
				const WCHAR*l_strEventDataNodeName = l_pTiXmlElement->Attribute(L"Name");
				l_pUserDayaNode->SetDataNode(l_pTiXmlElement->FirstChildElement());
				l_pUserDayaNode->SetName(l_strEventDataNodeName);
				if(!this->m_EventDataNodeList.AddObject(l_pUserDayaNode))
				{
					UT::ErrorMsg(L"same template node!",l_pTiXmlElement->Value());
					delete l_pUserDayaNode;
				}
				l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
			}
		}
		else
		{
			UT::ErrorMsg("ProcessEventDataNodeFile file open failed !",l_strFileName.c_str());
		}
	}

	void	cLevelData::ProcessImageResourceFile(const WCHAR*e_steImageResourceFileName)
	{
		std::string	l_strFileName = this->m_strCurrentDirectory;
		l_strFileName += UT::WcharToChar(e_steImageResourceFileName);
		cNodeISAX	l_cNodeISAX;
		if( l_cNodeISAX.ParseDataIntoXMLNode(l_strFileName.c_str()) )
		{
			TiXmlElement*l_pTiXmlElement = l_cNodeISAX.GetRootElement();
			ProcessImageResource(l_pTiXmlElement);
		}
		else
		{
			UT::ErrorMsg(L"image file open failed !",e_steImageResourceFileName);
		}
	}

	void	cLevelData::ProcessEventFile(const WCHAR*e_steEventFileName)
	{
		std::string	l_strFileName = this->m_strCurrentDirectory;
		l_strFileName += UT::WcharToChar(e_steEventFileName);
		SAFE_DELETE(m_pEventManager);
		m_pEventManager = new cEventManager();
		if(!m_pEventManager->ParseDataIntoXMLNode(l_strFileName.c_str()))
		{
			UT::ErrorMsg(e_steEventFileName,L"open file failed");
		}
	}

	//<PI FileName="aa.pi" />
	void	cLevelData::ProcessPIImage(TiXmlElement*e_pTiXmlElement)
	{
		const WCHAR*l_strValue = e_pTiXmlElement->Attribute(L"FileName");
		std::string	l_strFullName = this->m_strCurrentDirectory;
		l_strFullName += UT::WcharToChar(l_strValue);
		if(!cGameApp::m_spImageParser->GetPuzzleImageByFileName(l_strFullName.c_str()))
		{
			UT::ErrorMsg(l_strFullName.c_str(),"file parse failed!");
		}
	}
	//<MPDI FileName="aa.mppi" />
	void	cLevelData::ProcessMPDIImage(TiXmlElement*e_pTiXmlElement)
	{
		const WCHAR*l_strValue = e_pTiXmlElement->Attribute(L"FileName");
		std::string	l_strFullName = this->m_strCurrentDirectory;
		l_strFullName += UT::WcharToChar(l_strValue);
		if(!cGameApp::m_spAnimationParser->Parse(l_strFullName.c_str()))
		{
			UT::ErrorMsg(l_strFullName.c_str(),"file parse failed!");
		}
	}

	void	cLevelData::ProcessNoImage(TiXmlElement*e_pTiXmlElement)
	{
		POINT	l_Size = {50,50};
		const WCHAR*l_strValue = e_pTiXmlElement->Attribute(L"GridSize");
		const WCHAR*l_strName = e_pTiXmlElement->Attribute(L"Name");
		if( l_strValue )
			l_Size = GetPoint(l_strValue);
		assert(!cGameApp::m_spAnimationParser->GetObject(l_strName));
		cBaseImage*l_pImage = new cBaseImage(0,false);
		l_pImage->SetName(l_strName);
		l_pImage->SetWidth(l_Size.x);
		l_pImage->SetHeight(l_Size.y);
		cGameApp::m_spImageParser->AddObject(l_pImage);
	}

	// <ImageImageResource>
	//		<PI FileName="aa.pi" />
	//		<MPDI FileName="aa.mpdi" />
	//	</ImageImageResource>
	void	cLevelData::ProcessImageResource(TiXmlElement*e_pTiXmlElement)
	{
		TiXmlElement*l_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		while( l_pTiXmlElement )
		{
			if( !wcscmp(l_pTiXmlElement->Value(),L"PI") )
			{
				ProcessPIImage(l_pTiXmlElement);
			}
			else
			if( !wcscmp(l_pTiXmlElement->Value(),L"MPDI") )
			{
				ProcessMPDIImage(l_pTiXmlElement);
			}
			else
			if( !wcscmp(l_pTiXmlElement->Value(),L"NoImage") )
			{
				ProcessNoImage(l_pTiXmlElement);
			}
			else
			{
				assert(0);
			}
			l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
		}
	}

	void	cLevelData::ProcessTemplateWithPI(TiXmlElement*e_pTiXmlElement)
	{
		cLevelLayerGridData*l_pLevelLayerGridData = new cLevelLayerGridData();
		cPuzzleImage*l_pPI = 0;
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("PI")
			{
				l_pPI = dynamic_cast<cPuzzleImage*>(cGameApp::m_spImageParser->GetObject(l_strValue));
			}
			else
			COMPARE_NAME("Name")
			{
				l_pLevelLayerGridData->SetName(l_strValue);
			}
			else
			COMPARE_NAME("Image")
			{
				cPuzzleImageUnit*l_pPuzzleImageUnit = l_pPI->GetObject(l_strValue);
				assert(l_pPuzzleImageUnit);
				l_pLevelLayerGridData->SetImage(l_pPuzzleImageUnit);
				l_pLevelLayerGridData->SetName(l_pPuzzleImageUnit->GetName());
			}
			else
			COMPARE_NAME("TemplateDataNode")
			{
				cEventDataNode*l_pEventDataNode = this->m_TemplateDataNodeList.GetObject(l_strValue);
				l_pLevelLayerGridData->SetTemplateDataNode(l_pEventDataNode);
				if( !l_pEventDataNode )
				{
					UT::ErrorMsg(l_strValue,L"userdata node not exist!?");
				}
			}
		PARSE_NAME_VALUE_END
		assert(!m_pTemplateList->GetObject(l_pLevelLayerGridData->GetName()));
		m_pTemplateList->AddObjectNeglectExist(l_pLevelLayerGridData);
	}

	void	cLevelData::ProcessTemplateWithMPDI(TiXmlElement*e_pTiXmlElement)
	{
		cLevelLayerGridData*l_pLevelLayerGridData = new cLevelLayerGridData();
		cMPDIList*l_pMPDIList = 0;
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("MPDI")
			{
				//if( !m_bIgnoreParseImageFile )
				l_pMPDIList = dynamic_cast<cMPDIList*>(cGameApp::m_spAnimationParser->GetObject(l_strValue));
				if( !l_pMPDIList )
				{
					UT::ErrorMsg(l_strValue,L"MPDIist not exist!");
				}
			}
			else
			COMPARE_NAME("Name")
			{
				l_pLevelLayerGridData->SetName(l_strValue);
			}
			else
			COMPARE_NAME("Image")
			{
				cMPDI*l_pMPDI = l_pMPDIList->GetObject(l_strValue);
				if( !l_pMPDI )
				{
					UT::ErrorMsg(l_strValue,L"MPDI not exist!");
				}
				l_pLevelLayerGridData->SetImage(l_pMPDI);
			}
			else
			COMPARE_NAME("TemplateDataNode")
			{
				cEventDataNode*l_pEventDataNode = this->m_TemplateDataNodeList.GetObject(l_strValue);
				l_pLevelLayerGridData->SetTemplateDataNode(l_pEventDataNode);
				if( !l_pEventDataNode )
				{
					UT::ErrorMsg(l_strValue,L"userdata node not exist!?");
				}
			}
		PARSE_NAME_VALUE_END
		assert(!m_pTemplateList->GetObject(l_pLevelLayerGridData->GetName()));
		m_pTemplateList->AddObjectNeglectExist(l_pLevelLayerGridData);
	}
    //<BaseImageTemplate NoImage="qq" />
	void	cLevelData::ProcessTemplateWithBaseImage(TiXmlElement*e_pTiXmlElement)
	{
		cLevelLayerGridData*l_pLevelLayerGridData = new cLevelLayerGridData();
		const WCHAR*l_str = e_pTiXmlElement->Attribute(L"NoImage");
		if( l_str )
		{
			cBaseImage*l_pBaseImage = cGameApp::m_spImageParser->GetBaseImage(l_str);
			l_pLevelLayerGridData->SetImage(l_pBaseImage);
			l_pLevelLayerGridData->SetName(l_str);
		}
		assert(!m_pTemplateList->GetObject(l_pLevelLayerGridData->GetName()));
		m_pTemplateList->AddObjectNeglectExist(l_pLevelLayerGridData);
	}
	//<Template> 
	//	<PITemplate PI="" Image="" Name="" DataNode=""/>
	//	<MPDITemplate MPDI="" Image="" Name="" DataNode=""/>
	//</Template>
	void	cLevelData::ProcessTemplateFile(const WCHAR*e_steTemplateFileName)
	{
		std::string	l_strFileName = this->m_strCurrentDirectory;
		l_strFileName += UT::WcharToChar(e_steTemplateFileName);
		cNodeISAX	l_cNodeISAX;
		if(l_cNodeISAX.ParseDataIntoXMLNode(l_strFileName.c_str()))
		{
			TiXmlElement*l_pTiXmlElement = l_cNodeISAX.GetRootElement();
			l_pTiXmlElement = l_pTiXmlElement->FirstChildElement();
			while( l_pTiXmlElement )
			{
				COMPARE_TARGET_ELEMENT_VALUE(l_pTiXmlElement,"PITemplate")
				{
					ProcessTemplateWithPI(l_pTiXmlElement);
				}
				else
				COMPARE_TARGET_ELEMENT_VALUE(l_pTiXmlElement,"MPDITemplate")
				{
					ProcessTemplateWithMPDI(l_pTiXmlElement);
				}
				else
				COMPARE_TARGET_ELEMENT_VALUE(l_pTiXmlElement,"BaseImageTemplate")
				{
					ProcessTemplateWithBaseImage(l_pTiXmlElement);
				}
				else
				{
					UT::ErrorMsg(l_pTiXmlElement->Value(),L"unsupport value");
				}
				l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
			}
		}
		else
		{
				UT::ErrorMsg(e_steTemplateFileName,L"file opena failed!");
		}
	}

	void	cLevelData::ProcessTemplateDataNodeFile(const WCHAR*e_strTemplateDataNodeFile)
	{
		std::string	l_strFileName = this->m_strCurrentDirectory;
		l_strFileName += UT::WcharToChar(e_strTemplateDataNodeFile);
		cNodeISAX	l_cNodeISAX;
		if(l_cNodeISAX.ParseDataIntoXMLNode(l_strFileName.c_str()))
		{
			TiXmlElement*l_pTiXmlElement = l_cNodeISAX.GetRootElement();
			l_pTiXmlElement = l_pTiXmlElement->FirstChildElement();
			while( l_pTiXmlElement )
			{
				cEventDataNode*l_pUserDayaNode = new cEventDataNode();
				const WCHAR*l_strEventDataNodeName = l_pTiXmlElement->Attribute(L"Name");

				l_pUserDayaNode->SetDataNode(l_pTiXmlElement->FirstChildElement());
				if(!this->m_TemplateDataNodeList.AddObject(l_pUserDayaNode))
				{
					UT::ErrorMsg(L"same template node!",l_pTiXmlElement->Value());
					delete l_pUserDayaNode;
				}
				l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
			}
		}
		else
		{
			UT::ErrorMsg("ProcessEventDataNodeFile file open failed !",l_strFileName.c_str());
		}
	}
  //<StageData Name="3" LayerCount="2" StartPos="0.00,0.00,-0.00," Size="840.00,600.00" GridSize="60,60">
	 // <StageDataLayer Name="Layer1" Count="3" EventNode="Test">
		//<Data TemplateName="Env_Road" Pos="10.00,0.00,0.00," />
		//<Data TemplateName="Env_Road" Pos="20.00,0.00,0.00," />
		//<Data TemplateName="Env_Road" Pos="30.00,0.00,0.00," />
	 // <StageDataLayer/>
	 // <StageDataLayer Name="Layer2" Count="3" EventNode="Test">
		//<Data TemplateName="Env_Road" Pos="10.00,10.00,0.00," />
		//<Data TemplateName="Env_Road" Pos="20.00,20.00,0.00," />
		//<Data TemplateName="Env_Road" Pos="30.00,30.00,0.00," />
	 // <StageDataLayer/>
  //</StageData>

	void	cLevelData::ProcesssLevelData(TiXmlElement*e_pTiXmlElement)
	{
		cLevelLayerList*l_pLevelLayerData = new cLevelLayerList();
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("Name")
			{
				l_pLevelLayerData->SetName((l_strValue));
			}
			else
			COMPARE_NAME("LayerCount")
			{
				l_pLevelLayerData->GetList()->reserve(_wtoi(l_strValue));
			}
			else
			COMPARE_NAME("Size")
			{
				Vector2	l_vSize = GetVector2(l_strValue);
				l_pLevelLayerData->SetSize(l_vSize);
			}
			else
			COMPARE_NAME("GridSize")
			{
				l_pLevelLayerData->SetGridSize(GetPoint(l_strValue));
			}
			else
			COMPARE_NAME("StartPos")
			{
				l_pLevelLayerData->SetPos(GetVector3(l_strValue));
			}
		PARSE_NAME_VALUE_END
		assert(GetObject(l_pLevelLayerData->GetName())==0);
		bool	l_b = this->AddObjectNeglectExist(l_pLevelLayerData);
		assert(l_b);
		e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		while( e_pTiXmlElement )
		{
			ProcesssLevelLayerData(e_pTiXmlElement,l_pLevelLayerData);
			e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
		}
	}
	// <StageDataLayer Name="Layer1" Count="3" EventNode="Test">
	void	cLevelData::ProcesssLevelLayerData(TiXmlElement*e_pTiXmlElement,cLevelLayerList*e_pLevelLayerData)
	{
		cLayer*l_pMapLayer = new cLayer();
		e_pLevelLayerData->AddObjectNeglectExist(l_pMapLayer);
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("Name")
			{
				l_pMapLayer->SetName(l_strValue);
			}
			else
			COMPARE_NAME("Count")
			{
				l_pMapLayer->GetList()->reserve(VALUE_TO_INT);
			}
			else
			COMPARE_NAME("EventNode")
			{
				std::wstring*l_pData = new std::wstring;
				*l_pData = l_strValue;
				l_pMapLayer->SetData((char*)l_pData);
			}
		PARSE_NAME_VALUE_END
		TiXmlElement*l_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		while( l_pTiXmlElement )
		{
			const WCHAR*l_strName = l_pTiXmlElement->Value();
			COMPARE_NAME("LevelLayerGrid")
			{
				ProcesssLevelLayerGridData(l_pTiXmlElement,l_pMapLayer);
			}
			else
			{
				UT::ErrorMsg(l_strName,L"unsupport name!");
			}
			l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
		}
	}

	void	cLevelData::ProcesssLevelLayerGridData(TiXmlElement*e_pTiXmlElement,cLayer*e_pLayer)
	{
		cLevelLayerGridData*l_pTargetObject = m_pLastLevelLayerGridData;
		cLevelLayerGridData*l_pLevelLayerGridData = 0;
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("TemplateName")
			{
				WCHAR*l_strObjectNamew = (WCHAR*)(l_strValue);
				if(!l_pTargetObject||wcscmp(l_strObjectNamew,l_pTargetObject->GetName()))
					l_pTargetObject = m_pTemplateList->GetObject(l_strObjectNamew);
				if( !l_pTargetObject )
					UT::ErrorMsg(L"Error not such object",l_strObjectNamew);
				else
					l_pLevelLayerGridData = new cLevelLayerGridData(l_pTargetObject);
			}
			else
			COMPARE_NAME("Pos")
			{
				l_pLevelLayerGridData->SetPos(GetVector3(l_strValue));
			}
			else
			COMPARE_NAME("EventDataNode")
			{
				cEventDataNode*l_pEventDataNode = this->m_EventDataNodeList.GetObject(l_strValue);
				if( !l_pEventDataNode )
				{
					UT::ErrorMsg(l_strValue,L"event node not find!");
				}
				else
				{
					l_pLevelLayerGridData->m_pEventDataNode = l_pEventDataNode;
				}
				//swprintf(l_pLevelLayerGridData->m_strUniqueUserProperty,L"%s\0",l_strValue);
			}
			else
			COMPARE_NAME("TemplateDataNode")
			{
				cEventDataNode*l_pEventDataNode = m_TemplateDataNodeList.GetObject(l_strValue);
				if( !l_pEventDataNode )
				{
					UT::ErrorMsg(l_strValue,L"event node not find!");
				}
				else
				{
					l_pLevelLayerGridData->m_pTemplateNode = l_pEventDataNode;
				}
				//swprintf(l_pLevelLayerGridData->m_strUniqueUserProperty,L"%s\0",l_strValue);
			}
		PARSE_NAME_VALUE_END
		e_pLayer->AddObjectNeglectExist(l_pLevelLayerGridData);
	}


	cLevelLayerGridData*cLevelData::GetListChildByUniqueID(uint64 e_uiID)
	{
		for( int i=0;i<this->Count();++i )
		{
			cLevelLayerGridData*l_pData = (*this)[i]->GetListChildByUniqueID(e_uiID);
			if( l_pData )
				return l_pData;
		}
		return 0;
	}

	bool	cLevelData::ExportTemplateFile(const char*e_strFileName)
	{
		TiXmlDocument l_Doc;
		TiXmlElement*l_pTemplate = new TiXmlElement( L"Template" );
		l_Doc.LinkEndChild(l_pTemplate);
		for(int i=0;i<this->m_pTemplateList->Count();++i)
		{
			cLevelLayerGridData*l_pLevelLayerGridData = (*m_pTemplateList)[i];
			TiXmlElement*l_pTemplateNodeElement = 0;
			if(l_pLevelLayerGridData->GetImage()->Type() == cPuzzleImageUnit::TypeID)
			{
				l_pTemplateNodeElement = new TiXmlElement(L"PITemplate");
				l_pTemplateNodeElement->SetAttribute(L"PI",l_pLevelLayerGridData->GetImage()->GetOwner()->GetName());
			}
			else
			if(l_pLevelLayerGridData->GetImage()->Type() == cMPDI::TypeID)
			{
				l_pTemplateNodeElement = new TiXmlElement(L"MPDITemplate");
				l_pTemplateNodeElement->SetAttribute(L"MPDI",l_pLevelLayerGridData->GetImage()->GetOwner()->GetName());
			}
			else
			if(l_pLevelLayerGridData->GetImage()->Type() == cBaseImage::TypeID)
			{
				l_pTemplateNodeElement = new TiXmlElement(L"BaseImageTemplate");
				l_pTemplateNodeElement->SetAttribute(L"NoImage",l_pLevelLayerGridData->GetImage()->GetName());
			}
			else
			{
				assert(0);
			}
			l_pTemplateNodeElement->SetAttribute(L"Image",l_pLevelLayerGridData->GetImage()->GetName());
			l_pTemplateNodeElement->SetAttribute(L"Name",l_pLevelLayerGridData->GetName());
			if( l_pLevelLayerGridData->GetTemplateDataNode() )
				l_pTemplateNodeElement->SetAttribute(L"TemplateDataNode",l_pLevelLayerGridData->GetTemplateDataNode()->GetName());
			l_pTemplate->LinkEndChild(l_pTemplateNodeElement);
		}
		return l_Doc.SaveFile(e_strFileName);
	}

	bool	cLevelData::ExportResourceFile(const char*e_strFileName)
	{
		cNamedTypedObjectVector<NamedTypedObject>		l_ImageResourceTempForExportPI;
		cNamedTypedObjectVector<NamedTypedObject>		l_ImageResourceTempForExportMPDIList;
		cNamedTypedObjectVector<cBaseImage>		l_ImageResourceTempForNoImage;
		l_ImageResourceTempForExportPI.SetFromResource(true);
		l_ImageResourceTempForExportMPDIList.SetFromResource(true);
		l_ImageResourceTempForNoImage.SetFromResource(true);
		for(int i=0;i<this->m_pTemplateList->Count();++i)
		{
			cLevelLayerGridData*l_pLevelLayerGridData = m_pTemplateList->GetObject(i);
			cRenderObject*l_pRenderObjectBehavior = l_pLevelLayerGridData->GetImage();
			if(l_pRenderObjectBehavior->Type() == cPuzzleImageUnit::TypeID)
			{
				cPuzzleImage*l_pPuzzleImage = dynamic_cast<cPuzzleImage*>(l_pRenderObjectBehavior->GetOwner());
				l_ImageResourceTempForExportPI.AddObject(l_pPuzzleImage);
			}
			else
			if(l_pRenderObjectBehavior->Type() == cMPDI::TypeID)
			{
				cMPDIList*l_pMPDIList = dynamic_cast<cMPDIList*>(l_pRenderObjectBehavior->GetOwner());
				l_ImageResourceTempForExportMPDIList.AddObject(l_pMPDIList);
			}
			else
			if(l_pRenderObjectBehavior->Type() == cBaseImage::TypeID)
			{
				cBaseImage*l_pEmptyImage = dynamic_cast<cBaseImage*>(l_pRenderObjectBehavior);
				l_ImageResourceTempForNoImage.AddObject(l_pEmptyImage);
			}
			else
			{
				UT::ErrorMsg(l_pRenderObjectBehavior->Type(),L"not support format");
			}
		}
	//		<PI FileName="aa.pi" />
	//		<MPDI FileName="aa.mpdi" />
		TiXmlDocument l_Doc;
		TiXmlElement * l_pImageList = new TiXmlElement( L"ImageResource" );
		l_Doc.LinkEndChild(l_pImageList);
		std::wstring	l_strFileName;
		for( int i=0;i<l_ImageResourceTempForExportPI.Count();++i )
		{
			l_strFileName = l_ImageResourceTempForExportPI[i]->GetName();
			l_strFileName += L".pi";
			TiXmlElement*l_pImage = new TiXmlElement( L"PI" );
			l_pImage->SetAttribute(L"FileName",l_strFileName.c_str());
			l_pImageList->LinkEndChild(l_pImage);
		}
		for( int i=0;i<l_ImageResourceTempForExportMPDIList.Count();++i )
		{
			l_strFileName = UT::CharToWchar(cMPDIList::GetFileName(l_ImageResourceTempForExportMPDIList[i]->GetName()));
			TiXmlElement*l_pImage = new TiXmlElement( L"MPDI" );
			l_pImage->SetAttribute(L"FileName",l_strFileName.c_str());
			l_pImageList->LinkEndChild(l_pImage);		
		}
		for( int i=0;i<l_ImageResourceTempForNoImage.Count();++i )
		{
			TiXmlElement*l_pImage = new TiXmlElement( L"NoImage" );
			cBaseImage*l_pBaseImage = l_ImageResourceTempForNoImage[i];
			l_pImage->SetAttribute(L"GridSize",ValueToStringW(Vector2(l_pBaseImage->GetWidth(),l_pBaseImage->GetHeight())));
			l_pImage->SetAttribute(L"Name",l_pBaseImage->GetName());
			l_pImageList->LinkEndChild(l_pImage);
		}
		return l_Doc.SaveFile(e_strFileName);
	}

	bool	cLevelData::ExportTemplateDataNodeFile(const char*e_strFileName)
	{
		TiXmlDocument l_Doc;
		TiXmlElement*l_pTemplate = new TiXmlElement( L"ExportEventDataNodeFile" );
		l_Doc.LinkEndChild(l_pTemplate);
		for(int i=0;i<this->m_TemplateDataNodeList.Count();++i)
		{
			cEventDataNode*l_pEventDataNode = m_TemplateDataNodeList[i];
			TiXmlElement*l_pEventDataNodeElement = new TiXmlElement(L"TemplateDataNode");
			l_pEventDataNodeElement->SetAttribute(L"Name",ValueToStringW(l_pEventDataNode->GetName()));
			TiXmlElement*l_pCloneElement = (TiXmlElement*)l_pEventDataNode->GetDataNode()->Clone();
			l_pEventDataNodeElement->LinkEndChild(l_pCloneElement);
			l_pTemplate->LinkEndChild(l_pEventDataNodeElement);
		}
		return l_Doc.SaveFile(e_strFileName);	
	}

	bool	cLevelData::ExportEventDataNodeFile(const char*e_strFileName)
	{
		TiXmlDocument l_Doc;
		TiXmlElement*l_pTemplate = new TiXmlElement( L"ExportEventDataNodeFile" );
		l_Doc.LinkEndChild(l_pTemplate);
		for(int i=0;i<this->m_EventDataNodeList.Count();++i)
		{
			cEventDataNode*l_pEventDataNode = m_EventDataNodeList[i];
			TiXmlElement*l_pEventDataNodeElement = new TiXmlElement(L"EventDataNode");
			l_pEventDataNodeElement->SetAttribute(L"Name",ValueToStringW(l_pEventDataNode->GetUniqueID()));
			TiXmlElement*l_pCloneElement = (TiXmlElement*)l_pEventDataNode->GetDataNode()->Clone();
			l_pEventDataNodeElement->LinkEndChild(l_pCloneElement);
			l_pTemplate->LinkEndChild(l_pEventDataNodeElement);
		}
		return l_Doc.SaveFile(e_strFileName);
	}

	bool	cLevelData::Export(const char*e_strFileName,bool e_bDoYInvert,bool e_bAlsoExportXInvert)
	{
		if( !m_pTemplateList->Count() )
			return false;
		TiXmlDocument l_Doc;
		TiXmlElement * l_pRoot = new TiXmlElement( L"Root" );
		l_Doc.LinkEndChild( l_pRoot );
		std::string	l_strDirectory = UT::GetDirectoryWithoutFileName(e_strFileName);
		std::string	l_strFileName;
		std::string	l_MainFileName = UT::GetFileNameWithoutFullPath(e_strFileName);
#ifdef WIN32
		if( !UT::IsFileExists(e_strFileName) )
		{
			l_strDirectory += l_MainFileName;
			int	l_iValue = _mkdir(l_strDirectory.c_str());
			l_strDirectory += "/";
		}
#endif

		//if(this->m_EventDataNodeList.Count())
		{
			const WCHAR*l_strEventDataNodeFileName = L"EventDataNode";
			l_strFileName = l_MainFileName;	l_strFileName += UT::WcharToChar(l_strEventDataNodeFileName); l_strFileName += ".xml";
			l_pRoot->SetAttribute(l_strEventDataNodeFileName,UT::CharToWchar(l_strFileName.c_str()));
			l_strFileName = l_strDirectory;		l_strFileName += l_MainFileName;	l_strFileName += UT::WcharToChar(l_strEventDataNodeFileName); l_strFileName += ".xml";
			if( !e_bDoYInvert )
				ExportEventDataNodeFile(l_strFileName.c_str());
		}
		//if(this->m_pTemplateList->Count())
		{
			const WCHAR*l_strTemplateFileName = L"TemplateFile";
			const WCHAR*l_strResourceDataName = L"ResourceData";
			const WCHAR*l_strTemplateDataNodeFile = L"TemplateDataNodeFile";
			l_strFileName = l_strDirectory;		l_strFileName += l_MainFileName;	l_strFileName += UT::WcharToChar(l_strTemplateDataNodeFile); l_strFileName += ".xml";
			if( !e_bDoYInvert )
				ExportTemplateDataNodeFile(l_strFileName.c_str());
			l_strFileName = l_MainFileName;	l_strFileName += UT::WcharToChar(l_strTemplateDataNodeFile); l_strFileName += ".xml";
			l_pRoot->SetAttribute(l_strTemplateDataNodeFile,UT::CharToWchar(l_strFileName.c_str()));
			l_strFileName = l_MainFileName;	l_strFileName += UT::WcharToChar(l_strResourceDataName); l_strFileName += ".xml";
			l_pRoot->SetAttribute(l_strResourceDataName,UT::CharToWchar(l_strFileName.c_str()));
			l_strFileName = l_strDirectory;		l_strFileName += l_MainFileName;	l_strFileName += UT::WcharToChar(l_strResourceDataName); l_strFileName += ".xml";
			if( !e_bDoYInvert )
				ExportResourceFile(l_strFileName.c_str());
			l_strFileName = l_MainFileName;	l_strFileName += UT::WcharToChar(l_strTemplateFileName); l_strFileName += ".xml";
			l_pRoot->SetAttribute(l_strTemplateFileName,UT::CharToWchar(l_strFileName.c_str()));
			l_strFileName = l_strDirectory;		l_strFileName += l_MainFileName;	l_strFileName += UT::WcharToChar(l_strTemplateFileName); l_strFileName += ".xml";
			if( !e_bDoYInvert )
				ExportTemplateFile(l_strFileName.c_str());
		}
		if( m_pEventManager )
		{
			//l_pRoot->SetAttribute(L"Event",L"Event.xml");
		}
		for(int i=0;i<this->Count();++i)
		{
			TiXmlElement*l_pStageDataElement = new TiXmlElement( L"LevelData" );
			cLevelLayerList*l_pLevelLayerData = (*this)[i];
			l_pStageDataElement->SetAttribute(L"Name",l_pLevelLayerData->GetName());
			l_pStageDataElement->SetAttribute(L"LayerCount",l_pLevelLayerData->Count());
			l_pStageDataElement->SetAttribute(L"StartPos",ValueToStringW(l_pLevelLayerData->GetPos()));
			l_pStageDataElement->SetAttribute(L"Size",ValueToStringW(l_pLevelLayerData->GetSize()));
			l_pStageDataElement->SetAttribute(L"GridSize",ValueToStringW(l_pLevelLayerData->GetGridSize()));
			l_pRoot->LinkEndChild(l_pStageDataElement);
			for( int j=0;j<l_pLevelLayerData->Count();++j )
			{
				TiXmlElement*l_pStageDataLayerElement = new TiXmlElement( L"LevelDataLayer" );
				cLayer*l_pLayer = l_pLevelLayerData->GetObject(j);
				l_pStageDataLayerElement->SetAttribute(L"Name",l_pLayer->GetName());
				l_pStageDataLayerElement->SetAttribute(L"Count",l_pLayer->Count());
				l_pStageDataElement->LinkEndChild(l_pStageDataLayerElement);
				for( int k=0;k<l_pLayer->Count();++k )
				{
					TiXmlElement*l_pDataElement = new TiXmlElement( L"LevelLayerGrid" );
					cLevelLayerGridData*l_pLevelLayerGridData = l_pLayer->GetObject(k);
					l_pDataElement->SetAttribute(L"TemplateName",l_pLevelLayerGridData->GetName());
					Vector3 l_vPos = l_pLevelLayerGridData->GetPos();
					if( e_bDoYInvert )
					{
						l_vPos.y *= -1;
						if( e_bAlsoExportXInvert )
						{
							l_vPos.x *= -1;
						}
						l_pDataElement->SetAttribute(L"Pos",ValueToStringW(l_vPos));
					}
					else
						l_pDataElement->SetAttribute(L"Pos",ValueToStringW(l_pLevelLayerGridData->GetPos()));
					if(l_pLevelLayerGridData->GetEventDataNode())
					{
						l_pDataElement->SetAttribute(L"EventDataNode",ValueToStringW(l_pLevelLayerGridData->GetEventDataNode()->GetUniqueID()));
					}
					l_pStageDataLayerElement->LinkEndChild(l_pDataElement);
				}
			}
		}
		l_strDirectory += UT::GetFileNameWithoutFullPath(e_strFileName,false);
		l_Doc.SaveFile(l_strDirectory.c_str());
		return true;
	}

//end if namespace
}