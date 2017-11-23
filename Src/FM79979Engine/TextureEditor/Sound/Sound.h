#pragma once


namespace Sound 
{

	public ref class SoundObject : public System::Windows::Forms::Form
	{
	public:
		//SoundObject(GCFORM::TabControl^e_ptabControl,GCFORM::Form^e_pForm)
		SoundObject()
		{
			InitializeComponent();
			m_pGameApp = new cGameApp(0);
			m_pSoundParser = m_pGameApp->m_spSoundParser;
			//m_pForm = e_pForm;
			//if( m_pForm->Parent )
			//{
				//m_pParentHandle = (HWND)m_pForm->Parent->Handle.ToPointer();
			//}
			//if( e_ptabControl )
			//{
			//	m_pTabPage = gcnew GCFORM::TabPage();
			//	m_pTabPage->Text = "Án­µ/SoundObject";
			//	m_pTabPage->Controls->Add(this);
			//	e_ptabControl->TabPages->Add(m_pTabPage);
			//	e_ptabControl->SelectedIndexChanged += gcnew System::EventHandler(this, &SoundObject::tabControl_SelectedIndexChanged);
			//}
			//else
			//{
			//	m_pForm->Controls->Add(this);
			//}
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SoundObject()
		{
			if (components)
			{
				SAFE_DELETE(m_pGameApp);
				delete components;
			}
		}
	private: System::Windows::Forms::ListBox^  Sound_listBox;
	protected: 
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  SaveToolStripMenuItem;

	private: System::Windows::Forms::Button^  AddSound_button;
	private: System::Windows::Forms::Button^  DelSound_button;
	private: System::Windows::Forms::Button^  PlaySound_button;
	private: System::Windows::Forms::CheckBox^  SoundLoop_checkBox;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Button^  StopSound_button;
	private: System::ComponentModel::IContainer^  components;
	//my
	private: System::Windows::Forms::Form^	m_pForm;
	private: System::Windows::Forms::TabPage^m_pTabPage;
	private: System::Windows::Forms::NumericUpDown^  Piority_numericUpDown;
	private: System::Windows::Forms::Label^  label1;
	cGameApp*m_pGameApp;
			 cSoundParser*m_pSoundParser;
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->Sound_listBox = (gcnew System::Windows::Forms::ListBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->SaveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->AddSound_button = (gcnew System::Windows::Forms::Button());
			this->DelSound_button = (gcnew System::Windows::Forms::Button());
			this->PlaySound_button = (gcnew System::Windows::Forms::Button());
			this->SoundLoop_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->StopSound_button = (gcnew System::Windows::Forms::Button());
			this->Piority_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Piority_numericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// Sound_listBox
			// 
			this->Sound_listBox->FormattingEnabled = true;
			this->Sound_listBox->HorizontalScrollbar = true;
			this->Sound_listBox->ItemHeight = 12;
			this->Sound_listBox->Location = System::Drawing::Point(16, 47);
			this->Sound_listBox->Name = L"Sound_listBox";
			this->Sound_listBox->Size = System::Drawing::Size(194, 100);
			this->Sound_listBox->TabIndex = 0;
			this->Sound_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &SoundObject::Sound_listBox_SelectedIndexChanged);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->fileToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(356, 24);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->openToolStripMenuItem, 
				this->SaveToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(39, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(108, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &SoundObject::openToolStripMenuItem_Click);
			// 
			// SaveToolStripMenuItem
			// 
			this->SaveToolStripMenuItem->Name = L"SaveToolStripMenuItem";
			this->SaveToolStripMenuItem->Size = System::Drawing::Size(108, 22);
			this->SaveToolStripMenuItem->Text = L"Save";
			this->SaveToolStripMenuItem->Click += gcnew System::EventHandler(this, &SoundObject::openToolStripMenuItem_Click);
			// 
			// AddSound_button
			// 
			this->AddSound_button->Location = System::Drawing::Point(16, 160);
			this->AddSound_button->Name = L"AddSound_button";
			this->AddSound_button->Size = System::Drawing::Size(61, 24);
			this->AddSound_button->TabIndex = 2;
			this->AddSound_button->Text = L"AddSound";
			this->AddSound_button->UseVisualStyleBackColor = true;
			this->AddSound_button->Click += gcnew System::EventHandler(this, &SoundObject::AddSound_button_Click);
			// 
			// DelSound_button
			// 
			this->DelSound_button->Location = System::Drawing::Point(83, 160);
			this->DelSound_button->Name = L"DelSound_button";
			this->DelSound_button->Size = System::Drawing::Size(61, 24);
			this->DelSound_button->TabIndex = 3;
			this->DelSound_button->Text = L"DelSound";
			this->DelSound_button->UseVisualStyleBackColor = true;
			this->DelSound_button->Click += gcnew System::EventHandler(this, &SoundObject::AddSound_button_Click);
			// 
			// PlaySound_button
			// 
			this->PlaySound_button->Location = System::Drawing::Point(231, 82);
			this->PlaySound_button->Name = L"PlaySound_button";
			this->PlaySound_button->Size = System::Drawing::Size(83, 24);
			this->PlaySound_button->TabIndex = 4;
			this->PlaySound_button->Text = L"Play";
			this->PlaySound_button->UseVisualStyleBackColor = true;
			this->PlaySound_button->Click += gcnew System::EventHandler(this, &SoundObject::PlaySound_button_Click);
			// 
			// SoundLoop_checkBox
			// 
			this->SoundLoop_checkBox->AutoSize = true;
			this->SoundLoop_checkBox->Location = System::Drawing::Point(161, 168);
			this->SoundLoop_checkBox->Name = L"SoundLoop_checkBox";
			this->SoundLoop_checkBox->Size = System::Drawing::Size(49, 16);
			this->SoundLoop_checkBox->TabIndex = 5;
			this->SoundLoop_checkBox->Text = L"Loop";
			this->SoundLoop_checkBox->UseVisualStyleBackColor = true;
			this->SoundLoop_checkBox->CheckedChanged += gcnew System::EventHandler(this, &SoundObject::SoundLoop_checkBox_CheckedChanged);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &SoundObject::timer1_Tick);
			// 
			// StopSound_button
			// 
			this->StopSound_button->Location = System::Drawing::Point(231, 112);
			this->StopSound_button->Name = L"StopSound_button";
			this->StopSound_button->Size = System::Drawing::Size(83, 24);
			this->StopSound_button->TabIndex = 6;
			this->StopSound_button->Text = L"Stop";
			this->StopSound_button->UseVisualStyleBackColor = true;
			this->StopSound_button->Click += gcnew System::EventHandler(this, &SoundObject::PlaySound_button_Click);
			// 
			// Piority_numericUpDown
			// 
			this->Piority_numericUpDown->Location = System::Drawing::Point(231, 168);
			this->Piority_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->Piority_numericUpDown->Name = L"Piority_numericUpDown";
			this->Piority_numericUpDown->Size = System::Drawing::Size(90, 22);
			this->Piority_numericUpDown->TabIndex = 7;
			this->Piority_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &SoundObject::Piority_numericUpDown_ValueChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(229, 154);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(36, 12);
			this->label1->TabIndex = 8;
			this->label1->Text = L"Piority";
			// 
			// SoundObject
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(356, 210);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->Piority_numericUpDown);
			this->Controls->Add(this->StopSound_button);
			this->Controls->Add(this->SoundLoop_checkBox);
			this->Controls->Add(this->PlaySound_button);
			this->Controls->Add(this->DelSound_button);
			this->Controls->Add(this->AddSound_button);
			this->Controls->Add(this->Sound_listBox);
			this->Controls->Add(this->menuStrip1);
			this->Name = L"SoundObject";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Piority_numericUpDown))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		//static ogg_stream*l_pogg_stream = 0;
	private: System::Void AddSound_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( sender == DelSound_button )
				 {
					 if( Sound_listBox->SelectedIndex == -1 )
						 return;
					 m_pSoundParser->RemoveObject(Sound_listBox->SelectedIndex);
					 Sound_listBox->Items->RemoveAt(Sound_listBox->SelectedIndex);
				 }
				 else
				 {
					cli::array<String^>^l_strNameList = DNCT::OpenFileAndGetNames();
					if(!l_strNameList)
						return;
					for each(String^l_strName in l_strNameList)
					{
						String^l_strFileName = DNCT::GetFileNameWithoutFullPath(l_strName,true);
						if(!DNCT::CheckListContainStringAndAdd(this->Sound_listBox,l_strFileName))
						{
						  char*l_strFileName = DNCT::GcStringToChar(l_strName);
						  bool	l_b = m_pSoundParser->AddSound(m_pSoundParser,l_strFileName);
						  if(!l_b)
						  {
							  Sound_listBox->Items->RemoveAt(Sound_listBox->Items->Count-1);
						  }
						  //cOpanalWAV*l_pOpanalWAV = new cOpanalWAV(l_strFileName);
						  //if( wcslen(l_pOpanalWAV->GetName()) == 0 )
						  //{
							 // delete l_pOpanalWAV;
							 // Sound_listBox->Items->RemoveAt(Sound_listBox->Items->Count-1);
						  //}
						  //else
						  //{
							 // m_pSoundParser->AddObject(l_pOpanalWAV);
						  //}


						 // l_pogg_stream = new ogg_stream();
						 // l_pogg_stream->open(l_strFileName);
						 // if(!l_pogg_stream->playback())
						 // {
							//int a=0;
						 // }
						 // l_pogg_stream->update();
						  //l_pogg_stream->play();
						}
					}
				 }
			 }
