//#include "GLSLUiniform.h"
#include "Shader.h"
#include "GLSLUiniform.h"
#include "../../GameApp/GameApp.h"
#include "../OpenGLRender.h"
//#include "../../GameplayUT/GameApp.h"

GLenum	g_iDrawindiceType =
#if defined(WIN32) && !defined(UWP)
GL_UNSIGNED_INT;
#else
GL_UNSIGNED_SHORT;
#endif

GLenum	FMGetDrawIndiexType()
{
	static bool l_sbOnlyDoOnce = false;
	if (!l_sbOnlyDoOnce)
	{
		l_sbOnlyDoOnce = true;
#if !defined(WIN32)
		if (cOpenGLRender::m_siOpenGLESVersion < OPENGL_ES_30)
		{
			g_iDrawindiceType = GL_UNSIGNED_SHORT;
		}
		else
		{
			g_iDrawindiceType = GL_UNSIGNED_INT;
		}
#endif
	}
	return g_iDrawindiceType;
}

//#ifndef GL_PROGRAM_POINT_SIZE
//#define GL_PROGRAM_POINT_SIZE 0x8642
//#endif
namespace FATMING_CORE
{
	GLuint	g_iMatrixVPLoacation;
	GLuint	g_iMatrixWLoacation;
	GLuint	g_iTexLoacation[8];//max support 8 texture
	GLuint	g_iBonesMatrixoacation;

	GLuint	g_iColorLoacation;
	GLuint	g_iPointSizeLoacation;
	GLuint	g_uiAttribArray[TOTAL_FVF];

	float	g_fViewProjectionMatrix[16];
	float	g_fWorldMatrix[16];
	Vector4	g_vGlobalScaleColor(1,1,1,1);
	cBaseShader*g_pCurrentShader = nullptr;
	extern cGLSLProgram*g_pCurrentUsingGLSLProgram;
	bool	g_bShowErrorMessageBoxIfShaderIsWrong = true;
	cBaseShader::cBaseShader(const wchar_t*e_strName,bool e_bTexture)
	{
		g_pCurrentShader = nullptr;
		m_bDataUpdated = false;
		m_uiProgram = -1;
		m_uiVS = -1;
		m_uiFS = -1;
		m_uiPointSize = -1;
		m_uiBonesLocation = -1;
		m_uiColorLoacation = -1;
		m_uiTexLoacation = -1;
		m_uiMatrixWLoacation = -1;
		m_uiMatrixVPLoacation = -1;
		this->SetName(e_strName);
		m_bTexture = e_bTexture;
		memset(m_uiAttribArray,-1,sizeof(GLuint)*TOTAL_FVF);
		bool	l_b = CreateProgram(e_bTexture?g_strCommonVS:g_strCommonVSNoTexture,
									e_bTexture?g_strCommonFS:g_strCommonFSNoTexture,
									e_bTexture);
		std::wstring l_str = e_strName;
		l_str += L" shader created";
		FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
		assert(g_bShowErrorMessageBoxIfShaderIsWrong?l_b:true);
	}

