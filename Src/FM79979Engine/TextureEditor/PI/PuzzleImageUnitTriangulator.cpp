#include "stdafx.h"
#include "PuzzleImageUnitTriangulator.h"

#include "delaunay_triangulation/DelaunayVector2.h"
#include "delaunay_triangulation/triangle.h"
#include "delaunay_triangulation/delaunay.h"
extern cGlyphFontRender*g_pDebugFont;


void IncreaseLod(std::vector<Vector2> &e_Vector)
{
	std::vector<Vector2> NewPoints;

	// keep the first point
	NewPoints.push_back(e_Vector[0]);
	for (unsigned int i = 0; i < (e_Vector.size() - 1); ++i) 
	{
		// get 2 original points
		const Vector2& p0 = e_Vector[i];
		const Vector2& p1 = e_Vector[i + 1];
		Vector2 Q;
		Vector2 R;

		// average the 2 original points to create 2 new points. For each
		// CV, another 2 verts are created.
		Q.x = 0.75f*p0.x + 0.25f*p1.x;
		Q.y = 0.75f*p0.y + 0.25f*p1.y;
		//Q.z = 0.75f*p0.z + 0.25f*p1.z;
		//Q.z = 0.75f*p0.z + 0.25f*p1.z;

		R.x = 0.25f*p0.x + 0.75f*p1.x;
		R.y = 0.25f*p0.y + 0.75f*p1.y;
		//R.z = 0.25f*p0.z + 0.75f*p1.z;
		//R.z = 0.25f*p0.z + 0.75f*p1.z;

		NewPoints.push_back(Q);
		NewPoints.push_back(R);
	}
	// keep the last point
	NewPoints.push_back(e_Vector[e_Vector.size() - 1]);

	// update the points array
	e_Vector = NewPoints;
}

cPuzzleImageUnitTriangulator::cPuzzleImageUnitTriangulator(cUIImage*e_pTargetImage)
{
	m_bCollided = false;
	m_pbtConvexHullShape = nullptr;
	m_iLOD = 1;
	m_pReferenceImage = e_pTargetImage;
	m_iMouseClosestPointIndex = -1;
	m_bEdited = false;
	m_bWaitForGenerateTriangle = false;
	m_TriangleVector.reserve(500);
	m_iFocusPoint = -1;
	m_pTargetImage = new cUIImage(e_pTargetImage);
	POINT l_OffsetPos = { 0,0 };
	POINT l_OffsetRightDownPos = { m_pTargetImage->GetWidth(),m_pTargetImage->GetHeight() };
	m_pTargetImage->SetLocalPosition(Vector3::Zero);
	m_pTargetImage->SetOffsetPos(l_OffsetPos);
	m_pTargetImage->SetRightDownStripOffPos(l_OffsetRightDownPos);
	m_pTargetImage->SetPos(e_pTargetImage->GetPos());
	//left up,right up,right down,left down
	//must be this order or get wrong
	if (!wcscmp(L"Dicecup_Black", m_pReferenceImage->GetName()))
	{
		int a = 0;
	}
	Vector2 l_vPoints[4];
	if(GetImageBoard(l_vPoints,true))
	{
		for (int i = 0; i < 4; ++i)
			//for(int i=0;i<5;++i)
			m_PointVector.push_back(l_vPoints[i]);
		this->SetLOD(1,true);
	}
}


cPuzzleImageUnitTriangulator::~cPuzzleImageUnitTriangulator()
{
	SAFE_DELETE(m_pTargetImage);
	SAFE_DELETE(m_pbtConvexHullShape);
}

bool	cPuzzleImageUnitTriangulator::IsCollided(cbtConvexHullShape*e_pbtConvexHullShape)
{
	if (this->m_pbtConvexHullShape && e_pbtConvexHullShape)
	{
		return m_pbtConvexHullShape->Collide(e_pbtConvexHullShape);
	}
	return false;
}

