// EditorSample.cpp : main project file.

#include "stdafx.h"
#include "Harbor.h"
#include "TradeRouteTimeCalculate.h"
#include "vld.h"

using namespace System::Data;
char*g_strSelectDirectory = nullptr;
namespace EditorSample 
{
	HarborEditor::HarborEditor()
	{
//	using namespace System::Windows::Forms::Integration;
	//using namespace Microsoft::Office::Tools::Excel;
		//==================================
		//read xls or xlsm file
		//==================================
		//Microsoft::Office::Interop::Excel::Application^ exApp= gcnew Microsoft::Office::Interop::Excel::ApplicationClass();
  //      String^ filename="e:\\test.xlsm";
		//Microsoft::Office::Interop::Excel::Workbook^ wb = exApp->Workbooks->Open(filename, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing, Type::Missing);
  //      Microsoft::Office::Interop::Excel::Worksheet^  exWs  = (Microsoft::Office::Interop::Excel::Worksheet^)exApp->ActiveSheet;
  //      int row=1;
  //      int col=1;
  //      String^ tmp=((Microsoft::Office::Interop::Excel::Range^)exWs->Cells[(System::Object^)row, (System::Object^)col])->Value2->ToString();
  //      MessageBox::Show(tmp);
		//======================
		//
		//======================
		//HarborUI::MyExcel::ReadExcelTest();
		//HarborUI::MyExcel::ExcelTest();
		//ExcelLibrary
		InitializeComponent();
		m_pOrthgonalCamera = nullptr;
		m_pGameApp = nullptr;
		m_pWPFTradeRoutes = nullptr;
		m_pWPFHarborInfo = nullptr;
		m_pBGImage = nullptr;
		//m_pCommodityManager = nullptr;
		m_pLinerDataModifer = new cLinerDataModifer();
		m_pRegionMapPointInfoManager = new cRegionMapPointInfoManager();
		m_pTradeRoutesManager = new cTradeRoutesManager();
		m_pCurrentEditRoute = new cLinerDataProcessor<Vector3>();
		m_pLinerDataModifer->m_pSrcData = m_pCurrentEditRoute;
	}

	HarborEditor::~HarborEditor()
	{
		if (components)
		{
			delete components;
		}
		SAFE_DELETE(m_pCurrentEditRoute);
		//SAFE_DELETE(m_pCommodityManager);
		SAFE_DELETE(m_pLinerDataModifer);
		SAFE_DELETE(m_pRegionMapPointInfoManager);
		SAFE_DELETE(m_pTradeRoutesManager);
		SAFE_DELETE(m_pBGImage);
		m_pOrthgonalCamera->SetName(L"79979");
		SAFE_DELETE(m_pOrthgonalCamera);
		SAFE_DELETE(m_pGameApp);
	}


	cTradeRoutes*	HarborEditor::GetCurrentSelectTradeRoutes()
	{
		if( this->m_pWPFTradeRoutes != nullptr && m_pTradeRoutesManager != nullptr )
		{
			return m_pTradeRoutesManager->GetObject(m_pWPFTradeRoutes->m_RouteListBox->SelectedIndex);
		}
		return nullptr;
	}

