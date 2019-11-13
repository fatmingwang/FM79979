#include "StdAfx.h"
#include "Image.h"

namespace FMBookEditor
{
	cBaseImage*			Image::GetCurrentImage()
	{
		return m_pCurrentImage;
	}

	void			Image::RefreshImageList()
	{
        if( m_pImageParser )
        {
            int l_iCount = m_pImageParser->Count();
			ImageList_listBox->Items->Clear();
            for( int i=0;i<l_iCount;++i )
            {
                if( m_pImageParser->GetObject(i)->Type() == cBaseImage::TypeID )
                {
                    ImageList_listBox->Items->Add(DNCT::WcharToGcstring(m_pImageParser->GetObject(i)->GetName()));
                }
            }
            if( l_iCount>0 )
            {
                ImageList_listBox->SelectedIndex = ImageList_listBox->Items->Count-1;
            }
        }        	
	}

	System::Void	Image::AddImage_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
        if( m_pImageParser )
        {
            m_pCurrentImage = 0;
            cli::array<String^>^l_strFileNames = DNCT::OpenFileAndGetNames("image files (*.png)|*.png|All files (*.*)|*.*");
            for each(String^ l_strFileName in l_strFileNames )
            {
                String^l_strFile = System::IO::Path::GetFileNameWithoutExtension(l_strFileName);
                if(m_pImageParser->GetObject(DNCT::GcStringToWchar(l_strFile)))
                {
                    WARNING_MSG(l_strFile+" exists");
                    continue;
                }
                cBaseImage*l_pBaseImage = new cBaseImage(DNCT::GcStringToChar(l_strFileName).c_str());
                m_pImageParser->AddObjectNeglectExist(l_pBaseImage);
            }
			RefreshImageList();
        }	
	}

	System::Void	Image::DeleteImage_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
        if( m_pImageParser && ImageList_listBox->SelectedIndex != -1 )
        {
            m_pCurrentImage = 0;
            auto l_strName = DNCT::GcStringToWchar(ImageList_listBox->SelectedItem->ToString());
            m_pImageParser->RemoveObject(l_strName.c_str());
            RefreshImageList();
        }	
	}

	System::Void	Image::Image_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
        if( m_pImageParser && ImageList_listBox->SelectedIndex != -1 )
        {
            auto l_strName = DNCT::GcStringToWchar(ImageList_listBox->SelectedItem->ToString());
            m_pCurrentImage = dynamic_cast<cBaseImage*>(m_pImageParser->GetObject(l_strName.c_str()));
        }	
	}

}