bool cPuzzleImageUnitTriangulator::GetImageBoard(Vector2 * e_p4VectorPointer, bool e_bDoStrip)
{
	if (!m_pTargetImage || !m_pReferenceImage)
		return false;
	if (!e_bDoStrip)
	{
		e_p4VectorPointer[0] = Vector2(0, 0);
		e_p4VectorPointer[1] = Vector2((float)m_pTargetImage->GetWidth(),0);
		e_p4VectorPointer[2] = Vector2((float)m_pTargetImage->GetWidth(), (float)m_pTargetImage->GetHeight());
		e_p4VectorPointer[3] = Vector2(0, (float)m_pTargetImage->GetHeight());
	}
	else
	{
		e_p4VectorPointer[0] = Vector2(this->m_pReferenceImage->GetOffsetPos()->x, m_pReferenceImage->GetOffsetPos()->y);
		e_p4VectorPointer[1] = Vector2((float)m_pReferenceImage->GetRightDownStripOffPos().x, (float)m_pReferenceImage->GetOffsetPos()->y);
		e_p4VectorPointer[2] = Vector2((float)m_pReferenceImage->GetRightDownStripOffPos().x, (float)m_pReferenceImage->GetRightDownStripOffPos().y);
		e_p4VectorPointer[3] = Vector2((float)m_pReferenceImage->GetOffsetPos()->x, (float)m_pReferenceImage->GetRightDownStripOffPos().y);
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
	this->m_iMouseClosestPointIndex = GetClosestPoint(Vector2(e_iPosX, e_iPosY));
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
	if (m_iFocusPoint != -1 || m_ePointsToTriangulatorType == ePTPT_ADD)
	{
		SetLOD(this->m_iLOD, true);
		m_bEdited = true;
	}
	m_iFocusPoint = -1;
}

void cPuzzleImageUnitTriangulator::Render()
{
	if (m_bWaitForGenerateTriangle)
		return;
	if (!m_pTargetImage ||!m_pReferenceImage)
		return;

	Vector4 l_vBorder(0,0, (float)m_pReferenceImage->GetWidth(), (float)m_pReferenceImage->GetHeight());
	Vector4 l_vOffsetBorder((float)m_pReferenceImage->GetOffsetPos()->x, (float)m_pReferenceImage->GetOffsetPos()->y,
		(float)m_pReferenceImage->GetRightDownStripOffPos().x,(float)m_pReferenceImage->GetRightDownStripOffPos().y);
	Vector3 l_vPos = m_pTargetImage->GetPos();
	m_pTargetImage->SetPos(Vector3::Zero);
	this->m_pTargetImage->Render();
	m_pTargetImage->SetPos(l_vPos);
	if (m_TriangleVector.size() > 1)
	{
		int l_iCount = (int)m_TriangleVector.size()/3;
		Vector4 l_vColor = Vector4::Zero;
		l_vColor.a = 0.7f;
		l_vColor.g = 1.f;
		for (int i = 0; i < l_iCount; i++)
		{
			std::vector<Vector2>l_vPos;
			l_vPos.push_back(m_TriangleVector[i*3]);
			l_vPos.push_back(m_TriangleVector[i * 3+1]);
			l_vPos.push_back(m_TriangleVector[i * 3+2]);
			l_vPos.push_back(m_TriangleVector[i * 3]);
			//if (i % 2)
			//{
			//	l_vColor.r = (1.f*i) / l_iCount;
			//	l_vColor.b = 1 - l_vColor.r;
			//}
			//else
			//{
			//	l_vColor.b = (1.f*i) / l_iCount;
			//	l_vColor.r = 1 - l_vColor.b;
			//}
			GLRender::RenderLine(&l_vPos, l_vColor);
		}
	}
	if (g_pDebugFont)
	{
		g_pDebugFont->SetLocalTransform(cMatrix44::TranslationMatrix(-18.f,-24.f,0.f)*cMatrix44::ScaleMatrix(Vector3(0.7f, 0.7f, 1)));
		for (size_t i = 0; i < m_PointVector.size(); ++i)
		{
			auto l_vPos = m_PointVector[i];
			if (m_iMouseClosestPointIndex == i)
			{
				g_pDebugFont->SetColor(Vector4::Red);
				g_pDebugFont->RenderFont(l_vPos.x, l_vPos.y, ValueToStringW(i).c_str());
				g_pDebugFont->SetColor(Vector4::One);
			}
			else
				g_pDebugFont->RenderFont(l_vPos.x, l_vPos.y, ValueToStringW(i).c_str());
		}
		g_pDebugFont->SetLocalTransform(cMatrix44::ScaleMatrix(Vector3(1, 1, 1)));
	}
	RenderTriangleImage(Vector3((float)m_pTargetImage->GetWidth()+100, 0,0));
	if (m_pReferenceImage)
	{
		Vector4 l_vColor = Vector4::One;
		l_vColor.a = 0.3f;
		GLRender::RenderRectangle((float)m_pReferenceImage->GetWidth(), (float)m_pReferenceImage->GetHeight(), cMatrix44::Identity, l_vColor);
		GLRender::RenderRectangle(l_vOffsetBorder.Width(), l_vOffsetBorder.Height(), cMatrix44::TranslationMatrix(Vector3(l_vOffsetBorder.x, l_vOffsetBorder.y,0.f)), l_vColor);
	}
}


void	cPuzzleImageUnitTriangulator::RenderPointsShapeLine()
{
	//if (m_TriangleVector.size() > 1 && m_pTargetImage && this->m_bEdited)
	if (m_TriangleVector.size() > 1 && m_pTargetImage )
	{
		Vector4 l_vColor = Vector4::Zero;
		l_vColor.a = 0.7f;
		l_vColor.g = 1.f;
		if (m_bCollided)
		{
			l_vColor.r = 1.f;
			l_vColor.g = 0.f;
		}
		Vector3 l_vPos = this->m_pReferenceImage->GetPos();
		int l_iCount = (int)m_TriangleVector.size() / 3;
		std::vector<Vector2>l_vPosVector;
		for (int i = 0; i < l_iCount; i++)
		{
			l_vPosVector.push_back(m_TriangleVector[i * 3]+l_vPos);
			l_vPosVector.push_back(m_TriangleVector[i * 3 + 1]+ l_vPos);
			l_vPosVector.push_back(m_TriangleVector[i * 3 + 2]+ l_vPos);
			l_vPosVector.push_back(m_TriangleVector[i * 3]+ l_vPos);
		}
		GLRender::RenderLine(&l_vPosVector, l_vColor);
	}
}

void	cPuzzleImageUnitTriangulator::RenderTriangleImage(Vector3 e_vPos)
{
	if (m_pTargetImage)
	{
		m_pTargetImage->ApplyImage();
		int l_iNumTriangles = (int)m_s2DVertex.vPosVector.size();
		if(l_iNumTriangles > 0)
			RenderTrianglesWithMatrix((float*)&m_s2DVertex.vPosVector[0], (float*)&m_s2DVertex.vUVVector[0], (float*)&m_s2DVertex.vColorVector[0], cMatrix44::TranslationMatrix(e_vPos), 3, l_iNumTriangles);
	}
}

void	cPuzzleImageUnitTriangulator::SetPointsVector(std::vector<Vector2>*e_pVector)
{
	this->m_bEdited = true;
	m_PointVector = *e_pVector;
	SetLOD(this->m_iLOD, true);
}

std::vector<Vector2>*	cPuzzleImageUnitTriangulator::GetPointsVector()
{
	return &m_PointVector;
}


int		cPuzzleImageUnitTriangulator::GetLOD()
{
	return m_iLOD;
}

bool	cPuzzleImageUnitTriangulator::SetLOD(int e_iLODIndex, bool e_bForceUpdate)
{
	if (m_iLOD != e_iLODIndex || e_bForceUpdate)
	{
		if (e_iLODIndex < 1)
			m_iLOD = 1;
		else
			m_iLOD = e_iLODIndex;
		m_LODPointVector = m_PointVector;
		//for(int i=1;i< m_iLOD;++i)
		//	IncreaseLod(m_LODPointVector);
		//find a way to make LOD perfect here...?
		//if(m_iLOD != 1)
		//	m_LODPointVector.insert(m_LODPointVector.end(),m_PointVector.begin(), m_PointVector.end());
		GenerateTriangle();
		if (m_iLOD != 1)
		{
			for (int i = 1; i < m_iLOD; ++i)
			{
				std::vector<Vector2>		l_LODTriangleVector;
				for (int i = 0; i < m_TriangleVector.size() / 3; ++i)
				{
					auto v1 = m_TriangleVector[i * 3];
					auto v2 = m_TriangleVector[i * 3 +1];
					auto v3 = m_TriangleVector[i * 3 +2];
					auto l_vEdge1 = (v1 + v2) / 2;
					auto l_vEdge2 = (v2 + v3) / 2;
					auto l_vEdge3 = (v3 + v1) / 2;
					l_LODTriangleVector.push_back(v1);
					l_LODTriangleVector.push_back(l_vEdge1);
					l_LODTriangleVector.push_back(v2);
					l_LODTriangleVector.push_back(l_vEdge2);
					l_LODTriangleVector.push_back(v3);
					l_LODTriangleVector.push_back(l_vEdge3);
				}
				m_LODPointVector = l_LODTriangleVector;
				GenerateTriangle();
			}
		}

		return true;
	}
	return false;
}


void cPuzzleImageUnitTriangulator::GenerateTriangle()
{
	Vector2 l_vPoints[4];
	GetImageBoard(l_vPoints,false);
	m_s2DVertex.vPosVector.clear();
	m_s2DVertex.vUVVector.clear();
	m_s2DVertex.vColorVector.clear();
	m_bWaitForGenerateTriangle = true;
	m_TriangleVector.clear();
	//https://github.com/greenm01/poly2tri
	size_t l_uiPointSize = m_LODPointVector.size();
	if (l_uiPointSize >= 3)
	{
		Delaunay<float> triangulation;
		const std::vector<Triangle<float> > l_Triangles = triangulation.triangulate((std::vector<DelaunayVector2<float> >*)&m_LODPointVector);
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
		SAFE_DELETE(m_pbtConvexHullShape);
		m_pbtConvexHullShape = new cbtConvexHullShape((float*)&m_s2DVertex.vPosVector[0], (int)l_uiSize*3, sizeof(float) * 3);
	}
	m_bWaitForGenerateTriangle = false;
}

void cPuzzleImageUnitTriangulator::PointsToTriangulatorAddMouseDown(int e_iPosX, int e_iPosY, eMouseBehavior e_eMouseBehavior)
{
	MousePosAdjustToImageRectangle(e_iPosX, e_iPosY);
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
			SetLOD(this->m_iLOD, true);
		}
		break;
	}
}

