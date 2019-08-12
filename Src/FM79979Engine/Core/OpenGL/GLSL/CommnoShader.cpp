#include "../../stdafx.h"
#include "Shader.h"
//https://en.wikipedia.org/wiki/OpenGL_Shading_Language

//GLSL Version	OpenGL Version	Date			Shader Preprocessor
//1.10.59[1]	2.0				April 2004		#version 110
//1.20.8[2]		2.1				September 2006	#version 120
//1.30.10[3]	3.0				August 2008		#version 130
//1.40.08[4]	3.1				March 2009		#version 140
//1.50.11[5]	3.2				August 2009		#version 150
//3.30.6[6]		3.3				February 2010	#version 330
//4.00.9[7]		4.0				March 2010		#version 400
//4.10.6[8]		4.1				July 2010		#version 410
//4.20.11[9]	4.2				August 2011		#version 420
//4.30.8[10]	4.3				August 2012		#version 430
//4.40[11]		4.4				July 2013		#version 440
//4.50[12]		4.5				August 2014		#version 450
//4.60[13]		4.6				July 2017		#version 460
//shader opengl ES2 to openGL ES3
//
//http://www.shaderific.com/blog/2014/3/13/tutorial-how-to-update-a-shader-for-opengl-es-30
//OpenGL ES 2.0 / GLSL 1.00:					OpenGL ES 3.0 / GLSL 3.00 :
//attribute vec4 position;						in vec4 position;
//attribute vec2 texture;						in vec2 texCoord;
//varying vec2 textureCoordinate;				out vec2 textureCoordinate;
//varying vec2 textureCoordinate;				in vec2 textureCoordinate;
//mediump vec4 gl_FragColor;					out vec4 fragmentColor;
//vec4 texture2D(sampler2D, vec2 coord);		vec4 texture(sampler{ 2,3 }D sampler, vec2 coord);
//vec4 textureCube(samplerCube, vec3 coord;	vec4 texture(samplerCube sampler, vec3 coord);




//gl_Color
//gl_FrontColor
//gl_BackColor

//the value here,1 represent sizeof(float)
int	g_iFVF_DataStride[TOTAL_FVF] = {
									3,			//pos,0
									3,			//normal,1
									4,			//diffuse,2
									3,			//tagent,3
									3,			//bitagent,4
									4,			//influnce skinning weight,5
									4,			//influnce skinning bone indicex,6,4 byte as 1 float
									2,			//UV1,7
									2};			//UV2,8

GLenum	g_iFVF_DataType[TOTAL_FVF] = {	
									GL_FLOAT,			//pos
									GL_FLOAT,			//normal
									GL_FLOAT,			//diffuse
									GL_FLOAT,			//tagent
									GL_FLOAT,			//bitagent
									GL_FLOAT,			//influnce skinning weight
									GL_UNSIGNED_BYTE,	//influnce skinning bone indicex
									GL_FLOAT,			//UV1
									GL_FLOAT};			//UV2

char*g_strShaderAttribution[TOTAL_FVF] = {"VSPosition",		//0
											"VSNormal",				//1
											"VSColor",				//2
											"VSTangent",			//3
											"VSBiTangent",			//4
											"VSWeights",			//5
											"VSInflunceBoneIndices",//6
											"VSTexcoord",			//7
											"VSTexcoord1"};			//8
int		g_iFVF_DataSize[TOTAL_FVF] = {
#ifndef IOS
										(int)(g_iFVF_DataStride[0]*sizeof(float)),
										(int)(g_iFVF_DataStride[1]*sizeof(float)),
										(int)(g_iFVF_DataStride[2]*sizeof(float)),
										(int)(g_iFVF_DataStride[3]*sizeof(float)),
										(int)(g_iFVF_DataStride[4]*sizeof(float)),
										(int)(g_iFVF_DataStride[5]*sizeof(float)),
										(int)(g_iFVF_DataStride[6]*sizeof(unsigned char)),
										(int)(g_iFVF_DataStride[7]*sizeof(float)),
										(int)(g_iFVF_DataStride[8]*sizeof(float)),
#else
	g_iFVF_DataStride[0]*4,
	g_iFVF_DataStride[1]*4,
	g_iFVF_DataStride[2]*4,
	g_iFVF_DataStride[3]*4,
	g_iFVF_DataStride[4]*4,
	g_iFVF_DataStride[5]*4,
	g_iFVF_DataStride[6]*1,
	g_iFVF_DataStride[7]*4,
	g_iFVF_DataStride[8]*4,
#endif
										};
