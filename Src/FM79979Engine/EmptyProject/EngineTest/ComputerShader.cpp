#include "stdafx.h"
#include <iostream>
#include "ComputerShader.h"
#include "ComputerShaderUniform.h"
#include "../../Core/GameplayUT/Render/RenderQueue/RenderCommand.h"

GLuint			g_iProgramPipeline = -1;
//GLuint		g_iComputerShaderProgram = -1;
GLuint			g_iComputerShaderShader = -1;
GLuint			g_vComputerShaderWorkGroup[3] = { MY_CS_WORK_GROUP_SIZE,0,0};
GLuint			g_uiSSBO = -1;
GLuint			g_uiCSObjectID = -1;
int				g_iNewParticleShoot = 0;
sEmitterData	g_EmitterData;
cBatchRender*	g_pBatchRenderForTest = nullptr;
void MyCS_SSO_Update();

//https://arm-software.github.io/opengl-es-sdk-for-android/compute_intro.html
//C:\NVPACK\Samples\GameWorks_Samples\GraphicsSamples\samples\es3aep-kepler\ComputeParticles

cShaderStorageBuffer<sParticlesSSO>*	g_ParticlesSSOIn = nullptr;
cShaderStorageBuffer<sParticlesSSO>*	g_ParticlesSSOOut = nullptr;
cShaderStorageBuffer<sEmitterData>*		g_EmitterDataSSO = nullptr;

void	MyCS_SSO_Init()
{
	if (!g_pBatchRenderForTest)
	{
		//g_pBatchRenderForTest = new cBatchRender();
	}
	if (!g_ParticlesSSOIn)
	{
		g_ParticlesSSOIn = new cShaderStorageBuffer<sParticlesSSO>(128);
		g_ParticlesSSOOut = new cShaderStorageBuffer<sParticlesSSO>(128);
		g_EmitterDataSSO = new cShaderStorageBuffer<sEmitterData>(1);
		auto l_pData = g_ParticlesSSOIn->Map(GL_MAP_WRITE_BIT);
		for (int i = 0; i < MY_CS_WORK_GROUP_SIZE;++i)
		{
			l_pData[i].iIndex = i;
			l_pData[i].bInit = false;
			//l_pData->iIndex = i + 128;
		}
		g_ParticlesSSOIn->Unmap();
	}
	memset(&g_EmitterData, 0, sizeof(g_EmitterData));
	g_EmitterData.iTotalDispatchCount = 0;
	g_EmitterData.iDoParticleInitStartIndex = 0;
	g_EmitterData.iTotalParticleCount = 100;
	g_EmitterData.iAtomicCounterForOutIndexOfParticleSSO = 0;
	g_EmitterData.iOverDispatchCount = 0;
	g_EmitterData.fMinTime = 0.5f;
	g_EmitterData.InitFlag = 1;
	g_EmitterData.UpdateFlag = 1;
	//g_EmitterData.v3Velocity = Vector3(1,1,0);

	//WholdUVForAnimationImage;
}

void	KeyUpForEmitShoot(unsigned char e_ucKey)
{
	if (e_ucKey)
	{
		g_iNewParticleShoot = 50;
	}
}

void	MyCS_Program_Init()
{
	glUseProgram(0);
	std::string l_strMyCS = g_strMyCSUnifom;
	l_strMyCS += g_strMyCS;
	GLuint object;
	GLint status;
	CHECK_GL_ERROR("1");
#ifdef WIN32
	const char* shaderPrefix =
		R"(#version 430
		#define Vector3	vec3
		#define Vector4	vec4
	)";
#else
	const char* shaderPrefix = "#version 310 es\n#define Vector3 vec3\n#define Vector4	vec4\n";
#endif
	//const GLchar* fullSrc[2] = { l_strCombineShader.c_str() };
	const GLchar* fullSrc[2] = { shaderPrefix,l_strMyCS.c_str() };
	bool l_bDoTest = false;
	if (l_bDoTest)
	{
		auto l_uiProgram = glCreateProgram();
		std::string l_strShaderContent = shaderPrefix+ l_strMyCS;
		int l_iOutProgramID;
		if (CreateShader(l_strShaderContent.c_str(), GL_COMPUTE_SHADER, l_iOutProgramID))
		{

		}
		return;
	}
	object = glCreateShaderProgramv(GL_COMPUTE_SHADER, 2, fullSrc);
	CHECK_GL_ERROR("1");
	{
		GLint logLength;
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &logLength);
		CHECK_GL_ERROR("1");
		if (logLength)
		{
			char* log = new char[logLength];
			glGetProgramInfoLog(object, logLength, 0, log);
			FMLog::Log(UT::ComposeMsgByFormat("Shader pipeline program not valid:\n%s\n", log).c_str(), false);
			delete[] log;
		}
	}
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glGenProgramPipelines(1, &g_iProgramPipeline));
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glBindProgramPipeline(g_iProgramPipeline));
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glUseProgramStages(g_iProgramPipeline, GL_COMPUTE_SHADER_BIT, object));
	g_uiCSObjectID = object;
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glValidateProgramPipeline(g_iProgramPipeline));
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glGetProgramPipelineiv(g_iProgramPipeline, GL_VALIDATE_STATUS, &status));

	if (status != GL_TRUE)
	{
		GLint logLength;
		glGetProgramPipelineiv(g_iProgramPipeline, GL_INFO_LOG_LENGTH, &logLength);
		char* log = new char[logLength];
		glGetProgramPipelineInfoLog(g_iProgramPipeline, logLength, 0, log);
		FMLog::Log(UT::ComposeMsgByFormat("Shader pipeline not valid:\n%s\n", log).c_str(), false);
		delete[] log;
	}
	//https://stackoverflow.com/questions/46436682/opengl-compute-shader-binding-point-redundancy
	GLuint block_index = 9;
	block_index =glGetProgramResourceIndex(object,GL_SHADER_STORAGE_BLOCK,"MyEmitterData");
	block_index = glGetProgramResourceIndex(object, GL_SHADER_STORAGE_BLOCK, "ParticlesSSOIn");
	block_index = glGetProgramResourceIndex(object, GL_SHADER_STORAGE_BLOCK, "ParticlesSSOOut");
	block_index = glGetProgramResourceIndex(object, GL_SHADER_STORAGE_BLOCK, "ParticlesMatrix");
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glBindProgramPipeline(0));
}

