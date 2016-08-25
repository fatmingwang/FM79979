#pragma once
#include <string>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../XML/StringCompress.h"

#include "StageParser.h"
#include "ChartWithName.h"

namespace XMLDataEditor 
{

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
		Form1(String^e_strFileName)
		{
			InitializeComponent();

			m_pTargetControl = OpemnGL_panel;
			m_pGameApp = new cGameApp((HWND)m_pTargetControl->Handle.ToPointer());
			m_pGameApp->Init();
			m_HdcMV = GetDC((HWND)m_pTargetControl->Handle.ToPointer());
			m_HGLRCMV = m_pGameApp->m_sHGLRC;
			m_pOrthogonalCamera = new cOrthogonalCamera();
			//for mouse event
			m_pTargetControl->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseMove);
			m_pTargetControl->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseDown);
			m_pTargetControl->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseUp);
			m_pTargetControl->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::MyMouseUp);

			m_pStageParser = new cStageParser();
			m_pChartWithName = new cChartWithName();
			this->timer1->Enabled = true;
			m_bWaitParseFile = false;
			m_iFirstStageMonsterCount = 0;
			m_iFirstStageHurdleAndBreakableCount = 0;
			m_iFirstStagePickUpCount = 0;
			m_iFirstStageCoinCount = 0;
			m_fFirstSgateTime = 0.f;
		}

	protected:
		~Form1()
		{
			if (components)
			{
				delete components;
			}
			SAFE_DELETE(m_pOrthogonalCamera);
			SAFE_DELETE(m_pGameApp);
			SAFE_DELETE(m_pStageParser);
			SAFE_DELETE(m_pChartWithName);
		}
