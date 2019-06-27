#include "../stdafx.h"
#include "SimplePrimitive.h"
#include "BaseImage.h"
#include "../OpenGL/GLSL/Shader.h"
#include "../GameplayUT/GameApp.h"
//for opengl es1 compatibility,so we need color vertex buffer
void ASSIGN_2D_QUAD_COLOR(Vector4 Color)
{
	static float		l_f2DColorOne[16] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
	memcpy(l_f2DColorOne, Color, sizeof(float) * 4);
	memcpy(&l_f2DColorOne[4], Color, sizeof(float) * 4);
	memcpy(&l_f2DColorOne[8], Color, sizeof(float) * 4);
	memcpy(&l_f2DColorOne[12], Color, sizeof(float) * 4);
//#ifdef OPENGLES_2_X
	myGlColorPointer(4, l_f2DColorOne);
//#else
//	glColorPointer(4, GL_FLOAT, 0, g_f2DColorOne);
//#endif
}

void	ASSIGN_2D_QUAD_VerticesBySize(float Width, float Height, float Depth)
{
	static Vector3 l_v2DVertexBuffer[4] = { Vector3::Zero,Vector3::Zero,Vector3::Zero,Vector3::Zero };
	l_v2DVertexBuffer[0].x = (float)-Width;	l_v2DVertexBuffer[0].y = (float)-Height;
	l_v2DVertexBuffer[1].x = (float)Width;	l_v2DVertexBuffer[1].y = (float)-Height;
	l_v2DVertexBuffer[2].x = (float)-Width;	l_v2DVertexBuffer[2].y = (float)Height;
	l_v2DVertexBuffer[3].x = (float)Width;	l_v2DVertexBuffer[3].y = (float)Height;
	l_v2DVertexBuffer[0].z = l_v2DVertexBuffer[1].z = l_v2DVertexBuffer[2].z = l_v2DVertexBuffer[3].z = (float)Depth;
//#ifdef OPENGLES_2_X
	myGlVertexPointer(3, l_v2DVertexBuffer);
//#else
//	glVertexPointer(3, GL_FLOAT, 0, g_v2DVertexBuffer);
//#endif
}

void	ASSIGN_2D_QUAD_UV(float*UV)
{
	static float l_f2DTextureCoordinate[8] = { 0,0,1,0,0,1,1,1 };
	l_f2DTextureCoordinate[0] =UV[0];
	l_f2DTextureCoordinate[1] =UV[1];
	l_f2DTextureCoordinate[2] =UV[2];
	l_f2DTextureCoordinate[3] =UV[1];
	l_f2DTextureCoordinate[4] =UV[0];
	l_f2DTextureCoordinate[5] =UV[3];
	l_f2DTextureCoordinate[6] =UV[2];
	l_f2DTextureCoordinate[7] =UV[3];
//#ifdef OPENGLES_2_X
	myGlUVPointer(2, l_f2DTextureCoordinate);
//#else
//	glTexCoordPointer(2, GL_FLOAT, 0, g_f2DTextureCoordinate);
//#endif
	MyGlErrorTest("ASSIGN_2D_QUAD_UV");
}

void	ASSIGN_2D_QUAD_MIRROR_UV(float* UV)
{
	static float l_f2DTextureCoordinate[8] = { UV[2], UV[1], UV[0],UV[1], UV[2],UV[3],	UV[0],UV[3] };
//#ifdef OPENGLES_2_X
	myGlUVPointer(2, l_f2DTextureCoordinate);
//#else
//	glTexCoordPointer(2, GL_FLOAT, 0, g_f2DTextureCoordinate);
//#endif
	MyGlErrorTest("ASSIGN_2D_QUAD_MIRROR_UV");
}
#ifdef DEBUG
//#ifdef OPENGLES_2_X
void	myGlVertexPointer(int Stride, const GLvoid*pData)
{
	glVertexAttribPointer(g_uiAttribArray[FVF_POS], Stride, GL_FLOAT, 0, 0, pData);
	MyGlErrorTest("myGlVertexPointer");
}
void	myGlUVPointer(int Stride, const GLvoid*pData)
{
	glVertexAttribPointer(g_uiAttribArray[FVF_TEX0], Stride, GL_FLOAT, 0, 0, pData);
	MyGlErrorTest("myGlUVPointer");
}
void	myGlColorPointer(int Stride, const GLvoid*pData)
{
	glVertexAttribPointer(g_uiAttribArray[FVF_DIFFUSE], Stride, GL_FLOAT, 0, 0, pData);
	MyGlErrorTest("myGlColorPointer");
}
void	myGlNormalPointer(int Stride, const GLvoid*pData)
{
	glVertexAttribPointer(g_uiAttribArray[FVF_NORMAL], Stride, GL_FLOAT, 0, 0, pData);
	MyGlErrorTest("myGlNormalPointer");
}

void	myVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
{
	glVertexAttribPointer(index, size, type, normalized,stride,pointer);
	MyGlErrorTest("myVertexAttribPointer");
}
#endif
//#else
//void	myGlVertexPointer(int Stride, const GLvoid*pData)
//{
//	glVertexPointer(Stride, GL_FLOAT, 0, pData);
//	MyGlErrorTest("myGlVertexPointer");
//}
//void	myGlUVPointer(int Stride, const GLvoid*pData)
//{
//	glTexCoordPointer(Stride, GL_FLOAT, 0, pData);
//	MyGlErrorTest("myGlUVPointer");
//}
//void	myGlColorPointer(int Stride, const GLvoid*pData)
//{
//	glColorPointer(Stride, GL_FLOAT, 0, pData);
//	MyGlErrorTest("myGlColorPointer");
//}
//void	myGlNormalPointer(int Stride, const GLvoid*pData)
//{
//	glNormalPointer(GL_FLOAT, Stride, pData);
//	MyGlErrorTest("myGlNormalPointer");
//}
//#endif
#ifdef DEBUG
void	MY_GLDRAW_ARRAYS(GLenum mode, GLint first, GLsizei count)
{
#ifdef DEBUG
	MyGlErrorTest("MY_GLDRAW_ARRAYS");
#endif
	glDrawArrays(mode, first, count);
#ifdef DEBUG
	MyGlErrorTest("MY_GLDRAW_ARRAYS after glDrawArrays");
#endif
}

