#pragma once
#include "GlyphWriter.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace DrawFont {

	/// <summary>
	/// Summary for ShowFontPic
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ShowFontPic : public System::Windows::Forms::Form
	{
		ref class cFontInfo
		{
		public:
			ref class cGlyph :public System::Object
			{
			public:
				cGlyph( FILE_GLYPH_ATTR e_FILE_GLYPH_ATTR )
				{
					fLeft = e_FILE_GLYPH_ATTR.fLeft;
					fTop = e_FILE_GLYPH_ATTR.fTop;
					fRight = e_FILE_GLYPH_ATTR.fRight;
					fBottom = e_FILE_GLYPH_ATTR.fBottom;
					fWidth = e_FILE_GLYPH_ATTR.fWidth;
					fHeight = e_FILE_GLYPH_ATTR.fHeight;
				}
				~cGlyph(){}
				float  fLeft, fTop, fRight, fBottom;
				float  fWidth,fHeight;
			};
			System::Drawing::Font^	m_pFont;
			System::Drawing::Bitmap^m_pBitMap;
			System::String^	m_psCharacterInfo;//size pos
			System::Collections::ArrayList^	m_GlypgAttriList;
			int	m_iSpaceDis;
			float	m_fSize;//current don't work
			bool	m_bBold;
			bool	m_bItalic;
			bool	m_bStrikeOut;
			bool	m_bUnderline;
			//bool	m_bSquence;//not support because lazy to write
			System::String^m_psFontName;
			System::Drawing::FontFamily^m_pFontFamily;
			cFontInfo(System::Drawing::Bitmap^e_pBitmap,
				System::Drawing::Font^e_pFont,int e_iDis)
			{
				m_pFontFamily = e_pFont->FontFamily;
				m_bBold = e_pFont->Bold;
				m_bItalic = e_pFont->Italic;
				m_psFontName = e_pFont->Name;
				m_bStrikeOut = e_pFont->Strikeout;
				m_bUnderline = e_pFont->Underline;
				m_pFont = e_pFont;	m_pBitMap = e_pBitmap;
				m_iSpaceDis = e_iDis;	m_fSize = e_pFont->Size;
				m_psCharacterInfo = gcnew System::String("");
				m_GlypgAttriList = gcnew System::Collections::ArrayList();
			}
			~cFontInfo(){}
		};
	public:
		ShowFontPic(System::String^ e_pStringForOutPut)
		{
			InitializeComponent();
			m_pStringForOutput = TO_GCSTRING(e_pStringForOutPut);
			m_pFontInfoList = gcnew System::Collections::Hashtable;
			g_MaskColor = System::Drawing::Color::Black;
			g_FontColor = System::Drawing::Color::White;
			PicFormat_comboBox->SelectedIndex = 0;
			AntiAlias_comboBox->SelectedIndex = 2;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ShowFontPic()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureBox1;

	private: System::Windows::Forms::NumericUpDown^  FontDis_numericUpDown;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  FontSize_numericUpDown;

	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;

	private: System::Windows::Forms::Button^  ReFreshPic_button;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::NumericUpDown^  TextureSizeY_numericUpDown;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::NumericUpDown^  TextureSizeX_numericUpDown;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::CheckBox^  SequenceCharacter_checkBox;
	private: System::Windows::Forms::ToolStripMenuItem^  savePicToolStripMenuItem;
	public: System::Windows::Forms::ListBox^  AllFont_listBox;
	private: 

	private: System::Windows::Forms::Button^  DelPic_button;



	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  Color_button;

	private: System::Windows::Forms::ToolStripMenuItem^  checkOutputFileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  winToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  xboxToolStripMenuItem;
	private: System::Windows::Forms::ComboBox^  PicFormat_comboBox;
	private: System::Windows::Forms::Button^  FontColor_button;
private: System::Windows::Forms::CheckBox^  Outline_checkBox;
private: System::Windows::Forms::ComboBox^  AntiAlias_comboBox;
private: System::Windows::Forms::Label^  label6;




	private: System::Windows::Forms::ToolStripMenuItem^  openFileToolStripMenuItem;

//================================================
	
	private: System::Drawing::Bitmap^GenerateBitMapByString(
				 String^e_pString,
				 System::Drawing::Font^e_pFont,
				 int e_iDistance,
				 int e_iWidth,
				 int e_iHeight,
				 bool e_bSequence,std::vector<FILE_GLYPH_ATTR>*e_pGlyphList);//e_bSequence should be true
	private: System::String^	m_pStringForOutput;
	private: System::Collections::Hashtable^	m_pFontInfoList;
	private: System::Void	SaveFontFile();
	private: System::Void	OpenFontFile();
	private: System::String	^m_pForFontPosInfoTemp;//get all characters pos and size and give to desc
	private: System::Drawing::Color g_MaskColor;//background color
	private: System::Drawing::Color g_FontColor;//font color
	System::Drawing::Font^GetFontByName(System::String^e_pString){
		cFontInfo^l_p = safe_cast<cFontInfo^>(m_pFontInfoList[e_pString]);
		return l_p->m_pFont;}

	private: System::Void SaveBitInfoAndPic();
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->FontDis_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->FontSize_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->savePicToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->checkOutputFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->winToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->xboxToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ReFreshPic_button = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->TextureSizeY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->TextureSizeX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->SequenceCharacter_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->AllFont_listBox = (gcnew System::Windows::Forms::ListBox());
			this->DelPic_button = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->Color_button = (gcnew System::Windows::Forms::Button());
			this->PicFormat_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->FontColor_button = (gcnew System::Windows::Forms::Button());
			this->Outline_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->AntiAlias_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->FontDis_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->FontSize_numericUpDown))->BeginInit();
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TextureSizeY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TextureSizeX_numericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(0, 151);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(33, 25);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// FontDis_numericUpDown
			// 
			this->FontDis_numericUpDown->Location = System::Drawing::Point(91, 83);
			this->FontDis_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100, 0, 0, System::Int32::MinValue });
			this->FontDis_numericUpDown->Name = L"FontDis_numericUpDown";
			this->FontDis_numericUpDown->Size = System::Drawing::Size(83, 22);
			this->FontDis_numericUpDown->TabIndex = 2;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 90);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(68, 12);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Font Distance";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 114);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(48, 12);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Font Size";
			// 
			// FontSize_numericUpDown
			// 
			this->FontSize_numericUpDown->Location = System::Drawing::Point(91, 108);
			this->FontSize_numericUpDown->Name = L"FontSize_numericUpDown";
			this->FontSize_numericUpDown->Size = System::Drawing::Size(83, 22);
			this->FontSize_numericUpDown->TabIndex = 4;
			this->FontSize_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &ShowFontPic::FontSize_numericUpDown_ValueChanged);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->fileToolStripMenuItem,
					this->checkOutputFileToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(467, 24);
			this->menuStrip1->TabIndex = 7;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->openFileToolStripMenuItem,
					this->saveToolStripMenuItem, this->savePicToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(39, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openFileToolStripMenuItem
			// 
			this->openFileToolStripMenuItem->Name = L"openFileToolStripMenuItem";
			this->openFileToolStripMenuItem->Size = System::Drawing::Size(127, 22);
			this->openFileToolStripMenuItem->Text = L"OpenFile";
			this->openFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &ShowFontPic::openFileToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(127, 22);
			this->saveToolStripMenuItem->Text = L"SaveFile";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &ShowFontPic::openFileToolStripMenuItem_Click);
			// 
			// savePicToolStripMenuItem
			// 
			this->savePicToolStripMenuItem->Name = L"savePicToolStripMenuItem";
			this->savePicToolStripMenuItem->Size = System::Drawing::Size(127, 22);
			this->savePicToolStripMenuItem->Text = L"SavePic";
			this->savePicToolStripMenuItem->Click += gcnew System::EventHandler(this, &ShowFontPic::saveToolStripMenuItem_Click);
			// 
			// checkOutputFileToolStripMenuItem
			// 
			this->checkOutputFileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->winToolStripMenuItem,
					this->xboxToolStripMenuItem
			});
			this->checkOutputFileToolStripMenuItem->Name = L"checkOutputFileToolStripMenuItem";
			this->checkOutputFileToolStripMenuItem->Size = System::Drawing::Size(113, 20);
			this->checkOutputFileToolStripMenuItem->Text = L"CheckOutputFile";
			// 
			// winToolStripMenuItem
			// 
			this->winToolStripMenuItem->Name = L"winToolStripMenuItem";
			this->winToolStripMenuItem->Size = System::Drawing::Size(106, 22);
			this->winToolStripMenuItem->Text = L"Win";
			this->winToolStripMenuItem->Click += gcnew System::EventHandler(this, &ShowFontPic::winToolStripMenuItem_Click);
			// 
			// xboxToolStripMenuItem
			// 
			this->xboxToolStripMenuItem->Name = L"xboxToolStripMenuItem";
			this->xboxToolStripMenuItem->Size = System::Drawing::Size(106, 22);
			this->xboxToolStripMenuItem->Text = L"Xbox";
			this->xboxToolStripMenuItem->Click += gcnew System::EventHandler(this, &ShowFontPic::winToolStripMenuItem_Click);
			// 
			// ReFreshPic_button
			// 
			this->ReFreshPic_button->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->ReFreshPic_button->Enabled = false;
			this->ReFreshPic_button->Location = System::Drawing::Point(178, 108);
			this->ReFreshPic_button->Name = L"ReFreshPic_button";
			this->ReFreshPic_button->Size = System::Drawing::Size(78, 19);
			this->ReFreshPic_button->TabIndex = 8;
			this->ReFreshPic_button->Text = L"ReFreshPic";
			this->ReFreshPic_button->UseVisualStyleBackColor = true;
			this->ReFreshPic_button->Click += gcnew System::EventHandler(this, &ShowFontPic::ReFreshPic_button_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(12, 57);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(68, 12);
			this->label3->TabIndex = 12;
			this->label3->Text = L"TextureSizeY";
			// 
			// TextureSizeY_numericUpDown
			// 
			this->TextureSizeY_numericUpDown->Location = System::Drawing::Point(91, 51);
			this->TextureSizeY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10000000, 0, 0, 0 });
			this->TextureSizeY_numericUpDown->Name = L"TextureSizeY_numericUpDown";
			this->TextureSizeY_numericUpDown->Size = System::Drawing::Size(83, 22);
			this->TextureSizeY_numericUpDown->TabIndex = 11;
			this->TextureSizeY_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 600, 0, 0, 0 });
			this->TextureSizeY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &ShowFontPic::TextureSizeX_numericUpDown_ValueChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(12, 32);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(68, 12);
			this->label4->TabIndex = 10;
			this->label4->Text = L"TextureSizeX";
			// 
			// TextureSizeX_numericUpDown
			// 
			this->TextureSizeX_numericUpDown->Location = System::Drawing::Point(91, 26);
			this->TextureSizeX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10000000, 0, 0, 0 });
			this->TextureSizeX_numericUpDown->Name = L"TextureSizeX_numericUpDown";
			this->TextureSizeX_numericUpDown->Size = System::Drawing::Size(83, 22);
			this->TextureSizeX_numericUpDown->TabIndex = 9;
			this->TextureSizeX_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 800, 0, 0, 0 });
			this->TextureSizeX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &ShowFontPic::TextureSizeX_numericUpDown_ValueChanged);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(181, 32);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(0, 12);
			this->label5->TabIndex = 13;
			// 
			// SequenceCharacter_checkBox
			// 
			this->SequenceCharacter_checkBox->AutoSize = true;
			this->SequenceCharacter_checkBox->Checked = true;
			this->SequenceCharacter_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->SequenceCharacter_checkBox->Location = System::Drawing::Point(12, 130);
			this->SequenceCharacter_checkBox->Name = L"SequenceCharacter_checkBox";
			this->SequenceCharacter_checkBox->Size = System::Drawing::Size(113, 16);
			this->SequenceCharacter_checkBox->TabIndex = 14;
			this->SequenceCharacter_checkBox->Text = L"SequenceCharacter";
			this->SequenceCharacter_checkBox->UseVisualStyleBackColor = true;
			// 
			// AllFont_listBox
			// 
			this->AllFont_listBox->FormattingEnabled = true;
			this->AllFont_listBox->ItemHeight = 12;
			this->AllFont_listBox->Location = System::Drawing::Point(309, 70);
			this->AllFont_listBox->Name = L"AllFont_listBox";
			this->AllFont_listBox->Size = System::Drawing::Size(152, 76);
			this->AllFont_listBox->TabIndex = 15;
			this->AllFont_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ShowFontPic::AllFont_listBox_SelectedIndexChanged);
			// 
			// DelPic_button
			// 
			this->DelPic_button->Enabled = false;
			this->DelPic_button->Location = System::Drawing::Point(373, 46);
			this->DelPic_button->Name = L"DelPic_button";
			this->DelPic_button->Size = System::Drawing::Size(88, 18);
			this->DelPic_button->TabIndex = 16;
			this->DelPic_button->Text = L"Del";
			this->DelPic_button->UseVisualStyleBackColor = true;
			this->DelPic_button->Click += gcnew System::EventHandler(this, &ShowFontPic::DelPic_button_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(373, 24);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(87, 18);
			this->button2->TabIndex = 17;
			this->button2->Text = L"AddFontToList";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &ShowFontPic::fontAddToolStripMenuItem_Click);
			// 
			// Color_button
			// 
			this->Color_button->Location = System::Drawing::Point(177, 80);
			this->Color_button->Name = L"Color_button";
			this->Color_button->Size = System::Drawing::Size(63, 23);
			this->Color_button->TabIndex = 18;
			this->Color_button->Text = L"BK Color";
			this->Color_button->UseVisualStyleBackColor = true;
			this->Color_button->Click += gcnew System::EventHandler(this, &ShowFontPic::Color_button_Click);
			// 
			// PicFormat_comboBox
			// 
			this->PicFormat_comboBox->FormattingEnabled = true;
			this->PicFormat_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(5) { L"bmp", L"gif", L"jpeg", L"png", L"tiff" });
			this->PicFormat_comboBox->Location = System::Drawing::Point(296, 25);
			this->PicFormat_comboBox->Name = L"PicFormat_comboBox";
			this->PicFormat_comboBox->Size = System::Drawing::Size(73, 20);
			this->PicFormat_comboBox->TabIndex = 20;
			// 
			// FontColor_button
			// 
			this->FontColor_button->Location = System::Drawing::Point(242, 79);
			this->FontColor_button->Name = L"FontColor_button";
			this->FontColor_button->Size = System::Drawing::Size(66, 25);
			this->FontColor_button->TabIndex = 21;
			this->FontColor_button->Text = L"FontColor";
			this->FontColor_button->UseVisualStyleBackColor = true;
			this->FontColor_button->Click += gcnew System::EventHandler(this, &ShowFontPic::Color_button_Click);
			// 
			// Outline_checkBox
			// 
			this->Outline_checkBox->AutoSize = true;
			this->Outline_checkBox->Location = System::Drawing::Point(198, 8);
			this->Outline_checkBox->Name = L"Outline_checkBox";
			this->Outline_checkBox->Size = System::Drawing::Size(58, 16);
			this->Outline_checkBox->TabIndex = 22;
			this->Outline_checkBox->Text = L"Outline";
			this->Outline_checkBox->UseVisualStyleBackColor = true;
			// 
			// AntiAlias_comboBox
			// 
			this->AntiAlias_comboBox->FormattingEnabled = true;
			this->AntiAlias_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(6) {
				L"AntiAlias", L"AntiAliasGridFit", L"ClearTypeGridFit",
					L"SingleBitPerPixel", L"SingleBitPerPixelGridFit", L"SystemDefault"
			});
			this->AntiAlias_comboBox->Location = System::Drawing::Point(182, 130);
			this->AntiAlias_comboBox->Name = L"AntiAlias_comboBox";
			this->AntiAlias_comboBox->Size = System::Drawing::Size(121, 20);
			this->AntiAlias_comboBox->TabIndex = 23;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(128, 134);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(48, 12);
			this->label6->TabIndex = 24;
			this->label6->Text = L"AntiAlias";
			// 
			// ShowFontPic
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoScroll = true;
			this->ClientSize = System::Drawing::Size(467, 272);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->AntiAlias_comboBox);
			this->Controls->Add(this->Outline_checkBox);
			this->Controls->Add(this->FontColor_button);
			this->Controls->Add(this->PicFormat_comboBox);
			this->Controls->Add(this->Color_button);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->DelPic_button);
			this->Controls->Add(this->AllFont_listBox);
			this->Controls->Add(this->SequenceCharacter_checkBox);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->TextureSizeY_numericUpDown);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->TextureSizeX_numericUpDown);
			this->Controls->Add(this->ReFreshPic_button);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->FontSize_numericUpDown);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->FontDis_numericUpDown);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"ShowFontPic";
			this->Text = L"ShowFontPic";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->FontDis_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->FontSize_numericUpDown))->EndInit();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TextureSizeY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TextureSizeX_numericUpDown))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: void ChangePic()
			 {
				 System::String^l_pString = AllFont_listBox->SelectedItem->ToString();
				 cFontInfo^l_p = safe_cast<cFontInfo^>(m_pFontInfoList[l_pString]);
				 pictureBox1->Image = static_cast<System::Drawing::Bitmap^>(l_p->m_pBitMap);
				 pictureBox1->Size = l_p->m_pBitMap->Size;			 
				 TextureSizeX_numericUpDown->Value = l_p->m_pBitMap->Width;
				 TextureSizeY_numericUpDown->Value = l_p->m_pBitMap->Height;
			 }
	private: void ReFreshPic()
			 {
				 //std::vector<wchar_t> GetNotLegalCharacterArrayFromFont(System::Drawing::Font^e_pFont,String^e_strText)
				 System::String^l_pString = AllFont_listBox->SelectedItem->ToString();
				 cFontInfo^l_p = safe_cast<cFontInfo^>(m_pFontInfoList[l_pString]);
				 l_p->m_iSpaceDis = (int)FontDis_numericUpDown->Value;

				 std::vector<FILE_GLYPH_ATTR> l_GlyphList;
				 System::Drawing::Bitmap^l_pBitMap = GenerateBitMapByString(m_pStringForOutput,
					 GetFontByName( l_pString ),
					 (int)FontDis_numericUpDown->Value,
					 (int)TextureSizeX_numericUpDown->Value,
					 (int)TextureSizeY_numericUpDown->Value,
					 SequenceCharacter_checkBox->Checked,&l_GlyphList);
				 //get characters pos info

				 l_p->m_GlypgAttriList->Clear();
				 for( UINT i=0;i<l_GlyphList.size();i++ )
				 {
					 cFontInfo::cGlyph^l_pGlyph = gcnew cFontInfo::cGlyph(l_GlyphList[i]);
					 l_p->m_GlypgAttriList->Add(l_pGlyph);
				 }
				 l_p->m_psCharacterInfo = m_pForFontPosInfoTemp;
				 l_p->m_pBitMap = l_pBitMap;
				 pictureBox1->Image = static_cast<System::Drawing::Bitmap^>(l_pBitMap);
				 pictureBox1->Size = l_pBitMap->Size;
			 }
	private: System::Void saveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 SaveBitInfoAndPic();
			 }

