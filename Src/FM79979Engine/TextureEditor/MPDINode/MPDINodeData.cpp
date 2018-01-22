#include "stdafx.h"
#include "MPDINodeData.h"

namespace MPDINodeForm 
{

		MPDINodeData::MPDINodeData(TextureEditorWPF::MPDI::MPDINodeForm^e_pMPDINodeForm)
		{
			m_pMPDINode = nullptr;
			m_pMPDI_PathPointData = e_pMPDINodeForm->m_MPDI_PathPointData;
			m_bListboxValueChanged = false;
			m_pLinerDataModifer = new cLinerDataModifer();
			m_pSelectedPIUnit = 0;
			MPDINodeData_Load();
		}

		MPDINodeData::~MPDINodeData()
		{
			m_pMPDINode = nullptr;
			SAFE_DELETE(m_pLinerDataModifer);
		}

		System::Void MPDINodeData::MPDINodeData_Load()
		{
			//
			m_pMPDI_PathPointData->m_StartTime->f_ValueChanged				+= gcnew System::EventHandler(this, &MPDINodeData::StartTimeChanged);
			m_pMPDI_PathPointData->m_SelectedTime->f_ValueChanged			+= gcnew System::EventHandler(this, &MPDINodeData::SelectedTimeChanged);
			m_pMPDI_PathPointData->m_CurrentDataTotalTime->f_ValueChanged	+= gcnew System::EventHandler(this, &MPDINodeData::CurrentDataTotalTimeChanged);
			m_pMPDI_PathPointData->m_AllDataTotalTime->f_ValueChanged		+= gcnew System::EventHandler(this, &MPDINodeData::AllDataTotalTimeChanged);
			m_pMPDI_PathPointData->f_ColorFadeIn							+= gcnew System::EventHandler(this, &MPDINodeData::ColorFadeIn);
			m_pMPDI_PathPointData->f_ColorFadeOut							+= gcnew System::EventHandler(this, &MPDINodeData::ColorFadeOut);
			m_pMPDI_PathPointData->f_PositionAddButtonClick					+= gcnew System::EventHandler(this, &MPDINodeData::AddPositionButtonClick);
			m_pMPDI_PathPointData->f_PathBehaviorComboxSelecteChanged		+= gcnew System::EventHandler(this, &MPDINodeData::PathBehaviorComboxSelecteChanged);
			m_pMPDI_PathPointData->f_ColorAddButtonClick					+= gcnew System::EventHandler(this, &MPDINodeData::AddColorButtonClick);
			m_pMPDI_PathPointData->f_SizeAddButtonClick						+= gcnew System::EventHandler(this, &MPDINodeData::AddSizeButtonClick);
			m_pMPDI_PathPointData->f_RotationAddButtonClick					+= gcnew System::EventHandler(this, &MPDINodeData::AddRotationButtonClick);
			m_pMPDI_PathPointData->f_AnimationImageAddButtonClick			+= gcnew System::EventHandler(this, &MPDINodeData::AddAnimationImageButtonClick);
			m_pMPDI_PathPointData->f_DeleteButtonClick						+= gcnew System::EventHandler(this, &MPDINodeData::DeleteButtonClick);
			m_pMPDI_PathPointData->f_InsertButtonClick						+= gcnew System::EventHandler(this, &MPDINodeData::InsertButtonClick);
			m_pMPDI_PathPointData->f_LinerUpdateClick						+= gcnew System::EventHandler(this, &MPDINodeData::LinerUpdateClick);
			m_pMPDI_PathPointData->f_LoopClick								+= gcnew System::EventHandler(this, &MPDINodeData::LoopClick);
			m_pMPDI_PathPointData->m_LodCount->f_ValueChanged				+= gcnew System::EventHandler(this, &MPDINodeData::LODCountChanged);
			m_pMPDI_PathPointData->f_ImageAnchorSelectionChanged			+= gcnew System::EventHandler(this, &MPDINodeData::ImageAnchorChanged);
			m_pMPDI_PathPointData->f_ListBoxSelectionChanged				+= gcnew System::EventHandler(this, &MPDINodeData::ListBoxSelectionChanged);
			m_pMPDI_PathPointData->f_TabControlSelectionChanged				+= gcnew System::EventHandler(this, &MPDINodeData::TabControlSelectionChanged);
			m_pMPDI_PathPointData->m_Pos->f_ValueChanged					+= gcnew System::EventHandler(this, &MPDINodeData::PosValueChanged);
			m_pMPDI_PathPointData->m_Color->f_ValueChanged					+= gcnew System::EventHandler(this, &MPDINodeData::ColorChanged);
			m_pMPDI_PathPointData->m_Size->f_ValueChanged					+= gcnew System::EventHandler(this, &MPDINodeData::SizeChanged);
			m_pMPDI_PathPointData->m_Rotation->f_ValueChanged				+= gcnew System::EventHandler(this, &MPDINodeData::RotationChanged);
			m_pMPDI_PathPointData->m_PI->f_AddPIButtonClick					+= gcnew System::EventHandler(this, &MPDINodeData::AddPIButtonClick);
			m_pMPDI_PathPointData->m_PI->f_DelPIButtonClick					+= gcnew System::EventHandler(this, &MPDINodeData::DelPIButtonClick);
			m_pMPDI_PathPointData->m_PI->f_PIListBoxDrop					+= gcnew System::EventHandler(this, &MPDINodeData::PIListBoxDrop);
			m_pMPDI_PathPointData->m_PI->f_PIChangeSelection				+= gcnew System::EventHandler(this, &MPDINodeData::PIChangeSelection);
			m_pMPDI_PathPointData->m_PI->f_PIUnitChangeSelection			+= gcnew System::EventHandler(this, &MPDINodeData::PIUnitChangeSelection);
			m_pMPDI_PathPointData->InitData();
		}