void	MY_GLDRAW_ELEMENTS(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
	glDrawElements(mode, count, type, indices);
	//GL_NO_ERROR 0
	//GL_INVALID_ENUM 0x0500
	//GL_INVALID_VALUE 0x0501
	//GL_INVALID_OPERATION 0x0502
	int	l_i = glGetError();
	if (l_i != 0)
	{
		//opengl es1 code.
		//bool	l_b0 = glIsEnabled(GL_VERTEX_ARRAY)==GL_TRUE;
		//bool	l_b1 = glIsEnabled(GL_TEXTURE_COORD_ARRAY)==GL_TRUE;
		//bool	l_b2 = glIsEnabled(GL_NORMAL_ARRAY)==GL_TRUE;
		//bool	l_b3 = glIsEnabled(GL_COLOR_ARRAY)==GL_TRUE;
		int a = 0;
	}
}
#endif
namespace GLRender
{
	//========comment start======================
	//set glOrtho as glOrtho(vPort[0], vPort[0]+vPort[2], vPort[1]+vPort[3],vPort[1], -1, 1);for normal windows coordinate
	//(0,top) _____
	//		  |	   |
	//		  |	   |
	//		  |____|(right,0)
	//
	//
	//set glOrthoas glOrtho(vPort[0], vPort[0]+vPort[2], vPort[1], vPort[1]+vPort[3], -1, 1);for
	//
	//
	//==============================
	//GLenum	g_enumforCullFace = 0;
	//MyGLDisable(GL_CULL_FACE);
	//basicly just treat as left hand coordinate(forward Z is postive backward is nagtive)
	void glEnable2D(float e_fResolutionWidth,float e_fResolutionHeight,float*e_pfMatrix,bool e_bInvertYAxis)
	{
	   //int vPort[4];
	   //MyGLGetIntegerv(GL_CULL_FACE,&g_enumforCullFace);
	   //glIsEnabled(GL_CULL_FACE);
		//depend on how do y loading the image
	   //if the pixel data is lefr to right or right to left it will effect the UV
	   //glOrtho(vPort[0], vPort[0]+vPort[2], vPort[1], vPort[1]+vPort[3], -1, 1);
		//=====================
		//vertices order
		//       1 ____ 2
		//		  |	   |
		//		  |	   |
		//		 0|____|3
		//=====================
		//texture coordinate
		//(0,1)   _____(1,1)
		//		  |	   |
		//		  |	   |
		//(0,0)	  |____|(1,0)
		//=====================
		//vertices order
		//       0 ____ 3
		//		  |	   |
		//		  |	   |
		//		 1|____|2
		//=====================
		//texture coordinate
		//(0,0)   _____(1,0)
		//		  |	   |
		//		  |	   |
		//(0,1)	  |____|(1,1)
		if( e_bInvertYAxis )
			e_fResolutionHeight = -e_fResolutionHeight;
		cMatrix44	l_matProjection;
		glhOrthof2((float*)l_matProjection.m, 0, e_fResolutionWidth, e_fResolutionHeight, 0, -10000, 10000);
		FATMING_CORE::SetupShaderViewProjectionMatrix(l_matProjection,true);
		if(e_pfMatrix)
		{
			memcpy(e_pfMatrix,l_matProjection,sizeof(float)*16);
		}

		MyGLDisable(GL_DEPTH_TEST);
		MyGLDisable(GL_CULL_FACE);
	   //glDepthMask(GL_FALSE);
	}
	//===========================================
	//end 2d apply
	//===========================================
	void glDisable2D()
	{
	   FATMING_CORE::DisableShaderProgram();
	}
	//here is a sample to draw quad by triangles
	//Vector3	l_v1 = Vector3(100,0,0);
	//Vector3	l_v2 = Vector3(100,100,0);
	//Vector3	l_v3 = Vector3(200,100,0);
	//Vector3	l_v4 = Vector3(200,0,0);

	//glBegin(GL_TRIANGLES);//013,312
	//glTexCoord2f(0,0);	glVertex3fv((float*)&l_v1 );
	//glTexCoord2f(0,1);	glVertex3fv((float*)&l_v2 );
	//glTexCoord2f(1,0);	glVertex3fv((float*)&l_v4 );
	//glTexCoord2f(1,0);	glVertex3fv((float*)&l_v4 );
	//glTexCoord2f(0,1);	glVertex3fv((float*)&l_v2 );
	//glTexCoord2f(1,1);	glVertex3fv((float*)&l_v3 );
	//glEnd()
	//below is how to draw quad by triangle strip
	//glBegin(GL_TRIANGLE_STRIP);//0132
	//glTexCoord2f(0,0);	glVertex3fv((float*)&l_v1 );
	//glTexCoord2f(1,0);	glVertex3fv((float*)&l_v4 );
	//glTexCoord2f(0,1);	glVertex3fv((float*)&l_v2 );
	//glTexCoord2f(1,1);	glVertex3fv((float*)&l_v3 );	
	//glEnd();

	//tricky rotation method
	//how to rotation?
	//for rotation reason we have to substract Width/2 and Hight/2
	//and translate to destination+half of width and half height


	void    GetDrawQuadFVFDataByTriangleStrip(GLint e_iWidth,GLint e_iHeight,float *e_pfTexCoordinate,Vector4 e_vColor,float*e_pfVertices,float*e_pfUV,float*e_pColor,cMatrix44*e_pmat)
	{
		AssignUVDataToTriangleStrip(e_pfTexCoordinate,e_pfUV,false);
		for( int i=0;i<4;++i )
			memcpy(&e_pColor[i*4],&e_vColor,sizeof(Vector4));
		float	l_fHalfWidth = e_iWidth/2.f;
		float	l_fHalfHeight = e_iHeight/2.f;
		Vector3*l_pvPos = (Vector3*)e_pfVertices;
		l_pvPos[0] = Vector3(-l_fHalfWidth,-l_fHalfHeight,0.f);
		l_pvPos[1] = Vector3(l_fHalfWidth,-l_fHalfHeight,0.f);
		l_pvPos[2] = Vector3(-l_fHalfWidth,l_fHalfHeight,0.f);
		l_pvPos[3] = Vector3(l_fHalfWidth,l_fHalfHeight,0.f);
		if( e_pmat )
		{
			for( int i=0;i<4;++i )
			{
				l_pvPos[i] = e_pmat->TransformCoordinate(l_pvPos[i]);
			}
		}
	}