private: System::Void fontAddToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 System::Drawing::Font^l_pFont = DNCT::GetFontFromFontDialog();
			 if( !l_pFont )
				 return;

			 System::String^l_pStringForFontName = l_pFont->Name;
			 l_pStringForFontName+=l_pFont->Size.ToString();

			 if(l_pFont->Bold)
				 l_pStringForFontName+="B";

			 if(l_pFont->Italic)
				l_pStringForFontName +="I" ;

			 if(l_pFont->Underline)
				l_pStringForFontName +="U" ;

			 if(l_pFont->Strikeout)
				 l_pStringForFontName +="S" ;
			 System::String^l_pStringForNoSapce = gcnew String("");
			 for( int i=0;i<l_pStringForFontName->Length;i++ )
			 {
				 if( l_pStringForFontName[i]==L' ' )
					l_pStringForNoSapce+="_";
				 else
					 l_pStringForNoSapce+=l_pStringForFontName[i];
			 }
			 l_pStringForFontName = l_pStringForNoSapce;
			 if( !m_pFontInfoList->ContainsKey(l_pStringForFontName) )
			 {
				std::vector<FILE_GLYPH_ATTR>	l_GlypgAttriList;
				System::Drawing::Bitmap^l_pBitMap = GenerateBitMapByString(m_pStringForOutput,
					 l_pFont,
					 (int)FontDis_numericUpDown->Value,
					 (int)TextureSizeX_numericUpDown->Value,
					 (int)TextureSizeY_numericUpDown->Value,
					 SequenceCharacter_checkBox->Checked,&l_GlypgAttriList);
				cFontInfo^l_pFontInfo = gcnew cFontInfo(l_pBitMap,l_pFont,(int)FontDis_numericUpDown->Value);
				l_pFontInfo->m_psCharacterInfo = m_pForFontPosInfoTemp;
				for( UINT i=0;i<l_GlypgAttriList.size();i++ )
				{
					 cFontInfo::cGlyph^l_pGlyph = gcnew cFontInfo::cGlyph(l_GlypgAttriList[i]);
					 l_pFontInfo->m_GlypgAttriList->Add(l_pGlyph);
				}
				m_pFontInfoList->Add(l_pStringForFontName,l_pFontInfo);
				AllFont_listBox->Items->Add(l_pStringForFontName);
				AllFont_listBox->SelectedIndex = AllFont_listBox->Items->Count-1;
			 }
			 else
				 WARNING_MSG(" already exist ");
		 }
