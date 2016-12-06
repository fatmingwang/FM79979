#include "../stdafx.h"
#include "GLSLUiniform.h"
#include "Shader.h"
#include "../GameplayUT/GameApp.h"
GLenum	g_iDrawindiceType = 

#if defined(WIN32) || defined(LINUX)
GL_UNSIGNED_INT;
#else
GL_UNSIGNED_SHORT;
#endif
namespace FATMING_CORE
{
	GLuint	g_iMatrixVPLoacation;
	GLuint	g_iMatrixWLoacation;
	GLuint	g_iTexLoacation[8];//max support 8 texture
	GLuint	g_iBonesMatrixoacation;

	GLuint	g_iColorLoacation;
	GLuint	g_uiAttribArray[TOTAL_FVF];

	float	g_fViewProjectionMatrix[16];
	float	g_fWorldMatrix[16];
	Vector4	g_vGlobalScaleColor(1,1,1,1);
	cBaseShader*g_pCurrentShader = 0;
	extern cGLSLProgram*g_pCurrentUsingGLSLProgram;
	bool	g_bShowErrorMessageBoxIfShaderIsWrong = true;
	cBaseShader::cBaseShader(const wchar_t*e_strName,bool e_bTexture)
	{
		g_pCurrentShader = nullptr;
		m_bDataUpdated = false;
		m_uiProgram = -1;
		this->SetName(e_strName);
		m_bTexture = e_bTexture;
		m_uiTexLoacation = m_uiColorLoacation = m_uiMatrixVPLoacation = m_uiMatrixWLoacation = -1;
		memset(m_uiAttribArray,-1,sizeof(GLuint)*TOTAL_FVF);
		bool	l_b = CreateProgram(e_bTexture?g_strCommonVS:g_strCommonVSNoTexture,
									e_bTexture?g_strCommonFS:g_strCommonFSNoTexture,
									e_bTexture);
		assert(g_bShowErrorMessageBoxIfShaderIsWrong?l_b:true);
	}

	cBaseShader::cBaseShader(const char*e_strVS,const char*e_strPS,const wchar_t*e_strName,bool e_bTexture)
	{
		g_pCurrentShader = nullptr;
		m_bDataUpdated = false;
		m_uiProgram = -1;
		this->SetName(e_strName);
		m_bTexture = e_bTexture;
		m_uiTexLoacation = m_uiColorLoacation = m_uiMatrixVPLoacation = m_uiMatrixWLoacation = -1;
		memset(m_uiAttribArray,-1,sizeof(GLuint)*TOTAL_FVF);
		bool	l_b = CreateProgram(e_strVS,e_strPS,e_bTexture);
		assert(g_bShowErrorMessageBoxIfShaderIsWrong?l_b:true);
	}
	cBaseShader::cBaseShader()
	{
		g_pCurrentShader = nullptr;
		m_bDataUpdated = false;
		m_uiProgram = -1;
		memset(m_uiAttribArray,-1,sizeof(GLuint)*TOTAL_FVF);
	}
	cBaseShader::cBaseShader(const wchar_t*e_strName,bool *e_pbClientState)
	{
		g_pCurrentShader = nullptr;
		m_bDataUpdated = false;
		m_uiProgram = -1;
		this->SetName(e_strName);
		if( e_pbClientState != nullptr )
		{
			for( int i=0;i<TOTAL_FVF;++i )
				m_uiAttribArray[i] = e_pbClientState[i]?1:-1;
		}
	}

