#include "../stdafx.h"
#include "UVAnimationImage.h"
#include "SimplePrimitive.h"
namespace FATMING_CORE
{
	cUVAnimationImage::cUVAnimationImage(cBaseImage*e_pRenderImage,Vector2 e_vSize,eMoveDirection e_eOrientation,Vector2 e_vShowPos)
	{
		m_vShowPos = e_vShowPos;
		m_pRenderImage = e_pRenderImage;
		memcpy(m_fTargetUV,m_pRenderImage->GetUV(),sizeof(float)*4);
		m_vSize = e_vSize;
		m_eOrientation = e_eOrientation;
		m_vCurrentPos = Vector2::Zero;
	}

	cUVAnimationImage::~cUVAnimationImage()
	{
	
	}

	void	cUVAnimationImage::SetupRenderDataByCurrentPos()
	{
		if( !m_pRenderImage )
			return;
		float	l_fLength = 0.f;
		float	l_fGap = 0.f;
		float	l_fStartToEndDis = 0.f;
		float	l_fSecondPointDis = 0.f;
		switch(m_eOrientation)
		{
			case eMD_LEFT_TO_RIGHT:
			case eMD_RIGHT_TO_LEFT:
				//horizontal
				//0      2		4
				//
				//1      3		5
				//for UV
				l_fLength = m_fTargetUV[2]-m_fTargetUV[0];
				l_fGap = m_vCurrentPos.x/m_vSize.x;
				if( l_fGap > 0 )
					l_fGap = l_fGap-(int)l_fGap;
				else
					l_fGap = 1-(l_fGap-(int)l_fGap);
				m_vRenderVerticesUV[0].x = m_vRenderVerticesUV[1].x = (l_fGap*l_fLength)+m_fTargetUV[0];
				m_vRenderVerticesUV[2].x = m_vRenderVerticesUV[3].x = m_fTargetUV[2];
				m_vRenderVerticesUV[4].x = m_vRenderVerticesUV[5].x = m_vRenderVerticesUV[0].x;
				m_vRenderVerticesUV[0].y = m_vRenderVerticesUV[2].y = m_vRenderVerticesUV[4].y = m_fTargetUV[1];
				m_vRenderVerticesUV[1].y = m_vRenderVerticesUV[3].y = m_vRenderVerticesUV[5].y = m_fTargetUV[3];
				//for pos
				l_fStartToEndDis = m_fTargetUV[2]-m_vRenderVerticesUV[0].x;
				//for y
				m_vRenderVerticesPos[0].y = m_vRenderVerticesPos[2].y = m_vRenderVerticesPos[4].y = m_vShowPos.y;
				m_vRenderVerticesPos[1].y = m_vRenderVerticesPos[3].y = m_vRenderVerticesPos[5].y = m_vShowPos.y+m_vSize.y;
				//for x
				m_vRenderVerticesPos[0].x = m_vRenderVerticesPos[1].x = m_vShowPos.x;
				l_fSecondPointDis = m_vSize.x * (1-l_fGap);
				m_vRenderVerticesPos[2].x = m_vRenderVerticesPos[3].x = m_vRenderVerticesPos[0].x + l_fSecondPointDis;
				m_vRenderVerticesPos[4].x = m_vRenderVerticesPos[5].x = m_vShowPos.x+m_vSize.x;
				break;
			case eMD_UP_TO_DOWN:
			case eMD_DOWN_TO_UP:
				//vertical
				//for UV
				//0      1
				//
				//2      3
				//
				//4      5		
				l_fLength = m_fTargetUV[3]-m_fTargetUV[1];
				l_fGap = m_vCurrentPos.y/m_vSize.y;
				if( l_fGap > 0 )
					l_fGap = l_fGap-(int)l_fGap;
				else
					l_fGap = 1-(l_fGap-(int)l_fGap);
				m_vRenderVerticesUV[0].y = m_vRenderVerticesUV[1].y = (l_fGap*l_fLength)+m_fTargetUV[1];
				m_vRenderVerticesUV[2].y = m_vRenderVerticesUV[3].y = m_fTargetUV[3];
				m_vRenderVerticesUV[4].y = m_vRenderVerticesUV[5].y = m_vRenderVerticesUV[0].y;
				m_vRenderVerticesUV[0].x = m_vRenderVerticesUV[2].x = m_vRenderVerticesUV[4].x = m_fTargetUV[0];
				m_vRenderVerticesUV[1].x = m_vRenderVerticesUV[3].x = m_vRenderVerticesUV[5].x = m_fTargetUV[2];
				//for pos
				l_fStartToEndDis = m_fTargetUV[3]-m_vRenderVerticesUV[0].y;
				//for x
				m_vRenderVerticesPos[0].x = m_vRenderVerticesPos[2].x = m_vRenderVerticesPos[4].x = m_vShowPos.x;
				m_vRenderVerticesPos[1].x = m_vRenderVerticesPos[3].x = m_vRenderVerticesPos[5].x = m_vShowPos.x+m_vSize.x;
				//for y
				m_vRenderVerticesPos[0].y = m_vRenderVerticesPos[1].y = m_vShowPos.y;
				l_fSecondPointDis = m_vSize.y * (1-l_fGap);
				m_vRenderVerticesPos[2].y = m_vRenderVerticesPos[3].y = m_vRenderVerticesPos[0].y + l_fSecondPointDis;
				m_vRenderVerticesPos[4].y = m_vRenderVerticesPos[5].y = m_vShowPos.y+m_vSize.y;
				break;
			default:
				break;
		}
		for( int i=0;i<6;++i )
			m_vRenderVerticesPos[i].z = 0.f;
	}

