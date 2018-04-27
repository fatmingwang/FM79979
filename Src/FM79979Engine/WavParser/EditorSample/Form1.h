#pragma once
//#include "WavWaves.h"
#include "../MusicGame/KissFFTConvert.h"
#include "../MusicGame/KissFFTStreaming.h"
#include "../MusicGame/SoundFFTCapture.h"
#include "../MusicGame/TimeFrequencyAmplitudeValueCapture.h"
#include "../MusicGame/ToneData.h"
#include "../MusicGame/MusicGameApp.h"
#include "../MusicGame/FFTStoreExporter.h"
#include "../MusicGame/Parameters.h"

#include "MidiFile.h"
#include "Options.h"

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//ensure preprocessor definiation DEBUG not _DEBUG or it will occur memory problem.
//I donno why ask M$.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
extern int ONE_FRAME_NEED_NUM_FFT_DATA_COUNT;

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
			cSoundCompareParameter::m_seMusicGameMode = eMGM_EDITOR;
			//char**l_ppData = new char*[10];
			//for( int i=0;i<10;++i )
			//{
			//	l_ppData[i] = new char[200];
			//}
			//std::string l_str1;
			//std::wstring l_str2;
			//int l_ii = 0;
			//cSubMPDI l_subMPDI;
			//const char*po3 = typeid(l_subMPDI).name();
			//const char*po2 = typeid(l_str1).name();
			//const char*po = typeid(l_str2).name();
			//const char*po22 = typeid(l_str1).raw_name();
			//const char* l_pp2 = typeid(l_ii).name();
			//std::string ll = typename(l_ii);
			m_bTimerUpdateEnd = false;
			InitializeComponent();
			this->timer1->Enabled = false;
			//
			//TODO: Add the constructor code here
			//
			//m_pTargetControl = this;
			m_pTargetControl = this->OPENGL_panel;
			m_MyOpenGLWindowSize = m_pTargetControl->Size;
			m_WindowStartSize = this->Size;
			m_WindowStartSize.Height += 20;
			//m_pGameApp = new cMusicGameApp((HWND)m_pTargetControl->Handle.ToPointer(),Vector2(1280,720));
			//m_pGameApp = new cMusicGameApp((HWND)m_pTargetControl->Handle.ToPointer(),Vector2(1920,1080));
			m_pGameApp = new cMusicGameApp((HWND)m_pTargetControl->Handle.ToPointer(),Vector2(4096,1530));
			//m_pGameApp = new cGameApp((HWND)m_pTargetControl->Handle.ToPointer());
			m_pGameApp->EditorInit();
			m_pSoundCapture = m_pGameApp->m_pSoundCapture;
			m_pSoundFFTCapture = m_pGameApp->m_pSoundFFTCapture;

			//m_pSoundCapture->AddSoundRecord();
			m_HdcMV = GetDC((HWND)m_pTargetControl->Handle.ToPointer());
			m_HGLRCMV = m_pGameApp->m_sHGLRC;
			//
			this->m_pWaveInfo = nullptr;
			//this->m_pWavWaves = nullptr;
			//
			m_pKissFFTConvertBase = nullptr;
			//m_pKissFFTConvertBase = new cKissFFTConvert();
			//m_pKissFFTConvertBase = new cKissFFTStreamingConvert();
			//for mouse event
			m_pTargetControl->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseMove);
			m_pTargetControl->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseDown);
			m_pTargetControl->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseUp);
			m_pTargetControl->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseUp);
			m_pTargetControl->SizeChanged += gcnew System::EventHandler(this, &Form1::MySizeChanged);


			AssignDragEnterAndDropEventToTextboxWithFileName(RecordSoundFileName_textBox,this->Controls);

			std::vector<std::string>l_strDeviceList = cSoundCapture::GetAvalibeRecordDevice();
			for(auto l_strDeviceName :l_strDeviceList)
			{
				cGameApp::OutputDebugInfoString(l_strDeviceName.c_str());
			}
			cSoundCompareParameter::m_siDebugAmplitudeValue = (int)FFTStoreAmplitude_numericUpDown->Value;
			this->timer1->Enabled = true;
			//
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
			SAFE_DELETE(m_pKissFFTConvertBase);
			//SAFE_DELETE(this->m_pWavWaves);

			SAFE_DELETE(m_pGameApp);
		}
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Timer^  timer1;
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

		//my
		cMusicGameApp*				m_pGameApp;
		cSoundCapture*				m_pSoundCapture;
		cSoundFFTCapture*			m_pSoundFFTCapture;
		//
		cKissFFTConvertBase*		m_pKissFFTConvertBase;
		//
		HDC							m_HdcMV;
		HGLRC						m_HGLRCMV;
		FATMING_CORE::cSoundFile*	m_pWaveInfo;
		//cWavWaves*					m_pWavWaves;
		Control^					m_pTargetControl;
		//
		System::Drawing::Size		m_MyOpenGLWindowSize;
		System::Drawing::Size		m_WindowStartSize;
