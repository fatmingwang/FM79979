#include "StdAfx.h"
#include "EventValueChangeActiver.h"

namespace FMBookEditor
{
	EventValueChangeActiver_Editor::EventValueChangeActiver_Editor(FMBookEditorUI::Event::Activer::EventValueChangeActiver^e_pEventValueChangeActiver)
	{
		m_pEventValueChangeActiver = new cEventValueChangeActiver((TiXmlElement*)0);
		sFMBookEditorData::m_spEventInstance->m_ActiveEventList.AddObjectNeglectExist(m_pEventValueChangeActiver);
		//sFMBookEditorData::m_spEventObjectInstance->Init();
		m_pEventValueChangeActiverUI = e_pEventValueChangeActiver;
	}

	EventValueChangeActiver_Editor::EventValueChangeActiver_Editor(FMBookEditorUI::Event::Activer::EventValueChangeActiver^e_pEventValueChangeActiver,cEventValueChangeActiver*e_pEventValueChangeActiver2)
	{
		m_pEventValueChangeActiver = dynamic_cast<cEventValueChangeActiver*>(e_pEventValueChangeActiver2->Clone());
		sFMBookEditorData::m_spEventInstance->m_ActiveEventList.AddObjectNeglectExist(m_pEventValueChangeActiver);
		m_pEventValueChangeActiverUI = e_pEventValueChangeActiver;

		cEventManager*l_pEventManager = g_pEditorMode->m_spEventManager;
		std::wstring*l_TargetString = m_pEventValueChangeActiver->GetTargetString();

		cEventVariable*l_pTargetEventVariable = m_pEventValueChangeActiver->GetTargetEventVariable();
		cRestrictValueBase*l_pRestrictValueBase = m_pEventValueChangeActiver->GetRestrictValue();

		cEventVariable*l_pEventVariable = m_pEventValueChangeActiver->GetEventVariable();
		m_pEventValueChangeActiverUI->m_EventVariable->Text = DNCT::WcharToGcstring(l_pEventVariable->GetName());

		int	l_iSelectedIndex = m_pEventValueChangeActiverUI->m_TabControl->SelectedIndex;
		if( l_TargetString )
		{
			m_pEventValueChangeActiverUI->m_TabControl->SelectedIndex = 0;
			m_pEventValueChangeActiverUI->m_Value_0->Text = DNCT::WcharToGcstring(l_TargetString->c_str());
			
		}
		else
		if( l_pTargetEventVariable )
		{
			m_pEventValueChangeActiverUI->m_TabControl->SelectedIndex = 1;
			m_pEventValueChangeActiverUI->m_TargetVariable->Text = DNCT::WcharToGcstring(l_pTargetEventVariable->GetName());
		}
		else
		if( l_pRestrictValueBase )
		{
			cRestrictValue<int>*l_pRestrictValue = (cRestrictValue<int>*)l_pRestrictValueBase;
			m_pEventValueChangeActiverUI->m_TabControl->SelectedIndex = 2;
			m_pEventValueChangeActiverUI->m_RestrictValue1->m_IncreaseValue->Value = (float)l_pRestrictValue->GetIncreaseValue();
			m_pEventValueChangeActiverUI->m_RestrictValue1->m_Max->Value = (float)l_pRestrictValue->GetMaximumValue();
			m_pEventValueChangeActiverUI->m_RestrictValue1->m_Min->Value = (float)l_pRestrictValue->GetMinimumValue();
			m_pEventValueChangeActiverUI->m_RestrictValue1->m_Type->SelectedIndex = (int)m_pEventValueChangeActiver->GeteValueeChangeType();
		}
	}
	//EventValueChangeActiver_Editor::EventValueChangeActiver(FMBookEditorUI::Event::Activer::cEventValueChangeActiver^e_pEventValueChangeActiver);
	EventValueChangeActiver_Editor::~EventValueChangeActiver_Editor()
	{
		int	l_iIndex = sFMBookEditorData::m_spEventInstance->m_ActiveEventList.GetObjectIndexByPointer(m_pEventValueChangeActiver);
		if( l_iIndex != -1 )
		{
			sFMBookEditorData::m_spEventInstance->m_ActiveEventList.RemoveObject(l_iIndex);
		}
		else
		//for multi button UI
		{
			SAFE_DELETE(m_pEventValueChangeActiver);
		}	
	}