	cBaseShader::cBaseShader(const char*e_strVS,const char*e_strPS,const wchar_t*e_strName,bool e_bTexture)
	{
		g_pCurrentShader = nullptr;
		m_bDataUpdated = false;
		m_uiVS = -1;
		m_uiFS = -1;
		m_uiProgram = -1;
		m_uiPointSize = -1;
		m_uiBonesLocation = -1;
		m_uiColorLoacation = -1;
		m_uiTexLoacation = -1;
		m_uiMatrixWLoacation = -1;
		m_uiMatrixVPLoacation = -1;
		this->SetName(e_strName);
		m_bTexture = e_bTexture;
		memset(m_uiAttribArray,-1,sizeof(GLuint)*TOTAL_FVF);
		bool	l_b = CreateProgram(e_strVS,e_strPS,e_bTexture);
		assert(g_bShowErrorMessageBoxIfShaderIsWrong?l_b:true);
	}
	cBaseShader::cBaseShader()
	{
		g_pCurrentShader = nullptr;
		m_bDataUpdated = false;
		m_uiProgram = -1;
		m_uiVS = -1;
		m_uiFS = -1;
		memset(m_uiAttribArray,-1,sizeof(GLuint)*TOTAL_FVF);
	}
	cBaseShader::cBaseShader(const wchar_t*e_strName,bool *e_pbClientState)
	{
		g_pCurrentShader = nullptr;
		m_bDataUpdated = false;
		m_uiProgram = -1;
		m_uiVS = -1;
		m_uiFS = -1;
		this->SetName(e_strName);
		if( e_pbClientState != nullptr )
		{
			for( int i=0;i<TOTAL_FVF;++i )
				m_uiAttribArray[i] = e_pbClientState[i]?1:-1;
		}
	}

	cBaseShader::~cBaseShader()
	{
		if (m_uiVS != -1)
		{
			glDeleteShader(m_uiVS);
		}
		if (m_uiFS != -1)
		{
			glDeleteShader(m_uiFS);
		}
		if (m_uiProgram != -1)
		{
			glDeleteProgram(m_uiProgram);
		}
	}

	bool	cBaseShader::CreateVS(const char*e_strVS)
	{
		m_uiVS = glCreateShader( GL_VERTEX_SHADER );
		const char*l_strVS[1] = {e_strVS};
		// Pass our sources to OpenGL. Our sources are nullptr terminated, so pass nullptr for the lenghts.
		glShaderSource( m_uiVS, 1, l_strVS, nullptr );

		// Compile that one object.
		glCompileShader(m_uiVS);

		// Check for compilation success
		GLint compilationResult = 0;
		glGetShaderiv( m_uiVS, GL_COMPILE_STATUS, &compilationResult );
		std::wstring l_str = this->GetName();
		l_str += L" VS Compile";
		if( g_bShowErrorMessageBoxIfShaderIsWrong )
			FATMING_CORE::CheckShader(m_uiVS, GL_COMPILE_STATUS,l_str.c_str());
		else
			FATMING_CORE::CheckShader(m_uiVS, GL_COMPILE_STATUS,l_str.c_str(),&this->m_strShaderLog);
	   // current implementation always succeeds.
	   // The error will happen at link time.
	   if ( compilationResult == 0 )
	   {
		  FMLog::LogWithFlag(L"Failed to compile shader:", CORE_LOG_FLAG);
		  glDeleteShader(m_uiVS);
		  return false;
	   }
	   return true;
	}

	bool	cBaseShader::CreateFS(const char*e_strPS)
	{
		m_uiFS = glCreateShader( GL_FRAGMENT_SHADER );
		const char*l_strPS[1] = {e_strPS};
		// Pass our sources to OpenGL. Our sources are nullptr terminated, so pass nullptr for the lenghts.
		glShaderSource( m_uiFS, 1, l_strPS, nullptr );

		// Compile that one object.
		glCompileShader(m_uiFS);

		// Check for compilation success
		GLint compilationResult = 0;
		glGetShaderiv( m_uiFS, GL_COMPILE_STATUS, &compilationResult );
		std::wstring l_str = this->GetName();
		l_str += L" FS Compile";
		if( g_bShowErrorMessageBoxIfShaderIsWrong )
			FATMING_CORE::CheckShader(m_uiFS, GL_COMPILE_STATUS,l_str.c_str());
		else
			FATMING_CORE::CheckShader(m_uiFS, GL_COMPILE_STATUS,l_str.c_str(),&this->m_strShaderLog);
	   // current implementation always succeeds.
	   // The error will happen at link time.
	   if ( compilationResult == 0 )
	   {
		  FMLog::LogWithFlag(L"Failed to compile shader:", CORE_LOG_FLAG);
		  glDeleteShader(m_uiFS);
		  return false;
	   }
	   return true;
	}

