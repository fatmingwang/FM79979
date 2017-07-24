#include "stdafx.h"
#include "MPDI.h"
#include "../../../Include/IL/il.h"
#include "../../Core/GameplayUT/StringCompress.h"
#include "DotMPDICamera.h"
namespace MPDI
{
	System::Void MPDIEditor::saveAviFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if( !m_pMultiPathDynamicImageAWP )
			return;
		if( m_pMultiPathDynamicImageAWP->Count() <1 )
			return;
		this->timer1->Enabled = false;
		while( m_bTimerInRun ){}
		glAlphaFunc(GL_GREATER,0.00001f);
		glShadeModel(GL_SMOOTH);									// Select Smooth Shading
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(m_sfactor,m_dfactor);
		glEnable(GL_TEXTURE_2D);
		glEnable2D(m_pvResolution->x,m_pvResolution->y);
		float	l_fElpasedTime = 1/30.f;
		int	l_iWidth = splitContainer3->Panel1->Width;
		//set opengl aligment
		int	l_iRestOfWidthOffset = l_iWidth%4;
		int	l_iHeight = splitContainer3->Panel1->Height;
		//force to align for 4.
		while(l_iRestOfWidthOffset != 0)
		{
			splitContainer2->SplitterDistance = splitContainer2->Panel1->Width-1;
			l_iWidth = splitContainer3->Panel1->Width;
			l_iRestOfWidthOffset = l_iWidth%4;
		}
		l_iWidth = splitContainer3->Panel1->Width;
		l_iHeight = splitContainer3->Panel1->Height;
		//int	l_iWidth = (int)DRAW_PANEL_RESOLUTION_WIDTH;
		//int	l_iHeight = (int)DRAW_PANEL_RESOLUTION_HEIGHT;
		bool	l_bConpress = false;
		String^l_strAviName = DNCT::SaveFileAndGetName("avi files (*.avi)|*.avi|All files (*.*)|*.*");
		if( !l_strAviName )
			return;
		HAVI avi = CreateAvi(DNCT::GcStringToWchar(l_strAviName),(int)(l_fElpasedTime*1000),nullptr); // 1000ms is the period between frames
		Bitmap^ systemBitmap = gcnew Bitmap(l_iWidth, l_iHeight, System::Drawing::Imaging::PixelFormat::Format24bppRgb);
		int	l_ViewPort[4];
		glGetIntegerv(GL_VIEWPORT, l_ViewPort);

		int	l_iTotalSize = l_iWidth*l_iHeight*3;
		unsigned char *l_pPixelData = new unsigned char[l_iTotalSize];
		array<Byte>^ values = gcnew array<Byte>(l_iTotalSize);  
		m_pOrthogonalCamera->Render();

		cMPDI*l_pMPDI = new cMPDI(m_pMultiPathDynamicImageAWP);
		l_pMPDI->Init();
		l_pMPDI->SetAnimationLoop(false);
		while(!l_pMPDI->IsAnimationDone())
		{
			glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
			glClearColor( m_pvBGColor->x,m_pvBGColor->y,m_pvBGColor->z,m_pvBGColor->w );
			glClearDepth(1.0f);											// Depth Buffer Setup
			if(m_pBKImageAWP)
			{
				m_pBKImageAWP->Render();
			}
			l_pMPDI->Update(l_fElpasedTime);
			l_pMPDI->Render();
			SwapBuffers (m_HdcMV);
			glPixelStorei(GL_PACK_ALIGNMENT, 1);
			glReadPixels( 0, 0, l_iWidth, l_iHeight, GL_BGR_EXT , GL_UNSIGNED_BYTE, l_pPixelData );
			System::Runtime::InteropServices::Marshal::Copy((IntPtr)l_pPixelData, values, 0, l_iTotalSize);
			//create bitmap data, lock pixels to be written.
			System::Drawing::Imaging::BitmapData^ bitmapData = systemBitmap->LockBits(System::Drawing::Rectangle(0, 0, l_iWidth, l_iHeight), System::Drawing::Imaging::ImageLockMode::WriteOnly, systemBitmap->PixelFormat);
			System::Runtime::InteropServices::Marshal::Copy(values, 0, bitmapData->Scan0, l_iTotalSize);
			systemBitmap->UnlockBits(bitmapData);
			System::Drawing::Bitmap^l_pBitmap = systemBitmap;
			l_pBitmap->RotateFlip(System::Drawing::RotateFlipType::RotateNoneFlipY);

			//System::Drawing::Bitmap^l_pBitmap = gcnew System::Drawing::Bitmap(l_iWidth,l_iHeight,24,System::Drawing::Imaging::PixelFormat::Format24bppRgb,System::IntPtr((void*)l_pPixelData));
			HBITMAP l_Bitmap  = (HBITMAP)l_pBitmap->GetHbitmap().ToPointer();
			if (!l_bConpress) // Set up compression just before the first frame
			{ 
				l_bConpress = true;
				AVICOMPRESSOPTIONS opts; ZeroMemory(&opts,sizeof(opts));
				opts.fccHandler=mmioFOURCC('d','i','v','x');
				if( SetAviVideoCompression(avi,l_Bitmap,&opts,true,0)!= AVIERR_OK)
				{
					this->timer1->Enabled = true;
					return;
				}
			}
			AddAviFrame(avi,l_Bitmap);
			DeleteObject(l_Bitmap);
			if( 0 )
			{
				static int ll = 0;++ll;
				String^l_strtt = ll.ToString()+"test.png";
				SaveCurrentBufferToImage(DNCT::GcStringToChar(l_strtt));
				l_pBitmap->Save(l_strtt);
			}
		}
		CloseAvi(avi);
		delete l_pPixelData;
		delete l_pMPDI;
		this->timer1->Enabled = true;
	}
}