void cPuzzleImageUnitTriangulator::PointsToTriangulatorDeleteMouseDown(int e_iPosX, int e_iPosY, eMouseBehavior e_eMouseBehavior)
{
	//MousePosAdjustToImageRectangle(e_iPosX, e_iPosY);
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
	if(e_eMouseBehavior != eMB_DOWN)
		MousePosAdjustToImageRectangle(e_iPosX, e_iPosY);
	switch (e_eMouseBehavior)
	{
	case eMB_DOWN:
		m_iFocusPoint = GetClosestPoint(Vector2(e_iPosX, e_iPosY));
		break;
	case eMB_MOVE:
		if (m_iFocusPoint != -1)
		{
			m_PointVector[m_iFocusPoint] = Vector2(e_iPosX,e_iPosY);
		}
		break;
	case eMB_UP:
		m_iFocusPoint = -1;
		SetLOD(this->m_iLOD, true);
		break;
	}
}

void	cPuzzleImageUnitTriangulator::MousePosAdjustToImageRectangle(int &e_iPosX, int &e_iPosY)
{
	if (m_pReferenceImage)
	{
		if (e_iPosX < m_pReferenceImage->GetOffsetPos()->x)
			e_iPosX = m_pReferenceImage->GetOffsetPos()->x;
		if (e_iPosX > m_pReferenceImage->GetRightDownStripOffPos().x)
			e_iPosX = m_pReferenceImage->GetRightDownStripOffPos().x;
		if (e_iPosY < m_pReferenceImage->GetOffsetPos()->y)
			e_iPosY = m_pReferenceImage->GetOffsetPos()->y;
		if (e_iPosY > m_pReferenceImage->GetRightDownStripOffPos().y)
			e_iPosY = m_pReferenceImage->GetRightDownStripOffPos().y;
	}
}