	GLuint	cBaseShader::GetUniFormLocationByName(const char*e_strName)
	{
		CHECK_GL_ERROR("cBaseShader::GetUniFormLocationByName");
		GLuint	l_ui = glGetUniformLocation( m_uiProgram,e_strName);
		CHECK_GL_ERROR("cBaseShader::GetUniFormLocationByName glGetUniformLocation( m_uiProgram,e_strName);");
		return l_ui;
	}

	void	cBaseShader::DebugRender()
	{
		//float	l_fTextureCoordinate[] ={0,1,1,0};
		//RenderQuadWithTextureAndColorAndCoordinate(0,0,0.f,cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y,Vector4::One,l_fTextureCoordinate,Vector3::Zero,L"");	
	}


	bool	cBaseShader::CreateProgram(const char*e_strVS,const char*e_strPS,bool e_bTexture)
	{
		if (m_uiProgram != -1)
		{
			glDeleteProgram(m_uiProgram);
		}
		m_uiProgram = glCreateProgram();
		std::wstring l_strProgramInfo = this->GetName();;
		l_strProgramInfo += L" program location:";
		l_strProgramInfo += ValueToStringW((uint64)m_uiProgram);;
		FMLog::LogWithFlag(l_strProgramInfo.c_str(), CORE_LOG_FLAG);
		if( CreateVS(e_strVS) )
		{
			if( CreateFS(e_strPS) )
			{
				// Attach them to the program.
				glAttachShader( m_uiProgram, m_uiVS );
				glAttachShader( m_uiProgram, m_uiFS );
				// can be deleted since the program will keep a reference
				glDeleteShader(m_uiVS);
				glDeleteShader(m_uiFS);
				// Link the whole program together.
				glLinkProgram( m_uiProgram );
				// Check for link success
				glUseProgram( m_uiProgram );
				if( FATMING_CORE::CheckProgram(m_uiProgram,GL_LINK_STATUS,L"link") )
				{
					//GL_INVALID_VALUE is generated if program is not a value generated by OpenGL.
					//GL_INVALID_OPERATION is generated if program is not a program object.
					//GL_INVALID_OPERATION is generated if program has not been successfully linked.
					if( e_bTexture )
					{//there are 
						m_uiTexLoacation = GetUniFormLocationByName("texSample" );
						if (m_uiTexLoacation != (unsigned int)-1)
						{
							std::wstring l_str = this->GetName();;
							l_str += L" tex location:";
							l_str += ValueToStringW((uint64)m_uiTexLoacation);;
							FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
							glUniform1i(m_uiTexLoacation, 0);
							CHECK_GL_ERROR("cBaseShader::CreateProgram glUniform1i(m_uiTexLoacation)");

						}
					}
					CHECK_GL_ERROR("cBaseShader::123");
					m_uiColorLoacation = GetUniFormLocationByName("Color");
					m_uiMatrixVPLoacation = GetUniFormLocationByName("matVP");
					m_uiMatrixWLoacation = GetUniFormLocationByName("matW" );
					m_uiBonesLocation = GetUniFormLocationByName("matBones" );
					m_uiPointSize = GetUniFormLocationByName("PointSize");

					for (int i = 0; i < TOTAL_FVF; ++i)
					{
						m_uiAttribArray[i] = glGetAttribLocation(m_uiProgram, g_strShaderAttribution[i]);
					}
					CHECK_GL_ERROR("cBaseShader::456");
					glLinkProgram( m_uiProgram );
					glUseProgram( m_uiProgram );
					this->Unuse();
					return true;
				}
			}
		}
		return false;
	}