private: System::Void FontSize_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 GCFORM::NumericUpDown^l_pNumDown = (GCFORM::NumericUpDown^)sender;
//			 m_pFontCollection->Height = l_pNumDown->Value;
		 }
private: System::Void ReFreshPic_button_Click(System::Object^  sender, System::EventArgs^  e) {
				 ReFreshPic();
		 }

private: System::String^	StringAddSapce(System::String^e_pString,int e_iSpace)
		 {
			//System::String^ l_pString = gcnew System::String(e_pString);
			//assert(e_iSpace&&"space must bigger than 0");
			int	l_iStringLength = e_pString->Length;
			//one for end one for current character
			wchar_t *l_pTempString = new wchar_t[e_iSpace+1];
			for(  int j=0;j<e_iSpace;j++)
				l_pTempString[j] = L' ';
			l_pTempString[e_iSpace] = L'\0';
			std::wstring	l_StringForStore;
			//-1 for last character don't need sapce
			for(int i=0;i<l_iStringLength-1;++i)
			{
				wchar_t l_tempwchar[2] = { e_pString[i] ,L'\0'};
				l_StringForStore.append(l_tempwchar);
				l_StringForStore.append(l_pTempString);
			}
			delete l_pTempString;
			return gcnew System::String(l_StringForStore.c_str());
		 }
