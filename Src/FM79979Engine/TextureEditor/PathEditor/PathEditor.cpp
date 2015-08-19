// PathEditor.cpp : main project file.

#include "stdafx.h"
#include "PathEditor.h"
#include "../../../Include/IL/il.h"
#include "../../AllLibInclude.h"

namespace PathEditor
{
	System::Void EditPath::AddPath_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
		 if( AddPath_button != sender )
		 {
			if( WholePath_listBox->SelectedItem )
			{
				m_pCurveManagerEP->SetCurrentSelectedIndex(-1);
				m_pCurveManagerEP->RemoveObject(WholePath_listBox->SelectedIndex);
				this->WholePath_listBox->Items->Remove(this->WholePath_listBox->SelectedItem);
			}
		 }
		 else
		 {
		 	DNCT::InputNameDialog^l_pForm = gcnew DNCT::InputNameDialog;
			l_pForm->ShowDialog();
			String^	l_String = l_pForm->m_strTextName;
			if( l_pForm->m_strTextName && l_pForm->m_strTextName->Length )
			{
				if(!DNCT::CheckListContainStringAndAdd(this->WholePath_listBox,l_pForm->m_strTextName))
				{
					cCurveWithTime*l_pcCurveWithTime = new cCurveWithTime();
					l_pcCurveWithTime->SetName(DNCT::GcStringToWchar(l_pForm->m_strTextName));
					m_pCurveManagerEP->AddObject(l_pcCurveWithTime);
					WholePath_listBox->SelectedIndex = WholePath_listBox->Items->Count-1;
					l_pcCurveWithTime->SetLOD((int)PathLOD_numericUpDown->Value);
				}
				else
					WARNING_MSG("such name existed already");
			}
			delete l_pForm;
		 }
	 }
	System::Void EditPath::WholePath_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
	{
		if(WholePath_listBox->SelectedIndex==-1)
		{
			m_pCurveManagerEP->m_pCurrentCurve = 0;
			return;
		}
		m_pCurveManagerEP->SetCurrentSelectedIndex(WholePath_listBox->SelectedIndex);
		cCurveWithTime*l_cCurveWithTime = (*m_pCurveManagerEP)[m_pCurveManagerEP->GetCurrentSelectedIndex()];
		m_pCurveManagerEP->m_pCurrentCurve = l_cCurveWithTime;

		Loop_checkBox->Checked = l_cCurveWithTime->IsCurveLoop();

		std::vector<Vector3>	l_PointList = l_cCurveWithTime->GetOriginalPointList();
		std::vector<float>		l_TimeList = l_cCurveWithTime->GetOriginalTimeList();
		if( sender == WholePath_listBox )
		{
			PathLOD_numericUpDown->Value = l_cCurveWithTime->GetLOD();
			PathNode_listBox->Items->Clear();
			int	l_iNum = l_PointList.size();
			for( int i=0;i<l_iNum;++i )
			{
				Vector3	l_v2 = l_PointList[i];
				PathNode_listBox->Items->Add(i.ToString()+":X="+((int)l_v2.x).ToString()+",y="+((int)l_v2.y).ToString()+",z="+((int)l_v2.z).ToString()+",Time="+l_TimeList[i].ToString() );
			}
		}
		else
		if( sender == PathNode_listBox )
		{
			if( PathNode_listBox->SelectedIndex == -1 || m_iCurrentSelectedPathListIndex == PathNode_listBox->SelectedIndex  )
				return;
			m_iCurrentSelectedPathListIndex = PathNode_listBox->SelectedIndex;
			//WCHAR	l_str[TEMP_SIZE];
			//swprintf(l_str,TEMP_SIZE,L"Index = %d\n\0",PathNode_listBox->SelectedIndex);
			//OutputDebugString(l_str);
			PathNodePosX_numericUpDown->Value = (int)l_PointList[PathNode_listBox->SelectedIndex].x;
			PathNodePosY_numericUpDown->Value = (int)l_PointList[PathNode_listBox->SelectedIndex].y;
			PathNodePosZ_numericUpDown->Value = (int)l_PointList[PathNode_listBox->SelectedIndex].z;
			int l_Value = (int)(l_TimeList[PathNode_listBox->SelectedIndex]*1000.f);
			PathNodeTime_numericUpDown->Value = l_Value;
		}
	 }

	System::Void EditPath::PathNodePosX_numericUpDown_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
		if(WholePath_listBox->SelectedIndex==-1||PathNode_listBox->SelectedIndex == -1 )
			return;

		m_pCurveManagerEP->SetCurrentSelectedIndex(WholePath_listBox->SelectedIndex);
		cCurveWithTime*l_cCurveWithTime = (*m_pCurveManagerEP)[m_pCurveManagerEP->GetCurrentSelectedIndex()];
		std::vector<Vector3>	l_PointList = l_cCurveWithTime->GetOriginalPointList();
		std::vector<float>		l_TimeList = l_cCurveWithTime->GetOriginalTimeList();
		System::Collections::ArrayList^l_pSelectedIndices = gcnew System::Collections::ArrayList(PathNode_listBox->SelectedIndices);
		PathNode_listBox->SelectedIndices->Clear();
		for each( int l_iSelectedIndex in l_pSelectedIndices )
		{
			Vector3	l_vPos =  l_PointList[l_iSelectedIndex];
			if( sender == PathNodeTime_numericUpDown )
			{
				l_TimeList[l_iSelectedIndex] = (float)(PathNodeTime_numericUpDown->Value);
				l_TimeList[l_iSelectedIndex]/=1000.f;
			}
			else
			{
				if( sender == PathNodePosX_numericUpDown )
					l_vPos.x = (float)PathNodePosX_numericUpDown->Value;
				else
				if( sender == PathNodePosY_numericUpDown )
					l_vPos.y = (float)PathNodePosY_numericUpDown->Value;
				else
				if( sender == PathNodePosZ_numericUpDown )
					l_vPos.z = (float)PathNodePosZ_numericUpDown->Value;
				l_PointList[l_iSelectedIndex] = l_vPos;
			}
			float	l_fTime = l_TimeList[l_iSelectedIndex];
			l_vPos =  l_PointList[l_iSelectedIndex];
			PathNode_listBox->Items[l_iSelectedIndex] = l_iSelectedIndex.ToString()+":X="+((int)l_vPos.x).ToString()+",y="+((int)l_vPos.y).ToString()+",z="+((int)l_vPos.z).ToString()+",Time="+l_fTime.ToString();
			PathNode_listBox->SelectedIndices->Add(l_iSelectedIndex);
		}
		//if( l_TimeList.size()>1&&PathNode_listBox->SelectedIndex!=0 )
		//{
		//	if( (float)(PathNodeTime_numericUpDown->Value)<l_TimeList[PathNode_listBox->SelectedIndex-1] )
		//	{
		//		ErrorMsg_label->Text = "time can't smaller than prior one";
		//		return;
		//	}
		//}
		ErrorMsg_label->Text = "";
		l_cCurveWithTime->SetOriginalPointList(l_PointList);
		l_cCurveWithTime->SetOriginalTimeList(l_TimeList);
		l_cCurveWithTime->Refresh();
		m_iCurrentSelectedPathListIndex = PathNode_listBox->SelectedIndex;
	 }

	System::Void EditPath::PathNodeTime_numericUpDown_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
	{
				 System::Windows::Forms::KeyEventArgs^  e3;
				 PathNodePosX_numericUpDown_KeyUp(sender,e3);
	}

	System::Void EditPath::PathLOD_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
				if(WholePath_listBox->SelectedIndex!=-1)
				{
					m_pCurveManagerEP->GetObject(WholePath_listBox->SelectedIndex)->SetLOD((int)PathLOD_numericUpDown->Value);
					System::EventArgs^  e3;
					WholePath_listBox_SelectedIndexChanged(WholePath_listBox,e3);
				}
	}

	System::Void EditPath::ColorR_numericUpDown_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
				 System::Windows::Forms::KeyEventArgs^  e2;
				 ColorR_numericUpDown_KeyUp(sender,e2);
	}

	System::Void EditPath::ColorR_numericUpDown_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
				 if(WholePath_listBox->SelectedIndex == -1)
					 return;
	}

	System::Void EditPath::TotalPlayTime_numericUpDown_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
				 if( WholePath_listBox->SelectedIndex != -1 )
				 {
					if(m_pCurveManagerEP->m_pCurrentCurve)
					{
						std::vector<float>l_fTimeList =  m_pCurveManagerEP->m_pCurrentCurve->GetOriginalTimeList();
						if(m_pCurveManagerEP->m_pCurrentCurve)
						{
							std::vector<float> l_fTimeList = m_pCurveManagerEP->m_pCurrentCurve->GetOriginalTimeList();
							int	l_iNum = l_fTimeList.size();
							if(l_iNum == 1)//avoid only one point,we still have to setup data
							{
								l_iNum = 2;
								float	l_fTimeGap = (float)TotalPlayTime_numericUpDown->Value/(float)(l_iNum-1)/1000.f;
								l_fTimeList[0] = l_fTimeGap;
							}
							else
							{
								float	l_fTimeGap = (float)TotalPlayTime_numericUpDown->Value/(float)(l_iNum-1)/1000.f;
								for( int i=0;i<l_iNum;++i )
									l_fTimeList[i] = i*l_fTimeGap;
							}
							m_pCurveManagerEP->m_pCurrentCurve->SetOriginalTimeList(l_fTimeList);
							m_pCurveManagerEP->m_pCurrentCurve->Refresh();
							int	l_iIndex = WholePath_listBox->SelectedIndex;
							WholePath_listBox->SelectedIndex = -1;
							WholePath_listBox->SelectedIndex = l_iIndex;
						}
					}
				 }
	}

	System::Void EditPath::TotalPlayTime_numericUpDown_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
					System::Windows::Forms::KeyEventArgs^  e2;
					TotalPlayTime_numericUpDown_KeyUp(sender,e2);
	}

	System::Void EditPath::PathMode_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
				 if( PathMode_comboBox->SelectedIndex != -1 )
				 {
					if(  PathMode_comboBox->SelectedIndex == 0 )
						Play_checkBox->Checked = false;
					if( m_pCurveManagerEP )
						m_pCurveManagerEP->SetPathMode((ePathMode)PathMode_comboBox->SelectedIndex);
				 }
	}

	System::Void EditPath::DelPoint_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if( sender == InsertPoint_button )
		{
			 if( WholePath_listBox->SelectedIndex != -1 )
			 {
				 if( PathNode_listBox->SelectedIndex != -1 )
				 {
					if(m_pCurveManagerEP->m_pCurrentCurve)
					{
						Vector3	l_vPos = m_pCurveManagerEP->m_pCurrentCurve->GetOriginalPointList()[PathNode_listBox->SelectedIndex];
						float	l_fTime = m_pCurveManagerEP->m_pCurrentCurve->GetOriginalTimeList()[PathNode_listBox->SelectedIndex];
						if( PathNode_listBox->SelectedIndex+1<PathNode_listBox->Items->Count )
						{
							l_vPos += m_pCurveManagerEP->m_pCurrentCurve->GetOriginalPointList()[PathNode_listBox->SelectedIndex+1];
							l_vPos.x/=2.f;
							l_vPos.y/=2.f;
							l_vPos.z/=2.f;
						}
						m_pCurveManagerEP->m_pCurrentCurve->InsertPoint(l_vPos,l_fTime,PathNode_listBox->SelectedIndex+1);
						int	l_iIndex = WholePath_listBox->SelectedIndex;
						WholePath_listBox->SelectedIndex = -1;
						WholePath_listBox->SelectedIndex = l_iIndex;
					}
				 }
			 }
		}
		else
		{
			 if( WholePath_listBox->SelectedIndex != -1 )
			 {
				 if( PathNode_listBox->SelectedIndex != -1 )
				 {
					if(m_pCurveManagerEP->m_pCurrentCurve)
					{
						m_pCurveManagerEP->m_pCurrentCurve->DelPoint(PathNode_listBox->SelectedIndex);
						int	l_iIndex = WholePath_listBox->SelectedIndex;
						WholePath_listBox->SelectedIndex = -1;
						WholePath_listBox->SelectedIndex = l_iIndex;
					}
				 }				
			 }
		}
	}

	void	EditPath::PathEditElementControl(bool e_b)
	{
		if( e_b )//while it's play,only can adjust position ,can not asscep add point
			PathMode_comboBox->SelectedIndex =1;
		DelPoint_button->Enabled = e_b;
		PathNode_listBox->Enabled = e_b;
		PathLOD_numericUpDown->Enabled = e_b;
	}

	System::Void EditPath::Play_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
				 if(m_pCurveManagerEP->m_pCurrentCurve)
				 {
					 PathEditElementControl(!Play_checkBox->Checked);
					 if( Play_checkBox->Checked )
					 {
						m_pCurveManagerEP->m_pCurrentCurve->Init();
					 }
				 }
	}

	System::Void EditPath::Loop_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
				 if(m_pCurveManagerEP->m_pCurrentCurve)
				 {
					 if(Loop_checkBox->Checked)
						 m_pCurveManagerEP->m_pCurrentCurve->SetCurveLoop(true);
					 else
						 m_pCurveManagerEP->m_pCurrentCurve->SetCurveLoop(false);
				 }
	}

	System::Void EditPath::RefershPathData()
	{
		WholePath_listBox->Items->Clear();
		for( int i =0;i<m_pCurveManagerEP->Count();++i )
		{
			WholePath_listBox->Items->Add(String(UT::WcharToChar((*m_pCurveManagerEP)[i]->GetName()).c_str()).ToString());
		}
	}

	System::Void EditPath::PathEditFileOpen_ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 if( sender == saveToolStripMenuItem )
				 {
					 String^l_str = DNCT::SaveFileAndGetName(DNCT::ConvertExtensionToFilter(m_pCurveManagerEP->ExtensionName()));
					 if( l_str )
					 {
						l_str = ForceAddExtenName(l_str,(char*)m_pCurveManagerEP->ExtensionName());
						m_pCurveManagerEP->ExportData(DNCT::GcStringToChar(l_str));
//						FileToUnicode(l_str);
					 }
				 }
				 else
				 {
					 String^l_str = DNCT::OpenFileAndGetName("Path Files(*.dae;*.xml;*.path)|*.dae;*.xml;*.path|All files (*.*)|*.*");
					 if( l_str )
					 {
						 cCurveManager	*l_pDaeCurveManager = 0;
						 bool	l_bFromDaeFile = false;
						 if(System::IO::Path::GetExtension(l_str)->ToLower()->Equals(".dae"))
						 {
							//l_pDaeCurveManager = DaeDocumentToCurveManager(DNCT::GcStringToWchar(l_str));
							if( !l_pDaeCurveManager )
							{
								WARNING_MSG("no path files exists!");
								return;
							}
							l_bFromDaeFile = true;
						 }
						 if( sender == mergeToolStripMenuItem )
						 {
							bool	l_bReplace = false;
							cCurveManager	l_CurveManager;
							if( !l_bFromDaeFile )
								l_CurveManager.Parse(DNCT::GcStringToChar(l_str));
							else
							{
								l_pDaeCurveManager->DumpListPointer(&l_CurveManager);
							}
							for( int i=0;i<l_CurveManager.Count();++i )
							{
								if( m_pCurveManagerEP->GetObject(l_CurveManager.GetObject(i)->GetName()) )
								{
									WARING_YES_NO_TO_YES("some data is existed replace all file?")
										l_bReplace = true;
									break;
								}
							}
							if( l_bReplace )
							{
								for( int i=0;i<l_CurveManager.Count();++i )
								{//if name is same erase data and add it
									m_pCurveManagerEP->ReplaceObject(i,l_CurveManager.GetObject(i));
									l_CurveManager.RemoveObjectWithoutDelete(i);
								}
							}
							else
							{
								for( int i=0;i<l_CurveManager.Count();++i )
								{//if name is same erase data and add it
									if( !m_pCurveManagerEP->GetObject(l_CurveManager.GetObject(i)->GetName()) )
									{
										m_pCurveManagerEP->AddObject(l_CurveManager.GetObject(i));
										l_CurveManager.RemoveObjectWithoutDelete(i);
									}
								}
							}
						 }
						 else
						 {
							 m_pCurveManagerEP->Destroy();
							 m_pCurveManagerEP->SetCurrentSelectedIndex(-1);
							 if( !l_bFromDaeFile )
								m_pCurveManagerEP->Parse(DNCT::GcStringToChar(l_str));
							 else
								 l_pDaeCurveManager->DumpListPointer(m_pCurveManagerEP);
						 }
						 SAFE_DELETE(l_pDaeCurveManager);
						 RefershPathData();
					 }				
				 }
				 //PathEditFileOpen_ToolStripMenuItem
	}

	//void	EditPath::RenderSplitFrame()
	//{
	//	Vector4	l_vRect = m_pOrthogonalCamera->GetRECT();
	//	Vector2	l_vSize = m_pOrthogonalCamera->GetScreenViewPortSize();
	//	POINT l_ViewportSize = { splitContainer2->Panel1->Width,splitContainer2->Panel1->Height };
	//	m_pOrthogonalCamera->Render(l_ViewportSize);
	//	POINT	l_Pos = {(int)l_vRect.x,(int)l_vRect.y};
	//	RenderRectangle(l_Pos,(int)l_vSize.x,(int)l_vSize.y,Vector4(0,1,1,0.8f),0,10);
	//}
	void	EditPath::RenderSplit()
	{
		int	l_iWidth = splitContainer2->Panel1->Width;
		int	l_iHeight = splitContainer2->Panel1->Height;
		cFrameCamera	l_FrameCamera = &m_pSplitScreenCamera->m_sAxisCamera[3].FrameCamera;
		//opengl glViewport left lower corrner is 0,0,right upper is max wisth max height
		//first Left up,XView
		POINT	l_ViewPort = {l_iWidth/2,l_iHeight/2};
		//glDepthMask(GL_FALSE);
		//glDisable(GL_DEPTH_TEST);
		m_pSplitScreenCamera->RenderSplitScreen(l_ViewPort,MyMath::X);
		//glDepthMask(GL_TRUE);
		//glEnable(GL_DEPTH_TEST);
		Render(&m_pSplitScreenCamera->m_sAxisCamera[0].FrameCamera,&m_pSplitScreenCamera->m_sAxisCamera[0].OrthogonalCamera,l_ViewPort);
		//second Right Up,YView
		//glDisable(GL_DEPTH_TEST);
		//glDepthMask(GL_FALSE);
		m_pSplitScreenCamera->RenderSplitScreen(l_ViewPort,MyMath::Y);
		//glDepthMask(GL_TRUE);
		//glEnable(GL_DEPTH_TEST);
		Render(&m_pSplitScreenCamera->m_sAxisCamera[1].FrameCamera,&m_pSplitScreenCamera->m_sAxisCamera[1].OrthogonalCamera,l_ViewPort);
		//Third Left Down,ZView		
		//glDisable(GL_DEPTH_TEST);
		//glDepthMask(GL_FALSE);
		m_pSplitScreenCamera->RenderSplitScreen(l_ViewPort,MyMath::Z);
		//glDepthMask(GL_TRUE);
		//glEnable(GL_DEPTH_TEST);
		Render(&m_pSplitScreenCamera->m_sAxisCamera[2].FrameCamera,&m_pSplitScreenCamera->m_sAxisCamera[2].OrthogonalCamera,l_ViewPort);
		//Fourth Right Down
		//POINT l_ViewportSize = { splitContainer2->Panel1->Width,splitContainer2->Panel1->Height };
		//m_pSplitScreenCamera->m_sAxisCamera[3].OrthogonalCamera.Render(l_ViewportSize);
		//m_pSplitScreenCamera->m_sAxisCamera[3].OrthogonalCamera.DrawGrid(0,0,Vector4(0.5,0.5,0.f,1.f),2.f);
		//glDisable(GL_DEPTH_TEST);
		//glDepthMask(GL_FALSE);
		m_pSplitScreenCamera->RenderSplitScreen(l_ViewPort,MyMath::W);
		//glDepthMask(GL_TRUE);
		//glEnable(GL_DEPTH_TEST);
		Render(&m_pSplitScreenCamera->m_sAxisCamera[3].FrameCamera,&m_pSplitScreenCamera->m_sAxisCamera[3].OrthogonalCamera,l_ViewPort);
		RenderMousePos();
	}

	void	Test(System::Object^e_pTest)
	{
		EditPath^l_p = (EditPath^)e_pTest;
	}

	void	EditPath::Render(cFrameCamera*e_pFrameCamera,cOrthogonalCamera*e_pOrthogonalCamera,POINT e_ViewportSize)
	{
		//model render
		if( 0 )
		{
			if( this->m_pSplitScreenCamera->m_eProjectionType == Projection::Perspective )
			{

			}
			else
			{

			}
		}
		if( perspectiveToolStripMenuItem->Checked )
		{
			e_pFrameCamera->Render();
		}
		else
		{
			e_pOrthogonalCamera->Render();
			POINT	l_Pos = {0,0};
			RenderRectangle(l_Pos,(int)m_pvResolution->x,(int)m_pvResolution->y,Vector4(1.f,0.3f,0.3f,0.8f));
		}
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		if(m_pBKImageEP)
		{
			m_pBKImageEP->Render();
		}
		if( !Play_checkBox->Checked )
		{
			if( ShowImage_checkBox->Checked )
			{
				if( ImageName_listBox->SelectedIndex != -1 )
				{
					POINT	l_Pos = {(int)e_pOrthogonalCamera->GetMouseWorldPos().x,(int)e_pOrthogonalCamera->GetMouseWorldPos().y};
					cBaseImage*l_pImage = m_pBaseImageListEP->GetObject(ImageName_listBox->SelectedIndex);
					l_pImage->Render(l_Pos.x-(l_pImage->GetWidth()/2),l_Pos.y-(l_pImage->GetHeight()/2));
				}
			}
		}
		else
		{
			m_pCurveManagerEP->Update(m_pTimeAndFPS->fElpaseTime);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_ALPHA_TEST);
			m_pCurveManagerEP->RenderWithImage();
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_ALPHA_TEST);
		}
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
		if( perspectiveToolStripMenuItem->Checked )
		{
			if( PathMode_comboBox->SelectedIndex == 1 )//1 for fix point, 0 for add point
			{

			}		
		}
		else
		{
			Vector3	l_vMouseWorldPos = m_pSplitScreenCamera->GetCurrentMousePos();
			switch(m_pSplitScreenCamera->GetFocusPanelIndex())
			{
				case 0:
					break;
				case 1:
					break;
				case 2:
				case 3:
					break;
			}
			if( PathMode_comboBox->SelectedIndex == 1 )//1 for fix point, 0 for add point
			{
				//m_pCurveManagerEP->MouseMove(l_vMouseWorldPos);
			}
		}
		m_pCurveManagerEP->Render(Vector4(1,1,0,1));
		if( Play_checkBox->Checked )
		{
			m_pCurveManagerEP->Update(m_pTimeAndFPS->fElpaseTime);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_ALPHA_TEST);
			m_pCurveManagerEP->RenderWithImage();
		}
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
		//GLRender::glDisable2D();
	}

	void	EditPath::RenderMousePos()
	{
		glViewport(0,0,splitContainer2->Panel1->Width,splitContainer2->Panel1->Height);
		if( m_pDebugFont )
		{
			float	l_fScale = m_pDebugFont->GetScale();
			m_pDebugFont->SetScale(2.f);
			Vector3	l_vMousePos = m_pSplitScreenCamera->GetCurrentMousePos();
			POINT l_ViewportSize = { splitContainer2->Panel1->Width,splitContainer2->Panel1->Height };
			cOrthogonalCamera	l_OrthogonalCamera;
			l_OrthogonalCamera.Render();
			glEnable(GL_TEXTURE_2D);
			WCHAR*l_strMousePos;
			l_strMousePos = DNCT::GcStringToWchar("x:"+((int)l_vMousePos.x).ToString()+",y:"+((int)l_vMousePos.y).ToString()+",z:"+((int)l_vMousePos.z).ToString());
			m_pDebugFont->RenderFont(0,0,l_strMousePos);
			glDisable(GL_TEXTURE_2D);
			m_pDebugFont->SetScale(l_fScale);
		}
	}
}

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	ilInit();
	alutInit (0,0);
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 
	// Create the main window and run it
	Application::Run(gcnew PathEditor::EditPath());
	alutExit();
	ilShutDown();
	_CrtDumpMemoryLeaks();
	NamedTypedObject::DumpUnReleaseInfo();
	return 0;
}