	void	cUVAnimationImage::Update(float e_fElpaseTime,float e_fSpeed)
	{
		float	l_fMovDis = e_fSpeed*e_fElpaseTime;
		switch(m_eOrientation)
		{
			case eMD_LEFT_TO_RIGHT:
				m_vCurrentPos.x += l_fMovDis;
				break;
			case eMD_RIGHT_TO_LEFT:
				m_vCurrentPos.x -= l_fMovDis;
				break;
			case eMD_UP_TO_DOWN:
				m_vCurrentPos.y -= l_fMovDis;
				break;
			case eMD_DOWN_TO_UP:
				m_vCurrentPos.y += l_fMovDis;
				break;
			default:
				break;
		}
		SetupRenderDataByCurrentPos();
	}

	void	cUVAnimationImage::RenderByPos(Vector2 e_vPos)
	{
		m_vCurrentPos = e_vPos;
		SetupRenderDataByCurrentPos();
		Render();
	}

	void	cUVAnimationImage::Render()
	{
		if( m_pRenderImage )
			m_pRenderImage->ApplyImage();
		Vector4	l_vColor[12];
		Vector2	l_vUV[12];
		Vector3	l_vPos[12];
		for( int i=0;i<12;++i )
			l_vColor[i] = Vector4::One;

		float	l_fInputUV[4] = {m_vRenderVerticesUV[0].x,m_vRenderVerticesUV[0].y,m_vRenderVerticesUV[3].x,m_vRenderVerticesUV[3].y};
		Assign4VerticesDataTo2Triangles((float*)&m_vRenderVerticesPos,(float*)&l_vPos,(float*)&l_fInputUV,(float*)&l_vUV,3);
		float	l_fInputUV2[4] = {m_fTargetUV[0],m_fTargetUV[1],m_vRenderVerticesUV[5].x,m_vRenderVerticesUV[5].y};
		//2 is overlap start.
		Assign4VerticesDataTo2Triangles((float*)&m_vRenderVerticesPos[2],(float*)&l_vPos[6],(float*)&l_fInputUV2,(float*)&l_vUV[6],3);

		UseShaderProgram(DEFAULT_SHADER);
		SetupShaderWorldMatrix(cMatrix44::Identity);

		//test code
		//for( int i=6;i<12;++i )
		//	l_vColor[i] += Vector4::Blue;
		//for( int i=0;i<6;++i )
		//	l_vColor[i] += Vector4(0,0,0.5,1);
		//myGlVertexPointer(3,&l_vPos[0]);
		//myGlUVPointer(2,&l_vUV[0]);
		//myGlColorPointer(4,&l_vColor[0]);
		//MY_GLDRAW_ARRAYS(GL_TRIANGLES, 0, 6);

		//myGlVertexPointer(3,&l_vPos[6]);
		//myGlUVPointer(2,&l_vUV[6]);
		//myGlColorPointer(4,&l_vColor[6]);
		//MY_GLDRAW_ARRAYS(GL_TRIANGLES, 0, 6);
		
		myGlVertexPointer(3,&l_vPos[0]);
		myGlUVPointer(2,&l_vUV[0]);
		myGlColorPointer(4,&l_vColor[0]);
		MY_GLDRAW_ARRAYS(GL_TRIANGLES, 0, 12);
	}

