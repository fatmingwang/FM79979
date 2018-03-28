#include "../../stdafx.h"
#include "DataCurve.h"
#include "../GameApp.h"
#include <limits.h>
#include <float.h>
namespace FATMING_CORE
{
	#define	CURVE_EXTRA_SCALE	1.5
	cDataAndCurve::sDataAndCount::sDataAndCount(int e_iCount)
	{
		iOriginalCount = e_iCount;
		pdbOriginalfData = 0;
		if( e_iCount>0 )
		{
			pdbOriginalfData = new double[e_iCount];
			memset(pdbOriginalfData,0,sizeof(double)*e_iCount);
		}
	}

	cDataAndCurve::sDataAndCount*	cDataAndCurve::GetData()
	{
		return m_pOriginalData;
	}
	//
	cDataAndCurve::cDataAndCurve(int e_iCount)
	{
		m_dbFirstPointValue = 0;
		m_dbMaxValueForRealData = 0;
		m_dbMinValueForRealData = 0;
		m_pRenderLineData = 0;
		m_pCompressData = 0;
		m_pOriginalData = 0;
		m_dbMaxValue = m_dbMinValue = 0.f;
		m_dbAeverageValue = 0.f;
		m_dbTotalValue = 0;
		CreateBuffer(e_iCount);
	}
	cDataAndCurve::cDataAndCurve(double*e_pData,int e_iCount)
	{
		m_dbTotalValue = 0;
		m_dbFirstPointValue = 0;
		m_fMaxMinScale = 1.f;
		m_pRenderLineData = 0;
		m_pCompressData = 0;
		m_pOriginalData = 0;
		m_dbMaxValue = m_dbMinValue = 0.f;
		m_dbAeverageValue = 0.f;
		SetData(e_pData,e_iCount);
	}
	//
	cDataAndCurve::~cDataAndCurve()
	{
		SAFE_DELETE(m_pOriginalData);
		SAFE_DELETE(m_pCompressData);
	}
	//
	void	cDataAndCurve::CalMinMaxValue()
	{
		AssignMinMaxValue(this->m_pOriginalData);
	}