	void    GetDrawQuadFVFDataByTwoTriangles  (GLint e_iWidth,GLint e_iHeight,float *e_pfTexCoordinate,Vector4 e_vColor,float*e_pfVertices,float*e_pfUV,float*e_pColor,cMatrix44*e_pmat)
	{
		AssignUVDataTo2Triangles(e_pfTexCoordinate,e_pfUV,false);
		for( int i=0;i<6;++i )
			memcpy(&e_pColor[i*4],&e_vColor,sizeof(Vector4));
		float	l_fHalfWidth = e_iWidth/2.f;
		float	l_fHalfHeight = e_iHeight/2.f;
		Vector3	l_vPos[4];
		l_vPos[0] = Vector3(-l_fHalfWidth,-l_fHalfHeight,0.f);
		l_vPos[1] = Vector3(l_fHalfWidth,-l_fHalfHeight,0.f);
		l_vPos[2] = Vector3(-l_fHalfWidth,l_fHalfHeight,0.f);
		l_vPos[3] = Vector3(l_fHalfWidth,l_fHalfHeight,0.f);
		if( e_pmat )
		{
			cMatrix44	l_mat = *e_pmat;
			l_mat.m[3][0] += l_fHalfWidth;
			l_mat.m[3][1] += l_fHalfHeight;
			for( int i=0;i<4;++i )
			{
				l_vPos[i] = l_mat.TransformCoordinate(l_vPos[i]);
			}
		}
		//left down
		e_pfVertices[0] = l_vPos[2].x;
		e_pfVertices[1] = l_vPos[2].y;
		e_pfVertices[2] = l_vPos[2].z;
		//right down
		e_pfVertices[3] = l_vPos[3].x;
		e_pfVertices[4] = l_vPos[3].y;
		e_pfVertices[5] = l_vPos[3].z;
		//left upper
		e_pfVertices[6] = l_vPos[0].x;
		e_pfVertices[7] = l_vPos[0].y;
		e_pfVertices[8] = l_vPos[0].z;
		//left upper
		e_pfVertices[9] = l_vPos[0].x;
		e_pfVertices[10] = l_vPos[0].y;
		e_pfVertices[11] = l_vPos[0].z;
		//right down
		e_pfVertices[12] = l_vPos[3].x;
		e_pfVertices[13] = l_vPos[3].y;
		e_pfVertices[14] = l_vPos[3].z;
		//right upper
		e_pfVertices[15] = l_vPos[1].x;
		e_pfVertices[16] = l_vPos[1].y;
		e_pfVertices[17] = l_vPos[1].z;
	}

	void	RenderQuadWithTextureAndColorAndCoordinate(GLfloat e_iX,GLfloat e_iY,GLfloat e_fDepth,GLfloat  e_fWidth,GLfloat  e_fHeight,Vector4 e_vColor,float *e_pfTexCoordinate,Vector3 e_vRotationAngle,const wchar_t*e_strShaderName)
	{
		UseShaderProgram(e_strShaderName);
		e_fWidth/=2;
		e_fHeight/=2;
		cMatrix44 l_mat = cMatrix44::TranslationMatrix(Vector3(e_iX+e_fWidth,e_iY+e_fHeight, 0.f));
		l_mat *= cMatrix44::RotationMatrix(e_vRotationAngle);
		FATMING_CORE::SetupShaderWorldMatrix(l_mat);
		ASSIGN_2D_QUAD_UV(e_pfTexCoordinate);
		ASSIGN_2D_QUAD_VerticesBySize(e_fWidth,e_fHeight,e_fDepth);
		ASSIGN_2D_QUAD_COLOR(e_vColor);
		MY_GLDRAW_ARRAYS(GL_TRIANGLE_STRIP, 0, 4);
	}


	void	RenderMirrorQuadWithTextureAndColorAndCoordinate(GLfloat e_iX,GLfloat e_iY,GLfloat e_fDepth,float e_iWidth,float e_iHeight,Vector4 e_vColor,float *e_pfTexCoordinate,float e_fRotationAngle)
	{
		UseShaderProgram(DEFAULT_SHADER);
		e_iWidth/=2;
		e_iHeight/=2;
		cMatrix44 l_mat = cMatrix44::TranslationMatrix(Vector3(e_iX+e_iWidth,e_iY+e_iHeight, 0.f));
		if(e_fRotationAngle!=0.f)
			l_mat *= cMatrix44::ZAxisRotationMatrix(D3DXToRadian(e_fRotationAngle));
		FATMING_CORE::SetupShaderWorldMatrix(l_mat);
		ASSIGN_2D_QUAD_MIRROR_UV(e_pfTexCoordinate);
		ASSIGN_2D_QUAD_VerticesBySize(e_iWidth,e_iHeight,e_fDepth);
		ASSIGN_2D_QUAD_COLOR(e_vColor);
		MY_GLDRAW_ARRAYS(GL_TRIANGLE_STRIP, 0, 4);
	}

	
	void    RenderTrianglesWithMatrix(float*e_pfVertices, float*e_pfTextureUV, float*e_pvColor, float*e_pfMatrix, int e_iPosStride, int e_iNumTriangles, const wchar_t*e_strShaderName)
	{
		UseShaderProgram(e_strShaderName);
		FATMING_CORE::SetupShaderWorldMatrix(e_pfMatrix);
		myGlVertexPointer(e_iPosStride, e_pfVertices);
		myGlUVPointer(2, e_pfTextureUV);
		myGlColorPointer(4, e_pvColor);
		MY_GLDRAW_ARRAYS(GL_TRIANGLES, 0, 3 * e_iNumTriangles);
	}