	void	cUVAnimationImage::DebugRender()
	{
		std::vector<Vector3>l_vPoint;
		switch(m_eOrientation)
		{
			case eMD_LEFT_TO_RIGHT:
			case eMD_RIGHT_TO_LEFT:
				//horizontal
				//0      2		4
				//
				//1      3		5
				l_vPoint.push_back(m_vRenderVerticesPos[0]);
				l_vPoint.push_back(m_vRenderVerticesPos[1]);
				l_vPoint.push_back(m_vRenderVerticesPos[0]);
				l_vPoint.push_back(m_vRenderVerticesPos[2]);
				l_vPoint.push_back(m_vRenderVerticesPos[1]);
				l_vPoint.push_back(m_vRenderVerticesPos[3]);
				l_vPoint.push_back(m_vRenderVerticesPos[2]);
				l_vPoint.push_back(m_vRenderVerticesPos[3]);
				l_vPoint.push_back(m_vRenderVerticesPos[2]);
				l_vPoint.push_back(m_vRenderVerticesPos[4]);
				l_vPoint.push_back(m_vRenderVerticesPos[4]);
				l_vPoint.push_back(m_vRenderVerticesPos[5]);
				l_vPoint.push_back(m_vRenderVerticesPos[5]);
				l_vPoint.push_back(m_vRenderVerticesPos[3]);
				break;
			case eMD_UP_TO_DOWN:
			case eMD_DOWN_TO_UP:
				//vertical
				//for UV
				//0      1
				//
				//2      3
				//
				//4      5		
				l_vPoint.push_back(m_vRenderVerticesPos[0]);
				l_vPoint.push_back(m_vRenderVerticesPos[2]);
				l_vPoint.push_back(m_vRenderVerticesPos[4]);
				RenderLine(&l_vPoint,Vector4::Green,cMatrix44::Identity,true);
				l_vPoint.clear();
				l_vPoint.push_back(m_vRenderVerticesPos[5]);
				l_vPoint.push_back(m_vRenderVerticesPos[3]);
				l_vPoint.push_back(m_vRenderVerticesPos[1]);
				RenderLine(&l_vPoint,Vector4::Green,cMatrix44::Identity,true);
				l_vPoint.clear();
				l_vPoint.push_back(m_vRenderVerticesPos[0]);
				l_vPoint.push_back(m_vRenderVerticesPos[1]);
				RenderLine(&l_vPoint,Vector4::Red,cMatrix44::Identity,true);
				l_vPoint.clear();
				l_vPoint.push_back(m_vRenderVerticesPos[2]);
				l_vPoint.push_back(m_vRenderVerticesPos[3]);
				RenderLine(&l_vPoint,Vector4::Red,cMatrix44::Identity,true);
				l_vPoint.clear();
				l_vPoint.push_back(m_vRenderVerticesPos[4]);
				l_vPoint.push_back(m_vRenderVerticesPos[5]);
				RenderLine(&l_vPoint,Vector4::Red,cMatrix44::Identity,true);
				//l_vPoint.push_back(m_vRenderVerticesPos[0]);
				//l_vPoint.push_back(m_vRenderVerticesPos[1]);
				//l_vPoint.push_back(m_vRenderVerticesPos[0]);
				//l_vPoint.push_back(m_vRenderVerticesPos[2]);
				//l_vPoint.push_back(m_vRenderVerticesPos[1]);
				//l_vPoint.push_back(m_vRenderVerticesPos[3]);
				//l_vPoint.push_back(m_vRenderVerticesPos[2]);
				//l_vPoint.push_back(m_vRenderVerticesPos[3]);
				//l_vPoint.push_back(m_vRenderVerticesPos[2]);
				//l_vPoint.push_back(m_vRenderVerticesPos[4]);
				//l_vPoint.push_back(m_vRenderVerticesPos[4]);
				//l_vPoint.push_back(m_vRenderVerticesPos[5]);
				//l_vPoint.push_back(m_vRenderVerticesPos[5]);
				//l_vPoint.push_back(m_vRenderVerticesPos[3]);
				break;
			default:
				break;
		}
	}
//end namespace
}