	void	cDataAndCurve::CreateBuffer(int e_iCount)
	{
		SAFE_DELETE(m_pOriginalData);
		SAFE_DELETE(m_pCompressData);
		m_pOriginalData = new sDataAndCount(e_iCount);
		memset(m_pOriginalData->pdbOriginalfData,0,sizeof(double)*e_iCount);
		SAFE_DELETE(m_pRenderLineData);
		m_pRenderLineData = new Vector2[e_iCount*2-1];
	}
	//
	void	cDataAndCurve::AssignData(int e_iIndex,double e_fData)
	{
		assert(m_pOriginalData->iOriginalCount>e_iIndex);
		m_pOriginalData->pdbOriginalfData[e_iIndex] = e_fData;
	}
	//
	void	cDataAndCurve::SetData(double*e_pData,int e_iCount)
	{
		SAFE_DELETE(m_pOriginalData);
		SAFE_DELETE(m_pCompressData);
		m_pOriginalData = new sDataAndCount(e_iCount);
		memcpy(m_pOriginalData->pdbOriginalfData,e_pData,sizeof(double)*e_iCount);
		AssignMinMaxValue(m_pOriginalData);
		SAFE_DELETE(m_pRenderLineData);
		m_pRenderLineData = new Vector2[e_iCount*2-1];
	}
	//
	void	cDataAndCurve::AssignMinMaxValue(sDataAndCount*e_pData)
	{
		m_dbFirstPointValue = m_pOriginalData->pdbOriginalfData[0];
		m_dbMaxValue = DBL_MIN;
		m_dbMinValue = DBL_MAX;
		for( int i=0;i<e_pData->iOriginalCount;++i )
		{
		//	if( m_dbFirstPointValue == 0 )
		//		m_dbFirstPointValue = e_pData->pdbOriginalfData[i];
			double	l_fValue = e_pData->pdbOriginalfData[i];
			m_dbTotalValue += l_fValue;
			if( m_dbMaxValue < l_fValue )
				m_dbMaxValue = l_fValue;
			if( m_dbMinValue > l_fValue )
				m_dbMinValue = l_fValue;
		}
		m_dbMaxValueForRealData = m_dbMaxValue;
		m_dbMinValueForRealData = m_dbMinValue;
		m_dbMaxValue = fabs(m_dbMaxValue);
		m_dbMinValue = fabs(m_dbMinValue);
		double	l_dbMaxValue = m_dbMaxValue>m_dbMinValue?m_dbMaxValue:m_dbMinValue;
		m_dbMaxValue = l_dbMaxValue*CURVE_EXTRA_SCALE;
		m_dbMinValue = -l_dbMaxValue*CURVE_EXTRA_SCALE;
		m_dbAeverageValue = ((m_dbMaxValue-m_dbMinValue)/2)+m_dbMinValue;
	}
	//
	void	cDataAndCurve::Compress(double e_fPercentage)
	{
		if( !m_pOriginalData )
			return;
		int	l_iCount = (int)(m_pOriginalData->iOriginalCount*e_fPercentage);
		int	l_iDivide = m_pOriginalData->iOriginalCount/l_iCount;
		SAFE_DELETE(m_pCompressData);
		m_pCompressData = new sDataAndCount(l_iCount);
		SAFE_DELETE(m_pRenderLineData);
		m_pRenderLineData = new Vector2[l_iCount*2-1];
		int	l_iCurrentDivide = 0;
		double	l_fValue = 0.f;
		int	l_iDivideIndex = 0;
		for( int i=0;i<m_pOriginalData->iOriginalCount;++i )
		{
			++l_iCurrentDivide;
			l_fValue += m_pOriginalData->pdbOriginalfData[i];
			if( l_iCurrentDivide == l_iDivide )
			{
				l_iCurrentDivide = 0;
				l_fValue = l_fValue/l_iDivide;
				m_pCompressData->pdbOriginalfData[l_iDivideIndex] = l_fValue;
				++l_iDivideIndex;
				l_fValue = 0.f;
			}
			//m_pCompressData->iOriginalCount = l_iValue;
		}
		if( l_iDivideIndex*l_iDivide != m_pOriginalData->iOriginalCount )
		{//filled last
			int	l_iRestCount = m_pOriginalData->iOriginalCount-(l_iDivideIndex*l_iDivide);
			l_fValue /= l_iRestCount;
			m_pCompressData->pdbOriginalfData[m_pCompressData->iOriginalCount-1] = l_fValue;
		}
	}
	//
	void	cDataAndCurve::MouseMove(float e_fPosX,float e_fPosY)
	{
		m_vRealMousePoint.x = e_fPosX;
		m_vRealMousePoint.y = e_fPosY;
		m_strCurrentPointData = L"";
		sDataAndCount*l_pSrc = m_pCompressData;
		if( !l_pSrc )
			l_pSrc = m_pOriginalData;
		if( !l_pSrc )
			return;

		double	l_dbMaxValue = m_dbMaxValue*m_fMaxMinScale;
		double	l_dbMinValue = m_dbMinValue*m_fMaxMinScale;
		double	l_dbAeverage = m_dbAeverageValue*m_fMaxMinScale;
		double	l_fPointWidthDistance = m_vRenderRect.Width()/l_pSrc->iOriginalCount;
		double	l_fPointHeightDistance = m_vRenderRect.Height()/(l_dbMaxValue-l_dbMinValue);
		//from bottom to top
		double	l_fStartPosX = m_vRenderRect.x;
		double	l_fStartPosY = m_vRenderRect.y+m_vRenderRect.Height()/2.f;
		double	l_fEndPosY = m_vRenderRect.y+m_vRenderRect.Height();
		float	l_fYValue = (e_fPosY-m_vRenderRect.y)/m_vRenderRect.Height();
		l_fYValue = (float)(l_fYValue*(l_dbMaxValue-l_dbMinValue)+l_dbMinValue);
		l_fYValue *= -1;
		int	l_iLineCount = l_pSrc->iOriginalCount-1;
		double	l_dbStartValue = 0;
		for( int i=0;i<l_iLineCount;++i )
		{
			double	l_fValue1 = l_pSrc->pdbOriginalfData[i]-l_dbAeverage;
			double	l_fValue2 = l_pSrc->pdbOriginalfData[i+1]-l_dbAeverage;
			double	l_fPosX = (i*l_fPointWidthDistance)+l_fStartPosX;
			double	l_fPosY = l_fStartPosY-(l_fValue1*l_fPointHeightDistance);
			Vector2	l_vLine1Pos((float)l_fPosX,(float)l_fPosY);
			l_fPosX = ((i+1)*l_fPointWidthDistance)+l_fStartPosX;
			l_fPosY = l_fStartPosY-(l_fValue2*l_fPointHeightDistance);
			Vector2	l_vLine2Pos((float)l_fPosX,(float)l_fPosY);
			if(l_vLine1Pos.x<=e_fPosX&&l_vLine2Pos.x>=e_fPosX )
			{
				m_strCurrentPointData = L"Pos:";
				m_strCurrentPointData += ValueToStringW(i);;
				m_vMouseHitPoint.x = (float)l_fPosX;
				m_vMouseHitPoint.y = (float)l_fPosY;
				m_strCurrentPointData += L",Value";
				m_strCurrentPointData += ValueToStringW(l_fValue2);
				m_strCurrentPointData += L"\nY:";
				m_strCurrentPointData += ValueToStringW(l_fYValue);
				return ;
			}
		}	
	}
	//
	void	cDataAndCurve::Render(float e_fMaxMinScale,Vector4 e_vColor,Vector4 e_vRenderRect,bool e_bDrawRect)
	{
		m_fMaxMinScale = e_fMaxMinScale;
		m_vRenderRect = e_vRenderRect;
		sDataAndCount*l_pSrc = m_pCompressData;
		if( !l_pSrc )
			l_pSrc = m_pOriginalData;
		if( !l_pSrc )
			return;
		double	l_dbMaxValue = m_dbMaxValue*e_fMaxMinScale;
		double	l_dbMinValue = m_dbMinValue*e_fMaxMinScale;
		double	l_dbAeverage = m_dbAeverageValue*e_fMaxMinScale;
		double	l_fPointWidthDistance = e_vRenderRect.Width()/l_pSrc->iOriginalCount;
		double	l_fPointHeightDistance = e_vRenderRect.Height()/(l_dbMaxValue-l_dbMinValue);
		if( e_bDrawRect )
		{
			GLRender::RenderRectangle(Vector2(e_vRenderRect.x,e_vRenderRect.y),e_vRenderRect.Width(),e_vRenderRect.Height(),Vector4::One,0);
		}
		//from bottom to top
		double	l_fStartPosX = e_vRenderRect.x;
		double	l_fStartPosY = e_vRenderRect.y+e_vRenderRect.Height()/2.f;
		int	l_iLineCount = l_pSrc->iOriginalCount-1;
		double	l_dbStartValue = 0;
		for( int i=0;i<l_iLineCount;++i )
		{
			double	l_fValue1 = l_pSrc->pdbOriginalfData[i]-l_dbAeverage;
			double	l_fValue2 = l_pSrc->pdbOriginalfData[i+1]-l_dbAeverage;
			double	l_fPosX = (i*l_fPointWidthDistance)+l_fStartPosX;
			double	l_fPosY = l_fStartPosY-(l_fValue1*l_fPointHeightDistance);
			if( i == 0)
			{
				//l_dbStartValue = l_fValue1;
				//GLRender::RenderRectangle(Vector2(l_fPosX,l_fPosY),e_vRenderRect.Width(),1,Vector4::One,0,1);
			}
			m_pRenderLineData[i*2].x = (float)l_fPosX;
			m_pRenderLineData[i*2].y = (float)l_fPosY;
			l_fPosX = ((i+1)*l_fPointWidthDistance)+l_fStartPosX;
			l_fPosY = l_fStartPosY-(l_fValue2*l_fPointHeightDistance);
			m_pRenderLineData[i*2+1].x = (float)l_fPosX;
			m_pRenderLineData[i*2+1].y = (float)l_fPosY;
		}
		GLRender::RenderRectangle(Vector2((float)(l_fStartPosX),(float)(l_fStartPosY-(m_dbAeverageValue*l_fPointHeightDistance))),e_vRenderRect.Width(),1,Vector4::One,0,1);
		GLRender::RenderLine((float*)m_pRenderLineData,l_iLineCount*2,e_vColor,2);
		//
		if( e_bDrawRect )
		{
			std::wstring	l_str = UT::ComposeMsgByFormat(L"%.2f",this->m_dbMaxValue);
			cGameApp::m_spGlyphFontRender->RenderFont(e_vRenderRect.x,e_vRenderRect.y,l_str.c_str());

			//l_str = UT::ComposeMsgByFormat(L"Average%.2f",this->m_dbAeverageValue);
			//cGameApp::m_spGlyphFontRender->RenderFont(e_vRenderRect.x,e_vRenderRect.y+e_vRenderRect.Height()/2,l_str.c_str());

			l_str = UT::ComposeMsgByFormat(L"Result:%.2f",this->m_dbTotalValue);
			cGameApp::m_spGlyphFontRender->RenderFont(e_vRenderRect.x,60.f,l_str.c_str());

			l_str = UT::ComposeMsgByFormat(L"Start:%.2f",this->m_dbFirstPointValue);
			cGameApp::m_spGlyphFontRender->RenderFont(e_vRenderRect.x,e_vRenderRect.y+e_vRenderRect.Height()-40,l_str.c_str());

			l_str = UT::ComposeMsgByFormat(L"Count:%d",l_pSrc->iOriginalCount);
			cGameApp::m_spGlyphFontRender->RenderFont(e_vRenderRect.x+e_vRenderRect.Width()-340,e_vRenderRect.y+e_vRenderRect.Height()-40,l_str.c_str());

			l_str = UT::ComposeMsgByFormat(L"Max%.2f,Min%.2f",this->m_dbMaxValueForRealData,this->m_dbMinValueForRealData);
			cGameApp::m_spGlyphFontRender->RenderFont(e_vRenderRect.x+e_vRenderRect.Width()/2-300,e_vRenderRect.y+e_vRenderRect.Height()-40,l_str.c_str());

			l_str = UT::ComposeMsgByFormat(L"%.2f",l_dbStartValue);
			cGameApp::m_spGlyphFontRender->RenderFont(e_vRenderRect.x+e_vRenderRect.Width()-340,e_vRenderRect.y+e_vRenderRect.Height()/2,l_str.c_str());

			if( m_strCurrentPointData.c_str() )
			{
				cGameApp::m_spGlyphFontRender->RenderFont(100,100,m_strCurrentPointData.c_str());
				Vector4	l_vColor(1,1,1,1);
				static	float	l_fScale = 1.f;
				l_fScale += 0.016f;
				if(l_fScale>1.0f)
					l_fScale = 0.f;
				l_vColor *= l_fScale;
				l_vColor.x = 1.f;
				if( l_vColor.w <= 0.5f  )
					l_vColor.w = 0.5f;
				RenderPoint(m_vMouseHitPoint,15,l_vColor);
				RenderPoint(m_vRealMousePoint,3,l_vColor);
			}
		}
	}

