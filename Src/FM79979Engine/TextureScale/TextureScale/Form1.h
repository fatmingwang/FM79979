#pragma once
#include "../../DotNetCommon/DotNetCommonTools.h"

#include "DDSConvert.h"

namespace TextureScale 
{
	//-----------------------------------------------------------------------------
	// Global variables
	//-----------------------------------------------------------------------------
	LPDIRECT3D9         g_pD3D = NULL; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9   g_pd3dDevice = NULL; // Our rendering device

	int	ToMaxMipMap(int e_iWidth,int e_iHeight);
	LPDIRECT3DBASETEXTURE9 ImageResize(DWORD dwWidthNew, DWORD dwHeightNew,LPDIRECT3DBASETEXTURE9 pSrctex,DWORD e_dwMipMap,bool e_bIsVolumeMap,bool e_bIsCueMap);
	D3DFORMAT GetFormat(LPDIRECT3DBASETEXTURE9 ptex,bool e_bIsVolumeMap,bool e_bIsCueMap);
	HRESULT BltAllLevels(D3DCUBEMAP_FACES FaceType,LPDIRECT3DBASETEXTURE9 ptexSrc, LPDIRECT3DBASETEXTURE9 ptexDest,DWORD e_dwNumMips,bool e_bIsVolumeMap,bool e_bIsCueMap);
	HRESULT InitD3D( HWND hWnd );
	VOID Cleanup();
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Form1 的摘要
	///
	/// 警告: 如果您變更這個類別的名稱，就必須變更與這個類別所依據之所有 .resx 檔案關聯的
	///          Managed 資源編譯器工具的 'Resource File Name' 屬性。
	///          否則，這些設計工具
	///          將無法與這個表單關聯的當地語系化資源
	///          正確互動。
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			DDSFormat_comboBox->Visible = false;
			//
			//TODO: 在此加入建構函式程式碼
			//
			InitD3D((HWND)panel1->Handle.ToPointer());
			Format_comboBox->SelectedIndex = 4;
			DDSFormat_comboBox->SelectedIndex = 4;
			TransformType_comboBox->SelectedIndex = 0;
			m_pCDDSConvert = new CDDSConvert(g_pd3dDevice);;
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
			Cleanup();
		}
	private: System::Windows::Forms::ListBox^  AllFile_listBox;
	private: System::Windows::Forms::Button^  AddFile_button;
	private: System::Windows::Forms::Button^  DeleteFile_button;
	private: System::Windows::Forms::Button^  Export_button;
	private: System::Windows::Forms::NumericUpDown^  Width_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  Height_numericUpDown;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  RecursivelyAddFile_button;
	private: System::Windows::Forms::ComboBox^  Format_comboBox;
	private: System::Windows::Forms::ComboBox^  TransformType_comboBox;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::CheckBox^  ReplaceOriginalFile_checkBox;
	private: System::Windows::Forms::CheckBox^  CreateFolderIfNotReplaceOriginalFile_checkBox;
