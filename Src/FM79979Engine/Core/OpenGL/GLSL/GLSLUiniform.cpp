#include "../stdafx.h"
//#include "../XML/XMLLoader.h"
#include "GLSLUiniform.h"

#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <assert.h>
#include "../GameplayUT/GameApp.h"
using namespace std;
namespace FATMING_CORE
{
	cGLSLProgram*g_pCurrentUsingGLSLProgram = 0;
	extern cBaseShader*g_pCurrentShader;
	TYPDE_DEFINE_MARCO(cUniformManager);
	//=================================================================================================================================
	///
	/// Constructor
	///
	/// \param none
	///
	/// \return null
	//=================================================================================================================================
	cUniformData::cUniformData() :
	   m_iLocation(-1),
	   m_strName(nullptr),
	   m_eDatatype(0)
	{
	}

	//=================================================================================================================================
	///
	/// Destructor
	///
	/// \param none
	///
	/// \return null
	//=================================================================================================================================
	cUniformData::~cUniformData()   
	{
	   if ( m_strName != nullptr )
	   {
		  delete [] m_strName;
		  m_strName = nullptr;
	   }
	}


	cUniformManager::cUniformManager():ISAXCallback(true)
	{
		m_uiRecentProgramHandle = 0;
	}

	cUniformManager::~cUniformManager()
	{

	}

	//<GLSL ProgramName="mainProg" VS="data/vertex.glsl" FS="data/fragment.glsl" >
	//	<!-- sample 1D,2D,3D -->
	//	<Texture Name="tex0" samplerDimesion="2" />
	//	<!-- attribute enusude u call the vertexpointer and setup index to cooresponded-->
	//	<Attribute Name="position" Index="0">
	//	<Attribute Name="texture" Index="1">
	//	<Attribute Name="normal" Index="2">
	//	<!-- uniform -->
	//	<Uniform Name="MatrixVP" Type="float44" Data="1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f,"/>
	//	<Uniform Name="LightPos" Type="float3" Data="0.f,0.f,0.f"/>	
	//<HLSL/>
	void	cUniformManager::ProcessGLSLData()
	{
		//char	l_strShaderFileName[MAX_PATH];
		cGLSLProgram*l_pGLSLProgram = new cGLSLProgram();
		m_pCurrentGLSLProgram = l_pGLSLProgram;
		m_pCurrentUniformData->m_eDatatype = SAMPLE_PROGRAM;
		// Create the one program we are going to use.//ensure glewInit is called
		m_uiRecentProgramHandle = glCreateProgram();
		m_pCurrentGLSLProgram->SetProgramHandle(m_uiRecentProgramHandle);
		PARSE_CURRENT_ELEMENT_START
			COMPARE_NAME("ProgramName")
			{
				int	l_iLength = (int)wcslen(l_strValue);
				m_pCurrentUniformData->m_strName = new char [l_iLength+1];
				sprintf(m_pCurrentUniformData->m_strName,"%s\0",UT::WcharToChar(l_strValue).c_str());
				m_pCurrentGLSLProgram->SetName(UT::CharToWchar(m_pCurrentUniformData->m_strName));
			}
			else
			COMPARE_NAME("VS")
			{
				m_pCurrentGLSLProgram->m_uiVertShaderHandle = glCreateShader( GL_VERTEX_SHADER );
				LoadShaderObject( UT::WcharToChar(l_strValue).c_str(), m_pCurrentGLSLProgram->m_uiVertShaderHandle );
			}
			else
			COMPARE_NAME("FS")
			{
				m_pCurrentGLSLProgram->m_uiFragShaderHandle = glCreateShader( GL_FRAGMENT_SHADER );
				LoadShaderObject( UT::WcharToChar(l_strValue).c_str(), m_pCurrentGLSLProgram->m_uiFragShaderHandle );
			}
		PARSE_NAME_VALUE_END
		// Attach them to the program.
		glAttachShader( m_uiRecentProgramHandle, m_pCurrentGLSLProgram->m_uiVertShaderHandle );
		glAttachShader( m_uiRecentProgramHandle, m_pCurrentGLSLProgram->m_uiFragShaderHandle );
		// Link the whole program together.
		glLinkProgram( m_uiRecentProgramHandle );
		// Check for link success
		glUseProgram( m_uiRecentProgramHandle );
		this->m_pCurrentGLSLProgram->UpdateAllUniforms();
		CheckProgram(m_uiRecentProgramHandle,GL_LINK_STATUS,L"link");
	}

