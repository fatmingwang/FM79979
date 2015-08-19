#include "StdAfx.h"
#include "ObjectDataForm.h"
//all template data is here


#define	USER_DEFINE_DATA		"UserDefineData"
//#define	USER_DEFINE_DATA_STAGE	"Stage"
//#define	USER_DEFINE_DATA_LAYER	"Layer"
//#define	USER_DEFINE_DATA_INDEX	"Index"
namespace LevelEditor
{
	void	ObjectDataForm::SaveTreeNodeFile(String^e_strFileName,System::Collections::Hashtable^e_pHashTable,bool e_bUserDefineData)
	{
		if( !e_pHashTable )
			return;
		 //for all template data
		 String^l_strFileName = e_strFileName;
		 GCFORM::TreeNode^ l_pTemplateNode = gcnew GCFORM::TreeNode();
		 l_pTemplateNode->Text = "Root";
		 if( e_bUserDefineData )
		 {
			 for each (uint64 l_uiID in e_pHashTable->Keys)
			 {
				cLevelLayerGridData*l_pMap_Cell_ObjectData = m_pLevelEditorApp->m_pLevelData->GetListChildByUniqueID(l_uiID);
				cLayer*l_pMapLayer = dynamic_cast<cLayer*>(l_pMap_Cell_ObjectData->GetOwner());
				cLevelLayerList*l_pMap_CellData = dynamic_cast<cLevelLayerList*>(l_pMapLayer->GetOwner());

				GCFORM::TreeNode^l_pRootNode = gcnew GCFORM::TreeNode("UserDefineData");
				int	l_iIndex = l_pMapLayer->GetObjectIndexByPointer(l_pMap_Cell_ObjectData);
				l_pRootNode->Nodes->Add("Stage")->Name = DNCT::WcharToGcstring(l_pMap_CellData->GetName());
				l_pRootNode->Nodes->Add("Layer")->Name = DNCT::WcharToGcstring(l_pMapLayer->GetName());
				l_pRootNode->Nodes->Add("Index")->Name = l_iIndex.ToString();
				TreeNode^l_pTreeNode = (TreeNode^)e_pHashTable[l_uiID];
				l_pRootNode->Nodes->Add((TreeNode^)l_pTreeNode->Clone());
				l_pTemplateNode->Nodes->Add(l_pRootNode);
			 }
		 }
		 else
		 {
			 for each (TreeNode^ t in e_pHashTable->Values)
			 {
				if( t )
					l_pTemplateNode->Nodes->Add((TreeNode^)t->Clone());
			 }
		 }
		//SaveTreeNodeToFile(l_pTemplateNode,l_strFileName);
		 XmlDocument^ d = gcnew XmlDocument();
		 XmlNode^ n = d->CreateNode(XmlNodeType::Element,"root1","");
		 //for each (TreeNode^ t in m_TemplateNameAndAttributeNode->Values)
		 {
			 n->AppendChild(DNCT::getXmlNode(l_pTemplateNode, d));
		 }
		 for each(XmlNode^ e_ChildNode in n->ChildNodes)
			d->AppendChild(e_ChildNode);
		  d->Save(l_strFileName);
	 }