//my
	String^m_strDelectedDirectory;
	private: System::Windows::Forms::Label^  Progress_label;
	private: System::Windows::Forms::TextBox^  ErrorMsg_textBox;
	private: System::Windows::Forms::ComboBox^  DDSFormat_comboBox;
	CDDSConvert*	m_pCDDSConvert;
	private: System::Windows::Forms::CheckBox^  ScaleWithDirectX_checkBox;
	void	ScaleImageWithDirectX(String^e_strDirectoryName,String^e_strFileName,D3DXIMAGE_FILEFORMAT e_D3DXIMAGE_FILEFORMAT,String^e_strExtensionName);
	void	ScaleImageWithDotNet(String^e_strDirectoryName,String^e_strFileName,String^e_strExtensionName);
	String^	NewDirectoryCheck(String^e_strDirectoryName,String^e_strFileName,String^e_strDelectedDirectory);
	protected:
	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改這個方法的內容。
		///
		/// </summary>
		void InitializeComponent(void)
		{
			this->AllFile_listBox = (gcnew System::Windows::Forms::ListBox());
			this->AddFile_button = (gcnew System::Windows::Forms::Button());
			this->DeleteFile_button = (gcnew System::Windows::Forms::Button());
			this->Export_button = (gcnew System::Windows::Forms::Button());
			this->Width_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->Height_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->RecursivelyAddFile_button = (gcnew System::Windows::Forms::Button());
			this->Format_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->TransformType_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->ReplaceOriginalFile_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->CreateFolderIfNotReplaceOriginalFile_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->Progress_label = (gcnew System::Windows::Forms::Label());
			this->ErrorMsg_textBox = (gcnew System::Windows::Forms::TextBox());
			this->DDSFormat_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->ScaleWithDirectX_checkBox = (gcnew System::Windows::Forms::CheckBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Width_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Height_numericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// AllFile_listBox
			// 
			this->AllFile_listBox->AllowDrop = true;
			this->AllFile_listBox->FormattingEnabled = true;
			this->AllFile_listBox->HorizontalScrollbar = true;
			this->AllFile_listBox->ItemHeight = 12;
			this->AllFile_listBox->Location = System::Drawing::Point(26, 10);
			this->AllFile_listBox->Margin = System::Windows::Forms::Padding(2);
			this->AllFile_listBox->Name = L"AllFile_listBox";
			this->AllFile_listBox->Size = System::Drawing::Size(310, 268);
			this->AllFile_listBox->TabIndex = 0;
			this->AllFile_listBox->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::AllFile_listBox_DragDrop);
			this->AllFile_listBox->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::AllFile_listBox_DragEnter);
			// 
			// AddFile_button
			// 
			this->AddFile_button->Location = System::Drawing::Point(46, 352);
			this->AddFile_button->Margin = System::Windows::Forms::Padding(2);
			this->AddFile_button->Name = L"AddFile_button";
			this->AddFile_button->Size = System::Drawing::Size(49, 20);
			this->AddFile_button->TabIndex = 2;
			this->AddFile_button->Text = L"AddFile";
			this->AddFile_button->UseVisualStyleBackColor = true;
			// 
			// DeleteFile_button
			// 
			this->DeleteFile_button->Location = System::Drawing::Point(106, 352);
			this->DeleteFile_button->Margin = System::Windows::Forms::Padding(2);
			this->DeleteFile_button->Name = L"DeleteFile_button";
			this->DeleteFile_button->Size = System::Drawing::Size(49, 20);
			this->DeleteFile_button->TabIndex = 3;
			this->DeleteFile_button->Text = L"Delete";
			this->DeleteFile_button->UseVisualStyleBackColor = true;
			// 
			// Export_button
			// 
			this->Export_button->Location = System::Drawing::Point(176, 352);
			this->Export_button->Margin = System::Windows::Forms::Padding(2);
			this->Export_button->Name = L"Export_button";
			this->Export_button->Size = System::Drawing::Size(49, 20);
			this->Export_button->TabIndex = 4;
			this->Export_button->Text = L"Export";
			this->Export_button->UseVisualStyleBackColor = true;
			this->Export_button->Click += gcnew System::EventHandler(this, &Form1::Export_button_Click);
			// 
			// Width_numericUpDown
			// 
			this->Width_numericUpDown->Location = System::Drawing::Point(318, 291);
			this->Width_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->Width_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {8096, 0, 0, 0});
			this->Width_numericUpDown->Name = L"Width_numericUpDown";
			this->Width_numericUpDown->Size = System::Drawing::Size(81, 22);
			this->Width_numericUpDown->TabIndex = 5;
			this->Width_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			// 
			// Height_numericUpDown
			// 
			this->Height_numericUpDown->Location = System::Drawing::Point(318, 332);
			this->Height_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->Height_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {8096, 0, 0, 0});
			this->Height_numericUpDown->Name = L"Height_numericUpDown";
			this->Height_numericUpDown->Size = System::Drawing::Size(81, 22);
			this->Height_numericUpDown->TabIndex = 6;
			this->Height_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(260, 291);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(34, 12);
			this->label1->TabIndex = 7;
			this->label1->Text = L"Width";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(258, 332);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(36, 12);
			this->label2->TabIndex = 8;
			this->label2->Text = L"Height";
			// 
			// RecursivelyAddFile_button
			// 
			this->RecursivelyAddFile_button->Location = System::Drawing::Point(46, 291);
			this->RecursivelyAddFile_button->Margin = System::Windows::Forms::Padding(2);
			this->RecursivelyAddFile_button->Name = L"RecursivelyAddFile_button";
			this->RecursivelyAddFile_button->Size = System::Drawing::Size(178, 31);
			this->RecursivelyAddFile_button->TabIndex = 10;
			this->RecursivelyAddFile_button->Text = L"AddFileRecursively";
			this->RecursivelyAddFile_button->UseVisualStyleBackColor = true;
			this->RecursivelyAddFile_button->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// Format_comboBox
			// 
			this->Format_comboBox->FormattingEnabled = true;
			this->Format_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(10) {L"BMP", L"JPG", L"TGA", L"PNG", L"DDS", L"PPM", 
				L"DIB", L"HDR", L"PFM", L"All Image Support"});
			this->Format_comboBox->Location = System::Drawing::Point(318, 374);
			this->Format_comboBox->Margin = System::Windows::Forms::Padding(2);
			this->Format_comboBox->Name = L"Format_comboBox";
			this->Format_comboBox->Size = System::Drawing::Size(82, 20);
			this->Format_comboBox->TabIndex = 11;
			this->Format_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::Format_comboBox_SelectedIndexChanged);
			// 
			// TransformType_comboBox
			// 
			this->TransformType_comboBox->FormattingEnabled = true;
			this->TransformType_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"PercentScale/百分比縮放", L"FixedSize/照顧定尺寸縮放", 
				L"ZoneCut/裁切指定區域", L"ToPowerOf2WithPercentControl"});
			this->TransformType_comboBox->Location = System::Drawing::Point(245, 413);
			this->TransformType_comboBox->Margin = System::Windows::Forms::Padding(2);
			this->TransformType_comboBox->Name = L"TransformType_comboBox";
			this->TransformType_comboBox->Size = System::Drawing::Size(155, 20);
			this->TransformType_comboBox->TabIndex = 12;
			this->TransformType_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::TransformType_comboBox_SelectedIndexChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(216, 399);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(78, 12);
			this->label3->TabIndex = 13;
			this->label3->Text = L"TransformType";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(256, 381);
			this->label4->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(38, 12);
			this->label4->TabIndex = 14;
			this->label4->Text = L"Format";
			// 
			// panel1
			// 
			this->panel1->Location = System::Drawing::Point(70, 382);
			this->panel1->Margin = System::Windows::Forms::Padding(2);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(38, 50);
			this->panel1->TabIndex = 15;
			// 
			// ReplaceOriginalFile_checkBox
			// 
			this->ReplaceOriginalFile_checkBox->AutoSize = true;
			this->ReplaceOriginalFile_checkBox->Location = System::Drawing::Point(413, 413);
			this->ReplaceOriginalFile_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->ReplaceOriginalFile_checkBox->Name = L"ReplaceOriginalFile_checkBox";
			this->ReplaceOriginalFile_checkBox->Size = System::Drawing::Size(116, 16);
			this->ReplaceOriginalFile_checkBox->TabIndex = 16;
			this->ReplaceOriginalFile_checkBox->Text = L"ReplaceOriginalFile";
			this->ReplaceOriginalFile_checkBox->UseVisualStyleBackColor = true;
			// 
			// CreateFolderIfNotReplaceOriginalFile_checkBox
			// 
			this->CreateFolderIfNotReplaceOriginalFile_checkBox->AutoSize = true;
			this->CreateFolderIfNotReplaceOriginalFile_checkBox->Checked = true;
			this->CreateFolderIfNotReplaceOriginalFile_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->CreateFolderIfNotReplaceOriginalFile_checkBox->Location = System::Drawing::Point(537, 413);
			this->CreateFolderIfNotReplaceOriginalFile_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->CreateFolderIfNotReplaceOriginalFile_checkBox->Name = L"CreateFolderIfNotReplaceOriginalFile_checkBox";
			this->CreateFolderIfNotReplaceOriginalFile_checkBox->Size = System::Drawing::Size(201, 16);
			this->CreateFolderIfNotReplaceOriginalFile_checkBox->TabIndex = 17;
			this->CreateFolderIfNotReplaceOriginalFile_checkBox->Text = L"CreateFolderIfNotReplaceOriginalFile";
			this->CreateFolderIfNotReplaceOriginalFile_checkBox->UseVisualStyleBackColor = true;
			// 
			// Progress_label
			// 
			this->Progress_label->AutoSize = true;
			this->Progress_label->Location = System::Drawing::Point(502, 332);
			this->Progress_label->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->Progress_label->Name = L"Progress_label";
			this->Progress_label->Size = System::Drawing::Size(44, 12);
			this->Progress_label->TabIndex = 18;
			this->Progress_label->Text = L"Progress";
			// 
			// ErrorMsg_textBox
			// 
			this->ErrorMsg_textBox->Location = System::Drawing::Point(358, 19);
			this->ErrorMsg_textBox->Margin = System::Windows::Forms::Padding(2);
			this->ErrorMsg_textBox->Multiline = true;
			this->ErrorMsg_textBox->Name = L"ErrorMsg_textBox";
			this->ErrorMsg_textBox->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->ErrorMsg_textBox->Size = System::Drawing::Size(362, 258);
			this->ErrorMsg_textBox->TabIndex = 19;
			// 
			// DDSFormat_comboBox
			// 
			this->DDSFormat_comboBox->FormattingEnabled = true;
			this->DDSFormat_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(6) {L"DDSPF_DXT1//1 bit alpha channel -> black/white,does not support alpha channels", 
				L"DDSPF_DXT2", L"DDSPF_DXT3//sharp alpha transitions ", L"DDSPF_DXT4", L"DDSPF_DXT5//8 bit -> 256 grey, gradient alpha transitions.", 
				L"DDS_RGBA"});
			this->DDSFormat_comboBox->Location = System::Drawing::Point(413, 373);
			this->DDSFormat_comboBox->Name = L"DDSFormat_comboBox";
			this->DDSFormat_comboBox->Size = System::Drawing::Size(272, 20);
			this->DDSFormat_comboBox->TabIndex = 20;
			// 
			// ScaleWithDirectX_checkBox
			// 
			this->ScaleWithDirectX_checkBox->AutoSize = true;
			this->ScaleWithDirectX_checkBox->Checked = true;
			this->ScaleWithDirectX_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->ScaleWithDirectX_checkBox->Location = System::Drawing::Point(456, 291);
			this->ScaleWithDirectX_checkBox->Name = L"ScaleWithDirectX_checkBox";
			this->ScaleWithDirectX_checkBox->Size = System::Drawing::Size(107, 16);
			this->ScaleWithDirectX_checkBox->TabIndex = 21;
			this->ScaleWithDirectX_checkBox->Text = L"ScaleWithDirectX";
			this->ScaleWithDirectX_checkBox->UseVisualStyleBackColor = true;
			this->ScaleWithDirectX_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::ScaleWithDirectX_checkBox_CheckedChanged);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(739, 448);
			this->Controls->Add(this->ScaleWithDirectX_checkBox);
			this->Controls->Add(this->DDSFormat_comboBox);
			this->Controls->Add(this->ErrorMsg_textBox);
			this->Controls->Add(this->Progress_label);
			this->Controls->Add(this->CreateFolderIfNotReplaceOriginalFile_checkBox);
			this->Controls->Add(this->ReplaceOriginalFile_checkBox);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->TransformType_comboBox);
			this->Controls->Add(this->Format_comboBox);
			this->Controls->Add(this->RecursivelyAddFile_button);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->Height_numericUpDown);
			this->Controls->Add(this->Width_numericUpDown);
			this->Controls->Add(this->Export_button);
			this->Controls->Add(this->DeleteFile_button);
			this->Controls->Add(this->AddFile_button);
			this->Controls->Add(this->AllFile_listBox);
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"Form1";
			this->Text = L"Form1";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Width_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Height_numericUpDown))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 AllFile_listBox->Items->Clear();
				 String^l_strDirectoryName = DNCT::SelectDirectory();
				 m_strDelectedDirectory = l_strDirectoryName;
				 if( l_strDirectoryName )
				 {
					 System::Collections::ArrayList^l_pArrayList = gcnew System::Collections::ArrayList();
					 GetFilesNameByRecursivelyDirectory(l_strDirectoryName,l_pArrayList);
					 char*l_strAllSupportFormat[] = {
						".BMP",".JPG",
						".TGA",".PNG",".DDS",".PPM",
						".DIB",".HDR",".PFM",
						"All Image Support"
					 };
					 for each(String^l_strName in l_pArrayList)
					 {
						String^l_strExtensionName = System::IO::Path::GetExtension(l_strName);
						auto l_strFileExtensionName = DNCT::GcStringToChar(l_strExtensionName);
						if( Format_comboBox->SelectedIndex!=9 )
						{
							if(!_stricmp(l_strAllSupportFormat[Format_comboBox->SelectedIndex],l_strFileExtensionName.c_str()))
							//if( l_strExtensionName->Equals(l_strAllSupportFormat[Format_comboBox->SelectedIndex]) )
							{
								AllFile_listBox->Items->Add(l_strName);
							}
						}
						else
						{
							for(int i=0;i<9;++i)
							{
								if(!_stricmp(l_strAllSupportFormat[i],l_strFileExtensionName.c_str()))
								//if( l_strExtensionName->Equals(l_strAllSupportFormat[i]) )
								{
									AllFile_listBox->Items->Add(l_strName);
								}
							}
						}
					 }
				 }
		 }

