#pragma once
#include "../CommonRender/RenderObject.h"
#include "../../OpenGL/GLSL/ShaderStorageBuffer.h"
#include <functional>
#include <memory>
//github Xbox-GDK-Samples-main
//Xbox-GDK-Samples-main\Kits\DirectXTK12\Src\SpriteBatch.cpp

namespace FATMING_CORE
{
	//there is a batch render manager,
	//it contaion Pos and UV and Color and Matrix vector,these vector will auto increase if size is not enough.
	//every Render object assign its triangles to sRenderData(srouce array from vectors)
	//
	//
	enum eRenderCommandType
	{
		eRCT_RENDER_TRIANGLE_STRIP = 0,//QUAD
		eRCT_RENDER_TRIANGLES,
		eRCT_RENDER_PRIMITIVE,//line,points,sphere,panel,cube....
		eRCT_VIEWPORT_SETTING,//also scissor
		//eRCT_VIEW_PROJECTION_MATRIX,//don't need this?.
		eRCT_FBO_OBJECT,
		eRCT_VBO_OBJECT,
		eRCT_VIEW_PROJECTION_SETTING,//camea and shader
		eRCT_CUSTOM_RENDERING,//use callback function(f_CustomRenderfunction)
		eRCT_MAX
	};
	typedef std::function<void(void*)>	f_CustomRenderfunction;
	//
	//data from batch render manager
	//
	struct sRenderData final
	{
		unsigned int				uiTextureID;
		Vector3*					pvPos;
		union
		{
			Vector4* pvColor;
			Vector4* pvViewPort;
		};
		Vector2*					pvUV;
		unsigned int				uiCount;
		//
		eRenderCommandType			RenderCommandType;
		std::wstring				strShaderName;
		//return how many buffer require to cosumed.
		int							TrianglesAssignData(Vector3*e_pOutPos,Vector4*e_pOutColor,Vector2*e_pOutUV,
									unsigned int e_uiTextureID,Vector3* e_pInPos, Vector4* e_pInColor, Vector2* e_pInUV,unsigned int e_uiCount);
		int							TriangleStripAssignData(Vector3* e_pOutPos, Vector4* e_pOutColor, Vector2* e_pOutUV,
									unsigned int e_uiTextureID,Vector3* e_pInPos, Vector4* e_pInColor, Vector2* e_pInUV, unsigned int e_uiCount);
		int							ViewportAssignData(Vector3* e_pOutPos, Vector4* e_pOutColor, Vector2* e_pOutUV,Vector4 e_vViewPortData);
	};

	class cBatchRender:public NamedTypedObject
	{
	protected:
		cShaderStorageBuffer<char>* m_pVerticesIn;
		cShaderStorageBuffer<char>* m_pMatricesIndicesIn;
		cShaderStorageBuffer<char>* m_pMatricesIn;
		cShaderStorageBuffer<char>* m_pVertexOut;
		//
		std::map<NamedTypedObject*, unsigned int>	m_ShaderStorageBufferAndResourceIDMap;
		class cSimpleComputeShader* m_pSimpleComputeShader;
		//
		struct sRenderVertex
		{
			std::vector<Vector3>	PosVector;
			std::vector<Vector4>	ColorVector;
			std::vector<Vector2>	UVVector;
			std::vector<int>		iTargetMatrixIndexVector;
			void					Resize(unsigned int  e_uiSize);
			void					Copy(sRenderData*e_pRenderData,int e_iTargetMatrixIndex);
			void					Clear();
		};
		//from input
		sRenderVertex					m_RenderVertex;
		//for temp sotre
		sRenderVertex					m_OrderedRenderVertex;
		//
		//for draw triangles index buffer
		std::vector<unsigned int>		m_uiIndexBufferVector;
		//
		struct sIndexAndMatrix
		{
			unsigned int	uiIndex;
			cMatrix44		Mat;
		};

		std::vector<sIndexAndMatrix>	m_MatAndIndexVector;//num render batch(include viewport setting)
		//vertex
		unsigned int					m_uiCurrentVertexIndex;
		unsigned int					m_uiVertexArraySizeCount;
		//sRenderData
		size_t							m_uiCurrentRenderDataIndex;
		size_t							m_uiRenderDataQueueArraySize;
		std::unique_ptr<sRenderData[]>	m_RenderDataPtr;
		// Dynamically expands the array used to store pending sprite information.
		void							IncreaseData(int e_iNumTriangles);
		virtual void					GrowRenderData();
		virtual void					GrowVertexData();
		void							SortByMatrix();
		void							FlushBatch();
		//do compute shader
		void							ComputeVertexMatrix();
		void							RenderTriangles(sRenderVertex* e_pRenderVertex, int e_iStartIndex, int e_iCount, const wchar_t* e_strShaderName);
		void							RenderViewport(sRenderData*e_pRenderData);
		void							ProcessRenderCommand(eRenderCommandType e_eRenderCommandType,sRenderData* e_pRenderData,sRenderVertex*e_pRenderVertex,int e_iStartIndex,int e_iCount, const wchar_t* e_strShaderName);
	public:
		enum eRenderSortMode
		{
			eRS_ADD_ORDER,//
			eRS_Z_DEPTH_COMPARE_BACK_TO_FRONT,//by matrix z.
			eRS_Z_DEPTH_COMPARE_FRONT_TO_BACK,
			eRS_IMMEDIATELY_RENDER,
			eRS_MAX
		};
	protected:
		eRenderSortMode					m_eRenderSortMode;
	public:
		DEFINE_TYPE_INFO();
		cBatchRender();
		cBatchRender(const char*e_strShader, std::vector<const char*>&e_strVector);
		~cBatchRender();
		bool	Begin(eRenderSortMode e_eRenderSortMode);
		virtual bool	End();
		//return how many buffer require to cosumed.
		bool							Draw_TrianglesAssignData(unsigned int e_uiTextureID,int e_iPosStride,float* e_pInPos,float* e_pInColor,float* e_pInUV, unsigned int e_uiCount, cMatrix44 e_Matrix,const wchar_t*strShaderName);
		bool							Draw_TriangleStripAssignData(unsigned int e_uiTextureID,Vector3* e_pInPos, Vector4* e_pInColor, Vector2* e_pInUV, unsigned int e_uiCount, cMatrix44 e_Matrix, const wchar_t* strShaderName);
		bool							Draw_ViewportAssignData(Vector4 e_vViewPortData);
		void							InsertSSBWithResourceID(NamedTypedObject*e_pSSB, unsigned int e_uiResourceID);
	};
//end namespace FATMING_CORE
}