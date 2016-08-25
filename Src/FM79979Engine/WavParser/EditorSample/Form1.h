#pragma once
#include "WavInfo.h"
#include "WavWaves.h"

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//ensure preprocessor definiation DEBUG not _DEBUG or it will occur memory problem.
//I donno why ask M$.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
namespace EditorSample 
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			//m_pTargetControl = this;
			m_pTargetControl = this->OPENGL_panel;
			m_MyOpenGLWindowSize = m_pTargetControl->Size;
			m_WindowStartSize = this->Size;
			m_WindowStartSize.Height += 20;
			m_pGameApp = new cGameApp((HWND)m_pTargetControl->Handle.ToPointer());
			m_pGameApp->Init();
			m_HdcMV = GetDC((HWND)m_pTargetControl->Handle.ToPointer());
			m_HGLRCMV = m_pGameApp->m_sHGLRC;
			//
			this->m_pWaveInfo = nullptr;
			this->m_pWavWaves = nullptr;
			//for mouse event
			m_pTargetControl->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseMove);
			m_pTargetControl->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseDown);
			m_pTargetControl->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseUp);
			m_pTargetControl->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseUp);
			m_pTargetControl->SizeChanged += gcnew System::EventHandler(this, &Form1::MySizeChanged);
			//
			this->timer1->Enabled = true;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
			SAFE_DELETE(this->m_pWaveInfo);
			SAFE_DELETE(m_pGameApp);
			SAFE_DELETE(this->m_pWavWaves);
		}
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Timer^  timer1;
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

		//my
		cGameApp*					m_pGameApp;
		HDC							m_HdcMV;
		HGLRC						m_HGLRCMV;
		cWaveInfo*					m_pWaveInfo;
		cWavWaves*					m_pWavWaves;
		Control^					m_pTargetControl;
		//
		System::Drawing::Size		m_MyOpenGLWindowSize;
		System::Drawing::Size		m_WindowStartSize;
		//
	private: System::Windows::Forms::Button^  OpenWavFile_button;
	private: System::Windows::Forms::TextBox^  WavFileName_textBox;
	private: System::Windows::Forms::Button^  Play_button;
	private: System::Windows::Forms::Panel^  OPENGL_panel;
	private: System::Windows::Forms::TrackBar^  CurrentTime_trackBar;
	private: System::Windows::Forms::Label^  CurrentTime_label;
	private: System::Windows::Forms::ListBox^  WavInfo_listBox;


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->OpenWavFile_button = (gcnew System::Windows::Forms::Button());
			this->WavFileName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->Play_button = (gcnew System::Windows::Forms::Button());
			this->OPENGL_panel = (gcnew System::Windows::Forms::Panel());
			this->WavInfo_listBox = (gcnew System::Windows::Forms::ListBox());
			this->CurrentTime_trackBar = (gcnew System::Windows::Forms::TrackBar());
			this->CurrentTime_label = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CurrentTime_trackBar))->BeginInit();
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// OpenWavFile_button
			// 
			this->OpenWavFile_button->Location = System::Drawing::Point(12, 4);
			this->OpenWavFile_button->Name = L"OpenWavFile_button";
			this->OpenWavFile_button->Size = System::Drawing::Size(94, 34);
			this->OpenWavFile_button->TabIndex = 0;
			this->OpenWavFile_button->Text = L"open wav file";
			this->OpenWavFile_button->UseVisualStyleBackColor = true;
			this->OpenWavFile_button->Click += gcnew System::EventHandler(this, &Form1::OpenWavFile_button_Click);
			// 
			// WavFileName_textBox
			// 
			this->WavFileName_textBox->Location = System::Drawing::Point(112, 12);
			this->WavFileName_textBox->Name = L"WavFileName_textBox";
			this->WavFileName_textBox->Size = System::Drawing::Size(378, 20);
			this->WavFileName_textBox->TabIndex = 1;
			this->WavFileName_textBox->Text = L"C:\\Users\\admin\\Desktop\\FM79979\\Media\\MyFMBook\\iPhone_Monster\\1.wav";
			// 
			// Play_button
			// 
			this->Play_button->Location = System::Drawing::Point(496, 8);
			this->Play_button->Name = L"Play_button";
			this->Play_button->Size = System::Drawing::Size(59, 26);
			this->Play_button->TabIndex = 2;
			this->Play_button->Text = L"PLAY";
			this->Play_button->UseVisualStyleBackColor = true;
			this->Play_button->Click += gcnew System::EventHandler(this, &Form1::Play_button_Click);
			// 
			// OPENGL_panel
			// 
			this->OPENGL_panel->Location = System::Drawing::Point(193, 88);
			this->OPENGL_panel->Name = L"OPENGL_panel";
			this->OPENGL_panel->Size = System::Drawing::Size(729, 449);
			this->OPENGL_panel->TabIndex = 3;
			// 
			// WavInfo_listBox
			// 
			this->WavInfo_listBox->FormattingEnabled = true;
			this->WavInfo_listBox->Location = System::Drawing::Point(12, 52);
			this->WavInfo_listBox->Name = L"WavInfo_listBox";
			this->WavInfo_listBox->Size = System::Drawing::Size(175, 537);
			this->WavInfo_listBox->TabIndex = 4;
			// 
			// CurrentTime_trackBar
			// 
			this->CurrentTime_trackBar->Location = System::Drawing::Point(333, 38);
			this->CurrentTime_trackBar->Name = L"CurrentTime_trackBar";
			this->CurrentTime_trackBar->Size = System::Drawing::Size(446, 45);
			this->CurrentTime_trackBar->TabIndex = 5;
			// 
			// CurrentTime_label
			// 
			this->CurrentTime_label->AutoSize = true;
			this->CurrentTime_label->Location = System::Drawing::Point(220, 47);
			this->CurrentTime_label->Name = L"CurrentTime_label";
			this->CurrentTime_label->Size = System::Drawing::Size(64, 13);
			this->CurrentTime_label->TabIndex = 6;
			this->CurrentTime_label->Text = L"CurrentTime";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(946, 549);
			this->Controls->Add(this->CurrentTime_label);
			this->Controls->Add(this->CurrentTime_trackBar);
			this->Controls->Add(this->WavInfo_listBox);
			this->Controls->Add(this->OPENGL_panel);
			this->Controls->Add(this->Play_button);
			this->Controls->Add(this->WavFileName_textBox);
			this->Controls->Add(this->OpenWavFile_button);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Resize += gcnew System::EventHandler(this, &Form1::Form1_Resize);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CurrentTime_trackBar))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
		{
			wglMakeCurrent( m_HdcMV,m_HGLRCMV );
			RECT rcClient;
			GetClientRect((HWND)m_pTargetControl->Handle.ToPointer(), &rcClient);
			UseShaderProgram();
			cGameApp::m_svViewPortSize.x = 0.f;
			cGameApp::m_svViewPortSize.y = 0.f;
			cGameApp::m_svViewPortSize.z = (float)m_pTargetControl->Width;
			cGameApp::m_svViewPortSize.w = (float)rcClient.bottom;
			this->m_pGameApp->Run();
			glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
			glClearColor(0,0,0,0);
			glClearDepth(1.0f);
			glAlphaFunc(GL_GREATER,0.0001f);
			glShadeModel(GL_SMOOTH);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_ALPHA_TEST);
			cGameApp::ShowInfo();
			SwapBuffers(m_HdcMV);
		}

		System::Void MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			//this->Focus();
			if( !timer1->Enabled || !this->Visible )
				return;
			if( m_pGameApp )
				m_pGameApp->MouseDown(e->X,e->Y);
		}
		System::Void MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			if( !timer1->Enabled || !this->Visible )
				return;
			if( m_pGameApp )
				m_pGameApp->MouseMove(e->X,e->Y);
		}

		System::Void MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			if( !timer1->Enabled || !this->Visible )
				return;
			if( m_pGameApp )
				m_pGameApp->MouseUp(e->X,e->Y);
		}

		System::Void MySizeChanged(System::Object^  sender, System::EventArgs^  e) 
		{
		}
	private: System::Void Form1_Resize(System::Object^  sender, System::EventArgs^  e) 
			 {
				 Vector2 l_WindowSize(m_WindowStartSize.Width,m_WindowStartSize.Height);
				 Vector2 l_OpenGLWindowSize(m_MyOpenGLWindowSize.Width,m_MyOpenGLWindowSize.Height);
				 Vector2 l_SizeRation(this->Width/l_WindowSize.x,this->Height/l_WindowSize.y);
				 Vector2	l_NewSizeForOpenGL(l_SizeRation.x*l_OpenGLWindowSize.x,l_SizeRation.y*l_OpenGLWindowSize.y);
				 this->OPENGL_panel->Width = (int)l_NewSizeForOpenGL.x;
				 this->OPENGL_panel->Height = (int)l_NewSizeForOpenGL.y;
			 }