		bool		MPDINodeData::IsLegalApplyData()
		{
			bool	l_bData = m_bListboxValueChanged;
			m_bListboxValueChanged = false;
			return !l_bData;
		}

		System::Void MPDINodeData::StartTimeChanged(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode == nullptr )
				return;
			float	l_fTime = (float)m_pMPDI_PathPointData->m_SelectedTime->Value;
			this->m_pMPDINode->SetStartTime(l_fTime);
		}

		System::Void MPDINodeData::SelectedTimeChanged(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode == nullptr )
				return;
			cTimeAndDataLinerUpdateInterface*l_pTimeAndDataLinerUpdateInterface = GetTimeAndDataLinerUpdateInterfaceFromTabControlSelectedIndex(m_pMPDI_PathPointData,m_pMPDINode);			
			if( l_pTimeAndDataLinerUpdateInterface )
			{
				float	l_fTime = (float)m_pMPDI_PathPointData->m_SelectedTime->Value;
				l_pTimeAndDataLinerUpdateInterface->ChangeTime(m_pMPDI_PathPointData->m_CurrentWorkingListbox->SelectedIndex,l_fTime);
			}
		}

		System::Void MPDINodeData::CurrentDataTotalTimeChanged(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode == nullptr )
				return;
			cTimeAndDataLinerUpdateInterface*l_pTimeAndDataLinerUpdateInterface = GetTimeAndDataLinerUpdateInterfaceFromTabControlSelectedIndex(m_pMPDI_PathPointData,m_pMPDINode);
			if( l_pTimeAndDataLinerUpdateInterface )
			{
				float	l_fCurrentDataTotalTime = (float)m_pMPDI_PathPointData->m_CurrentDataTotalTime->Value;
				l_pTimeAndDataLinerUpdateInterface->Rearrange(l_fCurrentDataTotalTime);
			}
		}

		System::Void MPDINodeData::AllDataTotalTimeChanged(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode == nullptr )
				return;
			float	l_fTotalTime = (float)m_pMPDI_PathPointData->m_AllDataTotalTime->Value;
			m_pMPDINode->RearrangeTime(l_fTotalTime);
			RefreshTabcontrolSelectedIndex();
		}

		System::Void MPDINodeData::ColorFadeOut(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode )
			{
				m_pMPDINode->GetColorData()->DoDataLiner(Vector4(1,1,1,1),Vector4(0,0,0,0));
			}
		}

		System::Void MPDINodeData::ColorFadeIn(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode )
			{
				m_pMPDINode->GetColorData()->DoDataLiner(Vector4(0,0,0,0),Vector4(1,1,1,1));
			}
		}
		System::Void MPDINodeData::LODCountChanged(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode )
			{
				
				m_pMPDINode->GetPosData()->SetLOD((int)m_pMPDI_PathPointData->m_LodCount->Value);
			}
		}
		System::Void MPDINodeData::PathBehaviorComboxSelecteChanged(System::Object^  sender, System::EventArgs^  e)
		{
            //<ComboBoxItem Content="PointAdd" />            //<ComboBoxItem Content="PointModify" />            //<ComboBoxItem Content="PathMove" />
			switch( this->m_pMPDI_PathPointData->m_PathVehaviorCombox->SelectedIndex )
			{
			case 0:
				this->m_pLinerDataModifer->SetPathMode(ePM_ADD_POINT);
				break;
			case 1:
				this->m_pLinerDataModifer->SetPathMode(ePM_FIX);
				break;
			case 2:
				this->m_pLinerDataModifer->SetPathMode(ePM_PATH_MOVING_ALL);
				break;
			}
		}		
		System::Void MPDINodeData::AddPositionButtonClick(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode == nullptr )
				return;
			Vector3 l_vPos = GetPos(m_pMPDI_PathPointData);
			float	l_fTime = GetProperTime(m_pMPDINode->GetPosData());
			m_pMPDINode->GetPosData()->AddData(l_vPos,l_fTime);
			AddPosToListbox(l_fTime,l_vPos,m_pMPDI_PathPointData);
		}

		System::Void MPDINodeData::AddColorButtonClick(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode == nullptr )
				return;
			Vector4 l_vColor = GetColor(m_pMPDI_PathPointData);
			float	l_fTime = GetProperTime(m_pMPDINode->GetColorData());
			m_pMPDINode->GetColorData()->AddData(l_vColor,l_fTime);
			AddColorToListbox(l_fTime,l_vColor,m_pMPDI_PathPointData);
		}
		System::Void MPDINodeData::AddSizeButtonClick(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode == nullptr )
				return;
			Vector2 l_vSize = GetSize(m_pMPDI_PathPointData);
			float	l_fTime = GetProperTime(m_pMPDINode->GetSizeData());
			m_pMPDINode->GetSizeData()->AddData(l_vSize,l_fTime);
			AddSizeToListbox(l_fTime,l_vSize,m_pMPDI_PathPointData);
		}
		System::Void MPDINodeData::AddRotationButtonClick(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode == nullptr )
				return;
			Vector3 l_vRotation = GetRotation(m_pMPDI_PathPointData);
			float	l_fTime = GetProperTime(m_pMPDINode->GetRotationData());
			m_pMPDINode->GetRotationData()->AddData(l_vRotation,l_fTime);
			AddRotationToListbox(l_fTime,l_vRotation,m_pMPDI_PathPointData);
		}

		System::Void MPDINodeData::AddAnimationImageButtonClick(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode == nullptr )
				return;
			cPuzzleImageUnit*l_pPIUnit = GetPIUnit(m_pMPDI_PathPointData);
			float	l_fTime = GetProperTime(m_pMPDINode->GetImageData());
			m_pMPDINode->GetImageData()->AddData(l_pPIUnit,l_fTime);
			AddImageToListbox(l_fTime,l_pPIUnit,m_pMPDI_PathPointData);
		}

		System::Void MPDINodeData::PosValueChanged(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode == nullptr )
				return;
			ValueChange<Vector3>
			(
				this,
				m_pMPDINode->GetPosData(),
				GetPos(m_pMPDI_PathPointData),3
			);
		}

		System::Void MPDINodeData::SizeChanged(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode == nullptr )
				return;
			ValueChange<Vector2>
			(
				this,
				m_pMPDINode->GetSizeData(),
				GetSize(m_pMPDI_PathPointData),2
			);
		}

		System::Void MPDINodeData::RotationChanged(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode == nullptr )
				return;
			ValueChange<Vector3>
			(
				this,
				m_pMPDINode->GetRotationData(),
				GetRotation(m_pMPDI_PathPointData),3
			);
		}

		System::Void MPDINodeData::ColorChanged(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode == nullptr )
				return;
			ValueChange<Vector4>
			(
				this,
				m_pMPDINode->GetColorData(),
				GetColor(m_pMPDI_PathPointData),4
			);
		}

		System::Void MPDINodeData::DeleteButtonClick(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode == nullptr )
				return;
			int	l_iStartIndex = -1;
			cTimeAndDataLinerUpdateInterface*l_pTimeAndDataLinerUpdateInterface = GetTimeAndDataLinerUpdateInterfaceFromTabControlSelectedIndex(m_pMPDI_PathPointData,m_pMPDINode);			
			if( l_pTimeAndDataLinerUpdateInterface )
			{
				while(m_pMPDI_PathPointData->m_CurrentWorkingListbox->SelectedItems->Count)
				{
					Object^l_pObject = m_pMPDI_PathPointData->m_CurrentWorkingListbox->SelectedItems[0];
					int	l_iIndex = m_pMPDI_PathPointData->m_CurrentWorkingListbox->Items->IndexOf(l_pObject);
					if(!l_pTimeAndDataLinerUpdateInterface->RemoveDtaa(l_iIndex))
					{
						assert(0&&"delete data failed MPDINodeData::DeleteButtonClick");
					}
					m_pMPDI_PathPointData->m_CurrentWorkingListbox->Items->RemoveAt(l_iIndex);
					if( l_iStartIndex == -1 )
						l_iStartIndex = l_iIndex;
				}
				if( l_iStartIndex != -1 )
				{
					for(int i=l_iStartIndex;i<m_pMPDI_PathPointData->m_CurrentWorkingListbox->Items->Count;++i)
					{
						Object^l_pObject = m_pMPDI_PathPointData->m_CurrentWorkingListbox->Items[i];
						String^l_strDescription = (String^)l_pObject;						
						String^l_strDescription2 = ChangeIndex(i+1,l_strDescription);
						m_pMPDI_PathPointData->m_CurrentWorkingListbox->Items[i] = l_strDescription2;
					}
				}
			}
		}

		System::Void MPDINodeData::InsertButtonClick(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode == nullptr )
				return;
			if( m_pMPDI_PathPointData->m_CurrentWorkingListbox->SelectedItems->Count == 1 && m_pMPDI_PathPointData->m_CurrentWorkingListbox->SelectedIndex != -1 )
			{
				int	l_iCount = (int)m_pMPDI_PathPointData->m_InsertCount->Value;
				int	l_iInsertIndex = m_pMPDI_PathPointData->m_CurrentWorkingListbox->SelectedIndex;
				int	l_iStartIndex = l_iInsertIndex+1;
				int	l_iTargetIndex = l_iStartIndex+l_iCount;
				switch(m_pMPDI_PathPointData->m_TabControl->SelectedIndex)
				{
					case 0:
						if(m_pMPDINode->GetPosData()->InsertDataWithLiner(l_iInsertIndex ,l_iCount))
						{
							for( int i=l_iStartIndex;i<l_iTargetIndex;++i )
							{
								float	l_fTime = *m_pMPDINode->GetPosData()->GetTime(i);
								InsertPosToListbox(i,l_fTime,*m_pMPDINode->GetPosData()->GetData(i),m_pMPDI_PathPointData);
							}
						}
						break;
					case 1:
						if(m_pMPDINode->GetColorData()->InsertDataWithLiner(l_iInsertIndex ,l_iCount))
						{
							for( int i=l_iStartIndex;i<l_iTargetIndex;++i )
							{
								float	l_fTime = *m_pMPDINode->GetColorData()->GetTime(i);
								InsertColorToListbox(i,l_fTime,*m_pMPDINode->GetColorData()->GetData(i),m_pMPDI_PathPointData);
							}
						}
						break;
					case 2:
						if(m_pMPDINode->GetSizeData()->InsertDataWithLiner(l_iInsertIndex ,l_iCount))
						{
							for( int i=l_iStartIndex;i<l_iTargetIndex;++i )
							{
								float	l_fTime = *m_pMPDINode->GetSizeData()->GetTime(i);
								InsertSizeToListbox(i,l_fTime,*m_pMPDINode->GetSizeData()->GetData(i),m_pMPDI_PathPointData);
							}
						}
						break;
					case 3:
						if(m_pMPDINode->GetRotationData()->InsertDataWithLiner(l_iInsertIndex ,l_iCount))
						{
							for( int i=l_iStartIndex;i<l_iTargetIndex;++i )
							{
								float	l_fTime = *m_pMPDINode->GetRotationData()->GetTime(i);
								InsertRotationToListbox(i,l_fTime,*m_pMPDINode->GetRotationData()->GetData(i),m_pMPDI_PathPointData);
							}						
						}
						break;
				}
				int	l_iEndIndex = m_pMPDI_PathPointData->m_CurrentWorkingListbox->Items->Count;
				for( int i=l_iTargetIndex;i<l_iEndIndex;++i )
				{
					String^l_strDescription = (String^)m_pMPDI_PathPointData->m_CurrentWorkingListbox->Items[i];
					String^l_strDescription2 = ChangeIndex(i,l_strDescription);
					m_pMPDI_PathPointData->m_CurrentWorkingListbox->Items[i] = l_strDescription2;
				}
			}	
		}

		System::Void MPDINodeData::LinerUpdateClick(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode == nullptr )
				return;
			cTimeAndDataLinerUpdateInterface*l_pTimeAndDataLinerUpdateInterface = GetTimeAndDataLinerUpdateInterfaceFromTabControlSelectedIndex(m_pMPDI_PathPointData,m_pMPDINode);			
			if( l_pTimeAndDataLinerUpdateInterface )
				l_pTimeAndDataLinerUpdateInterface->SetUpdateWithLiner(m_pMPDI_PathPointData->m_LinerUpdateCheckBox->IsChecked.Value);
		}

		System::Void MPDINodeData::LoopClick(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode == nullptr )
				return;
			m_pMPDINode->SetPlayCount(m_pMPDI_PathPointData->m_LoopCheckBox->IsChecked.Value?-1:1);
		}

		System::Void MPDINodeData::AddPIButtonClick(System::Object^  sender, System::EventArgs^  e)
		{
			if( cGameApp::m_spImageParser == nullptr )
				return;
			cli::array<String^>^l_strFiles = DNCT::OpenFileAndGetNames();
			if( l_strFiles )
			{
				for each( String^l_strFileName in l_strFiles )
				{
					if( AddPIFile(l_strFileName) )
					{
						m_pMPDI_PathPointData->m_PI->m_PI_listBox->Items->Add(l_strFileName);
					}
				}
			}
		}

		System::Void MPDINodeData::DelPIButtonClick(System::Object^  sender, System::EventArgs^  e)
		{
			if( cGameApp::m_spImageParser == nullptr )
				return;
			int	l_iCount = m_pMPDI_PathPointData->m_PI->m_PI_listBox->SelectedItems->Count;
			for( int i=0;i<l_iCount;++i )
			{
			
			}
		}

		System::Void MPDINodeData::PIListBoxDrop(System::Object^  sender, System::EventArgs^  e)
		{
			System::Windows::DragEventArgs^l_pDragEventArgs = (System::Windows::DragEventArgs^)e;
			cli::array<String^>^l_strFileNames = CoreWPF::Common::CommonFunction::DragDropEventWhileFileDrop(l_pDragEventArgs);
			for each(String^l_strFileName in l_strFileNames)
			{
				if(AddPIFile(l_strFileName))
				{
					m_pMPDI_PathPointData->m_PI->m_PI_listBox->Items->Add(System::IO::Path::GetFileNameWithoutExtension(l_strFileName));
				}
			}
		}

		System::Void MPDINodeData::PIChangeSelection(System::Object^  sender, System::EventArgs^  e)
		{
			int	l_iSelectedIndex = m_pMPDI_PathPointData->m_PI->m_PI_listBox->SelectedIndex;
			if( l_iSelectedIndex != -1 )
			{
				m_pMPDI_PathPointData->m_PI->m_PIUnit_listBox->Items->Clear();
				cPuzzleImage*l_pPI = cGameApp::m_spImageParser->GetPuzzleImage(l_iSelectedIndex);
				if( l_pPI )
				{
					for(int i=0;i<l_pPI->Count();++i)
					{
						m_pMPDI_PathPointData->m_PI->m_PIUnit_listBox->Items->Add(DNCT::WcharToGcstring(l_pPI->GetObject(i)->GetName()));
					}
				}
			}
		}

		System::Void MPDINodeData::PIUnitChangeSelection(System::Object^  sender, System::EventArgs^  e)
		{
			m_pSelectedPIUnit = 0;
			if( m_pMPDI_PathPointData->m_PI->m_PIUnit_listBox->SelectedIndex == -1)
				return;
			cPuzzleImage*l_pPI = cGameApp::m_spImageParser->GetPuzzleImage(m_pMPDI_PathPointData->m_PI->m_PI_listBox->SelectedIndex);
			m_pSelectedPIUnit = l_pPI->GetObject(m_pMPDI_PathPointData->m_PI->m_PIUnit_listBox->SelectedIndex);
		}

		System::Void MPDINodeData::ImageAnchorChanged(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode == nullptr )
				return;
			this->m_pMPDINode->SetAnchorType((eImageAnchorType)m_pMPDI_PathPointData->m_ImageAnchor->SelectedIndex);
		}

		System::Void MPDINodeData::RefreshTabcontrolSelectedIndex()
		{
			if( m_pMPDINode == nullptr )
				return;
			m_bListboxValueChanged = true;
			m_pMPDI_PathPointData->m_CurrentWorkingListbox->Items->Clear();
			int	l_iTabControlSelevtedIndex = m_pMPDI_PathPointData->m_TabControl->SelectedIndex;
			switch( l_iTabControlSelevtedIndex )
			{
				case TextureEditorWPF::MPDI_PathPointData::eTabControlSelectedIndex::eTSI_POS:
					ApplyAllPosDataInfoToListBox(this->m_pMPDINode,m_pMPDI_PathPointData);
					break;
				case TextureEditorWPF::MPDI_PathPointData::eTabControlSelectedIndex::eTSI_COLOR:
					ApplyAllColorDataInfoToListBox(this->m_pMPDINode,m_pMPDI_PathPointData);
					break;
				case TextureEditorWPF::MPDI_PathPointData::eTabControlSelectedIndex::eTSI_SIZE:
					ApplyAllSizeDataInfoToListBox(this->m_pMPDINode,m_pMPDI_PathPointData);
					break;
				case TextureEditorWPF::MPDI_PathPointData::eTabControlSelectedIndex::eTSI_ROTATION:
					ApplyAllRotationDataInfoToListBox(this->m_pMPDINode,m_pMPDI_PathPointData);
					break;
				case TextureEditorWPF::MPDI_PathPointData::eTabControlSelectedIndex::eTSI_IMAGE:
					break;
				default:
					break;
			}				 
		}


		void	MPDINodeData::SetMPDINode(cMPDINode*e_pMPDINode)
		{
			m_pMPDINode = e_pMPDINode;
			m_pMPDINode->Init();
			m_pLinerDataModifer->m_pSrcData = m_pMPDINode->GetPosData();
			RefreshTabcontrolSelectedIndex();
			//this->tim
		}

		void	MPDINodeData::Run(Vector3 e_vMousePos)
		{
			if( m_pMPDINode == nullptr )
				return;
			//m_pMPDINode->Update();
			//m_pMPDINode->Render();
			if(EditorCommonVariables::m_sbShowPIUnitClick && m_pSelectedPIUnit)
			{
				m_pSelectedPIUnit->Render((int)e_vMousePos.x,(int)e_vMousePos.y);
			}
			float	l_fCurrentTime = (float)m_pMPDI_PathPointData->m_TimeSlider->Value/1000.f;
			l_fCurrentTime = 0.01f;
			m_pMPDINode->Init();
			m_pMPDINode->UpdateByGlobalTime(l_fCurrentTime);
			if( EditorCommonVariables::m_sbShowPathPos )
			{
				//m_pMPDINode->GetPosData()->DebugRender(true,true);
				m_pMPDINode->DebugRender();
			}

		}
		System::Void MPDINodeData::TabControlSelectionChanged(System::Object^  sender, System::EventArgs^  e)
		{
			RefreshTabcontrolSelectedIndex();
		}

		System::Void MPDINodeData::ListBoxSelectionChanged(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pMPDINode == nullptr )
				return;
			m_bListboxValueChanged = true;
			int	l_iSelectedIndex = m_pMPDI_PathPointData->m_CurrentWorkingListbox->SelectedIndex;
			if(l_iSelectedIndex != -1 && m_pMPDI_PathPointData->m_CurrentWorkingListbox->SelectedItems->Count == 1)
			{
				switch(m_pMPDI_PathPointData->m_TabControl->SelectedIndex)
				{
				case TextureEditorWPF::MPDI_PathPointData::eTabControlSelectedIndex::eTSI_POS:
				{
					Vector3	l_vPos = *this->m_pMPDINode->GetPosData()->GetData(l_iSelectedIndex);
					m_pMPDI_PathPointData->m_Pos->SetValue((double)l_vPos.x,(double)l_vPos.y,(double)l_vPos.z);
				}
					break;
				case TextureEditorWPF::MPDI_PathPointData::eTabControlSelectedIndex::eTSI_COLOR:
				{
					Vector3	l_vColor = *this->m_pMPDINode->GetColorData()->GetData(l_iSelectedIndex);
					m_pMPDI_PathPointData->m_Color->SetValue((double)l_vColor.x,(double)l_vColor.y,(double)l_vColor.z,(double)l_vColor.z);
				}
					break;
				case TextureEditorWPF::MPDI_PathPointData::eTabControlSelectedIndex::eTSI_SIZE:
				{
					Vector2	l_vSize = *this->m_pMPDINode->GetSizeData()->GetData(l_iSelectedIndex);
					m_pMPDI_PathPointData->m_Size->SetValue((double)l_vSize.x,(double)l_vSize.y);
				}
					break;
				case TextureEditorWPF::MPDI_PathPointData::eTabControlSelectedIndex::eTSI_ROTATION:
				{
					Vector3	l_vRotation = *this->m_pMPDINode->GetRotationData()->GetData(l_iSelectedIndex);
					m_pMPDI_PathPointData->m_Rotation->SetValue((double)l_vRotation.x,(double)l_vRotation.y,(double)l_vRotation.z);
				}
					break;
				case TextureEditorWPF::MPDI_PathPointData::eTabControlSelectedIndex::eTSI_IMAGE:
					//m_pMPDI_PathPointData->m_PI;
					break;
				}
			}
		}
}