int	GetStrideByFVF(int64 e_iFVF)
{
	int	l_iStride = 0;
	for( int i=0;i<TOTAL_FVF;++i )
	{
		if( BitMoveLeft(1,i) & e_iFVF )
		{
			l_iStride += g_iFVF_DataSize[i];
		}
	}
	return l_iStride;
}
//http://developer.apple.com/library/ios/#documentation/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/OpenGLESPlatforms/OpenGLESPlatforms.html
//OpenGL ES 2.0 on the PowerVR SGX
//Limits
//
//The maximum 2D or cube map texture size is 2048 x 2048. This is also the maximum renderbuffer size and viewport size.
//You can use up to 8 textures in a fragment shader. You cannot use texture lookups in a vertex shader.
//You can use up to 16 vertex attributes.
//You can use up to 8 varying vectors.
//You can use up to 128 uniform vectors in a vertex shader and up to 64 uniform vectors in a fragment shader.
//Points can range in size from 1.0 to 511.0 pixels.
//Lines can range in width from 1.0 to 16.0 pixels.
namespace FATMING_CORE
{
	//														pos, normal,diffuse,tangent,bitangent,  wight,bone,tex0, tex1
	bool	g_bCommonVSClientState[TOTAL_FVF]			=	{true,false, true,   false,  false,    false,false,true, false};
	bool	g_bCommonVSNoTextureClientState[TOTAL_FVF]	=	{true,false, false,  false,  false,    false,false,false,false};
	bool	g_bMyMeshVSClientState[TOTAL_FVF]			=	{true,true , false,  false,  false,    false,false,true, false};
	bool	g_bMySkinningMeshVSClientState[TOTAL_FVF]	=	{true,true , false,  false,  false,    false,false,true, false};
	//tex,color,pos
	wchar_t*DEFAULT_SHADER = L"Default";
	//pos only
	wchar_t*NO_TEXTURE_SHADER = L"NoTexture";
	//tex,color,pos,normal
	wchar_t*STATIC_MESH_SHADER = L"MeshShader";
	//pos,normal,bone,weight,tex
	wchar_t*SKINNING_MESH_SHADER = L"SkinningMeshShader";
	//precision mediump is not support in the MACbook Air.....fuck
	//and iphone shader should use lowp instead dsclare precision mediump ,
	//because mesh shader occur crash......fuck.
	//for opengl es1 compatibility,so we need color vertex buffer
	char*g_strCommonVS = "									\
	attribute vec3 VSPosition;								\
	attribute vec4 VSColor;									\
	attribute vec2 VSTexcoord;								\
	uniform mat4 matVP;										\
	uniform mat4 matW;										\
	varying vec2 PSTexcoord;								\
	varying vec4 PSColor;									\
	void main()												\
	{														\
		gl_Position = matVP*matW*vec4(VSPosition,1);		\
		PSTexcoord = VSTexcoord;							\
		PSColor = VSColor;									\
	}";
#if defined(IOS) || defined(ANDROID) || defined (WASM)
	char*g_strCommonFS = "										\
	uniform sampler2D texSample;								\
	varying lowp vec2 PSTexcoord;								\
	varying lowp vec4 PSColor;									\
																\
	void main()													\
	{															\
		gl_FragColor = texture2D(texSample, PSTexcoord)+(PSColor-vec4(1,1,1,1));\
	}";
#else
	char*g_strCommonFS = "										\
	uniform sampler2D texSample;								\
	varying vec2 PSTexcoord;									\
	varying vec4 PSColor;										\
																\
	void main()													\
	{															\
		gl_FragColor = texture2D(texSample, PSTexcoord)+(PSColor-vec4(1,1,1,1));\
	}";