	System::Void EventValueChangeActiver_Editor::Valdiate(System::Object^  sender, System::EventArgs^  e)
	{
	
	}
	System::String^ EventValueChangeActiver_Editor::IsLegal()
	{
		if( !m_pEventValueChangeActiver )
			return "no m_pEventValueChangeActiver";
		std::wstring*l_TargetString = 0;
		l_TargetString = m_pEventValueChangeActiver->GetTargetString();
		cEventVariable*l_pEventVariable = 0;
		String^l_strEventVariable = m_pEventValueChangeActiverUI->m_EventVariable->Text;
		m_pEventValueChangeActiver->SetEventVariable(0);
		m_pEventValueChangeActiver->SetTargetEventVariable(0);
		cRestrictValueBase*l_pRestrictValueBase = m_pEventValueChangeActiver->GetRestrictValue();
		if( l_pRestrictValueBase )
		{
			SAFE_DELETE(l_pRestrictValueBase);
		}
		if( l_TargetString )
		{
			SAFE_DELETE(l_TargetString);
		}
		//m_pEventValueChangeActiver->SeteValueeChangeType((eValueeChangeType)m_pEventValueChangeActiverUI->m_Type->SelectedIndex);
		int	l_iSelectedIndex = m_pEventValueChangeActiverUI->m_TabControl->SelectedIndex;
		cEventManager*l_pEventManager = g_pEditorMode->m_spEventManager;
		String^l_strErrorMsg;
		switch( l_iSelectedIndex )
		{
			case 0:
			{
				String^l_strValue = m_pEventValueChangeActiverUI->m_Value_0->Text;
				if( l_strValue->Length == 0 )
					l_strErrorMsg = "there is no data for event variable";
				else
				{
					if( !l_TargetString )
					{
						l_TargetString = new std::wstring;
						m_pEventValueChangeActiver->SetTargetString(l_TargetString);
					}
					*l_TargetString = DNCT::GcStringToWchar(l_strValue);
				}
				
			}
				break;
			case 1:
			{
				String^l_strTargetEventVariable = m_pEventValueChangeActiverUI->m_TargetVariable->Text;
				if( l_strTargetEventVariable )
				{
					cEventVariable*l_pTargetEventVariable = l_pEventManager->m_pEventVariableManager->GetObject(DNCT::GcStringToWchar(l_strTargetEventVariable));
					if( !l_pTargetEventVariable )
					{
						l_strErrorMsg = "there is no such event variable "+l_strTargetEventVariable;
					}
					else
					{
						m_pEventValueChangeActiver->SetTargetEventVariable(l_pTargetEventVariable);
					}
				}
				else
				{
					l_strErrorMsg = "there is no target event variable";
				}
				
			}
				break;
			case 2:
			{
				eValueeChangeType	l_iSelectedIndex = (eValueeChangeType)m_pEventValueChangeActiverUI->m_RestrictValue1->m_Type->SelectedIndex;
				if( l_iSelectedIndex == eCVT_TARGET_VALUE || l_iSelectedIndex == eCVT_RANDOM_VALUE )
				{
					l_strErrorMsg = "please select increase or decrease!";
				}
				else
				{
					cRestrictValue<int>*l_pRestrictValue = new cRestrictValue<int>;
					float	l_fMax = (float)m_pEventValueChangeActiverUI->m_RestrictValue1->m_Max->Value;
					float	l_fMin = (float)m_pEventValueChangeActiverUI->m_RestrictValue1->m_Min->Value;
					float	l_fIncrease = (float)m_pEventValueChangeActiverUI->m_RestrictValue1->m_IncreaseValue->Value;
					l_pRestrictValue->SetMaximumValue((int)l_fMax);
					l_pRestrictValue->SetMinimumValue((int)l_fMin);
					l_pRestrictValue->SetIncreaseValue((int)l_fIncrease);
					m_pEventValueChangeActiver->SetRestrictValue(l_pRestrictValue);
				}
			}
				break;
		}
		if( l_strEventVariable )
		{
			l_pEventVariable = l_pEventManager->m_pEventVariableManager->GetObject(DNCT::GcStringToWchar(l_strEventVariable));
			if( !l_pEventVariable )
			{
				l_strErrorMsg = "there is no such event variable "+l_strEventVariable;
			}
			else
				m_pEventValueChangeActiver->SetEventVariable(l_pEventVariable);
		}
		else
		{
			l_strErrorMsg = "there is no event variable";
		}
		return l_strErrorMsg;
	}
//end
}