#include "BatchRendering.h"
#include "../Texture/Texture.h"
#include "../CommonRender/SimplePrimitive.h"

namespace FATMING_CORE
{

	void sBatchData::Resize(size_t uiSize)
	{
		m_vPosVector.resize(uiSize);
		m_vUVVector.resize(uiSize);
		m_vColorVector.resize(uiSize);
	}

	void sBatchData::AddData(int e_iNumVertex, float* e_pvPos, float* e_pvUV, float* e_pColor, cTexture* e_pTexcture)
	{
		if (m_TextureVector.size())
		{
			if (m_TextureVector[m_TextureVector.size() - 1] != e_pTexcture)
			{
				m_TextureVector.push_back(e_pTexcture);
				m_ChangeTextureIndexVector.push_back(m_iNumVertex);
			}
		}
		else
		{
			m_TextureVector.push_back(e_pTexcture);
		}
		if (m_iNumVertex + e_iNumVertex >= m_vPosVector.size())
		{
			Resize((m_iNumVertex + e_iNumVertex) * 2);
		}
		memcpy(&m_vPosVector[m_iNumVertex], e_pvPos, sizeof(Vector3) * e_iNumVertex);
		memcpy(&m_vUVVector[m_iNumVertex], e_pvUV, sizeof(Vector2) * e_iNumVertex);
		memcpy(&m_vColorVector[m_iNumVertex], e_pColor, sizeof(Vector4) * e_iNumVertex);
		m_iNumVertex += e_iNumVertex;
	}

	void sBatchData::Render(cMatrix44 e_mat, const wchar_t* e_strShaderName)
	{
		int l_iCurrentIndex = 0;
		auto l_uiSize = m_ChangeTextureIndexVector.size();
		for (int i = 0; i < l_uiSize; ++i)
		{
			auto l_iVertices = m_ChangeTextureIndexVector[i];
			if (l_iVertices > 0)
			{
				int l_iNumTriangles = l_iVertices / 3;
				GLRender::RenderTrianglesWithTexture((float*)&m_vPosVector[l_iCurrentIndex], (float*)&m_vUVVector[l_iCurrentIndex], (float*)&m_vColorVector[l_iCurrentIndex], e_mat, 3, l_iNumTriangles, m_TextureVector[i]);
			}
			l_iCurrentIndex = l_iVertices;
		}
	}

	void sBatchData::Start()
	{
		m_iNumVertex = 0; m_TextureVector.clear(); m_ChangeTextureIndexVector.clear();
	}

	void sBatchData::End()
	{
		m_ChangeTextureIndexVector.push_back(m_iNumVertex);
	}
}