private: System::Windows::Forms::Label^  SoundCaptureHint_label;




		 bool						m_bTimerUpdateEnd;
		void						WaitUpdateEnd(){ while(!m_bTimerUpdateEnd){} }
		//

	private: System::Windows::Forms::TextBox^  WavFileName_textBox;
	private: System::Windows::Forms::Panel^  OPENGL_panel;
	private: System::Windows::Forms::TrackBar^  CurrentTime_trackBar;
	private: System::Windows::Forms::Label^  CurrentTime_label;
	private: System::Windows::Forms::Button^  RecordSound_button;
	private: System::Windows::Forms::Button^  RecordSoundPause_button;
	private: System::Windows::Forms::TextBox^  RecordSoundFileName_textBox;
	private: System::Windows::Forms::Button^  FFTTest_button;
	private: System::Windows::Forms::Button^  ConvertToWav_button;
	private: System::Windows::Forms::Button^  ToOgg_button;
	private: System::Windows::Forms::Button^  PlayOgg_button;
	private: System::Windows::Forms::Button^  PlayWav_button;


	private: System::Windows::Forms::CheckBox^  FFTTestPause_checkBox;
	private: System::Windows::Forms::Button^  WavToOggOnlyOneChannel;
	private: System::Windows::Forms::TrackBar^  GoToTime_trackBar;

	private: System::Windows::Forms::Label^  JumpToTime_label;
	private: System::Windows::Forms::Label^  SourceName_label;
	private: System::Windows::Forms::Label^  ConvertToName_label;
	private: System::Windows::Forms::GroupBox^  SoundFileConvert_groupBox;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::GroupBox^  SoundCapture_groupBox;
	private: System::Windows::Forms::Label^  ChartScale_label;
	private: System::Windows::Forms::NumericUpDown^  FFTChartScale_numericUpDown;


















	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  FFT_FPS_label;
	private: System::Windows::Forms::NumericUpDown^  FFT_FPS_numericUpDown;
	private: System::Windows::Forms::Button^  MIDIToXml_button;
	private: System::Windows::Forms::Button^  ForwardStep_button;
	private: System::Windows::Forms::Button^  BackStep_button;
	private: System::Windows::Forms::GroupBox^  FFTPrint_groupBox;
	private: System::Windows::Forms::ListBox^  FFTStore_listBox;
	private: System::Windows::Forms::Button^  OpenFileForFFTStore_button;
	private: System::Windows::Forms::Label^  FFTStoreAmplitude_label;
	private: System::Windows::Forms::Label^  FFTStoreGittedCountThreshold_label;
	private: System::Windows::Forms::NumericUpDown^  FFTStoreGittedCountThreshold_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  FFTStoreAmplitude_numericUpDown;
	private: System::Windows::Forms::Button^  FFTStoreExport_button;
	private: System::Windows::Forms::CheckBox^  FFTStore_checkBox;
	private: System::Windows::Forms::Button^  CpatureSound_button;
	private: System::Windows::Forms::Label^  CurrentRecordName_label;
	private: System::Windows::Forms::TextBox^  CurrentRecordName_textBox;


	private: System::Windows::Forms::GroupBox^  Filter_groupBox;
	private: System::Windows::Forms::GroupBox^  FFT_groupBox;
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->WavFileName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->OPENGL_panel = (gcnew System::Windows::Forms::Panel());
			this->CurrentTime_trackBar = (gcnew System::Windows::Forms::TrackBar());
			this->CurrentTime_label = (gcnew System::Windows::Forms::Label());
			this->RecordSound_button = (gcnew System::Windows::Forms::Button());
			this->RecordSoundPause_button = (gcnew System::Windows::Forms::Button());
			this->RecordSoundFileName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->FFTTest_button = (gcnew System::Windows::Forms::Button());
			this->ConvertToWav_button = (gcnew System::Windows::Forms::Button());
			this->ToOgg_button = (gcnew System::Windows::Forms::Button());
			this->PlayOgg_button = (gcnew System::Windows::Forms::Button());
			this->PlayWav_button = (gcnew System::Windows::Forms::Button());
			this->FFTTestPause_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->WavToOggOnlyOneChannel = (gcnew System::Windows::Forms::Button());
			this->GoToTime_trackBar = (gcnew System::Windows::Forms::TrackBar());
			this->JumpToTime_label = (gcnew System::Windows::Forms::Label());
			this->SourceName_label = (gcnew System::Windows::Forms::Label());
			this->ConvertToName_label = (gcnew System::Windows::Forms::Label());
			this->SoundFileConvert_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->FFT_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->FFT_FPS_label = (gcnew System::Windows::Forms::Label());
			this->FFT_FPS_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->ChartScale_label = (gcnew System::Windows::Forms::Label());
			this->FFTChartScale_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->ForwardStep_button = (gcnew System::Windows::Forms::Button());
			this->BackStep_button = (gcnew System::Windows::Forms::Button());
			this->FFTStoreExport_button = (gcnew System::Windows::Forms::Button());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->Filter_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->FFTStoreAmplitude_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->FFTStoreGittedCountThreshold_label = (gcnew System::Windows::Forms::Label());
			this->FFTStoreAmplitude_label = (gcnew System::Windows::Forms::Label());
			this->FFTStoreGittedCountThreshold_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->FFTPrint_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->FFTStore_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->FFTStore_listBox = (gcnew System::Windows::Forms::ListBox());
			this->OpenFileForFFTStore_button = (gcnew System::Windows::Forms::Button());
			this->MIDIToXml_button = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SoundCapture_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->CpatureSound_button = (gcnew System::Windows::Forms::Button());
			this->CurrentRecordName_label = (gcnew System::Windows::Forms::Label());
			this->CurrentRecordName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->SoundCaptureHint_label = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CurrentTime_trackBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GoToTime_trackBar))->BeginInit();
			this->SoundFileConvert_groupBox->SuspendLayout();
			this->FFT_groupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFT_FPS_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTChartScale_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->Filter_groupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTStoreAmplitude_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTStoreGittedCountThreshold_numericUpDown))->BeginInit();
			this->FFTPrint_groupBox->SuspendLayout();
			this->SoundCapture_groupBox->SuspendLayout();
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// WavFileName_textBox
			// 
			this->WavFileName_textBox->AllowDrop = true;
			this->WavFileName_textBox->Location = System::Drawing::Point(309, 36);
			this->WavFileName_textBox->Name = L"WavFileName_textBox";
			this->WavFileName_textBox->Size = System::Drawing::Size(178, 20);
			this->WavFileName_textBox->TabIndex = 1;
			this->WavFileName_textBox->Text = L"Sound/2owl.wav";
			this->WavFileName_textBox->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::WavFileName_textBox_DragDrop);
			this->WavFileName_textBox->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::WavFileName_textBox_DragEnter);
			// 
			// OPENGL_panel
			// 
			this->OPENGL_panel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->OPENGL_panel->Location = System::Drawing::Point(0, 0);
			this->OPENGL_panel->Name = L"OPENGL_panel";
			this->OPENGL_panel->Size = System::Drawing::Size(1884, 604);
			this->OPENGL_panel->TabIndex = 3;
			// 
			// CurrentTime_trackBar
			// 
			this->CurrentTime_trackBar->Location = System::Drawing::Point(5, 124);
			this->CurrentTime_trackBar->Name = L"CurrentTime_trackBar";
			this->CurrentTime_trackBar->Size = System::Drawing::Size(274, 45);
			this->CurrentTime_trackBar->TabIndex = 5;
			this->CurrentTime_trackBar->Value = 10;
			// 
			// CurrentTime_label
			// 
			this->CurrentTime_label->AutoSize = true;
			this->CurrentTime_label->Location = System::Drawing::Point(6, 97);
			this->CurrentTime_label->Name = L"CurrentTime_label";
			this->CurrentTime_label->Size = System::Drawing::Size(64, 13);
			this->CurrentTime_label->TabIndex = 6;
			this->CurrentTime_label->Text = L"CurrentTime";
			// 
			// RecordSound_button
			// 
			this->RecordSound_button->Location = System::Drawing::Point(4, 28);
			this->RecordSound_button->Name = L"RecordSound_button";
			this->RecordSound_button->Size = System::Drawing::Size(111, 34);
			this->RecordSound_button->TabIndex = 15;
			this->RecordSound_button->Text = L"RecordSound";
			this->RecordSound_button->UseVisualStyleBackColor = true;
			this->RecordSound_button->Click += gcnew System::EventHandler(this, &Form1::RecordSound_button_Click);
			// 
			// RecordSoundPause_button
			// 
			this->RecordSoundPause_button->Location = System::Drawing::Point(4, 69);
			this->RecordSoundPause_button->Name = L"RecordSoundPause_button";
			this->RecordSoundPause_button->Size = System::Drawing::Size(111, 33);
			this->RecordSoundPause_button->TabIndex = 16;
			this->RecordSoundPause_button->Text = L"RecordSoundPause";
			this->RecordSoundPause_button->UseVisualStyleBackColor = true;
			this->RecordSoundPause_button->Click += gcnew System::EventHandler(this, &Form1::RecordSoundPause_button_Click);
			// 
			// RecordSoundFileName_textBox
			// 
			this->RecordSoundFileName_textBox->Location = System::Drawing::Point(9, 32);
			this->RecordSoundFileName_textBox->Name = L"RecordSoundFileName_textBox";
			this->RecordSoundFileName_textBox->Size = System::Drawing::Size(162, 20);
			this->RecordSoundFileName_textBox->TabIndex = 17;
			this->RecordSoundFileName_textBox->Text = L"Sound\\21.wav";
			// 
			// FFTTest_button
			// 
			this->FFTTest_button->Location = System::Drawing::Point(312, 78);
			this->FFTTest_button->Name = L"FFTTest_button";
			this->FFTTest_button->Size = System::Drawing::Size(75, 23);
			this->FFTTest_button->TabIndex = 18;
			this->FFTTest_button->Text = L"Play FFT";
			this->FFTTest_button->UseVisualStyleBackColor = true;
			this->FFTTest_button->Click += gcnew System::EventHandler(this, &Form1::FFTTest_button_Click);
			// 
			// ConvertToWav_button
			// 
			this->ConvertToWav_button->Location = System::Drawing::Point(83, 92);
			this->ConvertToWav_button->Name = L"ConvertToWav_button";
			this->ConvertToWav_button->Size = System::Drawing::Size(59, 26);
			this->ConvertToWav_button->TabIndex = 19;
			this->ConvertToWav_button->Text = L"ToWav";
			this->ConvertToWav_button->UseVisualStyleBackColor = true;
			this->ConvertToWav_button->Click += gcnew System::EventHandler(this, &Form1::ToOgg_button_Click);
			// 
			// ToOgg_button
			// 
			this->ToOgg_button->Location = System::Drawing::Point(9, 89);
			this->ToOgg_button->Name = L"ToOgg_button";
			this->ToOgg_button->Size = System::Drawing::Size(59, 26);
			this->ToOgg_button->TabIndex = 20;
			this->ToOgg_button->Text = L"ToOgg";
			this->ToOgg_button->UseVisualStyleBackColor = true;
			this->ToOgg_button->Click += gcnew System::EventHandler(this, &Form1::ToOgg_button_Click);
			// 
			// PlayOgg_button
			// 
			this->PlayOgg_button->Location = System::Drawing::Point(1285, 644);
			this->PlayOgg_button->Name = L"PlayOgg_button";
			this->PlayOgg_button->Size = System::Drawing::Size(85, 34);
			this->PlayOgg_button->TabIndex = 21;
			this->PlayOgg_button->Text = L"play ogg ";
			this->PlayOgg_button->UseVisualStyleBackColor = true;
			this->PlayOgg_button->Click += gcnew System::EventHandler(this, &Form1::PlayOgg_button_Click);
			// 
			// PlayWav_button
			// 
			this->PlayWav_button->Location = System::Drawing::Point(1285, 684);
			this->PlayWav_button->Name = L"PlayWav_button";
			this->PlayWav_button->Size = System::Drawing::Size(75, 32);
			this->PlayWav_button->TabIndex = 22;
			this->PlayWav_button->Text = L"PlayWav";
			this->PlayWav_button->UseVisualStyleBackColor = true;
			this->PlayWav_button->Click += gcnew System::EventHandler(this, &Form1::PlayWav_button_Click);
			// 
			// FFTTestPause_checkBox
			// 
			this->FFTTestPause_checkBox->AutoSize = true;
			this->FFTTestPause_checkBox->Location = System::Drawing::Point(312, 120);
			this->FFTTestPause_checkBox->Name = L"FFTTestPause_checkBox";
			this->FFTTestPause_checkBox->Size = System::Drawing::Size(56, 17);
			this->FFTTestPause_checkBox->TabIndex = 25;
			this->FFTTestPause_checkBox->Text = L"Pause";
			this->FFTTestPause_checkBox->UseVisualStyleBackColor = true;
			this->FFTTestPause_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::FFTTestPause_checkBox_CheckedChanged);
			// 
			// WavToOggOnlyOneChannel
			// 
			this->WavToOggOnlyOneChannel->Location = System::Drawing::Point(9, 66);
			this->WavToOggOnlyOneChannel->Name = L"WavToOggOnlyOneChannel";
			this->WavToOggOnlyOneChannel->Size = System::Drawing::Size(162, 26);
			this->WavToOggOnlyOneChannel->TabIndex = 26;
			this->WavToOggOnlyOneChannel->Text = L"WavToOggOnlyOneChannel";
			this->WavToOggOnlyOneChannel->UseVisualStyleBackColor = true;
			this->WavToOggOnlyOneChannel->Click += gcnew System::EventHandler(this, &Form1::WavToOggOnlyOneChannel_button_Click);
			// 
			// GoToTime_trackBar
			// 
			this->GoToTime_trackBar->Location = System::Drawing::Point(9, 206);
			this->GoToTime_trackBar->Name = L"GoToTime_trackBar";
			this->GoToTime_trackBar->Size = System::Drawing::Size(274, 45);
			this->GoToTime_trackBar->TabIndex = 27;
			this->GoToTime_trackBar->Value = 10;
			this->GoToTime_trackBar->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::GoToTime_trackBar_MouseUp);
			// 
			// JumpToTime_label
			// 
			this->JumpToTime_label->AutoSize = true;
			this->JumpToTime_label->Location = System::Drawing::Point(6, 177);
			this->JumpToTime_label->Name = L"JumpToTime_label";
			this->JumpToTime_label->Size = System::Drawing::Size(68, 13);
			this->JumpToTime_label->TabIndex = 29;
			this->JumpToTime_label->Text = L"JumpToTime";
			// 
			// SourceName_label
			// 
			this->SourceName_label->AutoSize = true;
			this->SourceName_label->Location = System::Drawing::Point(306, 9);
			this->SourceName_label->Name = L"SourceName_label";
			this->SourceName_label->Size = System::Drawing::Size(159, 13);
			this->SourceName_label->TabIndex = 30;
			this->SourceName_label->Text = L"SoundSourceName(ogg or wav)";
			// 
			// ConvertToName_label
			// 
			this->ConvertToName_label->AutoSize = true;
			this->ConvertToName_label->Location = System::Drawing::Point(6, 16);
			this->ConvertToName_label->Name = L"ConvertToName_label";
			this->ConvertToName_label->Size = System::Drawing::Size(85, 13);
			this->ConvertToName_label->TabIndex = 31;
			this->ConvertToName_label->Text = L"ConvertToName";
			// 
			// SoundFileConvert_groupBox
			// 
			this->SoundFileConvert_groupBox->Controls->Add(this->ConvertToName_label);
			this->SoundFileConvert_groupBox->Controls->Add(this->RecordSoundFileName_textBox);
			this->SoundFileConvert_groupBox->Controls->Add(this->ToOgg_button);
			this->SoundFileConvert_groupBox->Controls->Add(this->WavToOggOnlyOneChannel);
			this->SoundFileConvert_groupBox->Controls->Add(this->ConvertToWav_button);
			this->SoundFileConvert_groupBox->Location = System::Drawing::Point(908, 16);
			this->SoundFileConvert_groupBox->Name = L"SoundFileConvert_groupBox";
			this->SoundFileConvert_groupBox->Size = System::Drawing::Size(175, 120);
			this->SoundFileConvert_groupBox->TabIndex = 33;
			this->SoundFileConvert_groupBox->TabStop = false;
			this->SoundFileConvert_groupBox->Text = L"SoundFileConvert";
			this->SoundFileConvert_groupBox->Visible = false;
			// 
			// FFT_groupBox
			// 
			this->FFT_groupBox->Controls->Add(this->FFT_FPS_label);
			this->FFT_groupBox->Controls->Add(this->FFT_FPS_numericUpDown);
			this->FFT_groupBox->Controls->Add(this->ChartScale_label);
			this->FFT_groupBox->Controls->Add(this->FFTStoreExport_button);
			this->FFT_groupBox->Controls->Add(this->FFTChartScale_numericUpDown);
			this->FFT_groupBox->Controls->Add(this->FFTTest_button);
			this->FFT_groupBox->Controls->Add(this->SourceName_label);
			this->FFT_groupBox->Controls->Add(this->CurrentTime_trackBar);
			this->FFT_groupBox->Controls->Add(this->JumpToTime_label);
			this->FFT_groupBox->Controls->Add(this->WavFileName_textBox);
			this->FFT_groupBox->Controls->Add(this->CurrentTime_label);
			this->FFT_groupBox->Controls->Add(this->ForwardStep_button);
			this->FFT_groupBox->Controls->Add(this->GoToTime_trackBar);
			this->FFT_groupBox->Controls->Add(this->BackStep_button);
			this->FFT_groupBox->Controls->Add(this->FFTTestPause_checkBox);
			this->FFT_groupBox->Location = System::Drawing::Point(3, 7);
			this->FFT_groupBox->Name = L"FFT_groupBox";
			this->FFT_groupBox->Size = System::Drawing::Size(490, 285);
			this->FFT_groupBox->TabIndex = 34;
			this->FFT_groupBox->TabStop = false;
			this->FFT_groupBox->Text = L"FFT";
			// 
			// FFT_FPS_label
			// 
			this->FFT_FPS_label->AutoSize = true;
			this->FFT_FPS_label->Location = System::Drawing::Point(9, 24);
			this->FFT_FPS_label->Name = L"FFT_FPS_label";
			this->FFT_FPS_label->Size = System::Drawing::Size(52, 13);
			this->FFT_FPS_label->TabIndex = 39;
			this->FFT_FPS_label->Text = L"FFT_FPS";
			// 
			// FFT_FPS_numericUpDown
			// 
			this->FFT_FPS_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->FFT_FPS_numericUpDown->Location = System::Drawing::Point(10, 43);
			this->FFT_FPS_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {60, 0, 0, 0});
			this->FFT_FPS_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {7, 0, 0, 0});
			this->FFT_FPS_numericUpDown->Name = L"FFT_FPS_numericUpDown";
			this->FFT_FPS_numericUpDown->Size = System::Drawing::Size(94, 20);
			this->FFT_FPS_numericUpDown->TabIndex = 38;
			this->FFT_FPS_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {22, 0, 0, 0});
			this->FFT_FPS_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::FFT_FPS_numericUpDown_ValueChanged);
			// 
			// ChartScale_label
			// 
			this->ChartScale_label->AutoSize = true;
			this->ChartScale_label->Location = System::Drawing::Point(309, 156);
			this->ChartScale_label->Name = L"ChartScale_label";
			this->ChartScale_label->Size = System::Drawing::Size(59, 13);
			this->ChartScale_label->TabIndex = 35;
			this->ChartScale_label->Text = L"ChartScale";
			// 
			// FFTChartScale_numericUpDown
			// 
			this->FFTChartScale_numericUpDown->DecimalPlaces = 2;
			this->FFTChartScale_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->FFTChartScale_numericUpDown->Location = System::Drawing::Point(312, 175);
			this->FFTChartScale_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->FFTChartScale_numericUpDown->Name = L"FFTChartScale_numericUpDown";
			this->FFTChartScale_numericUpDown->Size = System::Drawing::Size(120, 20);
			this->FFTChartScale_numericUpDown->TabIndex = 34;
			this->FFTChartScale_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->FFTChartScale_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::FFTChartScale_numericUpDown_ValueChanged);
			// 
			// ForwardStep_button
			// 
			this->ForwardStep_button->Location = System::Drawing::Point(195, 248);
			this->ForwardStep_button->Name = L"ForwardStep_button";
			this->ForwardStep_button->Size = System::Drawing::Size(75, 23);
			this->ForwardStep_button->TabIndex = 10;
			this->ForwardStep_button->Text = L">>";
			this->ForwardStep_button->UseVisualStyleBackColor = true;
			this->ForwardStep_button->Visible = false;
			this->ForwardStep_button->Click += gcnew System::EventHandler(this, &Form1::BackStep_button_Click);
			// 
			// BackStep_button
			// 
			this->BackStep_button->Location = System::Drawing::Point(9, 248);
			this->BackStep_button->Name = L"BackStep_button";
			this->BackStep_button->Size = System::Drawing::Size(75, 23);
			this->BackStep_button->TabIndex = 12;
			this->BackStep_button->Text = L"<<";
			this->BackStep_button->UseVisualStyleBackColor = true;
			this->BackStep_button->Visible = false;
			this->BackStep_button->Click += gcnew System::EventHandler(this, &Form1::BackStep_button_Click);
			// 
			// FFTStoreExport_button
			// 
			this->FFTStoreExport_button->Location = System::Drawing::Point(312, 206);
			this->FFTStoreExport_button->Name = L"FFTStoreExport_button";
			this->FFTStoreExport_button->Size = System::Drawing::Size(91, 20);
			this->FFTStoreExport_button->TabIndex = 2;
			this->FFTStoreExport_button->Text = L"FFTStoreExport";
			this->FFTStoreExport_button->UseVisualStyleBackColor = true;
			this->FFTStoreExport_button->Visible = false;
			this->FFTStoreExport_button->Click += gcnew System::EventHandler(this, &Form1::FFTStoreExport_button_Click);
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			this->splitContainer1->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->OPENGL_panel);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->Filter_groupBox);
			this->splitContainer1->Panel2->Controls->Add(this->FFTPrint_groupBox);
			this->splitContainer1->Panel2->Controls->Add(this->MIDIToXml_button);
			this->splitContainer1->Panel2->Controls->Add(this->label1);
			this->splitContainer1->Panel2->Controls->Add(this->SoundCapture_groupBox);
			this->splitContainer1->Panel2->Controls->Add(this->FFT_groupBox);
			this->splitContainer1->Panel2->Controls->Add(this->SoundFileConvert_groupBox);
			this->splitContainer1->Size = System::Drawing::Size(1884, 961);
			this->splitContainer1->SplitterDistance = 604;
			this->splitContainer1->TabIndex = 35;
			// 
			// Filter_groupBox
			// 
			this->Filter_groupBox->Controls->Add(this->FFTStoreAmplitude_numericUpDown);
			this->Filter_groupBox->Controls->Add(this->FFTStoreGittedCountThreshold_label);
			this->Filter_groupBox->Controls->Add(this->FFTStoreAmplitude_label);
			this->Filter_groupBox->Controls->Add(this->FFTStoreGittedCountThreshold_numericUpDown);
			this->Filter_groupBox->Location = System::Drawing::Point(499, 16);
			this->Filter_groupBox->Name = L"Filter_groupBox";
			this->Filter_groupBox->Size = System::Drawing::Size(206, 134);
			this->Filter_groupBox->TabIndex = 53;
			this->Filter_groupBox->TabStop = false;
			this->Filter_groupBox->Text = L"Decibels Value";
			this->Filter_groupBox->Visible = false;
			// 
			// FFTStoreAmplitude_numericUpDown
			// 
			this->FFTStoreAmplitude_numericUpDown->Location = System::Drawing::Point(9, 42);
			this->FFTStoreAmplitude_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {9999, 0, 0, 0});
			this->FFTStoreAmplitude_numericUpDown->Name = L"FFTStoreAmplitude_numericUpDown";
			this->FFTStoreAmplitude_numericUpDown->Size = System::Drawing::Size(101, 20);
			this->FFTStoreAmplitude_numericUpDown->TabIndex = 3;
			this->FFTStoreAmplitude_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {15, 0, 0, 0});
			this->FFTStoreAmplitude_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::FFTStoreAmplitude_numericUpDown_ValueChanged);
			// 
			// FFTStoreGittedCountThreshold_label
			// 
			this->FFTStoreGittedCountThreshold_label->AutoSize = true;
			this->FFTStoreGittedCountThreshold_label->Location = System::Drawing::Point(6, 69);
			this->FFTStoreGittedCountThreshold_label->Name = L"FFTStoreGittedCountThreshold_label";
			this->FFTStoreGittedCountThreshold_label->Size = System::Drawing::Size(113, 13);
			this->FFTStoreGittedCountThreshold_label->TabIndex = 48;
			this->FFTStoreGittedCountThreshold_label->Text = L"FFT Average Decibels";
			// 
			// FFTStoreAmplitude_label
			// 
			this->FFTStoreAmplitude_label->AutoSize = true;
			this->FFTStoreAmplitude_label->Location = System::Drawing::Point(6, 26);
			this->FFTStoreAmplitude_label->Name = L"FFTStoreAmplitude_label";
			this->FFTStoreAmplitude_label->Size = System::Drawing::Size(70, 13);
			this->FFTStoreAmplitude_label->TabIndex = 49;
			this->FFTStoreAmplitude_label->Text = L"FFT Decibels";
			// 
			// FFTStoreGittedCountThreshold_numericUpDown
			// 
			this->FFTStoreGittedCountThreshold_numericUpDown->Location = System::Drawing::Point(8, 85);
			this->FFTStoreGittedCountThreshold_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {9999, 0, 
				0, 0});
			this->FFTStoreGittedCountThreshold_numericUpDown->Name = L"FFTStoreGittedCountThreshold_numericUpDown";
			this->FFTStoreGittedCountThreshold_numericUpDown->Size = System::Drawing::Size(101, 20);
			this->FFTStoreGittedCountThreshold_numericUpDown->TabIndex = 4;
			this->FFTStoreGittedCountThreshold_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 
				0});
			this->FFTStoreGittedCountThreshold_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::FFTStoreAmplitude_numericUpDown_ValueChanged);
			// 
			// FFTPrint_groupBox
			// 
			this->FFTPrint_groupBox->Controls->Add(this->FFTStore_checkBox);
			this->FFTPrint_groupBox->Controls->Add(this->FFTStore_listBox);
			this->FFTPrint_groupBox->Controls->Add(this->OpenFileForFFTStore_button);
			this->FFTPrint_groupBox->Location = System::Drawing::Point(1089, 23);
			this->FFTPrint_groupBox->Name = L"FFTPrint_groupBox";
			this->FFTPrint_groupBox->Size = System::Drawing::Size(166, 323);
			this->FFTPrint_groupBox->TabIndex = 41;
			this->FFTPrint_groupBox->TabStop = false;
			this->FFTPrint_groupBox->Text = L"FFTPrint";
			this->FFTPrint_groupBox->Visible = false;
			// 
			// FFTStore_checkBox
			// 
			this->FFTStore_checkBox->AutoSize = true;
			this->FFTStore_checkBox->Location = System::Drawing::Point(6, 61);
			this->FFTStore_checkBox->Name = L"FFTStore_checkBox";
			this->FFTStore_checkBox->Size = System::Drawing::Size(105, 17);
			this->FFTStore_checkBox->TabIndex = 50;
			this->FFTStore_checkBox->Text = L"FFTStoreRender";
			this->FFTStore_checkBox->UseVisualStyleBackColor = true;
			this->FFTStore_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::FFTStore_checkBox_CheckedChanged);
			// 
			// FFTStore_listBox
			// 
			this->FFTStore_listBox->FormattingEnabled = true;
			this->FFTStore_listBox->Location = System::Drawing::Point(6, 102);
			this->FFTStore_listBox->Name = L"FFTStore_listBox";
			this->FFTStore_listBox->Size = System::Drawing::Size(129, 212);
			this->FFTStore_listBox->TabIndex = 1;
			this->FFTStore_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::FFTStore_listBox_SelectedIndexChanged);
			// 
			// OpenFileForFFTStore_button
			// 
			this->OpenFileForFFTStore_button->Location = System::Drawing::Point(6, 18);
			this->OpenFileForFFTStore_button->Name = L"OpenFileForFFTStore_button";
			this->OpenFileForFFTStore_button->Size = System::Drawing::Size(129, 23);
			this->OpenFileForFFTStore_button->TabIndex = 0;
			this->OpenFileForFFTStore_button->Text = L"OpenFileForFFTStore";
			this->OpenFileForFFTStore_button->UseVisualStyleBackColor = true;
			this->OpenFileForFFTStore_button->Click += gcnew System::EventHandler(this, &Form1::OpenFileForFFTStore_button_Click);
			// 
			// MIDIToXml_button
			// 
			this->MIDIToXml_button->Location = System::Drawing::Point(917, 158);
			this->MIDIToXml_button->Name = L"MIDIToXml_button";
			this->MIDIToXml_button->Size = System::Drawing::Size(116, 44);
			this->MIDIToXml_button->TabIndex = 40;
			this->MIDIToXml_button->Text = L"MIDIToXml";
			this->MIDIToXml_button->UseVisualStyleBackColor = true;
			this->MIDIToXml_button->Visible = false;
			this->MIDIToXml_button->Click += gcnew System::EventHandler(this, &Form1::MIDIToXml_button_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(1408, 8);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(464, 390);
			this->label1->TabIndex = 38;
			this->label1->Text = resources->GetString(L"label1.Text");
			this->label1->Visible = false;
			// 
			// SoundCapture_groupBox
			// 
			this->SoundCapture_groupBox->Controls->Add(this->SoundCaptureHint_label);
			this->SoundCapture_groupBox->Controls->Add(this->CpatureSound_button);
			this->SoundCapture_groupBox->Controls->Add(this->CurrentRecordName_label);
			this->SoundCapture_groupBox->Controls->Add(this->CurrentRecordName_textBox);
			this->SoundCapture_groupBox->Controls->Add(this->RecordSound_button);
			this->SoundCapture_groupBox->Controls->Add(this->RecordSoundPause_button);
			this->SoundCapture_groupBox->Location = System::Drawing::Point(740, 3);
			this->SoundCapture_groupBox->Name = L"SoundCapture_groupBox";
			this->SoundCapture_groupBox->Size = System::Drawing::Size(125, 302);
			this->SoundCapture_groupBox->TabIndex = 36;
			this->SoundCapture_groupBox->TabStop = false;
			this->SoundCapture_groupBox->Text = L"SoundCapture";
			// 
			// CpatureSound_button
			// 
			this->CpatureSound_button->Location = System::Drawing::Point(7, 111);
			this->CpatureSound_button->Name = L"CpatureSound_button";
			this->CpatureSound_button->Size = System::Drawing::Size(108, 36);
			this->CpatureSound_button->TabIndex = 51;
			this->CpatureSound_button->Text = L"Export selected decibles index";
			this->CpatureSound_button->UseVisualStyleBackColor = true;
			this->CpatureSound_button->Click += gcnew System::EventHandler(this, &Form1::CpatureSound_button_Click);
			// 
			// CurrentRecordName_label
			// 
			this->CurrentRecordName_label->AutoSize = true;
			this->CurrentRecordName_label->Location = System::Drawing::Point(4, 171);
			this->CurrentRecordName_label->Name = L"CurrentRecordName_label";
			this->CurrentRecordName_label->Size = System::Drawing::Size(81, 13);
			this->CurrentRecordName_label->TabIndex = 41;
			this->CurrentRecordName_label->Text = L"ExportFileName";
			// 
			// CurrentRecordName_textBox
			// 
			this->CurrentRecordName_textBox->AllowDrop = true;
			this->CurrentRecordName_textBox->Location = System::Drawing::Point(7, 194);
			this->CurrentRecordName_textBox->Name = L"CurrentRecordName_textBox";
			this->CurrentRecordName_textBox->Size = System::Drawing::Size(108, 20);
			this->CurrentRecordName_textBox->TabIndex = 40;
			this->CurrentRecordName_textBox->Text = L"qoo.wav";
			// 
			// SoundCaptureHint_label
			// 
			this->SoundCaptureHint_label->AutoSize = true;
			this->SoundCaptureHint_label->Location = System::Drawing::Point(6, 223);
			this->SoundCaptureHint_label->Name = L"SoundCaptureHint_label";
			this->SoundCaptureHint_label->Size = System::Drawing::Size(93, 52);
			this->SoundCaptureHint_label->TabIndex = 40;
			this->SoundCaptureHint_label->Text = L"Z zoom in\r\nX zoom in\r\nC chart move left\r\nV chart move right";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1884, 961);
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->PlayWav_button);
			this->Controls->Add(this->PlayOgg_button);
			this->KeyPreview = true;
			this->Name = L"Form1";
			this->Text = L" FFT";
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::Form1_KeyDown);
			this->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::Form1_KeyPress);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::Form1_KeyUp);
			this->Resize += gcnew System::EventHandler(this, &Form1::Form1_Resize);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CurrentTime_trackBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GoToTime_trackBar))->EndInit();
			this->SoundFileConvert_groupBox->ResumeLayout(false);
			this->SoundFileConvert_groupBox->PerformLayout();
			this->FFT_groupBox->ResumeLayout(false);
			this->FFT_groupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFT_FPS_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTChartScale_numericUpDown))->EndInit();
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->Panel2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->Filter_groupBox->ResumeLayout(false);
			this->Filter_groupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTStoreAmplitude_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTStoreGittedCountThreshold_numericUpDown))->EndInit();
			this->FFTPrint_groupBox->ResumeLayout(false);
			this->FFTPrint_groupBox->PerformLayout();
			this->SoundCapture_groupBox->ResumeLayout(false);
			this->SoundCapture_groupBox->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
		System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
		{
			m_bTimerUpdateEnd = false;
			wglMakeCurrent( m_HdcMV,m_HGLRCMV );
			RECT rcClient;
			GetClientRect((HWND)m_pTargetControl->Handle.ToPointer(), &rcClient);
			UseShaderProgram();
			cGameApp::m_svViewPortSize.x = 0.f;
			cGameApp::m_svViewPortSize.y = 0.f;
			cGameApp::m_svViewPortSize.z = (float)m_pTargetControl->Width;
			cGameApp::m_svViewPortSize.w = (float)rcClient.bottom;
			this->m_pGameApp->EditorRun();
			//if( m_pWavWaves )
			{
				//m_pWavWaves->Render();
			}
			float l_fElpaseTime = this->m_pGameApp->m_sTimeAndFPS.fElpaseTime*this->m_pGameApp->m_sfGameSpeedValue;
			if( cGameApp::m_spSoundParser )
					cGameApp::m_spSoundParser->Update(l_fElpaseTime);
			if( m_pKissFFTConvertBase )
			{
				//m_pKissFFTConvertBase->SetChartShowPos(Vector2(300,300));
				//m_pKissFFTConvertBase->SetChartResolution(Vector2(2280,720));
				m_pKissFFTConvertBase->Update(l_fElpaseTime);
				m_pKissFFTConvertBase->Render();
				float l_fCurrentPercentage = m_pKissFFTConvertBase->GetCurrentTimePercentage();
				if( l_fCurrentPercentage > 0.f )
				{
					int l_iValue = (int)(CurrentTime_trackBar->Maximum * l_fCurrentPercentage);
					if( l_iValue >= CurrentTime_trackBar->Maximum)
						l_iValue = CurrentTime_trackBar->Maximum;
					CurrentTime_trackBar->Value = l_iValue;
					CurrentTime_label->Text = CurrentTime_trackBar->Value+"/"+(m_pKissFFTConvertBase->GetTimeLength()*10);
				}
				if( l_fCurrentPercentage >= 1.f )
					FFT_FPS_numericUpDown->Enabled = true;
			}
			SwapBuffers(cGameApp::m_sHdc);
			m_bTimerUpdateEnd = true;
		}

		System::Void MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			//this->Focus();
			if( !timer1->Enabled || !this->Visible )
				return;
			if( m_pGameApp )
				m_pGameApp->MouseDown(e->X,e->Y);
			if(m_pSoundFFTCapture)
				m_pSoundFFTCapture->MouseDown(m_pGameApp->m_sMousePosition.x,m_pGameApp->m_sMousePosition.y);
			if( m_pKissFFTConvertBase )
				m_pKissFFTConvertBase->MouseDown(m_pGameApp->m_sMousePosition.x,m_pGameApp->m_sMousePosition.y);
		}
		System::Void MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			if( !timer1->Enabled || !this->Visible )
				return;
			if( m_pGameApp )
				m_pGameApp->MouseMove(e->X,e->Y);
			if(m_pSoundFFTCapture)
				m_pSoundFFTCapture->MouseMove(m_pGameApp->m_sMousePosition.x,m_pGameApp->m_sMousePosition.y);
			if( m_pKissFFTConvertBase )
				m_pKissFFTConvertBase->MouseMove(m_pGameApp->m_sMousePosition.x,m_pGameApp->m_sMousePosition.y);
		}

		System::Void MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			if( !timer1->Enabled || !this->Visible )
				return;
			if( m_pGameApp )
				m_pGameApp->MouseUp(e->X,e->Y);
			if(m_pSoundFFTCapture)
				m_pSoundFFTCapture->MouseUp(m_pGameApp->m_sMousePosition.x,m_pGameApp->m_sMousePosition.y);
			if( m_pKissFFTConvertBase )
				m_pKissFFTConvertBase->MouseUp(m_pGameApp->m_sMousePosition.x,m_pGameApp->m_sMousePosition.y);
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
private: System::Void RecordSound_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( m_pSoundCapture )
			 {
				 if(!m_pSoundCapture->IsRecording())
				 {
					 std::string l_strFileName = ::GcStringToChar(this->RecordSoundFileName_textBox->Text);
					 //l_strFileName.c_str(),eCaptureSoundFileFormat::eCSFF_WAV
					 m_pSoundCapture->StartRecord();
					 //m_pSoundCapture->StartRecord(l_strFileName.c_str(),eCaptureSoundFileFormat::eCSFF_OGG);
					 if( m_pSoundCapture->IsRecording() )
						RecordSound_button->Text = "Recording...";
					 else
						 RecordSound_button->Text = "open device failed...";
				 }
				 else
				 {
					 this->timer1->Enabled = false;
					 Sleep(10);
					 m_pSoundCapture->StopRecord();
					 RecordSound_button->Text = "Stop!";
					 this->timer1->Enabled = true;
				 }
			 }
		 }
