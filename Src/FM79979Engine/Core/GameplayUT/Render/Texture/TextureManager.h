#pragma once

#include "Texture.h"
#include "../../../Common/SmartPointer.h"
#include "../../../Common/Template/TemplateClass.h"

namespace FATMING_CORE
{
	bool	SaveCurrentBufferToImage(const char*e_strFileName,int e_iViewPortWidth, int e_iViewPortHeight);
	bool	SaveBufferToImage(const char*e_strFileName, int e_iWidth, int e_iHeight, unsigned char*e_pPixel, int e_iChannel);
	void	OpenGLTextureGenerate(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels, const wchar_t*e_strFileName);
	void	OpenGLDeleteTexture(cTexture*e_pSimpleGLTexture);

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
	};
}