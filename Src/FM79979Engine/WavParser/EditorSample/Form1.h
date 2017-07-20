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
private: System::Windows::Forms::NumericUpDown^  numericUpDown2;
private: System::Windows::Forms::Button^  button1;
private: System::Windows::Forms::NumericUpDown^  numericUpDown3;
private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
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
	private: System::Windows::Forms::NumericUpDown^  WaveUpdateIndex_numericUpDown;
	private: System::Windows::Forms::Label^  WaveUpdateIndex_label;
	private: System::Windows::Forms::CheckBox^  FFTTestPause_checkBox;
	private: System::Windows::Forms::Button^  WavToOggOnlyOneChannel;
	private: System::Windows::Forms::TrackBar^  GoToTime_trackBar;
	private: System::Windows::Forms::Label^  FFTDataCountScale_label;
	private: System::Windows::Forms::NumericUpDown^  FFTDataCountScale_numericUpDown;
	private: System::Windows::Forms::Label^  JumpToTime_label;
	private: System::Windows::Forms::Label^  SourceName_label;
	private: System::Windows::Forms::Label^  ConvertToName_label;
	private: System::Windows::Forms::GroupBox^  SoundFileConvert_groupBox;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::CheckBox^  DoFFTFilter_checkBox;
	private: System::Windows::Forms::GroupBox^  SoundCapture_groupBox;
	private: System::Windows::Forms::Label^  ChartScale_label;
	private: System::Windows::Forms::NumericUpDown^  FFTChartScale_numericUpDown;
	private: System::Windows::Forms::Label^  SoundCaptureDoFFTChartScale_label;
	private: System::Windows::Forms::NumericUpDown^  SoundCaptureDoFFTChartScale_numericUpDown;
	private: System::Windows::Forms::CheckBox^  SoundCaptureDoFFTFilter_checkBox;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Button^  CaptureToFile_button;
	private: System::Windows::Forms::Label^  CompareAndCaptureToFileMinAmplitude_label;
	private: System::Windows::Forms::Label^  CompareAndCaptureToFileKeepTime_label;
	private: System::Windows::Forms::Label^  CompareAndCaptureToFileParseFPS_label;
	private: System::Windows::Forms::NumericUpDown^  CompareAndCaptureToFileMinAmplitude_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  CompareAndCaptureToFileParseFPS_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  CompareAndCaptureToFileKeepTime_numericUpDown;
	private: System::Windows::Forms::Label^  CompareAndCaptureToFileFilterStrangth_label;
	private: System::Windows::Forms::NumericUpDown^  CompareAndCaptureToFileFilterStrangth_numericUpDown;
	private: System::Windows::Forms::Label^  CompareAndCaptureToFileFilterRange_label;
	private: System::Windows::Forms::NumericUpDown^  CompareAndCaptureToFileFilterRange_numericUpDown;
	private: System::Windows::Forms::Label^  CompareAndCaptureToFileMaxAllowFrequency_label;
	private: System::Windows::Forms::NumericUpDown^  CompareAndCaptureToFileMaxAllowFrequency_numericUpDown;
	private: System::Windows::Forms::Label^  CompareAndCaptureToFileMinAllowFrequency_label;
	private: System::Windows::Forms::NumericUpDown^  CompareAndCaptureToFileMinAllowFrequency_numericUpDown;
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
	private: System::Windows::Forms::Label^  FFTPrintFrequencyThreshold_label;
	private: System::Windows::Forms::NumericUpDown^  FFTPrintFrequencyThreshold_numericUpDown;
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
			this->WaveUpdateIndex_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->WaveUpdateIndex_label = (gcnew System::Windows::Forms::Label());
			this->FFTTestPause_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->WavToOggOnlyOneChannel = (gcnew System::Windows::Forms::Button());
			this->FFTDataCountScale_label = (gcnew System::Windows::Forms::Label());
			this->FFTDataCountScale_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->GoToTime_trackBar = (gcnew System::Windows::Forms::TrackBar());
			this->JumpToTime_label = (gcnew System::Windows::Forms::Label());
			this->SourceName_label = (gcnew System::Windows::Forms::Label());
			this->ConvertToName_label = (gcnew System::Windows::Forms::Label());
			this->SoundFileConvert_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->FFT_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->FFT_FPS_label = (gcnew System::Windows::Forms::Label());
			this->FFT_FPS_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->ChartScale_label = (gcnew System::Windows::Forms::Label());
			this->FFTStoreExport_button = (gcnew System::Windows::Forms::Button());
			this->FFTChartScale_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->DoFFTFilter_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ForwardStep_button = (gcnew System::Windows::Forms::Button());
			this->BackStep_button = (gcnew System::Windows::Forms::Button());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->Filter_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->numericUpDown3 = (gcnew System::Windows::Forms::NumericUpDown());
			this->FFTPrintFrequencyThreshold_label = (gcnew System::Windows::Forms::Label());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->FFTPrintFrequencyThreshold_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
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
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->CompareAndCaptureToFileMaxAllowFrequency_label = (gcnew System::Windows::Forms::Label());
			this->CompareAndCaptureToFileMaxAllowFrequency_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CompareAndCaptureToFileMinAllowFrequency_label = (gcnew System::Windows::Forms::Label());
			this->CompareAndCaptureToFileMinAllowFrequency_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CompareAndCaptureToFileFilterStrangth_label = (gcnew System::Windows::Forms::Label());
			this->CompareAndCaptureToFileFilterStrangth_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CompareAndCaptureToFileMinAmplitude_label = (gcnew System::Windows::Forms::Label());
			this->CompareAndCaptureToFileKeepTime_label = (gcnew System::Windows::Forms::Label());
			this->CompareAndCaptureToFileFilterRange_label = (gcnew System::Windows::Forms::Label());
			this->CompareAndCaptureToFileFilterRange_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CompareAndCaptureToFileParseFPS_label = (gcnew System::Windows::Forms::Label());
			this->CompareAndCaptureToFileMinAmplitude_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CompareAndCaptureToFileParseFPS_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CompareAndCaptureToFileKeepTime_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CaptureToFile_button = (gcnew System::Windows::Forms::Button());
			this->SoundCapture_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->CpatureSound_button = (gcnew System::Windows::Forms::Button());
			this->CurrentRecordName_label = (gcnew System::Windows::Forms::Label());
			this->CurrentRecordName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->SoundCaptureDoFFTChartScale_label = (gcnew System::Windows::Forms::Label());
			this->SoundCaptureDoFFTFilter_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->SoundCaptureDoFFTChartScale_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CurrentTime_trackBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->WaveUpdateIndex_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTDataCountScale_numericUpDown))->BeginInit();
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
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTPrintFrequencyThreshold_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTStoreAmplitude_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTStoreGittedCountThreshold_numericUpDown))->BeginInit();
			this->FFTPrint_groupBox->SuspendLayout();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileMaxAllowFrequency_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileMinAllowFrequency_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileFilterStrangth_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileFilterRange_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileMinAmplitude_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileParseFPS_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileKeepTime_numericUpDown))->BeginInit();
			this->SoundCapture_groupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SoundCaptureDoFFTChartScale_numericUpDown))->BeginInit();
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
			this->WavFileName_textBox->Location = System::Drawing::Point(285, 109);
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
			this->OPENGL_panel->Size = System::Drawing::Size(1884, 542);
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
			this->FFTTest_button->Location = System::Drawing::Point(281, 19);
			this->FFTTest_button->Name = L"FFTTest_button";
			this->FFTTest_button->Size = System::Drawing::Size(75, 23);
			this->FFTTest_button->TabIndex = 18;
			this->FFTTest_button->Text = L"FFT test";
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
			// WaveUpdateIndex_numericUpDown
			// 
			this->WaveUpdateIndex_numericUpDown->Location = System::Drawing::Point(9, 37);
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
			this->WaveUpdateIndex_label->Location = System::Drawing::Point(6, 15);
			this->WaveUpdateIndex_label->Name = L"WaveUpdateIndex_label";
			this->WaveUpdateIndex_label->Size = System::Drawing::Size(97, 13);
			this->WaveUpdateIndex_label->TabIndex = 24;
			this->WaveUpdateIndex_label->Text = L"WaveUpdateIndex";
			// 
			// FFTTestPause_checkBox
			// 
			this->FFTTestPause_checkBox->AutoSize = true;
			this->FFTTestPause_checkBox->Location = System::Drawing::Point(119, 61);
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
			// FFTDataCountScale_label
			// 
			this->FFTDataCountScale_label->AutoSize = true;
			this->FFTDataCountScale_label->Location = System::Drawing::Point(121, 16);
			this->FFTDataCountScale_label->Name = L"FFTDataCountScale_label";
			this->FFTDataCountScale_label->Size = System::Drawing::Size(104, 13);
			this->FFTDataCountScale_label->TabIndex = 28;
			this->FFTDataCountScale_label->Text = L"FFTDataCountScale";
			// 
			// FFTDataCountScale_numericUpDown
			// 
			this->FFTDataCountScale_numericUpDown->DecimalPlaces = 1;
			this->FFTDataCountScale_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->FFTDataCountScale_numericUpDown->Location = System::Drawing::Point(121, 35);
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
			this->SourceName_label->Location = System::Drawing::Point(282, 82);
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
			// SoundFileConvert_groupBox
			// 
			this->SoundFileConvert_groupBox->Controls->Add(this->ConvertToName_label);
			this->SoundFileConvert_groupBox->Controls->Add(this->RecordSoundFileName_textBox);
			this->SoundFileConvert_groupBox->Controls->Add(this->ToOgg_button);
			this->SoundFileConvert_groupBox->Controls->Add(this->WavToOggOnlyOneChannel);
			this->SoundFileConvert_groupBox->Controls->Add(this->ConvertToWav_button);
			this->SoundFileConvert_groupBox->Location = System::Drawing::Point(198, 292);
			this->SoundFileConvert_groupBox->Name = L"SoundFileConvert_groupBox";
			this->SoundFileConvert_groupBox->Size = System::Drawing::Size(175, 120);
			this->SoundFileConvert_groupBox->TabIndex = 33;
			this->SoundFileConvert_groupBox->TabStop = false;
			this->SoundFileConvert_groupBox->Text = L"SoundFileConvert";
			// 
			// FFT_groupBox
			// 
			this->FFT_groupBox->Controls->Add(this->FFT_FPS_label);
			this->FFT_groupBox->Controls->Add(this->FFT_FPS_numericUpDown);
			this->FFT_groupBox->Controls->Add(this->ChartScale_label);
			this->FFT_groupBox->Controls->Add(this->FFTStoreExport_button);
			this->FFT_groupBox->Controls->Add(this->FFTChartScale_numericUpDown);
			this->FFT_groupBox->Controls->Add(this->FFTTest_button);
			this->FFT_groupBox->Controls->Add(this->WaveUpdateIndex_label);
			this->FFT_groupBox->Controls->Add(this->DoFFTFilter_checkBox);
			this->FFT_groupBox->Controls->Add(this->SourceName_label);
			this->FFT_groupBox->Controls->Add(this->CurrentTime_trackBar);
			this->FFT_groupBox->Controls->Add(this->JumpToTime_label);
			this->FFT_groupBox->Controls->Add(this->WavFileName_textBox);
			this->FFT_groupBox->Controls->Add(this->FFTDataCountScale_label);
			this->FFT_groupBox->Controls->Add(this->CurrentTime_label);
			this->FFT_groupBox->Controls->Add(this->FFTDataCountScale_numericUpDown);
			this->FFT_groupBox->Controls->Add(this->ForwardStep_button);
			this->FFT_groupBox->Controls->Add(this->GoToTime_trackBar);
			this->FFT_groupBox->Controls->Add(this->BackStep_button);
			this->FFT_groupBox->Controls->Add(this->FFTTestPause_checkBox);
			this->FFT_groupBox->Controls->Add(this->WaveUpdateIndex_numericUpDown);
			this->FFT_groupBox->Location = System::Drawing::Point(3, 7);
			this->FFT_groupBox->Name = L"FFT_groupBox";
			this->FFT_groupBox->Size = System::Drawing::Size(490, 285);
			this->FFT_groupBox->TabIndex = 34;
			this->FFT_groupBox->TabStop = false;
			this->FFT_groupBox->Text = L"single FFT";
			// 
			// FFT_FPS_label
			// 
			this->FFT_FPS_label->AutoSize = true;
			this->FFT_FPS_label->Location = System::Drawing::Point(118, 81);
			this->FFT_FPS_label->Name = L"FFT_FPS_label";
			this->FFT_FPS_label->Size = System::Drawing::Size(52, 13);
			this->FFT_FPS_label->TabIndex = 39;
			this->FFT_FPS_label->Text = L"FFT_FPS";
			// 
			// FFT_FPS_numericUpDown
			// 
			this->FFT_FPS_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->FFT_FPS_numericUpDown->Location = System::Drawing::Point(119, 100);
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
			this->ChartScale_label->Location = System::Drawing::Point(285, 149);
			this->ChartScale_label->Name = L"ChartScale_label";
			this->ChartScale_label->Size = System::Drawing::Size(59, 13);
			this->ChartScale_label->TabIndex = 35;
			this->ChartScale_label->Text = L"ChartScale";
			// 
			// FFTStoreExport_button
			// 
			this->FFTStoreExport_button->Location = System::Drawing::Point(387, 20);
			this->FFTStoreExport_button->Name = L"FFTStoreExport_button";
			this->FFTStoreExport_button->Size = System::Drawing::Size(91, 20);
			this->FFTStoreExport_button->TabIndex = 2;
			this->FFTStoreExport_button->Text = L"FFTStoreExport";
			this->FFTStoreExport_button->UseVisualStyleBackColor = true;
			this->FFTStoreExport_button->Click += gcnew System::EventHandler(this, &Form1::FFTStoreExport_button_Click);
			// 
			// FFTChartScale_numericUpDown
			// 
			this->FFTChartScale_numericUpDown->DecimalPlaces = 2;
			this->FFTChartScale_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->FFTChartScale_numericUpDown->Location = System::Drawing::Point(288, 168);
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
			this->DoFFTFilter_checkBox->Location = System::Drawing::Point(289, 211);
			this->DoFFTFilter_checkBox->Name = L"DoFFTFilter_checkBox";
			this->DoFFTFilter_checkBox->Size = System::Drawing::Size(81, 17);
			this->DoFFTFilter_checkBox->TabIndex = 33;
			this->DoFFTFilter_checkBox->Text = L"DoFFTFilter";
			this->DoFFTFilter_checkBox->UseVisualStyleBackColor = true;
			this->DoFFTFilter_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::DoFFTFilter_checkBox_CheckedChanged);
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
			this->splitContainer1->Panel2->Controls->Add(this->groupBox1);
			this->splitContainer1->Panel2->Controls->Add(this->SoundCapture_groupBox);
			this->splitContainer1->Panel2->Controls->Add(this->FFT_groupBox);
			this->splitContainer1->Panel2->Controls->Add(this->SoundFileConvert_groupBox);
			this->splitContainer1->Size = System::Drawing::Size(1884, 961);
			this->splitContainer1->SplitterDistance = 542;
			this->splitContainer1->TabIndex = 35;
			// 
			// Filter_groupBox
			// 
			this->Filter_groupBox->Controls->Add(this->numericUpDown2);
			this->Filter_groupBox->Controls->Add(this->button1);
			this->Filter_groupBox->Controls->Add(this->numericUpDown3);
			this->Filter_groupBox->Controls->Add(this->FFTPrintFrequencyThreshold_label);
			this->Filter_groupBox->Controls->Add(this->numericUpDown1);
			this->Filter_groupBox->Controls->Add(this->FFTPrintFrequencyThreshold_numericUpDown);
			this->Filter_groupBox->Controls->Add(this->FFTStoreAmplitude_numericUpDown);
			this->Filter_groupBox->Controls->Add(this->FFTStoreGittedCountThreshold_label);
			this->Filter_groupBox->Controls->Add(this->FFTStoreAmplitude_label);
			this->Filter_groupBox->Controls->Add(this->FFTStoreGittedCountThreshold_numericUpDown);
			this->Filter_groupBox->Location = System::Drawing::Point(499, 16);
			this->Filter_groupBox->Name = L"Filter_groupBox";
			this->Filter_groupBox->Size = System::Drawing::Size(206, 375);
			this->Filter_groupBox->TabIndex = 53;
			this->Filter_groupBox->TabStop = false;
			this->Filter_groupBox->Text = L"FilterParameter";
			// 
			// numericUpDown2
			// 
			this->numericUpDown2->Location = System::Drawing::Point(26, 293);
			this->numericUpDown2->Name = L"numericUpDown2";
			this->numericUpDown2->Size = System::Drawing::Size(120, 20);
			this->numericUpDown2->TabIndex = 55;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(26, 327);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 53;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// numericUpDown3
			// 
			this->numericUpDown3->Location = System::Drawing::Point(26, 265);
			this->numericUpDown3->Name = L"numericUpDown3";
			this->numericUpDown3->Size = System::Drawing::Size(120, 20);
			this->numericUpDown3->TabIndex = 56;
			// 
			// FFTPrintFrequencyThreshold_label
			// 
			this->FFTPrintFrequencyThreshold_label->AutoSize = true;
			this->FFTPrintFrequencyThreshold_label->Location = System::Drawing::Point(9, 13);
			this->FFTPrintFrequencyThreshold_label->Name = L"FFTPrintFrequencyThreshold_label";
			this->FFTPrintFrequencyThreshold_label->Size = System::Drawing::Size(137, 26);
			this->FFTPrintFrequencyThreshold_label->TabIndex = 52;
			this->FFTPrintFrequencyThreshold_label->Text = L"FrequencyThreshold\r\nsmall than this will be ignore";
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Location = System::Drawing::Point(26, 239);
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(120, 20);
			this->numericUpDown1->TabIndex = 54;
			// 
			// FFTPrintFrequencyThreshold_numericUpDown
			// 
			this->FFTPrintFrequencyThreshold_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 
				65536});
			this->FFTPrintFrequencyThreshold_numericUpDown->Location = System::Drawing::Point(11, 40);
			this->FFTPrintFrequencyThreshold_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {44100, 0, 
				0, 0});
			this->FFTPrintFrequencyThreshold_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {7, 0, 0, 
				0});
			this->FFTPrintFrequencyThreshold_numericUpDown->Name = L"FFTPrintFrequencyThreshold_numericUpDown";
			this->FFTPrintFrequencyThreshold_numericUpDown->Size = System::Drawing::Size(103, 20);
			this->FFTPrintFrequencyThreshold_numericUpDown->TabIndex = 51;
			this->FFTPrintFrequencyThreshold_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {2000, 0, 0, 
				0});
			// 
			// FFTStoreAmplitude_numericUpDown
			// 
			this->FFTStoreAmplitude_numericUpDown->Location = System::Drawing::Point(13, 78);
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
			this->FFTStoreGittedCountThreshold_label->Location = System::Drawing::Point(10, 105);
			this->FFTStoreGittedCountThreshold_label->Name = L"FFTStoreGittedCountThreshold_label";
			this->FFTStoreGittedCountThreshold_label->Size = System::Drawing::Size(154, 13);
			this->FFTStoreGittedCountThreshold_label->TabIndex = 48;
			this->FFTStoreGittedCountThreshold_label->Text = L"FFTStoreGittedCountThreshold";
			// 
			// FFTStoreAmplitude_label
			// 
			this->FFTStoreAmplitude_label->AutoSize = true;
			this->FFTStoreAmplitude_label->Location = System::Drawing::Point(10, 62);
			this->FFTStoreAmplitude_label->Name = L"FFTStoreAmplitude_label";
			this->FFTStoreAmplitude_label->Size = System::Drawing::Size(144, 13);
			this->FFTStoreAmplitude_label->TabIndex = 49;
			this->FFTStoreAmplitude_label->Text = L"FFTStoreAmplitudeThreshold";
			// 
			// FFTStoreGittedCountThreshold_numericUpDown
			// 
			this->FFTStoreGittedCountThreshold_numericUpDown->Location = System::Drawing::Point(12, 121);
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
			this->FFTPrint_groupBox->Size = System::Drawing::Size(166, 372);
			this->FFTPrint_groupBox->TabIndex = 41;
			this->FFTPrint_groupBox->TabStop = false;
			this->FFTPrint_groupBox->Text = L"FFTPrint";
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
			this->MIDIToXml_button->Location = System::Drawing::Point(8, 354);
			this->MIDIToXml_button->Name = L"MIDIToXml_button";
			this->MIDIToXml_button->Size = System::Drawing::Size(116, 44);
			this->MIDIToXml_button->TabIndex = 40;
			this->MIDIToXml_button->Text = L"MIDIToXml";
			this->MIDIToXml_button->UseVisualStyleBackColor = true;
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
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileMaxAllowFrequency_label);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileMaxAllowFrequency_numericUpDown);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileMinAllowFrequency_label);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileMinAllowFrequency_numericUpDown);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileFilterStrangth_label);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileFilterStrangth_numericUpDown);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileMinAmplitude_label);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileKeepTime_label);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileFilterRange_label);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileFilterRange_numericUpDown);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileParseFPS_label);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileMinAmplitude_numericUpDown);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileParseFPS_numericUpDown);
			this->groupBox1->Controls->Add(this->CompareAndCaptureToFileKeepTime_numericUpDown);
			this->groupBox1->Controls->Add(this->CaptureToFile_button);
			this->groupBox1->Location = System::Drawing::Point(871, 8);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(210, 251);
			this->groupBox1->TabIndex = 37;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"CompareAndCaptureToFile";
			// 
			// CompareAndCaptureToFileMaxAllowFrequency_label
			// 
			this->CompareAndCaptureToFileMaxAllowFrequency_label->AutoSize = true;
			this->CompareAndCaptureToFileMaxAllowFrequency_label->Location = System::Drawing::Point(2, 191);
			this->CompareAndCaptureToFileMaxAllowFrequency_label->Name = L"CompareAndCaptureToFileMaxAllowFrequency_label";
			this->CompareAndCaptureToFileMaxAllowFrequency_label->Size = System::Drawing::Size(102, 13);
			this->CompareAndCaptureToFileMaxAllowFrequency_label->TabIndex = 51;
			this->CompareAndCaptureToFileMaxAllowFrequency_label->Text = L"MaxAllowFrequency";
			// 
			// CompareAndCaptureToFileMaxAllowFrequency_numericUpDown
			// 
			this->CompareAndCaptureToFileMaxAllowFrequency_numericUpDown->Location = System::Drawing::Point(103, 190);
			this->CompareAndCaptureToFileMaxAllowFrequency_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {44100, 
				0, 0, 0});
			this->CompareAndCaptureToFileMaxAllowFrequency_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 
				0, 0, 0});
			this->CompareAndCaptureToFileMaxAllowFrequency_numericUpDown->Name = L"CompareAndCaptureToFileMaxAllowFrequency_numericUpDown";
			this->CompareAndCaptureToFileMaxAllowFrequency_numericUpDown->Size = System::Drawing::Size(94, 20);
			this->CompareAndCaptureToFileMaxAllowFrequency_numericUpDown->TabIndex = 50;
			this->CompareAndCaptureToFileMaxAllowFrequency_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {20000, 
				0, 0, 0});
			// 
			// CompareAndCaptureToFileMinAllowFrequency_label
			// 
			this->CompareAndCaptureToFileMinAllowFrequency_label->AutoSize = true;
			this->CompareAndCaptureToFileMinAllowFrequency_label->Location = System::Drawing::Point(2, 165);
			this->CompareAndCaptureToFileMinAllowFrequency_label->Name = L"CompareAndCaptureToFileMinAllowFrequency_label";
			this->CompareAndCaptureToFileMinAllowFrequency_label->Size = System::Drawing::Size(99, 13);
			this->CompareAndCaptureToFileMinAllowFrequency_label->TabIndex = 49;
			this->CompareAndCaptureToFileMinAllowFrequency_label->Text = L"MinAllowFrequency";
			// 
			// CompareAndCaptureToFileMinAllowFrequency_numericUpDown
			// 
			this->CompareAndCaptureToFileMinAllowFrequency_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 
				0, 0, 65536});
			this->CompareAndCaptureToFileMinAllowFrequency_numericUpDown->Location = System::Drawing::Point(103, 163);
			this->CompareAndCaptureToFileMinAllowFrequency_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {44100, 
				0, 0, 0});
			this->CompareAndCaptureToFileMinAllowFrequency_numericUpDown->Name = L"CompareAndCaptureToFileMinAllowFrequency_numericUpDown";
			this->CompareAndCaptureToFileMinAllowFrequency_numericUpDown->Size = System::Drawing::Size(94, 20);
			this->CompareAndCaptureToFileMinAllowFrequency_numericUpDown->TabIndex = 48;
			this->CompareAndCaptureToFileMinAllowFrequency_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {60, 
				0, 0, 0});
			// 
			// CompareAndCaptureToFileFilterStrangth_label
			// 
			this->CompareAndCaptureToFileFilterStrangth_label->AutoSize = true;
			this->CompareAndCaptureToFileFilterStrangth_label->Location = System::Drawing::Point(7, 139);
			this->CompareAndCaptureToFileFilterStrangth_label->Name = L"CompareAndCaptureToFileFilterStrangth_label";
			this->CompareAndCaptureToFileFilterStrangth_label->Size = System::Drawing::Size(69, 13);
			this->CompareAndCaptureToFileFilterStrangth_label->TabIndex = 41;
			this->CompareAndCaptureToFileFilterStrangth_label->Text = L"FilterStrength";
			// 
			// CompareAndCaptureToFileFilterStrangth_numericUpDown
			// 
			this->CompareAndCaptureToFileFilterStrangth_numericUpDown->Location = System::Drawing::Point(103, 138);
			this->CompareAndCaptureToFileFilterStrangth_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 
				0, 0, 0});
			this->CompareAndCaptureToFileFilterStrangth_numericUpDown->Name = L"CompareAndCaptureToFileFilterStrangth_numericUpDown";
			this->CompareAndCaptureToFileFilterStrangth_numericUpDown->Size = System::Drawing::Size(94, 20);
			this->CompareAndCaptureToFileFilterStrangth_numericUpDown->TabIndex = 40;
			this->CompareAndCaptureToFileFilterStrangth_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {80, 
				0, 0, 0});
			// 
			// CompareAndCaptureToFileMinAmplitude_label
			// 
			this->CompareAndCaptureToFileMinAmplitude_label->AutoSize = true;
			this->CompareAndCaptureToFileMinAmplitude_label->Location = System::Drawing::Point(6, 88);
			this->CompareAndCaptureToFileMinAmplitude_label->Name = L"CompareAndCaptureToFileMinAmplitude_label";
			this->CompareAndCaptureToFileMinAmplitude_label->Size = System::Drawing::Size(70, 13);
			this->CompareAndCaptureToFileMinAmplitude_label->TabIndex = 47;
			this->CompareAndCaptureToFileMinAmplitude_label->Text = L"MinAmplitude";
			// 
			// CompareAndCaptureToFileKeepTime_label
			// 
			this->CompareAndCaptureToFileKeepTime_label->AutoSize = true;
			this->CompareAndCaptureToFileKeepTime_label->Location = System::Drawing::Point(6, 59);
			this->CompareAndCaptureToFileKeepTime_label->Name = L"CompareAndCaptureToFileKeepTime_label";
			this->CompareAndCaptureToFileKeepTime_label->Size = System::Drawing::Size(55, 13);
			this->CompareAndCaptureToFileKeepTime_label->TabIndex = 46;
			this->CompareAndCaptureToFileKeepTime_label->Text = L"KeepTime";
			// 
			// CompareAndCaptureToFileFilterRange_label
			// 
			this->CompareAndCaptureToFileFilterRange_label->AutoSize = true;
			this->CompareAndCaptureToFileFilterRange_label->Location = System::Drawing::Point(6, 113);
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
			this->CompareAndCaptureToFileFilterRange_numericUpDown->Location = System::Drawing::Point(103, 111);
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
			this->CompareAndCaptureToFileMinAmplitude_numericUpDown->Location = System::Drawing::Point(103, 86);
			this->CompareAndCaptureToFileMinAmplitude_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 
				0, 0, 0});
			this->CompareAndCaptureToFileMinAmplitude_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 
				0, 0, 0});
			this->CompareAndCaptureToFileMinAmplitude_numericUpDown->Name = L"CompareAndCaptureToFileMinAmplitude_numericUpDown";
			this->CompareAndCaptureToFileMinAmplitude_numericUpDown->Size = System::Drawing::Size(94, 20);
			this->CompareAndCaptureToFileMinAmplitude_numericUpDown->TabIndex = 42;
			this->CompareAndCaptureToFileMinAmplitude_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {80, 
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
			this->CompareAndCaptureToFileParseFPS_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {60, 0, 
				0, 0});
			this->CompareAndCaptureToFileParseFPS_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::CompareAndCaptureToFileParseFPS_numericUpDown_ValueChanged);
			// 
			// CompareAndCaptureToFileKeepTime_numericUpDown
			// 
			this->CompareAndCaptureToFileKeepTime_numericUpDown->DecimalPlaces = 2;
			this->CompareAndCaptureToFileKeepTime_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 
				0, 0, 196608});
			this->CompareAndCaptureToFileKeepTime_numericUpDown->Location = System::Drawing::Point(103, 57);
			this->CompareAndCaptureToFileKeepTime_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {16, 
				0, 0, 196608});
			this->CompareAndCaptureToFileKeepTime_numericUpDown->Name = L"CompareAndCaptureToFileKeepTime_numericUpDown";
			this->CompareAndCaptureToFileKeepTime_numericUpDown->Size = System::Drawing::Size(94, 20);
			this->CompareAndCaptureToFileKeepTime_numericUpDown->TabIndex = 38;
			this->CompareAndCaptureToFileKeepTime_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 
				0, 65536});
			this->CompareAndCaptureToFileKeepTime_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::CompareAndCaptureToFileParseFPS_numericUpDown_ValueChanged);
			// 
			// CaptureToFile_button
			// 
			this->CaptureToFile_button->Location = System::Drawing::Point(59, 216);
			this->CaptureToFile_button->Name = L"CaptureToFile_button";
			this->CaptureToFile_button->Size = System::Drawing::Size(100, 23);
			this->CaptureToFile_button->TabIndex = 0;
			this->CaptureToFile_button->Text = L"CaptureToFile";
			this->CaptureToFile_button->UseVisualStyleBackColor = true;
			this->CaptureToFile_button->Click += gcnew System::EventHandler(this, &Form1::CaptureToFile_button_Click);
			// 
			// SoundCapture_groupBox
			// 
			this->SoundCapture_groupBox->Controls->Add(this->CpatureSound_button);
			this->SoundCapture_groupBox->Controls->Add(this->CurrentRecordName_label);
			this->SoundCapture_groupBox->Controls->Add(this->CurrentRecordName_textBox);
			this->SoundCapture_groupBox->Controls->Add(this->SoundCaptureDoFFTChartScale_label);
			this->SoundCapture_groupBox->Controls->Add(this->SoundCaptureDoFFTFilter_checkBox);
			this->SoundCapture_groupBox->Controls->Add(this->SoundCaptureDoFFTChartScale_numericUpDown);
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
			this->CpatureSound_button->Text = L"CpatureSound\r\nExport";
			this->CpatureSound_button->UseVisualStyleBackColor = true;
			this->CpatureSound_button->Click += gcnew System::EventHandler(this, &Form1::CpatureSound_button_Click);
			// 
			// CurrentRecordName_label
			// 
			this->CurrentRecordName_label->AutoSize = true;
			this->CurrentRecordName_label->Location = System::Drawing::Point(4, 171);
			this->CurrentRecordName_label->Name = L"CurrentRecordName_label";
			this->CurrentRecordName_label->Size = System::Drawing::Size(104, 13);
			this->CurrentRecordName_label->TabIndex = 41;
			this->CurrentRecordName_label->Text = L"CurrentRecordName";
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
			// SoundCaptureDoFFTChartScale_label
			// 
			this->SoundCaptureDoFFTChartScale_label->AutoSize = true;
			this->SoundCaptureDoFFTChartScale_label->Location = System::Drawing::Point(4, 250);
			this->SoundCaptureDoFFTChartScale_label->Name = L"SoundCaptureDoFFTChartScale_label";
			this->SoundCaptureDoFFTChartScale_label->Size = System::Drawing::Size(59, 13);
			this->SoundCaptureDoFFTChartScale_label->TabIndex = 41;
			this->SoundCaptureDoFFTChartScale_label->Text = L"ChartScale";
			// 
			// SoundCaptureDoFFTFilter_checkBox
			// 
			this->SoundCaptureDoFFTFilter_checkBox->AutoSize = true;
			this->SoundCaptureDoFFTFilter_checkBox->Location = System::Drawing::Point(7, 223);
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
			this->SoundCaptureDoFFTChartScale_numericUpDown->Location = System::Drawing::Point(7, 266);
			this->SoundCaptureDoFFTChartScale_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 
				65536});
			this->SoundCaptureDoFFTChartScale_numericUpDown->Name = L"SoundCaptureDoFFTChartScale_numericUpDown";
			this->SoundCaptureDoFFTChartScale_numericUpDown->Size = System::Drawing::Size(93, 20);
			this->SoundCaptureDoFFTChartScale_numericUpDown->TabIndex = 40;
			this->SoundCaptureDoFFTChartScale_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->SoundCaptureDoFFTChartScale_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::SoundCaptureDoFFTChartScale_numericUpDown_ValueChanged);
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
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->WaveUpdateIndex_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTDataCountScale_numericUpDown))->EndInit();
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
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTPrintFrequencyThreshold_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTStoreAmplitude_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTStoreGittedCountThreshold_numericUpDown))->EndInit();
			this->FFTPrint_groupBox->ResumeLayout(false);
			this->FFTPrint_groupBox->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileMaxAllowFrequency_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileMinAllowFrequency_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileFilterStrangth_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileFilterRange_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileMinAmplitude_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileParseFPS_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CompareAndCaptureToFileKeepTime_numericUpDown))->EndInit();
			this->SoundCapture_groupBox->ResumeLayout(false);
			this->SoundCapture_groupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SoundCaptureDoFFTChartScale_numericUpDown))->EndInit();
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
			float l_fElpaseTime = this->m_pGameApp->m_sTimeAndFPS.fElpaseTime*this->m_pGameApp->m_sfDebugValue;
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
			}