	//void main()													\
	//{															\
	//	gl_FragColor = texture2D(texSample, PSTexcoord) + (PSColor - vec4(1, 1, 1, 1)); \
	//}";

#endif

	char*g_strColorFulFS = "													\
	uniform sampler2D texSample;												\
	varying vec2 PSTexcoord;													\
	varying vec4 PSColor;														\
	uniform vec4 MyColor;														\
																				\
	void main()																	\
	{																			\
		gl_FragColor = texture2D(texSample, PSTexcoord);						\
		gl_FragColor.xyz = gl_FragColor.xyz+(PSColor.xyz-vec3(0.5,0.5,0.5));	\
	}";
//=======================
//
//=======================
	char*g_strCommonVSNoTexture = "							\
	attribute vec3 VSPosition;								\
	attribute vec2 VSTexcoord;								\
	uniform mat4 matVP;										\
	uniform mat4 matW;										\
	uniform vec4 Color;										\
	varying vec4 PSColor;									\
	void main()												\
	{														\
		gl_Position = matVP*matW*vec4(VSPosition,1);		\
		PSColor = Color;									\
	}";
#if defined(IOS) || defined(ANDROID)|| defined (WASM)
	char*g_strCommonFSNoTexture = "							\
	varying lowp vec4 PSColor;								\
	void main()												\
	{														\
		gl_FragColor = PSColor;								\
	}";
#else
	char*g_strCommonFSNoTexture = "							\
	varying vec4 PSColor;									\
	void main()												\
	{														\
		gl_FragColor = PSColor;								\
	}";
#endif
//=======================
//for mesh
//=======================
	char*g_strMyMeshVS = "									\
	attribute vec3 VSPosition;								\
	attribute vec3 VSNormal;								\
	attribute vec2 VSTexcoord;								\
	uniform mat4 matVP;										\
	uniform mat4 matW;										\
	varying vec2 PSTexcoord;								\
	varying vec3 PSNormal;									\
	void main()												\
	{														\
		gl_Position = matVP*matW*vec4(VSPosition,1);		\
		PSTexcoord = VSTexcoord;							\
		PSNormal = VSNormal;								\
	}";

	char*g_strMyMeshWithVertexColorVS = "					\
	attribute vec3 VSPosition;								\
	attribute vec3 VSNormal;								\
	attribute vec4 VSColor;									\
	attribute vec2 VSTexcoord;								\
	uniform mat4 matVP;										\
	uniform mat4 matW;										\
	varying vec2 PSTexcoord;								\
	varying vec3 PSNormal;									\
	varying vec4 PSColor;									\
	void main()												\
	{														\
		gl_Position = matVP*matW*vec4(VSPosition,1);		\
		PSTexcoord = VSTexcoord;							\
		PSNormal = VSNormal;								\
		PSColor = VSColor;									\
	}";

#if defined(WIN32)
	char*g_strMyMeshWithVertexColorFS = "						\
	uniform sampler2D texSample;								\
	uniform vec4 PSColor;										\
	varying vec2 PSTexcoord;									\
	varying vec3 PSNormal;										\
																\
	void main()													\
	{															\
		gl_FragColor = texture2D(texSample, PSTexcoord)+(PSColor-vec4(1,1,1,1));\
	}";
	char*g_strMyMeshFS = "										\
	uniform sampler2D texSample;								\
	varying vec2 PSTexcoord;									\
	varying vec3 PSNormal;										\
																\
	void main()													\
	{															\
		gl_FragColor = texture2D(texSample, PSTexcoord);			\
	}";
#else
	char*g_strMyMeshWithVertexColorFS = "						\
	uniform sampler2D texSample;								\
	varying lowp vec4 PSColor;									\
	varying lowp vec2 PSTexcoord;								\
	varying lowp vec3 PSNormal;									\
																\
	void main()													\
	{															\
		gl_FragColor = texture2D(texSample, PSTexcoord)*PSColor;\
	}";

