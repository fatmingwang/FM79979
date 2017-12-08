#include "../stdafx.h"
#include "TerrianVertex.h"
#include "../Image/VertexBufferObject.h"
namespace FATMING_CORE
{
	//from 2DShader.cpp
	extern GLuint	g_uiAttribArray[9];//TOTAL_FVF
	cTerrianVertex::cTerrianVertex(int e_iGridWidth,int e_iGridHeight,bool e_bUseVBO,int e_iInputImageWidth,int e_iInputImageHeight,int64 e_iFVF)
	{
		m_uiIndexBufferCount = 0;
		m_iStride = 0;
		m_i64FVFFormat = e_iFVF;
		m_iStride = GetStrideByFVF(e_iFVF);
		m_puiIndexBuffer = 0;
		m_iGridWidth = 0;
		m_iGridHeight = 0;
		m_pVBOBuffer = 0;
		for( int i=0;i<TOTAL_FVF;++i )
			m_ppfVerticesBuffer[i] = 0;
		SetSize(e_iGridWidth,e_iGridHeight,e_bUseVBO,e_iInputImageWidth,e_iInputImageHeight);
	}

	cTerrianVertex::~cTerrianVertex()
	{
		Destroy();
	}

	void	cTerrianVertex::Destroy()
	{
		SAFE_DELETE(m_puiIndexBuffer);
		SAFE_DELETE(m_pVBOBuffer);
		for( int i=0;i<TOTAL_FVF;++i )
			SAFE_DELETE(m_ppfVerticesBuffer[i]);
	}

	void	cTerrianVertex::UpdateVertexHeight(float*e_pData,int e_iStartX,int e_iStartY,int e_iEffectedWidth,int e_iEffectedHeight)
	{
		if(  e_iStartX >= this->m_iGridWidth || e_iStartY >= this->m_iGridHeight  )
			return;
		if( e_iStartX < 0 )
		{
			e_iEffectedWidth += e_iStartX;
			e_iStartX = 0;
		}
		if( e_iStartY < 0 )
		{
			e_iEffectedHeight += e_iStartY;
			e_iStartY = 0;
		}
		if( m_ppfVerticesBuffer[FVF_POS] )
		{
			for( int i=e_iStartX;i<e_iEffectedWidth;++i )
			{
				for( int j=e_iStartY;j<e_iEffectedHeight;++j )
				{
					int	l_iIndex = (i+(j*this->m_iGridWidth));
					float	*l_pfHeight = &m_ppfVerticesBuffer[FVF_POS][l_iIndex*3+2];//x,y,z
					*l_pfHeight = e_pData[l_iIndex];
				}
			}
		}
		if( m_pVBOBuffer )
		{
			int	l_iGrideSize = m_iGridWidth*m_iGridHeight;
			m_pVBOBuffer->SetupVerticesBuffer(m_ppfVerticesBuffer[FVF_POS],FVF_POS,g_iFVF_DataStride[FVF_POS],l_iGrideSize);
		}	
	}

	void		cTerrianVertex::UpdateVertexHeight(unsigned char*e_pData,int e_iStartX,int e_iStartY,int e_iEffectedWidth,int e_iEffectedHeight,int e_iChannel)
	{
		int	l_iStride = e_iChannel*sizeof(char);
		//int	l_iStartIndex = e_iStartX+(e_iStartY*this->m_iGridWidth);
		//char*l_pStartData = e_pData+(l_iStartIndex*l_iStride);
		if(  e_iStartX >= this->m_iGridWidth || e_iStartY >= this->m_iGridHeight  )
			return;
		if( e_iStartX < 0 )
		{
			e_iEffectedWidth += e_iStartX;
			e_iStartX = 0;
		}
		if( e_iStartY < 0 )
		{
			e_iEffectedHeight += e_iStartY;
			e_iStartY = 0;
		}
		//e_iEffectedWidth += e_iStartX;if( e_iEffectedWidth>m_iGridWidth )e_iEffectedWidth = m_iGridWidth;
		//e_iEffectedHeight += e_iStartY;if( e_iEffectedHeight>m_iGridHeight )e_iEffectedHeight= m_iGridHeight;
		//011111110111111101111111 = 8355711
		//int	l_iZeroHeight = 8355711/100;
		if( m_ppfVerticesBuffer[FVF_POS] )
		{
			for( int i=e_iStartX;i<e_iEffectedWidth;++i )
			{
				for( int j=e_iStartY;j<e_iEffectedHeight;++j )
				{
					int	l_iIndex = (i+(j*this->m_iGridWidth));
					unsigned char*l_pTargetData = &e_pData[l_iIndex*l_iStride];
					//argb in dot net
					float	*l_pfHeight = &m_ppfVerticesBuffer[FVF_POS][l_iIndex*3+2];//x,y,z
					*l_pfHeight = (float)(l_pTargetData[0]-0xff/2);
					//unsigned char	l_R = l_pTargetData[0];
					//unsigned char	l_G = l_pTargetData[1];
					//unsigned char	l_B = l_pTargetData[2];
					//unsigned char	l_A = l_pTargetData[3];
					//int64	l_iColor = BitMoveLeft(l_R,16);
					//l_iColor += BitMoveLeft(l_G,8)+l_B;
					//l_iColor -= l_iZeroHeight;
					//*l_pfHeight = (float)l_iColor;
					//if( l_iColor != 0 )
					//{
					//	int a=0;
					//}
					
				}
			}
		}
		//do smooth and update normal
		//update VBO
		if( m_pVBOBuffer )
		{
			int	l_iGrideSize = m_iGridWidth*m_iGridHeight;
			m_pVBOBuffer->SetupVerticesBuffer(m_ppfVerticesBuffer[FVF_POS],FVF_POS,g_iFVF_DataStride[FVF_POS],l_iGrideSize);
		}
	}
	//void	AdjustVertexFormat(int e_iFormat);,do it later
	void		cTerrianVertex::HeightChangeBySphere(POINT e_CenterPos,int e_iRadius,float e_fCenterHeight,float e_fEdgeHeight,float e_fReduceValue)
	{

	}


