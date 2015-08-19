#include "../stdafx.h"
#include "ImageWithSlider.h"
#include "GameApp.h"
#include "../Image/MPDI.h"
namespace FATMING_CORE
{
	cImageSlider::cImageSlider(cPuzzleImageUnit*e_pIcon,cPuzzleImage*e_pPuzzleImage,Vector4 e_vTotalViewRect,Vector3 e_vIconPos,cImageButton*e_pCancelButton,bool e_bHorizontal)
	:cScroller(e_pPuzzleImage,e_vTotalViewRect,e_pCancelButton,e_pIcon,e_bHorizontal,&e_vIconPos)
	{
	}

	cImageSlider::~cImageSlider()
	{
	}


	void    cImageSlider::Render()
	{
		cScroller::Render();
		this->m_pIcon->Render();
		int	l_iCount = Count();
		cImageButton*l_pImage = 0;
		Vector3	l_vPos;
		if(this->m_eOrientation == eO_HORIZONTAL)
		{
			for( int i=0;i<l_iCount;++i )
			{
				l_pImage = (cImageButton*)this->m_ObjectList[i];
				l_vPos = l_pImage->GetPos();
				if(l_vPos.x+l_pImage->GetWidth()>=this->m_vCollisionRange.x)
				{
					if( l_vPos.x>this->m_vCollisionRange.z )
					{
						break;
					}
					cGameApp::m_spGlyphFontRender->RenderFont(l_vPos.x,l_vPos.y+l_pImage->GetHeight(),L"Cost:100");
				}
			}
		}
		else
		{
			for( int i=0;i<l_iCount;++i )
			{
				l_pImage = (cImageButton*)this->m_ObjectList[i];
				l_vPos = l_pImage->GetPos();
				if(l_vPos.y+l_pImage->GetHeight()>=this->m_vCollisionRange.y)
				{
					if( l_vPos.y>this->m_vCollisionRange.w )
					{
						break;
					}
					cGameApp::m_spGlyphFontRender->RenderFont(l_vPos.x,l_vPos.y+l_pImage->GetHeight(),L"Cost:100");
				}
			}
		}
		if(this->m_pCancelButton)
			this->m_pCancelButton->Render();
	}

//end namespace FATMING_CORE
}