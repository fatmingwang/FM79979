#pragma once
#include "../CommonRender/BaseImage.h"
namespace FATMING_CORE
{
	class	cUVAnimationImage
	{
		//for render
		cBaseImage*		m_pRenderImage;
		//target
		float			m_fTargetUV[4];
		//for render
		//float			m_fImage1UV[4];
		//float			m_fImage2UV[4];
		Vector2			m_vSize;
		eMoveDirection	m_eOrientation;
		Vector2			m_vShowPos;
		//for render
		Vector3			m_vRenderVerticesPos[6];
		Vector2			m_vRenderVerticesUV[6];
		//current left up position render point relateted to Draw Image.
		GET_SET_DEC(Vector2,m_vCurrentPos,GetCurrentPos,SetCurrentPos);
		void			SetupRenderDataByCurrentPos();
	public:
		cUVAnimationImage(cBaseImage*e_pRenderImage,Vector2 e_vSize,eMoveDirection e_eOrientation,Vector2 e_vShowPos);
		virtual ~cUVAnimationImage();

		void	Update(float e_fElpaseTime,float e_fSpeed);
		void	Render();
		void	RenderByPos(Vector2 e_vPos);
		void	DebugRender();
	};
//end namespace FATMING_CORE
}