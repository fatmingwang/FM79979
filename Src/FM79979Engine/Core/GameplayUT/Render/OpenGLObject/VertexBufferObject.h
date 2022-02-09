#pragma once

#include "../../OpenGL/Glh.h"
#include "../CommonRender/RenderObject.h"
namespace FATMING_CORE
{
	//==================================
	//ensure u have setup data order as 0,position 1,normal 2,color 3,tangent 4,bitangent 5texteure
	//==================================
	class	cVBOBuffer
	{
		GLuint	m_uiAllBufferID[TOTAL_FVF];
		int		m_iAllBufferStride[TOTAL_FVF];
		GLuint	m_uiDrawIndicesBufferID;
		uint32	m_uiIndicesCount;//for draw indices
		int		m_iVertexStride;
	public:
		cVBOBuffer();
		virtual ~cVBOBuffer();
		//enusre the count is calculate by float
		void	SetupVerticesBuffer(float*e_pfData,int e_iDataLocation,int e_iStride,UINT e_uiCount,int e_iDataSize = 4);//sizeof(float) = 4
		//hey ensure opengl ES index buffer size is unsigned short not not!!
		void	SetupIndicesBuffer(float*e_pfData,UINT e_uiCount,int e_iIndexSize = sizeof(unsigned int));
		//because the glVertexAttribPointer's parameter GLsizei stride

		//if e_pfData of SetupVerticesBuffer is independent e_benableDataStrideis false
		//ex:float*pfPositionData;float*pfNormalData;float*pfUVData;
		//
		//if e_pfData of SetupVerticesBuffer is a data from vertex data
		//ex:struct{Vector3 vPos;Vector3 vNormal;Vector2 fUV;};
		//
		void	Init();
		void	Render(bool e_benableDataStride);
		UINT	GetIndicexCount(){ return m_uiIndicesCount; }
		float*	GetData(int e_iDataLocation);
	};
}