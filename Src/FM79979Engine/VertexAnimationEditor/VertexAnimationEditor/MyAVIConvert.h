#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

//#include "../../Core/AVIPlayer/AndroidVideoPlayer.h"
#include "../../Core/GameplayUT/Render/VideoPlayer/AndroidVideoPlayer.h"

namespace VertexAnimationEditor
{

	/// <summary>
	/// Summary for MyAVIConvert
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class MyAVIConvert : public System::Windows::Forms::Form
	{
	public:
		MyAVIConvert(void)
		{
			m_pVideoConvert = 0;
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pTargetControl = this;
			m_pGameApp = new cGameApp((HWND)m_pTargetControl->Handle.ToPointer());
			m_pGameApp->Init();

			m_HdcMV = GetDC((HWND)m_pTargetControl->Handle.ToPointer());
			m_HGLRCMV = m_pGameApp->m_spOpenGLRender->m_HGLRC;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyAVIConvert()
		{
			SAFE_DELETE(m_pVideoConvert);
			SAFE_DELETE(m_pGameApp);
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  AVIConvert_button;
	private: System::Windows::Forms::Label^  Progress_label;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Button^  CompressVideoPlay_button;

	protected:
		//my
		cVideoConvert*		m_pVideoConvert;
		cFMVVideo*			m_pFMVVideo;
		cGameApp*			m_pGameApp;
		HDC					m_HdcMV;
		HGLRC				m_HGLRCMV;
		String^				m_strConvertVideoFileName;
	private: System::Windows::Forms::Timer^  timer2;
	private: System::Windows::Forms::NumericUpDown^  Width_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  Height_numericUpDown;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::NumericUpDown^  Quality_numericUpDown;
	protected: 
		Control^			m_pTargetControl;

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
			this->AVIConvert_button = (gcnew System::Windows::Forms::Button());
			this->Progress_label = (gcnew System::Windows::Forms::Label());
			this->CompressVideoPlay_button = (gcnew System::Windows::Forms::Button());
			this->timer2 = (gcnew System::Windows::Forms::Timer(this->components));
			this->Width_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->Height_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->Quality_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Width_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Height_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Quality_numericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// AVIConvert_button
			// 
			this->AVIConvert_button->Location = System::Drawing::Point(-1, 24);
			this->AVIConvert_button->Name = L"AVIConvert_button";
			this->AVIConvert_button->Size = System::Drawing::Size(92, 48);
			this->AVIConvert_button->TabIndex = 11;
			this->AVIConvert_button->Text = L"AVIConvert";
			this->AVIConvert_button->UseVisualStyleBackColor = true;
			this->AVIConvert_button->Click += gcnew System::EventHandler(this, &MyAVIConvert::AVIConvert_button_Click);
			// 
			// Progress_label
			// 
			this->Progress_label->AutoSize = true;
			this->Progress_label->Location = System::Drawing::Point(10, 9);
			this->Progress_label->Name = L"Progress_label";
			this->Progress_label->Size = System::Drawing::Size(44, 12);
			this->Progress_label->TabIndex = 12;
			this->Progress_label->Text = L"Progress";
			// 
			// CompressVideoPlay_button
			// 
			this->CompressVideoPlay_button->Location = System::Drawing::Point(108, 24);
			this->CompressVideoPlay_button->Name = L"CompressVideoPlay_button";
			this->CompressVideoPlay_button->Size = System::Drawing::Size(92, 48);
			this->CompressVideoPlay_button->TabIndex = 13;
			this->CompressVideoPlay_button->Text = L"CompressVideoPlay";
			this->CompressVideoPlay_button->UseVisualStyleBackColor = true;
			this->CompressVideoPlay_button->Click += gcnew System::EventHandler(this, &MyAVIConvert::CompressVideoPlay_button_Click);
			// 
			// timer2
			// 
			this->timer2->Interval = 1;
			this->timer2->Tick += gcnew System::EventHandler(this, &MyAVIConvert::timer2_Tick);
			// 
			// Width_numericUpDown
			// 
			this->Width_numericUpDown->Location = System::Drawing::Point(259, 17);
			this->Width_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2048, 0, 0, 0});
			this->Width_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, System::Int32::MinValue});
			this->Width_numericUpDown->Name = L"Width_numericUpDown";
			this->Width_numericUpDown->Size = System::Drawing::Size(67, 22);
			this->Width_numericUpDown->TabIndex = 14;
			this->Width_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, System::Int32::MinValue});
			// 
			// Height_numericUpDown
			// 
			this->Height_numericUpDown->Location = System::Drawing::Point(259, 50);
			this->Height_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2048, 0, 0, 0});
			this->Height_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, System::Int32::MinValue});
			this->Height_numericUpDown->Name = L"Height_numericUpDown";
			this->Height_numericUpDown->Size = System::Drawing::Size(67, 22);
			this->Height_numericUpDown->TabIndex = 15;
			this->Height_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, System::Int32::MinValue});
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(217, 23);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(34, 12);
			this->label1->TabIndex = 16;
			this->label1->Text = L"Width";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(217, 56);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(36, 12);
			this->label2->TabIndex = 17;
			this->label2->Text = L"Height";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(328, 23);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(39, 12);
			this->label3->TabIndex = 19;
			this->label3->Text = L"Quality";
			// 
			// Quality_numericUpDown
			// 
			this->Quality_numericUpDown->Location = System::Drawing::Point(370, 17);
			this->Quality_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->Quality_numericUpDown->Name = L"Quality_numericUpDown";
			this->Quality_numericUpDown->Size = System::Drawing::Size(74, 22);
			this->Quality_numericUpDown->TabIndex = 18;
			this->Quality_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			// 
			// MyAVIConvert
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(445, 358);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->Quality_numericUpDown);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->Height_numericUpDown);
			this->Controls->Add(this->Width_numericUpDown);
			this->Controls->Add(this->CompressVideoPlay_button);
			this->Controls->Add(this->Progress_label);
			this->Controls->Add(this->AVIConvert_button);
			this->Name = L"MyAVIConvert";
			this->Text = L"MyAVIConvert";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Width_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Height_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Quality_numericUpDown))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void AVIConvert_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 cli::array<String^>^l_strNameList = OpenFileAndGetNames("avi files (*.avil)|*.avi|All files (*.*)|*.*");
				 String^l_strErrorMsg = gcnew String("");
				 if( l_strNameList )
				 {
					 int	l_iNum = l_strNameList->Length;
					 int	l_iIndex = 0;
					 for each( String^l_strFileName in l_strNameList )
					 {
						this->Text = ((float)l_iIndex/l_iNum).ToString();
						m_strConvertVideoFileName = l_strFileName;
						SAFE_DELETE(m_pVideoConvert);
						m_pVideoConvert = new cVideoConvert();
						m_pVideoConvert->m_iTargetHeight = (int)Height_numericUpDown->Value;
						m_pVideoConvert->m_iTargetWidth = (int)Width_numericUpDown->Value;
						m_pVideoConvert->m_iQuality = (int)Quality_numericUpDown->Value;
						if( m_pVideoConvert->OpenFile(DNCT::GcStringToChar(m_strConvertVideoFileName).c_str()) )
						{
							String^l_strDestFileName = DNCT::ChangeFileExtensionName(gcnew String(m_strConvertVideoFileName),"fmv");
							std::string	l_strWriteFileName = DNCT::GcStringToChar(l_strDestFileName);
							FILE*l_pFile = fopen(l_strWriteFileName.c_str(),"w+b");
							if( l_pFile )
							{
								if(!m_pVideoConvert->StartConvert(l_pFile))
								{
									//WARNING_MSG("convert failed!");
								}
								fclose(l_pFile);
							}
						}
						else
						{
							l_strErrorMsg += m_strConvertVideoFileName;
							l_strErrorMsg += DNCT::GetChanglineString();
							//UT::ErrorMsg(L"open file failed!",L"format error!?");
						}
						++l_iIndex;
					}
				 }
				 if( l_strErrorMsg->Length )
				 {
					 l_strErrorMsg += "file open failed!";
					 WARNING_MSG(l_strErrorMsg);
				 }
				 else
				 {
					WARNING_MSG("convert ok");
				 }
			 }
	private: System::Void timer2_Tick(System::Object^  sender, System::EventArgs^  e)
			 {
				if( m_pGameApp )
					this->m_pGameApp->Run();
			//render
				wglMakeCurrent( m_HdcMV,m_HGLRCMV );
				RECT rcClient;
				GetClientRect((HWND)m_pTargetControl->Handle.ToPointer(), &rcClient);
				UseShaderProgram();
				cGameApp::m_spOpenGLRender->m_vViewPortSize.x = 0.f;
				cGameApp::m_spOpenGLRender->m_vViewPortSize.y = 0.f;
				cGameApp::m_spOpenGLRender->m_vViewPortSize.z = (float)Width;
				cGameApp::m_spOpenGLRender->m_vViewPortSize.w = (float)rcClient.bottom;//(float)Height-(Height-rcClient.bottom);
				//RECT	l_rc;
				//GetWindowRect((HWND)this->Handle.ToPointer(),&l_rc);
				//cGameApp::m_svViewPortSize.y = l_rc.top;
				glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
				glClearColor(0.5,0.5,0.5,0);
				glClearDepth(1.0f);											// Depth Buffer Setup
				glAlphaFunc(GL_GREATER,0.0001f);
				glShadeModel(GL_SMOOTH);									// Select Smooth Shading
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_ALPHA_TEST);
				FATMING_CORE::UseShaderProgram(DEFAULT_SHADER);
				glEnable2D(1920,1080);
				if( m_pFMVVideo )
				{
					if( !m_pFMVVideo->m_bUsingMultiThread )
						m_pFMVVideo->Update(cGameApp::m_sTimeAndFPS.fElpaseTime);
					m_pFMVVideo->Render();
					this->Text = gcnew String(m_pFMVVideo->GetFPS());
					if( m_pFMVVideo->IsPlayDone() )
					{
						timer2->Enabled = false;
					}
				}
				cGameApp::ShowInfo();
				SwapBuffers(m_HdcMV);
			 }
private: System::Void CompressVideoPlay_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 String^l_strFileName = DNCT::OpenFileAndGetName();
			 if( l_strFileName )
			 {			 
				 SAFE_DELETE(m_pFMVVideo);
				 m_pFMVVideo = new cFMVVideo();
				 //m_pFMVVideo->m_bUsingMultiThread = false;
				 if(m_pFMVVideo->OpenFile(DNCT::GcStringToChar(l_strFileName).c_str()))
				 {
					timer2->Enabled = true;
				 }
			 }
		 }
};
}
