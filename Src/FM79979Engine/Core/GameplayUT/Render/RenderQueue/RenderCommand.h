#pragma once
#include "../CommonRender/RenderObject.h"
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
	//cBaseImage
	//cUIIMage
	//cPuzzleImage
	//cMPDI
	//cSubMPDI
	//FrameBuffer
	//Viewport
	//Shader
	//Mesh
	//AnimationMesh
	//cNumeralImage
	//RenderLine
	//DrawPrimitive
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
		//return how many buffer require to cosumed.
		int							TrianglesAssignData(Vector3*e_pOutPos,Vector4*e_pOutColor,Vector2*e_pOutUV,
									unsigned int e_uiTextureID,Vector3* e_pInPos, Vector4* e_pInColor, Vector2* e_pInUV,unsigned int e_uiCount);
		int							TriangleStripAssignData(Vector3* e_pOutPos, Vector4* e_pOutColor, Vector2* e_pOutUV,
									unsigned int e_uiTextureID,Vector3* e_pInPos, Vector4* e_pInColor, Vector2* e_pInUV, unsigned int e_uiCount);
		int							ViewportAssignData(Vector3* e_pOutPos, Vector4* e_pOutColor, Vector2* e_pOutUV,Vector4 e_vViewPortData);
	};

	class cBatchRender
	{
		std::vector<Vector3>			m_PosVector;
		std::vector<Vector3>			m_CoorVector;
		std::vector<Vector3>			m_UVVector;
		std::vector<int>				m_MatrixIndexVector;
		//
		std::vector<cMatrix44>			m_MatVector;//num render batch(include viewport setting)
		//
		unsigned int					m_uiCurrentVertexCount;
		unsigned int					m_uiMaxVertexCount;

		size_t							m_uiRenderDataQueueCount;
		size_t							m_uiRenderDataQueueArraySize;
		std::unique_ptr<sRenderData[]>	m_RenderDataPtr;
		std::vector<sRenderData const*> m_SortedRenderData;
		// Dynamically expands the array used to store pending sprite information.
		void							GrowRenderData();
		void							GrowSortedSprites();
		//void SpriteBatch::Impl::GrowSpriteQueue()
		//{
		//	// Grow by a factor of 2.
		//	size_t newSize = std::max(InitialQueueSize, mSpriteQueueArraySize * 2);

		//	// Allocate the new array.
		//	auto newArray = std::make_unique<SpriteInfo[]>(newSize);

		//	// Copy over any existing sprites.
		//	for (size_t i = 0; i < mSpriteQueueCount; i++)
		//	{
		//		newArray[i] = mSpriteQueue[i];
		//	}

		//	// Replace the previous array with the new one.
		//	mSpriteQueue = std::move(newArray);
		//	mSpriteQueueArraySize = newSize;

		//	// Clear any dangling SpriteInfo pointers left over from previous rendering.
		//	mSortedSprites.clear();
		//}
		void		Sort();
		void		FlushBatch();
	public:
		cBatchRender();
		~cBatchRender();
		bool	Begin();
		bool	End();
		//return how many buffer require to cosumed.
		int							TrianglesAssignData(unsigned int e_uiTextureID,int e_iPosStride,float* e_pInPos,float* e_pInColor,float* e_pInUV, unsigned int e_uiCount, cMatrix44 e_Matrix);
		int							TriangleStripAssignData(unsigned int e_uiTextureID,Vector3* e_pInPos, Vector4* e_pInColor, Vector2* e_pInUV, unsigned int e_uiCount, cMatrix44 e_Matrix);
		int							ViewportAssignData(Vector4 e_vViewPortData);
	};
}