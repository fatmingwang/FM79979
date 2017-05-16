#pragma once
//#include "WavWaves.h"
#include "KissFFTConvert.h"
#include "KissFFTStreaming.h"
#include "SoundFFTCapture.h"
#include "TimeFrequencyAmplitudeValueCapture.h"
#include "ToneData.h"
#include "MusicGameApp.h"
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
			m_pGameApp = new cMusicGameApp((HWND)m_pTargetControl->Handle.ToPointer());
			//m_pGameApp = new cGameApp((HWND)m_pTargetControl->Handle.ToPointer());
			m_pGameApp->Init();
			m_pSoundFFTCapture = new cSoundFFTCapture();
			int l_iFrequence = SOUND_CAPTURE_FREQUENCE/2;
			m_pSoundCapture = new cSoundCapture(l_iFrequence,AL_FORMAT_MONO16,m_pSoundFFTCapture->GetOpanalCaptureBufferSize(ONE_FRAME_NEED_NUM_FFT_DATA_COUNT,l_iFrequence));
			m_pSoundCapture->AddObject(m_pSoundFFTCapture);
			//m_pSoundCapture->AddSoundRecord();
			m_HdcMV = GetDC((HWND)m_pTargetControl->Handle.ToPointer());
			m_HGLRCMV = m_pGameApp->m_sHGLRC;
			//
			this->m_pWaveInfo = nullptr;
			//this->m_pWavWaves = nullptr;
			//
			m_pKissFFTConvertBase = nullptr;
			m_pKissFFTConvertBase = nullptr;
			//m_pKissFFTConvertBase = new cKissFFTConvert();
			//m_pKissFFTConvertBase = new cKissFFTStreamingConvert();
			//for mouse event
			m_pTargetControl->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseMove);
			m_pTargetControl->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseDown);
			m_pTargetControl->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseUp);
			m_pTargetControl->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseUp);
			m_pTargetControl->SizeChanged += gcnew System::EventHandler(this, &Form1::MySizeChanged);
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
			SAFE_DELETE(m_pSoundCapture);
			m_pSoundFFTCapture = nullptr;
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
		cGameApp*					m_pGameApp;
		//
		cSoundCapture*				m_pSoundCapture;
		cSoundFFTCapture*			m_pSoundFFTCapture;
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
		//

	private: System::Windows::Forms::TextBox^  WavFileName_textBox;
	private: System::Windows::Forms::Panel^  OPENGL_panel;
	private: System::Windows::Forms::TrackBar^  CurrentTime_trackBar;
	private: System::Windows::Forms::Label^  CurrentTime_label;
	private: System::Windows::Forms::Button^  ForwardStep_button;

	private: System::Windows::Forms::Button^  BackStep_button;
	private: System::Windows::Forms::Button^  RecordSound_button;
	private: System::Windows::Forms::Button^  RecordSoundPause_button;
	private: System::Windows::Forms::TextBox^  RecordSoundFileName_textBox;
	private: System::Windows::Forms::Button^  FFTTest_button;
	private: System::Windows::Forms::Button^  ConvertToWav_button;
	private: System::Windows::Forms::Button^  ToOgg_button;
	private: System::Windows::Forms::Button^  PlayOgg_button;
	private: System::Windows::Forms::Button^  PlayWav_button;
	private: System::Windows::Forms::NumericUpDown^  WaveUpdateIndex_numericUpDown;
	private: System::Windows::Forms::Label^  WaveUpdateIndex_label;
	private: System::Windows::Forms::CheckBox^  FFTTestPause_checkBox;
	private: System::Windows::Forms::Button^  WavToOggOnlyOneChannel;
	private: System::Windows::Forms::Label^  GoToFFTTestTime_label;
	private: System::Windows::Forms::TrackBar^  GoToTime_trackBar;
	private: System::Windows::Forms::Label^  FFTDataCountScale_label;
	private: System::Windows::Forms::NumericUpDown^  FFTDataCountScale_numericUpDown;
	private: System::Windows::Forms::Label^  JumpToTime_label;
	private: System::Windows::Forms::Label^  SourceName_label;
	private: System::Windows::Forms::Label^  ConvertToName_label;
	private: System::Windows::Forms::NumericUpDown^  FFTFilterFrenquenceScale_numericUpDown;
	private: System::Windows::Forms::GroupBox^  SoundFileConvert_groupBox;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::Label^  FilterRange_label;
	private: System::Windows::Forms::CheckBox^  DoFFTFilter_checkBox;
	private: System::Windows::Forms::GroupBox^  SoundCapture_groupBox;
	private: System::Windows::Forms::Label^  ChartScale_label;
	private: System::Windows::Forms::NumericUpDown^  FFTChartScale_numericUpDown;
	private: System::Windows::Forms::Label^  SoundCaptureDoFFTChartScale_label;
	private: System::Windows::Forms::NumericUpDown^  SoundCaptureDoFFTChartScale_numericUpDown;
	private: System::Windows::Forms::CheckBox^  SoundCaptureDoFFTFilter_checkBox;
	private: System::Windows::Forms::Label^  SoundCaptureDoFFTFilterRange_label;
	private: System::Windows::Forms::NumericUpDown^  SoundCaptureDoFFTFilterRange_numericUpDown;
	private: System::Windows::Forms::Label^  FilterStrength_label;
	private: System::Windows::Forms::NumericUpDown^  FilterStrength_numericUpDown;
	private: System::Windows::Forms::Label^  SoundCaptureFilterStrength_label;
	private: System::Windows::Forms::NumericUpDown^  SoundCaptureFilterStrength_numericUpDown;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Button^  CaptureToFile_button;
	private: System::Windows::Forms::Label^  CompareAndCaptureToFileMinAmplitude_label;
	private: System::Windows::Forms::Label^  CompareAndCaptureToFileKeepTime_label;
	private: System::Windows::Forms::Label^  CompareAndCaptureToFileTolerateTime_label;
	private: System::Windows::Forms::Label^  CompareAndCaptureToFileFreqOffset_label;
	private: System::Windows::Forms::Label^  CompareAndCaptureToFileParseFPS_label;
	private: System::Windows::Forms::NumericUpDown^  CompareAndCaptureToFileMinAmplitude_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  CompareAndCaptureToFileParseFPS_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  CompareAndCaptureToFileFreqOffset_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  CompareAndCaptureToFileTolerateTime_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  CompareAndCaptureToFileKeepTime_numericUpDown;
	private: System::Windows::Forms::Label^  CompareAndCaptureToFileFilterStrangth_label;
	private: System::Windows::Forms::NumericUpDown^  CompareAndCaptureToFileFilterStrangth_numericUpDown;
	private: System::Windows::Forms::Label^  CompareAndCaptureToFileFilterRange_label;
	private: System::Windows::Forms::NumericUpDown^  CompareAndCaptureToFileFilterRange_numericUpDown;











private: System::Windows::Forms::GroupBox^  FFT_groupBox;