private: System::Void RecordSoundPause_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(m_pSoundCapture)
			 {
				 m_pSoundCapture->PauseRecord(!m_pSoundCapture->IsPause());
				 if( m_pSoundCapture->IsPause() )
					RecordSoundPause_button->Text = "Pause";
				 else
					RecordSoundPause_button->Text = "recording...";
			 }
		 }
private: System::Void FFTTest_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 this->timer1->Enabled = false;
			 if(System::IO::File::Exists(WavFileName_textBox->Text))
			 {
				 FFT_FPS_numericUpDown->Enabled = false;
				 SAFE_DELETE(m_pKissFFTConvertBase);
				 String^l_strExtensionName = System::IO::Path::GetExtension(WavFileName_textBox->Text);
				 if( l_strExtensionName->Equals(".wav") )
				 {
					m_pKissFFTConvertBase = new cKissFFTConvert();
				 }
				 else
				 if( l_strExtensionName->Equals(".ogg") )
				 {
					m_pKissFFTConvertBase = new cKissFFTStreamingConvert();
				 }
				 if( m_pKissFFTConvertBase )
				 {
					std::string l_strFileName = DNCT::GcStringToChar(WavFileName_textBox->Text);
					m_pKissFFTConvertBase->SetChartScale((float)FFTChartScale_numericUpDown->Value);
					//m_pKissFFTConvertBase->SetiFrenquenceFilterEndScaleValue((float)FFTFilterFrenquenceScale_numericUpDown->Value);
					//m_pKissFFTConvertBase->SetFilterStrengthValue((int)FilterStrength_numericUpDown->Value);
					if(m_pKissFFTConvertBase->FetchSoundDataStart(l_strFileName.c_str(),true,true))
					{
						CurrentTime_trackBar->Maximum = (int)(m_pKissFFTConvertBase->GetTimeLength()*10);
						GoToTime_trackBar->Maximum = CurrentTime_trackBar->Maximum;
					}
				 }
				 else
				 {
					 WARNING_MSG("only support wav and ogg,sorry!");
				 }
			 }
			 else
			 {
				WARNING_MSG("please give file name!");
			 }
			 cGameApp::m_sTimeAndFPS.Update();
			 this->timer1->Enabled = true;
		 }
