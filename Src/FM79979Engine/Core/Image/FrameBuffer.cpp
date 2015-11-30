#include "../stdafx.h"
#if defined WIN32 || defined OPENGLES_2_X//or iPhone??
#include "FrameBuffer.h"
#include "SimplePrimitive.h"
#include "../GLSL/Shader.h"
#include "../GameplayUT/GameApp.h"
namespace FATMING_CORE
{
#if defined(WIN32) || defined(LINUX)
	cScreenCapture::cScreenCapture()
	{
		m_pPixelBuffer = 0; 
		m_uiWidth = -1;
		m_uiHeight = -1;
		glGenTextures(1, &m_uiTextureID);
		//give a super big size
		m_pPixelBuffer = new char[1920*1080*3];
	}
	cScreenCapture::~cScreenCapture()
	{
		glDeleteTextures(1,&m_uiTextureID);
		SAFE_DELETE(m_pPixelBuffer);
	}
	void	cScreenCapture::Capture(int*e_piViewport)
	{
		if( m_pPixelBuffer == 0 || m_uiWidth != e_piViewport[2] || m_uiHeight != e_piViewport[3] )
		{
			m_uiWidth = e_piViewport[2];
			m_uiHeight = e_piViewport[3];
			if(!g_bSupportNonPowerOfTwoTexture)
			{
				m_uiWidth = UT::power_of_two(m_uiWidth);
				m_uiHeight = UT::power_of_two(m_uiHeight);
			}
			//SAFE_DELETE(m_pPixelBuffer);
			//m_pPixelBuffer = new char[m_uiHeight*m_uiWidth*3];
		}
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadBuffer(GL_BACK);
		glReadPixels(e_piViewport[0],e_piViewport[1],m_uiWidth,m_uiHeight,GL_RGB, GL_UNSIGNED_BYTE,m_pPixelBuffer);
		glBindTexture(GL_TEXTURE_2D, m_uiTextureID);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);	// Set Texture Max Filter
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);	// Set Texture Min Filter
		glTexImage2D(GL_TEXTURE_2D, 0,3,m_uiWidth,m_uiHeight, 0,GL_RGB,GL_UNSIGNED_BYTE,m_pPixelBuffer);
	}

	void	cScreenCapture::Render(Vector3 e_vPos,int e_iWidth,int e_iHeight)
	{
		if( m_uiWidth == -1 )
			return;
		glBindTexture( GL_TEXTURE_2D, m_uiTextureID);
		e_iWidth /= 2;
		e_iHeight /= 2;

		float	l_fTexPointer[] = {  0,1,
									 1,1,
									 0,0,
									 1,0};
		
		float	l_Vertices[] = { (float)-e_iWidth,(float)-e_iHeight,0,
								 (float)e_iWidth, (float)-e_iHeight,0,
								 (float)-e_iWidth, (float)e_iHeight,0,
								 (float)e_iWidth,(float)e_iHeight,0};

		cMatrix44	l_mat = cMatrix44::TranslationMatrix(Vector3((float)(e_vPos.x+e_iWidth),(float)(e_vPos.y+e_iHeight), e_vPos.z));
		SetupShaderWorldMatrix(l_mat*GetWorldTransform());
		myGlVertexPointer(3,l_Vertices);
		myGlUVPointer(2,l_fTexPointer);
		ASSIGN_2D_COLOR(Vector4::One);
		MY_GLDRAW_ARRAYS(GL_TRIANGLE_STRIP, 0, 4);

	}