	void    RenderQuadWithMatrix(float*e_pfVertices,float*e_pfTextureUV,Vector4 e_vColor,float*e_pfMatrix,int e_iPosStride,int e_iNumQuad,const wchar_t*e_strShaderName )
	{
		UseShaderProgram(e_strShaderName );
	   // draw with client side arrays (in real apps you should use cached VBOs which is much better for performance)
		FATMING_CORE::SetupShaderWorldMatrix(e_pfMatrix);
		ASSIGN_2D_QUAD_COLOR(e_vColor);
		myGlVertexPointer(e_iPosStride,e_pfVertices);
		myGlUVPointer(2,e_pfTextureUV);
		MY_GLDRAW_ARRAYS(GL_TRIANGLE_STRIP, 0, 4*e_iNumQuad);
	}
	void	Render3DArrow(Vector3 P,Vector3 D,Vector4 e_vColor,float e_fWidth)
	{
		UseShaderProgram(NO_TEXTURE_SHADER);
		float angle = atan2(D.y, D.x);
		float	l_fLength = D.Length();
		Vector3	l_vD10Percent = D*0.1f;
		float	l_fAllVertices[] = { 0.f,0.f,0.f,
									 D.x,D.y,D.z,

									 D.x,D.y,D.z,
									 D.x-l_vD10Percent.x,D.y-l_vD10Percent.y,D.z-l_vD10Percent.z,

									 D.x,D.y,D.z,
									 D.x-l_vD10Percent.x,D.y-l_vD10Percent.y,D.z+l_vD10Percent.z,

									 D.x,D.y,D.z,
									 D.x-l_vD10Percent.x,D.y+l_vD10Percent.y,D.z-l_vD10Percent.z,

									 D.x,D.y,D.z,
									 D.x-l_vD10Percent.x,D.y+l_vD10Percent.y,D.z+l_vD10Percent.z,


									 };
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(3,l_fAllVertices);
		FATMING_CORE::SetupShaderWorldMatrix(cMatrix44::Identity);
		MY_GLDRAW_ARRAYS(GL_LINES, 0, 10);
	}
	void RenderArrow(Vector3 P,Vector3 D, float radius,Vector4 e_vColor)
	{
		UseShaderProgram(NO_TEXTURE_SHADER);
		glLineWidth(radius);
		float	l_fDegree = 360.f-UT::GetAngleBy2Point(P,D);
		float	l_fRadian = D3DXToRadian(l_fDegree);
		float	l_fLength = (D-P).Length();
		cMatrix44 l_mat = cMatrix44::TranslationMatrix(Vector3(P.x, P.y, 0.0f));
		l_mat *= cMatrix44::ZAxisRotationMatrix(l_fRadian);
		//l_mat *= cMatrix44::ScaleMatrix(Vector3(l_fLength, l_fLength, 1.0f));
		//float	l_fAllVertices[] = { 0.f,0.f,
		//							 1.f,0.f,
		//							 1.f,0.f,
		//							 0.9f,-0.05f,
		//							 1.f,0.f,
		//							 0.9f,0.05f
		//							 };
		//Y up is 0 degree
		float	l_fAllVertices[] = { 0.f,0.f,
									 0.f,l_fLength,
									 0.f,l_fLength,
									 -0.05f*l_fLength,0.9f*l_fLength,
									 0.f,l_fLength,
									 0.05f*l_fLength,0.9f*l_fLength
		};
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(2,l_fAllVertices);
		FATMING_CORE::SetupShaderWorldMatrix(l_mat);
		MY_GLDRAW_ARRAYS(GL_LINES, 0, 6);
	}

	//enable point size in opengl ES2.0 shader
	//MyGLEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	//void VertexShader_main() 
	//{
	//	gl_PointSize = PointSize;
	//} 
	void	RenderPoint(Vector2	e_vPos,float e_fSize,Vector4 e_vColor)
	{
		UseShaderProgram(NO_TEXTURE_SHADER);
		//set gl_PointSize value at shader code.
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(2,e_vPos);
		FATMING_CORE::SetupShaderWorldMatrix(cMatrix44::Identity);
		MY_GLDRAW_ARRAYS(GL_POINTS, 0, 1);
	}

	void	RenderPoint(Vector3	e_vPos,float e_fSize,Vector4 e_vColor)
	{
		UseShaderProgram(NO_TEXTURE_SHADER);
		//set gl_PointSize value at shader code.
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(3,e_vPos);
		FATMING_CORE::SetupShaderWorldMatrix(cMatrix44::Identity);
		MY_GLDRAW_ARRAYS(GL_POINTS, 0, 1);
	}
	void	RenderPoints(Vector3*e_pvPos,int e_iNumPoints,float e_fSize,Vector4 e_vColor,float*e_pfMatrix)
	{
		if( e_iNumPoints < 1 )
			return;
		UseShaderProgram(NO_TEXTURE_SHADER);
		//set gl_PointSize value at shader code.
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(3,e_pvPos);
		FATMING_CORE::SetupShaderWorldMatrix(e_pfMatrix);
		MY_GLDRAW_ARRAYS(GL_POINTS, 0, e_iNumPoints);
	}

	void	RenderPoints(Vector2*e_pvPos,int e_iNumPoints,float e_fSize,Vector4 e_vColor,float*e_pfMatrix)
	{
		if( e_iNumPoints < 1 )
			return;
		UseShaderProgram(NO_TEXTURE_SHADER);
		//set gl_PointSize value at shader code.
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(2,e_pvPos);
		FATMING_CORE::SetupShaderWorldMatrix(e_pfMatrix);
		MY_GLDRAW_ARRAYS(GL_POINTS, 0, e_iNumPoints);
	}