	cBaseShader::~cBaseShader()
	{
		glDeleteShader(m_uiVS);
		glDeleteShader(m_uiFS);
		glDeleteProgram( m_uiProgram );
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
		  cGameApp::OutputDebugInfoString(L"Failed to compile shader:\n");
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
		  cGameApp::OutputDebugInfoString(L"Failed to compile shader:\n");
		  return false;
	   }
	   return true;
	}

	GLuint	cBaseShader::GetUniFormLocationByName(const char*e_strName)
	{
		MyGlErrorTest();
		GLuint	l_ui = glGetUniformLocation( m_uiProgram,e_strName);
		MyGlErrorTest();
		return l_ui;
	}

	void	cBaseShader::DebugRender()
	{
		float	l_fTextureCoordinate[] ={0,1,1,0};
		DrawQuadWithTextureAndColorAndCoordinate(0,0,0.f,cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,Vector4::One,l_fTextureCoordinate,Vector3::Zero,L"");	
	}


	bool	cBaseShader::CreateProgram(const char*e_strVS,const char*e_strPS,bool e_bTexture)
	{
		m_uiProgram = glCreateProgram();
		if( CreateVS(e_strVS) )
		{
			if( CreateFS(e_strPS) )
			{
				// Attach them to the program.
				glAttachShader( m_uiProgram, m_uiVS );
				glAttachShader( m_uiProgram, m_uiFS );
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
						if( m_uiTexLoacation != 0xffffffff )
							glUniform1i( m_uiTexLoacation, 0 );
					}
					
					m_uiColorLoacation = GetUniFormLocationByName("Color");
					m_uiMatrixVPLoacation = GetUniFormLocationByName("matVP");
					m_uiMatrixWLoacation = GetUniFormLocationByName("matW" );
					m_uiBonesLocation = GetUniFormLocationByName("matBones" );

					for(int i=0;i<TOTAL_FVF;++i)
						m_uiAttribArray[i] = glGetAttribLocation(m_uiProgram, g_strShaderAttribution[i]);

					glLinkProgram( m_uiProgram );
					glUseProgram( m_uiProgram );
					return true;
				}
			}
		}
		return false;
	}

	void	cBaseShader::Use(bool e_bUseLastWVPMatrix)
	{
		g_pCurrentShader = this;
		MyGlErrorTest();
#ifndef OPENGLES_2_X
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		if(m_uiAttribArray[FVF_TEX0] != -1)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnable(GL_TEXTURE_2D);
		}
		else
		{
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisable(GL_TEXTURE_2D);
		}
		if(m_uiAttribArray[FVF_NORMAL] != -1)
			glEnableClientState(GL_NORMAL_ARRAY);
		else
			glDisableClientState(GL_NORMAL_ARRAY);

		if(m_uiAttribArray[FVF_DIFFUSE]	!= -1)
			glEnableClientState(GL_COLOR_ARRAY);
		else
			glDisableClientState(GL_COLOR_ARRAY);
#else
		MyGlErrorTest();
		glUseProgram( m_uiProgram );
		MyGlErrorTest();
		//if u wanna to draw lines or else be careful the attribute array
		//0 for position,1 for UV,2 for color
		int	l_iIndex = 0;
		for( int i=0;i<TOTAL_FVF;++i )
		{
			if( m_uiAttribArray[i] == -1 )
			{
				glDisableVertexAttribArray( l_iIndex );
				++l_iIndex;
			}
		}
		for( int i=0;i<TOTAL_FVF;++i )
		{
			if( m_uiAttribArray[i] != -1 )
				glEnableVertexAttribArray( m_uiAttribArray[i] );
			//else
				//glDisableVertexAttribArray( m_uiAttribArray[i] );
		}
		MyGlErrorTest();
		if( m_uiTexLoacation != -1 )
		{
			//order is uniform,active,bind.
			glUniform1i( m_uiTexLoacation, 0 );
			glActiveTexture( GL_TEXTURE0  );
		}
		g_iMatrixVPLoacation =	m_uiMatrixVPLoacation;
		g_iMatrixWLoacation = m_uiMatrixWLoacation;
		g_iColorLoacation =	m_uiColorLoacation;
		g_iBonesMatrixoacation =	m_uiBonesLocation;
		if( g_iColorLoacation != -1 )
			SetupShaderColor(Vector4::One);
		memcpy(g_uiAttribArray,m_uiAttribArray,sizeof(GLuint)*TOTAL_FVF);;
