#include "../../../Android/JNIUtil.h"
#include "Texture.h"
#include "TextureManager.h"
#include "../CommonRender/SimplePrimitive.h"
#include "../../../Common/Utility.h"
#include "../../GameApp/GameApp.h"
#include "png/pngLoader.h"
#include "dds/nv_images.h"
#include "jpeg/jpgd.h"
#include "jpeg/jpge.h"
#ifdef ANDROID
#include <android/bitmap.h>
#endif
//nv_images.cpp
extern int LoadTextureFromDDSData(GLenum target, NvS32 startLevel, const NVHHDDSImage *pImage, NvBool useMipmaps);
#ifdef IOS
//saving an image 
extern UIImage*	glToUIImage();
extern void		captureToPhotoAlbum();
#endif 
//#define	ENABLE_FORCE_TRY_TO_PARSE_DDS
namespace FATMING_CORE
{
	//if graphic card does not support non power of two,set as false
#ifdef WIN32
	bool	g_bSupportNonPowerOfTwoTexture = true;
#else
	bool	g_bSupportNonPowerOfTwoTexture = false;
#endif
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

	GLint	GetPixelFormatByChannel(int e_iChannel)
	{
		if (e_iChannel == 1)
		{
			return GL_LUMINANCE;
		}
		if (e_iChannel == 3)
		{
			return GL_RGB;
		}
		return GL_RGBA;
	}