private: System::Void Export_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 ErrorMsg_textBox->Text = "";
			 Progress_label->Text = "0";
			 String^l_strExtensionName;
			 String^l_strDirectoryName = this->m_strDelectedDirectory?m_strDelectedDirectory:"";
			 if( !ReplaceOriginalFile_checkBox->Checked )
				l_strDirectoryName = DNCT::SelectDirectory();
			 if( !l_strDirectoryName )
				 return;
			 D3DXIMAGE_FILEFORMAT	l_D3DXIMAGE_FILEFORMAT = D3DXIFF_DDS;
			 switch(Format_comboBox->SelectedIndex)
			 {
				case 0:					l_D3DXIMAGE_FILEFORMAT = D3DXIFF_BMP;					l_strExtensionName = "bmp";				break;
				case 1:					l_D3DXIMAGE_FILEFORMAT = D3DXIFF_JPG;					l_strExtensionName = "jpg";				break;
				case 2:					l_D3DXIMAGE_FILEFORMAT = D3DXIFF_TGA;					l_strExtensionName = "tga";				break;
				case 3:					l_D3DXIMAGE_FILEFORMAT = D3DXIFF_PNG;					l_strExtensionName = "png";				break;
				case 4:					l_D3DXIMAGE_FILEFORMAT = D3DXIFF_DDS;					l_strExtensionName = "dds";				break;
				case 5:					l_D3DXIMAGE_FILEFORMAT = D3DXIFF_PPM;					l_strExtensionName = "ppm";				break;
				case 6:					l_D3DXIMAGE_FILEFORMAT = D3DXIFF_DIB;					l_strExtensionName = "dib";				break;
				case 7:					l_D3DXIMAGE_FILEFORMAT = D3DXIFF_HDR;					l_strExtensionName = "hdr";				break;
				case 8:					l_D3DXIMAGE_FILEFORMAT = D3DXIFF_PFM;					l_strExtensionName = "pfm";				break;
			 }
			 for(int i=0;i<AllFile_listBox->Items->Count;++i)
			 {
				 if( ScaleWithDirectX_checkBox->Checked )
					ScaleImageWithDirectX(l_strDirectoryName,AllFile_listBox->Items[i]->ToString(),l_D3DXIMAGE_FILEFORMAT,l_strExtensionName);
				 else
					 ScaleImageWithDotNet(l_strDirectoryName,AllFile_listBox->Items[i]->ToString(),l_strExtensionName);

				float	l_fProgress = i/(float)AllFile_listBox->Items->Count;
				Progress_label->Text = l_fProgress.ToString();
				this->Update();
				this->Refresh();
			 }
			 Progress_label->Text = "Done";
		 }
