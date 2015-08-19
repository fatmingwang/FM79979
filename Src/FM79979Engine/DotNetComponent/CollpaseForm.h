#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::ComponentModel::Design;

namespace DotNetComponent {

	/// <summary>
	/// CollpaseForm 的摘要
	/// </summary>
	//[Designer("System.WindowsForms.Design.ControlDesigner, System.Design", IDesigner::typeid)]
	[Designer("System.Windows.Forms.Design.ParentControlDesigner, System.Design", IDesigner::typeid)] 
	public ref class CollpaseForm : public System::Windows::Forms::UserControl
	{
	public:
		CollpaseForm(void)
		{
			InitializeComponent();
			//
			//TODO: 在此加入建構函式程式碼
			//
			//m_TargetSize = this->Size;
			m_Collpase = true;
			m_Extend = false;
			m_fScaleScalar = 0.f;
			timer1->Enabled = false;
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~CollpaseForm()
		{
			if (components)
			{
				delete components;
			}
		}
	public:[TypeConverterAttribute(String::typeid),DescriptionAttribute("CollpaseButtonTextDescriptionAttribute")]
		   [CategoryAttribute("My")]
		   property String^ CollpaseButtonName
		   {
				String^ get(){ return Collpase_button->Text; }
				System::Void set(String^ value){ Collpase_button->Text = value; }
		   }
	//PUBLIC_OBJECT_CATE_AND_CONVERT(System::Windows::Forms::Button^,Collpase_button,MyCollpaseButton,"My",System::Windows::Forms::Button,"Fatming qoo");
	private: System::Windows::Forms::Button^  Collpase_button;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;
    //my
	public:	 void	DoCollpease(){ timer1->Enabled = true; }
	private:	 System::Drawing::Size	m_TargetSize;
	private:	 bool  m_Extend;
	private:	 bool  m_Collpase;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private:	 float m_fScaleScalar;
	//public: [DesignerSerializationVisibility(DesignerSerializationVisibility::Visible)]System::Windows::Forms::GroupBox^  groupBox1;
	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改這個方法的內容。
		///
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->Collpase_button = (gcnew System::Windows::Forms::Button());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->SuspendLayout();
			// 
			// Collpase_button
			// 
			this->Collpase_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->Collpase_button->Dock = System::Windows::Forms::DockStyle::Top;
			this->Collpase_button->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->Collpase_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Collpase_button->Location = System::Drawing::Point(0, 0);
			this->Collpase_button->Margin = System::Windows::Forms::Padding(0);
			this->Collpase_button->Name = L"Collpase_button";
			this->Collpase_button->Size = System::Drawing::Size(285, 20);
			this->Collpase_button->TabIndex = 3;
			this->Collpase_button->Text = L"Test";
			this->Collpase_button->UseVisualStyleBackColor = false;
			this->Collpase_button->Click += gcnew System::EventHandler(this, &CollpaseForm::Collpase_button_Click);
			// 
			// timer1
			// 
			this->timer1->Interval = 10;
			this->timer1->Tick += gcnew System::EventHandler(this, &CollpaseForm::timer1_Tick);
			// 
			// groupBox1
			// 
			this->groupBox1->Dock = System::Windows::Forms::DockStyle::Left;
			this->groupBox1->Location = System::Drawing::Point(0, 20);
			this->groupBox1->Margin = System::Windows::Forms::Padding(0);
			this->groupBox1->MaximumSize = System::Drawing::Size(0, 9999);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(0, 214);
			this->groupBox1->TabIndex = 4;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"groupBox1";
			// 
			// CollpaseForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(144)), static_cast<System::Int32>(static_cast<System::Byte>(144)), 
				static_cast<System::Int32>(static_cast<System::Byte>(144)));
			this->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->Collpase_button);
			this->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"CollpaseForm";
			this->Size = System::Drawing::Size(285, 234);
			this->Load += gcnew System::EventHandler(this, &CollpaseForm::CollpaseForm_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void Collpase_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if(  m_TargetSize+System::Drawing::Size(groupBox1->Location.X,groupBox1->Location.Y) ==  this->Size )
				 {
					 m_Collpase = true;
					 m_Extend = false;
				 }
				 else
				 {
					 m_Collpase = false;
					 m_Extend = true;
				 }
				 timer1->Enabled = true;
				 m_fScaleScalar = 0.f;
			 }
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
			 {
				 m_fScaleScalar+=0.1f;
				 if( m_Collpase )
				 {
					Size = System::Drawing::Size(Size.Width,(int)(Size.Height-(15*m_fScaleScalar)));
					if( Size.Height<=Collpase_button->Height )
					{
						this->Size = System::Drawing::Size(Size.Width,Collpase_button->Size.Height);
						timer1->Enabled = false;
						m_fScaleScalar = 0.f;
						m_Collpase = false;
					}
				 }
				 else
				 if(m_Extend)
				 {
					Size = System::Drawing::Size(Size.Width,(int)(Size.Height+15*m_fScaleScalar));
					if( Size.Height>=m_TargetSize.Height )
					{
						this->Size = m_TargetSize+System::Drawing::Size(groupBox1->Location.X,groupBox1->Location.Y);
						timer1->Enabled = false;
						m_fScaleScalar = 0.f;
						m_Extend = false;
					}
				 }
			 }
	public:	 System::Void	SetupChild(System::Windows::Forms::Control^e_pTargetControl,
				 String^e_strFormName,System::Windows::Forms::Control::ControlCollection^e_pNewOwner,
				 System::Drawing::Size e_Size)
			 {
				System::Drawing::Point	l_Location;
				m_Collpase = true;
				m_Extend = false;
				m_fScaleScalar = 0.f;
				timer1->Enabled = true;
				m_TargetSize = e_Size;
				this->Size = m_TargetSize;
				Collpase_button->Text = e_strFormName;
				e_pNewOwner->Add(this);
				this->groupBox1->Controls->Add(e_pTargetControl);
				e_pTargetControl->Location = System::Drawing::Point(0,0);

			 }
//private: System::Void groupBox1_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
//		 {
//			 //System::ComponentModel::design
//			//if (System::ComponentModel::DesignerProperties ::GetIsInDesignMode(this))
//			{
//				
//			}
//		  }
//private: System::Void groupBox1_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
//		 {
//			//if (System::ComponentModel::DesignerProperties::GetIsInDesignMode(this))
//			{
//				// code...
//			}
//		 }
private: System::Void CollpaseForm_Load(System::Object^  sender, System::EventArgs^  e)
		 {
			m_TargetSize = this->Size;
			//timer1->Enabled = true;
			Collpase_button_Click(this,e);
		 }
};
}
