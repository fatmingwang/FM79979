#pragma once

#include "../Glh.h"
#include "../../../Common/NamedTypedObject.h"
#include "../../../Common/Template/TemplateClass.h"

#include "ShaderStorageBuffer.h"

#include <functional>

#define	A_QUAD_TWO_TRIANGLES						2
#define	A_QUAD_4_TRIANGLES							4
#define	TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT			6           //two triangle could be a quad,one triangle is 3 two triangle is 6
#define	TWO_TRIANGLE_VERTICES_UV_TO_QUAD_UV_COUNT	12    //2 trianle is 6 vertices,each vertex has U and V 6*2=12
#define	TWO_TRIANGLE_STRIP_UV_TO_QUAD_UV_COUNT		8     //2 triangles strip are 4 vertices equal one quad.
//
//Flexible Vertex Format Constants, or FVF codes,
//are used to describe the contents of vertices interleaved in a single data
//stream that will be processed by the fixed-function pipeline.
//
#define	FVF_POS					0//xyz  3float
#define	FVF_NORMAL				1//xyz  3float
#define	FVF_DIFFUSE				2//RGBA 4float
#define	FVF_TAGENT				3//xyz  3float
#define	FVF_BITAGENT			4//xyz  3float
#define	FVF_SKINNING_WEIGHT		5//4 inlunce bones 4float
#define	FVF_SKINNING_BONE_INDEX	6//the data for bone id usually is 4 unsigned byte to become 1 float.if the bone ID is bigger than 512,u have to ensure data swap problem
								 //1*4 byte one float
#define	FVF_TEX0				7//UV	2float
#define	FVF_TEX1				8//UV	2float

#define	FVF_POS_FLAG					1<<FVF_POS
#define	FVF_NORMAL_FLAG					1<<FVF_NORMAL
#define	FVF_DIFFUSE_FLAG				1<<FVF_DIFFUSE
#define	FVF_TAGENT_FLAG					1<<FVF_TAGENT
#define	FVF_BITAGENT_FLAG				1<<FVF_BITAGENT
#define	FVF_SKINNING_WEIGHT_FLAG		1<<FVF_SKINNING_WEIGHT
#define	FVF_SKINNING_BONE_INDEX_FLAG	1<<FVF_SKINNING_BONE_INDEX
#define	FVF_TEX0_FLAG					1<<FVF_TEX0
#define	FVF_TEX1_FLAG					1<<FVF_TEX1

#define	COMMON_MESH_FVF_FLAG			(FVF_POS_FLAG|FVF_NORMAL_FLAG|FVF_TEX0_FLAG)
#define	COMMON_2D_IMAGE_FVF_FLAG		(FVF_POS_FLAG|FVF_TEX0_FLAG)

//#define	FVF_TEX2				9//UV	2float
//#define	FVF_TEX3				10//UV	2float
//#define	FVF_TEX4				11//UV	2float
//#define	FVF_TEX5				12//UV	2float
//#define	FVF_TEX6				13//UV	2float
//#define	FVF_TEX7				14//UV	2float

//last one
#define	TOTAL_FVF				9

extern char*	g_strShaderAttribution[TOTAL_FVF];
extern int		g_iFVF_DataStride[TOTAL_FVF];
extern GLenum	g_iFVF_DataType[TOTAL_FVF];
extern int		g_iFVF_DataSize[TOTAL_FVF];
//opengles 1,2 GL_UNSIGNED_SHORT,win opengles3 GL_UNSIGNED_INT
extern GLenum	g_iDrawindiceType;
int				GetStrideByFVF(int64 e_iFVF);

namespace FATMING_CORE
{
	//for shader data
	extern bool	g_bCommonVSClientState[TOTAL_FVF];
	extern bool	g_bCommonVSNoTextureClientState[TOTAL_FVF];
	extern bool	g_bMyMeshVSClientState[TOTAL_FVF];
	extern bool	g_bMySkinningMeshVSClientState[TOTAL_FVF];

	extern const char*	g_strCommonVS;
	extern const char*	g_strCommonFS;
	extern const char*	g_strCommonVSNoTexture;
	extern const char*	g_strCommonFSNoTexture;
	extern const char*	g_strDrawPointsVS;
	//extern const char*	g_strDrawPointsFS;//use this g_strCommonFSNoTexture
	extern const char*	g_strMyMeshVS;
	extern const char*	g_strMyMeshFS;
	extern const char*	g_strMySkinningMeshVS;
	extern const char*	g_strMySkinningMeshFS;

	extern wchar_t*	DEFAULT_SHADER;
	extern wchar_t*	NO_TEXTURE_SHADER;
	extern wchar_t*	STATIC_MESH_SHADER;
	extern wchar_t*	SKINNING_MESH_SHADER;
	extern wchar_t*	DRAW_POINTS_SHADER;

