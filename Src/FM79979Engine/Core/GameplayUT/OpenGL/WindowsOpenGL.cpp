#include "WindowsOpenGL.h"
#if defined(WIN32) && !defined(UWP)
#include "Glh.h"
#include "OpenGLRender.h"
#include "assert.h"
bool	g_bArbMultisampleSupported = false;
int		g_bArbMultisampleFormat = 0;
bool	WGLisExtensionSupported(const char *extension)
{
	const size_t extlen = strlen(extension);
	const char *supported = nullptr;

	// Try To Use wglGetExtensionStringARB On Current DC, If Possible
	PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");

	if (wglGetExtString)
		supported = ((char*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());

	// If That Failed, Try Standard Opengl Extensions String
	if (supported == nullptr)
		supported = (char*)glGetString(GL_EXTENSIONS);

	// If That Failed Too, Must Be No Extensions Supported
	if (supported == nullptr)
		return false;

	// Begin Examination At Start Of String, Increment By 1 On False Match
	for (const char* p = supported; ; p++)
	{
		// Advance p Up To The Next Possible Match
		p = strstr(p, extension);

		if (p == nullptr)
			return false;						// No Match

												// Make Sure That Match Is At The Start Of The String Or That
												// The Previous Char Is A Space, Or Else We Could Accidentally
												// Match "wglFunkywglExtension" With "wglExtension"

												// Also, Make Sure That The Following Character Is Space Or nullptr
												// Or Else "wglExtensionTwo" Might Match "wglExtension"
		if ((p == supported || p[-1] == ' ') && (p[extlen] == '\0' || p[extlen] == ' '))
			return true;						// Match
	}
}
bool	InitMultisample(HDC e_HdcMV)
{
	// See If The String Exists In WGL!
	if (!WGLisExtensionSupported("WGL_ARB_multisample"))
	{
		g_bArbMultisampleSupported = false;
		return false;
	}

	// Get Our Pixel Format
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB =
		(PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

	if (!wglChoosePixelFormatARB)
	{
		// We Didn't Find Support For Multisampling, Set Our Flag And Exit Out.
		g_bArbMultisampleSupported = false;
		return false;
	}

	// Get Our Current Device Context. We Need This In Order To Ask The OpenGL Window What Attributes We Have
	HDC hDC = e_HdcMV;

	int pixelFormat;
	bool valid;
	UINT numFormats;
	float fAttributes[] = { 0,0 };

	// These Attributes Are The Bits We Want To Test For In Our Sample
	// Everything Is Pretty Standard, The Only One We Want To 
	// Really Focus On Is The SAMPLE BUFFERS ARB And WGL SAMPLES
	// These Two Are Going To Do The Main Testing For Whether Or Not
	// We Support Multisampling On This Hardware
	int iAttributes[] = { WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
		WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB,24,
		WGL_ALPHA_BITS_ARB,8,
		WGL_DEPTH_BITS_ARB,16,
		WGL_STENCIL_BITS_ARB,0,
		WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
		WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
		WGL_SAMPLES_ARB, 4 ,						// Check For 4x Multisampling
		0,0 };

	// First We Check To See If We Can Get A Pixel Format For 4 Samples
	valid = wglChoosePixelFormatARB(hDC, iAttributes, fAttributes, 1, &pixelFormat, &numFormats) ? true : false;

	// if We Returned True, And Our Format Count Is Greater Than 1
	if (valid && numFormats >= 1)
	{
		g_bArbMultisampleSupported = true;
		g_bArbMultisampleFormat = pixelFormat;
		//FMLog::LogWithFlag("support 4 Multisample", CORE_LOG_FLAG);
		return g_bArbMultisampleSupported;
	}

	// Our Pixel Format With 4 Samples Failed, Test For 2 Samples
	iAttributes[19] = 2;
	valid = wglChoosePixelFormatARB(hDC, iAttributes, fAttributes, 1, &pixelFormat, &numFormats) ? true : false;
	if (valid && numFormats >= 1)
	{
		g_bArbMultisampleSupported = true;
		g_bArbMultisampleFormat = pixelFormat;
		return g_bArbMultisampleSupported;
	}

	// Return The Valid Format
	return  g_bArbMultisampleSupported;
}
HGLRC	InitOpenGL(HWND e_pHwnd, bool e_bInitGlewInit, HDC e_HdcMV, bool e_bEnableMultisample)
{
	GLuint PixelFormat;													// Will Hold The Selected Pixel Format
	PIXELFORMATDESCRIPTOR pfd =											// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
		1,																// Version Number
		PFD_DRAW_TO_WINDOW |											// Format Must Support Window
		PFD_SUPPORT_OPENGL |											// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,												// Must Support Double Buffering
		PFD_TYPE_RGBA,													// Request An RGBA Format
		32,																// Select Our Color Depth
		0, 0, 0, 0, 0, 0,												// Color Bits Ignored
		1,																// No Alpha Buffer
		0,																// Shift Bit Ignored
		0,																// No Accumulation Buffer
		0, 0, 0, 0,														// Accumulation Bits Ignored
		16,																// 16Bit Z-Buffer (Depth Buffer)  
		0,																// No Stencil Buffer
		0,																// No Auxiliary Buffer
		PFD_MAIN_PLANE,													// Main Drawing Layer
		0,																// Reserved
		0, 0, 0															// Layer Masks Ignored
	};
	//fuck multi windows occur error
	//e_HdcMV = GetDC (e_pHwnd);									// Grab A Device Context For This Window
	if (e_HdcMV == 0)												// Did We Get A Device Context?
	{
		assert(0);
	}
	PixelFormat = ChoosePixelFormat(e_HdcMV, &pfd);				// Find A Compatible Pixel Format
	if (PixelFormat == 0)												// Did We Find A Compatible Format?
	{
		assert(0);
	}
	if (g_bArbMultisampleFormat != 0)//if u want to setup multi sample u have to ensure g_bArbMultisampleFormat have been assigned
		PixelFormat = g_bArbMultisampleFormat;
	if (SetPixelFormat(e_HdcMV, PixelFormat, &pfd) == FALSE)		// Try To Set The Pixel Format
	{
		assert(0);
	}
	HGLRC	l_HGLRC = wglCreateContext(e_HdcMV);						// Try To Get A Rendering Context
	if (l_HGLRC == 0)												// Did We Get A Rendering Context?
	{
		assert(0);												// Return False
	}

	// Make The Rendering Context Our Current Rendering Context
	if (wglMakeCurrent(e_HdcMV, l_HGLRC) == FALSE)
	{
		assert(0);
	}
	if (e_bInitGlewInit)
	{
		if (e_bEnableMultisample)
		{
			if (!InitMultisample(e_HdcMV))
			{
				//FMLog::LogWithFlag("not support Multisample", CORE_LOG_FLAG);
			}
		}
		GLenum	l_eErrorID = glewInit();
		assert(l_eErrorID == 0);
	}
	cOpenGLRender::m_sbVBOSupported = IsExtensionSupported("GL_ARB_vertex_buffer_object");
	CHECK_GL_ERROR("HGLRC	InitOpenGL");
	return l_HGLRC;
}
bool	SetOpenGLSwapInterval(bool e_bSwapInterval)
{
	typedef BOOL(WINAPI * PFNWGLSWAPINTERVALEXTPROC)(GLint);
	static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
	if (wglSwapIntervalEXT)
	{
		wglSwapIntervalEXT(e_bSwapInterval);
		return true;
	}
	return false;
}
bool	IsExtensionSupported(char* szTargetExtension)
{
	const unsigned char *pszExtensions = nullptr;
	const unsigned char *pszStart;
	unsigned char *pszWhere, *pszTerminator;

	// Extension names should not have spaces
	pszWhere = (unsigned char *)strchr(szTargetExtension, ' ');
	if (pszWhere || *szTargetExtension == '\0')
		return false;

	// Get Extensions String
	pszExtensions = glGetString(GL_EXTENSIONS);

	// Search The Extensions String For An Exact Copy
	pszStart = pszExtensions;
	for (;;)
	{
		pszWhere = (unsigned char *)strstr((const char *)pszStart, szTargetExtension);
		if (!pszWhere)
			break;
		pszTerminator = pszWhere + strlen(szTargetExtension);
		if (pszWhere == pszStart || *(pszWhere - 1) == ' ')
			if (*pszTerminator == ' ' || *pszTerminator == '\0')
				return true;
		pszStart = pszTerminator;
	}
	return false;
}
#else
//bool					InitMultisample(HDC e_HdcMV) { return false; }
//HGLRC					InitOpenGL(HWND e_pHwnd, bool e_bInitGlewInit, HDC e_HdcMV, bool e_bEnableMultisample) { return nullptr; }
//bool					SetOpenGLSwapInterval(bool e_bSwapInterval){ return false; }
//bool 					IsExtensionSupported(char* szTargetExtension){ return false; }
#endif