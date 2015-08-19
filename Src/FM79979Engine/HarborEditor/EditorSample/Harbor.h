#pragma once

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//ensure preprocessor definiation DEBUG not _DEBUG or it will occur memory problem.
//I donno why ask M$.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#include "LinerDataModifer.h"
#include "RegionMapInfo.h"
#include "TradeRoutes.h"
namespace EditorSample
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Windows::Forms::Integration;

	/// <summary>
	/// Summary for HarborEditor
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class HarborEditor : public System::Windows::Forms::Form
	{
	public:
		HarborEditor(void);
		~HarborEditor();
	//
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Timer^  timer1;
		//my
		WindowsFormsHost^						m_pWindowsFormsHost;
		cGameApp*								m_pGameApp;
		HarborUI::MainUI^						m_pWPFMainUI;
		HarborUI::HarborInfo^					m_pWPFHarborInfo;
		HarborUI::TradeRoutes^					m_pWPFTradeRoutes;
		cOrthogonalCamera*						m_pOrthgonalCamera;
		cBaseImage*								m_pBGImage;
		//
		cCommodityManager*						m_pCommodityManager;
		cLinerDataModifer*						m_pLinerDataModifer;
		cRegionMapPointInfoManager*				m_pRegionMapPointInfoManager;
		cTradeRoutesManager*					m_pTradeRoutesManager;
		cLinerDataProcessor<Vector3>*			m_pCurrentEditRoute;
		//
		cMapPointInfo*							GetCurrentSelectMapPointInfo();
		cTradeRoutes*							GetCurrentSelectTradeRoutes();
		System::Void							ResolutionAdjust();
		bool									IsRoutePointNameLegal();
		void									OpenFile(String^e_strDirectoryFile);
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
			this->timer1->Tick += gcnew System::EventHandler(this, &HarborEditor::timer1_Tick);
			// 
			// HarborEditor
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 262);
			this->Name = L"HarborEditor";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"HarborEditor";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Load += gcnew System::EventHandler(this, &HarborEditor::HarborEditor_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
		System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e);
		System::Void MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
		System::Void MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
		System::Void MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
		System::Void HarborEditor_Load(System::Object^  sender, System::EventArgs^  e);
		//
		System::Void BGImage(System::Object^  sender, System::EventArgs^  e);
		//
		System::Void AddMpaPoint(System::Object^  sender, System::EventArgs^  e);
		System::Void DelMpaPoint(System::Object^  sender, System::EventArgs^  e);
		System::Void HorborListboxSelectionChanged(System::Object^  sender, System::EventArgs^  e);
		System::Void MapPointTypeChange(System::Object^  sender, System::EventArgs^  e);
		System::Void AppllyMapPointHorbor(System::Object^  sender, System::EventArgs^  e);
		System::Void AppllyMapPointRoutePoint(System::Object^  sender, System::EventArgs^  e);
		//
		System::Void MapDataTradeRouteDataChanged(System::Object^  sender, System::EventArgs^  e);
		System::Void RouteNameCheckEvent(System::Object^  sender, System::EventArgs^  e);
		System::Void AddTradeRoute(System::Object^  sender, System::EventArgs^  e);
		System::Void DelTradeRoute(System::Object^  sender, System::EventArgs^  e);
		System::Void TradeRouteSelectionChanged(System::Object^  sender, System::EventArgs^  e);
		//
		System::Void FileOpen(System::Object^  sender, System::EventArgs^  e);
		System::Void FileSave(System::Object^  sender, System::EventArgs^  e);
		//
};
//end EditorSample
}