private: System::Drawing::Size	GetSizeFromString(System::String^e_pString)
		 {
				System::Drawing::Graphics^ l_pGrForMeasuring = System::Drawing::Graphics::FromImage(gcnew System::Drawing::Bitmap(8000,6000));
				System::Drawing::SizeF	l_BmpSizeF = l_pGrForMeasuring->MeasureString(e_pString,GetFontByName(AllFont_listBox->SelectedItem->ToString()));
				return System::Drawing::Size((int)l_BmpSizeF.Width,(int)l_BmpSizeF.Height);			
		 }
private: System::Void TextureSizeX_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e) {			 
			 if( AllFont_listBox->SelectedIndex == -1 )
				 return;
			 System::String^l_pString = AllFont_listBox->SelectedItem->ToString();
			 cFontInfo^l_p = safe_cast<cFontInfo^>(m_pFontInfoList[l_pString]);
//			 l_p->m_pBitMap->Width =  (int)TextureSizeX_numericUpDown->Value;
//			 l_p->m_pBitMap->Height = (int)TextureSizeY_numericUpDown->Value);
			 this->pictureBox1->Size = System::Drawing::Size((int)TextureSizeX_numericUpDown->Value,(int)TextureSizeY_numericUpDown->Value);

		 }
private: System::Void AllFont_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {			 
			 if( AllFont_listBox->SelectedIndex==-1 )
			 {
				ReFreshPic_button->Enabled = false;
				DelPic_button->Enabled = false;
				return;
			 }
			 ReFreshPic_button->Enabled = true;
			 DelPic_button->Enabled = true;
			 ChangePic();			 
		 }