	cMapPointInfo*	HarborEditor::GetCurrentSelectMapPointInfo()
	{
		if( m_pWPFHarborInfo && m_pRegionMapPointInfoManager )
		{
			int	l_iSelectedIndex = m_pWPFHarborInfo->m_HarborListbox->m_ListBox->SelectedIndex;
			if( l_iSelectedIndex != -1 )
			{
				return m_pRegionMapPointInfoManager->GetObject(l_iSelectedIndex);
			}
		}
		return nullptr;
	}
	System::Void HarborEditor::HarborEditor_Load(System::Object^  sender, System::EventArgs^  e)
	{
		m_pWPFMainUI = gcnew HarborUI::MainUI();
		m_pWPFTradeRoutes = m_pWPFMainUI->m_TradeRoutes;
		m_pWPFHarborInfo = m_pWPFMainUI->m_HarborInfo;

		System::Windows::Forms::Integration::ElementHost^l_pElementHots = gcnew System::Windows::Forms::Integration::ElementHost;
		l_pElementHots->Dock = DockStyle::Fill;
		l_pElementHots->Child = m_pWPFMainUI;
		Controls->Add(l_pElementHots);

		//for opengl
		System::Windows::Forms::Integration::WindowsFormsHost^l_pWindowsFormsHost = (System::Windows::Forms::Integration::WindowsFormsHost^)m_pWPFMainUI->m_RenderGrid->Child;
		m_pWindowsFormsHost = l_pWindowsFormsHost;
		m_pGameApp = new cGameApp((HWND)l_pWindowsFormsHost->Child->Handle.ToPointer());
		m_pGameApp->Init();
		//for mouse event
		l_pWindowsFormsHost->Child->MouseDown							+=	gcnew System::Windows::Forms::MouseEventHandler(this, &HarborEditor::MyMouseDown);
		l_pWindowsFormsHost->Child->MouseMove							+=	gcnew System::Windows::Forms::MouseEventHandler(this, &HarborEditor::MyMouseMove);
		l_pWindowsFormsHost->Child->MouseUp								+=	gcnew System::Windows::Forms::MouseEventHandler(this, &HarborEditor::MyMouseUp);
		l_pWindowsFormsHost->Child->MouseWheel							+=	gcnew System::Windows::Forms::MouseEventHandler(this, &HarborEditor::MyMouseUp);
		//
		m_pWPFMainUI->f_FileOpen										+=	gcnew System::EventHandler(this, &HarborEditor::FileOpen);
		m_pWPFMainUI->f_FileSave										+=	gcnew System::EventHandler(this, &HarborEditor::FileSave);

		m_pWPFMainUI->f_ExcelConvert									+=	gcnew System::EventHandler(this, &HarborEditor::FileExcelConvertSave);
		
		//
		m_pWPFHarborInfo->f_BGImageButton								+= gcnew System::EventHandler(this, &HarborEditor::BGImage);
		m_pWPFHarborInfo->f_AddButton									+= gcnew System::EventHandler(this, &HarborEditor::AddMpaPoint);
		//
		m_pWPFHarborInfo->m_MapPointHarbor->f_Apply						+= gcnew System::EventHandler(this, &HarborEditor::AppllyMapPointHorbor);
		m_pWPFHarborInfo->m_MapPointRoutePoint->f_Apply					+= gcnew System::EventHandler(this, &HarborEditor::AppllyMapPointRoutePoint);
		m_pWPFHarborInfo->f_MapPointTypeChange							+= gcnew System::EventHandler(this, &HarborEditor::MapPointTypeChange);
		//
		m_pWPFHarborInfo->m_HarborListbox->f_ListBoxSelectionChanged	+= gcnew System::EventHandler(this, &HarborEditor::HorborListboxSelectionChanged);
		m_pWPFHarborInfo->m_HarborListbox->f_DeleteSelectedItems		+= gcnew System::EventHandler(this, &HarborEditor::DelMpaPoint);
		//
		m_pWPFTradeRoutes->f_ListboxValueChanged						+= gcnew System::EventHandler(this, &HarborEditor::TradeRouteSelectionChanged);
		m_pWPFTradeRoutes->m_InsertPointCount->f_ValueChanged			+= gcnew System::EventHandler(this, &HarborEditor::MapDataTradeRouteDataChanged);
		m_pWPFTradeRoutes->m_LOD->f_ValueChanged						+= gcnew System::EventHandler(this, &HarborEditor::MapDataTradeRouteDataChanged);
		m_pWPFTradeRoutes->m_SEEffect->f_ValueChanged					+= gcnew System::EventHandler(this, &HarborEditor::MapDataTradeRouteDataChanged);
		m_pWPFTradeRoutes->m_ESEffect->f_ValueChanged					+= gcnew System::EventHandler(this, &HarborEditor::MapDataTradeRouteDataChanged);


		m_pWPFTradeRoutes->f_PointDropEvent								+= gcnew System::EventHandler(this, &HarborEditor::RouteNameCheckEvent);
		m_pWPFTradeRoutes->f_Add										+= gcnew System::EventHandler(this, &HarborEditor::AddTradeRoute);
		m_pWPFTradeRoutes->f_Delete										+= gcnew System::EventHandler(this, &HarborEditor::DelTradeRoute);
		

		m_pWPFMainUI->SetGoodSize();
		//
		m_pOrthgonalCamera = new cOrthogonalCamera();

		//OpenFile("C:/Users/USER/Desktop/New folder (2)/");
		this->timer1->Enabled = true;
	}

