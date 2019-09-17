#include "stdafx.h"
#include "ChartWithName.h"
//http://cloford.com/resources/colours/namedcol.htm

Vector4 g_vLineIndexAndColor[30] = {
	Vector4(1.f/3,1.f/10,1.f/3,1.f),//0
	Vector4(1.f/27,1.f/3,1.f/18,1.f),
	Vector4(1.f/1,1.f/15,1.f/17,1.f),
	Vector4(1.f/2,1.f/7,1.f/3,1.f),
	Vector4(1.f/1.5f,1.f/1.5f,1.f/5.f,1.f),//4
	Vector4(1.f/15,1.f/2,1.f/14,1.f),//
	Vector4(1.f/2,1.f/4,1.f/3,1.f),
	Vector4(1.f/9,1.f/6,1.f/2,1.f),
	Vector4(1.f/6,1.f/8,1.f/2,1.f),
	Vector4(1.f/3,1.f/10,1.f/10,1.f),//9
	Vector4(1.f/1,1.f/12,1.f/19,1.f),
	Vector4(1.f/2,1.f/14,1.f/18,1.f),
	Vector4(1.f/4,1.f/16,1.f/17,1.f),
	Vector4(1.f/6,1.f/8,1.f/6,1.f),
	Vector4(1.f/8,1.f/10,1.f/5,1.f),//14
	Vector4(1.f/1,1.f/2,1.f/4,1.f),
	Vector4(1.f/2,1.f/4,1.f/3,1.f),
	Vector4(1.f/4,1.f/6,1.f/2,1.f),
	Vector4(1.f/6,1.f/8,1.f/1,1.f),
	Vector4(1.f/8,1.f/10,1.f/10,1.f),//19
	Vector4(1.f/20,1.f/30,1.f/9,1.f),
	Vector4(1.f/22,1.f/30,1.f/8,1.f),
	Vector4(1.f/24,1.f/30,1.f/7,1.f),
	Vector4(1.f/26,1.f/30,1.f/6,1.f),//24
	Vector4(1.f/28,1.f/30,1.f/5,1.f),
	Vector4(1.f/1,1.f/30,1.f/4,1.f),
	Vector4(1.f/10,1.f/30,1.f/3,1.f),
	Vector4(1.f/20,1.f/30,1.f/2,1.f),
	Vector4(1.f/30,1.f/30,1.f/1,1.f),//29

};


cChartWithName::cChartWithName()
{
	m_ObjectIndexDistance = 60;
	m_fHeightScale = -3;
	m_bShowValue = false;
	m_vMaxValue.x = 0;
	m_vMaxValue.y = 0;
}
cChartWithName::~cChartWithName()
{
	DELETE_VECTOR(m_LineDataVector);
}

void	cChartWithName::AddData(sNameAndData* e_pNameAndData)
{
	this->m_LineDataVector.push_back(e_pNameAndData);
}

void	cChartWithName::Render()
{
	const	int	l_iIndexWidthOffset = 10;
	const	int	l_iIndexPosY = 30;
	const	int	l_iParemeterNAmePosX = -300;
	const	int	l_iParemeterNAmeOffsetY = 100;

	size_t l_uiSize = this->m_RenderIndex.size();
	if( l_uiSize == 0 )
	{
		l_uiSize = m_LineDataVector.size();
		for(size_t i=0;i<l_uiSize;++i)
		{
			this->m_LineDataVector[i]->m_pLineData->DebugRender(false,false,g_vLineIndexAndColor[i]);
			cGameApp::m_spGlyphFontRender->SetFontColor(g_vLineIndexAndColor[i]);
			cGameApp::RenderFont(l_iParemeterNAmePosX,i*l_iParemeterNAmeOffsetY,this->m_LineDataVector[i]->m_Name.c_str());
			if( m_bShowValue )
			{

			}
		}
	}
	else
	{
		for(size_t i=0;i<l_uiSize;++i)
		{
			int	l_iIndex = m_RenderIndex[i];
			this->m_LineDataVector[l_iIndex]->m_pLineData->DebugRender(false,false,g_vLineIndexAndColor[l_iIndex]);
			cGameApp::m_spGlyphFontRender->SetFontColor(g_vLineIndexAndColor[l_iIndex]);
			cGameApp::RenderFont(l_iParemeterNAmePosX,i*l_iParemeterNAmeOffsetY,this->m_LineDataVector[l_iIndex]->m_Name.c_str());
		}	
	}
	l_uiSize = m_RenderIndex.size();
	if( l_uiSize == 0 )
		l_uiSize = m_LineDataVector.size();
	for(size_t i=0;i<l_uiSize;++i)
	{
		int	l_iIndex = i;
		if( m_RenderIndex.size() )
			l_iIndex = m_RenderIndex[i];
		auto l_pData = this->m_LineDataVector[l_iIndex]->m_pLineData->GetLinerDataVector();
		for(size_t j=0;j<l_pData->size();++j)
		{
			Vector3 l_vPos = (*l_pData)[j];
			if( i == 0 )
				cGameApp::RenderFont(l_vPos.x-l_iIndexWidthOffset,(float)l_iIndexPosY,ValueToStringW(j));
			if( m_bShowValue )
				cGameApp::RenderFont(l_vPos.x,l_vPos.y,ValueToStringW(l_vPos.y/this->m_fHeightScale));
		}
	}
	int	l_TotalHeight = 200;
	int	l_DividValue = 50;
	int	l_iStep = l_TotalHeight/l_DividValue;
	for(int i=0;i<l_iStep;++i)
	{
		cGameApp::RenderFont(-30.f,i*l_DividValue*m_fHeightScale,ValueToStringW(l_DividValue*i));
	}
	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::One);
}

Vector2	cChartWithName::GetMaxValue()
{
	if(m_vMaxValue.x == -79979 &&
			m_vMaxValue.y == -79979 )
	{
		for(size_t i=0;i<this->m_LineDataVector.size();++i)
		{
			if(m_LineDataVector[i]->m_pLineData)
			{
				Vector3 l_vValue;
				if(m_LineDataVector[i]->m_pLineData->GetLastData(l_vValue))
				{
					if(fabs(this->m_vMaxValue.x) < fabs(l_vValue.x))
						this->m_vMaxValue.x = l_vValue.x;
					if(fabs(this->m_vMaxValue.y) < fabs(l_vValue.y))
						this->m_vMaxValue.y = l_vValue.y;
				}
			}
		}
	}
	return this->m_vMaxValue;
}

Vector2	cChartWithName::GetMaxValue(int e_iIndex)
{
	if( m_LineDataVector.size() == 0 )
		return Vector2::Zero;
	sNameAndData*l_pNameAndData = m_LineDataVector[0];
	if(l_pNameAndData->m_pLineData)
	{
		Vector3 l_vValue;
		if(l_pNameAndData->m_pLineData->GetLastData(l_vValue))
		{
			l_pNameAndData->m_vMaxValue.x = l_vValue.x;
			l_pNameAndData->m_vMaxValue.y = l_vValue.y;
		}
	}
	return l_pNameAndData->m_vMaxValue;
}