//my
		cStageParser*		m_pStageParser;
		cChartWithName*		m_pChartWithName;
		cGameApp*			m_pGameApp;
		HDC					m_HdcMV;
		HGLRC				m_HGLRCMV;
		Control^			m_pTargetControl;
		cOrthogonalCamera*	m_pOrthogonalCamera;
		bool				m_bWaitParseFile;
		int					m_iFirstStageMonsterCount;
		int					m_iFirstStageHurdleAndBreakableCount;
		int					m_iFirstStagePickUpCount;
		int					m_iFirstStageCoinCount;
		float				m_fFirstSgateTime;

	private: System::Windows::Forms::TextBox^  FileName_textBox;
	private: System::Windows::Forms::Button^  OpenFile_button;
	private: System::Windows::Forms::ListBox^  Node_listBox;

	private: System::Windows::Forms::Label^  Node_label;
	private: System::Windows::Forms::ListBox^  NodeGroupAndCount_listBox;
	private: System::Windows::Forms::Label^  NodeGroupAndCount_label;
	private: System::Windows::Forms::Panel^  OpemnGL_panel;


	private: System::Windows::Forms::Timer^  timer1;


	private: System::Windows::Forms::ListBox^  RenderMode_listBox;
	private: System::Windows::Forms::Label^  RenderMode_label;
	private: System::Windows::Forms::ListBox^  RenderParameterIndex_listBox;
	private: System::Windows::Forms::CheckBox^  ShowParameterValue_checkBox;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::Button^  button1;
	private: System::ComponentModel::IContainer^  components;
			 /// <summary>
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改這個方法的內容。
		///
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->FileName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->OpenFile_button = (gcnew System::Windows::Forms::Button());
			this->Node_listBox = (gcnew System::Windows::Forms::ListBox());
			this->Node_label = (gcnew System::Windows::Forms::Label());
			this->NodeGroupAndCount_listBox = (gcnew System::Windows::Forms::ListBox());
			this->NodeGroupAndCount_label = (gcnew System::Windows::Forms::Label());
			this->OpemnGL_panel = (gcnew System::Windows::Forms::Panel());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->RenderMode_listBox = (gcnew System::Windows::Forms::ListBox());
			this->RenderMode_label = (gcnew System::Windows::Forms::Label());
			this->RenderParameterIndex_listBox = (gcnew System::Windows::Forms::ListBox());
			this->ShowParameterValue_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->SuspendLayout();
			// 
			// FileName_textBox
			// 
			this->FileName_textBox->Location = System::Drawing::Point(12, 12);
			this->FileName_textBox->Name = L"FileName_textBox";
			this->FileName_textBox->Size = System::Drawing::Size(276, 20);
			this->FileName_textBox->TabIndex = 0;
			this->FileName_textBox->Text = L"gameplay.xml";
			// 
			// OpenFile_button
			// 
			this->OpenFile_button->Location = System::Drawing::Point(12, 38);
			this->OpenFile_button->Name = L"OpenFile_button";
			this->OpenFile_button->Size = System::Drawing::Size(117, 29);
			this->OpenFile_button->TabIndex = 1;
			this->OpenFile_button->Text = L"Open Gameplay File";
			this->OpenFile_button->UseVisualStyleBackColor = true;
			this->OpenFile_button->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// Node_listBox
			// 
			this->Node_listBox->FormattingEnabled = true;
			this->Node_listBox->Location = System::Drawing::Point(6, 16);
			this->Node_listBox->Name = L"Node_listBox";
			this->Node_listBox->Size = System::Drawing::Size(211, 485);
			this->Node_listBox->TabIndex = 2;
			this->Node_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::Node_listBox_SelectedIndexChanged);
			// 
			// Node_label
			// 
			this->Node_label->AutoSize = true;
			this->Node_label->Location = System::Drawing::Point(3, 0);
			this->Node_label->Name = L"Node_label";
			this->Node_label->Size = System::Drawing::Size(33, 13);
			this->Node_label->TabIndex = 3;
			this->Node_label->Text = L"Node";
			// 
			// NodeGroupAndCount_listBox
			// 
			this->NodeGroupAndCount_listBox->FormattingEnabled = true;
			this->NodeGroupAndCount_listBox->Location = System::Drawing::Point(6, 520);
			this->NodeGroupAndCount_listBox->Name = L"NodeGroupAndCount_listBox";
			this->NodeGroupAndCount_listBox->Size = System::Drawing::Size(169, 186);
			this->NodeGroupAndCount_listBox->TabIndex = 4;
			// 
			// NodeGroupAndCount_label
			// 
			this->NodeGroupAndCount_label->AutoSize = true;
			this->NodeGroupAndCount_label->Location = System::Drawing::Point(3, 504);
			this->NodeGroupAndCount_label->Name = L"NodeGroupAndCount_label";
			this->NodeGroupAndCount_label->Size = System::Drawing::Size(96, 13);
			this->NodeGroupAndCount_label->TabIndex = 5;
			this->NodeGroupAndCount_label->Text = L"Triggers and count";
			// 
			// OpemnGL_panel
			// 
			this->OpemnGL_panel->Location = System::Drawing::Point(325, 2);
			this->OpemnGL_panel->Name = L"OpemnGL_panel";
			this->OpemnGL_panel->Size = System::Drawing::Size(1547, 948);
			this->OpemnGL_panel->TabIndex = 7;
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// RenderMode_listBox
			// 
			this->RenderMode_listBox->FormattingEnabled = true;
			this->RenderMode_listBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"Map", L"ChartCompare"});
			this->RenderMode_listBox->Location = System::Drawing::Point(181, 542);
			this->RenderMode_listBox->Name = L"RenderMode_listBox";
			this->RenderMode_listBox->Size = System::Drawing::Size(118, 43);
			this->RenderMode_listBox->TabIndex = 10;
			// 
			// RenderMode_label
			// 
			this->RenderMode_label->AutoSize = true;
			this->RenderMode_label->Location = System::Drawing::Point(178, 517);
			this->RenderMode_label->Name = L"RenderMode_label";
			this->RenderMode_label->Size = System::Drawing::Size(69, 13);
			this->RenderMode_label->TabIndex = 11;
			this->RenderMode_label->Text = L"RenderMode";
			// 
			// RenderParameterIndex_listBox
			// 
			this->RenderParameterIndex_listBox->FormattingEnabled = true;
			this->RenderParameterIndex_listBox->Location = System::Drawing::Point(4, 735);
			this->RenderParameterIndex_listBox->Name = L"RenderParameterIndex_listBox";
			this->RenderParameterIndex_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiSimple;
			this->RenderParameterIndex_listBox->Size = System::Drawing::Size(169, 173);
			this->RenderParameterIndex_listBox->TabIndex = 12;
			this->RenderParameterIndex_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::RenderParameterIndex_listBox_SelectedIndexChanged);
			// 
			// ShowParameterValue_checkBox
			// 
			this->ShowParameterValue_checkBox->AutoSize = true;
			this->ShowParameterValue_checkBox->Location = System::Drawing::Point(4, 712);
			this->ShowParameterValue_checkBox->Name = L"ShowParameterValue_checkBox";
			this->ShowParameterValue_checkBox->Size = System::Drawing::Size(128, 17);
			this->ShowParameterValue_checkBox->TabIndex = 13;
			this->ShowParameterValue_checkBox->Text = L"ShowParameterValue";
			this->ShowParameterValue_checkBox->UseVisualStyleBackColor = true;
			this->ShowParameterValue_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::ShowParameterValue_checkBox_CheckedChanged);
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Location = System::Drawing::Point(8, 73);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(311, 938);
			this->tabControl1->TabIndex = 0;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->ShowParameterValue_checkBox);
			this->tabPage1->Controls->Add(this->Node_listBox);
			this->tabPage1->Controls->Add(this->RenderParameterIndex_listBox);
			this->tabPage1->Controls->Add(this->Node_label);
			this->tabPage1->Controls->Add(this->RenderMode_label);
			this->tabPage1->Controls->Add(this->NodeGroupAndCount_listBox);
			this->tabPage1->Controls->Add(this->RenderMode_listBox);
			this->tabPage1->Controls->Add(this->NodeGroupAndCount_label);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(303, 912);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"tabPage1";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// tabPage2
			// 
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(303, 912);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"tabPage2";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(171, 38);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(117, 29);
			this->button1->TabIndex = 8;
			this->button1->Text = L"Open Character File";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// Form1
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1884, 982);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->OpemnGL_panel);
			this->Controls->Add(this->OpenFile_button);
			this->Controls->Add(this->FileName_textBox);
			this->KeyPreview = true;
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Resize += gcnew System::EventHandler(this, &Form1::Form1_Resize);
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void Form1_Resize(System::Object^  sender, System::EventArgs^  e)
		 {
		 }