#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->WavFileName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->OPENGL_panel = (gcnew System::Windows::Forms::Panel());
			this->CurrentTime_trackBar = (gcnew System::Windows::Forms::TrackBar());
			this->CurrentTime_label = (gcnew System::Windows::Forms::Label());
			this->ForwardStep_button = (gcnew System::Windows::Forms::Button());
			this->BackStep_button = (gcnew System::Windows::Forms::Button());
			this->RecordSound_button = (gcnew System::Windows::Forms::Button());
			this->RecordSoundPause_button = (gcnew System::Windows::Forms::Button());
			this->RecordSoundFileName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->FFTTest_button = (gcnew System::Windows::Forms::Button());
			this->ConvertToWav_button = (gcnew System::Windows::Forms::Button());
			this->ToOgg_button = (gcnew System::Windows::Forms::Button());
			this->PlayOgg_button = (gcnew System::Windows::Forms::Button());
			this->PlayWav_button = (gcnew System::Windows::Forms::Button());
			this->WaveUpdateIndex_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->WaveUpdateIndex_label = (gcnew System::Windows::Forms::Label());
			this->FFTTestPause_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->WavToOggOnlyOneChannel = (gcnew System::Windows::Forms::Button());
			this->GoToFFTTestTime_label = (gcnew System::Windows::Forms::Label());
			this->FFTDataCountScale_label = (gcnew System::Windows::Forms::Label());
			this->FFTDataCountScale_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->GoToTime_trackBar = (gcnew System::Windows::Forms::TrackBar());
			this->JumpToTime_label = (gcnew System::Windows::Forms::Label());
			this->SourceName_label = (gcnew System::Windows::Forms::Label());
			this->ConvertToName_label = (gcnew System::Windows::Forms::Label());
			this->FFTFilterFrenquenceScale_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->SoundFileConvert_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->FFT_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->FilterStrength_label = (gcnew System::Windows::Forms::Label());
			this->FilterStrength_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->ChartScale_label = (gcnew System::Windows::Forms::Label());
			this->FFTChartScale_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->DoFFTFilter_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->FilterRange_label = (gcnew System::Windows::Forms::Label());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->CompareAndCaptureToFileFilterStrangth_label = (gcnew System::Windows::Forms::Label());
			this->CompareAndCaptureToFileFilterStrangth_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CompareAndCaptureToFileMinAmplitude_label = (gcnew System::Windows::Forms::Label());
			this->CompareAndCaptureToFileKeepTime_label = (gcnew System::Windows::Forms::Label());
			this->CompareAndCaptureToFileTolerateTime_label = (gcnew System::Windows::Forms::Label());
			this->CompareAndCaptureToFileFreqOffset_label = (gcnew System::Windows::Forms::Label());
			this->CompareAndCaptureToFileFilterRange_label = (gcnew System::Windows::Forms::Label());
			this->CompareAndCaptureToFileFilterRange_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CompareAndCaptureToFileParseFPS_label = (gcnew System::Windows::Forms::Label());
			this->CompareAndCaptureToFileMinAmplitude_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CompareAndCaptureToFileParseFPS_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CompareAndCaptureToFileFreqOffset_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CompareAndCaptureToFileTolerateTime_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CompareAndCaptureToFileKeepTime_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CaptureToFile_button = (gcnew System::Windows::Forms::Button());
			this->SoundCapture_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->SoundCaptureFilterStrength_label = (gcnew System::Windows::Forms::Label());
			this->SoundCaptureFilterStrength_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->SoundCaptureDoFFTChartScale_label = (gcnew System::Windows::Forms::Label());
			this->SoundCaptureDoFFTFilter_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->SoundCaptureDoFFTChartScale_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->SoundCaptureDoFFTFilterRange_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->SoundCaptureDoFFTFilterRange_label = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CurrentTime_trackBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->WaveUpdateIndex_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTDataCountScale_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GoToTime_trackBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTFilterFrenquenceScale_numericUpDown))->BeginInit();
			this->SoundFileConvert_groupBox->SuspendLayout();
			this->FFT_groupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FilterStrength_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTChartScale_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileFilterStrangth_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileFilterRange_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileMinAmplitude_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileParseFPS_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileFreqOffset_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileTolerateTime_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileKeepTime_numericUpDown))->BeginInit();
			this->SoundCapture_groupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SoundCaptureFilterStrength_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SoundCaptureDoFFTChartScale_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SoundCaptureDoFFTFilterRange_numericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// WavFileName_textBox
			// 
			this->WavFileName_textBox->Location = System::Drawing::Point(305, 51);
			this->WavFileName_textBox->Name = L"WavFileName_textBox";
			this->WavFileName_textBox->Size = System::Drawing::Size(220, 20);
			this->WavFileName_textBox->TabIndex = 1;
			this->WavFileName_textBox->Text = L"Sound\\1.ogg";
			// 
			// OPENGL_panel
			// 
			this->OPENGL_panel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->OPENGL_panel->Location = System::Drawing::Point(0, 0);
			this->OPENGL_panel->Name = L"OPENGL_panel";
			this->OPENGL_panel->Size = System::Drawing::Size(1884, 430);
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
			// ForwardStep_button
			// 
			this->ForwardStep_button->Location = System::Drawing::Point(195, 248);
			this->ForwardStep_button->Name = L"ForwardStep_button";
			this->ForwardStep_button->Size = System::Drawing::Size(75, 23);
			this->ForwardStep_button->TabIndex = 10;
			this->ForwardStep_button->Text = L">>";
			this->ForwardStep_button->UseVisualStyleBackColor = true;
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
			this->BackStep_button->Click += gcnew System::EventHandler(this, &Form1::BackStep_button_Click);
			// 
			// RecordSound_button
			// 
			this->RecordSound_button->Location = System::Drawing::Point(4, 28);
			this->RecordSound_button->Name = L"RecordSound_button";
			this->RecordSound_button->Size = System::Drawing::Size(94, 34);
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
			this->RecordSoundFileName_textBox->Size = System::Drawing::Size(220, 20);
			this->RecordSoundFileName_textBox->TabIndex = 17;
			this->RecordSoundFileName_textBox->Text = L"Sound\\21.wav";
			// 
			// FFTTest_button
			// 
			this->FFTTest_button->Location = System::Drawing::Point(109, 28);
			this->FFTTest_button->Name = L"FFTTest_button";
			this->FFTTest_button->Size = System::Drawing::Size(75, 23);
			this->FFTTest_button->TabIndex = 18;
			this->FFTTest_button->Text = L"FFT test";
			this->FFTTest_button->UseVisualStyleBackColor = true;
			this->FFTTest_button->Click += gcnew System::EventHandler(this, &Form1::FFTTest_button_Click);
			// 
			// ConvertToWav_button
			// 
			this->ConvertToWav_button->Location = System::Drawing::Point(9, 121);
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
			// WaveUpdateIndex_numericUpDown
			// 
			this->WaveUpdateIndex_numericUpDown->Location = System::Drawing::Point(9, 44);
			this->WaveUpdateIndex_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->WaveUpdateIndex_numericUpDown->Name = L"WaveUpdateIndex_numericUpDown";
			this->WaveUpdateIndex_numericUpDown->Size = System::Drawing::Size(94, 20);
			this->WaveUpdateIndex_numericUpDown->TabIndex = 23;
			this->WaveUpdateIndex_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {30, 0, 0, 0});
			this->WaveUpdateIndex_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::WaveUpdateIndex_numericUpDown_ValueChanged);
			// 
			// WaveUpdateIndex_label
			// 
			this->WaveUpdateIndex_label->AutoSize = true;
			this->WaveUpdateIndex_label->Location = System::Drawing::Point(6, 28);
			this->WaveUpdateIndex_label->Name = L"WaveUpdateIndex_label";
			this->WaveUpdateIndex_label->Size = System::Drawing::Size(97, 13);
			this->WaveUpdateIndex_label->TabIndex = 24;
			this->WaveUpdateIndex_label->Text = L"WaveUpdateIndex";
			// 
			// FFTTestPause_checkBox
			// 
			this->FFTTestPause_checkBox->AutoSize = true;
			this->FFTTestPause_checkBox->Location = System::Drawing::Point(200, 28);
			this->FFTTestPause_checkBox->Name = L"FFTTestPause_checkBox";
			this->FFTTestPause_checkBox->Size = System::Drawing::Size(96, 17);
			this->FFTTestPause_checkBox->TabIndex = 25;
			this->FFTTestPause_checkBox->Text = L"FFTTestPause";
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
			// GoToFFTTestTime_label
			// 
			this->GoToFFTTestTime_label->AutoSize = true;
			this->GoToFFTTestTime_label->Location = System::Drawing::Point(6, 74);
			this->GoToFFTTestTime_label->Name = L"GoToFFTTestTime_label";
			this->GoToFFTTestTime_label->Size = System::Drawing::Size(97, 13);
			this->GoToFFTTestTime_label->TabIndex = 28;
			this->GoToFFTTestTime_label->Text = L"GoToFFTTestTime";
			// 
			// FFTDataCountScale_label
			// 
			this->FFTDataCountScale_label->AutoSize = true;
			this->FFTDataCountScale_label->Location = System::Drawing::Point(111, 54);
			this->FFTDataCountScale_label->Name = L"FFTDataCountScale_label";
			this->FFTDataCountScale_label->Size = System::Drawing::Size(104, 13);
			this->FFTDataCountScale_label->TabIndex = 28;
			this->FFTDataCountScale_label->Text = L"FFTDataCountScale";
			// 
			// FFTDataCountScale_numericUpDown
			// 
			this->FFTDataCountScale_numericUpDown->DecimalPlaces = 1;
			this->FFTDataCountScale_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->FFTDataCountScale_numericUpDown->Location = System::Drawing::Point(114, 70);
			this->FFTDataCountScale_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->FFTDataCountScale_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->FFTDataCountScale_numericUpDown->Name = L"FFTDataCountScale_numericUpDown";
			this->FFTDataCountScale_numericUpDown->Size = System::Drawing::Size(120, 20);
			this->FFTDataCountScale_numericUpDown->TabIndex = 27;
			this->FFTDataCountScale_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 65536});
			this->FFTDataCountScale_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::FFTDataCountScale_numericUpDown_ValueChanged);
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
			this->SourceName_label->Location = System::Drawing::Point(302, 28);
			this->SourceName_label->Name = L"SourceName_label";
			this->SourceName_label->Size = System::Drawing::Size(100, 13);
			this->SourceName_label->TabIndex = 30;
			this->SourceName_label->Text = L"SoundSourceName";
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
			// FFTFilterFrenquenceScale_numericUpDown
			// 
			this->FFTFilterFrenquenceScale_numericUpDown->DecimalPlaces = 2;
			this->FFTFilterFrenquenceScale_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 
				65536});
			this->FFTFilterFrenquenceScale_numericUpDown->Location = System::Drawing::Point(305, 141);
			this->FFTFilterFrenquenceScale_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->FFTFilterFrenquenceScale_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->FFTFilterFrenquenceScale_numericUpDown->Name = L"FFTFilterFrenquenceScale_numericUpDown";
			this->FFTFilterFrenquenceScale_numericUpDown->Size = System::Drawing::Size(94, 20);
			this->FFTFilterFrenquenceScale_numericUpDown->TabIndex = 31;
			this->FFTFilterFrenquenceScale_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {7, 0, 0, 65536});
			this->FFTFilterFrenquenceScale_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::FFTFilterFrenquenceScale_numericUpDown_ValueChanged);
			// 
			// SoundFileConvert_groupBox
			// 
			this->SoundFileConvert_groupBox->Controls->Add(this->ConvertToName_label);
			this->SoundFileConvert_groupBox->Controls->Add(this->RecordSoundFileName_textBox);
			this->SoundFileConvert_groupBox->Controls->Add(this->ToOgg_button);
			this->SoundFileConvert_groupBox->Controls->Add(this->WavToOggOnlyOneChannel);
			this->SoundFileConvert_groupBox->Controls->Add(this->ConvertToWav_button);
			this->SoundFileConvert_groupBox->Location = System::Drawing::Point(562, 7);
			this->SoundFileConvert_groupBox->Name = L"SoundFileConvert_groupBox";
			this->SoundFileConvert_groupBox->Size = System::Drawing::Size(242, 167);
			this->SoundFileConvert_groupBox->TabIndex = 33;
			this->SoundFileConvert_groupBox->TabStop = false;
			this->SoundFileConvert_groupBox->Text = L"SoundFileConvert";
			// 
			// FFT_groupBox
			// 
			this->FFT_groupBox->Controls->Add(this->FilterStrength_label);
			this->FFT_groupBox->Controls->Add(this->FilterStrength_numericUpDown);
			this->FFT_groupBox->Controls->Add(this->ChartScale_label);
			this->FFT_groupBox->Controls->Add(this->FFTChartScale_numericUpDown);
			this->FFT_groupBox->Controls->Add(this->WaveUpdateIndex_label);
			this->FFT_groupBox->Controls->Add(this->DoFFTFilter_checkBox);
			this->FFT_groupBox->Controls->Add(this->FilterRange_label);
			this->FFT_groupBox->Controls->Add(this->SourceName_label);
			this->FFT_groupBox->Controls->Add(this->FFTFilterFrenquenceScale_numericUpDown);
			this->FFT_groupBox->Controls->Add(this->CurrentTime_trackBar);
			this->FFT_groupBox->Controls->Add(this->JumpToTime_label);
			this->FFT_groupBox->Controls->Add(this->WavFileName_textBox);
			this->FFT_groupBox->Controls->Add(this->FFTDataCountScale_label);
			this->FFT_groupBox->Controls->Add(this->CurrentTime_label);
			this->FFT_groupBox->Controls->Add(this->FFTDataCountScale_numericUpDown);
			this->FFT_groupBox->Controls->Add(this->ForwardStep_button);
			this->FFT_groupBox->Controls->Add(this->GoToTime_trackBar);
			this->FFT_groupBox->Controls->Add(this->BackStep_button);
			this->FFT_groupBox->Controls->Add(this->GoToFFTTestTime_label);
			this->FFT_groupBox->Controls->Add(this->FFTTestPause_checkBox);
			this->FFT_groupBox->Controls->Add(this->WaveUpdateIndex_numericUpDown);
			this->FFT_groupBox->Controls->Add(this->FFTTest_button);
			this->FFT_groupBox->Location = System::Drawing::Point(3, 7);
			this->FFT_groupBox->Name = L"FFT_groupBox";
			this->FFT_groupBox->Size = System::Drawing::Size(534, 296);
			this->FFT_groupBox->TabIndex = 34;
			this->FFT_groupBox->TabStop = false;
			this->FFT_groupBox->Text = L"FFT";
			// 
			// FilterStrength_label
			// 
			this->FilterStrength_label->AutoSize = true;
			this->FilterStrength_label->Location = System::Drawing::Point(302, 228);
			this->FilterStrength_label->Name = L"FilterStrength_label";
			this->FilterStrength_label->Size = System::Drawing::Size(69, 13);
			this->FilterStrength_label->TabIndex = 37;
			this->FilterStrength_label->Text = L"FilterStrength";
			// 
			// FilterStrength_numericUpDown
			// 
			this->FilterStrength_numericUpDown->Location = System::Drawing::Point(305, 247);
			this->FilterStrength_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->FilterStrength_numericUpDown->Name = L"FilterStrength_numericUpDown";
			this->FilterStrength_numericUpDown->Size = System::Drawing::Size(120, 20);
			this->FilterStrength_numericUpDown->TabIndex = 36;
			this->FilterStrength_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {6, 0, 0, 0});
			this->FilterStrength_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::FilterStrength_numericUpDown_ValueChanged);
			// 
			// ChartScale_label
			// 
			this->ChartScale_label->AutoSize = true;
			this->ChartScale_label->Location = System::Drawing::Point(302, 179);
			this->ChartScale_label->Name = L"ChartScale_label";
			this->ChartScale_label->Size = System::Drawing::Size(59, 13);
			this->ChartScale_label->TabIndex = 35;
			this->ChartScale_label->Text = L"ChartScale";
			// 
			// FFTChartScale_numericUpDown
			// 
			this->FFTChartScale_numericUpDown->DecimalPlaces = 2;
			this->FFTChartScale_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->FFTChartScale_numericUpDown->Location = System::Drawing::Point(305, 198);
			this->FFTChartScale_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->FFTChartScale_numericUpDown->Name = L"FFTChartScale_numericUpDown";
			this->FFTChartScale_numericUpDown->Size = System::Drawing::Size(120, 20);
			this->FFTChartScale_numericUpDown->TabIndex = 34;
			this->FFTChartScale_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->FFTChartScale_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::FFTChartScale_numericUpDown_ValueChanged);
			// 
			// DoFFTFilter_checkBox
			// 
			this->DoFFTFilter_checkBox->AutoSize = true;
			this->DoFFTFilter_checkBox->Location = System::Drawing::Point(305, 96);
			this->DoFFTFilter_checkBox->Name = L"DoFFTFilter_checkBox";
			this->DoFFTFilter_checkBox->Size = System::Drawing::Size(81, 17);
			this->DoFFTFilter_checkBox->TabIndex = 33;
			this->DoFFTFilter_checkBox->Text = L"DoFFTFilter";
			this->DoFFTFilter_checkBox->UseVisualStyleBackColor = true;
			this->DoFFTFilter_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::DoFFTFilter_checkBox_CheckedChanged);
			// 
			// FilterRange_label
			// 
			this->FilterRange_label->AutoSize = true;
			this->FilterRange_label->Location = System::Drawing::Point(302, 122);
			this->FilterRange_label->Name = L"FilterRange_label";
			this->FilterRange_label->Size = System::Drawing::Size(91, 13);
			this->FilterRange_label->TabIndex = 32;
			this->FilterRange_label->Text = L"FilterRange(0.1-1)";
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
			this->splitContainer1->Panel2->Controls->Add(this->groupBox1);
			this->splitContainer1->Panel2->Controls->Add(this->SoundCapture_groupBox);
			this->splitContainer1->Panel2->Controls->Add(this->FFT_groupBox);
			this->splitContainer1->Panel2->Controls->Add(this->SoundFileConvert_groupBox);
			this->splitContainer1->Size = System::Drawing::Size(1884, 861);
			this->splitContainer1->SplitterDistance = 430;
			this->splitContainer1->TabIndex = 35;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileFilterStrangth_label);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileFilterStrangth_numericUpDown);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileMinAmplitude_label);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileKeepTime_label);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileTolerateTime_label);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileFreqOffset_label);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileFilterRange_label);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileFilterRange_numericUpDown);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileParseFPS_label);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileMinAmplitude_numericUpDown);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileParseFPS_numericUpDown);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileFreqOffset_numericUpDown);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileTolerateTime_numericUpDown);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileKeepTime_numericUpDown);
			this->groupBox1->Controls->Add(this->CaptureToFile_button);
			this->groupBox1->Location = System::Drawing::Point(846, 21);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(210, 249);
			this->groupBox1->TabIndex = 37;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"CompareAndCaptureToFile";
			// 
			// CompareAndCaptureToFileFilterStrangth_label
			// 
			this->CompareAndCaptureToFileFilterStrangth_label->AutoSize = true;
			this->CompareAndCaptureToFileFilterStrangth_label->Location = System::Drawing::Point(7, 193);
			this->CompareAndCaptureToFileFilterStrangth_label->Name = L"CompareAndCaptureToFileFilterStrangth_label";
			this->CompareAndCaptureToFileFilterStrangth_label->Size = System::Drawing::Size(69, 13);
			this->CompareAndCaptureToFileFilterStrangth_label->TabIndex = 41;
			this->CompareAndCaptureToFileFilterStrangth_label->Text = L"FilterStrength";
			// 
			// CompareAndCaptureToFileFilterStrangth_numericUpDown
			// 
			this->CompareAndCaptureToFileFilterStrangth_numericUpDown->Location = System::Drawing::Point(103, 192);
			this->CompareAndCaptureToFileFilterStrangth_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 
				0, 0, 0});
			this->CompareAndCaptureToFileFilterStrangth_numericUpDown->Name = L"CompareAndCaptureToFileFilterStrangth_numericUpDown";
			this->CompareAndCaptureToFileFilterStrangth_numericUpDown->Size = System::Drawing::Size(94, 20);
			this->CompareAndCaptureToFileFilterStrangth_numericUpDown->TabIndex = 40;
			this->CompareAndCaptureToFileFilterStrangth_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {6, 
				0, 0, 0});
			// 
			// CompareAndCaptureToFileMinAmplitude_label
			// 
			this->CompareAndCaptureToFileMinAmplitude_label->AutoSize = true;
			this->CompareAndCaptureToFileMinAmplitude_label->Location = System::Drawing::Point(6, 142);
			this->CompareAndCaptureToFileMinAmplitude_label->Name = L"CompareAndCaptureToFileMinAmplitude_label";
			this->CompareAndCaptureToFileMinAmplitude_label->Size = System::Drawing::Size(70, 13);
			this->CompareAndCaptureToFileMinAmplitude_label->TabIndex = 47;
			this->CompareAndCaptureToFileMinAmplitude_label->Text = L"MinAmplitude";
			// 
			// CompareAndCaptureToFileKeepTime_label
			// 
			this->CompareAndCaptureToFileKeepTime_label->AutoSize = true;
			this->CompareAndCaptureToFileKeepTime_label->Location = System::Drawing::Point(6, 113);
			this->CompareAndCaptureToFileKeepTime_label->Name = L"CompareAndCaptureToFileKeepTime_label";
			this->CompareAndCaptureToFileKeepTime_label->Size = System::Drawing::Size(55, 13);
			this->CompareAndCaptureToFileKeepTime_label->TabIndex = 46;
			this->CompareAndCaptureToFileKeepTime_label->Text = L"KeepTime";
			// 
			// CompareAndCaptureToFileTolerateTime_label
			// 
			this->CompareAndCaptureToFileTolerateTime_label->AutoSize = true;
			this->CompareAndCaptureToFileTolerateTime_label->Location = System::Drawing::Point(6, 82);
			this->CompareAndCaptureToFileTolerateTime_label->Name = L"CompareAndCaptureToFileTolerateTime_label";
			this->CompareAndCaptureToFileTolerateTime_label->Size = System::Drawing::Size(69, 13);
			this->CompareAndCaptureToFileTolerateTime_label->TabIndex = 45;
			this->CompareAndCaptureToFileTolerateTime_label->Text = L"TolerateTime";
			// 
			// CompareAndCaptureToFileFreqOffset_label
			// 
			this->CompareAndCaptureToFileFreqOffset_label->AutoSize = true;
			this->CompareAndCaptureToFileFreqOffset_label->Location = System::Drawing::Point(6, 58);
			this->CompareAndCaptureToFileFreqOffset_label->Name = L"CompareAndCaptureToFileFreqOffset_label";
			this->CompareAndCaptureToFileFreqOffset_label->Size = System::Drawing::Size(56, 13);
			this->CompareAndCaptureToFileFreqOffset_label->TabIndex = 44;
			this->CompareAndCaptureToFileFreqOffset_label->Text = L"FreqOffset";
			// 
			// CompareAndCaptureToFileFilterRange_label
			// 
			this->CompareAndCaptureToFileFilterRange_label->AutoSize = true;
			this->CompareAndCaptureToFileFilterRange_label->Location = System::Drawing::Point(6, 167);
			this->CompareAndCaptureToFileFilterRange_label->Name = L"CompareAndCaptureToFileFilterRange_label";
			this->CompareAndCaptureToFileFilterRange_label->Size = System::Drawing::Size(91, 13);
			this->CompareAndCaptureToFileFilterRange_label->TabIndex = 39;
			this->CompareAndCaptureToFileFilterRange_label->Text = L"FilterRange(0.1-1)";
			// 
			// CompareAndCaptureToFileFilterRange_numericUpDown
			// 
			this->CompareAndCaptureToFileFilterRange_numericUpDown->DecimalPlaces = 2;
			this->CompareAndCaptureToFileFilterRange_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 
				0, 0, 65536});
			this->CompareAndCaptureToFileFilterRange_numericUpDown->Location = System::Drawing::Point(103, 165);
			this->CompareAndCaptureToFileFilterRange_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 
				0, 0, 0});
			this->CompareAndCaptureToFileFilterRange_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 
				0, 0, 65536});
			this->CompareAndCaptureToFileFilterRange_numericUpDown->Name = L"CompareAndCaptureToFileFilterRange_numericUpDown";
			this->CompareAndCaptureToFileFilterRange_numericUpDown->Size = System::Drawing::Size(94, 20);
			this->CompareAndCaptureToFileFilterRange_numericUpDown->TabIndex = 38;
			this->CompareAndCaptureToFileFilterRange_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {7, 
				0, 0, 65536});
			this->CompareAndCaptureToFileFilterRange_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::CompareAndCaptureToFileParseFPS_numericUpDown_ValueChanged);
			// 
			// CompareAndCaptureToFileParseFPS_label
			// 
			this->CompareAndCaptureToFileParseFPS_label->AutoSize = true;
			this->CompareAndCaptureToFileParseFPS_label->Location = System::Drawing::Point(6, 30);
			this->CompareAndCaptureToFileParseFPS_label->Name = L"CompareAndCaptureToFileParseFPS_label";
			this->CompareAndCaptureToFileParseFPS_label->Size = System::Drawing::Size(54, 13);
			this->CompareAndCaptureToFileParseFPS_label->TabIndex = 43;
			this->CompareAndCaptureToFileParseFPS_label->Text = L"ParseFPS";
			// 
			// CompareAndCaptureToFileMinAmplitude_numericUpDown
			// 
			this->CompareAndCaptureToFileMinAmplitude_numericUpDown->Location = System::Drawing::Point(103, 140);
			this->CompareAndCaptureToFileMinAmplitude_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 
				0, 0, 0});
			this->CompareAndCaptureToFileMinAmplitude_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 
				0, 0, 0});
			this->CompareAndCaptureToFileMinAmplitude_numericUpDown->Name = L"CompareAndCaptureToFileMinAmplitude_numericUpDown";
			this->CompareAndCaptureToFileMinAmplitude_numericUpDown->Size = System::Drawing::Size(94, 20);
			this->CompareAndCaptureToFileMinAmplitude_numericUpDown->TabIndex = 42;
			this->CompareAndCaptureToFileMinAmplitude_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {30, 
				0, 0, 0});
			this->CompareAndCaptureToFileMinAmplitude_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::CompareAndCaptureToFileParseFPS_numericUpDown_ValueChanged);
			// 
			// CompareAndCaptureToFileParseFPS_numericUpDown
			// 
			this->CompareAndCaptureToFileParseFPS_numericUpDown->Location = System::Drawing::Point(103, 25);
			this->CompareAndCaptureToFileParseFPS_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {60, 
				0, 0, 0});
			this->CompareAndCaptureToFileParseFPS_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {6, 0, 
				0, 0});
			this->CompareAndCaptureToFileParseFPS_numericUpDown->Name = L"CompareAndCaptureToFileParseFPS_numericUpDown";
			this->CompareAndCaptureToFileParseFPS_numericUpDown->Size = System::Drawing::Size(94, 20);
			this->CompareAndCaptureToFileParseFPS_numericUpDown->TabIndex = 41;
			this->CompareAndCaptureToFileParseFPS_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {25, 0, 
				0, 0});
			this->CompareAndCaptureToFileParseFPS_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::CompareAndCaptureToFileParseFPS_numericUpDown_ValueChanged);
			// 
			// CompareAndCaptureToFileFreqOffset_numericUpDown
			// 
			this->CompareAndCaptureToFileFreqOffset_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 
				0, 0, 65536});
			this->CompareAndCaptureToFileFreqOffset_numericUpDown->Location = System::Drawing::Point(103, 54);
			this->CompareAndCaptureToFileFreqOffset_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {300, 
				0, 0, 0});
			this->CompareAndCaptureToFileFreqOffset_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 
				0, 0, 0});
			this->CompareAndCaptureToFileFreqOffset_numericUpDown->Name = L"CompareAndCaptureToFileFreqOffset_numericUpDown";
			this->CompareAndCaptureToFileFreqOffset_numericUpDown->Size = System::Drawing::Size(94, 20);
			this->CompareAndCaptureToFileFreqOffset_numericUpDown->TabIndex = 40;
			this->CompareAndCaptureToFileFreqOffset_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 
				0, 0});
			this->CompareAndCaptureToFileFreqOffset_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::CompareAndCaptureToFileParseFPS_numericUpDown_ValueChanged);
			// 
			// CompareAndCaptureToFileTolerateTime_numericUpDown
			// 
			this->CompareAndCaptureToFileTolerateTime_numericUpDown->DecimalPlaces = 2;
			this->CompareAndCaptureToFileTolerateTime_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 
				0, 0, 131072});
			this->CompareAndCaptureToFileTolerateTime_numericUpDown->Location = System::Drawing::Point(103, 84);
			this->CompareAndCaptureToFileTolerateTime_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 
				0, 0, 0});
			this->CompareAndCaptureToFileTolerateTime_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {16, 
				0, 0, 196608});
			this->CompareAndCaptureToFileTolerateTime_numericUpDown->Name = L"CompareAndCaptureToFileTolerateTime_numericUpDown";
			this->CompareAndCaptureToFileTolerateTime_numericUpDown->Size = System::Drawing::Size(94, 20);
			this->CompareAndCaptureToFileTolerateTime_numericUpDown->TabIndex = 39;
			this->CompareAndCaptureToFileTolerateTime_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {32, 
				0, 0, 196608});
			this->CompareAndCaptureToFileTolerateTime_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::CompareAndCaptureToFileParseFPS_numericUpDown_ValueChanged);
			// 
			// CompareAndCaptureToFileKeepTime_numericUpDown
			// 
			this->CompareAndCaptureToFileKeepTime_numericUpDown->DecimalPlaces = 2;
			this->CompareAndCaptureToFileKeepTime_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 
				0, 0, 196608});
			this->CompareAndCaptureToFileKeepTime_numericUpDown->Location = System::Drawing::Point(103, 111);
			this->CompareAndCaptureToFileKeepTime_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {16, 
				0, 0, 196608});
			this->CompareAndCaptureToFileKeepTime_numericUpDown->Name = L"CompareAndCaptureToFileKeepTime_numericUpDown";
			this->CompareAndCaptureToFileKeepTime_numericUpDown->Size = System::Drawing::Size(94, 20);
			this->CompareAndCaptureToFileKeepTime_numericUpDown->TabIndex = 38;
			this->CompareAndCaptureToFileKeepTime_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {7, 0, 
				0, 65536});
			this->CompareAndCaptureToFileKeepTime_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::CompareAndCaptureToFileParseFPS_numericUpDown_ValueChanged);
			// 
			// CaptureToFile_button
			// 
			this->CaptureToFile_button->Location = System::Drawing::Point(8, 218);
			this->CaptureToFile_button->Name = L"CaptureToFile_button";
			this->CaptureToFile_button->Size = System::Drawing::Size(100, 23);
			this->CaptureToFile_button->TabIndex = 0;
			this->CaptureToFile_button->Text = L"CaptureToFile";
			this->CaptureToFile_button->UseVisualStyleBackColor = true;
			this->CaptureToFile_button->Click += gcnew System::EventHandler(this, &Form1::CaptureToFile_button_Click);
			// 
			// SoundCapture_groupBox
			// 
			this->SoundCapture_groupBox->Controls->Add(this->SoundCaptureFilterStrength_label);
			this->SoundCapture_groupBox->Controls->Add(this->SoundCaptureFilterStrength_numericUpDown);
			this->SoundCapture_groupBox->Controls->Add(this->SoundCaptureDoFFTChartScale_label);
			this->SoundCapture_groupBox->Controls->Add(this->SoundCaptureDoFFTFilter_checkBox);
			this->SoundCapture_groupBox->Controls->Add(this->SoundCaptureDoFFTChartScale_numericUpDown);
			this->SoundCapture_groupBox->Controls->Add(this->RecordSound_button);
			this->SoundCapture_groupBox->Controls->Add(this->SoundCaptureDoFFTFilterRange_numericUpDown);
			this->SoundCapture_groupBox->Controls->Add(this->RecordSoundPause_button);
			this->SoundCapture_groupBox->Controls->Add(this->SoundCaptureDoFFTFilterRange_label);
			this->SoundCapture_groupBox->Location = System::Drawing::Point(562, 210);
			this->SoundCapture_groupBox->Name = L"SoundCapture_groupBox";
			this->SoundCapture_groupBox->Size = System::Drawing::Size(242, 189);
			this->SoundCapture_groupBox->TabIndex = 36;
			this->SoundCapture_groupBox->TabStop = false;
			this->SoundCapture_groupBox->Text = L"SoundCapture";
			// 
			// SoundCaptureFilterStrength_label
			// 
			this->SoundCaptureFilterStrength_label->AutoSize = true;
			this->SoundCaptureFilterStrength_label->Location = System::Drawing::Point(121, 137);
			this->SoundCaptureFilterStrength_label->Name = L"SoundCaptureFilterStrength_label";
			this->SoundCaptureFilterStrength_label->Size = System::Drawing::Size(69, 13);
			this->SoundCaptureFilterStrength_label->TabIndex = 43;
			this->SoundCaptureFilterStrength_label->Text = L"FilterStrength";
			// 
			// SoundCaptureFilterStrength_numericUpDown
			// 
			this->SoundCaptureFilterStrength_numericUpDown->Location = System::Drawing::Point(124, 156);
			this->SoundCaptureFilterStrength_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 
				0});
			this->SoundCaptureFilterStrength_numericUpDown->Name = L"SoundCaptureFilterStrength_numericUpDown";
			this->SoundCaptureFilterStrength_numericUpDown->Size = System::Drawing::Size(92, 20);
			this->SoundCaptureFilterStrength_numericUpDown->TabIndex = 42;
			this->SoundCaptureFilterStrength_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->SoundCaptureFilterStrength_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::SoundCaptureFilterStrength_numericUpDown_ValueChanged);
			// 
			// SoundCaptureDoFFTChartScale_label
			// 
			this->SoundCaptureDoFFTChartScale_label->AutoSize = true;
			this->SoundCaptureDoFFTChartScale_label->Location = System::Drawing::Point(121, 98);
			this->SoundCaptureDoFFTChartScale_label->Name = L"SoundCaptureDoFFTChartScale_label";
			this->SoundCaptureDoFFTChartScale_label->Size = System::Drawing::Size(59, 13);
			this->SoundCaptureDoFFTChartScale_label->TabIndex = 41;
			this->SoundCaptureDoFFTChartScale_label->Text = L"ChartScale";
			// 
			// SoundCaptureDoFFTFilter_checkBox
			// 
			this->SoundCaptureDoFFTFilter_checkBox->AutoSize = true;
			this->SoundCaptureDoFFTFilter_checkBox->Location = System::Drawing::Point(123, 28);
			this->SoundCaptureDoFFTFilter_checkBox->Name = L"SoundCaptureDoFFTFilter_checkBox";
			this->SoundCaptureDoFFTFilter_checkBox->Size = System::Drawing::Size(81, 17);
			this->SoundCaptureDoFFTFilter_checkBox->TabIndex = 39;
			this->SoundCaptureDoFFTFilter_checkBox->Text = L"DoFFTFilter";
			this->SoundCaptureDoFFTFilter_checkBox->UseVisualStyleBackColor = true;
			this->SoundCaptureDoFFTFilter_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::SoundCaptureDoFFTFilter_checkBox_CheckedChanged);
			// 
			// SoundCaptureDoFFTChartScale_numericUpDown
			// 
			this->SoundCaptureDoFFTChartScale_numericUpDown->DecimalPlaces = 1;
			this->SoundCaptureDoFFTChartScale_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 
				0, 65536});
			this->SoundCaptureDoFFTChartScale_numericUpDown->Location = System::Drawing::Point(124, 114);
			this->SoundCaptureDoFFTChartScale_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 
				65536});
			this->SoundCaptureDoFFTChartScale_numericUpDown->Name = L"SoundCaptureDoFFTChartScale_numericUpDown";
			this->SoundCaptureDoFFTChartScale_numericUpDown->Size = System::Drawing::Size(93, 20);
			this->SoundCaptureDoFFTChartScale_numericUpDown->TabIndex = 40;
			this->SoundCaptureDoFFTChartScale_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->SoundCaptureDoFFTChartScale_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::SoundCaptureDoFFTChartScale_numericUpDown_ValueChanged);
			// 
			// SoundCaptureDoFFTFilterRange_numericUpDown
			// 
			this->SoundCaptureDoFFTFilterRange_numericUpDown->DecimalPlaces = 2;
			this->SoundCaptureDoFFTFilterRange_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 
				0, 65536});
			this->SoundCaptureDoFFTFilterRange_numericUpDown->Location = System::Drawing::Point(123, 73);
			this->SoundCaptureDoFFTFilterRange_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 
				0});
			this->SoundCaptureDoFFTFilterRange_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 
				65536});
			this->SoundCaptureDoFFTFilterRange_numericUpDown->Name = L"SoundCaptureDoFFTFilterRange_numericUpDown";
			this->SoundCaptureDoFFTFilterRange_numericUpDown->Size = System::Drawing::Size(94, 20);
			this->SoundCaptureDoFFTFilterRange_numericUpDown->TabIndex = 37;
			this->SoundCaptureDoFFTFilterRange_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 
				0});
			this->SoundCaptureDoFFTFilterRange_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::SoundCaptureDoFFTFilterRange_numericUpDown_ValueChanged);
			// 
			// SoundCaptureDoFFTFilterRange_label
			// 
			this->SoundCaptureDoFFTFilterRange_label->AutoSize = true;
			this->SoundCaptureDoFFTFilterRange_label->Location = System::Drawing::Point(120, 54);
			this->SoundCaptureDoFFTFilterRange_label->Name = L"SoundCaptureDoFFTFilterRange_label";
			this->SoundCaptureDoFFTFilterRange_label->Size = System::Drawing::Size(91, 13);
			this->SoundCaptureDoFFTFilterRange_label->TabIndex = 38;
			this->SoundCaptureDoFFTFilterRange_label->Text = L"FilterRange(0.1-1)";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1884, 861);
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->PlayWav_button);
			this->Controls->Add(this->PlayOgg_button);
			this->Name = L"Form1";
			this->Text = L" ";
			this->Resize += gcnew System::EventHandler(this, &Form1::Form1_Resize);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CurrentTime_trackBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->WaveUpdateIndex_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTDataCountScale_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GoToTime_trackBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTFilterFrenquenceScale_numericUpDown))->EndInit();
			this->SoundFileConvert_groupBox->ResumeLayout(false);
			this->SoundFileConvert_groupBox->PerformLayout();
			this->FFT_groupBox->ResumeLayout(false);
			this->FFT_groupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FilterStrength_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTChartScale_numericUpDown))->EndInit();
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileFilterStrangth_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileFilterRange_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileMinAmplitude_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileParseFPS_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileFreqOffset_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileTolerateTime_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileKeepTime_numericUpDown))->EndInit();
			this->SoundCapture_groupBox->ResumeLayout(false);
			this->SoundCapture_groupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SoundCaptureFilterStrength_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SoundCaptureDoFFTChartScale_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SoundCaptureDoFFTFilterRange_numericUpDown))->EndInit();
			this->ResumeLayout(false);

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
			//glEnable2D(1920*2,1080*2);
			glEnable2D(1920,1080*2);
			glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
			glClearColor(0,0,0,0);
			glClearDepth(1.0f);
			glAlphaFunc(GL_GREATER,0.0001f);
			glShadeModel(GL_SMOOTH);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_ALPHA_TEST);
			//if( m_pWavWaves )
			{
				//m_pWavWaves->Render();
			}
			float l_fElpaseTime = this->m_pGameApp->m_sTimeAndFPS.fElpaseTime;
			if( cGameApp::m_spSoundParser )
					cGameApp::m_spSoundParser->Update(l_fElpaseTime);
			if( m_pKissFFTConvertBase )
			{
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
			}
			if( m_pSoundFFTCapture )
			{
				m_pSoundFFTCapture->Update(l_fElpaseTime);
				m_pSoundFFTCapture->Render();
			}
			this->m_pGameApp->Run();
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
					 RecordSound_button->Text = "Recording...";
				 }
				 else
				 {
					 m_pSoundCapture->StopRecord();
					 RecordSound_button->Text = "Stop!";
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
					float l_fScale = (float)FFTDataCountScale_numericUpDown->Value;
					this->m_pKissFFTConvertBase->SetFFTSampleScale(l_fScale,true);
					m_pKissFFTConvertBase->SetChartScale((float)FFTChartScale_numericUpDown->Value);
					m_pKissFFTConvertBase->SetFilter(DoFFTFilter_checkBox->Checked);
					m_pKissFFTConvertBase->SetiFrenquenceFilterEndScaleValue((float)FFTFilterFrenquenceScale_numericUpDown->Value);
					m_pKissFFTConvertBase->SetFilterStrengthValue((int)FilterStrength_numericUpDown->Value);
					 float l_fTimeToUpDate = 1.f/(int)WaveUpdateIndex_numericUpDown->Value;
					 m_pKissFFTConvertBase->SetFFTDataUpdateTime(l_fTimeToUpDate);
					if(m_pKissFFTConvertBase->FetchSoundDataStart(l_strFileName.c_str()))
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
			 if(System::IO::File::Exists(WavFileName_textBox->Text))
			 {
				std::string l_FileName = DNCT::GcStringToChar(this->WavFileName_textBox->Text);
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
private: System::Void WaveUpdateIndex_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( m_pKissFFTConvertBase )
			 {
				 float l_fTimeToUpDate = 1.f/(int)WaveUpdateIndex_numericUpDown->Value;
				 m_pKissFFTConvertBase->SetFFTDataUpdateTime(l_fTimeToUpDate);
			 }
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
				int l_iTargetSecond = (int)GoToTime_trackBar->Value;
				m_pKissFFTConvertBase->GoToTime(l_iTargetSecond/10.f);
			 }
			 
		 }
