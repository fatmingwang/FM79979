#include "stdafx.h"
#include "PuzzleImageUnitTriangulator.h"
#include "poly2tri\poly2tri.h"
using namespace p2t;
cPuzzleImageUnitTriangulator::cPuzzleImageUnitTriangulator(cPuzzleImageUnit*e_pTargetImage)
{
	m_TriangleVector.reserve(500);
	m_iFocusPoint = -1;
	m_pTargetImage = e_pTargetImage;
	auto l_pOffsetPos = e_pTargetImage->GetOffsetPos();
	//left up,right up,left down,right down
	Vector2 l_vPoints[4] = 
	{
		Vector2((float)l_pOffsetPos->x, (float)l_pOffsetPos->y),
		Vector2((float)l_pOffsetPos->x + e_pTargetImage->GetWidth(), (float)l_pOffsetPos->y),
		Vector2((float)l_pOffsetPos->x, (float)l_pOffsetPos->y+e_pTargetImage->GetHeight()),
		Vector2((float)l_pOffsetPos->x + e_pTargetImage->GetWidth(), (float)l_pOffsetPos->y + e_pTargetImage->GetHeight())
	};
	for(int i=0;i<4;++i)
		m_PointVector.push_back(l_vPoints[i]);
	GenerateTriangle();
}

//cPuzzleImageUnitTriangulator::cPuzzleImageUnitTriangulator(TiXmlElement * e_pTiXmlElement)
//{
//	m_TriangleVector.reserve(500);
//	m_iFocusPoint = -1;
//}

cPuzzleImageUnitTriangulator::~cPuzzleImageUnitTriangulator()
{
}

int	cPuzzleImageUnitTriangulator::GetClosestPoint(Vector2 e_vPos)
{
	size_t l_uiSize = m_PointVector.size();
	float l_fMinLength = 999999.f;
	int  l_iIndex = -1;
	for (int i = 0; i < (int)l_uiSize; ++i)
	{
		Vector2 l_vDisVector = m_PointVector[i]- e_vPos;
		float l_fLength = l_vDisVector.Length();
		if (l_fMinLength>l_fLength)
		{
			l_fMinLength = l_fLength;
			l_iIndex = i;
		}
	}
	if(l_fMinLength<=30)
		return l_iIndex;
	return -1;
}

void cPuzzleImageUnitTriangulator::MouseDown(int e_iPosX, int e_iPosY)
{
	m_iFocusPoint = -1;
}

void cPuzzleImageUnitTriangulator::MouseMove(int e_iPosX, int e_iPosY)
{
}

void cPuzzleImageUnitTriangulator::MouseUp(int e_iPosX, int e_iPosY)
{
}

void cPuzzleImageUnitTriangulator::Render()
{
}

void cPuzzleImageUnitTriangulator::GenerateTriangle()
{
	m_TriangleVector.clear();
	//https://github.com/greenm01/poly2tri
	size_t l_uiSize = m_PointVector.size();
	vector<p2t::Point*>*l_pPolyline = new vector<p2t::Point*>;
	for (size_t i = 0; i < l_uiSize; ++i)
	{
		Vector2 l_vPos = m_PointVector[i];
		l_pPolyline->push_back(new p2t::Point(l_vPos.x, l_vPos.y));
	}
	CDT* cdt = new CDT(*l_pPolyline);
	cdt->Triangulate();
	std::vector<p2t::Triangle*>l_Triangles = cdt->GetTriangles();
	l_uiSize = l_Triangles.size();
	for (size_t i = 0; i < l_uiSize; ++i)
	{
		p2t::Triangle*l_pTriangle = l_Triangles[i];
		auto l_pP1 = l_pTriangle->GetPoint(0);
		auto l_pP2 = l_pTriangle->GetPoint(1);
		auto l_pP3 = l_pTriangle->GetPoint(2);
		m_TriangleVector.push_back(Vector2(l_pP1->x, l_pP1->y));
		m_TriangleVector.push_back(Vector2(l_pP2->x, l_pP2->y));
		m_TriangleVector.push_back(Vector2(l_pP3->x, l_pP3->y));
	}
	delete cdt;
	DELETE_POINTER_VECTOR(l_pPolyline, p2t::Point*);
}
