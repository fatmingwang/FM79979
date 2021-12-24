#include "RenderCommand.h"
namespace FATMING_CORE
{
	int sRenderData::TrianglesAssignData(Vector3* e_pOutPos, Vector4* e_pOutColor, Vector2* e_pOutUV,
		unsigned int e_uiTextureID, Vector3* e_pInPos, Vector4* e_pInColor, Vector2* e_pInUV, unsigned int e_uiCount)
	{
		RenderCommandType = eRCT_RENDER_TRIANGLES;
		pvPos = e_pOutPos;
		pvUV = e_pOutUV;
		pvColor = e_pOutColor;
		memcpy(e_pOutPos, e_pInPos, sizeof(Vector3)* e_uiCount);
		memcpy(e_pOutColor, e_pInPos, sizeof(Vector4) * e_uiCount);
		memcpy(e_pOutUV, e_pInPos, sizeof(Vector2) * e_uiCount);
		uiTextureID = e_uiTextureID;
		uiCount = e_uiCount;
		return e_uiCount;
	}

	int sRenderData::TriangleStripAssignData(Vector3* e_pOutPos, Vector4* e_pOutColor, Vector2* e_pOutUV,
		unsigned int e_uiTextureID, Vector3* e_pInPos, Vector4* e_pInColor, Vector2* e_pInUV, unsigned int e_uiCount)
	{
		RenderCommandType = eRCT_RENDER_TRIANGLES,
		assert(e_uiCount == 4 && "I am lazy to do this,now triangle strip only support 4points");
		uiCount = e_uiCount+2;
		pvPos = e_pOutPos;
		pvUV = e_pOutUV;
		pvColor = e_pOutColor;
		//230,031
		int l_iIndexArray[TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT] = {2,3,0,0,3,1};
		for (int i = 0; i < TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT; ++i)
		{
			e_pOutPos[i] = e_pInPos[l_iIndexArray[i]];
			e_pOutColor[i] = e_pInColor[l_iIndexArray[i]];
			e_pOutUV[i] = e_pInUV[l_iIndexArray[i]];
		}
		uiTextureID = e_uiTextureID;
		return uiCount;
	}

	int sRenderData::ViewportAssignData(Vector3* e_pOutPos, Vector4* e_pOutColor, Vector2* e_pOutUV, Vector4 e_vViewPortData)
	{
		pvPos = e_pOutPos;
		pvUV = e_pOutUV;
		pvViewPort = e_pOutColor;
		RenderCommandType = eRCT_VIEWPORT_SETTING;
		*pvViewPort = e_vViewPortData;
		return 0;
	}

	cBatchRender::cBatchRender()
	{
		m_uiCurrentVertexCount = 0;
		m_uiMaxVertexCount = 0;
	}
	cBatchRender::~cBatchRender()
	{
	}

	void cBatchRender::GrowSortedSprites()
	{
		size_t previousSize = m_SortedRenderData.size();

		m_SortedRenderData.resize(m_uiRenderDataQueueCount);

		for (size_t i = previousSize; i < m_uiRenderDataQueueCount; i++)
		{
			m_SortedRenderData[i] = &m_RenderDataPtr[i];
		}
	}

	void cBatchRender::GrowRenderData()
	{
		const int l_ciMinCount = 1000;
		// Grow by a factor of 2.
		auto l_uiNewSize = max(l_ciMinCount, m_uiMaxVertexCount * 2);

		m_PosVector.resize(l_uiNewSize);
		m_CoorVector.resize(l_uiNewSize);
		m_UVVector.resize(l_uiNewSize);
		m_MatrixIndexVector.resize(l_uiNewSize);

		// Allocate the new array.
		auto l_NewArray = std::make_unique<sRenderData[]>(l_uiNewSize);

		// Copy over any existing sprites.
		for (size_t i = 0; i < m_uiCurrentVertexCount; i++)
		{
			l_NewArray[i] = m_RenderDataPtr[i];
		}
		// Replace the previous array with the new one.
		m_RenderDataPtr = std::move(l_NewArray);
		m_uiMaxVertexCount = l_uiNewSize;

		m_SortedRenderData.clear();
	}

	bool cBatchRender::Begin()
	{
		return false;
	}

	bool cBatchRender::End()
	{
		return false;
	}
	//enum eRenderCommandType
	//{
	//	eRCT_RENDER_QUAD = 0,
	//	eRCT_RENDER_TRIANGLE,
	//	eRCT_RENDER_PRIMITIVE,//line,points,sphere,panel,cube....
	//	eRCT_VIEWPORT_SETTING,
	//	eRCT_SCISSOR_SETTING,
	//	eRCT_FBO_OBJECT,
	//	eRCT_VBO_OBJECT,
	//	eRCT_MAX
	//};
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
}