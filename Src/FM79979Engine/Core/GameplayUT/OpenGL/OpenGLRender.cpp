#include "OpenGLRender.h"
#include "../../Common/Log/FMLog.h"
#include "../../Common/StringToStructure.h"
#include "GLSL/Shader.h"
#include "assert.h"
#include "../Render/Texture/Texture.h"
#include "../Render/CommonRender/SimplePrimitive.h"
#include "../Render/OpenGLObject/VertexBufferObject.h"
#if defined(WIN32)
#include "WindowsOpenGL.h"
#endif
namespace FATMING_CORE
{
	bool				cOpenGLRender::m_sbVBOSupported = true;
	extern cBaseShader*	g_pCurrentShader;
	bool				cOpenGLRender::m_sbSupportNonPowerOfTwoTexture = true;
	std::vector<int>*	cOpenGLRender::m_piSupportCompressedFormatVector = nullptr;
	float				cOpenGLRender::m_sfOpenGLVersion = 0.f;
	////#if defined(ANDROID) || defined(IOS) 
	//eDeviceDirection									cGameApp::m_eDeviceDirection = eDD_PORTRAIT;
	////eDeviceDirection									cGameApp::m_eDeviceDirection = eDD_LANDSCAPE_LEFT;
	////eDeviceDirection									cGameApp::m_eDeviceDirection = eDD_LANDSCAPE_RIGHT;
	////eDeviceDirection									cGameApp::m_eDeviceDirection = eDD_UPSIDE_DOWN;
	////#else
	////eDeviceDirection									cGameApp::m_eDeviceDirection = eDD_LANDSCAPE_LEFT;
	////#endif
	cOpenGLRender::cOpenGLRender(Vector2 e_vGameResolution, Vector2 e_vViewportSize)
	{
		m_vGameScale = Vector2(1, 1);
		m_vBGColor = Vector4(0.1f, 0.1f, 0.1f, 1.f);
		this->m_vGameResolution = e_vGameResolution;
		m_vViewPortSize.x = 0.f;
		m_vViewPortSize.y = 0.f;
		m_vViewPortSize.z = (float)e_vViewportSize.x;
		m_vViewPortSize.w = (float)e_vViewportSize.y;
		m_vDeviceViewPortSize = m_vViewPortSize;
		m_eDeviceDirection = eDeviceDirection::eDD_PORTRAIT;
#if defined(WIN32)
		m_Hdc = nullptr;
		m_HGLRC = nullptr;
		m_Handle = nullptr;
#endif
	}
	cOpenGLRender::~cOpenGLRender()
	{
		SAFE_DELETE(m_piSupportCompressedFormatVector);
		DeleteAllShader();
	}
#ifdef WIN32
	void cOpenGLRender::Init(HWND e_Hwnd, bool e_bMultiSample)
	{
		m_sMultisample = e_bMultiSample;
		if (e_Hwnd)
		{
			m_Hdc = GetDC(e_Hwnd);
			FMLog::LogWithFlag("init openGL", CORE_LOG_FLAG, true);
			m_HGLRC = InitOpenGL(e_Hwnd, true, m_Hdc, e_bMultiSample);
			if(m_HGLRC)
				FMLog::LogWithFlag("openGL Created.", CORE_LOG_FLAG, true);
			else
				FMLog::LogWithFlag("openGL create failed.", CORE_LOG_FLAG, true);
			m_Handle = e_Hwnd;
		}
		if (m_HGLRC)
		{
			CreateDefaultShader();
		}
	}
#else
	void cOpenGLRender::Init()
	{
		CreateDefaultShader();
	}
#endif
	void cOpenGLRender::CreateDefaultShader()
	{
		glUseProgram(0);
		//#else
		//		glEnableClientState(GL_VERTEX_ARRAY);
		//#endif
		FMLog::LogWithFlag("init shader start", CORE_LOG_FLAG, true);
		//2d image shader
		CreateShader(g_bCommonVSClientState, DEFAULT_SHADER);
		//for non texture shader
		CreateShader(g_bCommonVSNoTextureClientState, NO_TEXTURE_SHADER);

		CreateShader(g_bMyMeshVSClientState, g_strMyMeshVS, g_strMyMeshFS, STATIC_MESH_SHADER);
		//if crush go to char*g_strMySkinningMeshVS = "
		//fin matBones[32] and change its size...
		//CreateShader(g_bMySkinningMeshVSClientState, g_strMySkinningMeshVS, g_strMySkinningMeshFS, SKINNING_MESH_SHADER);
		FMLog::LogWithFlag("init shader end", CORE_LOG_FLAG, true);
	}
	void	DumpGraphicsInfo()
	{
		std::wstring	l_str;
		const GLubyte*l_strGL_VERSION = glGetString(GL_VERSION);
		const GLubyte*l_strGL_SHADING_LANGUAGE_VERSION = glGetString(GL_SHADING_LANGUAGE_VERSION);
		//https://stackoverflow.com/questions/28242148/glews-glewissupportedgl-version-3-1-returning-false-on-mbp-osx-10-10-1-with
		//OSX
		//const GLubyte*l_strGL_GL_MINOR_VERSION = glGetString(GL_MINOR_VERSION);
		//const GLubyte*l_strGL_GL_MAJOR_VERSION = glGetString(GL_MAJOR_VERSION);
		//too much things...I am lazy to parse this
		//const GLubyte*l_strGL_GL_EXTENSIONS = glGetString(GL_EXTENSIONS);
		if (!l_strGL_VERSION)
		{
			FMLog::LogWithFlag(L"openGL not actived", CORE_LOG_FLAG);
			return;
		}
		float l_fVersion = GetFloat((char*)l_strGL_VERSION);
		float l_f2 = GetFloat((char*)l_strGL_SHADING_LANGUAGE_VERSION);
		cOpenGLRender::m_sfOpenGLVersion = l_fVersion;
		l_str = L"GL_VERSION:";					l_str += ValueToStringW((char*)l_strGL_VERSION);	l_str += L"\n";	FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
		l_str = L"GL_SHADING_LANGUAGE_VERSION:";					l_str += ValueToStringW((char*)l_strGL_SHADING_LANGUAGE_VERSION);	l_str += L"\n";	FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);

