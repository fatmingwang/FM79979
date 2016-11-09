#include "stdafx.h"
#include "DotMPDICamera.h"
#include "TimeWithVectorDataApply.h"
using namespace System::Windows::Forms::Integration;
namespace MPDI
{
	cDotNEtMPDICamera::cDotNEtMPDICamera(System::Windows::Forms::Control::ControlCollection^e_pContainer,cOrthogonalCamera*e_pOrthogonalCamera)
	{
		m_pCameraAnimationManager = new cCameraAnimationManager();
		m_bDoUpdate = false;
		m_pOrthogonalCamera = e_pOrthogonalCamera;
		m_p2DCameraAnimation = new cCameraAnimation();
		m_pMPDICamera = gcnew TextureEditorWPF::MPDICamera();


		//e_pContainer->Add((System::Windows::Forms::Control^)AddWPFComponentByElementHost(m_pMPDICamera));
		ElementHost^l_pHost = GetMyElementHost(l_pHost);
		l_pHost->Dock = DockStyle::Fill;
		l_pHost->Child = m_pMPDICamera;
		e_pContainer->Add(l_pHost);


		DNCT::cTimeWithVector3^l_pp = gcnew DNCT::cTimeWithVector3(m_pMPDICamera->m_AngleUI,m_p2DCameraAnimation->GetAngleAnimationData());
		//DNCT::cTimeWithVector4^l_pp2 = gcnew DNCT::cTimeWithVector4(m_pMPDICamera->m_GameResolutionUI,m_p2DCameraAnimation->GetGameResolutionAnimationData());
		m_pMPDICamera->f_AngleApply += gcnew System::EventHandler(this, &cDotNEtMPDICamera::AngleApply);
		m_pMPDICamera->f_ViewWidthHeightApply += gcnew System::EventHandler(this, &cDotNEtMPDICamera::ViewWidthHeightApply);
		m_pMPDICamera->f_PositionApply += gcnew System::EventHandler(this, &cDotNEtMPDICamera::PositionApply);
        m_pMPDICamera->SetAngleContent("Test");
        m_pMPDICamera->SetGameresolutionContent("Test2");
		System::EventArgs^  e;		ViewWidthHeightApply(this,e);		AngleApply(this,e); PositionApply(this,e);
		m_pMPDICamera->f_AddClick += gcnew System::EventHandler(this, &cDotNEtMPDICamera::AddClick);
		m_pMPDICamera->f_DeleteClick += gcnew System::EventHandler(this, &cDotNEtMPDICamera::DelClick);
		m_pMPDICamera->f_RevertClick += gcnew System::EventHandler(this, &cDotNEtMPDICamera::RevertClick);
		m_pMPDICamera->m_CameraAnimationNameListbox->f_Replace += gcnew System::EventHandler(this, &cDotNEtMPDICamera::Replace);
		m_pMPDICamera->m_CameraAnimationNameListbox->f_ChangeName += gcnew System::EventHandler(this, &cDotNEtMPDICamera::ChangeName);
	}

	cDotNEtMPDICamera::~cDotNEtMPDICamera()
	{
		SAFE_DELETE(m_p2DCameraAnimation);
		SAFE_DELETE(m_pCameraAnimationManager);
	}

	void	cDotNEtMPDICamera::Update(float e_fElpaseTime,float e_fGlobalTime)
	{
		Vector4	l_vViewPort;
		Vector4	l_vGameResolution;
		Vector4	l_vAngle;
		if( m_bDoUpdate )
		{
			if( e_fGlobalTime != -1.f )
			{
				m_p2DCameraAnimation->RenderByGlobalTime(e_fGlobalTime);
				//m_p2DCameraAnimation->Update(e_fElpaseTime);
			}
		}
	}

	void	cDotNEtMPDICamera::Render()
	{
		if( m_bDoUpdate )
		{
			m_p2DCameraAnimation->Render();
		}
	}

	System::Void cDotNEtMPDICamera::AddClick(System::Object^  sender, System::EventArgs^  e)
	{
		String^l_str = m_pMPDICamera->m_CameraAnimationNameTextbox->Text;
		//if( m_pMPDICamera->m_CameraAnimationNameListbox->IndexOf(l_str)
		m_pMPDICamera->m_CameraAnimationNameListbox->AddListBoxItem(l_str);
	}

	System::Void cDotNEtMPDICamera::DelClick(System::Object^  sender, System::EventArgs^  e)
	{
		int	l_iIndex = m_pMPDICamera->m_CameraAnimationNameListbox->m_ListBox->Items->IndexOf(sender);
		if( l_iIndex != -1 )
			m_pCameraAnimationManager->RemoveObject(l_iIndex);
	}

	System::Void cDotNEtMPDICamera::RevertClick(System::Object^  sender, System::EventArgs^  e)
	{
		int	l_iIndex = m_pMPDICamera->m_CameraAnimationNameListbox->m_ListBox->Items->IndexOf(sender);
		if( l_iIndex != -1 )
		{
			*m_p2DCameraAnimation = *dynamic_cast<cCameraAnimation*>(m_pCameraAnimationManager->GetObject(l_iIndex));
			UIDataUpdate();
		}	
	}

	System::Void cDotNEtMPDICamera::ViewWidthHeightApply(System::Object^  sender, System::EventArgs^  e)
	{
		//m_pMPDICamera->m_GameResolution->SetValue((float*)m_pOrthogonalCamera->GetViewRectPointer());
	}

	System::Void cDotNEtMPDICamera::PositionApply(System::Object^  sender, System::EventArgs^  e)
	{
		//assert(0);
	}

	System::Void cDotNEtMPDICamera::AngleApply(System::Object^  sender, System::EventArgs^  e)
	{
		m_pMPDICamera->m_Angle->SetValue((float*)m_pOrthogonalCamera->GetCameraAngle());
	}


	System::Void cDotNEtMPDICamera::Replace(System::Object^  sender, System::EventArgs^  e)
	{
		int	l_iIndex = m_pMPDICamera->m_CameraAnimationNameListbox->m_ListBox->Items->IndexOf(sender);
		if( l_iIndex != -1 )
		{
			cCameraAnimation*l_p2DCameraAnimation = dynamic_cast<cCameraAnimation*>(m_p2DCameraAnimation->Clone());
			m_pCameraAnimationManager->ReplaceObject(l_iIndex,l_p2DCameraAnimation);
		}
		else
		{
			UT::ErrorMsg(L"ReplaceName error",L"Warning");
		}
	}

	System::Void cDotNEtMPDICamera::ChangeName(System::Object^  sender, System::EventArgs^  e)
	{
		int	l_iIndex = m_pMPDICamera->m_CameraAnimationNameListbox->m_ListBox->Items->IndexOf(sender);
		if( l_iIndex != -1 )
		{
			m_pCameraAnimationManager->GetObject(l_iIndex)->SetName(DNCT::GcStringToWchar(m_pMPDICamera->m_CameraAnimationNameListbox->SelectedName()));
		}
		else
		{
			UT::ErrorMsg(L"ReplaceName error",L"Warning");
		}	
	}

	void	cDotNEtMPDICamera::Export(char*e_strFileName)
	{
		if( m_pCameraAnimationManager->Count() )
			m_pCameraAnimationManager->Export(e_strFileName);
	}

	void	cDotNEtMPDICamera::UIDataUpdate()
	{
	
	}

	void	cDotNEtMPDICamera::LostFocus()
	{
	
	}

	float	cDotNEtMPDICamera::GetTotalPlayTime()
	{
		return m_p2DCameraAnimation->GetEndTime();
	}
}