void	EmitInit()
{
	KeyUpForEmitShoot(1);
	g_EmitterData.iTotalDispatchCount = 0;
	g_EmitterData.iDoParticleInitStartIndex = g_EmitterData.iTotalParticleCount;
	g_EmitterData.iTotalParticleCount += g_iNewParticleShoot;
	g_EmitterData.iAtomicCounterForOutIndexOfParticleSSO = 0;
	g_EmitterData.iOverDispatchCount = 0;
	g_EmitterData.fMinTime = 0.5f;
	g_EmitterData.InitFlag = 1;
	g_EmitterData.UpdateFlag = 1;
}

void	MyCSInit()
{
	MyCS_Program_Init();
	MyCS_SSO_Init();
	MyCS_SSO_Update();
	EmitInit();
	MyCS_SSO_Update();
}

void MyCS_SSO_Update()
{
	// Invoke the compute shader to integrate the particles
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glBindProgramPipeline(g_iProgramPipeline));
	//glUseProgramStages(g_iProgramPipeline, GL_COMPUTE_SHADER_BIT, g_uiCSObjectID);
	auto* l_pEmitterData = LAZY_DO_GL_COMMAND_AND_GET_ERROR(g_EmitterDataSSO->Map());
	//g_EmitterData.iTotalParticleCount += g_iNewParticleShoot;
	*l_pEmitterData = g_EmitterData;
	g_EmitterDataSSO->Unmap();
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
	//for test.
	//g_EmitterData.fMinTime = 79979.f;
	//l_pEmitterData = g_EmitterDataSSO->map(GL_MAP_READ_BIT);
	//g_EmitterData = *l_pEmitterData;
	//g_EmitterDataSSO->unmap();
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_3D, m_noiseTex);
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, g_EmitterDataSSO->GetBufferID()));
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, g_ParticlesSSOOut->GetBufferID()));
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, g_ParticlesSSOIn->GetBufferID()));
	int l_iNumWorkGroup = g_EmitterData.iTotalParticleCount / MY_CS_WORK_GROUP_SIZE;
	if (l_iNumWorkGroup < 1)
	{
		l_iNumWorkGroup = 1;
	}
	if (1)
	{//before write
		auto l_pInData = g_ParticlesSSOIn->Map(GL_MAP_READ_BIT);
		for (int i = 0; i < MY_CS_WORK_GROUP_SIZE; ++i)
		{
			auto l_Data = l_pInData[i];
			int a = 0;
		}
		g_ParticlesSSOIn->Unmap();
	}
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glDispatchCompute(10, 1, 1));
	// We need to block here on compute completion to ensure that the
	// computation is done before we render
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
	l_pEmitterData = g_EmitterDataSSO->Map(GL_MAP_READ_BIT);
	g_EmitterData = *l_pEmitterData;
	g_EmitterDataSSO->Unmap();
	auto l_pInData = g_ParticlesSSOIn->Map(GL_MAP_READ_BIT);
	for (int i = 0; i < MY_CS_WORK_GROUP_SIZE; ++i)
	{
		auto l_Data = l_pInData[i];
		int a = 0;
	}
	g_ParticlesSSOIn->Unmap();
	auto l_pOutData = g_ParticlesSSOOut->Map(GL_MAP_READ_BIT);
	for (int i = 0; i < MY_CS_WORK_GROUP_SIZE; ++i)
	{
		auto l_Data = l_pOutData[i];
		int a = 0;
	}
	g_ParticlesSSOOut->Unmap();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
	glBindProgramPipeline(0);
}