	void	cDataAndCurve::Render(float e_fMaxMinScale,int e_iStartCount,int e_iEndCount,Vector4 e_vColor,Vector4 e_vRenderRect,bool e_bDrawRect)
	{
		m_fMaxMinScale = e_fMaxMinScale;
		m_vRenderRect = e_vRenderRect;
		sDataAndCount*l_pSrc = m_pCompressData;
		if( !l_pSrc )
			l_pSrc = m_pOriginalData;
		if( !l_pSrc )
			return;
		//GLRender::RenderRectangle();
	}

	cDataAndCurveListWithXMlSechma::cDataAndCurveListWithXMlSechma(const char*e_strXMLSechemaName,const char*e_strBinaryFile)
	{
		m_dbMaxValue = 0;
		m_dbMinValue = 0;
		m_pFile = 0;
		m_dbAeverageValue = 0;
		m_Header.DataType = eDT_MAX;
		cNodeISAX	l_Node;
		if(l_Node.ParseDataIntoXMLNode(e_strXMLSechemaName))
		{
			m_pFile = new cBinaryFile();
			if(!m_pFile->Openfile(e_strBinaryFile))
			{
				UT::ErrorMsg("open file failed",e_strBinaryFile);
				return;
			}
			ProcessRootData(l_Node.GetRootElement());
		}
	}

