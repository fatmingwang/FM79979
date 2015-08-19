#include "StdAfx.h"
#include "EventButtonTrigger.h"

namespace FMBookEditor
{
		EventButtonTrigger_Editor::EventButtonTrigger_Editor(FMBookEditorUI::Event::Trigger::EventButton^e_pEventButton)
		{
			m_pButton = new cEventButton((TiXmlElement*)0);
			sFMBookEditorData::m_spEventObjectInstance->Init();
			m_pEventButton = e_pEventButton;
			m_pEventButton->f_OpenColorDialog += gcnew EventHandler(this,&EventButtonTrigger_Editor::OpenColorDialog);
			m_pEventButton->f_Valdiate += gcnew EventHandler(this,&EventButtonTrigger_Editor::Valdiate);

			m_pEventButton->m_ImagePos->m_X_numericUpDown->f_ValueChanged += gcnew EventHandler(this,&EventButtonTrigger_Editor::PosChange);
			m_pEventButton->m_ImagePos->m_Y_numericUpDown->f_ValueChanged += gcnew EventHandler(this,&EventButtonTrigger_Editor::PosChange);
			m_pEventButton->m_ImagePos->m_Z_numericUpDown->f_ValueChanged += gcnew EventHandler(this,&EventButtonTrigger_Editor::PosChange);

			m_pvButtonPos = new Vector3(Vector3::Zero);
			m_pEventButton->m_ImagePos->SetValue((float*)m_pvButtonPos);
		}

		EventButtonTrigger_Editor::EventButtonTrigger_Editor(FMBookEditorUI::Event::Trigger::EventButtonTrigger^e_pEventButtonTrigger)
		{
			m_pButton = new cEventButton((TiXmlElement*)0);
			sFMBookEditorData::m_spEventInstance->m_TriggerEventList.AddObjectNeglectExist(m_pButton);
			sFMBookEditorData::m_spEventObjectInstance->Init();
			m_pEventButton = e_pEventButtonTrigger->m_Button;
			m_pEventButton->f_OpenColorDialog += gcnew EventHandler(this,&EventButtonTrigger_Editor::OpenColorDialog);
			m_pEventButton->f_Valdiate += gcnew EventHandler(this,&EventButtonTrigger_Editor::Valdiate);

			m_pEventButton->m_ImagePos->m_X_numericUpDown->f_ValueChanged += gcnew EventHandler(this,&EventButtonTrigger_Editor::PosChange);
			m_pEventButton->m_ImagePos->m_Y_numericUpDown->f_ValueChanged += gcnew EventHandler(this,&EventButtonTrigger_Editor::PosChange);
			m_pEventButton->m_ImagePos->m_Z_numericUpDown->f_ValueChanged += gcnew EventHandler(this,&EventButtonTrigger_Editor::PosChange);

			m_pvButtonPos = new Vector3(Vector3::Zero);
			m_pEventButton->m_ImagePos->SetValue((float*)m_pvButtonPos);
		}

		EventButtonTrigger_Editor::EventButtonTrigger_Editor(FMBookEditorUI::Event::Trigger::EventButtonTrigger^e_pEventButtonTrigger,cEventButton*e_pEventButton)
		{
			m_pButton = 0;
			m_pvButtonPos = 0;
			m_pEventButton = e_pEventButtonTrigger->m_Button;
			SetData(e_pEventButton,true);
		}

		EventButtonTrigger_Editor::~EventButtonTrigger_Editor()
		{
			int	l_iIndex = sFMBookEditorData::m_spEventInstance->m_TriggerEventList.GetObjectIndexByPointer(m_pButton);
			if( l_iIndex != -1 )
			{
				sFMBookEditorData::m_spEventInstance->m_TriggerEventList.RemoveObject(l_iIndex);
			}
			else
			//for multi button UI
			{
				SAFE_DELETE(m_pButton);
			}
			SAFE_DELETE(m_pvButtonPos);
		}
		System::Void EventButtonTrigger_Editor::OpenColorDialog(System::Object^  sender, System::EventArgs^  e)
		{
			System::Windows::Forms::ColorDialog^l_pDialog = gcnew System::Windows::Forms::ColorDialog;
			if(l_pDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				m_pEventButton->m_BGColor->Text = l_pDialog->Color.R.ToString()+","+l_pDialog->Color.G.ToString()+","+l_pDialog->Color.B.ToString()+","+l_pDialog->Color.A.ToString();
			}
		}

