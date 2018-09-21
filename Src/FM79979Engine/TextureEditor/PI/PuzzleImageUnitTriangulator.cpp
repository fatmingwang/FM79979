#include "stdafx.h"
#include "PuzzleImageUnitTriangulator.h"

#include "delaunay_triangulation/DelaunayVector2.h"
#include "delaunay_triangulation/triangle.h"
#include "delaunay_triangulation/delaunay.h"
extern cGlyphFontRender*g_pDebugFont;
cPuzzleImageUnitTriangulator::cPuzzleImageUnitTriangulator(cUIImage*e_pTargetImage)
{
	m_bWaitForGenerateTriangle = false;
	m_TriangleVector.reserve(500);
	m_iFocusPoint = -1;
	m_pTargetImage = new cUIImage(e_pTargetImage);
	POINT l_OffsetPos = { 0,0 };
	POINT l_OffsetRightDownPos = { m_pTargetImage->GetWidth(),m_pTargetImage->GetHeight() };
	m_pTargetImage->SetOffsetPos(l_OffsetPos);
	m_pTargetImage->SetRightDownStripOffPos(l_OffsetRightDownPos);
	//left up,right up,right down,left down
	//must be this order or get wrong
	Vector2 l_vPoints[4];
	if(GetImageBoard(l_vPoints))
	{
		for (int i = 0; i < 4; ++i)
			//for(int i=0;i<5;++i)
			m_PointVector.push_back(l_vPoints[i]);
		GenerateTriangle();
	}
}

//cPuzzleImageUnitTriangulator::cPuzzleImageUnitTriangulator(TiXmlElement * e_pTiXmlElement)
//{
//	m_TriangleVector.reserve(500);
//	m_iFocusPoint = -1;
//}

cPuzzleImageUnitTriangulator::~cPuzzleImageUnitTriangulator()
{
	SAFE_DELETE(m_pTargetImage);
}

bool cPuzzleImageUnitTriangulator::GetImageBoard(Vector2 * e_p4VectorPointer)
{
	if (!m_pTargetImage)
		return false;
	//bool l_bStrip = false;
	//if (m_pTargetImage->GetOffsetPos()->x || m_pTargetImage->GetOffsetPos()->y)
	//	l_bStrip = true;
	//if(m_pTargetImage->GetRightDownStripOffPos().x!= m_pTargetImage->GetWidth() ||
	//	m_pTargetImage->GetRightDownStripOffPos().y != m_pTargetImage->GetWidth())
	//	l_bStrip = true;
	//if (e_bStripSize)
	//{
	//	e_p4VectorPointer[0] = Vector2(m_pTargetImage->GetOffsetPos()->x, m_pTargetImage->GetOffsetPos()->y);
	//	e_p4VectorPointer[1] = Vector2((float)m_pTargetImage->GetRightDownStripOffPos().x, (float)m_pTargetImage->GetOffsetPos()->y);
	//	e_p4VectorPointer[2] = Vector2((float)m_pTargetImage->GetRightDownStripOffPos().x, (float)m_pTargetImage->GetRightDownStripOffPos().y);
	//	e_p4VectorPointer[3] = Vector2((float)m_pTargetImage->GetOffsetPos()->x, (float)m_pTargetImage->GetRightDownStripOffPos().y);
	//}
	//else
	{
		e_p4VectorPointer[0] = Vector2(0, 0);
		e_p4VectorPointer[1] = Vector2((float)m_pTargetImage->GetWidth(),0);
		e_p4VectorPointer[2] = Vector2((float)m_pTargetImage->GetWidth(), (float)m_pTargetImage->GetHeight());
		e_p4VectorPointer[3] = Vector2(0, (float)m_pTargetImage->GetHeight());
	}
	return true;
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
	if (m_bWaitForGenerateTriangle)
		return;
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
	if (m_bWaitForGenerateTriangle)
		return;
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
	if (m_bWaitForGenerateTriangle)
		return;
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
	m_iFocusPoint = -1;
}