	cDataAndCurveListWithXMlSechma::~cDataAndCurveListWithXMlSechma()
	{
		SAFE_DELETE(m_pFile);
	}

	cDataAndCurveListWithXMlSechma::sData		cDataAndCurveListWithXMlSechma::ProcessData(TiXmlElement*e_pTiXmlElement)
	{
		sData	l_Data;
		l_Data.DataType = eDT_MAX;
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("Name")
			{
				l_Data.strName = l_strValue;
			}
			else
			COMPARE_NAME("Size")
			{
				//l_Data.iSize = VALUE_TO_INT;
			}
			else
			COMPARE_NAME("Type")
			{
				l_Data.DataType = GetDataType(l_strValue);
			}
		PARSE_NAME_VALUE_END
		return l_Data;
	}

	void		cDataAndCurveListWithXMlSechma::ProcessRootData(TiXmlElement*e_pTiXmlElement)
	{
		if( !e_pTiXmlElement )
			return;
		//m_Header.iSize = 0;
		m_Header.DataType = eDT_MAX;
		m_Header.strName = L"";
		e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		std::vector<sData>	l_DataVector;
		while( e_pTiXmlElement )
		{
			const wchar_t*l_strValue = e_pTiXmlElement->Value();
			COMPARE_VALUE("Header")
			{
				//assert(m_Header.iSize == 0);
				m_Header = ProcessData(e_pTiXmlElement);
			}
			else
			COMPARE_VALUE("Data")
			{
				l_DataVector.push_back(ProcessData(e_pTiXmlElement));
			}
			e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
		}
		//
		int	l_iTotalSize = 0;
		size_t	l_uiSize = l_DataVector.size();
		for(size_t i=0;i<l_uiSize;++i)
		{
			l_iTotalSize += g_iDataTypeSize[l_DataVector[i].DataType];
		}
		//
		int	l_iMySkipHeader = 0;
		if( m_Header.DataType != eDT_MAX )
			l_iMySkipHeader = g_iDataTypeSize[m_Header.DataType];
		int	l_iFileSize = m_pFile->GetDataFileSize();
		int	l_iOSSkipHeaderSize = (l_iFileSize-l_iMySkipHeader)%l_iTotalSize;
		int	l_iNumData = (l_iFileSize-l_iOSSkipHeaderSize)/l_iTotalSize;
		void*l_pData = m_pFile->GetDataFile(l_iOSSkipHeaderSize+l_iMySkipHeader);
		int	l_iCurrentSkipByte = 0;
		for( size_t i=0;i<l_uiSize;++i )
		{
			int	l_iDataSize = g_iDataTypeSize[l_DataVector[i].DataType];
			eDataType		l_eDataType = l_DataVector[i].DataType;

			char*l_pTargetData = (char*)l_pData;
			if( i != 0 )
				l_iCurrentSkipByte += g_iDataTypeSize[l_DataVector[i-1].DataType];
			l_pTargetData += l_iCurrentSkipByte;

			cDataAndCurve*l_pDataAndCurve = new cDataAndCurve(l_iNumData);
			l_pDataAndCurve->SetName(l_DataVector[i].strName.c_str());
			this->AddObjectNeglectExist(l_pDataAndCurve);
			for( int j=0;j<l_iNumData;++j )
			{
				double	l_dbFinalValue = 0;
				switch(l_eDataType)
				{
					case 	eDT_BYTE:
						l_dbFinalValue = (double)cBinaryFile::GetChar(l_pTargetData);
						break;
					case 	eDT_FLOAT:
						l_dbFinalValue = (double)cBinaryFile::GetFloat(l_pTargetData);
						break;
					case 	eDT_DOUBLE:
						l_dbFinalValue = (double)cBinaryFile::GetInt64(l_pTargetData);
						break;
					case 	eDT_INT:
						l_dbFinalValue = (double)cBinaryFile::GetInt(l_pTargetData);
						break;
					case 	eDT_INT64:
						l_dbFinalValue = (double)cBinaryFile::GetInt64(l_pTargetData);
						break;
					//case 	eDT_UINT64:
					//	l_dbFinalValue = cBinaryFile::GetInt64(l_pTargetData);
					default:
						UT::ErrorMsg(L"doesn't support this size",ValueToStringW(l_iDataSize));
						return;
						break;
				}
				//FMLog::LogWithFlag(ValueToStringW(l_dbFinalValue));
				//FMLog::LogWithFlag(L",");
				l_pDataAndCurve->AssignData(j,l_dbFinalValue);
				l_pTargetData += l_iTotalSize;
			}
			//FMLog::LogWithFlag(L"\n");
		}
	}

