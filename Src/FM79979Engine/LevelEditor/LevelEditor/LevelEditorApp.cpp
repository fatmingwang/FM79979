#include "stdafx.h"
#include "LevelEditorApp.h"



cLevelEditorApp::cLevelEditorApp(HWND e_Hwnd):cGameApp(e_Hwnd)
{
	m_pLevelData = new cLevelData;
	m_pCurrentStageEditData = new cLevelLayerList;
	m_pOrthogonalCamera = new cOrthogonalCamera;
	m_pCurrentSelectedImage = m_pBGImage = 0;
	m_vStartPos = Vector2::Zero;
	m_vCurrentPlacePos = Vector3::Zero;;
	m_bAutoAllignGrid = m_bShowGrid = m_bShowResolutionBorder = true;
	m_bAllowSamepositionObject = m_bStageDataChanged = m_bPlaceObjectByImageCenter = false;
	m_SelectedLayerObjects.reserve(5000);
	m_vGridAlignPos = m_vMousePos = m_vSelectedObjectOriginalPos = Vector3::Zero;
	m_eMoudeMode = eMM_AddObject;
	m_pHintImage = 0;
}

cLevelEditorApp::~cLevelEditorApp()
{
	SAFE_DELETE(m_pBGImage);
	SAFE_DELETE(m_pLevelData);
	SAFE_DELETE(m_pCurrentStageEditData);
	SAFE_DELETE(m_pOrthogonalCamera);
}


void	cLevelEditorApp::Init()
{
	cGameApp::Init();
}

void	cLevelEditorApp::DestoryOldData()
{
	SAFE_DELETE(m_pBGImage);
	SAFE_DELETE(m_pLevelData);
	SAFE_DELETE(m_pCurrentStageEditData);
	SAFE_DELETE(m_pOrthogonalCamera);
	m_pLevelData = new cLevelData;
	m_pCurrentStageEditData = new cLevelLayerList;
	m_pOrthogonalCamera = new cOrthogonalCamera;
}


void	cLevelEditorApp::Update(float e_fElpaseTime)
{

}