private: System::Void BackStep_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( m_pKissFFTConvertBase )
			 {
				 if( sender == this->ForwardStep_button )
					m_pKissFFTConvertBase->Update(0.016f);
				 else
					m_pKissFFTConvertBase->Update(-0.016f);
			 }
		 }
private: System::Void ToOgg_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(System::IO::File::Exists(RecordSoundFileName_textBox->Text))
			 {
				std::string l_FileName = DNCT::GcStringToChar(this->RecordSoundFileName_textBox->Text);
				FATMING_CORE::cSoundFile*l_pSoundFile = new FATMING_CORE::cSoundFile();
				if( sender == ToOgg_button )
				{
					std::string l_strOutputFileName = UT::ChangeFileExtensionName(l_FileName.c_str(),"ogg");
					l_pSoundFile->WavToOggFile(l_FileName.c_str(),l_strOutputFileName.c_str());
				}
				else
				{
					std::string l_strOutputFileName = UT::ChangeFileExtensionName(l_FileName.c_str(),"wav");
					l_pSoundFile->OggToWavFile(l_FileName.c_str(),l_strOutputFileName.c_str());			 
				}
				SAFE_DELETE(l_pSoundFile);
			 }
		 }
private: System::Void PlayOgg_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 std::string l_FileName = DNCT::GcStringToChar(this->WavFileName_textBox->Text);
			 cOpanalOgg*l_pOpanalOgg = new cOpanalOgg(cGameApp::m_spSoundParser,l_FileName.c_str(),true);
			 cGameApp::m_spSoundParser->AddObject(l_pOpanalOgg);
			 l_pOpanalOgg->Play(true);
		 }