	void	RenderPoints(Vector4*e_vPos,int e_iNumPoints,float e_fSize,Vector4 e_vColor,float*e_pfMatrix)
	{
		if( e_iNumPoints < 1 )
			return;
		UseShaderProgram(NO_TEXTURE_SHADER);
		std::vector<Vector3>	l_TempVector;
		l_TempVector.resize(e_iNumPoints);
		for( int i=0;i<e_iNumPoints;++i )
		{
			l_TempVector[i] = Vector3(e_vPos[i]);
		}
		//set gl_PointSize value at shader code.
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(3,&l_TempVector[0]);
		FATMING_CORE::SetupShaderWorldMatrix(e_pfMatrix);
		MY_GLDRAW_ARRAYS(GL_POINTS, 0, e_iNumPoints);
	}
	//e_iNum as many as line points
	void	RenderLine(float*e_pPosition,int e_iNumPoints,Vector4 e_vColor,int e_iStride,float*e_pfMatrix,float e_fLineSize,const wchar_t*e_strShaderName)
	{
		if( e_iNumPoints<2 )
			return;
		UseShaderProgram(e_strShaderName);
		glLineWidth(e_fLineSize);
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(e_iStride,e_pPosition);
		FATMING_CORE::SetupShaderWorldMatrix(e_pfMatrix);
		assert((e_iStride == 2 || e_iStride == 3)&&"draw line stride must be 2 or 3");
		MY_GLDRAW_ARRAYS(GL_LINES, 0, e_iNumPoints);
	}
	void	RenderLine(std::vector<Vector2>* e_pPositionPoint,Vector4 e_vColor,float*e_pfMatrix,bool e_bRenderPoints,bool e_bRenderIndexFont, const wchar_t*e_strShaderName)
	{
		if( e_pPositionPoint->size() < 2 )
			return;
		UseShaderProgram(e_strShaderName);
		//draw line
		std::vector<Vector2>	l_CurvePointVector;
		int	l_iNumPoint = ((int)e_pPositionPoint->size()-1)*2;
		l_CurvePointVector.resize(l_iNumPoint);
		for( int i=0;i<l_iNumPoint/2;++i )
		{
			l_CurvePointVector[i*2] = (*e_pPositionPoint)[i];
			l_CurvePointVector[i*2+1] = (*e_pPositionPoint)[i+1];
		}
		cMatrix44	l_mat(e_pfMatrix);
		RenderLine((float*)&l_CurvePointVector[0],l_iNumPoint,e_vColor,2,e_pfMatrix);
		if( e_bRenderPoints )
			RenderPoints(&(*e_pPositionPoint)[0],(int)e_pPositionPoint->size(),15,e_vColor,e_pfMatrix);
		if( e_bRenderIndexFont )
		{
			size_t	l_uiSize = e_pPositionPoint->size();
			for( size_t i=0;i<l_uiSize;++i )
			{
				Vector3	l_vPos((*e_pPositionPoint)[i]);
				l_vPos = l_mat.TransformCoordinate(l_vPos);
				cGameApp::RenderFont(l_vPos.x,l_vPos.y,ValueToStringW(i).c_str());
			}
		}
	}
	void	RenderLine(std::vector<Vector3>* e_pPositionPoint,Vector4 e_vColor,float*e_pfMatrix,bool e_bRenderPoints,bool e_bRenderIndexFont, const wchar_t*e_strShaderName)
	{
		if( e_pPositionPoint->size() == 0 )
			return;
		if( e_pPositionPoint->size() > 1 )
		{
			UseShaderProgram(e_strShaderName);
			//draw line
			std::vector<Vector3>	l_CurvePointVector;
			int	l_iNumPoint = ((int)e_pPositionPoint->size()-1)*2;
			l_CurvePointVector.resize(l_iNumPoint);
			for( int i=0;i<l_iNumPoint/2;++i )
			{
				l_CurvePointVector[i*2] = (*e_pPositionPoint)[i];
				l_CurvePointVector[i*2+1] = (*e_pPositionPoint)[i+1];
			}
			RenderLine((float*)&l_CurvePointVector[0],l_iNumPoint,e_vColor,3,e_pfMatrix);
			if (e_bRenderPoints)
				RenderPoints(&(*e_pPositionPoint)[0], (int)e_pPositionPoint->size(), 15, e_vColor, e_pfMatrix);
			if (e_bRenderIndexFont)
			{
				cMatrix44	l_mat(e_pfMatrix);
				size_t	l_uiSize = e_pPositionPoint->size();
				for (size_t i = 0; i<l_uiSize; ++i)
				{
					Vector3	l_vPos((*e_pPositionPoint)[i]);
					l_vPos = l_mat.TransformCoordinate(l_vPos);
					cGameApp::RenderFont(l_vPos.x, l_vPos.y, ValueToStringW(i).c_str());
				}
			}
		}
	}

	void	RenderLine(std::vector<Vector4>* e_pPositionPoint,Vector4 e_vColor,float*e_pfMatrix,bool e_bRenderPoints,bool e_bRenderIndexFont, const wchar_t*e_strShaderName)
	{
		if( e_pPositionPoint->size() == 0 )
			return;
		if( e_pPositionPoint->size() > 1 )
		{
			UseShaderProgram(e_strShaderName);
			//draw line
			std::vector<Vector3>	l_CurvePointVector;
			int	l_iNumPoint = ((int)e_pPositionPoint->size()-1)*2;
			l_CurvePointVector.resize(l_iNumPoint);
			for( int i=0;i<l_iNumPoint/2;++i )
			{
				l_CurvePointVector[i*2] = Vector3((*e_pPositionPoint)[i]);
				l_CurvePointVector[i*2+1] = Vector3((*e_pPositionPoint)[i+1]);
			}
			RenderLine((float*)&l_CurvePointVector[0],l_iNumPoint,e_vColor,3,e_pfMatrix);
			if (e_bRenderPoints)
				RenderPoints(&(*e_pPositionPoint)[0], (int)e_pPositionPoint->size(), 15, e_vColor, e_pfMatrix);
			if (e_bRenderIndexFont)
			{
				cMatrix44	l_mat(e_pfMatrix);
				size_t	l_uiSize = e_pPositionPoint->size();
				for (size_t i = 0; i<l_uiSize; ++i)
				{
					Vector3	l_vPos((*e_pPositionPoint)[i]);
					l_vPos = l_mat.TransformCoordinate(l_vPos);
					cGameApp::RenderFont(l_vPos.x, l_vPos.y, ValueToStringW(i).c_str());
				}
			}
		}
	}

	void	RenderSphere(cMatrix44 e_mat,float e_fRadius,Vector4 e_vColor,bool e_bDrawPoint,float e_fDensityAngle)
	{
		UseShaderProgram(NO_TEXTURE_SHADER);
		if( e_fDensityAngle<1.f )
			e_fDensityAngle = 1.f;
		int	l_iNum = (int)(360.f/e_fDensityAngle);
		float	l_fStepAngle = 1/e_fDensityAngle;
		Vector2	l_vFirestPos = Vector2(sin(l_fStepAngle)*e_fRadius,cos(l_fStepAngle)*e_fRadius);
		for( int i=0;i<l_iNum;++i )
		{
			float	l_fX = sin(l_fStepAngle*i)*e_fRadius;
			float	l_fY = cos(l_fStepAngle*i)*e_fRadius;
			Vector2	l_vPos = Vector2(l_fX,l_fY);
			g_fGlobalTempBufferForRenderVertices[i*4] = l_vFirestPos.x;
			g_fGlobalTempBufferForRenderVertices[i*4+1] = l_vFirestPos.y;
			g_fGlobalTempBufferForRenderVertices[i*4+2] = l_vPos.x;
			g_fGlobalTempBufferForRenderVertices[i*4+3] = l_vPos.y;
			l_vFirestPos = l_vPos;
		}
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(2, g_fGlobalTempBufferForRenderVertices);
		FATMING_CORE::SetupShaderWorldMatrix(e_mat);
		MY_GLDRAW_ARRAYS(GL_LINES, 0, l_iNum*2);
		if( e_bDrawPoint )
		{
			for( int i=0;i<l_iNum;++i )
			{
				g_fGlobalTempBufferForRenderVertices[i*2] = sin(l_fStepAngle*i)*e_fRadius;
				g_fGlobalTempBufferForRenderVertices[i*2+1] = cos(l_fStepAngle*i)*e_fRadius;
			}
			myGlVertexPointer(2, g_fGlobalTempBufferForRenderVertices);
			MY_GLDRAW_ARRAYS(GL_LINES, 0, l_iNum);
		}
	}

