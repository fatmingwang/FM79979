#include "../stdafx.h"
#include "BinaryToTexture.h"
namespace FATMING_CORE
{
	char*	TextureToPowOfTwo(char*e_pSrc,int e_iWidth,int e_iHeight,bool e_balpah)
	{
		int	l_iStride = e_balpah?4:3;
		int	l_iPowOf2Width  = UT::power_of_two(e_iWidth);
		int	l_iPowOf2Height  = UT::power_of_two(e_iHeight);
		int	l_iTotalDataSize = l_iStride*l_iPowOf2Width*l_iPowOf2Height;
		char*	l_pPixelData = new char[l_iTotalDataSize];
		//if( e_balpah )
			memset(l_pPixelData,0,l_iTotalDataSize*sizeof(char));
		for(int j=0;j<e_iHeight;++j)
		{
			char*	l_srcData = &e_pSrc[e_iWidth*j*l_iStride];
			char*	l_destData = &l_pPixelData[l_iPowOf2Width*j*l_iStride];
			memcpy(l_destData,l_srcData,sizeof(char)*l_iStride*e_iWidth);
		}
		return l_pPixelData;
	}
}