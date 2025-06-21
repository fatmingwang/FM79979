#pragma once


//iTotalParticleCount is set while emitter shoot is called.
//every update iAtomicCounterForOutIndexOfParticleSSO set to 0
//after update iTotalParticleCount should equal iAtomicCounterForOutIndexOfParticleSSO

//Vector4	WholdUVForAnimationImage[100];								\
//Vector4	v3Velocity;													\
//Vector4	v3StartPos;													\

//ERROR: 0 : 4 : 'unsigned' : Reserved word.
//ERROR : 0 : 4 : 'unsigned' : Syntax error : syntax error
//INTERNAL ERROR : no main() function!
//ERROR : 2 compilation errors.No code generated.



#ifndef uint
#define	uint	unsigned int
#endif
//CS data alignment
//https://stackoverflow.com/questions/38172696/should-i-ever-use-a-vec3-inside-of-a-uniform-buffer-or-shader-storage-buffer-o/38172697#38172697
#define	MUCS_UNIFORM													\
struct sParticlesSSO													\
{																		\
	uint	iOldIndex;													\
	bool    bInit;														\
	float	fLife;														\
	uint	iIndex;														\
	Vector4	color;														\
	Vector4	rotation;													\
	Vector4	velocity;													\
	Vector4	Pos;														\
};																		\
struct sEmitterData														\
{																		\
	int		iTotalDispatchCount;										\
	int		iDoParticleInitStartIndex;									\
	int		iTotalParticleCount;										\
	int		iAtomicCounterForOutIndexOfParticleSSO;						\
	int		iOverDispatchCount;											\
	float	fMinTime;													\
	uint	InitFlag;													\
	uint	UpdateFlag;													\
};
const uint		MY_CS_WORK_GROUP_SIZE = 128;

//MUCS_UNIFORM;
auto g_strMyCSUnifom = TO_STRING_MARCO(MUCS_UNIFORM);

MUCS_UNIFORM;
////https://community.arm.com/developer/tools-software/graphics/b/blog/posts/get-started-with-compute-shaders
auto g_strShaderTestt = R"(
	#version 310 es
	// The uniform parameters which is passed from application for every frame.
	uniform float radius;
	// Declare custom data struct, which represents either vertex or colour.
	struct Vector3f
	{
		  float x;
		  float y;
		  float z;
		  float w;
	};
	// Declare the custom data type, which represents one point of a circle.
	// And this is vertex position and colour respectively.
	// As you may already noticed that will define the interleaved data within
	// buffer which is Vertex|Colour|Vertex|Colour|;K
	struct AttribData
	{
		  Vector3f v;
		  Vector3f c;
	};
	// Declare input/output buffer from/to wich we will read/write data.
	// In this particular shader we only write data into the buffer.
	// If you do not want your data to be aligned by compiler try to use:
	// packed or shared instead of std140 keyword.
	// We also bind the buffer to index 0. You need to set the buffer binding
	// in the range [0..3];V this is the minimum range approved by Khronos.
	// Notice that various platforms might support more indices than that.
	layout(std140, binding = 0) buffer destBuffer
	{
		  AttribData data[];
	} outBuffer;

	// Declare what size is the group. In our case is 8x8, which gives
	// 64 group size.
	layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
	// Declare main program function which is executed once
	// glDispatchCompute is called from the application.
	void main()
	{
		  // Read current global position for this thread
		  ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
		  // Calculate the global number of threads (size) for this
		  uint gWidth = gl_WorkGroupSize.x * gl_NumWorkGroups.x;
		  uint gHeight = gl_WorkGroupSize.y * gl_NumWorkGroups.y;
		  uint gSize = gWidth * gHeight;
		  // Since we have 1D array we need to calculate offset.
		  uint offset = storePos.y * gWidth + storePos.x;
		  // Calculate an angle for the current thread
		  float alpha = 2.0 * 3.14159265359 * (float(offset) / float(gSize));
		  // Calculate vertex position based on the already calculate angle
		  // and radius, which is given by application
		  outBuffer.data[offset].v.x = sin(alpha) * radius;
		  outBuffer.data[offset].v.y = cos(alpha) * radius;
		  outBuffer.data[offset].v.z = 0.0;
		  outBuffer.data[offset].v.w = 1.0;
		  // Assign colour for the vertex
		  outBuffer.data[offset].c.x = storePos.x / float(gWidth);
		  outBuffer.data[offset].c.y = 0.0;
		  outBuffer.data[offset].c.z = 1.0;
		  outBuffer.data[offset].c.w = 1.0;
	}
)";