		GLint	l_iValue = 0;
		MyGLGetIntegerv(GL_MAX_TEXTURE_SIZE, &l_iValue);					l_str = L"GL_MAX_TEXTURE_SIZE:";					l_str += ValueToStringW(l_iValue);	l_str += L"\n";	FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
		MyGLGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &l_iValue);	l_str = L"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:";	l_str += ValueToStringW(l_iValue);	l_str += L"\n";	FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
		MyGLGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &l_iValue);			l_str = L"GL_MAX_TEXTURE_IMAGE_UNITS:";			l_str += ValueToStringW(l_iValue);	l_str += L"\n";	FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
		//#ifndef WIN32
		MyGLGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &l_iValue);		l_str = L"GL_MAX_FRAGMENT_UNIFORM_VECTORS:";		l_str += ValueToStringW(l_iValue);	l_str += L"\n";	FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
		MyGLGetIntegerv(GL_MAX_VARYING_VECTORS, &l_iValue);				l_str = L"GL_MAX_VARYING_VECTORS:";				l_str += ValueToStringW(l_iValue);	l_str += L"\n";	FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
		MyGLGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &l_iValue);			l_str = L"GL_MAX_VERTEX_UNIFORM_VECTORS:";		l_str += ValueToStringW(l_iValue);	l_str += L"\n";	FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
		//#endif
		MyGLGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &l_iValue);					l_str = L"GL_MAX_VERTEX_ATTRIBS:";				l_str += ValueToStringW(l_iValue);	l_str += L"\n";	FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
		MyGLGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &l_iValue);		l_str = L"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS:";	l_str += ValueToStringW(l_iValue);	l_str += L"\n";	FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
		//MyGLGetIntegerv(GL_MAX_VIEWPORT_DIMS,&l_iValue);					l_str = L"GL_MAX_VIEWPORT_DIMS:";				l_str += ValueToStringW(l_iValue);	l_str += L"\n";	FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
#ifdef WIN32
		MyGLGetIntegerv(GL_ARB_texture_non_power_of_two, &l_iValue);
		cOpenGLRender::m_sbSupportNonPowerOfTwoTexture = l_iValue ? true : false;
