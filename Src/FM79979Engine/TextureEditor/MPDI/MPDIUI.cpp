#include "StdAfx.h"
#include "MPDIUI.h"
#include "MPDI.h"
namespace MPDI
{
	String^CueToStartCurveWithTimeDataCheck(cCueToStartCurveWithTime*e_pCueToStartCurveWithTime)
	{
		String^l_strErrorMsg;
		cCueToStartCurveWithTime*l_pCueToStartCurvesWithTime = e_pCueToStartCurveWithTime;
		for( UINT i=0;i<l_pCueToStartCurvesWithTime->GetOriginalPointList().size();++i )
		{
			if( (*l_pCueToStartCurvesWithTime->GetPointDataList())[i]->iImageIndex == -1 )
			{
				l_strErrorMsg += DNCT::WcharToGcstring(l_pCueToStartCurvesWithTime->GetName())+" image index is empty!!"+DNCT::GetChanglineString();
			}
			if( i == 0 )
			{
				float   l_fTime = l_pCueToStartCurvesWithTime->GetOriginalTimeList()[0];
				//if( l_fTime > 0.f)
				{
				//	l_strErrorMsg += DNCT::WcharToGcstring(l_pCueToStartCurvesWithTime->GetName())+" first point time must be 0!!"+DNCT::GetChanglineString();
				}
			}
			else
			{
				if( (l_pCueToStartCurvesWithTime->GetOriginalTimeList()[i] - l_pCueToStartCurvesWithTime->GetOriginalTimeList()[i-1]) <= 0.f )
				{
					l_strErrorMsg += String(l_pCueToStartCurvesWithTime->GetCharName().c_str()).ToString()+" index"+i.ToString()+"is error,later index point time must bigger than early one!"+DNCT::GetChanglineString();
				}
			}
			if( i == l_pCueToStartCurvesWithTime->GetOriginalPointList().size()-1 )
			{
				float   l_fTime = l_pCueToStartCurvesWithTime->GetOriginalTimeList()[i];
				if( l_fTime <= 0.f)
				{
					l_strErrorMsg += DNCT::WcharToGcstring(l_pCueToStartCurvesWithTime->GetName())+" last point time can not be 0!!"+DNCT::GetChanglineString();
				}
			}
		}
		return l_strErrorMsg;
	}

	String^MultiPathDynamicImageDataCheck(cMultiPathDynamicImage*e_pMultiPathDynamicImage)
	{
		String^l_strErrorMsg;
		for(int j=0;j<e_pMultiPathDynamicImage->Count();++j)
		{
			l_strErrorMsg += CueToStartCurveWithTimeDataCheck((*e_pMultiPathDynamicImage)[j]);
		}
		return l_strErrorMsg;
	}

	String^MPDIListDataCheck(cMPDIList*e_pMPDIList)
	{
		String^l_strErrorMsg;
		for(int i=0;i<e_pMPDIList->Count();++i)
		{
			cMultiPathDynamicImage*l_pMultiPathDynamicImage = (cMultiPathDynamicImage*)(*e_pMPDIList)[i];
			l_strErrorMsg += MultiPathDynamicImageDataCheck(l_pMultiPathDynamicImage);
		}
		return l_strErrorMsg;
	}

	eAnimationEvent	IntToAnimationEvent(int e_iIndex)
	{
		switch(e_iIndex)
		{
			case 0:
				return eAE_NONE;
			case 1:
				return eAE_SOUND;
			case 2:
				return eAE_PARTICLE;
		}
		//it'd not be here.
		return eAE_MAX;
	}

	char*	eAnimationEventToAnimationEventString(eAnimationEvent e_eAnimationEvent)
	{
		switch(e_eAnimationEvent)
		{
			case eAE_NONE:
				return "None";
			case eAE_PARTICLE:
				return "Particle";
			case eAE_SOUND:
				return "Sound";
		}
		//it'd not be here.
		return "Max";
	}

	void	RenderHintPoint(cCueToStartCurveWithTime*e_pObject,bool e_bShowBorder)
	{
		if( e_pObject )
		{
			if( e_pObject->GetOriginalPointList().size() )
			{
				std::vector<sTexBehaviorDataWithImageIndexData*>*l_pPointList = e_pObject->GetPointDataList();
				cPuzzleImage*l_pPI = 0;
				for(UINT i=0;i<e_pObject->GetOriginalPointList().size();++i)
				{
					sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = (*l_pPointList)[i];
					l_pTexBehaviorDataWithImageIndexData->DumpData(e_pObject->GetCurrentPointData());
					e_pObject->SetCurrentPosition(e_pObject->GetOriginalPointList()[i]);
					if( l_pPI != l_pTexBehaviorDataWithImageIndexData->pPI )
					{
						l_pPI = l_pTexBehaviorDataWithImageIndexData->pPI;
					}
					e_pObject->Render(l_pPI->GetPuzzleData(l_pTexBehaviorDataWithImageIndexData->iImageIndex));
				}
				if( e_bShowBorder )
				{
					cCueToStartCurveWithTime	l_cCueToStartCurveWithTime(e_pObject);
					l_cCueToStartCurveWithTime.Init();
					for(UINT i=0;i<e_pObject->GetOriginalPointList().size();++i)
					{
						float	l_fTime = e_pObject->GetOriginalTimeList()[i];
						l_cCueToStartCurveWithTime.Update(l_fTime);
						l_cCueToStartCurveWithTime.RenderCollide();
					}
				}
			}
		}	
	}

