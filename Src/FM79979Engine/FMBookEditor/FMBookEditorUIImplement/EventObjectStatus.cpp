#include "StdAfx.h"
#include "EventObjectStatus.h"

namespace FMBookEditor 
{
	bool	g_bSelecteImageIsPI = false;

	EventObjectStatus::~EventObjectStatus()
	{
		SAFE_DELETE(m_pEditorEventObjectStatu);
	}

	System::Void EventObjectStatus::TextStatusTextChanged(System::Object^  sender,System::Windows::Controls::TextChangedEventArgs^  e)
	{
		if( g_pEditorEventObject )
		{
			if( g_pEditorEventObject->Type() == cEventText::TypeID )
			{
				cEventText*l_pEventText = (cEventText*)g_pEditorEventObject;
				l_pEventText->SetText(DNCT::GcStringToWchar(this->m_pEventObjectStatus->m_TextText->Text));
			}
		}
	}

	System::Void EventObjectStatus::UICombineRemove()
	{
		m_pEventObjectStatus->m_ImagePosXYZNumeric->SetValue(0);
		m_pEventObjectStatus->m_ImageSizeXYNumeric->SetValue(0);
		m_pEventObjectStatus->m_MPDIClickToPlay->SetValue(0);
		m_pEventObjectStatus->m_MPDIPosXYZNumeric->SetValue(0);
		m_pEventObjectStatus->m_MPDISizeXYNumeric->SetValue(0);
	}

	System::Void EventObjectStatus::UICombine(cEventObject*e_pEventObject)
	{
		if( !e_pEventObject )
			return;
		if( e_pEventObject->Type() == cEventImage::TypeID)
		{
			this->m_pEventObjectStatus->m_ImagePosXYZNumeric->SetValue((float*)e_pEventObject->GetLocalPositionPointer());
			this->m_pEventObjectStatus->m_ImageSizeXYNumeric->SetValue((float*)e_pEventObject->GetSize());
		}
		else
		if( e_pEventObject->Type() == cEventText::TypeID)
		{
			this->m_pEventObjectStatus->m_TextPosXYZNumeric->SetValue((float*)e_pEventObject->GetLocalPositionPointer());
		}
		else
		if( e_pEventObject->Type() == cEventMPDI::TypeID)
		{
			cEventMPDI*l_pEventMPDI = dynamic_cast<cEventMPDI*>(e_pEventObject);
			m_pEventObjectStatus->m_MPDIClickToPlay->SetValue((bool*)l_pEventMPDI->IsClickToPlay());
			m_pEventObjectStatus->m_MPDIPosXYZNumeric->SetValue((float*)l_pEventMPDI->GetLocalPositionPointer());
			m_pEventObjectStatus->m_MPDISizeXYNumeric->SetValue((float*)l_pEventMPDI->GetSize());
		}
		else
		if( e_pEventObject->Type() == cEventSound::TypeID)
		{
		
		}
		else
		if( e_pEventObject->Type() == cEventParticle::TypeID)
		{
		
		}
		else
		if( e_pEventObject->Type() == cEventParticleGroup::TypeID)
		{
		
		}
		else
		if( g_pEditorEventObject->Type() == cEventVariableDataRender::TypeID)
		{
			cEventVariableDataRender*l_pEventVariableDataRender = dynamic_cast<cEventVariableDataRender*>(e_pEventObject);
			l_pEventVariableDataRender->SetPrefixText(DNCT::GcStringToWchar(m_pEventObjectStatus->m_VariableRenderPrefixText->Text));
			cEventVariable*l_pEventVariable = g_pEditorMode->m_spEventManager->GetEventVariable(DNCT::GcStringToWchar(m_pEventObjectStatus->m_VariableRenderEventVariableName->Text));
			l_pEventVariableDataRender->SetEventVariable(l_pEventVariable);
			m_pEventObjectStatus->m_VariableRenderPosNumeric->SetValue((float*)l_pEventVariableDataRender->GetLocalPositionPointer());
		}
		else
		if( e_pEventObject->Type() == cEVentXMLNodeText::TypeID)
		{
		
		}
		else
		{
			std::string	l_str = UT::WcharToChar(g_pEditorEventObject->Type());
			l_str += "not support yet!";
			char*l_str2 = (char*)l_str.c_str();
			WARNING_MSG( String(l_str2).ToString() );
		}
	}

