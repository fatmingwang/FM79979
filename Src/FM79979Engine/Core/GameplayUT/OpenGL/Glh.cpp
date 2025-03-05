#include "Glh.h"
#include "assert.h"
#include "../../Common/StringToStructure.h"
#include "../../Common/Log/FMLog.h"
void MultiplyMatrices4by4OpenGL_FLOAT(float *result, float *matrix1, float *matrix2)
{
	result[0]=matrix1[0]*matrix2[0]+
		matrix1[4]*matrix2[1]+
		matrix1[8]*matrix2[2]+
		matrix1[12]*matrix2[3];
	result[4]=matrix1[0]*matrix2[4]+
		matrix1[4]*matrix2[5]+
		matrix1[8]*matrix2[6]+
		matrix1[12]*matrix2[7];
	result[8]=matrix1[0]*matrix2[8]+
		matrix1[4]*matrix2[9]+
		matrix1[8]*matrix2[10]+
		matrix1[12]*matrix2[11];
	result[12]=matrix1[0]*matrix2[12]+
		matrix1[4]*matrix2[13]+
		matrix1[8]*matrix2[14]+
		matrix1[12]*matrix2[15];

	result[1]=matrix1[1]*matrix2[0]+
		matrix1[5]*matrix2[1]+
		matrix1[9]*matrix2[2]+
		matrix1[13]*matrix2[3];
	result[5]=matrix1[1]*matrix2[4]+
		matrix1[5]*matrix2[5]+
		matrix1[9]*matrix2[6]+
		matrix1[13]*matrix2[7];
	result[9]=matrix1[1]*matrix2[8]+
		matrix1[5]*matrix2[9]+
		matrix1[9]*matrix2[10]+
		matrix1[13]*matrix2[11];
	result[13]=matrix1[1]*matrix2[12]+
		matrix1[5]*matrix2[13]+
		matrix1[9]*matrix2[14]+
		matrix1[13]*matrix2[15];

	result[2]=matrix1[2]*matrix2[0]+
		matrix1[6]*matrix2[1]+
		matrix1[10]*matrix2[2]+
		matrix1[14]*matrix2[3];
	result[6]=matrix1[2]*matrix2[4]+
		matrix1[6]*matrix2[5]+
		matrix1[10]*matrix2[6]+
		matrix1[14]*matrix2[7];
	result[10]=matrix1[2]*matrix2[8]+
		matrix1[6]*matrix2[9]+
		matrix1[10]*matrix2[10]+
		matrix1[14]*matrix2[11];
	result[14]=matrix1[2]*matrix2[12]+
		matrix1[6]*matrix2[13]+
		matrix1[10]*matrix2[14]+
		matrix1[14]*matrix2[15];

	result[3]=matrix1[3]*matrix2[0]+
		matrix1[7]*matrix2[1]+
		matrix1[11]*matrix2[2]+
		matrix1[15]*matrix2[3];
	result[7]=matrix1[3]*matrix2[4]+
		matrix1[7]*matrix2[5]+
		matrix1[11]*matrix2[6]+
		matrix1[15]*matrix2[7];
	result[11]=matrix1[3]*matrix2[8]+
		matrix1[7]*matrix2[9]+
		matrix1[11]*matrix2[10]+
		matrix1[15]*matrix2[11];
	result[15]=matrix1[3]*matrix2[12]+
		matrix1[7]*matrix2[13]+
		matrix1[11]*matrix2[14]+
		matrix1[15]*matrix2[15];
}

void glhFrustumf2(float *matrix, float left, float right, float bottom, float top,
				  float znear, float zfar)
{
	float temp, temp2, temp3, temp4;
	temp = 2 * znear;
	temp2 = right - left;
	temp3 = top - bottom;
	temp4 = zfar - znear;
	matrix[0] = temp / temp2;
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;
	matrix[4] = 0.0f;
	matrix[5] = temp / temp3;
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;
	matrix[8] = (right + left) / temp2;
	matrix[9] = (top + bottom) / temp3;
	matrix[10] = (-zfar - znear) / temp4;
	matrix[11] = -1.0f;
	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = (-temp * zfar) / temp4;
	matrix[15] = 0.0f;
}

