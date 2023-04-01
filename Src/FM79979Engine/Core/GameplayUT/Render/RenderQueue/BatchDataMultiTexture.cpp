#include "BatchDataMultiTexture.h"
#include "../Texture/Texture.h"
#include "../CommonRender/SimplePrimitive.h"
#include "../../OpenGL/OpenGLRender.h"
namespace FATMING_CORE
{
	//opengl es use low so it have to be different shader declartion.
	const char* g_strCommonVSWithTextureArray =
		R"(
		attribute vec3 VSPosition;
		attribute vec4 VSColor;
		attribute vec3 VSTexcoord;
		uniform mat4 matVP;
		uniform mat4 matW;
		varying vec3 PSTexcoord;
		varying vec4 PSColor;
		void main()
		{
			gl_Position = matVP*matW*vec4(VSPosition,1);
			PSTexcoord = VSTexcoord;
			PSColor = VSColor;
		}
	)";

	const char* g_strColorFulFSWithTextureArray =
		R"(
		#define numTextures 4
		uniform sampler2D texSample[numTextures];
		varying vec3 PSTexcoord;
		varying vec4 PSColor;
		uniform vec4 MyColor;
		void main()
		{
			gl_FragColor = texture2D(texSample[PSTexcoord.z], PSTexcoord.xy);
			gl_FragColor.xyz = gl_FragColor.xyz+(PSColor.xyz-vec3(0.5,0.5,0.5));
		}
	)";

	//https://blog.csdn.net/shixunzheng/article/details/74134355
	//fuck this nit working this eqquire create a 3dTexture
	//glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	// 
	// 
	//so use something like this   https://webglfundamentals.org/webgl/lessons/webgl-qna-how-to-bind-an-array-of-textures-to-a-webgl-shader-uniform-.html
	//#define numTextures 4
	//uniform sampler2D u_textures[numTextures];
	//var textureLoc = gl.getUniformLocation(program, "u_textures");
	//// Tell the shader to use texture units 0 to 3
	//gl.uniform1iv(textureLoc, [0, 1, 2, 3]);
	extern cNamedTypedObjectVector<cBaseShader>* g_pAll2DShaderList;

	cMultiTextureBaseShader* cMultiTextureBaseShader::CreateShader()
	{
		cMultiTextureBaseShader* l_pMultiTextureBaseShader = new cMultiTextureBaseShader(g_strCommonVSWithTextureArray, g_strColorFulFSWithTextureArray);
		g_pAll2DShaderList->AddObjectNeglectExist(l_pMultiTextureBaseShader);
		return l_pMultiTextureBaseShader;
	}
	cMultiTextureBaseShader::cMultiTextureBaseShader(const char* e_strVS, const char* e_strPS) :cBaseShader(e_strVS, e_strPS, cMultiTextureBaseShader::TypeID, false)
	{
		//var textureLoc = gl.getUniformLocation(program, "u_textures");
		//// Tell the shader to use texture units 0 to 3
		//gl.uniform1iv(textureLoc, [0, 1, 2, 3]);

		glUseProgram(m_uiProgram);
		m_uiTexLoacation = GetUniFormLocationByName("texSample");
		if (m_uiTexLoacation != (unsigned int)-1)
		{
			std::wstring l_str = this->GetName();;
			l_str += L" tex location:";
			l_str += ValueToStringW((uint64)m_uiTexLoacation);;
			FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
			const GLint l_Value[] = { 0,1,2,3 };
			glUniform1iv(m_uiTexLoacation, 4, l_Value);
			CHECK_GL_ERROR("cBaseShader::CreateProgram glUniform1i(m_uiTexLoacation)");
		}
		glUseProgram(0);
		//if (m_uiTexLoacation != (unsigned int)-1)
		//{
		//	//order is uniform,active,bind.
		//	glUniform4i(m_uiTexLoacation, 0, 1, 2, 3);
		//	//ensure call cTexture::ApplyImageWithActiveTextureID to setup glActiveTexture(GL_TEXTURE0 + N);
		//	CHECK_GL_ERROR("cMultiTextureBaseShader::Use glActiveTexture");
		//}
	}
	cMultiTextureBaseShader::~cMultiTextureBaseShader() {}

	void				cMultiTextureBaseShader::Use(bool e_bUseLastWVPMatrix)
	{
		//sBatchData
		auto l_m_uiTexLoacation = m_uiTexLoacation;
		m_uiTexLoacation = -1;
		cBaseShader::Use(e_bUseLastWVPMatrix);
		m_uiTexLoacation = l_m_uiTexLoacation;
		if (m_uiTexLoacation != (unsigned int)-1)
		{
			//order is uniform,active,bind.
			//this not working I have no idea,why?
			//glUniform4i(m_uiTexLoacation, 0, 1, 2, 3);
			const GLint l_Value[] = { 0,1,2,3 };
			glUniform1iv(m_uiTexLoacation, 4, l_Value);// 
			//ensure call cTexture::ApplyImageWithActiveTextureID to setup glActiveTexture(GL_TEXTURE0 + N);
			CHECK_GL_ERROR("cMultiTextureBaseShader::Use glActiveTexture");
		}
	}


	TYPDE_DEFINE_MARCO(cMultiTextureBaseShader);


	void	cBatchDataMultiTexture::s4TextureBatchData::Resize(size_t uiSize)
	{
		if (m_iNumVertex < uiSize)
		{
			m_vPosVector.resize(uiSize * 2);
			m_vUVVector.resize(uiSize * 2);
			m_vColorVector.resize(uiSize * 2);
		}
	}

	void cBatchDataMultiTexture::s4TextureBatchData::AddData(int e_iNumVertex, float* e_pvPos, float* e_pvUV, float* e_pColor, cTexture* e_pTexture, int e_iVUStride)
	{
		if (!e_pTexture)
		{
			FMLOG("cBatchDataMultiTexture::s4TextureBatchData::AddData,e_pTexcture is nullptr");
			return;
		}
		Resize(e_iNumVertex);
		memcpy(&m_vPosVector[m_iNumVertex], e_pvPos, sizeof(Vector3) * e_iNumVertex);
		if (e_iVUStride == 2)
		{
			auto l_IT = m_TextureAndIndexOfArrayMap.find(e_pTexture);
			if (l_IT != m_TextureAndIndexOfArrayMap.end())
			{
				int l_iIndex = l_IT->second;
				Vector2* l_vpUVData = (Vector2*)e_pvUV;
				for (int i = 0; i < e_iNumVertex; ++i)
				{
					m_vUVVector[m_iNumVertex + i].x = l_vpUVData[i].x;
					m_vUVVector[m_iNumVertex + i].y = l_vpUVData[i].y;
					m_vUVVector[m_iNumVertex + i].z = (float)l_iIndex;
				}
			}
			else
			{
				FMLOG("cBatchDataMultiTexture::s4TextureBatchData::AddData,cannt found e_pTexcture at TextureAndIndexOfArrayMap");
			}
		}
		else
		{
			assert(e_iVUStride == 3 && "cBatchDataMultiTexture::s4TextureBatchData::AddData only allow UV stride 3 or 2");
			memcpy(&m_vUVVector[m_iNumVertex], e_pvUV, sizeof(Vector3) * e_iNumVertex);
		}

		memcpy(&m_vColorVector[m_iNumVertex], e_pColor, sizeof(Vector4) * e_iNumVertex);
		m_iNumVertex += e_iNumVertex;
	}
	void	cBatchDataMultiTexture::s4TextureBatchData::Reset()
	{
		m_TextureAndIndexOfArrayMap.clear();
		m_iNumVertex = 0;
		for (int i = 0; i < MULTITEXTURE_RENDER_SHDER_COUNT; ++i)
		{
			m_pTextureArray[i] = nullptr;
		}

	}
	//return ok to use texture index of texture array,-1 for can't be used
	int		cBatchDataMultiTexture::s4TextureBatchData::GetTextureIndexFromCurrentData(cTexture* e_pTexture, bool e_AssignTexture, GLenum e_Src, GLenum e_Dest)
	{
		if (m_iNumVertex != 0)
		{
			if (e_Src != m_SrcBlendingMode || e_Dest != m_DestBlendingMode)
			{
				return -1;
			}
		}
		else
		{
			m_SrcBlendingMode = e_Src;
			m_DestBlendingMode = e_Dest;
			auto l_IT = m_TextureAndIndexOfArrayMap.find(e_pTexture);
			if (l_IT != m_TextureAndIndexOfArrayMap.end())
			{
				return l_IT->second;
			}
			//too many textures.
			if (m_TextureAndIndexOfArrayMap.size() >= MULTITEXTURE_RENDER_SHDER_COUNT)
			{
				return -1;
			}
		}
		int l_iIndexOfArrayIsEmpty = 0;
		for (int i = 0; i < MULTITEXTURE_RENDER_SHDER_COUNT; ++i)
		{
			if (!m_pTextureArray[i])
			{
				if (e_AssignTexture)
				{
					m_pTextureArray[i] = e_pTexture;
					m_TextureAndIndexOfArrayMap[e_pTexture] = l_iIndexOfArrayIsEmpty;
				}
				break;
			}
			if (e_pTexture == m_pTextureArray[i])
			{
				break;
			}
			++l_iIndexOfArrayIsEmpty;
		}
		if (l_iIndexOfArrayIsEmpty >= MULTITEXTURE_RENDER_SHDER_COUNT)
		{
			return -1;
		}
		return l_iIndexOfArrayIsEmpty;
	}
	void	cBatchDataMultiTexture::s4TextureBatchData::Render()
	{
		sBlendfunction l_BlendfunctionRestore(m_SrcBlendingMode, m_DestBlendingMode);
		l_BlendfunctionRestore.GetStatus();
		l_BlendfunctionRestore.Render();
		for (int i = 0; i < MULTITEXTURE_RENDER_SHDER_COUNT; ++i)
		{
			if (m_pTextureArray[i] != nullptr)
			{
				m_pTextureArray[i]->ApplyImageWithActiveTextureID(i);
			}
			else
			{
				break;
			}
		}
		FATMING_CORE::SetupShaderWorldMatrix(cMatrix44::Identity);
		myGlVertexPointer(3, &m_vPosVector[0]);
		myGlUVPointer(3, &m_vUVVector[0]);
		myGlColorPointer(4, &m_vColorVector[0]);
		MY_GLDRAW_ARRAYS(GL_TRIANGLES, 0, m_iNumVertex);
		l_BlendfunctionRestore.Restore();
	}

	cBatchDataMultiTexture::cBatchDataMultiTexture()
	{
		cMultiTextureBaseShader::CreateShader();
		m_bDoBatchRendering = false;
		m_iCurrentDataIndex = 0;
		for (int i = 0; i < 5; ++i)
		{
			AssignBatchData();
		}
		m_iCurrentDataIndex = 0;
		m_pCurrentData = m_BatchDataVector[m_iCurrentDataIndex];
	}
	cBatchDataMultiTexture::~cBatchDataMultiTexture()
	{
		DELETE_VECTOR(m_BatchDataVector);
	}

	cBatchDataMultiTexture::s4TextureBatchData* cBatchDataMultiTexture::AssignBatchData()
	{
		++m_iCurrentDataIndex;
		if (m_iCurrentDataIndex >= m_BatchDataVector.size())
		{
			s4TextureBatchData* l_pCurrentData = new s4TextureBatchData();
			m_pCurrentData = l_pCurrentData;
			m_BatchDataVector.push_back(l_pCurrentData);
		}
		else
		{
			m_pCurrentData = m_BatchDataVector[m_iCurrentDataIndex];
		}
		m_pCurrentData->Reset();
		return m_pCurrentData;
	}

	void	cBatchDataMultiTexture::AssignCurrentData(cTexture* e_pTexture, GLenum e_Src, GLenum e_Dest, bool e_AssignTexture)
	{
		if (m_pCurrentData)
		{
			if (m_pCurrentData->GetTextureIndexFromCurrentData(e_pTexture, e_AssignTexture, e_Src, e_Dest) == -1)
			{
				AssignBatchData();
			}
		}
		else
		{//should't happen.
			Start();
		}
	}

	void	cBatchDataMultiTexture::AddData(int e_iNumVertex, float* e_pvPos, float* e_pvUV, float* e_pColor, cTexture* e_pTexcture, GLenum e_Src, GLenum e_Dest, int e_iVUStride)
	{
		AssignCurrentData(e_pTexcture, e_Src, e_Dest);
		if (m_pCurrentData)
		{
			m_pCurrentData->AddData(e_iNumVertex, e_pvPos, e_pvUV, e_pColor, e_pTexcture, e_iVUStride);
		}
	}
	void	cBatchDataMultiTexture::Render()
	{
		UseShaderProgram(cMultiTextureBaseShader::TypeID);
		for (int i = 0; i < m_iCurrentDataIndex + 1; ++i)
		{
			m_BatchDataVector[i]->Render();
		}
	}
	void	cBatchDataMultiTexture::Start()
	{
		m_bDoBatchRendering = true;
		m_iCurrentDataIndex = -1;
		AssignBatchData();

	}
	void	cBatchDataMultiTexture::End()
	{
		Render();
		m_iCurrentDataIndex = 0;
		m_pCurrentData = m_BatchDataVector[m_iCurrentDataIndex];
		m_pCurrentData->Reset();
		m_bDoBatchRendering = false;
	}
//end namespace FATMING_CORE
}