private:System::Void DumpStatic()
		{
			SAFE_DELETE(m_pChartWithName);
			m_pChartWithName = new cChartWithName();
			RenderParameterIndex_listBox->Items->Clear();
//			this->Static_textBox->Text = "";
			String^l_str;
			cChartWithName::sNameAndData*	l_pTime = new cChartWithName::sNameAndData();
			cChartWithName::sNameAndData*	l_pPickup = new cChartWithName::sNameAndData();
			cChartWithName::sNameAndData*	l_pMonster = new cChartWithName::sNameAndData();
			cChartWithName::sNameAndData*	l_pHurdlwAndBreakable = new cChartWithName::sNameAndData();
			cChartWithName::sNameAndData*	l_pCoin = new cChartWithName::sNameAndData();
			cChartWithName::sNameAndData*	l_pMonsterAndDamageObject = new cChartWithName::sNameAndData();
			l_pTime->m_Name = L"Time";
			l_pPickup->m_Name = L"Pickup";
			l_pMonster->m_Name = L"Monster";
			l_pHurdlwAndBreakable->m_Name = L"HurdlwAndBreakable";
			l_pCoin->m_Name = L"Coin";
			l_pMonsterAndDamageObject->m_Name = L"MonsterAndDamageObject";
			const int	l_Width = m_pChartWithName->m_ObjectIndexDistance;
			const int	l_iHeightScale = m_pChartWithName->m_iHeightScale;
			//m_pChartWithName;
			for(size_t i=0;i<m_pStageParser->m_GamePlayVector.size();++i)
			{
				sGameNode*l_pGameNode = &m_pStageParser->m_GamePlayVector[i];
				std::wstring l_strGamePlayName = l_pGameNode->getValue(L"gameplayName");
				//this->Static_textBox->Text += DNCT::WcharToGcstring(l_strGamePlayName.c_str());
				//this->Static_textBox->Text += DNCT::GetChanglineString();
				//l_str = "Time:"+l_pGameNode->GetFinishTime()+DNCT::GetChanglineString();
				//this->Static_textBox->Text += l_str;
				//l_str = "Pickup:"+l_pGameNode->GetPickupCount()+DNCT::GetChanglineString();
				//this->Static_textBox->Text += l_str;
				//l_str = "Monster:"+l_pGameNode->GetMonsterCount()+DNCT::GetChanglineString();
				//this->Static_textBox->Text += l_str;
				//l_str = "HurdlAndBreakable:"+l_pGameNode->GetHurdleAndBreakableCount()+DNCT::GetChanglineString();
				//this->Static_textBox->Text += l_str;
				//l_str = "Coin:"+l_pGameNode->m_iCoinCount+DNCT::GetChanglineString();
				//this->Static_textBox->Text += l_str;
				l_pTime->m_pData->AddData(Vector3(i*l_Width,l_pGameNode->GetFinishTime()*l_iHeightScale,0),0);
				l_pPickup->m_pData->AddData(Vector3(i*l_Width,l_pGameNode->GetPickupCount()*l_iHeightScale,0),0);
				l_pMonster->m_pData->AddData(Vector3(i*l_Width,l_pGameNode->GetMonsterCount()*l_iHeightScale,0),0);
				l_pHurdlwAndBreakable->m_pData->AddData(Vector3(i*l_Width,l_pGameNode->GetHurdleAndBreakableCount()*l_iHeightScale,0),0);
				l_pCoin->m_pData->AddData(Vector3(i*l_Width,l_pGameNode->m_iCoinCount*l_iHeightScale,0),0);

				l_pMonsterAndDamageObject->m_pData->AddData(Vector3(i*l_Width,l_pGameNode->GetHurdleAndBreakableCount()*l_iHeightScale+l_pGameNode->GetMonsterCount()*l_iHeightScale,0),0);


				//l_str = "TimeScale:"+(l_pGameNode->GetFinishTime()/m_fFirstSgateTime)+DNCT::GetChanglineString();
				//this->Static_textBox->Text += l_str;
				//l_str = "MonsterScale:"+((float)l_pGameNode->GetMonsterCount()/m_iFirstStageMonsterCount)+DNCT::GetChanglineString();
				//this->Static_textBox->Text += l_str;
				//l_str = "HurdleAndBreakScale:"+((float)l_pGameNode->GetHurdleAndBreakableCount()/m_iFirstStageHurdleAndBreakableCount)+DNCT::GetChanglineString();
				//this->Static_textBox->Text += l_str;
				//l_str = "PickScale:"+((float)l_pGameNode->GetPickupCount()/m_iFirstStagePickUpCount)+DNCT::GetChanglineString();
				//this->Static_textBox->Text += l_str;
				//l_str = "CoinScale:"+((float)l_pGameNode->m_iCoinCount/this->m_iFirstStageCoinCount)+DNCT::GetChanglineString();
				//this->Static_textBox->Text += l_str;
				//this->Static_textBox->Text += "=========================================";
				//this->Static_textBox->Text += DNCT::GetChanglineString();
			}
			m_pChartWithName->AddData(l_pTime);
			m_pChartWithName->AddData(l_pPickup);
			m_pChartWithName->AddData(l_pMonster);
			m_pChartWithName->AddData(l_pHurdlwAndBreakable);
			m_pChartWithName->AddData(l_pCoin);
			m_pChartWithName->AddData(l_pMonsterAndDamageObject);
			for(size_t i=0;i<m_pChartWithName->m_DataVector.size();++i)
			{
				RenderParameterIndex_listBox->Items->Add(DNCT::WcharToGcstring(m_pChartWithName->m_DataVector[i]->m_Name.c_str()));
			}
		}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 m_bWaitParseFile = true;
				 String^l_strFileName = nullptr;
				 if( FileName_textBox->Text->Length != 0 )
					 l_strFileName = FileName_textBox->Text;
				 Node_listBox->Items->Clear();
				 if( !System::IO::File::Exists(l_strFileName) )
				 {
					 String^l_strFileName2 =  System::IO::Directory::GetCurrentDirectory()+"/"+l_strFileName;
					 if( !System::IO::File::Exists(l_strFileName2) )
					 {
						l_strFileName = DNCT::OpenFileAndGetName();
					 }
					 else
					 {
						l_strFileName = l_strFileName2;
					 }
				 }
				 if( l_strFileName )
				 {
					 String^l_strTempFileName = System::IO::Directory::GetCurrentDirectory()+"/mytemp.xml";
					 if(!DNCT::FileToUnicode(l_strFileName,nullptr,l_strTempFileName))
					 {
						 WARNING_MSG("file not exists!");
						return;
					 }
					 if(m_pStageParser->ParseWithMyParse(DNCT::GcStringToChar(l_strTempFileName)))
					 {
						 for(size_t i=0;i<m_pStageParser->m_GamePlayVector.size();++i)
						 {
							sGameNode*l_pGameNode = &m_pStageParser->m_GamePlayVector[i];
							std::wstring l_strValue = m_pStageParser->m_GamePlayVector[i].getValue(L"gameplayName");
							String^l_strStageName = i+":"+DNCT::WcharToGcstring(l_strValue.c_str());
							float l_fTimeToCloseStage = l_pGameNode->GetFinishTime();
							l_strStageName += "         Time:";
							l_strStageName += l_fTimeToCloseStage;
							Node_listBox->Items->Add(l_strStageName);

							if( i == 1)
							{
								m_iFirstStageMonsterCount = l_pGameNode->GetMonsterCount();
								m_iFirstStageHurdleAndBreakableCount = l_pGameNode->GetHurdleAndBreakableCount();
								m_iFirstStagePickUpCount = l_pGameNode->GetPickupCount();
								m_iFirstStageCoinCount = l_pGameNode->m_iCoinCount;
								m_fFirstSgateTime = l_fTimeToCloseStage;
								//TutorialStatic_label->Text = "";
								//TutorialStatic_label->Text += "Time:";
								//TutorialStatic_label->Text += m_fFirstSgateTime+DNCT::GetChanglineString();
								//TutorialStatic_label->Text += "Monster:";
								//TutorialStatic_label->Text += m_iFirstStageMonsterCount+DNCT::GetChanglineString();
								//TutorialStatic_label->Text += "HurdleAndBreakable:";
								//TutorialStatic_label->Text += m_iFirstStageHurdleAndBreakableCount+DNCT::GetChanglineString();
								//TutorialStatic_label->Text += "Pickup:";
								//TutorialStatic_label->Text += m_iFirstStagePickUpCount+DNCT::GetChanglineString();
							}

						 }
						 System::IO::File::Delete(l_strTempFileName);
						 DumpStatic();
					 }

				 }
				 m_bWaitParseFile = false;
			 }
	private: System::Void Node_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 int	l_iSelectedIndex = Node_listBox->SelectedIndex;
				 if( l_iSelectedIndex == -1 )
					 return;
				 if((int)this->m_pStageParser->m_GamePlayVector.size() > l_iSelectedIndex)
				 {
					sGameNode *l_pGameNode = &this->m_pStageParser->m_GamePlayVector[l_iSelectedIndex];
					std::vector<std::wstring>l_NameVector;
					std::vector<int>l_CountVector;
					l_pGameNode->DumpyGroupTypeInfo(&l_NameVector,&l_CountVector);
					l_pGameNode->PlayerGo();
					NodeGroupAndCount_listBox->Items->Clear();
					for(size_t i=0;i<l_CountVector.size();++i)
					{
						String^l_str = DNCT::WcharToGcstring(l_NameVector[i].c_str());
						l_str += ":"+l_CountVector[i];
						//NodeGroupAndCount_listBox->Items->Add(l_str);
					}
					String^l_str;
					l_str = "Time:"+l_pGameNode->GetFinishTime();
					NodeGroupAndCount_listBox->Items->Add(l_str);
					l_str = "Pickup:"+l_pGameNode->GetPickupCount();
					NodeGroupAndCount_listBox->Items->Add(l_str);
					l_str = "Monster:"+l_pGameNode->GetMonsterCount();
					NodeGroupAndCount_listBox->Items->Add(l_str);
					l_str = "HurdlAndBreakable:"+l_pGameNode->GetHurdleAndBreakableCount();
					NodeGroupAndCount_listBox->Items->Add(l_str);
					l_str = "Coin:"+l_pGameNode->m_iCoinCount;
					NodeGroupAndCount_listBox->Items->Add(l_str);

					l_str = "TimeScale:"+(l_pGameNode->GetFinishTime()/m_fFirstSgateTime);
					NodeGroupAndCount_listBox->Items->Add(l_str);
					l_str = "MonsterScale:"+((float)l_pGameNode->GetMonsterCount()/m_iFirstStageMonsterCount);
					NodeGroupAndCount_listBox->Items->Add(l_str);
					l_str = "HurdleAndBreakScale:"+((float)l_pGameNode->GetHurdleAndBreakableCount()/m_iFirstStageHurdleAndBreakableCount);
					NodeGroupAndCount_listBox->Items->Add(l_str);
					l_str = "PickScale:"+((float)l_pGameNode->GetPickupCount()/m_iFirstStagePickUpCount);
					NodeGroupAndCount_listBox->Items->Add(l_str);
					l_str = "CoinScale:"+((float)l_pGameNode->m_iCoinCount/m_iFirstStagePickUpCount);
					NodeGroupAndCount_listBox->Items->Add(l_str);
				 }
			 }