	void	cBaseShader::Use(bool e_bUseLastWVPMatrix)
	{
		g_pCurrentShader = this;
		glUseProgram( m_uiProgram );
#if defined(WASM) || defined(UWP)
		m_uiTexLoacation = GetUniFormLocationByName("texSample");
		m_uiColorLoacation = GetUniFormLocationByName("Color");
		m_uiMatrixVPLoacation = GetUniFormLocationByName("matVP");
		m_uiMatrixWLoacation = GetUniFormLocationByName("matW");
		m_uiBonesLocation = GetUniFormLocationByName("matBones");
		m_uiPointSize = GetUniFormLocationByName("PointSize");
		CHECK_GL_ERROR("cBaseShader::Use get new iniform location");
		for (int i = 0; i<TOTAL_FVF; ++i)
			m_uiAttribArray[i] = glGetAttribLocation(m_uiProgram, g_strShaderAttribution[i]);
#endif
		//if u wanna to draw lines or else be careful the attribute array
		//0 for position,1 for UV,2 for color
		int	l_iIndex = 0;
		for( int i=0;i<TOTAL_FVF;++i )
		{
			if( m_uiAttribArray[i] == (unsigned int)-1 )
			{
				glDisableVertexAttribArray( l_iIndex );
				CHECK_GL_ERROR("cBaseShader::Use after glDisableVertexAttribArray");
				++l_iIndex;
			}
		}
		for( int i=0;i<TOTAL_FVF;++i )
		{
			if (m_uiAttribArray[i] != (unsigned int)-1)
			{
				glEnableVertexAttribArray(m_uiAttribArray[i]);
				CHECK_GL_ERROR("cBaseShader::Use after glEnableVertexAttribArray");
			}
			//else
				//glDisableVertexAttribArray( m_uiAttribArray[i] );
		}
		if( m_uiTexLoacation != (unsigned int)-1 )
		{
			//order is uniform,active,bind.
			glUniform1i( m_uiTexLoacation, 0 );
			glActiveTexture( GL_TEXTURE0  );
			CHECK_GL_ERROR("cBaseShader::Use glActiveTexture");
		}
		g_iMatrixVPLoacation =	m_uiMatrixVPLoacation;
		g_iMatrixWLoacation = m_uiMatrixWLoacation;
		g_iColorLoacation =	m_uiColorLoacation;
		g_iBonesMatrixoacation =	m_uiBonesLocation;
		g_iPointSizeLoacation = m_uiPointSize;

#if defined(WIN32) && !defined(UWP)
		if (g_iPointSizeLoacation != -1)
		{
			MyGLEnable(GL_PROGRAM_POINT_SIZE);
		}
		else
		{
			MyGLDisable(GL_PROGRAM_POINT_SIZE);
		}
#endif

		if (g_iColorLoacation != (unsigned int)-1)
		{
			SetupShaderColor(Vector4::One);
		}
		memcpy(g_uiAttribArray,m_uiAttribArray,sizeof(GLuint)*TOTAL_FVF);;
//#endif
		if(e_bUseLastWVPMatrix)
		{
			SetupShaderViewProjectionMatrix(g_fViewProjectionMatrix,false);
			SetupShaderWorldMatrix(g_fWorldMatrix);
		}
	}
	void cBaseShader::Unuse()
	{
		int	l_iIndex = 0;
		for (int i = 0; i < TOTAL_FVF; ++i)
		{
			if (m_uiAttribArray[i] != (unsigned int)-1)
			{
				glDisableVertexAttribArray(l_iIndex);
				CHECK_GL_ERROR("cBaseShader::unuse after glDisableVertexAttribArray");
				++l_iIndex;
			}
		}
		if (m_uiTexLoacation != (unsigned int)-1)
		{
			//glBindTexture(GL_TEXTURE_2D, 0);
		}
		glUseProgram(0);
		g_pCurrentShader = nullptr;
	}
	void	cBaseShader::Disable()
	{
		for( int i=0;i<TOTAL_FVF;++i )
		{
			if( m_uiAttribArray[i] != (unsigned int)-1 )
				glDisableVertexAttribArray( m_uiAttribArray[i] );
		}
		glUseProgram( 0 );
	}

