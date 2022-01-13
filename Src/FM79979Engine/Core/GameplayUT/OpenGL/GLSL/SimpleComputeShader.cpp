#include "SimpleComputeShader.h"

namespace FATMING_CORE
{
	extern cBaseShader* g_pCurrentShader;
	void	BufferUpdateByComputeShaderTest(cSimpleComputeShader*g_pBufferUpdateByComputeShader)
	{

	}
	cSimpleComputeShader::cSimpleComputeShader(const char* e_strCS)
	{
		m_uiShaderProgramID = -1;
		m_uiProgramPipeline = -1;
		if (CreateCSProgram(e_strCS))
		{
			if (!CreateProgramPipeline())
			{

			}
		}
	}
	
	cSimpleComputeShader::~cSimpleComputeShader()
	{
		if (m_uiProgramPipeline != -1)
		{
			glDeleteProgramPipelines(1, &m_uiProgramPipeline);
		}
		if (m_uiShaderProgramID != -1)
		{
			glDeleteProgram(m_uiShaderProgramID);
		}
	}

	void cSimpleComputeShader::Use(bool e_bUseLastWVPMatrix)
	{
		g_pCurrentShader = this;
		if (m_uiProgram != -1)
		{
			LAZY_DO_GL_COMMAND_AND_GET_ERROR(glUseProgram(m_uiProgram));
		}
		LAZY_DO_GL_COMMAND_AND_GET_ERROR(glBindProgramPipeline(m_uiProgramPipeline));
	}
	
	bool cSimpleComputeShader::CreateCSProgram(const char* e_strCS)
	{
		glUseProgram(0);
		GLint	l_iStatus = -1;
#ifdef WIN32
		const char* l_strShaderPrefix =
			R"(	#version 430
				#define Vector3	vec3
				#define Vector4	vec4
			)";
#else
		const char* shaderPrefix = "#version 310 es\n";
#endif
		const GLchar* fullSrc[2] = { l_strShaderPrefix,e_strCS };
		m_uiShaderProgramID = glCreateShaderProgramv(GL_COMPUTE_SHADER, 2, fullSrc);
		{
			GLint l_iLogLength = 0;
			glGetProgramiv(m_uiShaderProgramID, GL_INFO_LOG_LENGTH, &l_iLogLength);
			if (l_iLogLength)
			{
				char*l_strLogInfo = new char[l_iLogLength];
				glGetProgramInfoLog(m_uiShaderProgramID, l_iLogLength, 0, l_strLogInfo);
				FMLog::Log(UT::ComposeMsgByFormat("Shader pipeline program not valid:\n%s\n", l_strLogInfo).c_str(), false);
				delete[] l_strLogInfo;
				return false;
			}
		}
		glUseProgram(0);
		return true;
	}

	bool cSimpleComputeShader::CreateProgramPipeline()
	{
		GLint l_iStatus = -1;
		glBindProgramPipeline(0);
		LAZY_DO_GL_COMMAND_AND_GET_ERROR(glGenProgramPipelines(1, &this->m_uiProgramPipeline));
		glBindProgramPipeline(m_uiProgramPipeline);
		glUseProgramStages(m_uiProgramPipeline, GL_COMPUTE_SHADER_BIT, m_uiShaderProgramID);
		glValidateProgramPipeline(m_uiProgramPipeline);
		LAZY_DO_GL_COMMAND_AND_GET_ERROR(glGetProgramPipelineiv(m_uiProgramPipeline, GL_VALIDATE_STATUS, &l_iStatus));

		if (l_iStatus != GL_TRUE)
		{
			GLint l_iLogLength;
			glGetProgramPipelineiv(m_uiProgramPipeline, GL_INFO_LOG_LENGTH, &l_iLogLength);
			char* l_strLog = new char[l_iLogLength];
			glGetProgramPipelineInfoLog(m_uiShaderProgramID, l_iLogLength, 0, l_strLog);
			FMLog::Log(UT::ComposeMsgByFormat("Shader pipeline not valid:\n%s\n", l_strLog).c_str(), false);
			delete[] l_strLog;
			return false;
		}
		//https://stackoverflow.com/questions/46436682/opengl-compute-shader-binding-point-redundancy
		//GLuint block_index = 9;
		//block_index = glGetProgramResourceIndex(m_uiShaderProgramID, GL_SHADER_STORAGE_BLOCK, "MyEmitterData");
		//block_index = glGetProgramResourceIndex(m_uiShaderProgramID, GL_SHADER_STORAGE_BLOCK, "ParticlesSSOIn");
		//block_index = glGetProgramResourceIndex(m_uiShaderProgramID, GL_SHADER_STORAGE_BLOCK, "ParticlesSSOOut");
		LAZY_DO_GL_COMMAND_AND_GET_ERROR(glBindProgramPipeline(0));
		return true;
	}
	
	bool cSimpleComputeShader::DispatchCompute(int e_iSizeX, int e_iSizeY, int e_iSizeZ)
	{
		LAZY_DO_GL_COMMAND_AND_GET_ERROR(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
		m_iWorkGroupsDimenstionSize[0] = e_iSizeX;
		m_iWorkGroupsDimenstionSize[1] = e_iSizeY;
		m_iWorkGroupsDimenstionSize[2] = e_iSizeZ;
		glDispatchCompute(m_iWorkGroupsDimenstionSize[0], m_iWorkGroupsDimenstionSize[1], m_iWorkGroupsDimenstionSize[2]);
		return false;
	}

}