#endif
		if(e_bUseLastWVPMatrix)
		{
			SetupShaderViewProjectionMatrix(g_fViewProjectionMatrix,false);
			SetupShaderWorldMatrix(g_fWorldMatrix);
		}
	}
	void	cBaseShader::Disable()
	{
#ifdef OPENGLES_2_X
		for( int i=0;i<TOTAL_FVF;++i )
		{
			if( m_uiAttribArray[i] != -1 )
				glDisableVertexAttribArray( m_uiAttribArray[i] );
		}
		glUseProgram( 0 );
#endif
	}

	cNamedTypedObjectVector<cBaseShader>*g_pAll2DShaderList = 0;


	cBaseShader*	CreateShader(bool *e_pbClientState,wchar_t*e_strName)
	{
		cBaseShader*l_p2DShader = 0;
		if( !g_pAll2DShaderList )
		{
			g_pAll2DShaderList = new cNamedTypedObjectVector<cBaseShader>;
		}
		l_p2DShader = g_pAll2DShaderList->GetObject(e_strName);
		if( l_p2DShader )
			return l_p2DShader;
#ifdef OPENGLES_2_X
		glActiveTexture( GL_TEXTURE0  );
		l_p2DShader = new cBaseShader(e_strName,e_pbClientState[FVF_TEX0]);
#else
		l_p2DShader = new cBaseShader(e_strName,e_pbClientState);
		glEnableClientState(GL_VERTEX_ARRAY);
#endif
		g_pAll2DShaderList->AddObject(l_p2DShader);
		return l_p2DShader;
	}
	cBaseShader*	CreateShader(bool *e_pbClientState,const char*e_strVS,const char*e_strPS,wchar_t*e_strName)
	{
#ifndef OPENGLES_2_X
		glEnableClientState(GL_VERTEX_ARRAY);
#endif
		cBaseShader*l_p2DShader = 0;
		if( !g_pAll2DShaderList )
		{
			glActiveTexture( GL_TEXTURE0  );
			g_pAll2DShaderList = new cNamedTypedObjectVector<cBaseShader>;
		}
		l_p2DShader = g_pAll2DShaderList->GetObject(e_strName);
		if( l_p2DShader )
			return l_p2DShader;
#ifdef OPENGLES_2_X
		glActiveTexture( GL_TEXTURE0  );
		l_p2DShader = new cBaseShader(e_strVS,e_strPS,e_strName,e_pbClientState[FVF_TEX0]);
#else
		l_p2DShader = new cBaseShader(e_strName,e_pbClientState);
		glEnableClientState(GL_VERTEX_ARRAY);
#endif
		g_pAll2DShaderList->AddObject(l_p2DShader);
		return l_p2DShader;
	}

	cBaseShader*	GetShader(wchar_t*e_strName)
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

	void	ShaderUpdate(float e_fElpaseTime)
	{
		if( g_pAll2DShaderList )
		{
			for(int i=0;i<g_pAll2DShaderList->Count();++i)
			{
				g_pAll2DShaderList->GetObject(i)->Update(e_fElpaseTime);
			}
		}		
	}

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
			if( l_p2DShader&&g_pCurrentShader && g_pCurrentShader == l_p2DShader )
				return;
			if( l_p2DShader )
				l_p2DShader->Use(e_bUseLastWVPMatrix);
//#ifdef DEBUG
//			else
//			{
//				std::wstring l_str = e_strName;
//				l_str += L"  shader is not exists!";
//				cGameApp::OutputDebugInfoString(l_str);
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
	void	SetupShaderViewProjectionMatrix(float*e_pfVPMatrix,bool e_bDoOrientation)
	{
		if( e_bDoOrientation )
		{
			switch(cGameApp::m_seDeviceDirection)
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
			}
		}
		else
			memcpy(g_fViewProjectionMatrix,e_pfVPMatrix,sizeof(float)*16);
