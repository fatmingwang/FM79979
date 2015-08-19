#ifndef _TIME_WITH_VECTOR_DATA_APPLY_H_
#define _TIME_WITH_VECTOR_DATA_APPLY_H_

namespace DNCT
{
	template<class T,class UI_COMPONENT>ref  class	cTimeWithVectorDataApply abstract
	{
		System::String^				GetDataStringByIndex(int e_iIndex)
		{
			if( e_iIndex != -1 && m_pTimeWithVector->m_DataListBox->Items->Count > e_iIndex)
			{
				float*	l_pTime = m_pData->GetTime(e_iIndex);
				T*	l_pData = m_pData->GetData(e_iIndex);
				System::String^l_str = e_iIndex.ToString()+":Time:"+l_pTime->ToString()+",Data:"+String(ValueToString(*l_pData).c_str()).ToString();
				return l_str;
			}
			return nullptr;
		}
		CoreWPF::Common::TimeWithVector^m_pTimeWithVector;
		cLinerDataProcessor<T>*			m_pData;
		UI_COMPONENT^					m_pUIData;
		private: System::Void f_AddClick(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pUIData->GetValue() && m_pTimeWithVector->m_Time->GetValue() )
			{
				float	l_fTime = (float)m_pTimeWithVector->m_Time->Value;
				T	l_Data = *(T*)m_pUIData->GetValue();
				m_pData->AddData(l_Data,l_fTime);
				System::String^l_str = m_pTimeWithVector->m_DataListBox->Items->Count.ToString()+":Time:"+l_fTime.ToString()+",Data:"+String(ValueToString(l_Data).c_str()).ToString();
				m_pTimeWithVector->m_DataListBox->Items->Add(l_str);
			}
		}

