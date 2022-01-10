#include "BufferUpdateByComputeShader.h"

namespace FATMING_CORE
{
	extern cBaseShader* g_pCurrentShader;

	cBufferUpdateByComputeShader::cBufferUpdateByComputeShader(const char* e_strCS)
	{
		m_pIn = nullptr;
		m_pOut = nullptr;
		m_uiShaderObjectID = -1;
	}


	
	cBufferUpdateByComputeShader::~cBufferUpdateByComputeShader()
	{
		SAFE_DELETE(m_pIn);
		SAFE_DELETE(m_pOut);
	}

	bool cBufferUpdateByComputeShader::CreateProgramPipeline()
	{
		GLint l_iStatus = -1;
		glBindProgramPipeline(m_uiProgramPipeline);
		glUseProgramStages(m_uiProgramPipeline, GL_COMPUTE_SHADER_BIT, m_uiShaderObjectID);
		glValidateProgramPipeline(m_uiProgramPipeline);
		LAZY_DO_GL_COMMAND_AND_GET_ERROR(glGetProgramPipelineiv(m_uiProgramPipeline, GL_VALIDATE_STATUS, &l_iStatus));

		if (l_iStatus != GL_TRUE)
		{
			GLint l_iLogLength;
			glGetProgramPipelineiv(m_uiProgramPipeline, GL_INFO_LOG_LENGTH, &l_iLogLength);
			char* l_strLog = new char[l_iLogLength];
			glGetProgramPipelineInfoLog(m_uiShaderObjectID, l_iLogLength, 0, l_strLog);
			FMLog::Log(UT::ComposeMsgByFormat("Shader pipeline not valid:\n%s\n", l_strLog).c_str(), false);
			delete[] l_strLog;
		}
		//https://stackoverflow.com/questions/46436682/opengl-compute-shader-binding-point-redundancy
		//GLuint block_index = 9;
		//block_index = glGetProgramResourceIndex(m_uiShaderObjectID, GL_SHADER_STORAGE_BLOCK, "MyEmitterData");
		//block_index = glGetProgramResourceIndex(m_uiShaderObjectID, GL_SHADER_STORAGE_BLOCK, "ParticlesSSOIn");
		//block_index = glGetProgramResourceIndex(m_uiShaderObjectID, GL_SHADER_STORAGE_BLOCK, "ParticlesSSOOut");
		LAZY_DO_GL_COMMAND_AND_GET_ERROR(glBindProgramPipeline(0));
		return false;
	}

	void cBufferUpdateByComputeShader::Use(bool e_bUseLastWVPMatrix)
	{
		g_pCurrentShader = this;
		if (m_uiProgram != -1)
		{
			LAZY_DO_GL_COMMAND_AND_GET_ERROR(glUseProgram(m_uiProgram));
		}
		glDispatchCompute(m_iWorkGroupsDimenstionSize[0], m_iWorkGroupsDimenstionSize[1], m_iWorkGroupsDimenstionSize[2]);
	}

	
	bool cBufferUpdateByComputeShader::CreateCSProgram(const char* e_strCS)
	{
		glUseProgram(0);
		GLint	l_iStatus = -1;
		LAZY_DO_GL_COMMAND_AND_GET_ERROR(glGenProgramPipelines(1, &this->m_uiProgram));
#ifdef WIN32
		const char* shaderPrefix =
			R"(	#version 430
				#define Vector3	vec3
				#define Vector4	vec4
			)";
#else
		const char* shaderPrefix = "#version 310 es\n";
#endif
		//const GLchar* fullSrc[2] = { l_strCombineShader.c_str() };
		const GLchar* fullSrc[2] = { shaderPrefix,e_strCS };
		m_uiShaderObjectID = glCreateShaderProgramv(GL_COMPUTE_SHADER, 2, fullSrc);
		{
			GLint l_iLogLength = 0;
			glGetProgramiv(m_uiShaderObjectID, GL_INFO_LOG_LENGTH, &l_iLogLength);
			if (l_iLogLength)
			{
				char*l_strLogInfo = new char[l_iLogLength];
				glGetProgramInfoLog(m_uiShaderObjectID, l_iLogLength, 0, l_strLogInfo);
				FMLog::Log(UT::ComposeMsgByFormat("Shader pipeline program not valid:\n%s\n", log).c_str(), false);
				delete[] l_strLogInfo;
				return false;
			}
		}
		return true;
	}

	
	void cBufferUpdateByComputeShader::SetSizeAndData(size_t e_uiSize, char* e_pData)
	{
		if (m_pIn && m_pIn->GetSize() != e_uiSize)
		{
			SAFE_DELETE(m_pIn);
			SAFE_DELETE(m_pOut);
		}
		if (!m_pIn)
		{
			m_pIn = new cShaderStorageBuffer<char>(e_uiSize);
			m_pOut = new cShaderStorageBuffer<char>(e_uiSize);
		}
		m_pIn->Bind();
	}

	
	void cBufferUpdateByComputeShader::SetDataKeepSize(char* e_pData)
	{
	}

	
	void cBufferUpdateByComputeShader::SetSizeKeepData(size_t e_uiSize)
	{
	}
	
	bool cBufferUpdateByComputeShader::SetWorkGroupDimension(int e_iSizeX, int e_iSizeY, int e_iSizeZ)
	{
		m_iWorkGroupsDimenstionSize[0] = e_iSizeX;
		m_iWorkGroupsDimenstionSize[1] = e_iSizeY;
		m_iWorkGroupsDimenstionSize[2] = e_iSizeZ;
		return false;
	}

	
	bool cBufferUpdateByComputeShader::CopyDatTo(char* e_pOutData, int&e_iNumCopied)
	{
		if (!m_pOut)
		{
			if (m_pOut->GetSize() >= 0)
			{

			}
		}
		return false;
	}
}