	System::Void EventObjectStatus::TabControlSelectionChanged(System::Object^  sender, System::EventArgs^  e)
	{
		System::Windows::Controls::TabControl^l_pTabControl = m_pTabControl;//(System::Windows::Controls::TabControl^)sender;
		System::Windows::Controls::TabItem^l_pTabItem = (System::Windows::Controls::TabItem^)l_pTabControl->SelectedItem;
		if( !l_pTabItem )
			return;
		m_pShowObjectTextBox->Text = "";
		System::Windows::Controls::TabItem^l_pEventObjectStatusTabItem = (System::Windows::Controls::TabItem^)this->m_pEventObjectStatus->m_TabPageControl->SelectedItem;
		if(l_pEventObjectStatusTabItem&&l_pEventObjectStatusTabItem->Header->ToString()->Equals("Text"))
		{			
			m_eEventObjectType = eEOT_TEXT;
		}
		else
		if(l_pTabItem->Header->ToString()->Equals("MPDI"))
		{
			if( m_pFatmingUtilities->m_MPDI->m_MPDI_listBox->SelectedItem )
			{
				System::Windows::Controls::ListBoxItem^l_pListBoxItem = (System::Windows::Controls::ListBoxItem^)m_pFatmingUtilities->m_MPDI->m_MPDIList_listBox->SelectedItem;
				//System::String^l_pListBoxItem = (System::String^)m_pFatmingUtilities->m_MPDI->m_MPDIList_listBox->SelectedItem;
				if( l_pListBoxItem )
				{
					m_pShowObjectTextBox->Text = l_pListBoxItem->Content->ToString();
					//m_pShowObjectTextBox->Text = l_pListBoxItem;
					POINT	l_Size = {100,100};						
					m_eEventObjectType = eEOT_MPDI;
					m_pEventObjectStatus->m_TabPageControl->SelectedIndex = 2;
				}
			}
		}
		else
		if(l_pTabItem->Header->ToString()->Equals("PI"))
		{
			if( m_pFatmingUtilitiesData->m_pPI->m_pCurrentPIUnit )
			//if( m_pFatmingUtilities->m_PI->m_PIUnit_listBox->SelectedItem )
			{
				//System::Windows::Controls::ListBoxItem^l_pListBoxItem = (System::Windows::Controls::ListBoxItem^)m_pFatmingUtilities->m_PI->m_PIUnit_listBox->SelectedItem;
				//if( l_pListBoxItem )
				{
					
					//m_pShowObjectTextBox->Text = l_pListBoxItem->Content->ToString();
					m_pShowObjectTextBox->Text = DNCT::WcharToGcstring(m_pFatmingUtilitiesData->m_pPI->m_pCurrentPIUnit->GetName());
					m_eEventObjectType = eEOT_IMAGE;
					g_bSelecteImageIsPI = true;
					m_pEventObjectStatus->m_TabPageControl->SelectedIndex = 0;
				}
			}
		}
		else
		if(l_pTabItem->Header->ToString()->Equals("Sound"))
		{
			if( m_pFatmingUtilitiesData->m_pSound->m_pCurrentBasicSound )
			{
				//System::Windows::Controls::ListBoxItem^l_pListBoxItem = (System::Windows::Controls::ListBoxItem^)m_pFatmingUtilities->m_Sound->m_Sound_Listbox->SelectedItem;
				//if( l_pListBoxItem )
				{
					//m_pShowObjectTextBox->Text = l_pListBoxItem->Content->ToString();
					m_pShowObjectTextBox->Text = DNCT::WcharToGcstring(m_pFatmingUtilitiesData->m_pSound->m_pCurrentBasicSound->GetName());
					m_eEventObjectType = eEOT_SOUND;
					m_pEventObjectStatus->m_TabPageControl->SelectedIndex = 6;
				}
			}
		}
		else
		if(l_pTabItem->Header->ToString()->Equals("Particle"))
		{
			if( m_pFatmingUtilitiesData->m_pParticle->m_iCurrentSelectDataType != -1 )
			{
				//System::Windows::Controls::ListBoxItem^l_pListBoxItem = (System::Windows::Controls::ListBoxItem^)m_pFatmingUtilities->m_Particle->m_Particle_listBox->SelectedItem;
				if( m_pFatmingUtilitiesData->m_pParticle->m_iCurrentSelectDataType == 1 )
				{
					//m_pShowObjectTextBox->Text = l_pListBoxItem->Content->ToString();
					m_pShowObjectTextBox->Text = DNCT::WcharToGcstring(m_pFatmingUtilitiesData->m_pParticle->m_pSelectedParticleEmitterGroup->GetName());
					m_eEventObjectType = eEOT_PARTICLE_GROUP;
				}
				else
				{
					//l_pListBoxItem = (System::Windows::Controls::ListBoxItem^)m_pFatmingUtilities->m_Particle->m_ParticleGroup_listBox->SelectedItem;
					//if( l_pListBoxItem )
					if( m_pFatmingUtilitiesData->m_pParticle->m_iCurrentSelectDataType == 0 )
					{
						//m_pShowObjectTextBox->Text = l_pListBoxItem->Content->ToString();
						m_pShowObjectTextBox->Text = DNCT::WcharToGcstring(m_pFatmingUtilitiesData->m_pParticle->m_pSelectedPrtEmitter->GetName());
						m_eEventObjectType = eEOT_PARTICLE;
					}
				}
				m_pEventObjectStatus->m_TabPageControl->SelectedIndex = 3;
			}
		}
		else
		if(l_pTabItem->Header->ToString()->Equals("XMLText"))
		{
			if( m_pFatmingUtilitiesData->m_pXMLText->m_pstrCurrentNode )
			{
				if( m_pFatmingUtilitiesData->m_pXMLText->m_pstrCurrentNode->length() )
				{
					m_pShowObjectTextBox->Text = DNCT::WcharToGcstring((WCHAR*)m_pFatmingUtilitiesData->m_pXMLText->m_pstrCurrentNode->c_str());
					this->m_pEventObjectStatus->m_TextText->Text = DNCT::WcharToGcstring((WCHAR*)m_pFatmingUtilitiesData->m_pXMLText->m_pstrCurrentText->c_str());
					m_eEventObjectType = eEOT_TEXT;
					m_pEventObjectStatus->m_TabPageControl->SelectedIndex = 5;
				}
			}
		}
		else
		if(l_pTabItem->Header->ToString()->Equals("Image"))
		{
			if( m_pFatmingUtilitiesData->m_pImage->GetCurrentImage() )
			{
				m_pShowObjectTextBox->Text = DNCT::WcharToGcstring(m_pFatmingUtilitiesData->m_pImage->GetCurrentImage()->GetName());
				m_eEventObjectType = eEOT_IMAGE;
				g_bSelecteImageIsPI = false;
				m_pEventObjectStatus->m_TabPageControl->SelectedIndex = 0;
			}
		}
	}