//https://github.com/mattdesl/lwjgl-basics/wiki/GLSL-Versions
//https://www.uni-weimar.de/fileadmin/user/fak/medien/professuren/Computer_Graphics/CG_WS_18_19/CG/06_ShaderBuffers.pdf
//page 3
//page5
//There are 4 alternatives Memory Layouts:
//packed: performance¡Voriented and implementation¡Vdetermined, the
//layout is determined by the internals and the actual exchanged data
//may vary.
//shared: loosened version of packed; no variation on the exchanged
//data format, a certain type of Block has the same layout in different
//programs.
//std140: fixed layout at declaration time, wastes space introducing
//paddings to 16¡Vbits multiples.
//std430: similar to std140, but without the additional paddings (to
//be used only with Shader Storage Blocks)
//
//https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL
const char* g_strMyCS =
R"(							
		layout(std140, binding=1) buffer MyEmitterData
		{
			sEmitterData EmitterData;
		};
		layout( std140, binding=2 ) buffer ParticlesSSOIn
		{
			sParticlesSSO ParticlesSSODataIn[];
		};
		layout( std140, binding=3 ) buffer ParticlesSSOOut
		{
			sParticlesSSO ParticlesSSODataOut[];
		};

		layout( std140, binding=4 ) buffer ParticlesMatrix
		{
			mat4 Matrices[];
		};

		layout(local_size_x = 128,  local_size_y = 1, local_size_z = 1) in;
	
		void	InitPolicy(uint e_iID)
		{
			ParticlesSSODataIn[e_iID].bInit = true;
			ParticlesSSODataIn[e_iID].fLife = EmitterData.fMinTime;
		}

		void	ActPolicy(uint e_iID)
		{
			ParticlesSSODataIn[e_iID].fLife -= 0.4;
		}

		void main()
		{
			//https://www.opengl.org/sdk/docs/reference_card/opengl44-quick-reference-card.pdf
			//page6.
			atomicAdd(EmitterData.iTotalDispatchCount,1);
			uint iLocalID =  gl_LocalInvocationID.x;
			uint iID =		gl_GlobalInvocationID.x;
			ParticlesSSODataIn[iID].iIndex = iID;
			ParticlesSSODataIn[iID].iOldIndex = iLocalID;
			//
			//Thread group size should be a multiple of the wavefront size (64)
			//Otherwise, [(Thread Group Size) mod 64] threads go unused!
			//
			if( iID >= EmitterData.iTotalParticleCount )
			{
				atomicAdd(EmitterData.iOverDispatchCount,1);
				return;
			}
			if(iID>=EmitterData.iDoParticleInitStartIndex)
			{
				InitPolicy(iID);
			} 
			ActPolicy(iID);
			//don't need this?.
			//memoryBarrier(); //to make all thread all done.
			//memoryBarrierShared();
			if(ParticlesSSODataIn[iID].fLife<=0)
			{
				ParticlesSSODataIn[iID].bInit = false;
				//https://stackoverflow.com/questions/41667701/execution-order-of-threads-in-opengl-compute-shader
				//https://www.khronos.org/opengl/wiki/Compute_Shader#Atomic_operations
				atomicAdd(EmitterData.iTotalParticleCount,-1);
				return;
			}
			int l_iIndex = atomicAdd(EmitterData.iAtomicCounterForOutIndexOfParticleSSO,1);
			ParticlesSSODataOut[l_iIndex] = ParticlesSSODataIn[iID];
			ParticlesSSODataOut[l_iIndex].iOldIndex = iID;
			ParticlesSSODataOut[l_iIndex].iIndex = l_iIndex;
		}

)";

const char* l_strComputerShader =
R"(
		#version 310 es
		layout(local_size_x = 128) in;
		layout(std430) buffer;
		layout(binding = 0) writeonly buffer Output {
			vec4 elements[];
		} output_data;
		layout(binding = 1) readonly buffer Input0 {
			vec4 elements[];
		} input_data0;
		layout(binding = 2) readonly buffer Input1 {
			vec4 elements[];
		} input_data1;
		void main()
		{
			uint ident = gl_GlobalInvocationID.x;
			output_data.elements[ident] = input_data0.elements[ident] * input_data1.elements[ident];
		}

	)";