	typedef	std::function<void(class cBaseShader*)>	f_BindGLDataFunction;
	//https://www.khronos.org/opengl/wiki/Built-in_Variable_(GLSL)
	//built in variables.
	//https://github.com/mattdesl/lwjgl-basics/wiki/GLSL-Versions
	//check GLSL version to call different unfirom data assign function.
	//OpenGL ES Version	GLSL ES Version
	//2.0	100
	//3.0	300
	enum eGLSLVersion
	{
		eGLSL_V100 = 0,
		eGLSL_V300,
		eGLSL_MAX,
	};

	class	cBaseShader:public NamedTypedObject
	{
		public:
		std::string m_strShaderLog;
	protected:
		//to check camera or light updated or something else...not necessary
		bool	m_bDataUpdated;
		GLuint	m_uiVS;
		GLuint	m_uiFS;
		GLuint	m_uiProgram;
		bool	CreateVS(const char*e_strVS);
		bool	CreateFS(const char*e_strPS);
		bool	CreateProgram(const char*e_strVS,const char*e_strPS,bool e_bTexture);
		bool	m_bTexture;
		eGLSLVersion	m_eGLSLVersion = eGLSL_V100;
	public:
		GLuint	m_uiMatrixVPLoacation;
		GLuint	m_uiMatrixWLoacation;
		GLuint	m_uiTexLoacation;
		GLuint	m_uiAttribArray[TOTAL_FVF];
		//for non texture,directly setup pixel shader's color
		GLuint	m_uiColorLoacation;
		//for skinning
		GLuint	m_uiBonesLocation;
		//for Point Size
		GLuint	m_uiPointSize;
		//if e_bTexture is true it texture coordinate attribute and texture UV will be assign.
		cBaseShader(const wchar_t*e_strName,bool e_bTexture = true);
		cBaseShader(const char*e_strVS,const char*e_strPS,const wchar_t*e_strName,bool e_bTexture = true);
		//for opengl es1
		cBaseShader(const wchar_t*e_strName,bool *e_pbClientState);
		cBaseShader();
		virtual	NamedTypedObject*	Clone(){ return nullptr; }
		virtual ~cBaseShader();
		virtual	void				Use(bool e_bUseLastWVPMatrix = true);
		void						Unuse();
		//do something if shader need to do in each frame update.
		virtual	void				Update(float e_fElpaseTime){ m_bDataUpdated = false; }
		void						Disable();
		GLuint						GetUniFormLocationByName(const char*e_strName);
		virtual	void				DebugRender();
		eGLSLVersion				GetGLSLVersion() { return m_eGLSLVersion; }
	};

	//e_pbClientState to setup opengl es client state
	cBaseShader*	CreateShader(bool *e_pbClientState,wchar_t*e_strName = DEFAULT_SHADER);
	//e_pbClientState to setup opengl es client state
	cBaseShader*	CreateShader(bool *e_pbClientState,const char*e_strVS,const char*e_strPS,wchar_t*e_strName = DEFAULT_SHADER);
	cBaseShader*	GetShader(wchar_t*e_strName);
	cBaseShader*	GetCurrentShader();
	//
	//
	//void	ShaderUpdate(float e_fElpaseTime);
	//
	void	DeleteAllShader();
	bool	DeleteShader(wchar_t*e_strName = DEFAULT_SHADER);
	//only call once before draw any 2D image
	//if e_bUseLastWVPMatrix is true it will use the WVP matrix last has been called
	void	UseShaderProgram(const wchar_t*e_strName = DEFAULT_SHADER,bool e_bUseLastWVPMatrix = true);
	void	UseShaderProgram(cBaseShader*e_p2DShader);
	//only call once if u wanna to disable it.
	void	DisableShaderProgram(wchar_t*e_strName = DEFAULT_SHADER);
	//void	SetupWorldViewProjectionMatrix(float*e_pfWVPMatrix);
	//while shader is applied,not set value by camera,ignore rotation,because it has beenn rotated
	void	SetupShaderViewProjectionMatrix(float*e_pfVPMatrix,bool e_bOrientation, eDeviceDirection e_eDeviceDirection = eDD_MAX);
	float*	GetCurrentViewProjectionMatrix();
	//use DRAW_POINTS_SHADER
	void	SetShaderPointSize(float e_fSize);
	void	SetupShaderWorldMatrix(float*e_pfWMatrix);
	//ensure current program is not support texture
	void	SetupShaderColor(Vector4 e_vcolor);
	//ensure current program is not support texture
	void	SetupShaderBonesData(cMatrix44*e_pBoneMatrices,int e_iCount);
	bool	CheckProgram(int Object, int Type,wchar_t*e_strMessage);
	//CheckShader(e_uiShaderHandle, GL_COMPILE_STATUS,L"Compile\n");
	bool	CheckShader(int Object, int Type, const wchar_t*e_strMessage,std::string*e_strOutputLog = nullptr);
	//
	void	SetGlobalScaleColor(Vector4 e_vColor);	
}