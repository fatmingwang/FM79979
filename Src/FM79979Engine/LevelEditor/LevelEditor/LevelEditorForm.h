#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "ObjectDataForm.h"
#include "EventEditor.h"

extern cOrthogonalCamera	*g_pOrthogonalCamera;
namespace LevelEditor 
{

	public ref class LevelEditorForm : public System::Windows::Forms::Form
	{
	public:
		LevelEditorForm(void)
		{
			ilInit();
			alutInit (0,0);
			InitializeComponent();
			//
			//TODO: 在此加入建構函式程式碼
			//
			gcnew ObjectDataForm(tabControl1,"LevelEditor",this);
			tabControl1->TabPages->Remove(tabPage1);
			//gcnew EventEditor(tabControl1);
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~LevelEditorForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  resolutionToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem2;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem3;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem4;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem5;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem6;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem7;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem8;
	private: System::Windows::Forms::ToolStripMenuItem^  assistanceToolStripMenuItem;
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
        this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
        this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
        this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
        this->resolutionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
        this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
        this->toolStripMenuItem3 = (gcnew System::Windows::Forms::ToolStripMenuItem());
        this->toolStripMenuItem4 = (gcnew System::Windows::Forms::ToolStripMenuItem());
        this->toolStripMenuItem5 = (gcnew System::Windows::Forms::ToolStripMenuItem());
        this->toolStripMenuItem6 = (gcnew System::Windows::Forms::ToolStripMenuItem());
        this->toolStripMenuItem7 = (gcnew System::Windows::Forms::ToolStripMenuItem());
        this->toolStripMenuItem8 = (gcnew System::Windows::Forms::ToolStripMenuItem());
        this->assistanceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
        this->tabControl1->SuspendLayout();
        this->menuStrip1->SuspendLayout();
        this->SuspendLayout();
        // 
        // tabControl1
        // 
        this->tabControl1->Controls->Add(this->tabPage1);
        this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
        this->tabControl1->Location = System::Drawing::Point(0, 24);
        this->tabControl1->Margin = System::Windows::Forms::Padding(2);
        this->tabControl1->Multiline = true;
        this->tabControl1->Name = L"tabControl1";
        this->tabControl1->SelectedIndex = 0;
        this->tabControl1->Size = System::Drawing::Size(610, 434);
        this->tabControl1->TabIndex = 4;
        // 
        // tabPage1
        // 
        this->tabPage1->Location = System::Drawing::Point(4, 22);
        this->tabPage1->Margin = System::Windows::Forms::Padding(2);
        this->tabPage1->Name = L"tabPage1";
        this->tabPage1->Size = System::Drawing::Size(602, 408);
        this->tabPage1->TabIndex = 0;
        this->tabPage1->Text = L"tabPage1";
        this->tabPage1->UseVisualStyleBackColor = true;
        // 
        // menuStrip1
        // 
        this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->resolutionToolStripMenuItem, 
            this->assistanceToolStripMenuItem});
        this->menuStrip1->Location = System::Drawing::Point(0, 0);
        this->menuStrip1->Name = L"menuStrip1";
        this->menuStrip1->Padding = System::Windows::Forms::Padding(4, 2, 0, 2);
        this->menuStrip1->Size = System::Drawing::Size(610, 24);
        this->menuStrip1->TabIndex = 5;
        this->menuStrip1->Text = L"menuStrip1";
        // 
        // resolutionToolStripMenuItem
        // 
        this->resolutionToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->toolStripMenuItem2, 
            this->toolStripMenuItem3, this->toolStripMenuItem4, this->toolStripMenuItem5, this->toolStripMenuItem6, this->toolStripMenuItem7, 
            this->toolStripMenuItem8});
        this->resolutionToolStripMenuItem->Name = L"resolutionToolStripMenuItem";
        this->resolutionToolStripMenuItem->Size = System::Drawing::Size(69, 20);
        this->resolutionToolStripMenuItem->Text = L"Resolution";
        // 
        // toolStripMenuItem2
        // 
        this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
        this->toolStripMenuItem2->Size = System::Drawing::Size(128, 22);
        this->toolStripMenuItem2->Text = L"1920X1200";
        this->toolStripMenuItem2->Click += gcnew System::EventHandler(this, &LevelEditorForm::toolStripMenuItem2_Click);
        // 
        // toolStripMenuItem3
        // 
        this->toolStripMenuItem3->Name = L"toolStripMenuItem3";
        this->toolStripMenuItem3->Size = System::Drawing::Size(128, 22);
        this->toolStripMenuItem3->Text = L"1920X1080";
        this->toolStripMenuItem3->Click += gcnew System::EventHandler(this, &LevelEditorForm::toolStripMenuItem2_Click);
        // 
        // toolStripMenuItem4
        // 
        this->toolStripMenuItem4->Name = L"toolStripMenuItem4";
        this->toolStripMenuItem4->Size = System::Drawing::Size(128, 22);
        this->toolStripMenuItem4->Text = L"1600X1440";
        this->toolStripMenuItem4->Click += gcnew System::EventHandler(this, &LevelEditorForm::toolStripMenuItem2_Click);
        // 
        // toolStripMenuItem5
        // 
        this->toolStripMenuItem5->Name = L"toolStripMenuItem5";
        this->toolStripMenuItem5->Size = System::Drawing::Size(128, 22);
        this->toolStripMenuItem5->Text = L"1280X1024";
        this->toolStripMenuItem5->Click += gcnew System::EventHandler(this, &LevelEditorForm::toolStripMenuItem2_Click);
        // 
        // toolStripMenuItem6
        // 
        this->toolStripMenuItem6->Name = L"toolStripMenuItem6";
        this->toolStripMenuItem6->Size = System::Drawing::Size(128, 22);
        this->toolStripMenuItem6->Text = L"1280X720";
        this->toolStripMenuItem6->Click += gcnew System::EventHandler(this, &LevelEditorForm::toolStripMenuItem2_Click);
        // 
        // toolStripMenuItem7
        // 
        this->toolStripMenuItem7->Name = L"toolStripMenuItem7";
        this->toolStripMenuItem7->Size = System::Drawing::Size(128, 22);
        this->toolStripMenuItem7->Text = L"640X480";
        this->toolStripMenuItem7->Click += gcnew System::EventHandler(this, &LevelEditorForm::toolStripMenuItem2_Click);
        // 
        // toolStripMenuItem8
        // 
        this->toolStripMenuItem8->Name = L"toolStripMenuItem8";
        this->toolStripMenuItem8->Size = System::Drawing::Size(128, 22);
        this->toolStripMenuItem8->Text = L"480X320";
        this->toolStripMenuItem8->Click += gcnew System::EventHandler(this, &LevelEditorForm::toolStripMenuItem2_Click);
        // 
        // assistanceToolStripMenuItem
        // 
        this->assistanceToolStripMenuItem->Name = L"assistanceToolStripMenuItem";
        this->assistanceToolStripMenuItem->Size = System::Drawing::Size(70, 20);
        this->assistanceToolStripMenuItem->Text = L"Assistance";
        // 
        // LevelEditorForm
        // 
        this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
        this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
        this->ClientSize = System::Drawing::Size(610, 458);
        this->Controls->Add(this->tabControl1);
        this->Controls->Add(this->menuStrip1);
        this->KeyPreview = true;
        this->MainMenuStrip = this->menuStrip1;
        this->Margin = System::Windows::Forms::Padding(2);
        this->Name = L"LevelEditorForm";
        this->Text = L"LevelEditorForm";
        this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
        this->tabControl1->ResumeLayout(false);
        this->menuStrip1->ResumeLayout(false);
        this->menuStrip1->PerformLayout();
        this->ResumeLayout(false);
        this->PerformLayout();

            }