	System::Void HarborEditor::timer1_Tick(System::Object^  sender, System::EventArgs^  e)
	{
		cGameApp::m_spGlyphFontRender->SetScale(0.5f);
		cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(0,1,0,1));
		ResolutionAdjust();
		wglMakeCurrent( m_pGameApp->m_sHdc,m_pGameApp->m_sHGLRC );
		UseShaderProgram();
		this->m_pGameApp->Run();
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		glClearColor(0.3f,0.3f,0.3f,1);
		glClearDepth(1.0f);
		glAlphaFunc(GL_GREATER,0.0001f);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_ALPHA_TEST);
		if( m_pOrthgonalCamera )
		{
			m_pOrthgonalCamera->Render();
			//if(EditorCommonVariables::m_sbShowGrid)
			{
				m_pOrthgonalCamera->DrawGrid();
			}
		}
		if( m_pBGImage )
			m_pBGImage->Render();
		m_pRegionMapPointInfoManager->m_bRenderDebugInfo = (bool)this->m_pWPFHarborInfo->m_ShowHarborInfo->IsChecked;
		m_pRegionMapPointInfoManager->m_bAllHarborMove = (bool)this->m_pWPFHarborInfo->m_AllHarborMove->IsChecked;
		if( m_pRegionMapPointInfoManager )
			m_pRegionMapPointInfoManager->DebugRender();
		GLRender::RenderRectangle(1024,576,cMatrix44::Identity,Vector4::Red);

		if( m_pCurrentEditRoute && m_pWPFMainUI->m_Tabcontrol->SelectedIndex == 1 )
		{
			cGameApp::m_spGlyphFontRender->SetScale(0.3f);
			cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(0,1,1,1));
			m_pCurrentEditRoute->DebugRender(true,true,Vector4::Blue);
			for each(String^l_StrSelectedItem in this->m_pWPFTradeRoutes->m_RouteListBox->SelectedItems)
			{
				WCHAR*l_strName = DNCT::GcStringToWchar(l_StrSelectedItem);
				cTradeRoutes*l_pTradeRoutes = m_pTradeRoutesManager->GetObject(l_strName);
				l_pTradeRoutes->GetRoute()->DebugRender(true,true,Vector4::Red);
			}
			//std::vector<Vector3>*l_pLinerData = m_pCurrentEditRoute->GetLinerDataVector();
			//if( l_pLinerData->size() > 2 )
			//{
			//	GLRender::RenderPoints(&(*l_pLinerData)[0],l_pLinerData->size());
			//}
		}
		cGameApp::m_spGlyphFontRender->SetScale(0.5f);
		cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(0,1,0,1));
		if( m_pTradeRoutesManager && (bool)this->m_pWPFTradeRoutes->m_ShowAllRoutesCheckbox->IsChecked == true )
		{
			m_pTradeRoutesManager->DebugRender();
		}
		glEnable2D(1024,576);
		if( m_pOrthgonalCamera )
		{
			POINT	ptCursor = {(int)m_pOrthgonalCamera->GetMouseWorldPos().x,(int)m_pOrthgonalCamera->GetMouseWorldPos().y};
			WCHAR*l_strMousePos;
			l_strMousePos = DNCT::GcStringToWchar(ptCursor.x.ToString()+","+ptCursor.y.ToString());
			cGameApp::RenderFont(0,0,l_strMousePos);
		}

		SwapBuffers(m_pGameApp->m_sHdc);
	}

	System::Void HarborEditor::MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		//this->Focus();
		if( !timer1->Enabled || !this->Visible )
			return;
		if( m_pOrthgonalCamera )
		{
			System::Windows::Forms::Integration::WindowsFormsHost^l_pWindowsFormsHost = m_pWindowsFormsHost;
			System::Windows::Forms::Panel^l_pPanel = (System::Windows::Forms::Panel^)l_pWindowsFormsHost->Child;
			if(!l_pPanel->Focused)
				l_pPanel->Focus();
			GCFORM::MouseButtons l_MouseButton = e->Button;
			m_pOrthgonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
				,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height()));

			POINT	ptCursor = {(int)m_pOrthgonalCamera->GetMouseWorldPos().x,(int)m_pOrthgonalCamera->GetMouseWorldPos().y};
			if( m_pRegionMapPointInfoManager&& this->m_pWPFMainUI->m_Tabcontrol->SelectedIndex == 0 )
			{
				if((bool)this->m_pWPFHarborInfo->m_AllHarborMove->IsChecked)
				{
					int	l_iCount = this->m_pWPFHarborInfo->m_HarborListbox->m_ListBox->SelectedItems->Count;
					m_pRegionMapPointInfoManager->m_SelectedIndices.clear();
					for (int i = 0; i < l_iCount; i++)
					{
						m_pRegionMapPointInfoManager->m_SelectedIndices.push_back(
							m_pWPFHarborInfo->m_HarborListbox->m_ListBox->Items->IndexOf(m_pWPFHarborInfo->m_HarborListbox->m_ListBox->SelectedItems[i])
						);
					}
				}
				m_pRegionMapPointInfoManager->EditorMouseDown(ptCursor.x,ptCursor.y);
				if(m_pRegionMapPointInfoManager->GetCurrentSelectedHarborInfo())
				{
					if( m_pWPFHarborInfo )
						m_pWPFHarborInfo->m_HarborPos->Text = ptCursor.x.ToString()+","+ptCursor.y.ToString();
				}
			}
			else
			if( m_pLinerDataModifer&& this->m_pWPFMainUI->m_Tabcontrol->SelectedIndex == 1 )
			{
				m_pLinerDataModifer->MouseDown(Vector3((float)ptCursor.x,(float)ptCursor.y,0.f));
			}
		}
	}
	System::Void HarborEditor::MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if( !timer1->Enabled || !this->Visible )
			return;
		if( m_pOrthgonalCamera )
		{
			System::Windows::Forms::Integration::WindowsFormsHost^l_pWindowsFormsHost = m_pWindowsFormsHost;
			System::Windows::Forms::Panel^l_pPanel = (System::Windows::Forms::Panel^)l_pWindowsFormsHost->Child;
			if(!l_pPanel->Focused)
				l_pPanel->Focus();
			GCFORM::MouseButtons l_MouseButton = e->Button;
			m_pOrthgonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
				,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height()));
			POINT	ptCursor = {(int)m_pOrthgonalCamera->GetMouseWorldPos().x,(int)m_pOrthgonalCamera->GetMouseWorldPos().y};
			if( m_pRegionMapPointInfoManager && this->m_pWPFMainUI->m_Tabcontrol->SelectedIndex == 0 )
			{
				m_pRegionMapPointInfoManager->EditorMouseMove(ptCursor.x,ptCursor.y,l_MouseButton == System::Windows::Forms::MouseButtons::Left?true:false);
				if(m_pRegionMapPointInfoManager->GetCurrentSelectedHarborInfo())
				{
					if( m_pWPFHarborInfo )
						m_pWPFHarborInfo->m_HarborPos->Text = ptCursor.x.ToString()+","+ptCursor.y.ToString();
				}
			}
			else
			if( m_pLinerDataModifer && this->m_pWPFMainUI->m_Tabcontrol->SelectedIndex == 1 )
			{
				m_pLinerDataModifer->MouseMove(Vector3((float)ptCursor.x,(float)ptCursor.y,0.f));
				m_pRegionMapPointInfoManager->EditorMouseMove(ptCursor.x,ptCursor.y,false);
			}
		}
	}

	System::Void HarborEditor::MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if( !timer1->Enabled || !this->Visible )
			return;
		if( m_pOrthgonalCamera )
		{
			System::Windows::Forms::Integration::WindowsFormsHost^l_pWindowsFormsHost = m_pWindowsFormsHost;
			System::Windows::Forms::Panel^l_pPanel = (System::Windows::Forms::Panel^)l_pWindowsFormsHost->Child;
			if(!l_pPanel->Focused)
				l_pPanel->Focus();
			GCFORM::MouseButtons l_MouseButton = e->Button;
			m_pOrthgonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
				,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height()));
			POINT	ptCursor = {(int)m_pOrthgonalCamera->GetMouseWorldPos().x,(int)m_pOrthgonalCamera->GetMouseWorldPos().y};
			if( m_pRegionMapPointInfoManager&& this->m_pWPFMainUI->m_Tabcontrol->SelectedIndex == 0 )
				m_pRegionMapPointInfoManager->EditorMouseUp(ptCursor.x,ptCursor.y);
			else
			if( m_pLinerDataModifer&& this->m_pWPFMainUI->m_Tabcontrol->SelectedIndex == 1 )
			{
				m_pLinerDataModifer->MouseUp(Vector3((float)ptCursor.x,(float)ptCursor.y,0.f));
			}
		}
	}

	System::Void HarborEditor::ResolutionAdjust()
	{
		RECT rcClient;
		GetClientRect((HWND)m_pWindowsFormsHost->Child->Handle.ToPointer(), &rcClient);
		cGameApp::m_svViewPortSize.x = 0.f;
		cGameApp::m_svViewPortSize.y = 0.f;
		cGameApp::m_svViewPortSize.z = (float)m_pWindowsFormsHost->Child->Width;
		cGameApp::m_svViewPortSize.w = (float)rcClient.bottom;
		Vector2	l_vCameraResolution;
		l_vCameraResolution.x = cGameApp::m_svViewPortSize.z;
		l_vCameraResolution.y = cGameApp::m_svViewPortSize.w;
		if( m_pOrthgonalCamera )
			m_pOrthgonalCamera->SetResolution(l_vCameraResolution);
	}

	void	HarborEditor::OpenFile(String^e_strDirectoryFile)
	{
		String^l_strDirectory = e_strDirectoryFile;
		if( l_strDirectory )
		{
			String^l_strCoomodityFileName = l_strDirectory+"/Commodity.xml";
			String^l_strHarborInfoFileName = l_strDirectory+"/HarborInfo.xml";
			String^l_strRouteFileName = l_strDirectory+"/Route.xml";
			//SAFE_DELETE(m_pCommodityManager);
			SAFE_DELETE(m_pRegionMapPointInfoManager);
			SAFE_DELETE(m_pTradeRoutesManager);
			//m_pCommodityManager = new cCommodityManager();
			m_pRegionMapPointInfoManager = new cRegionMapPointInfoManager();
			m_pTradeRoutesManager = new cTradeRoutesManager();
			m_pCurrentEditRoute->Clear();
			/*if(!m_pCommodityManager->ParseWithMyParse(DNCT::GcStringToChar(l_strCoomodityFileName)))
			{
				WARNING_MSG(l_strCoomodityFileName + "parse failed");
			}*/
			if(!m_pRegionMapPointInfoManager->ParseWithMyParse(DNCT::GcStringToChar(l_strHarborInfoFileName)))
			{
				WARNING_MSG(l_strHarborInfoFileName + "parse failed");
			}
			else
			{
				m_pWPFHarborInfo->m_HarborListbox->m_ListBox->Items->Clear();
				int	l_iCount = m_pRegionMapPointInfoManager->Count();
				for( int i=0;i<l_iCount;++i )
				{
					m_pWPFHarborInfo->m_HarborListbox->AddListBoxItem(gcnew String(m_pRegionMapPointInfoManager->GetObject(i)->GetCharName().c_str()));
				}
				
			}
			if(!m_pTradeRoutesManager->ParseWithMyParse(DNCT::GcStringToChar(l_strRouteFileName)))
			{
				WARNING_MSG(l_strRouteFileName + "parse failed");
			}
			else
			{
				this->m_pWPFTradeRoutes->m_RouteListBox->Items->Clear();
				int	l_iCount = m_pTradeRoutesManager->Count();
				for( int i=0;i<l_iCount;++i )
				{
					cTradeRoutes*l_pTradeRoutes = m_pTradeRoutesManager->GetObject(i);
					this->m_pWPFTradeRoutes->m_RouteListBox->Items->Add(gcnew String(l_pTradeRoutes->GetCharName().c_str()));
				}
				this->m_pWPFTradeRoutes->m_RouteListBox->Items->SortDescriptions->Add(System::ComponentModel::SortDescription("",System::ComponentModel::ListSortDirection::Ascending));
				cTradeRoutesManager l_Temp;
				m_pTradeRoutesManager->DumpListPointer(&l_Temp);
				for( int i=0;i<l_iCount;++i )
				{
					String^l_strTemp = (String^)this->m_pWPFTradeRoutes->m_RouteListBox->Items[i];
					auto*l_object = l_Temp.GetObject(DNCT::GcStringToWchar(l_strTemp));
					m_pTradeRoutesManager->AddObjectNeglectExist(l_object);
				}
				l_Temp.SetFromResource(true);
			}
		}	
	}


	bool	HarborEditor::IsRoutePointNameLegal()
	{
		if(this->m_pWPFHarborInfo->m_HarborListbox->IsContain(m_pWPFTradeRoutes->m_StartName->Text) &&this->m_pWPFHarborInfo->m_HarborListbox->IsContain(m_pWPFTradeRoutes->m_EndName->Text))
		{
			return true;
		}
		return false;
	}

	System::Void HarborEditor::BGImage(System::Object^  sender, System::EventArgs^  e)
	{
		String^l_strFileName = DNCT::OpenFileAndGetName();
		if( l_strFileName )
		{
			m_pBGImage = new cBaseImage(DNCT::GcStringToChar(l_strFileName));
		}
	}

	System::Void HarborEditor::AddMpaPoint(System::Object^  sender, System::EventArgs^  e)
	{
		if( m_pWPFHarborInfo->m_HarborNameTextBox->Text->Length > 0)
		{
			const WCHAR*l_strName = DNCT::GcStringToWchar(m_pWPFHarborInfo->m_HarborNameTextBox->Text);
			if( !m_pRegionMapPointInfoManager->GetObject(l_strName) )
			{
				m_pWPFHarborInfo->m_HarborListbox->AddListBoxItem(m_pWPFHarborInfo->m_HarborNameTextBox->Text);
				m_pWPFHarborInfo->m_HarborListbox->m_ListBox->SelectedIndex = m_pWPFHarborInfo->m_HarborListbox->m_ListBox->Items->Count-1;
				cMapPointInfo*l_pMapPointInfo = new cMapPointInfo();
				l_pMapPointInfo->SetName(l_strName);
				l_pMapPointInfo->SetPointType(ePointType::ePT_HARBOR);
				l_pMapPointInfo->GetMapPointData()->vPos = Vector2(0,0);
				m_pWPFHarborInfo->m_HarborPos->Text = "0,0";
				m_pRegionMapPointInfoManager->AddObjectNeglectExist(l_pMapPointInfo);
				m_pWPFHarborInfo->UIClear();

			}
			else
			{
				WARNING_MSG("name exists!");
			}
		}
	}

	System::Void HarborEditor::DelMpaPoint(System::Object^  sender, System::EventArgs^  e)
	{
		int	l_iIndex = m_pWPFHarborInfo->m_HarborListbox->m_ListBox->Items->IndexOf(sender);
		m_pRegionMapPointInfoManager->RemoveObject(l_iIndex);
	}

	System::Void HarborEditor::HorborListboxSelectionChanged(System::Object^  sender, System::EventArgs^  e)
	{
		cMapPointInfo*l_pMapPointInfo = this->GetCurrentSelectMapPointInfo();
		if( l_pMapPointInfo == nullptr )
			return;
		ePointType	l_ePointType = l_pMapPointInfo->GetPointType();
		m_pWPFHarborInfo->m_bDisableMapPointTypeChange = true;
		m_pWPFHarborInfo->m_MapPointTypeCombobox->SelectedIndex = (int)l_ePointType;
		m_pWPFHarborInfo->m_MapPointTYpeTabControl->SelectedIndex = (int)l_ePointType;
		if( l_ePointType == ePointType::ePT_HARBOR )
		{
			sHarborData*l_pHarborData = (sHarborData*)l_pMapPointInfo->GetMapPointData();
			std::vector<int>l_CommodityIDVector = l_pHarborData->GetAllCommodityID();
			String^l_strID = gcnew String("");
			for(size_t i=0;i<l_CommodityIDVector.size();++i)
			{
				l_strID += l_CommodityIDVector[i].ToString();
				if( i != l_CommodityIDVector.size()-1 )
					l_strID += ",";
			}
			m_pWPFHarborInfo->m_MapPointHarbor->m_CommodityTextbox->Text = l_strID;
		}
		else
		if( l_ePointType == ePointType::ePT_POINT )
		{
			m_pWPFHarborInfo->m_MapPointRoutePoint->m_MapNameTextbox->Text = "";
			m_pWPFHarborInfo->m_MapPointRoutePoint->m_RoutePointTextBox->Text = "";
			sTradeRoutesPointData*l_pTradeRoutesPointData = (sTradeRoutesPointData*)l_pMapPointInfo->GetMapPointData();
			size_t	l_uiSize = l_pTradeRoutesPointData->JunctionPointNameVectorData.size();
			String^l_strMap = gcnew String("");
			String^l_strPointName = gcnew String("");
			if( l_uiSize != 0 )
			{
				for(size_t i=0;i<l_uiSize;++i)
				{
					sTradeRoutesPointData::sJunctionPointData*l_pJunctionPointData = &l_pTradeRoutesPointData->JunctionPointNameVectorData[i];
					l_strMap += DNCT::WcharToGcstring(l_pJunctionPointData->strMapName.c_str());
					l_strPointName += DNCT::WcharToGcstring(l_pJunctionPointData->strPointName.c_str());
					if( i != l_uiSize-1 )
					{
						l_strMap += ",";
						l_strPointName += ",";
					}
				}
				m_pWPFHarborInfo->m_MapPointRoutePoint->m_MapNameTextbox->Text = l_strMap;
				m_pWPFHarborInfo->m_MapPointRoutePoint->m_RoutePointTextBox->Text = l_strPointName;
			}
		}
	}

	System::Void HarborEditor::MapPointTypeChange(System::Object^  sender, System::EventArgs^  e)
	{
		cMapPointInfo*l_pMapPointInfo = HarborEditor::GetCurrentSelectMapPointInfo();
		if( l_pMapPointInfo )
		{
			Vector2	l_vPos = l_pMapPointInfo->GetMapPointData()->vPos;
			l_pMapPointInfo->SetPointType((ePointType)m_pWPFHarborInfo->m_MapPointTypeCombobox->SelectedIndex);
			l_pMapPointInfo->GetMapPointData()->vPos = l_vPos;
			m_pWPFHarborInfo->m_MapPointTYpeTabControl->SelectedIndex = m_pWPFHarborInfo->m_MapPointTypeCombobox->SelectedIndex;
		}
	}

	System::Void HarborEditor::AppllyMapPointHorbor(System::Object^  sender, System::EventArgs^  e)
	{
		//if( m_pCommodityManager == nullptr )
		//{
		//	if(!System::IO::File::Exists("Commodity.xml"))
		//	{
		//		WARNING_MSG("default commodity.xml is not found!");
		//		return ;
		//	}
		//	m_pCommodityManager = new cCommodityManager();
		//	m_pCommodityManager->ParseWithMyParse("Commodity.xml");
		//}
		if( m_pWPFHarborInfo->m_MapPointTypeCombobox->SelectedIndex == 0  )
		{
			cMapPointInfo*l_pMapPointInfo = HarborEditor::GetCurrentSelectMapPointInfo();
			if( l_pMapPointInfo )
			{
				sHarborData*l_pHarborData = (sHarborData*)l_pMapPointInfo->GetMapPointData();
				//do AreaID
				l_pHarborData->m_AreaID = (int)m_pWPFHarborInfo->m_MapPointHarbor->m_AreaIDNumeric->Value;
				//do KindomID
				l_pHarborData->m_iKingdomID = (int)m_pWPFHarborInfo->m_MapPointHarbor->m_KindomIDNumeric->Value;
				//do commodity
				std::vector<std::string>l_strRuelt = DNCT::GCStringToSTDStringVector(m_pWPFHarborInfo->m_MapPointHarbor->m_CommodityTextbox->Text);
				
				l_pHarborData->m_HarborItemInfoVector.Destroy();
				size_t	l_uiSize = l_strRuelt.size();
				//for( size_t i=0;i<l_uiSize;++i )
				//{
				//	cCommodity*l_pCommodity = m_pCommodityManager->GetCommodity(GetInt(l_strRuelt[i].c_str()));
				//	if( l_pCommodity )
				//	{
				//		if(!l_pHarborData->IsContainCommodity(l_pCommodity))
				//		{
				//			l_pHarborData->m_HarborItemInfoVector.AddObjectNeglectExist(l_pCommodity);
				//		}
				//		else
				//		{
				//			String^l_strErrorMsg = "same commodity exists";
				//			l_strErrorMsg += l_pCommodity->GetID().ToString();
				//			WARNING_MSG(l_strErrorMsg);
				//		}
				//	}
				//	else
				//	{
				//		String^l_strErrorMsg = "commodity name not exist,ID:";
				//		l_strErrorMsg += gcnew String(l_strRuelt[i].c_str());
				//		WARNING_MSG(l_strErrorMsg);
				//	}
				//}
			}
		}
	}

	System::Void HarborEditor::AppllyMapPointRoutePoint(System::Object^  sender, System::EventArgs^  e)
	{
		cMapPointInfo*l_pMapPointInfo = HarborEditor::GetCurrentSelectMapPointInfo();
		if( l_pMapPointInfo && m_pWPFHarborInfo->m_MapPointTypeCombobox->SelectedIndex == 1 )
		{
			std::vector<std::wstring>l_strMapRuelt = GCStringToSTDWStringVector(m_pWPFHarborInfo->m_MapPointRoutePoint->m_MapNameTextbox->Text);
			std::vector<std::wstring>l_strRoutePointRuelt = GCStringToSTDWStringVector(m_pWPFHarborInfo->m_MapPointRoutePoint->m_RoutePointTextBox->Text);
			int	l_iSize = (int)l_strMapRuelt.size();
			if( l_iSize != l_strRoutePointRuelt.size() )
			{
				WARNING_MSG("count is not match!");
				return;
			}
			sTradeRoutesPointData*l_pTradeRoutesPointData = (sTradeRoutesPointData*)l_pMapPointInfo->GetMapPointData();
			l_pTradeRoutesPointData->JunctionPointNameVectorData.clear();
			for( int i=0;i<l_iSize;++i )
			{
				sTradeRoutesPointData::sJunctionPointData	l_JunctionPointData = {l_strRoutePointRuelt[i],l_strMapRuelt[i]};
				l_pTradeRoutesPointData->JunctionPointNameVectorData.push_back(l_JunctionPointData);
			}
		}
	}


	System::Void HarborEditor::FileOpen(System::Object^  sender, System::EventArgs^  e)
	{
		String^l_strDirectory = SelectDirectory();
		OpenFile(l_strDirectory);
	}

	System::Void HarborEditor::FileSave(System::Object^  sender, System::EventArgs^  e)
	{
		String^l_strDirectory = SelectDirectory();
		if( l_strDirectory )
		{
			String^l_strCoomodityFileName = l_strDirectory+"/Commodity.xml";
			String^l_strHarborInfoFileName = l_strDirectory+"/HarborInfo.xml";
			String^l_strRouteFileName = l_strDirectory+"/Route.xml";

			//if(m_pCommodityManager)
			//	m_pCommodityManager->Export(DNCT::GcStringToChar(l_strCoomodityFileName));
			if(m_pRegionMapPointInfoManager)
				m_pRegionMapPointInfoManager->Export(DNCT::GcStringToChar(l_strHarborInfoFileName));
			if(m_pTradeRoutesManager)
				m_pTradeRoutesManager->Export(DNCT::GcStringToChar(l_strRouteFileName));
		}
	}

	System::Void HarborEditor::FileExcelConvertSave(System::Object^  sender, System::EventArgs^  e)
	{
		String^l_strFileName = DNCT::SaveFileAndGetName("xlsx files (*.xlsx)|*.xlsx|All files (*.*)|*.*");
		if( l_strFileName && m_pTradeRoutesManager )
		{
			System::Data::DataTable^l_pTable = gcnew System::Data::DataTable("Test123");
			l_pTable->Columns->Add("Name");
			l_pTable->Columns->Add("Distance");
			l_pTable->Columns->Add("NewDistance");
			
			cTradeRoutes*l_pTradeRoutes = nullptr;
			for (int i = 0; i < m_pTradeRoutesManager->Count(); i++)
			{
				
				l_pTradeRoutes = (*m_pTradeRoutesManager)[i];
				String^l_strName = DNCT::WcharToGcstring(l_pTradeRoutes->GetStartPoint()->GetName())+"_"+DNCT::WcharToGcstring(l_pTradeRoutes->GetEndPoint()->GetName());
				//String^l_strName = l_pTradeRoutes->GetStartPoint()->GetName();
				l_pTable->Rows->Add(l_strName,l_pTradeRoutes->GetDistance().ToString(),"0");
			}
			System::Data::DataSet^l_pDataSet = gcnew System::Data::DataSet("Test456");
			l_pDataSet->Tables->Add(l_pTable);
			HarborUI::MyExcel::Save(l_strFileName,l_pDataSet);
		}
	}

	System::Void HarborEditor::MapDataTradeRouteDataChanged(System::Object^  sender, System::EventArgs^  e)
	{
		cTradeRoutes*l_pTradeRoutes = GetCurrentSelectTradeRoutes();
		cLinerDataProcessor<Vector3>*l_pRoute = m_pCurrentEditRoute;
		if(IsRoutePointNameLegal())
		{
			cMapPointInfo*l_pStartMapPoint = this->m_pRegionMapPointInfoManager->GetObject(DNCT::GcStringToWchar(m_pWPFTradeRoutes->m_StartName->Text));
			cMapPointInfo*l_pEndMapPoint = this->m_pRegionMapPointInfoManager->GetObject(DNCT::GcStringToWchar(m_pWPFTradeRoutes->m_EndName->Text));
			float	l_fESEffect						= (float)m_pWPFTradeRoutes->m_ESEffect->Value;
			float	l_fSEEffect						= (float)m_pWPFTradeRoutes->m_SEEffect->Value;
			int		l_iLOD							= (int)m_pWPFTradeRoutes->m_LOD->Value;
			int		l_iTradeRoutesInsertPointCount	= (int)m_pWPFTradeRoutes->m_InsertPointCount->Value;
			if( l_pTradeRoutes )
			{
				l_pTradeRoutes->SetESEffect(l_fESEffect);
				l_pTradeRoutes->SetSEEffect(l_fSEEffect);
			}
			if( l_pStartMapPoint && l_pEndMapPoint )
			{
				if( this->m_pWPFTradeRoutes->m_InsertPointCount->m_bIsValueChangedTof_ValueChanged )
				{
					l_pRoute->Clear();
					l_pRoute->AddData(l_pStartMapPoint->GetMapPointData()->vPos,0);
					l_pRoute->AddData(l_pEndMapPoint->GetMapPointData()->vPos,0);
					l_pRoute->InsertDataWithLiner(0,l_iTradeRoutesInsertPointCount);
				}
				else
				if( this->m_pWPFTradeRoutes->m_LOD->m_bIsValueChangedTof_ValueChanged )
					l_pRoute->SetLOD(l_iLOD);
			}
		}
	}

	System::Void HarborEditor::RouteNameCheckEvent(System::Object^  sender, System::EventArgs^  e)
	{
		this->m_pWPFTradeRoutes->m_RouteListBox->SelectedIndex = -1;
		if( IsRoutePointNameLegal() )
		{
			cMapPointInfo*l_pStartMapPoint = this->m_pRegionMapPointInfoManager->GetObject(DNCT::GcStringToWchar(m_pWPFTradeRoutes->m_StartName->Text));
			cMapPointInfo*l_pEndMapPoint = this->m_pRegionMapPointInfoManager->GetObject(DNCT::GcStringToWchar(m_pWPFTradeRoutes->m_EndName->Text));
			m_pCurrentEditRoute->Clear();
			m_pCurrentEditRoute->AddData(l_pStartMapPoint->GetMapPointData()->vPos,0);
			m_pCurrentEditRoute->AddData(l_pEndMapPoint->GetMapPointData()->vPos,0);
		}
	}

	System::Void HarborEditor::AddTradeRoute(System::Object^  sender, System::EventArgs^  e)
	{
		if(IsRoutePointNameLegal())
		{
			this->timer1->Enabled = false;
			std::wstring	l_strStartName = DNCT::GcStringToWchar(m_pWPFTradeRoutes->m_StartName->Text);
			std::wstring	l_strEndName = DNCT::GcStringToWchar(m_pWPFTradeRoutes->m_EndName->Text);
			cMapPointInfo*l_pStartMapPoint = this->m_pRegionMapPointInfoManager->GetObject(DNCT::GcStringToWchar(m_pWPFTradeRoutes->m_StartName->Text));
			cMapPointInfo*l_pEndMapPoint = this->m_pRegionMapPointInfoManager->GetObject(DNCT::GcStringToWchar(m_pWPFTradeRoutes->m_EndName->Text));
			if( l_pStartMapPoint == nullptr || l_pEndMapPoint == nullptr )
			{
				WARNING_MSG("harbor name is not exists");
			}
			String^l_strRouteName = m_pWPFTradeRoutes->m_StartName->Text+","+m_pWPFTradeRoutes->m_EndName->Text;
			cTradeRoutes*l_pTradeRoutes = nullptr;
			int	l_iSameNameIndex = CoreWPF::Common::CommonFunction::IsListboxContainString(this->m_pWPFTradeRoutes->m_RouteListBox,l_strRouteName);
			if( l_iSameNameIndex != -1 )
			{
				WARING_YES_NO_TO_NO("same name replace old one?")
				{
					return;
				}
				this->m_pTradeRoutesManager->RemoveObject(l_iSameNameIndex);
			}
			if(m_pTradeRoutesManager->IsAllowToAdded(l_pStartMapPoint,l_pEndMapPoint) == false)
			{
				WARING_YES_NO_TO_NO("start and end point is not allow to has same!?")
				return ;
			}
			l_pTradeRoutes = new cTradeRoutes();
			l_pTradeRoutes->SetName(DNCT::GcStringToWchar(l_strRouteName));
			l_pTradeRoutes->SetStartPoint(l_pStartMapPoint);
			l_pTradeRoutes->SetEndPoint(l_pEndMapPoint);
			l_pTradeRoutes->SetSEEffect((float)m_pWPFTradeRoutes->m_SEEffect->Value);
			l_pTradeRoutes->SetESEffect((float)m_pWPFTradeRoutes->m_ESEffect->Value);
			l_pTradeRoutes->SetDistance(l_pTradeRoutes->GetRoute()->GetTotalDistance());
			l_pTradeRoutes->SetRoute((cLinerDataProcessor<Vector3>*)m_pCurrentEditRoute->Clone());
			if( l_iSameNameIndex == -1 )
			{
				m_pTradeRoutesManager->AddObjectNeglectExist(l_pTradeRoutes);
				m_pWPFTradeRoutes->m_RouteListBox->Items->Add(l_strRouteName);
			}
			else
			{
				if(!m_pTradeRoutesManager->InsertObject(l_iSameNameIndex,l_pTradeRoutes))
				{
					WARNING_MSG("call programmer somewehere wrong");
					SAFE_DELETE(l_pTradeRoutes);
				}
			}
			this->timer1->Enabled = true;
		}
		else
		{
			WARNING_MSG("route point name is not illegal");
		}
	}

	System::Void HarborEditor::DelTradeRoute(System::Object^  sender, System::EventArgs^  e)
	{
		int	l_iSelectedIndex = m_pWPFTradeRoutes->m_RouteListBox->SelectedIndex;
		if( l_iSelectedIndex != -1 )
		{
			m_pWPFTradeRoutes->m_RouteListBox->Items->RemoveAt(l_iSelectedIndex);
			m_pTradeRoutesManager->RemoveObject(l_iSelectedIndex);
		}
	}

	System::Void HarborEditor::TradeRouteSelectionChanged(System::Object^  sender, System::EventArgs^  e)
	{
		cTradeRoutes*l_pTradeRoutes = HarborEditor::GetCurrentSelectTradeRoutes();
		if( l_pTradeRoutes == nullptr )
			return;
		this->m_pWPFTradeRoutes->m_SEEffect->m_bIgnoreValueChanged = true;
		this->m_pWPFTradeRoutes->m_ESEffect->m_bIgnoreValueChanged = true;
		this->m_pWPFTradeRoutes->m_InsertPointCount->m_bIgnoreValueChanged = true;
		this->m_pWPFTradeRoutes->m_LOD->m_bIgnoreValueChanged = true;
		this->m_pWPFTradeRoutes->m_SEEffect->Value = l_pTradeRoutes->GetSEEffect();
		this->m_pWPFTradeRoutes->m_ESEffect->Value = l_pTradeRoutes->GetESEffect();		
		this->m_pWPFTradeRoutes->m_InsertPointCount->Value = l_pTradeRoutes->GetRoute()->GetLinerDataVector()->size();
		this->m_pWPFTradeRoutes->m_LOD->Value = l_pTradeRoutes->GetRoute()->GetLOD();
		this->m_pWPFTradeRoutes->m_Distance->Text = l_pTradeRoutes->GetRoute()->GetTotalDistance().ToString();
		this->m_pWPFTradeRoutes->m_StartName->Text = gcnew String(l_pTradeRoutes->GetStartPoint()->GetCharName().c_str());
		this->m_pWPFTradeRoutes->m_EndName->Text = gcnew String(l_pTradeRoutes->GetEndPoint()->GetCharName().c_str());
		l_pTradeRoutes->GetRoute()->DumpTo(m_pCurrentEditRoute);

	}
}



using namespace EditorSample;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	//Application::Run(gcnew HarborEditor());
	Application::Run(gcnew TradeRouteTimeCalculate());
	NamedTypedObject::DumpUnReleaseInfo();
	_CrtDumpMemoryLeaks();
	return 0;
}