private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			int	l_iSelectedIndex = Node_listBox->SelectedIndex;
			if( m_bWaitParseFile )
				return;
			//cGameApp::m_svGameResolution.x = (float)m_pTargetControl->Width;
			//cGameApp::m_svGameResolution.y = (float)m_pTargetControl->Height;
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
			glClearDepth(1.0f);
			glAlphaFunc(GL_GREATER,0.0001f);
			glShadeModel(GL_SMOOTH);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_ALPHA_TEST);
			Vector2	l_vViewPort(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height());
			if(m_pOrthogonalCamera)
			{
				//m_pOrthogonalCamera->CameraUpdateByMouse(cGameApp::m_sbMouseClickStatus[0],cGameApp::m_sbMouseClickStatus[1],cGameApp::m_sMouseWhellDelta,cGameApp::m_sScreenMousePosition.x,cGameApp::m_sScreenMousePosition.y,l_vViewPort);
				m_pOrthogonalCamera->Render();
				m_pOrthogonalCamera->DrawGrid(100,100,Vector4(0.05f,0,0,1));
			}
			if( RenderMode_listBox->SelectedIndex != 1 )
			{
				if( l_iSelectedIndex != -1 )
				{
					sGameNode* l_pGameNode = &this->m_pStageParser->m_GamePlayVector[l_iSelectedIndex];
					l_pGameNode->Update(cGameApp::m_sTimeAndFPS.fElpaseTime);
					if(l_pGameNode->MouseMove(m_pOrthogonalCamera->GetMouseWorldPos().x,m_pOrthogonalCamera->GetMouseWorldPos().y))
					{
	//					l_pGameNode->m_ShowHitPos.x = cGameApp::m_sMousePosition.x;
		//				l_pGameNode->m_ShowHitPos.y = cGameApp::m_sMousePosition.y;
					}
					l_pGameNode->RenderScene();
				}
			}
			else
			{
				m_pChartWithName->Render();
			}
			cGameApp::ShowInfo();
			SwapBuffers(m_HdcMV);
		 }
		System::Void MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			//this->Focus();
			if( !timer1->Enabled || !this->Visible )
				return;
			//make mouse wheel work
			m_pTargetControl->Focus();
			if( m_pGameApp )
				m_pGameApp->MouseDown(e->X,e->Y);
			POINT	ptCursor = {(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y};
			GCFORM::MouseButtons l_MouseButton = e->Button;
			m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
				,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)this->m_pTargetControl->Width,(float)this->m_pTargetControl->Height));
		}
		System::Void MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			if( !timer1->Enabled || !this->Visible )
				return;
			if( m_pGameApp )
				m_pGameApp->MouseMove(e->X,e->Y);
			POINT	ptCursor = {(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y};
			GCFORM::MouseButtons l_MouseButton = e->Button;
			m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
				,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)this->m_pTargetControl->Width,(float)this->m_pTargetControl->Height));
		}

		System::Void MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			if( !timer1->Enabled || !this->Visible )
				return;
			if( m_pGameApp )
				m_pGameApp->MouseUp(e->X,e->Y);
			POINT	ptCursor = {(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y};
			GCFORM::MouseButtons l_MouseButton = e->Button;
			m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
				,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)this->m_pTargetControl->Width,(float)this->m_pTargetControl->Height));
		}
private: System::Void RenderParameterIndex_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 this->m_pChartWithName->m_RenderIndex.clear();
			 for(int i=0;i<RenderParameterIndex_listBox->SelectedIndices->Count;++i)
			 {
				 this->m_pChartWithName->m_RenderIndex.push_back(RenderParameterIndex_listBox->SelectedIndices[i]);
			 }
		 }
private: System::Void ShowParameterValue_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 this->m_pChartWithName->m_bShowValue = ShowParameterValue_checkBox->Checked;
		 }
};
}