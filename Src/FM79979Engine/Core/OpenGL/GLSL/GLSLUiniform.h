#pragma once
//#ifndef WIN32
#if defined(IOS)
	#include <OpenGLES/ES2/gl.h>
	#include <OpenGLES/ES2/glext.h>
#elif defined(ANDROID)
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#include <GLES2/gl2platform.h>
#endif
namespace FATMING_CORE
{
	#define GEN_TEX_UNIT	0
	#define GEN_TEX_ID		100

	#define SAMPLE_FLOAT        0x1
	#define SAMPLE_FLOAT_VEC2   0x2
	#define SAMPLE_FLOAT_VEC3   0x3
	#define SAMPLE_FLOAT_VEC4   0x4
	#define SAMPLE_INT          0x5
	#define SAMPLE_FLOAT_MAT4   0xd
	#define SAMPLE_PROGRAM      0xe

	#define	UNIFORM_DATA_TYPE_cMatrix44	L"cMatrix44"
	#define	UNIFORM_DATA_TYPE_FLOAT1	L"Float1"
	#define	UNIFORM_DATA_TYPE_FLOAT2	L"Float2"
	#define	UNIFORM_DATA_TYPE_FLOAT3	L"Float3"
	#define	UNIFORM_DATA_TYPE_FLOAT4	L"Float4"
	#define	UNIFORM_DATA_TYPE_INT		L"int"

	//=================================================================================================================================
	/// cUniformData class - Contains the actual data for uniforms, textures, and attributes.
	//=================================================================================================================================
	class cUniformData
	{
	public:
	   /// \brief Constructor.
	   cUniformData();

	   /// \brief Destructor.
	   virtual ~cUniformData();

	public:
	   /// Locations in uniform/texture/attribute index (or location)
	   int          m_iLocation;

	   /// Text name used for this uniform or attribute
	   char*		m_strName;

	   /// Type of data used to represent this uniform
	   GLenum       m_eDatatype;

	   /// Matrix data
	   //float        m_matrixData[4][4];

	   /// Matrix and Vector float data
	   union
	   {
			float        m_fData[4*4];
			int			 m_iData[4*4];
	   };
	};

	//===================================
	bool LoadShaderObject( const char* e_strFilename, GLuint e_uiShaderHandle );
	//=================================================================================================================================
	/// Uniform class - Derived from cUniformData class.  This class loads and sets the uniforms, attributes, and textures.
	//=================================================================================================================================

	class	cGLSLProgram:public NamedTypedObject
	{
		typedef std::vector<cUniformData*>	   cUniformDataList;
		typedef cUniformDataList::iterator	cUniformDataIterator;

		/// Vector of all the data for the different uniforms/textures/attributes.
		cUniformDataList m_Uniforms;
		//
		GET_SET_DEC(GLuint,m_uiProgramHandle,GetProgramHandle,SetProgramHandle);
		//position,tex,color....etc attribute data loation and name
		//it should be fix order as follow
		std::map<int,string>	m_AttributeDataVector;
	   /// GL vertex shader id
	   GLuint         m_uiVertShaderHandle;
	   /// GL fragment shader id
	   GLuint         m_uiFragShaderHandle;
	public:
		cGLSLProgram():m_uiVertShaderHandle(0),m_uiFragShaderHandle(0){}
		virtual ~cGLSLProgram(){ FreeAllData(); }
		//virtual	NamedTypedObject*	Clone(){ return 0; }
		//
		void			AddUniform(cUniformData*e_pUniformData){ m_Uniforms.push_back(e_pUniformData); }
		cUniformData*	GetUniform(char*e_strName);
		/// \brief Updates the value of a uniform
		bool			UpdateOneUniform( const char* e_strName, float* e_pfVals );
		/// \brief Deletes all the GL resources that we have loaded
		void			FreeAllData();
		/// \brief Updates all the uniform data after a link
		void			UpdateAllUniforms();
		//bond program and attribute location
		void			UsingProgram();
		void			DisableProgram();
		//
		void			AddAttributeData(const char*e_strName,int e_iLocation);
		void			AddAttributeData(std::string e_strName,int e_iLocation);
	};
	extern cGLSLProgram*g_pCurrentUsingGLSLProgram;
	//<GLSL ProgramName="mainProg" VS="data/vertex.glsl" FS="data/fragment.glsl" >
	//	<!-- sample 1D,2D,3D -->
	//	<please add manual/>
	//	<!-- attribute enusude u call the vertexpointer and setup index to cooresponded-->
	//	<Attribute Name="VSPosition">
	//	<Attribute Name="VSTexcoord">
	//	<Attribute Name="VSNormal">
	//	<!-- uniform -->
	//	<Uniform Name="MatrixVP" DataType="cMatrix44" Data="1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f,"/>
	//	<Uniform Name="LightPos" DataType="float3" Data="0.f,0.f,0.f"/>	
	//<GLSL/>
	class cUniformManager : public ISAXCallback,public cNamedTypedObjectVector<cGLSLProgram>
	{
		//below data for assist parse xml.
		cUniformData*	m_pCurrentUniformData;
		cGLSLProgram*	m_pCurrentGLSLProgram;
		// The most recent program handle (used to know which program attribs and uniforms are bound to)
		GLuint m_uiRecentProgramHandle;
	//=========for parse glsl extra data.
		void	ProcessGLSLData();
		//void	ProcessTextureData();
		void	ProcessAttributeData();
		void	ProcessUniformData();
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement)override;
	public:
		DEFINE_TYPE_INFO()
		cUniformManager();
		virtual ~cUniformManager();
	};
}