	char*g_strMyMeshFS = "										\
	uniform sampler2D texSample;								\
	varying lowp vec2 PSTexcoord;								\
	varying lowp vec3 PSNormal;									\
																\
	void main()													\
	{															\
		gl_FragColor = texture2D(texSample, PSTexcoord);		\
	}";
#endif

//matBones size will be different on different device...
#if defined(WIN32)
	char*g_strMySkinningMeshWithVertexColorVS = "			\
	attribute	vec3	VSPosition;							\
	attribute	vec3	VSNormal;							\
	attribute	vec4	VSColor;							\
	attribute	vec2	VSTexcoord;							\
    attribute	vec4	VSWeights;							\
    attribute	vec4	VSInflunceBoneIndices;				\
	uniform		mat4	matVP;								\
	uniform		mat4	matW;								\
	uniform		mat4	matBones[200];						\
	varying		vec2	PSTexcoord;							\
	varying		vec3	PSNormal;							\
	varying		vec4	PSColor;							\
	void main()												\
	{																						\
		gl_Position = vec4(0,0,0,0);														\
		PSNormal = vec3(0,0,0);																\
		vec4 curIndex = VSInflunceBoneIndices;												\
		vec4 curWeight = VSWeights;															\
		for( int i=0;i<4;i++ )																\
		{																					\
				mat4 m44 = matBones[int(curIndex.x)];										\
				gl_Position += m44 * vec4(VSPosition,1) * curWeight.x;						\
				mat3 m33 = mat3(m44[0].xyz,													\
								m44[1].xyz,													\
								m44[2].xyz);												\
				PSNormal += m33 * VSNormal * curWeight.x;									\
				curIndex = curIndex.yzwx;													\
				curWeight = curWeight.yzwx;													\
		}																					\
		gl_Position = matVP *matW* gl_Position;												\
		PSTexcoord = VSTexcoord;															\
		PSNormal = VSNormal;																\
		PSColor = VSColor;																	\
	}";
	char*g_strMySkinningMeshWithVertexColorFS = "								\
	uniform sampler2D texSample;								\
	uniform vec4 PSColor;										\
	varying vec2 PSTexcoord;									\
	varying vec3 PSNormal;										\
																\
	void main()													\
	{															\
		gl_FragColor = texture2D(texSample, PSTexcoord)+(PSColor-vec4(1,1,1,1));\
	}";
	char*g_strMySkinningMeshVS = "			\
	attribute	vec3	VSPosition;							\
	attribute	vec3	VSNormal;							\
	attribute	vec2	VSTexcoord;							\
    attribute	vec4	VSWeights;							\
    attribute	vec4	VSInflunceBoneIndices;				\
	uniform		mat4	matVP;								\
	uniform		mat4	matW;								\
	uniform		mat4	matBones[200];						\
	varying		vec2	PSTexcoord;							\
	varying		vec3	PSNormal;							\
	void main()												\
	{																						\
		gl_Position = vec4(0,0,0,0);														\
		PSNormal = vec3(0,0,0);																\
		vec4 curIndex = VSInflunceBoneIndices;												\
		vec4 curWeight = VSWeights;															\
		for( int i=0;i<4;i++ )																\
		{																					\
				mat4 m44 = matBones[int(curIndex.x)];										\
				gl_Position += m44 * vec4(VSPosition,1) * curWeight.x;						\
				mat3 m33 = mat3(m44[0].xyz,													\
								m44[1].xyz,													\
								m44[2].xyz);												\
				PSNormal += m33 * VSNormal * curWeight.x;									\
				curIndex = curIndex.yzwx;													\
				curWeight = curWeight.yzwx;													\
		}																					\
		gl_Position = matVP *matW* gl_Position;												\
		PSTexcoord = VSTexcoord;															\
		PSNormal = VSNormal;																\
	}";
	char*g_strMySkinningMeshFS = "								\
	uniform sampler2D texSample;								\
	varying vec2 PSTexcoord;									\
	varying vec3 PSNormal;										\
																\
	void main()													\
	{															\
		gl_FragColor = texture2D(texSample, PSTexcoord);		\
	}";

#else
//because some device bufferis not enough so 32 is a safe range
	char*g_strMySkinningMeshVS = "							\
	attribute	vec3	VSPosition;							\
	attribute	vec3	VSNormal;							\
	attribute	vec2	VSTexcoord;							\
    attribute	vec4	VSWeights;							\
    attribute	vec4	VSInflunceBoneIndices;				\
	uniform		mat4	matVP;								\
	uniform		mat4	matW;								\
	uniform		mat4	matBones[32];						\
	varying		vec2	PSTexcoord;							\
	varying		vec3	PSNormal;							\
	void main()												\
	{																						\
		gl_Position = vec4(0,0,0,0);														\
		PSNormal = vec3(0,0,0);																\
		vec4 curIndex = VSInflunceBoneIndices;												\
		vec4 curWeight = VSWeights;															\
		for( int i=0;i<4;i++ )																\
		{																					\
				mat4 m44 = matBones[int(curIndex.x)];										\
				gl_Position += m44 * vec4(VSPosition,1) * curWeight.x;						\
				mat3 m33 = mat3(m44[0].xyz,													\
								m44[1].xyz,													\
								m44[2].xyz);												\
				PSNormal += m33 * VSNormal * curWeight.x;									\
				curIndex = curIndex.yzwx;													\
				curWeight = curWeight.yzwx;													\
		}																					\
		gl_Position = matVP *matW* gl_Position;												\
		PSTexcoord = VSTexcoord;															\
		PSNormal = VSNormal;																\
	}";