	//void	cUniformManager::ProcessTextureData()
	//{
	////	<Texture Name="tex0" SamplerDimesion="2" Unit="0" />
	//	PARSE_CURRENT_ELEMENT_START
	//		COMPARE_NAME("Name")
	//		{
	//			int	l_iLength = (int)wcslen(l_strValue);
	//			m_pCurrentUniformData->m_strName = new char [l_iLength+1];
	//			sprintf(m_pCurrentUniformData->m_strName,"%s\0",UT::WcharToChar(l_strValue).c_str());
	//		}
	//		else
	//		COMPARE_NAME("SamplerDimesion")//1D,2D,3D
	//		{
	//	
	//		}
	//		else
	//		COMPARE_NAME("Unit")
	//		{
	//			m_pCurrentUniformData->m_uiTextureUnit = (UINT)_wtoi(l_strValue);
	//		}
	//	PARSE_NAME_VALUE_END
	//	m_pCurrentUniformData->m_eDatatype = SAMPLE_INT;
	//	//u have to assign texture ID
	//	m_pCurrentUniformData->m_uiTextureHandle = -1;
	//	glUseProgram(m_uiRecentProgramHandle);
	//	m_pCurrentUniformData->m_iLocation = glGetUniformLocation( m_uiRecentProgramHandle,"tex0" );
	//	assert(m_pCurrentUniformData->m_iLocation!=-1);
	//	//http://www.gamedev.net/community/forums/topic.asp?topic_id=516840
	//	//Because half of your shader doesn't do anything, and is optimized
	//	//away by the compiler. The uniforms don't exist anymore after compilation 
	//	//(because they aren't used) and will therefore not be found by glGetUniformLocation.
	//	//Unless you are accessing them by a pixel shader. But since you didn't post any, my 
	//	//first assumption would be this.
	//	glUniform1i( m_pCurrentUniformData->m_iLocation, m_pCurrentUniformData->m_uiTextureUnit );
	//}

	void	cUniformManager::ProcessAttributeData()
	{
		std::wstring	l_strAttributeName;
		int	l_iIndex = -1;
		PARSE_CURRENT_ELEMENT_START
			COMPARE_NAME("Name")
			{
				l_strAttributeName = l_strValue;
				l_iIndex = glGetUniformLocation( m_uiRecentProgramHandle,UT::WcharToChar(l_strValue).c_str());
				MyGlErrorTest("cUniformManager::ProcessAttributeData  Name");
			}
		PARSE_NAME_VALUE_END
		assert(l_iIndex!=-1);
		this->m_pCurrentGLSLProgram->AddAttributeData(UT::WcharToChar(l_strAttributeName.c_str()),l_iIndex);
		SAFE_DELETE(m_pCurrentUniformData);
	}

