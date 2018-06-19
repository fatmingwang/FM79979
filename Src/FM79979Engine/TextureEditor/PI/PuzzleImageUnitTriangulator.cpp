#include "stdafx.h"
#include "PuzzleImageUnitTriangulator.h"
#include "poly2tri\poly2tri.h"
#include "EarClipping.h"
using namespace p2t;
cPuzzleImageUnitTriangulator::cPuzzleImageUnitTriangulator(cUIImage*e_pTargetImage)
{
	m_TriangleVector.reserve(500);
	m_iFocusPoint = -1;
	m_pTargetImage = e_pTargetImage;
	if (m_pTargetImage)
		this->SetName(m_pTargetImage->GetName());
	auto l_pOffsetPos = e_pTargetImage->GetOffsetPos();
	//left up,right up,right down,left down
	//must be this order or get wrong
	Vector2 l_vPoints[4] = 
	//Vector2 l_vPoints[5] =
	{
		Vector2(e_pTargetImage->GetOffsetPos()->x,e_pTargetImage->GetOffsetPos()->y),
		Vector2((float)e_pTargetImage->GetRightDownStripOffPos().x,(float)e_pTargetImage->GetOffsetPos()->y),
		Vector2((float)e_pTargetImage->GetRightDownStripOffPos().x,(float)e_pTargetImage->GetRightDownStripOffPos().y),
		Vector2((float)e_pTargetImage->GetOffsetPos()->x,(float)e_pTargetImage->GetRightDownStripOffPos().y),
		//Vector2(e_pTargetImage->GetWidth()/2,e_pTargetImage->GetHeight()/2),
	};
	for (int i = 0; i<4; ++i)
	//for(int i=0;i<5;++i)
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
	m_MouseMoveData.MouseDown(e_iPosX,e_iPosY);
	m_iFocusPoint = -1;
	switch (m_ePointsToTriangulatorType)
	{
		case ePTPT_ADD:
			PointsToTriangulatorAddMouseDown(e_iPosX, e_iPosY, eMB_DOWN);
			break;
		case ePTPT_DELETE:
			PointsToTriangulatorDeleteMouseDown(e_iPosX, e_iPosY, eMB_DOWN);
			break;
		case ePTPT_MOVE:
			PointsToTriangulatorMoveMouseDown(e_iPosX, e_iPosY, eMB_DOWN);
			break;
	}
}

void cPuzzleImageUnitTriangulator::MouseMove(int e_iPosX, int e_iPosY)
{
	m_MouseMoveData.MouseMove(e_iPosX, e_iPosY);
	switch (m_ePointsToTriangulatorType)
	{
	case ePTPT_ADD:
		PointsToTriangulatorAddMouseDown(e_iPosX, e_iPosY, eMB_MOVE);
		break;
	case ePTPT_DELETE:
		PointsToTriangulatorDeleteMouseDown(e_iPosX, e_iPosY, eMB_MOVE);
		break;
	case ePTPT_MOVE:
		PointsToTriangulatorMoveMouseDown(e_iPosX, e_iPosY, eMB_MOVE);
		break;
	}
}

void cPuzzleImageUnitTriangulator::MouseUp(int e_iPosX, int e_iPosY)
{
	m_MouseMoveData.MouseUp(e_iPosX, e_iPosY);
	switch (m_ePointsToTriangulatorType)
	{
	case ePTPT_ADD:
		PointsToTriangulatorAddMouseDown(e_iPosX, e_iPosY, eMB_UP);
		break;
	case ePTPT_DELETE:
		PointsToTriangulatorDeleteMouseDown(e_iPosX, e_iPosY, eMB_UP);
		break;
	case ePTPT_MOVE:
		PointsToTriangulatorMoveMouseDown(e_iPosX, e_iPosY, eMB_UP);
		break;
	}
	if (m_iFocusPoint != -1)
	{
		GenerateTriangle();
	}
}

void cPuzzleImageUnitTriangulator::Render()
{
	if (!m_pTargetImage)
		return;
	Vector3 l_vPos = m_pTargetImage->GetLocalPosition();
	m_pTargetImage->SetLocalPosition(Vector3::Zero);
	this->m_pTargetImage->Render();
	m_pTargetImage->SetLocalPosition(l_vPos);
	if (m_TriangleVector.size() > 1)
	{
		int l_iCount = (int)m_TriangleVector.size()/3;
		for (int i = 0; i < l_iCount; i++)
		{
			Vector4 l_vColor = Vector4::Zero;
			l_vColor.a = 1.f;
			if (i<10)
				l_vColor.r += (i + 1);
			if (i<20)
				l_vColor.g += (i + 1);
			if (i<30)
				l_vColor.b += (i + 1);
			std::vector<Vector2>l_vPos;
			l_vPos.push_back(m_TriangleVector[i*3]);
			l_vPos.push_back(m_TriangleVector[i * 3+1]);
			l_vPos.push_back(m_TriangleVector[i * 3+2]);
			l_vPos.push_back(m_TriangleVector[i * 3]);
			GLRender::RenderLine(&l_vPos, l_vColor);
		}
	}
}