private: System::Void PlayWav_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {

		 }
private: System::Void FFTTestPause_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(m_pKissFFTConvertBase)
				 m_pKissFFTConvertBase->Pause(FFTTestPause_checkBox->Checked);

		 }
private: System::Void WavToOggOnlyOneChannel_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
				std::string l_FileName = DNCT::GcStringToChar(this->WavFileName_textBox->Text);
				FATMING_CORE::cSoundFile*l_pSoundFile = new FATMING_CORE::cSoundFile();
				//if( sender == ToOgg_button )
				{
					std::string l_strOutputFileName = UT::ChangeFileExtensionName(l_FileName.c_str(),"ogg");
					l_pSoundFile->WavToOggFileOnlyChannel(l_FileName.c_str(),l_strOutputFileName.c_str(),1);
					delete l_pSoundFile;
				}
		 }
private: System::Void GoToTime_trackBar_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			 if(m_pKissFFTConvertBase)
			 {
				this->timer1->Enabled = false;
				WaitUpdateEnd();
				int l_iTargetSecond = (int)GoToTime_trackBar->Value;
				m_pKissFFTConvertBase->GoToTime(l_iTargetSecond/10.f);
				this->timer1->Enabled = true;
			 }
			 
		 }


private: System::Void FFTChartScale_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( m_pKissFFTConvertBase )
				m_pKissFFTConvertBase->SetChartScale((float)FFTChartScale_numericUpDown->Value);
		 }