private: System::Void AllFile_listBox_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
		 {
			AllFile_listBox->Items->Clear();
			this->Text = "";
			cli::array<String^>^l_strFileNames = DNCT::DragDropEventWhileFileDrop(e);
			for each(String^l_str in l_strFileNames)
			{
				AllFile_listBox->Items->Add(l_str);
			}
		 }
private: System::Void AllFile_listBox_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) 
		 {
			 if ( e->Data->GetDataPresent( DataFormats::FileDrop ) )
			 {
				e->Effect = DragDropEffects::Copy;
			 }
			 else
				 e->Effect = DragDropEffects::None;
		 }
private: System::Void Format_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( Format_comboBox->SelectedIndex == 4 )
			 {
				 DDSFormat_comboBox->Visible = true;
			 }
			 else
			 {
				DDSFormat_comboBox->Visible = false;
			 }
		 }
private: System::Void ScaleWithDirectX_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(ScaleWithDirectX_checkBox->Checked)
			 {
				ScaleWithDirectX_checkBox->Text = "Scale With DirectX";
			 }
			 else
			 {
				ScaleWithDirectX_checkBox->Text = "Scale With dot net";
			 }
		 }
private: System::Void TransformType_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if( TransformType_comboBox->SelectedIndex == 2 )
				Progress_label->Text = "only dot support";
			 else
				 Progress_label->Text = "";
		 }
};
}

