#include "stdafx.h"
#include "MPDI.h"
#include "../../../Include/IL/il.h"
#include "../../Core/Common/BinaryFile/StringCompress.h"
#include "DotMPDICamera.h"
#include "gif.h"
namespace MPDI
{
	//charlietangora thanks
	//https ://github.com/charlietangora/gif-h
	System::Void MPDIEditor::saveGIFFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (!m_pMultiPathDynamicImageAWP)
			return;
		if (m_pMultiPathDynamicImageAWP->Count() < 1)
			return;
		this->timer1->Enabled = false;
		while (m_bTimerInRun) {}
		glAlphaFunc(GL_GREATER, 0.00001f);
		glShadeModel(GL_SMOOTH);									// Select Smooth Shading
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(m_sfactor, m_dfactor);
		glEnable(GL_TEXTURE_2D);
		glEnable2D(m_pvResolution->x, m_pvResolution->y);
		int l_iFPS = 30;
		float	l_fElpasedTime = 1.f / l_iFPS;
		int	l_iWidth = splitContainer3->Panel1->Width;
		//set opengl aligment
		int	l_iRestOfWidthOffset = l_iWidth % 4;
		int	l_iHeight = splitContainer3->Panel1->Height;
		//force to align for 4.
		while (l_iRestOfWidthOffset != 0)
		{
			splitContainer2->SplitterDistance = splitContainer2->Panel1->Width - 1;
			l_iWidth = splitContainer3->Panel1->Width;
			l_iRestOfWidthOffset = l_iWidth % 4;
		}
		l_iWidth = splitContainer3->Panel1->Width;
		l_iHeight = splitContainer3->Panel1->Height;
		String^l_strAviName = DNCT::SaveFileAndGetName("GIF files (*.gif)|*.gif|All files (*.*)|*.*");
		if (!l_strAviName)
			return;
		int	l_ViewPort[4];
		glGetIntegerv(GL_VIEWPORT, l_ViewPort);

		int	l_iTotalSize = l_iWidth * l_iHeight * 4;
		unsigned char *l_pPixelData = new unsigned char[l_iTotalSize];
		cli::array<Byte>^ values = gcnew cli::array<Byte>(l_iTotalSize);
		m_pOrthogonalCamera->Render(true);

		cMPDI*l_pMPDI = new cMPDI(m_pMultiPathDynamicImageAWP);
		l_pMPDI->Init();
		l_pMPDI->SetAnimationLoop(false);

		std::string l_strFileName = DNCT::GcStringToChar(l_strAviName);
		GifWriter g;
		GifBegin(&g, l_strFileName.c_str(), l_iWidth, l_iHeight, l_iFPS/10);
		while (!l_pMPDI->IsAnimationDone())
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(m_pvBGColor->x, m_pvBGColor->y, m_pvBGColor->z, m_pvBGColor->w);
			glClearDepth(1.0f);											// Depth Buffer Setup
			if (m_pBKImageAWP)
			{
				m_pBKImageAWP->Render();
			}
			l_pMPDI->Update(l_fElpasedTime*2);
			l_pMPDI->Render();
			SwapBuffers(m_HdcMV);
			glPixelStorei(GL_PACK_ALIGNMENT, 1);
			glReadPixels(0, 0, l_iWidth, l_iHeight, GL_RGBA, GL_UNSIGNED_BYTE, l_pPixelData);
			GifWriteFrame(&g, l_pPixelData, l_iWidth, l_iHeight, l_iFPS/10);
		}
		//CloseAvi(avi);
		delete l_pPixelData;
		delete l_pMPDI;
		GifEnd(&g);
		this->timer1->Enabled = true;
	}
}