private: System::Void Form1_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 this->m_pGameApp->KeyUp((char)e->KeyCode);
			 
		 }
private: System::Void Form1_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e)
		 {
			 //this->m_pGameApp->KeyPress((char)e->KeyChar);
		 }
private: System::Void Form1_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 this->m_pGameApp->KeyDown((char)e->KeyCode);
		 }
private: System::Void FFT_FPS_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 ONE_FRAME_NEED_NUM_FFT_DATA_COUNT = (int)FFT_FPS_numericUpDown->Value;
		 }
private: System::Void DebugAmplitudeValue_NumericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 cSoundCompareParameter::m_siDebugAmplitudeValue = (int)FFTStoreAmplitude_numericUpDown->Value;
		 }
private: System::Void MIDIToXml_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
//<cSoundTimeLineDataCollection ToneDataFileName="MusicGame/Piano/Piano.xml" ChartResolution="1820,600" ChartShowPos="10,10" MoveDirection="UpToDown" BeforeTime="4" AfterTime="6">
//	<cSoundTimeLineData ID="a3" Time="0.5" TuneKeepTime="0.5" />
//	<cSoundTimeLineData ID="b3" Time="1.0" />
//	<cSoundTimeLineData ID="b3" Time="2.5" />
//	<cSoundTimeLineData ID="b3" Time="3.0" />
//	<cSoundTimeLineData ID="a3" Time="4.5" TuneKeepTime="2.5"/>
//	<cSoundTimeLineData ID="c3" Time="6.5" />
//</cSoundTimeLineDataCollection>			 
			 auto l_pFileName = DNCT::OpenFileAndGetNames("midi files (*.mid)|*.mid|All files (*.*)|*.*");
			 if(  l_pFileName )
			 {
				this->timer1->Enabled = false;
				int l_iCount = l_pFileName->Length;
				 for( int i=0;i<l_iCount;++i )
				 {
					ISAXCallback l_ISAXCallback;
					TiXmlDocument*l_pTiXmlDocument = new TiXmlDocument();
					TiXmlElement*l_pRootTiXmlElement = new TiXmlElement(L"cSoundTimeLineDataCollection");
					l_pTiXmlDocument->LinkEndChild(l_pRootTiXmlElement);
					l_ISAXCallback.SetDoc(l_pTiXmlDocument);
					l_pRootTiXmlElement->SetAttribute(L"ToneDataFileName",L"MusicGame/Piano/Piano.xml");
					l_pRootTiXmlElement->SetAttribute(L"ChartResolution",L"1920,600");
					l_pRootTiXmlElement->SetAttribute(L"ChartShowPos",L"10,10");
					l_pRootTiXmlElement->SetAttribute(L"MoveDirection",L"UpToDown");
					l_pRootTiXmlElement->SetAttribute(L"BeforeTime",L"4");
					l_pRootTiXmlElement->SetAttribute(L"AfterTime",L"6");
					auto l_gcstrFileName = l_pFileName[i];
					std::string l_strFileName = DNCT::GcStringToChar(l_gcstrFileName);
					//https://github.com/craigsapp/midifile/blob/462f55a906faa874a9b94fee18d4efd6945da7f8/src-programs/durations.cpp
					//https://github.com/craigsapp/midifile/issues/42
					MidiFile midifile;
					midifile.read(l_strFileName.c_str());
					int deltatick = 0;
					double l_dbDuration = 0;
					double l_dbStartTime = -1;
					midifile.absoluteTicks();
				    midifile.linkNotePairs();
				    midifile.joinTracks();
				    midifile.doTimeAnalysis();
					int tracks = midifile.getTrackCount();
					for (int l_iTrack = 0; l_iTrack < tracks; ++l_iTrack)
					{
						MidiEventList* l_pMidiEventList = &midifile[l_iTrack];
						for (int l_iEvent = 0; l_iEvent < l_pMidiEventList->size(); l_iEvent++)
						{
							MidiEvent*l_pMidiEvent = &(*l_pMidiEventList)[l_iEvent];
							int l_iKeyNumber = l_pMidiEvent->getKeyNumber();
							if ( l_pMidiEvent->isNoteOn() )
							{
								l_dbStartTime = l_pMidiEvent->seconds;
								l_dbDuration = l_pMidiEvent->getDurationInSeconds();
								TiXmlElement*l_pSoundTimeLineDataTiXmlElement = new TiXmlElement(L"cSoundTimeLineData");
								l_pSoundTimeLineDataTiXmlElement->SetAttribute(L"Time",(float)l_dbStartTime);
								l_pSoundTimeLineDataTiXmlElement->SetAttribute(L"ID",l_pMidiEvent->getKeyNumber());
								l_pSoundTimeLineDataTiXmlElement->SetAttribute(L"TuneKeepTime",(float)l_dbDuration);
								l_pRootTiXmlElement->LinkEndChild(l_pSoundTimeLineDataTiXmlElement);
							}
						}
					}
					String^l_gcstrOutputFileName = System::IO::Path::ChangeExtension(l_pFileName[i],".xml");
					std::string l_strOutputFileName = ::GcStringToChar(l_gcstrOutputFileName);
					l_ISAXCallback.Export(l_strOutputFileName.c_str(),false);
				 }
				 this->timer1->Enabled = true;
			 }
		 }
