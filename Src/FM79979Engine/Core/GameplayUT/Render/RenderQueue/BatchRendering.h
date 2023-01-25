#pragma once

#include <vector>
#include "../../../AllMathInclude.h"

namespace FATMING_CORE
{
	class cTexture;
	extern wchar_t* DEFAULT_SHADER;
	struct sBatchData
	{
		sBatchData()
		{
			m_iNumVertex = 0;
			Resize(100);
		}
		std::vector<Vector3>	m_vPosVector;
		std::vector<Vector2>	m_vUVVector;
		std::vector<Vector4>	m_vColorVector;
		std::vector<cTexture*>	m_TextureVector;
		std::vector<int>		m_ChangeTextureIndexVector;
		int		m_iNumVertex;
		void	Resize(size_t uiSize);
		void	AddData(int e_iNumVertex, float* e_pvPos, float* e_pvUV, float* e_pColor, cTexture* e_pTexcture);
		void	Render(cMatrix44 e_mat, const wchar_t* e_strShaderName = DEFAULT_SHADER);
		void	Start();
		void	End();
	};
}