#pragma endregion
private: System::Void toolStripMenuItem2_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( toolStripMenuItem2 == sender )
			 {
				cGameApp::m_svGameResolution.x = 1920.f;
				cGameApp::m_svGameResolution.y = 1200.f;
			 }
			 else
			 if( toolStripMenuItem3 == sender )
			 {
				cGameApp::m_svGameResolution.x = 1920.f;
				cGameApp::m_svGameResolution.y = 1080.f;			 
			 }
			 else
			 if( toolStripMenuItem4 == sender )
			 {
				cGameApp::m_svGameResolution.x = 1600.f;
				cGameApp::m_svGameResolution.y = 1440.f;			 
			 }
			 else
			 if( toolStripMenuItem5 == sender )
			 {
				cGameApp::m_svGameResolution.x = 1280.f;
				cGameApp::m_svGameResolution.y = 1024.f;
			 }
			 else
			 if( toolStripMenuItem6 == sender )
			 {
				cGameApp::m_svGameResolution.x = 1280.f;
				cGameApp::m_svGameResolution.y = 720.f;
			 }
			 else
			 if( toolStripMenuItem7 == sender )
			 {
				cGameApp::m_svGameResolution.x = 640.f;
				cGameApp::m_svGameResolution.y = 480.f;
			 }
			 else
			 if( toolStripMenuItem8 == sender )
			 {
				cGameApp::m_svGameResolution.x = 480.f;
				cGameApp::m_svGameResolution.y = 320.f;
			 }
		 }
};
}