private: System::Void OpenWavFile_button_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 //"png files (*.png)|*.png|All files (*.*)|*.*"
			 String^l_strFileName = DNCT::OpenFileAndGetName("wav files (*.wav)|*.wav|All files (*.*)|*.*");
			 if(l_strFileName)
			 {
				 this->WavFileName_textBox->Text = l_strFileName;
			 }
		 }
private: System::Void Play_button_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			if(this->WavFileName_textBox->Text && m_pGameApp)
			{
				//if(this->m_pGameApp->m_spSoundParser->AddSound(DNCT::GcStringToChar(this->WavFileName_textBox->Text)))
				{
					std::wstring l_FileName = DNCT::GcStringToWchar(this->WavFileName_textBox->Text);
					//this->m_pGameApp->SoundPlay(l_FileName.c_str(),true);
					SAFE_DELETE(this->m_pWaveInfo);
					this->m_pWaveInfo = new cWaveInfo();
					this->WavInfo_listBox->Items->Clear();
					if(this->m_pWaveInfo->OpenFile(UT::WcharToChar(l_FileName).c_str()))
					{
						this->WavInfo_listBox->Items->Clear();
						String^l_str = "";
						String^l_Temp = nullptr;
						const char*l_FMTID = (const char*)&this->m_pWaveInfo->m_WAVFileHdr_Struct.Id[0];
						l_Temp = gcnew String(l_FMTID);
						l_str = "id "+ l_Temp;
						this->WavInfo_listBox->Items->Add(gcnew String("========WAVFileHdr==========="));
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = "size "+this->m_pWaveInfo->m_WAVFileHdr_Struct.Size;
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = "type ";
						l_str += gcnew String((const char*)this->m_pWaveInfo->m_WAVFileHdr_Struct.Type);
						this->WavInfo_listBox->Items->Add(l_str);
						this->WavInfo_listBox->Items->Add(gcnew String("======FMTHdr=========="));
						l_str = "chunk id "+ gcnew String((const char*)this->m_pWaveInfo->m_FMT_And_Data_Header[0].Id);
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = "SizeSize "+this->m_pWaveInfo->m_FMT_And_Data_Header[0].Size;
						this->WavInfo_listBox->Items->Add(l_str);
						this->WavInfo_listBox->Items->Add(gcnew String("========WAVFmtHdr==========="));
						l_str = "bits perSample "+this->m_pWaveInfo->m_WAVFmtHdr_Struct.BitsPerSample;
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = "block align "+this->m_pWaveInfo->m_WAVFmtHdr_Struct.BlockAlign;
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = "BytesRate "+this->m_pWaveInfo->m_WAVFmtHdr_Struct.BytesRate;
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = "iChannels "+this->m_pWaveInfo->m_WAVFmtHdr_Struct.Channels;
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = "Format "+this->m_pWaveInfo->m_WAVFmtHdr_Struct.Format;
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = "SampleRate "+this->m_pWaveInfo->m_WAVFmtHdr_Struct.SampleRate;
						this->WavInfo_listBox->Items->Add(l_str);
						this->WavInfo_listBox->Items->Add(gcnew String("=======WAVFmtExHdr============"));
						l_str = "SamplesPerBlock "+this->m_pWaveInfo->m_WAVFmtExHdr_Struct.SamplesPerBlock;
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = "Size "+this->m_pWaveInfo->m_WAVFmtExHdr_Struct.Size;
						this->WavInfo_listBox->Items->Add(l_str);
						this->WavInfo_listBox->Items->Add(gcnew String("======WAVSmplHdr============="));
						l_str = "FineTune "+this->m_pWaveInfo->m_WAVSmplHdr_Struct.FineTune;
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = +this->m_pWaveInfo->m_WAVSmplHdr_Struct.Loop?"Loop:true":"Loop:false";
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = this->m_pWaveInfo->m_WAVSmplHdr_Struct.Loops?"Loops:true":"Loops:false";
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = "Manufacturer "+this->m_pWaveInfo->m_WAVSmplHdr_Struct.Manufacturer;
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = "Note "+this->m_pWaveInfo->m_WAVSmplHdr_Struct.Note;
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = "Product "+this->m_pWaveInfo->m_WAVSmplHdr_Struct.Product;
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = "SamplePeriod "+this->m_pWaveInfo->m_WAVSmplHdr_Struct.SamplePeriod;
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = "SamplerData "+this->m_pWaveInfo->m_WAVSmplHdr_Struct.SamplerData;
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = "SMPTEFormat "+this->m_pWaveInfo->m_WAVSmplHdr_Struct.SMPTEFormat;
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = "SMPTEOffest "+this->m_pWaveInfo->m_WAVSmplHdr_Struct.SMPTEOffest;
						this->WavInfo_listBox->Items->Add(l_str);
						this->WavInfo_listBox->Items->Add(gcnew String("======DataHdr=========="));
						l_str = "chunk id "+ gcnew String((const char*)this->m_pWaveInfo->m_FMT_And_Data_Header[1].Id);
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = "SizeSize "+this->m_pWaveInfo->m_FMT_And_Data_Header[1].Size;
						this->WavInfo_listBox->Items->Add(l_str);
						this->WavInfo_listBox->Items->Add(gcnew String("====common==========="));
						l_str = "Format "+this->m_pWaveInfo->m_Format;
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = "Time "+this->m_pWaveInfo->m_fTime;
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = "Freq "+this->m_pWaveInfo->m_iFreq;
						this->WavInfo_listBox->Items->Add(l_str);
						l_str = "Size "+this->m_pWaveInfo->m_iSoundDataSize;
						this->WavInfo_listBox->Items->Add(l_str);
						//is->m_pWaveInfo->m_WAVChunkHdr_Struct.
					}

					//WavInfo_listBox->Items->Add();
				}
			}
		 }
};
}