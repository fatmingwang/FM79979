#pragma once

#include "Texture.h"
#include "../../../Common/SmartPointer.h"
#include "../../../Common/Template/TemplateClass.h"

namespace FATMING_CORE
{
	//RGBA not RGB
	unsigned char*	GetScreenPixels(int e_iViewPortWidth, int e_iViewPortHeight);
	bool			SaveCurrentBufferToImage(const char*e_strFileName,int e_iViewPortWidth, int e_iViewPortHeight);
	//e_bDoYCoordianteInvert=>opengl Y up but my is Y down
	bool			SaveBufferToImage(const char*e_strFileName, int e_iWidth, int e_iHeight, unsigned char*e_pPixel, int e_iChannel,bool e_bDoYCoordianteInvert = false);
	void			OpenGLTextureGenerate(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels, const wchar_t*e_strFileName, bool e_bShowLog = true);
	void			OpenGLDeleteTexture(cTexture*e_pSimpleGLTexture);

	class cTexture;
	class cTextureManager:public cNamedTypedObjectVector<cTexture>
	{
		static cTextureManager*m_spTextureManager;
		//
		cTextureManager();
		virtual ~cTextureManager();
	public:
		static cTextureManager *GetInstance();
		void	AddObjectWithDebugInfo(cTexture*e_pTexture);
		void	RemoveObjectWithDebugInfo(cTexture*e_pTexture);
		//using	cNamedTypedObjectVector::GetObject;
		cTexture*	GetObject(NamedTypedObject*e_pOwner, const char*e_strImageFileName, bool e_bFetchPixelData = false);
		//call this if loading is in another thread(not rendering thread)
		void	UpdateOpenGLTextureBecauseMultiThreadLoading();
		static cTexture* GetObjectByPixels(NamedTypedObject*e_pHolder,void*e_pPixelsData,int e_iWidth, int e_iHeight,const wchar_t*e_strName,int e_iDataFormat);
	};
}