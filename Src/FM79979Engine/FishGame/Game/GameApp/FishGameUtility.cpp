#include "stdafx.h"

cClickMouseBehavior* g_pDebugForceKillFishClickMouseBehavior = nullptr;
cClickMouseBehavior* g_pDebugMouseZoomClickMouseBehavior = nullptr;
cClickMouseBehavior* g_pDebugUpdateAndRenderTime = nullptr;
cClickMouseBehavior* g_pDebugSwapInterval = nullptr;
cClickMouseBehavior* g_pDebugSkipRendering = nullptr;

void				DebugInit()
{
	if (cGameApp::m_sbDebugFunctionWorking && !g_pDebugForceKillFishClickMouseBehavior)
	{
		g_pDebugForceKillFishClickMouseBehavior = new cClickMouseBehavior();
		g_pDebugMouseZoomClickMouseBehavior = new cClickMouseBehavior();
		g_pDebugUpdateAndRenderTime = new cClickMouseBehavior();
		g_pDebugSwapInterval = new cClickMouseBehavior();
		g_pDebugSkipRendering = new cClickMouseBehavior();
	}
}

void				DebugUpdate(float e_fElpaseTime)
{
	if (g_pDebugSkipRendering)
	{
		g_pDebugSkipRendering->SetKeyData(L'T');//enter
		g_pDebugSkipRendering->SingnalProcess();
		g_pDebugSkipRendering->Update(0.016f);
		if (g_pDebugSkipRendering->IsSatisfiedCondition())
		{
			g_sbCollisionRender = !g_sbCollisionRender;
			//cFishApp::m_siSkipRenderingCount++;
			//if (cFishApp::m_siSkipRenderingCount >= 10)
			//{
			//	cFishApp::m_siSkipRenderingCount = 5;
			//	cGameApp::m_sbSkipRendering = false;
			//	cGameApp::m_sbDoLockFPS = false;
			//	cGameApp::m_sfDebugValue = 1.f;
			//}
			//else
			//{
			//	cGameApp::m_sbDoLockFPS = true;
			//	cGameApp::m_sbSkipRendering = true;
			//	cGameApp::m_sfDebugValue = 16.f;
			//}
			g_pDebugSkipRendering->Init();
		}
	}
	if (g_pDebugSwapInterval)
	{
		g_pDebugSwapInterval->SetKeyData(L'O');//enter
		g_pDebugSwapInterval->SingnalProcess();
		g_pDebugSwapInterval->Update(0.016f);
		if (g_pDebugSwapInterval->IsSatisfiedCondition())
		{
			//if (cGameApp::m_sucKeyData[17])
			//{
			//	cGameApp::m_sbSwapInterval = !cGameApp::m_sbSwapInterval;
			//	typedef BOOL(WINAPI * PFNWGLSWAPINTERVALEXTPROC)(GLint);
			//	static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
			//	if (wglSwapIntervalEXT)
			//	{
			//		wglSwapIntervalEXT(cGameApp::m_sbSwapInterval);
			//	}
			//}
			g_pDebugSwapInterval->Init();
		}
	}

	if (g_pDebugForceKillFishClickMouseBehavior)
	{
		g_pDebugForceKillFishClickMouseBehavior->SetKeyData(116);//f5
		g_pDebugForceKillFishClickMouseBehavior->SingnalProcess();
		g_pDebugForceKillFishClickMouseBehavior->Update(0.016f);
		if (g_pDebugForceKillFishClickMouseBehavior->IsSatisfiedCondition())
		{

		}
	}
}