	System::Void EventObjectStatus::StatusApplyTest_button_Click(System::Object^  sender, System::EventArgs ^  e)
	{
		if( m_pShowObjectTextBox->Text->Length == 0 )
			return;
		Vector2	l_vSize;
		SAFE_DELETE(g_pEditorEventObject);
		UICombineRemove();
		switch(m_eEventObjectType)
		{
			case eEOT_XML_TEXT:
				 //= new cEVentXMLNodeText();
				WARNING_MSG("not support yet!");
				return;
				//if( this->m_pEventObjectStatus->m_TextText->Text->Length )
				{
					//g_pEditorEventObject = new cEventText(Vector3(0,0,0),DNCT::GcStringToWchar(this->m_pEventObjectStatus->m_TextText->Text));
				}
			break;
			case eEOT_TEXT:
				//WCHAR*e_strText,float e_fFontSize = 12
				//m_pFatmingUtilitiesData->m_pText->m_pstrCurrentText
				if( this->m_pEventObjectStatus->m_TextText->Text->Length )
				{
					g_pEditorEventObject = new cEventText(Vector3(0,0,0),DNCT::GcStringToWchar(this->m_pEventObjectStatus->m_TextText->Text));
				}
				break;
			case eEOT_IMAGE:
				if(g_bSelecteImageIsPI&&m_pFatmingUtilitiesData->m_pPI&&m_pFatmingUtilitiesData->m_pPI->m_pCurrentPIUnit)
				{
					l_vSize.x = (float)m_pFatmingUtilitiesData->m_pPI->m_pCurrentPIUnit->GetWidth();
					l_vSize.y = (float)m_pFatmingUtilitiesData->m_pPI->m_pCurrentPIUnit->GetHeight();
					g_pEditorEventObject = new cEventImage(Vector3(0,0,0),l_vSize,m_pFatmingUtilitiesData->m_pPI->m_pCurrentPIUnit);
				}
				else
				if( g_bSelecteImageIsPI == false )
				{
					cBaseImage*l_pImage = m_pFatmingUtilitiesData->m_pImage->GetCurrentImage();
					l_vSize.x = (float)l_pImage->GetWidth();
					l_vSize.y = (float)l_pImage->GetHeight();
					g_pEditorEventObject = new cEventImage(Vector3(0,0,0),l_vSize,l_pImage);
				}
				break;
			case eEOT_MPDI:
				if(m_pFatmingUtilitiesData->m_pMPDI->m_pCurrentMPDI)
				{
					l_vSize.x = 1.f;
					l_vSize.y = 1.f;
					m_pFatmingUtilitiesData->m_pMPDI->m_pCurrentMPDI->SetAnimationLoop(true);
					cEventMPDI*l_pEventMPDI = new cEventMPDI(Vector3(0,0,0),m_pFatmingUtilitiesData->m_pMPDI->m_pCurrentMPDI,true);
					g_pEditorEventObject = l_pEventMPDI;
					l_pEventMPDI->Init();
					//g_pEditorEventObject->Start();
					//m_pEventObjectStatus->m_MPDIClickToPlay->SetValue(l_pEventMPDI->IsClickToPlay());
				}
				break;
			case eEOT_SOUND:
				g_pEditorEventObject = new cEventSound(m_pFatmingUtilitiesData->m_pSound->m_pCurrentBasicSound);
				break;
			case eEOT_PARTICLE:
				g_pEditorEventObject = new cEventParticle(Vector3::Zero,m_pFatmingUtilitiesData->m_pParticle->m_pSelectedPrtEmitter);
				(dynamic_cast<cEventParticle*>(g_pEditorEventObject))->Init();
				break;
			case eEOT_PARTICLE_GROUP:
			{
				g_pEditorEventObject = new cEventParticleGroup(L"FileName",Vector3::Zero,m_pFatmingUtilitiesData->m_pParticle->m_pSelectedParticleEmitterGroup);
				(dynamic_cast<cEventParticleGroup*>(g_pEditorEventObject))->Init();
			}
				break;
			case eEOT_VARIABLE_RENDER:
				{				
					//g_pEditorEventObject = new cEventVariableDataRender(L"FileName",Vector3::Zero,m_pFatmingUtilitiesData->m_pParticle->m_pSelectedParticleEmitterGroup);
				}

				break;
		}
		UICombine(g_pEditorEventObject);
	}

