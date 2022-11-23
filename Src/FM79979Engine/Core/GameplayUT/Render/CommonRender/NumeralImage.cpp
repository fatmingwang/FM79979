#include "NumeralImage.h"
#include "SimplePrimitive.h"

#include "../PuzzleImage/PuzzleImage.h"
#include "../../GameApp/GameApp.h"
#include "../MPDI/MPDI.h"
#include "../../ResourceMaintaner/ObjectXMLParser.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cNumeralImage);
	TYPDE_DEFINE_MARCO(cTimeNumerialImage);
	const int	g_iNumNumber = 20;
	cNumeralImage::cNumeralImage(char*e_strImageName):cBaseImage(e_strImageName)
	{
		m_bValueChanged = true;
		m_iVertexBufferCount = 0;
		m_pvVertexBuffer = nullptr;
		m_pvTextureUVBuffer = nullptr;
		m_pvColorBuffer = nullptr;
		GenerateVertexBuffer(g_iNumNumber);
		m_bDrawOnCenter = false;
		m_i64Value = 0;
		this->m_iSingleImageHeight = m_iHeight;	
		this->m_iSingleImageWidth = m_iWidth/10;
		m_iHeight = m_iSingleImageHeight;
		m_iWidth =  m_iSingleImageWidth;
		//this->m_iImageSpace = e_iImageSpace;
		this->m_iNumIndex = 10;
		m_pfTexCoordinate = new float[4*m_iNumIndex];
		float	l_fStep = this->m_fUV[2]/(float)m_iNumIndex;
		for( int i=0;i<m_iNumIndex;++i )
		{
			m_pfTexCoordinate[i*4  ] = (float)i*l_fStep;
			m_pfTexCoordinate[i*4+1] = m_fUV[1];
			m_pfTexCoordinate[i*4+2] = (float)i*l_fStep+l_fStep;
			m_pfTexCoordinate[i*4+3] = m_fUV[3];
		}
		m_eDirection = eD_LEFT;
	}

	cNumeralImage::cNumeralImage(cBaseImage*e_pImage,float*e_pftexCoordinate):cBaseImage(e_pImage)
	{
		m_bValueChanged = true;
		m_iVertexBufferCount = 0;
		m_pvVertexBuffer = nullptr;
		m_pvTextureUVBuffer = nullptr;
		m_pvColorBuffer = nullptr;
		GenerateVertexBuffer(g_iNumNumber);
		m_bDrawOnCenter = false;
		m_i64Value = 0;
		m_eDirection = eD_LEFT;
		//this->m_iImageSpace = e_iImageSpace;
		this->m_iNumIndex = 10;
		memcpy(m_fUV,e_pftexCoordinate,sizeof(float)*4);
		m_pfTexCoordinate = new float[4*m_iNumIndex];
		float	l_fStep = (m_fUV[2]-m_fUV[0])/(float)m_iNumIndex;
		for( int i=0;i<m_iNumIndex;++i )
		{
			m_pfTexCoordinate[i*4  ] = (float)i*l_fStep+m_fUV[0];
			m_pfTexCoordinate[i*4+1] = m_fUV[1];
			m_pfTexCoordinate[i*4+2] = (float)i*l_fStep+l_fStep+m_fUV[0];
			m_pfTexCoordinate[i*4+3] = m_fUV[3];
		}		
		m_iSingleImageHeight = m_iHeight;
		m_iSingleImageWidth = m_iWidth/10;
	}

	cNumeralImage::cNumeralImage(cBaseImage*e_pImage0,cBaseImage*e_pImage9):cBaseImage(e_pImage0)
	{
		m_bValueChanged = true;
		m_iVertexBufferCount = 0;
		m_pvVertexBuffer = nullptr;
		m_pvTextureUVBuffer = nullptr;
		m_pvColorBuffer = nullptr;
		GenerateVertexBuffer(g_iNumNumber);
		m_bDrawOnCenter = false;
		m_i64Value = 0;
		m_eDirection = eD_LEFT;
		//this->m_iImageSpace = e_iImageSpace;
		this->m_iNumIndex = 10;
		m_fUV[0] = e_pImage0->GetUV()[0];m_fUV[1] = e_pImage0->GetUV()[1];
		m_fUV[2] = e_pImage9->GetUV()[2];m_fUV[3] = e_pImage9->GetUV()[3];
		m_pfTexCoordinate = new float[4*m_iNumIndex];
		float	l_fStep = (m_fUV[2]-m_fUV[0])/(float)m_iNumIndex;
		cPuzzleImageUnit*l_pPIUnit = (cPuzzleImageUnit*)e_pImage0;
		for( int i=0;i<m_iNumIndex;++i )
		{
			float*l_pUV = l_pPIUnit->GetUV();
			memcpy(&m_pfTexCoordinate[i*4],l_pUV,sizeof(float)*4);
			l_pPIUnit = l_pPIUnit->GetNext();
			//m_pfTexCoordinate[i*4  ] = ;
			//m_pfTexCoordinate[i*4+1] = m_fUV[1];
			//m_pfTexCoordinate[i*4+2] = i*l_fStep+l_fStep+m_fUV[0];
			//m_pfTexCoordinate[i*4+3] = m_fUV[3];
		}		
		m_iSingleImageHeight = m_iHeight;
		m_iSingleImageWidth = m_iWidth;
	}

	cNumeralImage::cNumeralImage(cNumeralImage*e_pNumeralImage):cBaseImage(e_pNumeralImage)
	{
		m_bValueChanged = true;
		GenerateVertexBuffer(e_pNumeralImage->m_iVertexBufferCount);
		m_bDrawOnCenter = e_pNumeralImage->m_bDrawOnCenter;
		m_i64Value = 0;
		m_eDirection = e_pNumeralImage->m_eDirection;
		//this->m_iImageSpace = e_iImageSpace;
		this->m_iNumIndex = e_pNumeralImage->m_iNumIndex;
		memcpy(m_fUV,e_pNumeralImage->m_fUV,sizeof(float)*4);
		int	l_iSize = 4*m_iNumIndex;
		m_pfTexCoordinate = new float[l_iSize];
		memcpy(m_pfTexCoordinate,e_pNumeralImage->m_pfTexCoordinate,sizeof(float)*l_iSize);
		m_iSingleImageHeight = m_iHeight = e_pNumeralImage->m_iSingleImageHeight;
		m_iSingleImageWidth = m_iWidth = e_pNumeralImage->m_iSingleImageWidth;
	}

	//<cNumeralImage PI="Image/commons.pi" Image0="number_0" Image9="number_9" Pos="547,121" />
	//	<NumeralPos>
	//		<cSubMPDI cMPDIList="Image/commons_mpdi.mpdi" cMPDI="CommonsNum" cSubMPDI="HeartNum" />
	//	</NumeralPos>
	//</cNumeralImage>
	cNumeralImage*	cNumeralImage::GetMe(TiXmlElement*e_pXmlelement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pXmlelement,cNumeralImage::TypeID);
		cBaseImage*l_pBG0 = 0;
		cBaseImage*l_pBG9 = 0;
		cPuzzleImage*l_pPI = 0;
		bool l_bDrawCenter = false;
		Vector3	l_vPos = Vector3::Zero;;
		POINT	l_Size = { 0,0 };
		cNumeralImage*l_pNumeralImage = 0;
		std::wstring l_strNumeralImageName;
		PARSE_ELEMENT_START(e_pXmlelement)
			COMPARE_NAME("Image0")
			{
				if( l_pPI )
					l_pBG0 = l_pPI->GetObject(l_strValue);
			}
			else
			COMPARE_NAME("Image9")
			{
				if( l_pPI )
					l_pBG9 = l_pPI->GetObject(l_strValue);
			}
			else
			COMPARE_NAME("PI")
			{
				l_pPI = cGameApp::GetPuzzleImageByFileName(l_strValue);
			}
			else
			COMPARE_NAME("Pos")
			{
				l_vPos = VALUE_TO_VECTOR3;
			}
			else
			COMPARE_NAME("DrawCenter")
			{
				l_bDrawCenter = VALUE_TO_BOOLEAN;
			}
			else
			COMPARE_NAME("Size")
			{
				l_Size = VALUE_TO_POINT;
			}
			else
			COMPARE_NAME("Name")
			{
				l_strNumeralImageName = l_strValue;
			}
		PARSE_NAME_VALUE_END
		if( l_pBG9 && l_pBG0 )
		{
			l_pNumeralImage = new cNumeralImage(l_pBG0,l_pBG9);
			if (l_strNumeralImageName.length())
			{
				l_pNumeralImage->SetName(l_strNumeralImageName.c_str());
			}
			if (l_Size.x != 0 || l_Size.y != 0)
			{
				l_pNumeralImage->SetSingleImageWidth(l_Size.x);
				l_pNumeralImage->SetSingleImageHeight(l_Size.y);
			}
			e_pXmlelement = e_pXmlelement->FirstChildElement();
			if( e_pXmlelement )
			{
				COMPARE_TARGET_ELEMENT_VALUE(e_pXmlelement,"NumeralPos")
				{
					cSubMPDI*l_pSubMPDI = cSubMPDI::GetMe(e_pXmlelement->FirstChildElement(),false);
					if( l_pSubMPDI )
						l_vPos = l_pSubMPDI->GetPointList()[0];
				}
			}
			l_pNumeralImage->SetLocalPosition(l_vPos);
			if (l_bDrawCenter)
				l_pNumeralImage->SetDrawOnCenter(l_bDrawCenter);
		}
		return l_pNumeralImage;
	}

	cNumeralImage::~cNumeralImage()
	{
		SAFE_DELETE(m_pfTexCoordinate);
		SAFE_DELETE(m_pvVertexBuffer);
		SAFE_DELETE(m_pvTextureUVBuffer);
		SAFE_DELETE(m_pvColorBuffer);
	}

	void	cNumeralImage::SetColor(Vector4 e_vColor)
	{
		if (m_vColor != e_vColor)
		{
			m_vColor = e_vColor;
			this->m_bValueChanged = true;
		}
	}


	void	cNumeralImage::GenerateVertexBuffer(int e_iNumBuffer)
	{
		m_iVertexBufferCount = e_iNumBuffer;
		//draw by quad...!?
		m_pvVertexBuffer = new Vector2[m_iVertexBufferCount*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];
		m_pvTextureUVBuffer = new Vector2[m_iVertexBufferCount*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];
		m_pvColorBuffer = new Vector4[m_iVertexBufferCount*TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];
		for (int i = 0; i<m_iVertexBufferCount * TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT; ++i)
			m_pvColorBuffer[i] = Vector4::One;
	}

	void	cNumeralImage::Draw(const char*e_strData,int e_iCount,int e_iPosX,int e_iPosY,float*e_pmat,bool e_bCenter)
	{
		int l_iNumTriangles = 0;
		cMatrix44 l_mat;
		if (ProcessTriangulatorRenderData(e_strData, e_iCount, e_iPosX, e_iPosY, e_pmat, e_bCenter, l_mat, l_iNumTriangles))
		{
			RenderTrianglesWithMatrix((float*)m_pvVertexBuffer, (float*)m_pvTextureUVBuffer, (float*)m_pvColorBuffer, l_mat, 2, l_iNumTriangles * A_QUAD_TWO_TRIANGLES);
		}
	}
	bool	cNumeralImage::ProcessTriangulatorRenderData(const char* e_strData, int e_iNumberStringLength, int e_iPosX, int e_iPosY, float* e_pmat, bool e_bCenter, cMatrix44& e_OutMat, int& e_iNumTriangles)
	{
		if (!this->m_bVisible)
		{
			return false;
		}
		std::string	l_str = e_strData;
		int	l_iStringLength = e_iNumberStringLength;
		float	l_fHalfWidth = 0.f;
		float	l_fHalfHeight = 0.f;
		float	l_fOffsetPosX = 0.f;
		float	l_fOffsetPosY = 0.f;
		//from small to big
		assert(l_iStringLength < m_iVertexBufferCount && "cNumeralImage buffersize is not enough...");
		if (l_iStringLength > m_iVertexBufferCount)
		{
			l_iStringLength = m_iVertexBufferCount;
		}
		switch (m_eDirection)
		{
		case eD_LEFT:
			l_fOffsetPosX = (float)m_iSingleImageWidth;
			l_fHalfWidth = (m_iSingleImageWidth * l_iStringLength / 2.f);
			l_fHalfHeight = m_iSingleImageHeight / 2.f;
			break;
		case eD_DOWN:
			l_fOffsetPosY = (float)m_iSingleImageHeight;
			l_fHalfWidth = m_iSingleImageWidth / 2.f;
			l_fHalfHeight = m_iSingleImageHeight * l_iStringLength / 2.f;
			break;
		default:
			assert(0 && " lazy to implement cNumeralImage::Draw");
			break;
		}
		if (m_bValueChanged)
		{
			m_bValueChanged = false;
			if (l_iStringLength)
			{
				float	l_fImageHalfWidth = m_iSingleImageWidth / 2.f;
				float	l_fImageHalfHeight = m_iSingleImageHeight / 2.f;
				float	l_fPosX = -l_fHalfWidth;
				float	l_fPosY = -l_fHalfHeight;
				for (int i = 0; i < l_iStringLength; ++i)
				{
					float* l_pfVertices = (float*)&m_pvVertexBuffer[i * TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];
					float* l_pfUV = (float*)&m_pvTextureUVBuffer[i * TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];
					float* l_pfColor = (float*)&m_pvColorBuffer[i * TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT];
					AssignUVDataTo2Triangles(&m_pfTexCoordinate[(l_str[i] - '0') * 4], l_pfUV, false);
					for (int j = 0; j < TWO_TRIANGLE_VERTICES_TO_QUAD_COUNT; ++j)
					{
						memcpy(&l_pfColor[j * 4], &m_vColor, sizeof(Vector4));
					}
					Vector2	l_vPos[4];
					l_vPos[0] = Vector2(l_fPosX, l_fPosY);
					l_vPos[1] = Vector2(l_fPosX + (float)m_iSingleImageWidth, l_fPosY);
					l_vPos[2] = Vector2(l_fPosX, l_fPosY + (float)m_iSingleImageHeight);
					l_vPos[3] = Vector2(l_fPosX + (float)m_iSingleImageWidth, l_fPosY + (float)m_iSingleImageHeight);
					Assign4VerticesDataTo2Triangles((float*)l_vPos, l_pfVertices, 2);
					l_fPosX += l_fOffsetPosX;
					l_fPosY += l_fOffsetPosY;
				}
			}
			else
			{
				return false;
			}
		}
		cMatrix44 l_mat;
		if (e_bCenter)
		{
			l_mat = cMatrix44::TranslationMatrix((float)e_iPosX, (float)e_iPosY, 0);
		}
		else
		{
			int	l_iMinusCount = l_iStringLength - 1;
			l_mat = cMatrix44::TranslationMatrix((float)e_iPosX, (float)e_iPosY, 0) * cMatrix44::TranslationMatrix(l_fHalfWidth - l_iMinusCount * m_iSingleImageWidth, l_fHalfHeight, 0.f);
		}
		if (e_pmat)
		{
			l_mat = cMatrix44(e_pmat) * l_mat;
		}
		e_iNumTriangles = l_iStringLength;
		e_OutMat = l_mat;
		return true;
	}
	void	cNumeralImage::Draw(int64	e_iValue,int e_iPosX,int e_iPosY,float*e_pmat,bool e_bCenter)
	{
		if (!m_bVisible)
		{
			return;
		}
		std::string	l_str = ValueToString(e_iValue);
		int	l_iStringLength = (int)strlen(l_str.c_str());
		Draw(l_str.c_str(), l_iStringLength,e_iPosX,e_iPosY,e_pmat,e_bCenter);
	}
	void	cNumeralImage::Draw(int	e_iValue,int e_iPosX,int e_iPosY,float*e_pmat,bool e_bCenter)
	{
		if( !m_bVisible )
			return;
		char	l_str[TEMP_SIZE];
		itoa(e_iValue,l_str,10);
		int	l_iStringLength = (int)strlen(l_str);
		Draw(l_str, l_iStringLength,e_iPosX,e_iPosY,e_pmat,e_bCenter);
	}

	void	cNumeralImage::DrawOnCenter(int64	e_iValue,int e_iPosX,int e_iPosY,float*e_pmat)
	{
		Draw(e_iValue,e_iPosX,e_iPosY,e_pmat,true);
	}

	void	cNumeralImage::DrawOnCenter(int	e_iValue,int e_iPosX,int e_iPosY,float*e_pmat)
	{
		Draw(e_iValue,e_iPosX,e_iPosY,e_pmat,true);
	}

	void	cNumeralImage::Render()
	{
		if (this->m_bDrawOnCenter)
		{
			DrawOnCenter(m_i64Value, 0, 0, this->GetWorldTransform());
		}
		else
		{
			Draw(m_i64Value, 0, 0, this->GetWorldTransform());
		}
	}

	void	cNumeralImage::SetValue(int64 e_i64Value)
	{
		if (m_i64Value != e_i64Value)
			m_bValueChanged = true;
		m_i64Value = e_i64Value;
	}

	cTexture* cNumeralImage::GetTriangulatorRenderDataForBatchRendering(int& e_iOutNumVertex, Vector3* e_pvOutPos, Vector2* e_pvOutUV, Vector4* e_pvOutColor)
	{
		if (!m_bVisible)
		{
			return nullptr;
		}
		std::string	l_str = ValueToString(m_i64Value);
		int	l_iStringLength = (int)strlen(l_str.c_str());
		if (this->m_bDrawOnCenter)
		{
			DrawOnCenter(m_i64Value, 0, 0, this->GetWorldTransform());
		}
		else
		{
			Draw(m_i64Value, 0, 0, this->GetWorldTransform());
		}
		cMatrix44 l_Mat;
		int l_iNumTriangles = 0;
		ProcessTriangulatorRenderData(l_str.c_str(), l_iStringLength,0,0,this->GetWorldTransform(),this->m_bDrawOnCenter, l_Mat, l_iNumTriangles);
		for (int i = 0; i < l_iNumTriangles; ++i)
		{

		}
		return m_pTexture;
	}

	cTimeNumerialImage::cTimeNumerialImage(cBaseImage*e_pImage0,cBaseImage*e_pImage9,cCueToStartCurveWithTime*e_pHourSubMPDI,cCueToStartCurveWithTime*e_pMinSubMPDI,cCueToStartCurveWithTime*e_pSecondSubMPDI,cRenderObject*e_pDisableObject):cNumeralImage(e_pImage0,e_pImage9)
	{
		m_pDisableObject = nullptr;
		if( e_pDisableObject )
		{
			cRenderObject*l_pNamedTypedObject = dynamic_cast<cRenderObject*>(e_pDisableObject->Clone());
			//have no idea why this occur crush
			//m_pDisableObject = reinterpret_cast<cRenderObject*>(l_pNamedTypedObject);
			m_pDisableObject = dynamic_cast<cRenderObject*>(l_pNamedTypedObject);
		}
		m_bEnableHour = true;
		m_bEnableMinute = true;
		m_bEnableSecond = true;
		//m_iSingleImageWidth = e_pImage0->GetOriginalSize().x;
		m_vSecondPos = Vector3::Zero;
		int	l_iImageWidth = 16;
		if( e_pImage0 )
			l_iImageWidth = e_pImage0->GetWidth();
		if(e_pSecondSubMPDI)
		{
			if( e_pSecondSubMPDI->GetOriginalPointList().size() > 0 )
				m_vSecondPos = e_pSecondSubMPDI->GetOriginalPointList()[0];
		}
		else
		{
			m_bEnableSecond = false;
		}

		if(e_pMinSubMPDI)
		{
			if( e_pMinSubMPDI->GetOriginalPointList().size() > 0 )
				m_vMinPos = e_pMinSubMPDI->GetOriginalPointList()[0];
		}
		else
		{
			m_bEnableMinute = false;
			m_vMinPos = m_vSecondPos;
			m_vMinPos.x -= l_iImageWidth*2;
		}


		if(e_pHourSubMPDI)
		{
			if( e_pHourSubMPDI->GetOriginalPointList().size() > 0 )
				m_vHourPos = e_pHourSubMPDI->GetOriginalPointList()[0];			
		}
		else
		{
			m_vHourPos = m_vMinPos;
			m_vHourPos.x -= (float)l_iImageWidth*2.f;
			m_bEnableHour = false;
		}
	}

	cTimeNumerialImage::~cTimeNumerialImage()
	{
		SAFE_DELETE(m_pDisableObject);
	}

	//<cTimeNumerialImage PI="" Image0="number_0" Image9="number_9">
	//	<HourPos>
	//		<cSubMPDI cMPDIList="Image/Main_Massage.mpdi" cMPDI="Main" cSubMPDI="Number_0" />
	//	</HourPos>
	//	<MinPos>
	//		<cSubMPDI cMPDIList="Image/Main_Massage.mpdi" cMPDI="Main" cSubMPDI="Number_0" />
	//	</MinPos>
	//	<SecPos>
	//		<cSubMPDI cMPDIList="Image/Main_Massage.mpdi" cMPDI="Main" cSubMPDI="Number_0" />
	//	</SecPos>
	//</cTimeNumerialImage>
	cTimeNumerialImage*cTimeNumerialImage::GetMe(TiXmlElement*e_pXmlelement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pXmlelement,cTimeNumerialImage::TypeID);			
		const wchar_t*l_strPI = e_pXmlelement->Attribute(L"PI");
		const wchar_t*l_strImage0 = e_pXmlelement->Attribute(L"Image0");
		const wchar_t*l_strImage9 = e_pXmlelement->Attribute(L"Image9");
		cBaseImage*l_pImage0 = 0;
		cBaseImage*l_pImage9 = 0;
		if( l_strPI )
		{
			 cPuzzleImage*l_pPI = cGameApp::GetPuzzleImageByFileName(l_strPI);
			 if( l_pPI )
			 {
				 if( l_strImage0 )
					 l_pImage0 = l_pPI->GetObject(l_strImage0);
				 if( l_strImage9 )
					 l_pImage9 = l_pPI->GetObject(l_strImage9);
			 }
			 else
			 {
				UT::ErrorMsg(L"Numerial image PI not exist",l_strPI);
			 }
		}
		if( !l_pImage0 || !l_pImage9 )
		{
			UT::ErrorMsg(L"Numerial image l_str Image0 or l_str Image9 not exist",l_strImage0);
		}
		e_pXmlelement = e_pXmlelement->FirstChildElement();
		cSubMPDI*l_pHour = 0;
		cSubMPDI*l_pMin = 0;
		cSubMPDI*l_pSec = 0;
		cRenderObject*l_pRenderObjectBehavior = 0;
		while( e_pXmlelement )
		{
			if(!wcscmp(e_pXmlelement->Value(),L"HourPos"))
			{
				l_pHour = cSubMPDI::GetMe(e_pXmlelement->FirstChildElement(),false);
			}
			else
			if(!wcscmp(e_pXmlelement->Value(),L"MinPos"))
			{
				l_pMin = cSubMPDI::GetMe(e_pXmlelement->FirstChildElement(),false);
			}
			else
			if(!wcscmp(e_pXmlelement->Value(),L"SecPos"))
			{
				l_pSec = cSubMPDI::GetMe(e_pXmlelement->FirstChildElement(),false);
			}
			else
			if(!wcscmp(e_pXmlelement->Value(),L"DisableObject"))
			{
				l_pRenderObjectBehavior = GetRenderObjectBehavior(e_pXmlelement->FirstChildElement());
			}
			e_pXmlelement = e_pXmlelement->NextSiblingElement();
		}
		cTimeNumerialImage*l_pTimeNumerialImage = new cTimeNumerialImage(l_pImage0,l_pImage9,l_pHour,l_pMin,l_pSec,l_pRenderObjectBehavior);
		if( l_pRenderObjectBehavior )
			SAFE_DELETE(l_pRenderObjectBehavior);
		return l_pTimeNumerialImage;
	}

	int	AssignNumerialData(int* e_piIndex,float*e_pfUV,Vector4 e_vColor,int e_iImageWidth,int e_iImageHeight,Vector3 e_vPos,int e_iValue,int e_iShowNumImage)
	{
		float	l_fPosX = e_vPos.x;
		float	l_fPosY = e_vPos.y;
		wchar_t	l_Value[2];
		int	l_iNum = e_iShowNumImage;
		l_Value[0] = e_iValue%10;
		l_Value[1] = e_iValue/10;
		for( int i=0;i<l_iNum;++i )
		{
			int	l_iIndex = *e_piIndex;
			float	*l_pfVertices	=	&g_fGlobalTempBufferForRenderVertices[l_iIndex*3*6];
			float	*l_pfUV			=	&g_fGlobalTempBufferForRenderUV[l_iIndex*2*6];
			float	*l_pfColor		=	&g_fGlobalTempBufferForRenderColor[l_iIndex*4*6];
			AssignUVDataTo2Triangles(&e_pfUV[(l_Value[i])*4],l_pfUV,false);
			for (int i = 0; i < 6; ++i)
			{
				memcpy(&l_pfColor[i * 4], &e_vColor, sizeof(Vector4));
			}
			Vector3	l_vPos[4];
			l_vPos[0] = Vector3(l_fPosX						,l_fPosY,0.f);
			l_vPos[1] = Vector3(l_fPosX+ (float)e_iImageWidth	,l_fPosY,0.f);
			l_vPos[2] = Vector3(l_fPosX						,l_fPosY+ (float)e_iImageHeight,0.f);
			l_vPos[3] = Vector3(l_fPosX+ (float)e_iImageWidth	,l_fPosY+ (float)e_iImageHeight,0.f);
			Assign4VerticesDataTo2Triangles((float*)l_vPos,l_pfVertices,3);
			l_fPosX -= (float)e_iImageWidth;
			++*e_piIndex;
		}
		return l_iNum;
	}

	void	cTimeNumerialImage::Init()
	{
		if( m_pDisableObject )
		{ 
			m_pDisableObject->Init();
		}
	}

	void	cTimeNumerialImage::Update(float e_fElpaseTime)
	{
		if (m_pDisableObject)
		{
			m_pDisableObject->Update(e_fElpaseTime);
		}
	}

	void	cTimeNumerialImage::Render()
	{
		if( !m_bVisible )
			return;
		int	l_iSecond = (int)(this->m_i64Value%60);
		int	l_iMin = (int)(this->m_i64Value/60%60);
		int	l_iHour = (int)(this->m_i64Value/60/60%24);
		int	l_iIndex = 0;
		int	l_iNum = 0;
		if (m_bEnableSecond)
		{
			l_iNum += AssignNumerialData(&l_iIndex, this->m_pfTexCoordinate, m_vColor, this->m_iSingleImageWidth, this->m_iSingleImageHeight, m_vSecondPos, l_iSecond, 2);
		}
		if (m_bEnableMinute)
		{
			l_iNum += AssignNumerialData(&l_iIndex, this->m_pfTexCoordinate, m_vColor, this->m_iSingleImageWidth, this->m_iSingleImageHeight, this->m_vMinPos, l_iMin, 2);
		}
		if (m_bEnableHour)
		{
			l_iNum += AssignNumerialData(&l_iIndex, this->m_pfTexCoordinate, m_vColor, this->m_iSingleImageWidth, this->m_iSingleImageHeight, this->m_vHourPos, l_iHour, 1);
		}
		this->ApplyImage();

		RenderTrianglesWithMatrix(g_fGlobalTempBufferForRenderVertices, g_fGlobalTempBufferForRenderUV, g_fGlobalTempBufferForRenderColor, cMatrix44::Identity, 3, l_iNum * A_QUAD_TWO_TRIANGLES);
		if( m_pDisableObject && m_i64Value == 0 )
		{
			m_pDisableObject->Render();
		}
	}
}