	cNamedTypedObjectVector<cBaseShader>*g_pAll2DShaderList = nullptr;


	cBaseShader*	CreateShader(bool *e_pbClientState,wchar_t*e_strName)
	{
		cBaseShader*l_p2DShader = 0;
		if( !g_pAll2DShaderList )
		{
			g_pAll2DShaderList = new cNamedTypedObjectVector<cBaseShader>;
		}
		l_p2DShader = g_pAll2DShaderList->GetObject(e_strName);
		if (l_p2DShader)
		{
			return l_p2DShader;
		}
		glActiveTexture( GL_TEXTURE0  );
		l_p2DShader = new cBaseShader(e_strName,e_pbClientState[FVF_TEX0]);
		g_pAll2DShaderList->AddObject(l_p2DShader);
		return l_p2DShader;
	}
	cBaseShader*	CreateShader(bool *e_pbClientState,const char*e_strVS,const char*e_strPS,wchar_t*e_strName)
	{
		cBaseShader*l_p2DShader = 0;
		if( !g_pAll2DShaderList )
		{
			g_pAll2DShaderList = new cNamedTypedObjectVector<cBaseShader>;
		}
		l_p2DShader = g_pAll2DShaderList->GetObject(e_strName);
		if (l_p2DShader)
		{
			return l_p2DShader;
		}
		glActiveTexture( GL_TEXTURE0  );
		l_p2DShader = new cBaseShader(e_strVS,e_strPS,e_strName,e_pbClientState[FVF_TEX0]);
		g_pAll2DShaderList->AddObject(l_p2DShader);
		return l_p2DShader;
	}

	cBaseShader*	GetShader(const wchar_t*e_strName)
	{
		if( !g_pAll2DShaderList )
		{
			return 0;
		}
		else
		{
			return g_pAll2DShaderList->GetObject(e_strName);
		}
	}

	//void	ShaderUpdate(float e_fElpaseTime)
	//{
	//	if( g_pAll2DShaderList )
	//	{
	//		for(int i=0;i<g_pAll2DShaderList->Count();++i)
	//		{
	//			g_pAll2DShaderList->GetObject(i)->Update(e_fElpaseTime);
	//		}
	//	}		
	//}

	void	DeleteAllShader()
	{
		if( g_pAll2DShaderList )
		{
			while(g_pAll2DShaderList->Count())
			{
				g_pAll2DShaderList->RemoveObject(0);
			}
			SAFE_DELETE(g_pAll2DShaderList);
		}	
	}

	bool	DeleteShader(wchar_t*e_strName)
	{
		bool l_b = false;
		if( g_pAll2DShaderList )
		{
			if( g_pAll2DShaderList->GetObject(e_strName) )
				l_b = g_pAll2DShaderList->RemoveObject(e_strName);
			if( g_pAll2DShaderList->Count() == 0 )
				SAFE_DELETE(g_pAll2DShaderList);
		}
		return l_b;
	}

	cBaseShader*	GetCurrentShader()
	{
		return g_pCurrentShader;
	}
	void	UseShaderProgram(cBaseShader*e_p2DShader)
	{
		if( !e_p2DShader )
			return;
		if( g_pCurrentShader && e_p2DShader == g_pCurrentShader )
			return;
		UseShaderProgram(e_p2DShader->GetName());
	}
	//only call once before draw any 2D image
	void	UseShaderProgram(const wchar_t*e_strName,bool e_bUseLastWVPMatrix)
	{
		g_pCurrentUsingGLSLProgram = nullptr;
		if( g_pAll2DShaderList )
		{
			cBaseShader*l_p2DShader = g_pAll2DShaderList->GetObject(e_strName);
			if (l_p2DShader && g_pCurrentShader && g_pCurrentShader == l_p2DShader)
			{
				return;
			}
			if (l_p2DShader)
			{
				if (g_pCurrentShader)
				{
					g_pCurrentShader->Unuse();
				}
				l_p2DShader->Use(e_bUseLastWVPMatrix);
			}
//#ifdef DEBUG
//			else
//			{
//				std::wstring l_str = e_strName;
//				l_str += L"  shader is not exists!";
//				FMLog::LogWithFlag(l_str, CORE_LOG_FLAG);
//			}
//#endif
		}
	}