void				DebugRender()
{
	if (g_pDebugMouseZoomClickMouseBehavior)
	{
		static bool l_bTest = false;
		g_pDebugMouseZoomClickMouseBehavior->SetKeyData('Z');
		g_pDebugMouseZoomClickMouseBehavior->SingnalProcess();
		g_pDebugMouseZoomClickMouseBehavior->Update(0.016f);
		float l_fScale[4] = { 0,0.5,1,2 };
		static int l_siCurrentIndex = 0;
		if (g_pDebugMouseZoomClickMouseBehavior->IsSatisfiedCondition())
		{
			if (cGameApp::m_sucKeyData[17])
				l_bTest = !l_bTest;
			g_pDebugMouseZoomClickMouseBehavior->Init();
			++l_siCurrentIndex;
			if (l_siCurrentIndex >= 4)
				l_siCurrentIndex = 0;
		}
		if (l_bTest)
		{
			float l_fWidth = cGameApp::m_svGameResolution.x * (l_fScale[l_siCurrentIndex]);
			float l_fHeight = cGameApp::m_svGameResolution.y * (l_fScale[l_siCurrentIndex]);
			cMatrix44 l_matProjection;
			glhOrthof2((float*)l_matProjection.m, -l_fWidth, cGameApp::m_svGameResolution.x + l_fWidth, cGameApp::m_svGameResolution.y + l_fHeight, -l_fHeight, -10000, 10000);
			FATMING_CORE::SetupShaderViewProjectionMatrix(l_matProjection, true);
		}
	}
}

void				DebugLastRender()
{
	if (g_pDebugUpdateAndRenderTime)
	{
		static bool l_bTest = false;
		g_pDebugUpdateAndRenderTime->SetKeyData(117);//f6?
		g_pDebugUpdateAndRenderTime->SingnalProcess();
		g_pDebugUpdateAndRenderTime->Update(0.016f);
		if (g_pDebugUpdateAndRenderTime->IsSatisfiedCondition())
		{
			if (cGameApp::m_sucKeyData[17])
				l_bTest = !l_bTest;
			g_pDebugUpdateAndRenderTime->Init();
		}
		if (l_bTest)
		{
			int l_iPosX = 800;
			int l_iPosY = 100;
			int l_iGap = 60;
			//GLRender::RenderFilledRectangle(Vector2(l_iPosX, l_iPosY), 500, 750, Vector4(1, 0, 0, 0.5),0);
		}
	}
}

