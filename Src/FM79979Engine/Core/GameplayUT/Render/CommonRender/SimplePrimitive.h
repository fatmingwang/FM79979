#pragma once
#include "../../../AllMathInclude.h"
#include "../Texture/Texture.h"
#include "../../OpenGL/GLSL/Shader.h"
using namespace FATMING_CORE;
namespace FATMING_CORE
{
	//from 2DShader.cpp
	extern GLuint	g_uiAttribArray[TOTAL_FVF];//TOTAL_FVF
	extern wchar_t*	DEFAULT_SHADER;
}

//static float		g_f2DTextureOriginalCoordinateBuffer[8] = { 0,0,1,0,0,1,1,1 };//texture coordinate
//static float		g_f2DTextureOriginalCoordinateMirrorBuffer[8] = {1,0,0,0,1,1,0,1};
//static float		g_f2DColorOne[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
//static float		g_f2DTextureCoordinate[8] = {1,1,1,1,1,1,1,1};
//static Vector3		g_v2DVertexBuffer[4]={ Vector3::Zero,Vector3::Zero,Vector3::Zero,Vector3::Zero };
//static Vector4		g_f2DColor[4]={Vector4::Zero,Vector4::Zero,Vector4::Zero,Vector4::Zero};
//for the object need a lot buffer
static float		g_fGlobalTempBufferForRenderVertices[640*2*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];//7.5k
static float		g_fGlobalTempBufferForRenderUV[640*2*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];//7.5k
static float		g_fGlobalTempBufferForRenderColor[640*4*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];//15k