void cPuzzleImageUnitTriangulator::Render()
{
	if (m_bWaitForGenerateTriangle)
		return;
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
	if (g_pDebugFont)
	{
		g_pDebugFont->SetLocalTransform(cMatrix44::TranslationMatrix(-18.f,-24.f,0.f)*cMatrix44::ScaleMatrix(Vector3(0.3f, 0.3f, 1)));
		for (size_t i = 0; i < m_PointVector.size(); ++i)
		{
			auto l_vPos = m_PointVector[i];
			g_pDebugFont->RenderFont(l_vPos.x, l_vPos.y, ValueToStringW(i).c_str());
		}
		g_pDebugFont->SetLocalTransform(cMatrix44::ScaleMatrix(Vector3(1, 1, 1)));
	}
	RenderTriangleImage(Vector3((float)m_pTargetImage->GetWidth(), (float)m_pTargetImage->GetHeight(),0));
	GLRender::RenderRectangle(1000,1000,cMatrix44::Identity,Vector4::Red);
}

void	cPuzzleImageUnitTriangulator::RenderTriangleImage(Vector3 e_vPos)
{
	if (m_pTargetImage)
	{
		m_pTargetImage->ApplyImage();
		int l_iNumTriangles = (int)m_s2DVertex.vPosVector.size();
		RenderTrianglesWithMatrix((float*)&m_s2DVertex.vPosVector[0], (float*)&m_s2DVertex.vUVVector[0], (float*)&m_s2DVertex.vColorVector[0], cMatrix44::TranslationMatrix(e_vPos), 3, l_iNumTriangles);
	}
}

void	cPuzzleImageUnitTriangulator::SetTriangulatorPointsVector(std::vector<Vector2>*e_pVector)
{

}

TiXmlElement* cPuzzleImageUnitTriangulator::ToTiXmlElement()
{
	TiXmlElement*l_pTiXmlElement = new TiXmlElement(L"PIUnitTriangulator");
	auto l_strPoints = ValueToStringW(m_PointVector);
	l_pTiXmlElement->SetAttribute(L"Points", l_strPoints);
	return nullptr;
}

void cPuzzleImageUnitTriangulator::GenerateTriangle()
{
	Vector2 l_vPoints[4];
	GetImageBoard(l_vPoints);
	m_s2DVertex.vPosVector.clear();
	m_s2DVertex.vUVVector.clear();
	m_s2DVertex.vColorVector.clear();
	m_bWaitForGenerateTriangle = true;
	m_TriangleVector.clear();
	//https://github.com/greenm01/poly2tri
	size_t l_uiPointSize = m_PointVector.size();
	if (l_uiPointSize >= 3)
	{
		Delaunay<float> triangulation;
		const std::vector<Triangle<float> > l_Triangles = triangulation.triangulate((std::vector<DelaunayVector2<float> >*)&m_PointVector);
		size_t l_uiSize = l_Triangles.size();
		for (size_t i = 0; i < l_uiSize; i++)
		{
			s2DVertex::s3PosPoints l_v3Pos = {	*(Vector2*)(&l_Triangles[i].p1),
									*(Vector2*)(&l_Triangles[i].p2),
									*(Vector2*)(&l_Triangles[i].p3) };
			s2DVertex::s3UVPoints l_v3UV = {	Vector2(l_v3Pos.vPos[0].x / l_vPoints[2].x,l_v3Pos.vPos[0].y / l_vPoints[2].y),
												Vector2(l_v3Pos.vPos[1].x / l_vPoints[2].x,l_v3Pos.vPos[1].y / l_vPoints[2].y),
												Vector2(l_v3Pos.vPos[2].x / l_vPoints[2].x,l_v3Pos.vPos[2].y / l_vPoints[2].y) };
			s2DVertex::s3ColorPoints l_v3Color = {Vector4::One,Vector4::One ,Vector4::One };
			m_s2DVertex.vPosVector.push_back(l_v3Pos);
			m_s2DVertex.vUVVector.push_back(l_v3UV);
			m_s2DVertex.vColorVector.push_back(l_v3Color);
			m_TriangleVector.push_back(Vector2(l_v3Pos.vPos[0].x, l_v3Pos.vPos[0].y));
			m_TriangleVector.push_back(Vector2(l_v3Pos.vPos[1].x, l_v3Pos.vPos[1].y));
			m_TriangleVector.push_back(Vector2(l_v3Pos.vPos[2].x, l_v3Pos.vPos[2].y));
		}
	}
	m_bWaitForGenerateTriangle = false;
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
		if (l_pObject->GetTargetImage()->IsSameName(e_pUIImage))
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
