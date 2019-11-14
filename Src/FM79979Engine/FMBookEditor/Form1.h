#pragma once


#include "FatmingUtilities.h"

#include "FMBookEditorUIImplement/EventObjectStatusList.h"
#include "FMBookEditorUIImplement/EventObjectStatus.h"
#include "FMBookEditorUIImplement/EventVariableManager.h"
#include "FMBookEditorUIImplement/EventPackages.h"
#include "FMBookEditorUIImplement/EventInstanceList.h"
#include "FMBookEditorUIImplement/EventObjectInstanceList.h"
namespace FMBookEditor 
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO:
			//
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
			delete m_pFMBookUI;
			delete m_pFatmingUtilitiesData;
			delete m_pEventInstance_Editor;
			delete m_pEventObjectStatusList_Editor;
			delete m_pEventObjectStatus;
			delete m_pEventPackages_Editor;
			delete m_pEventInstanceList_Editor;
			delete m_pEventObjectInstance_Editor;
			SAFE_DELETE(m_pFMBookApp);
		}

	private:
	private: System::Windows::Forms::Timer^  timer1;
		//my
		public:FMBookEditorUI::FMBookUI^					m_pFMBookUI;
		cFatmingUtilitiesData^								m_pFatmingUtilitiesData;
		EventPackages_Editor^								m_pEventPackages_Editor;
		public:FMBookEditor::cEventInstance_Editor^			m_pEventInstance_Editor;
		public:FMBookEditor::EventObjectStatus^				m_pEventObjectStatus;
			   FMBookEditor::EventInstanceList_Editor^		m_pEventInstanceList_Editor;
		public:FMBookEditor::EventObjectStatusList_Editor^	m_pEventObjectStatusList_Editor;
			   FMBookEditor::cEventVariableManager_Editor^	m_pEventVariableManager_Editor;
			   FMBookEditor::cEventObjectInstance_Editor^	m_pEventObjectInstance_Editor;

			   System::Windows::Controls::Border^m_pRenderBorder;
			   cFMBookApp*m_pFMBookApp;
	private: System::ComponentModel::IContainer^  components;


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改這個方法的內容。
		///
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 262);
			this->KeyPreview = true;
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
			 {				 
				System::Windows::Forms::Integration::WindowsFormsHost^l_pWindowsFormsHost = (System::Windows::Forms::Integration::WindowsFormsHost^)m_pRenderBorder->Child;
				GCFORM::Panel^l_pPanel = (GCFORM::Panel^)l_pWindowsFormsHost->Child;
				if( m_pFMBookApp && m_pFMBookApp->m_bInitOk )
				{
					m_pFMBookApp->m_svViewPortSize.x = 0;
					m_pFMBookApp->m_svViewPortSize.y = 0;
					m_pFMBookApp->m_svViewPortSize.z = (float)l_pPanel->Width;
					m_pFMBookApp->m_svViewPortSize.w = (float)l_pPanel->Height;//(float)this->m_pFMBookUI->ActualHeight;
					m_pFMBookApp->Run();
				}
			 }

private: System::Void FileOpen_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 this->timer1->Enabled = false;
			 String^l_strFileName = DNCT::OpenFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
			 if( l_strFileName )
			 {
				 if(sFMBookEditorData::OpenFile(DNCT::GcStringToChar(l_strFileName).c_str()))
				 {
					 this->Text = l_strFileName;
				 }
				 else
				 {
					this->Text = l_strFileName+"open failed";
				 }
				 m_pFatmingUtilitiesData->Refresh();
				 m_pEventPackages_Editor->Refresh();
				 m_pEventObjectStatusList_Editor->Refresh();
				 m_pEventInstanceList_Editor->Refresh();
				 m_pEventVariableManager_Editor->Refresh();
				 m_pEventObjectInstance_Editor->Refresh();
			 }
			 this->timer1->Enabled = true;
		 }

private: System::Void FileSave_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 this->timer1->Enabled = false;
			 String^l_strFileName = DNCT::SaveFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
			 if( l_strFileName )
			 {
				sFMBookEditorData::SaveFile(DNCT::GcStringToChar(l_strFileName).c_str());
			 }
			 this->timer1->Enabled = true;		 
		 }

private: System::Void Form1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			//System::Windows::Forms::Integration::WindowsFormsHost^l_pWindowsFormsHost = (System::Windows::Forms::Integration::WindowsFormsHost^)m_pRenderBorder->Child;
			//l_pWindowsFormsHost->Child->Focus();
			 m_pFMBookApp->MouseMove(e->X,e->Y);
		 }
private: System::Void Form1_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			 m_pFMBookApp->MouseUp(e->X,e->Y);
		 }
private: System::Void Form1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			 m_pFMBookApp->MouseDown(e->X,e->Y);
		 }
private: System::Void Form1_KeyDown(System::Object^  sender, System::Windows::Input::KeyEventArgs^  e)
		 {
			 System::Windows::Input::KeyEventArgs^ l_e = (System::Windows::Input::KeyEventArgs^)e;
			 System::Windows::Input::KeyConverter^l_pKeyConverter = gcnew System::Windows::Input::KeyConverter;
			 String^l_str = l_pKeyConverter->ConvertToString(l_e->Key);			 
			 m_pFMBookApp->KeyDown((char)l_str[0]);
			 delete l_pKeyConverter;
		 }
