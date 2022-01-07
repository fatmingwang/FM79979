//#include "stdafx.h"
//#include <iostream> 
//#include "ComputerShader.h"
//#include "ComputerShaderUniform.h"
//
//GLuint g_iProgramPipeline = -1;
////GLuint g_iComputerShaderProgram = -1;
//GLuint g_iComputerShaderShader = -1;
//GLuint g_vComputerShaderWorkGroup[3] = { MY_CS_WORK_GROUP_SIZE,0,0};
//GLuint g_uiSSBO = -1;
//GLuint g_uiCSObjectID = -1;
//int				g_iNewParticleShoot = 0;
//sEmitterData	g_EmitterData;
//#define	LAZY_GET_GLERROR(Fun)Fun;CHECK_GL_ERROR(#Fun);
//
//void MyCS_SSO_Update();
//
////https://arm-software.github.io/opengl-es-sdk-for-android/compute_intro.html
////C:\NVPACK\Samples\GameWorks_Samples\GraphicsSamples\samples\es3aep-kepler\ComputeParticles
//
//cShaderStorageBuffer<sParticlesSSO>*	g_ParticlesSSOIn = nullptr;
//cShaderStorageBuffer<sParticlesSSO>*	g_ParticlesSSOOut = nullptr;
//cShaderStorageBuffer<sEmitterData>*		g_EmitterDataSSO = nullptr;
//
//void	MyCS_SSO_Init()
//{
//	if (!g_ParticlesSSOIn)
//	{
//		g_ParticlesSSOIn = new cShaderStorageBuffer<sParticlesSSO>(128);
//		g_ParticlesSSOOut = new cShaderStorageBuffer<sParticlesSSO>(128);
//		g_EmitterDataSSO = new cShaderStorageBuffer<sEmitterData>(1);
//		auto l_pData = g_ParticlesSSOIn->map(GL_MAP_WRITE_BIT);
//		for (int i = 0; i < MY_CS_WORK_GROUP_SIZE;++i)
//		{
//			l_pData[i].iIndex = i;
//			l_pData[i].bInit = false;
//			//l_pData->iIndex = i + 128;
//		}
//		g_ParticlesSSOIn->unmap();
//	}
//	memset(&g_EmitterData, 0, sizeof(g_EmitterData));
//	g_EmitterData.iTotalDispatchCount = 0;
//	g_EmitterData.iDoParticleInitStartIndex = 0;
//	g_EmitterData.iTotalParticleCount = 100;
//	g_EmitterData.iAtomicCounterForOutIndexOfParticleSSO = 0;
//	g_EmitterData.iOverDispatchCount = 0;
//	g_EmitterData.fMinTime = 0.5f;
//	g_EmitterData.InitFlag = 1;
//	g_EmitterData.UpdateFlag = 1;
//	//g_EmitterData.v3Velocity = Vector3(1,1,0);
//
//	//WholdUVForAnimationImage;
//}
//
//void	KeyUpForEmitShoot(unsigned char e_ucKey)
//{
//	if (e_ucKey)
//	{
//		g_iNewParticleShoot = 50;
//	}
//}
//
//void	MyCS_Program_Init()
//{
//	glUseProgram(0);
//	std::string l_strMyCS = g_strMyCSUnifom;
//	l_strMyCS += g_strMyCS;
//	GLuint object;
//	GLint status;
//	glGenProgramPipelines(1, &g_iProgramPipeline);
//	CHECK_GL_ERROR("1");
//#ifdef WIN32
//	const char* shaderPrefix =
//		R"(#version 430
//		#define Vector3	vec3
//		#define Vector4	vec4
//	)";
//#else
//	const char* shaderPrefix = "#version 310 es\n";
//#endif
//	//const GLchar* fullSrc[2] = { l_strCombineShader.c_str() };
//	const GLchar* fullSrc[2] = { shaderPrefix,l_strMyCS.c_str() };
//	object = glCreateShaderProgramv(GL_COMPUTE_SHADER, 2, fullSrc);
//	{
//		GLint logLength;
//		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &logLength);
//		if (logLength)
//		{
//			char* log = new char[logLength];
//			glGetProgramInfoLog(object, logLength, 0, log);
//			FMLog::Log(UT::ComposeMsgByFormat("Shader pipeline program not valid:\n%s\n", log).c_str(), false);
//			delete[] log;
//		}
//	}
//
//	glBindProgramPipeline(g_iProgramPipeline);
//	glUseProgramStages(g_iProgramPipeline, GL_COMPUTE_SHADER_BIT, object);
//	g_uiCSObjectID = object;
//	glValidateProgramPipeline(g_iProgramPipeline);
//	LAZY_GET_GLERROR(glGetProgramPipelineiv(g_iProgramPipeline, GL_VALIDATE_STATUS, &status));
//
//	if (status != GL_TRUE)
//	{
//		GLint logLength;
//		glGetProgramPipelineiv(g_iProgramPipeline, GL_INFO_LOG_LENGTH, &logLength);
//		char* log = new char[logLength];
//		glGetProgramPipelineInfoLog(g_iProgramPipeline, logLength, 0, log);
//		FMLog::Log(UT::ComposeMsgByFormat("Shader pipeline not valid:\n%s\n", log).c_str(), false);
//		delete[] log;
//	}
//	//https://stackoverflow.com/questions/46436682/opengl-compute-shader-binding-point-redundancy
//	GLuint block_index = 9;
//	block_index =glGetProgramResourceIndex(object,GL_SHADER_STORAGE_BLOCK,"MyEmitterData");
//	block_index = glGetProgramResourceIndex(object, GL_SHADER_STORAGE_BLOCK, "ParticlesSSOIn");
//	block_index = glGetProgramResourceIndex(object, GL_SHADER_STORAGE_BLOCK, "ParticlesSSOOut");
//	LAZY_GET_GLERROR(glBindProgramPipeline(0));
//}
//
//void	EmitInit()
//{
//	KeyUpForEmitShoot(1);
//	g_EmitterData.iTotalDispatchCount = 0;
//	g_EmitterData.iDoParticleInitStartIndex = g_EmitterData.iTotalParticleCount;
//	g_EmitterData.iTotalParticleCount += g_iNewParticleShoot;
//	g_EmitterData.iAtomicCounterForOutIndexOfParticleSSO = 0;
//	g_EmitterData.iOverDispatchCount = 0;
//	g_EmitterData.fMinTime = 0.5f;
//	g_EmitterData.InitFlag = 1;
//	g_EmitterData.UpdateFlag = 1;
//}
//
//void	MyCSInit()
//{
//	MyCS_Program_Init();
//	MyCS_SSO_Init();
//	MyCS_SSO_Update();
//	EmitInit();
//	MyCS_SSO_Update();
//}
//
//void MyCS_SSO_Update()
//{
//	// Invoke the compute shader to integrate the particles
//	LAZY_GET_GLERROR(glBindProgramPipeline(g_iProgramPipeline));
//	//glUseProgramStages(g_iProgramPipeline, GL_COMPUTE_SHADER_BIT, g_uiCSObjectID);
//	auto* l_pEmitterData = LAZY_GET_GLERROR(g_EmitterDataSSO->map());
//	//g_EmitterData.iTotalParticleCount += g_iNewParticleShoot;
//	*l_pEmitterData = g_EmitterData;
//	g_EmitterDataSSO->unmap();
//	LAZY_GET_GLERROR(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
//	//for test.
//	//g_EmitterData.fMinTime = 79979.f;
//	//l_pEmitterData = g_EmitterDataSSO->map(GL_MAP_READ_BIT);
//	//g_EmitterData = *l_pEmitterData;
//	//g_EmitterDataSSO->unmap();
//	//glActiveTexture(GL_TEXTURE0);
//	//glBindTexture(GL_TEXTURE_3D, m_noiseTex);
//	LAZY_GET_GLERROR(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, g_EmitterDataSSO->getBuffer()));
//	LAZY_GET_GLERROR(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, g_ParticlesSSOOut->getBuffer()));
//	LAZY_GET_GLERROR(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, g_ParticlesSSOIn->getBuffer()));
//	int l_iNumWorkGroup = g_EmitterData.iTotalParticleCount / MY_CS_WORK_GROUP_SIZE;
//	if (l_iNumWorkGroup < 1)
//	{
//		l_iNumWorkGroup = 1;
//	}
//	if (1)
//	{//before write
//		auto l_pInData = g_ParticlesSSOIn->map(GL_MAP_READ_BIT);
//		for (int i = 0; i < MY_CS_WORK_GROUP_SIZE; ++i)
//		{
//			auto l_Data = l_pInData[i];
//			int a = 0;
//		}
//		g_ParticlesSSOIn->unmap();
//	}
//	LAZY_GET_GLERROR(glDispatchCompute(10, 1, 1));
//	// We need to block here on compute completion to ensure that the
//	// computation is done before we render
//	LAZY_GET_GLERROR(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
//	l_pEmitterData = g_EmitterDataSSO->map(GL_MAP_READ_BIT);
//	g_EmitterData = *l_pEmitterData;
//	g_EmitterDataSSO->unmap();
//	auto l_pInData = g_ParticlesSSOIn->map(GL_MAP_READ_BIT);
//	for (int i = 0; i < MY_CS_WORK_GROUP_SIZE; ++i)
//	{
//		auto l_Data = l_pInData[i];
//		int a = 0;
//	}
//	g_ParticlesSSOIn->unmap();
//	auto l_pOutData = g_ParticlesSSOOut->map(GL_MAP_READ_BIT);
//	for (int i = 0; i < MY_CS_WORK_GROUP_SIZE; ++i)
//	{
//		auto l_Data = l_pOutData[i];
//		int a = 0;
//	}
//	g_ParticlesSSOOut->unmap();
//	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, 0);
//	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);
//	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
//	glBindProgramPipeline(0);
//}
//
//
//void	ComputerShaderInit()
//{
//	//glsl binding index coorespond to glBindBufferBase
//	//layout(std140, binding = 2) buffer Vel {vec4 vel[];	};
//	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_pos->getBuffer());
//
//
//	int cx = 0, cy = 0, cz = 0;
//	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &cx);
//	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &cy);
//	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &cz);
//	FMLog::Log(UT::ComposeMsgByFormat("Max compute work group count = %d, %d, %d\n", cx, cy, cz).c_str(), false);
//
//	int sx = 0, sy = 0, sz = 0;
//	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &sx);
//	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &sy);
//	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &sz);
//	FMLog::Log(UT::ComposeMsgByFormat("Max compute work group size  = %d, %d, %d\n", sx, sy, sz).c_str(), false);
//	GLint	l_iValue = 0;
//	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &l_iValue);
//	//g_iComputerShaderProgram = glCreateProgram();
//	//g_iComputerShaderShader = glCreateShader(GL_COMPUTE_SHADER);
//	//glShaderSource(g_iComputerShaderShader,1, gls,nullptr);
//	//glCompileShader(g_iComputerShaderShader);
//	//glAttachShader(g_iComputerShaderProgram, g_iComputerShaderShader);
//	//glLinkProgram(g_iComputerShaderProgram);
//	MyCSInit();
//}
//
//void	ComputerShaderUpdate()
//{
//
//}
//
//void	ComputerShaderRender()
//{
//	//glUseProgram(g_iComputerShaderProgram); // Compute shader program.
//	glDispatchCompute(g_vComputerShaderWorkGroup[0], g_vComputerShaderWorkGroup[1], g_vComputerShaderWorkGroup[2]);
//}
//
//void	ComputerShaderDestroy()
//{
//}
//
//void allocateSSBO() 
//{
//	// Build and populate
//	glGenBuffers(1, &g_uiSSBO);
//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_uiSSBO);
//	float color[] = { 1.f, 1.f, 1.f };
//	glBufferData(GL_SHADER_STORAGE_BUFFER, 3 * sizeof(float), color, GL_DYNAMIC_COPY);
//
//	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, g_uiSSBO);
//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//	assert(glGetError() == GL_NO_ERROR);
//}
//
//
//void SSBOTest()
//{
//	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buffer);
//	//glUseProgram(update_vbo_program);
//	//glDispatchCompute(GROUPS, 1, 1); // <-- Write to buffer
//	//glUseProgram(render_program);
//	//glBindVertexArray(vertex_array);
//	//glDrawElements(GL_TRIANGLES, ...); // <-- Read from buffer
//}
//
//void	Barrier()
//{
//	//glMemoryBarrier();
//	//glTextureBarrier();
//	//glMemoryBarrierByRegion();
//	//glMemoryBarrierShared();
//	//glMemoryBarrierImage();
//	//glMemoryBarrierBuffer();
//	//glMemoryBarrierAtomicCounter();
//	//glMroupMemoryBarrier();
//	//// Thread 1
//	//A = 1;
//	//memoryBarrierShared(); // Write to B cannot complete until A is completed.
//	//B = 1;
//	//// Thread 2
//	//int readB = B;
//	//memoryBarrierShared(); // Reads from A cannot happen before we've read B.
//	//int readA = A;
//	//if (readA == 0 && readB == 1)
//	//{
//	//	// This case is now impossible. If B == 1, we know that the write to A must have happened.
//	//	// Since we are guaranteed to read A after B, we must read A == 1 as well.
//	//}
//}
//namespace FATMING_CORE
//{
//	extern cBaseShader* g_pCurrentShader;
//}
//
//cComputeShader::cComputeShader()
//{
//}
//
//cComputeShader::~cComputeShader()
//{
//}
//
//void cComputeShader::Use(bool e_bUseLastWVPMatrix)
//{
//	//g_pCurrentShader = this;
//	glUseProgram(m_uiProgram);
//	if (m_fBindGLDataFunction)
//	{
//		m_fBindGLDataFunction(this);
//	}
//}
//
//void cComputeShader::Update(float e_fElpaseTime)
//{
//}
////http://byteblacksmith.com/improvements-to-the-canonical-one-liner-glsl-rand-for-opengl-es-2-0/
////https://www.wolframalpha.com/input/?i=plot%28+mod%28+sin%28x*12.9898+%2B+y*78.233%29+*+43758.5453%2C1%29x%3D0..2%2C+y%3D0..2%29
////https://stackoverflow.com/questions/12964279/whats-the-origin-of-this-glsl-rand-one-liner
////https://stackoverflow.com/questions/4200224/random-noise-functions-for-glsl
//GLuint createNoiseTexture4f3D(int w, int h, int d, GLint internalFormat)
//{
//	uint8_t* data = new uint8_t[w * h * d * 4];
//	if (0)
//	{
//		uint8_t* ptr = data;
//		for (int z = 0; z < d; z++) 
//		{
//			for (int y = 0; y < h; y++) 
//			{
//				for (int x = 0; x < w; x++) 
//				{
//					*ptr++ = rand() & 0xff;
//					*ptr++ = rand() & 0xff;
//					*ptr++ = rand() & 0xff;
//					*ptr++ = rand() & 0xff;
//				}
//			}
//		}
//	}
//	else
//	{
//		float* ptr = (float*)data;
//		for (int z = 0; z < d; z++)
//		{
//			for (int y = 0; y < h; y++)
//			{
//				for (int x = 0; x < w; x++)
//				{
//					*ptr++ = frand(0,5.f);
//				}
//			}
//		}
//	}
//	GLuint tex;
//	LAZY_GET_GLERROR(glGenTextures(1, &tex));
//	
//	LAZY_GET_GLERROR(glBindTexture(GL_TEXTURE_3D, tex));
//	LAZY_GET_GLERROR(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
//	LAZY_GET_GLERROR(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
//	LAZY_GET_GLERROR(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT));
//	LAZY_GET_GLERROR(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT));
//	LAZY_GET_GLERROR(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT));
//	//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//	LAZY_GET_GLERROR(glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, w, h, d, 0, GL_RGBA, GL_BYTE, data));
//	
//
//	delete[] data;
//	return tex;
//}
//
//
////unsigned short CRC_CREATE(unsigned char* data, unsigned char lenth);
////struct sShinGanLineProtocol
////{
////	char cHeader = 0x02;
////	char cControllerID = 0x11;//0x11 ¡V 0x20,from xml.
////	char cCommandID = 0x11;//0x11 for query car status, `0x14
////	char cWhichTrack = 0x11;//0x11~0x13,0x21~0x23
////};
////
////struct sShinGanLineProtocolWithCRC:public sShinGanLineProtocol
////{
////	char cCRC16[2];
////	char cEnd = 0x03;
////	void	AssignCRC()
////	{
////		unsigned short l_CRC = CRC_CREATE((unsigned char*)this, sizeof(sShinGanLineProtocol));
////		char* l_pCRC = (char*)&l_CRC;
////		cCRC16[0] = l_pCRC[0];
////		cCRC16[1] = l_pCRC[1];
////	}
////};
////
////
////unsigned short CRC_CREATE(unsigned char* data, unsigned char lenth)
////{
////	unsigned char j;
////	unsigned short reg_crc = 0, temp = 0xFFFF;
////	while (lenth--)
////	{
////		temp ^= *data++;
////		for (j = 0; j < 8; j++)
////		{
////			if (temp & 0x01) /* LSB(b0)=1 */
////				temp = (temp >> 1) ^ 0xA001;
////			else
////				temp = temp >> 1;
////		}
////	}
////	reg_crc = ((temp & 0x00ff) << 8) | ((temp & 0xff00) >> 8);
////	return reg_crc;
////}