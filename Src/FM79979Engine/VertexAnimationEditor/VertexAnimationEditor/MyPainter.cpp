#include "stdafx.h"
#include "MyPainter.h"
#include "../../Core/FatmingCollada/AllColladaInclude.h"
namespace FATMING_CORE
{
	extern bool	g_bSupportNonPowerOfTwoTexture;
}
cShockBoobsShader*g_pShockBoobsShader = 0;
void*	FetchPixeldata(System::Drawing::Bitmap^e_pImage,int*e_piWidth,int*e_piHeight,int*e_piChannel)
{
	if( e_pImage )
	{
		System::Drawing::Rectangle l_Rect = System::Drawing::Rectangle(0,0,e_pImage->Width,e_pImage->Height);
		System::Drawing::Imaging::BitmapData^l_BmpData = e_pImage->LockBits( l_Rect, System::Drawing::Imaging::ImageLockMode::ReadOnly, e_pImage->PixelFormat );
		int	l_iPixelSize = e_pImage->Width*e_pImage->Height*(e_pImage->PixelFormat==System::Drawing::Imaging::PixelFormat::Format32bppArgb?4:3);
		char*l_pData = new char[l_iPixelSize];
		memcpy(l_pData,l_BmpData->Scan0.ToPointer(),l_iPixelSize);
		e_pImage->UnlockBits( l_BmpData );
		*e_piWidth = e_pImage->Width;
		*e_piHeight = e_pImage->Height;
		*e_piChannel = e_pImage->PixelFormat==System::Drawing::Imaging::PixelFormat::Format32bppArgb?4:3;
		return (void*)l_pData;
	}
	return 0;
}
extern cMesh*g_pMesh;
cMyBoobsPainter::cMyBoobsPainter(char*e_strImageFileName)
{
	m_fImageMoveDistance = 5.f;
	m_ePatinerMode = ePM_PAINT;
	m_pFloatPixelDataForModify = 0;
	g_bSupportNonPowerOfTwoTexture = true;
	m_pPixelDataForImage = 0;
	m_pOriginalPaintImage = 0;
	m_pTerrianVertex = 0;
	m_pPaintImage = new cBaseImage((const char*)0);
	if( e_strImageFileName &&  UT::IsFileExists(e_strImageFileName) )
	{
		ChangePaintingImage(e_strImageFileName);
	}
	m_eColorType = eTP_ADD;
	m_pShockBoobsShader = new cShockBoobsShader(cShockBoobsShader::m_sVS,cShockBoobsShader::m_sFS);
	g_pShockBoobsShader = m_pShockBoobsShader;
	//g_pMesh = new cMesh("F_Lizard.DAE");
	m_bMouseDown = false;
	m_iRadius = 100;
	SetCircleRadius(100);
	SetColor(Vector4(0,0,3,127));
	m_DrawGapTC.SetTargetTime(0.032f);
	m_DrawGapTC.Start();
}

cMyBoobsPainter::~cMyBoobsPainter()
{
	Destroy();
}

//<cMyBoobsPainter ImageMoveDistance="5.00" HeightMapFile="qoo.height" OriginalImageName="123.png">
//    <ShaderParmeter ShockSpeed="1.00" ShockSpeedMinus="1.00" ShockStrgethValue="1.00" MaximumShockStregth="30.00" />
//</cMyBoobsPainter>

void	cMyBoobsPainter::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("cMyBoobsPainter")
	{
		ProcesscMyBoobsPainterData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("ShaderParmeter")
	{
		ProcessShaderParmeterData(e_pTiXmlElement);
	}
}