	void	RenderSphere(Vector2 e_vPos,float e_fRadius,Vector4 e_vColor,bool e_bDrawPoint,float e_fDensityAngle)
	{
		UseShaderProgram(NO_TEXTURE_SHADER);
		if( e_fDensityAngle<1.f )
			e_fDensityAngle = 1.f;
		int	l_iNum = (int)(360.f/e_fDensityAngle);
		float	l_fStepAngle = 1/e_fDensityAngle;
		Vector2	l_vFirestPos = Vector2(sin(l_fStepAngle)*e_fRadius,cos(l_fStepAngle)*e_fRadius);
		cMatrix44 l_mat = cMatrix44::TranslationMatrix(Vector3(e_vPos.x,e_vPos.y, 0.0f));
		for( int i=0;i<l_iNum;++i )
		{
			float	l_fX = sin(l_fStepAngle*i)*e_fRadius;
			float	l_fY = cos(l_fStepAngle*i)*e_fRadius;
			Vector2	l_vPos = Vector2(l_fX,l_fY);
			g_fGlobalTempBufferForRenderVertices[i*4] = l_vFirestPos.x;
			g_fGlobalTempBufferForRenderVertices[i*4+1] = l_vFirestPos.y;
			g_fGlobalTempBufferForRenderVertices[i*4+2] = l_vPos.x;
			g_fGlobalTempBufferForRenderVertices[i*4+3] = l_vPos.y;
			l_vFirestPos = l_vPos;
		}
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(2, g_fGlobalTempBufferForRenderVertices);
		FATMING_CORE::SetupShaderWorldMatrix(l_mat);
		MY_GLDRAW_ARRAYS(GL_LINES, 0, l_iNum*2);
		if( e_bDrawPoint )
		{
			for( int i=0;i<l_iNum;++i )
			{
				g_fGlobalTempBufferForRenderVertices[i*2] = sin(l_fStepAngle*i)*e_fRadius;
				g_fGlobalTempBufferForRenderVertices[i*2+1] = cos(l_fStepAngle*i)*e_fRadius;
			}
			myGlVertexPointer(2, g_fGlobalTempBufferForRenderVertices);
			MY_GLDRAW_ARRAYS(GL_LINES, 0, l_iNum);
		}
	}


	void	RenderRectangle(Vector2 e_Pos,float e_fWidth,float e_fHeight,Vector4 e_vColor,float e_fAngle,float e_fLineWidth)
	{
		RenderRectangle(e_Pos,e_fWidth,e_fHeight,e_vColor,Vector3(0,0,e_fAngle),e_fLineWidth);
	}
	void	RenderRectangle(Vector3 e_Pos,float e_fWidth,float e_fHeight,Vector4 e_vColor,Vector3 e_vAngle,float e_fLineWidth)
	{
		RenderRectangle(Vector2(e_Pos.x,e_Pos.y),e_fWidth,e_fHeight,e_vColor,e_vAngle,e_fLineWidth);
	}
	void	RenderRectangle(Vector2 e_Pos,float e_fWidth,float e_fHeight,Vector4 e_vColor,Vector3 e_vAngle,float e_fLineWidth)
	{
		cMatrix44 l_mat = cMatrix44::TranslationMatrix(Vector3(e_Pos.x,e_Pos.y, 0.f));
		l_mat *= cMatrix44::RotationMatrix(e_vAngle);
		RenderRectangle(e_fWidth,e_fHeight, l_mat,e_vColor,e_fLineWidth);
	}

	void	RenderRectangle(float e_fWidth,float e_fHeight,cMatrix44 e_Mat,Vector4 e_vColor,float e_fLineWidth)
	{
		UseShaderProgram(NO_TEXTURE_SHADER);
		glLineWidth(e_fLineWidth);
		float	l_fWidth = e_fWidth;
		float	l_fHeight = e_fHeight;
		float	l_fAllVertices[16] = {0,0,
									  0,l_fHeight,

									  0,0,
									  l_fWidth,0,

									  0,l_fHeight,
									  l_fWidth,l_fHeight,

									  l_fWidth,l_fHeight,
									  l_fWidth,0};

		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(2,l_fAllVertices);
		FATMING_CORE::SetupShaderWorldMatrix(e_Mat);
		MY_GLDRAW_ARRAYS(GL_LINES, 0, 8);

		//cBaseShader*l_p2DShader = GetCurrentShader();
		//UseShaderProgram(NO_TEXTURE_SHADER);
		//glLineWidth(e_fLineWidth);
		//float	l_fWidth = e_fWidth/2.f;
		//float	l_fHeight = e_fHeight/2.f;
		//float	l_fAllVertices[16] = {-l_fWidth,-l_fHeight,
		//							  l_fWidth,-l_fHeight,
		//							  -l_fWidth,-l_fHeight,
		//							  -l_fWidth,l_fHeight,
		//							  -l_fWidth,l_fHeight,
		//							  l_fWidth,l_fHeight,
		//							  l_fWidth,l_fHeight,
		//							  l_fWidth,-l_fHeight};

		//FATMING_CORE::SetupShaderColor(e_vColor);
		//myGlVertexPointer(2,l_fAllVertices);
		//FATMING_CORE::SetupShaderWorldMatrix(e_Mat);
		//MY_GLDRAW_ARRAYS(GL_LINES, 0, 8);
		//UseShaderProgram(l_p2DShader);
			
	}

	void	RenderFilledRectangle(Vector2 e_Pos,float e_fWidth,float e_fHeight,Vector4 e_vColor,float e_fAngle)
	{
		UseShaderProgram(NO_TEXTURE_SHADER);
		e_fWidth/=2;
		e_fHeight/=2;
		cMatrix44 l_mat = cMatrix44::TranslationMatrix(Vector3(e_Pos.x+e_fWidth,e_Pos.y+e_fHeight, 0.f));
		if(e_fAngle!=0.f)
			l_mat *= cMatrix44::ZAxisRotationMatrix(D3DXToRadian(e_fAngle));
		FATMING_CORE::SetupShaderWorldMatrix(l_mat);
		ASSIGN_2D_QUAD_VerticesBySize(e_fWidth,e_fHeight,0.f);
		FATMING_CORE::SetupShaderColor(e_vColor);
		MY_GLDRAW_ARRAYS(GL_TRIANGLE_STRIP, 0, 4);
	}