	void	MPDIEditor::UIInitialize()
	{
		LanguageSwitch(this->Controls,"/",this->Handle);
		LanguageSwitch(Path_tabPage->Controls,"/",this->Handle);
		LanguageSwitch(this->splitContainer1->Controls,"/",this->Handle);
		LanguageSwitch(AnimationData_collpaseForm->Controls,"/",this->Handle);
		LanguageSwitch(Path_groupBox->Controls,"/",this->Handle);
		LanguageSwitch(ImageHintPoint_groupBox->Controls,"/",this->Handle);
		LanguageSwitch(HintPoint_groupBox->Controls,"/",this->Handle);

		m_pMPDIUI->SelectEvenPoints_button->Click += gcnew System::EventHandler(this, &MPDIEditor::SelectAllPoints_button_Click);
		m_pMPDIUI->TotalPlayTime_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::TotalPlayTime_numericUpDown_ValueChanged);
		m_pMPDIUI->RearrangeWholePathTime_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::RearrangeWholePathTime_numericUpDown_ValueChanged);
		m_pMPDIUI->PathNodeTime_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
		m_pMPDIUI->StartTime_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
		m_pMPDIUI->PathLOD_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::PathLOD_numericUpDown_ValueChanged);
		m_pMPDIUI->WholePath_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MPDIEditor::WholePath_listBox_SelectedIndexChanged);
		m_pMPDIUI->AngleY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
		m_pMPDIUI->AngleX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
		m_pMPDIUI->AngleZ_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
		m_pMPDIUI->WholePuzzleImageUnit_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MPDIEditor::WholePuzzleImageUnit_listBox_SelectedIndexChanged);
		m_pMPDIUI->Loop_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MPDIEditor::Play_checkBox_CheckedChanged);
		m_pMPDIUI->AssignImage_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AssignImage_button_Click);
		m_pMPDIUI->EnableColorBlending_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MPDIEditor::EnableColorBlending_checkBox_CheckedChanged);
		m_pMPDIUI->SrcBlending_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MPDIEditor::SrcBlending_comboBox_SelectedIndexChanged);
		m_pMPDIUI->DestBlending_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MPDIEditor::DestBlending_comboBox_SelectedIndexChanged);
		m_pMPDIUI->SrcBlending_comboBox->DropDownStyle = ComboBoxStyle::DropDownList;
		m_pMPDIUI->DestBlending_comboBox->DropDownStyle = ComboBoxStyle::DropDownList;
		m_pMPDIUI->InsertPoint_button->Click += gcnew System::EventHandler(this, &MPDIEditor::DelPoint_button_Click);
		m_pMPDIUI->InvertAllPath_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddPath_button_Click);
		AllPlay_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MPDIEditor::Play_checkBox_CheckedChanged);
		m_pMPDIUI->SelectAllPoints_button->Click += gcnew System::EventHandler(this, &MPDIEditor::SelectAllPoints_button_Click);
		m_pMPDIUI->DelPI_button->Click += gcnew System::EventHandler(this, &MPDIEditor::DelPI_button_Click);
		CurrentProgress_trackBar->Scroll += gcnew System::EventHandler(this, &MPDIEditor::CurrentProgress_trackBar_Scroll);
		CurrentProgress_trackBar->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MPDIEditor::CurrentProgress_trackBar_MouseDown);
		m_pMPDIUI->DelPoint_button->Click += gcnew System::EventHandler(this, &MPDIEditor::DelPoint_button_Click);
		m_pMPDIUI->ImageAnchor_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MPDIEditor::ImageAnchor_comboBox_SelectedIndexChanged);
		AllCurveLoop_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MPDIEditor::Play_checkBox_CheckedChanged);
		m_pMPDIUI->AddPath_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddPath_button_Click);
		m_pMPDIUI->UseViewPort_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MPDIEditor::UseViewPort_checkBox_CheckedChanged);
		m_pMPDIUI->DelPath_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddPath_button_Click);
		m_pMPDIUI->SelectFirstAndLast_button->Click += gcnew System::EventHandler(this, &MPDIEditor::SelectFirstAndLast_button_Click);
		m_pMPDIUI->AllPI_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MPDIEditor::AllPI_listBox_SelectedIndexChanged);
		m_pMPDIUI->Play_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MPDIEditor::Play_checkBox_CheckedChanged); 
		m_pMPDIUI->PathNode_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MPDIEditor::WholePath_listBox_SelectedIndexChanged);
		m_pMPDIUI->Width_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
		m_pMPDIUI->Height_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
		m_pMPDIUI->SizeScale_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
		m_pMPDIUI->ClonePath_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddPath_button_Click);
		m_pMPDIUI->RedColor_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
		m_pMPDIUI->GreenColor_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
		m_pMPDIUI->AlphaColor_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
		m_pMPDIUI->BlueColor_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
		//don't need this.
		//m_pMPDIUI->SubMPDI_StayAtLastFrame_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MPDIEditor::Play_checkBox_CheckedChanged);
		m_pMPDIUI->SubMPDI_StayAtLastFrame_checkBox->Visible = false;
		m_pMPDIUI->InvertPath_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddPath_button_Click);
		m_pMPDIUI->PathNodePosX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
		m_pMPDIUI->PathNodePosY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
		m_pMPDIUI->PathNodePosZ_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
		m_pMPDIUI->FadeIn_toolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::FadeIn_toolStripMenuItem_Click);
		m_pMPDIUI->FadeOut_toolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::FadeIn_toolStripMenuItem_Click);
		m_pMPDIUI->toolStripMenuItem1->Click += gcnew System::EventHandler(this, &MPDIEditor::toolStripMenuItem1_Click);
		m_pMPDIUI->addPointDataWithImageByPathNameToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::addPointDataWithImageByPathNameToolStripMenuItem_Click);
		m_pMPDIUI->AddSliderBarUI_toolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::AddSliderBarUI_toolStripMenuItem_Click);
		m_pMPDIUI->AddMultiPageUI_toolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::AddSliderBarUI_toolStripMenuItem_Click);
		m_pMPDIUI->addSliderBarWithTabPagesUIToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::AddSliderBarUI_toolStripMenuItem_Click);
		m_pMPDIUI->addMultipageWithTabPagesUIToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::AddSliderBarUI_toolStripMenuItem_Click);
		m_pMPDIUI->yesNoDialogToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::AddSliderBarUI_toolStripMenuItem_Click);

		m_pMPDIUI->WholePath_listBox->ContextMenuStrip = m_pMPDIUI->GameUIData_contextMenuStrip;

		m_pMPDIUI->WholePuzzleImageUnit_listBox->ContextMenuStrip = m_pMPDIUI->PathGroup_contextMenuStrip;
		m_pMPDIUI->PathNode_listBox->ContextMenuStrip = m_pMPDIUI->PointDataUtility_contextMenuStrip;
		m_pMPDIUI->BehaviorUpdateActive_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MPDIEditor::BehaviorUpdateActive_checkBox_CheckedChanged);
		m_pMPDIUI->PathPiorityUp_button->Click += gcnew System::EventHandler(this, &MPDIEditor::PathPiorityUp_button_Click);
		m_pMPDIUI->PathPiorityDown_button->Click += gcnew System::EventHandler(this, &MPDIEditor::PathPiorityUp_button_Click);
		m_pMPDIUI->ApplyLODToPoints_button->Click += gcnew System::EventHandler(this, &MPDIEditor::ApplyLODToPoints_button_Click);

		this->flowLayoutPanel1->Controls->Add(this->MPDIExtra_xmlNodeEditComponent);
		this->MPDIExtra_xmlNodeEditComponent->AllowDrop = true;
		this->MPDIExtra_xmlNodeEditComponent->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
			static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
		this->MPDIExtra_xmlNodeEditComponent->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
		this->MPDIExtra_xmlNodeEditComponent->CollpaseButtonName = L"XMLData";
		this->MPDIExtra_xmlNodeEditComponent->CollpaseXMlElementValuePanel = false;
		this->MPDIExtra_xmlNodeEditComponent->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
			static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
		this->MPDIExtra_xmlNodeEditComponent->Location = System::Drawing::Point(2, 639);
		this->MPDIExtra_xmlNodeEditComponent->Margin = System::Windows::Forms::Padding(2);
		this->MPDIExtra_xmlNodeEditComponent->Name = L"MPDIExtra_xmlNodeEditComponent";
		this->MPDIExtra_xmlNodeEditComponent->Size = System::Drawing::Size(260, 288);
		this->MPDIExtra_xmlNodeEditComponent->TabIndex = 5;
	}

	System::Void MPDIUI::ColorChanged(System::Object^  sender, System::EventArgs^  e)
	{
		CoreWPF::Common::ColorPicker^l_pColorPicker = m_pColorPicker;
		RedColor_numericUpDown->Value =  (System::Decimal)(1+(l_pColorPicker->GetSelectedColor().R-128)/128.f);
		GreenColor_numericUpDown->Value = (System::Decimal)(1+(l_pColorPicker->GetSelectedColor().G-128)/128.f);
		BlueColor_numericUpDown->Value = (System::Decimal)(1+(l_pColorPicker->GetSelectedColor().B-128)/128.f);
		//if(!m_pColorPicker->m_ColorPicker->ShowStandardColors)
			//AlphaColor_numericUpDown->Value = (System::Decimal)(1+(l_pColorPicker->m_ColorPicker->SelectedColor.A-128)/128.f);
	}
//end namespace MPDI
}