//below function should input shader's info to find out glsl version
//then use glBindBufferBase or old style glSomethingPointer
void	ASSIGN_2D_QUAD_COLOR(Vector4 Color);
void	ASSIGN_2D_QUAD_VerticesBySize(float Width,float Height,float Depth);
void	ASSIGN_2D_QUAD_UV(float*UV);
void	ASSIGN_2D_QUAD_MIRROR_UV(float* UV);
void	myGLBlendFunc(GLenum e_Src, GLenum e_Dest);
#ifdef DEBUG
void	myGlVertexPointer(int Stride, const GLvoid*pData);
void	myGlUVPointer(int Stride, const GLvoid*pData);
void	myGlColorPointer(int Stride, const GLvoid*pData);
void	myGlNormalPointer(int Stride, const GLvoid*pData);
void	myVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
#else
#define	myGlVertexPointer(Stride,pData)										glVertexAttribPointer(g_uiAttribArray[FVF_POS], Stride, GL_FLOAT, 0, 0, pData)
#define	myGlUVPointer(Stride,pData)											glVertexAttribPointer(g_uiAttribArray[FVF_TEX0], Stride, GL_FLOAT, 0, 0, pData)
#define	myGlColorPointer(Stride,pData)										glVertexAttribPointer(g_uiAttribArray[FVF_DIFFUSE], Stride, GL_FLOAT, 0, 0, pData)
#define	myGlNormalPointer(Stride,pData)										glVertexAttribPointer(g_uiAttribArray[FVF_NORMAL], Stride, GL_FLOAT, 0, 0, pData);
#define	myVertexAttribPointer(index,size,type,normalized,stride,pointer);	glVertexAttribPointer(index,size,type,normalized,stride,pointer);
#endif
#ifndef DEBUG
#define	MY_GLDRAW_ARRAYS(mode, first,count)	glDrawArrays(mode, first,count)
#define	MY_GLDRAW_ELEMENTS(mode,count,type,indices)	glDrawElements(mode,count,type,indices)
#else
void	MY_GLDRAW_ARRAYS(GLenum mode, GLint first, GLsizei count);
void	MY_GLDRAW_ELEMENTS(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
#endif
namespace GLRender
{
	//before call any 2d image function must betwnne glEnable2D glDisable2D,if GLSL here need a Matrix to setup ViewProjection matrix,fixed pipeline won't return any value.
	//2D y axis down is positive up is negative.
	void	glEnable2D(float e_fResolutionWidth,float e_fResolutionHeight,float*e_pfMatrix = 0,bool e_bInvertYAxis = false);
	void	glDisable2D();
	//
	void	RenderQuadWithTextureAndColorAndCoordinate(GLfloat e_iX,GLfloat e_iY,GLfloat e_fDepth,GLfloat e_iWidth,GLfloat e_iHeight,Vector4 e_vColor,float *e_pfTexCoordinate,Vector3 e_vRotationAngle = Vector3::Zero,const wchar_t*e_strShaderName = DEFAULT_SHADER);
	void	RenderMirrorQuadWithTextureAndColorAndCoordinate(GLfloat e_iX,GLfloat e_iY,GLfloat e_fDepth,GLint e_iWidth,GLint e_iHeight,Vector4 e_vColor,float *e_pfTexCoordinate,float e_fRotationAngle = 0.f);
    //current it only works for 2D(for MPDI)
	void    RenderQuadWithMatrix(float*e_pfVertices,float*e_pfTextureUV,Vector4 e_vColor,float*e_pfMatrix,int e_iPosStride,int e_iNumQuad,const wchar_t*e_strShaderName = DEFAULT_SHADER);
	void    RenderTrianglesWithMatrix(float*e_pfVertices, float*e_pfTextureUV, float*e_pvColor, float*e_pfMatrix, int e_iPosStride, int e_iNumTriangles, const wchar_t*e_strShaderName = DEFAULT_SHADER);
	//
	void	RenderVertexByIndexBuffer(cMatrix44 e_Mat, int e_iPosStride, float*e_pVertexBuffer, float*e_pUVBuffer, float*e_pColorBuffer, void*e_pIndexBuffer, int e_iIndexBufferCount, const wchar_t*e_strShaderName = DEFAULT_SHADER);
	//
	void    GetDrawQuadFVFDataByTriangleStrip(GLint e_iWidth,GLint e_iHeight,float *e_pfTexCoordinate,Vector4 e_vColor,float*e_pfVertices,float*e_pfUV,float*e_pColor,cMatrix44*e_pmat);
	void    GetDrawQuadFVFDataByTwoTriangles(GLint e_iWidth,GLint e_iHeight,float *e_pfTexCoordinate,Vector4 e_vColor,float*e_pfVertices,float*e_pfUV,float*e_pColor,cMatrix44*e_pmat);
	//2point,P for start position,D for destination,
	//Vector3	l_vPoint;
	//l_vPoint.x = 0;
	//l_vPoint.y = 0;
	//l_vPoint.z = 0;
	//Vector3	l_vDirection;
	//l_vDirection.x = 1920;
	//l_vDirection.y = 1080;
	//l_vDirection.z = 0;
	//RenderArrow(l_vPoint,l_vDirection,30,0xffffffff);
	//u have to disable client state or it may occor crash
	void	RenderArrow(Vector3 P,Vector3 D, float radius,Vector4 e_vColor);
	void	Render3DArrow(Vector3 P,Vector3 D,Vector4 e_vColor = Vector4::One,float e_fWidth = 8);
	//if you want enable point size please write shader(gl_PointSize) and enable GL_VERTEX_PROGRAM_POINT_SIZE
	void	RenderPoint(Vector2	e_vPos,float e_fSize = 15,Vector4 e_vColor = Vector4::One);
	void	RenderPoint(Vector3	e_vPos,float e_fSize = 15,Vector4 e_vColor = Vector4::One);
	void	RenderPoints(Vector3*e_vPos,int e_iNum,float e_fSize = 15,Vector4 e_vColor = Vector4::One,float*e_pfMatrix = cMatrix44::Identity);
	void	RenderPoints(Vector2*e_vPos,int e_iNum,float e_fSize = 15,Vector4 e_vColor = Vector4::One,float*e_pfMatrix = cMatrix44::Identity);
	//4th element won't show,because I have no idea how to show 4th space
	void	RenderPoints(Vector4*e_vPos,int e_iNum,float e_fSize = 15,Vector4 e_vColor = Vector4::One,float*e_pfMatrix = cMatrix44::Identity);
	void	RenderLine(float*e_pPosition,int e_iNum,Vector4 e_vColor,int e_iStride,float*e_pfMatrix = cMatrix44::Identity,float e_fLineSize = 2,const wchar_t*e_strShaderName = NO_TEXTURE_SHADER);
	void	RenderLine(std::vector<Vector2>* e_pPositionPoint,Vector4 e_vColor,float*e_pfMatrix = cMatrix44::Identity,bool e_bRenderPoints = false,bool e_bRenderIndexFont = false,const wchar_t*e_strShaderName = NO_TEXTURE_SHADER);
	void	RenderLine(std::vector<Vector3>* e_pPositionPoint,Vector4 e_vColor,float*e_pfMatrix = cMatrix44::Identity,bool e_bRenderPoints = false,bool e_bRenderIndexFont = false,const wchar_t*e_strShaderName = NO_TEXTURE_SHADER);
	//this one will strip 4th element,because I have no idea how to show 4th space
	void	RenderLine(std::vector<Vector4>* e_pPositionPoint,Vector4 e_vColor,float*e_pfMatrix = cMatrix44::Identity,bool e_bRenderPoints = false,bool e_bRenderIndexFont = false,const wchar_t*e_strShaderName = NO_TEXTURE_SHADER);
	//e_fDensityAngle get smaller get smooth
	//u have to multiply matrx as u want
	//bad performance,here should instead by a Sphere class
	void	RenderSphere(Vector2 e_vPos,float e_fRadius,Vector4 e_vColor = Vector4(0.f,1.f,0.f,1.f),bool e_bDrawPoint = false,float e_fDensityAngle = 5.f);
	void	RenderSphere(cMatrix44 e_mat,float e_fRadius,Vector4 e_vColor = Vector4(0.f,1.f,0.f,1.f),bool e_bDrawPoint = false,float e_fDensityAngle = 5.f);
	//draw by center orientation.
	//a 50,50 RECT left up is -25,-25
	void	RenderRectangle(POINT e_Pos,int e_iWidth,int e_iHeight,Vector4 e_vColor,float e_fAngle = 0.f,float e_fLineWidth = 3);
	void	RenderRectangle(Vector2 e_Pos,float e_fWidth,float e_fHeight,Vector4 e_vColor,float e_fAngle,float e_fLineWidth = 3);
	void	RenderRectangle(Vector2 e_Pos,float e_fWidth,float e_fHeight,Vector4 e_vColor,Vector3 e_vAngle = Vector3::Zero,float e_fLineWidth = 3);
	void	RenderRectangle(Vector3 e_Pos,float e_fWidth,float e_fHeight,Vector4 e_vColor,Vector3 e_vAngle = Vector3::Zero,float e_fLineWidth = 3);
	void	RenderRectangle(float e_fWidth,float e_fHeight,cMatrix44 e_Mat,Vector4 e_vColor,float e_fLineWidth = 3);
	void	RenderFilledRectangle(Vector2 e_Pos,float e_fWidth,float e_fHeight,Vector4 e_vColor,float e_fAngle);

	void	RenderCube(Vector3 e_vSize,cMatrix44 e_mat,Vector4 e_vColor);
	void	RenderPlane(cMatrix44 e_mat,float	e_fWidth = 100,float	e_fHeight = 100,float e_fGridWidthDistance = 10,float e_fGridHeightDistance = 10,float e_fLineWidth = 7.f,Vector4 e_vColor = Vector4(1,1,1,1));
	//
	void	RenderVectorProject(Vector3 e_v1,Vector3 e_v2,cMatrix44 e_mat = cMatrix44::TranslationMatrix(Vector3(300,-300,0)));
	//frome UV(left up right buttom) to 2 triangles vertex
	void	AssignUVDataTo2Triangles(float*e_pSrc,float*e_pDest,bool e_bMirror);
	//frome UV(left up right buttom) to  triangle strip vertex
	void	AssignUVDataToTriangleStrip(float*e_pSrc,float*e_pDest,bool e_bMirror);
	void	Assign4VerticesDataTo2Triangles(float*e_pSrc,float*e_pDest,int e_iStride);
	//e_pUVSrc is left,up right down.cBaseImage::fUV[4]
	void	Assign4VerticesDataTo2Triangles(float*e_pVertexSrc,float*e_pVertexDest,float*e_pUVSrc,float*e_pUVDest,int e_iStride);


	struct sBlendfunction
	{
		sBlendfunction(){}
		sBlendfunction(GLenum e_eSrcBlendingMode, GLenum e_eDestBlendingMode)
		{
			eSrcBlendingMode = e_eSrcBlendingMode;
			eDestBlendingMode = e_eDestBlendingMode;
		}
		GLint iColorParameter[4];
		GLenum	eSrcBlendingMode = GL_SRC_ALPHA;
		GLenum	eDestBlendingMode = GL_ONE_MINUS_SRC_ALPHA;
		static	GLenum	eLastSrcBlendingMode;
		static	GLenum	eLastDestBlendingMode;
		bool	bDoRestore;
		void	GetStatus();
		void	Render();
		void	Restore();
	};
};
using namespace GLRender;