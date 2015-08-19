#pragma once
#include "MPDINodeData.h"
namespace MPDINodeForm
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MPDINodeEditor
	/// </summary>
	public ref class MPDINodeEditor : public System::Windows::Forms::Form
	{
	public:
		MPDINodeEditor(void);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MPDINodeEditor();
	private: System::ComponentModel::IContainer^  components;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		//my
		cOrthogonalCamera*							m_pOrthgonalCamera;
		TextureEditorWPF::MPDI::MPDINodeForm^		m_pMPDINodeForm;
		MPDINodeData^								m_pMPDINodeData;
		System::Windows::Controls::Border^			m_pRenderBorder;
		cGameApp*									m_pGameApp;
		cMPDINode*									m_pMPDIRootNode;
		private: System::Windows::Forms::Timer^		timer1;
		System::Void								SeetingChanged(System::Object^  sender, System::EventArgs^  e);
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
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
			this->timer1->Tick += gcnew System::EventHandler(this, &MPDINodeEditor::timer1_Tick);
			// 
			// MPDINodeEditor
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(836, 531);
			this->Name = L"MPDINodeEditor";
			this->Text = L"MPDINodeEditor";
			this->Load += gcnew System::EventHandler(this, &MPDINodeEditor::MPDINodeEditor_Load);
			this->SizeChanged += gcnew System::EventHandler(this, &MPDINodeEditor::MPDINodeEditor_SizeChanged);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void KeyDown(System::Object^  sender, System::Windows::Input::KeyEventArgs^  e);
	private: System::Void KeyUp(System::Object^  sender,  System::Windows::Input::KeyEventArgs^  e);
	private: System::Void MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void CameraButtonClick(System::Object^  sender, System::EventArgs^  e);
	private: System::Void AddNewMPDINode(System::String^  e_strType, System::Windows::Controls::TreeViewItem^e_pTreeViewItem);
	private: System::Void MPDINodePaste(System::Object^  sender, System::EventArgs^  e);
	private: System::Void MPDINodeDelete(System::Object^  sender, System::EventArgs^  e);
	private: System::Void DropTreeViewItemToAnotherTreeViewItem(System::Windows::Controls::TreeViewItem^e_pDragObject, System::Windows::Controls::TreeViewItem^e_pDropObject);

	private: System::Void ChangeSelectedItem(System::Object^  sender, System::EventArgs^  e);
			 
	//end class MPDINodeEditor
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e);
	private: System::Void MPDINodeEditor_Load(System::Object^  sender, System::EventArgs^  e);
	private: System::Void CameraAdjustPara();
	private: System::Void MPDINodeEditor_SizeChanged(System::Object^  sender, System::EventArgs^  e);
	};
//end namespace MPDINodeForm
}