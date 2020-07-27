#pragma once

#if defined(UWP)
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif defined(WIN32)
	#include "../../../Include/glew.h"
	#include "../../../Include/glext.h"
	#include "../../../Include/wglew.h"
#include "../../../Include/wglext.h"
	#include "windows.h"
#elif defined(IOS)
	#include <OpenGLES/ES2/gl.h>
	#include <OpenGLES/ES2/glext.h>
#elif defined(ANDROID)
//https://developer.android.com/ndk/guides/stable_apis.html
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#include <GLES2/gl2platform.h>
	//Android 4.3 (android-18)
	//glShaderSource es 2 and es 3 is different...fuck-_-
	//#include <GLES3/gl3.h>
	//#include <GLES3/gl3ext.h>
#elif defined(LINUX)
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#elif defined(WASM)
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#include <GLES3/gl3.h>
#endif
//lazy include
#include "glesConvert.h"
#include "../../AllMathInclude.h"
#include "../../Common/CommonDefine.h"
#include "../../Common/Utility.h"
#include "../../Common/StringToStructure.h"
#include "../GameUTDefine.h"
//=======================
//the old function for setup matrix is glMultMatrixf
//but for the shader we need to ge tthe martix and setup it
//so those function to fetch matrix to replace old.
//=======================

	void	glhFrustumf2(float *matrix, float left, float right, float bottom, float top,
					  float znear, float zfar);

	void	glhPerspectivef2(float *matrix, float fovyInDegrees, float aspectRatio,
						  float znear, float zfar);

	void	glhOrthof2(float *matrix, float left, float right, float bottom, float top, float znear, float zfar);

	/* projection d	u point (objx,objy,obz) sur l'ecran (winx,winy,winz) */
	GLint 	gluProject(GLfloat objx, GLfloat  objy, GLfloat  objz, const GLfloat  model[16], const GLfloat  proj[16], const GLint viewport[4], GLfloat  * winx, GLfloat  * winy, GLfloat  * winz);
	GLenum	IndexToBlendingIndex(int e_iIndex);
	int		BlendingIndexToIndex(GLenum e_eBlendingIndex);
#ifdef DEBUG
	void					MyGlErrorTest(const char*e_strMessage);
	void					MyGLEnable(GLenum e_GLenum);
	void					MyGLDisable(GLenum e_GLenum);
	void					MyGLGetIntegerv(GLenum e_GLenum, GLint *params);
	void					MyglGetBooleanv(GLenum e_GLenum, GLboolean* data);
#else
#define	MyGlErrorTest(q)
#define	MyGLEnable(p)			glEnable(p)
#define	MyGLDisable(p)			glDisable(p)
#define	MyGLGetIntegerv(p,q)	glGetIntegerv(p,q)
#endif