	void	cUniformManager::ProcessUniformData()
	{
		PARSE_CURRENT_ELEMENT_START
			COMPARE_NAME("Name")
			{
				int	l_iLength = (int)wcslen(l_strValue);
				m_pCurrentUniformData->m_strName = new char [l_iLength+1];
				sprintf(m_pCurrentUniformData->m_strName,"%s\0",UT::WcharToChar(l_strValue).c_str());
				this->m_pCurrentUniformData->m_iLocation = glGetUniformLocation( m_uiRecentProgramHandle, UT::WcharToChar(l_strValue).c_str() );
				//http://www.gamedev.net/community/forums/topic.asp?topic_id=516840
				//Because half of your shader doesn't do anything, and is optimized
				//away by the compiler. The uniforms don't exist anymore after compilation 
				//(because they aren't used) and will therefore not be found by glGetUniformLocation.
				//Unless you are accessing them by a pixel shader. But since you didn't post any, my 
				//first assumption would be this.
				assert(this->m_pCurrentUniformData->m_iLocation != -1);
			}
			else
			COMPARE_NAME_WITH_DEFINE(DATA_TYPE)
			{
				COMPARE_VALUE_WITH_DEFINE(UNIFORM_DATA_TYPE_cMatrix44)
				{
					m_pCurrentUniformData->m_eDatatype = SAMPLE_FLOAT_MAT4;
				}
				else
				COMPARE_VALUE_WITH_DEFINE(UNIFORM_DATA_TYPE_FLOAT1)
				{
					m_pCurrentUniformData->m_eDatatype = SAMPLE_FLOAT;
				}
				else
				COMPARE_VALUE_WITH_DEFINE(UNIFORM_DATA_TYPE_FLOAT2)
				{
					m_pCurrentUniformData->m_eDatatype = SAMPLE_FLOAT_VEC2;
				}
				else
				COMPARE_VALUE_WITH_DEFINE(UNIFORM_DATA_TYPE_FLOAT3)
				{
					m_pCurrentUniformData->m_eDatatype = SAMPLE_FLOAT_VEC3;
				}
				else
				COMPARE_VALUE_WITH_DEFINE(UNIFORM_DATA_TYPE_FLOAT4)
				{
					m_pCurrentUniformData->m_eDatatype = SAMPLE_FLOAT_VEC4;
				}
				else
				COMPARE_VALUE_WITH_DEFINE(UNIFORM_DATA_TYPE_INT)
				{
					m_pCurrentUniformData->m_eDatatype = SAMPLE_INT;
				}
				else
				{
					UT::ErrorMsg((wchar_t*)l_strValue,L"there is no such type");
				}
			}
			else
			COMPARE_NAME("Data")
			{
				switch(m_pCurrentUniformData->m_eDatatype)
				{
					case SAMPLE_FLOAT:
						this->m_pCurrentUniformData->m_fData[0] = (float)_wtof(l_strValue);
						glUniform1f( m_pCurrentUniformData->m_iLocation, this->m_pCurrentUniformData->m_fData[0] );
						break;
					case SAMPLE_FLOAT_VEC2:
					{
						Vector2	l_v2 = GetVector2((char*)l_strValue);
						memcpy(this->m_pCurrentUniformData->m_fData,&l_v2,sizeof(float)*2);
						glUniform2fv( m_pCurrentUniformData->m_iLocation,1, &this->m_pCurrentUniformData->m_fData[0] );
					}
						break;
					case SAMPLE_FLOAT_VEC3:
					{
						Vector3	l_v3 = GetVector3((char*)l_strValue);
						memcpy(this->m_pCurrentUniformData->m_fData,&l_v3,sizeof(float)*3);
						glUniform3fv( m_pCurrentUniformData->m_iLocation,1, &this->m_pCurrentUniformData->m_fData[0] );
					}
						break;
					case SAMPLE_FLOAT_VEC4:
					{
						Vector4	l_v4 = GetVector4((char*)l_strValue);
						memcpy(this->m_pCurrentUniformData->m_fData,&l_v4,sizeof(float)*4);
						glUniform4fv( m_pCurrentUniformData->m_iLocation,1, &this->m_pCurrentUniformData->m_fData[0] );
					}
						break;
					case SAMPLE_INT:
						this->m_pCurrentUniformData->m_iData[0] = _wtoi(l_strValue);
						glUniform1i( m_pCurrentUniformData->m_iLocation, this->m_pCurrentUniformData->m_iData[0] );
						break;
					case SAMPLE_FLOAT_MAT4:
					{
						cMatrix44	l_mat = GetMatrix((char*)l_strValue);
						memcpy(this->m_pCurrentUniformData->m_fData,&l_mat,sizeof(float)*16);
						glUniformMatrix4fv( m_pCurrentUniformData->m_iLocation, 1, GL_FALSE, &m_pCurrentUniformData->m_fData[0] );
					}
						break;
				}
			}
		PARSE_NAME_VALUE_END
	}