//System::Void	ClearData()
//{
//	m_pSoundParser->Clear();
//	Sound_listBox->Items->Clear();
//}
private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			if( sender == SaveToolStripMenuItem )
			{
				String^l_FileName =  DNCT::SaveFileAndGetName("(*.xml)|*.xml");
				if(!l_FileName)
					return;
				l_FileName = ForceAddExtenName(l_FileName,"xml");
				char*l_strFileName = DNCT::GcStringToChar(l_FileName);
				if( l_strFileName )
				{
					m_pSoundParser->Export(l_strFileName);
//					FileToUnicode(String(l_strFileName).ToString());
				}
			}
			else
			if( sender == openToolStripMenuItem )
			{
				String^l_FileName = DNCT::OpenFileAndGetName("(*.xml)|*.xml");
				if( l_FileName )
				{
					//ClearData();
					bool l_b = m_pSoundParser->Parse( DNCT::GcStringToChar(l_FileName) );
					if( !l_b )
					{
						WARNING_MSG("data error");
						return;
					}
					int	l_i = m_pSoundParser->Count();
					for( int i=0;i<l_i;++i )
					{
						Sound_listBox->Items->Add(
							String(
							UT::WcharToChar(
							(*m_pSoundParser)[i]->GetName()
							).c_str()).ToString()
							);
					}
				}
			}
		 }
