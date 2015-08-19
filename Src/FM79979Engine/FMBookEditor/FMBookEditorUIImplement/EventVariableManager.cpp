#include "StdAfx.h"
#include "EventVariableManager.h"

namespace FMBookEditor
{
	System::Void cEventVariableManager_Editor::Add(System::Object^  sender, System::EventArgs^  e)
	{
		String^l_strValue = m_pEventVariableManager->m_VariableValue->Text;
		String^l_strName = m_pEventVariableManager->m_VariableName->Text;
		int	l_iSelectedIndex = m_pEventVariableManager->m_TypeComboBox->SelectedIndex;
		if( l_strName->Length && l_strValue->Length && l_iSelectedIndex!= -1 )
		{
			//m_pEventVariableManager->m_EventVariable_listBox;
			std::wstring	l_strVariableName = DNCT::GcStringToWchar(l_strName);
			cEventVariable*l_pEventVariable = g_pEditorMode->m_spEventManager->m_pEventVariableManager->GetObject(l_strVariableName.c_str());
			if( l_pEventVariable )
			{
				WARING_YES_NO_TO_NO("overwrite?")
					return;
			}
			if( l_pEventVariable )
				l_pEventVariable->SetDataByString((WCHAR*)DNCT::GcStringToWchar(l_strValue));
			else
			{
				l_pEventVariable = new cEventVariable((WCHAR*)l_strVariableName.c_str(),(eDataType)l_iSelectedIndex,(WCHAR*)DNCT::GcStringToWchar(l_strValue));
				g_pEditorMode->m_spEventManager->m_pEventVariableManager->AddObject(l_pEventVariable);
				m_pEventVariableManager->m_EventVariable_listBox->Items->Add(l_strName);
			}
			//l_pEventVariable->SetName(l_strVariableName);
			//l_pEventVariable->SetVariableDataType((eDataType)l_iSelectedIndex);
			//l_pEventVariable->SetDataByString(DNCT::GcStringToWchar(l_strValue),(eDataType)l_iSelectedIndex);
		}
	}

	System::Void cEventVariableManager_Editor::Del(System::Object^  sender, System::EventArgs^  e)
	{
		if( m_pEventVariableManager->m_EventVariable_listBox->SelectedIndex != -1 )
		{
			//check all event does't use this variable then remove it.
			g_pEditorMode->m_spEventManager->m_pEventVariableManager->RemoveObject(m_pEventVariableManager->m_EventVariable_listBox->SelectedIndex);
			m_pEventVariableManager->m_EventVariable_listBox->Items->RemoveAt(m_pEventVariableManager->m_EventVariable_listBox->SelectedIndex);
		}			
	}

	System::Void cEventVariableManager_Editor::Revert(System::Object^  sender, System::EventArgs^  e)
	{
		if( m_pEventVariableManager->m_EventVariable_listBox->SelectedIndex != -1 )
		{
			cEventVariable*l_pEventVariable = g_pEditorMode->m_spEventManager->m_pEventVariableManager->GetObject(m_pEventVariableManager->m_EventVariable_listBox->SelectedIndex);
			m_pEventVariableManager->m_VariableName->Text = DNCT::WcharToGcstring(l_pEventVariable->GetName());
			m_pEventVariableManager->m_VariableValue->Text = DNCT::WcharToGcstring((WCHAR*)l_pEventVariable->ConvertToString().c_str());
			m_pEventVariableManager->m_TypeComboBox->SelectedIndex = (int)l_pEventVariable->GetDataType();
		}
	}

	void	cEventVariableManager_Editor::Refresh()
	{
		m_pEventVariableManager->m_EventVariable_listBox->Items->Clear();
		int	l_iCount = g_pEditorMode->m_spEventManager->m_pEventVariableManager->Count();
		for( int i=0;i<l_iCount;++i )
		{
			cEventVariable*l_pEventVariable = g_pEditorMode->m_spEventManager->m_pEventVariableManager->GetObject(i);
			m_pEventVariableManager->m_EventVariable_listBox->Items->Add(DNCT::WcharToGcstring(l_pEventVariable->GetName()));
		}
	}
}