private: System::Void FFTDataCountScale_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( this->m_pKissFFTConvertBase )
			 {
				this->timer1->Enabled = false;
				cGameApp::m_sbGamePause = true;
				Sleep(100);
				float l_fScale = (float)FFTDataCountScale_numericUpDown->Value;;
				this->m_pKissFFTConvertBase->SetFFTSampleScale(l_fScale);
				this->timer1->Enabled = true;
				cGameApp::m_sbGamePause = false;
			 }
		 }
private: System::Void FFTFilterFrenquenceScale_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( m_pKissFFTConvertBase )
				m_pKissFFTConvertBase->SetiFrenquenceFilterEndScaleValue((float)FFTFilterFrenquenceScale_numericUpDown->Value);
		 }
private: System::Void DoFFTFilter_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( m_pKissFFTConvertBase )
				m_pKissFFTConvertBase->SetFilter(DoFFTFilter_checkBox->Checked);
		 }
private: System::Void FFTChartScale_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( m_pKissFFTConvertBase )
				m_pKissFFTConvertBase->SetChartScale((float)FFTChartScale_numericUpDown->Value);
		 }
private: System::Void SoundCaptureDoFFTFilter_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(m_pSoundFFTCapture)
			 {
				 m_pSoundFFTCapture->SetFilter(SoundCaptureDoFFTFilter_checkBox->Checked);
			 }
		 }
