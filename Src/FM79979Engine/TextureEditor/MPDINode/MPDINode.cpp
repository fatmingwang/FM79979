#include "stdafx.h"
#include "MPDINode.h"
namespace MPDINodeForm 
{
	MPDINodeEditor::MPDINodeEditor()
	{
		m_pMPDIRootNode = new cMPDINode();;
		m_pMPDIRootNode->SetName(L"Root");
		InitializeComponent();
		m_pMPDINodeForm = gcnew TextureEditorWPF::MPDI::MPDINodeForm();
		m_pMPDINodeData = gcnew MPDINodeData(m_pMPDINodeForm);
		//
		System::Windows::Forms::Integration::ElementHost^l_pElementHots = gcnew System::Windows::Forms::Integration::ElementHost;
		l_pElementHots->Dock = DockStyle::Fill;
		l_pElementHots->Child = m_pMPDINodeForm;
		Controls->Add(l_pElementHots);
		m_pRenderBorder = m_pMPDINodeForm->m_RenderGrid;
		System::Windows::Forms::Integration::WindowsFormsHost^l_pWindowsFormsHost = (System::Windows::Forms::Integration::WindowsFormsHost^)m_pRenderBorder->Child;
		m_pGameApp = new cGameApp((HWND)l_pWindowsFormsHost->Child->Handle.ToPointer());
		m_pGameApp->Init();
		m_pOrthgonalCamera = new cOrthogonalCamera();
		//
		m_pMPDINodeForm->m_ResolutionHeight->f_ValueChanged +=	gcnew System::EventHandler(this, &MPDINodeEditor::SeetingChanged);
		m_pMPDINodeForm->m_ResolutionWidth->f_ValueChanged	+=	gcnew System::EventHandler(this, &MPDINodeEditor::SeetingChanged);
		m_pMPDINodeForm->m_BGColor->f_SelectedColorChanged	+=	gcnew System::EventHandler(this, &MPDINodeEditor::SeetingChanged);
		m_pMPDINodeForm->f_ShowCheckboxCheckedChange		+=	gcnew System::EventHandler(this, &MPDINodeEditor::SeetingChanged);
		m_pMPDINodeForm->m_TreeView->f_ChangeSelectedItem	+=	gcnew System::EventHandler(this, &MPDINodeEditor::ChangeSelectedItem);
		m_pMPDINodeForm->m_TreeView->f_AddNewItem			+=	gcnew CoreWPF::Common::OnAddNewItem(this, &MPDINodeEditor::AddNewMPDINode);
		m_pMPDINodeForm->m_TreeView->f_DropTreeViewItemToAnotherTreeViewItem +=	gcnew CoreWPF::Common::OnTreeViewItemDragAndDrop(this, &MPDINodeEditor::DropTreeViewItemToAnotherTreeViewItem);

		m_pMPDINodeForm->m_TreeView->f_Delete				+=	gcnew System::EventHandler(this, &MPDINodeEditor::MPDINodeDelete);
		m_pMPDINodeForm->m_TreeView->f_Paste				+=	gcnew System::EventHandler(this, &MPDINodeEditor::MPDINodePaste);

		m_pMPDINodeForm->f_OpenGLFormSizeChanged			+=	gcnew System::EventHandler(this, &MPDINodeEditor::MPDINodeEditor_SizeChanged); 
		m_pMPDINodeForm->f_KeyUp							+=	gcnew System::Windows::Input::KeyEventHandler(this, &MPDINodeEditor::KeyUp);
		m_pMPDINodeForm->f_KeyDown							+=	gcnew System::Windows::Input::KeyEventHandler(this, &MPDINodeEditor::KeyDown);
		l_pWindowsFormsHost->Child->MouseDown				+=	gcnew System::Windows::Forms::MouseEventHandler(this, &MPDINodeEditor::MouseDown);
		l_pWindowsFormsHost->Child->MouseMove				+=	gcnew System::Windows::Forms::MouseEventHandler(this, &MPDINodeEditor::MouseMove);
		l_pWindowsFormsHost->Child->MouseUp					+=	gcnew System::Windows::Forms::MouseEventHandler(this, &MPDINodeEditor::MouseUp);
		l_pWindowsFormsHost->Child->MouseWheel				+=	gcnew System::Windows::Forms::MouseEventHandler(this, &MPDINodeEditor::MouseUp);
		//http://stackoverflow.com/questions/4942311/how-do-i-assign-an-event-handler-to-an-event-in-c-cli
		//auto l_CameraReset = delegate System::Void	CameraresetButtonClick(System::Object^sender,EventArgs^e)
		//{
		
		//}
		m_pMPDINodeForm->f_CameraResetButtonClick	+= gcnew System::EventHandler(this, &MPDINodeEditor::CameraButtonClick);

		this->WindowState = FormWindowState::Maximized;
		this->timer1->Enabled = true;
	}


