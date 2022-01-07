#include "RenderCommand.h"
#include <iterator>
#include <algorithm>
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
		return 1;
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

	void cBatchRender::GrowVertexData()
	{
		const int l_ciMinCount = 1000;
		// Grow by a factor of 2.
		auto l_uiNewSize = max(l_ciMinCount, m_uiVertexArraySizeCount * 2);
		m_RenderVertex.Resize(l_uiNewSize);
		m_OrderedRenderVertex.Resize(l_uiNewSize);
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
		m_uiIndexBufferVector.resize(l_uiNewSize);
		auto l_uiPreviousIndex = m_uiIndexBufferVector.size();
		for (size_t i = l_uiPreviousIndex; i < l_uiNewSize; ++i)
		{
			size_t l_iIndex = i * 3;
			m_uiIndexBufferVector[l_iIndex] = (unsigned int)l_iIndex;
			m_uiIndexBufferVector[l_iIndex+1] = (unsigned int)l_iIndex+1;
			m_uiIndexBufferVector[l_iIndex+2] = (unsigned int)l_iIndex+2;
		}
	}

	void	cBatchRender::Sort()
	{
		if (m_uiCurrentRenderDataIndex > 0)
		{
			//size_t l_uiPreviousSize = m_SortedRenderData.size();
			//if (l_uiPreviousSize < m_uiCurrentRenderDataIndex)
			//{
			//	m_SortedRenderData.resize(m_uiCurrentRenderDataIndex);
			//}
			//https://stackoverflow.com/questions/23215102/how-can-i-insert-the-content-of-arrays-into-a-vector
			//m_SortedRenderData.insert(m_SortedRenderData.begin(), m_RenderDataPtr.get(), m_RenderDataPtr.get() + m_uiCurrentRenderDataIndex);
			//std::copy(m_RenderDataPtr.get(), m_RenderDataPtr.get() + m_uiCurrentRenderDataIndex, std::back_inserter(m_SortedRenderData));
			//for (size_t i = 0; i < m_uiCurrentRenderDataIndex; i++)
			//{
			//	m_SortedRenderData[i] = &m_RenderDataPtr[i];
			//}
		}
		if (m_eRenderSortMode == eRS_Z_DEPTH_COMPARE_BACK_TO_FRONT)
		{
			std::sort(m_MatAndIndexVector.begin(),
				m_MatAndIndexVector.begin() + static_cast<int>(m_uiCurrentRenderDataIndex),
				[](sIndexAndMatrix x, sIndexAndMatrix y) noexcept -> bool
				{
					return x.Mat.GetTranslation().z < y.Mat.GetTranslation().z;
				});
		}
		else
		if ( m_eRenderSortMode == eRS_Z_DEPTH_COMPARE_FRONT_TO_BACK )
		{
			std::sort(m_MatAndIndexVector.begin(),
				m_MatAndIndexVector.begin() + static_cast<int>(m_uiCurrentRenderDataIndex),
				[](sIndexAndMatrix x, sIndexAndMatrix y) noexcept -> bool
				{
					return x.Mat.GetTranslation().z > y.Mat.GetTranslation().z;
				});
		}
	}

	void	cBatchRender::FlushBatch()
	{
		size_t l_uiSize = m_MatAndIndexVector.size();
		unsigned int	l_uiNumVertex = 0;
		eRenderCommandType	l_PreviousRenderCommandType = eRenderCommandType::eRCT_MAX;
		std::wstring l_strPreviousShaderName;
		for (size_t i = 0; i < l_uiSize; ++i)
		{
			int l_iIndex = m_MatAndIndexVector[i].uiIndex;
			auto l_pRenderData = &m_RenderDataPtr[l_iIndex];
			auto l_CommandType = l_pRenderData->RenderCommandType;
			if (l_PreviousRenderCommandType != l_CommandType || l_pRenderData->strShaderName.compare(l_strPreviousShaderName))
			{
				ProcessRenderCommand(l_CommandType, l_pRenderData,&m_OrderedRenderVertex,0, l_uiNumVertex, l_strPreviousShaderName.c_str());
			}
			else
			{
				m_OrderedRenderVertex.Copy(l_pRenderData);
				l_uiNumVertex += l_pRenderData->uiCount;
			}
			l_strPreviousShaderName = l_pRenderData->strShaderName;
		}
	}

	void cBatchRender::ComputeVertexMatrix()
	{

	}

	void cBatchRender::RenderTriangles(sRenderVertex* e_pRenderVertex, int e_iStartIndex, int e_iCount, const wchar_t* e_strShaderName)
	{
		RenderVertexByIndexBuffer(cMatrix44::Identity,3, e_pRenderVertex->ColorVector[e_iStartIndex],e_pRenderVertex->UVVector[e_iStartIndex], e_pRenderVertex->PosVector[e_iStartIndex],
			&m_uiIndexBufferVector[0], e_iCount, e_strShaderName);
	}

	void cBatchRender::RenderViewport(sRenderData* e_pRenderData)
	{
		glViewport((GLint)e_pRenderData->pvViewPort->x,(GLint)e_pRenderData->pvViewPort->y,
			(GLsizei)e_pRenderData->pvViewPort->z, (GLsizei)e_pRenderData->pvViewPort->w);
	}

	void cBatchRender::ProcessRenderCommand(eRenderCommandType e_eRenderCommandType,sRenderData*e_pRenderData,sRenderVertex* e_pRenderVertex, int e_iStartIndex, int e_iCount, const wchar_t* e_strShaderName)
	{
		switch(e_eRenderCommandType)
		{
			case eRCT_VIEWPORT_SETTING:
				RenderViewport(e_pRenderData);
				break;
			case eRCT_RENDER_TRIANGLES:
				RenderTriangles(e_pRenderVertex, e_iStartIndex, e_iCount, e_strShaderName);
				break;
			default:
				break;
		}
	}

	bool cBatchRender::Begin(eRenderSortMode e_eRenderSortMode)
	{
		m_eRenderSortMode = e_eRenderSortMode;
		m_uiCurrentRenderDataIndex = 0;
		m_uiCurrentVertexIndex = 0;
		return true;
	}

	bool cBatchRender::End()
	{
		ComputeVertexMatrix();
		Sort();
		FlushBatch();
		return false;
	}

	void	cBatchRender::IncreaseData(int e_iNumTriangles)
	{
		if (m_uiCurrentRenderDataIndex + 1 >= m_uiRenderDataQueueArraySize)
		{
			this->GrowRenderData();
		}
		if (m_uiCurrentVertexIndex + e_iNumTriangles >= m_uiVertexArraySizeCount)
		{
			GrowVertexData();
		}
	}

	bool	cBatchRender::Draw_TrianglesAssignData(unsigned int e_uiTextureID, int e_iPosStride, float* e_pInPos, float* e_pInColor, float* e_pInUV,
		unsigned int e_uiCount, cMatrix44 e_Matrix, const wchar_t* strShaderName)
	{
		IncreaseData(e_uiCount);
		auto l_pRenderData =	&m_RenderDataPtr[m_uiCurrentVertexIndex];
		auto l_pvPos =			&m_RenderVertex.PosVector[m_uiCurrentVertexIndex];
		auto l_pvColor =		&m_RenderVertex.ColorVector[m_uiCurrentVertexIndex];
		auto l_pvUV =			&m_RenderVertex.UVVector[m_uiCurrentVertexIndex];
		auto l_iNumAdded = l_pRenderData->TrianglesAssignData(l_pvPos, l_pvColor, l_pvUV, e_uiTextureID,
			(Vector3*)e_pInPos, (Vector4*)e_pInColor, (Vector2*)e_pInUV,e_uiCount);
		l_pRenderData->strShaderName = strShaderName;
		m_uiCurrentVertexIndex += l_iNumAdded;
		m_MatAndIndexVector.push_back({ m_uiCurrentVertexIndex,e_Matrix });
		++m_uiCurrentRenderDataIndex;
		return true;
	}
	bool	cBatchRender::Draw_TriangleStripAssignData(unsigned int e_uiTextureID, Vector3* e_pInPos, Vector4* e_pInColor, Vector2* e_pInUV,
		unsigned int e_uiCount, cMatrix44 e_Matrix, const wchar_t* strShaderName)
	{
		assert(e_uiCount % 4 == 0 &&"triangles must bet a quad,because I am lazy");
		IncreaseData(e_uiCount / 4 * 6);
		auto l_pRenderData = &m_RenderDataPtr[m_uiCurrentVertexIndex];
		auto l_pvPos = &m_RenderVertex.PosVector[m_uiCurrentVertexIndex];
		auto l_pvColor = &m_RenderVertex.ColorVector[m_uiCurrentVertexIndex];
		auto l_pvUV = &m_RenderVertex.UVVector[m_uiCurrentVertexIndex];
		auto l_iNumAdded = l_pRenderData->TriangleStripAssignData(l_pvPos, l_pvColor, l_pvUV, e_uiTextureID,
			(Vector3*)e_pInPos, (Vector4*)e_pInColor, (Vector2*)e_pInUV, e_uiCount);
		l_pRenderData->strShaderName = strShaderName;
		m_uiCurrentVertexIndex += l_iNumAdded;
		m_MatAndIndexVector.push_back({ m_uiCurrentVertexIndex,e_Matrix });
		++m_uiCurrentRenderDataIndex;
		return true;
	}

	bool	cBatchRender::Draw_ViewportAssignData(Vector4 e_vViewPortData)
	{
		IncreaseData(1);
		auto l_pRenderData = &m_RenderDataPtr[m_uiCurrentVertexIndex];
		auto l_pvPos = &m_RenderVertex.PosVector[m_uiCurrentVertexIndex];
		auto l_pvColor = &m_RenderVertex.ColorVector[m_uiCurrentVertexIndex];
		auto l_pvUV = &m_RenderVertex.UVVector[m_uiCurrentVertexIndex];
		auto l_iNumAdded = l_pRenderData->ViewportAssignData(l_pvPos, l_pvColor, l_pvUV, e_vViewPortData);
		m_uiCurrentVertexIndex += l_iNumAdded;
		++m_uiCurrentRenderDataIndex;
		m_MatAndIndexVector.push_back({ m_uiCurrentVertexIndex,cMatrix44::Identity });
		return true;
	}

	void cBatchRender::sRenderVertex::Resize(unsigned int e_uiSize)
	{
		PosVector.resize(e_uiSize);
		ColorVector.resize(e_uiSize);
		UVVector.resize(e_uiSize);
	}
	void cBatchRender::sRenderVertex::Copy(sRenderData* e_pRenderData)
	{
		PosVector.insert(PosVector.end(), e_pRenderData->pvPos, e_pRenderData->pvPos + e_pRenderData->uiCount);
		ColorVector.insert(ColorVector.end(), e_pRenderData->pvColor, e_pRenderData->pvColor + e_pRenderData->uiCount);
		UVVector.insert(UVVector.end(), e_pRenderData->pvUV, e_pRenderData->pvUV + e_pRenderData->uiCount);
	}
}