private: System::Void SoundCaptureDoFFTFilterRange_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(m_pSoundFFTCapture)
			 {
				 m_pSoundFFTCapture->SetiFrenquenceFilterEndScaleValue((float)SoundCaptureDoFFTFilterRange_numericUpDown->Value);
			 }
		 }
private: System::Void SoundCaptureDoFFTChartScale_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(m_pSoundFFTCapture)
			 {
				 m_pSoundFFTCapture->SetChartScale((float)SoundCaptureDoFFTChartScale_numericUpDown->Value);
			 }
		 }
private: System::Void FilterStrength_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( m_pKissFFTConvertBase )
				 m_pKissFFTConvertBase->SetFilterStrengthValue((int)FilterStrength_numericUpDown->Value);
		 }
private: System::Void SoundCaptureFilterStrength_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(m_pSoundFFTCapture)
			 {
				 m_pSoundFFTCapture->SetFilterStrengthValue((int)SoundCaptureFilterStrength_numericUpDown->Value);
			 }
		 }
private: System::Void CaptureToFile_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			array<String^>^l_strFileNames = DNCT::OpenFileAndGetNames("wav files (*.wav)|*.wav|All files (*.*)|*.*");
			if( l_strFileNames )
			{
				int		l_iAmplitude = (int)CompareAndCaptureToFileMinAmplitude_numericUpDown->Value;
				int		l_iParseFPS = (int)CompareAndCaptureToFileParseFPS_numericUpDown->Value;
				float	l_fFreqOffset = (float)CompareAndCaptureToFileFreqOffset_numericUpDown->Value;
				float	l_fTolerateTime = (float)CompareAndCaptureToFileTolerateTime_numericUpDown->Value;
				float	l_fKeepTime = (float)CompareAndCaptureToFileKeepTime_numericUpDown->Value;
				float	l_fFilterRange = (float)CompareAndCaptureToFileFilterRange_numericUpDown->Value;
				int		l_iFilterStrangth = (int)CompareAndCaptureToFileFilterStrangth_numericUpDown->Value;
				String^l_strGCOutputFileName = System::IO::Path::GetFileName(System::IO::Path::GetDirectoryName(l_strFileNames[0]));
				String^l_strDirectory = System::IO::Path::GetDirectoryName(l_strFileNames[0]);
				l_strDirectory += "/"+l_strGCOutputFileName;
				l_strDirectory += ".xml";

				//std::wstring l_strOutputFolderName = DNCT::GcStringToWchar(l_strDirectory);
				std::wstring l_strOutputFolderName = L"MusicGame/";
				l_strOutputFolderName += DNCT::GcStringToWchar(l_strGCOutputFileName);
				l_strOutputFolderName += L"/";
				l_strOutputFolderName += DNCT::GcStringToWchar(l_strGCOutputFileName);
				l_strOutputFolderName += L".xml";
				ISAXCallback l_ISAXCallback;
				TiXmlDocument*l_pTiXmlDocument = new TiXmlDocument();
				TiXmlElement*l_pRootTiXmlElement = new TiXmlElement(L"cToneDataVector");				
				//l_pRootTiXmlElement->SetAttribute(L"ToneDataFileName",l_strOutputFolderName.c_str());
				l_pTiXmlDocument->LinkEndChild(l_pRootTiXmlElement);
				for each(auto l_strFileName in l_strFileNames)
				{
					if( l_strFileName && l_strFileName->Length > 0 )
					{
						cTimeFrequencyAmplitudeValueCapture l_TimeFrequencyAmplitudeValueCapture;
						l_TimeFrequencyAmplitudeValueCapture.m_iParseFPS = l_iParseFPS;
						l_TimeFrequencyAmplitudeValueCapture.m_fFrequencyOffsetRange = l_fFreqOffset;
						l_TimeFrequencyAmplitudeValueCapture.m_fTolerateTime = l_fTolerateTime;
						l_TimeFrequencyAmplitudeValueCapture.m_fMinKeepTime = l_fKeepTime;
						l_TimeFrequencyAmplitudeValueCapture.m_iMinAmplitude = l_iAmplitude;
						std::string	l_strFileName2 = DNCT::GcStringToChar(l_strFileName);
						std::string	l_strOutputFileName = DNCT::GcStringToChar(System::IO::Path::ChangeExtension(l_strFileName,".soundFFT"));
						l_TimeFrequencyAmplitudeValueCapture.ParseAndSaveFileName(l_strFileName2.c_str(),l_iFilterStrangth,l_fFilterRange,l_strOutputFileName.c_str());
						std::wstring l_wstrFileName = DNCT::GcStringToWchar(System::IO::Path::ChangeExtension(System::IO::Path::GetFileName(l_strFileName),TONE_DATA_FILE_EXTENSION_NAME));
						TiXmlElement*l_pTiXmlElement = new TiXmlElement(cToneData::TypeID);
						std::wstring l_strSoundFileName = L"MusicGame/";
						l_strSoundFileName += DNCT::GcStringToWchar(l_strGCOutputFileName);
						l_strSoundFileName += L"/";
						l_strSoundFileName += l_wstrFileName;
						l_pTiXmlElement->SetAttribute(UT::CharToWchar(TONE_DATA_SOUND_SOURCE_FILE_NAME).c_str(),l_strSoundFileName.c_str());
						l_pTiXmlElement->SetAttribute(UT::CharToWchar(TONE_DATA_ID).c_str(),UT::GetFileNameWithoutFullPath(l_wstrFileName.c_str()));
						l_pRootTiXmlElement->LinkEndChild(l_pTiXmlElement);
					}
				}
				l_ISAXCallback.SetDoc(l_pTiXmlDocument);
				l_ISAXCallback.Export(l_strOutputFolderName.c_str(),false);

			}
		 }
private: System::Void CompareAndCaptureToFileParseFPS_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			int		l_iAmplitude = (int)CompareAndCaptureToFileMinAmplitude_numericUpDown->Value;
			float	l_fParseFPS = (float)CompareAndCaptureToFileParseFPS_numericUpDown->Value;
			int		l_iFreqOffset = (int)CompareAndCaptureToFileFreqOffset_numericUpDown->Value;
			int		l_iTolerateTime = (int)CompareAndCaptureToFileTolerateTime_numericUpDown->Value;
			int		l_iKeepTime = (int)CompareAndCaptureToFileKeepTime_numericUpDown->Value;
		 }
};
}