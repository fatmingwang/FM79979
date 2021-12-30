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
		m_uiCurrentRenderDataIndex = 0;
		m_uiCurrentVertexIndex = 0;
		m_uiVertexArraySizeCount = 0;
		GrowRenderData();
		GrowVertexData();
	}
	cBatchRender::~cBatchRender()
	{
	}

	void cBatchRender::GrowSortedSprites()
	{
		size_t previousSize = m_SortedRenderData.size();

		m_SortedRenderData.resize(m_uiCurrentRenderDataIndex);

		for (size_t i = previousSize; i < m_uiCurrentRenderDataIndex; i++)
		{
			m_SortedRenderData[i] = &m_RenderDataPtr[i];
		}
	}
	void cBatchRender::GrowVertexData()
	{
		const int l_ciMinCount = 1000;
		// Grow by a factor of 2.
		auto l_uiNewSize = max(l_ciMinCount, m_uiVertexArraySizeCount * 2);
		m_PosVector.resize(l_uiNewSize);
		m_ColorVector.resize(l_uiNewSize);
		m_UVVector.resize(l_uiNewSize);
		m_MatrixIndexVector.resize(l_uiNewSize);
		m_uiVertexArraySizeCount = l_uiNewSize;
	}
	void cBatchRender::GrowRenderData()
	{
		const int l_ciMinCount = 1000;
		// Grow by a factor of 2.
		auto l_uiNewSize = max(l_ciMinCount, m_uiRenderDataQueueArraySize * 2);
		// Allocate the new array.
		auto l_NewArray = std::make_unique<sRenderData[]>(l_uiNewSize);
		// Copy over any existing sprites.
		for (size_t i = 0; i < m_uiCurrentRenderDataIndex; i++)
		{
			l_NewArray[i] = m_RenderDataPtr[i];
		}
		// Replace the previous array with the new one.
		m_RenderDataPtr = std::move(l_NewArray);
		m_uiRenderDataQueueArraySize = l_uiNewSize;
		m_SortedRenderData.clear();
	}

	void	cBatchRender::Sort()
	{

	}

	void	cBatchRender::FlushBatch()
	{

	}

	bool cBatchRender::Begin()
	{
		m_uiCurrentRenderDataIndex = 0;
		m_uiCurrentVertexIndex = 0;
		return false;
	}

	bool cBatchRender::End()
	{
		Sort();
		FlushBatch();
		return false;
	}
	bool	cBatchRender::Draw_TrianglesAssignData(unsigned int e_uiTextureID, int e_iPosStride, float* e_pInPos, float* e_pInColor, float* e_pInUV, unsigned int e_uiCount, cMatrix44 e_Matrix)
	{
		if (m_uiCurrentRenderDataIndex +1 >= m_uiRenderDataQueueArraySize)
		{
			this->GrowRenderData();
		}
		if (m_uiCurrentVertexIndex + e_uiCount >= m_uiVertexArraySizeCount)
		{
			GrowVertexData();
		}
		auto l_pRenderData =	&m_RenderDataPtr[m_uiCurrentVertexIndex];
		auto l_pvPos =			&m_PosVector[m_uiCurrentVertexIndex];
		auto l_pvColor =		&m_ColorVector[m_uiCurrentVertexIndex];
		auto l_pvUV =			&m_UVVector[m_uiCurrentVertexIndex];
		auto l_iNumAdded = l_pRenderData->TrianglesAssignData(l_pvPos, l_pvColor, l_pvUV, e_uiTextureID,
			(Vector3*)e_pInPos, (Vector4*)e_pInColor, (Vector2*)e_pInUV,e_uiCount);
		m_uiCurrentVertexIndex += l_iNumAdded;
		++m_uiCurrentRenderDataIndex;
		m_MatVector.push_back(e_Matrix);
		return true;
	}
	bool	cBatchRender::Draw_TriangleStripAssignData(unsigned int e_uiTextureID, Vector3* e_pInPos, Vector4* e_pInColor, Vector2* e_pInUV, unsigned int e_uiCount, cMatrix44 e_Matrix)
	{
		return true;
	}
	bool	cBatchRender::Draw_ViewportAssignData(Vector4 e_vViewPortData)
	{
		return true;
	}
}