cPuzzleImageUnitTriangulatorManager::cPuzzleImageUnitTriangulatorManager()
{
	m_bObjectOverlap = false;
}

cPuzzleImageUnitTriangulatorManager::~cPuzzleImageUnitTriangulatorManager()
{
}

void	cPuzzleImageUnitTriangulatorManager::AssignDataFromPuzzleImage(cPuzzleImage*e_pPI, cUIImage*e_pUIImage)
{
	int l_iIndex = e_pPI->GetObjectIndexByName(e_pUIImage->GetName());
	if (l_iIndex != -1)
	{
		cPuzzleImageUnitTriangulator*l_pPuzzleImageUnitTriangulator = GetObject(e_pUIImage);
		std::vector<Vector2>*l_pPointsVector = e_pPI->GetImageShapePointVector(l_iIndex);
		int	l_iLOD = e_pPI->GetImageShapePointLOD(l_iIndex);
		if (l_pPointsVector)
		{
			l_pPuzzleImageUnitTriangulator->SetLOD(l_iLOD, false);
			l_pPuzzleImageUnitTriangulator->SetPointsVector(l_pPointsVector);
		}
	}
}

bool	cPuzzleImageUnitTriangulatorManager::IsTriangulatorEdited(cUIImage*e_pUIImage)
{
	int l_iCount = this->Count();
	for (int i = 0; i < l_iCount; i++)
	{
		auto l_pObject = GetObject(i);
		if (l_pObject->GetTargetImage() && l_pObject->GetTargetImage()->IsSameName(e_pUIImage))
		{
			return true;
		}
	}
	return false;
}

