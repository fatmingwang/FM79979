#ifndef	GLSL_UINIFORM
#define	GLSL_UINIFORM
//#ifndef WIN32
#if defined(IOS)
	#include <OpenGLES/ES2/gl.h>
	#include <OpenGLES/ES2/glext.h>
#elif defined(ANDROID)
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES/glplatform.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#include <GLES2/gl2platform.h>
#endif
namespace FATMING_CORE
{
	#define GEN_TEX_UNIT 0
	#define GEN_TEX_ID 100

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
	   
	   /// Vector int data
	   int          m_iIntData[4];

	   /// GL texture id
	   GLuint       m_uiTextureHandle;

	   /// GL texture unit number
	   GLuint       m_uiTextureUnit;

	   /// Keeps track of whether this is depth texture or not
	   bool         m_bDepthTex;

	   /// Texture width
	   int          m_iWidth;

	   /// Texture height
	   int          m_iHeight;

	   /// GL fbo id
	   GLuint         m_uiFBOHandle;

	   /// GL color render buffer handle
	   GLuint         m_uiColorRbHandle;

	   /// GL vertex shader id
	   GLuint         m_uiVertShaderHandle;

	   /// GL fragment shader id
	   GLuint         m_uiFragShaderHandle;
	};

	//===================================
	bool LoadShaderObject( const char* e_strFilename, GLuint e_uiShaderHandle );
	//=================================================================================================================================
	/// Uniform class - Derived from cUniformData class.  This class loads and sets the uniforms, attributes, and textures.
	//=================================================================================================================================
	//attribute data has follow below
	//1st:position,2nd:normal,3rd:texture,4th;color,5th:tangent,6th:bitangent
	//attribute the data has set(position ,color...)so we could know the mesh data
	//uniform the data has setup let gl_xxxx to get relate information and varying data could be out
	//varying is the data for out(color position and....etc)
	//Attribute data has order as below
	//POSITION,NORMAL,TEXTURE,COLOR,TANGENT,BITANGENT

	#define	VERTEX_ATTRIBUTE_POSITION	L"POSITION"		//attribute location 0
	#define	VERTEX_ATTRIBUTE_NORMAL		L"NORMAL"		//attribute location 1
	#define	VERTEX_ATTRIBUTE_TEXTURE	L"TEXTURE"		//attribute location 2
	#define	VERTEX_ATTRIBUTE_COLOR		L"COLOR"			//attribute location 3
	#define	VERTEX_ATTRIBUTE_TANGENT	L"TANGENT"		//attribute location 4
	#define	VERTEX_ATTRIBUTE_BITANGENT	L"BITANGENT"		//attribute location 5

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
		//1st:position,2nd:normal,3rd:texture,4th;color,5th:tangent,6th:bitangent
		std::map<int,string>	m_AttributeDataVector;
	public:
		cGLSLProgram(){}
		virtual ~cGLSLProgram(){ FreeAllData(); }
		virtual	NamedTypedObject*	Clone(){ return 0; }
		//
		void	AddUniform(cUniformData*e_pUniformData){ m_Uniforms.push_back(e_pUniformData); }
		cUniformData*GetUniform(char*e_strName);
		/// \brief Updates the value of a uniform
		bool UpdateOneUniform( const char* e_strName, float* e_pfVals );

		///// \brief Gets the program handle
		//GLuint GetProgramHandle( const char* e_strName );

		/// \brief Binds a texture into GL
		bool BindTexture( const char* e_strName, int e_iWidth, int e_iHeight );

		/// \brief Binds an FBO into GL
		bool BindFbo( const char* e_sreName, int e_iWidth, int e_iHeight );

		/// \brief Deletes all the GL resources that we have loaded
		void FreeAllData();

		/// \brief Updates all the uniform data after a link
		void UpdateAllUniforms();
		//bond program and attribute location
		void	UsingProgram();
		void	DisableProgram();
		//
		void	AddAttributeData(const char*e_strName,int e_iLocation);
		void	AddAttributeData(std::string e_strName,int e_iLocation);
	};
	extern cGLSLProgram*g_pCurrentUsingGLSLProgram;
	//<GLSL ProgramName="mainProg" VS="data/vertex.glsl" FS="data/fragment.glsl" >
	//	<!-- sample 1D,2D,3D -->
	//	<Texture Name="tex0" samplerDimesion="2" Unit="0" />
	//	<!-- attribute enusude u call the vertexpointer and setup index to cooresponded-->
	//	<Attribute Name="position" TYPE="POSITION">
	//	<Attribute Name="texture" TYPE="TEXTURE">
	//	<Attribute Name="normal" TYPE="NORMAL">
	//	<!-- uniform -->
	//	<Uniform Name="MatrixVP" DataType="float44" Data="1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,1.f,"/>
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
		void	ProcessTextureData();
		void	ProcessAttributeData();
		void	ProcessUniformData();
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
	public:
		DEFINE_TYPE_INFO();
		cUniformManager();
		virtual ~cUniformManager();
	};

	//if u want to porting to iPhone3GS or Android 2.0 platform u have to call those
	//using a default shader for 2D only
	bool	Init2DShader(char*e_strFileName="Media/Shader/2D/2DShaderSetup.xml");
	//delete the data above
	void	Destroy2DShader();
}

#endif