	//only call once if u wanna to disable it.
	void	DisableShaderProgram(wchar_t*e_strName)
	{
		if( g_pAll2DShaderList )
		{
			cBaseShader*l_p2DShader = g_pAll2DShaderList->GetObject(e_strName);
			if( l_p2DShader )
				l_p2DShader->Disable();
			g_pCurrentShader = nullptr;
		}
	}
	//void	SetupWorldViewProjectionMatrix(float*e_pfWVPMatrix)
	//{
	//
	//}
	float*	GetCurrentViewProjectionMatrix()
	{
		return g_fViewProjectionMatrix;
	}

	void SetShaderPointSize(float e_fSize)
	{
		CHECK_GL_ERROR("SetShaderPointSize");
		if(g_iPointSizeLoacation != -1)
			glUniform1f(g_iPointSizeLoacation, e_fSize);
		CHECK_GL_ERROR("after SetShaderPointSize");
	}

	void	SetupShaderViewProjectionMatrix(float*e_pfVPMatrix,bool e_bDoOrientation, eDeviceDirection e_eDeviceDirection)
	{
		if( e_bDoOrientation )
		{
			switch(cGameApp::m_spOpenGLRender->m_eDeviceDirection)
			{
				case eDD_PORTRAIT:
					memcpy(g_fViewProjectionMatrix,e_pfVPMatrix,sizeof(float)*16);
					break;
				case eDD_UPSIDE_DOWN:
					memcpy(g_fViewProjectionMatrix,cMatrix44::ZAxisRotationMatrix(D3DXToRadian(180.f))*cMatrix44(e_pfVPMatrix),sizeof(float)*16);
					break;
				case eDD_LANDSCAPE_LEFT://for 2D Y is invert so angle is -90
					memcpy(g_fViewProjectionMatrix,cMatrix44::ZAxisRotationMatrix(D3DXToRadian(90.f))*cMatrix44(e_pfVPMatrix),sizeof(float)*16);
					break;
				case eDD_LANDSCAPE_RIGHT://for 2D Y is invert so angle is 90
					memcpy(g_fViewProjectionMatrix,cMatrix44::ZAxisRotationMatrix(D3DXToRadian(-90.f))*cMatrix44(e_pfVPMatrix),sizeof(float)*16);
					break;
				default:
					break;
			}
		}
		else
			memcpy(g_fViewProjectionMatrix,e_pfVPMatrix,sizeof(float)*16);
#ifdef DEBUG
		int ll= g_iMatrixVPLoacation;
		CHECK_GL_ERROR("SetupShaderViewProjectionMatrix");
#endif
		glUniformMatrix4fv(g_iMatrixVPLoacation, 1, GL_FALSE, g_fViewProjectionMatrix);
		CHECK_GL_ERROR("SetupShaderViewProjectionMatrix after glUniformMatrix4fv");
	}


	void	SetupShaderWorldMatrix(float*e_pfWMatrix)
	{
		memcpy(g_fWorldMatrix,e_pfWMatrix,sizeof(float)*16);
		glUniformMatrix4fv( g_iMatrixWLoacation , 1, GL_FALSE, e_pfWMatrix);
#ifdef DEBUG
		cMatrix44 l_mat = g_fWorldMatrix;
		int ll = g_iMatrixWLoacation;
		CHECK_GL_ERROR("SetupShaderWorldMatrix");
#endif
	}

