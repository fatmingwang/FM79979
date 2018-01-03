#include "../stdafx.h"
#include "Texture.h"
#include "TextureManager.h"
#include "SimplePrimitive.h"
#include "../Utility.h"
#include "BinaryToTexture.h"
#include "../GameplayUT/GameApp.h"
#include "../GameplayUT/BinaryFile.h"
#include "png/pngLoader.h"
#include "dds/nv_images.h"
#include "jpeg/jpgd.h"
#include "jpeg/jpge.h"

//nv_images.cpp
extern int LoadTextureFromDDSData(GLenum target, NvS32 startLevel, const NVHHDDSImage *pImage, NvBool useMipmaps);
#ifdef IOS
//saving an image 
extern UIImage*	glToUIImage();
extern void		captureToPhotoAlbum();
#endif 
namespace FATMING_CORE
{
	//if graphic card does not support non power of two,set as false
	//bool	g_bSupportNonPowerOfTwoTexture = true;
	bool	g_bSupportNonPowerOfTwoTexture = false;
	//https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/discard.php
	//http://jerome.jouvie.free.fr/opengl-tutorials/Tutorial6.php
	//GLfloat	g_fMAG_FILTERValue = GL_NEAREST;
	//GLfloat	g_fMIN_FILTERValue = GL_NEAREST;
	GLfloat	g_fMAG_FILTERValue = GL_LINEAR;
	GLfloat	g_fMIN_FILTERValue = GL_LINEAR;
    GLuint  cTexture::m_suiLastUsingImageIndex = -1;
	TYPDE_DEFINE_MARCO(cTexture);
	//===============
	//
	//===============
	cTexture::cTexture(NamedTypedObject*e_pOwner,char*e_pPixels,int e_iWidth,int e_iHeight,const wchar_t*e_strName,bool e_bCopyPixels,bool e_bAllocateMemoryForPixelIfFetch,GLenum e_eImageType)
	{

#ifdef DEBUG
		std::wstring l_strFileName = e_strName;
		l_strFileName += L" start to parse:Texture";
		cGameApp::OutputDebugInfoString(l_strFileName, true, true);
#endif
		m_pstrFullFileName = nullptr;
		m_pPixels = 0;
		m_fUV[0] = 0.f; m_fUV[1] = 0.f; m_fUV[2] = 1.f; m_fUV[3] = 1.f;
		m_uiImageIndex = -1;
		m_iChannel = GetChannelByColorFormat(e_eImageType);
		this->SetName(e_strName);
		this->AddRef(e_pOwner);
		this->m_iHeight = e_iHeight;
		this->m_iWidth = e_iWidth;
		if( e_bCopyPixels )
		{
			if( e_bAllocateMemoryForPixelIfFetch )
			{
				int	l_iSize = m_iHeight*m_iWidth;
				if( e_eImageType == GL_RGBA )
				{
					m_pPixels = new char[l_iSize*4];
					memcpy(m_pPixels,e_pPixels,sizeof(unsigned char)*l_iSize*4);
				}
				else
				if( e_eImageType == GL_RGB )
				{
					m_pPixels = new char[l_iSize*3];
					memcpy(m_pPixels,e_pPixels,sizeof(unsigned char)*l_iSize*3);
				}
			}
			else
				m_pPixels = e_pPixels;
		}
		else
			m_pPixels = 0;
		m_iPixelFormat = e_eImageType;
		glGenTextures(1, &m_uiImageIndex); /* Texture name generation */
		assert(m_uiImageIndex!=(UINT)-1&&"opengl init???");
//#ifdef 	OPENGLES_2_X
		glActiveTexture(GL_TEXTURE0);
//#endif
		m_suiLastUsingImageIndex = -1;
		this->ApplyImage();/* Binding of texture name */
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, g_fMAG_FILTERValue); /* We will use linear  interpolation for magnification filter */
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, g_fMIN_FILTERValue); /* We will use linear  interpolation for minifying filter */
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);
		//quick way to find out power of two.
		//if (width & (width - 1)) or (height & (height - 1)):
		int	l_iWidthPO2 = power_of_two(m_iWidth);
		int	l_iHeightPO2 = power_of_two(m_iHeight);
		GLint texSize; MyGLGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
		//make sure power of 2,because not every fukcing graphic card support it
		//but if u exactly sure it do support power of 2 u could mark this.
		if( !g_bSupportNonPowerOfTwoTexture&&(l_iWidthPO2!=m_iWidth||l_iHeightPO2!=m_iHeight) )
		{
			assert((l_iWidthPO2<=texSize||l_iHeightPO2<=texSize)&&"texture size is too big then card support");
	#ifdef DEBUG
			cGameApp::OutputDebugInfoString(e_strName);
			cGameApp::OutputDebugInfoString(L"  image has not correct respond width and height,because none power of 2\n", true, true);
			if( m_iPixelFormat == GL_RGB )
				cGameApp::OutputDebugInfoString(L"UV is changed,because image size is not to become power of 2", true, true);
	#endif
			char*l_pNewPixelData = TextureToPowOfTwo((char*)e_pPixels,m_iWidth,m_iHeight,m_iPixelFormat==GL_RGBA?true:false);
			OpenGLTextureGenerate(GL_TEXTURE_2D, 0, m_iChannel==4?GL_RGBA:GL_RGB, l_iWidthPO2,l_iHeightPO2, 0,m_iPixelFormat, GL_UNSIGNED_BYTE,l_pNewPixelData,e_strName); // Texture specification.
			//assign new UV
			m_fUV[2] = (float)m_iWidth/l_iWidthPO2;
			m_fUV[3] =  (float)m_iHeight/l_iHeightPO2;
			delete l_pNewPixelData;
		}
		else
		{
			assert((m_iWidth<=texSize||m_iHeight<=texSize)&&"texture size is too big then card support");
			OpenGLTextureGenerate(GL_TEXTURE_2D, 0, m_iChannel==4?GL_RGBA:GL_RGB, m_iWidth,m_iHeight, 0,m_iPixelFormat, GL_UNSIGNED_BYTE,e_pPixels,e_strName); // Texture specification.
		}
		cTextureManager::GetInstance()->AddObjectWithDebugInfo(this);
	}
	//===============
	//
	//===============
	cTexture::cTexture(NamedTypedObject*e_pOwner,const char*e_strImageFileName,bool e_bFetchPixelData)
	{
#ifdef DEBUG
		if (e_strImageFileName)
		{
			std::wstring l_strFileName = ValueToStringW(e_strImageFileName);
			l_strFileName += L" start to parse:Texture";
			cGameApp::OutputDebugInfoString(l_strFileName, true, true);
		}
		else
		{
			cGameApp::OutputDebugInfoString("start to parse:Texture but Imahe FileNmae is nullptr", true, true);
		}
#endif
		m_iChannel = 0;
		m_pstrFullFileName = nullptr;
	    m_pPixels = 0;
		m_uiImageIndex = -1;
		m_fUV[0] = 0.f; m_fUV[1] = 0.f; m_fUV[2] = 1.f; m_fUV[3] = 1.f;
		this->m_iHeight = 0;
		this->m_iWidth = 0;
		this->AddRef(e_pOwner);
		if( !e_strImageFileName )
		{//if image resource does't need to read,the texture resource is come from external,so we do not to delete it
			return;
		}
		if (!LoadImage(e_strImageFileName, e_bFetchPixelData))
		{
#ifdef DEBUG
			std::string l_strFileName = e_strImageFileName;
			l_strFileName += "parse:Texture failed!";
			cGameApp::OutputDebugInfoString(l_strFileName, true, true);
#endif
		}
		cTextureManager::GetInstance()->AddObjectWithDebugInfo(this);
	}
	//===============
	//
	//===============
	cTexture::~cTexture()
	{
		Destroy();
		cTextureManager::GetInstance()->RemoveObjectWithDebugInfo(this);
	}

	//===============
	//
	//===============
	bool	cTexture::LoadDDS(const char*e_strFileName)
	{
		//glGetError();
		NvBool l_bAlpha = true;
		//NvBool flipVertical(false), useMipmaps(false), l_bAlpha, l_bCubeMap;

		//GLuint tex = 0;
		//NVHHDDSImage *img = NVHHDDSLoad(e_strFileName, flipVertical ? 1 : 0);
		//if (img)
		//{
		//	l_bCubeMap = img->cubemap ? NV_TRUE : NV_FALSE;
		//	m_iWidth = img->width;
		//	m_iHeight = img->height;
		//	l_bAlpha = img->alpha ? NV_TRUE : NV_FALSE;
		//	glGenTextures(1, &tex);
		//	if (!img->cubemap)
		//	{
		//		glBindTexture(GL_TEXTURE_2D, tex);
		//		LoadTextureFromDDSData(GL_TEXTURE_2D, 0, img, useMipmaps);
		//	}
		//	else
		//	{
		//		NvS32 baseLevel = 0;
		//		glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

		//		LoadTextureFromDDSData(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, baseLevel, img, useMipmaps);
		//		baseLevel += img->numMipmaps ? img->numMipmaps : 1;

		//		LoadTextureFromDDSData(GL_TEXTURE_CUBE_MAP_POSITIVE_X, baseLevel, img, useMipmaps);
		//		baseLevel += img->numMipmaps ? img->numMipmaps : 1;

		//		LoadTextureFromDDSData(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, baseLevel, img, useMipmaps);
		//		baseLevel += img->numMipmaps ? img->numMipmaps : 1;

		//		LoadTextureFromDDSData(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, baseLevel, img, useMipmaps);
		//		baseLevel += img->numMipmaps ? img->numMipmaps : 1;

		//		LoadTextureFromDDSData(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, baseLevel, img, useMipmaps);
		//		baseLevel += img->numMipmaps ? img->numMipmaps : 1;

		//		LoadTextureFromDDSData(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, baseLevel, img, useMipmaps);
		//		baseLevel += img->numMipmaps ? img->numMipmaps : 1;
		//	}

		//	GLint err = glGetError();
		//	if (err)
		//	{
		//		//NVLogError("NvCreateTextureFromDDSEx error", "");
		//		cGameApp::OutputDebugInfoString(L"NvCreateTextureFromDDSEx error");
		//	}

		//	NVHHDDSFree(img);
		//}

		////return tex;



		//=================
		m_iChannel = 3;
		m_iPixelFormat = GL_RGB;
//		unsigned char l_bAlpha = false;
		this->m_uiImageIndex = NvCreateTextureFromDDSEx(e_strFileName,false,false,&this->m_iWidth,&this->m_iHeight,&l_bAlpha,NULL);
		if( l_bAlpha > 0 )
		{
			this->m_iChannel = 4;
			m_iPixelFormat = GL_RGBA;
		}
		if( m_uiImageIndex != 0 )
			return true;
		return false;
	}
	//===============
	//
	//===============
	bool	cTexture::LoadImage(const char*e_strImageFileName,bool e_bFetchPixelData)
	{
		assert(e_strImageFileName);
		std::string	l_strImageName = UT::GetFileNameWithoutFullPath(e_strImageFileName);
		std::wstring	l_strImageNameW =  CharToWchar(l_strImageName.c_str());
		//is loading the same image again?
		if( this->IsSameName(l_strImageNameW.c_str()) )
			return false;
		//delete old one if existed
		Destroy();
		//asign new name
		this->SetName(l_strImageNameW);
		SAFE_DELETE(m_pstrFullFileName);
		m_pstrFullFileName = new std::string;
		*m_pstrFullFileName = e_strImageFileName;
		std::vector<unsigned char> image;
		unsigned char*l_pucPixelData = nullptr;
		unsigned l_uWidth, l_uHeight;
		std::string l_strExtensionName = UT::GetFileExtensionName(e_strImageFileName);
		if( l_strExtensionName.compare("dds") == 0 || l_strExtensionName.compare("DDS") == 0  )
		{
			//dds set all data(height,texture index...) so return ture
			if(LoadDDS(e_strImageFileName))
				return true;
			return false;
		}
		else
		if( l_strExtensionName.compare("png") == 0  )
		{
			unsigned error = lodepng::decode(image, l_uWidth, l_uHeight, e_strImageFileName);
			m_iPixelFormat = GL_RGBA;
			//now I am laze to do bitmpa and dds file so force maake it as 4 channel
			m_iChannel = 4;
			// If there's an error, display it.
			if(error != 0)
			{
				UT::ErrorMsg(e_strImageFileName,lodepng_error_text(error));
				return false;
			}
			l_pucPixelData = &image[0];
		}
		else
		if(l_strExtensionName.compare("jpg") == 0 )
		{
			int	l_iChannel = 0;
			unsigned char*l_pImageData = nullptr;
			//3 for rgb no alpha here
			l_pImageData = jpgd::decompress_jpeg_image_from_file(e_strImageFileName,(int*)&l_uWidth,(int*)&l_uHeight,&l_iChannel,3);
			if( l_pImageData == nullptr )
				return false;
			l_pucPixelData = l_pImageData;
			//int	l_iImageSize = 3*l_uWidth*l_uHeight;
			//image.resize(l_iImageSize);
			//memcpy(&image[0],l_pImageData,l_iImageSize);
			m_iPixelFormat = GL_RGB;
			//now I am laze to do bitmpa and dds file so force maake it as 4 channel
			m_iChannel = 3;
		}
		else
		{
			return false;
		}
		m_iWidth = l_uWidth;
		m_iHeight = l_uHeight;
		glGenTextures(1, &m_uiImageIndex); /* Texture name generation */
		//		GLenum	l_GLenum = glGetError();
		//		assert(l_GLenum);
//#ifdef 	OPENGLES_2_X
		glActiveTexture(GL_TEXTURE0);
//#endif
		m_suiLastUsingImageIndex = -1;
		this->ApplyImage();
		// Set the texture parameters to use a minifying filter and a linear filer (weighted average)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, g_fMIN_FILTERValue);				
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, g_fMAG_FILTERValue); /* We will use linear  interpolation for magnification filter */
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);
		UpdatePixels((const GLvoid*)l_pucPixelData,e_bFetchPixelData);
