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
#ifdef WASM
#include "../../Emscripten/JSBindCode.h"
#endif

#include "../Render/RenderQueue/BatchDataMultiTexture.h"

//m_spBatchDataMultiTexture->Start();
//int l_iOutNumVertex = 0;
//Vector3 l_vPosArray[20 * TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];
//Vector2 l_vUVArray[20 * TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];
//Vector4 l_vColorArray[20 * TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];
//m_pScoreNumberImage->SetValue(90001);
//cTexture* l_pTexture = m_pScoreNumberImage->GetTriangulatorRenderDataForBatchRendering(l_iOutNumVertex, l_vPosArray, l_vUVArray, l_vColorArray);
//m_spBatchDataMultiTexture->AddData(l_iOutNumVertex, (float*)l_vPosArray, (float*)l_vUVArray, (float*)l_vColorArray, l_pTexture);
//m_spBatchDataMultiTexture->End();
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cOpenGLRender)
	extern cBaseShader*			g_pCurrentShader;
	bool						cOpenGLRender::m_sbVBOSupported = true;
	bool						cOpenGLRender::m_sbSupportNonPowerOfTwoTexture = true;
	std::vector<int>*			cOpenGLRender::m_piSupportCompressedFormatVector = nullptr;
	cBatchDataMultiTexture*		cOpenGLRender::m_spBatchDataMultiTexture = nullptr;
	int							cOpenGLRender::m_siOpenGLVersion			= 40;
	int							cOpenGLRender::m_siOpenGL_ShaderVersion		= 40;
	int							cOpenGLRender::m_siOpenGLESVersion			= 20;
	int							cOpenGLRender::m_siOpenGLES_ShaderVersion	= 10;
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
#ifdef WASM
		EMSDK::EMSDK_JSInit();
#endif
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
		SAFE_DELETE(m_spBatchDataMultiTexture);
	}
#if defined(WIN32) && !defined(UWP)
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

	bool	cOpenGLRender::IsDoBatchRendering()
	{
		if (m_spBatchDataMultiTexture)
		{
			return m_spBatchDataMultiTexture->IsEnable();
		}
		return false;
	}

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
		//
		CreateShader(g_bCommonVSNoTextureClientState, g_strDrawPointsVS, g_strCommonFSNoTexture, DRAW_POINTS_SHADER);
		//
		CreateShader(g_bMyMeshVSClientState, g_strMyMeshVS, g_strMyMeshFS, STATIC_MESH_SHADER);
		//
		m_spBatchDataMultiTexture = new cBatchDataMultiTexture();
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
		int l_iMajor = 0;
		int l_iMinor = 0;
		float l_iShaderVer = 0;
		glGetIntegerv(GL_MAJOR_VERSION, &l_iMajor);
		glGetIntegerv(GL_MINOR_VERSION, &l_iMinor);
		int		l_iGLVersion = l_iMajor*10 + l_iMinor;
		int		l_iGLShaderVer = (int)(GetFloat((char*)l_strGL_SHADING_LANGUAGE_VERSION)*10);
#ifdef WIN32
		cOpenGLRender::m_siOpenGLVersion = l_iGLVersion;
		cOpenGLRender::m_siOpenGL_ShaderVersion = l_iGLShaderVer;
		if (l_iGLVersion >= 4.5)
		{
			cOpenGLRender::m_siOpenGLESVersion = 32;
			cOpenGLRender::m_siOpenGLES_ShaderVersion = 32;
		}
		else
		if (l_iGLVersion >= 4.0)
		{
			cOpenGLRender::m_siOpenGLESVersion = 30;
			cOpenGLRender::m_siOpenGLES_ShaderVersion = 30;
		}
		else
		if (l_iGLVersion >= 3.0)
		{
			cOpenGLRender::m_siOpenGLESVersion = 20;
			cOpenGLRender::m_siOpenGLES_ShaderVersion = 10;
		}
		else
		{
			cOpenGLRender::m_siOpenGLESVersion = 1;
		}
#else
		cOpenGLRender::m_siOpenGLESVersion = l_iGLVersion;
		cOpenGLRender::m_siOpenGLVersion = cOpenGLRender::m_siOpenGLESVersion + 10;
		//fuck this get wrong but I am lazy to fix this.
		cOpenGLRender::m_siOpenGLES_ShaderVersion = l_iGLShaderVer;
		cOpenGLRender::m_siOpenGL_ShaderVersion = l_iGLShaderVer+10;
#endif
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
#if defined(WIN32) && !defined(UWP)
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
#ifdef WIN32
		if (!this->m_Handle)
		{
#ifndef UWP
			return;
#endif
		}
#endif
		g_pCurrentShader = nullptr;
		glEnable(GL_BLEND);
		myGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		CHECK_GL_ERROR("before viewport");
#ifdef WASM
		EMSDK::EMSDK_JSViewportUpdate();
		auto l_iViewportWidth = EMSDK::EMSDK_GetViewportWidth();
		auto l_iViewportHeight = EMSDK::EMSDK_GetViewportHeight();
		SetAcceptRationWithGameresolution(l_iViewportWidth, l_iViewportHeight,
			(int)m_vGameResolution.x, (int)m_vGameResolution.y);
#else
		glViewport(0, 0, (GLsizei)this->m_vDeviceViewPortSize.Width(), (GLsizei)this->m_vDeviceViewPortSize.Height());
		//need this one or screen flash...and I dont know why
		glScissor(0, 0, (GLsizei)this->m_vDeviceViewPortSize.Width(), (GLsizei)this->m_vDeviceViewPortSize.Height());
		CHECK_GL_ERROR("after viewport");
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(m_vBGColor.x, m_vBGColor.y, m_vBGColor.z, m_vBGColor.w);
		CHECK_GL_ERROR("before viewport 1");
		if (m_vDeviceViewPortSize.x != m_vViewPortSize.x ||
			m_vDeviceViewPortSize.y != m_vViewPortSize.y ||
			m_vDeviceViewPortSize.z != m_vViewPortSize.z ||
			m_vDeviceViewPortSize.w != m_vViewPortSize.w)
		{
			glViewport((GLint)m_vViewPortSize.x, (GLint)m_vViewPortSize.y, (int)m_vViewPortSize.Width(), (int)m_vViewPortSize.Height());
			CHECK_GL_ERROR("after viewport 2");
			MyGLEnable(GL_SCISSOR_TEST);
			CHECK_GL_ERROR("before scissor");
			glScissor((GLint)m_vViewPortSize.x, (GLint)m_vViewPortSize.y, (int)m_vViewPortSize.Width(), (int)m_vViewPortSize.Height());
			CHECK_GL_ERROR("after scissor");
		}
#endif
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

	void	cOpenGLRender::DoBatchDataMultiTextureStart()
	{
		if (cOpenGLRender::m_spBatchDataMultiTexture)
		{
			cOpenGLRender::m_spBatchDataMultiTexture->Start();
		}
	}
	void	cOpenGLRender::DoBatchDataMultiTextureEnd()
	{
		if (cOpenGLRender::m_spBatchDataMultiTexture)
		{
			cOpenGLRender::m_spBatchDataMultiTexture->End();
		}
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
		//FMLog::Log("cOpenGLRender::SetAcceptRationWithGameresolution called", false);
	}
}