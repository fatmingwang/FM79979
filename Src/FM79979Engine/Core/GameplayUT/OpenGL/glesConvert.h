#pragma once

#if defined WIN32 && !defined(UWP)
	#ifndef GL_COLOR_ATTACHMENT0
	#define GL_COLOR_ATTACHMENT0                            GL_COLOR_ATTACHMENT0_EXT
	#endif
	#ifndef GL_DEPTH_ATTACHMENT
	#define GL_DEPTH_ATTACHMENT                             GL_DEPTH_ATTACHMENT_EXT
	#endif
	#ifndef GL_FRAMEBUFFER_COMPLETE
	#define GL_FRAMEBUFFER_COMPLETE                         GL_FRAMEBUFFER_COMPLETE_EXT
	#endif
	#ifndef GL_FRAMEBUFFER_UNSUPPORTED
	#define GL_FRAMEBUFFER_UNSUPPORTED                      GL_FRAMEBUFFER_UNSUPPORTED_EXT
	#endif
	#ifndef GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT
	#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT    GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT
	#endif
	#ifndef GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT
	#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT            GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT
	#endif
	#ifndef GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
	#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS            GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
	#endif
	#ifndef GL_FRAMEBUFFER
	#define GL_FRAMEBUFFER                                  GL_FRAMEBUFFER_EXT
	#endif
	#ifndef GL_RENDERBUFFER
	#define GL_RENDERBUFFER                                 GL_RENDERBUFFER_EXT
	#endif
	#ifndef glGenFramebuffers
	#define glGenFramebuffers                               glGenFramebuffersEXT
	#endif
	#ifndef glBindFramebuffer
	#define glBindFramebuffer                               glBindFramebufferEXT
	#endif
	#ifndef glBindRenderbuffer
	#define glBindRenderbuffer                              glBindRenderbufferEXT
	#endif
	#ifndef glCheckFramebufferStatus
	#define glCheckFramebufferStatus                        glCheckFramebufferStatusEXT
	#endif
	#ifndef glFramebufferRenderbuffer
	#define glFramebufferRenderbuffer                       glFramebufferRenderbufferEXT
	#endif
	#ifndef glFramebufferTexture2D
	#define glFramebufferTexture2D                          glFramebufferTexture2DEXT
	#endif
	#ifndef glGenRenderbuffers
	#define glGenRenderbuffers                              glGenRenderbuffersEXT
	#endif
	#ifndef glRenderbufferStorage
	#define glRenderbufferStorage                           glRenderbufferStorageEXT
	#endif
	#ifndef glDeleteFramebuffers
	#define glDeleteFramebuffers                            glDeleteFramebuffersEXT
	#endif
	#ifndef glDeleteRenderbuffers
	#define glDeleteRenderbuffers                           glDeleteRenderbuffersEXT
	#endif
	#ifndef glClearDepthf
	#define glClearDepthf                                   glClearDepth
	#endif
	#ifndef GL_RGBA32F
	#define GL_RGBA32F										GL_RGBA32F_ARB
	#endif
	#ifndef GL_RGB32F
	#define GL_RGB32F										GL_RGB32F_ARB
	#endif
	#ifndef GL_RGBA16F
	#define GL_RGBA16F										GL_RGBA16F_ARB
	#endif
	#ifndef GL_RGB16F
	#define GL_RGB16F										GL_RGB16F_ARB
	#endif
#else
	#ifndef glClearDepth
	#define glClearDepth                                   glClearDepthf
	#endif
#endif

#ifndef GL_BLEND_SRC
#define GL_BLEND_SRC GL_BLEND_SRC_RGB
#endif
#ifndef GL_BLEND_DST
#define GL_BLEND_DST GL_BLEND_DST_RGB
#endif