	bool		cTerrianVertex::SetSize(int e_iWidth,int e_iHeight,bool e_bUseVBO,int e_iImageWidth,int e_iImageHeight)
	{
		if( e_iWidth < 2 || e_iHeight< 2)
			return false;
		if( m_iGridWidth != e_iWidth || m_iGridWidth != e_iHeight )
		{
			Destroy();
			if( e_bUseVBO )
				m_pVBOBuffer = new cVBOBuffer();
			m_iGridWidth = e_iWidth;
			m_iGridHeight = e_iHeight;
			int	l_iGrideSize = m_iGridWidth*m_iGridHeight;
			int	l_iVertexSize = l_iGrideSize*m_iStride*sizeof(float);

			for( int i=0;i<TOTAL_FVF;++i )
			{
				if( 1<<i & (int)m_i64FVFFormat )
				{
					int	l_iSize = l_iGrideSize*sizeof(float)*g_iFVF_DataStride[i];
					char*l_pData = new char[l_iSize];
					m_ppfVerticesBuffer[i] = (float*)l_pData;
					memset(m_ppfVerticesBuffer[i],0,l_iSize);
				}
			}
			//each grid has 2 triangles 
			//one triangle 3 points
			m_uiIndexBufferCount = (m_iGridWidth-1)*(m_iGridHeight-1)*2*3;
#ifdef WIN32
			m_puiIndexBuffer = new unsigned int[m_uiIndexBufferCount];
#else
			m_puiIndexBuffer = new unsigned short[m_uiIndexBufferCount];
#endif
			float	l_fUStep = 1.f/(m_iGridWidth-1);
			float	l_fVStep = 1.f/(m_iGridHeight-1);
			if( !g_bSupportNonPowerOfTwoTexture )
			{
				int	l_iPow2Width = UT::power_of_two(e_iImageWidth);
				int	l_iPow2Height = UT::power_of_two(e_iImageHeight);
				float	l_fUScale = (float)e_iImageWidth/l_iPow2Width;
				float	l_fVScale = (float)e_iImageHeight/l_iPow2Height;
				l_fUStep *= l_fUScale;
				l_fVStep *= l_fVScale;
			}
			float	l_fXStep = 1.f;
			float	l_fYStep = 1.f;
			assert(COMMON_2D_IMAGE_FVF_FLAG == m_i64FVFFormat&&"sorry now only support COMMON_2D_IMAGE_FVF_FLAG");
			Vector3*l_pPosVertexData = (Vector3*)m_ppfVerticesBuffer[FVF_POS];
			Vector3*l_pNormalVertexData = (Vector3*)m_ppfVerticesBuffer[FVF_NORMAL];
			Vector2*l_pTextureUVData = (Vector2*)m_ppfVerticesBuffer[FVF_TEX0];
			for( int i=0;i<m_iGridHeight;++i )
			{
				for( int j=0;j<m_iGridWidth;++j )
				{
					l_pPosVertexData->x = j*l_fXStep;
					l_pPosVertexData->y = i*l_fYStep;
					l_pPosVertexData->z = 0.f;
					++l_pPosVertexData;
					if( l_pNormalVertexData )
					{
						*l_pNormalVertexData = Vector3(Vector3::ZAxis);
						++l_pNormalVertexData;
					}
					l_pTextureUVData->x = j*l_fUStep;
					l_pTextureUVData->y = i*l_fVStep;
					++l_pTextureUVData;
				}
			}
			int	l_iIndexBuffer = 0;
			for( int i=0;i<m_iGridWidth-1;++i )
			{
				for( int j=1;j<m_iGridHeight;++j )
				{
					int	l_iTriangleIndex1 = i+m_iGridWidth*j;
					int	l_iTriangleIndex2 = l_iTriangleIndex1+1;
					int	l_iTriangleIndex3 = l_iTriangleIndex1-m_iGridWidth;
					m_puiIndexBuffer[l_iIndexBuffer] = l_iTriangleIndex1;++l_iIndexBuffer;
					m_puiIndexBuffer[l_iIndexBuffer] = l_iTriangleIndex2;++l_iIndexBuffer;
					m_puiIndexBuffer[l_iIndexBuffer] = l_iTriangleIndex3;++l_iIndexBuffer;
					int	l_iTriangle2Index1 = l_iTriangleIndex3;
					int	l_iTriangle2Index2 = l_iTriangleIndex2;
					int	l_iTriangle2Index3 = l_iTriangleIndex3+1;
					//debug code.
					//sVertexFormat*l_pVertexFormat = (sVertexFormat*)m_pfVertex;
					//Vector3	l_vPos1 = l_pVertexFormat[l_iTriangleIndex1].vPos;
					//Vector3	l_vPos2 = l_pVertexFormat[l_iTriangleIndex2].vPos;
					//Vector3	l_vPos3 = l_pVertexFormat[l_iTriangleIndex3].vPos;
					//Vector3	l_v2Pos1 = l_pVertexFormat[l_iTriangle2Index1].vPos;
					//Vector3	l_v2Pos2 = l_pVertexFormat[l_iTriangle2Index2].vPos;
					//Vector3	l_v2Pos3 = l_pVertexFormat[l_iTriangle2Index3].vPos;
					m_puiIndexBuffer[l_iIndexBuffer] = l_iTriangle2Index1;++l_iIndexBuffer;
					m_puiIndexBuffer[l_iIndexBuffer] = l_iTriangle2Index2;++l_iIndexBuffer;
					m_puiIndexBuffer[l_iIndexBuffer] = l_iTriangle2Index3;++l_iIndexBuffer;
				}
			}
			//assign position,normal,UV
			if( m_pVBOBuffer )
			{
				m_pVBOBuffer->SetupVerticesBuffer(m_ppfVerticesBuffer[FVF_POS],FVF_POS,g_iFVF_DataStride[FVF_POS],l_iGrideSize);
				if( m_ppfVerticesBuffer[FVF_NORMAL] )
					m_pVBOBuffer->SetupVerticesBuffer(m_ppfVerticesBuffer[FVF_NORMAL],FVF_NORMAL,g_iFVF_DataStride[FVF_NORMAL],l_iGrideSize);
				m_pVBOBuffer->SetupVerticesBuffer(m_ppfVerticesBuffer[FVF_TEX0],FVF_TEX0,g_iFVF_DataStride[FVF_TEX0],l_iGrideSize);
				m_pVBOBuffer->SetupIndicesBuffer((float*)m_puiIndexBuffer,m_uiIndexBufferCount);
				//no need anymore,because use VBO
				SAFE_DELETE(m_puiIndexBuffer);
			}
			//
			return true;
		}
		return false;
	}
	void		cTerrianVertex::GenerateEmptyData(int e_iWidth,int e_iHeight,int e_iFVF)
	{

	}
	void		cTerrianVertex::CalculateNormal(POINT e_iStart,POINT e_iEnd)
	{

	}
	void		cTerrianVertex::CalculateNormal()
	{

	}