private: System::Void Form1_KeyUp(System::Object^  sender,  System::Windows::Input::KeyEventArgs^  e)
		 {
			 System::Windows::Input::KeyEventArgs^ l_e = (System::Windows::Input::KeyEventArgs^)e;
			 System::Windows::Input::KeyConverter^l_pKeyConverter = gcnew System::Windows::Input::KeyConverter;
			 String^l_str = l_pKeyConverter->ConvertToString(l_e->Key);
			 m_pFMBookApp->KeyUp((char)l_str[0]);
			 delete l_pKeyConverter;
		 }
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e)
		 {

			m_pFMBookUI = gcnew FMBookEditorUI::FMBookUI;
			m_pRenderBorder =  m_pFMBookUI->m_RenderWindow;
			//UT::InitOpenGL((HWND)l_pWindowsFormsHost->Child->Handle.ToPointer(),true,l_dc);
			m_pFMBookUI->f_KeyUp += gcnew System::Windows::Input::KeyEventHandler(this, &Form1::Form1_KeyUp);
			m_pFMBookUI->f_KeyDown += gcnew System::Windows::Input::KeyEventHandler(this, &Form1::Form1_KeyDown);
			m_pFMBookUI->f_FileOpen += gcnew EventHandler(this,&Form1::FileOpen_button_Click);
			m_pFMBookUI->f_FileSave += gcnew EventHandler(this,&Form1::FileSave_button_Click);

			//m_pFMBookUI->InitializeComponent();
			System::Windows::Forms::Integration::ElementHost^l_pElementHots = gcnew System::Windows::Forms::Integration::ElementHost;
			l_pElementHots->Dock = DockStyle::Fill;
			l_pElementHots->Child = m_pFMBookUI;
			this->Controls->Add(l_pElementHots);
			this->Text = "FMBook";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;

			
			//if(m_pFMBookApp->m_spEventManager->ParseDataIntoXMLNode("C:/Users/Fatming_Air/Desktop/EnglishTalk/EventPachages.xml"))
			//{
			//	 m_pFMBookApp->m_spEventManager->m_pEventPackageRunner->LoadPackage();
			//}
			System::Windows::Forms::Integration::WindowsFormsHost^l_pWindowsFormsHost = (System::Windows::Forms::Integration::WindowsFormsHost^)m_pRenderBorder->Child;
			l_pWindowsFormsHost->Child->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Form1_MouseDown);
			l_pWindowsFormsHost->Child->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Form1_MouseMove);
			l_pWindowsFormsHost->Child->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Form1_MouseUp);
			HDC	l_dc = GetDC((HWND)l_pWindowsFormsHost->Child->Handle.ToPointer());

			//FMBookSetup.xml
			//<FMBookSetup Mode="1" Resolution="1280,720" Description="Mode1是書本模式0是編集模式" />
			//GetLastError();
			SetLastError(0);
			cNodeISAX	l_cNodeISAX;
			Vector2	l_vGameResolution(1920.f,1080.f);
			int	l_iMode = 0;
			if(l_cNodeISAX.ParseDataIntoXMLNode("FMBookSetup.xml"))
			{
				TiXmlElement*l_pElement = l_cNodeISAX.GetRootElement();
				if( l_pElement )
				{
					const WCHAR*l_strResolution = l_pElement->Attribute(L"Resolution");
					const WCHAR*l_strMode = l_pElement->Attribute(L"Mode");
					if( l_strResolution )
						l_vGameResolution = GetVector2(l_strResolution);
					if(l_strMode)
						l_iMode = GetInt(l_strMode);
				}
			}
			m_pFMBookApp = new cFMBookApp(l_iMode,(HWND)l_pWindowsFormsHost->Child->Handle.ToPointer(),l_vGameResolution);
			m_pFMBookApp->Init();
			m_pEventVariableManager_Editor = gcnew cEventVariableManager_Editor(m_pFMBookUI);
			m_pFatmingUtilitiesData = gcnew cFatmingUtilitiesData(m_pFMBookUI,m_pFMBookApp);
			m_pEventInstance_Editor = gcnew cEventInstance_Editor(m_pFMBookUI);
			m_pEventObjectStatus = gcnew EventObjectStatus(m_pFMBookUI,m_pFatmingUtilitiesData);
			m_pEventObjectStatusList_Editor = gcnew EventObjectStatusList_Editor(m_pFMBookUI,m_pEventObjectStatus);
			m_pEventPackages_Editor = gcnew EventPackages_Editor(m_pFMBookUI);
			m_pEventInstanceList_Editor = gcnew EventInstanceList_Editor(m_pFMBookUI,m_pEventInstance_Editor);
			m_pEventObjectInstance_Editor = gcnew cEventObjectInstance_Editor(m_pFMBookUI,m_pEventInstanceList_Editor);
			timer1->Enabled = true;
			//l_pWindowsFormsHost->Focus();
		 }
};
}

