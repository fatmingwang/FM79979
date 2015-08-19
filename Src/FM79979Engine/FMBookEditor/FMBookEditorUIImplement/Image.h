#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace FMBookEditor
{

	public ref class Image:public cFatmingUtilitiesBase
	{
	public:
		Image(CoreWPF::FMC::Image^e_pUserControl1)
		{
			e_pUserControl1->f_Add += gcnew EventHandler(this,&Image::AddImage_button_Click);
			e_pUserControl1->f_Del += gcnew EventHandler(this,&Image::DeleteImage_button_Click);
			e_pUserControl1->f_SelectedIndexChanged += gcnew EventHandler(this,&Image::Image_listBox_SelectedIndexChanged);

			ImageList_listBox = e_pUserControl1->m_Image_listBox;
			m_pCurrentImage = 0;
		}
		~Image()
		{

		}
		System::Windows::Controls::ListBox^ImageList_listBox;
		cImageParser*		m_pImageParser;
		cBaseImage*			m_pCurrentImage;
		//seprate cAnimationParser's ImagePaser children list
		public:FileIsUsing	m_pImageListIsUsing;

		public:void			SetImageListIsUsing(void* e_pImageIsUsing){m_pImageListIsUsing = (FileIsUsing)e_pImageIsUsing;}
		public:cBaseImage*	GetCurrentImage();
		public:void			SetImageParser(void*e_pImageParser){ m_pImageParser = (cImageParser*)e_pImageParser; RefreshImageList(); }
		public:void			RefreshImageList();
		private: System::Void AddImage_button_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void DeleteImage_button_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void Image_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	};
}
