#ifndef _GLH_H_
#define _GLH_H_

#if defined(WASM)
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

	void glhOrthof2(GLfloat *matrix, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat znear, GLfloat zfar);

#endif