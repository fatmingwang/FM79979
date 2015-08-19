#pragma once
namespace MPDI 
{

	//#include "EditorInterface.h"

	//class	cFUSynchronized;

	//public ref class	cEditorInterface abstract
	//{
	//public:
	//	cEditorInterface(){}
	//	~cEditorInterface(){ Destroy(); }
	//	virtual	void	Update(float e_fElpaseTime) = 0;
	//	virtual	void	Render() = 0;
	//	virtual	void	Destroy() = 0;
	//};
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	public ref class	cMPDIVideo_Editor//:public cEditorInterface
	{
		System::Windows::Controls::Label^m_TimeLabel;
	//	cFUSynchronized*m_pLocker;
		cAVIPlayer*	  m_pAVIPlayer;
		cVideoSubMPDI*m_pVideoSubMPDI;
		CoreWPF::VideoPlayer::VideoPlayer^m_pVideoPlayer;
		System::Windows::Controls::Slider^m_pSliderTime;
		void	FileOpen(char*e_strFileName);
		private: System::Void FileOpen(System::Object^  sender, System::EventArgs^  e);
		private: System::Void StartTimeValueChanged(System::Object^  sender, System::EventArgs^  e);
		private: System::Void EndTimeValueChanged(System::Object^  sender, System::EventArgs^  e);
	public:
		cMPDIVideo_Editor(CoreWPF::VideoPlayer::VideoPlayer^e_pVideoPlayer);
		~cMPDIVideo_Editor();
		void			UpdateTrackBar();
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	Render();
		virtual	void	Destroy();
		public:  System::Void SetCurrentVideoTime(float e_fTime);
		public:  System::Void Play(System::Object^  sender, System::EventArgs^  e);
		public:  System::Void Pause(System::Object^  sender, System::EventArgs^  e);
		public:  System::Void Stop(System::Object^  sender, System::EventArgs^  e);
		public:  System::Void SliderToTime(System::Object^  sender, System::EventArgs^  e);
	};
//end namespace
}