	void	SetupShaderColor(Vector4 e_vColor)
	{
		CHECK_GL_ERROR("SetupShaderColor");
		e_vColor = Vector4Multiply(e_vColor,g_vGlobalScaleColor);
		glUniform4fv(g_iColorLoacation, 1, (float*)&e_vColor);;
		CHECK_GL_ERROR("SetupShaderColor after glUniform4fv");
	}

	void	SetupShaderBonesData(cMatrix44*e_pBoneMatrices,int e_iCount)
	{
		glUniformMatrix4fv(g_iBonesMatrixoacation, e_iCount, GL_FALSE, (float*)e_pBoneMatrices);
		CHECK_GL_ERROR("SetupShaderBonesData");
	}

	bool CreateShader(const char* e_strShader, GLenum e_eShaderType, int& e_iOutProgramID)
	{
		e_iOutProgramID = glCreateShader(e_eShaderType);
		const char* l_strVS[1] = { e_strShader };
		// Pass our sources to OpenGL. Our sources are nullptr terminated, so pass nullptr for the lenghts.
		glShaderSource(e_iOutProgramID, 1, l_strVS, nullptr);

		// Compile that one object.
		glCompileShader(e_iOutProgramID);

		// Check for compilation success
		GLint compilationResult = 0;
		glGetShaderiv(e_iOutProgramID, GL_COMPILE_STATUS, &compilationResult);
		FATMING_CORE::CheckShader(e_iOutProgramID, GL_COMPILE_STATUS,L"");
		// current implementation always succeeds.
		// The error will happen at link time.
		if (compilationResult == 0)
		{
			FMLog::LogWithFlag(L"Failed to compile shader:", CORE_LOG_FLAG);
			glDeleteShader(e_iOutProgramID);
			e_iOutProgramID = -1;
			return false;
		}
		return true;
	}

	bool CheckProgram(int Object, int Type,wchar_t*e_strMessage)
	{
		int Success;
		glGetProgramiv(Object, Type, &Success);
		if(Success == GL_FALSE)
		{
	#ifdef DEBUG
			int InfoLogSize;
			glGetProgramiv(Object, GL_INFO_LOG_LENGTH, &InfoLogSize);
			char* Buffer = new char[InfoLogSize];
			glGetProgramInfoLog(Object, InfoLogSize, nullptr, Buffer);
			FMLog::LogWithFlag(Buffer, CORE_LOG_FLAG);
			FMLog::LogWithFlag(e_strMessage, CORE_LOG_FLAG);
			assert(0);
			delete[] Buffer;
			return false;
	#endif
		}
		return true;
	}
	//
	//CheckShader(e_uiShaderHandle, GL_COMPILE_STATUS,L"Compile\n");
	bool CheckShader(int Object, int Type, const wchar_t*e_strMessage,std::string*e_strOutputLog)
	{
		int Success;
		glGetShaderiv(Object, Type, &Success);
		if(Success == GL_FALSE)
		{
	#ifdef DEBUG
			int InfoLogSize = 0;
			glGetShaderiv(Object, GL_INFO_LOG_LENGTH, &InfoLogSize);
			char* Buffer = new char[InfoLogSize];
			glGetShaderInfoLog(Object, InfoLogSize, nullptr, Buffer);
			if( e_strOutputLog )
			{
				*e_strOutputLog = Buffer;
			}
			else
			{
				FMLog::LogWithFlag(e_strMessage, CORE_LOG_FLAG);
				FMLog::LogWithFlag(Buffer, CORE_LOG_FLAG);
				if (e_strMessage)
				{
					UT::ErrorMsg(Buffer, WcharToChar(e_strMessage).c_str());
				}
				assert(0);			
			}
			delete[] Buffer;
	#endif        
			return false;
		}
		return true;
	}

	void	SetGlobalScaleColor(Vector4 e_vColor)
	{
		g_vGlobalScaleColor	= e_vColor;
	}
}