cPuzzleImageUnitTriangulator* cPuzzleImageUnitTriangulatorManager::GetObject(cUIImage*e_pUIImage)
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


void	cPuzzleImageUnitTriangulatorManager::RenderPointsShapeLine()
{
	int l_iCount = this->Count();
	for (int i = 0; i < l_iCount; i++)
	{
		this->GetObject(i)->RenderPointsShapeLine();
	}
}

void cPuzzleImageUnitTriangulatorManager::MouseMove(int e_iPosX, int e_iPosY)
{
	m_bObjectOverlap = false;
	int l_iCount = this->Count();
	for (int i = 0; i < l_iCount; i++)
	{
		cPuzzleImageUnitTriangulator*l_pPuzzleImageUnitTriangulator = this->GetObject(i);
		if (!l_pPuzzleImageUnitTriangulator->m_pReferenceImage)
			continue;
		auto l_mat = l_pPuzzleImageUnitTriangulator->m_pReferenceImage->GetWorldTransform();
		//auto l_OffsetPos = l_pPuzzleImageUnitTriangulator->m_pReferenceImage->GetOffsetPos();
		//Vector3 l_vPos = l_mat.GetTranslation();
		//l_vPos.x += l_OffsetPos->x;
		//l_vPos.y += l_OffsetPos->y;
		//l_mat.SetTranslation(l_vPos);
		l_pPuzzleImageUnitTriangulator->m_pbtConvexHullShape->SetTransform(l_mat);
		l_pPuzzleImageUnitTriangulator->m_bCollided = false;
	}
	for (int i = 0; i < l_iCount; i++)
	{
		cPuzzleImageUnitTriangulator*l_pPuzzleImageUnitTriangulator = this->GetObject(i);
		for (int j = i+1; j < l_iCount; j++)
		{
			cPuzzleImageUnitTriangulator*l_pPuzzleImageUnitTriangulator2 = this->GetObject(j);
			if (!l_pPuzzleImageUnitTriangulator2->m_pReferenceImage)
				continue;
			if (l_pPuzzleImageUnitTriangulator->IsCollided(l_pPuzzleImageUnitTriangulator2->m_pbtConvexHullShape))
			{
				l_pPuzzleImageUnitTriangulator->m_bCollided = true;
				l_pPuzzleImageUnitTriangulator2->m_bCollided = true;
				m_bObjectOverlap = true;
			}
		}
	}

}

std::vector<Vector2>	Triangulator(std::vector<Vector2>*e_pData)
{
	std::vector<Vector2> l_TriangleVector;
	Delaunay<float> triangulation;
	const std::vector<Triangle<float> > l_Triangles = triangulation.triangulate((std::vector<DelaunayVector2<float> >*)e_pData);
	size_t l_uiSize = l_Triangles.size();
	for (size_t i = 0; i < l_uiSize; i++)
	{
		Vector2 l_v3Pos[3] = { *(Vector2*)(&l_Triangles[i].p1),
								*(Vector2*)(&l_Triangles[i].p2),
								*(Vector2*)(&l_Triangles[i].p3) };
		l_TriangleVector.push_back(Vector2(l_v3Pos[0].x, l_v3Pos[0].y));
		l_TriangleVector.push_back(Vector2(l_v3Pos[1].x, l_v3Pos[1].y));
		l_TriangleVector.push_back(Vector2(l_v3Pos[2].x, l_v3Pos[2].y));
	}
	return l_TriangleVector;
}