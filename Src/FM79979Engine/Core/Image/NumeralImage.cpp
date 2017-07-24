#include "../stdafx.h"
#include "NumeralImage.h"
#include "SimplePrimitive.h"

#include "../GLSL/Shader.h"
#include "PuzzleImage.h"
#include "../GamePlayUT/GameApp.h"
#include "../XML/XMLLoader.h"
#include "../GamePlayUT/ObjectXMLParser.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cNumeralImage);
	TYPDE_DEFINE_MARCO(cTimeNumerialImage);
	cNumeralImage::cNumeralImage(char*e_strImageName):cBaseImage(e_strImageName)
	{
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
			m_pfTexCoordinate[i*4  ] = i*l_fStep;
			m_pfTexCoordinate[i*4+1] = m_fUV[1];
			m_pfTexCoordinate[i*4+2] = i*l_fStep+l_fStep;
			m_pfTexCoordinate[i*4+3] = m_fUV[3];
		}
		m_eDirection = eD_LEFT;
	}

	cNumeralImage::cNumeralImage(cBaseImage*e_pImage,float*e_pftexCoordinate):cBaseImage(e_pImage)
	{
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
			m_pfTexCoordinate[i*4  ] = i*l_fStep+m_fUV[0];
			m_pfTexCoordinate[i*4+1] = m_fUV[1];
			m_pfTexCoordinate[i*4+2] = i*l_fStep+l_fStep+m_fUV[0];
			m_pfTexCoordinate[i*4+3] = m_fUV[3];
		}		
		m_iSingleImageHeight = m_iHeight;
		m_iSingleImageWidth = m_iWidth/10;
	}

	cNumeralImage::cNumeralImage(cBaseImage*e_pImage0,cBaseImage*e_pImage9):cBaseImage(e_pImage0)
	{
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
		Vector3	l_vPos = Vector3::Zero;;
		cNumeralImage*l_pNumeralImage = 0;
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
		PARSE_NAME_VALUE_END
		if( l_pBG9 && l_pBG0 )
		{
			l_pNumeralImage = new cNumeralImage(l_pBG0,l_pBG9);
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
		}
		return l_pNumeralImage;
	}

	cNumeralImage::~cNumeralImage()
	{
		SAFE_DELETE(m_pfTexCoordinate);
	}

	void	cNumeralImage::Draw(const char*e_strData,int e_iCount,int e_iPosX,int e_iPosY,float*e_pmat,bool e_bCenter)
	{
		if( !m_bVisible )
			return;
		std::string	l_str = e_strData;
		int	l_iNum = e_iCount;
		if( l_iNum )
		{
			float	l_fHalfWidth = 0.f;
			float	l_fHalfHeight = 0.f;
			float	l_fOffsetPosX = 0.f;
			float	l_fOffsetPosY = 0.f;
			switch(m_eDirection)
			{
				case eD_LEFT:
					l_fOffsetPosX = (float)m_iSingleImageWidth;
					l_fHalfWidth = (m_iSingleImageWidth*l_iNum/2.f);
					l_fHalfHeight = m_iSingleImageHeight/2.f;
					break;
				case eD_DOWN:
					l_fOffsetPosY = (float)m_iSingleImageHeight;
					l_fHalfWidth = m_iSingleImageWidth/2.f;
					l_fHalfHeight = m_iSingleImageHeight*l_iNum/2.f;
					break;
			}
			float	l_fImageHalfWidth = m_iSingleImageWidth/2.f;
			float	l_fImageHalfHeight = m_iSingleImageHeight/2.f;
			float	l_fPosX = -l_fHalfWidth;
			float	l_fPosY = -l_fHalfHeight;
			//from small to big
			for( int i=0;i<l_iNum;++i )
			{
				float	*l_pfVertices	=	&g_fMPDIOptmizeRenderVertices[i*3*6];
				float	*l_pfUV			=	&g_fMPDIOptmizeRenderUV[i*2*6];
				float	*l_pfColor		=	&g_fMPDIOptmizeRenderColor[i*4*6];

				AssignUVDataTo2Triangles(&m_pfTexCoordinate[(l_str[i]-'0')*4],l_pfUV,false);
				for( int j=0;j<6;++j )
					memcpy(&l_pfColor[j*4],&m_vColor,sizeof(Vector4));
				Vector3	l_vPos[4];
				l_vPos[0] = Vector3(l_fPosX						,l_fPosY,0.f);
				l_vPos[1] = Vector3(l_fPosX+m_iSingleImageWidth	,l_fPosY,0.f);
				l_vPos[2] = Vector3(l_fPosX						,l_fPosY+m_iSingleImageHeight,0.f);
				l_vPos[3] = Vector3(l_fPosX+m_iSingleImageWidth	,l_fPosY+m_iSingleImageHeight,0.f);
				Assign4VerticesDataTo2Triangles((float*)l_vPos,l_pfVertices,3);
				l_fPosX += l_fOffsetPosX;
				l_fPosY += l_fOffsetPosY;
			}
			UseShaderProgram(DEFAULT_SHADER);
			this->ApplyImage();
			cMatrix44 l_mat;
			if( e_bCenter )
			{
				l_mat = cMatrix44::TranslationMatrix((float)e_iPosX,(float)e_iPosY,0);
			}
			else
			{
				int	l_iMinusCount = l_iNum-1;
				l_mat = cMatrix44::TranslationMatrix((float)e_iPosX,(float)e_iPosY,0)*cMatrix44::TranslationMatrix(l_fHalfWidth-l_iMinusCount*m_iSingleImageWidth,l_fHalfHeight,0.f);
			}
			if( e_pmat )
			{
				l_mat = l_mat*cMatrix44(e_pmat);
			}
			SetupShaderWorldMatrix(l_mat);
			myGlVertexPointer(3,g_fMPDIOptmizeRenderVertices);
			myGlUVPointer(2,g_fMPDIOptmizeRenderUV);
			myGlColorPointer(4,g_fMPDIOptmizeRenderColor);
			MY_GLDRAW_ARRAYS(GL_TRIANGLES, 0, l_iNum*6);
		}
	}
	void	cNumeralImage::Draw(int64	e_iValue,int e_iPosX,int e_iPosY,float*e_pmat,bool e_bCenter)
	{
		if( !m_bVisible )
			return;
		std::string	l_str = ValueToString(e_iValue);
		int	l_iNum = (int)strlen(l_str.c_str());
		Draw(l_str.c_str(),l_iNum,e_iPosX,e_iPosY,e_pmat,e_bCenter);
	}
	void	cNumeralImage::Draw(int	e_iValue,int e_iPosX,int e_iPosY,float*e_pmat,bool e_bCenter)
	{
		if( !m_bVisible )
			return;
		char	l_str[TEMP_SIZE];
		itoa(e_iValue,l_str,10);
		int	l_iNum = (int)strlen(l_str);
		Draw(l_str,l_iNum,e_iPosX,e_iPosY,e_pmat,e_bCenter);
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
		if( this->m_bDrawOnCenter )
			DrawOnCenter(m_i64Value,0,0,this->GetWorldTransform());
		else
			Draw(m_i64Value,0,0,this->GetWorldTransform());
	}

	void	cNumeralImage::SetValue(int64 e_i64Value)
	{
		m_i64Value = e_i64Value;
	}

	cTimeNumerialImage::cTimeNumerialImage(cBaseImage*e_pImage0,cBaseImage*e_pImage9,cCueToStartCurveWithTime*e_pHourSubMPDI,cCueToStartCurveWithTime*e_pMinSubMPDI,cCueToStartCurveWithTime*e_pSecondSubMPDI,cRenderObject*e_pDisableObject):cNumeralImage(e_pImage0,e_pImage9)
	{
		m_pDisableObject = 0;
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
			m_vHourPos.x -= l_iImageWidth*2;
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
			float	*l_pfVertices	=	&g_fMPDIOptmizeRenderVertices[l_iIndex*3*6];
			float	*l_pfUV			=	&g_fMPDIOptmizeRenderUV[l_iIndex*2*6];
			float	*l_pfColor		=	&g_fMPDIOptmizeRenderColor[l_iIndex*4*6];
			AssignUVDataTo2Triangles(&e_pfUV[(l_Value[i])*4],l_pfUV,false);
			for( int i=0;i<6;++i )
				memcpy(&l_pfColor[i*4],&e_vColor,sizeof(Vector4));
			Vector3	l_vPos[4];
			l_vPos[0] = Vector3(l_fPosX						,l_fPosY,0.f);
			l_vPos[1] = Vector3(l_fPosX+e_iImageWidth	,l_fPosY,0.f);
			l_vPos[2] = Vector3(l_fPosX						,l_fPosY+e_iImageHeight,0.f);
			l_vPos[3] = Vector3(l_fPosX+e_iImageWidth	,l_fPosY+e_iImageHeight,0.f);
			Assign4VerticesDataTo2Triangles((float*)l_vPos,l_pfVertices,3);
			l_fPosX -= e_iImageWidth;
			++*e_piIndex;
		}
		return l_iNum;
	}

	void	cTimeNumerialImage::Init()
	{
		if( m_pDisableObject )
			m_pDisableObject->Init();
	}

	void	cTimeNumerialImage::Update(float e_fElpaseTime)
	{
		m_pDisableObject->Update(e_fElpaseTime);
	}

	void	cTimeNumerialImage::Render()
	{
		if( !m_bVisible )
			return;
		int	l_iSecond = this->m_i64Value%60;
		int	l_iMin = this->m_i64Value/60%60;
		int	l_iHour = this->m_i64Value/60/60%24;
		int	l_iIndex = 0;
		int	l_iNum = 0;
		if( m_bEnableSecond )
			l_iNum += AssignNumerialData(&l_iIndex ,this->m_pfTexCoordinate,m_vColor,this->m_iSingleImageWidth,this->m_iSingleImageHeight,m_vSecondPos,l_iSecond,2);
		if( m_bEnableMinute )
			l_iNum += AssignNumerialData(&l_iIndex ,this->m_pfTexCoordinate,m_vColor,this->m_iSingleImageWidth,this->m_iSingleImageHeight,this->m_vMinPos,l_iMin,2);
		if( m_bEnableHour )
			l_iNum += AssignNumerialData(&l_iIndex ,this->m_pfTexCoordinate,m_vColor,this->m_iSingleImageWidth,this->m_iSingleImageHeight,this->m_vHourPos,l_iHour,1);
		this->ApplyImage();
		UseShaderProgram(DEFAULT_SHADER);
		SetupShaderWorldMatrix(cMatrix44::Identity);
		myGlVertexPointer(3,g_fMPDIOptmizeRenderVertices);
		myGlUVPointer(2,g_fMPDIOptmizeRenderUV);
		myGlColorPointer(4,g_fMPDIOptmizeRenderColor);
		MY_GLDRAW_ARRAYS(GL_TRIANGLES, 0, l_iNum*6);
		if( m_pDisableObject && m_i64Value == 0 )
		{
			m_pDisableObject->Render();
		}
	}
}