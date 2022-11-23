#include "../Texture/Texture.h"
#include "../Texture/TextureManager.h"
#include "../../../XML/XMLLoader.h"
#include "BaseImage.h"
#include "SimplePrimitive.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cBaseImage);
	TYPDE_DEFINE_MARCO(cUIImage);
	//====================
	//
	//====================
	cBaseImage::cBaseImage(const char*e_strImageName,bool e_bFetchPixels)
	{
		m_pTexture = nullptr;
		m_iWidth = 0;
		m_iHeight = 0;
		ParseTexture(e_strImageName, e_bFetchPixels);
	}

	cBaseImage::cBaseImage(cBaseImage*e_pBaseImage):cRenderObject(e_pBaseImage),Frame(e_pBaseImage)
	{
		m_pTexture = nullptr;
		SetTexBehaviorData(e_pBaseImage);
	}

	bool	cBaseImage::ParseTexture(const char*e_strImageName, bool e_bFetchPixels)
	{
		if (e_strImageName)
		{
			this->SetName(e_strImageName);
			m_pTexture = cTextureManager::GetInstance()->GetObject(this, e_strImageName, e_bFetchPixels);
			if (m_pTexture)
			{
				m_iWidth = m_pTexture->GetWidth();
				m_iHeight = m_pTexture->GetHeight();
				memcpy(this->m_fUV, m_pTexture->GetUV(), sizeof(float) * 4);
			}
		}
		m_bMirror = false;
		m_bVisible = true;
		//if( !g_bSupportNonPowerOfTwoTexture )
		//{
		//	if(m_pTexture->GetUV()[2] != 1.f || m_pTexture->GetUV()[3] == 1.f)
		//	{
		//		m_iWidth = (int)(m_pTexture->GetWidth()*m_pTexture->GetUV()[2]);
		//		m_iHeight = (int)(m_pTexture->GetHeight()*m_pTexture->GetUV()[3]);
		//	}
		//}
		m_vColor = Vector4::One;
		m_OffsetPos.x = m_OffsetPos.y = 0;
		m_OriginalSize.x = m_iWidth;
		m_OriginalSize.y = m_iHeight;
		if (m_pTexture)
			return true;
		return false;
	}

	//<cBaseImage ImageName="" Pos="" Color="" Width="" Height="" FetchPixel="0" />
	cBaseImage*	cBaseImage::GetMe(TiXmlElement*e_pTiXmlElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cBaseImage::TypeID);
		cBaseImage*l_pBaseImage = 0;
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("ImageName")
			{
				l_pBaseImage = new cBaseImage(UT::WcharToChar(l_strValue).c_str());
			}
			else
			IMAGE_COMMON_PARAMETERS(l_pBaseImage)
		PARSE_NAME_VALUE_END
		return l_pBaseImage;
	}

	cBaseImage::cBaseImage(cTexture*e_pTexture)
	{
		this->m_pTexture = nullptr;
		SetTexture(e_pTexture);
	}

	cBaseImage::cBaseImage(const wchar_t*e_strName)
	{
		this->SetName(e_strName);
		m_pTexture = nullptr;
		m_bMirror = false;
		m_bVisible = true;
		m_iWidth = 0;
		m_iHeight = 0;
		m_vColor = Vector4::One;
		m_OffsetPos.x = m_OffsetPos.y = 0;
		m_OriginalSize.x = m_iWidth;
		m_OriginalSize.y = m_iHeight;
	}
	cBaseImage::~cBaseImage()
	{
		SAFE_RELEASE(m_pTexture, this);
	}
	//===============
	//
	//===============
	void	cBaseImage::SetTexture(cTexture*e_pTexture)
	{
		if (m_pTexture)
		{
			this->m_pTexture->Release(this);
		}
		m_pTexture = e_pTexture;
		m_pTexture->AddRef(this);
		memcpy(this->m_fUV,m_pTexture->GetUV(),sizeof(float)*4);
		m_bMirror = false;
		m_bVisible = true;
		m_iWidth = m_pTexture->GetWidth();
		m_iHeight = m_pTexture->GetHeight();
		m_vColor = Vector4::One;
		m_OffsetPos.x = m_OffsetPos.y = 0;	
		this->m_OriginalSize.x = m_iWidth;
		this->m_OriginalSize.y = m_iHeight;
	}
	void	cBaseImage::SetPos(POINT e_Pos)
	{
		m_pvPos->x = (float)e_Pos.x;
		m_pvPos->y = (float)e_Pos.y;
		this->SetCachedWorldTransformDirty();
		//Frame::SetLocalPosition(*m_pvPos);
	}
	void	cBaseImage::SetPos(Vector2 e_vPos)
	{
		m_pvPos->x = e_vPos.x;
		m_pvPos->y = e_vPos.y;
		this->SetCachedWorldTransformDirty();
		//Frame::SetLocalPosition(*m_pvPos);
	}
	void	cBaseImage::SetPosByImageCenter(Vector3 e_vPos)
	{
		m_pvPos->x = e_vPos.x-(float)this->m_OriginalSize.x /2.f;
		m_pvPos->y = e_vPos.y- (float)m_OriginalSize.y /2.f;
		this->SetCachedWorldTransformDirty();
		//Frame::SetLocalPosition(*m_pvPos);
	}

    bool    cBaseImage::CollideTexture(int e_iX,int e_iY,bool e_bTestAlphaChannel,Vector4*e_pvCollidedColor)
	{
		e_iY = (int)(e_iY-(int)this->m_pvPos->y);
        e_iX = (int)(e_iX- (int)this->m_pvPos->x);
		if( e_iX<m_OffsetPos.x||e_iY<m_OffsetPos.y||m_OffsetPos.x+m_iWidth<e_iX||m_OffsetPos.y+m_iHeight<e_iY )
			return false;
		if( !e_bTestAlphaChannel || !this->m_pTexture->GetPixels() )
            return true;
        int l_iLocalPosX = e_iX-m_OffsetPos.x;
        int l_iLocalPosY = e_iY-m_OffsetPos.y;
		unsigned char*l_pPixels = (unsigned char*)m_pTexture->GetPixels();
        assert(m_pTexture->GetPixelFormat() == GL_RGBA);
        //m_iWidth to get original size
        int l_iPixelsID = (l_iLocalPosX+(l_iLocalPosY*m_pTexture->GetWidth()))*4;
        //+3 for alpha
        if(l_pPixels[l_iPixelsID+3] != 0)
		{
			unsigned char   l_Color[4] = { l_pPixels[l_iPixelsID],l_pPixels[l_iPixelsID+1],l_pPixels[l_iPixelsID+2],l_pPixels[l_iPixelsID+3] };
			Vector4 e_vColor(l_Color[0]/255.f,l_Color[1]/255.f,l_Color[2]/255.f,l_Color[3]/255.f);
			if( e_pvCollidedColor )
				*e_pvCollidedColor = e_vColor;
            return true;
		}
        return false;
	}
	//==================
	//move point to image's local position
	//now set image;s center as 0,0,so set localX and Localy tp correspond position(width for minus half image)
	//
	//==================
    bool    cBaseImage::CollideTextureWithTransform(int e_iX,int e_iY,bool e_bTestAlphaChannel)
    {
		//fuck result is wrong.
        int l_iLocalY = (int)(e_iY- (int)this->m_pvPos->y);
        int l_iLocalX = (int)(e_iX- (int)this->m_pvPos->x);
		cMatrix44	l_matrix = cMatrix44::Identity;
		Vector3 l_vPos = Vector3((float)l_iLocalX-m_pTexture->GetWidth()/2.f, (float)l_iLocalY-m_pTexture->GetHeight()/2.f,0);
		l_matrix = l_matrix.ZAxisRotationMatrix(this->GetRotation().z);
		Vector3 l_vFinalPos = l_matrix.TransformVector(l_vPos);
		l_iLocalY = (int)(l_vFinalPos.y+ (float)this->m_pTexture->GetHeight()/2);
		l_iLocalX = (int)(l_vFinalPos.x+ (float)this->m_pTexture->GetWidth()/2);
		if( l_iLocalX<0||l_iLocalY<0 )
			return false;
		if( l_iLocalX>this->m_iWidth||l_iLocalY>this->m_iHeight )
			return false;
		//it might be a PuzzleImageUnit so get correct width size
		int l_iActulWidthSize = m_pTexture->GetWidth();
		//fuck.....
        //if( !g_bSupportNonPowerOfTwoTexture && this->GetOwner() )
        //{
          //  l_iActulWidthSize = UT::power_of_two(m_pTexture->GetWidth());
        //}
		int	l_iIndex = l_iLocalY*l_iActulWidthSize+l_iLocalX;
		if( l_iIndex<0 )
			return false;
		if( !e_bTestAlphaChannel )
        {
            RECT    l_rc = {this->m_OffsetPos.x,this->m_OffsetPos.y,this->m_iWidth,this->m_iHeight};
            if(UT::RectCollidePoint(l_rc,l_iLocalX,l_iLocalY))
                return true;
            return false;
        }
		unsigned char l_Alpha = m_pTexture->GetPixels()[l_iIndex*4];//argb
		//unsigned char l_Alpha = this->m_pPixels[l_iIndex*4+3];//rgba
		//UINT32	l_Alpha2 = *(UINT32*)&this->m_pPixels[l_iIndex*4];//argb
	//	unsigned char	l_cR = this->m_pPixels[l_iIndex*4+1];
	//	unsigned char	l_cG = this->m_pPixels[l_iIndex*4+2];
	//	unsigned char	l_cB = this->m_pPixels[l_iIndex*4+4];

		if( l_Alpha != 0 )
		{//there is alpha with color is default empty color,so it's not collide
			//if( l_cR==255&&
			//	l_cG==255&&
			//	l_cB==255)
			//	return false;
			//this->Destroy();
			////memset(m_pPixels,130,sizeof(m_pTexture->GetWidth()*m_iHeight)*4);
			//glGenTextures(1,&m_uiImageIndex);
			//glBindTexture( GL_TEXTURE_2D, m_uiImageIndex );
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			//m_pPixels[l_iIndex*4] = 125;
			//m_pPixels[l_iIndex*4+1] = 125;
			//m_pPixels[l_iIndex*4+2] = 125;
			//m_pPixels[l_iIndex*4+3] = 125;
			//glTexImage2D(GL_TEXTURE_2D,
			//  0,
			//  GL_RGBA,
			//  m_pTexture->GetWidth(),
			//  m_iHeight,
			//  0,
			//  GL_RGBA ,
			//  GL_UNSIGNED_BYTE,
			//  m_pPixels);
			return true;
		}
		return false;
    }

	void	cBaseImage::SetMirror(bool e_bMirror)
	{
		if(e_bMirror == m_bMirror)
			return;
		m_bMirror = e_bMirror;
		float	l_fX[2] = {m_fUV[0],m_fUV[2]};
		m_fUV[0] = l_fX[1];
		m_fUV[2] = l_fX[0];
	}

	void	cBaseImage::SetTexBehaviorData(cBaseImage*e_pBaseImage)
	{
		this->SetName(e_pBaseImage->GetName());
		SAFE_RELEASE(m_pTexture, this);
		m_pTexture = e_pBaseImage->m_pTexture;
		if (m_pTexture)
			m_pTexture->AddRef(this);
		m_OriginalSize = e_pBaseImage->m_OriginalSize;
		memcpy(this->m_fUV,e_pBaseImage->GetUV(),sizeof(float)*4);
		m_bMirror = e_pBaseImage->m_bMirror;
		m_bVisible = true;
		m_vColor = e_pBaseImage->m_vColor;
		m_OffsetPos = e_pBaseImage->m_OffsetPos;
		m_iWidth = e_pBaseImage->m_iWidth;
		m_iHeight = e_pBaseImage->m_iHeight;
	}

	bool	cBaseImage::ApplyImage()
	{
		if (m_pTexture)
		{
			return this->m_pTexture->ApplyImage();
		}
		return false;
	}

	void	cBaseImage::Render(int e_iX,int e_iY)
	{
		if( !m_bVisible )
			return;
		Vector3	l_vPos((float)e_iX,(float)e_iY,0.f);
		Render(l_vPos);
	}

	void	cBaseImage::Render(Vector3 e_vPos)
	{
		if( !m_bVisible || !m_pTexture)
			return;
		if(m_pTexture->ApplyImage())
		{
			GLRender::RenderQuadWithTextureAndColorAndCoordinate(e_vPos.x+m_OffsetPos.x,e_vPos.y+m_OffsetPos.y,e_vPos.z,(float)this->m_iWidth,(float)this->m_iHeight,this->m_vColor,m_fUV,this->GetRotation());
		}
		else
		{
			DebugRender();
		}
	}

	void	cBaseImage::DebugRender()
	{
		Vector3	l_vPos = this->GetWorldPosition();
		GLRender::RenderRectangle(l_vPos,(float)this->m_OriginalSize.x,(float)this->m_OriginalSize.y,Vector4::One,this->GetRotation());
	}
	//====================
	//
	//====================	
  //  void	cBaseImage::RenderWithOffset()
  //  {
		//if( !m_bVisible )
		//	return;
		//m_pTexture->ApplyImage();
		//Vector3	l_vPos = this->GetWorldPosition();
		//RenderQuadWithTextureAndColorAndCoordinate(l_vPos.x+this->m_OffsetPos.x,l_vPos.y+this->m_OffsetPos.y,l_vPos.z,this->m_iWidth,this->m_iHeight,m_vColor,this->m_fUV,Vector3::Zero);
  //  }
	void	cBaseImage::RenderWithShader(const WCHAR*e_strShaderName)
	{
		if( !m_bVisible )
			return;
		if(m_pTexture->ApplyImage())
		{
			Vector3 l_vPos = this->GetWorldPosition();
			GLRender::RenderQuadWithTextureAndColorAndCoordinate(l_vPos.x+(float)m_OffsetPos.x,l_vPos.y+(float)m_OffsetPos.y,0.f,(float)this->m_iWidth,(float)this->m_iHeight,this->m_vColor,m_fUV,this->GetRotation(),e_strShaderName);
		}
		else
		{
			DebugRender();
		}
	}
	//====================
	//
	//====================
	void	cBaseImage::Render()
	{
		if( !m_bVisible )
			return;
		Render(this->GetWorldTransform());
	}
	
    void	cBaseImage::Render(cMatrix44 e_Mat)
    {
		if( !m_bVisible )
			return;
		if(m_pTexture)
			m_pTexture->ApplyImage();
		POINT l_vOffsetPos = m_OffsetPos;
		if (m_OffsetPos.x != 0 || m_OffsetPos.y != 0)
		{
			float l_fScaleX = (float)m_iWidth / ((float)m_pTexture->GetWidth()*(m_fUV[2] - m_fUV[0]));
			float l_fScaleY = (float)m_iHeight / ((float)m_pTexture->GetHeight()*(m_fUV[3] - m_fUV[1]));
			l_vOffsetPos.x = (long)(l_fScaleX*m_OffsetPos.x);
			l_vOffsetPos.y = (long)(l_fScaleY*m_OffsetPos.y);
		}
		float l_fWidth = (float)m_iWidth/2.f;
		float l_fHeight = (float)m_iHeight/2.f;
		float	l_Vertices[] = { -l_fWidth + (float)l_vOffsetPos.x,-l_fHeight + (float)l_vOffsetPos.y,
								  l_fWidth + (float)l_vOffsetPos.x,-l_fHeight + (float)l_vOffsetPos.y,
								 -l_fWidth + (float)l_vOffsetPos.x, l_fHeight + (float)l_vOffsetPos.y,
								  l_fWidth + (float)l_vOffsetPos.x, l_fHeight + (float)l_vOffsetPos.y };

		float	l_fTexPointer[] = {  m_fUV[0],m_fUV[1],
								   m_fUV[2],m_fUV[1],
								   m_fUV[0],m_fUV[3],
								   m_fUV[2],m_fUV[3],};
		cMatrix44 l_Mat = cMatrix44::TranslationMatrix(l_fWidth,l_fHeight,0.f)*e_Mat;
        RenderQuadWithMatrix(l_Vertices,l_fTexPointer,this->m_vColor,l_Mat,2,1);
    }

	void	cBaseImage::RenderBySpecificPos(Vector3 e_vStartPos,Vector3 e_vEndPos)
	{
		Vector3	l_vUpDirection = e_vStartPos-e_vEndPos;
		if( l_vUpDirection.Length()<=0.1f )
			return;
		Vector3	l_vRightDirection = l_vUpDirection.Normalize()^Vector3(0,0,-1);
		int	l_iWidth =	this->m_iHeight;
		Vector3	l_v2 = l_vRightDirection*(float)l_iWidth+e_vStartPos;
		Vector3	l_v3 = l_vRightDirection*(float)-l_iWidth+e_vStartPos;
		Vector3	l_v1 = l_vRightDirection*(float)l_iWidth+e_vEndPos;;
		Vector3	l_v0 = l_vRightDirection*(float)-l_iWidth+e_vEndPos;
		m_pTexture->ApplyImage();
		float*l_pfTex = this->GetUV();
		float	l_pfTexPointer[8] = { l_pfTex[2],l_pfTex[1],
									l_pfTex[2],l_pfTex[3],
									l_pfTex[0],l_pfTex[1],
									l_pfTex[0],l_pfTex[3] };

		float	l_pVertices[8] = { l_v3.x,l_v3.y,l_v2.x,l_v2.y,l_v0.x,l_v0.y,l_v1.x,l_v1.y};
		RenderQuadWithMatrix(l_pVertices, l_pfTexPointer, m_vColor, cMatrix44::Identity, 2, 1);
	}

	void	cBaseImage::SetupTexture(GLint e_iChannel,GLsizei e_iWidth,GLsizei e_iHeight,GLenum e_Format,GLenum e_Type,bool e_bGeneratePixels,const GLvoid *e_pPixels, bool e_bShowLog)
	{
		this->m_pTexture->SetupTexture(e_iChannel,e_iWidth,e_iHeight,e_Format,e_Type,e_bGeneratePixels,e_pPixels, e_bShowLog);
		memcpy(this->m_fUV,m_pTexture->GetUV(),sizeof(float)*4);
		this->m_iWidth = m_pTexture->GetWidth();
		this->m_iHeight = m_pTexture->GetHeight();
	}

	POINT	cBaseImage::GetSize()
	{
		POINT l_Size = {this->m_iWidth,this->m_iHeight};
		return l_Size;
	}


	const cBound*	cBaseImage::GenerateBound()
	{
		auto l_Size = GetSize();
		RECT l_Rect = CreateRect(l_Size,this->GetLocalPosition());
		cBound l_Bound(l_Rect);
		this->SetLocalBound(&l_Bound);
		return this->GetLocalBound();
	}
	cTexture* cBaseImage::GetTriangulatorRenderDataForBatchRendering(int& e_iOutNumVertex,Vector3* e_pvOutPos, Vector2* e_pvOutUV, Vector4* e_pvOutColor)
	{
		if (!m_bVisible)
		{
			return nullptr;
		}
		POINT l_vOffsetPos = m_OffsetPos;
		if (m_OffsetPos.x != 0 || m_OffsetPos.y != 0)
		{
			float l_fScaleX = (float)m_iWidth / ((float)m_pTexture->GetWidth() * (m_fUV[2] - m_fUV[0]));
			float l_fScaleY = (float)m_iHeight / ((float)m_pTexture->GetHeight() * (m_fUV[3] - m_fUV[1]));
			l_vOffsetPos.x = (long)(l_fScaleX * m_OffsetPos.x);
			l_vOffsetPos.y = (long)(l_fScaleY * m_OffsetPos.y);
		}
		float l_fWidth = (float)m_iWidth / 2.f;
		float l_fHeight = (float)m_iHeight / 2.f;
		float	l_Vertices[] = { -l_fWidth + (float)l_vOffsetPos.x,-l_fHeight + (float)l_vOffsetPos.y,0.f,
								  l_fWidth + (float)l_vOffsetPos.x,-l_fHeight + (float)l_vOffsetPos.y,0.f,
								 -l_fWidth + (float)l_vOffsetPos.x, l_fHeight + (float)l_vOffsetPos.y,0.f,
								  l_fWidth + (float)l_vOffsetPos.x, l_fHeight + (float)l_vOffsetPos.y,0.f, };
		Vector4	l_vColor[TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT] = {m_vColor,m_vColor ,m_vColor ,m_vColor,m_vColor,m_vColor };
		Vector3 l_vTrianglesPos[TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];
		Vector2 l_vTrianglesUV[TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];
		auto l_Mat = cMatrix44::TranslationMatrix(l_fWidth, l_fHeight, 0.f) * this->GetWorldTransform();
		//for (int i = 0; i < A_QUAD_4_TRIANGLES; ++i)
		//{
		//	Vector3* l_pPos = (Vector3*)&l_Vertices[i * 3];
		//	*l_pPos = l_Mat.TransformCoordinate(*l_pPos);
		//}
		VerticesApplyTransform(A_QUAD_4_TRIANGLES, l_Vertices, l_Mat, 3);
		Assign4VerticesDataTo2Triangles(l_Vertices,(float*) &l_vTrianglesPos, m_fUV, (float*)&l_vTrianglesUV, 3);
		memcpy(e_pvOutPos, l_vTrianglesPos, sizeof(Vector3) * TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT);
		memcpy(e_pvOutUV, l_vTrianglesUV, sizeof(Vector2) * TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT);
		memcpy(e_pvOutColor, l_vColor, sizeof(Vector4) * TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT);
		e_iOutNumVertex = TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT;
		return this->m_pTexture;
	}

	cTexture* cBaseImage::GetQuadRenderDataForBatchRendering(int& e_iOutNumVertex, cMatrix44& e_OutMat, Vector3* e_pvOutPos, Vector2* e_pvOutUV, Vector4* e_pvOutColor)
	{
		if (!m_bVisible)
		{
			return nullptr;
		}
		POINT l_vOffsetPos = m_OffsetPos;
		if (m_OffsetPos.x != 0 || m_OffsetPos.y != 0)
		{
			float l_fScaleX = (float)m_iWidth / ((float)m_pTexture->GetWidth() * (m_fUV[2] - m_fUV[0]));
			float l_fScaleY = (float)m_iHeight / ((float)m_pTexture->GetHeight() * (m_fUV[3] - m_fUV[1]));
			l_vOffsetPos.x = (long)(l_fScaleX * m_OffsetPos.x);
			l_vOffsetPos.y = (long)(l_fScaleY * m_OffsetPos.y);
		}
		float l_fWidth = (float)m_iWidth / 2.f;
		float l_fHeight = (float)m_iHeight / 2.f;
		float	l_Vertices[] = { -l_fWidth + (float)l_vOffsetPos.x,-l_fHeight + (float)l_vOffsetPos.y,0.f,
								  l_fWidth + (float)l_vOffsetPos.x,-l_fHeight + (float)l_vOffsetPos.y,0.f,
								 -l_fWidth + (float)l_vOffsetPos.x, l_fHeight + (float)l_vOffsetPos.y,0.f,
								  l_fWidth + (float)l_vOffsetPos.x, l_fHeight + (float)l_vOffsetPos.y,0.f, };
		float	l_fTexPointer[] = { m_fUV[0],m_fUV[1],
								   m_fUV[2],m_fUV[1],
								   m_fUV[0],m_fUV[3],
								   m_fUV[2],m_fUV[3] };
		Vector4	l_vColor[4] = { m_vColor,m_vColor ,m_vColor ,m_vColor };
		e_OutMat = cMatrix44::TranslationMatrix(l_fWidth, l_fHeight, 0.f) * this->GetWorldTransform();
		memcpy(e_pvOutPos, l_Vertices, sizeof(Vector3) * 4);
		memcpy(e_pvOutUV, l_Vertices, sizeof(Vector2) * 4);
		memcpy(e_pvOutColor, l_vColor, sizeof(Vector4) * 4);
		e_iOutNumVertex = 4;
		return this->m_pTexture;
	}
	//void	cBaseImage::RenderWithoutOffset(Vector3 e_vPos)
	//{
	//	if( !m_bVisible )
	//		return;
	//	Render(Vector3(e_vPos.x+this->m_OffsetPos.x,e_vPos.y+this->m_OffsetPos.y,0.f));
	//}
	//void	cBaseImage::RenderWithoutOffset(int e_iX,int e_iY)
	//{
	//	if( !m_bVisible )
	//		return;
	//	Render(e_iX+this->m_OffsetPos.x,e_iY+this->m_OffsetPos.y);
	//}
	//====================
	//
	//====================
	cUIImage::cUIImage(const char*e_strImageName):cBaseImage(e_strImageName,true)
	{
		m_pvPos->x = 0;m_pvPos->y = 0;
		m_OriginalImageSize.x = -1;
		m_OriginalImageSize.y = -1;
		m_RightDownStripOffPos.x = this->m_iWidth-1;
		m_RightDownStripOffPos.y = this->m_iHeight-1;
		m_iCollisionImageWidth = m_iWidth;
		m_iCollisionImageHeight = m_iHeight;
		m_pEditorAttachParent = 0;
		m_vEditorAttachParentRelativePos = Vector3::Zero;
	}

	cUIImage::cUIImage(const wchar_t*e_strName,char*e_pPixelsData,int e_iWidth,int e_iHeight,GLint e_iDataFormat):cBaseImage(e_strName)
	{
		this->SetName(e_strName);
		m_OriginalImageSize.x = -1;
		m_OriginalImageSize.y = -1;
		m_fUV[0] = 0.f; m_fUV[1] = 0.f; m_fUV[2] = 1.f; m_fUV[3] = 1.f;
		m_pvPos->x = 0;m_pvPos->y = 0; 
		m_RightDownStripOffPos.x = e_iWidth-1;
		m_RightDownStripOffPos.y = e_iHeight-1;
		m_iCollisionImageWidth = e_iWidth;
		m_iCollisionImageHeight = e_iHeight;
		this->m_iHeight = e_iHeight;
		this->m_iWidth = e_iWidth;
		int	l_iWidthPO2 = power_of_two(m_iWidth);
		int	l_iHeightPO2 = power_of_two(m_iHeight);
		GLint texSize; MyGLGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
		int	l_iChannel = e_iDataFormat == GL_RGB?3:4;
		if(m_pTexture)
			m_pTexture->Release(this);
		//make sure power of 2,because not every fukcing graphic card support it
		//but if u exactly sure it do support power of 2 u could mark this.
		if( !g_bSupportNonPowerOfTwoTexture&&(l_iWidthPO2!=e_iWidth||l_iHeightPO2!=e_iHeight) )
		{
			assert((l_iWidthPO2<=texSize||l_iHeightPO2<=texSize)&&"texture size is too big then card support");
	#ifdef WIN32
	#ifdef DEBUG
			FMLog::LogWithFlag(e_strName, CORE_LOG_FLAG);
			FMLog::LogWithFlag(L"  image has not correct respond width and height,because none power of 2\n", CORE_LOG_FLAG);
			if( e_iDataFormat == GL_RGB )
				FMLog::LogWithFlag(L"UV is changed,because image size is not to become power of 2", CORE_LOG_FLAG);
	#endif
	#endif
			bool e_balpah = false;
#ifdef ANDROID
			if( e_iDataFormat == GL_RGBA )//android not support GL_BGRA?or just nvidia ignore it?
#else
			if( e_iDataFormat == GL_RGBA 
#if !defined(WASM) && !defined(LINUX) && !defined(UWP)//https://www.opengl.org/discussion_boards/showthread.php/185197-Why-OpenGLES-2-spec-doesn-t-support-BGRA-texture-format
				|| e_iDataFormat == GL_BGRA 
#endif
				)
#endif
				e_balpah = true;
			char*l_pNewPixelData = TextureToPowOfTwo((char*)e_pPixelsData,e_iWidth,e_iHeight,e_balpah);
			delete e_pPixelsData;
			this->m_iWidth = l_iWidthPO2; this->m_iHeight = l_iHeightPO2;
			m_pTexture = new cTexture(this,l_pNewPixelData,l_iWidthPO2,l_iHeightPO2,e_strName,true,false,e_iDataFormat);
			//assign new UV
			m_fUV[2] = (float)m_iWidth/ (float)l_iWidthPO2;
			m_fUV[3] =  (float)m_iHeight/ (float)l_iHeightPO2;
		}
		else
		{
			m_pTexture = new cTexture(this,e_pPixelsData,e_iWidth,e_iHeight,e_strName,true,false,e_iDataFormat);

		}
		m_pEditorAttachParent = 0;
		m_vEditorAttachParentRelativePos = Vector3::Zero;
		m_OriginalSize.x = e_iWidth;
		m_OriginalSize.y = e_iHeight;
	}

	cUIImage::cUIImage(cUIImage*e_pUIImage):cBaseImage(e_pUIImage)
	{
		m_pvPos->x = 0;m_pvPos->y = 0;
		m_OriginalImageSize.x = -1;
		m_OriginalImageSize.y = -1;
		m_RightDownStripOffPos = e_pUIImage->m_RightDownStripOffPos;
		m_iCollisionImageWidth = e_pUIImage->m_iCollisionImageWidth;
		m_iCollisionImageHeight = e_pUIImage->m_iCollisionImageHeight;
		m_pEditorAttachParent = e_pUIImage;
		m_vEditorAttachParentRelativePos = Vector3::Zero;
	}

	cUIImage::~cUIImage()
	{
	}

	void	cUIImage::Render()
	{
		if( !m_bVisible )
			return;
		if(m_pTexture->ApplyImage())
		{
			Vector3	l_vPos = this->GetWorldPosition();
			GLRender::RenderQuadWithTextureAndColorAndCoordinate(l_vPos.x,l_vPos.y,l_vPos.z,(float)this->m_iWidth,(float)this->m_iHeight,this->m_vColor,m_fUV,this->GetRotation());
		}
		else
		{
			DebugRender();
		}	
	}

	void	cUIImage::DebugRender()
	{
		GLRender::RenderRectangle(this->GetWorldPosition(),(float)this->m_iWidth,(float)this->m_iHeight,Vector4::One,this->GetRotation());
	}
	//void	cUIImage::RenderWithOffset()
	//{
	//	if( !m_bVisible )
	//		return;
	//	Vector3	l_vPos = this->GetWorldPosition();
	//	cBaseImage::Render((int)l_vPos.x-this->m_OffsetPos.x,(int)l_vPos.y-this->m_OffsetPos.y);
	//}


	bool	cUIImage::Collide(int e_iLocalX,int e_iLocalY)
	{
		if( this->m_pTexture->GetPixelFormat()  == GL_RGB)//there is no alpha so we can't find any info
		{
			if( e_iLocalX<this->m_iCollisionImageWidth&&e_iLocalX>=0 )
				if( e_iLocalY<this->m_iCollisionImageHeight&&e_iLocalY>=0 )
					return true;
			return false;
		}
		cMatrix44	l_matrix;
		Vector3 l_vPos = Vector3((float)m_pTexture->GetHeight()/2.f-e_iLocalY,e_iLocalX- (float)m_pTexture->GetWidth()/2.f,0);
		l_matrix = l_matrix.Identity;
		l_matrix = l_matrix.ZAxisRotationMatrix(MyMath::DegToRad(360-this->GetRotation().z));
		Vector3 l_vFinalPos = l_matrix.TransformVector(l_vPos);
		e_iLocalY = (int)(-l_vFinalPos.x)+this->m_pTexture->GetHeight()/2;
		e_iLocalX = (int)l_vFinalPos.y+this->m_pTexture->GetWidth()/2;
		if( e_iLocalX<0||e_iLocalY<0 )
			return false;
		if( e_iLocalX>m_pTexture->GetWidth()||e_iLocalY>m_pTexture->GetHeight() )
			return false;
		int	l_iIndex = e_iLocalY*this->m_iCollisionImageWidth+e_iLocalX;
		if( l_iIndex<0 )
			return false;
        //+4!!!!!!!!!?FUCK...
		unsigned char l_Alpha = this->m_pTexture->GetPixels()[l_iIndex*4];//argb
		//UINT32	l_Alpha2 = *(UINT32*)&this->m_pPixels[l_iIndex*4];//argb
	//	unsigned char	l_cR = this->m_pPixels[l_iIndex*4+1];
	//	unsigned char	l_cG = this->m_pPixels[l_iIndex*4+2];
	//	unsigned char	l_cB = this->m_pPixels[l_iIndex*4+4];

		if( l_Alpha != 0 )
		{//there is alpha with color is default empty color,so it's not collide
			//if( l_cR==255&&
			//	l_cG==255&&
			//	l_cB==255)
			//	return false;
			//this->Destroy();
			////memset(m_pPixels,130,sizeof(m_pTexture->GetWidth()*m_iHeight)*4);
			//glGenTextures(1,&m_uiImageIndex);
			//glBindTexture( GL_TEXTURE_2D, m_uiImageIndex );
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			//m_pPixels[l_iIndex*4] = 125;
			//m_pPixels[l_iIndex*4+1] = 125;
			//m_pPixels[l_iIndex*4+2] = 125;
			//m_pPixels[l_iIndex*4+3] = 125;
			//glTexImage2D(GL_TEXTURE_2D,
			//  0,
			//  GL_RGBA,
			//  m_pTexture->GetWidth(),
			//  m_iHeight,
			//  0,
			//  GL_RGBA ,
			//  GL_UNSIGNED_BYTE,
			//  m_pPixels);
			return true;
		}
		return false;
	}

	bool	cUIImage::CollideWithOutAlphaTest(int e_iLocalX,int e_iLocalY)
	{
		if( this->m_pTexture->GetPixelFormat() == GL_RGB)//there is no alpha so we can't find any info
		{
			if( e_iLocalX<this->m_iCollisionImageWidth&&e_iLocalX>=0 )
				if( e_iLocalY<this->m_iCollisionImageHeight&&e_iLocalY>=0 )
					return true;
			return false;
		}
		cMatrix44	l_matrix;
		Vector3 l_vPos = Vector3(this->m_iHeight/2.f-e_iLocalY,e_iLocalX-this->m_pTexture->GetWidth()/2.f,0);
		l_matrix = l_matrix.Identity;
		l_matrix = l_matrix.ZAxisRotationMatrix(MyMath::DegToRad(360-this->GetRotation().z));
		Vector3 l_vFinalPos = l_matrix.TransformVector(l_vPos);
		e_iLocalY = (int)(-l_vFinalPos.x)+this->m_iHeight/2;
		e_iLocalX = (int)l_vFinalPos.y+this->m_pTexture->GetWidth()/2;
		if( e_iLocalX<0||e_iLocalY<0 )
			return false;
		if( e_iLocalX>m_pTexture->GetWidth()||e_iLocalY>m_iHeight )
			return false;
			
		if( e_iLocalX>(this->m_RightDownStripOffPos.x-this->m_OffsetPos.x)||e_iLocalY>this->m_RightDownStripOffPos.y-this->m_OffsetPos.y )
			return false;			
		int	l_iIndex = e_iLocalY*this->m_iCollisionImageWidth+e_iLocalX;
		if( l_iIndex<0 )
			return false;
		return true;
	}
}