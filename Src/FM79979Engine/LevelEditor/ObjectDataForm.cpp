#include "StdAfx.h"
#include "ObjectDataForm.h"
cMap_CellData*g_pForListBoxTemplateCellData = 0;
cMap_CellData*g_pCurrentStageEditData = 0;
cBaseImage*g_pCurrentSelectedImage = 0;
cBaseImage*g_pBackgroundImage = 0;
namespace LevelEditor
{

	System::Void ObjectDataForm::MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		m_pOpenglFormControl->splitContainer1->Panel1->Focus();
		CameraUpdate(e,m_pOpenglFormControl->splitContainer1->Panel1,this->m_pOrthogonalCamera);
		
		//cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = new cMap_Cell_ObjectData();
	}

	System::Void ObjectDataForm::MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		CameraUpdate(e,m_pOpenglFormControl->splitContainer1->Panel1,this->m_pOrthogonalCamera);
	}

	System::Void ObjectDataForm::MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		CameraUpdate(e,m_pOpenglFormControl->splitContainer1->Panel1,this->m_pOrthogonalCamera);
		Vector2	l_vCurrentPos = m_pOrthogonalCamera->GetMouseWorldPos();
		if( autoAllignGridToolStripMenuItem->Checked )
		{
			float	l_fWidth = m_pOrthogonalCamera->GetGridWidth()*(int)Row_numericUpDown->Value;
			float	l_fHeight = m_pOrthogonalCamera->GetGridHeight()*(int)Column_numericUpDown->Value;
			if( l_vCurrentPos.x >= 0.f&&l_fWidth >= l_vCurrentPos.x&&l_vCurrentPos.y >= 0.f&&l_fHeight >= l_vCurrentPos.y )
			{
				int	l_iRow = (int)(l_vCurrentPos.x/m_pOrthogonalCamera->GetGridWidth());
				int	l_iColumn = (int)(l_vCurrentPos.y/m_pOrthogonalCamera->GetGridHeight());
				l_vCurrentPos.x = l_iRow*m_pOrthogonalCamera->GetGridWidth();
				l_vCurrentPos.y = l_iColumn*m_pOrthogonalCamera->GetGridHeight();
			}
		}
		if( Erase_checkBox->Checked )
		{
			int	l_iNum = g_pCurrentStageEditData->GetNum();
			for( int i=0;i<l_iNum;++i )
			{
				cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = g_pCurrentStageEditData->GetObject(i);
				Vector3	l_vPos = l_pMap_Cell_ObjectData->GetPos();
				if( l_vPos.x == l_vCurrentPos.x && l_vPos.y == l_vCurrentPos.y )
				{
					g_pCurrentStageEditData->RemoveObject(i);
					i = l_iNum;
				}
			}
			return;
		}
		if( Template_listBox->SelectedIndex == -1 )
			return;
		bool	l_bSamePositiob = false;
		WCHAR*l_strName = DNCT::GcStringToWchar(Template_listBox->SelectedItem->ToString());
		if( !AllowSamepositionObject_checkBox->Checked )//if same object is not allow to at same position
		{
			int	l_iNum = g_pCurrentStageEditData->GetNum();
			for( int i=0;i<l_iNum;++i )
			{
				cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = g_pCurrentStageEditData->GetObject(i);
				Vector3	l_vPos = l_pMap_Cell_ObjectData->GetPos();
				if( l_vPos.x == l_vCurrentPos.x && l_vPos.y == l_vCurrentPos.y )
				{
					if(!wcscmp(l_pMap_Cell_ObjectData->GetName(),g_pForListBoxTemplateCellData->GetObject(l_strName)->GetName()))
						l_bSamePositiob = true;
					else
						g_pCurrentStageEditData->RemoveObject(i);
					i = l_iNum;
				}
			}
		}
		if( !l_bSamePositiob )
		{
			cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = new cMap_Cell_ObjectData(g_pForListBoxTemplateCellData->GetObject(l_strName));
			l_pMap_Cell_ObjectData->SetName(l_strName);
			l_pMap_Cell_ObjectData->SetPos(Vector3(l_vCurrentPos.x,l_vCurrentPos.y,0.f));
			g_pCurrentStageEditData->AddObjectNeglectExist(l_pMap_Cell_ObjectData);
			NumObjectInCurrentStagelabel_label->Text = g_pCurrentStageEditData->GetNum().ToString();
		}
	}

	System::Void ObjectDataForm::Render()
	{
		glViewport(0,0,m_pOpenglFormControl->splitContainer1->Panel1->Width,m_pOpenglFormControl->splitContainer1->Panel1->Height);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		glClearColor( 0,0,0,1 );
		glClearDepth(1.0f);											// Depth Buffer Setup
		POINT l_ViewportSize = { splitContainer2->Panel2->Width,splitContainer2->Panel2->Height };
		glEnableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		m_pOrthogonalCamera->Update(l_ViewportSize);
		m_pOrthogonalCamera->DrawGrid();
		if(showGridToolStripMenuItem->Checked)
		{
			static UINT	l_suiColor = 0;
			//l_suiColor = rand()%0xffffff;
			//l_suiColor+=0x000000ff;
			l_suiColor = 0xffffffff;
			float	l_fWidth = m_pOrthogonalCamera->GetGridWidth()*(int)Column_numericUpDown->Value;
			float	l_fHeight = m_pOrthogonalCamera->GetGridHeight()*(int)Row_numericUpDown->Value;
			RenderRectangle(Vector2(0.f,0.f),l_fWidth,l_fHeight,l_suiColor);
		}
		POINT	l_Pos = {0,0};
		Vector2	l_vPos = m_pOrthogonalCamera->GetMouseWorldPos();
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		if( showResolutionBorderToolStripMenuItem->Checked )
			RenderRectangle(l_Pos,(int)DRAW_PANEL_RESOLUTION_WIDTH,(int)DRAW_PANEL_RESOLUTION_HEIGHT,0xffff0000);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		if( g_pBackgroundImage )
			g_pBackgroundImage->Render(0,0);
		for(int i=0;i<g_pCurrentStageEditData->GetNum();++i)
		{
			(*g_pCurrentStageEditData)[i]->Render();
		}
		if( !Erase_checkBox->Checked )
		if( g_pCurrentSelectedImage )
		{
			if( autoAllignGridToolStripMenuItem->Checked )
			{
				float	l_fWidth = m_pOrthogonalCamera->GetGridWidth()*(int)Row_numericUpDown->Value;
				float	l_fHeight = m_pOrthogonalCamera->GetGridHeight()*(int)Column_numericUpDown->Value;
				if( l_vPos.x >= 0.f&&l_fWidth >= l_vPos.x&&l_vPos.y >= 0.f&&l_fHeight >= l_vPos.y )
				{
					int	l_iRow = (int)(l_vPos.x/m_pOrthogonalCamera->GetGridWidth());
					int	l_iColumn = (int)(l_vPos.y/m_pOrthogonalCamera->GetGridHeight());
					l_vPos.x = l_iRow*m_pOrthogonalCamera->GetGridWidth();
					l_vPos.y = l_iColumn*m_pOrthogonalCamera->GetGridHeight();
				}
			}
			else
			{
				//l_vPos.x -= g_pCurrentSelectedImage->GetImageWidth()/2.f;
				//l_vPos.y -= g_pCurrentSelectedImage->GetImageHeight()/2.f;
			}
			g_pCurrentSelectedImage->Render((int)l_vPos.x,(int)l_vPos.y);
		}
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
		GLRender::glDisable2D();
		SwapBuffers (m_pOpenglFormControl->m_HdcMV);
	}

	System::Void ObjectDataForm::TemplateDataInvertorReEdit()
	{
		if( Template_listBox->SelectedIndex != -1 )
		{
			String^l_strName = Template_listBox->SelectedItem->ToString();
			GCFORM::TreeNode^l_pNode = (GCFORM::TreeNode^)m_TemplateNameAndAttributeNode[l_strName];
			xmlNodeEditComponent1->CurrentAttribute_treeView->Nodes->Clear();
			if( l_pNode )
			{
				xmlNodeEditComponent1->CurrentAttribute_treeView->Nodes->Add((GCFORM::TreeNode^)l_pNode->Clone());
				xmlNodeEditComponent1->CurrentAttribute_treeView->Nodes[0]->Expand();
			}
			else
			{
				m_TemplateNameAndAttributeNode->Remove(l_strName);
			}
			TemplateName_textBox->Text = l_strName;
			Image_textBox->Text = l_strName;
			//cMap_CellData*l_pMap_CellData = m_pMapDataManager->GetObject(DNCT::GcStringToWchar(l_strName));1
			//POINT	l_GridSize = l_pMap_CellData->GetGridSize();
			//Vector2	l_vSize = l_pMap_CellData->GetSize();
			//Row_numericUpDown->Value = l_GridSize.x;
			//Column_numericUpDown->Value = l_GridSize.y;
			//GridWidth_numericUpDown->Value = (int)l_vSize.x;
			//GridHeight_numericUpDown->Value = (int)l_vSize.y;
		}
	}
}