void glhPerspectivef2(float *matrix, float fovyInDegrees, float aspectRatio,
					  float znear, float zfar)
{
	float ymax, xmax;
	float l_pi = 3.14159265358979323846f;
	ymax = znear * tanf(fovyInDegrees * l_pi / 360.0f);
	//ymin = -ymax;
	//xmin = -ymax * aspectRatio;
	xmax = ymax * aspectRatio;
	glhFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

void glhOrthof2(float *matrix, float left, float right, float bottom, float top, float znear, float zfar)
{
	float temp2, temp3, temp4;//, resultMatrix[16];
	temp2=right-left;
	temp3=top-bottom;
	temp4=zfar-znear;
	matrix[0]=2.f/temp2;
	matrix[1]=0.0;
	matrix[2]=0.0;
	matrix[3]=0.0;
	matrix[4]=0.0;
	matrix[5]=2.f/temp3;
	matrix[6]=0.0;
	matrix[7]=0.0;
	matrix[8]=0.0;
	matrix[9]=0.0;
	matrix[10]=-2.f/temp4;
	matrix[11]=0.0;
	matrix[12]=(-right-left)/temp2;
	matrix[13]=(-top-bottom)/temp3;
	matrix[14]=(-zfar-znear)/temp4;
	matrix[15]=1.0;
}

GLenum	IndexToBlendingIndex(int e_iIndex)
{
	switch (e_iIndex)
	{
	case 0:
		return GL_ZERO;
	case 1:
		return GL_ONE;
	case 2:
		return GL_ONE_MINUS_SRC_COLOR;
	case 3:
		return GL_DST_COLOR;
	case 4:
		return GL_ONE_MINUS_DST_COLOR;
	case 5:
		return GL_SRC_ALPHA;
	case 6:
		return GL_ONE_MINUS_SRC_ALPHA;
	case 7:
		return GL_DST_ALPHA;
	case 8:
		return GL_ONE_MINUS_DST_ALPHA;
	}
	return e_iIndex;
	//return 0;
}

int BlendingIndexToIndex(GLenum e_eBlendingIndex)
{
	switch (e_eBlendingIndex)
	{
	case GL_ZERO:
		return 0;
	case GL_ONE:
		return 1;
	case GL_ONE_MINUS_SRC_COLOR:
		return 2;
	case GL_DST_COLOR:
		return 3;
	case GL_ONE_MINUS_DST_COLOR:
		return 4;
	case GL_SRC_ALPHA:
		return 5;
	case GL_ONE_MINUS_SRC_ALPHA:
		return 6;
	case GL_DST_ALPHA:
		return 7;
	case GL_ONE_MINUS_DST_ALPHA:
		return 8;
	}
	assert(0 && "combox has not follow rule");
	return -1;
}

#ifdef DEBUG
void	MyGlErrorTest(const char*e_strMessage, const char* e_strFileName, int32_t e_iCodeLine)
{
	int l_iTestCont = 5;
	int	l_iError = glGetError();
	while(l_iError != 0 && l_iTestCont>0)
	{
		--l_iTestCont;
		std::string l_str = GET_FILENAME_AND_LINE(e_strFileName,e_iCodeLine);
		l_str += "   ";
		l_str += e_strMessage;
		l_str += "   glGetError:";
		const char* errorString = 0;
		switch (l_iError)
		{
			case GL_INVALID_ENUM: l_str += "GL_INVALID_ENUM"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: l_str += "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
			case GL_INVALID_VALUE: l_str += "GL_INVALID_VALUE"; break;
			case GL_INVALID_OPERATION: l_str += "GL_INVALID_OPERATION"; break;
			case GL_OUT_OF_MEMORY: l_str += "GL_OUT_OF_MEMORY"; break;
			default: l_str += FATMING_CORE::ValueToString(l_iError); break;
		}
		FMLog::LogWithFlag(l_str.c_str(), CORE_LOG_FLAG);
		l_iError = glGetError();
	}
}

void					MyGLEnable(GLenum e_GLenum)
{
	glEnable(e_GLenum);
	std::string l_strInfo = "MyGLEnable:";
	l_strInfo += FATMING_CORE::ValueToString((uint64)e_GLenum);
	CHECK_GL_ERROR(l_strInfo.c_str());
}

void					MyGLDisable(GLenum e_GLenum)
{
	glDisable(e_GLenum);
	std::string l_strInfo = "MyGLDisable:";
	l_strInfo += FATMING_CORE::ValueToString((uint64)e_GLenum);
	CHECK_GL_ERROR(l_strInfo.c_str());
}

void		MyGLGetIntegerv(GLenum e_GLenum, GLint *params)
{
	glGetIntegerv(e_GLenum, params);
	std::string l_strInfo = "MyGLGetIntegerv:";
	l_strInfo += FATMING_CORE::ValueToString((uint64)e_GLenum);
	CHECK_GL_ERROR(l_strInfo.c_str());
}

void MyglGetBooleanv(GLenum e_GLenum, GLboolean* data)
{
	glGetBooleanv(e_GLenum, data);
	std::string l_strInfo = "MyglGetBooleanv:";
	l_strInfo += FATMING_CORE::ValueToString((uint64)e_GLenum);
	CHECK_GL_ERROR(l_strInfo.c_str());
}

void	MyGLTexParameteri(GLenum e_GLenum, GLenum e_Target, GLint e_iValue)
{
	glTexParameteri(e_GLenum, e_Target, e_iValue);
	std::string l_strInfo = "MyGLTexParameteri:";
	l_strInfo += FATMING_CORE::ValueToString((uint64)e_GLenum);
	CHECK_GL_ERROR(l_strInfo.c_str());
}

#endif