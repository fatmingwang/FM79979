#pragma once

#include "../SmartPointer.h"
namespace FATMING_CORE
{
	class cTextureManager;
	extern bool	g_bSupportNonPowerOfTwoTexture;
	//=======================
	//base image that contain UV and width and height data
	//also contain the pixel data if u want to fetch it.
	//please do not Inheritance it,it's a smart pointer!
	//=======================
	class	cTexture:public cSmartObject
	{
		friend class cTextureManager;
		bool	LoadDDS(const char*e_strFileName);
		cTexture(NamedTypedObject*e_pOwner, const char*e_strImageFileName, bool e_bFetchPixelData = false);
	protected:
		//texture original size.won't be effected by power of two
		int	m_iWidth;
		int	m_iHeight;
		//UV  left,top,right,button,it could possible not be 0,0,1,1 if image is not power of 2.
		float	m_fUV[4];														//left top right buttom.
		//pixels format,GL_RGB,GL_RGBA.
		GLint	m_iPixelFormat;
		GLint	m_iChannel;
		//pixel data.
		GET_SET_DEC(char*,m_pPixels,GetPixels,SetPixels);
		//texture ID.
		GLuint	m_uiImageIndex;
		//check power of two
		void				UpdatePixels(const GLvoid *e_pPixels,bool e_bFetchPuxelData);
	public:
		//last image index has been used
        static  GLuint  m_suiLastUsingImageIndex;
		DEFINE_TYPE_INFO()
		//if e_bCopyPixels is true,u have to ensure the e_pPixels has delete or keep in somewhere u want keep!!
		//!!ensure u know how to use it!
		cTexture(NamedTypedObject*e_pOwner,char*e_pPixels,int e_iWidth,int e_iHeight,const wchar_t*e_strName,bool e_bCopyPixels,bool e_bAllocateMemoryForPixelIfFetch,GLenum e_eImageType = GL_RGBA);
		virtual ~cTexture();
		void	Destroy();

		//it e_bFetchPixelData true m_pPixels will fill of pixel data.
		bool	LoadImage(const char*e_strImageFileName,bool e_bFetchPixelData = false);
		bool	ApplyImage();
		static bool	ApplyImage(GLuint e_TextureID);
		GLuint	GetImageIndex(){ return m_uiImageIndex;}
		//should't call this!!,instead u should call SetWidth and SetHeight.
		void	SetImageSize(int e_iWidth,int e_iHeight){ m_iWidth = e_iWidth; m_iHeight = e_iHeight; }
		int		GetWidth(){ return m_iWidth; }
		int		GetHeight(){ return m_iHeight; }
		GLint	GetPixelFormat(){ return m_iPixelFormat; }
		inline	bool	IsSameTextureID(cTexture*e_pSimpleGLTexture){return e_pSimpleGLTexture->m_uiImageIndex == m_uiImageIndex?true:false;}
		virtual std::wstring DescriptionInfo();
		float*	GetUV();
		std::string*		m_pstrFullFileName;
		//GLAPI void GLAPIENTRY glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_iWidth, m_iHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, m_pDIBData);
		void				SetupTexture(GLint e_iChannel,GLsizei e_iWidth,GLsizei e_iHeight,GLenum e_Format,GLenum e_Type,bool e_bGeneratePixels,const GLvoid *e_pPixels);
		void				SetupTexture(GLint e_iChannel,GLsizei e_iWidth,GLsizei e_iHeight,GLenum e_Format,GLenum e_Type);
		//ensure power of problem is done or it will be trobule
		void				ForceUpdatePixels(const GLvoid *e_pPixels);
		//must delete return data .
		static char*		GeneratePowerOfTwoPixelData(GLint e_iChannel,GLsizei e_iWidth,GLsizei e_iHeight,GLenum e_Format,GLenum e_Type,const GLvoid *e_pPixels);
		static  void		DumpDebugInfo();
	};
	//sample to get pixel by texture ID
	////Use our texture
	//glBindTexture( GL_TEXTURE_2D, m_uiImageIndex );
	//   
	////Get the pixel data
	//glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pPixels );
	//m_iPixelFormat = GL_RGBA;
}