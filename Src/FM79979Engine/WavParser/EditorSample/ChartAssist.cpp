#include "stdafx.h"
#include "ChartAssist.h"

//cChartAssist::cChartAssist()
//{
//	m_pData = nullptr;
//}
//
//cChartAssist::~cChartAssist()
//{
//	
//}
//
//void	cChartAssist::SetData(int e_iXAxisValue,int e_iYAxisValue,std::vector<Vector2>*e_pData)
//{
//	m_XAxis.push_back(Vector2(0,0));
//	m_XAxis.push_back(Vector2(e_iXAxisValue,0));
//	m_YAxis.push_back(Vector2(0,0));
//	m_YAxis.push_back(Vector2(0,e_iYAxisValue));
//	m_pData = e_pData;
//}
//
//void	cChartAssist::Render()
//{
//	GLRender::RenderLine(&m_XAxis,Vector4::One);	
//	GLRender::RenderLine(&m_YAxis,Vector4::One);
//	GLRender::RenderLine(&m_pData,Vector4::One);
//}