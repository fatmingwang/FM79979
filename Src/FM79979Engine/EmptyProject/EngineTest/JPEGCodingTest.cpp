#include "stdafx.h"
#include "JPEGCodingTest.h"

cJPEGCodingTest::cJPEGCodingTest()
{
}

cJPEGCodingTest::~cJPEGCodingTest()
{
}

void cJPEGCodingTest::Init()
{
}

void cJPEGCodingTest::CaptureScreentToEncode()
{
	auto l_iWidth = cGameApp::m_spOpenGLRender->m_vViewPortSize.Width();
	auto l_iHeight = cGameApp::m_spOpenGLRender->m_vViewPortSize.Height();
	unsigned char*l_pData = GetScreenPixels(l_iWidth, l_iHeight);
	SaveCurrentBufferToImage("99.jpg",l_iWidth,l_iHeight);
}
