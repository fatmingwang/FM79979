#include "stdafx.h"
#include "ChartWithName.h"


cChartWithName::cChartWithName()
{
	m_ObjectIndexDistance = 60;
	m_iHeightScale = -3;
	m_bShowValue = false;
}
cChartWithName::~cChartWithName()
{
	DELETE_VECTOR(m_DataVector,cChartWithName::sNameAndData*);
}

void	cChartWithName::AddData(sNameAndData* e_pNameAndData)
{
	this->m_DataVector.push_back(e_pNameAndData);
}

void	cChartWithName::Render()
{
	const	int	l_iIndexWidthOffset = 10;
	const	int	l_iIndexPosY = 30;
	const	int	l_iParemeterNAmePosX = -300;
	const	int	l_iParemeterNAmeOffsetY = 100;
	Vector4 l_vColor[6] = {
		Vector4(1.0f		,1.0f	,1.0f	,1.f),
		Vector4(1.0f		,0.0f	,0.0f	,1.f),
		Vector4(0.0f		,1.0f	,0.0f	,1.f),
		Vector4(0.0f		,0.0f	,1.0f	,1.f),
		Vector4(1.0f		,1.0f	,0.0f	,1.f),
		Vector4(0.0f		,1.0f	,1.0f	,1.f),
	};
	size_t l_uiSize = this->m_RenderIndex.size();
	if( l_uiSize == 0 )
	{
		l_uiSize = m_DataVector.size();
		for(size_t i=0;i<l_uiSize;++i)
		{
			this->m_DataVector[i]->m_pData->DebugRender(false,false,l_vColor[i]);
			cGameApp::m_spGlyphFontRender->SetFontColor(l_vColor[i]);
			cGameApp::RenderFont(l_iParemeterNAmePosX,i*l_iParemeterNAmeOffsetY,this->m_DataVector[i]->m_Name.c_str());
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
			this->m_DataVector[l_iIndex]->m_pData->DebugRender(false,false,l_vColor[l_iIndex]);
			cGameApp::m_spGlyphFontRender->SetFontColor(l_vColor[l_iIndex]);
			cGameApp::RenderFont(l_iParemeterNAmePosX,i*l_iParemeterNAmeOffsetY,this->m_DataVector[l_iIndex]->m_Name.c_str());
		}	
	}
	l_uiSize = m_RenderIndex.size();
	if( l_uiSize == 0 )
		l_uiSize = m_DataVector.size();
	for(size_t i=0;i<l_uiSize;++i)
	{
		int	l_iIndex = i;
		if( m_RenderIndex.size() )
			l_iIndex = m_RenderIndex[i];
		auto l_pData = this->m_DataVector[l_iIndex]->m_pData->GetLinerDataVector();
		for(size_t j=0;j<l_pData->size();++j)
		{
			Vector3 l_vPos = (*l_pData)[j];
			if( i == 0 )
				cGameApp::RenderFont(l_vPos.x-l_iIndexWidthOffset,(float)l_iIndexPosY,ValueToStringW(j));
			if( m_bShowValue )
				cGameApp::RenderFont(l_vPos.x,l_vPos.y,ValueToStringW(l_vPos.y/this->m_iHeightScale));
		}
	}
	int	l_TotalHeight = 200;
	int	l_DividValue = 50;
	int	l_iStep = l_TotalHeight/l_DividValue;
	for(int i=0;i<l_iStep;++i)
	{
		cGameApp::RenderFont(-30,i*l_DividValue*m_iHeightScale,ValueToStringW(l_DividValue*i));
	}
	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4::One);
}