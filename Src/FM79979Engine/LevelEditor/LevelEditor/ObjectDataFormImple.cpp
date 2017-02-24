#include "StdAfx.h"
#include "ObjectDataFormImple.h"
namespace LevelEditor
{
	cObjectDataFormImple::cObjectDataFormImple(ObjectDataForm^e_pObjectDataForm)
	{
		OutputDebugString(L"fix this problem m_iShowSelectedLayer");
		splitContainer1 = e_pObjectDataForm->splitContainer1;
		splitContainer2 = e_pObjectDataForm->splitContainer2;
		menuStrip1 = e_pObjectDataForm->menuStrip1;
		fileToolStripMenuItem = e_pObjectDataForm->fileToolStripMenuItem;
		openToolStripMenuItem = e_pObjectDataForm->openToolStripMenuItem;
		saveToolStripMenuItem = e_pObjectDataForm->saveToolStripMenuItem;
		imageToolStripMenuItem = e_pObjectDataForm->imageToolStripMenuItem;
		openTemplateToolStripMenuItem = e_pObjectDataForm->openTemplateToolStripMenuItem;
		imageToolStripMenuItem1 = e_pObjectDataForm->imageToolStripMenuItem1;
		assistanceToolStripMenuItem = e_pObjectDataForm->assistanceToolStripMenuItem;
		showGridToolStripMenuItem = e_pObjectDataForm->showGridToolStripMenuItem;
		autoAllignGridToolStripMenuItem = e_pObjectDataForm->autoAllignGridToolStripMenuItem;
		XMLDataForQuickApply_treeView = e_pObjectDataForm->XMLDataForQuickApply_treeView;
		//components = e_pObjectDataForm->components;
		UserDefineData__contextMenuStrip = e_pObjectDataForm->UserDefineData__contextMenuStrip;
		refreshToolStripMenuItem = e_pObjectDataForm->refreshToolStripMenuItem;
		addNewNodeFromFileToolStripMenuItem = e_pObjectDataForm->addNewNodeFromFileToolStripMenuItem;
		deleteNodeToolStripMenuItem = e_pObjectDataForm->deleteNodeToolStripMenuItem;
		label1 = e_pObjectDataForm->label1;
		ObjectTemplate_xmlNodeEditComponent = e_pObjectDataForm->ObjectTemplate_xmlNodeEditComponent;
		AllStage_listBox = e_pObjectDataForm->AllStage_listBox;
		StageName_textBox = e_pObjectDataForm->StageName_textBox;
		AddStage_button = e_pObjectDataForm->AddStage_button;
		DelStage_button = e_pObjectDataForm->DelStage_button;
		flowLayoutPanel1 = e_pObjectDataForm->flowLayoutPanel1;
		ReRditStagebutton = e_pObjectDataForm->ReRditStagebutton;
		camerResetToolStripMenuItem = e_pObjectDataForm->camerResetToolStripMenuItem;
		showResolutionBorderToolStripMenuItem = e_pObjectDataForm->showResolutionBorderToolStripMenuItem;
		backgroundImageToolStripMenuItem = e_pObjectDataForm->backgroundImageToolStripMenuItem;
		label10 = e_pObjectDataForm->label10;
		label4 = e_pObjectDataForm->label4;
		StartY_numericUpDown = e_pObjectDataForm->StartY_numericUpDown;
		StartX_numericUpDown = e_pObjectDataForm->StartX_numericUpDown;
		AddToLayer_button = e_pObjectDataForm->AddToLayer_button;
		LayerDown_button = e_pObjectDataForm->LayerDown_button;
		LayerUp_button = e_pObjectDataForm->LayerUp_button;
		Layer_listBox = e_pObjectDataForm->Layer_listBox;
		DeleteLayer_button = e_pObjectDataForm->DeleteLayer_button;
		autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem = e_pObjectDataForm->autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem;
		cameraResetToolStripMenuItem = e_pObjectDataForm->cameraResetToolStripMenuItem;
		LayerObjectData_listBox = e_pObjectDataForm->LayerObjectData_listBox;
		DeleteObject_button = e_pObjectDataForm->DeleteObject_button;
		showWarningMsgToolStripMenuItem = e_pObjectDataForm->showWarningMsgToolStripMenuItem;
		groupBox1 = e_pObjectDataForm->groupBox1;
		label3 = e_pObjectDataForm->label3;
		AllowSamepositionObject_checkBox = e_pObjectDataForm->AllowSamepositionObject_checkBox;
		GridHeight_numericUpDown = e_pObjectDataForm->GridHeight_numericUpDown;
		label6 = e_pObjectDataForm->label6;
		GridWidth_numericUpDown = e_pObjectDataForm->GridWidth_numericUpDown;
		label7 = e_pObjectDataForm->label7;
		label2 = e_pObjectDataForm->label2;
		MapHeight_numericUpDown = e_pObjectDataForm->MapHeight_numericUpDown;
		MapWidth_numericUpDown = e_pObjectDataForm->MapWidth_numericUpDown;
		AddObjectToStage_button = e_pObjectDataForm->AddObjectToStage_button;
		label5 = e_pObjectDataForm->label5;
		label15 = e_pObjectDataForm->label15;
		DelTemplate_button = e_pObjectDataForm->DelTemplate_button;
		label13 = e_pObjectDataForm->label13;
		Template_listBox = e_pObjectDataForm->Template_listBox;
		AddTemplate_button = e_pObjectDataForm->AddTemplate_button;
		AddObjectY_numericUpDown = e_pObjectDataForm->AddObjectY_numericUpDown;
		DelImage_button = e_pObjectDataForm->DelImage_button;
		AddObjectX_numericUpDown = e_pObjectDataForm->AddObjectX_numericUpDown;
		AddImage_button = e_pObjectDataForm->AddImage_button;
		TemplateName_textBox = e_pObjectDataForm->TemplateName_textBox;
		Clear_button = e_pObjectDataForm->Clear_button;
		ReEditTemplate_button = e_pObjectDataForm->ReEditTemplate_button;
		label17 = e_pObjectDataForm->label17;
		LayerName_textBox = e_pObjectDataForm->LayerName_textBox;
		ImageList_treeView = e_pObjectDataForm->ImageList_treeView;
		UserDefineData_contextMenuStrip = e_pObjectDataForm->UserDefineData_contextMenuStrip;
		addToolStripMenuItem = e_pObjectDataForm->addToolStripMenuItem;
		editToolStripMenuItem = e_pObjectDataForm->editToolStripMenuItem;
		deleteToolStripMenuItem = e_pObjectDataForm->deleteToolStripMenuItem;
		LayerData_contextMenuStrip = e_pObjectDataForm->LayerData_contextMenuStrip;
		selectAllThisTypeTemplateToolStripMenuItem = e_pObjectDataForm->selectAllThisTypeTemplateToolStripMenuItem;
		refreshUserDataFromToolStripMenuItem = e_pObjectDataForm->refreshUserDataFromToolStripMenuItem;
		label14 = e_pObjectDataForm->label14;
		AddObjectZ_numericUpDown = e_pObjectDataForm->AddObjectZ_numericUpDown;
		timer1 = e_pObjectDataForm->timer1;
		Main_splitContainer = e_pObjectDataForm->Main_splitContainer;
		PlaceObjectByImageCenter_checkBox = e_pObjectDataForm->PlaceObjectByImageCenter_checkBox;
		StageData_groupBox = e_pObjectDataForm->StageData_groupBox;
		TemplateList_groupBox = e_pObjectDataForm->TemplateList_groupBox;
		XMLData_groupBox = e_pObjectDataForm->XMLData_groupBox;
		tabControl1 = e_pObjectDataForm->tabControl1;
		tabPage1 = e_pObjectDataForm->tabPage1;
		tabPage2 = e_pObjectDataForm->tabPage2;
		EventData_flowLayoutPanel = e_pObjectDataForm->EventData_flowLayoutPanel;
		LayerVisible_checkBox = e_pObjectDataForm->LayerVisible_checkBox;
		MouseMode_comboBox = e_pObjectDataForm->MouseMode_comboBox;
		MouseMode_label = e_pObjectDataForm->MouseMode_label;
		LayerList_groupBox = e_pObjectDataForm->LayerList_groupBox;
		Object_groupBox = e_pObjectDataForm->Object_groupBox;
		assignToObjectEventDataToolStripMenuItem = e_pObjectDataForm->assignToObjectEventDataToolStripMenuItem;
		NoImage_button = e_pObjectDataForm->NoImage_button;
		LevelLayerGridEventDataNode_xmlNodeEditComponent = e_pObjectDataForm->LevelLayerGridEventDataNode_xmlNodeEditComponent;
		InGameRender_checkBox = e_pObjectDataForm->InGameRender_checkBox;
		m_pForm = e_pObjectDataForm->m_pForm;
		//
		this->timer1->Tick += gcnew System::EventHandler(this, &cObjectDataFormImple::timer1_Tick);
		this->XMLDataForQuickApply_treeView->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &cObjectDataFormImple::treeView1_DragEnter);
		this->XMLDataForQuickApply_treeView->DoubleClick += gcnew System::EventHandler(this, &cObjectDataFormImple::AssignTemplateData_Click);
		this->addNewNodeFromFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::addNewNodeFromFileToolStripMenuItem_Click);
		this->XMLDataForQuickApply_treeView->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &cObjectDataFormImple::treeView1_DragDrop);
		this->deleteNodeToolStripMenuItem->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::deleteNodeToolStripMenuItem_Click);
		this->XMLDataForQuickApply_treeView->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &cObjectDataFormImple::treeView1_KeyUp);
		this->AddImage_button->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::AddImage_button_Click);
		this->AddTemplate_button->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::AddTemplate_button_Click);
		this->AddTemplate_button->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::AddTemplate_button_Click);
		this->DelTemplate_button->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::AddTemplate_button_Click);
		this->DelStage_button->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::AddStage_button_Click);
		this->ReRditStagebutton->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::AddStage_button_Click);
		this->AddStage_button->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::AddStage_button_Click);
		this->GridHeight_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cObjectDataFormImple::GridHeight_numericUpDown_ValueChanged);
		this->GridWidth_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cObjectDataFormImple::GridHeight_numericUpDown_ValueChanged);
		this->showWarningMsgToolStripMenuItem->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::showGridToolStripMenuItem_Click);
		this->autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::showGridToolStripMenuItem_Click);
		this->showResolutionBorderToolStripMenuItem->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::showGridToolStripMenuItem_Click);
		this->camerResetToolStripMenuItem->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::showGridToolStripMenuItem_Click);
		this->autoAllignGridToolStripMenuItem->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::showGridToolStripMenuItem_Click);
		this->showGridToolStripMenuItem->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::showGridToolStripMenuItem_Click);
		this->MapHeight_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cObjectDataFormImple::Row_numericUpDown_ValueChanged);
		this->MapWidth_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cObjectDataFormImple::Row_numericUpDown_ValueChanged);
		this->Template_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &cObjectDataFormImple::Template_listBox_SelectedIndexChanged);
		this->Clear_button->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::Clear_button_Click);
		this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::saveToolStripMenuItem_Click);
		this->backgroundImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::backgroundImageToolStripMenuItem_Click);
		this->ReEditTemplate_button->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::ReEditTemplate_button_Click);
		this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::openToolStripMenuItem_Click);
		this->StartY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cObjectDataFormImple::StartX_numericUpDown_ValueChanged);
		this->NoImage_button->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::NoImage_button_Click);
		this->ImageList_treeView->BeforeSelect += gcnew System::Windows::Forms::TreeViewCancelEventHandler(this, &cObjectDataFormImple::ImageList_treeView_BeforeSelect);
		this->ImageList_treeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &cObjectDataFormImple::ImageList_treeView_AfterSelect);
		this->ImageList_treeView->MouseLeave += gcnew System::EventHandler(this, &cObjectDataFormImple::ImageList_treeView_MouseLeave);
		this->XMLDataForQuickApply_treeView->ItemDrag += gcnew System::Windows::Forms::ItemDragEventHandler(this, &cObjectDataFormImple::XMLDataForQuickApply_treeView_ItemDrag);
		this->assignToObjectEventDataToolStripMenuItem->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::assignToObjectEventDataToolStripMenuItem_Click);
		this->InGameRender_checkBox->CheckedChanged += gcnew System::EventHandler(this, &cObjectDataFormImple::InGameRender_checkBox_CheckedChanged);
		this->Layer_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &cObjectDataFormImple::Layer_listBox_SelectedIndexChanged);
		this->LayerUp_button->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::LayerDown_button_Click);
		this->DeleteLayer_button->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::AddToLayer_button_Click);
		this->LayerDown_button->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::LayerDown_button_Click);
		this->AddToLayer_button->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::AddToLayer_button_Click);
		this->LayerVisible_checkBox->CheckedChanged += gcnew System::EventHandler(this, &cObjectDataFormImple::InGameRender_checkBox_CheckedChanged);
		this->AllStage_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &cObjectDataFormImple::AllStage_listBox_SelectedIndexChanged);
		this->AllStage_listBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &cObjectDataFormImple::AllStage_listBox_KeyUp);
		this->AllStage_listBox->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &cObjectDataFormImple::AllStage_listBox_MouseDoubleClick);
		this->StartX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cObjectDataFormImple::StartX_numericUpDown_ValueChanged);
		this->AddObjectToStage_button->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::AddObjectToStage_button_Click);
		this->DeleteObject_button->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::DeleteObject_button_Click);
		this->LayerObjectData_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &cObjectDataFormImple::LayerObjectData_listBox_SelectedIndexChanged);
		this->cameraResetToolStripMenuItem->Click += gcnew System::EventHandler(this, &cObjectDataFormImple::cameraResetToolStripMenuItem_Click);
		//this->Load += gcnew System::EventHandler(this, &cObjectDataFormImple::ObjectDataForm_Load);
		this->LevelLayerGridEventDataNode_xmlNodeEditComponent->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &cObjectDataFormImple::ObjectData_treeView_DragEnter);
		this->Main_splitContainer->Panel1->SizeChanged += gcnew System::EventHandler(this, &cObjectDataFormImple::Main_splitContainer_Panel1_SizeChanged);
		//
		 splitContainer1->SplitterDistance = 213;
		MouseMode_comboBox->SelectedIndex = 0;
		m_pLevelEditorApp = new cLevelEditorApp((HWND)Main_splitContainer->Panel1->Handle.ToPointer());
		m_pLevelEditorApp->Init();
			

		LevelLayerGridEventDataNode_xmlNodeEditComponent->m_TreeViewDeleteOrCreateFunction += gcnew System::EventHandler(this,&cObjectDataFormImple::LevelLayerGridEventDataNode_xmlNodeEditComponentTreeViewDeleteOrCreateFunction);
		Main_splitContainer->Panel1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &cObjectDataFormImple::MyMouseMove);
		Main_splitContainer->Panel1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &cObjectDataFormImple::MyMouseDown);
		Main_splitContainer->Panel1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &cObjectDataFormImple::MyMouseUp);
		Main_splitContainer->Panel1->DoubleClick += gcnew System::EventHandler(this, &cObjectDataFormImple::MyMouseDoubleClick);
		//make sure u hvae call focus while the panel is mouse down
		Main_splitContainer->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &cObjectDataFormImple::MyMouseUp);
		m_pForm->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &cObjectDataFormImple::MyKeyPress);


		m_UniqueUserDefineDataForMap_Cell_ObjectData = gcnew System::Collections::Hashtable;
		m_TemplateNameAndAttributeNode = gcnew System::Collections::Hashtable;

		ToolTip^l_pToolTip = gcnew ToolTip();
		l_pToolTip->SetToolTip( TemplateList_groupBox, "select images and data template to add new data" );
		this->timer1->Enabled = true;
	}
	cObjectDataFormImple::~cObjectDataFormImple()
	{
		if( m_UniqueUserDefineDataForMap_Cell_ObjectData )
			delete m_UniqueUserDefineDataForMap_Cell_ObjectData;
		if(m_TemplateNameAndAttributeNode)
			delete m_TemplateNameAndAttributeNode;
		SAFE_DELETE(m_pLevelEditorApp);	
	}

	void	cObjectDataFormImple::SaveTreeNodeFile(String^e_strFileName,System::Collections::Hashtable^e_pHashTable,bool e_bUserDefineData)
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



	System::Void cObjectDataFormImple::MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if( !Main_splitContainer->Focused )
			Main_splitContainer->Focus();
		GCFORM::MouseButtons l_MouseButton = e->Button;
		this->m_pLevelEditorApp->GetOrthogonalCamera()->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
			,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)Main_splitContainer->Panel1->Size.Width,(float)Main_splitContainer->Panel1->Size.Height));
		this->m_pLevelEditorApp->MouseDown(e->X,e->Y);
	}

	System::Void cObjectDataFormImple::MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		GCFORM::MouseButtons l_MouseButton = e->Button;
		this->m_pLevelEditorApp->GetOrthogonalCamera()->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
			,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)Main_splitContainer->Panel1->Size.Width,(float)Main_splitContainer->Panel1->Size.Height));
		this->m_pLevelEditorApp->MouseMove(e->X,e->Y,LayerObjectData_listBox);

	}

	System::Void cObjectDataFormImple::MyMouseDoubleClick(System::Object^  sender, System::EventArgs^  e)
	{
		this->m_pLevelEditorApp->MouseDoubleClick(LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView,m_UniqueUserDefineDataForMap_Cell_ObjectData,m_TemplateNameAndAttributeNode);
	}

	System::Void cObjectDataFormImple::MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
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

	System::Void cObjectDataFormImple::Render()
	{
		m_pLevelEditorApp->m_svViewPortSize.x = 0;
		m_pLevelEditorApp->m_svViewPortSize.y = 0;
		m_pLevelEditorApp->m_svViewPortSize.z = (float)Main_splitContainer->Panel1->Width;
		m_pLevelEditorApp->m_svViewPortSize.w = (float)Main_splitContainer->Panel1->Height;
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

		this->m_pLevelEditorApp->m_iShowSelectedLayer = Layer_listBox->SelectedIndex;
		this->m_pLevelEditorApp->Run();

	}

	//System::Void cObjectDataFormImple::RemoveUserDefineNode(cLevelLayerGridData*e_pMap_Cell_ObjectData)
	//{
	//	if(this->m_UniqueUserDefineDataForMap_Cell_ObjectData->ContainsKey(e_pMap_Cell_ObjectData->GetUniqueID()))
	//	{
	//		this->m_UniqueUserDefineDataForMap_Cell_ObjectData->Remove(e_pMap_Cell_ObjectData->GetUniqueID());
	//	}
	//		
	//}

	//System::Void cObjectDataFormImple::RemoveUserDefineNode(cLevelLayerList*e_pMap_CellData)
	//{
	//	for( int i=0;i<e_pMap_CellData->Count();++i )
	//	{
	//		RemoveUserDefineNode((e_pMap_CellData->GetObject(i)));
	//	}
	//}

	//System::Void cObjectDataFormImple::RemoveUserDefineNode(cLayer*e_pMapLayer)
	//{
	//	for( int i=0;i<e_pMapLayer->Count();++i )
	//	{
	//		RemoveUserDefineNode((*e_pMapLayer)[i]);
	//	}
	//}

	//System::Void cObjectDataFormImple::CopyUserDataToNewData(cLevelLayerList*e_pSrc,cLevelLayerList*e_pDest)
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

	System::Void cObjectDataFormImple::TemplateDataInvertorReEdit()
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


	void	cObjectDataFormImple::OpenUserDefineData(String^e_strFileName)
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

	void	cObjectDataFormImple::OpenQuickApplyXmlFile(String^e_strFileName)
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

	void	cObjectDataFormImple::AddEventDataNode(cLevelLayerGridData*e_pLevelLayerGridData,GCFORM::TreeNode^e_pNode)
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

	void	cObjectDataFormImple::RemoveEventDataNode(cLevelLayerGridData*e_pLevelLayerGridData)
	{
		if( e_pLevelLayerGridData->GetEventDataNode() )
		{
			m_UniqueUserDefineDataForMap_Cell_ObjectData->Remove(e_pLevelLayerGridData->GetEventDataNode()->GetUniqueID());
			m_pLevelEditorApp->m_pLevelData->m_EventDataNodeList.RemoveObject(e_pLevelLayerGridData->GetEventDataNode());
			e_pLevelLayerGridData->SetEventDataNode(0);
			LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();
		}
	}

		System::Void cObjectDataFormImple::timer1_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(this->m_pForm->Visible)
				Render();
		 }
		System::Void cObjectDataFormImple::treeView1_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
		 {
			 if ( e->Data->GetDataPresent( DataFormats::FileDrop ) )
			 {
				e->Effect = DragDropEffects::Copy;
			 }
			 else
				 e->Effect = DragDropEffects::None;
		 }
		System::Void cObjectDataFormImple::treeView1_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
		 {
			array<String^>^l_strFileNames = DNCT::DragDropEventWhileFileDrop(e);
			for each(String^l_str in l_strFileNames)
			{
				 GCFORM::TreeNode^l_pNode = DNCT::OpenXmlFileToNode(l_str);
				 if( l_pNode )
				 {
					 //this->Text += System::IO::Path::GetFileName(l_str);
					 XMLDataForQuickApply_treeView->Nodes->Add(l_pNode);
				 }
			}
		 }

		System::Void cObjectDataFormImple::AssignData()
		 {
			 if( XMLDataForQuickApply_treeView->SelectedNode )
			 {
				 //if( XMLDataForQuickApply_treeView->SelectedNode->Nodes->Count != 0 )
				 {
						ObjectTemplate_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Add((GCFORM::TreeNode^)XMLDataForQuickApply_treeView->SelectedNode->Clone());
				 }
				 //else
				 {
					//WARNING_MSG("there is no attributes could be edit");
				 }
			 }		 
		 }

		System::Void cObjectDataFormImple::AssignTemplateData_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 AssignData();
		 }

		System::Void cObjectDataFormImple::addNewNodeFromFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			array<String^>^l_strFileNames = DNCT::OpenFileAndGetNames();
			if(!l_strFileNames)
				return;
			for each(String^l_str in l_strFileNames)
			{
				 GCFORM::TreeNode^l_pNode = DNCT::OpenXmlFileToNode(l_str);
				 if( l_pNode )
				 {
					 //this->Text += System::IO::Path::GetFileName(l_str);
					 XMLDataForQuickApply_treeView->Nodes->Add(l_pNode);
				 }
			}
		 }

		 void	cObjectDataFormImple::RemoveSelectedNode()
		 {
			 if( XMLDataForQuickApply_treeView->SelectedNode )
			 {
				 XMLDataForQuickApply_treeView->Nodes->Remove(XMLDataForQuickApply_treeView->SelectedNode);
			 }			 
		 }
			//this->LayerObjectData_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &cObjectDataFormImple::LayerObjectData_listBox_SelectedIndexChanged);
		System::Void cObjectDataFormImple::deleteNodeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			RemoveSelectedNode();
		 }
		System::Void cObjectDataFormImple::treeView1_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 if( e->KeyCode == GCFORM::Keys::Delete )
			 {
				RemoveSelectedNode();
			 }
		 }

		System::Void cObjectDataFormImple::AddImage_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			if( sender == AddImage_button )
			{
				array<String^>^l_strFileNames = DNCT::OpenFileAndGetNames("Image Files(*.pi;*.mpdi)|*.pi;*.mpdi|All files (*.*)|*.*");
				if(l_strFileNames)
				for each(String^l_strName in l_strFileNames)
				{
					if( System::String::Compare(System::IO::Path::GetExtension(l_strName),".pi") == 0 )
					{
						if(cGameApp::m_spImageParser->GetObject(DNCT::GcStringToWchar(System::IO::Path::GetFileNameWithoutExtension(l_strName))))
						{
							WARNING_MSG(l_strName+" exists!");
							continue;
						}
						
						if(cGameApp::m_spImageParser->Parse(DNCT::GcStringToChar(l_strName)))
						{
							ImageList_treeView->Nodes->Add(DNCT::PIToTreeNode(dynamic_cast<cPuzzleImage*>(cGameApp::m_spImageParser->GetLastObject())));
						}
						else
						{
							WARNING_MSG(l_strName+" open failed");
						}
					}
					else
					if( System::String::Compare(System::IO::Path::GetExtension(l_strName),".mpdi") == 0 )
					{
						if(cGameApp::m_spImageParser->GetObject(DNCT::GcStringToWchar(System::IO::Path::GetFileNameWithoutExtension(l_strName))))
						{
							WARNING_MSG(l_strName+" exists!");
							continue;
						}
						
						if(cGameApp::m_spAnimationParser->Parse(DNCT::GcStringToChar(l_strName)))
						{
							ImageList_treeView->Nodes->Add(DNCT::MPDIListToTreeNode(dynamic_cast<cMPDIList*>(cGameApp::m_spAnimationParser->GetLastObject())));
						}
						else
						{
							WARNING_MSG(l_strName+" open failed");
						}
					}
				}
			}
			else
			if( sender == DelImage_button )
			{
				if(RemoveTextureFromSelectedNodesTreeView(ImageList_treeView,cGameApp::m_spImageParser))
					ImageList_treeView->SelectedNode->Remove();
			}
		 }

		System::Void cObjectDataFormImple::AddTemplate_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == DelTemplate_button )
			 {
				 bool	l_bFirstTime = true;
				 bool	l_bEraseOldData = false;//if yes and template is conflict,erase the data
				 while(Template_listBox->SelectedIndices->Count)
				 {
					 String^l_strName = Template_listBox->Items[Template_listBox->SelectedIndices[0]]->ToString();
					 //check for old data using this image
					 cLevelLayerGridData*l_pMap_Cell_ObjectData = this->m_pLevelEditorApp->m_pLevelData->m_pTemplateList->GetObject(DNCT::GcStringToWchar(l_strName));
					 assert(l_pMap_Cell_ObjectData&&"call fatming!! if( sender == DelTemplate_button )");
					 for(int i=0;i<this->m_pLevelEditorApp->m_pLevelData->Count();++i)
					 {
						 cLevelLayerList*l_pMap_CellData = this->m_pLevelEditorApp->m_pLevelData->GetObject(i);
						 for(int j=0;j<l_pMap_CellData->Count();++j)
						 {//this template has been assigned to the orther data,check for delete or reserved
							 cLayer*l_pMapLayer = (*l_pMap_CellData)[j];
							 for( int k=0;k<l_pMapLayer->Count();++k )
							 {
								 if(l_pMapLayer->GetObject(k)->GetImage() == l_pMap_Cell_ObjectData->GetImage())
								 {
									 if( !l_bFirstTime )
									 {
										l_bFirstTime = true;
										WARING_YES_NO_TO_YES("would u like to erase the data has been assigned by this data?")
										{
											l_bEraseOldData = true;
										}
										else
										{
											WARNING_MSG("the data has been referenced can't be delete");
											return;
										}
									 }
									 if( l_bEraseOldData )
									 {
										 l_pMapLayer->RemoveObject(k);
										 --k;
									 }
								 }
							 }
						 }
					 }
					 Template_listBox->Items->RemoveAt(Template_listBox->SelectedIndices[0]);
					 m_TemplateNameAndAttributeNode->Remove(l_strName);
					 this->m_pLevelEditorApp->m_pLevelData->m_pTemplateList->RemoveObject(DNCT::GcStringToWchar(l_strName));
				 }
			 }
			 else
			 if(sender == AddTemplate_button)
			 {
				 //if( !ImageList_treeView->SelectedNode )
				 //{
					// WARNING_MSG("please select Image");
					// return;
				 //}
				 //if(!ImageList_treeView->SelectedNode->Parent)
				 //{
					// WARNING_MSG("please pi's child");
					// return;				 
				 //}
				 String^l_strTemplateName = TemplateName_textBox->Text->Trim();
				 if( l_strTemplateName->Length >0 )
				 {
					 if( l_strTemplateName[0]>=L'0'&&l_strTemplateName[0]<=L'9' )
					 {
						WARNING_MSG("first character can't be numerial");
						return;
					 }
					 GCFORM::TreeNode^l_pNode;
					 cLevelLayerGridData*l_pLevelLayerGridData = this->m_pLevelEditorApp->m_pLevelData->m_pTemplateList->GetObject(DNCT::GcStringToWchar(l_strTemplateName));
					 if( l_pLevelLayerGridData || m_TemplateNameAndAttributeNode->ContainsKey(l_strTemplateName) )
					 {
						WARING_YES_NO_TO_NO("replace old data!?")
						{
							return;
						}
						Template_listBox->Items->Remove(l_strTemplateName);
						if( l_pLevelLayerGridData->GetTemplateDataNode() )
						{
							std::wstring l_strTemplateDataNodeName = l_pLevelLayerGridData->GetTemplateDataNode()->GetName();
							m_TemplateNameAndAttributeNode->Remove(DNCT::WcharToGcstring( l_strTemplateDataNodeName.c_str() ));
							m_pLevelEditorApp->m_pLevelData->m_TemplateDataNodeList.RemoveObject(l_strTemplateDataNodeName.c_str());
						}
						m_pLevelEditorApp->m_pLevelData->m_pTemplateList->RemoveObject(DNCT::GcStringToWchar(l_strTemplateName));
					 }
					 cRenderObject*l_pRenderObjectBehavior = 0;
					 //if(!this->m_pLevelEditorApp->m_pLevelData->GetObject(DNCT::GcStringToWchar(TemplateName_textBox->Text->Trim()))
					 {
						 if( ImageList_treeView->SelectedNode )
						 {
							 cPuzzleImage*l_pPi = cGameApp::m_spImageParser->GetPuzzleImage(DNCT::GcStringToWchar(ImageList_treeView->SelectedNode->Parent->Text));
							 if( !l_pPi )
							 {
								 cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIList(DNCT::GcStringToWchar(ImageList_treeView->SelectedNode->Parent->Text));
								 l_pRenderObjectBehavior = l_pMPDIList->GetObject(DNCT::GcStringToWchar(ImageList_treeView->SelectedNode->Text));
							 }
							 else
							 {
								l_pRenderObjectBehavior = l_pPi->GetObject(DNCT::GcStringToWchar(ImageList_treeView->SelectedNode->Text));
							 }
							if( !l_pRenderObjectBehavior )
							{
								WARNING_MSG("image is not exists");
								return;
							}
						}
						 else
						 {//create a empty image
							if(cGameApp::m_spImageParser->GetObject(DNCT::GcStringToWchar(TemplateName_textBox->Text)))
							{
								WARNING_MSG("empty image name is exists!!");
								return ;
							}
							cBaseImage*l_pBaseImage = new cBaseImage(0,false);
							cGameApp::m_spImageParser->AddObject(l_pBaseImage);
							l_pBaseImage->SetName(DNCT::GcStringToWchar(TemplateName_textBox->Text));
							l_pBaseImage->SetWidth((int)GridWidth_numericUpDown->Value);
							l_pBaseImage->SetHeight((int)GridHeight_numericUpDown->Value);
							l_pRenderObjectBehavior = l_pBaseImage;
						 }
						cEventDataNode*l_pEventDataNode = 0;
						 if(ObjectTemplate_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Count!=0)
						 {
							 //set node name as same as l_strTemplateName
							 //l_pNode = ObjectTemplate_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes[0];
							 //if(l_pNode->Text->CompareTo(l_strTemplateName))
							 //{
								//l_pNode = gcnew GCFORM::TreeNode();
								//l_pNode->Text = l_strTemplateName;
								//l_pNode->Nodes->Add((GCFORM::TreeNode^)ObjectTemplate_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes[0]->Clone());
							 //}
							 //else
								l_pNode = (GCFORM::TreeNode^)ObjectTemplate_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes[0]->Clone();

							 String^l_strText = l_pNode->Text;
							 //l_pNode->Text = l_strTemplateName;
							 l_pEventDataNode = new cEventDataNode();
							 l_pEventDataNode->SetName(DNCT::GcStringToWchar(l_strText));
							 TiXmlElement*l_pTiXmlElement = DNCT::TreeNodeToTinyXMLNode(l_pNode,0);
							 l_pEventDataNode->SetDataNode(l_pTiXmlElement);
							 SAFE_DELETE(l_pTiXmlElement);
							 m_pLevelEditorApp->m_pLevelData->m_TemplateDataNodeList.AddObject(l_pEventDataNode);
							m_TemplateNameAndAttributeNode[l_strText] = l_pNode;
						 }
						 Template_listBox->Items->Add(l_strTemplateName);
						 cLevelLayerGridData*l_pLevelLayerGridData = new cLevelLayerGridData();
						 l_pLevelLayerGridData->SetName(DNCT::GcStringToWchar(l_strTemplateName));
						 l_pLevelLayerGridData->SetImage(l_pRenderObjectBehavior);
						 l_pLevelLayerGridData->SetTemplateDataNode(l_pEventDataNode);
						 bool	l_b = this->m_pLevelEditorApp->m_pLevelData->m_pTemplateList->AddObject(l_pLevelLayerGridData);
						 assert(l_b&&"this template exists!!");
						 ObjectTemplate_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();
						 Template_listBox->SelectedIndex = Template_listBox->Items->Count-1;
					 }
				 }
			 }
		 }

		System::Void cObjectDataFormImple::AddStage_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			if( sender == AddStage_button )
			{
				WCHAR*	l_strName = DNCT::GcStringToWchar(StageName_textBox->Text);
				if( m_pLevelEditorApp->m_pLevelData->GetObject(l_strName) )
				{
					WARING_YES_NO_TO_NO("replace data?")
					{
						return;
					}
					//this->RemoveUserDefineNode(m_pLevelEditorApp->m_pLevelData->GetObject(l_strName));
					m_pLevelEditorApp->m_pLevelData->RemoveObject(l_strName);
					AllStage_listBox->Items->Remove(StageName_textBox->Text);
					m_pLevelEditorApp->m_bStageDataChanged = false;
				}
				m_pLevelEditorApp->m_pCurrentStageEditData->SetName(l_strName);
				Row_numericUpDown_ValueChanged(sender,e);
				GridHeight_numericUpDown_ValueChanged(sender,e);
				StartX_numericUpDown_ValueChanged(sender,e);
				cLevelLayerList*l_pMap_CellData = new cLevelLayerList(m_pLevelEditorApp->m_pCurrentStageEditData);
				//this->RemoveUserDefineNode(m_pLevelEditorApp->m_pCurrentStageEditData);
				m_pLevelEditorApp->m_pLevelData->AddObject(l_pMap_CellData);
				m_pLevelEditorApp->m_pCurrentStageEditData->Destroy();
				m_pLevelEditorApp->m_pCurrentStageEditData->SetName(L"");
				AllStage_listBox->Items->Add(StageName_textBox->Text);
				LayerObjectData_listBox->Items->Clear();
				Layer_listBox->Items->Clear();
				LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();
			}
			else
			{
				if( AllStage_listBox->SelectedIndex == -1 )
					return;
				//if( m_pLevelEditorApp->m_bStageDataChanged )
				//{
				//	WARING_YES_NO_TO_NO("ignore current edit data?")
				//	{
				//		return;
				//	}
				//}
				this->timer1->Enabled = false;
				if( sender == ReRditStagebutton )
				{//fuck......it will lost unique user define data..go check m_UniqueUserDefineDataForMap_Cell_ObjectData
					Layer_listBox->Items->Clear();
					SAFE_DELETE(m_pLevelEditorApp->m_pCurrentStageEditData);
					m_pLevelEditorApp->m_pCurrentStageEditData = new cLevelLayerList(this->m_pLevelEditorApp->m_pLevelData->GetObject(DNCT::GcStringToWchar(AllStage_listBox->SelectedItem->ToString())));
					//CopyUserDataToNewData(this->m_pLevelEditorApp->m_pLevelData->GetObject(DNCT::GcStringToWchar(AllStage_listBox->SelectedItem->ToString())),m_pLevelEditorApp->m_pCurrentStageEditData);
					Vector2	l_vSize = m_pLevelEditorApp->m_pCurrentStageEditData->GetSize();
					POINT	l_GridSize = m_pLevelEditorApp->m_pCurrentStageEditData->GetGridSize();
					Vector3	l_vPos = m_pLevelEditorApp->m_pCurrentStageEditData->GetPos();
					this->StartX_numericUpDown->Value = (int)l_vPos.x;
					this->StartY_numericUpDown->Value = (int)l_vPos.y;
					GridWidth_numericUpDown->Value = (int)l_GridSize.x;
					GridHeight_numericUpDown->Value = (int)l_GridSize.y;
					MapWidth_numericUpDown->Value = (int)l_vSize.x;
					MapHeight_numericUpDown->Value = (int)l_vSize.y;
					LayerObjectData_listBox->Items->Clear();
					for(int i=0;i<m_pLevelEditorApp->m_pCurrentStageEditData->Count();++i)
					{
						cLayer*l_pLayer = m_pLevelEditorApp->m_pCurrentStageEditData->GetObject(i);
						const wchar_t*	l_strName = l_pLayer->GetName();
						Layer_listBox->Items->Add(DNCT::WcharToGcstring(l_strName));
					}
					if( Layer_listBox->Items->Count )
					{
						Layer_listBox->SelectedIndex = 0;
					}
					LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();
				}
				else
				if( sender == DelStage_button )
				{
					//this->RemoveUserDefineNode(m_pLevelEditorApp->m_pLevelData->GetObject(AllStage_listBox->SelectedIndex));
					m_pLevelEditorApp->m_pLevelData->RemoveObject(AllStage_listBox->SelectedIndex);
					AllStage_listBox->Items->RemoveAt(AllStage_listBox->SelectedIndex);
				}
				this->timer1->Enabled = true;
			}
		 }
		System::Void cObjectDataFormImple::GridHeight_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 Vector2	l_vSize = m_pLevelEditorApp->m_pCurrentStageEditData->GetSize();
			 l_vSize.x = (float)GridWidth_numericUpDown->Value;
			 l_vSize.y = (float)GridHeight_numericUpDown->Value;
			 POINT	l_GridSize = {(int)GridWidth_numericUpDown->Value,(int)GridHeight_numericUpDown->Value};
			 m_pLevelEditorApp->m_pCurrentStageEditData->SetGridSize(l_GridSize);
			 if( autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem->Checked )
			 {
				 float	l_fWidth = m_pLevelEditorApp->m_pOrthogonalCamera->GetGridWidth()*(int)MapWidth_numericUpDown->Value;
				 float	l_fHeight = m_pLevelEditorApp->m_pOrthogonalCamera->GetGridHeight()*(int)MapHeight_numericUpDown->Value;
				 for( int i=0;i<m_pLevelEditorApp->m_pCurrentStageEditData->Count();++i )
				 {
					cLayer*l_pMapLayer = (*m_pLevelEditorApp->m_pCurrentStageEditData)[i];
					for( int j=0;j<l_pMapLayer->Count();++j )
					{
						cLevelLayerGridData*l_pMap_Cell_ObjectData = (*l_pMapLayer)[i];
						Vector3	l_vPos = l_pMap_Cell_ObjectData->GetPos();
						if( autoAllignGridToolStripMenuItem->Checked )//by grid position
						{
							//if( l_vPos.x >= 0.f&&l_fWidth >= l_vPos.x&&l_vPos.y >= 0.f&&l_fHeight >= l_vPos.y )
							{
								int	l_iRow = (int)(l_vPos.x/m_pLevelEditorApp->m_pOrthogonalCamera->GetGridWidth());
								int	l_iColumn = (int)(l_vPos.y/m_pLevelEditorApp->m_pOrthogonalCamera->GetGridHeight());
								l_vPos.x = l_iRow*l_vSize.x;
								l_vPos.y = l_iColumn*l_vSize.y;
							}
						}
						else//by scale rate
						{
							float	l_fScaleRateX = l_vSize.x/m_pLevelEditorApp->m_pOrthogonalCamera->GetGridWidth();
							float	l_fScaleRateY = l_vSize.y/m_pLevelEditorApp->m_pOrthogonalCamera->GetGridHeight();
							l_vPos.x *= l_fScaleRateX;
							l_vPos.y *= l_fScaleRateY;
						}
						l_pMap_Cell_ObjectData->SetPos(l_vPos);
					}
				 }
			 }
			 m_pLevelEditorApp->m_pOrthogonalCamera->SetGridHeight((float)GridHeight_numericUpDown->Value);
			 m_pLevelEditorApp->m_pOrthogonalCamera->SetGridWidth((float)GridWidth_numericUpDown->Value);
		 }
		System::Void cObjectDataFormImple::showGridToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 //if( sender == showGridToolStripMenuItem )
			 {
				 System::Windows::Forms::ToolStripMenuItem^l_pSender = (System::Windows::Forms::ToolStripMenuItem^)sender;
				 l_pSender->Checked = !l_pSender->Checked;
			 }
		 }
		System::Void cObjectDataFormImple::Row_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 Vector2	l_Size = m_pLevelEditorApp->m_pCurrentStageEditData->GetSize();
			 l_Size.x = (float)MapWidth_numericUpDown->Value;
			 l_Size.y = (float)MapHeight_numericUpDown->Value;
			 m_pLevelEditorApp->m_pCurrentStageEditData->SetSize(l_Size);
		 }
		System::Void cObjectDataFormImple::Template_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( Template_listBox->SelectedIndex != -1 )
			 {
				 WCHAR*l_strName = DNCT::GcStringToWchar(Template_listBox->SelectedItem->ToString());
				 m_pLevelEditorApp->m_pCurrentSelectedImage = this->m_pLevelEditorApp->m_pLevelData->m_pTemplateList->GetObject(l_strName)->GetImage();
				 TemplateDataInvertorReEdit();
			 }
		 }
		System::Void cObjectDataFormImple::Clear_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			if( m_pLevelEditorApp->m_pCurrentStageEditData->GetCurrentList() )
				m_pLevelEditorApp->m_pCurrentStageEditData->GetCurrentList()->Destroy();
			LayerObjectData_listBox->Items->Clear();
		 }

		System::Void cObjectDataFormImple::saveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 String^l_strFileName = DNCT::SaveFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
			 if( l_strFileName )
			 {
				 String^l_strForUnity = System::IO::Path::GetDirectoryName(l_strFileName);
				l_strForUnity += "_Unity";
				l_strForUnity += System::IO::Path::GetFileName(l_strFileName);
				this->m_pLevelEditorApp->m_pLevelData->Export(DNCT::GcStringToChar(l_strFileName));
				this->m_pLevelEditorApp->m_pLevelData->Export(DNCT::GcStringToChar(l_strForUnity),true,true);
				//for quick apply
				String^l_strTempDirectory = System::IO::Path::GetDirectoryName(l_strFileName)+"/XmlTemp";
				System::IO::Directory::CreateDirectory(l_strTempDirectory);
				l_strTempDirectory +="/temp.xml";
				DNCT::SaveTreeViewToFile(XMLDataForQuickApply_treeView,l_strTempDirectory);
				//for template file
				//cObjectDataFormImple::SaveTreeNodeFile( System::IO::Path::ChangeExtension(l_strFileName,"tmp"),m_TemplateNameAndAttributeNode,false);
				//for unique user define data
				//cObjectDataFormImple::SaveTreeNodeFile( System::IO::Path::ChangeExtension(l_strFileName,"userdefine"),m_UniqueUserDefineDataForMap_Cell_ObjectData,true);
			 }
		 }
		System::Void cObjectDataFormImple::backgroundImageToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			String^l_strFileName = DNCT::OpenFileAndGetName();
			if( l_strFileName )
			{
				SAFE_DELETE(m_pLevelEditorApp->m_pBGImage);
				m_pLevelEditorApp->m_pBGImage = new cBaseImage(DNCT::GcStringToChar(l_strFileName));
			}
		 }

		System::Void cObjectDataFormImple::ReEditTemplate_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 TemplateDataInvertorReEdit();
		 }
		System::Void cObjectDataFormImple::openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			timer1->Enabled = false;
			String^l_strFileName = DNCT::OpenFileAndGetName();
			if( l_strFileName )
			{
				Layer_listBox->Items->Clear();
				AllStage_listBox->Items->Clear();
				this->ImageList_treeView->Nodes->Clear();
				LayerObjectData_listBox->Items->Clear();
				this->m_TemplateNameAndAttributeNode->Clear();
				this->Template_listBox->Items->Clear();
				m_UniqueUserDefineDataForMap_Cell_ObjectData->Clear();
				cGameApp::m_spAnimationParser->Destroy();
				cGameApp::m_spImageParser->Destroy();
				m_pLevelEditorApp->DestoryOldData();
				bool	l_b = this->m_pLevelEditorApp->m_pLevelData->ParseWithMyParse(DNCT::GcStringToChar(l_strFileName));
				if( l_b )
				{
					//return;
					//for(int i=0;i<cGameApp::m_spImageParser->Count();++i)
					{
						ImagerParserAddIntiTreeView(cGameApp::m_spImageParser,ImageList_treeView,true);
					}
					//for(int i=0;i<cGameApp::m_spImageParser->Count();++i)
					{
						ImagerParserAddIntiTreeView(cGameApp::m_spAnimationParser,ImageList_treeView,false);
					}
					//l_strFileName = System::IO::Path::ChangeExtension(l_strFileName,"tmp");
					//treeView1->Nodes->Clear();
					//GCFORM::TreeNode^l_pNode = DNCT::OpenXmlFileToNode(l_strFileName);
					//if(l_pNode)
					//{
					//	for each(GCFORM::TreeNode^l_pNode in l_pNode->Nodes)
					//	{
					//		treeView1->Nodes->Add((GCFORM::TreeNode^)l_pNode->Clone());
					//		m_TemplateNameAndAttributeNode->Add(l_pNode->Text,l_pNode->Clone());
					//	}
					//}
					for(int i=0;i<this->m_pLevelEditorApp->m_pLevelData->m_pTemplateList->Count();++i)
					{
						this->Template_listBox->Items->Add(DNCT::WcharToGcstring((*this->m_pLevelEditorApp->m_pLevelData->m_pTemplateList)[i]->GetName()));
					}
					for(int i=0;i<this->m_pLevelEditorApp->m_pLevelData->Count();++i)
					{
						AllStage_listBox->Items->Add(DNCT::WcharToGcstring(this->m_pLevelEditorApp->m_pLevelData->GetObject(i)->GetName()));
					}
					//open quick apply xml data
					OpenQuickApplyXmlFile(l_strFileName);
					//OpenUserDefineData(l_strFileName);
					for(int i=0;i<m_pLevelEditorApp->m_pLevelData->m_TemplateDataNodeList.Count();++i)
					{
						cEventDataNode*l_pEventDataNode = m_pLevelEditorApp->m_pLevelData->m_TemplateDataNodeList[i];
						TiXmlElement*l_pTiXmlElement = l_pEventDataNode->GetDataNode();
						GCFORM::TreeNode^l_pTreeNode = TinyXMLNodeToTreeNode(l_pTiXmlElement);
						m_TemplateNameAndAttributeNode[gcnew String(l_pEventDataNode->GetCharName().c_str())] = l_pTreeNode;
					}
					for( int i=0;i<m_pLevelEditorApp->m_pLevelData->m_EventDataNodeList.Count();++i )
					{
						cEventDataNode*l_pEventDataNode = m_pLevelEditorApp->m_pLevelData->m_EventDataNodeList[i];
						GCFORM::TreeNode^l_pTreeNode = TinyXMLNodeToTreeNode(l_pEventDataNode->GetDataNode());
						m_UniqueUserDefineDataForMap_Cell_ObjectData[l_pEventDataNode->GetUniqueID()] = l_pTreeNode;
					}
				}
			}
			Main_splitContainer_Panel1_SizeChanged(sender,e);
			timer1->Enabled = true;
		 }
		System::Void cObjectDataFormImple::StartX_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 m_pLevelEditorApp->m_pCurrentStageEditData->SetPos(Vector3((float)this->StartX_numericUpDown->Value,(float)this->StartY_numericUpDown->Value,0.f));
		 }
		System::Void cObjectDataFormImple::AddToLayer_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( DeleteLayer_button == sender )
			 {
				 timer1->Enabled = false;
				 Sleep(10);
				 LayerObjectData_listBox->Items->Clear();
				 while(Layer_listBox->SelectedIndices->Count>0)
				 {
					int	l_iSelectedIndex = Layer_listBox->SelectedIndices[0];
					//RemoveUserDefineNode(m_pLevelEditorApp->m_pCurrentStageEditData->GetObject(l_iSelectedIndex));
					m_pLevelEditorApp->m_pCurrentStageEditData->RemoveObject(l_iSelectedIndex);
					Layer_listBox->Items->RemoveAt(l_iSelectedIndex);
					m_pLevelEditorApp->m_pCurrentStageEditData->SetCurrentList((cLayer*)0);
				 }
				 timer1->Enabled = true;
			 }
			 else
			 if( sender == AddToLayer_button )
			 {
				 if(LayerName_textBox->Text->Length)
				 {
					 if(!DNCT::CheckListContainStringAndAdd(Layer_listBox,LayerName_textBox->Text,true))
					 {
						 cLayer*l_pMapLayer = new cLayer();
						 l_pMapLayer->SetName(DNCT::GcStringToWchar(LayerName_textBox->Text));
						 m_pLevelEditorApp->m_pCurrentStageEditData->AddObject(l_pMapLayer);
						 Layer_listBox->SelectedIndices->Clear();
						 for( int i=0;i<Layer_listBox->Items->Count;++i )
							Layer_listBox->SelectedIndices->Add(i);
						 m_pLevelEditorApp->m_pCurrentStageEditData->SetCurrentList(Layer_listBox->Items->Count-1);
					 }
				 }
			 }
		 }

		System::Void cObjectDataFormImple::AllStage_listBox_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			if( m_pLevelEditorApp->m_pCurrentStageEditData->Count() )
			{
				if( showWarningMsgToolStripMenuItem->Checked  )
				{
					WARING_YES_NO_TO_NO("it will clear exist data!would u like to do it?")
						return;
				}
			}
			AddStage_button_Click(ReRditStagebutton,e);
		 }
		System::Void cObjectDataFormImple::LayerDown_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( LayerUp_button == sender )
			 {
				 if( Layer_listBox->SelectedIndex != 0 && Layer_listBox->Items->Count>=2 )
				 {
					 int	l_iSelectedIndex = Layer_listBox->SelectedIndex;
					 String^l_strName1 = Layer_listBox->Items[l_iSelectedIndex]->ToString();
					 String^l_strName2 = Layer_listBox->Items[l_iSelectedIndex-1]->ToString();
					 Layer_listBox->Items[l_iSelectedIndex] = l_strName2;
					 Layer_listBox->Items[l_iSelectedIndex-1] = l_strName1;
					 m_pLevelEditorApp->m_pCurrentStageEditData->SwapIndex(l_iSelectedIndex,l_iSelectedIndex-1);
				 }
			 }
			 else
			 if( sender == LayerDown_button )
			 {
				 if( Layer_listBox->SelectedIndex != Layer_listBox->Items->Count-1&& Layer_listBox->Items->Count>=2 )
				 {
					 int	l_iSelectedIndex = Layer_listBox->SelectedIndex;
					 String^l_strName1 = Layer_listBox->Items[l_iSelectedIndex]->ToString();
					 String^l_strName2 = Layer_listBox->Items[l_iSelectedIndex+1]->ToString();
					 Layer_listBox->Items[l_iSelectedIndex] = l_strName2;
					 Layer_listBox->Items[l_iSelectedIndex+1] = l_strName1;				 
					 m_pLevelEditorApp->m_pCurrentStageEditData->SwapIndex(l_iSelectedIndex,l_iSelectedIndex+1);
				 }				
			 }
		 }
		System::Void cObjectDataFormImple::AllStage_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(AllStage_listBox->SelectedItem)
			 {
				 StageName_textBox->Text = AllStage_listBox->SelectedItem->ToString();
			 }
		 }
		System::Void cObjectDataFormImple::cameraResetToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			m_pLevelEditorApp->m_pOrthogonalCamera->SetResolution(Vector2(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y));
			m_pLevelEditorApp->m_pOrthogonalCamera->Reset();
		 }
		System::Void cObjectDataFormImple::AddObjectToStage_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 Vector3	l_vPos = Vector3((float)AddObjectX_numericUpDown->Value,(float)AddObjectY_numericUpDown->Value,(float)AddObjectZ_numericUpDown->Value);
			 m_pLevelEditorApp->AddObjectIntoCurrentStage(l_vPos,LayerObjectData_listBox,Template_listBox);
		 }
		System::Void cObjectDataFormImple::DeleteObject_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 while(LayerObjectData_listBox->SelectedIndices->Count>0)
			 {
				int	l_iSelectedIndex = LayerObjectData_listBox->SelectedIndices[0];
				//RemoveUserDefineNode(m_pLevelEditorApp->m_pCurrentStageEditData->GetCurrentList()->GetObject(l_iSelectedIndex));
				m_pLevelEditorApp->m_pCurrentStageEditData->GetCurrentList()->RemoveObject(l_iSelectedIndex);
				LayerObjectData_listBox->Items->RemoveAt(l_iSelectedIndex);
			 }
		 }
		System::Void cObjectDataFormImple::MyKeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e)
		 {
		    if( this->splitContainer2->Focused )
			{
                if( e->KeyChar == L'D'||
                    e->KeyChar == L'd' )
				{
					int	l_iColldedIndex = m_pLevelEditorApp->m_pCurrentStageEditData->CollideForSamePosition(m_pLevelEditorApp->m_vCurrentPlacePos);
			        if( l_iColldedIndex != -1 )
			        {
				        m_pLevelEditorApp->m_pCurrentStageEditData->GetCurrentList()->RemoveObject(l_iColldedIndex);
				        LayerObjectData_listBox->Items->RemoveAt(l_iColldedIndex);
			        }
				}
				else
				if( e->KeyChar == L'a'||
				    e->KeyChar == L'A' )
				{
				    if( !AllowSamepositionObject_checkBox->Checked )
					{
						this->m_pLevelEditorApp->AddObjectOnCell(LayerObjectData_listBox,Template_listBox);
					}
				}
            }
		 }
		System::Void cObjectDataFormImple::MyKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
		{
				if( e->KeyCode == Keys::Add  )
				{

				}
			 }

		System::Void cObjectDataFormImple::ImageList_treeView_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
		 {
            if( ImageList_treeView->SelectedNode && ImageList_treeView->SelectedNode->Parent )
            {
				 TemplateName_textBox->Text = ImageList_treeView->SelectedNode->Text;
				 WCHAR*l_strSelectedName = DNCT::GcStringToWchar(ImageList_treeView->SelectedNode->Parent->Text);
                 cPuzzleImage*l_pPI = dynamic_cast<cPuzzleImage*>(cGameApp::m_spImageParser->GetObject(l_strSelectedName));
				 if( l_pPI )
				 {
					cPuzzleImageUnit*l_pPuzzleImageUnit = dynamic_cast<cPuzzleImageUnit*>(l_pPI->GetObject(DNCT::GcStringToWchar(ImageList_treeView->SelectedNode->Text)));
					//label9->Text = DNCT::WcharToGcstring(l_pPuzzleImageUnit->GetTexture()->DescriptionInfo());
					m_pLevelEditorApp->m_pHintImage = l_pPuzzleImageUnit;
				 }
				 else
				 {
					cMPDIList*l_pMPDIList = dynamic_cast<cMPDIList*>(cGameApp::m_spAnimationParser->GetObject(l_strSelectedName));
					if( l_pMPDIList )
					{
						cMPDI*l_pMPDI = l_pMPDIList->GetObject(DNCT::GcStringToWchar(ImageList_treeView->SelectedNode->Text));
						//label9->Text = DNCT::WcharToGcstring(l_pMPDI->DescriptionInfo());
						l_pMPDI->Init();
						m_pLevelEditorApp->m_pHintImage = l_pMPDI;
					}
				 }
			}
			else
			{
			    //label9->Text = "";
			}
			//if( ImageList_listBox->SelectedIndex!=-1 )
			//{
			//	cBaseImage*l_pNewImage = cGameApp::m_spImageParser->GetObject(DNCT::GcStringToWchar(ImageList_listBox->Items[ImageList_listBox->SelectedIndex]->ToString()));
			//	if( Template_listBox->SelectedIndex != -1 )
			//	{//change old data image as new
			//		String^l_strName = Template_listBox->Items[Template_listBox->SelectedIndex]->ToString();
			//		cLevelLayerGridData*l_pMap_Cell_ObjectData = this->m_pLevelEditorApp->m_pLevelData->m_pTemplateList->GetObject(DNCT::GcStringToWchar(l_strName));
			//		assert(l_pMap_Cell_ObjectData&&"call fatming!! if( sender == DelTemplate_button )");
			//		for(int i=0;i<this->m_pLevelEditorApp->m_pLevelData->Count();++i)
			//		{
			//			cLevelLayerList*l_pMap_CellData = this->m_pLevelEditorApp->m_pLevelData->GetObject(i);
			//			for(int j=0;j<l_pMap_CellData->Count();++j)
			//			{
			//				if((*l_pMap_CellData)[j]->GetImage() == l_pMap_Cell_ObjectData->GetImage())
			//				{
			//					(*l_pMap_CellData)[j]->SetImage(l_pNewImage);
			//				}
			//			}
			//		}
			//	}
			//	Image_textBox->Text = ImageList_listBox->Items[ImageList_listBox->SelectedIndex]->ToString();
			//	//m_pLevelEditorApp->m_pCurrentSelectedImage = l_pNewImage;
			//}
			////else
			////	m_pLevelEditorApp->m_pCurrentSelectedImage = 0;
		 }
		System::Void cObjectDataFormImple::Layer_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 LayerObjectData_listBox->Items->Clear();
			 if( Layer_listBox->SelectedIndex == -1 )
			 {
				 m_pLevelEditorApp->m_pCurrentStageEditData->SetCurrentList((cLayer*)0);
			 }
			 else
			 {
				m_pLevelEditorApp->m_pCurrentStageEditData->SetCurrentList(Layer_listBox->SelectedIndex);
				DumpObjectListIntoListBox(LayerObjectData_listBox,m_pLevelEditorApp->m_pCurrentStageEditData->GetCurrentList(),true);
			 }
		 }
		System::Void cObjectDataFormImple::ImageList_treeView_BeforeSelect(System::Object^  sender, System::Windows::Forms::TreeViewCancelEventArgs^  e)
         {
            if(this->ImageList_treeView->SelectedNode)
            {
                this->ImageList_treeView->SelectedNode->BackColor = System::Drawing::Color::White;
                this->ImageList_treeView->SelectedNode->ForeColor = System::Drawing::Color::Black;
            }
         }
		System::Void cObjectDataFormImple::AllStage_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 if( e->KeyCode == GCFORM::Keys::Enter )
			 {
				System::Windows::Forms::MouseEventArgs^  e2;
				AllStage_listBox_MouseDoubleClick(sender,e2);
			 }
		 }
		System::Void cObjectDataFormImple::LayerObjectData_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
        {
            if( LayerObjectData_listBox->SelectedIndex != -1)
            {
                LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();
                m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData = m_pLevelEditorApp->m_pCurrentStageEditData->GetCurrentList()->GetObject(LayerObjectData_listBox->SelectedIndex);
                if( m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData->GetEventDataNode() && m_UniqueUserDefineDataForMap_Cell_ObjectData->ContainsKey(m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData->GetEventDataNode()->GetUniqueID()) )
                {
                    GCFORM::TreeNode^l_pNode = (GCFORM::TreeNode^)m_UniqueUserDefineDataForMap_Cell_ObjectData[m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData->GetEventDataNode()->GetUniqueID()];
                    if( l_pNode )
                    {
                        LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Add((GCFORM::TreeNode^)l_pNode->Clone());
                    }
                }
            }
			else
			{
				LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();
				ObjectTemplate_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();
				m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData = 0;
			}
        }

		System::Void cObjectDataFormImple::SyncUserDefineDataByFile_button_Click(System::Object^  sender, System::EventArgs^  e)
        {
            WARNING_MSG("select event data to sync userdefine data,it's possible edit event data at another file,and want to sync current user define data");
            String^l_strFileName = DNCT::OpenFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
            if( l_strFileName )
            {
                GCFORM::TreeNode^l_pEventDefinedEventNode = DNCT::OpenXmlFileToNode(nullptr);
                for each(String^l_strKey in this->m_TemplateNameAndAttributeNode->Keys)
                {
                    //if the userdefine data is define by event editor it will contain a name has named in the editor
                    //now we find it out
                    //<UserDefineData Stage="cc" Layer="a1" Index="9">
                    GCFORM::TreeNode^l_pUserDefineNode = (GCFORM::TreeNode^)m_TemplateNameAndAttributeNode[l_strKey];
                    if( l_pUserDefineNode->Nodes->Count == 1 )//UserDefineData
                    {
                        assert(l_pUserDefineNode->Nodes[0]->Nodes->Count>=4&&"this user define data is empty!?");
                        //get event node?(because its possible not)
                        GCFORM::TreeNode^l_pUserdefineNodeFromEventNode = l_pUserDefineNode->Nodes[0]->Nodes[3];
                        if( l_pUserdefineNodeFromEventNode->Name->Length )
                        {//it is a attribute,it's not from event editor
                            continue;
                        }
                        //ok it's possible defined by event editor,now check the name if it exist set the node
                        for each( GCFORM::TreeNode^l_pNode in l_pEventDefinedEventNode->Nodes )
                        {
                            //ensure it's edit in the event the first node is a element without attrbute
                            if( l_pNode->Name->Length == 0 )
                            {
                                //ok the name is matched, it might  edited by event editor
                                if(l_pUserdefineNodeFromEventNode->Text->Equals(l_pNode->Text))
                                {
                                    //remove old data sync event file to this
                                    l_pUserDefineNode->Nodes[0]->Nodes[3]->Remove();
                                    l_pUserDefineNode->Nodes[0]->Nodes->Add((GCFORM::TreeNode^)l_pNode->Clone());
                                }
                            }
                        }
                    }
                }
            }
        }
		System::Void cObjectDataFormImple::XMLDataForQuickApply_treeView_ItemDrag(System::Object^  sender, System::Windows::Forms::ItemDragEventArgs^  e) 
		 {
			 if(XMLDataForQuickApply_treeView->SelectedNode)
			 {
				 this->XMLDataForQuickApply_treeView->DoDragDrop(XMLDataForQuickApply_treeView->SelectedNode,DragDropEffects::Move);
			 }
		 }
		System::Void cObjectDataFormImple::assignToObjectEventDataToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if( LayerObjectData_listBox->SelectedIndex != -1 )
			 {
				 AddEventDataNode(m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData,XMLDataForQuickApply_treeView->SelectedNode);
			 }
			 else
			 {
				 WARNING_MSG("please select object!");
			 }
		 }

		System::Void cObjectDataFormImple::ObjectData_treeView_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
		 {
			 GCFORM::TreeNode^l_pTreeNode;
			 int l_iIndex = LayerObjectData_listBox->SelectedIndex;
			 if( l_iIndex != -1 )
			 {
				l_pTreeNode = DragDropEventWhileTreeNodeDrop(e,true);
				if( l_pTreeNode )
				{
					AddEventDataNode(m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData,l_pTreeNode);
				}			 
			 }
		 }


		System::Void	cObjectDataFormImple::LevelLayerGridEventDataNode_xmlNodeEditComponentTreeViewDeleteOrCreateFunction(System::Object^sender,System::EventArgs^e)
		 {
			 if( m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData )
			 {
				if( LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Count == 0 )
				{
					RemoveEventDataNode(m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData);
					return;
				}
				else
				//if(!m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData->GetEventDataNode())
				{
					AddEventDataNode(m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData,LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes[0]);
				}
			 }
			 m_UniqueUserDefineDataForMap_Cell_ObjectData[m_pLevelEditorApp->m_pCurrentEditUserPropertyObjectData->GetUniqueID()] = LevelLayerGridEventDataNode_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes[0];
		 }
		System::Void cObjectDataFormImple::ImageList_treeView_MouseLeave(System::Object^  sender, System::EventArgs^  e) 
		 {
			m_pLevelEditorApp->m_pHintImage = 0;
		 }
		System::Void cObjectDataFormImple::NoImage_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 ImageList_treeView->SelectedNode = nullptr;
		 }

		System::Void cObjectDataFormImple::InGameRender_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pLevelEditorApp->m_pCurrentStageEditData && Layer_listBox->SelectedIndex != -1 )
			{
				if( LayerVisible_checkBox == sender )
				{
					m_pLevelEditorApp->m_pCurrentStageEditData->GetObject(Layer_listBox->SelectedIndex)->SetInEditorRender(LayerVisible_checkBox->Checked);
				}
				else
				if( InGameRender_checkBox == sender )
				{
					m_pLevelEditorApp->m_pCurrentStageEditData->GetObject(Layer_listBox->SelectedIndex)->SetInGameRender(InGameRender_checkBox->Checked);
				}
			}
		}

		System::Void cObjectDataFormImple::Main_splitContainer_Panel1_SizeChanged(System::Object^  sender, System::EventArgs^  e)
		{
			cGameApp::m_svViewPortSize.x = 0;
			cGameApp::m_svViewPortSize.y = 0;
			cGameApp::m_svViewPortSize.z = (float)this->Main_splitContainer->Panel1->Width;
			cGameApp::m_svViewPortSize.w = (float)this->Main_splitContainer->Panel1->Height;
			if( m_pLevelEditorApp )
				m_pLevelEditorApp->m_pOrthogonalCamera->ViewportChangeApplyNewResolution((int)cGameApp::m_svViewPortSize.z,(int)cGameApp::m_svViewPortSize.w);
		}
//end namespace LevelEditor
}