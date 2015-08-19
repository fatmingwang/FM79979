#ifndef _MPDI_CAMERA_H_
#define	_MPDI_CAMERA_H_

namespace MPDI
{
	ref class	cDotNEtMPDICamera
	{
		cOrthogonalCamera*					m_pOrthogonalCamera;
	public: cCameraAnimation*			m_p2DCameraAnimation;
		TextureEditorWPF::MPDICamera^		m_pMPDICamera;

		private: System::Void AddClick(System::Object^  sender, System::EventArgs^  e);
		private: System::Void DelClick(System::Object^  sender, System::EventArgs^  e);
		private: System::Void RevertClick(System::Object^  sender, System::EventArgs^  e);

		private: System::Void ViewWidthHeightApply(System::Object^  sender, System::EventArgs^  e);
		private: System::Void AngleApply(System::Object^  sender, System::EventArgs^  e);
		private: System::Void PositionApply(System::Object^  sender, System::EventArgs^  e);
		private: System::Void Replace(System::Object^  sender, System::EventArgs^  e);
		private: System::Void ChangeName(System::Object^  sender, System::EventArgs^  e);
				 cCameraAnimationManager*						m_pCameraAnimationManager;
				 void	UIDataUpdate();
	public:
		bool	m_bDoUpdate;
		cDotNEtMPDICamera(System::Windows::Forms::Control::ControlCollection^e_pContainer,cOrthogonalCamera*e_pOrthogonalCamera);
		~cDotNEtMPDICamera();
		void	Update(float e_fElpaseTime,float e_fGlobalTime);
		void	Render();
		void	Export(char*e_strFileName);
		void	LostFocus();
		float	GetTotalPlayTime();
	};
}

#endif