#endif	
	//CHECK_FRAMEBUFFER_STATUS() 
	//{ 
	//	printf("Checking framebuffer status.\n");
	//	GLenum status; 
	//	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER); 
	//	switch(status) { 
	//		case GL_FRAMEBUFFER_COMPLETE: 
	//		printf("Framebuffer complete.\n");
	//		break; 
	//		case GL_FRAMEBUFFER_UNSUPPORTED:
	//		printf("Framebuffer unsupported.\n");
	//		/* choose different formats */ 
	//		break; 
	//		default: 
	//		/* programming error; will fail on all hardware */ 
	//		assert(0); 
	//	}
	//}

	bool CheckFramebufferStatus( bool silent = false)
	{
		GLenum status;
		status = (GLenum) glCheckFramebufferStatus(GL_FRAMEBUFFER);
		switch(status) {
			case GL_FRAMEBUFFER_COMPLETE:
				break;
			case GL_FRAMEBUFFER_UNSUPPORTED:
				if (!silent) printf("Unsupported framebuffer format\n");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				if (!silent) printf("Framebuffer incomplete, missing attachment\n");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				if (!silent) printf("Framebuffer incomplete, duplicate attachment\n");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
				if (!silent) printf("Framebuffer incomplete, attached images must have same dimensions\n");
				return false;
#if defined(WIN32) || defined(LINUX)
			case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
				if (!silent) printf("Framebuffer incomplete, attached images must have same format\n");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
				if (!silent) printf("Framebuffer incomplete, missing draw buffer\n");
				return false;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
				if (!silent) printf("Framebuffer incomplete, missing read buffer\n");
				return false;
#endif
			default:
				assert(0);
				return false;
		}
		return true;
	}

	cFrameBuffer::cFrameBuffer(int e_iWidth,int e_iHeight,bool e_bDepthNeed)
	{
	//Setting up a framebuffer for texturing
	//generate mipmap
	//glGenTextures(1, &img);
	//glBindTexture(GL_TEXTURE_2D, img);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glGenerateMipmapEXT(GL_TEXTURE_2D);

		m_bDepthNeed = e_bDepthNeed;
		m_uiWidth = e_iWidth;
		m_uiHeight = e_iHeight;
		//in openglES 2.0 glGenFramebuffers
		glGenFramebuffers(1, &m_uiFramebufferID); 
		{//glBindFramebuffer
			// Set up the FBO with one texture attachment 
			//in openglES 2.0 glBindFramebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, m_uiFramebufferID);
			//initialize renderbuffer
			if( m_bDepthNeed )
			{
				glGenRenderbuffers(1, &this->m_uiRenderufferID);
				glBindRenderbuffer(GL_RENDERBUFFER, m_uiRenderufferID);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, e_iWidth, e_iHeight);//GL_DEPTH_COMPONENT24
				//bind  render buffer with texture
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
								 GL_RENDERBUFFER, m_uiRenderufferID);
			}
			// Now setup a texture to render to
			glGenTextures(1, &m_uiTextureID);
			glBindTexture(GL_TEXTURE_2D, m_uiTextureID); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, e_iWidth, e_iHeight, 0,
							GL_RGB, GL_UNSIGNED_BYTE, nullptr);
			//  The following 3 lines enable mipmap filtering and generate the mipmap data so rendering works
			//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			//	glGenerateMipmapEXT(GL_TEXTURE_2D);
			//assign texture to framebuffer
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_2D, m_uiTextureID, 0);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		static bool	l_bCheck = false;
		if( !l_bCheck )
		{
			l_bCheck = true;
			bool	l_b = CheckFramebufferStatus();
			assert(l_b&&"display card is too old!!");
		}
	}

	cFrameBuffer::~cFrameBuffer()
	{
		//Tear down the FBO and texture attachment 
		glDeleteTextures(1, &m_uiTextureID); 
		glDeleteFramebuffers(1, &m_uiFramebufferID); 
		if( m_bDepthNeed )
			glDeleteRenderbuffers(1, &m_uiRenderufferID);


	}

	void	cFrameBuffer::StartDraw(bool e_bClearScreen)
	{
		//for depth
		glGetIntegerv(GL_VIEWPORT,m_iOriginalViewPortSize);
		cGameApp::m_svViewPortSize.x = 0.f;
		cGameApp::m_svViewPortSize.y = 0.f;
		cGameApp::m_svViewPortSize.z = (float)this->m_uiWidth;
		cGameApp::m_svViewPortSize.w = (float)this->m_uiHeight;
		glBindFramebuffer(GL_FRAMEBUFFER, m_uiFramebufferID);
		if( e_bClearScreen )
		{
			glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
			//glClearColor( 0,0.5,0.5,0.f );
			glClearColor( 0,0,0,1 );
		}
		//for RGB
		//glBindRenderbufferEXT(GL_RENDERBUFFER, m_RenderufferID);
//		glPushAttrib(GL_VIEWPORT_BIT|GL_COLOR_BUFFER_BIT);
		glViewport(0,0,this->m_uiWidth,this->m_uiHeight);

		// Set the render target
		//glDrawBuffer(GL_COLOR_ATTACHMENT0);
		// Render as normal here
		// output goes to the FBO and itís attached buffers
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index,
		//                          newTarget, texture.getID(), 0);

	}

	void	cFrameBuffer::EndDraw()
	{
//		glPopAttrib();
		//glViewport(m_OriginalViewPortSize[0],m_OriginalViewPortSize[1],m_OriginalViewPortSize[2],m_OriginalViewPortSize[3]);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		cGameApp::m_svViewPortSize.x = (float)m_iOriginalViewPortSize[0];
		cGameApp::m_svViewPortSize.y = (float)m_iOriginalViewPortSize[1];
		cGameApp::m_svViewPortSize.z = (float)m_iOriginalViewPortSize[2];
		cGameApp::m_svViewPortSize.w = (float)m_iOriginalViewPortSize[3];
		glViewport((int)cGameApp::m_svViewPortSize.x,(int)cGameApp::m_svViewPortSize.y,(int)cGameApp::m_svViewPortSize.z,(int)cGameApp::m_svViewPortSize.w);
	}

	void	cFrameBuffer::DrawBuffer(POINT e_Pos,POINT e_Size)
	{
		glBindTexture( GL_TEXTURE_2D, m_uiTextureID);
		//I have no idea why the frame buffer have to rotate Y with180,and change UV
		//real freak........
		//int e_iWidth = e_Size.x/2;
		//int e_iHeight = e_Size.y/2;
		//float	l_fTexPointer[] = {  1,0,
		//							 0,0,
		//							 1,1,
		//							 0,1};
		//
		////float	l_Vertices[] = { (float)-e_iWidth,(float)-e_iHeight,0,
		////						 (float)e_iWidth, (float)-e_iHeight,0,
		////						 (float)-e_iWidth, (float)e_iHeight,0,
		////						 (float)e_iWidth,(float)e_iHeight,0};

		//cMatrix44	l_mat = cMatrix44::TranslationMatrix(Vector3((float)(e_Pos.x+e_iWidth),(float)(e_Pos.y+e_iHeight), 0))*  cMatrix44::ZAxisRotationMatrix(D3DXToRadian(180.f));
		//SetupShaderWorldMatrix(l_mat*GetWorldTransform());
		//ASSIGN_2D_COLOR(Vector4::One);
		////myGlVertexPointer( 2, l_Vertices );
		//ASSIGN_2D_VerticesBySize(e_iWidth,e_iHeight,0.f);
		//myGlUVPointer(  2, l_fTexPointer );
		//MY_GLDRAW_ARRAYS(GL_TRIANGLE_STRIP, 0, 4);
		//UseShaderProgram(l_p2DShader);
		float	l_fTextureCoordinate[] ={0,1,1,0};
		DrawQuadWithTextureAndColorAndCoordinate((float)e_Pos.x,(float)e_Pos.y,0.f,(float)e_Size.x,(float)e_Size.y,Vector4::One,l_fTextureCoordinate,Vector3::Zero);
	}
















	//
	//
	//
	//
	//
	//
	//#ifndef FRAME_BUFFER_OBJECT_H
	//#define FRAME_BUFFER_OBJECT_H
	//
	//#include "../Appearance/Texture.h"
	//#include "../Tools/Logger.h"
	//
	//class FrameBufferObject
	//{
	//  private:
	//    GLuint stencilBufferID,
	//           depthBufferID,
	//           frameBufferID,
	//           height,
	//           width;
	//
	//    bool   sharedStencilBuffer,
	//           sharedDepthBuffer;
	//
	//  public:
	//     FrameBufferObject();
	//    ~FrameBufferObject();
	//
	//     bool initialize(Texture &texture, 
	//                     GLuint   sharedStencilBufferID = 0,
	//                     GLuint   sharedDepthBufferID   = 0,
	//                     bool     depthOnly             = false);
	//     void attachRenderTarget(Texture &texture, int index = 0, int newTarget = -1);
	//     void detachRenderTarget(Texture &texture, int index = 0, int newTarget = -1);
	//
	//     static const void deactivate();
	//     void   activate();
	//
	//     const GLuint getStencilBufferID() const;
	//     const GLuint getDepthBufferID()   const;
	//     const GLuint getFrameBufferID()   const;
	//     const GLuint getHeight()          const;
	//     const GLuint getWidth()           const;
	//
	//     static  const bool  checkStatus();
	//};
	//#endif
	//
	//
	//
	//
	//
	//
	//#include "FrameBufferObject.h" 
	//
	//FrameBufferObject::FrameBufferObject()
	//{
	//  sharedStencilBuffer = false;
	//  sharedDepthBuffer   = false;
	//  stencilBufferID     =     0;
	//  frameBufferID       =     0;
	//  depthBufferID       =     0;
	//  height              =     0;
	//  width               =     0;
	//}
	//
	//bool  FrameBufferObject::initialize(Texture &texture, 
	//                                    GLuint   sharedStencilBufferID,
	//                                    GLuint   sharedDepthBufferID,
	//                                    bool     depthOnly)
	//{
	//  if(frameBufferID)
	//    return Logger::writeErrorLog("FrameBufferObject already initialized!");
	// 
	//  if(texture.getWidth() <= 0 || texture.getHeight() <= 0)
	//    return Logger::writeErrorLog("Width and Height of FBO must be positive, non-zero");
	//
	//  if(texture.getTarget() != GL_TEXTURE_2D           &&
	//     texture.getTarget() != GL_TEXTURE_CUBE_MAP_ARB &&
	//     texture.getTarget() != GL_TEXTURE_RECTANGLE_ARB)
	//    return Logger::writeErrorLog("Current FBO implementation only supports 2D/RECT/CUBE textures");
	//  
	//  if(!texture.getID())
	//    return Logger::writeErrorLog("FBO need a valid Texture ID");
	//
	//  sharedStencilBuffer = (sharedStencilBufferID!= 0);
	//  sharedDepthBuffer   = (sharedDepthBufferID  != 0);
	//
	//  height              = texture.getHeight();
	//  width               = texture.getWidth();
	//
	//  glGenFramebuffersEXT(1, &frameBufferID);
	//  glBindFramebufferEXT(GL_FRAMEBUFFER, frameBufferID);
	//
	//  if(!sharedDepthBufferID && !depthOnly)
	//  {
	//    glGenRenderbuffersEXT(1, &depthBufferID);
	//    glBindRenderbufferEXT(GL_RENDERBUFFER, depthBufferID);
	//    glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	//    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
	//                                 GL_RENDERBUFFER, depthBufferID);
	//  }
	//  
	//  if(sharedDepthBufferID)
	//    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
	//                                 GL_RENDERBUFFER, sharedDepthBufferID);
	//
	//  if(!sharedStencilBufferID)
	//  {
	//    glGenRenderbuffersEXT(1, &stencilBufferID);
	//    glBindRenderbufferEXT(GL_RENDERBUFFER, stencilBufferID);
	//    glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_STENCIL_INDEX, width, height);
	//    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
	//                                 GL_RENDERBUFFER, stencilBufferID);
	//  }
	//  else
	//    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
	//                                 GL_RENDERBUFFER, sharedStencilBufferID);
	//
	//
	//  if((texture.getTarget() == GL_TEXTURE_RECTANGLE_ARB) ||
	//     (texture.getTarget() == GL_TEXTURE_2D))
	//  {
	//    glFramebufferTexture2DEXT(GL_FRAMEBUFFER,
	//                              depthOnly ? GL_DEPTH_ATTACHMENT : GL_COLOR_ATTACHMENT0,
	//                              texture.getTarget(), 
	//                              texture.getID(),
	//                              0);
	//    if(depthOnly)
	//    {
	//      glDrawBuffer(GL_NONE);
	//      glReadBuffer(GL_NONE); 
	//    }
	//  }
	//  else
	//  {
	//    glFramebufferTexture2DEXT(GL_FRAMEBUFFER,
	//                              GL_COLOR_ATTACHMENT0,
	//                              GL_TEXTURE_CUBE_MAP_POSITIVE_X, 
	//                              texture.getID(),
	//                              0);
	//
	//  } 
	//  bool result = FrameBufferObject::checkStatus();
	//  glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	//  return result;
	//}
	//
	///***************************************************************************************/
	///*                                                                                     */
	///*This function will report the status of the currently bound FBO                      */
	///*                                                                                     */
	///***************************************************************************************/
	//const bool FrameBufferObject::checkStatus()                            
	//{     
	//
	//  GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);  
	//
	//  //Our FBO is perfect, return true
	//  if(status == GL_FRAMEBUFFER_COMPLETE)
	//    return true;
	//
	//  switch(status) 
	//  {                                          
	//    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT: 
	//      return Logger::writeErrorLog("FBO has one or several image attachments with different internal formats");
	// 
	//    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT: 
	//      return Logger::writeErrorLog("FBO has one or several image attachments with different dimensions");
	//
	//    case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT: 
	//      return Logger::writeErrorLog("FBO has a duplicate image attachment");
	//
	//    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT: 
	//      return Logger::writeErrorLog("FBO missing an image attachment");
	//
	//    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:                   
	//      return Logger::writeErrorLog("FBO format unsupported");
	//  }
	//  return Logger::writeErrorLog("Unknown FBO error");                                       
	//}
	//
	//void FrameBufferObject::attachRenderTarget(Texture &texture, int index, int newTarget)
	//{
	//  newTarget = (newTarget == -1) ? texture.getTarget() : newTarget;
	//  index     = clamp(index, 0, 15);
	//
	//  if(frameBufferID)
	//    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + index,
	//                              newTarget, texture.getID(), 0);
	//  else
	//    Logger::writeErrorLog("Invalid FrameBufferObject index");
	//}
	//
	//void FrameBufferObject::detachRenderTarget(Texture &texture, int index, int newTarget)
	//{
	//  newTarget = (newTarget == -1) ? texture.getTarget() : newTarget;
	//  index     = clamp(index, 0, 15);
	//
	//  if(frameBufferID)
	//    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + index,
	//                              newTarget, 0, 0);
	//  else
	//    Logger::writeErrorLog("Invalid FrameBufferObject index");
	//}
	//
	//void FrameBufferObject::activate()
	//{
	//  if(frameBufferID)
	//    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferID);
	//  else
	//    Logger::writeErrorLog("Invalid FrameBufferObject index");
	//}
	//
	//void const FrameBufferObject::deactivate()
	//{
	//  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	//}
	//
	//const GLuint FrameBufferObject::getStencilBufferID() const { return stencilBufferID; }
	//const GLuint FrameBufferObject::getDepthBufferID()   const { return depthBufferID;   }
	//const GLuint FrameBufferObject::getFrameBufferID()   const { return frameBufferID;   }
	//const GLuint FrameBufferObject::getHeight()          const { return height;          }
	//const GLuint FrameBufferObject::getWidth()           const { return width;           }
	//
	//FrameBufferObject::~FrameBufferObject()
	//{
	//  if(stencilBufferID && !sharedStencilBuffer)
	//    glDeleteRenderbuffersEXT(1, &stencilBufferID);
	//
	//  if(depthBufferID && !sharedDepthBuffer)
	//    glDeleteRenderbuffersEXT(1, &depthBufferID);
	//
	//  if(frameBufferID)
	//    glDeleteFramebuffersEXT(1, &frameBufferID);
	//
	//  sharedStencilBuffer = false;
	//  sharedDepthBuffer   = false;
	//  stencilBufferID     =     0;
	//  frameBufferID       =     0;
	//  depthBufferID       =     0;
	//  height              =     0;
	//  width               =     0;
	//}
	//
	//
}
//end OPENGLES_2_X
#endif