void	cMyBoobsPainter::ProcesscMyBoobsPainterData(TiXmlElement*e_pTiXmlElement)
{
	int			l_iWidth = 0;
	int			l_iHeight = 0;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("ImageMoveDistance")
		{
			this->m_fImageMoveDistance = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("HeightMapFile")
		{
			std::string	l_strFileName = this->m_strCurrentDirectory;
			l_strFileName += UT::WcharToChar(l_strValue).c_str();
			NvFile*l_pFile = UT::MyFileOpen(l_strFileName.c_str(),"rb");
			if( l_pFile )
			{
				long	l_uiSize = UT::GetFileSize(l_pFile);
				SAFE_DELETE(m_pFloatPixelDataForModify);
				char*l_pData = new char[l_uiSize];
				NvFRead(l_pData,1,l_uiSize,l_pFile);
				this->m_pFloatPixelDataForModify = (float*)l_pData;
				NvFClose(l_pFile);
			}
			else
			{
				UT::ErrorMsg(L"height file open failed",l_strValue);
			}
		}
		else
		COMPARE_NAME("OriginalImageName")
		{
			SAFE_DELETE(m_pOriginalPaintImage);
			std::string	l_strFileName = this->m_strCurrentDirectory;
			l_strFileName += UT::WcharToChar(l_strValue).c_str();
			this->m_pOriginalPaintImage = new cBaseImage(l_strFileName.c_str());
		}
		else
		COMPARE_NAME("GridWidth")
		{
			l_iWidth = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("GridHeight")
		{
			l_iHeight = VALUE_TO_INT;
		}
	PARSE_NAME_VALUE_END
	if( l_iWidth>0 && l_iHeight>0 )
	{
		SAFE_DELETE(this->m_pTerrianVertex);
		SAFE_DELETE(m_pPixelDataForImage);
		m_pTerrianVertex = new cTerrianVertex(l_iWidth,l_iHeight,false);
		m_pPixelDataForImage = new unsigned char[l_iWidth*l_iHeight*4];//rgba
		if( m_pFloatPixelDataForModify )
		{
			//m_pTerrianVertex->UpdateVertexHeight(m_pFloatPixelDataForModify,0,0,l_iWidth,l_iHeight);
			unsigned char*l_pPixelData = 0;
			for( int i=0;i<l_iWidth;++i  )
			{
				for( int j=0;j<l_iHeight;++j  )
				{
					int		l_iIndex = l_iWidth*j+i;//rgba
					int		l_iIndexWithStride = l_iIndex*4;//rgba
					l_pPixelData = &m_pPixelDataForImage[l_iIndexWithStride];
					int	l_iHeight = (int)m_pFloatPixelDataForModify[l_iIndex];
					for( int i=0;i<3;++i )
					{
						int	l_iColor = 0xff/2+l_iHeight;
						if( l_iColor > 255 )l_pPixelData[i] = 255;else l_pPixelData[i] = l_iColor;
					}
					l_pPixelData[3] = 0xff/2;
					if( l_iHeight != 0 )
					{
						int	l_iTargetColor = 0xff/2+(l_iHeight*5);
						if( l_iTargetColor >255 )
							l_iTargetColor = 255;
						else
						if( l_iTargetColor <0 )
							l_iTargetColor = 0;
						l_pPixelData[3] = l_iTargetColor;
					}
				}
			}
			UpdatePixelData(0,0,l_iWidth,l_iHeight);
		}
		else
		{
			SAFE_DELETE(m_pFloatPixelDataForModify);
			m_pFloatPixelDataForModify = new float[l_iWidth*l_iHeight];
		}
	}
}

void	cMyBoobsPainter::ProcessShaderParmeterData(TiXmlElement*e_pTiXmlElement)
{
	float	l_fShockSpeed = 1.f;
	float	l_fShockSpeedMinus = 1.f;
	float	l_fShockStrgethValue = 1.f;
	float	l_fMaximumShockStregth = 30.f;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("ShockSpeed")
		{
			l_fShockSpeed = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("ShockSpeedMinus")
		{
			l_fShockSpeedMinus = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("ShockStrgethValue")
		{
			l_fShockStrgethValue = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("MaximumShockStregth")
		{
			l_fMaximumShockStregth = VALUE_TO_FLOAT;
		}
	PARSE_NAME_VALUE_END
	this->m_pShockBoobsShader->SetParmater(l_fShockSpeed,l_fShockSpeedMinus,l_fShockStrgethValue,l_fMaximumShockStregth);
}

void	cMyBoobsPainter::SetColor(Vector4 e_vColor)
{
	m_vColor = e_vColor;
}

void	cMyBoobsPainter::SetCircleRadius(int e_iRadius)
{
	m_iRadius = e_iRadius;
}

void	cMyBoobsPainter::UpdatePixelData(int e_iStartPosX,int e_iStartPosY,int e_iWidth,int e_iHeight)
{
	if( this->m_pPixelDataForImage && m_pTerrianVertex && m_pPaintImage )
	{
		int	l_iWidth = m_pTerrianVertex->GetGridWidth();
		int	l_iHeight = m_pTerrianVertex->GetGridHeight();
		m_pPaintImage->SetupTexture(4,l_iWidth,l_iHeight,GL_RGBA,GL_UNSIGNED_BYTE,false,m_pPixelDataForImage);
		//m_pTerrianVertex->UpdateVertexHeight(m_pPixelDataForImage,e_iStartPosX,e_iStartPosY,e_iWidth,e_iHeight,4);
		m_pTerrianVertex->UpdateVertexHeight(m_pFloatPixelDataForModify,e_iStartPosX,e_iStartPosY,e_iWidth,e_iHeight);
	}
}

void	cMyBoobsPainter::Destroy()
{
	SAFE_DELETE(m_pFloatPixelDataForModify);
	SAFE_DELETE(m_pOriginalPaintImage);
	SAFE_DELETE(m_pPaintImage);
	SAFE_DELETE(m_pTerrianVertex);
	SAFE_DELETE(m_pShockBoobsShader);
}

void	cMyBoobsPainter::SetSize(int e_iWidth,int e_iHeight,int e_iChannel)
{
	if( m_pPixelDataForImage && m_pTerrianVertex )
	{
		int	l_iWidth = m_pTerrianVertex->GetGridWidth();
		int	l_iHeight = m_pTerrianVertex->GetGridHeight();
		if( e_iWidth == l_iWidth && e_iHeight == l_iHeight )
			return;
	}
	int	l_iSize = e_iWidth*e_iHeight;
	unsigned char*l_pPixelDataForModify = new unsigned char[l_iSize*4];
	memset(l_pPixelDataForModify,0xff/2,sizeof(char)*l_iSize*4);
	float*l_pHeightData = new float[l_iSize];
	memset(l_pHeightData,0,sizeof(float)*l_iSize);
	//do copy old pixel
	//if( m_pPixelDataForImage && m_pTerrianVertex )
	//{
	//	int	l_iWidth = m_pTerrianVertex->GetGridWidth();
	//	int	l_iHeight = m_pTerrianVertex->GetGridHeight();
	//	for( int i=0;i<l_iWidth;++i )
	//	{
	//		for( int j=0;j<l_iHeight;++j )
	//		{
	//		
	//		}
	//	}
	//	m_pPixelDataForImage
	//}
	SAFE_DELETE(m_pPixelDataForImage);
	m_pPixelDataForImage = l_pPixelDataForModify;
	SAFE_DELETE(m_pFloatPixelDataForModify);
	m_pFloatPixelDataForModify = l_pHeightData;
	m_pTerrianVertex = new FATMING_CORE::cTerrianVertex(e_iWidth,e_iHeight,false);
	UpdatePixelData(0,0,e_iWidth,e_iHeight);
}

void	cMyBoobsPainter::Update(float e_fElpaseTime)
{
	if( m_ePatinerMode == ePM_PAINT )
	{
		if( m_DrawGapTC.bTragetTimrReached )
		{
			if( m_bMouseDown )
			{
				DrawCircle(m_MousePos);
				m_DrawGapTC.Start();
			}
		}
		else
			m_DrawGapTC.Update(e_fElpaseTime);
	}
	else
	{
	
	}
	//g_pMesh->Update(e_fElpaseTime);
}


void	cMyBoobsPainter::Clean()
{
	int	l_iWidth = m_pTerrianVertex->GetGridWidth();
	int	l_iHeight = m_pTerrianVertex->GetGridHeight();
	memset(this->m_pPixelDataForImage,0xff/2,sizeof(char)*l_iWidth*l_iHeight*4);
	memset(m_pFloatPixelDataForModify,0,sizeof(float)*l_iWidth*l_iHeight);
	UpdatePixelData(0,0,l_iWidth,l_iHeight);
	
}

//void	cMyBoobsPainter::EraseCircle(System::Drawing::Point e_Pos)
//{
//	System::Drawing::Rectangle l_Rect = System::Drawing::Rectangle(0,0,e_pImage->Width,e_pImage->Height);
//	System::Drawing::Imaging::BitmapData^l_BmpData = e_pImage->LockBits( l_Rect, System::Drawing::Imaging::ImageLockMode::ReadOnly, e_pImage->PixelFormat );
//}
//
//void	cMyBoobsPainter::MinusCircle(System::Drawing::Point e_Pos)
//{
//
//}

void	cMyBoobsPainter::DrawCircle(POINT e_Pos)
{
	if( m_ePatinerMode != ePM_PAINT )
		return;
	int	l_iWidth = m_pTerrianVertex->GetGridWidth();
	int	l_iHeight = m_pTerrianVertex->GetGridHeight();
	RECT	l_rc = {e_Pos.x-m_iRadius,e_Pos.y-m_iRadius,e_Pos.x+m_iRadius,e_Pos.y+m_iRadius};
	RECT	l_rc2 = {0,0,l_iWidth,l_iHeight};
	if(!UT::RectCollideRect(l_rc,l_rc2))
		return;
	if( !m_pPixelDataForImage || !m_pTerrianVertex )
		return;
	e_Pos.x -= m_iRadius;
	e_Pos.y -= m_iRadius;
	int	l_iDrawWidth = e_Pos.x+m_iRadius*2;
	int	l_iDrawHeight = e_Pos.y+m_iRadius*2;
	if( l_iDrawWidth > m_pTerrianVertex->GetGridWidth())
		l_iDrawWidth = m_pTerrianVertex->GetGridWidth();
	if( l_iDrawHeight > m_pTerrianVertex->GetGridHeight())
		l_iDrawHeight = m_pTerrianVertex->GetGridHeight();
	int	l_iStartX = e_Pos.x>=0?e_Pos.x:0;
	int	l_iStartY = e_Pos.y>=0?e_Pos.y:0;
	Vector2	l_vCenter((float)this->m_iRadius+e_Pos.x,(float)this->m_iRadius+e_Pos.y);
	unsigned char*l_pPixelData = 0;
	Vector3 l_vResult;
	//my little tricky
	cCurve	l_Curve = cCurve::GetInterplotValue(Vector3::Zero,Vector3::One,7,0.7f,&l_vResult);
	for( int i=l_iStartX;i<l_iDrawWidth;++i  )
	//for( int i=l_iStartX+m_iRadius;i<l_iStartX+m_iRadius+1;++i  )
	{
		for( int j=l_iStartY;j<l_iDrawHeight;++j  )
		{
			int		l_iIndex = l_iWidth*j+i;//rgba
			int		l_iIndexWithStride = l_iIndex*4;//rgba
			float	l_fLength = Vector2(l_vCenter-Vector2((float)i,(float)j)).Length();
			if( l_fLength > this->m_iRadius )
				continue;
			float	l_fGradient = 1-(l_fLength/this->m_iRadius);
			float	l_fLerp = l_Curve.GetInterplotValue(l_fGradient).y;
			l_fLerp *= l_fLerp;
			int		l_iTargetColor[4] = {(int)(m_vColor.x*l_fLerp),(int)(m_vColor.y*l_fLerp),(int)(m_vColor.z*l_fLerp),(int)m_vColor.w};
			float	l_fTargetColor = m_vColor.x*l_fLerp;
			l_pPixelData = &m_pPixelDataForImage[l_iIndexWithStride];
			int	l_iAlphaColor = l_pPixelData[3];
			if( m_eColorType == eTP_ADD )
			{
				m_pFloatPixelDataForModify[l_iIndex] += l_fTargetColor;
				for( int i=0;i<3;++i )
				{
					int	l_iColor = 0xff/2+(int)m_pFloatPixelDataForModify[l_iIndex];
					if( l_iColor > 255 )l_pPixelData[i] = 255;else l_pPixelData[i] = l_iColor;
				}
				l_iAlphaColor += 5;
				if( l_iAlphaColor >= 255 )l_iAlphaColor = 255;
				l_pPixelData[3] = l_iAlphaColor;
			}
			else
			if( m_eColorType == eTP_DEL )
			{
				m_pFloatPixelDataForModify[l_iIndex] -= l_fTargetColor;
				for( int i=0;i<3;++i )
				{
					int	l_iColor = 0xff/2+(int)m_pFloatPixelDataForModify[l_iIndex];
					if( l_iColor < 0 )l_pPixelData[i] = 0;else l_pPixelData[i] = l_iColor;
				}
				l_iAlphaColor -= 5;
				if( l_iAlphaColor < 0 )l_iAlphaColor = 0;
				l_pPixelData[3] = l_iAlphaColor;
			}
			else
			if( m_eColorType == eTP_NONE )
			{
				m_pFloatPixelDataForModify[l_iIndex] = 0;
				for( int i=0;i<3;++i )
				{
					l_pPixelData[i] = 0xff/2;
				}
				l_pPixelData[3] = 0;
			}
		}
	}
	UpdatePixelData(l_iStartX,l_iStartY,l_iDrawWidth,l_iDrawHeight);
}

void	cMyBoobsPainter::DebugRenderTerrian(cFrameCamera*e_pFrameCamera)
{
	if( e_pFrameCamera )
	{
		FATMING_CORE::UseShaderProgram(NO_TEXTURE_SHADER);
		e_pFrameCamera->Render();
		cMatrix44	l_mat = cMatrix44::XAxisRotationMatrix(D3DXToRadian(90));
		DrawPlane(cMatrix44::TranslationMatrix(Vector3(0,0,-5))*l_mat,10,10,1,1,1,Vector4(0.5,1,0,1));
		FATMING_CORE::UseShaderProgram(DEFAULT_SHADER);
		e_pFrameCamera->Render();
	}
	if( m_pTerrianVertex )
	{
		if( m_pPaintImage )
		{
			if( !e_pFrameCamera )
			{
				m_pTerrianVertex->SetLocalTransform(
					cMatrix44::TranslationMatrix((float)*m_pPaintImage->GetWidth(),0,0)
					//*cMatrix44::RotationMatrix(Vector3(40+l_f,40,0))
					*cMatrix44::ScaleMatrix(Vector3(2,2,2))
					);
			}
		}
	
		glEnable(GL_DEPTH_TEST);
		m_pShockBoobsShader->Update(0.016f);
		m_pTerrianVertex->DebugRender((int)m_pPaintImage->GetTexture()->GetImageIndex(),m_pShockBoobsShader->GetName());
		glDisable(GL_DEPTH_TEST);
	}
}

void	cMyBoobsPainter::RenderTerrian(cFrameCamera*e_pFrameCamera)
{
	if( e_pFrameCamera )
	{
		FATMING_CORE::UseShaderProgram(NO_TEXTURE_SHADER);
		e_pFrameCamera->Render();
		cMatrix44	l_mat = cMatrix44::XAxisRotationMatrix(D3DXToRadian(90));
		DrawPlane(cMatrix44::TranslationMatrix(Vector3(0,0,-5))*l_mat,10,10,1,1,1,Vector4(0.5,1,0,1));
		FATMING_CORE::UseShaderProgram(DEFAULT_SHADER);
		e_pFrameCamera->Render();
	}
	if( m_pTerrianVertex )
	{
		if( m_pPaintImage )
		{
			if( !e_pFrameCamera )
			{
				Vector3	l_vDirection = this->m_pShockBoobsShader->GetCurrentShockDirectionValue()*m_fImageMoveDistance;
				Vector3	l_vPos((float)*m_pPaintImage->GetWidth(),0,0);
				l_vPos += l_vDirection;
				m_pTerrianVertex->SetLocalTransform(
					cMatrix44::TranslationMatrix(l_vPos)
					//*cMatrix44::RotationMatrix(Vector3(40+l_f,40,0))
					*cMatrix44::ScaleMatrix(Vector3(2,2,2))
					);
			}
		}
	
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		m_pShockBoobsShader->Update(0.016f);
		m_pTerrianVertex->Render((int)m_pOriginalPaintImage->GetTexture()->GetImageIndex(),m_pShockBoobsShader->GetName());
		glDisable(GL_DEPTH_TEST);
		//glDisable(GL_CULL_FACE);
	}
}

void	cMyBoobsPainter::Render()
{
	if( m_pOriginalPaintImage )
	{
		m_pOriginalPaintImage->Render();
	}
	if( m_pPaintImage )
	{
		m_pPaintImage->SetColor(Vector4(1,1,1,0.7f));
		//m_pPaintImage->SetPos(Vector3(0,*m_pOriginalPaintImage->GetHeight(),0));
		m_pPaintImage->Render();
		m_pPaintImage->DebugRender();
	}
	Vector4	l_vColor = m_vColor/255.f;
	l_vColor.w = 1.f;
	GLRender::RenderCircle(Vector2((float)m_MousePos.x,(float)m_MousePos.y),(float)this->m_iRadius,l_vColor);
	if( g_pMesh )
		g_pMesh->Render();
}

void	cMyBoobsPainter::MouseDown(int e_iPosX,int e_iPosY)
{
	m_bMouseDown = true;
	POINT l_Pos = {e_iPosX,e_iPosY};
	if( m_ePatinerMode == ePM_PAINT )
	{
		DrawCircle(l_Pos);
	}
	else
	if( m_ePatinerMode == ePM_SHOCK )
	{
		m_vMouseDownPos = Vector2(e_iPosX,e_iPosY);
	}
	m_MousePos = l_Pos;
}

void	cMyBoobsPainter::MouseMove(int e_iPosX,int e_iPosY)
{
	POINT l_Pos = {e_iPosX,e_iPosY};
	m_MousePos = l_Pos;
}

void	cMyBoobsPainter::MouseUp(int e_iPosX,int e_iPosY)
{
	m_bMouseDown = false;
	if( m_ePatinerMode == ePM_SHOCK )
	{
		Vector2	l_vUpPos = Vector2(e_iPosX,e_iPosY);;
		Vector3	l_vMovePos(l_vUpPos-m_vMouseDownPos);
		float	l_fLength = l_vMovePos.Length();
		if( l_fLength == 0 )
			return;
		float	l_fStregth = l_fLength/cGameApp::m_svGameResolution.Length();
		l_fStregth *= 2;
		l_vMovePos /= l_fLength;
		l_vMovePos *= l_fStregth;
		this->m_pShockBoobsShader->SetShockDirection(l_vMovePos);
	}
}

void	cMyBoobsPainter::SetParmater(float e_fShockSpeed,float e_fShockSpeedMinus,float e_fShockStrgethValue,float e_fMaximumShockStregth)
{
	m_pShockBoobsShader->SetParmater(e_fShockSpeed,e_fShockSpeedMinus,e_fShockStrgethValue,e_fMaximumShockStregth);
}

void	cMyBoobsPainter::SetShockDirection(Vector3 e_vDirection)
{
	m_pShockBoobsShader->SetShockDirection(e_vDirection);
}

bool	cMyBoobsPainter::SaveFile(char*e_strFileName)
{
	std::string	l_strHeightFileName = UT::ChangeFileExtensionName(e_strFileName,"height").c_str();
	NvFile*l_pHeightMapFile = UT::MyFileOpen(l_strHeightFileName.c_str(),"wb");
	if( !l_pHeightMapFile )
		return false;
	NvFWrite(this->m_pFloatPixelDataForModify,sizeof(float),this->m_pTerrianVertex->GetGridHeight()*m_pTerrianVertex->GetGridWidth(),l_pHeightMapFile);
	NvFClose(l_pHeightMapFile);
	char*l_strDirectory = UT::GetDirectoryWithoutFileName(e_strFileName);
	std::string	l_CopyImageFileName = l_strDirectory;
	l_CopyImageFileName += UT::GetFileNameWithoutFullPath(m_pOriginalPaintImage->GetTexture()->m_pstrFullFileName->c_str(),false);
	UT::MyFileCopy(m_pOriginalPaintImage->GetTexture()->m_pstrFullFileName->c_str(),l_CopyImageFileName.c_str());
	//if(this->m_pOriginalPaintImage->GetTexture()->m_pstrFullFileName)
	//{
	//	if(UT::IsFileExists(m_pOriginalPaintImage->GetTexture()->m_pstrFullFileName->c_str()))
	//	{
	//		NvFile*l_pImageFile = UT::MyFileOpen(m_pOriginalPaintImage->GetTexture()->m_pstrFullFileName->c_str(),"rb");
	//		if( l_pImageFile )
	//		{
	//			long	l_uiFileSize = UT::GetFileSize(l_pImageFile);
	//			char*l_Temp = new char[l_uiFileSize];
	//			NvFRead(l_Temp,1,l_uiFileSize,l_pImageFile);
	//			char*l_strDirectory = UT::GetDirectoryWithoutFileName(e_strFileName);
	//			std::string	l_CopyImageFileName = l_strDirectory;
	//			l_CopyImageFileName += UT::GetFileNameWithoutFullPath(m_pOriginalPaintImage->GetTexture()->m_pstrFullFileName->c_str(),false);
	//			NvFile*l_pCopyImageFile = UT::MyFileOpen(l_CopyImageFileName.c_str(),"wb");
	//			if( l_pCopyImageFile )
	//			{
	//				NvFWrite(l_Temp,1,l_uiFileSize,l_pCopyImageFile);
	//				NvFClose(l_pCopyImageFile);
	//			}
	//			delete l_Temp;
	//			NvFClose(l_pImageFile);
	//		}
	//	}
	//}
	TiXmlDocument doc;
	TiXmlElement *l_pMyBoobsPainteRoot = new TiXmlElement( L"cMyBoobsPainter" );  	doc.LinkEndChild( l_pMyBoobsPainteRoot );
	l_pMyBoobsPainteRoot->SetAttribute(L"GridWidth",this->m_pTerrianVertex->GetGridWidth());
	l_pMyBoobsPainteRoot->SetAttribute(L"GridHeight",m_pTerrianVertex->GetGridHeight());
	l_pMyBoobsPainteRoot->SetAttribute(L"ImageMoveDistance",ValueToStringW(m_fImageMoveDistance).c_str());
	l_pMyBoobsPainteRoot->SetAttribute(L"HeightMapFile",ValueToStringW(UT::GetFileNameWithoutFullPath(l_strHeightFileName,false)).c_str());
	std::wstring	l_strOriginalImageFileName = UT::CharToWchar(this->m_pOriginalPaintImage->GetTexture()->m_pstrFullFileName->c_str());
	l_pMyBoobsPainteRoot->SetAttribute(L"OriginalImageName",UT::GetFileNameWithoutFullPath(l_strOriginalImageFileName.c_str(),false));
	{
		//Shader Parameter
		TiXmlElement *l_pShaderParmeterElement = new TiXmlElement( L"ShaderParmeter" );	l_pMyBoobsPainteRoot->LinkEndChild( l_pShaderParmeterElement );
		l_pShaderParmeterElement->SetAttribute(L"ShockSpeed",ValueToStringW(m_pShockBoobsShader->GetShockSpeed()).c_str());
		l_pShaderParmeterElement->SetAttribute(L"ShockSpeedMinus",ValueToStringW(m_pShockBoobsShader->GetShockSpeedMinus()).c_str());
		l_pShaderParmeterElement->SetAttribute(L"ShockStrgethValue",ValueToStringW(m_pShockBoobsShader->GetShockStrgethValue()).c_str());
		l_pShaderParmeterElement->SetAttribute(L"MaximumShockStregth",ValueToStringW(m_pShockBoobsShader->GetMaximumShockStregth()).c_str());
	}
	return doc.SaveFile( e_strFileName );
}

bool	cMyBoobsPainter::ChangePaintingImage(char*e_strFileName)
{
	SAFE_DELETE(m_pOriginalPaintImage);
	m_pOriginalPaintImage = new cBaseImage(e_strFileName);
	SetSize(*m_pOriginalPaintImage->GetWidth(),*m_pOriginalPaintImage->GetHeight(),4);
	return true;
}