	void	cUniformManager::HandleElementData(TiXmlElement*e_pTiXmlElement)
	{
		if( !e_pTiXmlElement->m_bDone )
		{
			cUniformData *l_pNewUniform = new cUniformData;
			//memset( l_pNewUniform, 0, sizeof(cUniformData) );
			m_pCurrentUniformData = l_pNewUniform;
			const wchar_t*	l_strName = e_pTiXmlElement->Value();
			COMPARE_NAME("GLSL")
			{
				ProcessGLSLData();
			}
			//else
			//COMPARE_NAME("Texture")
			//{
			//	ProcessTextureData();
			//}
			else
			COMPARE_NAME("Attribute")
			{
				ProcessAttributeData();
			}
			else
			COMPARE_NAME("Uniform")
			{
				ProcessUniformData();
			}
			else
			{
				assert(0);
			}
			assert( m_uiRecentProgramHandle != 0);
			if( m_pCurrentUniformData )
			{
				assert(m_pCurrentUniformData->m_strName);
				m_pCurrentGLSLProgram->AddUniform(m_pCurrentUniformData);
			}
		}
		else
		{
			const wchar_t*	l_strName = e_pTiXmlElement->Value();
			COMPARE_NAME("GLSL")
			{
				glLinkProgram( m_uiRecentProgramHandle );
				CheckProgram(m_uiRecentProgramHandle,GL_LINK_STATUS,L"link");
				m_pCurrentGLSLProgram->UpdateAllUniforms();
				this->AddObject(m_pCurrentGLSLProgram);
				m_uiRecentProgramHandle = 0;
			}
		}
	}
	//=================================================================================================================================
	///
	/// Loads all the shader objects.
	///
	/// \param fileName is the name for the file where we get objects
	/// \param stateHandle is the ES handle to different types of shaders
	///
	/// \return bool saying whether we passed or failed
	//=================================================================================================================================
	bool LoadShaderObject( const char* e_strFileName, GLuint e_uiShaderHandle )
	{
		char* source = nullptr;
		{
		  // Use file io to load the code of the shader.
		  std::ifstream fp( e_strFileName , ios_base::binary );
		  if( fp.fail() ) 
		  {
	#ifdef WIN32
			 FMLog::LogWithFlag(L"Failed to open shader file:", CORE_LOG_FLAG);
			 FMLog::LogWithFlag(UT::CharToWchar(e_strFileName), CORE_LOG_FLAG);
	#endif
			 assert(0);
			 return false;
		  }

		  fp.seekg( 0, std::ios_base::end );
		  const long len = (const long)fp.tellg();
		  fp.seekg( 0, std::ios_base::beg );
		  // The +1 is optional, depending on how you call glShaderSourceARB
		  source = new char[len+1];
		  fp.read(source, sizeof(char)*len);
		  source[len] = 0;
		}

		const char* gls[1] = { source };

		// Pass our sources to OpenGL. Our sources are nullptr terminated, so pass nullptr for the lenghts.
		glShaderSource( e_uiShaderHandle, 1, gls, nullptr );
		// OpenGL made a copy. Do not need the source anymore.
		delete[] source;
		source = 0;

		// Compile that one object.
		glCompileShader(e_uiShaderHandle);

		// Check for compilation success
		GLint compilationResult = 0;
		glGetShaderiv( e_uiShaderHandle, GL_COMPILE_STATUS, &compilationResult );
		CheckShader(e_uiShaderHandle, GL_COMPILE_STATUS,L"Compile\n");
	   // current implementation always succeeds.
	   // The error will happen at link time.
	   if ( compilationResult == 0 )
	   {
	#ifdef WIN32
		  FMLog::LogWithFlag(L"Failed to compile shader:", CORE_LOG_FLAG);
		  FMLog::LogWithFlag(UT::CharToWchar(e_strFileName), CORE_LOG_FLAG);
	#endif
		  return false;
	   }
	   return true;
	}

	cUniformData*	cGLSLProgram::GetUniform(char*e_strName)
	{
	   for ( cUniformDataIterator index = m_Uniforms.begin(); index != m_Uniforms.end() ; ++index)
	   {
		  cUniformData* current = *index;
		  if ( strcmp( e_strName, current->m_strName ) == 0 )
			  return current;
	   }
	   return 0;
	}
	//=================================================================================================================================
	///
	/// Updates the value of a uniform
	///
	/// \param name - The name we gave to the uniform
	/// \param vals - An array of values we want to to replace the current uniform values with
	///
	/// \return true=pass ... false=fail
	//=================================================================================================================================
	bool cGLSLProgram::UpdateOneUniform( const char* name, float* vals )
	{
	   bool updated = false;
	   cUniformDataIterator index;

	   for ( index = m_Uniforms.begin(); index != m_Uniforms.end() ; ++index)
	   {
		  cUniformData* current = *index;

		  if ( strcmp( name, current->m_strName ) == 0 )
		  {
			 updated = true;

			 switch( current->m_eDatatype )
			 {
			 case SAMPLE_FLOAT:
				memcpy( current->m_fData, vals, sizeof(float) * 1 );
				glUniform1f( current->m_iLocation, current->m_fData[0] );
				break;
			 case SAMPLE_FLOAT_VEC2:
				memcpy( current->m_fData, vals, sizeof(float) * 2 );
				glUniform2f( current->m_iLocation, current->m_fData[0], current->m_fData[1] );
				break;
			 case SAMPLE_FLOAT_VEC3:
				memcpy( current->m_fData, vals, sizeof(float) * 3 );
				glUniform3f( current->m_iLocation, current->m_fData[0], current->m_fData[1], current->m_fData[2] );
				break;
			 case SAMPLE_FLOAT_VEC4:
				memcpy( current->m_fData, vals, sizeof(float) * 4 );
				glUniform4f( current->m_iLocation, current->m_fData[0], current->m_fData[1], current->m_fData[2], current->m_fData[3] );
				break;
			 case SAMPLE_FLOAT_MAT4:
				memcpy( current->m_fData, vals, sizeof(float) * 16 );
				glUniformMatrix4fv( current->m_iLocation, 1, GL_FALSE, &current->m_fData[0] );
				break;
			 default:
				assert(0);
				break;
			 }
		  }

		  if ( updated )
		  {
			 break;
		  }
	   }
	   
	   assert( updated == true ); // They probably passed in un unsupported type or invalid name
	   return updated;
	}