void cPuzzleImageUnitTriangulator::GenerateTriangle()
{
	m_TriangleVector.clear();
	//https://github.com/greenm01/poly2tri
	size_t l_uiSize = m_PointVector.size();
	if (l_uiSize < 4)
		return;
	vector<p2t::Point*> l_Polyline;
	//l_Polyline.push_back(new p2t::Point(0,0));
	//l_Polyline.push_back(new p2t::Point(m_pTargetImage->GetWidth(),0));
	//l_Polyline.push_back(new p2t::Point(m_pTargetImage->GetWidth(), m_pTargetImage->GetHeight()));
	//l_Polyline.push_back(new p2t::Point(0, m_pTargetImage->GetHeight()));
	const int l_iBoundingBoxPointsCount = 4;
	size_t l_uiSizeStripBoundingBox = l_uiSize - l_iBoundingBoxPointsCount;
	for (size_t i = 0; i < l_iBoundingBoxPointsCount; ++i)
	{
		Vector2 l_vPos = m_PointVector[i];
		l_Polyline.push_back(new p2t::Point(l_vPos.x, l_vPos.y));
	}
	CDT* cdt = new CDT(l_Polyline);
	//std::vector<p2t::Point*>		l_PointHoleVector;
	for (size_t i = l_iBoundingBoxPointsCount; i < l_uiSizeStripBoundingBox; ++i)
	{
		Vector2 l_vPos = m_PointVector[i];
		//l_PointHoleVector.push_back(new p2t::Point(l_vPos.x, l_vPos.y));
		cdt->AddPoint(new p2t::Point(l_vPos.x, l_vPos.y));
	}
	//cdt->AddHole(l_PointHoleVector);
	cdt->Triangulate();
	auto l_pMap = cdt->GetMap();
	std::vector<size_t>l_Reult =  GetIndicesFromPolygon(m_PointVector);
	//from indices
	if (1)
	{
		int l_iCount = (int)l_Reult.size()/3;
		for (int i = 0; i < l_iCount; i++)
		{
			int l_iIndex1 = (int)l_Reult[i * 3];
			int l_iIndex2 = (int)l_Reult[i * 3+1];
			int l_iIndex3 = (int)l_Reult[i * 3+2];
			m_TriangleVector.push_back(m_PointVector[l_iIndex1]);
			m_TriangleVector.push_back(m_PointVector[l_iIndex2]);
			m_TriangleVector.push_back(m_PointVector[l_iIndex3]);
		}
	}//from triangulator
	else
	{
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
	}
	delete cdt;
}

void cPuzzleImageUnitTriangulator::PointsToTriangulatorAddMouseDown(int e_iPosX, int e_iPosY, eMouseBehavior e_eMouseBehavior)
{
	switch (e_eMouseBehavior)
	{
	case eMB_DOWN:
		
		break;
	case eMB_MOVE:
		break;
	case eMB_UP:
		if (m_MouseMoveData.DownUpDistance() <= 5.f)
		{
			m_PointVector.push_back(Vector2(e_iPosX, e_iPosY));
			GenerateTriangle();
		}
		break;
	}
}

void cPuzzleImageUnitTriangulator::PointsToTriangulatorDeleteMouseDown(int e_iPosX, int e_iPosY, eMouseBehavior e_eMouseBehavior)
{
	switch (e_eMouseBehavior)
	{
	case eMB_DOWN:
		m_iFocusPoint = GetClosestPoint(Vector2(e_iPosX, e_iPosY));
		break;
	case eMB_MOVE:
		break;
	case eMB_UP:
		if (m_iFocusPoint != -1 &&m_iFocusPoint == GetClosestPoint(Vector2(e_iPosX, e_iPosY)))
		{//same point
			m_PointVector.erase(m_PointVector.begin()+m_iFocusPoint);
		}
		break;
	}
}

void cPuzzleImageUnitTriangulator::PointsToTriangulatorMoveMouseDown(int e_iPosX, int e_iPosY, eMouseBehavior e_eMouseBehavior)
{
	switch (e_eMouseBehavior)
	{
	case eMB_DOWN:
		m_iFocusPoint = GetClosestPoint(Vector2(e_iPosX, e_iPosY));
		break;
	case eMB_MOVE:
		if (m_iFocusPoint != -1)
		{
			//this->m_MouseMoveData.DownMove();
			m_PointVector[m_iFocusPoint] = Vector2(e_iPosX,e_iPosY);
		}
		break;
	case eMB_UP:
		m_iFocusPoint = -1;
		GenerateTriangle();
		break;
	}
}

cPuzzleImageUnitTriangulatorManager::cPuzzleImageUnitTriangulatorManager()
{
}

cPuzzleImageUnitTriangulatorManager::~cPuzzleImageUnitTriangulatorManager()
{
}

cPuzzleImageUnitTriangulator * cPuzzleImageUnitTriangulatorManager::GetObject(cUIImage*e_pUIImage)
{
	int l_iCount = this->Count();
	for (int i = 0; i < l_iCount; i++)
	{
		auto l_pObject = GetObject(i);
		if (l_pObject->GetTargetImage() == e_pUIImage)
		{
			return l_pObject;
		}
	}
	cPuzzleImageUnitTriangulator*l_pPuzzleImageUnitTriangulator = new cPuzzleImageUnitTriangulator(e_pUIImage);
	this->AddObjectNeglectExist(l_pPuzzleImageUnitTriangulator);
	return l_pPuzzleImageUnitTriangulator;
}

void cPuzzleImageUnitTriangulatorManager::RemoveObject(cUIImage * e_pUIImage)
{
	auto l_pObject = GetObject(e_pUIImage);
	if (l_pObject)
	{
		int l_iIndex = this->GetObjectIndexByPointer(l_pObject);
		this->RemoveObject(l_iIndex);
	}
}