#ifdef DEBUG
			GLRender::RenderRectangle(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,cMatrix44::Identity,Vector4(1,1,0,1));
			cGameApp::ShowInfo();
#endif
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
					 RecordSound_button->Text = "Recording...";
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
					//m_pKissFFTConvertBase->SetiFrenquenceFilterEndScaleValue((float)FFTFilterFrenquenceScale_numericUpDown->Value);
					//m_pKissFFTConvertBase->SetFilterStrengthValue((int)FilterStrength_numericUpDown->Value);
					 float l_fTimeToUpDate = 1.f/(int)WaveUpdateIndex_numericUpDown->Value;
					 m_pKissFFTConvertBase->SetFFTDataUpdateTime(l_fTimeToUpDate);
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
				this->timer1->Enabled = false;
				WaitUpdateEnd();
				int l_iTargetSecond = (int)GoToTime_trackBar->Value;
				m_pKissFFTConvertBase->GoToTime(l_iTargetSecond/10.f);
				this->timer1->Enabled = true;
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

private: System::Void SoundCaptureDoFFTChartScale_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(m_pSoundFFTCapture)
			 {
				 m_pSoundFFTCapture->SetChartScale((float)SoundCaptureDoFFTChartScale_numericUpDown->Value);
			 }
		 }

private: System::Void CaptureToFile_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			cli::array<String^>^l_strFileNames = DNCT::OpenFileAndGetNames("wav files (*.wav)|*.wav|All files (*.*)|*.*");
			if( l_strFileNames )
			{
				int		l_iAmplitude = (int)CompareAndCaptureToFileMinAmplitude_numericUpDown->Value;
				int		l_iParseFPS = (int)CompareAndCaptureToFileParseFPS_numericUpDown->Value;
				float	l_fKeepTime = (float)CompareAndCaptureToFileKeepTime_numericUpDown->Value;
				float	l_fFilterRange = (float)CompareAndCaptureToFileFilterRange_numericUpDown->Value;
				int		l_iFilterStrangth = (int)CompareAndCaptureToFileFilterStrangth_numericUpDown->Value;
				int		l_iMinAllowFrequency = (int)CompareAndCaptureToFileMinAllowFrequency_numericUpDown->Value;
				int		l_iMaxAllowFrequency = (int)CompareAndCaptureToFileMaxAllowFrequency_numericUpDown->Value;
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
				std::vector<int> l_SourceFrequencyVector;
				//l_pRootTiXmlElement->SetAttribute(L"ToneDataFileName",l_strOutputFolderName.c_str());
				l_pTiXmlDocument->LinkEndChild(l_pRootTiXmlElement);
				for each(auto l_strFileName in l_strFileNames)
				{
					if( l_strFileName && l_strFileName->Length > 0 )
					{
						cTimeFrequencyAmplitudeValueCapture l_TimeFrequencyAmplitudeValueCapture;
						l_TimeFrequencyAmplitudeValueCapture.m_iParseFPS = l_iParseFPS;
						//l_TimeFrequencyAmplitudeValueCapture.m_fFrequencyOffsetRange = l_fFreqOffset;
						//l_TimeFrequencyAmplitudeValueCapture.m_fTolerateTime = l_fTolerateTime;
						l_TimeFrequencyAmplitudeValueCapture.m_fCaptureSoundRequireMinTime = l_fKeepTime;
						l_TimeFrequencyAmplitudeValueCapture.m_iMinAmplitude = l_iAmplitude;
						l_TimeFrequencyAmplitudeValueCapture.m_iMinAllowFrequency = l_iMinAllowFrequency;
						l_TimeFrequencyAmplitudeValueCapture.m_iMaxAllowFrequency = l_iMaxAllowFrequency;
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
						l_pTiXmlElement->SetAttribute(UT::CharToWchar(TONE_DATA_PICTURE_OFFSET_POS).c_str(),L"30,0");
						l_pRootTiXmlElement->LinkEndChild(l_pTiXmlElement);
						//
						cKissFFTConvert*l_pKissFFTConvert = l_TimeFrequencyAmplitudeValueCapture.GetKissFFTConvert();
						int l_iSourceFreq = l_TimeFrequencyAmplitudeValueCapture.GetKissFFTConvert()->GetSoundFile()->m_iFreq;
						if(!IsVectorContain<int>(l_iSourceFreq,&l_SourceFrequencyVector))
						{
							l_SourceFrequencyVector.push_back(l_iSourceFreq);
						}
						
					}
				}
				l_pRootTiXmlElement->SetAttribute(L"SourceFreq",ValueToStringW(l_SourceFrequencyVector));
				l_ISAXCallback.SetDoc(l_pTiXmlDocument);
				l_ISAXCallback.Export(l_strOutputFolderName.c_str(),false);

			}
		 }