void	cLevelEditorApp::Render()
{
	wglMakeCurrent(this->m_sHdc, m_sHGLRC);
	//DEFAULT_SHADER = L"ObjectDataFormShader";
	//NO_TEXTURE_SHADER = L"ObjectDataFormNoTextureShader";
	cTexture::m_suiLastUsingImageIndex = -1;
	UseShaderProgram(NO_TEXTURE_SHADER);
	glViewport(0,0,(int)this->m_svViewPortSize.x,(int)this->m_svViewPortSize.y);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glClearColor( 0,0,0,1 );
	glClearDepth(1.0f);											// Depth Buffer Setup
	float	l_fStartPos[2] = {m_vStartPos.x,m_vStartPos.y};
	m_pOrthogonalCamera->Render();
	m_pOrthogonalCamera->DrawGrid(l_fStartPos[0],l_fStartPos[1]);
	if(m_bShowGrid)
	{
		static Vector4 l_vColor = Vector4::One;
		//l_suiColor = rand()%0xffffff;
		//l_suiColor+=0x000000ff;
		l_vColor = Vector4::One;
		//float	l_fWidth = m_pOrthogonalCamera->GetGridWidth()*(int)MapWidth_numericUpDown->Value;
		//float	l_fHeight = m_pOrthogonalCamera->GetGridHeight()*(int)MapHeight_numericUpDown->Value;
		RenderRectangle(Vector2(l_fStartPos[0],l_fStartPos[1]),m_vMapWidth.x,m_vMapWidth.y,l_vColor,0);
		//RenderRectangle(Vector2(l_fStartPos[0],l_fStartPos[1]),l_fWidth,l_fHeight,l_suiColor);

		//// 
		//// label18
		//// 
		//this->label18->AutoSize = true;
		//this->label18->Location = System::Drawing::Point(2, 384);
		//this->label18->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
		//this->label18->Name = L"label18";
		//this->label18->Size = System::Drawing::Size(138, 72);
		//this->label18->TabIndex = 63;
		//this->label18->Text = L"TemplateList_Type\r\nstill in progress...\r\nbecause TMP has\r\ntoo much members\r\nso se" 
		//	L"t a type category\r\nis easy to select what u want!\r\n";
	}
	if( m_bShowResolutionBorder )
		RenderRectangle(Vector2(0.f,0.f),cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,Vector4(0,1,1,0.5f),0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	if( m_pBGImage )
	{
		m_pBGImage->SetColor(Vector4(1,1,1,0.6f));
		m_pBGImage->Render(0,0);
	}
	//for(int l_iSelectedIndex=0;l_iSelectedIndex<Layer_listBox->Items->Count;++l_iSelectedIndex)
	//for each (int l_iSelectedIndex in Layer_listBox->Items)
	//for each (int l_iSelectedIndex in Layer_listBox->SelectedIndices)
	for(int l_iSelectedIndex=0;l_iSelectedIndex<m_pCurrentStageEditData->Count();++l_iSelectedIndex)
	{
		m_pCurrentStageEditData->RenderLayer(l_iSelectedIndex);
	}
	if( m_eMoudeMode == eMM_AddObject)
	{
		if( m_pCurrentSelectedImage )
		{
			Vector3	l_vPos = m_vCurrentPlacePos;
			m_pCurrentSelectedImage->SetColor(Vector4(1,1,1,0.5f));
			if( m_pCurrentSelectedImage->Type() == cPuzzleImageUnit::TypeID )
			{
				cPuzzleImageUnit*l_pPIUnit = (cPuzzleImageUnit*)m_pCurrentSelectedImage;
				if( m_bPlaceObjectByImageCenter )
				{
					l_vPos.x -= l_pPIUnit->GetPuzzleData()->OriginalSize.x/2.f;
					l_vPos.y -= l_pPIUnit->GetPuzzleData()->OriginalSize.y/2.f;
				}
			}
			else
			{
				if( m_bPlaceObjectByImageCenter )
				{
					if( m_pCurrentSelectedImage->Type() == cPuzzleImageUnit::TypeID )
					{
						cPuzzleImageUnit*l_pPIUnit = (cPuzzleImageUnit*)m_pCurrentSelectedImage;
						l_vPos.x -= l_pPIUnit->GetWidth()/2.f;
						l_vPos.y -= l_pPIUnit->GetHeight()/2.f;
					}
					else
					{
						assert(0);
					}
				}				
			}
			m_pCurrentSelectedImage->SetPos(l_vPos);
			m_pCurrentSelectedImage->Render();
			m_pCurrentSelectedImage->SetColor(Vector4::One);
		}
	}
	if( this->m_spGlyphFontRender )
	{
		glEnable2D(1920.f,1080.f);
		m_spGlyphFontRender->RenderFont(0,0,ValueToStringW(m_pOrthogonalCamera->GetMouseWorldPos()));
	}
	m_pOrthogonalCamera->Render();
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	if( m_eMoudeMode == eMM_EraseObject )
	{
		Vector2	l_vPos2 = m_pOrthogonalCamera->GetMouseWorldPos();
		RenderPoint(l_vPos2,5.f);
	}
	//for each (int l_iIndex in LayerObjectData_listBox->SelectedIndices)
	for(size_t i=0;i<m_SelectedLayerObjects.size();++i) 
	{
		int l_iIndex = m_SelectedLayerObjects[i];
		static float l_sfloatColor = 0.f;
		l_sfloatColor += this->m_sTimeAndFPS.fElpaseTime;
		if( l_sfloatColor>1.f )
			l_sfloatColor = 0.f;
		(*m_pCurrentStageEditData->GetCurrentList())[l_iIndex]->DebugRender(l_sfloatColor);
	}
	if( m_pHintImage )
	{
		glEnable2D(1920.f,1080.f);
		m_pHintImage->Update(0.016f);
		m_pHintImage->Render();
	}
	//GLRender::glDisable2D();
	SwapBuffers(this->m_sHdc);
}

void	cLevelEditorApp::MouseDown(int e_iPosX,int e_iPosY)
{
	int	l_iSelectedObjectIndex = m_pCurrentStageEditData->Collide(m_pOrthogonalCamera->GetMouseWorldPos());
	if( l_iSelectedObjectIndex != -1 )
	{
		if( m_eMoudeMode == eMM_MoveObject )
		{
			m_pCurrentStageEditData->SetSelectedObject(m_pCurrentStageEditData->GetCurrentList()->GetObject(l_iSelectedObjectIndex));
			m_vSelectedObjectOriginalPos = m_pCurrentStageEditData->GetCurrentList()->GetObject(l_iSelectedObjectIndex)->GetPos();
		}
	}
}

void	cLevelEditorApp::MouseMove(int e_iPosX,int e_iPosY,GCFORM::ListBox^e_pLayerObjectData_listBox)
{
	Vector2	l_vPos = m_pOrthogonalCamera->GetMouseWorldPos();

	m_vMousePos = Vector3(l_vPos.x,l_vPos.y,0.f);
	float	l_fStartPos[2] = {m_vStartPos.x,m_vStartPos.y};
	if( m_bAutoAllignGrid )
	{
		float	l_fWidth = m_pOrthogonalCamera->GetGridWidth()*(int)m_vMapWidth.x;
		float	l_fHeight = m_pOrthogonalCamera->GetGridHeight()*(int)m_vMapWidth.y;
		//if( l_vPos.x >= 0.f&&l_fWidth >= l_vPos.x&&l_vPos.y >= 0.f&&l_fHeight >= l_vPos.y )
		{
			int	l_iRow = (int)((l_vPos.x-l_fStartPos[0])/m_pOrthogonalCamera->GetGridWidth());
			int	l_iColumn = (int)((l_vPos.y-l_fStartPos[1])/m_pOrthogonalCamera->GetGridHeight());
			if(l_vPos.x < l_fStartPos[0] )
				l_iRow-=1;
			if(l_vPos.y < l_fStartPos[1] )
				l_iColumn-=1;
			m_vGridAlignPos.x = l_iRow*m_pOrthogonalCamera->GetGridWidth()+l_fStartPos[0];
			m_vGridAlignPos.y = l_iColumn*m_pOrthogonalCamera->GetGridHeight()+l_fStartPos[1];
		}
		m_vCurrentPlacePos = m_vGridAlignPos;

		//m_pForm->Text = ((int)m_vCurrentPlacePos.x).ToString()+","+((int)g_vCurrentPlacePos.y).ToString();
	}
	else
	{
		//m_pForm->Text = ((int)m_vMousePos.x).ToString()+","+((int)m_vMousePos.y).ToString();
		m_vCurrentPlacePos = m_vMousePos;
	}
	if( m_pCurrentStageEditData )
	{
		//NumObjectInCurrentStagelabel_label->Text = g_pCurrentStageEditData->Count().ToString();
		if( m_pCurrentStageEditData->GetSelectedObject() )
		{
			Vector3	l_vPos = m_pCurrentStageEditData->GetSelectedObject()->GetPos();
			//label12->Text = "ObjectPos:"+((int)l_vPos.x).ToString()+","+((int)l_vPos.y).ToString();
		}
		Vector2	l_vCameraPos = m_pOrthogonalCamera->GetMouseWorldPos();
		if( m_pCurrentStageEditData->GetSelectedObject() )
		{
			m_pCurrentStageEditData->GetSelectedObject()->SetPos(m_vCurrentPlacePos);
		}
		else
		{
			int	l_iSelectedObjectIndex = m_pCurrentStageEditData->Collide(m_pOrthogonalCamera->GetMouseWorldPos());
			if( l_iSelectedObjectIndex != -1 )
			{
				Vector3	l_vPos = m_pCurrentStageEditData->GetCurrentList()->GetObject(l_iSelectedObjectIndex)->GetPos();
				//int	l_iNiceNextX = *m_pCurrentStageEditData->GetCurrentList()->GetObject(l_iSelectedObjectIndex)->GetImage()->GetWidth()+(int)l_vPos.x;
				//int	l_iNiceNextY = (int)l_vPos.y;
				//label12->Text = "ObjectPos:"+((int)l_vPos.x).ToString()+","+((int)l_vPos.y).ToString()+"NiceNext:"+l_iNiceNextX.ToString()+","+l_iNiceNextY.ToString();
				e_pLayerObjectData_listBox->SelectedItems->Clear();
				e_pLayerObjectData_listBox->SelectedIndex = l_iSelectedObjectIndex;
			}
			else
			{
				//label12->Text = "";
			}
		}
	}

}

void	cLevelEditorApp::MouseUp(int e_iPosX,int e_iPosY,
								 System::Collections::Hashtable^e_UniqueUserDefineDataForMap_Cell_ObjectData,
								 GCFORM::ListBox^e_pLayerObjectData_listBox,GCFORM::ListBox^e_pTemplate_listBox)
{
	if( m_eMoudeMode == eMM_MoveObject )
	{
		if( m_pCurrentStageEditData->GetSelectedObject() )
		{
			//int	l_iCollidedObjectIndex = g_pCurrentStageEditData->CollideWithoutSelected(g_vCurrentPlacePos);
			int	l_iCollidedObjectIndex = m_pCurrentStageEditData->CollideForSamePositionWithoutSelected(m_vCurrentPlacePos);
			if( l_iCollidedObjectIndex != -1 )
			{
				m_pCurrentStageEditData->GetSelectedObject()->SetPos(m_vSelectedObjectOriginalPos);
			}
			m_pCurrentStageEditData->SetSelectedObject(0);

		}
	}
	else
	if( m_eMoudeMode == eMM_EraseObject )
	{
		int	l_iNum = m_pCurrentStageEditData->Count();
		for( int i=0;i<l_iNum;++i )
		{
			int	l_iCount = m_pCurrentStageEditData->GetCurrentList()->Count();
			for( int j=0;j<l_iCount;++j )
			{
				cLevelLayerGridData*l_pMap_Cell_ObjectData = m_pCurrentStageEditData->GetCurrentList()->GetObject(j);
				Vector3	l_vPos = l_pMap_Cell_ObjectData->GetPos();
				if(l_pMap_Cell_ObjectData->IsCollide(m_vMousePos))
				{
					if(e_UniqueUserDefineDataForMap_Cell_ObjectData->ContainsKey(l_pMap_Cell_ObjectData->GetUniqueID()))
					{
						e_UniqueUserDefineDataForMap_Cell_ObjectData->Remove(l_pMap_Cell_ObjectData->GetUniqueID());
					}
					m_pCurrentStageEditData->GetCurrentList()->RemoveObject(j);

					e_pLayerObjectData_listBox->Items->RemoveAt(i);

					i = l_iNum;
					m_bStageDataChanged = true;
					m_pCurrentEditUserPropertyObjectData = 0;
					int	l_iOriginalSelectedIndex = e_pLayerObjectData_listBox->SelectedIndex;
					e_pLayerObjectData_listBox->SelectedIndex = -1;
					e_pLayerObjectData_listBox->SelectedIndex = l_iOriginalSelectedIndex;
					return;
				}
			}
		}
	}
	else
		AddObjectOnCell(e_pLayerObjectData_listBox,e_pTemplate_listBox);
}

void	cLevelEditorApp::MouseDoubleClick(GCFORM::TreeView^e_pTreeView,
System::Collections::Hashtable^e_UniqueUserDefineDataForMap_Cell_ObjectData,
System::Collections::Hashtable^e_TemplateNameAndAttributeNode)
{
	Vector2	l_vMousePos = m_pOrthogonalCamera->GetMouseWorldPos();
	cLevelLayerGridData*l_pMap_Cell_ObjectData = 0;
	for(int i=0;i<m_pCurrentStageEditData->Count();++i)
	{
		l_pMap_Cell_ObjectData = m_pCurrentStageEditData->GetCurrentList()->GetObject(i);
		Vector3	l_vPos = l_pMap_Cell_ObjectData->GetPos();
		if( l_pMap_Cell_ObjectData->IsCollide(l_vMousePos) )
		{
			e_pTreeView->Nodes->Clear();
			bool	l_bUniqueData = false;
			GCFORM::TreeNode^l_pNode;
			if( e_UniqueUserDefineDataForMap_Cell_ObjectData->ContainsKey(l_pMap_Cell_ObjectData->GetUniqueID()) )
			{
				l_bUniqueData = true;
				l_pNode = (GCFORM::TreeNode^)e_TemplateNameAndAttributeNode[l_pMap_Cell_ObjectData->GetUniqueID()];
			}

			//NodeName_textBox->Text = "";
			//this->ObjectData_textBox->Text = "";

			m_pCurrentEditUserPropertyObjectData = l_pMap_Cell_ObjectData;
			if( l_pNode )
			{
				e_pTreeView->Nodes->Add((GCFORM::TreeNode^)l_pNode->Clone());
				e_pTreeView->Nodes[0]->Expand();
				GCFORM::MessageBox::Show("Start to Edit Object property","GO!",GCFORM::MessageBoxButtons::OK);
			}
			else
			{
				if( l_bUniqueData )
				{
					WARNING_MSG("user property is not find!!! ensure data exists");
				}
			}
			return;
		}
	}
}


void	cLevelEditorApp::AddObjectOnCell(GCFORM::ListBox^e_pLayerObjectData_listBox,GCFORM::ListBox^e_pTemplate_listBox)
{
	Vector2	l_vCurrentPos = m_pOrthogonalCamera->GetMouseWorldPos();
	if( e_pTemplate_listBox->SelectedIndex == -1 )
		return;
	if( this->m_bAutoAllignGrid )
	{
		float	l_fStartPos[2] = {this->m_vStartPos.x,this->m_vStartPos.y};
		float	l_fWidth = m_pOrthogonalCamera->GetGridWidth()*(int)this->m_vMapWidth.x;
		float	l_fHeight = m_pOrthogonalCamera->GetGridHeight()*(int)this->m_vMapWidth.y;
		//if( l_vCurrentPos.x >= 0.f&&l_fWidth >= l_vCurrentPos.x&&l_vCurrentPos.y >= 0.f&&l_fHeight >= l_vCurrentPos.y )
		{
			int	l_iRow = (int)((l_vCurrentPos.x-l_fStartPos[0])/m_pOrthogonalCamera->GetGridWidth());
			int	l_iColumn = (int)((l_vCurrentPos.y-l_fStartPos[1])/m_pOrthogonalCamera->GetGridHeight());
			POINT	l_GridSize = this->m_pCurrentStageEditData->GetGridSize();
			Vector2	l_vSize = m_pCurrentStageEditData->GetSize();
			POINT	l_TotalRowAndColumn = { (int)l_vSize.x/l_GridSize.x,(int)l_vSize.y/l_GridSize.y};
			if( l_TotalRowAndColumn.x <= l_iRow || l_TotalRowAndColumn.y <= l_iColumn )
			{
				::MessageBox(0,L"this position is not allow to put object,or resize grid data",L"warning!!",MB_OK);
				return;
			}
			if(l_vCurrentPos.x < l_fStartPos[0] )
				l_iRow-=1;
			if(l_vCurrentPos.y < l_fStartPos[1] )
				l_iColumn-=1;
			l_vCurrentPos.x = l_iRow*m_pOrthogonalCamera->GetGridWidth()+l_fStartPos[0];
			l_vCurrentPos.y = l_iColumn*m_pOrthogonalCamera->GetGridHeight()+l_fStartPos[1];
		}
	}
	bool	l_bSamePositiob = false;
	WCHAR*l_strName = DNCT::GcStringToWchar(e_pTemplate_listBox->SelectedItem->ToString());
	if( !m_bAllowSamepositionObject )//if same object is not allow to at same position
	{
		//if( l_vPos.x == l_vCurrentPos.x && l_vPos.y == l_vCurrentPos.y )
		int	l_iColldedIndex = m_pCurrentStageEditData->CollideForSamePosition(m_vCurrentPlacePos);
		if( l_iColldedIndex != -1 )
		{
			l_bSamePositiob = true;
			m_pCurrentStageEditData->GetCurrentList()->RemoveObject(l_iColldedIndex);
			e_pLayerObjectData_listBox->Items->RemoveAt(l_iColldedIndex);
		}
	}
	AddObjectIntoCurrentStage(Vector3(l_vCurrentPos.x,l_vCurrentPos.y,0.f),e_pLayerObjectData_listBox,e_pTemplate_listBox);
}

void	cLevelEditorApp::AddObjectIntoCurrentStage(Vector3 e_vPos,GCFORM::ListBox^e_pLayerObjectData_listBox,GCFORM::ListBox^e_pTemplate_listBox)
{
	if( !e_pTemplate_listBox->SelectedItem )
		return;
	if( !m_pCurrentStageEditData->GetCurrentList() )
	{
		WARNING_MSG("please add or select layer first!");
		return;
	}

	WCHAR*l_strName = DNCT::GcStringToWchar(e_pTemplate_listBox->SelectedItem->ToString());
	cLevelLayerGridData*l_pLevelLayerGridData = new cLevelLayerGridData(m_pLevelData->m_pTemplateList->GetObject(l_strName));
	l_pLevelLayerGridData->SetName(l_strName);
	if(l_pLevelLayerGridData->GetImage()->Type() == cPuzzleImageUnit::TypeID)
	{
		cPuzzleImageUnit*l_pPIUnit = (cPuzzleImageUnit*)l_pLevelLayerGridData->GetImage();
		if( this->m_bPlaceObjectByImageCenter )
		{
			e_vPos.x -= l_pPIUnit->GetPuzzleData()->OriginalSize.x/2.f;
			e_vPos.y -= l_pPIUnit->GetPuzzleData()->OriginalSize.y/2.f;
			//l_pLevelLayerGridData->SetRenderByCenter(true);
		}
	}
	else
	if(l_pLevelLayerGridData->GetImage()->Type() == cMPDI::TypeID)
	{
		
	}
	else
	if(l_pLevelLayerGridData->GetImage()->Type() == cBaseImage::TypeID)
	{
	
	}
	else
	{
		assert(0);
	}
	l_pLevelLayerGridData->SetPos(e_vPos);
	m_pCurrentStageEditData->GetCurrentList()->AddObjectNeglectExist(l_pLevelLayerGridData);

	String^l_strImageInfo = m_pCurrentStageEditData->GetCurrentList()->Count().ToString()+":"+e_pTemplate_listBox->SelectedItem->ToString();
	e_pLayerObjectData_listBox->SelectedItems->Clear();
	e_pLayerObjectData_listBox->Items->Add(l_strImageInfo);
	m_bStageDataChanged = true;
}

void	cLevelEditorApp::KeyDown(char e_char)
{
	 cGameApp::KeyDown(e_char);
}
void	cLevelEditorApp::KeyUp(char e_char)
{
	cGameApp::KeyUp(e_char);

}