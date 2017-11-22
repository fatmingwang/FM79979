//#if defined WIN32 || defined OPENGLES_2_X//or iPhone??
#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

namespace FATMING_CORE
{
	extern wchar_t*DEFAULT_SHADER;
//==========================
//if there is no framebuffer exist...
//==========================
#if defined(WIN32) || defined(LINUX)
	class	cScreenCapture:public Frame
	{
		char*m_pPixelBuffer;
		GLuint	m_uiWidth;
		GLuint	m_uiHeight;
		GLuint	m_uiTextureID;
	public:
		cScreenCapture();
		virtual ~cScreenCapture();
		void	Capture(int*e_piViewport);
		void	Render(Vector3 e_vPos,int e_iWidth,int e_iHeight);
	};
#endif
//=====================
//
//=====================
	class	cFrameBuffer:public Frame
	{
		GLboolean	m_bEnableScissor;
		GLint		m_iOriginalScissortSize[4];
		GLint		m_iOriginalViewPortSize[4];
		GLuint		m_uiFramebufferID;	//id for frame buffer
		GLuint		m_uiTextureID;		//where to render
		GLuint		m_uiRenderufferID;	//for depth
		UINT		m_uiWidth;			//
		UINT		m_uiHeight;			//
		bool		m_bDepthNeed;		//2d may do not need this
		GLenum		m_eImageType;		//GL_RGB or GL_RGBA
		GLenum		m_eRGBDataType;		//GL_UNSIGNED_BYTE,GL_FLOAT
	public:
		cFrameBuffer(int e_iWidth,int e_iHeight,bool e_bDepthNeed = false,GLenum e_eImageType = GL_RGB,GLenum e_eRGBDataType = GL_UNSIGNED_BYTE);//GL_RGB32F,GL_RGB16F
		virtual ~cFrameBuffer();
		void	StartDraw(bool e_bClearScreen = true);		//store original
		void	EndDraw();			//restore original
		UINT	GetWidth(){ return m_uiWidth; }
		UINT	GetHeight(){ return m_uiHeight; }
		void	DrawBuffer(POINT e_Pos,POINT e_Size,const WCHAR*e_strShaderName = DEFAULT_SHADER);
		GLuint	GetTextureID(){return m_uiTextureID;}
	};
	//

	class	cMSAAFrameBuffer :public Frame
	{
		GLboolean	m_bEnableScissor;
		GLint		m_iOriginalScissortSize[4];
		GLint		m_iOriginalViewPortSize[4];
		GLuint		m_uiFramebufferID;	//id for frame buffer
		GLuint		m_uiTextureID;		//where to render
		GLuint		m_uiRenderufferID;	//for depth
		UINT		m_uiWidth;			//
		UINT		m_uiHeight;			//
		GLenum		m_eImageType;		//GL_RGB or GL_RGBA
		GLenum		m_eRGBDataType;		//GL_UNSIGNED_BYTE,GL_FLOAT
	public:
		cMSAAFrameBuffer(int e_iWidth, int e_iHeight, GLenum e_eImageType = GL_RGB, GLenum e_eRGBDataType = GL_UNSIGNED_BYTE);//GL_RGB32F,GL_RGB16F
		virtual ~cMSAAFrameBuffer();
		void	StartDraw(bool e_bClearScreen = true);		//store original
		void	EndDraw();			//restore original
		UINT	GetWidth() { return m_uiWidth; }
		UINT	GetHeight() { return m_uiHeight; }
		void	DrawBuffer(POINT e_Pos, POINT e_Size, const WCHAR*e_strShaderName = DEFAULT_SHADER);
		GLuint	GetTextureID() { return m_uiTextureID; }
	};

	//// Your code to draw content to the FBO 
	//// ... 
	//// Make the window the target 
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); 
	////Your code to use the contents of the FBO 
	//// ... 

	//
	//
	//
	//
	//
	//Setting up a renderbuffer for drawing images
	//
	//GLuint framebuffer, renderbuffer; 
	//GLenum status; 
	//// Set the width and height appropriately for you image 
	//GLuint texWidth = 1024, 
	//       texHeight = 1024; 
	////Set up a FBO with one renderbuffer attachment 
	//glGenFramebuffersEXT(1, &framebuffer); 
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer); 

	//glGenRenderbuffersEXT(1, &renderbuffer); 
	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderbuffer); 
	//glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA8, texWidth, texHeight); 

	//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, 
	//                 GL_RENDERBUFFER_EXT, renderbuffer); 

	//status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT); 
	//if (status != GL_FRAMEBUFFER_COMPLETE_EXT) 
	//                // Handle errors 
	////Your code to draw content to the renderbuffer 
	//// ... 
	//// Make the window the target 
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); 
	////Your code to use the contents 
	//// ... 
	//// Delete the renderbuffer attachment 
	//glDeleteRenderbuffersEXT(1, &renderbuffer); 

}
//end FRAME_BUFFER_H
#endif
//end OPENGLES_2_X
//#endif