	void	RenderRectangle(POINT e_Pos,int e_iWidth,int e_iHeight,Vector4 e_vColor,float e_fAngle,float e_fLineWidth)
	{
		UseShaderProgram(NO_TEXTURE_SHADER);
		glLineWidth(e_fLineWidth);
		float	l_fWidth = e_iWidth/2.f;
		float	l_fHeight = e_iHeight/2.f;
		cMatrix44 l_mat = cMatrix44::TranslationMatrix(Vector3(e_Pos.x+l_fWidth,e_Pos.y+l_fHeight, 0.f));
		if( e_fAngle !=0.f )
			l_mat *= cMatrix44::ZAxisRotationMatrix(e_fAngle);
		float	l_fAllVertices[16] = {-l_fWidth,-l_fHeight,
									  l_fWidth,-l_fHeight,
									  -l_fWidth,-l_fHeight,
									  -l_fWidth,l_fHeight,
									  -l_fWidth,l_fHeight,
									  l_fWidth,l_fHeight,
									  l_fWidth,l_fHeight,
									  l_fWidth,-l_fHeight};

		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(2,l_fAllVertices);
		FATMING_CORE::SetupShaderWorldMatrix(l_mat);
		MY_GLDRAW_ARRAYS(GL_LINES, 0, 8);
	}

	void	RenderPlane(cMatrix44 e_mat,float	e_fWidth,float	e_fHeight,float e_fGridWidthDistance,float e_fGridHeightDistance,float e_fLineWidth,Vector4 e_vColor)
	{
		assert(e_fWidth>e_fGridWidthDistance&&e_fHeight>e_fGridHeightDistance);
		int	l_iRow = (int)(e_fWidth/e_fGridWidthDistance)+1;
		int	l_iColumn = (int)(e_fHeight/e_fGridHeightDistance)+1;
		int	l_iTotalCount = (l_iRow+l_iColumn)*2;
		//if( l_iTotalCount>200 )//-.- to big lah.
		//	return;
		UseShaderProgram(NO_TEXTURE_SHADER);
		glLineWidth(e_fLineWidth);
		Vector2	l_vLeftUp(-e_fWidth/2,-e_fHeight/2.f);
		Vector2	l_vRightDown(e_fWidth/2,e_fHeight/2.f);
		Vector2	*l_pvVerticesForRenderPlane = (Vector2*)g_fGlobalTempBufferForRenderVertices;
		//for row
		for( int i=0;i<l_iRow;++i )
		{
			l_pvVerticesForRenderPlane[i*2] = Vector2(l_vLeftUp.x,l_vLeftUp.y+i*e_fGridHeightDistance);
			l_pvVerticesForRenderPlane[i*2+1] = Vector2(l_vRightDown.x,l_vLeftUp.y+i*e_fGridHeightDistance);
		}
		//+2 from l_iTotalCount
		int	l_iTotalRow = l_iRow*2;
		for( int j=0;j<l_iColumn+2;++j )
		{
			l_pvVerticesForRenderPlane[l_iTotalRow+j*2] = Vector2(l_vLeftUp.x+j*e_fGridWidthDistance,l_vLeftUp.y);
			l_pvVerticesForRenderPlane[l_iTotalRow+j*2+1] = Vector2(l_vLeftUp.x+j*e_fGridWidthDistance,l_vRightDown.y);
		}
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(2,l_pvVerticesForRenderPlane);
		FATMING_CORE::SetupShaderWorldMatrix(e_mat);
		MY_GLDRAW_ARRAYS(GL_LINES, 0, l_iTotalCount);
		
	}

	void	AssignUVDataTo2Triangles(float*e_pSrc,float*e_pDest,bool e_bMirror)
	{
		//for UV
		if( !e_bMirror )
		{
			//left down
			e_pDest[0] = e_pSrc[0];			e_pDest[1] = e_pSrc[3];
			//right down
			e_pDest[2] = e_pSrc[2];			e_pDest[3] = e_pSrc[3];
			//left up
			e_pDest[4] = e_pSrc[0];			e_pDest[5] = e_pSrc[1];
			//left up
			e_pDest[6] = e_pSrc[0];			e_pDest[7] = e_pSrc[1];
			//right down
			e_pDest[8] = e_pSrc[2];			e_pDest[9] = e_pSrc[3];
			//right top
			e_pDest[10] = e_pSrc[2];		e_pDest[11] = e_pSrc[1];
		}
		else
		{
			e_pDest[0] = e_pSrc[2];			e_pDest[1] = e_pSrc[3];
			e_pDest[2] = e_pSrc[0];			e_pDest[3] = e_pSrc[3];
			e_pDest[4] = e_pSrc[2];			e_pDest[5] = e_pSrc[1];

			e_pDest[6] = e_pSrc[2];			e_pDest[7] = e_pSrc[1];
			e_pDest[8] = e_pSrc[0];			e_pDest[9] = e_pSrc[3];
			e_pDest[10] = e_pSrc[0];		e_pDest[11] = e_pSrc[1];
		}
	}

	void	AssignUVDataToTriangleStrip(float*e_pSrc,float*e_pDest,bool e_bMirror)
	{
		if( !e_bMirror )
		{
			e_pDest[0] = e_pSrc[0];			e_pDest[1] = e_pSrc[1];
			e_pDest[2] = e_pSrc[2];			e_pDest[3] = e_pSrc[1];
			e_pDest[4] = e_pSrc[0];			e_pDest[5] = e_pSrc[3];
			e_pDest[6] = e_pSrc[2];			e_pDest[7] = e_pSrc[3];
		}
		else
		{
			e_pDest[0] = e_pSrc[2];			e_pDest[1] = e_pSrc[1];
			e_pDest[2] = e_pSrc[0];			e_pDest[3] = e_pSrc[1];
			e_pDest[4] = e_pSrc[2];			e_pDest[5] = e_pSrc[3];
			e_pDest[6] = e_pSrc[0];			e_pDest[7] = e_pSrc[3];
		}
	}

	void	Assign4VerticesDataTo2Triangles(float*e_pSrc,float*e_pDest,int e_iStrip)
	{
		if( e_iStrip == 3 )
		{
			Vector3*l_pvSrc = (Vector3*)e_pSrc;
			Vector3*l_pvDest = (Vector3*)e_pDest;
			//left down
			l_pvDest[0] = l_pvSrc[2];
			//right down
			l_pvDest[1] = l_pvSrc[3];
			//left up
			l_pvDest[2] = l_pvSrc[0];
			//left up
			l_pvDest[3] = l_pvSrc[0];
			//right down
			l_pvDest[4] = l_pvSrc[3];
			//right top
			l_pvDest[5] = l_pvSrc[1];
		}
		else
		if( e_iStrip == 2 )
		{
			Vector2*l_pvSrc = (Vector2*)e_pSrc;
			Vector2*l_pvDest = (Vector2*)e_pDest;
			//left down
			l_pvDest[0] = l_pvSrc[2];
			//right down
			l_pvDest[1] = l_pvSrc[3];
			//left up
			l_pvDest[2] = l_pvSrc[0];
			//left up
			l_pvDest[3] = l_pvSrc[0];
			//right down
			l_pvDest[4] = l_pvSrc[3];
			//right top
			l_pvDest[5] = l_pvSrc[1];		
		}
	}