		System::Void EventButtonTrigger_Editor::Valdiate(System::Object^  sender, System::EventArgs^  e)
		{
			cClickMouseBehavior*l_pButton = m_pButton->GetButton();
			if(this->m_pEventButton->m_Result->Text->Length>0)
			{
				m_pButton->SetResult(DNCT::GcStringToWchar(this->m_pEventButton->m_Result->Text));
			}
			else
			{
				m_pButton->SetResult(L"");
			}
			m_pButton->SetButton(0,0);
			//base image
			if(m_pEventButton->m_ButtonTypeListbox->SelectedIndex == 0 )
			{
				WCHAR*l_strImageName = DNCT::GcStringToWchar(m_pEventButton->m_ImageName->Text);
				if(l_strImageName)
				{
					cBaseImage*l_pImage = cGameApp::GetBaseImage(l_strImageName);
					if( l_pImage )
					{
						l_pButton = new cImageButton(l_pImage,*m_pvButtonPos);
						m_pButton->SetButton(l_pButton,l_pImage);
					}
				}
			}
			else//PIUnit
			if(m_pEventButton->m_ButtonTypeListbox->SelectedIndex == 1 )
			{
				WCHAR*l_strPIName = DNCT::GcStringToWchar(m_pEventButton->m_PIName->Text);
				if(l_strPIName)
				{
					cPuzzleImage*l_pPI = cGameApp::GetPuzzleImage(l_strPIName);
					WCHAR*l_strPIunitName = DNCT::GcStringToWchar(m_pEventButton->m_PIUnitName->Text);
					if( l_strPIunitName )
					{
						cPuzzleImageUnit*l_pPIUnit = l_pPI->GetObject(l_strPIunitName);
						if( l_pPIUnit )
						{
							l_pButton = new cImageButton(l_pPIUnit,*m_pvButtonPos);
							m_pButton->SetButton(l_pButton,l_pPIUnit);
						}
					}
				}
			}
			else//text
			if(m_pEventButton->m_ButtonTypeListbox->SelectedIndex == 2 )
			{
				int	l_iSize = 0;
				std::vector<int>l_Color = DNCT::GetIntegerListByCommaDivide(m_pEventButton->m_BGColor->Text,&l_iSize);
				if( l_iSize == 4 )
				{
					if(m_pEventButton->m_Text->Text->Length>0)
					{
						WCHAR*l_strText = DNCT::GcStringToWchar(m_pEventButton->m_Text->Text);
						Vector4	l_vColor(-1,-1,-1,-1);
						l_vColor[0] = l_Color[0]/255.f;
						l_vColor[2] = l_Color[1]/255.f;
						l_vColor[3] = l_Color[2]/255.f;
						l_vColor[4] = l_Color[3]/255.f;
						l_pButton = new cTextButton(Vector2(m_pvButtonPos->x,m_pvButtonPos->y),g_pCurrentEventManager->m_pEventFont,l_vColor,l_strText,0,0);
						m_pButton->SetButton(l_pButton,0);
					}
				}
			}
			if( l_pButton )
			{
				if(m_pEventButton->m_RepeatCheckbox)
				{
					m_pButton->SetRepeatEnable(true);
				}
			}
		}

		System::Void EventButtonTrigger_Editor::PosChange(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pButton && m_pButton->GetButton())
			{
				m_pButton->GetButton()->SetLocalPosition(*m_pvButtonPos);
			}
		}


		System::String^EventButtonTrigger_Editor::IsLegal()
		{
			System::String^l_strErrorMessage;
			if( m_pButton && !m_pButton->GetButton())
			{
				l_strErrorMessage = "no assign button data";
			}
			return l_strErrorMessage;
		}

		void		EventButtonTrigger_Editor::SetData(cEventButton*e_pEventButton,bool e_bNewInstance)
		{
			if( !m_pEventButton )
				return;
			SAFE_DELETE(m_pButton);
			m_pButton = new cEventButton(e_pEventButton);
			if( e_bNewInstance )
			{
				sFMBookEditorData::m_spEventInstance->m_TriggerEventList.AddObjectNeglectExist(m_pButton);
			}
			sFMBookEditorData::m_spEventObjectInstance->Init();
			m_pEventButton->m_Result->Text = DNCT::WcharToGcstring(m_pButton->GetResult());
			if( !m_pvButtonPos )
			{
				m_pvButtonPos = new Vector3(Vector3::Zero);
				m_pEventButton->m_ImagePos->SetValue((float*)m_pvButtonPos);
			}

			if(e_pEventButton->GetButton()->Type() == cTextButton::TypeID)
			{
				cTextButton*l_pTextButton = (cTextButton*)e_pEventButton->GetButton();
				m_pEventButton->m_ButtonTypeListbox->SelectedIndex = 2;
				m_pEventButton->m_Text->Text = DNCT::WcharToGcstring(l_pTextButton->GetTxt());
			}
			else
			if(e_pEventButton->GetButton()->Type() == cImageButton::TypeID)
			{
				cImageButton*l_pImageButton = (cImageButton*)e_pEventButton->GetButton();
				cRenderObject*l_pReferenceImage = e_pEventButton->GetReferenceImage();
				if(cBaseImage::TypeID == l_pReferenceImage->Type())
				{
					m_pEventButton->m_ButtonTypeListbox->SelectedIndex = 0;
					m_pEventButton->m_ImageName->Text = DNCT::WcharToGcstring(l_pReferenceImage->GetName());
				}
				else
				if(cPuzzleImageUnit::TypeID == l_pReferenceImage->Type())
				{
					m_pEventButton->m_ButtonTypeListbox->SelectedIndex = 1;
					cPuzzleImageUnit*l_pPIUnit = (cPuzzleImageUnit*)l_pReferenceImage;
					m_pEventButton->m_PIName->Text = DNCT::WcharToGcstring(l_pPIUnit->GetOwner()->GetName());
					m_pEventButton->m_PIUnitName->Text = DNCT::WcharToGcstring(l_pPIUnit->GetName());
				}
			}		
		}
//end namespace FMBookEditor
}