		private: System::Void f_DeleteClick(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pTimeWithVector->m_DataListBox->SelectedIndex != -1)
			{
				while(m_pTimeWithVector->m_DataListBox->SelectedItems->Count)
				{
					int l_iIndex = (int)m_pTimeWithVector->m_DataListBox->Items->IndexOf(m_pTimeWithVector->m_DataListBox->SelectedItems[0]);
					m_pData->RemoveDtaa(l_iIndex);
					m_pTimeWithVector->m_DataListBox->Items->RemoveAt(l_iIndex);
				}
				for(int i=0;i<m_pTimeWithVector->m_DataListBox->Items->Count;++i)
				{
					m_pTimeWithVector->m_DataListBox->Items[i] = GetDataStringByIndex(i);
				}
			}
		}
		private: System::Void f_RevertClick(System::Object^  sender, System::EventArgs^  e)
		{
			//if( m_pTimeWithVector->m_DataListBox->SelectedIndex != -1)
			//{
			//	int l_iIndex = (int)m_pTimeWithVector->m_DataListBox->Items->IndexOf(m_pTimeWithVector->m_DataListBox->SelectedItems[0]);
			//	m_pTimeWithVector->m_DataListBox->Items->RemoveAt(l_iIndex);
			//}
		}
		private: System::Void f_InevertClick(System::Object^  sender, System::EventArgs^  e)
		{
			m_pData->InvertOrder();
			for(int i=0;i<m_pTimeWithVector->m_DataListBox->Items->Count;++i)
			{
				m_pTimeWithVector->m_DataListBox->Items[i] = GetDataStringByIndex(i);
			}
		}
		private: System::Void f_TimeChanged(System::Object^  sender, System::EventArgs^  e)
		{

		}

		private: System::Void f_SelectionChanged(System::Object^  sender, System::EventArgs^  e)
		{
			if( m_pTimeWithVector->m_DataListBox->SelectedIndex != -1)
			{
				float*	l_pData = (float*)m_pData->GetData(m_pTimeWithVector->m_DataListBox->SelectedIndex);
				if( l_pData )
					m_pUIData->SetValue(l_pData);
			}
			else
			{
				m_pUIData->SetValue(0);
			}
		}
		private: System::Void f_LinerCheckboxChecked(System::Object^  sender, System::EventArgs^  e)
		{

		}
		private: System::Void f_AverageAssignCheckboxChecked(System::Object^  sender, System::EventArgs^  e)
		{

		}
		private: System::Void f_NumericUpDownValueChanged(System::Object^  sender, System::EventArgs^  e)
		{

		}
		public:virtual System::Void f_DataValueChanged(System::Object^  sender, System::EventArgs^  e) 
		{
			//if( m_pTimeWithVector->m_LinerCheckBox->Checked )
			{
			
			}
		}

	public:
		cTimeWithVectorDataApply(CoreWPF::Common::TimeWithVector^e_pTimeWithVector,cLinerDataProcessor<T>*e_pData)
		{
			m_pTimeWithVector = e_pTimeWithVector;
			m_pData = e_pData;
			m_pUIData = (UI_COMPONENT^)m_pTimeWithVector->m_DataObject;
			m_pTimeWithVector->f_AddClick += gcnew System::EventHandler(this, &cTimeWithVectorDataApply::f_AddClick);
			m_pTimeWithVector->f_DeleteClick += gcnew System::EventHandler(this, &cTimeWithVectorDataApply::f_DeleteClick);
			m_pTimeWithVector->f_RevertClick += gcnew System::EventHandler(this, &cTimeWithVectorDataApply::f_RevertClick);
			m_pTimeWithVector->f_InevertClick += gcnew System::EventHandler(this, &cTimeWithVectorDataApply::f_InevertClick);
			m_pTimeWithVector->f_TimeChanged += gcnew System::EventHandler(this, &cTimeWithVectorDataApply::f_TimeChanged);
			m_pTimeWithVector->f_SelectionChanged += gcnew System::EventHandler(this, &cTimeWithVectorDataApply::f_SelectionChanged);
			m_pTimeWithVector->f_LinerCheckboxChecked += gcnew System::EventHandler(this, &cTimeWithVectorDataApply::f_LinerCheckboxChecked);
			m_pTimeWithVector->f_AverageAssignCheckboxChecked += gcnew System::EventHandler(this, &cTimeWithVectorDataApply::f_AverageAssignCheckboxChecked);
			//m_pTimeWithVector->f_NumericUpDownValueChanged += gcnew System::EventHandler(this, &cTimeWithVectorDataApply::f_NumericUpDownValueChanged);
			m_pUIData->f_ValueChanged += gcnew System::EventHandler(this, &cTimeWithVectorDataApply::f_DataValueChanged);
			m_pUIData->SetValue(0);
		}
		~cTimeWithVectorDataApply()
		{

		}
	};


	ref class	cTimeWithVector3:public cTimeWithVectorDataApply<Vector3,CoreWPF::Common::XYZNumeric>
	{
		public:virtual	System::Void f_DataValueChanged(System::Object^  sender, System::EventArgs^  e) override
		{
		
		}
	public:
		cTimeWithVector3(CoreWPF::Common::TimeWithVector^e_pTimeWithVector,cLinerDataProcessor<Vector3>*e_pData):cTimeWithVectorDataApply(e_pTimeWithVector,e_pData)
		{
		}
		~cTimeWithVector3()
		{
		}
	};

	ref class	cTimeWithVector4:public cTimeWithVectorDataApply<Vector4,CoreWPF::Common::XYZWNumeric>
	{
		public:virtual	System::Void f_DataValueChanged(System::Object^  sender, System::EventArgs^  e) override
		{
		
		}
	public:
		cTimeWithVector4(CoreWPF::Common::TimeWithVector^e_pTimeWithVector,cLinerDataProcessor<Vector4>*e_pData):cTimeWithVectorDataApply(e_pTimeWithVector,e_pData)
		{
		}
		~cTimeWithVector4()
		{
		}
	};
}

#endif