void	ComputerShaderInit()
{
	//glsl binding index coorespond to glBindBufferBase
	//layout(std140, binding = 2) buffer Vel {vec4 vel[];	};
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_pos->getBuffer());


	int cx = 0, cy = 0, cz = 0;
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &cx);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &cy);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &cz);
	FMLog::Log(UT::ComposeMsgByFormat("Max compute work group count = %d, %d, %d\n", cx, cy, cz).c_str(), false);

	int sx = 0, sy = 0, sz = 0;
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &sx);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &sy);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &sz);
	FMLog::Log(UT::ComposeMsgByFormat("Max compute work group size  = %d, %d, %d\n", sx, sy, sz).c_str(), false);
	GLint	l_iValue = 0;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &l_iValue);
	//g_iComputerShaderProgram = glCreateProgram();
	//g_iComputerShaderShader = glCreateShader(GL_COMPUTE_SHADER);
	//glShaderSource(g_iComputerShaderShader,1, gls,nullptr);
	//glCompileShader(g_iComputerShaderShader);
	//glAttachShader(g_iComputerShaderProgram, g_iComputerShaderShader);
	//glLinkProgram(g_iComputerShaderProgram);
	MyCSInit();
}

void	ComputerShaderUpdate()
{

}

void	ComputerShaderRender()
{
	//glUseProgram(g_iComputerShaderProgram); // Compute shader program.
	glDispatchCompute(g_vComputerShaderWorkGroup[0], g_vComputerShaderWorkGroup[1], g_vComputerShaderWorkGroup[2]);
}

void	ComputerShaderDestroy()
{
}

void allocateSSBO()
{
	// Build and populate
	glGenBuffers(1, &g_uiSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_uiSSBO);
	float color[] = { 1.f, 1.f, 1.f };
	glBufferData(GL_SHADER_STORAGE_BUFFER, 3 * sizeof(float), color, GL_DYNAMIC_COPY);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, g_uiSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	assert(glGetError() == GL_NO_ERROR);
}


void SSBOTest()
{
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buffer);
	//glUseProgram(update_vbo_program);
	//glDispatchCompute(GROUPS, 1, 1); // <-- Write to buffer
	//glUseProgram(render_program);
	//glBindVertexArray(vertex_array);
	//glDrawElements(GL_TRIANGLES, ...); // <-- Read from buffer
}

void	Barrier()
{
	//glMemoryBarrier();
	//glTextureBarrier();
	//glMemoryBarrierByRegion();
	//glMemoryBarrierShared();
	//glMemoryBarrierImage();
	//glMemoryBarrierBuffer();
	//glMemoryBarrierAtomicCounter();
	//glMroupMemoryBarrier();
	//// Thread 1
	//A = 1;
	//memoryBarrierShared(); // Write to B cannot complete until A is completed.
	//B = 1;
	//// Thread 2
	//int readB = B;
	//memoryBarrierShared(); // Reads from A cannot happen before we've read B.
	//int readA = A;
	//if (readA == 0 && readB == 1)
	//{
	//	// This case is now impossible. If B == 1, we know that the write to A must have happened.
	//	// Since we are guaranteed to read A after B, we must read A == 1 as well.
	//}
}
namespace FATMING_CORE
{
	extern cBaseShader* g_pCurrentShader;
}

cComputeShader::cComputeShader()
{
}

cComputeShader::~cComputeShader()
{
}

void cComputeShader::Use(bool e_bUseLastWVPMatrix)
{
	//g_pCurrentShader = this;
	glUseProgram(m_uiProgram);
	if (m_fBindGLDataFunction)
	{
		m_fBindGLDataFunction(this);
	}
}

void cComputeShader::Update(float e_fElpaseTime)
{
}
//http://byteblacksmith.com/improvements-to-the-canonical-one-liner-glsl-rand-for-opengl-es-2-0/
//https://www.wolframalpha.com/input/?i=plot%28+mod%28+sin%28x*12.9898+%2B+y*78.233%29+*+43758.5453%2C1%29x%3D0..2%2C+y%3D0..2%29
//https://stackoverflow.com/questions/12964279/whats-the-origin-of-this-glsl-rand-one-liner
//https://stackoverflow.com/questions/4200224/random-noise-functions-for-glsl
GLuint createNoiseTexture4f3D(int w, int h, int d, GLint internalFormat)
{
	uint8_t* data = new uint8_t[w * h * d * 4];
	if (0)
	{
		uint8_t* ptr = data;
		for (int z = 0; z < d; z++) 
		{
			for (int y = 0; y < h; y++) 
			{
				for (int x = 0; x < w; x++) 
				{
					*ptr++ = rand() & 0xff;
					*ptr++ = rand() & 0xff;
					*ptr++ = rand() & 0xff;
					*ptr++ = rand() & 0xff;
				}
			}
		}
	}
	else
	{
		float* ptr = (float*)data;
		for (int z = 0; z < d; z++)
		{
			for (int y = 0; y < h; y++)
			{
				for (int x = 0; x < w; x++)
				{
					*ptr++ = frand(0,5.f);
				}
			}
		}
	}
	GLuint tex;
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glGenTextures(1, &tex));
	
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glBindTexture(GL_TEXTURE_3D, tex));
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT));
	//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, w, h, d, 0, GL_RGBA, GL_BYTE, data));
	

	delete[] data;
	return tex;
}