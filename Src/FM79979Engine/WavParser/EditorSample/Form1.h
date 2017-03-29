#pragma once
#include "WavWaves.h"
#include "KissFFTConvert.h"
#include "KissFFTStreaming.h"
#include "SoundFFTCapture.h"
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
			m_pSoundFFTCapture = new cSoundFFTCapture();
			int l_iFrequence = SOUND_CAPTURE_FREQUENCE*2;
			m_pSoundCapture = new cSoundCapture(l_iFrequence,AL_FORMAT_MONO16,m_pSoundFFTCapture->GetOpanalCaptureBufferSize(60,l_iFrequence));
			m_pSoundCapture->AddObject(m_pSoundFFTCapture);
			//m_pSoundCapture->AddSoundRecord();
			m_HdcMV = GetDC((HWND)m_pTargetControl->Handle.ToPointer());
			m_HGLRCMV = m_pGameApp->m_sHGLRC;
			//
			this->m_pWaveInfo = nullptr;
			this->m_pWavWaves = nullptr;
			//
			//m_pKissFFTConvertBase = new cKissFFTConvert();
			m_pKissFFTConvertBase = new cKissFFTStreamingConvert();
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
			SAFE_DELETE(m_pSoundCapture);
			m_pSoundFFTCapture = nullptr;
			SAFE_DELETE(m_pKissFFTConvertBase);
			SAFE_DELETE(this->m_pWavWaves);

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
		cWavWaves*					m_pWavWaves;
		Control^					m_pTargetControl;
		//
		System::Drawing::Size		m_MyOpenGLWindowSize;
		System::Drawing::Size		m_WindowStartSize;
		//
	private: System::Windows::Forms::Button^  OpenWavFile_button;
	private: System::Windows::Forms::TextBox^  WavFileName_textBox;
	private: System::Windows::Forms::Button^  ParseWav_button;


	private: System::Windows::Forms::Panel^  OPENGL_panel;
	private: System::Windows::Forms::TrackBar^  CurrentTime_trackBar;
	private: System::Windows::Forms::Label^  CurrentTime_label;
	private: System::Windows::Forms::NumericUpDown^  DataCompressRate_numericUpDown;
	private: System::Windows::Forms::Label^  DataCompressRate__label;
	private: System::Windows::Forms::Button^  ForwardStep_button;


	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  BackStep_button;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::NumericUpDown^  ShowWaveSeconds_numericUpDown;
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
private: System::Windows::Forms::Panel^  panel1;
private: System::Windows::Forms::TrackBar^  GoToTime_trackBar;
private: System::Windows::Forms::Label^  FFTDataCountScale_label;
private: System::Windows::Forms::NumericUpDown^  FFTDataCountScale_numericUpDown;



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
			this->ParseWav_button = (gcnew System::Windows::Forms::Button());
			this->OPENGL_panel = (gcnew System::Windows::Forms::Panel());
			this->WavInfo_listBox = (gcnew System::Windows::Forms::ListBox());
			this->CurrentTime_trackBar = (gcnew System::Windows::Forms::TrackBar());
			this->CurrentTime_label = (gcnew System::Windows::Forms::Label());
			this->DataCompressRate_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->DataCompressRate__label = (gcnew System::Windows::Forms::Label());
			this->ForwardStep_button = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->BackStep_button = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->ShowWaveSeconds_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
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
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->FFTDataCountScale_label = (gcnew System::Windows::Forms::Label());
			this->FFTDataCountScale_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->GoToTime_trackBar = (gcnew System::Windows::Forms::TrackBar());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CurrentTime_trackBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->DataCompressRate_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ShowWaveSeconds_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->WaveUpdateIndex_numericUpDown))->BeginInit();
			this->panel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTDataCountScale_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GoToTime_trackBar))->BeginInit();
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// OpenWavFile_button
			// 
			this->OpenWavFile_button->Location = System::Drawing::Point(915, 694);
			this->OpenWavFile_button->Name = L"OpenWavFile_button";
			this->OpenWavFile_button->Size = System::Drawing::Size(85, 34);
			this->OpenWavFile_button->TabIndex = 0;
			this->OpenWavFile_button->Text = L"open wav file";
			this->OpenWavFile_button->UseVisualStyleBackColor = true;
			this->OpenWavFile_button->Click += gcnew System::EventHandler(this, &Form1::OpenWavFile_button_Click);
			// 
			// WavFileName_textBox
			// 
			this->WavFileName_textBox->Location = System::Drawing::Point(591, 734);
			this->WavFileName_textBox->Name = L"WavFileName_textBox";
			this->WavFileName_textBox->Size = System::Drawing::Size(378, 20);
			this->WavFileName_textBox->TabIndex = 1;
			this->WavFileName_textBox->Text = L"C:\\Users\\leeyo\\Desktop\\FM79979\\Media\\Sound\\1.ogg";
			// 
			// ParseWav_button
			// 
			this->ParseWav_button->Location = System::Drawing::Point(915, 665);
			this->ParseWav_button->Name = L"ParseWav_button";
			this->ParseWav_button->Size = System::Drawing::Size(59, 26);
			this->ParseWav_button->TabIndex = 2;
			this->ParseWav_button->Text = L"ParseFile";
			this->ParseWav_button->UseVisualStyleBackColor = true;
			this->ParseWav_button->Click += gcnew System::EventHandler(this, &Form1::ParseWav_button_Click);
			// 
			// OPENGL_panel
			// 
			this->OPENGL_panel->Dock = System::Windows::Forms::DockStyle::Top;
			this->OPENGL_panel->Location = System::Drawing::Point(0, 0);
			this->OPENGL_panel->Name = L"OPENGL_panel";
			this->OPENGL_panel->Size = System::Drawing::Size(1884, 522);
			this->OPENGL_panel->TabIndex = 3;
			// 
			// WavInfo_listBox
			// 
			this->WavInfo_listBox->Dock = System::Windows::Forms::DockStyle::Left;
			this->WavInfo_listBox->FormattingEnabled = true;
			this->WavInfo_listBox->Location = System::Drawing::Point(0, 522);
			this->WavInfo_listBox->Name = L"WavInfo_listBox";
			this->WavInfo_listBox->Size = System::Drawing::Size(175, 339);
			this->WavInfo_listBox->TabIndex = 4;
			// 
			// CurrentTime_trackBar
			// 
			this->CurrentTime_trackBar->Location = System::Drawing::Point(527, 556);
			this->CurrentTime_trackBar->Name = L"CurrentTime_trackBar";
			this->CurrentTime_trackBar->Size = System::Drawing::Size(274, 45);
			this->CurrentTime_trackBar->TabIndex = 5;
			this->CurrentTime_trackBar->Value = 10;
			// 
			// CurrentTime_label
			// 
			this->CurrentTime_label->AutoSize = true;
			this->CurrentTime_label->Location = System::Drawing::Point(528, 529);
			this->CurrentTime_label->Name = L"CurrentTime_label";
			this->CurrentTime_label->Size = System::Drawing::Size(64, 13);
			this->CurrentTime_label->TabIndex = 6;
			this->CurrentTime_label->Text = L"CurrentTime";
			// 
			// DataCompressRate_numericUpDown
			// 
			this->DataCompressRate_numericUpDown->Location = System::Drawing::Point(196, 790);
			this->DataCompressRate_numericUpDown->Name = L"DataCompressRate_numericUpDown";
			this->DataCompressRate_numericUpDown->Size = System::Drawing::Size(46, 20);
			this->DataCompressRate_numericUpDown->TabIndex = 7;
			// 
			// DataCompressRate__label
			// 
			this->DataCompressRate__label->AutoSize = true;
			this->DataCompressRate__label->Location = System::Drawing::Point(189, 824);
			this->DataCompressRate__label->Name = L"DataCompressRate__label";
			this->DataCompressRate__label->Size = System::Drawing::Size(99, 13);
			this->DataCompressRate__label->TabIndex = 8;
			this->DataCompressRate__label->Text = L"DataCompressRate";
			// 
			// ForwardStep_button
			// 
			this->ForwardStep_button->Location = System::Drawing::Point(382, 684);
			this->ForwardStep_button->Name = L"ForwardStep_button";
			this->ForwardStep_button->Size = System::Drawing::Size(75, 23);
			this->ForwardStep_button->TabIndex = 10;
			this->ForwardStep_button->Text = L">>";
			this->ForwardStep_button->UseVisualStyleBackColor = true;
			this->ForwardStep_button->Click += gcnew System::EventHandler(this, &Form1::BackStep_button_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(292, 684);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->TabIndex = 11;
			this->button3->Text = L">";
			this->button3->UseVisualStyleBackColor = true;
			// 
			// BackStep_button
			// 
			this->BackStep_button->Location = System::Drawing::Point(196, 684);
			this->BackStep_button->Name = L"BackStep_button";
			this->BackStep_button->Size = System::Drawing::Size(75, 23);
			this->BackStep_button->TabIndex = 12;
			this->BackStep_button->Text = L"<<";
			this->BackStep_button->UseVisualStyleBackColor = true;
			this->BackStep_button->Click += gcnew System::EventHandler(this, &Form1::BackStep_button_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(297, 824);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(105, 13);
			this->label1->TabIndex = 14;
			this->label1->Text = L"ShowWaveSeconds";
			// 
			// ShowWaveSeconds_numericUpDown
			// 
			this->ShowWaveSeconds_numericUpDown->Location = System::Drawing::Point(298, 790);
			this->ShowWaveSeconds_numericUpDown->Name = L"ShowWaveSeconds_numericUpDown";
			this->ShowWaveSeconds_numericUpDown->Size = System::Drawing::Size(46, 20);
			this->ShowWaveSeconds_numericUpDown->TabIndex = 13;
			// 
			// RecordSound_button
			// 
			this->RecordSound_button->Location = System::Drawing::Point(591, 790);
			this->RecordSound_button->Name = L"RecordSound_button";
			this->RecordSound_button->Size = System::Drawing::Size(94, 34);
			this->RecordSound_button->TabIndex = 15;
			this->RecordSound_button->Text = L"RecordSound";
			this->RecordSound_button->UseVisualStyleBackColor = true;
			this->RecordSound_button->Click += gcnew System::EventHandler(this, &Form1::RecordSound_button_Click);
			// 
			// RecordSoundPause_button
			// 
			this->RecordSoundPause_button->Location = System::Drawing::Point(591, 831);
			this->RecordSoundPause_button->Name = L"RecordSoundPause_button";
			this->RecordSoundPause_button->Size = System::Drawing::Size(122, 33);
			this->RecordSoundPause_button->TabIndex = 16;
			this->RecordSoundPause_button->Text = L"RecordSoundPause";
			this->RecordSoundPause_button->UseVisualStyleBackColor = true;
			this->RecordSoundPause_button->Click += gcnew System::EventHandler(this, &Form1::RecordSoundPause_button_Click);
			// 
			// RecordSoundFileName_textBox
			// 
			this->RecordSoundFileName_textBox->Location = System::Drawing::Point(596, 764);
			this->RecordSoundFileName_textBox->Name = L"RecordSoundFileName_textBox";
			this->RecordSoundFileName_textBox->Size = System::Drawing::Size(378, 20);
			this->RecordSoundFileName_textBox->TabIndex = 17;
			this->RecordSoundFileName_textBox->Text = L"RecordSoundFileName.ogg";
			// 
			// FFTTest_button
			// 
			this->FFTTest_button->Location = System::Drawing::Point(106, 16);
			this->FFTTest_button->Name = L"FFTTest_button";
			this->FFTTest_button->Size = System::Drawing::Size(75, 23);
			this->FFTTest_button->TabIndex = 18;
			this->FFTTest_button->Text = L"FFT test";
			this->FFTTest_button->UseVisualStyleBackColor = true;
			this->FFTTest_button->Click += gcnew System::EventHandler(this, &Form1::FFTTest_button_Click);
			// 
			// ConvertToWav_button
			// 
			this->ConvertToWav_button->Location = System::Drawing::Point(915, 633);
			this->ConvertToWav_button->Name = L"ConvertToWav_button";
			this->ConvertToWav_button->Size = System::Drawing::Size(59, 26);
			this->ConvertToWav_button->TabIndex = 19;
			this->ConvertToWav_button->Text = L"ToWav";
			this->ConvertToWav_button->UseVisualStyleBackColor = true;
			this->ConvertToWav_button->Click += gcnew System::EventHandler(this, &Form1::ToOgg_button_Click);
			// 
			// ToOgg_button
			// 
			this->ToOgg_button->Location = System::Drawing::Point(915, 601);
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
			this->WaveUpdateIndex_numericUpDown->Location = System::Drawing::Point(6, 32);
			this->WaveUpdateIndex_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->WaveUpdateIndex_numericUpDown->Name = L"WaveUpdateIndex_numericUpDown";
			this->WaveUpdateIndex_numericUpDown->Size = System::Drawing::Size(94, 20);
			this->WaveUpdateIndex_numericUpDown->TabIndex = 23;
			this->WaveUpdateIndex_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {6, 0, 0, 0});
			this->WaveUpdateIndex_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::WaveUpdateIndex_numericUpDown_ValueChanged);
			// 
			// WaveUpdateIndex_label
			// 
			this->WaveUpdateIndex_label->AutoSize = true;
			this->WaveUpdateIndex_label->Location = System::Drawing::Point(3, 16);
			this->WaveUpdateIndex_label->Name = L"WaveUpdateIndex_label";
			this->WaveUpdateIndex_label->Size = System::Drawing::Size(97, 13);
			this->WaveUpdateIndex_label->TabIndex = 24;
			this->WaveUpdateIndex_label->Text = L"WaveUpdateIndex";
			// 
			// FFTTestPause_checkBox
			// 
			this->FFTTestPause_checkBox->AutoSize = true;
			this->FFTTestPause_checkBox->Location = System::Drawing::Point(197, 16);
			this->FFTTestPause_checkBox->Name = L"FFTTestPause_checkBox";
			this->FFTTestPause_checkBox->Size = System::Drawing::Size(96, 17);
			this->FFTTestPause_checkBox->TabIndex = 25;
			this->FFTTestPause_checkBox->Text = L"FFTTestPause";
			this->FFTTestPause_checkBox->UseVisualStyleBackColor = true;
			this->FFTTestPause_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::FFTTestPause_checkBox_CheckedChanged);
			// 
			// WavToOggOnlyOneChannel
			// 
			this->WavToOggOnlyOneChannel->Location = System::Drawing::Point(915, 578);
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
			this->GoToFFTTestTime_label->Location = System::Drawing::Point(3, 62);
			this->GoToFFTTestTime_label->Name = L"GoToFFTTestTime_label";
			this->GoToFFTTestTime_label->Size = System::Drawing::Size(97, 13);
			this->GoToFFTTestTime_label->TabIndex = 28;
			this->GoToFFTTestTime_label->Text = L"GoToFFTTestTime";
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->FFTDataCountScale_label);
			this->panel1->Controls->Add(this->FFTDataCountScale_numericUpDown);
			this->panel1->Controls->Add(this->GoToTime_trackBar);
			this->panel1->Controls->Add(this->GoToFFTTestTime_label);
			this->panel1->Controls->Add(this->WaveUpdateIndex_label);
			this->panel1->Controls->Add(this->FFTTest_button);
			this->panel1->Controls->Add(this->WaveUpdateIndex_numericUpDown);
			this->panel1->Controls->Add(this->FFTTestPause_checkBox);
			this->panel1->Location = System::Drawing::Point(190, 529);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(305, 128);
			this->panel1->TabIndex = 0;
			// 
			// FFTDataCountScale_label
			// 
			this->FFTDataCountScale_label->AutoSize = true;
			this->FFTDataCountScale_label->Location = System::Drawing::Point(108, 42);
			this->FFTDataCountScale_label->Name = L"FFTDataCountScale_label";
			this->FFTDataCountScale_label->Size = System::Drawing::Size(104, 13);
			this->FFTDataCountScale_label->TabIndex = 28;
			this->FFTDataCountScale_label->Text = L"FFTDataCountScale";
			// 
			// FFTDataCountScale_numericUpDown
			// 
			this->FFTDataCountScale_numericUpDown->DecimalPlaces = 1;
			this->FFTDataCountScale_numericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->FFTDataCountScale_numericUpDown->Location = System::Drawing::Point(111, 58);
			this->FFTDataCountScale_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->FFTDataCountScale_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->FFTDataCountScale_numericUpDown->Name = L"FFTDataCountScale_numericUpDown";
			this->FFTDataCountScale_numericUpDown->Size = System::Drawing::Size(120, 20);
			this->FFTDataCountScale_numericUpDown->TabIndex = 27;
			this->FFTDataCountScale_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->FFTDataCountScale_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::FFTDataCountScale_numericUpDown_ValueChanged);
			// 
			// GoToTime_trackBar
			// 
			this->GoToTime_trackBar->Location = System::Drawing::Point(6, 80);
			this->GoToTime_trackBar->Name = L"GoToTime_trackBar";
			this->GoToTime_trackBar->Size = System::Drawing::Size(274, 45);
			this->GoToTime_trackBar->TabIndex = 27;
			this->GoToTime_trackBar->Value = 10;
			this->GoToTime_trackBar->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::GoToTime_trackBar_MouseUp);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1884, 861);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->WavToOggOnlyOneChannel);
			this->Controls->Add(this->PlayWav_button);
			this->Controls->Add(this->PlayOgg_button);
			this->Controls->Add(this->ToOgg_button);
			this->Controls->Add(this->ConvertToWav_button);
			this->Controls->Add(this->RecordSoundFileName_textBox);
			this->Controls->Add(this->RecordSoundPause_button);
			this->Controls->Add(this->RecordSound_button);
			this->Controls->Add(this->OpenWavFile_button);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->ShowWaveSeconds_numericUpDown);
			this->Controls->Add(this->BackStep_button);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->ForwardStep_button);
			this->Controls->Add(this->DataCompressRate__label);
			this->Controls->Add(this->DataCompressRate_numericUpDown);
			this->Controls->Add(this->CurrentTime_label);
			this->Controls->Add(this->CurrentTime_trackBar);
			this->Controls->Add(this->WavInfo_listBox);
			this->Controls->Add(this->OPENGL_panel);
			this->Controls->Add(this->ParseWav_button);
			this->Controls->Add(this->WavFileName_textBox);
			this->Name = L"Form1";
			this->Text = L" ";
			this->Resize += gcnew System::EventHandler(this, &Form1::Form1_Resize);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CurrentTime_trackBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->DataCompressRate_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ShowWaveSeconds_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->WaveUpdateIndex_numericUpDown))->EndInit();
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->FFTDataCountScale_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GoToTime_trackBar))->EndInit();
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
				if( m_pSoundFFTCapture )
				{
					m_pSoundFFTCapture->Update(l_fElpaseTime);
					m_pSoundFFTCapture->Render();
				}
			}
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
private: System::Void ParseWav_button_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			this->timer1->Enabled = false;
			if(this->WavFileName_textBox->Text && m_pGameApp )
			{
				if( !System::IO::File::Exists(this->WavFileName_textBox->Text) )
				{
					String^l_strFileName = DNCT::OpenFileAndGetName();
					if( l_strFileName )
						this->WavFileName_textBox->Text = l_strFileName;
				}
				//if(this->m_pGameApp->m_spSoundParser->AddSound(DNCT::GcStringToChar(this->WavFileName_textBox->Text)))
				{
					std::wstring l_FileName = DNCT::GcStringToWchar(this->WavFileName_textBox->Text);
					//this->m_pGameApp->SoundPlay(l_FileName.c_str(),true);
					SAFE_DELETE(this->m_pWaveInfo);
					this->m_pWaveInfo = new FATMING_CORE::cSoundFile();
					this->WavInfo_listBox->Items->Clear();
					if(this->m_pWaveInfo->OpenFile(UT::WcharToChar(l_FileName).c_str()))
					{
						SAFE_DELETE(m_pWavWaves);
						m_pWavWaves = new cWavWaves();
						m_pWavWaves->SetData(this->m_pWaveInfo);
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
//						l_str = "chunk id "+ gcnew String((const char*)this->m_pWaveInfo->m_FMT_And_Data_Header[0].Id);
	//					this->WavInfo_listBox->Items->Add(l_str);
		//				l_str = "SizeSize "+this->m_pWaveInfo->m_FMT_And_Data_Header[0].Size;
			//			this->WavInfo_listBox->Items->Add(l_str);
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
						//l_str = "chunk id "+ gcnew String((const char*)this->m_pWaveInfo->m_FMT_And_Data_Header[1].Id);
						//this->WavInfo_listBox->Items->Add(l_str);
						//l_str = "SizeSize "+this->m_pWaveInfo->m_FMT_And_Data_Header[1].Size;
						//this->WavInfo_listBox->Items->Add(l_str);
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
			this->timer1->Enabled = true;
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
				 std::string l_strFileName = DNCT::GcStringToChar(WavFileName_textBox->Text);
				 if(m_pKissFFTConvertBase->FetchSoundDataStart(l_strFileName.c_str()))
				 {
					 CurrentTime_trackBar->Maximum = (int)(m_pKissFFTConvertBase->GetTimeLength()*10);
					 GoToTime_trackBar->Maximum = CurrentTime_trackBar->Maximum;
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
};
}