	System::Void ObjectDataForm::MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if( !Main_splitContainer->Focused )
			Main_splitContainer->Focus();
		GCFORM::MouseButtons l_MouseButton = e->Button;
		this->m_pLevelEditorApp->GetOrthogonalCamera()->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
			,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)Main_splitContainer->Panel1->Size.Width,(float)Main_splitContainer->Panel1->Size.Height));
		this->m_pLevelEditorApp->MouseDown(e->X,e->Y);
	}

	System::Void ObjectDataForm::MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		GCFORM::MouseButtons l_MouseButton = e->Button;
		this->m_pLevelEditorApp->GetOrthogonalCamera()->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
			,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)Main_splitContainer->Panel1->Size.Width,(float)Main_splitContainer->Panel1->Size.Height));
		this->m_pLevelEditorApp->MouseMove(e->X,e->Y,LayerObjectData_listBox);

	}

	System::Void ObjectDataForm::MyMouseDoubleClick(System::Object^  sender, System::EventArgs^  e)
	{
		this->m_pLevelEditorApp->MouseDoubleClick(LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView,m_UniqueUserDefineDataForMap_Cell_ObjectData,m_TemplateNameAndAttributeNode);
	}

	System::Void ObjectDataForm::MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		GCFORM::MouseButtons l_MouseButton = e->Button;
		this->m_pLevelEditorApp->GetOrthogonalCamera()->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
			,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,
			e->Delta,e->X,e->Y,
			Vector2((float)Main_splitContainer->Panel1->Size.Width,(float)Main_splitContainer->Panel1->Size.Height));
		//m_pLevelEditorApp->m_pOrthogonalCamera->Render();
		if( e->Button != GCFORM::MouseButtons::Left )
			return;
		if( !this->m_pLevelEditorApp->m_pCurrentStageEditData->GetCurrentList() )
		{
			WARNING_MSG("please add or select layer first!");
			return;
		}
		this->m_pLevelEditorApp->MouseUp(e->X,e->Y,m_UniqueUserDefineDataForMap_Cell_ObjectData,LayerObjectData_listBox,this->Template_listBox);
	}

	System::Void ObjectDataForm::Render()
	{
		m_pLevelEditorApp->m_svViewPortSize.x = (float)Main_splitContainer->Panel1->Width;
		m_pLevelEditorApp->m_svViewPortSize.y = (float)Main_splitContainer->Panel1->Height;
		m_pLevelEditorApp->m_vStartPos = Vector2((float)StartX_numericUpDown->Value,(float)StartY_numericUpDown->Value);
		m_pLevelEditorApp->m_bShowGrid = showGridToolStripMenuItem->Checked;
		m_pLevelEditorApp->m_vMapWidth = Vector2((float)MapWidth_numericUpDown->Value,(float)MapHeight_numericUpDown->Value);
		m_pLevelEditorApp->m_bShowResolutionBorder = showResolutionBorderToolStripMenuItem->Checked;
		m_pLevelEditorApp->m_eMoudeMode = (eMouseMode)MouseMode_comboBox->SelectedIndex;
		m_pLevelEditorApp->m_bPlaceObjectByImageCenter = PlaceObjectByImageCenter_checkBox->Checked;
		m_pLevelEditorApp->m_bAutoAllignGrid = autoAllignGridToolStripMenuItem->Checked;
		m_pLevelEditorApp->m_bAllowSamepositionObject = AllowSamepositionObject_checkBox->Checked;
		m_pLevelEditorApp->m_SelectedLayerObjects.clear();
		for each (int l_iIndex in LayerObjectData_listBox->SelectedIndices)
			m_pLevelEditorApp->m_SelectedLayerObjects.push_back(l_iIndex);
		this->m_pLevelEditorApp->Run();

	}

	//System::Void ObjectDataForm::RemoveUserDefineNode(cLevelLayerGridData*e_pMap_Cell_ObjectData)
	//{
	//	if(this->m_UniqueUserDefineDataForMap_Cell_ObjectData->ContainsKey(e_pMap_Cell_ObjectData->GetUniqueID()))
	//	{
	//		this->m_UniqueUserDefineDataForMap_Cell_ObjectData->Remove(e_pMap_Cell_ObjectData->GetUniqueID());
	//	}
	//		
	//}

	//System::Void ObjectDataForm::RemoveUserDefineNode(cLevelLayerList*e_pMap_CellData)
	//{
	//	for( int i=0;i<e_pMap_CellData->Count();++i )
	//	{
	//		RemoveUserDefineNode((e_pMap_CellData->GetObject(i)));
	//	}
	//}

	//System::Void ObjectDataForm::RemoveUserDefineNode(cLayer*e_pMapLayer)
	//{
	//	for( int i=0;i<e_pMapLayer->Count();++i )
	//	{
	//		RemoveUserDefineNode((*e_pMapLayer)[i]);
	//	}
	//}

	//System::Void ObjectDataForm::CopyUserDataToNewData(cLevelLayerList*e_pSrc,cLevelLayerList*e_pDest)
	//{
	//	for( int i=0;i<e_pSrc->Count();++i )
	//	{
	//		cLayer*l_pMapLayer = e_pSrc->GetObject(i);
	//		for( int j=0;j<l_pMapLayer->Count();++j )
	//		{
	//			if(l_pMapLayer->GetObject(j)->GetEventDataNode())
	//			{
	//				e_pDest->GetObject(i)->GetObject(j)->SetEventDataNode(l_pMapLayer->GetObject(j)->GetTemplateDataNode());
	//				if(!this->m_UniqueUserDefineDataForMap_Cell_ObjectData->ContainsKey( l_pMapLayer->GetObject(j)->GetEventDataNode()->GetUniqueID() ))
	//				{
	//					WARNING_MSG("lost user define data!!!!fuck!!how come!!");
	//				}
	//			}
	//		}
	//	}
	//}

	System::Void ObjectDataForm::TemplateDataInvertorReEdit()
	{
		if( Template_listBox->SelectedIndex != -1 )
		{
			String^l_strEventDataNodeName;
			String^l_strName = Template_listBox->SelectedItem->ToString();
			cLevelData*l_pLevelData = m_pLevelEditorApp->m_pLevelData;
			cLevelLayerGridData*l_pLevelLayerGridData = l_pLevelData->m_pTemplateList->GetObject( DNCT::GcStringToWchar(Template_listBox->SelectedItem->ToString()) );
			if(l_pLevelLayerGridData->GetTemplateDataNode()&&l_pLevelLayerGridData->GetTemplateDataNode()->GetDataNode())
			{
				l_strEventDataNodeName = DNCT::WcharToGcstring(l_pLevelLayerGridData->GetTemplateDataNode()->GetDataNode()->Value());
			}
			GCFORM::TreeNode^l_pNode;
			if( l_strEventDataNodeName && m_TemplateNameAndAttributeNode->ContainsKey(l_strEventDataNodeName))
				l_pNode = (GCFORM::TreeNode^)m_TemplateNameAndAttributeNode[l_strEventDataNodeName];
			ObjectTemplate_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();
			if( l_pNode )
			{
				ObjectTemplate_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Add((GCFORM::TreeNode^)l_pNode->Clone());
				ObjectTemplate_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes[0]->Expand();
			}
			TemplateName_textBox->Text = l_strName;
			cRenderObject*l_pRenderObjectBehavior = this->m_pLevelEditorApp->m_pLevelData->m_pTemplateList->GetObject(DNCT::GcStringToWchar(l_strName))->GetImage();
			if( l_pRenderObjectBehavior && l_pRenderObjectBehavior->Type() == cPuzzleImageUnit::TypeID )
			{
				cPuzzleImageUnit*l_pPIUnit = (cPuzzleImageUnit*)l_pRenderObjectBehavior;
				cPuzzleImage*l_pPI = dynamic_cast<cPuzzleImage*>(l_pPIUnit->GetOwner());
				String^l_strPIName = DNCT::WcharToGcstring(l_pPI->GetName());
				String^l_strPIUnit = DNCT::WcharToGcstring(l_pPIUnit->GetName());
				if( ImageList_treeView->Nodes[l_strPIName] )
				{
					ImageList_treeView->SelectedNode = ImageList_treeView->Nodes[l_strPIName]->Nodes[l_strPIUnit];
					this->ImageList_treeView->SelectedNode->BackColor = System::Drawing::Color::Blue;
					this->ImageList_treeView->SelectedNode->ForeColor = System::Drawing::Color::White;
				}
				
				 if( !Layer_listBox->Items->Count )
				 {
					GridWidth_numericUpDown->Value = System::Decimal(l_pPIUnit->GetWidth());
					GridHeight_numericUpDown->Value = System::Decimal(l_pPIUnit->GetHeight());
				 }
			}
			LayerName_textBox->Text = l_strName;
		}
	}


	void	ObjectDataForm::OpenUserDefineData(String^e_strFileName)
	{
		GCFORM::TreeNode^l_pUserDefineTreeNode = DNCT::OpenXmlFileToNode(e_strFileName);
		if( l_pUserDefineTreeNode )
		for each( GCFORM::TreeNode^l_pNode in l_pUserDefineTreeNode->Nodes )
		{
			if(l_pNode->Text->Equals(USER_DEFINE_DATA))
			{
				assert(l_pNode->Nodes->Count == 4);
				cLevelLayerList*l_pMap_CellData = 0;
				cLayer*l_pMapLayer = 0;
				cLevelLayerGridData*l_pMap_Cell_ObjectData = 0;
				for( int i=0;i<l_pNode->Nodes->Count;++i )
				{
					GCFORM::TreeNode^l_pTargetNode = l_pNode->Nodes[i];
					switch(i)
					{
						case 0:
							l_pMap_CellData = this->m_pLevelEditorApp->m_pLevelData->GetObject(DNCT::GcStringToWchar(l_pTargetNode->Name));
							break;
						case 1:
							l_pMapLayer = l_pMap_CellData->GetObject(DNCT::GcStringToWchar(l_pTargetNode->Name));
							break;
						case 2:
							l_pMap_Cell_ObjectData = l_pMapLayer->GetObject(System::Int16::Parse(l_pTargetNode->Name));
							break;
						case 3:
							assert(0);
							//l_pMap_Cell_ObjectData->SetContainUserDefineData(true);
							this->m_UniqueUserDefineDataForMap_Cell_ObjectData[l_pMap_Cell_ObjectData->GetUniqueID()] = l_pTargetNode->Clone();
							break;
					}
				}
			}
			else
			{
				WARNING_MSG("user define data error!");
			}
		}
	}

	void	ObjectDataForm::OpenQuickApplyXmlFile(String^e_strFileName)
	{
		XMLDataForQuickApply_treeView->Nodes->Clear();
		String^l_strTempDirectory = System::IO::Path::GetDirectoryName(e_strFileName)+"/XmlTemp";
		l_strTempDirectory +="/temp.xml";
		if( System::IO::File::Exists(l_strTempDirectory) )
		{
			GCFORM::TreeNode^l_pTreeNode = DNCT::OpenXmlFileToNode(l_strTempDirectory);
			if( l_pTreeNode )
				XMLDataForQuickApply_treeView->Nodes->Add(l_pTreeNode);
		}
	}

		void	ObjectDataForm::AddEventDataNode(cLevelLayerGridData*e_pLevelLayerGridData,GCFORM::TreeNode^e_pNode)
		{
			if( e_pLevelLayerGridData )
			{
				if(e_pLevelLayerGridData->GetEventDataNode())
				{
					RemoveEventDataNode(e_pLevelLayerGridData);
				}
			}
			else
				return;
			cEventDataNode*l_pEventDataNode = new cEventDataNode();
			GCFORM::TreeNode^l_pCloneNode = (GCFORM::TreeNode^)e_pNode->Clone();
			LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Add(l_pCloneNode);
			m_UniqueUserDefineDataForMap_Cell_ObjectData[l_pEventDataNode->GetUniqueID()] = l_pCloneNode;
			l_pEventDataNode->SetName(ValueToStringW(e_pLevelLayerGridData->GetUniqueID()));
			TiXmlElement*l_pTiXmlElement = DNCT::TreeNodeToTinyXMLNode(l_pCloneNode,0);
			l_pEventDataNode->SetDataNode(l_pTiXmlElement);
			SAFE_DELETE(l_pTiXmlElement);
			m_pLevelEditorApp->m_pLevelData->m_EventDataNodeList.AddObject(l_pEventDataNode);
			e_pLevelLayerGridData->SetEventDataNode(l_pEventDataNode);
		}

	void	ObjectDataForm::RemoveEventDataNode(cLevelLayerGridData*e_pLevelLayerGridData)
{
	if( e_pLevelLayerGridData->GetEventDataNode() )
	{
		m_UniqueUserDefineDataForMap_Cell_ObjectData->Remove(e_pLevelLayerGridData->GetEventDataNode()->GetUniqueID());
		m_pLevelEditorApp->m_pLevelData->m_EventDataNodeList.RemoveObject(e_pLevelLayerGridData->GetEventDataNode());
		e_pLevelLayerGridData->SetEventDataNode(0);
		LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();
	}
}
}