	System::Void EventObjectStatus::AddEventObjectStatus_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(m_pStatusNameTextBox->Text->Length == 0)
			return;
		if( !m_pEditorEventObjectStatu )
		{
			m_pEditorEventObjectStatu = new cEventObjectStatus();
		}
		WCHAR*l_strStatusName = DNCT::GcStringToWchar(m_pStatusNameTextBox->Text);
		cObjectAndName<cEventObject>*l_pNewObject = m_pEditorEventObjectStatu->GetObject(l_strStatusName);
		if( !l_pNewObject )
		{
			if( !g_pEditorEventObject )
			{
				WARNING_MSG("please select a animation first!");
				return ;
			}
			l_pNewObject = new cObjectAndName<cEventObject>(l_strStatusName);
			cEventObject*l_pObject = dynamic_cast<cEventObject*>(g_pEditorEventObject->Clone());
			l_pNewObject->SetObject(l_pObject);
			m_pEditorEventObjectStatu->AddObject(l_pNewObject);
			m_pEventObjectStatus->m_Status_listBox->Items->Add(m_pStatusNameTextBox->Text);
		}
		else
		{
			WARNING_MSG("status already exist!");
		}
		UICombineRemove();
	}

	System::Void EventObjectStatus::DeleteEventObjectStatus_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if( !m_pEditorEventObjectStatu )
		{
			return ;
		}
		while(m_pEventObjectStatus->m_Status_listBox->SelectedItems->Count)
		{
			int	l_iSelectedIndex = m_pEventObjectStatus->m_Status_listBox->Items->IndexOf(m_pEventObjectStatus->m_Status_listBox->SelectedItems[0]);
			if( l_iSelectedIndex != -1 )
			{
				m_pEventObjectStatus->m_Status_listBox->Items->RemoveAt(l_iSelectedIndex);
				m_pEditorEventObjectStatu->RemoveObject(l_iSelectedIndex);
			}
		}
	}

	System::Void EventObjectStatus::RevertEventObjectStatus_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
		int	l_iSelectedIndex = m_pEventObjectStatus->m_Status_listBox->SelectedIndex;
		if( l_iSelectedIndex != -1 )
		{
		//here ensure u have rest the data u don't expect.
		//ex: if u are using image u have to set m_pEventObjectStatus->m_MPDIPosXYZNumeric->SetValue(0)
		//or it might crash.
			//reset all value as 0 avoid crash
			UICombineRemove();
			SAFE_DELETE(g_pEditorEventObject);
			g_pEditorEventObject = dynamic_cast<cEventObject*>(m_pEditorEventObjectStatu->GetObject(l_iSelectedIndex)->GetObject()->Clone());
			UICombine(g_pEditorEventObject);
		}
	}

	System::Void EventObjectStatus::EventObjectStatus_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{


	}


}