	void	Assign4VerticesDataTo2Triangles(float*e_pVertexSrc,float*e_pVertexDest,float*e_pUVSrc,float*e_pUVDest,int e_iStrip)
	{
		Assign4VerticesDataTo2Triangles(e_pVertexSrc,e_pVertexDest,e_iStrip);
		AssignUVDataTo2Triangles(e_pUVSrc,e_pUVDest,false);
	}

	void	RenderVectorProject(Vector3 e_v1,Vector3 e_v2,cMatrix44 e_mat)
	{
		Vector3	l_vD3 = e_v1.Projected(e_v2);
		float	l_f1[6] = {0,0,0,e_v1.x,e_v1.y,e_v1.z};
		float	l_f2[6] = {0,0,0,e_v2.x,e_v2.y,e_v2.z};
		float	l_f3[6] = {0,0,0,l_vD3.x,l_vD3.y,l_vD3.z};
		glEnable2D(640,480,0,true);
		GLRender::RenderLine(l_f1,2,Vector4(1,0,0,1),3,e_mat);
		GLRender::RenderLine(l_f2,2,Vector4(0,1,0,1),3,e_mat);
		GLRender::RenderLine(l_f3,2,Vector4(0,0,1,1),3,e_mat);
		glDisable2D();	
	}

	void	RenderCube(Vector3 e_vSize,cMatrix44 e_mat,Vector4 e_vColor)
	{
				//2,3				5
				//0					1,4
		UseShaderProgram(NO_TEXTURE_SHADER);
		FATMING_CORE::SetupShaderWorldMatrix(e_mat);
		FATMING_CORE::SetupShaderColor(e_vColor);
		Vector4	l_vColor[36];
		Vector3	l_vVetrices[] = {
			//
			Vector3(-e_vSize.x, e_vSize.y, e_vSize.z),					// Bottom Left Of The Quad (Top)
			Vector3( e_vSize.x, e_vSize.y, e_vSize.z),					// Bottom Right Of The Quad (Top)
			Vector3(-e_vSize.x, e_vSize.y,-e_vSize.z),					// Top Left Of The Quad (Top)

			Vector3(-e_vSize.x, e_vSize.y,-e_vSize.z),					// Top Left Of The Quad (Top)
			Vector3( e_vSize.x, e_vSize.y, e_vSize.z),					// Bottom Right Of The Quad (Top)
			Vector3( e_vSize.x, e_vSize.y,-e_vSize.z),					// Top Right Of The Quad (Top)
			//
			Vector3(-e_vSize.x,e_vSize.y,e_vSize.z),					// Bottom Left Of The Quad (Bottom)
			Vector3( e_vSize.x,e_vSize.y,e_vSize.z),					// Bottom Right Of The Quad (Bottom)
			Vector3(-e_vSize.x,e_vSize.y, -e_vSize.z),					// Top Left Of The Quad (Bottom)
			Vector3(-e_vSize.x,e_vSize.y, -e_vSize.z),					// Top Left Of The Quad (Bottom)
			Vector3( e_vSize.x,e_vSize.y,e_vSize.z),					// Bottom Right Of The Quad (Bottom)
			Vector3( e_vSize.x,e_vSize.y, -e_vSize.z),					// Top Right Of The Quad (Bottom)
			//
			Vector3(-e_vSize.x,-e_vSize.y, e_vSize.z),					// Bottom Left Of The Quad (Front)
			Vector3( e_vSize.x,-e_vSize.y, e_vSize.z),					// Bottom Right Of The Quad (Front)
			Vector3(-e_vSize.x, e_vSize.y, e_vSize.z),					// Top Left Of The Quad (Front)
			Vector3(-e_vSize.x, e_vSize.y, e_vSize.z),					// Top Left Of The Quad (Front)
			Vector3( e_vSize.x,-e_vSize.y, e_vSize.z),					// Bottom Right Of The Quad (Front)
			Vector3( e_vSize.x, e_vSize.y, e_vSize.z),					// Top Right Of The Quad (Front)
			
			//
			Vector3(-e_vSize.x, -e_vSize.y,-e_vSize.z),					// Bottom Left Of The Quad (Back)
			Vector3( e_vSize.x, -e_vSize.y,-e_vSize.z),					// Bottom Right Of The Quad (Back)
			Vector3(-e_vSize.x,e_vSize.y,-e_vSize.z),					// Top Left Of The Quad (Back)
			Vector3(-e_vSize.x,e_vSize.y,-e_vSize.z),					// Top Left Of The Quad (Back)
			Vector3( e_vSize.x,-e_vSize.y,-e_vSize.z),					// Bottom Right Of The Quad (Back)
			Vector3( e_vSize.x,e_vSize.y,-e_vSize.z),					// Top Right Of The Quad (Back)
			
			//
			Vector3(-e_vSize.x,-e_vSize.y, e_vSize.z),					// Bottom Left Of The Quad (Left)
			Vector3(-e_vSize.x,-e_vSize.y,-e_vSize.z),					// Bottom Right Of The Quad (Left)
			Vector3(-e_vSize.x, e_vSize.y, e_vSize.z),					// Top Left Of The Quad (Left)
			Vector3(-e_vSize.x, e_vSize.y, e_vSize.z),					// Top Left Of The Quad (Left)
			Vector3(-e_vSize.x,-e_vSize.y,-e_vSize.z),					// Bottom Right Of The Quad (Left)
			Vector3(-e_vSize.x, e_vSize.y,-e_vSize.z),					// Top Right Of The Quad (Left)
			
			//
			Vector3( e_vSize.x,-e_vSize.y,-e_vSize.z),					// Bottom Left Of The Quad (Right)
			Vector3( e_vSize.x,-e_vSize.y, e_vSize.z),					// Bottom Right Of The Quad (Right)
			Vector3( e_vSize.x, e_vSize.y,-e_vSize.z),					// Top Left Of The Quad (Right)
			Vector3( e_vSize.x, e_vSize.y,-e_vSize.z),					// Top Left Of The Quad (Right)
			Vector3( e_vSize.x,-e_vSize.y, e_vSize.z),					// Bottom Right Of The Quad (Right)
			Vector3( e_vSize.x, e_vSize.y, e_vSize.z),					// Top Right Of The Quad (Right)
			
		};
		myGlVertexPointer(3,l_vVetrices);
		MY_GLDRAW_ARRAYS(GL_TRIANGLES, 0, 36);
	}
//end namespace GLRender
};