#endif
		//http://www.informit.com/articles/article.aspx?p=770639&seqNum=3
		//GLfloat l_fGL_TEXTURE_COMPRESSED;
		const int l_ciSupportFormatCountTestValue = 100;
		GLint	l_iSupportFormat[l_ciSupportFormatCountTestValue];
		memset(l_iSupportFormat, 0, sizeof(l_iSupportFormat));
		SAFE_DELETE(cOpenGLRender::m_piSupportCompressedFormatVector);
		cOpenGLRender::m_piSupportCompressedFormatVector = new std::vector<int>();
		MyGLGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, l_iSupportFormat);
		l_str = L"GL_COMPRESSED_TEXTURE_FORMATS:";
		for (int i = 0; i < l_ciSupportFormatCountTestValue; ++i)
		{
			if (l_iSupportFormat[i] == 0)
				break;
			cOpenGLRender::m_piSupportCompressedFormatVector->push_back(l_iSupportFormat[i]);
			char hexString[20];
			itoa(l_iSupportFormat[i], hexString, 16);
			l_str += ValueToStringW(hexString);
			l_str += L",";
		}
		l_str += L"\n";
		FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
		//
		//glGetTexLevelParameterfv(GL_TEXTURE_2D,0, GL_TEXTURE_COMPRESSED,&l_fGL_TEXTURE_COMPRESSED);
	}

	void cOpenGLRender::Update(float e_fElpaseTime)
	{
	}

	void cOpenGLRender::Render()
	{
		g_pCurrentShader = nullptr;
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		MyGlErrorTest("before viewport");
		glViewport(0, 0, (GLsizei)this->m_vDeviceViewPortSize.Width(), (GLsizei)this->m_vDeviceViewPortSize.Height());
		//need this one or screen flash...and I dont know why
		glScissor(0, 0, (GLsizei)this->m_vDeviceViewPortSize.Width(), (GLsizei)this->m_vDeviceViewPortSize.Height());
		MyGlErrorTest("after viewport");
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(m_vBGColor.x, m_vBGColor.y, m_vBGColor.z, m_vBGColor.w);
		MyGlErrorTest("before viewport 1");
		if (m_vDeviceViewPortSize.x != m_vViewPortSize.x ||
			m_vDeviceViewPortSize.y != m_vViewPortSize.y ||
			m_vDeviceViewPortSize.z != m_vViewPortSize.z ||
			m_vDeviceViewPortSize.w != m_vViewPortSize.w)
		{
			glViewport((GLint)m_vViewPortSize.x, (GLint)m_vViewPortSize.y, (int)m_vViewPortSize.Width(), (int)m_vViewPortSize.Height());
			MyGlErrorTest("after viewport 2");
			MyGLEnable(GL_SCISSOR_TEST);
			MyGlErrorTest("before scissor");
			glScissor((GLint)m_vViewPortSize.x, (GLint)m_vViewPortSize.y, (int)m_vViewPortSize.Width(), (int)m_vViewPortSize.Height());
			MyGlErrorTest("after scissor");
		}
		SystemErrorCheck();
		UseShaderProgram(DEFAULT_SHADER);
		cTexture::m_suiLastUsingImageIndex = -1;
		//glClearDepth(1.0f);								// Depth Buffer Setup
		glEnable2D(m_vGameResolution.x, m_vGameResolution.y);
		SystemErrorCheck();
	}

	POINT	cOpenGLRender::ConvertCoordinate(int e_iPosX, int e_iPosY, POINT e_ViewPort)
	{
		Vector2		l_vScale = cOpenGLRender::GetViewPortAndGameResolutionScale();
		POINT	l_Pos = { e_iPosX,e_iPosY };
		switch (m_eDeviceDirection)
		{
		case eDD_PORTRAIT://do nothing
			l_Pos.x = e_iPosX;
			l_Pos.y = e_iPosY;
			break;
		case eDD_UPSIDE_DOWN:
			l_Pos.x = e_ViewPort.x - e_iPosX;
			l_Pos.y = e_ViewPort.y - e_iPosY;
			break;
			//windows and iOS is not same?
		case eDD_LANDSCAPE_LEFT://ensure view port is right,x and y swap
			l_Pos.x = e_ViewPort.y - e_iPosY;
			l_Pos.y = e_iPosX;
			break;//ensure view port is right,x and y swap
		case eDD_LANDSCAPE_RIGHT:

			l_Pos.x = e_iPosY;
			l_Pos.y = e_ViewPort.x - e_iPosX;
			break;
		default:
			assert(0 && "cOpenGLRender::ConvertCoordinate switch not in range");
			break;
		}
		l_Pos.x = (int)(l_Pos.x - m_vViewPortSize.x);
		l_Pos.y = (int)(l_Pos.y - m_vViewPortSize.y);
		l_Pos.x = (int)(l_Pos.x*l_vScale.x);
		l_Pos.y = (int)(l_Pos.y*l_vScale.y);
		return l_Pos;
	}

	Vector2 cOpenGLRender::GetViewPortAndGameResolutionScale()
	{
		switch (m_eDeviceDirection)
		{
		case eDD_PORTRAIT:
		case eDD_UPSIDE_DOWN:
			return Vector2(m_vGameResolution.x / m_vViewPortSize.Width(), m_vGameResolution.y / m_vViewPortSize.Height());
		case eDD_LANDSCAPE_LEFT:
		case eDD_LANDSCAPE_RIGHT:
			return Vector2(m_vGameResolution.x / m_vViewPortSize.Height(),m_vGameResolution.y / m_vViewPortSize.Width());
		default:
			break;
		}
		return Vector2(1, 1);
	}

	bool cOpenGLRender::IsCompressedFormatSupport(int e_iFormat)
	{
		if (!cOpenGLRender::m_piSupportCompressedFormatVector)
		{
			DumpGraphicsInfo();
		}
		if (cOpenGLRender::m_piSupportCompressedFormatVector)
		{
			int l_iSize = (int)cOpenGLRender::m_piSupportCompressedFormatVector->size();
			for (int i = 0; i < l_iSize; ++i)
			{
				if (e_iFormat == (*cOpenGLRender::m_piSupportCompressedFormatVector)[i])
					return true;
			}
		}
		return false;
	}

	void cOpenGLRender::SetAcceptRationWithGameresolution(int e_iDeviceViewportWidth, int e_iDeviceViewportHeight, int e_iTargetResolutionWidth, int e_iTargetResolutionHeight)
	{
		double l_DeviceScreenRation = (double)e_iDeviceViewportWidth / e_iDeviceViewportHeight;
		double l_GameResolutionRation = (double)e_iTargetResolutionWidth / e_iTargetResolutionHeight;
		if (l_DeviceScreenRation == l_GameResolutionRation)
			return;
		float l_fScaleX = e_iDeviceViewportWidth / (float)e_iTargetResolutionWidth;
		float l_fScaleY = e_iDeviceViewportHeight / (float)e_iTargetResolutionHeight;
		l_fScaleX = l_fScaleY = min(l_fScaleX, l_fScaleY);
		// calculate the rect of viewport
		float l_fViewPortW = (float)e_iTargetResolutionWidth * l_fScaleX;
		float l_fViewPortH = (float)e_iTargetResolutionHeight * l_fScaleY;

		m_vViewPortSize.x = (e_iDeviceViewportWidth - l_fViewPortW) / 2;
		m_vViewPortSize.y = (e_iDeviceViewportHeight - l_fViewPortH) / 2;
		m_vViewPortSize.z = m_vViewPortSize.x + l_fViewPortW;
		m_vViewPortSize.w = m_vViewPortSize.y + l_fViewPortH;
		glViewport((int)m_vViewPortSize.x, (int)m_vViewPortSize.y, (int)m_vViewPortSize.Width(), (int)m_vViewPortSize.Height());
#ifdef WIN32
		m_vDeviceViewPortSize.x = 0;
		m_vDeviceViewPortSize.y = 0;
		m_vDeviceViewPortSize.z = (float)e_iDeviceViewportWidth;
		m_vDeviceViewPortSize.w = (float)e_iDeviceViewportHeight;
#endif
	}
}