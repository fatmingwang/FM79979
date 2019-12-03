#pragma once

#ifdef WIN32
#include "../../../include/glew.h"
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif
//=======================
//the old function for setup matrix is glMultMatrixf
//but for the shader we need to ge tthe martix and setup it
//so those function to fetch matrix to replace old.
//=======================

	void glhFrustumf2(float *matrix, float left, float right, float bottom, float top,
					  float znear, float zfar);

	void glhPerspectivef2(float *matrix, float fovyInDegrees, float aspectRatio,
						  float znear, float zfar);

	void glhOrthof2(float *matrix, float left, float right, float bottom, float top, float znear, float zfar);

#ifdef DEBUG
	void					MyGlErrorTest(const char*e_strMessage);
	void					MyGLEnable(GLenum e_GLenum);
	void					MyGLDisable(GLenum e_GLenum);
	void					MyGLGetIntegerv(GLenum e_GLenum, GLint *params);
#else
#define	MyGlErrorTest(q)
#define	MyGLEnable(p)			glEnable(p)
#define	MyGLDisable(p)			glDisable(p)
#define	MyGLGetIntegerv(p,q)	glGetIntegerv(p,q)
#endif