private: System::Void DebugAmplitudeLineValue_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {

		 }
private: System::Void OpenFileForFFTStore_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 this->timer1->Enabled = false;
			 this->m_pGameApp->m_pFFTStoreExporter->Destroy();
			 FFTStore_listBox->Items->Clear();
			 cli::array<String^>^ l_strFileNames = DNCT::OpenFileAndGetNames("wav files (*.wav)|*.wav|All files (*.*)|*.*");
			 for each(auto l_strFileName in l_strFileNames)
			 {
				 this->m_pGameApp->m_pFFTStoreExporter->AddFile(DNCT::GcStringToChar(l_strFileName).c_str());
				 FFTStore_listBox->Items->Add(System::IO::Path::GetFileName(l_strFileName));
			 }
			 this->timer1->Enabled = true;
		 }
private: System::Void FFTStore_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 this->m_pGameApp->m_pFFTStoreExporter->SetSelectedIndex( FFTStore_listBox->SelectedIndex);
		 }
private: System::Void FFTStoreAmplitude_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			if( FFTStore_listBox->SelectedIndex != -1 )
			{
				this->m_pGameApp->m_pFFTStoreExporter->GetObject(FFTStore_listBox->SelectedIndex)->GetFFTDataStore()->SetExportThresholdValue((int)FFTStoreGittedCountThreshold_numericUpDown->Value);
			}
			cSoundCompareParameter::m_siDebugAmplitudeValue = (int)FFTStoreAmplitude_numericUpDown->Value;
			cSoundCompareParameter::m_siFFTStoreThresholeValue = (int)FFTStoreGittedCountThreshold_numericUpDown->Value;
			if(m_pKissFFTConvertBase && m_pKissFFTConvertBase->GetFFTDataStore())
			{
				m_pKissFFTConvertBase->GetFFTDataStore()->SetExportThresholdValue((int)FFTStoreGittedCountThreshold_numericUpDown->Value);
			}
			 if( m_pSoundFFTCapture && m_pSoundFFTCapture->GetFFTDataStore() )
			 {
				 m_pSoundFFTCapture->GetFFTDataStore()->SetExportThresholdValue((int)FFTStoreGittedCountThreshold_numericUpDown->Value);
			 }
			
		 }