void				DebugDestory()
{
	SAFE_DELETE(g_pDebugForceKillFishClickMouseBehavior);
	SAFE_DELETE(g_pDebugMouseZoomClickMouseBehavior);
	SAFE_DELETE(g_pDebugUpdateAndRenderTime);
	SAFE_DELETE(g_pDebugSwapInterval);
	SAFE_DELETE(g_pDebugSkipRendering);
}
//extern HINSTANCE		hInst;
//extern HWND				g_hWnd;
//
//void	cFishApp::MyResoluctionParse(char*e_strFileName)
//{
//	cNodeISAX	l_NodeISAX;
//	bool	l_bFullScreen = false;
//	bool	l_b = l_NodeISAX.ParseDataIntoXMLNode(e_strFileName);
//	if (l_b)
//	{
//		TiXmlElement*l_pTiXmlElement = l_NodeISAX.GetRootElement();
//		const wchar_t*l_strFullscreen = l_pTiXmlElement->Attribute(L"FullScreen");
//		if (l_strFullscreen)
//			m_sbFullScreen = _wtoi(l_strFullscreen) ? true : false;
//		const wchar_t*l_strResolution = l_pTiXmlElement->Attribute(L"Resolution");
//		const wchar_t*l_strViewPort = l_pTiXmlElement->Attribute(L"ViewPort");
//		const wchar_t*l_strDeviceOrietation = l_pTiXmlElement->Attribute(L"DeviceOrietation");
//		const wchar_t*l_strHideMouseCursor = l_pTiXmlElement->Attribute(L"HideMouseCursor");
//		const wchar_t*l_strTexturePowerOfTwo = l_pTiXmlElement->Attribute(L"TexturePowerOfTwo");
//		const wchar_t*l_strDevice = l_pTiXmlElement->Attribute(L"Device");
//		const wchar_t*l_strDebugFunctionWorking = l_pTiXmlElement->Attribute(L"DebugFunction");
//		const wchar_t*l_strDownSampleScale = l_pTiXmlElement->Attribute(L"DownSampleScale");
//		const wchar_t*l_strUseMultiSample = l_pTiXmlElement->Attribute(L"UseMultiSample");
//		const wchar_t*l_strLockFPS = l_pTiXmlElement->Attribute(L"LockFPS");
//		const wchar_t*l_strSwapIntervalEXT = l_pTiXmlElement->Attribute(L"SwapIntervalEXT");
//		const wchar_t*l_strUseKeyboardToControlPlayer = l_pTiXmlElement->Attribute(L"UseKeyboardToControlPlayer");
//		if(l_strUseKeyboardToControlPlayer)
//			cFishApp::m_sbUseKeyboardToControlPlayer = GetBoolean(l_strUseKeyboardToControlPlayer);
//		if (l_strSwapIntervalEXT)
//		{
//			if (GetInt(l_strUseMultiSample))
//			{
//				cGameApp::m_sbSwapInterval = true;
//				typedef BOOL(WINAPI * PFNWGLSWAPINTERVALEXTPROC)(GLint);
//				static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
//				if (wglSwapIntervalEXT)
//				{
//					wglSwapIntervalEXT(cGameApp::m_sbSwapInterval);
//				}
//			}
//		}
//		if (l_strUseMultiSample)
//		{
//			if (GetInt(l_strUseMultiSample))
//				cGameApp::m_sbMultisample = true;
//		}
//		if (l_strDebugFunctionWorking)
//		{
//			m_sbDebugFunctionWorking = _wtoi(l_strDebugFunctionWorking) ? true : false;
//		}
//		if (l_strDownSampleScale)
//		{
//			m_sfDownSampleScale = GetFloat(l_strDownSampleScale);
//		}
//		if (l_strDevice)
//			m_sbDeviceExist = _wtoi(l_strDevice) ? true : false;
//		if (l_strResolution)
//		{
//			Vector2	l_vResolution = GetVector2(l_strResolution);
//			if (!l_strViewPort)
//			{
//				cGameApp::m_svViewPortSize.x = 0;
//				cGameApp::m_svViewPortSize.y = 0;
//				cGameApp::m_svViewPortSize.z = l_vResolution.x;
//				cGameApp::m_svViewPortSize.w = l_vResolution.y;
//			}
//			cGameApp::m_svGameResolution.x = l_vResolution.x;
//			cGameApp::m_svGameResolution.y = l_vResolution.y;
//			//if(!m_spClickMouseBehavior)
//			//	m_spClickMouseBehavior = new cClickMouseBehavior();
//			//m_spClickMouseBehavior->SetCollisionRange(Vector4(0,0,m_svGameResolution.x,m_svGameResolution.y));
//		}
//		if (l_strViewPort)
//		{
//			Vector2	l_vResolution = GetVector2(l_strViewPort);
//			cGameApp::m_svViewPortSize.x = 0;
//			cGameApp::m_svViewPortSize.y = 0;
//			cGameApp::m_svViewPortSize.z = l_vResolution.x;
//			cGameApp::m_svViewPortSize.w = l_vResolution.y;
//		}
//		if (l_strDeviceOrietation)
//		{
//			int	l_iIndex = _wtoi(l_strDeviceOrietation);
//			if (l_iIndex <= eDD_LANDSCAPE_RIGHT)
//				cGameApp::m_seDeviceDirection = (eDeviceDirection)l_iIndex;
//		}
//		if (l_strHideMouseCursor)
//		{
//#ifdef WIN32
//			ShowCursor(_wtoi(l_strHideMouseCursor) == 0 ? false : true);
//#endif
//		}
//		if (l_strTexturePowerOfTwo)
//		{
//			g_bSupportNonPowerOfTwoTexture = _wtoi(l_strTexturePowerOfTwo) ? true : false;
//		}
//		if (l_strLockFPS)
//		{
//			cGameApp::m_sbDoLockFPS = GetBoolean(l_strLockFPS);
//		}
//	}
//}