	char*g_strMySkinningMeshFS = "								\
	uniform sampler2D texSample;								\
	varying lowp vec2 PSTexcoord;								\
	varying lowp vec3 PSNormal;									\
																\
	void main()													\
	{															\
		gl_FragColor = texture2D(texSample, PSTexcoord);		\
	}";
//because some device bufferis not enough so 32 is a safe range
	char*g_strMySkinningMeshWithVertexColorVS = "			\
	attribute	vec3	VSPosition;							\
	attribute	vec3	VSNormal;							\
	attribute	vec4	VSColor;							\
	attribute	vec2	VSTexcoord;							\
    attribute	vec4	VSWeights;							\
    attribute	vec4	VSInflunceBoneIndices;				\
	uniform		mat4	matVP;								\
	uniform		mat4	matW;								\
	uniform		mat4	matBones[32];						\
	varying		vec2	PSTexcoord;							\
	varying		vec3	PSNormal;							\
	varying		vec4	PSColor;							\
	void main()												\
	{																						\
		gl_Position = vec4(0,0,0,0);														\
		PSNormal = vec3(0,0,0);																\
		vec4 curIndex = VSInflunceBoneIndices;												\
		vec4 curWeight = VSWeights;															\
		for( int i=0;i<4;i++ )																\
		{																					\
				mat4 m44 = matBones[int(curIndex.x)];										\
				gl_Position += m44 * vec4(VSPosition,1) * curWeight.x;						\
				mat3 m33 = mat3(m44[0].xyz,													\
								m44[1].xyz,													\
								m44[2].xyz);												\
				PSNormal += m33 * VSNormal * curWeight.x;									\
				curIndex = curIndex.yzwx;													\
				curWeight = curWeight.yzwx;													\
		}																					\
		gl_Position = matVP *matW* gl_Position;												\
		PSTexcoord = VSTexcoord;															\
		PSNormal = VSNormal;																\
		PSColor = VSColor;																	\
	}";
	char*g_strMySkinningMeshWithVertexColorFS = "				\
	uniform sampler2D texSample;								\
	varying lowp vec4 PSColor;									\
	varying lowp vec2 PSTexcoord;								\
	varying lowp vec3 PSNormal;									\
																\
	void main()													\
	{															\
		gl_FragColor = texture2D(texSample, PSTexcoord)*PSColor;\
	}";

#endif
//end namespace FATMING_CORE
}
//end _COMMON_SHADER_H_