	void		cDataAndCurveListWithXMlSechma::Render(float e_fScale,int e_iIndex,Vector4 e_vRenderRect,Vector4 e_vColor,bool e_bDrawRect)
	{
		cDataAndCurve*l_pDataAndCurve = this->GetObject(e_iIndex);
		if( l_pDataAndCurve )
		{
			l_pDataAndCurve->Render(e_fScale,e_vColor,e_vRenderRect,e_bDrawRect);
		}
	}

	void		cDataAndCurveListWithXMlSechma::Render(wchar_t*e_strName,Vector4 e_vRenderRect)
	{
	}

	void		cDataAndCurveListWithXMlSechma::Render(std::vector<int>e_RenderObjectList,Vector4 e_vRenderRect)
	{
	}

	void		cDataAndCurveListWithXMlSechma::RenderAll(Vector4 e_vRenderRect)
	{

	}

	void		cDataAndCurveListWithXMlSechma::CalMinMaxValue()
	{
		std::vector<int>	l_iIndexVector;
		for( int i=0;i<this->Count();++i )
		{
			l_iIndexVector.push_back(i);
		}
		CalMinMaxValue(l_iIndexVector);
	}

	void		cDataAndCurveListWithXMlSechma::CalMinMaxValue(std::vector<int>e_SelectIndices)
	{
		this->m_dbMaxValue = DBL_MIN;
		this->m_dbMinValue = DBL_MAX;
		for( size_t i=0;i<e_SelectIndices.size();++i )
		{
			cDataAndCurve*l_pDataAndCurve = this->GetObject(e_SelectIndices[i]);
			l_pDataAndCurve->CalMinMaxValue();
			if( m_dbMaxValue < l_pDataAndCurve->m_dbMaxValue )
				m_dbMaxValue = l_pDataAndCurve->m_dbMaxValue;
			if( m_dbMinValue > l_pDataAndCurve->m_dbMinValue )
				m_dbMinValue = l_pDataAndCurve->m_dbMinValue;
		}
		m_dbAeverageValue = ((m_dbMaxValue-m_dbMinValue)/2)+m_dbMinValue;
		for( size_t i=0;i<e_SelectIndices.size();++i )	
		{
			cDataAndCurve*l_pDataAndCurve = this->GetObject(e_SelectIndices[i]);
			l_pDataAndCurve->m_dbMaxValue = m_dbMaxValue;
			l_pDataAndCurve->m_dbMinValue = m_dbMinValue;
			l_pDataAndCurve->m_dbAeverageValue = m_dbAeverageValue;
		}
	}
}