	//=================================================================================================================================
	///
	/// Deletes all the GL resources that we have loaded
	///
	/// \param name - The name we gave to the program
	///
	/// \return true=pass ... false=fail
	//=================================================================================================================================
	void cGLSLProgram::FreeAllData()
	{
		cUniformDataIterator index;
		for ( index = m_Uniforms.begin(); index != m_Uniforms.end() ; ++index)
		{
		  if ( m_uiVertShaderHandle )
		  {
			 glDeleteShader( m_uiVertShaderHandle );
		  }
		  if ( m_uiFragShaderHandle )
		  {
			 glDeleteShader( m_uiFragShaderHandle );
		  }
		}

		for ( UINT i = 0; i < m_Uniforms.size() ; i++ )
		{
		  if ( m_Uniforms[i] != nullptr )
		  {
			 delete m_Uniforms[i];
		  }
		}
		m_Uniforms.clear();
		glDeleteProgram( m_uiProgramHandle );
	}

	//=================================================================================================================================
	///
	/// Updates all the uniform data after a link
	///
	/// \param void
	///
	/// \return void
	//=================================================================================================================================
	void cGLSLProgram::UpdateAllUniforms()
	{
	   cUniformDataIterator index;

	   for ( index = m_Uniforms.begin(); index != m_Uniforms.end() ; ++index)
	   {
		 //GLenum	l_en = (*index)->m_eDatatype;
		 switch( (*index)->m_eDatatype )
		 {
		 case SAMPLE_FLOAT:
			glUniform1f( (*index)->m_iLocation, (*index)->m_fData[0] );
			break;
		 case SAMPLE_FLOAT_VEC2:
			glUniform2f( (*index)->m_iLocation, (*index)->m_fData[0], (*index)->m_fData[1] );
			break;
		 case SAMPLE_FLOAT_VEC3:
			glUniform3f( (*index)->m_iLocation, (*index)->m_fData[0], (*index)->m_fData[1], (*index)->m_fData[2] );
			break;
		 case SAMPLE_FLOAT_VEC4:
			glUniform4f( (*index)->m_iLocation, (*index)->m_fData[0], (*index)->m_fData[1], (*index)->m_fData[2], (*index)->m_fData[3] );
			break;
		 case SAMPLE_FLOAT_MAT4:
			glUniformMatrix4fv( (*index)->m_iLocation, 1, GL_FALSE, &(*index)->m_fData[0] );
			break;
		 case SAMPLE_PROGRAM:
			break;
		 default:
			assert(0);
			break;
		 }
	   }
	}

	void	cGLSLProgram::AddAttributeData(std::string e_strName,int e_iLocation)
	{
		AddAttributeData(e_strName.c_str(),e_iLocation);
	}

	void	cGLSLProgram::AddAttributeData(const char*e_strName,int e_iLocation)
	{
		this->m_AttributeDataVector.insert(std::make_pair(e_iLocation,std::string(e_strName)));
	}

	void	cGLSLProgram::UsingProgram()
	{
		g_pCurrentShader = nullptr;
		if( g_pCurrentUsingGLSLProgram != this )
		{
			g_pCurrentUsingGLSLProgram = this;
			glUseProgram(this->m_uiProgramHandle);
			std::map<int,std::string>::iterator	l_iterator = m_AttributeDataVector.begin();
			for( ;l_iterator != m_AttributeDataVector.end();++l_iterator )
			{
				int	l_iIndex = l_iterator->first;
				glEnableVertexAttribArray(l_iIndex);
				//const char*l_str = l_iterator->second.c_str();
				glBindAttribLocation( m_uiProgramHandle, l_iterator->first, l_iterator->second.c_str() );
				//const char*l_str = (const char*)glGetString(glGetError());
			}
		}
	}

	void	cGLSLProgram::DisableProgram()
	{
		assert(g_pCurrentUsingGLSLProgram == this);
		glUseProgram(0);
		std::map<int,std::string>::iterator	l_iterator = m_AttributeDataVector.begin();
		for( ;l_iterator != m_AttributeDataVector.end();++l_iterator )
		{
			int	l_iIndex = l_iterator->first;
			glDisableVertexAttribArray(l_iIndex);
		}
	}
	//========================
	//
	//========================
}