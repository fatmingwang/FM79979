#ifndef XWBImageHeader_H
#define XWBImageHeader_H
namespace FATMING_CORE
{
	//
	inline	GLuint	CreateTextureFromBinary(int e_iChannel,void*e_pPixelsData,int e_iWidth,int e_iHeight,GLint e_iFormat)
	{
		GLuint	l_uiTexIndex = 0;
		glGenTextures(1, &l_uiTexIndex);
		assert(l_uiTexIndex&&"out of opengl generate texture failed");
		glBindTexture(GL_TEXTURE_2D, l_uiTexIndex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, e_iChannel, e_iWidth, e_iHeight, 0,
						e_iFormat, GL_UNSIGNED_BYTE, e_pPixelsData);
		return l_uiTexIndex;
	}

	//offset size to determinate pow of two texture.
	inline	GLuint	CreateTextureFromBinary(int e_iChannel,char*e_pPixelsData,int e_iWidth,int e_iHeight,GLint e_iDataFormat,bool e_bPowOfTwoTexture)
	{
		GLint l_iFormat = GL_RGB;
		char*	l_pNewPixelsData = 0;
		int	l_iNewWidth = e_iWidth;
		int	l_iNewHeight = e_iHeight;
		int	l_iOriginalTotalPixels = l_iNewWidth*l_iNewHeight;	//how many pixels we need to copy
		//is it need to create a pow of 2 texture
		if( e_bPowOfTwoTexture )
		{
			l_iNewWidth = power_of_two(l_iNewWidth);
			l_iNewHeight = power_of_two(l_iNewHeight);
			l_iFormat = GL_RGBA;
		}

		if( e_iDataFormat == GL_RGBA )
			l_iFormat = GL_RGBA;
		
		if( GL_RGB == l_iFormat )
		{
			l_pNewPixelsData = new char[l_iNewWidth*l_iNewHeight*3];
			//set all alpha to complete transparent
			memset(l_pNewPixelsData,255,sizeof(char)*l_iNewWidth*l_iNewHeight*3);
			for( int i=0;i<l_iOriginalTotalPixels;++i )
			{
				int	l_iIndex = i*3;
				char*	l_DestPointer = &l_pNewPixelsData[l_iIndex];
				memcpy(l_DestPointer,&e_pPixelsData[l_iIndex],sizeof(char)*3);
			}
		}
		else
		if( GL_RGBA == l_iFormat)
		{
			l_pNewPixelsData = new char[l_iNewWidth*l_iNewHeight*4];
			memset(l_pNewPixelsData,255,sizeof(char)*l_iNewWidth*l_iNewHeight*4);
			if( GL_RGB == e_iDataFormat )
			{
				//assign alpha to 0
				for( int i=0;i<l_iOriginalTotalPixels;++i )
				{
					int	l_iIndex = i*4;
					char*	l_DestPointer = &l_pNewPixelsData[l_iIndex];
					memcpy(l_DestPointer,&e_pPixelsData[i*3],sizeof(char)*3);
					l_DestPointer[3] = 0;
				}
			}
			else
			{
				for( int i=0;i<l_iOriginalTotalPixels;++i )
				{
					int	l_iIndex = i*4;
					char*	l_DestPointer = &l_pNewPixelsData[l_iIndex];
					memcpy(l_DestPointer,&e_pPixelsData[l_iIndex],sizeof(char)*4);
				}
			}
		}
		GLuint l_uiTexIndex = CreateTextureFromBinary(e_iChannel,l_pNewPixelsData,l_iNewWidth,l_iNewHeight,l_iFormat);
		delete l_pNewPixelsData;
		return l_uiTexIndex;
	}
	//=================
	//force to add alpha channel and become power of 2
	//ensure delete data after used
	//=================
	char*	TextureToPowOfTwo(char*e_pSrc,int e_iWidth,int e_iHeight,bool e_balpah);
}
#endif