	cTexture::cTexture(NamedTypedObject*e_pOwner,char*e_pPixels,int e_iWidth,int e_iHeight,const wchar_t*e_strName,bool e_bCopyPixels,bool e_bAllocateMemoryForPixelIfFetch,GLenum e_eImageType)
	:cSmartObject(this)
	{

#ifdef DEBUG
		std::wstring l_strFileName = e_strName;
		l_strFileName += L" start to parse:Texture";
		FMLog::LogWithFlag(l_strFileName, CORE_LOG_FLAG);
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
				m_pPixels = new char[l_iSize* m_iChannel];
				memcpy(m_pPixels,e_pPixels,sizeof(unsigned char)*l_iSize* m_iChannel);
			}
			else
			{
				m_pPixels = e_pPixels;
			}
		}
		else
		{
			m_pPixels = nullptr;
		}
		m_iPixelFormat = e_eImageType;
		glGenTextures(1, &m_uiImageIndex); /* Texture name generation */
		assert(m_uiImageIndex!=(UINT)-1&&"opengl init???");
		glActiveTexture(GL_TEXTURE0);
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
			FMLog::LogWithFlag(e_strName, CORE_LOG_FLAG);
			FMLog::LogWithFlag(L"  image has not correct respond width and height,because none power of 2\n", true);
			if( m_iPixelFormat == GL_RGB )
				FMLog::LogWithFlag(L"UV is changed,because image size is not to become power of 2", true);
	#endif
			char*l_pNewPixelData = TextureToPowOfTwo((char*)e_pPixels,m_iWidth,m_iHeight,m_iPixelFormat==GL_RGBA?true:false);
			OpenGLTextureGenerate(GL_TEXTURE_2D, 0, m_iChannel==4?GL_RGBA:GL_RGB, l_iWidthPO2,l_iHeightPO2, 0,m_iPixelFormat, GL_UNSIGNED_BYTE,l_pNewPixelData,e_strName); // Texture specification.
			//assign new UV
			m_fUV[2] = (float)m_iWidth/ (float)l_iWidthPO2;
			m_fUV[3] =  (float)m_iHeight/ (float)l_iHeightPO2;
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
	cTexture::cTexture(NamedTypedObject*e_pOwner,const char*e_strImageFileName,bool e_bFetchPixelData) :cSmartObject(this)
	{
#ifdef DEBUG
		if (e_strImageFileName)
		{
			std::wstring l_strFileName = ValueToStringW(e_strImageFileName);
			l_strFileName += L" start to parse:Texture";
			FMLog::LogWithFlag(l_strFileName, true);
		}
		else
		{
			FMLog::LogWithFlag("start to parse:Texture but Imahe FileNmae is nullptr", true);
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
			FMLog::LogWithFlag(l_strFileName, true);
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
		//		FMLog::LogWithFlag(L"NvCreateTextureFromDDSEx error");
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
#ifndef DEBUG
//#define	USE_HW_DECODE_BITMAP
#endif
#define	USE_HW_DECODE_BITMAP
#if defined(ANDROID) && defined(USE_HW_DECODE_BITMAP)
	//std::string	GetFileFullPathInAndroid(const char* e_strFileName)
	//{
	//	//http://blog.sephiroth.it/2010/10/24/reading-resource-files-from-native-code/
	//	//http://androgeek.info/?p=275
	//	//try external sd card first
	//	NvFile*fp = nullptr;
	//	if (cGameApp::m_spExternalSDDirectory)
	//	{
	//		std::string l_strFileName = *cGameApp::m_spExternalSDDirectory;
	//		l_strFileName += *cGameApp::m_psstrGameAppName;
	//		l_strFileName += "/";
	//		l_strFileName += e_strFileName;
	//		fp = NvFOpen(l_strFileName.c_str(), "r");
	//		if (fp)
	//		{
	//			NvFClose(fp);
	//			return l_strFileName;
	//		}
	//	}
	//	//try write file into
	//	std::string l_strFileName = "/sdcard/";
	//	l_strFileName += *cGameApp::m_psstrGameAppName;
	//	l_strFileName += "/";
	//	l_strFileName += e_strFileName;
	//	fp = NvFOpen(l_strFileName.c_str(), "r");
	//	if (!fp)
	//	{//write into internal memory
	//		l_strFileName = cGameApp::m_spInternalDirectory->c_str();
	//		l_strFileName += "/";
	//		l_strFileName += *cGameApp::m_psstrGameAppName;
	//		l_strFileName += "/";
	//		l_strFileName += e_strFileName;
	//		fp = NvFOpen(l_strFileName.c_str(), "r");
	//	}
	//	if (fp)
	//		NvFClose(fp);
	//	return l_strFileName;
	//}
	bool	cTexture::LoadImage(const char*e_strImageFileName, bool e_bFetchPixelData)
	{
		assert(e_strImageFileName);
		std::string	l_strImageName = e_strImageFileName;//UT::GetFileNameWithoutFullPath(e_strImageFileName);
		std::wstring	l_strImageNameW = CharToWchar(l_strImageName.c_str());
		//is loading the same image again?
		if (this->IsSameName(l_strImageNameW.c_str()))
			return false;
		//delete old one if existed
		Destroy();
		//asign new name
		this->SetName(l_strImageNameW);
		SAFE_DELETE(m_pstrFullFileName);
		m_pstrFullFileName = new std::string;
		*m_pstrFullFileName = e_strImageFileName;
#ifdef ENABLE_FORCE_TRY_TO_PARSE_DDS	//default load dds
		std::string l_strDDSFileName = ChangeFileExtensionName(m_pstrFullFileName->c_str(), "dds");
		if (UT::IsFileExists(l_strDDSFileName.c_str()))
		{
			*m_pstrFullFileName = l_strDDSFileName;
		}
#endif
		jobject l_BitMapObject = nullptr;
		AndroidBitmapInfo l_AndroidBitmapInfo;
		std::vector<unsigned char> image;
		unsigned char*l_pucPixelData = nullptr;
		std::string l_strExtensionName = UT::GetFileExtensionName(m_pstrFullFileName->c_str());
		if (l_strExtensionName.compare("jpg") == 0 || l_strExtensionName.compare("png") == 0)
		{
			auto l_strandroidFileName = ValueToStringW(e_strImageFileName);
			jstring l_jstrFileName = WcharToJava(g_pMainThreadJNIUtilData->pJNIEnv, l_strandroidFileName.c_str());
			EXCEPTION_RETURN_NULL(g_pMainThreadJNIUtilData->pJNIEnv);
			jclass	l_MyBitMapClass = GetCustomJavaClass("util/MyBitmap");
			EXCEPTION_RETURN_NULL(g_pMainThreadJNIUtilData->pJNIEnv);
			jmethodID l_LoadBitmapAndPassToTextureMethodID = g_pMainThreadJNIUtilData->pJNIEnv->GetStaticMethodID(l_MyBitMapClass, "LoadBitmapAndPassToTexture", "(Landroid/content/Context;Ljava/lang/String;)Landroid/graphics/Bitmap;");
			EXCEPTION_RETURN_NULL(g_pMainThreadJNIUtilData->pJNIEnv);
			l_BitMapObject = g_pMainThreadJNIUtilData->pJNIEnv->CallStaticObjectMethod(l_MyBitMapClass, l_LoadBitmapAndPassToTextureMethodID,cGameApp::m_spANativeActivity->clazz, l_jstrFileName);
			EXCEPTION_RETURN_NULL(g_pMainThreadJNIUtilData->pJNIEnv);
			void*l_pixels;
			int l_iResult = AndroidBitmap_getInfo(g_pMainThreadJNIUtilData->pJNIEnv, l_BitMapObject, &l_AndroidBitmapInfo);
			l_iResult = AndroidBitmap_lockPixels(g_pMainThreadJNIUtilData->pJNIEnv, l_BitMapObject, &l_pixels);
			l_pucPixelData = (unsigned char*)l_pixels;
			m_iPixelFormat = GL_RGBA;
			m_iChannel = 4;
			this->m_iWidth = l_AndroidBitmapInfo.width;
			this->m_iHeight = l_AndroidBitmapInfo.height;
		}
		else
		if (l_strExtensionName.compare("dds") == 0 || l_strExtensionName.compare("DDS") == 0)
		{
			//dds set all data(height,texture index...) so return ture
			if (LoadDDS(m_pstrFullFileName->c_str()))
				return true;
			return false;
		}
		else
		{
			return false;
		}
		glGenTextures(1, &m_uiImageIndex); /* Texture name generation */
										   //		GLenum	l_GLenum = glGetError();
										   //		assert(l_GLenum);
		glActiveTexture(GL_TEXTURE0);
		//#endif
		m_suiLastUsingImageIndex = -1;
		this->ApplyImage();
		// Set the texture parameters to use a minifying filter and a linear filer (weighted average)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, g_fMIN_FILTERValue);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, g_fMAG_FILTERValue); /* We will use linear  interpolation for magnification filter */
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		UpdatePixels((const GLvoid*)l_pucPixelData, e_bFetchPixelData);
		//#endif
		if (l_BitMapObject)
		{
			AndroidBitmap_unlockPixels(g_pMainThreadJNIUtilData->pJNIEnv, l_BitMapObject);
		}
		//if loading failed show alert
		if (!m_uiImageIndex)
		{
			ErrorMsg(L"opengl process Image Failed ", UT::CharToWchar(m_pstrFullFileName->c_str()));
			return false;
		}
		return true;
	}
#else
	bool	cTexture::LoadImage(const char*e_strImageFileName, bool e_bFetchPixelData)
	{
		assert(e_strImageFileName);
		std::string	l_strImageName = e_strImageFileName;// UT::GetFileNameWithoutFullPath(e_strImageFileName);
		std::wstring	l_strImageNameW = CharToWchar(l_strImageName.c_str());
		//is loading the same image again?
		if (this->IsSameName(l_strImageNameW.c_str()))
			return false;
		//delete old one if existed
		Destroy();
		//asign new name
		this->SetName(l_strImageNameW);
		SAFE_DELETE(m_pstrFullFileName);
		m_pstrFullFileName = new std::string;
		*m_pstrFullFileName = e_strImageFileName;
#ifdef ENABLE_FORCE_TRY_TO_PARSE_DDS	//default load dds
		std::string l_strDDSFileName = ChangeFileExtensionName(m_pstrFullFileName->c_str(), "dds");
		if (UT::IsFileExists(l_strDDSFileName.c_str()))
		{
			*m_pstrFullFileName = l_strDDSFileName;
		}
#endif
		std::vector<unsigned char> image;
		unsigned char*l_pucPixelData = nullptr;
		std::string l_strExtensionName = UT::GetFileExtensionName(m_pstrFullFileName->c_str());
		if (l_strExtensionName.compare("dds") == 0 || l_strExtensionName.compare("DDS") == 0)
		{
			//dds set all data(height,texture index...) so return ture
			if (LoadDDS(m_pstrFullFileName->c_str()))
				return true;
			return false;
		}
		else
		if( l_strExtensionName.compare("png") == 0  )
		{
			unsigned l_uiWidth = 0;
			unsigned l_uiHeight = 0;
			unsigned error = lodepng::decode(image, l_uiWidth, l_uiHeight, m_pstrFullFileName->c_str());
			m_iWidth = l_uiWidth;
			m_iHeight = l_uiHeight;
			m_iPixelFormat = GL_RGBA;
			//now I am laze to do bitmpa and dds file so force maake it as 4 channel
			m_iChannel = 4;
			// If there's an error, display it.
			if(error != 0)
			{
				UT::ErrorMsg(m_pstrFullFileName->c_str(),lodepng_error_text(error));
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
			l_pImageData = jpgd::decompress_jpeg_image_from_file(m_pstrFullFileName->c_str(),(int*)&m_iWidth,(int*)&m_iHeight,&l_iChannel,3);
			if( l_pImageData == nullptr )
				return false;
			l_pucPixelData = l_pImageData;
			m_iPixelFormat = GL_RGB;
			//now I am laze to do bitmpa and dds file so force maake it as 4 channel
			m_iChannel = 3;
		}
		else
		{
			return false;
		}
		glGenTextures(1, &m_uiImageIndex); /* Texture name generation */
		CHECK_GL_ERROR("cTexture::LoadImage glGenTextures");
		//		GLenum	l_GLenum = glGetError();
		//		assert(l_GLenum);
		glActiveTexture(GL_TEXTURE0);
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
			ErrorMsg(L"opengl process Image Failed ",UT::CharToWchar(m_pstrFullFileName->c_str()));
			return false;
		}
		return true;
	}
#endif
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

	std::wstring cTexture::DescriptionInfo()
	{
		wchar_t   l_strStrDescription[MAX_PATH];
		swprintf(l_strStrDescription, MAX_PATH, L"Size:%d,%d\nUV:%.2f,%2.f,%.2f,%.2f", m_iWidth, m_iHeight, m_fUV[0], m_fUV[1], m_fUV[2], m_fUV[3]);
		std::wstring l_strResult = l_strStrDescription;
		return l_strResult;
	}

	bool	cTexture::ApplyImageWithActiveTextureID(GLuint e_uintActiveTextureID)
	{
		glActiveTexture(GL_TEXTURE0 + e_uintActiveTextureID);
		glBindTexture(GL_TEXTURE_2D, m_uiImageIndex);
		m_suiLastUsingImageIndex = -1;
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
		CHECK_GL_ERROR("cTexture::SetupTexture 1");
		if( m_uiImageIndex == (GLuint)-1 )
		{
			glGenTextures(1, &m_uiImageIndex); /* Texture name generation */	
			glBindTexture( GL_TEXTURE_2D, m_uiImageIndex);
			auto l_uiPixelFormat = GetPixelFormatByChannel(m_iChannel);
			glTexImage2D(GL_TEXTURE_2D, 0, l_uiPixelFormat, l_iPixelWidth, l_iPixelHeight, 0, m_iPixelFormat, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);	// Set Texture Max Filter
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);	// Set Texture Min Filter
			
		}
		CHECK_GL_ERROR("cTexture::SetupTexture 2");
	}

	void	cTexture::SetupTexture(GLint e_iChannel,GLsizei e_iWidth,GLsizei e_iHeight,GLenum e_Format,GLenum e_Type,bool e_bFetchPixels,const GLvoid *e_pPixels, bool e_bShowLog)
	{
		SetupTexture(e_iChannel,e_iWidth,e_iHeight,e_Format,e_Type);
		if( e_pPixels )
		{
			UpdatePixels(e_pPixels, e_bFetchPixels, e_bShowLog);
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

	void	cTexture::UpdatePixels(const GLvoid *e_pPixels,bool e_bFetchPuxelData, bool e_bShowLog)
	{
		CHECK_GL_ERROR("cTexture::UpdatePixels 1");
		SAFE_DELETE(m_pPixels);
		glBindTexture( GL_TEXTURE_2D, m_uiImageIndex);
		int	l_iWidthPO2 = power_of_two(m_iWidth);
		int	l_iHeightPO2 = power_of_two(m_iHeight);
		if(  !g_bSupportNonPowerOfTwoTexture && (l_iWidthPO2!=m_iWidth||l_iHeightPO2!=m_iHeight) )//make it power of two
		{
			char*l_pNewPixelData = TextureToPowOfTwo((char*)e_pPixels,m_iWidth,m_iHeight,this->m_iChannel==4?true:false);
			OpenGLTextureGenerate(GL_TEXTURE_2D, 0, this->m_iChannel == 3?GL_RGB:GL_RGBA, l_iWidthPO2,l_iHeightPO2, 0,this->m_iPixelFormat, GL_UNSIGNED_BYTE,l_pNewPixelData,this->GetName(), e_bShowLog); // Texture specification.
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
			auto l_uiPixelFormat = GetPixelFormatByChannel(m_iChannel);
			OpenGLTextureGenerate(GL_TEXTURE_2D, 0, l_uiPixelFormat, m_iWidth, m_iHeight, 0, m_iPixelFormat, GL_UNSIGNED_BYTE, e_pPixels, GetName(), e_bShowLog); // Texture specification.
			if( e_bFetchPuxelData )
			{
				int	l_iDataSize = m_iWidth*m_iHeight*4;
				m_pPixels = (char*)malloc(l_iDataSize);
				memcpy(m_pPixels,e_pPixels,sizeof(char)*l_iDataSize);
			}		
		}
		CHECK_GL_ERROR("cTexture::UpdatePixels 2");
	}

	char*		cTexture::GeneratePowerOfTwoPixelData(GLint e_iChannel,GLsizei e_iWidth,GLsizei e_iHeight,GLenum e_Format,GLenum e_Type,const GLvoid *e_pPixels)
	{
		int	l_iWidthPO2 = power_of_two(e_iWidth);
		int	l_iHeightPO2 = power_of_two(e_iHeight);
		if(  !g_bSupportNonPowerOfTwoTexture && (l_iWidthPO2!=e_iWidth||l_iHeightPO2!=e_iHeight) )//make it power of two
			return TextureToPowOfTwo((char*)e_pPixels,e_iWidth,e_iHeight,e_iChannel==4?true:false);
		return 0;
	}

	GLuint	CreateTextureFromBinary(int e_iChannel,void*e_pPixelsData,int e_iWidth,int e_iHeight,GLint e_iFormat)
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
	GLuint	CreateTextureFromBinary(int e_iChannel,char*e_pPixelsData,int e_iWidth,int e_iHeight,GLint e_iDataFormat,bool e_bPowOfTwoTexture)
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
	char*	TextureToPowOfTwo(char*e_pSrc, int e_iWidth, int e_iHeight, bool e_balpah)
	{
		int	l_iStride = e_balpah ? 4 : 3;
		int	l_iPowOf2Width = UT::power_of_two(e_iWidth);
		int	l_iPowOf2Height = UT::power_of_two(e_iHeight);
		int	l_iTotalDataSize = l_iStride * l_iPowOf2Width*l_iPowOf2Height;
		char*	l_pPixelData = new char[l_iTotalDataSize];
		//if( e_balpah )
		memset(l_pPixelData, 0, l_iTotalDataSize * sizeof(char));
		for (int j = 0; j < e_iHeight; ++j)
		{
			char*	l_srcData = &e_pSrc[e_iWidth*j*l_iStride];
			char*	l_destData = &l_pPixelData[l_iPowOf2Width*j*l_iStride];
			memcpy(l_destData, l_srcData, sizeof(char)*l_iStride*e_iWidth);
		}
		return l_pPixelData;
	}

	float*	UVToTriangleStrip(float*e_pfUV)
	{
		static	float l_sUV[TWO_TRIANGLE_STRIP_UV_TO_QUAD_UV_COUNT];
		//0
		l_sUV[0] = e_pfUV[0]; l_sUV[1] = e_pfUV[1];
		//1
		l_sUV[2] = e_pfUV[2]; l_sUV[3] = e_pfUV[1];
		//2
		l_sUV[4] = e_pfUV[0]; l_sUV[5] = e_pfUV[3];
		//3
		l_sUV[6] = e_pfUV[2]; l_sUV[7] = e_pfUV[3];
		return l_sUV;
	}
	float*	UVToTwoTriangle(float*e_pfUV)
	{
		static	float l_sUV[TWO_TRIANGLE_VERTICES_UV_TO_QUAD_UV_COUNT];
		//2,3   5
		//0     1,4
		//0
		l_sUV[0] = e_pfUV[0]; l_sUV[1] = e_pfUV[3];
		//1
		l_sUV[2] = e_pfUV[2]; l_sUV[3] = e_pfUV[3];
		//2
		l_sUV[4] = e_pfUV[0]; l_sUV[5] = e_pfUV[1];
		//3
		l_sUV[6] = e_pfUV[0]; l_sUV[7] = e_pfUV[1];
		//4
		l_sUV[8] = e_pfUV[2]; l_sUV[9] = e_pfUV[3];
		//5
		l_sUV[10] = e_pfUV[2]; l_sUV[11] = e_pfUV[1];
		return l_sUV;
	}
	int		GetChannelByColorFormat(GLenum e_GLenum)
	{
		int	l_iChannel = 4;
		switch (e_GLenum)
		{
		case GL_RGBA:
#if !defined(ANDROID) && !defined(WASM) && !defined(LINUX) && !defined(UWP)
		case GL_BGRA:
#endif
			l_iChannel = 4;
			break;
		case GL_RGB:
			l_iChannel = 3;
			break;
		case GL_LUMINANCE:
			l_iChannel = 1;
			break;
		default:
			FMLog::LogWithFlag(L"not support color format", true);
			break;
		}
		return l_iChannel;
	}
//end namespace FATMING_CORE.
}