#ifndef	_TERRIAN_VERTEX_H_
#define	_TERRIAN_VERTEX_H_

#include "../OpenGL/GLSL/Shader.h"
#include "../Image/SimplePrimitive.h"
namespace FATMING_CORE
{
	class	cVBOBuffer;
//http://stackoverflow.com/questions/13983189/opengl-how-to-calculate-normals-in-a-terrain-height-grid
// # P.xy store the position for which we want to calculate the normals
// # height() here is a function that return the height at a point in the terrain
// read neightbor heights using an arbitrary small offset
//vec3 off = vec3(1.0, 1.0, 0.0);
//float hL = height(P.xy - off.xz);
//float hR = height(P.xy + off.xz);
//float hD = height(P.xy - off.zy);
//float hU = height(P.xy + off.zy);
//// deduce terrain normal
//N.x = hL - hR;
//N.y = hD - hU;
//N.z = 2.0;
//N = normalize(N);

//FVF_POS
//FVF_NORMAL
//FVF_TEX0


//cCurve		cCurve::GetInterplotValue(Vector3 e_vStart,Vector3 e_vEnd,int e_iLOD,float e_fInterpolationValue,Vector3*e_vResult)
//above function could be a gradually value change by smooth LERP.
	class	cTerrianVertex:public Frame
	{
		cVBOBuffer*m_pVBOBuffer;
		//
		//now for fast dev I do using this format
		//struct	sVertexFormat
		//{
		//	Vector3	vPos;
		//	Vector3	vNormal;
		//	Vector2	vUV;
		//	sVertexFormat(){ vPos = Vector3::Zero;vUV.x = vUV.y = 0;vNormal = Vector3::ZAxis; }
		//};
		float*			m_ppfVerticesBuffer[TOTAL_FVF];
		int64			m_i64FVFFormat;//default is position,normal,UV.
		int				m_iStride;
		//
		UINT			m_uiIndexBufferCount;
	#ifdef WIN32
		unsigned int*	m_puiIndexBuffer;
	#else
		unsigned short*	m_puiIndexBuffer;
	#endif
		//m_piVectexIndexData count is m_iGridWidth*m_iGridHeight
		//
		int			m_iGridWidth;
		int			m_iGridHeight;
		//Vector3		m_vScale;		//real world size,original grid is restrict at 0,1
		//
		void		GenerateEmptyData(int e_iWidth,int e_iHeight,int e_iFVF);
		void		CalculateNormal(POINT e_iStart,POINT e_iEnd);
		void		CalculateNormal();
		void		Destroy();
	public:
		cTerrianVertex(int e_iGridWidth,int e_iGridHeight,bool e_bUseVBO,int e_iInputImageWidth,int e_iInputImageHeight,int64 e_i64FVF = COMMON_2D_IMAGE_FVF_FLAG);
		//cTerrianVertex(int e_iGridWidth,int e_iGridHeight,bool e_bUseVBO,int64 e_i64FVF = COMMON_MESH_FVF_FLAG);
		~cTerrianVertex();
		//ensure size of input data is match gridwidth and geidheight.
		//for image color convert
		void		UpdateVertexHeight(unsigned char*e_pData,int e_iStartX,int e_iStartY,int e_iEffectedWidth,int e_iEffectedHeight,int e_iChannel);
		//for new position data
		void		UpdateVertexHeight(float*e_pData,int e_iStartX,int e_iStartY,int e_iEffectedWidth,int e_iEffectedHeight);
		//void	AdjustVertexFormat(int e_iFormat);,do it later
		void		HeightChangeBySphere(POINT e_CenterPos,int e_iRadius,float e_fCenterHeight,float e_fEdgeHeight,float e_fReduceValue);
		bool		SetSize(int e_iWidth,int e_iHeight,bool e_bUseVBO,int e_iImageWidth,int e_iImageHeight);
		//void		SetSize(int e_iWidth,int e_iHeight,int e_iFVF,int e_iImageWidth,,int e_iImageHeight);
		int			GetGridWidth(){return m_iGridWidth;}
		int			GetGridHeight(){return m_iGridHeight;}
		void		SetSizeAndvertexData(int e_iWidth,int e_iHeight,float*e_pfVertexData);
		void		Render(int e_iTextureID,const wchar_t*e_strShaderName = STATIC_MESH_SHADER);
		void		DebugRender(int e_iTextureID,const wchar_t*e_strShaderName = STATIC_MESH_SHADER);
	};
//end FATMING_CORE
}
//end _TERRIAN_VERTEX_H_
#endif