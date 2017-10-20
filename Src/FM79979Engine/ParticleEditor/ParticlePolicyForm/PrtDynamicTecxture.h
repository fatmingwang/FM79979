#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

extern cPrtTextureActDynamicTexture*g_pPrtTextureActDynamicTexture;
extern cPaticleManager*g_pPaticleManager;
namespace ParticlePolicyForm 
{

	/// <summary>
	/// Summary for DynamicTexture
	/// </summary>
	public ref class DynamicTexture : public System::Windows::Forms::UserControl
	{
	public:
		DynamicTexture(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~DynamicTexture()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListBox^  AnimationData_listBox;
	private: System::Windows::Forms::ListBox^  PI_listBox;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
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
			this->AnimationData_listBox = (gcnew System::Windows::Forms::ListBox());
			this->PI_listBox = (gcnew System::Windows::Forms::ListBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// AnimationData_listBox
			// 
			this->AnimationData_listBox->FormattingEnabled = true;
			this->AnimationData_listBox->ItemHeight = 12;
			this->AnimationData_listBox->Location = System::Drawing::Point(129, 19);
			this->AnimationData_listBox->Name = L"AnimationData_listBox";
			this->AnimationData_listBox->Size = System::Drawing::Size(120, 100);
			this->AnimationData_listBox->TabIndex = 0;
			this->AnimationData_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &DynamicTexture::AnimationData_listBox_SelectedIndexChanged);
			// 
			// PI_listBox
			// 
			this->PI_listBox->FormattingEnabled = true;
			this->PI_listBox->ItemHeight = 12;
			this->PI_listBox->Location = System::Drawing::Point(3, 19);
			this->PI_listBox->Name = L"PI_listBox";
			this->PI_listBox->Size = System::Drawing::Size(120, 100);
			this->PI_listBox->TabIndex = 1;
			this->PI_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &DynamicTexture::PI_listBox_SelectedIndexChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(32, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"PIFile";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(131, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(75, 12);
			this->label2->TabIndex = 3;
			this->label2->Text = L"AnimationData";
			// 
			// DynamicTexture
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->PI_listBox);
			this->Controls->Add(this->AnimationData_listBox);
			this->Name = L"DynamicTexture";
			this->Size = System::Drawing::Size(259, 128);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	public: System::Void RefreshPI()
			 {
				 PI_listBox->Items->Clear();
				 AnimationData_listBox->Items->Clear();
				if( g_pPaticleManager )
				{
					if( g_pPaticleManager->GetImageParser() )
					{
						int	l_iCount = g_pPaticleManager->GetImageParser()->Count();
						bool    l_bContainPI = false;
						for( int i=0;i<l_iCount;++i )
						{
							cBaseImage*l_pBaseImage = dynamic_cast<cBaseImage*>(g_pPaticleManager->GetImageParser()->GetObject(i));
							if( l_pBaseImage->Type() == cPuzzleImage::TypeID )
							{
								cPuzzleImage*l_pPI = dynamic_cast<cPuzzleImage*>(l_pBaseImage);
								PI_listBox->Items->Add(DNCT::WcharToGcstring(l_pPI->GetName()));
								l_bContainPI = true;
							}
						}
						if( l_bContainPI )
						{
						    if( g_pPrtTextureActDynamicTexture )
							{
								cPrtEmitter*l_pPrtEmitter = g_pPrtTextureActDynamicTexture->GetEmitterOwner();
						        PI_listBox->SelectedIndex = 0;
							}
						}
						else
						{
							if( g_pPrtTextureActDynamicTexture )
								g_pPrtTextureActDynamicTexture->m_pImageIndexOfAnimation = 0;
						}
					}
				}
			 }
		private: System::Void PI_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 AnimationData_listBox->Items->Clear();
			 cPuzzleImage*l_pPI = 0;
			 if( PI_listBox->SelectedIndex != -1 )
			 {
				 if( g_pPrtTextureActDynamicTexture->GetEmitterOwner()&&g_pPrtTextureActDynamicTexture->GetEmitterOwner()->GetBaseImage() )
				 {
					 if( g_pPrtTextureActDynamicTexture->GetEmitterOwner()->GetBaseImage()->Type() != cPuzzleImage::TypeID )
					 {
						WARNING_MSG("please select pi as texture");
						return;
					 }
					 l_pPI = dynamic_cast<cPuzzleImage*>(g_pPaticleManager->GetImageParser()->GetObject(DNCT::GcStringToWchar(PI_listBox->SelectedItem->ToString())));
					 if( !l_pPI )
					 {
						 RefreshPI();
						return;
					 }
					 g_pPrtTextureActDynamicTexture->SetPuzzleImage(l_pPI);
					 cNamedTypedObjectVector<cImageIndexOfAnimation>*l_pImageIndexOfAnimationList = l_pPI->GetImageIndexOfAnimationList();
					 if( l_pImageIndexOfAnimationList )
					 {
						int	l_iCount = l_pImageIndexOfAnimationList->Count();
						for( int i=0;i<l_iCount;++i )
						{
							AnimationData_listBox->Items->Add(DNCT::WcharToGcstring(l_pImageIndexOfAnimationList->GetObject(i)->GetName()));
						}
					 }
				}
				else
				{
					WARNING_MSG("please select pi as texture!");
				}
			}
		 }
private: System::Void AnimationData_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			if(g_pPrtTextureActDynamicTexture)
			{
				if( AnimationData_listBox->SelectedIndex != -1 )
				{
					cPuzzleImage*l_pPI = dynamic_cast<cPuzzleImage*>(g_pPaticleManager->GetImageParser()->GetObject(DNCT::GcStringToWchar(PI_listBox->SelectedItem->ToString())));
					cNamedTypedObjectVector<cImageIndexOfAnimation>*l_pImageIndexOfAnimationList = l_pPI->GetImageIndexOfAnimationList();
					g_pPrtTextureActDynamicTexture->m_pImageIndexOfAnimation = l_pImageIndexOfAnimationList->GetObject(AnimationData_listBox->SelectedIndex);
				}
			}
		 }
};
}