private: System::Void CompareAndCaptureToFileParseFPS_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			int		l_iAmplitude = (int)CompareAndCaptureToFileMinAmplitude_numericUpDown->Value;
			float	l_fParseFPS = (float)CompareAndCaptureToFileParseFPS_numericUpDown->Value;
			int		l_iKeepTime = (int)CompareAndCaptureToFileKeepTime_numericUpDown->Value;
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
				 this->m_pGameApp->m_pFFTStoreExporter->AddFile(DNCT::GcStringToChar(l_strFileName));
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
			if(m_pKissFFTConvertBase)
			{
				m_pKissFFTConvertBase->GetFFTDataStore()->SetExportThresholdValue((int)FFTStoreGittedCountThreshold_numericUpDown->Value);
			}
			 if( m_pSoundFFTCapture )
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
				 m_pKissFFTConvertBase->DumpDebugInfo(cSoundCompareParameter::m_siDebugAmplitudeValue,l_strFileName.c_str(),(int)FFTPrintFrequencyThreshold_numericUpDown->Value);
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
				 m_pSoundFFTCapture->DumpDebugInfo(cSoundCompareParameter::m_siDebugAmplitudeValue,l_strOutName.c_str(),(int)FFTPrintFrequencyThreshold_numericUpDown->Value);
			 }
		 }
};
}