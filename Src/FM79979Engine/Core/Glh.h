#ifndef _GLH_H_
#define _GLH_H_

//OPENGLES_2_X	//for shader
//OPENGLES_1_X	//fixed pipeline and no GL_UNSIGNED_INT.
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