private: System::Void FFTStore_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(!FFTStore_checkBox->Checked)
				 this->m_pGameApp->m_pFFTStoreExporter->SetSelectedIndex(-1);
			 else
			 if( FFTStore_listBox->SelectedIndex != -1 )
			 {
				 this->m_pGameApp->m_pFFTStoreExporter->SetSelectedIndex( FFTStore_listBox->SelectedIndex);
			 }
		 }
private: System::Void WavFileName_textBox_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
		 {
			 WavFileName_textBox->Text = "";
			auto l_pData = DNCT::DragDropEventWhileFileDrop(e);
			for each(auto l_strFileName in l_pData )
			{
				WavFileName_textBox->Text += l_strFileName;
			}
		 }
private: System::Void WavFileName_textBox_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
		 {
			if (e->Data->GetDataPresent(DataFormats::FileDrop, false) == true)
			{
				e->Effect = DragDropEffects::Copy;
			}
		 }
private: System::Void FFTStoreExport_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(m_pKissFFTConvertBase)
			 {
				 std::string l_strFileName = DNCT::GcStringToChar(System::IO::Path::GetFileName(WavFileName_textBox->Text));
				 m_pKissFFTConvertBase->DumpDebugInfo(cSoundCompareParameter::m_siDebugAmplitudeValue,l_strFileName.c_str());
			 }
		 }
private: System::Void CpatureSound_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( m_pSoundFFTCapture )
			 {
				 String^l_strFileName = CurrentRecordName_textBox->Text;
				 if( l_strFileName->Length == 0 )
					 l_strFileName = "c4.wav";
				 std::string l_strOutName = DNCT::GcStringToChar(l_strFileName);
				 m_pSoundFFTCapture->DumpDebugInfo(cSoundCompareParameter::m_siDebugAmplitudeValue,l_strOutName.c_str());
			 }
		 }
};
}