private: System::Void DelPic_button_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->m_pFontInfoList->Remove(AllFont_listBox->SelectedItem->ToString());
			 AllFont_listBox->Items->RemoveAt(AllFont_listBox->SelectedIndex);			 
		 }
private: System::Void openFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 if( sender == saveToolStripMenuItem )
			 {
				 SaveFontFile();				 
			 }
			 else
			 {
				OpenFontFile();
			 }
		 }
private: System::Void Color_button_Click(System::Object^  sender, System::EventArgs^  e) {
			 if( FontColor_button == sender )
			 {
				 GCNEW(GCFORM::ColorDialog,l_pColorDialog);
				 DIALOG_OK(l_pColorDialog)
					 g_FontColor = l_pColorDialog->Color;			 
			 }
			 else
			 {
				 GCNEW(GCFORM::ColorDialog,l_pColorDialog);
				 DIALOG_OK(l_pColorDialog)
					 g_MaskColor = l_pColorDialog->Color;
			 }
		 }
private: System::Void winToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 String^l_pString = DNCT::OpenFileAndGetName();
			 if( !l_pString->Length )
				 return;
			 WCHAR	l_temp[MAX_PATH];
			 GCSTRING_TO_WCHAR(l_pString,l_temp);
			 if( sender == xboxToolStripMenuItem )
			 {


			 }
			 else
			 {
				GlyphReader	l_GlyphReader;				
				l_GlyphReader.LoadFontDataFile(l_temp);
				System::String^l_pString = gcnew System::String("");
				for( DWORD i=0;i<l_GlyphReader.m_dwNumGlyphs;i++ )
				{
					l_pString+=l_GlyphReader.m_pGlyphs[i].fLeft.ToString();l_pString+=",";
					l_pString+=l_GlyphReader.m_pGlyphs[i].fTop.ToString();l_pString+=",";
					l_pString+=l_GlyphReader.m_pGlyphs[i].fRight.ToString();l_pString+=",";
					l_pString+=l_GlyphReader.m_pGlyphs[i].fBottom.ToString();l_pString+=",";
					l_pString+=l_GlyphReader.m_pGlyphs[i].fWidth.ToString();
					l_pString+=DNCT::GetChanglineString();
				}
				WARNING_MSG("Output Binary To Text");
				DNCT::StreamWriteFileFromString(l_pString);
			  }
		 }
};

}