	//void		cTerrianVertex::SetSizeAndvertexData(int e_iWidth,int e_iHeight,float*e_pfVertexData,int e_iImageWidth,,int e_iImageHeight)
	//{

	//}

	void		cTerrianVertex::Render(int e_iTextureID,const wchar_t*e_strShaderName)
	{
		assert(COMMON_2D_IMAGE_FVF_FLAG == m_i64FVFFormat&&"sorry now only support COMMON_MESH_FVF_FLAG");
		UseShaderProgram(e_strShaderName);
		SetupShaderWorldMatrix(this->GetWorldTransform());
		glBindTexture( GL_TEXTURE_2D, e_iTextureID);
		if( m_pVBOBuffer )
		{
			m_pVBOBuffer->Render(false);
		}
		else
		{
			//DrawCube(Vector3(1,1,1),cMatrix44::Identity,Vector4(1,1,0,1));
			for( int i=0;i<TOTAL_FVF;++i )
			{
				if( this->m_ppfVerticesBuffer[i] && g_uiAttribArray[i] != -1 )
				{
					glVertexAttribPointer(g_uiAttribArray[i],g_iFVF_DataStride[i], g_iFVF_DataType[i],0, 0, m_ppfVerticesBuffer[i]);
				}
			}
			MY_GLDRAW_ELEMENTS(GL_TRIANGLES,m_uiIndexBufferCount, g_iDrawindiceType,m_puiIndexBuffer );
		}
	}

	void	cTerrianVertex::DebugRender(int e_iTextureID,const wchar_t*e_strShaderName)
	{
#ifdef WIN32
		// Turn on wireframe mode
		glPolygonMode(GL_FRONT, GL_LINE);
//		glPolygonMode(GL_BACK, GL_LINE);
		Render(e_iTextureID,e_strShaderName);
		// Turn off wireframe mode
		glPolygonMode(GL_FRONT, GL_FILL);
//		glPolygonMode(GL_BACK, GL_FILL);
#endif
	}
//end FATMING_CORE
}