	MPDINodeEditor::~MPDINodeEditor()
	{
		if (components)
		{
			delete components;
		}
		delete m_pMPDINodeData;
		SAFE_DELETE(m_pMPDIRootNode);
		SAFE_DELETE(m_pOrthgonalCamera);
		SAFE_DELETE(m_pGameApp);
	}

	System::Void MPDINodeEditor::MPDINodeEditor_SizeChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if( m_pMPDINodeForm == nullptr )
			return;
		CameraAdjustPara();
		m_pOrthgonalCamera->CameraUpdateByMouse(false,false,0,0,0,Vector2(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height()));
	}
	System::Void MPDINodeEditor::CameraAdjustPara()
	{
		cGameApp::m_svViewPortSize.z = (float)m_pMPDINodeForm->m_iOpenglRenderPanelWidth;
		cGameApp::m_svViewPortSize.w = (float)m_pMPDINodeForm->m_iOpenglRenderPanelHeight;
		Vector2	l_vCameraResolution;
		if( !EditorCommonVariables::m_sbCameraFitViewport )
		{
			l_vCameraResolution.x = cGameApp::m_svGameResolution.x;
			l_vCameraResolution.y = cGameApp::m_svGameResolution.y;
		}
		else
		{
			l_vCameraResolution.x = cGameApp::m_svViewPortSize.z;
			l_vCameraResolution.y = cGameApp::m_svViewPortSize.w;
		}
		m_pOrthgonalCamera->SetResolution(l_vCameraResolution);
	}

	System::Void MPDINodeEditor::MPDINodeEditor_Load(System::Object^  sender, System::EventArgs^  e)
	{
		m_pMPDINodeForm->SetGoodSize();
		m_pOrthgonalCamera->Reset();
		//m_pOrthgonalCamera->CameraUpdateByMouse(false,false,0,0,0,l_vViewPort);
	}

	System::Void MPDINodeEditor::SeetingChanged(System::Object^  sender, System::EventArgs^  e)
	{
		EditorCommonVariables::m_sbShowResolutionGrid = m_pMPDINodeForm->m_ShowResolutionGrid->IsChecked.Value;
		EditorCommonVariables::m_sbShowGrid = m_pMPDINodeForm->m_ShowGrid->IsChecked.Value;
		cGameApp::m_svGameResolution.x = (float)m_pMPDINodeForm->m_ResolutionWidth->Value.Value;
		cGameApp::m_svGameResolution.y = (float)m_pMPDINodeForm->m_ResolutionHeight->Value.Value;
		EditorCommonVariables::m_sbShowPathPos = m_pMPDINodeForm->m_ShowPosPath->IsChecked.Value;
		EditorCommonVariables::m_sbCameraFitViewport = m_pMPDINodeForm->m_CameraFit->IsChecked.Value;
		System::Windows::Media::Color l_vColor = m_pMPDINodeForm->m_BGColor->GetSelectedColor();
		EditorCommonVariables::m_sfBGColor[0] = l_vColor.R/255.f;
		EditorCommonVariables::m_sfBGColor[1] = l_vColor.G/255.f;
		EditorCommonVariables::m_sfBGColor[2] = l_vColor.B/255.f;
		EditorCommonVariables::m_sbShowPIUnitClick = m_pMPDINodeForm->m_ShowPIUnit->IsChecked.Value;
		CameraAdjustPara();
		m_pOrthgonalCamera->CameraUpdateByMouse(false,false,0,0,0,Vector2(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height()));
	}

	System::Void MPDINodeEditor::KeyDown(System::Object^  sender, System::Windows::Input::KeyEventArgs^  e)
	{
		System::Windows::Input::KeyEventArgs^ l_e = (System::Windows::Input::KeyEventArgs^)e;
		System::Windows::Input::KeyConverter^l_pKeyConverter = gcnew System::Windows::Input::KeyConverter;
		String^l_str = l_pKeyConverter->ConvertToString(l_e->Key);			 
		delete l_pKeyConverter;
	}
	System::Void MPDINodeEditor::KeyUp(System::Object^  sender,  System::Windows::Input::KeyEventArgs^  e)
	{
		System::Windows::Input::KeyEventArgs^ l_e = (System::Windows::Input::KeyEventArgs^)e;
		System::Windows::Input::KeyConverter^l_pKeyConverter = gcnew System::Windows::Input::KeyConverter;
		String^l_str = l_pKeyConverter->ConvertToString(l_e->Key);
		delete l_pKeyConverter;
	}
	System::Void MPDINodeEditor::MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if(!timer1->Enabled)
			return;
		System::Windows::Forms::Integration::WindowsFormsHost^l_pWindowsFormsHost = (System::Windows::Forms::Integration::WindowsFormsHost^)m_pRenderBorder->Child;
		System::Windows::Forms::Panel^l_pPanel = (System::Windows::Forms::Panel^)l_pWindowsFormsHost->Child;
		if(!l_pPanel->Focused)
			l_pPanel->Focus();
		GCFORM::MouseButtons l_MouseButton = e->Button;
		m_pOrthgonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
			,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height()));
		if(this->m_pMPDINodeData->m_pMPDINode == nullptr)
			return;
		Vector3	l_vPos(m_pOrthgonalCamera->GetMouseWorldPos());
		if( l_MouseButton == System::Windows::Forms::MouseButtons::Left &&m_pMPDINodeForm->m_MPDI_PathPointData->m_TabControl->SelectedIndex == (int)TextureEditorWPF::MPDI_PathPointData::eTabControlSelectedIndex::eTSI_POS)
		{
			this->m_pMPDINodeData->m_pLinerDataModifer->MouseMove(l_vPos);
			if( m_pMPDINodeData->m_pLinerDataModifer->m_bFixPoint )
			{
				m_pMPDINodeData->m_pLinerDataModifer->m_bFixPoint = false;
				ChangePosData(m_pMPDINodeData->m_pLinerDataModifer->m_iFocusPoint,this->m_pMPDINodeData->m_pLinerDataModifer->m_fAddTime,this->m_pMPDINodeData->m_pLinerDataModifer->m_vAddPos,this->m_pMPDINodeData->m_pMPDI_PathPointData);
			}
		}
	}

	System::Void MPDINodeEditor::MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if(!timer1->Enabled)
			return;
		GCFORM::MouseButtons l_MouseButton = e->Button;
		m_pOrthgonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
			,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height()));
		if(this->m_pMPDINodeData->m_pMPDINode == nullptr)
			return;
		Vector3	l_vPos(m_pOrthgonalCamera->GetMouseWorldPos());
		if( l_MouseButton == System::Windows::Forms::MouseButtons::Left &&m_pMPDINodeForm->m_MPDI_PathPointData->m_TabControl->SelectedIndex == (int)TextureEditorWPF::MPDI_PathPointData::eTabControlSelectedIndex::eTSI_POS)
		{
			this->m_pMPDINodeData->m_pLinerDataModifer->MouseUp(l_vPos);
			if(this->m_pMPDINodeData->m_pLinerDataModifer->m_bAddNewPoint)
			{
				AddPosToListbox(this->m_pMPDINodeData->m_pLinerDataModifer->m_fAddTime,this->m_pMPDINodeData->m_pLinerDataModifer->m_vAddPos,this->m_pMPDINodeData->m_pMPDI_PathPointData);
				this->m_pMPDINodeData->m_pLinerDataModifer->m_bAddNewPoint = false;
			}
		}
	} 
	System::Void MPDINodeEditor::MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if(!timer1->Enabled )
			return;
		GCFORM::MouseButtons l_MouseButton = e->Button;
		m_pOrthgonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
			,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2(cGameApp::m_svViewPortSize.Width(),cGameApp::m_svViewPortSize.Height()));
		if(this->m_pMPDINodeData->m_pMPDINode == nullptr)
			return;
		Vector3	l_vPos(m_pOrthgonalCamera->GetMouseWorldPos());
		
		if( l_MouseButton == System::Windows::Forms::MouseButtons::Left &&m_pMPDINodeForm->m_MPDI_PathPointData->m_TabControl->SelectedIndex == (int)TextureEditorWPF::MPDI_PathPointData::eTabControlSelectedIndex::eTSI_POS)
		{
			this->m_pMPDINodeData->m_pLinerDataModifer->MouseDown(l_vPos);
		}
	}
	System::Void MPDINodeEditor::CameraButtonClick(System::Object^  sender, System::EventArgs^  e)
	{
		m_pOrthgonalCamera->Reset();
	}

	System::Void MPDINodeEditor::DropTreeViewItemToAnotherTreeViewItem(System::Windows::Controls::TreeViewItem^e_pDragObject, System::Windows::Controls::TreeViewItem^e_pDropObject)
	{
		cMPDINode*l_pDragMPDINode = GetMPDINodeByTreeViewItem(m_pMPDINodeForm->m_TreeView->m_TreeView,e_pDragObject,m_pMPDIRootNode);
		cMPDINode*l_pDropMPDINode = GetMPDINodeByTreeViewItem(m_pMPDINodeForm->m_TreeView->m_TreeView,e_pDropObject,m_pMPDIRootNode);
		l_pDragMPDINode->SetParent(nullptr);
		l_pDropMPDINode->AddChildToLast(l_pDragMPDINode);
	}

	System::Void MPDINodeEditor::AddNewMPDINode(System::String^  e_strType, System::Windows::Controls::TreeViewItem^e_pTreeViewItem)
	{
		cMPDINode*l_pMPDINode = new cMPDINode();
		static int	l_i = 0;
		l_pMPDINode->SetName(DNCT::GcStringToWchar(e_strType));
		if( e_pTreeViewItem != nullptr )
		{
			cMPDINode*l_pParentMPDINode = GetMPDINodeByTreeViewItem(m_pMPDINodeForm->m_TreeView->m_TreeView,e_pTreeViewItem,m_pMPDIRootNode);
			l_pParentMPDINode->AddChildToLast(l_pMPDINode);
		}
		else
		{
			m_pMPDIRootNode->AddChildToLast(l_pMPDINode);
		}
		++l_i;
	}

	System::Void MPDINodeEditor::MPDINodePaste(System::Object^  sender, System::EventArgs^  e)
	{
		cMPDINode*l_pParentMPDINode = GetMPDINodeByTreeViewItem(m_pMPDINodeForm->m_TreeView->m_TreeView,m_pMPDINodeForm->m_TreeView->m_PasteItem,m_pMPDIRootNode);
		if( m_pMPDINodeForm->m_TreeView->m_CopyItem != nullptr )
		{
			cMPDINode*l_pTargetMPDINode = GetMPDINodeByTreeViewItem(m_pMPDINodeForm->m_TreeView->m_TreeView,m_pMPDINodeForm->m_TreeView->m_CopyItem,m_pMPDIRootNode);
			cMPDINode*l_pCloneMPDINode = dynamic_cast<cMPDINode*>(l_pTargetMPDINode->Clone());
			l_pParentMPDINode->AddChildToLast(l_pCloneMPDINode);
		}
		else
		if( m_pMPDINodeForm->m_TreeView->m_CutItem != nullptr )
		{
			cMPDINode*l_pTargetMPDINode = GetMPDINodeByTreeViewItem(m_pMPDINodeForm->m_TreeView->m_TreeView,m_pMPDINodeForm->m_TreeView->m_CutItem,m_pMPDIRootNode);
			if( l_pTargetMPDINode == l_pParentMPDINode )
				return;
			l_pTargetMPDINode->SetParent(nullptr);
			l_pParentMPDINode->AddChildToLast(l_pTargetMPDINode);
		}
	}

	System::Void MPDINodeEditor::MPDINodeDelete(System::Object^  sender, System::EventArgs^  e)
	{
		cMPDINode*l_pTargetMPDINode = GetMPDINodeByTreeViewItem(m_pMPDINodeForm->m_TreeView->m_TreeView,(System::Windows::Controls::TreeViewItem^)m_pMPDINodeForm->m_TreeView->m_DeleteItem,m_pMPDIRootNode);
		l_pTargetMPDINode->SetParent(nullptr);
		SAFE_DELETE(l_pTargetMPDINode);
	}

	System::Void MPDINodeEditor::ChangeSelectedItem(System::Object^  sender, System::EventArgs^  e)
	{
		cMPDINode*l_pTargetMPDINode = GetMPDINodeByTreeViewItem(m_pMPDINodeForm->m_TreeView->m_TreeView,(System::Windows::Controls::TreeViewItem^)m_pMPDINodeForm->m_TreeView->m_TreeView->SelectedItem,m_pMPDIRootNode);	 
		m_pMPDINodeData->SetMPDINode(l_pTargetMPDINode);
		//m_pMPDIRootNode->DumpDebugInfo();
	}

	System::Void MPDINodeEditor::timer1_Tick(System::Object^  sender, System::EventArgs^  e)
	{
		CameraAdjustPara();
		wglMakeCurrent(cGameApp::m_sHdc,cGameApp::m_sHGLRC);
		glViewport((int)cGameApp::m_svViewPortSize.x,(int)cGameApp::m_svViewPortSize.y,(int)cGameApp::m_svViewPortSize.z,(int)cGameApp::m_svViewPortSize.w);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		glClearColor(EditorCommonVariables::m_sfBGColor[0],EditorCommonVariables::m_sfBGColor[1],EditorCommonVariables::m_sfBGColor[2],1.f);
		glClearDepth(1.0f);											// Depth Buffer Setup
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		this->m_pGameApp->Run();
		if( m_pOrthgonalCamera )
		{
			m_pOrthgonalCamera->Render();
			if(EditorCommonVariables::m_sbShowGrid)
			{
				m_pOrthgonalCamera->DrawGrid();
			}
		}
		if(EditorCommonVariables::m_sbShowResolutionGrid)
		{
			GLRender::RenderRectangle(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,cMatrix44::Identity,Vector4::Red);
		}
		if( m_pMPDINodeData )
		{
			Vector3	l_vPos(m_pOrthgonalCamera->GetMouseWorldPos());
			m_pMPDINodeData->Run(l_vPos);
		}

		glEnable2D(1280,720);
		POINT	ptCursor = {(int)m_pOrthgonalCamera->GetMouseWorldPos().x,(int)m_pOrthgonalCamera->GetMouseWorldPos().y};
		WCHAR*l_strMousePos;
		l_strMousePos = DNCT::GcStringToWchar(ptCursor.x.ToString()+","+ptCursor.y.ToString());
		cGameApp::RenderFont(0,0,l_strMousePos);
		SwapBuffers(cGameApp::m_sHdc);
	}
//end namespace MPDINodeForm 
}