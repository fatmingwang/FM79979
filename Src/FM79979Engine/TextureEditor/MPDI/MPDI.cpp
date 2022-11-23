#include "stdafx.h"
#include "MPDI.h"
//#include "../../../Include/IL/il.h"
#include "../../Core/Common/BinaryFile/StringCompress.h"
#include "DotMPDICamera.h"
namespace FATMING_CORE
{
	extern bool	g_bSupportNonPowerOfTwoTexture;
}
namespace MPDI
{

//	MPDIEditor::MPDIEditor(GCFORM::TabControl^e_ptabControl,GCFORM::Form^e_pForm,String^e_strFileName)
	MPDIEditor::MPDIEditor(String^e_strFileName)
	{
		g_bSupportNonPowerOfTwoTexture = true;
		cGameApp::CreateDefaultOpenGLRender();
		FMLog::Init();
		m_pvBGColor = new Vector4(0, 0, 0, 1);
		if (System::IO::File::Exists("MPDISetup.xml"))
		{
			g_bLanguageChinese = true;
			cNodeISAX l_NodeISAX;
			if (l_NodeISAX.ParseDataIntoXMLNode("MPDISetup.xml"))
			{
				auto l_pRoot = l_NodeISAX.GetRootElement();
				PARSE_ELEMENT_START(l_pRoot)
					COMPARE_NAME("Chinese")
					{
						if (wcslen(l_strValue) == 4 || GetInt(l_strValue)>0)
							g_bLanguageChinese = true;
						else
							g_bLanguageChinese = false;
					}
					else
					COMPARE_NAME("BGColor")
					{
						*m_pvBGColor = GetVector4(l_strValue);
						cGameApp::m_spOpenGLRender->m_vBGColor = *m_pvBGColor;
					}
				PARSE_NAME_VALUE_END
				//Chinese = "true" NonPowerOfTwoSupport = "true" BGColor = "0.5,0.5,0.5,1"
			}
		}
		InitializeComponent();
		this->Dock = GCFORM::DockStyle::Fill;
		m_pMPDIUI = gcnew MPDIUI();
		this->MPDIExtra_xmlNodeEditComponent = (gcnew XMLDataEditor::XMLForm());
		m_bTimerInRun = false;
		m_pvResolution = new Vector2(1920,1080);
		m_pTimeAndFPS = new UT::sTimeAndFPS;
		m_pOrthogonalCamera = new cOrthogonalCamera();
		m_pTimeCounterForAutoSaveFile = new UT::sTimeCounter();
		m_sfactor = GL_SRC_ALPHA;
		m_dfactor = GL_ONE_MINUS_SRC_ALPHA;
		m_bDataChanged = false;

		m_HdcMV = GetDC((HWND)this->splitContainer3->Panel1->Handle.ToPointer());
		m_pGameApp = new cGameApp((HWND)this->splitContainer3->Panel1->Handle.ToPointer());
		m_pGameApp->Init();
		m_HGLRCMV = m_pGameApp->m_spOpenGLRender->m_HGLRC;
		m_fAlpha = 0.f;
		m_pMultiPathDynamicImageAWP = 0;
		m_pImagePlayManagerAWP = 0;
		m_pReferenceCurveManagerAWP = 0;
		m_pBKImageAWP  = 0;
		m_pMPDIList = 0;
		m_iCurrentSelectedPathListIndexAWP = -1;
		m_iCurrentSelectedPathNodeIndexAWP = -1;
		m_pInterlUseForShowImagePos = 0;
		m_pParentHandle = 0;
		m_pCurrentWorkingPI = 0;
		m_bPreventChangeAllTimeValueSetup = false;
		m_bPreventChangeValue = false;
		m_bMouseEnterPathNodeListbox = false;
		m_pParentHandle = 0;
		//m_pForm = e_pForm;
		//if( m_pForm->Parent )
		//{
		//	m_pParentHandle = (HWND)m_pForm->Parent->Handle.ToPointer();
		//}
		//if( e_ptabControl )
		//{
		//	m_pTabPage = gcnew GCFORM::TabPage();
		//	m_pTabPage->Text = "MPDI/路徑圖片編輯";
		//	m_pTabPage->AutoScroll = true;
		//	m_pTabPage->Controls->Add(this);
		//	e_ptabControl->TabPages->Add(m_pTabPage);
		//	m_pTabControl = e_ptabControl;
		//	m_pTabControl->SelectedIndexChanged += gcnew System::EventHandler(this, &MPDIEditor::tabControl_SelectedIndexChanged);
		//}
		//else
		//{
		//	e_pForm->Controls->Add(this);
		//}

		splitContainer3->Panel1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MPDIEditor::MyMouseMove);
		splitContainer3->Panel1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MPDIEditor::MyMouseDown);
		splitContainer3->Panel1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MPDIEditor::MyMouseUp);
		splitContainer3->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MPDIEditor::MyMouseUp);
		StayAtLastFrame_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MPDIEditor::Play_checkBox_CheckedChanged);
		m_pMPDIExtraData = gcnew System::Collections::Hashtable;

		UIInitialize();
		m_bAnimationPauseByTrackValue = false;
		//m_HdcMV = GetDC((HWND)splitContainer3->Panel1->Handle.ToPointer());		
		m_pDebugFont = m_pGameApp->m_spGlyphFontRender;
		GroupPositionMoveType_comboBox->SelectedIndex = 0;
		GroupRotationType_comboBox->SelectedIndex = 0;
		m_pTimeCounterForAutoSaveFile->SetTargetTime(300.f);
		//1
		//m_pMPDICamera = gcnew cDotNEtMPDICamera(Camera_tabPage->Controls,this->m_pOrthogonalCamera);
		//2
		//System::Windows::Forms::Integration::ElementHost^l_pElementHots = DNCT::GetMyElementHost();
		//l_pElementHots->Dock = DockStyle::Fill;
		//if( 1 )
		//{
		//	CoreWPF::VideoPlayer::VideoPlayer^l_pData = gcnew CoreWPF::VideoPlayer::VideoPlayer();
		//	l_pElementHots->Child = l_pData;
		//	m_pMPDIThread = gcnew cMPDIThread();
		//	m_pMPDIThread->SetVideoPlayer(l_pData);
		//	l_pElementHots->Child = l_pData;
		//}
		//else
		//{
		//	TextureEditorWPF::MPDI_PathPointData^l_pData = gcnew TextureEditorWPF::MPDI_PathPointData;
		//	l_pElementHots->Child = l_pData;
		//}
		//3
		System::Windows::Forms::Integration::ElementHost^l_pElementHots = GetMyElementHost(l_pElementHots);
		l_pElementHots->Dock = DockStyle::Fill;
		CoreWPF::Common::FMTreeView^l_pFMTreeView = gcnew CoreWPF::Common::FMTreeView();
		l_pElementHots->Child = l_pFMTreeView;
		Camera_tabPage->Controls->Add(l_pElementHots);
		//Video_groupBox->Controls->Add(l_pElementHots);
		if( e_strFileName && e_strFileName->Length )
			OpenMPDIFile(e_strFileName);
		OutputDebugString(L"Video has memory leak so I am lazy to do this\n");

		{
			ElementHost^l_pHost = GetMyElementHost(l_pHost);
			l_pHost->Dock = DockStyle::Fill;
			m_pBGColorPicker = gcnew CoreWPF::Common::ColorPicker;
			l_pHost->Child = m_pBGColorPicker;
			BGColor_panel->Controls->Add(l_pHost);
			m_pBGColorPicker->f_SelectedColorChanged += gcnew System::EventHandler(this, &MPDIEditor::ColorChanged);
		}
		

		this->Path_tabPage->Controls->Add(m_pMPDIUI);
		this->timer1->Enabled = true;
	}

	MPDIEditor::~MPDIEditor()
	{
		if (components)
		{
			delete components;
		}
		delete m_pBGColorPicker;
		if( m_pMPDICamera )
			delete m_pMPDICamera;
		SAFE_DELETE(m_pvBGColor);
		SAFE_DELETE(m_pvResolution);
		SAFE_DELETE(m_pMultiPathDynamicImageAWP);
		if( m_pMPDIList&& !m_pMPDIList->GetOwner() )
		{
			SAFE_DELETE(m_pMPDIList);
		}
		SAFE_DELETE(m_pBKImageAWP);		
		SAFE_DELETE(m_pTimeAndFPS);
		SAFE_DELETE(m_pReferenceCurveManagerAWP);
		SAFE_DELETE(m_pInterlUseForShowImagePos);
		SAFE_DELETE(m_pTimeCounterForAutoSaveFile);
		SAFE_DELETE(m_pOrthogonalCamera);
		SAFE_DELETE(m_pImagePlayManagerAWP);
		SAFE_DELETE(m_pGameApp);
		delete m_pMPDIUI;
		if( m_pMPDIThread )
			delete m_pMPDIThread;
		FMLog::Destroy();
	}


	void	MPDIEditor::DelPoint(System::Object^  sender, System::EventArgs^  e)
	{
		if( sender == m_pMPDIUI->InsertPoint_button )
		{
			 if( m_pMPDIUI->WholePath_listBox->SelectedIndex != -1 )
			 {
				 if( m_pMPDIUI->PathNode_listBox->SelectedIndex != -1 )
				 {
					 if(m_pMultiPathDynamicImageAWP->m_pCurrentCurve&& (int)m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalPointList().size()>m_pMPDIUI->PathNode_listBox->SelectedIndex)
					{
						int	l_iNumPoint = (int)m_pMPDIUI->NumPointsToInsert_numericUpDown->Value;
						Vector3	l_vPos = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalPointList()[m_pMPDIUI->PathNode_listBox->SelectedIndex];
						float	l_fTime = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalTimeList()[m_pMPDIUI->PathNode_listBox->SelectedIndex];
						//relate  to select point
						float	l_fNextPointTime = l_fTime;
						if( m_pMPDIUI->PathNode_listBox->Items->Count-1>m_pMPDIUI->PathNode_listBox->SelectedIndex )
						{
							l_fNextPointTime = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalTimeList()[m_pMPDIUI->PathNode_listBox->SelectedIndex+1];
						}
						float	l_fStepX = 0.f;
						float	l_fStepY = 0.f;
						if( m_pMPDIUI->PathNode_listBox->SelectedIndex+1<m_pMPDIUI->PathNode_listBox->Items->Count )
						{
							Vector3	l_vDestPos = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalPointList()[m_pMPDIUI->PathNode_listBox->SelectedIndex+1];
							l_fStepX = (l_vDestPos.x-l_vPos.x)/l_iNumPoint;
							l_fStepY = (l_vDestPos.y-l_vPos.y)/l_iNumPoint;
						}
						l_iNumPoint--;
						float	l_fStepTime = (l_fNextPointTime-l_fTime)/(l_iNumPoint+1);
						for( int i=0;i<l_iNumPoint;++i )
						{
							l_vPos.x+=l_fStepX;
							l_vPos.y+=l_fStepY;
							sTexBehaviorDataWithImageIndexData	*l_pTexBehaviorDataWithImageIndexData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[m_pMPDIUI->PathNode_listBox->SelectedIndex];
							m_pMultiPathDynamicImageAWP->m_pCurrentCurve->InsertPoint(l_vPos,(l_fStepTime*(i+1))+l_fTime,l_pTexBehaviorDataWithImageIndexData,m_pMPDIUI->PathNode_listBox->SelectedIndex+1+i);
						}
						int	l_iIndex = m_pMPDIUI->WholePath_listBox->SelectedIndex;
						m_pMPDIUI->WholePath_listBox->SelectedIndex = -1;
						m_pMPDIUI->WholePath_listBox->SelectedIndex = l_iIndex;
					}
				 }
			 }
		}
		else
		if( sender == m_pMPDIUI->DelPoint_button )
		{
			//if(AllPlay_checkBox->Checked)
			//{
			//	WARNING_MSG("please stop play");
			//	return;
			//}
			 if( m_pMPDIUI->WholePath_listBox->SelectedIndex != -1 )
			 {
				 if( m_pMPDIUI->PathNode_listBox->SelectedIndex != -1 )
				 {
					if(m_pMultiPathDynamicImageAWP->m_pCurrentCurve)
					{
						this->timer1->Enabled = false;
						while( m_bTimerInRun )
						{
						}
						System::Collections::ArrayList^	l_MyList = gcnew System::Collections::ArrayList;
						l_MyList->AddRange(m_pMPDIUI->PathNode_listBox->SelectedItems);
						for each (System::Object^l_pObject in l_MyList)
						{
							int	l_iIndex = m_pMPDIUI->PathNode_listBox->Items->IndexOf(l_pObject);
							if( l_iIndex == -1 )
								continue;
							m_pMPDIUI->PathNode_listBox->Items->Remove(l_pObject);
							m_pMultiPathDynamicImageAWP->m_pCurrentCurve->DelPoint(l_iIndex);
						}
						m_pMultiPathDynamicImageAWP->RefreshTotalPlayTime();
						m_pMultiPathDynamicImageAWP->RefreshData();
						CurrentProgressValueChange(1);
						int	l_iIndex = m_pMPDIUI->WholePath_listBox->SelectedIndex;
						m_pMPDIUI->WholePath_listBox->SelectedIndex = -1;
						m_pMPDIUI->WholePath_listBox->SelectedIndex = l_iIndex;
						this->timer1->Enabled = true;
					}
				 }
			 }
		}	
	}

	void	MPDIEditor::PathSelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if( m_pMPDIUI->WholePath_listBox->SelectedIndex == -1 )
		{
			m_pMultiPathDynamicImageAWP->m_pCurrentCurve = 0;
			m_pMPDIUI->PathNode_listBox->Items->Clear();
			AllEventPointHint_listBox->Items->Clear();
			return;
		}
		m_bPreventChangeValue = true;
		m_pMultiPathDynamicImageAWP->SetCurrentSelectedIndex(m_pMPDIUI->WholePath_listBox->SelectedIndex);
		if( !m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
			return;
		cCurveWithTime*l_cCurveWithTime = (*m_pMultiPathDynamicImageAWP)[m_pMultiPathDynamicImageAWP->GetCurrentSelectedIndex()];

		m_pMPDIUI->Loop_checkBox->Checked = l_cCurveWithTime->IsCurveLoop();

		std::vector<Vector3>	l_PointList = l_cCurveWithTime->GetOriginalPointList();
		std::vector<float>		l_TimeList = l_cCurveWithTime->GetOriginalTimeList();
		if( sender == m_pMPDIUI->WholePath_listBox )
		{
			m_bPreventChangeAllTimeValueSetup = true;
			if( m_pMultiPathDynamicImageAWP->Count()>m_pMPDIUI->WholePath_listBox->SelectedIndex )
			{
				m_pMultiPathDynamicImageAWP->m_pCurrentCurve = (*m_pMultiPathDynamicImageAWP)[m_pMPDIUI->WholePath_listBox->SelectedIndex];
				m_pMPDIUI->ImageAnchor_comboBox->SelectedIndex = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetAnchorType();
				m_pMPDIUI->EnableColorBlending_checkBox->Checked = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->IsColorBlending();
				m_pMPDIUI->BehaviorUpdateActive_checkBox->Checked = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->IsBehaviorUpdateActive();
				m_pMPDIUI->RotationPosOffset_xyzNumeriaclContro->SetValue(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetRotationAnglePosOffset());
			}
			m_pMPDIUI->PathLOD_numericUpDown->Value = l_cCurveWithTime->GetLOD();
			m_pMPDIUI->PathNode_listBox->Items->Clear();
			int	l_iNum = (int)l_PointList.size();
			for( int i=0;i<l_iNum;++i )
			{
				Vector3	l_v2 = l_PointList[i];
				//String^l_strTime = l_TimeList[i].ToString();
				//m_pMPDIUI->PathNode_listBox->Items->Add( i.ToString()+":X="+((int)l_v2.x).ToString()+",y="+((int)l_v2.y).ToString()+",z="+((int)l_v2.z).ToString()+",Time="+l_TimeList[i].ToString() );
				if( m_pImageResourceManagerAWP->Count() )
				{
					cCueToStartCurveWithTime*l_pCueToStartCurveWithTime = m_pMultiPathDynamicImageAWP->m_pCurrentCurve;
					int	l_iIndex = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[i]->iImageIndex;
					if( l_iIndex != -1 )
					{
					    sPuzzleData*l_pAllPuzzleData = 0;
					    if( (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[i]->pPI )
					    {
							l_pAllPuzzleData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[i]->pPI->GetAllPuzzleData();
					    }
					    else
							l_pAllPuzzleData = m_pMPDIList->GetAllPuzzleData();
						m_pMPDIUI->PathNode_listBox->Items->Add( i.ToString()+":"+DNCT::WcharToGcstring(l_pAllPuzzleData[l_iIndex].strFileName )+":X="+((int)l_v2.x).ToString()+",y="+((int)l_v2.y).ToString()+",z="+((int)l_v2.z).ToString()+",Time="+l_TimeList[i].ToString() );
					}
				}
			}
			if( l_TimeList.size() && !AllPlay_checkBox->Checked )
			{
				//CurrentProgress_trackBar->Maximum = (int)(l_TimeList[l_TimeList.size()-1]*1000);
				CurrentProgressValueChange(2);
			}
			//OffsetPos_checkBox->Checked = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->IsAvtivePosOffset();
			m_pMPDIUI->StartTime_numericUpDown->Value = (int)(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetStartTime()*1000);
			m_pMPDIUI->TotalPlayTime_numericUpDown->Value = (int)((m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetEndTime()-m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetStartTime())*1000);
			m_bPreventChangeValue = false;
			m_bPreventChangeAllTimeValueSetup = false;
			AllEventPointHint_listBox->Items->Clear();
			cCueToStartCurveWithTime*l_pCueToStartCurvesWithTime = m_pMultiPathDynamicImageAWP->GetObject(m_pMPDIUI->WholePath_listBox->SelectedIndex);
			std::vector<sHintPoint>*l_pVectorPointer = l_pCueToStartCurvesWithTime->GetHintPointVector();
			for( UINT i=0;i<l_pVectorPointer->size();++i )
			{
				sHintPoint*l_pHintPoint = &(*l_pVectorPointer)[i];
				AllEventPointHint_listBox->Items->Add(
					l_pHintPoint->iPointIndex.ToString()+":"+
					gcnew String(eAnimationEventToAnimationEventString(l_pHintPoint->AnimationEvent))+","+
					DNCT::WcharToGcstring(l_pHintPoint->strEventName));
			}
			if( moveAllPathToolStripMenuItem->Checked ||  rotateAllPathToolStripMenuItem->Checked )
			{
				m_pMultiPathDynamicImageAWP->m_SelectedIndicesToMovingOrRotating.clear();
				for each (int l_iSelectedIndex in m_pMPDIUI->WholePath_listBox->SelectedIndices)
				{
					m_pMultiPathDynamicImageAWP->m_SelectedIndicesToMovingOrRotating.push_back(l_iSelectedIndex);
				}
			}
			if( sender == m_pMPDIUI->WholePath_listBox )
			{
				if( m_pMPDIUI->PathNode_listBox->Items->Count == 1 )
					m_pMPDIUI->PathNode_listBox->SelectedIndex = 0;
			}
		}
		else
		if( sender == m_pMPDIUI->PathNode_listBox )
		{
			if( m_pMPDIUI->PathNode_listBox->SelectedIndex == -1 || (m_iCurrentSelectedPathNodeIndexAWP == m_pMPDIUI->PathNode_listBox->SelectedIndex && m_iCurrentSelectedPathListIndexAWP == m_pMPDIUI->WholePath_listBox->SelectedIndex) )
			{
				m_bPreventChangeValue = false;
				return;
			}
			if( m_pMPDIUI->PathNode_listBox->SelectedIndices->Count!=1 )
			{
				m_bPreventChangeValue = false;
				return;
			}
			if( !m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
				m_pMultiPathDynamicImageAWP->m_pCurrentCurve = (*m_pMultiPathDynamicImageAWP)[m_pMPDIUI->WholePath_listBox->SelectedIndex];
			sTexBehaviorDataWithImageIndexData	*l_pTexBehaviorDataWithImageIndexData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[m_pMPDIUI->PathNode_listBox->SelectedIndex];
			m_iCurrentSelectedPathNodeIndexAWP = m_pMPDIUI->PathNode_listBox->SelectedIndex;
			m_iCurrentSelectedPathListIndexAWP = m_pMPDIUI->WholePath_listBox->SelectedIndex;
			//WCHAR	l_str[TEMP_SIZE];
			//swprintf(l_str,TEMP_SIZE,L"Index = %d\n\0",m_pMPDIUI->PathNode_listBox->SelectedIndex);
			//OutputDebugString(l_str);
			m_pMPDIUI->PathNodePosX_numericUpDown->Value = (int)l_PointList[m_pMPDIUI->PathNode_listBox->SelectedIndex].x;
			m_pMPDIUI->PathNodePosY_numericUpDown->Value = (int)l_PointList[m_pMPDIUI->PathNode_listBox->SelectedIndex].y;
			m_pMPDIUI->PathNodePosZ_numericUpDown->Value = (int)l_PointList[m_pMPDIUI->PathNode_listBox->SelectedIndex].z;
			m_pMPDIUI->AngleX_numericUpDown->Value = (int)l_pTexBehaviorDataWithImageIndexData->vAngle.x;
			m_pMPDIUI->AngleY_numericUpDown->Value = (int)l_pTexBehaviorDataWithImageIndexData->vAngle.y;
			m_pMPDIUI->AngleZ_numericUpDown->Value = (int)l_pTexBehaviorDataWithImageIndexData->vAngle.z;
			m_pMPDIUI->Width_numericUpDown->Value = (int)l_pTexBehaviorDataWithImageIndexData->Size.x;
			m_pMPDIUI->Height_numericUpDown->Value = (int)l_pTexBehaviorDataWithImageIndexData->Size.y;
			Vector4	l_vColor = l_pTexBehaviorDataWithImageIndexData->vColor;
			m_pMPDIUI->RedColor_numericUpDown->Value = System::Decimal(l_vColor.x);
			m_pMPDIUI->GreenColor_numericUpDown->Value = System::Decimal(l_vColor.y);
			m_pMPDIUI->BlueColor_numericUpDown->Value = System::Decimal(l_vColor.z);
			m_pMPDIUI->AlphaColor_numericUpDown->Value = System::Decimal(l_vColor.w);
			this->m_pMPDIUI->Mirror_checkBox->Checked = l_pTexBehaviorDataWithImageIndexData->bMirror;
			int l_Value = (int)(l_TimeList[m_pMPDIUI->PathNode_listBox->SelectedIndex]*1000.f);
			m_pMPDIUI->PathNodeTime_numericUpDown->Value = (int)l_Value;
			m_pMPDIUI->StartTime_numericUpDown->Value = (int)(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetStartTime()*1000);
			m_pMPDIUI->BehaviorUpdateActive_checkBox->Checked = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->IsBehaviorUpdateActive();
			if( sender == AddShowImageOnScreenObject_button )
			{
				for each( int l_iIndex in m_pMPDIUI->PathNode_listBox->SelectedIndices )
				{
					sTexBehaviorDataWithImageIndexData *l_pTexBehaviorDataWithImageIndexData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[l_iIndex];
					Vector3	l_vPos = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalPointList()[l_iIndex];
					m_pInterlUseForShowImagePos->AddPoint(l_vPos,0,l_pTexBehaviorDataWithImageIndexData->Size,l_pTexBehaviorDataWithImageIndexData->vAngle,l_pTexBehaviorDataWithImageIndexData->vColor,l_pTexBehaviorDataWithImageIndexData->iImageIndex,l_pTexBehaviorDataWithImageIndexData->bMirror,l_pTexBehaviorDataWithImageIndexData->pPI);
					ShowImageOnScreen_listBox->Items->Add(m_pMPDIUI->WholePath_listBox->SelectedItem->ToString()+":"+l_iIndex.ToString());
					m_pInterlUseForShowImagePos->SetAnimationDone(false);
					m_pInterlUseForShowImagePos->SetStart(true);
				}
			}
		}
		m_bPreventChangeValue = false;
	}

	void	MPDIEditor::PlayCheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		 if( !m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
			 return;
		 if( sender == m_pMPDIUI->Play_checkBox )
		 {
			 if( m_pMPDIUI->Play_checkBox->Checked )
			 {
				 if( !m_pMultiPathDynamicImageAWP )
				 {
					 m_pMPDIUI->Play_checkBox->Checked = false;
					 return;
				 }
				 m_pMPDIUI->TotalPlayTime_numericUpDown->Enabled = false;
				 m_pMultiPathDynamicImageAWP->RefreshTotalPlayTime();
				 ((cFMTimeLineAnimationRule*)(m_pMultiPathDynamicImageAWP->m_pCurrentCurve))->Init();
				 m_pMultiPathDynamicImageAWP->m_pCurrentCurve->SetStart(true);
				 AllPlay_checkBox->Checked = false;
				 m_fTimeForCurrentProgess = 0.f;
				 size_t	l_iSize = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalTimeList().size();
				 if( l_iSize )
				 {
					 CurrentProgressValueChange(1);
				 }
			 }
			 else
				 m_pMPDIUI->TotalPlayTime_numericUpDown->Enabled = true;
			 m_bAnimationPauseByTrackValue = !m_pMPDIUI->Play_checkBox->Checked;
			 if( m_pMPDIThread )
				this->m_pMPDIThread->GetVideo()->Play(sender,e);
		 }
		 else
		 if( sender == m_pMPDIUI->Loop_checkBox )
		 {
			 if( m_bPreventChangeValue == false )
			 {
				for each(int l_iSelectedIndex in m_pMPDIUI->WholePath_listBox->SelectedIndices)
					m_pMultiPathDynamicImageAWP->GetObject(l_iSelectedIndex)->SetAnimationLoop(m_pMPDIUI->Loop_checkBox->Checked);
			 }
		 }
		 else
		 if( sender == AllPlay_checkBox )
		 {
			 if( !m_pMultiPathDynamicImageAWP )
			 {
				 AllPlay_checkBox->Checked = false;
				 return;
			 }
			 if( AllPlay_checkBox->Checked )
			 {
				 m_pMultiPathDynamicImageAWP->Init();
				 m_fTimeForCurrentProgess = 0.f;
				 m_pMPDIUI->TotalPlayTime_numericUpDown->Enabled = false;
			 }
			 else
			 {
				 m_pMPDIUI->TotalPlayTime_numericUpDown->Enabled = true;
			 }
			 if( AllPlay_checkBox->Checked )
				m_pMPDIUI->Play_checkBox->Checked = false;
			 m_pMultiPathDynamicImageAWP->RefreshTotalPlayTime();
			 CurrentProgressValueChange(1);
			 m_bAnimationPauseByTrackValue = !AllPlay_checkBox->Checked;
			 if( m_pMPDIThread&&this->m_pMPDIThread->GetVideo() )
				this->m_pMPDIThread->GetVideo()->Play(sender,e);
			 if( AllPlay_checkBox->Checked && m_pMPDICamera )
			 {
				m_pMPDICamera->m_bDoUpdate = AllPlay_checkBox->Checked;
				m_pMPDICamera->m_p2DCameraAnimation->Init();
			 }
		 }
		 else
		 if( sender == StayAtLastFrame_checkBox )
		 {
			 if( m_pMultiPathDynamicImageAWP )
			 {
				 m_pMultiPathDynamicImageAWP->SetStayAtLastFrame(StayAtLastFrame_checkBox->Checked);
			 }
		 }
		 else
		 if( sender == AllCurveLoop_checkBox )
		 {
			 if( m_pMultiPathDynamicImageAWP )
			 {
				 if(AllCurveLoop_checkBox->Checked)
					 m_pMultiPathDynamicImageAWP->SetPlayCount(-1);
				 else
					 m_pMultiPathDynamicImageAWP->SetPlayCount(1);
			 }
		 }
	}

	void	MPDIEditor::AddPath(String^e_strPathName)
	{
		if( !m_pMPDIList )
			return;
		if(!DNCT::CheckListContainStringAndAdd(this->m_pMPDIUI->WholePath_listBox,e_strPathName))
		{
			m_pMPDIUI->WholePath_listBox->SelectedItems->Clear();
			cCueToStartCurveWithTime*l_pCueToStartCurvesWithTimeAWP = new cCueToStartCurveWithTime(m_pMPDIList);
			l_pCueToStartCurvesWithTimeAWP->SetName(DNCT::GcStringToWchar(e_strPathName));			
			if( !m_pMultiPathDynamicImageAWP )
				m_pMultiPathDynamicImageAWP = new cMPDIForEditor(m_pMPDIList);
			m_pMultiPathDynamicImageAWP->AddObject(l_pCueToStartCurvesWithTimeAWP);
			m_pMultiPathDynamicImageAWP->RefreshData();
			m_pMultiPathDynamicImageAWP->m_pCurrentCurve = l_pCueToStartCurvesWithTimeAWP;
			m_pMPDIUI->RotationPosOffset_xyzNumeriaclContro->SetValue(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetRotationAnglePosOffset());
			l_pCueToStartCurvesWithTimeAWP->SetLOD((int)m_pMPDIUI->PathLOD_numericUpDown->Value);
			m_pMPDIUI->WholePath_listBox->SelectedIndex = m_pMPDIUI->WholePath_listBox->Items->Count-1;
		}
		else
			WARNING_MSG("such name existed already");	
	}

	void	MPDIEditor::AddPath(System::Object^  sender, System::EventArgs^  e)
	{
		 if( m_pMPDIUI->DelPath_button == sender )
		 {
			if( m_pMPDIUI->WholePath_listBox->SelectedItem )
			{
				AllPlay_checkBox->Checked = false;
				m_pMultiPathDynamicImageAWP->SetCurrentSelectedIndex(-1);

				for each (String^l_strSelectedName in m_pMPDIUI->WholePath_listBox->SelectedItems)
				{
					m_pMultiPathDynamicImageAWP->RemoveObject(DNCT::GcStringToWchar(l_strSelectedName));
				}
				while(m_pMPDIUI->WholePath_listBox->SelectedItems->Count>0)
					m_pMPDIUI->WholePath_listBox->Items->Remove(m_pMPDIUI->WholePath_listBox->SelectedItem);
				if( m_pMultiPathDynamicImageAWP->Count() == 0 )
				{
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve = 0;
					m_pMPDIUI->RotationPosOffset_xyzNumeriaclContro->SetValue(0);
				}
				else
				{
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve = m_pMultiPathDynamicImageAWP->GetObject(0);
					m_pMPDIUI->RotationPosOffset_xyzNumeriaclContro->SetValue(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetRotationAnglePosOffset());
				}
				m_pMultiPathDynamicImageAWP->RefreshData();
				m_pMPDIUI->PathNode_listBox->Items->Clear();
				if( AllPlay_checkBox->Checked )
				{
					CurrentProgressValueChange(1);
					//if( CurrentProgress_trackBar->Minimum>=CurrentProgress_trackBar->Maximum )
					//	CurrentProgress_trackBar->Minimum = 0;
				}
			}
		 }
		 else
		 if( sender == m_pMPDIUI->AddPath_button )
		 {
			if( !m_pMPDIList )
				return;
			DNCT::InputNameDialog^l_pForm = gcnew DNCT::InputNameDialog;
			if(m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedItem)
			{
				l_pForm->SetupTextBoxData(m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedItem->ToString());
			}
			l_pForm->ShowDialog();
			if( l_pForm->m_strTextName && l_pForm->m_strTextName->Length )
			{
				AddPath(l_pForm->m_strTextName);
			}
			m_pMPDIUI->ImageAnchor_comboBox->SelectedIndex = eIAT_CENTER_CENTER;
			m_pMPDIUI->Play_checkBox->Checked = false;
			delete l_pForm;
		 }
		 else
		 if( sender == m_pMPDIUI->ClonePath_button )
		 {
			if( m_pMPDIUI->WholePath_listBox->SelectedItem )
			{
	 			DNCT::InputNameDialog^l_pForm = gcnew DNCT::InputNameDialog;

				if(m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedItem)
				{
					l_pForm->SetupTextBoxData(m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedItem->ToString());
				}
				else
					l_pForm->SetupTextBoxData(m_pMPDIUI->WholePath_listBox->SelectedItem->ToString());
				l_pForm->ShowDialog();
				if( l_pForm->m_strTextName && l_pForm->m_strTextName->Length )
				{
					if(!DNCT::CheckListContainStringAndAdd(this->m_pMPDIUI->WholePath_listBox,l_pForm->m_strTextName))
					{
						std::wstring	l_strName = DNCT::GcStringToWchar(m_pMPDIUI->WholePath_listBox->SelectedItem->ToString());
						cCueToStartCurveWithTime*l_pOriginalCueToStartCurvesWithTimeAWP = m_pMultiPathDynamicImageAWP->GetObject(l_strName);
						cCueToStartCurveWithTime*l_pCueToStartCurvesWithTime = new cCueToStartCurveWithTime(l_pOriginalCueToStartCurvesWithTimeAWP);

						l_pCueToStartCurvesWithTime->SetName(DNCT::GcStringToWchar(l_pForm->m_strTextName));

						m_pMultiPathDynamicImageAWP->AddObject(l_pCueToStartCurvesWithTime);
						m_pMultiPathDynamicImageAWP->m_pCurrentCurve = l_pCueToStartCurvesWithTime;
						l_pCueToStartCurvesWithTime->SetLOD((int)m_pMPDIUI->PathLOD_numericUpDown->Value);
						m_pMPDIUI->WholePath_listBox->SelectedIndices->Clear();
						m_pMPDIUI->WholePath_listBox->SelectedIndex = m_pMPDIUI->WholePath_listBox->Items->Count-1;
					}
					else
						WARNING_MSG("such name existed already");
				}
				delete l_pForm;
			}
		 }
		 else
		 if( sender == m_pMPDIUI->InvertPath_button )
		 {
			 if( m_pMultiPathDynamicImageAWP )
			 {
				 for each( int l_iSelectedIndex in m_pMPDIUI->WholePath_listBox->SelectedIndices )
				 {
					 m_pMultiPathDynamicImageAWP->SetCurrentSelectedIndex(l_iSelectedIndex);
					 if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
					 {
						 m_pMultiPathDynamicImageAWP->m_pCurrentCurve->InvertOrder();
					 }
				 }
				 WholePath_listBox_SelectedIndexChanged(m_pMPDIUI->WholePath_listBox,e);
			 }
		 }
		 else
		 if( m_pMPDIUI->InvertAllPath_button == sender )
		 {
			 if( m_pMPDIUI->WholePath_listBox->SelectedIndex != -1 )
			 {
				 if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
				 {
					 for( int i=0;i<m_pMultiPathDynamicImageAWP->Count();++i )
					 {
						(*m_pMultiPathDynamicImageAWP)[i]->InvertOrder();
					 }
					 WholePath_listBox_SelectedIndexChanged(m_pMPDIUI->WholePath_listBox,e);
				 }
			 }
		 }
	}

	void	MPDIEditor::AddIntoList()
	{
		 if(!m_pMultiPathDynamicImageAWP)
		 {
			WARNING_MSG("make a animation before u add new one");
			return;
		 }
		 String^l_strErrorMsg = MultiPathDynamicImageDataCheck(m_pMultiPathDynamicImageAWP);
		 if( l_strErrorMsg&&l_strErrorMsg->Length )
		 {
			WARNING_MSG(l_strErrorMsg);
		 }
		 bool	l_bNewObject = true;
		 std::wstring l_strName = DNCT::GcStringToWchar( AnimationName_textBox->Text );
		 if( CheckListContainStringAndAdd(WholeAnimationList_listBox,AnimationName_textBox->Text ) )
		 {
			 WARING_YES_NO_TO_NO("such object name already exists,would u like to replace?"+DNCT::GetChanglineString()+
				 "該物件名稱已存在,要覆蓋嗎?")
			 {
				return;
			 }
			 else
			 {
				l_bNewObject = false;
				m_pMPDIList->RemoveObject(l_strName);
				WholeAnimationList_listBox->Items->Remove(AnimationName_textBox->Text);
			 }
		 }
		 cMultiPathDynamicImage*l_pMultiPathDynamicImageAWP = new cMultiPathDynamicImage(m_pMultiPathDynamicImageAWP);
		 l_pMultiPathDynamicImageAWP->SetName(l_strName);
		 m_pMPDIList->AddObject(l_pMultiPathDynamicImageAWP);
		 if( !l_bNewObject )
			WholeAnimationList_listBox->Items->Add(AnimationName_textBox->Text);
		 if(this->m_pMPDIExtraData->ContainsKey(AnimationName_textBox->Text))
		 {
			 m_pMPDIExtraData->Remove(AnimationName_textBox->Text);
		 }
		 if( MPDIExtra_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Count )
		 {
			 MPDIExtra_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes[0]->Text = AnimationName_textBox->Text;
			 m_pMPDIExtraData[AnimationName_textBox->Text] = MPDIExtra_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes[0]->Clone();
		 }
		 MPDIExtra_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();

		 if(m_strCurrentSaveFileName&&System::IO::File::Exists(m_strCurrentSaveFileName))
		 {
			Save(m_strCurrentSaveFileName);
		 }
		 else
		 {
			System::EventArgs^  e;
			SaveAndOpen_MPDI(saveToolStripMenuItem,e);
		 }
	}

	void	MPDIEditor::MPDIRender()
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(m_sfactor,m_dfactor);
		glEnable(GL_TEXTURE_2D);
		if(m_pBKImageAWP)
		{
			if(this->showBGToolStripMenuItem->Checked)
				m_pBKImageAWP->Render();
		}
		if( m_pMultiPathDynamicImageAWP )
		{
			POINT	l_CameraMousePos = {(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y};
			if(m_pMPDIUI->EnableColorBlending_checkBox->Checked)
			{
				glBlendFunc(IndexToBlendingIndex(this->m_pMPDIUI->SrcBlending_comboBox->SelectedIndex),IndexToBlendingIndex(this->m_pMPDIUI->DestBlending_comboBox->SelectedIndex));
			}
			if(m_pMPDIUI->EnableColorBlending_checkBox->Checked)
			{
				glBlendFunc(m_sfactor,m_dfactor);		
			}
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_ALPHA_TEST);
			if( m_pMPDIUI->ShowAllPath_checkBox->Checked )
			{
				m_pMultiPathDynamicImageAWP->RenderALLCurve();
			}
			m_pOrthogonalCamera->Render();
			//???it should call in mouse move event???
			//if( fixPointModeToolStripMenuItem->Checked)
			//{
			//	m_pMultiPathDynamicImageAWP->MouseMove(l_CameraMousePos);
			//}
			if( m_pMPDIThread )
			{
				m_pMPDIThread->GetVideo()->SetCurrentVideoTime(CurrentProgress_trackBar->Value/1000.f);
				m_pMPDIThread->GetVideo()->UpdateTrackBar();
				m_pMPDIThread->Render();
			}
			if( m_pMPDIUI->Play_checkBox->Checked )
			{
				if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
				{
					size_t	l_iSize = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalTimeList().size();
					if( l_iSize )
					{
						float	l_fTotalPlayTime = m_pMultiPathDynamicImageAWP->GetEndTime();
						
						if( m_bAnimationPauseByTrackValue == false )
							m_fTimeForCurrentProgess+=m_pTimeAndFPS->fElpaseTime;
						label9->Text = "Progress:"+CurrentProgress_trackBar->Value.ToString();
						if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve->IsAnimationLoop())
						{
							if( m_fTimeForCurrentProgess>=l_fTotalPlayTime )
								m_fTimeForCurrentProgess = 0.f;
						}
						if( m_fTimeForCurrentProgess>=l_fTotalPlayTime )
						{
							int	l_iDestValue = (int)(l_fTotalPlayTime*1000.f);
							if( CurrentProgress_trackBar->Maximum >= l_iDestValue )							
								CurrentProgress_trackBar->Value = l_iDestValue;
						}
						else
						{
							int	l_iDestValue = (int)(m_fTimeForCurrentProgess*1000.f);
							if( CurrentProgress_trackBar->Maximum >= l_iDestValue )
								CurrentProgress_trackBar->Value = l_iDestValue;
						}
						//m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPositionByTime(m_fTimeForCurrentProgess);
						//m_pMultiPathDynamicImageAWP->Update(m_pTimeAndFPS->fElpaseTime);
						glEnable(GL_TEXTURE_2D);
						glEnable(GL_ALPHA_TEST);
						//m_pMultiPathDynamicImageAWP->RenderWithImage();
						//if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetCurrentImageIndex() !=-1 )
						{
							//int	l_i = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetCurrentImageIndex();
							//WCHAR*l_strPIName = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPuzzleData()[m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetCurrentImageIndex()]->strFileName;
							//m_pImageResourceManagerAWP->GetPuzzleImage(l_strPIName)->ApplyImage();
							//int	l_iImageIndex = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetCurrentImageIndex();
							m_pMultiPathDynamicImageAWP->m_pCurrentCurve->RenderByGlobalTime(m_fTimeForCurrentProgess);
							//m_pMultiPathDynamicImageAWP->m_pCurrentCurve->Render(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPuzzleData()[l_iImageIndex]);
						}
						glDisable(GL_TEXTURE_2D);
						glDisable(GL_ALPHA_TEST);
					}
				}
			}
			else
			//if( AllPlay_checkBox->Checked )
			{
				if( m_pMultiPathDynamicImageAWP )
				{
					glEnable(GL_TEXTURE_2D);
					glEnable(GL_ALPHA_TEST);
					m_pMultiPathDynamicImageAWP->UpdateByGlobalTime(m_fTimeForCurrentProgess);
					label9->Text = "Progress:"+CurrentProgress_trackBar->Value.ToString();


					Vector4*l_pViewPort = m_pMultiPathDynamicImageAWP->GetViewPort();
					if( l_pViewPort )
					{
						glEnable(GL_SCISSOR_TEST);
						Vector4 l_ResultViewPort;
						POINT l_ViewportSize = { splitContainer3->Panel1->Width,splitContainer3->Panel1->Height };
						Vector2	l_vViewPort((float)l_ViewportSize.x,(float)l_ViewportSize.y);
						//fuck here do this for scissor
						cGameApp::m_spOpenGLRender->m_vGameResolution = m_pOrthogonalCamera->GetScreenViewPortSize();
						if(m_pOrthogonalCamera->ViewportConvert(*l_pViewPort,l_vViewPort,&l_ResultViewPort))
						{
							glScissor((int)l_ResultViewPort.x,(int)l_ResultViewPort.y,(int)l_ResultViewPort.z,(int)l_ResultViewPort.w);
						}
					}
					m_pMultiPathDynamicImageAWP->RenderByGlobalTime();
					if(showImageBoderToolStripMenuItem->Checked)
						m_pMultiPathDynamicImageAWP->RenderCollide();
					if( l_pViewPort )
					{
						glDisable(GL_SCISSOR_TEST);
					}
					if( m_bAnimationPauseByTrackValue == false )
						m_fTimeForCurrentProgess+=m_pTimeAndFPS->fElpaseTime;
					if( m_pMultiPathDynamicImageAWP->GetCurrentProgress() < 1.f )
					{
						int	l_iDestValue = (int)(m_pMultiPathDynamicImageAWP->GetEndTime()*1000*m_pMultiPathDynamicImageAWP->GetCurrentProgress());
						if( CurrentProgress_trackBar->Maximum >= l_iDestValue )
						{
							CurrentProgress_trackBar->Value = l_iDestValue;
						}
					}
					if( m_pMultiPathDynamicImageAWP->GetPlayCount() == -1 )
					{
						if( m_fTimeForCurrentProgess >= m_pMultiPathDynamicImageAWP->GetEndTime() )
						{
							m_fTimeForCurrentProgess = 0.f;
						}
					}
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_ALPHA_TEST);
				}
			}
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_ALPHA_TEST);
			RenderHintPoint(m_pInterlUseForShowImagePos,showImageBoderToolStripMenuItem->Checked);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_ALPHA_TEST);
			if( showPIImageToolStripMenuItem->Checked&&addPointModeToolStripMenuItem->Checked )
			{
				if( m_pCurrentWorkingPI&&m_pMultiPathDynamicImageAWP->m_pBasemageForShow )
				{
					glEnable(GL_TEXTURE_2D);
					glEnable(GL_ALPHA_TEST);
					sPuzzleData*l_pMainPuzzleData = this->m_pCurrentWorkingPI->GetAllPuzzleData();
					sPuzzleData*l_pPuzzleData = &l_pMainPuzzleData[m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedIndex];
					POINT	l_OriginalSize = l_pPuzzleData->OriginalSize;
					POINT	l_ShowPos = {l_CameraMousePos.x,l_CameraMousePos.y};
					//POINT	l_OffsetPos = l_pPuzzleData->OffsetPos;
					m_pMultiPathDynamicImageAWP->m_pBasemageForShow->Render(l_ShowPos.x,l_ShowPos.y);
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_ALPHA_TEST);
					if(m_pMPDIUI->ShowSelectedImageBorder_checkBox->Checked)
					{
						l_ShowPos.x = l_CameraMousePos.x;
						l_ShowPos.y = l_CameraMousePos.y;
						RenderRectangle(l_ShowPos,l_OriginalSize.x,l_OriginalSize.y,Vector4(1.f,1.f,0.3f,0.8f));
					}
				}
			}
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_ALPHA_TEST);
			if( showSelectedImageToolStripMenuItem->Checked )
			{
				if( m_pMultiPathDynamicImageAWP->m_pBasemageForShow )
				{
					Vector2	l_vCameraPos = m_pOrthogonalCamera->GetCameraPos();
					m_pMultiPathDynamicImageAWP->m_pBasemageForShow->Render((int)(l_vCameraPos.x-m_pOrthogonalCamera->GetResolution().x/2*m_pOrthogonalCamera->GetScale()),
						(int)(l_vCameraPos.y-m_pOrthogonalCamera->GetResolution().y/2*m_pOrthogonalCamera->GetScale()));
				}
			}
			cCueToStartCurveWithTime	l_CueToStartCurvesWithTime(m_pMPDIList);
			{//render selected node
				if( !AllPlay_checkBox->Checked )
				{
					for each( int l_iIndex in m_pMPDIUI->PathNode_listBox->SelectedIndices )
					{
						if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve && (int)m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList()->size()>l_iIndex )
						{
							sTexBehaviorDataWithImageIndexData *l_pTexBehaviorDataWithImageIndexData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[l_iIndex];
							Vector3	l_vPos = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalPointList()[l_iIndex];
							Vector4	l_vColor = l_pTexBehaviorDataWithImageIndexData->vColor;
							if( flashSelectedImageToolStripMenuItem->Checked )
								l_vColor.w = m_fAlpha;
							l_CueToStartCurvesWithTime.AddPoint(l_vPos,0,l_pTexBehaviorDataWithImageIndexData->Size,
								l_pTexBehaviorDataWithImageIndexData->vAngle,l_vColor,l_pTexBehaviorDataWithImageIndexData->iImageIndex,l_pTexBehaviorDataWithImageIndexData->bMirror,l_pTexBehaviorDataWithImageIndexData->pPI,false);
						}
					}
					if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
					{
						l_CueToStartCurvesWithTime.SetRotationAnglePosOffset(*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetRotationAnglePosOffset());
						l_CueToStartCurvesWithTime.SetAnchorType(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetAnchorType());
					}
					l_CueToStartCurvesWithTime.SetAnimationDone(false);
					l_CueToStartCurvesWithTime.SetStart(true);
					RenderHintPoint(&l_CueToStartCurvesWithTime, showImageBoderToolStripMenuItem->Checked);
				}
			}
			glDisable(GL_ALPHA_TEST);
		}
		else
		{
			if( m_pMPDIThread )
			{
				m_pMPDIThread->Update(m_pTimeAndFPS->fElpaseTime);
				m_pMPDIThread->Render();
			}
		}

		if( m_pDebugFont )
		{
			glEnable(GL_BLEND);
			glBlendFunc(m_sfactor,m_dfactor);
			glEnable(GL_TEXTURE_2D);
			glEnable2D(1920,1080);
			for(int i=0;i<m_pMPDIUI->PathNode_listBox->SelectedIndices->Count;++i)
			{
				int	l_iSelectedIndex = m_pMPDIUI->PathNode_listBox->SelectedIndices[i];
				sTexBehaviorDataWithImageIndexData	*l_pTexBehaviorDataWithImageIndexData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[l_iSelectedIndex];
				sPuzzleData*l_pPuzzleData = l_pTexBehaviorDataWithImageIndexData->pPI->GetPuzzleData(l_pTexBehaviorDataWithImageIndexData->iImageIndex);
//				float	l_fVertices[8];
				//float	l_fUV[8];
				m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPositionByTime(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalTimeList()[l_iSelectedIndex]);
				//cMatrix44	l_mat2DTransform = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetRenderPuzzleDataAndMatrix(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetCurrentPointData(),2,l_fVertices,l_fUV,l_pPuzzleData,m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalPointList()[l_iSelectedIndex],l_pTexBehaviorDataWithImageIndexData->vAngle);
				//Vector3	l_vPos(l_fVertices[0],l_fVertices[1],0.f);
				//l_vPos = l_mat2DTransform.TransformCoordinate(l_vPos);
				//std::wstring	l_strLeftUpPos = L"Pos:";
				//l_strLeftUpPos += ValueToStringW(l_vPos);
				//m_pDebugFont->RenderFont(0,0+30*i+30,l_strLeftUpPos.c_str());
			}
			glDisable(GL_TEXTURE_2D);
		}

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
		m_pOrthogonalCamera->Render();
		if( showPath_ToolStripMenuItem->Checked )
		{
			//if( flashSelectedImageToolStripMenuItem->Checked )
			//	m_fAlpha = 1.f;
			if( m_pMultiPathDynamicImageAWP )
				m_pMultiPathDynamicImageAWP->Render(Vector4(1,m_fAlpha,m_fAlpha,0.5f));
		}
		//GLRender::glDisable2D();
	}

	void	MPDIEditor::AssignValueChange(System::Object^  sender)
	{
		int	l_iWholePathSelectedIndec = m_pMPDIUI->WholePath_listBox->SelectedIndex;
		if( m_pMPDIUI->WholePath_listBox->SelectedIndex != -1 && m_pMPDIUI->WholePath_listBox->SelectedItems->Count == 1 &&
			m_pMPDIUI->PathNode_listBox->SelectedIndices->Count >0)//only for one path,and specific nodes.
		{
			if( m_pMPDIUI->StartTime_numericUpDown == sender )
			{
				for each(int l_iSelectedIndex in m_pMPDIUI->WholePath_listBox->SelectedIndices)
				{
					m_pMultiPathDynamicImageAWP->GetObject(l_iSelectedIndex)->SetStartTime((float)m_pMPDIUI->StartTime_numericUpDown->Value/1000.f);
				}
				return;
			}
			if(m_pMPDIUI->PathNode_listBox->SelectedIndex == -1)
				return;
			int	l_iSmallestIndex = m_pMPDIUI->PathNode_listBox->Items->Count;
			for each ( int l_SelectedIndex in m_pMPDIUI->PathNode_listBox->SelectedIndices )
				if(l_iSmallestIndex>l_SelectedIndex)
					l_iSmallestIndex = l_SelectedIndex;
			//find first data want to change,and get it's original data to do a data mining
			sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[l_iSmallestIndex];
			m_pMultiPathDynamicImageAWP->SetCurrentSelectedIndex(m_pMPDIUI->WholePath_listBox->SelectedIndex);
			bool	l_bAeverageAssign = m_pMPDIUI->AverageAssign_checkBox->Checked;
			Vector3	l_vAngle((float)m_pMPDIUI->AngleX_numericUpDown->Value,(float)m_pMPDIUI->AngleY_numericUpDown->Value,(float)m_pMPDIUI->AngleZ_numericUpDown->Value);
			float	l_fStartAngle = l_pTexBehaviorDataWithImageIndexData->vAngle.z;

			Vector4	l_vColor = Vector4((float)m_pMPDIUI->RedColor_numericUpDown->Value,
									  (float)m_pMPDIUI->GreenColor_numericUpDown->Value,
									  (float)m_pMPDIUI->BlueColor_numericUpDown->Value,
									  (float)m_pMPDIUI->AlphaColor_numericUpDown->Value);
			Vector3	l_vFinalolor(0.f,0.f,0.f);
			float	l_fScaleValue = 1.f;
			Vector2	l_vStartSize = l_pTexBehaviorDataWithImageIndexData->Size;
			float	l_fWantedSize = 0.f;//for width or height
			if( sender == m_pMPDIUI->Width_numericUpDown)
			{
				l_fWantedSize = (float)m_pMPDIUI->Width_numericUpDown->Value;
			}
			else
			if( sender == m_pMPDIUI->Height_numericUpDown)
			{
				l_fWantedSize = (float)m_pMPDIUI->Height_numericUpDown->Value;
			}
			if( l_bAeverageAssign )
			{
				if( m_pMPDIUI->PathNode_listBox->SelectedIndices->Count>1 )
					l_fWantedSize/=(m_pMPDIUI->PathNode_listBox->SelectedIndices->Count-1);
				else
					l_fWantedSize/=m_pMPDIUI->PathNode_listBox->SelectedIndices->Count;
			}
			//to get each step value if l_bAeverageAssign is true
			if( sender == m_pMPDIUI->SizeScale_numericUpDown )
			{
				l_fScaleValue = (float)m_pMPDIUI->SizeScale_numericUpDown->Value/100.f;
				if( l_bAeverageAssign )
				{
					if( m_pMPDIUI->PathNode_listBox->SelectedIndices->Count>1 )
					{
						l_fScaleValue/=(m_pMPDIUI->PathNode_listBox->SelectedIndices->Count-1);
					}
					else
					{
						l_fScaleValue/=m_pMPDIUI->PathNode_listBox->SelectedIndices->Count;
					}
				}
			}
			
			if( sender == m_pMPDIUI->AngleX_numericUpDown || sender == m_pMPDIUI->AngleY_numericUpDown || sender == m_pMPDIUI->AngleZ_numericUpDown )
			{
				//here is a little bit tricky,ex:the points is 6 and angle is 360,so angle for each point 360/6 = 60,
				//but the final point we won't do it,because it's end point,so we have to do as below
				//each point 360/(6-1) = 72,
				if( l_bAeverageAssign )
				{
					if( m_pMPDIUI->PathNode_listBox->SelectedIndices->Count>1 )
					{
						l_vAngle.x/=(m_pMPDIUI->PathNode_listBox->SelectedIndices->Count-1);
						l_vAngle.y/=(m_pMPDIUI->PathNode_listBox->SelectedIndices->Count-1);
						l_vAngle.z/=(m_pMPDIUI->PathNode_listBox->SelectedIndices->Count-1);
					}
					else
					{
						l_vAngle.x/=m_pMPDIUI->PathNode_listBox->SelectedIndices->Count;
						l_vAngle.y/=m_pMPDIUI->PathNode_listBox->SelectedIndices->Count;
						l_vAngle.z/=m_pMPDIUI->PathNode_listBox->SelectedIndices->Count;
					}
				}
			}
			if( m_pMPDIUI->RedColor_numericUpDown == sender )
			{
				if( l_bAeverageAssign )
				{
					if( m_pMPDIUI->PathNode_listBox->SelectedIndices->Count>1 )
						l_vFinalolor.z = l_vColor.x / (float)(m_pMPDIUI->PathNode_listBox->SelectedIndices->Count-1);
					else
						l_vFinalolor.z = l_vColor.y / (float)(m_pMPDIUI->PathNode_listBox->SelectedIndices->Count);
				}
			}
			if( m_pMPDIUI->GreenColor_numericUpDown == sender )
			{
				if( l_bAeverageAssign )
				{
					if( m_pMPDIUI->PathNode_listBox->SelectedIndices->Count>1 )
						l_vFinalolor.z = l_vColor.y / (float)(m_pMPDIUI->PathNode_listBox->SelectedIndices->Count-1);
					else
						l_vFinalolor.z = l_vColor.y / (float)(m_pMPDIUI->PathNode_listBox->SelectedIndices->Count);
				}
			}
			if( m_pMPDIUI->BlueColor_numericUpDown == sender )
			{
				if( l_bAeverageAssign )
				{
					if( m_pMPDIUI->PathNode_listBox->SelectedIndices->Count>1 )
						l_vFinalolor.z = l_vColor.z / (float)(m_pMPDIUI->PathNode_listBox->SelectedIndices->Count-1);
					else
						l_vFinalolor.z = l_vColor.z / (float)(m_pMPDIUI->PathNode_listBox->SelectedIndices->Count);
				}
			}
			if( m_pMPDIUI->AlphaColor_numericUpDown == sender )
			{
				if( l_bAeverageAssign )
				{
					if( m_pMPDIUI->PathNode_listBox->SelectedIndices->Count>1 )
						l_vFinalolor.z = l_vColor.w / (float)(m_pMPDIUI->PathNode_listBox->SelectedIndices->Count-1);
					else
						l_vFinalolor.z = l_vColor.w / (float)(m_pMPDIUI->PathNode_listBox->SelectedIndices->Count);
				}
			}			

			System::Collections::ArrayList^l_pSelectedIdices = gcnew System::Collections::ArrayList();
			for(int i=0;i<m_pMPDIUI->PathNode_listBox->SelectedIndices->Count;++i)
			{
				int l_iSelectedIndex =  m_pMPDIUI->PathNode_listBox->SelectedIndices[i];
				l_pSelectedIdices->Add(l_iSelectedIndex);
				l_pTexBehaviorDataWithImageIndexData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[l_iSelectedIndex];
				if( sender == m_pMPDIUI->AngleX_numericUpDown||sender == m_pMPDIUI->AngleY_numericUpDown||sender == m_pMPDIUI->AngleZ_numericUpDown )
				{
					if( l_bAeverageAssign )
					{
						l_pTexBehaviorDataWithImageIndexData->vAngle.x = l_fStartAngle+l_vAngle.x*i;
						l_pTexBehaviorDataWithImageIndexData->vAngle.y = l_fStartAngle+l_vAngle.y*i;
						l_pTexBehaviorDataWithImageIndexData->vAngle.z = l_fStartAngle+l_vAngle.z*i;
					}
					else
					{
						l_pTexBehaviorDataWithImageIndexData->vAngle.x = l_vAngle.x;
						l_pTexBehaviorDataWithImageIndexData->vAngle.y = l_vAngle.y;
						l_pTexBehaviorDataWithImageIndexData->vAngle.z = l_vAngle.z;
					}
				}
				Vector3	l_Pos = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalPointList()[l_iSelectedIndex];
				float	l_fTime = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalTimeList()[l_iSelectedIndex];
				if( sender == m_pMPDIUI->PathNodeTime_numericUpDown)
					l_fTime = (float)m_pMPDIUI->PathNodeTime_numericUpDown->Value/1000.f;
				if( sender == m_pMPDIUI->PathNodePosX_numericUpDown)
					l_Pos.x = (float)m_pMPDIUI->PathNodePosX_numericUpDown->Value;
				if( sender == m_pMPDIUI->PathNodePosY_numericUpDown)
					l_Pos.y = (float)m_pMPDIUI->PathNodePosY_numericUpDown->Value;
				else
				if( sender == m_pMPDIUI->PathNodePosZ_numericUpDown)
					l_Pos.z = (float)m_pMPDIUI->PathNodePosZ_numericUpDown->Value;
				if( sender == m_pMPDIUI->Width_numericUpDown)
				{
					//float	l_fOriginaleWidth = l_pTexBehaviorDataWithImageIndexData->Size.x;
					if( l_bAeverageAssign )
						l_pTexBehaviorDataWithImageIndexData->Size.x = l_vStartSize.x+l_fWantedSize*i;
					else
						l_pTexBehaviorDataWithImageIndexData->Size.x = l_fWantedSize;
					//float	l_fChangedWidth = l_pTexBehaviorDataWithImageIndexData->Size.x-l_fOriginaleWidth;
					//switch(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetAnchorType())
					//{
					//	case eIAT_LEFT_UP:
					//	case eIAT_LEFT_CENTER:
					//	case eIAT_LEFT_DOWN:
					//		l_Pos.x -= l_fChangedWidth;
					//		break;
					//	case eIAT_CENTER_DOWN:
					//	case eIAT_CENTER_CENTER:
					//	case eIAT_CENTER_UP:
					//		break;
					//	case eIAT_RIGHT_UP:
					//	case eIAT_RIGHT_CENTER:
					//	case eIAT_RIGHT_DOWN:
					//		l_Pos.x += l_fChangedWidth;
					//		break;
					//}
				}
				if( sender == m_pMPDIUI->Height_numericUpDown)
				{
					//float	l_fOriginaleHeight = l_pTexBehaviorDataWithImageIndexData->Size.y;
					if( l_bAeverageAssign )
						l_pTexBehaviorDataWithImageIndexData->Size.y = l_vStartSize.y+l_fWantedSize*i;
					else
						l_pTexBehaviorDataWithImageIndexData->Size.y = l_fWantedSize;
					//float	l_fChangedHeight = l_pTexBehaviorDataWithImageIndexData->Size.y-l_fOriginaleHeight;
					//switch(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetAnchorType())
					//{
					//	case eIAT_LEFT_UP:
					//	case eIAT_RIGHT_UP:
					//	case eIAT_CENTER_UP:
					//		l_Pos.y -= l_fChangedHeight;
					//		break;
					//	case eIAT_RIGHT_CENTER:
					//	case eIAT_LEFT_CENTER:
					//	case eIAT_CENTER_CENTER:
					//		break;
					//	case eIAT_CENTER_DOWN:
					//	case eIAT_RIGHT_DOWN:
					//	case eIAT_LEFT_DOWN:
					//		l_Pos.y += l_fChangedHeight;
					//		break;
					//}
				}
				
				m_pMultiPathDynamicImageAWP->m_pCurrentCurve->FixPoint(l_iSelectedIndex,l_Pos,l_fTime);
				m_pMultiPathDynamicImageAWP->m_pCurrentCurve->Refresh();
				if( m_pMPDIUI->RedColor_numericUpDown == sender )
				{
					if( l_bAeverageAssign )
						l_pTexBehaviorDataWithImageIndexData->vColor.x = (l_vFinalolor.z*i);
					else
						l_pTexBehaviorDataWithImageIndexData->vColor.x = l_vColor.x;
				}
				if( m_pMPDIUI->GreenColor_numericUpDown == sender )
				{
					if( l_bAeverageAssign )
						l_pTexBehaviorDataWithImageIndexData->vColor.y = (l_vFinalolor.z*i);
					else
						l_pTexBehaviorDataWithImageIndexData->vColor.y = l_vColor.y;
				}
				if( m_pMPDIUI->BlueColor_numericUpDown == sender )
				{
					if( l_bAeverageAssign )
						l_pTexBehaviorDataWithImageIndexData->vColor.z = (l_vFinalolor.z*i);
					else
						l_pTexBehaviorDataWithImageIndexData->vColor.z = l_vColor.z;
				}
				if( m_pMPDIUI->AlphaColor_numericUpDown == sender )
				{
					if( l_bAeverageAssign )
						l_pTexBehaviorDataWithImageIndexData->vColor.w = (l_vFinalolor.z*i);
					else
						l_pTexBehaviorDataWithImageIndexData->vColor.w = l_vColor.w;
				}
				if( sender == m_pMPDIUI->SizeScale_numericUpDown )
				{
					sPuzzleData*l_pPuzzleData = l_pTexBehaviorDataWithImageIndexData->pPI->GetPuzzleData(l_pTexBehaviorDataWithImageIndexData->iImageIndex);
					if( l_bAeverageAssign )
					{
						
						l_pTexBehaviorDataWithImageIndexData->Size = Vector2(l_pPuzzleData->Size.x*(i*l_fScaleValue),l_pPuzzleData->Size.y*(i*l_fScaleValue));
					}
					else
					{
						l_pTexBehaviorDataWithImageIndexData->Size = Vector2(l_pPuzzleData->Size.x*l_fScaleValue,l_pPuzzleData->Size.y*l_fScaleValue);
					}
				}
				m_pMultiPathDynamicImageAWP->m_pCurrentCurve->FixPoint(l_iSelectedIndex,l_Pos,l_fTime);
				//m_pMPDIUI->PathNode_listBox->Items[l_iSelectedIndex] = gcnew String( l_iSelectedIndex.ToString()+":X="+l_Pos.x.ToString()+",y="+l_Pos.y.ToString()+",Time="+l_fTime.ToString() );
			}
			m_pMPDIUI->WholePath_listBox->SelectedIndex = -1;
			m_pMPDIUI->WholePath_listBox->SelectedIndex = l_iWholePathSelectedIndec;
			while(l_pSelectedIdices->Count)
			{
				m_pMPDIUI->PathNode_listBox->SelectedIndices->Add(System::Int16::Parse(l_pSelectedIdices[0]->ToString()));
				l_pSelectedIdices->RemoveAt(0);
			}
		 }
		 else
		 if( m_pMPDIUI->WholePath_listBox->Items->Count>=1 )
		 {
			for each(int l_iSelecteIndex in m_pMPDIUI->WholePath_listBox->SelectedIndices)
			{
				m_pMultiPathDynamicImageAWP->SetCurrentSelectedIndex(l_iSelecteIndex);//set current ccurve as our expection
				cCueToStartCurveWithTime*l_pCueToStartCurvesWithTime = m_pMultiPathDynamicImageAWP->GetObject(l_iSelecteIndex);
				if( m_pMPDIUI->StartTime_numericUpDown == sender )
				{
					l_pCueToStartCurvesWithTime->SetStartTime((float)m_pMPDIUI->StartTime_numericUpDown->Value/1000.f);
				}
				bool	l_bAeverageAssign = m_pMPDIUI->AverageAssign_checkBox->Checked;
				int	l_iNumPoint = (int)l_pCueToStartCurvesWithTime->GetOriginalPointList().size();
				float	l_fStartAngle = 0.f;
				Vector2	l_vStartSize;
				float	l_fWantedSize = 0.f;//for width or height
				if( sender == m_pMPDIUI->Width_numericUpDown )
					l_fWantedSize = (float)m_pMPDIUI->Width_numericUpDown->Value;
				else
				if( m_pMPDIUI->Height_numericUpDown == sender )
					l_fWantedSize = (float)m_pMPDIUI->Height_numericUpDown->Value;
				if( l_bAeverageAssign )
				{
					if( l_iNumPoint>1 )
						l_fWantedSize /= (l_iNumPoint-1);
					else
						l_fWantedSize /= l_iNumPoint;
				}
				for(int i=0;i<l_iNumPoint;++i)
				{
					int l_SelectedIndex =  i;
					sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = (*l_pCueToStartCurvesWithTime->GetPointDataList())[l_SelectedIndex];
					if( i == 0 )
					{
						l_fStartAngle = l_pTexBehaviorDataWithImageIndexData->vAngle.z;
						l_vStartSize = l_pTexBehaviorDataWithImageIndexData->Size;
					}
					Vector3	l_vAngle((float)m_pMPDIUI->AngleX_numericUpDown->Value,(float)m_pMPDIUI->AngleY_numericUpDown->Value,(float)m_pMPDIUI->AngleZ_numericUpDown->Value);
					Vector4	l_vColor = Vector4((float)m_pMPDIUI->RedColor_numericUpDown->Value,
											  (float)m_pMPDIUI->GreenColor_numericUpDown->Value,
											  (float)m_pMPDIUI->BlueColor_numericUpDown->Value,
											  (float)m_pMPDIUI->AlphaColor_numericUpDown->Value);
					Vector3	l_vFinalolor(0.f,0.f,0.f);
					float	l_fScaleValue = (float)m_pMPDIUI->SizeScale_numericUpDown->Value/100.f;
					if( sender == m_pMPDIUI->SizeScale_numericUpDown )
					{
						if( l_bAeverageAssign )
						{
							if( l_iNumPoint>1 )
								l_fScaleValue/=(l_iNumPoint-1);
							else
								l_fScaleValue/=l_iNumPoint;
						}
					}
					if( sender == m_pMPDIUI->AngleX_numericUpDown ||sender == m_pMPDIUI->AngleY_numericUpDown ||sender == m_pMPDIUI->AngleZ_numericUpDown )
					{
						//here is a little bit tricky,ex:the points is 6 and angle is 360,so angle for each point 360/6 = 60,
						//but the final point we won't do it,because it's end point,so we have to do as below
						//each point 360/(6-1) = 72,
						if( l_bAeverageAssign )
						{
							if( l_iNumPoint>1 )
							{
								l_vAngle.x/=(l_iNumPoint-1);
								l_vAngle.y/=(l_iNumPoint-1);
								l_vAngle.z/=(l_iNumPoint-1);
							}
							else
							{
								l_vAngle.x/=l_iNumPoint;
								l_vAngle.y/=l_iNumPoint;
								l_vAngle.z/=l_iNumPoint;
							}
						}
					}
					if( m_pMPDIUI->RedColor_numericUpDown == sender )
					{
						if( l_bAeverageAssign )
						{
							if( l_iNumPoint>1 )
								l_vFinalolor.z = l_vColor.x / (float)(l_iNumPoint-1);
							else
								l_vFinalolor.z = l_vColor.x / (float)(l_iNumPoint);
						}
					}
					if( m_pMPDIUI->GreenColor_numericUpDown == sender )
					{
						if( l_bAeverageAssign )
						{
							if( l_iNumPoint>1 )
								l_vFinalolor.z = l_vColor.y / (float)(l_iNumPoint-1);
							else
								l_vFinalolor.z = l_vColor.y / (float)(l_iNumPoint);
						}
					}
					if( m_pMPDIUI->BlueColor_numericUpDown == sender )
					{
						if( l_bAeverageAssign )
						{
							if( l_iNumPoint>1 )
								l_vFinalolor.z = l_vColor.z / (float)(l_iNumPoint-1);
							else
								l_vFinalolor.z = l_vColor.z / (float)(l_iNumPoint);
						}
					}
					if( m_pMPDIUI->AlphaColor_numericUpDown == sender )
					{
						if( l_bAeverageAssign )
						{
							if( l_iNumPoint>1 )
								l_vFinalolor.z = l_vColor.w / (float)(l_iNumPoint-1);
							else
								l_vFinalolor.z = l_vColor.w / (float)(l_iNumPoint);
						}
					}
					if( sender == m_pMPDIUI->AngleX_numericUpDown||sender == m_pMPDIUI->AngleY_numericUpDown||sender == m_pMPDIUI->AngleZ_numericUpDown )
					{
						if( l_bAeverageAssign )
						{
							l_pTexBehaviorDataWithImageIndexData->vAngle.x = l_fStartAngle+l_vAngle.x*i;
							l_pTexBehaviorDataWithImageIndexData->vAngle.y = l_fStartAngle+l_vAngle.y*i;
							l_pTexBehaviorDataWithImageIndexData->vAngle.z = l_fStartAngle+l_vAngle.z*i;
						}
						else
						{
							l_pTexBehaviorDataWithImageIndexData->vAngle.x = l_vAngle.x;
							l_pTexBehaviorDataWithImageIndexData->vAngle.y = l_vAngle.y;
							l_pTexBehaviorDataWithImageIndexData->vAngle.z = l_vAngle.z;
						}
					}
					Vector3	l_Pos = l_pCueToStartCurvesWithTime->GetOriginalPointList()[l_SelectedIndex];
					float	l_fTime = l_pCueToStartCurvesWithTime->GetOriginalTimeList()[l_SelectedIndex];
					if( sender == m_pMPDIUI->PathNodeTime_numericUpDown)
						l_fTime = (float)m_pMPDIUI->PathNodeTime_numericUpDown->Value/1000.f;
					if( sender == m_pMPDIUI->PathNodePosX_numericUpDown)
						l_Pos.x = (float)m_pMPDIUI->PathNodePosX_numericUpDown->Value;
					else
					if( sender == m_pMPDIUI->PathNodePosY_numericUpDown)
						l_Pos.y = (float)m_pMPDIUI->PathNodePosY_numericUpDown->Value;
					else
					if( sender == m_pMPDIUI->PathNodePosZ_numericUpDown)
						l_Pos.z = (float)m_pMPDIUI->PathNodePosZ_numericUpDown->Value;
					if( sender == m_pMPDIUI->Width_numericUpDown)
					{
						if( l_bAeverageAssign )
							l_pTexBehaviorDataWithImageIndexData->Size.x = l_vStartSize.x+l_fWantedSize*i;
						else
							l_pTexBehaviorDataWithImageIndexData->Size.x = l_fWantedSize;
							
					}
					if( sender == m_pMPDIUI->Height_numericUpDown)
					{
						if( l_bAeverageAssign )
							l_pTexBehaviorDataWithImageIndexData->Size.y = l_vStartSize.y+l_fWantedSize*i;
						else
							l_pTexBehaviorDataWithImageIndexData->Size.y = l_fWantedSize;
					}
					
					l_pCueToStartCurvesWithTime->FixPoint(l_SelectedIndex,l_Pos,l_fTime);
					l_pCueToStartCurvesWithTime->Refresh();
					if( m_pMPDIUI->RedColor_numericUpDown == sender )
					{
						if( l_bAeverageAssign )
							l_pTexBehaviorDataWithImageIndexData->vColor.x = (l_vFinalolor.z*i);
						else
							l_pTexBehaviorDataWithImageIndexData->vColor.x = l_vColor.x;
					}
					if( m_pMPDIUI->GreenColor_numericUpDown == sender )
					{
						if( l_bAeverageAssign )
							l_pTexBehaviorDataWithImageIndexData->vColor.y = (l_vFinalolor.z*i);
						else
							l_pTexBehaviorDataWithImageIndexData->vColor.y = l_vColor.y;
					}
					if( m_pMPDIUI->BlueColor_numericUpDown == sender )
					{
						if( l_bAeverageAssign )
							l_pTexBehaviorDataWithImageIndexData->vColor.y = (l_vFinalolor.z*i);
						else
							l_pTexBehaviorDataWithImageIndexData->vColor.y = l_vColor.z;
					}
					if( m_pMPDIUI->AlphaColor_numericUpDown == sender )
					{
						if( l_bAeverageAssign )
							l_pTexBehaviorDataWithImageIndexData->vColor.x = (l_vFinalolor.z*i);
						else
							l_pTexBehaviorDataWithImageIndexData->vColor.x = l_vColor.x;
					}
					if( sender == m_pMPDIUI->SizeScale_numericUpDown )
					{
						if( l_bAeverageAssign )
							l_pTexBehaviorDataWithImageIndexData->Size*=(i*l_fScaleValue);
						else
							l_pTexBehaviorDataWithImageIndexData->Size*=l_fScaleValue;
					}
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve->FixPoint(l_SelectedIndex,l_Pos,l_fTime);
				}
			}
		}
	}
	void	MPDIEditor::ImageApply()
	{
		 if( m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedIndex !=-1 )
		 {
			 if(m_pMPDIUI->WholePath_listBox->SelectedIndex!=-1&&m_pMPDIUI->PathNode_listBox->SelectedIndex!=-1)
			 {
				 sPuzzleData*l_pAllPuzzleData = this->m_pCurrentWorkingPI->GetAllPuzzleData();
				 sPuzzleData*l_pPuzzleData = &l_pAllPuzzleData[m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedIndex];
				 int	l_iStartIndex = m_pMPDIUI->PathNode_listBox->Items->Count;
				 int	l_iEndIndex = 0;
				 int	l_iStartImageIndex = 0;
				 int	l_iEndImageIndex = 0;
				 int	l_iCurrentImageIndex = 0;
				 //auto assign image
				 if( m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedIndices->Count>2 )
				 {
					WARNING_MSG("only could select 2 index");
					return;
				 }
				 for each( int l_iIndex in m_pMPDIUI->PathNode_listBox->SelectedIndices )
				 {
					if( l_iStartIndex > l_iIndex)
						l_iStartIndex = l_iIndex;
					if( l_iEndIndex < l_iIndex)
						l_iEndIndex = l_iIndex;
				 }
				 //assign by range
				 if( m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedIndices->Count == 2 )
				 {
					 l_iStartImageIndex = m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedIndices[0];
					 l_iEndImageIndex = m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedIndices[1];
					 if(l_iStartImageIndex>l_iEndImageIndex)
					 {
						l_iEndImageIndex = l_iStartImageIndex;
						l_iStartImageIndex = m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedIndices[1];
					 }
					 l_iCurrentImageIndex = l_iStartImageIndex;
					cCurveWithTime*l_cCurveWithTime = (*m_pMultiPathDynamicImageAWP)[m_pMultiPathDynamicImageAWP->GetCurrentSelectedIndex()];
					 std::vector<Vector3>	l_PointList = l_cCurveWithTime->GetOriginalPointList();
					 std::vector<float>		l_TimeList = l_cCurveWithTime->GetOriginalTimeList();
					 for( int l_iIndex = l_iStartIndex;l_iIndex<l_iEndIndex+1;++l_iIndex )
					 {
						Vector3	l_v2 =  l_PointList[l_iIndex];
						if( this->m_pCurrentWorkingPI->Count() <= l_iCurrentImageIndex )
							l_iCurrentImageIndex = l_iStartImageIndex;
						sTexBehaviorDataWithImageIndexData	*l_pData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[l_iIndex];
						l_pData->iImageIndex = l_iCurrentImageIndex;
						l_pData->pPI = m_pCurrentWorkingPI;
						l_pData->Size = Vector2((float)l_pAllPuzzleData[l_iCurrentImageIndex].Size.x,(float)l_pAllPuzzleData[l_iCurrentImageIndex].Size.y);
						m_pMPDIUI->PathNode_listBox->Items[l_iIndex] = ( l_iIndex.ToString()+":"+DNCT::WcharToGcstring(l_pAllPuzzleData[l_iCurrentImageIndex].strFileName )+":X="+((int)l_v2.x).ToString()+",y="+((int)l_v2.y).ToString()+",z="+((int)l_v2.z).ToString()+",Time="+l_TimeList[l_iIndex].ToString() );
						l_iCurrentImageIndex++;
						if( l_iCurrentImageIndex > l_iEndImageIndex )
							l_iCurrentImageIndex = l_iStartImageIndex;
					 }
				 }
				 else
				 if( m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedIndices->Count == 1 )
				 {					 
					l_iStartImageIndex = m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedIndex;
					l_iCurrentImageIndex = l_iStartImageIndex;
					cCurveWithTime*l_cCurveWithTime = (*m_pMultiPathDynamicImageAWP)[m_pMultiPathDynamicImageAWP->GetCurrentSelectedIndex()];
					std::vector<Vector3>	l_PointList = l_cCurveWithTime->GetOriginalPointList();
					std::vector<float>		l_TimeList = l_cCurveWithTime->GetOriginalTimeList();
					std::vector<int>		l_ScelectedIndices;
					for each( int l_iIndex in m_pMPDIUI->PathNode_listBox->SelectedIndices )
						l_ScelectedIndices.push_back(l_iIndex);
					for(size_t i=0;i<l_ScelectedIndices.size();++i)
					{
						int	l_iIndex = l_ScelectedIndices[i];
						Vector3	l_v2 =  l_PointList[l_iIndex];
						sTexBehaviorDataWithImageIndexData	*l_pData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[l_iIndex];
						l_pData->pPI = m_pCurrentWorkingPI;
						l_pData->iImageIndex = l_iCurrentImageIndex;
						l_pData->Size = Vector2((float)l_pAllPuzzleData[l_iCurrentImageIndex].Size.x,(float)l_pAllPuzzleData[l_iCurrentImageIndex].Size.y);
						m_pMPDIUI->PathNode_listBox->Items[l_iIndex] = ( l_iIndex.ToString()+":"+DNCT::WcharToGcstring(l_pAllPuzzleData[l_iCurrentImageIndex].strFileName )+":X="+((int)l_v2.x).ToString()+",y="+((int)l_v2.y).ToString()+",z="+((int)l_v2.z).ToString()+",Time="+l_TimeList[l_iIndex].ToString() );
					}
				 }
				 int	l_iLastIndex = m_pMPDIUI->PathNode_listBox->SelectedIndex;
				 m_pMPDIUI->PathNode_listBox->SelectedIndex = -1;
				 m_pMPDIUI->PathNode_listBox->SelectedIndex = l_iLastIndex;
			 }
		 }
	}

	void	MPDIEditor::SaveAndOpen_MPDI(System::Object^  sender, System::EventArgs^  e)
	{
		if( saveToolStripMenuItem == sender )
		{
			if( !m_pImagePlayManagerAWP )
				return;
			if( m_pMPDIList->Count() == 0 )
			{
				WARNING_MSG("please save to animation list please!");
				return;
			}
			String^l_strFileName = DNCT::SaveFileAndGetName(DNCT::ConvertExtensionToFilter(m_pImagePlayManagerAWP->ExtensionName()).c_str());
			Save(l_strFileName);
		}
		else
		{
			String^l_strFatFileName = DNCT::OpenFileAndGetName("mpdi files (*.mpdi;*.mpdib)|*.mpdi;*.mpdib|All files (*.*)|*.*");
			//String^l_strFatFileName = DNCT::OpenFileAndGetName(DNCT::ConvertExtensionToFilter(cAnimationParser::ExtensionNameID));
			if( !l_strFatFileName )
			{
				return;
			}
			OpenMPDIFile(l_strFatFileName);
		}
	}

	System::Void MPDIEditor::MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if(!timer1->Enabled)
			return;
		GCFORM::MouseButtons l_MouseButton = e->Button;
		m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
			,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)splitContainer3->Panel1->Size.Width,(float)splitContainer3->Panel1->Size.Height));
		if( !addPointModeToolStripMenuItem->Checked )//1 for fix point, 0 for add point
		{
			Vector3	l_vMouseWorldPos((float)(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(float)(int)m_pOrthogonalCamera->GetMouseWorldPos().y,0.f);
			m_pMultiPathDynamicImageAWP->MouseMove(l_vMouseWorldPos);
			if( m_pMultiPathDynamicImageAWP->m_iFocusPoint != -1&&
				m_pMultiPathDynamicImageAWP->GetPathMode() == ePM_FIX_WITH_MOUSE_DRAG)
			{
				Vector2	l_vPos = Vector2(l_vMouseWorldPos.x,l_vMouseWorldPos.y);
				m_pMultiPathDynamicImageAWP->m_pCurrentCurve->FixPoint(m_pMultiPathDynamicImageAWP->m_iFocusPoint,l_vPos);
			}
		}
	}

	System::Void MPDIEditor::MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		splitContainer3->Panel1->Focus();
		if(!timer1->Enabled||!m_pMultiPathDynamicImageAWP)
			return;	
		if( e->Button != System::Windows::Forms::MouseButtons::Left )
			return;
		GCFORM::MouseButtons l_MouseButton = e->Button;
		m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
			,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)splitContainer3->Panel1->Size.Width,(float)splitContainer3->Panel1->Size.Height));
		Vector3	l_vMouseWorldPos((float)(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(float)(int)m_pOrthogonalCamera->GetMouseWorldPos().y,0.f);
		if( addPointModeToolStripMenuItem->Checked )
		{
			if(m_pMPDIUI->WholePath_listBox->SelectedIndex!=-1)
			{
    			cPuzzleImage*l_pPI = this->m_pCurrentWorkingPI;
				int	l_iSelectedIndex = 0;
				if( m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedIndex != -1 )
					l_iSelectedIndex = l_pPI->GetObjectIndexByName(DNCT::GcStringToWchar(m_pMPDIUI->WholePuzzleImageUnit_listBox->Items[m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedIndex]->ToString()).c_str());
				POINT	l_Size = l_pPI->GetPuzzleData(l_iSelectedIndex)->Size;
				//POINT	l_Size = l_pPI->GetPuzzleData()[l_iSelectedIndex]->OriginalSize;
				size_t	l_uiNumPoint = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList()->size();
				if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList()->size() == 0 )
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve->AddPoint(Vector2(l_vMouseWorldPos.x,l_vMouseWorldPos.y),0.000f,Vector2((float)l_Size.x,(float)l_Size.y),Vector3::Zero,Vector4::One,l_iSelectedIndex,m_pMPDIUI->Mirror_checkBox->Checked,l_pPI,false);
				else
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve->AddPoint(Vector2(l_vMouseWorldPos.x,l_vMouseWorldPos.y),m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetTimeList()[l_uiNumPoint -1]+0.1f,Vector2((float)l_Size.x,(float)l_Size.y),Vector3::Zero,Vector4::One,l_iSelectedIndex,m_pMPDIUI->Mirror_checkBox->Checked,l_pPI);
				System::EventArgs^  e3;
				WholePath_listBox_SelectedIndexChanged(m_pMPDIUI->WholePath_listBox,e3);
			}
		}
		else
		{
			if( moveWholePathToolStripMenuItem->Checked || rotatePathToolStripMenuItem->Checked )
			{
				m_pMultiPathDynamicImageAWP->m_SelectedNodes.clear();
				for each(int l_iIndex in m_pMPDIUI->PathNode_listBox->SelectedIndices)
					m_pMultiPathDynamicImageAWP->m_SelectedNodes.push_back(l_iIndex);
			}
			else
			if( sender == moveAllPathToolStripMenuItem )
			{
			
			}
			m_pMultiPathDynamicImageAWP->MouseDown(l_vMouseWorldPos);
		}
	}

	System::Void MPDIEditor::MyMouseHover(System::Object^  sender, System::EventArgs^  e)
	{
		 if(!timer1->Enabled||!m_pMultiPathDynamicImageAWP)
			return;
	}

	System::Void MPDIEditor::MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	 {
		 if(!this->Visible)
			return;
		GCFORM::MouseButtons l_MouseButton = e->Button;
		m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
			,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)splitContainer3->Panel1->Size.Width,(float)splitContainer3->Panel1->Size.Height));

		Vector3	l_vMouseWorldPos((float)(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(float)(int)m_pOrthogonalCamera->GetMouseWorldPos().y,0.f);
		if( m_pMultiPathDynamicImageAWP )
		{
			m_pMultiPathDynamicImageAWP->MouseUp(l_vMouseWorldPos);
			if( l_MouseButton==System::Windows::Forms::MouseButtons::Left && m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
				m_bDataChanged = true;
			if( moveWholePathToolStripMenuItem->Checked || rotatePathToolStripMenuItem->Checked )
			{
				if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
				{
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve->SetOriginalPointList(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalPointList());
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve->DoLOD();

				}
			}
			else
			if( moveAllPathToolStripMenuItem->Checked || rotateAllPathToolStripMenuItem->Checked  )
			{
				if( m_pMultiPathDynamicImageAWP )
				{
					int	l_i = m_pMultiPathDynamicImageAWP->Count();
					for( int i=0;i<l_i;++i )
					{
						(*m_pMultiPathDynamicImageAWP)[i]->SetOriginalPointList((*m_pMultiPathDynamicImageAWP)[i]->GetOriginalPointList());
						(*m_pMultiPathDynamicImageAWP)[i]->DoLOD();
					}
				}
			}
		}
	 }

	void	MPDIEditor::PathMoveModifyMode(System::Object^  sender, System::EventArgs^  e)
	{
		if( !m_pMultiPathDynamicImageAWP )
			return;
		 fixPointModeToolStripMenuItem->Checked= false;
		 addPointModeToolStripMenuItem->Checked = false;
		 moveWholePathToolStripMenuItem->Checked = false;
		 moveAllPathToolStripMenuItem->Checked = false;
		 rotatePathToolStripMenuItem->Checked = false;
		 rotateAllPathToolStripMenuItem->Checked = false;
		 System::Windows::Forms::ToolStripMenuItem^l_pSender = (System::Windows::Forms::ToolStripMenuItem^)sender;
		 l_pSender->Checked = true;
		 if( sender == fixPointModeToolStripMenuItem )
		 {
			m_pMultiPathDynamicImageAWP->SetPathMode(ePM_FIX);
		 }
		 else
		 if( sender == moveWholePathToolStripMenuItem )
		 {
			m_pMultiPathDynamicImageAWP->SetPathMode(ePM_PATH_MOVING);
		 }
		 else
		 if( sender == moveAllPathToolStripMenuItem )
		 {
			m_pMultiPathDynamicImageAWP->SetPathMode(ePM_PATH_MOVING_ALL);
			m_pMultiPathDynamicImageAWP->m_SelectedIndicesToMovingOrRotating.clear();
			for each (int l_iSelectedIndex in m_pMPDIUI->WholePath_listBox->SelectedIndices)//haha......fucking sux here
			{
				m_pMultiPathDynamicImageAWP->m_SelectedIndicesToMovingOrRotating.push_back(l_iSelectedIndex);
			}
		 }
		 else
		 if( sender == rotatePathToolStripMenuItem )
		 {
			m_pMultiPathDynamicImageAWP->SetPathMode(ePM_PATH_ROTATION);
		 }
		 else
		 if( sender == rotateAllPathToolStripMenuItem )
		 {
			m_pMultiPathDynamicImageAWP->SetPathMode(ePM_ALL_PATH_ROTATION);
			m_pMultiPathDynamicImageAWP->m_SelectedIndicesToMovingOrRotating.clear();
			for each (int l_iSelectedIndex in m_pMPDIUI->WholePath_listBox->SelectedIndices)//haha......fucking sux here
			{
				m_pMultiPathDynamicImageAWP->m_SelectedIndicesToMovingOrRotating.push_back(l_iSelectedIndex);
			}
		 }
		 else
		 {
			m_pMultiPathDynamicImageAWP->SetPathMode(ePM_EDIT);
		 }
	}

	void	MPDIEditor::PathModifyFunction(System::Object^  sender, System::EventArgs^  e)
	{
		 if( sender == AddAnimationToList_button )
		 {
			 if( !AnimationName_textBox->Text->Length )
				 return;
			AddIntoList();
		 }
		 else
		 if( WholeAnimationList_listBox->SelectedIndex != -1 )
		 {
			 if( sender == DeleteListObject_button )
			 {
				 String^l_strName = DNCT::WcharToGcstring(m_pMPDIList->GetObject(WholeAnimationList_listBox->SelectedIndex)->GetName());
				 m_pMPDIList->RemoveObject(WholeAnimationList_listBox->SelectedIndex);
				 WholeAnimationList_listBox->Items->RemoveAt(WholeAnimationList_listBox->SelectedIndex);
				 if(this->m_pMPDIExtraData->ContainsKey(l_strName))
				 {
					 m_pMPDIExtraData->Remove(l_strName);
				 }
				 MPDIExtra_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();
				 Save(m_strCurrentSaveFileName);
			 }
			 else
			 if( sender == AnimationInvert_button )
			 {
				 ePathMode	l_ePathMode = m_pMultiPathDynamicImageAWP->GetPathMode();
				 if( WholeAnimationList_listBox->SelectedIndex != -1)
				 {
					 m_bPreventChangeAllTimeValueSetup = true;
					 SAFE_DELETE(m_pMultiPathDynamicImageAWP);
					 cMPDI*l_pMPDI = m_pMPDIList->GetObject(WholeAnimationList_listBox->SelectedIndex);
					 if( l_pMPDI->Count()>0 )
					 {
						 m_pMPDIUI->TotalPlayTime_numericUpDown->Value = (int)((l_pMPDI->GetObject(0)->GetEndTime()-l_pMPDI->GetObject(0)->GetStartTime())*1000);
					 }
					 m_pMultiPathDynamicImageAWP = new cMPDIForEditor(m_pMPDIList->GetObject(WholeAnimationList_listBox->SelectedIndex));
					 bool l_bStayAtLastFrame = m_pMultiPathDynamicImageAWP->IsStayAtLastFrame();
					 StayAtLastFrame_checkBox->Checked = l_bStayAtLastFrame;
					 Vector3 scale; Vector3 rotation; Vector3 translation; float inverted;
					 CameraRotationX_numericUpDown->Value = (int)0;
					 CameraRotationY_numericUpDown->Value = (int)0;
					 CameraRotationZ_numericUpDown->Value = (int)0;
					 CameraRotationCenterX_numericUpDown->Value = (int)0;
					 CameraRotationCenterY_numericUpDown->Value = (int)0;
					 CameraRotationCenterZ_numericUpDown->Value = (int)0;

					 m_pMultiPathDynamicImageAWP->GetLocalTransform().Decompose(scale,rotation,translation,inverted);
					 rotation = Vector3(D3DXToDegree(rotation.x),D3DXToDegree(rotation.y),D3DXToDegree(rotation.z));
					 if( rotation.x-(int)rotation.x>0.f ) rotation.x = (int)rotation.x+1.f;
					 if( rotation.y-(int)rotation.y>0.f ) rotation.y = (int)rotation.y+1.f;
					 if( rotation.z-(int)rotation.z>0.f ) rotation.z = (int)rotation.z+1.f;
					 CameraRotationX_numericUpDown->Value = (int)rotation.x;
					 CameraRotationY_numericUpDown->Value = (int)rotation.y;
					 CameraRotationZ_numericUpDown->Value = (int)rotation.z;
					 CameraRotationCenterX_numericUpDown->Value = (int)translation.x;
					 CameraRotationCenterY_numericUpDown->Value = (int)translation.y;
					 CameraRotationCenterZ_numericUpDown->Value = (int)translation.z;

					 //if( m_pMultiPathDynamicImageAWP->GetCameraView() )
					 //{
						// m_pMultiPathDynamicImageAWP->GetCameraView()->Decompose(scale,rotation,translation,inverted);
						// rotation = Vector3(D3DXToDegree(rotation.x),D3DXToDegree(rotation.y),D3DXToDegree(rotation.z));
						// if( rotation.x-(int)rotation.x>0.f ) rotation.x = (int)rotation.x+1.f;
						// if( rotation.y-(int)rotation.y>0.f ) rotation.y = (int)rotation.y+1.f;
						// if( rotation.z-(int)rotation.z>0.f ) rotation.z = (int)rotation.z+1.f;
						// CameraRotationX_numericUpDown->Value = (int)rotation.x;
						// CameraRotationY_numericUpDown->Value = (int)rotation.y;
						// CameraRotationZ_numericUpDown->Value = (int)rotation.z;
						// CameraRotationCenterX_numericUpDown->Value = (int)translation.x;
						// CameraRotationCenterY_numericUpDown->Value = (int)translation.y;
						// CameraRotationCenterZ_numericUpDown->Value = (int)translation.z;
					 //}

					 m_pMPDIUI->WholePath_listBox->Items->Clear();
					 for(int i=0;i<m_pMultiPathDynamicImageAWP->Count();++i)
					 {
						cCueToStartCurveWithTime*l_pCueToStartCurvesWithTime = (*m_pMultiPathDynamicImageAWP)[i];
						m_pMPDIUI->WholePath_listBox->Items->Add(DNCT::WcharToGcstring(l_pCueToStartCurvesWithTime->GetName()));
						if( i == 0 )
							m_pMPDIUI->Loop_checkBox->Checked = l_pCueToStartCurvesWithTime->IsCurveLoop();
					 }
					 if( m_pMPDIUI->WholePath_listBox->Items->Count )
						m_pMPDIUI->WholePath_listBox->SelectedIndex = 0;
					 bool	l_b = m_pMultiPathDynamicImageAWP->IsAnimationLoop();
					 AllCurveLoop_checkBox->Checked = m_pMultiPathDynamicImageAWP->IsAnimationLoop();
					 AllPlay_checkBox->Checked = true;
					 if( m_pMultiPathDynamicImageAWP->Count() )
						 this->m_pMPDIUI->Loop_checkBox->Checked = m_pMultiPathDynamicImageAWP->GetObject(m_pMPDIUI->WholePath_listBox->SelectedIndex)->IsAnimationLoop();
					 Play_checkBox_CheckedChanged(AllPlay_checkBox,e);

					 MPDIExtra_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();
					 String^l_strSelectedMPDIName = DNCT::WcharToGcstring(m_pMultiPathDynamicImageAWP->GetName());
					 if(m_pMPDIExtraData->ContainsKey(l_strSelectedMPDIName))
					 {
						GCFORM::TreeNode^l_pExtractDataNode = (GCFORM::TreeNode^)m_pMPDIExtraData[l_strSelectedMPDIName];
						MPDIExtra_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Add(l_pExtractDataNode);
					 }					 
					switch(l_ePathMode)
					{
						case ePM_EDIT:
							PathMoveModifyMode(addPointModeToolStripMenuItem,e);
							break;
						case ePM_FIX:
							PathMoveModifyMode(fixPointModeToolStripMenuItem,e);
							break;
						case ePM_PATH_MOVING:
							PathMoveModifyMode(moveWholePathToolStripMenuItem,e);
							break;
						case ePM_PATH_MOVING_ALL:
							PathMoveModifyMode(moveAllPathToolStripMenuItem,e);
							break;
						case ePM_PATH_ROTATION:
							PathMoveModifyMode(rotatePathToolStripMenuItem,e);
							break;
						case ePM_ALL_PATH_ROTATION:
							PathMoveModifyMode(rotateAllPathToolStripMenuItem,e);
							break;
					}
					 m_bPreventChangeAllTimeValueSetup = false;
					 WholePuzzleImageUnit_listBox_SelectedIndexChanged(sender,e);
				 }
			 }
		 }	
	}

	 void	MPDIEditor::ReferencePathModify(System::Object^  sender, System::EventArgs^  e)
	 {
		 if( !m_pReferenceCurveManagerAWP )
		 {
			m_pReferenceCurveManagerAWP = new cCurveManager;
		 }
		 if( !m_pMultiPathDynamicImageAWP&&!m_pMPDIList )
			 return;
		 if( sender == UsingIntoCurrentPath_button )
		 {
			 if( AllPath_listBox->SelectedIndex != -1 )
			 {
				AddPath(m_pMPDIUI->AddPath_button,e);
				cCurveWithTime*l_pCurvesWithTime = m_pReferenceCurveManagerAWP->GetObject(AllPath_listBox->SelectedIndex);
				if(m_pMPDIUI->WholePath_listBox->SelectedIndex!=-1)
				{
					cCurveWithTime*l_pDestCurvesWithTime =	m_pMultiPathDynamicImageAWP->GetObject(m_pMultiPathDynamicImageAWP->Count()-1);
					l_pDestCurvesWithTime->SetOriginalPointList(l_pCurvesWithTime->GetOriginalPointList());
					l_pDestCurvesWithTime->SetOriginalTimeList(l_pCurvesWithTime->GetOriginalTimeList());
					l_pDestCurvesWithTime->SetPointList(l_pCurvesWithTime->GetPointList());
					l_pDestCurvesWithTime->SetTimeList(l_pCurvesWithTime->GetTimeList());
					l_pDestCurvesWithTime->SetLOD(l_pCurvesWithTime->GetLOD());
					l_pDestCurvesWithTime->SetCurveLoop(l_pCurvesWithTime->IsCurveLoop());

					POINT	l_Size = m_pMultiPathDynamicImageAWP->GetAllPuzzleData()[0].Size;
					size_t	l_iuNumPoint = l_pDestCurvesWithTime->GetOriginalPointList().size();
					cPuzzleImage*l_pPI = m_pInterlUseForShowImagePos->GetPuzzleImage(DNCT::GcStringToWchar(m_pMPDIUI->AllPI_listBox->SelectedItem->ToString()).c_str());
					for(size_t i=0;i<l_iuNumPoint;++i)
						m_pMultiPathDynamicImageAWP->m_pCurrentCurve->AddPoint(l_pDestCurvesWithTime->GetOriginalPointList()[i],l_pCurvesWithTime->GetOriginalTimeList()[i],
						Vector2((float)l_Size.x,(float)l_Size.y),Vector3::Zero,Vector4::One,0,m_pMPDIUI->Mirror_checkBox->Checked,l_pPI);
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve->DoLOD();
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve->SetAnimationLoop(l_pCurvesWithTime->IsCurveLoop());
					WholePath_listBox_SelectedIndexChanged(m_pMPDIUI->WholePath_listBox,e);
				}
			 }
		 }
		 else
		 if( sender == AddPathIntoAllPath_button )
		 {
			 if( m_pMPDIUI->WholePath_listBox->SelectedIndex !=-1 )
			 {
				DNCT::InputNameDialog^l_pForm = gcnew DNCT::InputNameDialog;
				l_pForm->SetupTextBoxData(m_pMPDIUI->WholePath_listBox->SelectedItem->ToString());
				l_pForm->ShowDialog();
				if( l_pForm->m_strTextName && l_pForm->m_strTextName->Length )
				{
					String^l_strName = l_pForm->m_strTextName;
					if( AllPath_listBox->Items->Contains(l_strName) )
					{
						WARING_YES_NO_TO_NO("replace original?覆蓋原本物件?")
						{
							return;
						}
					}
					else
						AllPath_listBox->Items->Add(l_strName);
					cCurveWithTime*l_pNewCurvesWithTime = new cCurveWithTime();
					l_pNewCurvesWithTime->SetName(DNCT::GcStringToWchar(l_pForm->m_strTextName));
					cCurveWithTime*l_pSrcCurveWithTime =	m_pMultiPathDynamicImageAWP->GetObject(m_pMPDIUI->WholePath_listBox->SelectedIndex);
					l_pNewCurvesWithTime->SetOriginalPointList(l_pSrcCurveWithTime->GetOriginalPointList());
					l_pNewCurvesWithTime->SetOriginalTimeList(l_pSrcCurveWithTime->GetOriginalTimeList());
					l_pNewCurvesWithTime->SetPointList(l_pSrcCurveWithTime->GetPointList());
					l_pNewCurvesWithTime->SetTimeList(l_pSrcCurveWithTime->GetTimeList());
					l_pNewCurvesWithTime->SetLOD(l_pSrcCurveWithTime->GetLOD());
					l_pNewCurvesWithTime->SetCurveLoop(l_pSrcCurveWithTime->IsCurveLoop());
					m_pReferenceCurveManagerAWP->AddObject(l_pNewCurvesWithTime);	
				}
				delete l_pForm;
			 }
		 }
		 else
		 if( sender == DelPathIntoAllPath_button )
		 {
			 if( AllPath_listBox->SelectedIndex == -1)
				 return;
			 m_pReferenceCurveManagerAWP->RemoveObject(AllPath_listBox->SelectedIndex);
			 AllPath_listBox->Items->RemoveAt(AllPath_listBox->SelectedIndex);
		 }
	 }

	System::Void MPDIEditor::EventPoint(System::Object^  sender, System::EventArgs^  e)
	{
		 std::vector<sHintPoint>*l_pVectorPointer = 0;
		 if( m_pMPDIUI->WholePath_listBox->SelectedIndex!=-1 )
		 {
			cCueToStartCurveWithTime*l_pCueToStartCurvesWithTime = m_pMultiPathDynamicImageAWP->GetObject(m_pMPDIUI->WholePath_listBox->SelectedIndex);
			l_pVectorPointer = l_pCueToStartCurvesWithTime->GetHintPointVector();
		 }
		 if( sender == AddEventPOint_button )
		 {
			 if( m_pMPDIUI->PathNode_listBox->SelectedIndex != -1 )
			 {
				 eAnimationEvent	l_eIndex = IntToAnimationEvent(HintPointType_comboBox->SelectedIndex);
				 if( l_eIndex == eAE_MAX )
				 {
					 WARNING_MSG("please select type!!");
					 return;
				 }
				 std::wstring l_strText = L"__empty";
				 if( l_eIndex != eAE_NONE )
				 {
					if(EventPointName_textBox->Text->Length == 0)
					{
						WARNING_MSG("please input the name for hint point!!");
						return;
					}
					l_strText = DNCT::GcStringToWchar(EventPointName_textBox->Text);
				 }
				 sHintPoint l_HintPoint(m_pMPDIUI->PathNode_listBox->SelectedIndex,l_strText.c_str(),l_eIndex);
				 l_pVectorPointer->push_back(l_HintPoint);
				 AllEventPointHint_listBox->Items->Add(m_pMPDIUI->PathNode_listBox->SelectedIndex.ToString()+":"+HintPointType_comboBox->SelectedItem->ToString()+","+EventPointName_textBox->Text);
			 }
		 }
		 else
		 if( sender == DelEventPoint_button )
		 {
			 if( l_pVectorPointer&&AllEventPointHint_listBox->SelectedIndex != -1  )
			 {
				 l_pVectorPointer->erase(l_pVectorPointer->begin()+AllEventPointHint_listBox->SelectedIndex);
				 AllEventPointHint_listBox->Items->RemoveAt(AllEventPointHint_listBox->SelectedIndex);
			 }
		 }
	}
	System::Void MPDIEditor::AddGameUI(System::Object^  sender)
	{
		#define	TABPAGE_CONTROLLER_DATA_MPDI_BACKGROUND			L"BG"				//
		#define	TABPAGE_CONTROLLER_DATA_MPDI_SLIDER_OBJECT		L"SliderObject"		//each object's position
		#define	TABPAGE_CONTROLLER_DATA_MPDI_MULTIPAGE_OBJECT	L"MultiPageObject"	//each object's position
		#define	TABPAGE_CONTROLLER_DATA_MPDI_CANCEL_BUTTON		L"CB"				//
		#define	TABPAGE_CONTROLLER_DATA_MPDI_PRIOR_PAGE			L"PriorPage"		//proio page button position
		#define	TABPAGE_CONTROLLER_DATA_MPDI_NEXT_PAGE			L"NextPage"			//next page button position
		#define	TABPAGE_CONTROLLER_DATA_MPDI_ICON_OBJECT_POS	L"IconObjectPos"	//icon object's position
		if( sender == m_pMPDIUI->AddSliderBarUI_toolStripMenuItem )
		{
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_CANCEL_BUTTON).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_BACKGROUND).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_SLIDER_OBJECT).ToString());
			WARNING_MSG("BG for background,CB for cancel button,SliderObject for SliderPosition and orientation");
		}
		else
		if( sender == m_pMPDIUI->AddMultiPageUI_toolStripMenuItem )
		{
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_CANCEL_BUTTON).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_BACKGROUND).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_SLIDER_OBJECT).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_PRIOR_PAGE).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_NEXT_PAGE).ToString());
			WARNING_MSG("BG for background,CB for cancel button,SliderObject for SliderPosition and orientation\nPriorPage for prior page button NextPage for next page button\nIconObjectPos for tabPages's category icon");
		}
		else
		if( sender == m_pMPDIUI->addMultipageWithTabPagesUIToolStripMenuItem )
		{
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_CANCEL_BUTTON).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_BACKGROUND).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_SLIDER_OBJECT).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_PRIOR_PAGE).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_NEXT_PAGE).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_ICON_OBJECT_POS).ToString());
			WARNING_MSG("BG for background,CB for cancel button,SliderObject for SliderPosition and orientation\nPriorPage for prior page button NextPage for next page button");
		}
		else
		if( sender == m_pMPDIUI->addSliderBarWithTabPagesUIToolStripMenuItem )
		{
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_CANCEL_BUTTON).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_BACKGROUND).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_SLIDER_OBJECT).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_ICON_OBJECT_POS).ToString());
			WARNING_MSG("BG for background,CB for cancel button,SliderObject for SliderPosition and orientation\nIconObjectPos for tabPages's category icon");
		}
		else
		if( sender == m_pMPDIUI->yesNoDialogToolStripMenuItem )
		{
			#define		YES_NO_DIALOG_YES_BUTTON	"Yes"
			#define		YES_NO_DIALOG_NO_BUTTON		"No"
			#define		YES_NO_DIALOG_BG			"BG"
			#define		YES_NO_DIALOG_FONT			"Font"
			AddPath(String(YES_NO_DIALOG_YES_BUTTON).ToString());
			AddPath(String(YES_NO_DIALOG_NO_BUTTON).ToString());
			AddPath(String(YES_NO_DIALOG_BG).ToString());
			AddPath(String(YES_NO_DIALOG_FONT).ToString());
			WARNING_MSG("BG for background,Yes and No for button,font for description show position");
		}
	}

	void	MPDIEditor::AddPIFile(String^e_strFileName)
	{
		std::wstring l_strName = DNCT::GcStringToWchar(System::IO::Path::GetFileNameWithoutExtension(e_strFileName));
		if(m_pImageResourceManagerAWP->GetObject(l_strName))
		{
			WARNING_MSG("this pi's name has already added!");
			return;
		}
		cPuzzleImage*l_pPuzzleImageAWP = m_pImageResourceManagerAWP->GetPuzzleImageByFileName(DNCT::GcStringToChar(e_strFileName).c_str());
		if( l_pPuzzleImageAWP)
		{
			m_pMPDIList->SetPuzzleImage(l_pPuzzleImageAWP);
			m_pMPDIUI->AllPI_listBox->Items->Add(DNCT::WcharToGcstring(l_pPuzzleImageAWP->GetNameWithoutFullPath()));
		}
		else
		{
		
		}		   
	}

	void	MPDIEditor::CurrentProgressValueChange(int e_iSelection)
	{
		//MPDI
		//if( e_iSelection == 1 )
		{
			int	l_iMaxTime = 0;
			if( MPDIWorking_checkBox->Checked )
			{
				l_iMaxTime = (int)(m_pMultiPathDynamicImageAWP->GetEndTime()*1000);
			}
			if(CameraWorking_checkBox->Checked && m_pMPDICamera)
			{
				int	l_iTime = (int)(m_pMPDICamera->GetTotalPlayTime()*1000);
				if( l_iMaxTime < l_iTime )
					l_iMaxTime = l_iTime;
			}
			CurrentProgress_trackBar->Maximum = l_iMaxTime;
		}
		//else
		//if( e_iSelection == 2 )
		{//subMPDI
		
		}

	}

	void	MPDIEditor::OpenPIFile(String^e_strFileName)
	{
		WholeAnimationList_listBox->Items->Clear();
		SAFE_DELETE(m_pImagePlayManagerAWP);
		SAFE_DELETE(m_pInterlUseForShowImagePos);
		ShowImageOnScreen_listBox->Items->Clear();
		m_pMPDIUI->AllPI_listBox->Items->Clear();
		m_pMPDIUI->WholePath_listBox->Items->Clear();
		m_pMPDIUI->PathNode_listBox->Items->Clear();
		m_pMPDIUI->WholePuzzleImageUnit_listBox->Items->Clear();
		m_pMPDIUI->AddPath_button->Enabled = false;
		m_pImagePlayManagerAWP = new cAnimationParser();
		m_pImageResourceManagerAWP = m_pImagePlayManagerAWP->GetAllBaseImageList();
		cPuzzleImage*l_pPuzzleImageAWP = m_pImageResourceManagerAWP->GetPuzzleImageByFileName(DNCT::GcStringToChar(e_strFileName).c_str());
		if(l_pPuzzleImageAWP)
		{
			m_pMPDIList = new cMPDIList();
			m_pInterlUseForShowImagePos = new cCueToStartCurveWithTime(m_pMPDIList);
			m_pImagePlayManagerAWP->AddObject(m_pMPDIList);
			l_pPuzzleImageAWP->GenerateAllPuzzleImageUnit();

			m_pMPDIList->SetPuzzleImage(l_pPuzzleImageAWP);

			m_pMultiPathDynamicImageAWP = new cMPDIForEditor(m_pMPDIList);

			for( int i=0;i<l_pPuzzleImageAWP->Count();++i )
			{
				m_pMPDIUI->WholePuzzleImageUnit_listBox->Items->Add(DNCT::WcharToGcstring(l_pPuzzleImageAWP->GetObject(i)->GetName()));
			}
			m_pMPDIUI->AddPath_button->Enabled = true;
			m_pMPDIUI->Play_checkBox->Enabled = true;
			AllPlay_checkBox->Enabled = true;
			m_pMPDIUI->Loop_checkBox->Enabled = true;
			AllCurveLoop_checkBox->Enabled = true;
			m_pMPDIUI->AllPI_listBox->Items->Add(DNCT::WcharToGcstring(l_pPuzzleImageAWP->GetNameWithoutFullPath()));
			if( m_strCurrentSaveFileName )
				delete m_strCurrentSaveFileName;
			m_strCurrentSaveFileName = nullptr;
		}
		m_pMPDIUI->AssignLatestPI();
	}

	void	MPDIEditor::Save(String^e_strFileName)
	{
		if( !m_pImagePlayManagerAWP )
			return;
		if( m_pMPDIList->Count() == 0 )
		{
			WARNING_MSG("please save to animation list please!");
			return;
		}
		 String^l_strFileName = e_strFileName;
		 l_strFileName = ForceAddExtenName(l_strFileName,(char*)m_pImagePlayManagerAWP->ExtensionName());
		 if( l_strFileName )
		 {
			 m_strCurrentSaveFileName = l_strFileName;
			 m_pMPDIList->SetName(DNCT::GcStringToWchar(DNCT::GetFileNameWithoutFullPath(l_strFileName,true)));
			 m_pImagePlayManagerAWP->Export(DNCT::GcStringToChar(l_strFileName).c_str());
			 if(this->m_pMPDIExtraData->Count)
			 {
				 GCFORM::TreeNode^l_pNodes = gcnew GCFORM::TreeNode(DNCT::WcharToGcstring(m_pMPDIList->GetName()));
				 for each ( GCFORM::TreeNode^l_pTargetNode in m_pMPDIExtraData->Values )
				 {
					 l_pNodes->Nodes->Add((GCFORM::TreeNode^)l_pTargetNode->Clone());
				 }
				 DNCT::SaveTreeNodeToFile(l_pNodes,l_strFileName+"_extra");
			 }
			 Text = l_strFileName+"		SaveTime:"+System::DateTime::Now.ToString();
			 m_bDataChanged = false;
//				 FileToUnicode(l_strFileName);	
		 }
	}

	void	MPDIEditor::Merge(String^e_strFileName)
    {
		 if( !e_strFileName )
			 return;
		 int	l_iCount = m_pImagePlayManagerAWP->Count();
		 if( !m_pImagePlayManagerAWP || m_pImagePlayManagerAWP->Count() == 0 )
			 return;
		 std::wstring l_strMPDIListName = DNCT::GcStringToWchar(e_strFileName);//cMPDIList::FileToMPDIListName(DNCT::GcStringToWchar(e_strFileName).c_str());
		 if(!m_pImagePlayManagerAWP->GetObject(l_strMPDIListName.c_str()))
		 {
			 WholeAnimationList_listBox->Items->Clear();
			 m_pMPDIUI->AllPI_listBox->Items->Clear();
			 std::string	l_str = DNCT::GcStringToChar(e_strFileName);
			 if(m_pImagePlayManagerAWP->Parse(l_str.c_str()))
			 {
				 cMPDIList*l_pOriginaleMPDIList = m_pMPDIList;
				 cMPDIList*l_pMPDIList = m_pImagePlayManagerAWP->GetMPDIList(l_strMPDIListName.c_str());
				 l_pOriginaleMPDIList->Merge(l_pMPDIList);
				 l_iCount = m_pImagePlayManagerAWP->Count();

				 for( int i=0;i<m_pMPDIList->Count();++i )
				 {
					cMultiPathDynamicImage*l_pMultiPathDynamicImage = (*m_pMPDIList)[i];
					WholeAnimationList_listBox->Items->Add(DNCT::WcharToGcstring(l_pMultiPathDynamicImage->GetName()));
				 }
				 for (int i = 0; i < m_pMPDIList->GetPIList()->Count(); ++i)
				 {
					 m_pMPDIUI->AllPI_listBox->Items->Add(DNCT::WcharToGcstring((*m_pMPDIList->GetPIList())[i]->GetNameWithoutFullPath()));
				 }
				 m_pImagePlayManagerAWP->RemoveObject(l_pMPDIList->GetName());
				 int	l_iCount = m_pImagePlayManagerAWP->Count();
			 }
		 }
		 else
		 {
			WARNING_MSG("this mpdi already exists");
		 }
    }

	void	MPDIEditor::OpenMPDIFile(String^e_strFileName)
	{
		m_pMPDIList = 0;
		SAFE_DELETE(m_pInterlUseForShowImagePos);
		SAFE_DELETE(m_pImagePlayManagerAWP);
		SAFE_DELETE(m_pMultiPathDynamicImageAWP);
		this->m_pMPDIExtraData->Clear();
		m_pImagePlayManagerAWP = new cAnimationParser();
		m_pMPDIUI->WholePuzzleImageUnit_listBox->Items->Clear();
		WholeAnimationList_listBox->Items->Clear();
		m_pMPDIUI->WholePath_listBox->Items->Clear();
		m_pMPDIUI->AllPI_listBox->Items->Clear();
		ShowImageOnScreen_listBox->Items->Clear();
		AllEventPointHint_listBox->Items->Clear();
		m_pMPDIUI->PathNode_listBox->Items->Clear();
		System::Object^  l_pSender;
		System::EventArgs^l_pEventArgsE;
		WholePath_listBox_SelectedIndexChanged(l_pSender,l_pEventArgsE);
		std::string	l_strFileName = DNCT::GcStringToChar(e_strFileName);
		m_pImagePlayManagerAWP->m_bShowErrorMsg = showErrorMsh_ToolStripMenuItem->Checked;
		bool	l_b = m_pImagePlayManagerAWP->Parse(l_strFileName.c_str());
		if( l_b )
		{
			CameraRotationX_numericUpDown->Value = 0;
			CameraRotationY_numericUpDown->Value = 0;
			CameraRotationZ_numericUpDown->Value = 0;
			m_strCurrentSaveFileName = e_strFileName;
			if( m_pImagePlayManagerAWP->Count() == 0 )
				return;
			if( System::IO::File::Exists(e_strFileName+"_extra") )
			{
				GCFORM::TreeNode^l_pRootNode = OpenXmlFileToNode(e_strFileName+"_extra",false);
				if( l_pRootNode->Nodes->Count )
				{
					int	l_iCount = l_pRootNode->Nodes->Count;
					for each(GCFORM::TreeNode^l_pMPDIDataNode in l_pRootNode->Nodes)
					{
						String^l_strKey = l_pMPDIDataNode->Text;
						m_pMPDIExtraData[l_strKey] = l_pMPDIDataNode;
					}
				}
			}
			m_pMPDIList = dynamic_cast<cMPDIList*>((*m_pImagePlayManagerAWP)[0]);
			m_pMultiPathDynamicImageAWP = new cMPDIForEditor(m_pMPDIList);
			m_pInterlUseForShowImagePos = new cCueToStartCurveWithTime(m_pMPDIList);
			m_strCurrentFileName = DNCT::WcharToGcstring(m_pMPDIList->GetName());
			m_pImageResourceManagerAWP = m_pImagePlayManagerAWP->GetAllBaseImageList();
			for( int i=0;i<m_pMPDIList->Count();++i )
			{
				cMultiPathDynamicImage*l_pMultiPathDynamicImage = (*m_pMPDIList)[i];
				WholeAnimationList_listBox->Items->Add(DNCT::WcharToGcstring(l_pMultiPathDynamicImage->GetName()));
			}
			for (int i = 0; i < m_pMPDIList->GetPIList()->Count(); ++i)
			{
				m_pMPDIUI->AllPI_listBox->Items->Add(DNCT::WcharToGcstring((*m_pMPDIList->GetPIList())[i]->GetNameWithoutFullPath()));
			}
			if(m_pMPDIUI->AllPI_listBox->Items->Count>0)
				m_pMPDIUI->AllPI_listBox->SelectedIndex = 0;
			sPuzzleData*l_pAllPuzzleData = m_pMPDIList->GetAllPuzzleData();
			if(l_pAllPuzzleData)
			{
				for( int i=0;i<m_pMPDIList->GetNumImage();++i )
				{
					m_pMPDIUI->WholePuzzleImageUnit_listBox->Items->Add(DNCT::WcharToGcstring(l_pAllPuzzleData[i].strFileName));
				}
			}
			m_pMPDIUI->AddPath_button->Enabled = true;
			m_pMPDIUI->Play_checkBox->Enabled = true;
			AllPlay_checkBox->Enabled = true;
			m_pMPDIUI->Loop_checkBox->Enabled = true;
			AllCurveLoop_checkBox->Enabled = true;
			m_bDataChanged = false;
			m_pMPDIUI->AssignLatestPI();
			if( showErrorMsh_ToolStripMenuItem->Checked )
			{
				String^l_strErrorMsg = MPDIListDataCheck(m_pMPDIList);
				if( l_strErrorMsg->Length )
				{
					WARNING_MSG(l_strErrorMsg);
				}
			}
			if( this->m_pForm )
				this->m_pForm->Text = e_strFileName+"   :OpenTime:"+System::DateTime::Now.ToString();
			else
				Text = e_strFileName+"   :OpenTime:"+System::DateTime::Now.ToString();
		}
		else
		{
			WARNING_MSG("file format error");
		}		   
	}
	System::Void MPDIEditor::OpenImageFile_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
		wglMakeCurrent( m_HdcMV,m_HGLRCMV );
		String^l_strFileName =  DNCT::OpenFileAndGetName("(*.pi;*.pib)|*.pi;*.pib");
		if( !l_strFileName )
		{
			return;
		}
		//ensure this is add or call first time add
		if( sender == AddPI_toolStripMenuItem && m_pImageResourceManagerAWP && m_pImageResourceManagerAWP->Count() >= 1 )
		{
			AddPIFile(l_strFileName);
		}
		else
		//first time add
		{
			OpenPIFile(l_strFileName);
			this->Text = l_strFileName;
		 }
		m_pMPDIUI->AssignLatestPI();
	}

	System::Void MPDIEditor::BKImage_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
		String^l_strFileName =  DNCT::OpenFileAndGetName("(*.png)|*.png");
		if( l_strFileName )
		{
			SAFE_DELETE(m_pBKImageAWP);
			m_pBKImageAWP = new cBaseImage(DNCT::GcStringToChar(l_strFileName).c_str());
		}
	}

	System::Void MPDIEditor::AddPath_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				while( m_bTimerInRun ){}
				 m_bDataChanged = true;
				 AddPath(sender,e);
				this->timer1->Enabled = true;
			 }

	System::Void MPDIEditor::WholePath_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( m_pMPDIUI->WholePath_listBox->SelectedItems->Count != 0 )
					PathSelectedIndexChanged(sender,e);
			 }
	System::Void MPDIEditor::timer1_Tick(System::Object^  sender, System::EventArgs^  e)
			 {
				if(!this->Visible)
					 return;
				if( m_pMPDICamera )
					m_pMPDICamera->m_bDoUpdate = CameraWorking_checkBox->Checked;
				m_bTimerInRun = true;
				cGameApp::m_spOpenGLRender->m_vViewPortSize.x = 0.f;
				cGameApp::m_spOpenGLRender->m_vViewPortSize.y = 0.f;
				cGameApp::m_spOpenGLRender->m_vViewPortSize.z = (float)splitContainer3->Panel1->Width;
				cGameApp::m_spOpenGLRender->m_vViewPortSize.w = (float)splitContainer3->Panel1->Height;
				wglMakeCurrent( m_HdcMV,m_HGLRCMV );
				this->m_pGameApp->Run();
				UseShaderProgram();
				m_fAlpha += 0.03f;
				if( m_fAlpha>1.f )
					m_fAlpha = 0.f;
				cGameApp::ApplyViewPort();
				m_pTimeAndFPS->Update();
				glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
				glClearColor(m_pvBGColor->x,m_pvBGColor->y,m_pvBGColor->z,m_pvBGColor->w );
				glClearDepth(1.0f);											// Depth Buffer Setup
				glAlphaFunc(GL_GREATER,0.0001f);
				glShadeModel(GL_SMOOTH);									// Select Smooth Shading
				POINT l_ViewportSize = { splitContainer3->Panel1->Width,splitContainer3->Panel1->Height };
				m_pOrthogonalCamera->Render();
				if(this->showGridToolStripMenuItem->Checked)
					m_pOrthogonalCamera->DrawGrid(0,0,Vector4(0.3f,0.7f,0.3f,0.7f));

				if(m_pMPDICamera && CameraWorking_checkBox->Checked)
				{
					//m_bDoUpdate = !m_bDoUpdate;
					//m_p2DCameraAnimation->Init();
					m_pMPDICamera->Update(m_pTimeAndFPS->fElpaseTime,CurrentProgress_trackBar->Value/1000.f);
					m_pMPDICamera->Render();
				}
				if( MPDIWorking_checkBox->Checked )
					MPDIRender();
				if( m_pDebugFont )
				{
					glEnable(GL_BLEND);
					glBlendFunc(m_sfactor,m_dfactor);
					glEnable(GL_TEXTURE_2D);
					glEnable2D(1920,1080);
					POINT	ptCursor = {(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y};
					std::wstring l_strMousePos;
					l_strMousePos = DNCT::GcStringToWchar(ptCursor.x.ToString()+","+ptCursor.y.ToString());
					m_pDebugFont->RenderFont(0,0,l_strMousePos.c_str());
					glDisable(GL_TEXTURE_2D);
				}
				m_pOrthogonalCamera->Render();
				POINT	l_Pos = {0,0};
				RenderRectangle(l_Pos,(int)m_pvResolution->x,(int)m_pvResolution->y,Vector4(1.f,0.f,0.f,1.f));
				SwapBuffers (m_HdcMV);
				m_pTimeCounterForAutoSaveFile->Update(m_pTimeAndFPS->fElpaseTime);
				//for tempo file if the editor occur crash...
				if( m_pTimeCounterForAutoSaveFile->bTragetTimrReached )
				{
					if( m_pMultiPathDynamicImageAWP&&m_pMPDIList&&m_pImagePlayManagerAWP )
					if( m_pMPDIList->Count() )
					{
						//this->timer1->Enabled = false;
						//String^l_strFileName = "_temp.mpdi";
						//m_pMultiPathDynamicImageAWP->SetName(L"_temp");
						//m_pMPDIList->AddObjectNeglectExist(m_pMultiPathDynamicImageAWP);
						//m_pMPDIList->SetName(DNCT::GcStringToWchar(l_strFileName));
						//m_pImagePlayManagerAWP->Export(DNCT::GcStringToChar(l_strFileName));
						//m_pMPDIList->RemoveObjectWithoutDelete(m_pMPDIList->GetObjectIndexByPointer(m_pMultiPathDynamicImageAWP));
						//this->timer1->Enabled = true;
					}
					m_pTimeCounterForAutoSaveFile->Start();
				}
				m_bTimerInRun = false;
			 }

	System::Void MPDIEditor::PathLOD_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
	{
				if(m_pMPDIUI->WholePath_listBox->SelectedIndex != -1)
				{
					m_pMultiPathDynamicImageAWP->GetObject(m_pMPDIUI->WholePath_listBox->SelectedIndex)->SetLOD((int)m_pMPDIUI->PathLOD_numericUpDown->Value);
					System::EventArgs^  e3;
					WholePath_listBox_SelectedIndexChanged(m_pMPDIUI->WholePath_listBox,e3);
				}
	}

	System::Void MPDIEditor::DelPoint_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
				this->timer1->Enabled = false;
				::Threading::Thread::Sleep(100);
				while( m_bTimerInRun ){}
				DelPoint(sender,e);
				this->timer1->Enabled = true;
	}

	System::Void MPDIEditor::AssignImage_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				while( m_bTimerInRun ){}
				ImageApply();
				this->timer1->Enabled = true;
			 }

	System::Void MPDIEditor::AlphaColor_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				 while( m_bTimerInRun ){}
				m_pMPDIUI->Play_checkBox->Checked = false;
				if( !m_bPreventChangeValue )
					AssignValueChange(sender);
				this->timer1->Enabled = true;
			 }

	System::Void MPDIEditor::Play_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				while( m_bTimerInRun ){}
				PlayCheckedChanged(sender,e);
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::TotalPlayTime_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( m_bPreventChangeAllTimeValueSetup )
					 return;
				 if( m_pMPDIUI->WholePath_listBox->SelectedIndex != -1 )
				 {
					int	l_iIndex = m_pMPDIUI->WholePath_listBox->SelectedIndex;
					for each (int l_iIndex in m_pMPDIUI->WholePath_listBox->SelectedIndices )
					{
						cCueToStartCurveWithTime*	l_pCueToStartCurveWithTime = m_pMultiPathDynamicImageAWP->GetObject(l_iIndex);
						if(l_pCueToStartCurveWithTime)
						{
							std::vector<float> l_fTimeList = l_pCueToStartCurveWithTime->GetOriginalTimeList();
							size_t	l_uiNum = l_fTimeList.size();
							if(l_uiNum == 1)//avoid only one point,we still have to setup data
							{
								l_uiNum = 2;
								float	l_fTimeGap = (float)m_pMPDIUI->TotalPlayTime_numericUpDown->Value/(float)(l_uiNum -1)/1000.f;
								l_fTimeList[0] = l_fTimeGap;
							}
							else
							{
								float	l_fTimeGap = (float)m_pMPDIUI->TotalPlayTime_numericUpDown->Value/(float)(l_uiNum -1)/1000.f;
								for( size_t i=0;i<l_uiNum;++i )
									l_fTimeList[i] = i*l_fTimeGap;
							}
							l_pCueToStartCurveWithTime->SetOriginalTimeList(l_fTimeList);
							l_pCueToStartCurveWithTime->Refresh();
						}
					}
					if( m_pMPDIUI->WholePath_listBox->SelectedIndices->Count == 1 )
					{
						m_pMPDIUI->WholePath_listBox->SelectedIndex = -1;
						m_pMPDIUI->WholePath_listBox->SelectedIndex = l_iIndex;
					}
				 }
			 }

	System::Void MPDIEditor::AddAnimationToList_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				while( m_bTimerInRun ){}
				PathModifyFunction(sender,e);
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				while( m_bTimerInRun ){}
				SaveAndOpen_MPDI(sender,e);
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::CurrentProgress_trackBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
				m_fTimeForCurrentProgess = (float)CurrentProgress_trackBar->Value/1000.f;

			 }
	System::Void MPDIEditor::ApplyLODToPoints_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				 while( m_bTimerInRun ){}
				 if( m_pMPDIUI->WholePath_listBox->SelectedIndex != -1 )
				 {
					if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
					{
						m_pMultiPathDynamicImageAWP->m_pCurrentCurve->SetOriginalToFinal();
						int	l_iIndex = m_pMPDIUI->WholePath_listBox->SelectedIndex;
						m_pMPDIUI->WholePath_listBox->SelectedIndex = -1;
						m_pMPDIUI->WholePath_listBox->SelectedIndex = l_iIndex;
					}
				 }
				 this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::BehaviorUpdateActive_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if(m_pMultiPathDynamicImageAWP && m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve->SetBehaviorUpdateActive( m_pMPDIUI->BehaviorUpdateActive_checkBox->Checked );
			 }

	System::Void MPDIEditor::AddShowImageOnScreenObject_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				while( m_bTimerInRun ){}
				if( sender == DelShowImageOnScreenObject_button )
				{
					System::Collections::ArrayList^	l_MyList = gcnew System::Collections::ArrayList;
					l_MyList->AddRange(ShowImageOnScreen_listBox->SelectedItems);
					for each (System::Object^l_pObject in l_MyList)
					{
						int	l_iIndex = ShowImageOnScreen_listBox->Items->IndexOf(l_pObject);
						m_pInterlUseForShowImagePos->DelPoint(l_iIndex);
						ShowImageOnScreen_listBox->Items->RemoveAt(l_iIndex);
					}
				}
				else
				{
					if( m_pMPDIUI->WholePath_listBox->SelectedItem )
					if( m_pMPDIUI->PathNode_listBox->SelectedItem )
					{
						for each( int l_iIndex in m_pMPDIUI->PathNode_listBox->SelectedIndices )
						{
							if( sender == AddShowImageOnScreenObject_button )
							{
								sTexBehaviorDataWithImageIndexData *l_pTexBehaviorDataWithImageIndexData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[l_iIndex];
								Vector3	l_vPos = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalPointList()[l_iIndex];
								m_pInterlUseForShowImagePos->AddPoint(l_vPos,0,l_pTexBehaviorDataWithImageIndexData->Size,l_pTexBehaviorDataWithImageIndexData->vAngle,l_pTexBehaviorDataWithImageIndexData->vColor,l_pTexBehaviorDataWithImageIndexData->iImageIndex,l_pTexBehaviorDataWithImageIndexData->bMirror,l_pTexBehaviorDataWithImageIndexData->pPI);
								ShowImageOnScreen_listBox->Items->Add(m_pMPDIUI->WholePath_listBox->SelectedItem->ToString()+":"+l_iIndex.ToString());
								m_pInterlUseForShowImagePos->SetAnimationDone(false);
								m_pInterlUseForShowImagePos->SetStart(true);
							}
						}
					}
				}
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::WholePuzzleImageUnit_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				 while( m_bTimerInRun ){}
				 if( m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedIndex != -1 )
				 {
					 //SAFE_DELETE(m_pMultiPathDynamicImageAWP->m_pBasemageForShow);
					 if( m_pCurrentWorkingPI )
					 {
						 cPuzzleImageUnit*l_pPuzzleImageUnit = m_pCurrentWorkingPI->GetObject(m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedIndex);
						 m_pMultiPathDynamicImageAWP->m_pBasemageForShow = l_pPuzzleImageUnit;
						 //m_pMultiPathDynamicImageAWP->m_pBasemageForShow = new cBaseImage(l_pPuzzleImageUnit);
						 //SAFE_DELETE(m_pMultiPathDynamicImageAWP->m_pBasemageForShow);
					 }
				 }
				 this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::addPointModeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				while( m_bTimerInRun ){}
				PathMoveModifyMode(sender,e);
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::PathPiorityUp_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				 while( m_bTimerInRun ){}
				 ListBoxSelectedItemWithObjectListMove<cMultiPathDynamicImage,cMultiPathDynamicImage>(sender == m_pMPDIUI->PathPiorityDown_button?false:true,m_pMPDIUI->WholePath_listBox,m_pMultiPathDynamicImageAWP,0);
				 this->timer1->Enabled = true;
			 }

	System::Void MPDIEditor::Mirror_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				if( m_pMPDIUI->WholePath_listBox->SelectedIndex != -1 && m_pMPDIUI->WholePath_listBox->SelectedIndices->Count == 1 )
				{
					if(m_pMPDIUI->PathNode_listBox->SelectedIndex == -1)
						return;
					int	l_iSmallestIndex = m_pMPDIUI->PathNode_listBox->Items->Count;
					for each ( int l_SelectedIndex in m_pMPDIUI->PathNode_listBox->SelectedIndices )
						if(l_iSmallestIndex>l_SelectedIndex)
							l_iSmallestIndex = l_SelectedIndex;
					sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[l_iSmallestIndex];
					m_pMultiPathDynamicImageAWP->SetCurrentSelectedIndex(m_pMPDIUI->WholePath_listBox->SelectedIndex);
					for(int i=0;i<m_pMPDIUI->PathNode_listBox->SelectedIndices->Count;++i)
					{
						int l_SelectedIndex =  m_pMPDIUI->PathNode_listBox->SelectedIndices[i];
						l_pTexBehaviorDataWithImageIndexData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[l_SelectedIndex];
						l_pTexBehaviorDataWithImageIndexData->bMirror = m_pMPDIUI->Mirror_checkBox->Checked;
					}
				 }
				else
				if( m_pMPDIUI->WholePath_listBox->SelectedIndices->Count > 1 )
				{
				
				}
			  }
	System::Void MPDIEditor::WholeAnimationList_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( WholeAnimationList_listBox->SelectedItem )
					AnimationName_textBox->Text = WholeAnimationList_listBox->SelectedItem->ToString();
			 }
	System::Void MPDIEditor::WholeAnimationList_listBox_DoubleClick(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				while( m_bTimerInRun ){}
				AddAnimationToList_button_Click(AnimationInvert_button,e);
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::EnableColorBlending_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( m_pMPDIUI->WholePath_listBox->SelectedIndex != -1 )
				 {
					 if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
					 {
						 m_pMPDIUI->SrcBlending_comboBox->SelectedIndex = BlendingIndexToIndex( m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetSrcBlendingMode() );
						 m_pMPDIUI->DestBlending_comboBox->SelectedIndex = BlendingIndexToIndex( m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetDestBlendingMode() );
						 m_pMultiPathDynamicImageAWP->m_pCurrentCurve->SetColorBlending(m_pMPDIUI->EnableColorBlending_checkBox->Checked);
						 m_pMPDIUI->SrcBlending_comboBox->Enabled = m_pMPDIUI->EnableColorBlending_checkBox->Checked;
						 m_pMPDIUI->DestBlending_comboBox->Enabled = m_pMPDIUI->EnableColorBlending_checkBox->Checked;
					 }
				 }
			 }
	System::Void MPDIEditor::WholeAnimationList_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				 while( m_bTimerInRun ){}
				 if(e->KeyCode == Keys::Enter)
				 {
					AddAnimationToList_button_Click(AnimationInvert_button,e);
				 }
				 this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::SrcBlending_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve->SetSrcBlendingMode(IndexToBlendingIndex(this->m_pMPDIUI->SrcBlending_comboBox->SelectedIndex));
			 }
	System::Void MPDIEditor::DestBlending_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve->SetDestBlendingMode(IndexToBlendingIndex(this->m_pMPDIUI->DestBlending_comboBox->SelectedIndex));
			 }

	System::Void MPDIEditor::openPathFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				if( sender == saveGamePathToolStripMenuItem )
				{
					String^l_str = DNCT::SaveFileAndGetName();
					if( l_str )
					{
						cCurveManager*l_pCurveManager = new cCurveManager;
						if( m_pMPDIList )
						{
							int	l_iNum = m_pMPDIList->Count();
							for( int i=0;i<l_iNum;++i )
							{
								cMultiPathDynamicImage*l_pMultiPathDynamicImage = (*m_pMPDIList)[i];
								int	l_iNum2 = l_pMultiPathDynamicImage->Count();
								for( int j=0;j<l_iNum2;++j )
								{
									cCueToStartCurveWithTime*l_pCueToStartCurvesWithTime = (*l_pMultiPathDynamicImage)[j];
									cCurveWithTime*l_pNewCurve = new cCueToStartCurveWithTime(m_pMPDIList);
									*l_pNewCurve = *l_pCueToStartCurvesWithTime;
									if(!l_pCurveManager->GetObject(l_pNewCurve->GetName()))
										l_pCurveManager->AddObject(l_pNewCurve);
									else
									{
										static int	l_siSeriesNumber = 0;
										WCHAR	l_strNewCurveName[MAX_PATH];
										while(l_pCurveManager->GetObject(l_pNewCurve->GetName()))
										{
											++l_siSeriesNumber;
											swprintf_s(l_strNewCurveName,MAX_PATH,L"%s%d\0",l_pNewCurve->GetName(),l_siSeriesNumber);
											l_pNewCurve->SetName(l_strNewCurveName);
										}
										l_pCurveManager->AddObject(l_pNewCurve);
									}
								}
							}
						}
						l_pCurveManager->ExportData(DNCT::GcStringToChar(l_str).c_str());
						SAFE_DELETE(l_pCurveManager);
					}			
				}
				else
				if( sender == savePathFileToolStripMenuItem )
				{
					if( !m_pReferenceCurveManagerAWP )
						return;
					String^l_str = DNCT::SaveFileAndGetName();
					if( l_str )
						m_pReferenceCurveManagerAWP->ExportData(DNCT::GcStringToChar(l_str).c_str());
				}
				else
				if( sender == openPathFileToolStripMenuItem )
				{
					String^l_str = DNCT::OpenFileAndGetName("(*path)|*.path");
					if( l_str )
					{
						if( !m_pReferenceCurveManagerAWP )
							m_pReferenceCurveManagerAWP = new cCurveManager();
						m_pReferenceCurveManagerAWP->Destroy();
						m_pReferenceCurveManagerAWP->SetCurrentSelectedIndex(-1);
						m_pReferenceCurveManagerAWP->Parse(DNCT::GcStringToChar(l_str));
						AllPath_listBox->Items->Clear();
						for( int i =0;i<m_pReferenceCurveManagerAWP->Count();++i )
						{
							AllPath_listBox->Items->Add(String(UT::WcharToChar((*m_pReferenceCurveManagerAWP)[i]->GetName()).c_str()).ToString());
						}
					}
				}
			 }
	System::Void MPDIEditor::AddPathIntoAllPath_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				 while( m_bTimerInRun ){}
				 ReferencePathModify(sender,e);
				 this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::AddEventPOint_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				 while( m_bTimerInRun ){}
				 EventPoint(sender,e);
				 this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::RearrangeWholePathTime_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				 while( m_bTimerInRun ){}
				if( m_pMultiPathDynamicImageAWP )
				{
					float	l_fNewTime = (float)m_pMPDIUI->RearrangeWholePathTime_numericUpDown->Value/1000.f;
					m_pMultiPathDynamicImageAWP->RearrangeTime(l_fNewTime);
					this->m_pMPDIUI->WholePath_listBox->SelectedIndex = -1;
				}
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::showImageBoderToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 System::Windows::Forms::ToolStripMenuItem^l_pObject = (System::Windows::Forms::ToolStripMenuItem^)sender;
				 l_pObject->Checked = !l_pObject->Checked;
				 if( sender == cameraFitToolStripMenuItem )
				 {
					splitContainer3_Panel1_Resize(sender,e);
				 }
			 }
	System::Void MPDIEditor::MergeMPDIData_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				 while( m_bTimerInRun ){}
				 if( WholeAnimationList_listBox->SelectedIndex != -1 )
				 {
					 if( WholeAnimationList_listBox->SelectedIndex != -1)
					 {
						 if( m_pMultiPathDynamicImageAWP )
						 {
							 m_bPreventChangeAllTimeValueSetup = true;
							 cMultiPathDynamicImage*l_pMultiPathDynamicImage = m_pMPDIList->GetObject(WholeAnimationList_listBox->SelectedIndex);
							 //set new offset time if need
							 WARING_YES_NO_TO_YES("add start time to all curve")
							 {
								float	l_fStartTime = CreateNumericFormAndGetValue(1000000.f,100.f,0);
								l_fStartTime /= 1000.f;
								int	l_iSelectedMPDICount = l_pMultiPathDynamicImage->Count();
								int	l_iTargetMPDICount = m_pMultiPathDynamicImageAWP->Count();
								int	l_iTargetMPDIOriginalCount = l_iTargetMPDICount-l_iSelectedMPDICount;
								for( int i=0;i<l_iSelectedMPDICount;++i )
								{
									cCueToStartCurveWithTime*l_pCueToStartCurveWithTime = m_pMultiPathDynamicImageAWP->GetObject(i+l_iTargetMPDIOriginalCount);
									l_pCueToStartCurveWithTime->SetStartTime(l_pMultiPathDynamicImage->GetObject(i)->GetStartTime()+l_fStartTime);
								}
							 }
							 m_pMultiPathDynamicImageAWP->Merge(l_pMultiPathDynamicImage);
							 m_pMPDIUI->WholePath_listBox->Items->Clear();
							 for(int i=0;i<m_pMultiPathDynamicImageAWP->Count();++i)
							 {
								cCueToStartCurveWithTime*l_pCueToStartCurvesWithTime = (*m_pMultiPathDynamicImageAWP)[i];
								String^l_strObjectName = DNCT::WcharToGcstring(l_pCueToStartCurvesWithTime->GetName());
								m_pMPDIUI->WholePath_listBox->Items->Add(l_strObjectName);
							 }
							 AllCurveLoop_checkBox->Checked = m_pMultiPathDynamicImageAWP->IsAnimationLoop();
							 AllPlay_checkBox->Checked = false;
							 Play_checkBox_CheckedChanged(AllPlay_checkBox,e);
							 m_bPreventChangeAllTimeValueSetup = false;
						 }
					 }
				 }
				 this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::CurrentProgress_trackBar_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				if(CurrentProgress_trackBar->Focused)
					m_bAnimationPauseByTrackValue = true;
			 }

	System::Void MPDIEditor::showPath_ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 System::Windows::Forms::ToolStripMenuItem^l_pToolStripMenuItem = (System::Windows::Forms::ToolStripMenuItem^)sender;
				 l_pToolStripMenuItem->Checked = !l_pToolStripMenuItem->Checked;
			 }
	System::Void MPDIEditor::DelPI_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				while( m_bTimerInRun ){}
				while(m_pMPDIUI->AllPI_listBox->SelectedItems->Count)
				{
					String^l_strPIName = m_pMPDIUI->AllPI_listBox->SelectedItems[0]->ToString();
					cPuzzleImage*l_pPuzzleImage = dynamic_cast<cPuzzleImage*>(m_pImageResourceManagerAWP->GetObject(DNCT::GcStringToWchar(l_strPIName)));
					bool    l_bInUsing = false;
					if( m_pMultiPathDynamicImageAWP )
					if( m_pMultiPathDynamicImageAWP->Count() != 0 )
					{
						if(m_pMultiPathDynamicImageAWP->IsUsingPI(l_pPuzzleImage,true))
						{
							l_bInUsing = true;
						}
					}
					std::wstring	l_strInUsingInfo;
					if( !l_bInUsing&&m_pMPDIList->RemovePIReference(l_pPuzzleImage,&l_strInUsingInfo) )
					{
						if( m_pMultiPathDynamicImageAWP )
							m_pMultiPathDynamicImageAWP->RemovePIReference(l_pPuzzleImage);
						m_pImageResourceManagerAWP->RemoveObject(l_pPuzzleImage->GetName());
						m_pMPDIUI->AllPI_listBox->Items->Remove(l_strPIName);
					}
					else
					{
						l_bInUsing = true;
					}
					if(l_bInUsing)
					{

						WARNING_MSG(l_strPIName+
							" in used or please remove current edit path data\n"
							+
							String(UT::WcharToChar(l_strInUsingInfo.c_str()).c_str()).ToString()
							);
					}
					m_pMPDIUI->AllPI_listBox->SelectedItems->Remove(l_strPIName);
				}
				m_pCurrentWorkingPI = 0;
				if( m_pMPDIUI->AllPI_listBox->Items->Count )
					m_pMPDIUI->AllPI_listBox->SelectedIndex = 0;
				if( m_pMPDIUI->AllPI_listBox->Items->Count == 0)
					m_pMPDIUI->AddPath_button->Enabled = false;
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::AllPI_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				m_pMPDIUI->WholePuzzleImageUnit_listBox->Items->Clear();
				if( m_pMPDIUI->AllPI_listBox->SelectedIndex != -1 )
				{
					cPuzzleImage*l_pPuzzleImageAWP = dynamic_cast<cPuzzleImage*>(m_pImageResourceManagerAWP->GetObject(DNCT::GcStringToWchar(m_pMPDIUI->AllPI_listBox->SelectedItem->ToString())));
					for( int i=0;i<l_pPuzzleImageAWP->Count();++i )
					{
						m_pMPDIUI->WholePuzzleImageUnit_listBox->Items->Add(DNCT::WcharToGcstring(l_pPuzzleImageAWP->GetObject(i)->GetName()));
					}
					if( m_pMPDIList )
					{
						m_pCurrentWorkingPI = m_pMPDIList->GetPuzzleImage(DNCT::GcStringToWchar(m_pMPDIUI->AllPI_listBox->SelectedItem->ToString()).c_str());
						if( m_pCurrentWorkingPI->Count() )
							m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedIndex = 0;
					}
				}
				else
				{
					m_pMPDIUI->WholePuzzleImageUnit_listBox->Items->Clear();
					m_pCurrentWorkingPI = 0;
				}
			 }

			 System::Void MPDIEditor::ApplyGroupDataChange_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				if( !m_pMultiPathDynamicImageAWP )
					return;
				Vector3	l_vPos = Vector3((float)GroupPosX_numericUpDown->Value,(float)GroupPosY_numericUpDown->Value,(float)GroupPosZ_numericUpDown->Value);
				switch(GroupPositionMoveType_comboBox->SelectedIndex)
				{
					case 1://relative
						m_pMultiPathDynamicImageAWP->SetRelativePosToChild(l_vPos);
						break;
					case 2://world
						m_pMultiPathDynamicImageAWP->SetCenterPosToChild(l_vPos);
						break;
				}
				float	l_fRotationXAngle = (float)GroupRotationX_numericUpDown->Value;
				float	l_fRotationYAngle = (float)GroupRotationY_numericUpDown->Value;
				float	l_fRotationZAngle = (float)GroupRotationZ_numericUpDown->Value;
				Quaternion l_Quaternion = Quaternion::EulerRotationQuaternion(D3DXToRadian(l_fRotationXAngle),D3DXToRadian(l_fRotationYAngle),D3DXToRadian(l_fRotationZAngle));
				switch(GroupRotationType_comboBox->SelectedIndex)
				{
					case 1://self
						m_pMultiPathDynamicImageAWP->SetSelfRotationToChild(l_Quaternion,GroupImageRotate_checkBox->Checked);
						break;
					case 2://world
						m_pMultiPathDynamicImageAWP->SetCenterRotationToChild(l_Quaternion,GroupImageRotate_checkBox->Checked);
						break;
				}
				int l_iScale = (int)GroupScale_numericUpDown->Value;
				m_pMultiPathDynamicImageAWP->SetScaleForAllChildPointData(l_iScale/100.f);
				
			 }
	System::Void MPDIEditor::ScaleWithPIAlphaStripPixel_toolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {

				 //make selected image at center.
				 WARING_YES_NO_TO_YES("ensure you have backup the file,because this convert may has problem")
				 {
					 if( !m_pMPDIList )
						 return;
					 int	l_i = m_pMPDIList->Count();
					 for( int i=0;i<l_i;++i )
					 {
						 cMultiPathDynamicImage*l_pMultiPathDynamicImage = m_pMPDIList->GetObject(i);
						 int	l_i2 = l_pMultiPathDynamicImage->Count();
						 for( int j=0;j<l_i2;++j )
						 {
							cCueToStartCurveWithTime*l_pCueToStartCurveWithTime = l_pMultiPathDynamicImage->GetObject(j);
							size_t l_i3 = l_pCueToStartCurveWithTime->GetPointDataList()->size();
							std::vector<Vector3>	l_OriginalPointList = l_pCueToStartCurveWithTime->GetOriginalPointList();
							for( size_t k=0;k<l_i3;++k )
							{
								//change size
								sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = (*l_pCueToStartCurveWithTime->GetPointDataList())[k];
								sPuzzleData*l_pPuzzleData = l_pTexBehaviorDataWithImageIndexData->pPI->GetPuzzleData(l_pTexBehaviorDataWithImageIndexData->iImageIndex);
								if( l_pPuzzleData->Size.x != l_pPuzzleData->OriginalSize.x ||
									l_pPuzzleData->Size.y != l_pPuzzleData->OriginalSize.y)
								{
									Vector2	l_vImageCenterOffset = l_pPuzzleData->GetCenterOffsetxWithRenderSize(l_pTexBehaviorDataWithImageIndexData->Size);
									Vector3	l_vPos = l_OriginalPointList[k];
									l_vPos.x -= l_vImageCenterOffset.x;	l_vPos.y -= l_vImageCenterOffset.y;
									//l_OriginalPointList[k] = l_vPos;
									if( l_pPuzzleData->OriginalSize.x == (int)l_pTexBehaviorDataWithImageIndexData->Size.x &&
										l_pPuzzleData->OriginalSize.y == (int)l_pTexBehaviorDataWithImageIndexData->Size.y)
									{
  										l_pTexBehaviorDataWithImageIndexData->Size.x = (float)l_pPuzzleData->Size.x;
										l_pTexBehaviorDataWithImageIndexData->Size.y = (float)l_pPuzzleData->Size.y;
									}
									else
									{
										Vector2	l_vScale((float)l_pPuzzleData->Size.x/(float)l_pPuzzleData->OriginalSize.x,(float)l_pPuzzleData->Size.y/(float)l_pPuzzleData->OriginalSize.y);
										l_pTexBehaviorDataWithImageIndexData->Size.x *= l_vScale.x;
										l_pTexBehaviorDataWithImageIndexData->Size.y *= l_vScale.y;								
									}
								}
								//change position
							}
							l_pCueToStartCurveWithTime->SetOriginalPointList(l_OriginalPointList);
						 }
					 }
				 }
			 }
	System::Void MPDIEditor::SelectFirstAndLast_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( m_pMPDIUI->PathNode_listBox->Items->Count>1 )
				 {
					 m_pMPDIUI->PathNode_listBox->SelectedIndices->Clear();
					 m_pMPDIUI->PathNode_listBox->SelectedIndices->Add(0);
					 m_pMPDIUI->PathNode_listBox->SelectedIndices->Add(m_pMPDIUI->PathNode_listBox->Items->Count-1);
				 }
			 }
	System::Void MPDIEditor::WholeAnimationListUp_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				while( m_bTimerInRun ){}
				ListBoxSelectedItemWithObjectListMove<cMPDIList,cMPDIList>(sender == WholeAnimationListUp_button?true:false,WholeAnimationList_listBox,m_pMPDIList,0);
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::saveImageToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				Sleep(10);
				m_pOrthogonalCamera->Reset();
				SaveCurrentBufferToImage("test.png",(int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Height());
				Sleep(10);
				System::Drawing::Bitmap^l_pBitMap = gcnew System::Drawing::Bitmap("test.png");
				Sleep(10);
				System::Drawing::Image^l_pNewImage = FixedSize2(l_pBitMap,(int)m_pOrthogonalCamera->GetResolution().x,(int)m_pOrthogonalCamera->GetResolution().y);
				if(System::IO::File::Exists("test2.png"))
					System::IO::File::Delete("test2.png");
				Sleep(10);
				l_pNewImage->Save("test2.png");
				delete l_pNewImage;
				delete l_pBitMap;            
				Sleep(10);
				System::IO::File::Delete("test.png");
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::WholePath_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
			{
				if( e->KeyCode == GCFORM::Keys::Enter )
				{
					m_pMPDIUI->Play_checkBox->Checked = !m_pMPDIUI->Play_checkBox->Checked;
					//PlayCheckedChanged(Play_checkBox,e);
				}
			}
	System::Void MPDIEditor::SelectAllPoints_button_Click(System::Object^  sender, System::EventArgs^  e)
			{
				if( sender == m_pMPDIUI->SelectAllPoints_button )
				{
					if(m_pMPDIUI->PathNode_listBox->Items->Count)
					{
						m_pMPDIUI->PathNode_listBox->SelectedIndices->Clear();
						for(int i=0;i<m_pMPDIUI->PathNode_listBox->Items->Count;++i)
							m_pMPDIUI->PathNode_listBox->SelectedIndices->Add(i);
					}
				}
				else
				if( sender == m_pMPDIUI->SelectEvenPoints_button )
				{
					if(m_pMPDIUI->PathNode_listBox->Items->Count)
					{
						m_pMPDIUI->PathNode_listBox->SelectedIndices->Clear();
						static bool l_sbEven = true;
						int	l_iStartIndex = 0;
						if( l_sbEven )
							l_iStartIndex = 1;
						for(int i=l_iStartIndex;i<m_pMPDIUI->PathNode_listBox->Items->Count;i+=2)
						{
							m_pMPDIUI->PathNode_listBox->SelectedIndices->Add(i);
						}
						l_sbEven = !l_sbEven;
					}				
				}
			}
	System::Void MPDIEditor::renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( !m_pMPDIList )
					 return;
				 int	l_i = m_pMPDIList->Count();
				 for( int i=0;i<l_i;++i )
				 {
					 cMultiPathDynamicImage*l_pMultiPathDynamicImage = m_pMPDIList->GetObject(i);
					 int	l_i2 = l_pMultiPathDynamicImage->Count();
					 for( int j=0;j<l_i2;++j )
					 {
						cCueToStartCurveWithTime*l_pCueToStartCurveWithTime = l_pMultiPathDynamicImage->GetObject(j);
						size_t l_i3 = l_pCueToStartCurveWithTime->GetPointDataList()->size();
						std::vector<Vector3>	l_OriginalPointList = l_pCueToStartCurveWithTime->GetOriginalPointList();
						for( size_t k=0;k<l_i3;++k )
						{
							//change size
							sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = (*l_pCueToStartCurveWithTime->GetPointDataList())[k];
							sPuzzleData*l_pPuzzleData = l_pTexBehaviorDataWithImageIndexData->pPI->GetPuzzleData(l_pTexBehaviorDataWithImageIndexData->iImageIndex);
							if( l_pPuzzleData->Size.x != l_pPuzzleData->OriginalSize.x ||
								l_pPuzzleData->Size.y != l_pPuzzleData->OriginalSize.y)
							{
								Vector2	l_vImageCenterOffset = l_pPuzzleData->GetCenterOffsetxWithRenderSize(l_pTexBehaviorDataWithImageIndexData->Size);
								Vector3	l_vPos = l_OriginalPointList[k];
								l_pTexBehaviorDataWithImageIndexData->Size.x = (float)l_pPuzzleData->Size.x;
								l_pTexBehaviorDataWithImageIndexData->Size.y = (float)l_pPuzzleData->Size.y;
							}
							//change position
						}
						l_pCueToStartCurveWithTime->SetOriginalPointList(l_OriginalPointList);
					 }
				 }
			 }
	System::Void MPDIEditor::toolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedItem )
				 {
					 for each( String^l_strItem in m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedItems )
					 {
						AddPath(l_strItem);
					 }
				 }
			 }
	System::Void MPDIEditor::splitContainer3_Panel1_Resize(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( !m_pOrthogonalCamera )
					 return;
				 if( cameraFitToolStripMenuItem->Checked )
				 {
					POINT l_ViewportSize = { splitContainer3->Panel1->Width,splitContainer3->Panel1->Height };
					Vector2	l_vOriginalResolution = m_pOrthogonalCamera->GetResolution();
					Vector2	l_vOriginalLeftUpPos = m_pOrthogonalCamera->GetCameraPos()-(l_vOriginalResolution/2.f);
					m_pOrthogonalCamera->SetResolution(Vector2((float)l_ViewportSize.x,(float)l_ViewportSize.y));
					m_pOrthogonalCamera->SetCameraPos(l_vOriginalLeftUpPos+m_pOrthogonalCamera->GetResolution()/2.f);
					m_pOrthogonalCamera->CameraUpdateByMouse(false,false,0,0,0,Vector2((float)splitContainer3->Panel1->Size.Width,(float)splitContainer3->Panel1->Size.Height));
				 }
				 else
				 {
					m_pOrthogonalCamera->SetResolution(*m_pvResolution);
					m_pOrthogonalCamera->CameraUpdateByMouse(false,false,0,0,0,Vector2((float)splitContainer3->Panel1->Size.Width,(float)splitContainer3->Panel1->Size.Height));
				 }
			 }

	System::Void MPDIEditor::AddSliderBarUI_toolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 MPDIEditor::AddGameUI(sender);
			 }
	System::Void MPDIEditor::addPointDataWithImageByPathNameToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 toolStripMenuItem1_Click(sender,e);
				 m_pMPDIUI->WholePath_listBox->SelectedIndices->Clear();
				 for (int l_iIndex = 0;l_iIndex<m_pMPDIUI->WholePath_listBox->Items->Count;++l_iIndex  )
					 m_pMPDIUI->WholePath_listBox->SelectedIndices->Add(l_iIndex);
				 if( m_pMPDIUI->WholePath_listBox->SelectedIndices->Count )
				 {
					if( !m_pCurrentWorkingPI )
					{
						WARNING_MSG("please select a pi");
						return;
					}
					bool	l_bAdded = false;
					for each( int l_iIndex in m_pMPDIUI->WholePath_listBox->SelectedIndices )
					{
						l_bAdded = true;
						std::wstring l_strImageName = DNCT::GcStringToWchar(m_pMPDIUI->WholePath_listBox->Items[l_iIndex]->ToString());
						cCueToStartCurveWithTime*l_pCueToStartCurveWithTime = m_pMultiPathDynamicImageAWP->GetObject(l_iIndex);
						cPuzzleImageUnit*l_pPIUnit = m_pCurrentWorkingPI->GetObject(l_strImageName);
						if( l_pCueToStartCurveWithTime->GetOriginalPointList().size() == 0 )
						{
							if( l_pPIUnit )
							{
								int	l_iImageIndex = m_pCurrentWorkingPI->GetObjectIndexByPointer(l_pPIUnit);
								POINT	l_Size = m_pCurrentWorkingPI->GetPuzzleData(l_iImageIndex)->Size;
								l_pCueToStartCurveWithTime->AddPoint(Vector3::Zero,0,Vector2((float)l_Size.x,(float)l_Size.y),0,Vector4::One,l_iImageIndex,false,m_pCurrentWorkingPI);
								l_pCueToStartCurveWithTime->AddPoint(Vector3::Zero,1.f,Vector2((float)l_Size.x,(float)l_Size.y),0,Vector4::One,l_iImageIndex,false,m_pCurrentWorkingPI);
							}
							else
							{
								POINT	l_Size = m_pCurrentWorkingPI->GetPuzzleData(0)->Size;
								l_pCueToStartCurveWithTime->AddPoint(Vector3::Zero,0,Vector2((float)l_Size.x,(float)l_Size.y),0,Vector4::One,0,false,m_pCurrentWorkingPI);
								l_pCueToStartCurveWithTime->AddPoint(Vector3::Zero,1.f,Vector2((float)l_Size.x,(float)l_Size.y),0,Vector4::One,0,false,m_pCurrentWorkingPI);						
							}
						}
					}
					if(l_bAdded)
					{
						m_pMPDIUI->WholePath_listBox->SelectedIndex = -1;
						m_pMPDIUI->WholePath_listBox->SelectedIndex = m_pMPDIUI->WholePath_listBox->Items->Count-1;
					}
				 }
			 }
	System::Void MPDIEditor::FadeIn_toolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if(m_pMPDIUI->PathNode_listBox->Items->Count)
				 {
					if(m_pMultiPathDynamicImageAWP->m_pCurrentCurve)
					{
						int	l_iSize = (int)m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList()->size();
						float	l_fStep = 1.f/l_iSize;
						if( FadeIn_toolStripMenuItem == sender )
						{
							float	l_fAlpha = 1.f;
							for( int i=0;i<l_iSize;++i )
							{
								sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointData(i);
								l_pTexBehaviorDataWithImageIndexData->vColor.w = l_fAlpha;
								l_fAlpha -= l_fStep;
							}
						}
						else
						if( sender == FadeOut_toolStripMenuItem )
						{
							float	l_fAlpha = 0;
							for( int i=0;i<l_iSize;++i )
							{
								sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointData(i);
								l_pTexBehaviorDataWithImageIndexData->vColor.w = l_fAlpha;
								l_fAlpha += l_fStep;						
							}
						}
					}
					}
			 }
	System::Void MPDIEditor::mergeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 String^l_strFileName = DNCT::OpenFileAndGetName("mpdi files (*.mpdi;*.mpdib)|*.mpdi;*.mpdib|All files (*.*)|*.*");
				 if( l_strFileName )
				 {
					 Merge(l_strFileName);				 
				 }
				 
			 }
	System::Void MPDIEditor::UseViewPort_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( !m_pMultiPathDynamicImageAWP )
					 return;
				 Vector4*l_pViewPort = 0;
				 if( m_pMPDIUI->UseViewPort_checkBox->Checked )
				 {
					 if(!m_pMultiPathDynamicImageAWP->GetViewPort())
					 {
						m_pMultiPathDynamicImageAWP->SetViewPort(Vector4(0,0,m_pvResolution->x,m_pvResolution->y));
					 }
					l_pViewPort = m_pMultiPathDynamicImageAWP->GetViewPort();
					m_pMPDIUI->ViewPort_xyzwNumericalControl->SetValue(&l_pViewPort->x,&l_pViewPort->y,&l_pViewPort->z,&l_pViewPort->w);
				 }
				 else
				 {
					m_pMPDIUI->ViewPort_xyzwNumericalControl->SetValue(0,0,0,0);
					m_pMultiPathDynamicImageAWP->RemoveViewPort();
				 }
			 }
	System::Void MPDIEditor::ImageAnchor_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if(m_bPreventChangeAllTimeValueSetup)
					 return;
				if( m_pMultiPathDynamicImageAWP && m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
				{
					eImageAnchorType	l_eImageAnchorType = (eImageAnchorType)m_pMPDIUI->ImageAnchor_comboBox->SelectedIndex;;

					for each(int l_iSelectedIndex in m_pMPDIUI->WholePath_listBox->SelectedIndices)
					{
						m_pMultiPathDynamicImageAWP->GetObject(l_iSelectedIndex)->SetAnchorType(l_eImageAnchorType);
					}
				}
			 }
	System::Void MPDIEditor::floatToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if(m_pForm)
				 {
					 HWND	l_pHwnd = (HWND)m_pForm->Handle.ToPointer();
					 if( sender == floatToolStripMenuItem )
					 {
						 SetParent(l_pHwnd,0);
						 m_pForm->TopLevel = true;
					 }
					 else
					 if( sender == childToolStripMenuItem )
					 {
						 m_pForm->TopLevel = false;
						SetParent(l_pHwnd,m_pParentHandle);
					 }
				 }
			 }
	System::Void MPDIEditor::MyKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
		if( e->KeyCode == GCFORM::Keys::F11 )
		{
			if( splitContainer1->Panel1Collapsed )
			{
				splitContainer1->Panel1Collapsed = false;
				splitContainer2->Panel1Collapsed = false;
				menuStrip1->Visible = true;
				this->FormBorderStyle = GCFORM::FormBorderStyle::Sizable;
				//this->WindowState = GCFORM::FormWindowState::Normal;
			}
			else
			{
				splitContainer1->Panel1Collapsed = true;
				splitContainer2->Panel1Collapsed = true;
				menuStrip1->Visible = false;
				this->FormBorderStyle = GCFORM::FormBorderStyle::None;
				this->WindowState = GCFORM::FormWindowState::Maximized;
				SetWindowPos((HWND)this->Handle.ToPointer(), HWND_TOP, 0, 0, GetSystemMetrics(0), GetSystemMetrics(1), SWP_SHOWWINDOW);
			}
		}
		else
		if( e->KeyCode == GCFORM::Keys::F10 )
		{
			if( splitContainer2->Panel1Collapsed )
			{
				splitContainer2->Panel1Collapsed = false;
			}
			else
			{
				splitContainer2->Panel1Collapsed = true;
			}
		}
		else
		if( e->KeyCode == GCFORM::Keys::F9 )
		{
			if( splitContainer1->Panel1Collapsed )
			{
				splitContainer1->Panel1Collapsed = false;
			}
			else
			{
				splitContainer1->Panel1Collapsed = true;
			}
		}
	}

	System::Void MPDIEditor::CameraRotationX_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if( m_bPreventChangeAllTimeValueSetup )
			return;
		if( m_pMultiPathDynamicImageAWP )
		{
			cMatrix44 l_Mat = m_pMultiPathDynamicImageAWP->GetLocalTransform();
			Vector3	l_vRotation((float)CameraRotationX_numericUpDown->Value,
								(float)CameraRotationY_numericUpDown->Value,
								(float)CameraRotationZ_numericUpDown->Value);
			Vector3	l_vRotationCenter((float)CameraRotationCenterX_numericUpDown->Value,
								(float)CameraRotationCenterY_numericUpDown->Value,
								(float)CameraRotationCenterZ_numericUpDown->Value);
			if( l_vRotation.x !=0 || l_vRotation.y !=0 || l_vRotation.z !=0 || 
				l_vRotationCenter.x != 0 || l_vRotationCenter.y !=0 || l_vRotationCenter.z !=0)
			{
				l_Mat = cMatrix44::RotationMatrix(l_vRotation);
				l_Mat.m[3][0] = l_vRotationCenter.x;
				l_Mat.m[3][1] = l_vRotationCenter.y;
				l_Mat.m[3][2] = l_vRotationCenter.z;
				//m_pMultiPathDynamicImageAWP->SetRotateAroundOriginlPoint
			}
			m_pMultiPathDynamicImageAWP->SetLocalTransform(l_Mat);
		}
	}

	System::Void MPDIEditor::EventFindFile_button_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		String^l_strFilter = "xml files (*.xml)|*.xml|All files (*.*)|*.*";
		if( HintPointType_comboBox->SelectedIndex == 1 )
			l_strFilter = "Sound Files(*.wav)|*.wav";
		else
			l_strFilter = "Particle Files(*.prt)|*.prt";
		String^l_strFileName = DNCT::OpenFileAndGetName("Event Files(*.wav;*.prt)|*.wav;*.prt");
		if( l_strFileName && HintPointType_comboBox->SelectedIndex == 1 )
		{
			cGameApp::m_spSoundParser->AddStaticSound(cGameApp::m_spSoundParser,DNCT::GcStringToChar(l_strFileName).c_str());
			EventPointName_textBox->Text = DNCT::GetFileNameWithoutFullPath(l_strFileName);
		}
	}

	System::Void MPDIEditor::splitContainer3_Resize(System::Object^  sender, System::EventArgs^  e) 
	{
		if( splitContainer1->Panel1Collapsed && splitContainer2->Panel1Collapsed )
		{
			splitContainer3->Panel2Collapsed = true;
		}
		else
		{
			splitContainer3->Panel2Collapsed = false;
			splitContainer3->SplitterDistance = splitContainer3->Height-25;
		}
	}

	System::Void MPDIEditor::x1080ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		x1080ToolStripMenuItem->Checked = false;
		x720ToolStripMenuItem->Checked = false;
		x480ToolStripMenuItem->Checked = false;
		x1280ToolStripMenuItem->Checked = false;
		x1024ToolStripMenuItem->Checked = false;
		x1334ToolStripMenuItem->Checked = false;
		x2208ToolStripMenuItem->Checked = false;
		x2048ToolStripMenuItem->Checked = false;
		x2732ToolStripMenuItem->Checked = false;
		x900ToolStripMenuItem->Checked = false;
		x1920ToolStripMenuItem->Checked = false;
		System::Windows::Forms::ToolStripMenuItem^l_pObject = (System::Windows::Forms::ToolStripMenuItem^)sender;
		l_pObject->Checked = true;
		if( l_pObject == x480ToolStripMenuItem )
		{
			m_pvResolution->x = 640.f;
			m_pvResolution->y = 480.f;		
		}
		else
		if (l_pObject == x720ToolStripMenuItem)
		{
			m_pvResolution->x = 1280.f;
			m_pvResolution->y = 720.f;
		}
		else
		if(l_pObject == x900ToolStripMenuItem)
		{
			m_pvResolution->x = 1440.f;
			m_pvResolution->y = 900.f;
		}
		else
		if (l_pObject == x1080ToolStripMenuItem)
		{
			m_pvResolution->x = 1920.f;
			m_pvResolution->y = 1080.f;
		}
		else
		if( l_pObject == x1280ToolStripMenuItem )
		{
			m_pvResolution->x = 720.f;
			m_pvResolution->y = 1280.f;		
		}
		else
		if( l_pObject == x1024ToolStripMenuItem )
		{
			m_pvResolution->x = 1600.f;
			m_pvResolution->y = 1024.f;
		}
		else
		if( l_pObject == x1334ToolStripMenuItem )
		{
			m_pvResolution->x = 1334.f;
			m_pvResolution->y = 750.f;
		}
		else
		if( l_pObject == x2208ToolStripMenuItem )
		{
			m_pvResolution->x = 2208.f;
			m_pvResolution->y = 1242.f;
		}
		else
		if( l_pObject == x2048ToolStripMenuItem )
		{
			m_pvResolution->x = 2048;
			m_pvResolution->y = 1536;
		}
		else
		if( l_pObject == x2732ToolStripMenuItem )
		{
			m_pvResolution->x = 2732;
			m_pvResolution->y = 2048;
		}
		else
		if (l_pObject == x1920ToolStripMenuItem)
		{
			m_pvResolution->x = 1080;
			m_pvResolution->y = 1920;
		}
	}

	System::Void MPDIEditor::ColorChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if (!this->timer1->Enabled)
			return;
		CoreWPF::Common::ColorPicker^l_pColorPicker = m_pBGColorPicker;
		m_pvBGColor->x = l_pColorPicker->GetSelectedColor().ScR;
		m_pvBGColor->y = l_pColorPicker->GetSelectedColor().ScG;
		m_pvBGColor->z = l_pColorPicker->GetSelectedColor().ScB;
		m_pvBGColor->w = l_pColorPicker->GetSelectedColor().ScA;
		cGameApp::m_spOpenGLRender->m_vBGColor = *m_pvBGColor;
	}
//end namespace
}