#ifdef DEBUG
		int ll= g_iMatrixVPLoacation;
		MyGlErrorTest();
#endif
#ifdef OPENGLES_2_X
		glUniformMatrix4fv( g_iMatrixVPLoacation, 1, GL_FALSE, g_fViewProjectionMatrix);
#else
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(g_fViewProjectionMatrix);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
#endif
		MyGlErrorTest();
	}


	void	SetupShaderWorldMatrix(float*e_pfWMatrix)
	{
		memcpy(g_fWorldMatrix,e_pfWMatrix,sizeof(float)*16);
#ifdef OPENGLES_2_X
		glUniformMatrix4fv( g_iMatrixWLoacation , 1, GL_FALSE, e_pfWMatrix);
#else
		glLoadMatrixf(e_pfWMatrix);
#endif
#ifdef DEBUG
		cMatrix44 l_mat = g_fWorldMatrix;
		int ll = g_iMatrixWLoacation;
		MyGlErrorTest();
#endif
	}

	void	SetupShaderColor(Vector4 e_vColor)
	{
		MyGlErrorTest();
		e_vColor = Vector4Multiply(e_vColor,g_vGlobalScaleColor);
#ifdef OPENGLES_2_X
		glUniform4fv( g_iColorLoacation,1, (float*)&e_vColor);;
#else
		glColor4f(e_vColor.x,e_vColor.y,e_vColor.z,e_vColor.w);
#endif
#ifdef DEBUG
		MyGlErrorTest();
#endif
	}

	void	SetupShaderBonesData(cMatrix44*e_pBoneMatrices,int e_iCount)
	{
#ifdef OPENGLES_2_X
		glUniformMatrix4fv( g_iBonesMatrixoacation , e_iCount, GL_FALSE, (float*)e_pBoneMatrices);
#else

#endif
#ifdef DEBUG
		MyGlErrorTest();
#endif
	}

	void	MY_GLDRAW_ARRAYS(GLenum mode, GLint first, GLsizei count)
	{
#ifdef DEBUG
		MyGlErrorTest();
#endif
		glDrawArrays(mode,first,count);
#ifdef DEBUG
		MyGlErrorTest();
#endif
	}

	void	MY_GLDRAW_ELEMENTS(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
	{
		glDrawElements(mode,count,type,indices);
		//GL_NO_ERROR 0
		//GL_INVALID_ENUM 0x0500
		//GL_INVALID_VALUE 0x0501
		//GL_INVALID_OPERATION 0x0502
		int	l_i = glGetError();
		if( l_i != 0 )
		{
			bool	l_b0 = glIsEnabled(GL_VERTEX_ARRAY)==GL_TRUE;
			bool	l_b1 = glIsEnabled(GL_TEXTURE_COORD_ARRAY)==GL_TRUE;
			bool	l_b2 = glIsEnabled(GL_NORMAL_ARRAY)==GL_TRUE;
			bool	l_b3 = glIsEnabled(GL_COLOR_ARRAY)==GL_TRUE;
			int a=0;
		}
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
			cGameApp::OutputDebugInfoString(UT::CharToWchar(Buffer));
			cGameApp::OutputDebugInfoString(e_strMessage);
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
			int InfoLogSize;
			glGetShaderiv(Object, GL_INFO_LOG_LENGTH, &InfoLogSize);
			char* Buffer = new char[InfoLogSize];
			glGetShaderInfoLog(Object, InfoLogSize, nullptr, Buffer);
			if( e_strOutputLog )
			{
				*e_strOutputLog = Buffer;
			}
			else
			{
				cGameApp::OutputDebugInfoString(e_strMessage);
				cGameApp::OutputDebugInfoString(UT::CharToWchar(Buffer));
				UT::ErrorMsg(Buffer,WcharToChar(e_strMessage).c_str());
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