//#endif
		if(!e_bFetchPixelData && l_strExtensionName.compare("jpg") == 0 )
		{
			delete l_pucPixelData;
		}
		//if loading failed show alert
		if(!m_uiImageIndex)
		{
			ErrorMsg(L"opengl process Image Failed",UT::CharToWchar(e_strImageFileName));
			return false;
		}
		return true;
	}

	bool	cTexture::ApplyImage(GLuint e_TextureID)
	{
	    if( m_suiLastUsingImageIndex != e_TextureID )
	    {
			if( e_TextureID == 0 )
				return false;
		    glBindTexture( GL_TEXTURE_2D, e_TextureID);
		    m_suiLastUsingImageIndex = e_TextureID;
			return true;
		}
		return true;	
	}

	bool	cTexture::ApplyImage()
	{
	    if( m_suiLastUsingImageIndex != m_uiImageIndex )
	    {
			if( m_uiImageIndex == 0 )
				return false;
		    glBindTexture( GL_TEXTURE_2D, m_uiImageIndex);
		    m_suiLastUsingImageIndex = m_uiImageIndex;
			return true;
		}
		return true;
	}
	//===============
	//
	//===============
	void	cTexture::Destroy()
	{
		SAFE_DELETE(m_pstrFullFileName);
		OpenGLDeleteTexture(this);
		m_uiImageIndex = -1;
		//if the pixels data is not from resource,free it
		SAFE_DELETE(m_pPixels);
	}

	float*	cTexture::GetUV()
	{
		return m_fUV;
	}
	void	cTexture::SetupTexture(GLint e_iChannel,GLsizei e_iWidth,GLsizei e_iHeight,GLenum e_Format,GLenum e_Type)
	{
		m_iChannel = e_iChannel;
		m_iPixelFormat = e_Format;
		this->m_iWidth = e_iWidth;
		this->m_iHeight = e_iHeight;
		int	l_iWidthPO2 = power_of_two(m_iWidth);
		int	l_iHeightPO2 = power_of_two(m_iHeight);
		int	l_iPixelWidth = m_iWidth;
		int	l_iPixelHeight = m_iHeight;
		if(  !g_bSupportNonPowerOfTwoTexture && (l_iWidthPO2!=m_iWidth||l_iHeightPO2!=m_iHeight) )//make it power of two
		{
			m_fUV[2] = (float)m_iWidth/(float)l_iWidthPO2;
			m_fUV[3] =  (float)m_iHeight/(float)l_iHeightPO2;		
			l_iPixelWidth = l_iWidthPO2;
			l_iPixelHeight = l_iHeightPO2;
		}
		if( m_uiImageIndex == -1 )
		{
			glGenTextures(1, &m_uiImageIndex); /* Texture name generation */	
//#ifndef OPENGLES_2_X
//			MyGLEnable(GL_TEXTURE_2D);
//#endif
			glBindTexture( GL_TEXTURE_2D, m_uiImageIndex);
			glTexImage2D(GL_TEXTURE_2D, 0, this->m_iChannel == 3?GL_RGB:GL_RGBA, l_iPixelWidth,l_iPixelHeight, 0,m_iPixelFormat, GL_UNSIGNED_BYTE,0);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);	// Set Texture Max Filter
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);	// Set Texture Min Filter
			
		}
	}

	void	cTexture::SetupTexture(GLint e_iChannel,GLsizei e_iWidth,GLsizei e_iHeight,GLenum e_Format,GLenum e_Type,bool e_bGeneratePixels,const GLvoid *e_pPixels)
	{
		SetupTexture(e_iChannel,e_iWidth,e_iHeight,e_Format,e_Type);
		if( e_pPixels )
		{
			UpdatePixels(e_pPixels,e_bGeneratePixels);
		}
	}

	void	cTexture::ForceUpdatePixels(const GLvoid *e_pPixels)
	{
		int	l_iPixelWidth = m_iWidth;
		int	l_iPixelHeight = m_iHeight;
		int	l_iWidthPO2 = power_of_two(m_iWidth);
		int	l_iHeightPO2 = power_of_two(m_iHeight);
		if(  !g_bSupportNonPowerOfTwoTexture && (l_iWidthPO2!=m_iWidth||l_iHeightPO2!=m_iHeight) )//make it power of two
		{
			l_iPixelWidth = l_iWidthPO2;
			l_iPixelHeight = l_iHeightPO2;
		}
		glBindTexture( GL_TEXTURE_2D, m_uiImageIndex);
		glTexSubImage2D(GL_TEXTURE_2D,0,0,0,l_iPixelWidth,l_iPixelHeight,m_iPixelFormat,GL_UNSIGNED_BYTE,e_pPixels);
	}

	void	cTexture::UpdatePixels(const GLvoid *e_pPixels,bool e_bFetchPuxelData)
	{
		SAFE_DELETE(m_pPixels);
		glBindTexture( GL_TEXTURE_2D, m_uiImageIndex);
		int	l_iWidthPO2 = power_of_two(m_iWidth);
		int	l_iHeightPO2 = power_of_two(m_iHeight);
		if(  !g_bSupportNonPowerOfTwoTexture && (l_iWidthPO2!=m_iWidth||l_iHeightPO2!=m_iHeight) )//make it power of two
		{
			char*l_pNewPixelData = TextureToPowOfTwo((char*)e_pPixels,m_iWidth,m_iHeight,this->m_iChannel==4?true:false);
			OpenGLTextureGenerate(GL_TEXTURE_2D, 0, this->m_iChannel == 3?GL_RGB:GL_RGBA, l_iWidthPO2,l_iHeightPO2, 0,this->m_iPixelFormat, GL_UNSIGNED_BYTE,l_pNewPixelData,this->GetName()); // Texture specification.
			//assign new UV
			m_fUV[2] = (float)m_iWidth/(float)l_iWidthPO2;
			m_fUV[3] =  (float)m_iHeight/(float)l_iHeightPO2;
			if( e_bFetchPuxelData )
			{//because we wanna to detected the pixels alpha for collide so we have to resize
				m_pPixels = l_pNewPixelData;
			}
			else
			{//we do not need the alpha so modify UV to show the part we needed
				delete l_pNewPixelData;
			}		
		}
		else
		{
			OpenGLTextureGenerate(GL_TEXTURE_2D, 0, this->m_iChannel == 3?GL_RGB:GL_RGBA, m_iWidth,m_iHeight, 0,m_iPixelFormat, GL_UNSIGNED_BYTE,e_pPixels,GetName()); // Texture specification.
			if( e_bFetchPuxelData )
			{
				int	l_iDataSize = m_iWidth*m_iHeight*4;
				m_pPixels = (char*)malloc(l_iDataSize);
				memcpy(m_pPixels,e_pPixels,sizeof(char)*l_iDataSize);
			}		
		}
	}

	char*		cTexture::GeneratePowerOfTwoPixelData(GLint e_iChannel,GLsizei e_iWidth,GLsizei e_iHeight,GLenum e_Format,GLenum e_Type,const GLvoid *e_pPixels)
	{
		int	l_iWidthPO2 = power_of_two(e_iWidth);
		int	l_iHeightPO2 = power_of_two(e_iHeight);
		if(  !g_bSupportNonPowerOfTwoTexture && (l_iWidthPO2!=e_iWidth||l_iHeightPO2!=e_iHeight) )//make it power of two
			return TextureToPowOfTwo((char*)e_pPixels,e_iWidth,e_iHeight,e_iChannel==4?true:false);
		return 0;
	}
}