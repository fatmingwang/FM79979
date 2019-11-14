#include "StdAfx.h"
#include "EventButtonSelectedActiver.h"

namespace FMBookEditor
{
	EventButtonSelectedActiver_Editor::EventButtonSelectedActiver_Editor(FMBookEditorUI::Event::Activer::EventButtonSelectedActiver^e_pEventButtonSelectedActiver)
	{
		m_pEventButtonSelectedActiver = new cEventButtonSelectedActiver((TiXmlElement*)0);
		sFMBookEditorData::m_spEventInstance->m_ActiveEventList.AddObjectNeglectExist(m_pEventButtonSelectedActiver);
		m_pEventButtonSelectedActiverUI = e_pEventButtonSelectedActiver;
		m_pEventButtonSelectedActiverUI->f_AddResult += gcnew EventHandler(this,&EventButtonSelectedActiver_Editor::AddResult);
        m_pEventButtonSelectedActiverUI->f_DelResult += gcnew EventHandler(this,&EventButtonSelectedActiver_Editor::DelResult);
	}

	EventButtonSelectedActiver_Editor::EventButtonSelectedActiver_Editor(FMBookEditorUI::Event::Activer::EventButtonSelectedActiver^e_pEventButtonSelectedActiver,cEventButtonSelectedActiver*e_pEventButtonSelectedActiver2)
	{
		m_pEventButtonSelectedActiver = dynamic_cast<cEventButtonSelectedActiver*>(e_pEventButtonSelectedActiver2->Clone());
		sFMBookEditorData::m_spEventInstance->m_ActiveEventList.AddObjectNeglectExist(m_pEventButtonSelectedActiver);
		m_pEventButtonSelectedActiverUI = e_pEventButtonSelectedActiver;
		m_pEventButtonSelectedActiverUI->f_AddResult += gcnew EventHandler(this,&EventButtonSelectedActiver_Editor::AddResult);
        m_pEventButtonSelectedActiverUI->f_DelResult += gcnew EventHandler(this,&EventButtonSelectedActiver_Editor::DelResult);
		if(m_pEventButtonSelectedActiver->GetEventVariable())
		{
			m_pEventButtonSelectedActiverUI->m_EventVariableName->Text = WcharToGcstring(m_pEventButtonSelectedActiver->GetEventVariable()->GetName());
		}
		for( size_t i=0;i<m_pEventButtonSelectedActiver->GetResult()->size();++i )
		{
			m_pEventButtonSelectedActiverUI->m_ResultListBox->Items->Add( WcharToGcstring((*m_pEventButtonSelectedActiver->GetResult())[i].c_str())  );
		}
	}

	EventButtonSelectedActiver_Editor::~EventButtonSelectedActiver_Editor()
	{
		int	l_iIndex = sFMBookEditorData::m_spEventInstance->m_ActiveEventList.GetObjectIndexByPointer(m_pEventButtonSelectedActiver);
		if( l_iIndex != -1 )
		{
			sFMBookEditorData::m_spEventInstance->m_ActiveEventList.RemoveObject(l_iIndex);
		}
		else
		//for multi button UI
		{
			SAFE_DELETE(m_pEventButtonSelectedActiver);
		}
	}

	System::Void EventButtonSelectedActiver_Editor::Valdiate(System::Object^  sender, System::EventArgs^  e)
	{
		cEventManager*l_pEventManager = g_pEditorMode->m_spEventManager;
		if(m_pEventButtonSelectedActiverUI->m_EventVariableName->Text->Length)
		{
			auto l_strName = DNCT::GcStringToWchar(m_pEventButtonSelectedActiverUI->m_EventVariableName->Text);
			m_pEventButtonSelectedActiver->SetEventVariable(l_pEventManager->m_pEventVariableManager->GetObject(l_strName.c_str()));
		}
	}
	System::String^ EventButtonSelectedActiver_Editor::IsLegal()
	{
		String^l_strError;
		m_pEventButtonSelectedActiver->SetEventVariable(0);
		if(m_pEventButtonSelectedActiverUI->m_EventVariableName->Text->Length == 0)
		{
			l_strError += "no EventVariableName";
		}
		else
		{
			auto l_strEventVariableName = DNCT::GcStringToWchar(m_pEventButtonSelectedActiverUI->m_EventVariableName->Text);
			cEventVariable*l_pEventVariable = g_pEditorMode->m_spEventManager->m_pEventVariableManager->GetObject(l_strEventVariableName.c_str());
			if( !l_pEventVariable )
			{
				l_strError += m_pEventButtonSelectedActiverUI->m_EventVariableName->Text;
				l_strError += "not find in EventVariables";
			}
			else
			{
				m_pEventButtonSelectedActiver->SetEventVariable(l_pEventVariable);
			}
		}
		if(m_pEventButtonSelectedActiverUI->m_ResultListBox->Items->Count == 0)
		{
			l_strError += "no Result";
		}
		return l_strError;
	}

	System::Void EventButtonSelectedActiver_Editor::AddResult(System::Object^  sender, System::EventArgs^  e)
	{
		if(this->m_pEventButtonSelectedActiverUI->m_Result->Text->Length)
		{
			this->m_pEventButtonSelectedActiverUI->m_ResultListBox->Items->Add(m_pEventButtonSelectedActiverUI->m_Result->Text);
			std::wstring l_str = DNCT::GcStringToWchar(m_pEventButtonSelectedActiverUI->m_Result->Text);
			m_pEventButtonSelectedActiver->GetResult()->push_back(l_str);
		}
	}

	System::Void EventButtonSelectedActiver_Editor::DelResult(System::Object^  sender, System::EventArgs^  e)
	{
		while(this->m_pEventButtonSelectedActiverUI->m_ResultListBox->SelectedItems->Count)
		{
			int	l_iIndex =  m_pEventButtonSelectedActiverUI->m_ResultListBox->Items->IndexOf(this->m_pEventButtonSelectedActiverUI->m_ResultListBox->SelectedItems[0]);
			m_pEventButtonSelectedActiverUI->m_ResultListBox->Items->RemoveAt(l_iIndex );
			//m_pEventButtonSelectedActiverUI->m_ResultListBox->SelectedItems->RemoveAt(0);
			m_pEventButtonSelectedActiver->GetResult()->erase(m_pEventButtonSelectedActiver->GetResult()->begin()+l_iIndex);
		}
	}
//end
}