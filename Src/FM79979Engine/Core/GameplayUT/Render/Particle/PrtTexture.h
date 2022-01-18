#pragma once
#include "../PuzzleImage/PuzzleImage.h"
#include "../PuzzleImage/BaseUVImageData.h"
#include "ParticleEmitter.h"
namespace FATMING_CORE
{
	class	cPrtEmitter;
	class cPrtTextureActDynamicTexture:public cParticleBase,public cMulti_PI_Image
	{
		//get from pi's data
		float*m_pfAllUVToTwoTriangleData;
	public:
		cImageIndexOfAnimation*m_pImageIndexOfAnimation;
		DEFINE_TYPE_INFO()
		explicit cPrtTextureActDynamicTexture();
		virtual ~cPrtTextureActDynamicTexture();
		virtual inline void	Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData)override
		{
			if( m_pImageIndexOfAnimation )
			{
				float	l_fTimeGap = e_pParticleData->fOriginaLifeSpan-e_pParticleData->fLifespan;
				e_pParticleData->iCurrentAnimationTextureIndex = m_pImageIndexOfAnimation->GetImageIndexByCurrentTime(l_fTimeGap);
			}
			else
			{
				if( e_pParticleData->iCurrentAnimationTextureIndex++ >= this->m_pPuzzleImage->GetNumImage() )
					e_pParticleData->iCurrentAnimationTextureIndex = 0;
			}
			memcpy(&m_pEmitterOwner->m_pvAllTexCoordinatePointer[e_iIndex*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT],&m_pfAllUVToTwoTriangleData[e_pParticleData->iCurrentAnimationTextureIndex*TWO_TRIANGLE_VERTICES_UV_TO_QUAD_UV_COUNT],sizeof(float)*TWO_TRIANGLE_VERTICES_UV_TO_QUAD_UV_COUNT);
		}
		//usage explanation
		static char*	UsageExplanation;
		//
		virtual NamedTypedObject*	Clone()override;
		virtual	void				SetPuzzleImage(cPuzzleImage*e_pPuzzleImage)override;
		//
		virtual std::string			GetDataInfo()override;
		//this one for output data value by "," to separate value
		virtual std::string			GetOutputDataString()override;
		//input the output data string,and analyze it
		virtual bool				SetDataByDataString(const char*e_pString)override;
		//owner is important so this one can't be cloned!.
		virtual	bool				IsOwnerIsImportant()override { return true; }
	};
//end namespace FATMING_CORE
}