private: System::Void PlaySound_button_Click(System::Object^  sender, System::EventArgs^  e) {
			if( sender == StopSound_button )
			{
				for( int i=0;i<m_pSoundParser->Count();++i )
					(*m_pSoundParser)[i]->Play(false);
				return;
			}
			if( Sound_listBox->SelectedIndex == -1 )
				return;
			(*m_pSoundParser)[Sound_listBox->SelectedIndex]->Play(true);
			 
		 }
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( m_pSoundParser )
				 m_pSoundParser->Update(0.016f);
			 //for(int i=0;i<(int)SoundList.size();++i)
			 //{
				//if( l_pogg_stream )
				//{
				//	//if(!l_pogg_stream->playing())
				//	{
				//		if(!l_pogg_stream->playing())
				//		if(l_pogg_stream->update())
				//			//if(!l_pogg_stream->playing())
				//				if(!l_pogg_stream->playback())
				//				{
				//				
				//				}
				//					//throw string("Ogg abruptly stopped.");
				//	}
				//	////l_pogg_stream->update();
				//	//static int l_i = 0;
				//	//l_i++;
				//	//if( l_i>=1000 )
				//	//	l_i = 0;
				//	//PlaySound_button->Text = l_i.ToString();
				//}
			 //}
		 }
private: System::Void Sound_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 if( Sound_listBox->SelectedIndex == -1)
				 return;
			SoundLoop_checkBox->Checked = (*m_pSoundParser)[Sound_listBox->SelectedIndex]->IsLoop();
			Piority_numericUpDown->Value = (*m_pSoundParser)[Sound_listBox->SelectedIndex]->GetPiority();
		 }

private: System::Void SoundLoop_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if( Sound_listBox->SelectedIndex == -1)
				 return;
			 (*m_pSoundParser)[Sound_listBox->SelectedIndex]->SetLoop(  SoundLoop_checkBox->Checked  );
		 }
private: System::Void tabControl_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 GCFORM::TabControl^l_ptabControl = (GCFORM::TabControl^)sender;
			if( l_ptabControl->SelectedTab!=m_pTabPage )
			{

			}
			else
			{
				AssignListData(Sound_listBox,m_pSoundParser);
			}
		 }
private: System::Void Piority_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( Sound_listBox->SelectedIndex == -1)
				 return;
			 (*m_pSoundParser)[Sound_listBox->SelectedIndex]->SetPiority(  (int)Piority_numericUpDown->Value  );
		 }
};
}

