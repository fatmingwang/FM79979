#include "Texture.h"
#include "TextureManager.h"
//#include "SimplePrimitive.h"
//#include "../Common/Utility.h"
//#include "BinaryToTexture.h"
#include "../../GameplayUT/GameApp/GameApp.h"
//#include "../GameplayUT/BinaryFile.h"
#include "png/pngLoader.h"
#include "dds/nv_images.h"
#include "jpeg/jpgd.h"
#include "jpeg/jpge.h"
#if defined(WIN32)
#elif defined(IOS)
#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIImage.h>
#import <UIKit/UIImagePickerController.h>
#elif defined(ANDROID)
#include "../../../Android/nv_images.h"
#include "../.././Android/JNIUtil.h"
#elif defined(LINUX)
//#include "IL/il.h"//linux
#endif
#ifdef IOS
//saving an image 
UIImage * glToUIImage()
{
	//int l_iWidth  = (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.x;
	//int l_iHeight  = (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.y;
	int l_iWidth = (int)FATMING_CORE::cGameApp::m_spOpenGLRender->m_vViewPortSize.w;
	int l_iHeight = (int)FATMING_CORE::cGameApp::m_spOpenGLRender->m_vViewPortSize.z;

	NSInteger myDataLength = l_iWidth * l_iHeight * 4;

	// allocate array and read pixels into it.
	GLubyte *buffer = (GLubyte *)malloc(myDataLength);
	glReadPixels(0, 0, l_iWidth, l_iHeight, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	// gl renders "upside down" so swap top to bottom into new array.
	// there's gotta be a better way, but this works.
	GLubyte *buffer2 = (GLubyte *)malloc(myDataLength);
	for (int y = 0; y < l_iHeight; y++)
	{
		for (int x = 0; x < l_iWidth * 4; x++)
		{
			buffer2[(l_iHeight - 1 - y) * l_iWidth * 4 + x] = buffer[y * 4 * l_iWidth + x];
		}
	}

	// make data provider with data.
	CGDataProviderRef provider = CGDataProviderCreateWithData(nullptr, buffer2, myDataLength, nullptr);

	// prep the ingredients
	int bitsPerComponent = 8;
	int bitsPerPixel = 32;
	int bytesPerRow = 4 * l_iWidth;
	CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
	CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
	CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;

	// make the cgimage
	CGImageRef imageRef = CGImageCreate(l_iWidth, l_iHeight, bitsPerComponent, bitsPerPixel, bytesPerRow, colorSpaceRef, bitmapInfo, provider, nullptr, NO, renderingIntent);

	// then make the uiimage from that
	UIImage *myImage = [UIImage imageWithCGImage : imageRef];
	return myImage;
}

void    captureToPhotoAlbum()
{
	UIImage *image = glToUIImage();
	UIImageWriteToSavedPhotosAlbum(image, nil, nil, nil);
	//[image release];
}
#endif 
namespace FATMING_CORE
{
	UINT	g_iAteVideoMomory = 0;//in KB
	cTextureManager*cTextureManager::m_spTextureManager = nullptr;;
	cTextureManager* cTextureManager::GetInstance()
	{
		if (!m_spTextureManager)
		{
			m_spTextureManager = new cTextureManager();
		}
		return m_spTextureManager;
	}
	cTextureManager::cTextureManager()
	{
		SetName(L"cTextureManager");
	}

	cTextureManager::~cTextureManager()
	{
		m_spTextureManager = nullptr;
	}

	void	cTextureManager::AddObjectWithDebugInfo(cTexture*e_pTexture)
	{
#ifdef DEBUG
		if (GetObjectIndexByName(e_pTexture->GetName()) != -1)
		{
			UT::ErrorMsgByFormat(L"%ls -PI same name as PIUnit while it's PI editor or something wrong?", e_pTexture->GetName());
		}
		std::wstring l_str = e_pTexture->GetName();
		l_str += L" add into cTextureManager";
		FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
#endif
		assert(GetObjectIndexByName(e_pTexture->GetName()) == -1);
		AddObjectNeglectExist(e_pTexture);
	}
	void	cTextureManager::RemoveObjectWithDebugInfo(cTexture*e_pTexture)
	{
		assert(GetObjectIndexByName(e_pTexture->GetName()) != -1);
		RemoveObjectWithoutDelete(e_pTexture);
#ifdef DEBUG
		std::wstring l_strFileName = e_pTexture->GetName();
		l_strFileName += L" destroy:Texture,Ate Ram:";
		int l_iMB = g_iAteVideoMomory / 1024;
		int l_iKB = g_iAteVideoMomory % 1024;
		if (l_iMB > 0)
		{
			l_strFileName += ValueToStringW(l_iMB);
			l_strFileName += L"M";
		}
		if (l_iKB > 0)
		{
			l_strFileName += ValueToStringW(l_iKB);
			l_strFileName += L"K";
		}
		FMLog::LogWithFlag(l_strFileName.c_str(), CORE_LOG_FLAG);
#endif
		if (Count() == 0)
		{
			SAFE_DELETE(m_spTextureManager);
		}
	}

	void	cTextureManager::UpdateOpenGLTextureBecauseMultiThreadLoading()
	{
		int l_iCount = this->Count();
		for (int i = 0; i < l_iCount; ++i)
		{
			auto l_pObject = cNamedTypedObjectVector<cTexture>::GetObject(i);
			if (l_pObject->GetImageIndex() == -1 && l_pObject->GetPixels())
			{
				
			}
		}
	}

	cTexture*	cTextureManager::GetObject(NamedTypedObject*e_pOwner, const char*e_strImageFileName, bool e_bFetchPixelData)
	{
		cTexture*l_pTexture = nullptr;
		if (e_strImageFileName)
		{
			l_pTexture = cNamedTypedObjectVector<cTexture>::GetObject(e_strImageFileName);
			if (!l_pTexture)
			{
				std::string		l_strImageName = UT::GetFileNameWithoutFullPath(e_strImageFileName);
				l_pTexture = cNamedTypedObjectVector<cTexture>::GetObject(l_strImageName.c_str());
			}
			if (l_pTexture)
			{
				l_pTexture->AddRef(e_pOwner);
				return l_pTexture;
			}
		}
		l_pTexture = new cTexture(e_pOwner, e_strImageFileName, e_bFetchPixelData);
		return l_pTexture;
	}

	void		cTexture::DumpDebugInfo()
	{
		auto l_pTextureStore = cTextureManager::GetInstance();
		if (l_pTextureStore)
		{
			int l_iCount = l_pTextureStore->Count();
			for (int i = 0; i < l_iCount; ++i)
			{
				std::wstring l_strInfo = L"Texture Not Delete:";
				auto l_pTexture = (*l_pTextureStore)[i];
				if (l_pTexture)
					l_strInfo += l_pTexture->GetName();
				else
				{
					l_strInfo += L"call fatming!";
				}
				FMLog::LogWithFlag(l_strInfo.c_str(), CORE_LOG_FLAG);
			}
		}
	}
								  //intel chip,return MB
	int GetVideoMemorySize()
	{
	#ifdef WIN32
		unsigned long	i[5] = { 0, 0, 0x27, 0, 0 };
		unsigned long	o[5] = { 0, 0, 0, 0, 0 };

		HDC hdc = CreateDC(L"DISPLAY", 0, 0, 0);

		if (hdc == nullptr)
		{
			return 0;
		}

		int s = ExtEscape(hdc, 0x7032, 0x14, (LPCSTR)i, 0x14, (LPSTR)o);

		DeleteDC(hdc);

		if (s <= 0)
		{
			return 0;
		}

		return o[3];
	#else
		return 0;
	#endif
	}
	//check video ram,return by kb
	int	CheckRestVRam()
	{
		//const GLubyte*l_str1 = glGetString(GL_VERSION);//const GLubyte*l_str3 = glGetString(GL_RENDERER);
		const GLubyte*l_str2 = glGetString(GL_VENDOR);
		if (!l_str2)
			return 0;
	#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
	#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049
		char str[] = "NVIDIA"; char * pch = strstr((char*)l_str2, str);
		if (pch)//this is nvidia
		{
			//GLint total_mem_kb = 0;
			//MyGLGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,&total_mem_kb);
			//int	l_iTotalMB = total_mem_kb/1024;
			//int	l_iRestTotalKB = total_mem_kb%1024;
			GLint cur_avail_mem_kb = 0;
			MyGLGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &cur_avail_mem_kb);
			return cur_avail_mem_kb;
		}
		else
		{//ATI
			if (strstr((char*)glGetString(GL_EXTENSIONS), "GL_ATI_meminfo"))
			{//http://www.geeks3d.com/20100531/programming-tips-how-to-know-the-graphics-memory-size-and-usage-in-opengl/
	#ifndef GL_VBO_FREE_MEMORY_ATI
	#define GL_VBO_FREE_MEMORY_ATI 0x87FB
	#endif
	#ifndef GL_TEXTURE_FREE_MEMORY_ATI
	#define GL_TEXTURE_FREE_MEMORY_ATI 0x87FC
	#endif
	#ifndef GL_RENDERBUFFER_FREE_MEMORY_ATI
	#define GL_RENDERBUFFER_FREE_MEMORY_ATI 0x87FD
	#endif
			 //returns in kB, so if you want MB,divide by 1024
				int free_mem = -1;
				MyGLGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI, &free_mem);
				return free_mem;

			}
			else
			{//intel?
				return GetVideoMemorySize();
			}
		}
		//have no idea what the graphic card is it
		return 0;
	}

	void	OpenGLDeleteTexture(cTexture*e_pSimpleGLTexture)
	{
		GLuint	l_iImageIndex = e_pSimpleGLTexture->GetImageIndex();
		if (l_iImageIndex == -1)
			return;
	#ifdef DEBUG
		int	l_iBeforeAlivableVRamKB = CheckRestVRam();
	#endif
		glDeleteTextures(1, &l_iImageIndex);
	#ifdef DEBUG
		int	l_iAfterAlivableVRamKB = CheckRestVRam();
		int	l_iDiffErentKB = l_iBeforeAlivableVRamKB - l_iAfterAlivableVRamKB;
		int	l_iAteVRamKB = e_pSimpleGLTexture->GetWidth()*e_pSimpleGLTexture->GetHeight()*(e_pSimpleGLTexture->GetPixelFormat() == GL_RGBA ? 4 : 3) / 1024;
		g_iAteVideoMomory -= l_iAteVRamKB;
		//g_iAteVideoMomory += l_iValue;
		float	l_fMB = l_iAteVRamKB / 1024.f;
		float	l_fTotalMB = g_iAteVideoMomory / 1024.f;
		std::wstring	l_str = UT::ComposeMsgByFormat(L"-------------DeleteTexture-------------\nTextureName:%ls:\t\t\t%.2fMB,\nVideoMomory Use:%.2fMB\nRestVRam:%.2f\n", e_pSimpleGLTexture->GetName(), l_fMB, l_fTotalMB, l_iAfterAlivableVRamKB / 1024.f);
		FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
	#endif
	}
	void OpenGLTextureGenerate(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels, const wchar_t*e_strFileName)
	{
	#ifdef DEBUG
		int	l_iBeforeAlivableVRamKB = CheckRestVRam();
	#endif
		glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
	#ifdef DEBUG
		int	l_iAfterAlivableVRamKB = CheckRestVRam();
		int	l_iDiffErentKB = l_iBeforeAlivableVRamKB - l_iAfterAlivableVRamKB;
		int	l_iAteVRamKB = (width*height*(format == GL_RGBA ? 4 : 3)) / 1024;
		g_iAteVideoMomory += l_iAteVRamKB;
		//g_iAteVideoMomory += l_iValue;
		float	l_fMB = l_iAteVRamKB / 1024.f;
		float	l_fTotalMB = g_iAteVideoMomory / 1024.f;
		MyGlErrorTest("MyTextureGenerate");
		std::wstring	l_str = UT::ComposeMsgByFormat(L"-------------GenerateTexture-------------\nTextureName:%ls:\t\t\t%.2fMB,\nVideoMomory Use:%.2fMB\nRestVRam:%.2f\n-------------\n",e_strFileName,l_fMB,l_fTotalMB,l_iAfterAlivableVRamKB/1024.f);
		FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
	#endif
	}
	//here si the idea for store screen buffer to 4 block
	//http://www.gamedev.net/community/forums/topic.asp?topic_id=489893
	//here we divided screen to 4 parts
	//       
	//  |---|---|
	//  | 1 | 2 |
	//  |---|---|
	//  | 3 | 4 |
	//  |---|---|



	//Lord_Evil, you talk about GL_PACK as if the alignment was its only parameter. There are many parameters you can adjust,
	//and alignment is indeed one of them, but I'm talking about adjusting row skip, pixel skip and row length.
	//
	//GLubyte *img = new GLubyte[1280*1024*4];
	//
	//glPixelStorei(GL_PACK_ROW_LENGTH, 1280);
	//
	//// Block 1
	//glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
	//glPixelStorei(GL_PACK_SKIP_ROWS, 512);
	//glReadPixels(0, 512, 640, 512, GL_RGBA, GL_UNSIGNED_BYTE, img);
	//
	//// Block 2
	//glPixelStorei(GL_PACK_SKIP_PIXELS, 640);
	//glPixelStorei(GL_PACK_SKIP_ROWS, 512);
	//glReadPixels(640, 512, 640, 512, GL_RGBA, GL_UNSIGNED_BYTE, img);
	//
	//// Block 3
	//glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
	//glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	//glReadPixels(0, 0, 640, 512, GL_RGBA, GL_UNSIGNED_BYTE, img);
	//
	//// Block 4
	//glPixelStorei(GL_PACK_SKIP_PIXELS, 640);
	//glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	//glReadPixels(640, 0, 640, 512, GL_RGBA, GL_UNSIGNED_BYTE, img);


	//=====================
	//simple method to store the specific area to buffer
	//x, y, width, and height indicate the area of the screen you want to capture,
	//where x and y are the lower left of the rectangle, 
	//and width and height are the dimensions. 
	//If you want to read the whole screen, just set both x and y to 0, and width and height to the dimensions of the screen.

	//def readScreen(x, y, width, height):
	//    """ Read in the screen information in the area specified """
	//    glFinish()
	//    glPixelStorei(GL_PACK_ALIGNMENT, 4)
	//    glPixelStorei(GL_PACK_ROW_LENGTH, 0)
	//    glPixelStorei(GL_PACK_SKIP_ROWS, 0)
	//    glPixelStorei(GL_PACK_SKIP_PIXELS, 0)
	//
	//    data = glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE)
	//
	//    return data
	//

	//=====================






	//==============
	//http://ubuntuforums.org/showthread.php?t=396281
	//wybiral said:
	//when u loading a image with alpha u 'd call below beofre apply drawing
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.5);


	//http://www.gamedev.net/community/forums/topic.asp?topic_id=452780&forum_id=25&gforum_id=0
	//gluBuild2DMipmaps is very slow (+ buggy)
	//
	//if a texture is not power of 2 sized then gluBuild2DMipmaps will first resize it so it is, i assume thats where the problem lies somehow.
	//
	//to artifically see if mipmapping is working draw the texture with a high texture_lod_bias
	//perhaps draw the texture on a fullscreen sized polygon
	//+ then with a keypress cycle through the mipmap levels


	//If you want to use the GL_GENERATE_MIPMAP you don't use gluBuild2DMipmaps() you use glTexImage2D() and call the correct glTexParameteri() with true in it.
	//
	//The above method works, and will automatically generate mipmaps for your dynamic textures for all levels above base level.
	//
	//With GenerateMipmapEXT() it will generate all mipmaps above base also but when you choose to not the driver.
	//==============
	//glCopyTexSubImage2D

	//- ()saveImage:(UIImage*)image:(NSString*)imageName
	//{ 
	//    NSData *imageData = UIImagePNGRepresentation(image); //convert image into .png format.     
	//    NSFileManager *fileManager = [NSFileManager defaultManager];//create instance of NSFileManager     
	//    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES); //create an array and store result of our search for the documents directory in it     
	//    NSString *documentsDirectory = [paths objectAtIndex:0]; //create NSString object, that holds our exact path to the documents directory     
	//    NSString *fullPath = [documentsDirectory stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.png", imageName]]; //add our image to the path     
	//    [fileManager createFileAtPath:fullPath contents:imageData attributes:nil]; //finally save the path (image)
	//    NSLog(@"image saved");
	//}
	// 
	////removing an image
	// 
	//- ()removeImage:(NSString*)fileName {
	// 
	//NSFileManager *fileManager = [NSFileManager defaultManager];
	// 
	//NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	// 
	//NSString *documentsDirectory = [paths objectAtIndex:0];
	// 
	//NSString *fullPath = [documentsDirectory stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.png", fileName]];
	// 
	//[fileManager removeItemAtPath: fullPath error:nullptr];
	// 
	//NSLog(@"image removed");
	// 
	//}
	// 
	////loading an image
	// 
	//- (UIImage*)loadImage:(NSString*)imageName {
	// 
	//NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	// 
	//NSString *documentsDirectory = [paths objectAtIndex:0];
	// 
	//NSString *fullPath = [documentsDirectory stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.png", imageName]];
	// 
	//return [UIImage imageWithContentsOfFile:fullPath];
	// 
	//Now, you can easily save an image like:
	//
	//1
	//[self saveImage: myUIImage: @"myUIImageName"];
	//or load it like:
	//
	//1
	//myUIImage = [self loadImage: @"myUIImageName"];
	//or remove it like:
	//
	//1
	//[self removeImage: @"myUIImageName"

	bool	SaveCurrentBufferToImage(const char*e_strFileName, int e_iViewPortWidth, int e_iViewPortHeight)
	{
		bool l_bResult = true;
#ifndef IOS
		int l_iNumChannel = 4;//3 androd only support 4 channel...
		GLenum l_Format = GL_RGBA;//GL_RGB
		int l_iWidth = e_iViewPortWidth;
		int l_iHeight = e_iViewPortHeight;
		unsigned char *l_pPixelData = new unsigned char[l_iWidth*l_iHeight*l_iNumChannel];
		unsigned char *l_pPixelData2 = new unsigned char[l_iWidth*l_iNumChannel];
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(0, 0, l_iWidth, (GLsizei)l_iHeight, l_Format, GL_UNSIGNED_BYTE, l_pPixelData);
		int l_iHalfHeight = l_iHeight / 2;
		for (int i = 0; i<l_iHalfHeight; ++i)
		{
			int l_iIndex1 = l_iNumChannel * l_iWidth*(l_iHeight - i - 1);
			int l_iIndex2 = l_iNumChannel * l_iWidth*i;
			memcpy(l_pPixelData2, &l_pPixelData[l_iIndex1], sizeof(unsigned char)*l_iNumChannel*l_iWidth);
			memcpy(&l_pPixelData[l_iIndex1], &l_pPixelData[l_iIndex2], sizeof(unsigned char)*l_iNumChannel*l_iWidth);
			memcpy(&l_pPixelData[l_iIndex2], l_pPixelData2, sizeof(unsigned char)*l_iNumChannel*l_iWidth);
		}
		l_bResult = SaveBufferToImage(e_strFileName, l_iWidth, l_iHeight, l_pPixelData, l_iNumChannel);
		delete[] l_pPixelData;
		delete[] l_pPixelData2;
#elif defined(IOS)//for iphone,save into album
		captureToPhotoAlbum();
#endif
		return l_bResult;
	}

	bool	SaveBufferToImage(const char*e_strFileName, int e_iWidth, int e_iHeight, unsigned char*e_pPixel, int e_iChannel)
	{
		std::string l_strExtensionName = GetFileExtensionName(e_strFileName);
		
#if defined(ANDROID)
		//I donno why sometimes android just cannt write file.(file size is 0)
		int l_iTotalIntSize = e_iWidth*e_iHeight;//channel is 4.4 byte is 1 int
		jstring l_strFileName = cGameApp::m_spThreadEnv->NewStringUTF(e_strFileName);
		EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
		jintArray l_Array = cGameApp::m_spThreadEnv->NewIntArray(l_iTotalIntSize);
		EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
		cGameApp::m_spThreadEnv->SetIntArrayRegion(l_Array, 0, l_iTotalIntSize, reinterpret_cast<jint*>(e_pPixel));
		EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
		jstring strClassName = cGameApp::m_spThreadEnv->NewStringUTF("util/MyBitmap");
		EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
		jclass l_ClassIWant = (jclass)cGameApp::m_spThreadEnv->CallObjectMethod(g_pMainThreadJNIUtilData->cls, g_pMainThreadJNIUtilData->findClass, strClassName);
		EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
		cGameApp::m_spThreadEnv->DeleteLocalRef(strClassName);
		EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
		jmethodID l_Method = cGameApp::m_spThreadEnv->GetStaticMethodID(l_ClassIWant, "JpegToFile", "([IIILjava/lang/String;)V");
		EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
		cGameApp::m_spThreadEnv->CallStaticVoidMethod(l_ClassIWant, l_Method, l_Array, e_iWidth, e_iHeight, l_strFileName);
		EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
		cGameApp::m_spThreadEnv->DeleteLocalRef(l_ClassIWant);
		EXCEPTION_RETURN(cGameApp::m_spThreadEnv);
#elif defined(WIN32)
		if (l_strExtensionName.compare("png") == 0 || l_strExtensionName.compare("PNG") == 0)
		{
			//8bit?
			lodepng_encode_file(e_strFileName, e_pPixel, e_iWidth,e_iHeight, e_iChannel==3?LodePNGColorType::LCT_RGB: LodePNGColorType ::LCT_RGBA,8);
		}
		else
			jpge::compress_image_to_jpeg_file(e_strFileName, e_iWidth, e_iHeight, e_iChannel, e_pPixel);
#elif defined(IOS)
		captureToPhotoAlbum();
#endif
		return true;
	}
//end namespace FATMING_CORE
}