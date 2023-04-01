#pragma once

#include "../../OpenGL/GLSL/Shader.h"

#define	MULTITEXTURE_RENDER_SHDER_COUNT	4
namespace FATMING_CORE
{
	class	cMultiTextureBaseShader :public cBaseShader
	{
	public:

		static cMultiTextureBaseShader* CreateShader();
		cMultiTextureBaseShader(const char* e_strVS, const char* e_strPS);
		virtual ~cMultiTextureBaseShader();
		DEFINE_TYPE_INFO();
		//https://stackoverflow.com/questions/25252512/how-can-i-pass-multiple-textures-to-a-single-shader
		virtual	void				Use(bool e_bUseLastWVPMatrix = true)override;
	};

	//cBatchDataMultiTexture::Start
	//		N object do this....
	//		cBatchDataMultiTexture::AddData
	//cBatchDataMultiTexture::End
	class cBatchDataMultiTexture :public NamedTypedObject
	{
		bool	m_bDoBatchRendering = false;
	public:
		struct s4TextureBatchData
		{
			//it should check blending status
			//if blending is not same it should be rendered.
			class cTexture* m_pTextureArray[MULTITEXTURE_RENDER_SHDER_COUNT];
			std::vector<Vector3>		m_vPosVector;
			std::vector<Vector3>		m_vUVVector;
			std::vector<Vector4>		m_vColorVector;
			int							m_iNumVertex = 0;
			GLenum						m_SrcBlendingMode = GL_SRC_ALPHA;
			GLenum						m_DestBlendingMode = GL_ONE_MINUS_SRC_ALPHA;
			std::map<cTexture*, int>	m_TextureAndIndexOfArrayMap;
			void						Resize(size_t uiSize);
			//float must be 3?
			void						AddData(int e_iNumVertex, float* e_pvPos, float* e_pvUV, float* e_pColor, cTexture* e_pTexcture, int e_iVUStride = 2);
			void						Reset();
			//return ok to use texture index of texture array,-1 for can't be used
			int							GetTextureIndexFromCurrentData(cTexture* e_pTexture, bool e_AssignTexture = true, GLenum e_Src = GL_SRC_ALPHA, GLenum e_Dest = GL_ONE_MINUS_SRC_ALPHA);
			void						Render();
		};
	private:
		std::vector<s4TextureBatchData*>	m_BatchDataVector;
		s4TextureBatchData* m_pCurrentData;
		int									m_iCurrentDataIndex = -1;
		void								AssignCurrentData(cTexture* e_pTexture, GLenum e_Src = GL_SRC_ALPHA, GLenum e_Dest = GL_ONE_MINUS_SRC_ALPHA, bool e_AssignTexture = true);
		s4TextureBatchData* AssignBatchData();
		void								Render();
	public:
		cBatchDataMultiTexture();
		virtual ~cBatchDataMultiTexture();
		//UV must be float3
		void								AddData(int e_iNumVertex, float* e_pvPos, float* e_pvUV, float* e_pColor, cTexture* e_pTexcture, GLenum e_Src = GL_SRC_ALPHA, GLenum e_Dest = GL_ONE_MINUS_SRC_ALPHA, int e_iVUStride = 2);
		void								Start();
		void								End();
		bool								IsEnable() { return m_bDoBatchRendering; }
	};
//end namespace FATMING_CORE
}