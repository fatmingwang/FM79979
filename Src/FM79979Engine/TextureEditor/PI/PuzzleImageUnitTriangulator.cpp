#include "stdafx.h"
#include "PuzzleImageUnitTriangulator.h"

#include "delaunay_triangulation/DelaunayVector2.h"
#include "delaunay_triangulation/triangle.h"
#include "delaunay_triangulation/delaunay.h"


#include "TrianglesToDrawIndicesBuffer.h"
#include "MorphingAnimation_Editor.h"
extern cGlyphFontRender*g_pDebugFont;

std::vector<cPuzzleImageUnitTriangulator::sPosAndVertexIndex> cPuzzleImageUnitTriangulator::GetPosAndVertexIndexVector()
{
	std::vector<sPosAndVertexIndex> l_Vector;
	if (m_pTrianglesToDrawIndicesBuffer)
	{
		for (size_t i = 0; i < m_pTrianglesToDrawIndicesBuffer->vPosVector.size(); ++i)
		{
			sPosAndVertexIndex l_sPosAndVertexIndex;
			l_sPosAndVertexIndex.iVertexIndex = (int)i;
			l_sPosAndVertexIndex.vPos = m_pTrianglesToDrawIndicesBuffer->vPosVector[i];
			l_Vector.push_back(l_sPosAndVertexIndex);
		}
	}
	return l_Vector;
}

cPuzzleImageUnitTriangulator::cPuzzleImageUnitTriangulator(cUIImage*e_pTargetImage)
{
	this->SetName(e_pTargetImage->GetName());
	m_vCenterOffset = Vector2::Zero;
	m_pTrianglesToDrawIndicesBuffer = new sTrianglesToDrawIndicesBuffer();
	m_pEditor_MorphingAnimation = new cEditor_MorphingAnimation(m_pTrianglesToDrawIndicesBuffer);
	m_pEditor_MorphingAnimation->SetName(e_pTargetImage->GetName());
	m_bCollided = false;
	m_pbtConvexHullShape = nullptr;
	m_iLOD = 1;
	m_pReferenceImage = e_pTargetImage;
	m_iAnimationEditSelectedVertexIndex = m_iMouseMoveHittedHintVertexIndex = m_iMouseClosestPointIndex = -1;
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
	SAFE_DELETE(m_pEditor_MorphingAnimation);
	SAFE_DELETE(m_pTrianglesToDrawIndicesBuffer);
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
	return GetClosestPointIndex(e_vPos, &m_PointVector);
	/*size_t l_uiSize = m_PointVector.size();
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
	return -1;*/
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
	Vector3 l_vPos((float)e_iPosX, (float)e_iPosY, 0);
	int l_iBeforeDeleteVertexIndex = -1;
	m_MouseMoveData.MouseUp(e_iPosX, e_iPosY);
	//for morphing data
	std::vector<sPosAndVertexIndex>	l_BeforeDeleteVertex_PosAndVertexIndexVector;
	std::vector<sPosAndVertexIndex>	l_AfterDeleteVertex_PosAndVertexIndexVector;
	std::vector<Vector3>			l_OldVertexPosVector;
	//if (m_ePointsToTriangulatorType != ePTPT_MOVE)
	{//reassign morphing animation vertex index
		l_BeforeDeleteVertex_PosAndVertexIndexVector = GetPosAndVertexIndexVector();
		l_OldVertexPosVector = m_pTrianglesToDrawIndicesBuffer->vPosVector;
	}
	switch (m_ePointsToTriangulatorType)
	{
	case ePTPT_ADD:
		PointsToTriangulatorAddMouseDown(e_iPosX, e_iPosY, eMB_UP);
		break;
	case ePTPT_DELETE:
		//process vertex index before change
		if (m_pTrianglesToDrawIndicesBuffer)
		{
			l_iBeforeDeleteVertexIndex = m_pTrianglesToDrawIndicesBuffer->GetVertexIndexByPos(l_vPos);
		}
		PointsToTriangulatorDeleteMouseDown(e_iPosX, e_iPosY, eMB_UP);
		//process index after change
		break;
	case ePTPT_MOVE:
		PointsToTriangulatorMoveMouseDown(e_iPosX, e_iPosY, eMB_UP);
		break;
	}
	l_AfterDeleteVertex_PosAndVertexIndexVector = GetPosAndVertexIndexVector();
	for (auto l_NewIndexData : l_AfterDeleteVertex_PosAndVertexIndexVector)
	{
		//l_BeforeDeleteVertex_PosAndVertexIndexVector
		int l_iOldIndex = IsVectorContain(l_OldVertexPosVector, l_NewIndexData.vPos);
		if (l_NewIndexData.iVertexIndex == -1)
		{
			int a = 0;
		}
		m_BeforeAndAfterVertexIndexMap[l_iOldIndex] = l_NewIndexData.iVertexIndex;
	}
	bool l_bIndexChanged = false;
	int l_iNotMatchedIndex = -79979;
	for (auto l_Iterator : m_BeforeAndAfterVertexIndexMap)
	{
		if (l_Iterator.first != l_Iterator.second)
		{
			l_bIndexChanged = true;
			l_iNotMatchedIndex = l_Iterator.first;
			break;
		}
	}
	if (l_bIndexChanged)
	{
		if (m_ePointsToTriangulatorType == ePTPT_MOVE)
		{//should be only 1 vertex not match
			std::vector<int> l_iVector1;
			for (size_t i = 0; i < m_BeforeAndAfterVertexIndexMap.size(); ++i)
				l_iVector1.push_back((int)i);
			for (auto l_Iterator : m_BeforeAndAfterVertexIndexMap)
			{
				auto l_iIndex = IndexOfVector<int>(l_Iterator.first, &l_iVector1);
				if(l_iIndex != -1)
				{
					l_iVector1.erase(l_iVector1.begin()+l_iIndex);
				}
			}
			int l_iNdexIndex = m_BeforeAndAfterVertexIndexMap[l_iNotMatchedIndex];
			int l_iOldIndex = l_iVector1[0];
			m_BeforeAndAfterVertexIndexMap[l_iOldIndex] = l_iNdexIndex;
			m_BeforeAndAfterVertexIndexMap.erase(l_iNotMatchedIndex);
		}
		//reassign morphing animation vertex index
		if (m_pEditor_MorphingAnimation)
		{
			m_pEditor_MorphingAnimation->ApplyVertexIndexChangeForMorphingAnimation(m_BeforeAndAfterVertexIndexMap);
		}
		m_bEdited = true;
	}
	//if(m_ePointsToTriangulatorType == ePTPT_MOVE)
	//{
	//	if (m_pEditor_MorphingAnimation && !l_bIndexChanged)
	//	{
	//		int l_iFocusPoint = GetClosestPointIndex(l_vPos, &m_pTrianglesToDrawIndicesBuffer->vPosVector);
	//		m_pEditor_MorphingAnimation->ChangeKeyTime0Data(l_iFocusPoint, l_vPos);
	//	}
	//}
	m_BeforeAndAfterVertexIndexMap.clear();
	m_iFocusPoint = -1;
}

void cPuzzleImageUnitTriangulator::MorphingEditUpdateanimationByBlobalTime(float e_fTargetTime)
{
	if (m_pEditor_MorphingAnimation)
	{
		m_pEditor_MorphingAnimation->UpdateAnimationByGlobalTime(e_fTargetTime);
	}
}

void cPuzzleImageUnitTriangulator::MorphingEditMouseDown(int e_iPosX, int e_iPosY)
{
	m_iAnimationEditSelectedVertexIndex = -1;
	if (m_pTrianglesToDrawIndicesBuffer && m_pEditor_MorphingAnimation)
	{
		Vector3 l_vPos((float)e_iPosX, (float)e_iPosY,0.f);
		m_iAnimationEditSelectedVertexIndex = m_pEditor_MorphingAnimation->FinClosestVertexIndex(l_vPos);
		//float l_fCurrentListboxTime = m_pEditor_MorphingAnimation->GetCurrentListboxTime();
		//if (!m_pEditor_MorphingAnimation->IsTimeAvaliable(m_iAnimationEditSelectedVertexIndex, l_fCurrentListboxTime))
		//{
		//	m_iAnimationEditSelectedVertexIndex = -1;
		//}
	}
}

void cPuzzleImageUnitTriangulator::MorphingEditMouseMove(int e_iPosX, int e_iPosY)
{
	if (!m_pEditor_MorphingAnimation)
		return;
	Vector3 l_vPos((float)e_iPosX, (float)e_iPosY, 0.f);
	if (m_iAnimationEditSelectedVertexIndex != -1)
	{
		float l_fCurrentListboxTime = m_pEditor_MorphingAnimation->GetCurrentListboxTime();
		m_pEditor_MorphingAnimation->ChangeData(m_iAnimationEditSelectedVertexIndex, l_vPos, l_fCurrentListboxTime);
	}
	else
	{
		m_iMouseMoveHittedHintVertexIndex = m_pEditor_MorphingAnimation->FinClosestVertexIndex(l_vPos);
	}
}

void cPuzzleImageUnitTriangulator::MorphingEditMouseUp(int e_iPosX, int e_iPosY)
{
	if (m_iAnimationEditSelectedVertexIndex != -1)
	{
		Vector3 l_vPos((float)e_iPosX, (float)e_iPosY, 0.f);
		float l_fCurrentListboxTime = m_pEditor_MorphingAnimation->GetCurrentListboxTime();
		m_pEditor_MorphingAnimation->ChangeData(m_iAnimationEditSelectedVertexIndex, l_vPos, l_fCurrentListboxTime);
		m_iAnimationEditSelectedVertexIndex = -1;
	}
}

void cPuzzleImageUnitTriangulator::MorphingEditAddKey(float e_fTime, bool e_bUsePreKeyData)
{
	if (m_pEditor_MorphingAnimation)
	{
		m_pEditor_MorphingAnimation->AddKeyTime(e_fTime, e_bUsePreKeyData);
	}
}

bool cPuzzleImageUnitTriangulator::MorphingEditDeleteKey(int e_iTimeKeyIndex)
{
	if (m_pEditor_MorphingAnimation)
	{
		return m_pEditor_MorphingAnimation->DeleteListboxTime(e_iTimeKeyIndex);
	}
	return false;
}

bool cPuzzleImageUnitTriangulator::MorphingEditChangeTime(int e_iTimeListBoxSelectedIndex, float e_fNewTime)
{
	if (m_pEditor_MorphingAnimation)
	{
		return m_pEditor_MorphingAnimation->ChangeListboxTime(e_iTimeListBoxSelectedIndex, e_fNewTime);
	}
	return false;
}

bool cPuzzleImageUnitTriangulator::MorphingEditInsertTime(int e_iTimeListBoxSelectedIndex, float e_fNewTime, bool e_bUsePreKeyData)
{
	if (m_pEditor_MorphingAnimation)
	{
		return m_pEditor_MorphingAnimation->InsertListboxTime(e_iTimeListBoxSelectedIndex, e_fNewTime, e_bUsePreKeyData);
	}
	return false;
}

bool cPuzzleImageUnitTriangulator::MorphingEditApplyEmptyAnimationData()
{
	if (m_pEditor_MorphingAnimation)
	{
		return m_pEditor_MorphingAnimation->ApplyEmptyAnimationData();
	}
	return false;
}

bool cPuzzleImageUnitTriangulator::MorphingEditAddListboxTime(float e_fTime, bool e_bUsePreKeyData)
{
	if (m_pEditor_MorphingAnimation)
	{
		return m_pEditor_MorphingAnimation->AddListboxTime(e_fTime, e_bUsePreKeyData);
	}
	return false;
}

bool cPuzzleImageUnitTriangulator::MorphingEditDeleteListboxTime(int e_iIndex)
{
	if (m_pEditor_MorphingAnimation)
	{
		return m_pEditor_MorphingAnimation->DeleteListboxTime(e_iIndex);
	}
	return false;
}

bool cPuzzleImageUnitTriangulator::MorphingEditChangeListboxTime(int e_iIndex, float e_fTime)
{
	if (m_pEditor_MorphingAnimation)
	{
		return m_pEditor_MorphingAnimation->ChangeListboxTime(e_iIndex,e_fTime);
	}
	return false;
}

bool cPuzzleImageUnitTriangulator::MorphingEditSetCurrentListboxTime(int e_iIndex)
{
	if (m_pEditor_MorphingAnimation)
	{
		return m_pEditor_MorphingAnimation->SetCurrentListboxTime(e_iIndex);
	}
	return false;
}

bool cPuzzleImageUnitTriangulator::MorphingEditRearrangeTime(float e_fTime)
{
	if (m_pEditor_MorphingAnimation)
	{
		m_pEditor_MorphingAnimation->RearrangeTime(e_fTime);
		return true;
	}
	return false;
}

void cPuzzleImageUnitTriangulator::MorphingEditRender()
{
	if (m_pEditor_MorphingAnimation)
	{
		m_pEditor_MorphingAnimation->Render(cMatrix44::Identity,(cBaseImage*)this->m_pReferenceImage);
		if (m_iMouseMoveHittedHintVertexIndex != -1)
		{
			this->m_pEditor_MorphingAnimation->RenderVertexPointByVertexIndex(m_iMouseMoveHittedHintVertexIndex, Vector4::Red, HINT_VERTEX_POINT_SIZE);
		}
	}
}

void cPuzzleImageUnitTriangulator::MorphingEditRenderByTimeVectorIndex(std::vector<int> e_TimeVector)
{
	if (m_pEditor_MorphingAnimation)
	{
		if (m_pEditor_MorphingAnimation->m_fListboxTimeVector.size() >= e_TimeVector.size())
		{
			Vector4 l_vColor = Vector4::One;
			for (auto l_iIndex : e_TimeVector)
			{
				auto l_fTime = m_pEditor_MorphingAnimation->m_fListboxTimeVector[l_iIndex];
				//l_vColor.x = 1.f / (l_iIndex + 1);
				//l_vColor.y = 1.f / (l_iIndex + 1);
				//l_vColor.z = 1.f / (l_iIndex + 1);
				m_pEditor_MorphingAnimation->RenderByTimeForHint(l_fTime, l_vColor, cMatrix44::Identity, (cBaseImage*)this->m_pReferenceImage);
			}
		}
	}
	if (m_iMouseMoveHittedHintVertexIndex != -1)
	{
		this->m_pEditor_MorphingAnimation->RenderVertexPointByVertexIndex(m_iMouseMoveHittedHintVertexIndex, Vector4::Red, HINT_VERTEX_POINT_SIZE);
	}
}

float cPuzzleImageUnitTriangulator::MorphingEditGetEndTime()
{
	if (m_pEditor_MorphingAnimation)
	{
		return m_pEditor_MorphingAnimation->GetEndTime();
	}
	return false;
}

std::vector<float>*cPuzzleImageUnitTriangulator::MorphingEditGetListboxTimeVector()
{
	if (m_pEditor_MorphingAnimation)
	{
		return m_pEditor_MorphingAnimation->GetListboxTimeVector();
	}
	return nullptr;
}

void cPuzzleImageUnitTriangulator::MorphingEditDataCleanUp()
{
	m_pEditor_MorphingAnimation->DataCleanUp();
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
			std::vector<Vector3>l_vPos;
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
	//RenderTriangleImage(Vector3((float)m_pTargetImage->GetWidth()+100, 0,0));
	if (m_pReferenceImage)
	{
		Vector4 l_vColor = Vector4::One;
		l_vColor.a = 0.3f;
		//GLRender::RenderRectangle((float)m_pReferenceImage->GetWidth(), (float)m_pReferenceImage->GetHeight(), cMatrix44::Identity, l_vColor);
		//GLRender::RenderRectangle(l_vOffsetBorder.Width(), l_vOffsetBorder.Height(), cMatrix44::TranslationMatrix(Vector3(l_vOffsetBorder.x, l_vOffsetBorder.y,0.f)), l_vColor);
		if (m_pTrianglesToDrawIndicesBuffer)
		{
			m_pReferenceImage->ApplyImage();
			cMatrix44 l_mat = cMatrix44::TranslationMatrix(Vector3(0, (float)m_pReferenceImage->GetHeight()+100.f,0));
			m_pTrianglesToDrawIndicesBuffer->Render(l_mat);
			l_mat = cMatrix44::TranslationMatrix(Vector3(0.f,(float)m_pReferenceImage->GetHeight()*2 + 100.f, 0));
			m_pTrianglesToDrawIndicesBuffer->RenderInfo(l_mat);
		}
	}
	if (m_iMouseMoveHittedHintVertexIndex != -1 && m_pEditor_MorphingAnimation)
	{
		this->m_pEditor_MorphingAnimation->RenderVertexPointByVertexIndex(m_iMouseMoveHittedHintVertexIndex, Vector4::Red, HINT_VERTEX_POINT_SIZE);
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
		std::vector<Vector3>l_vPosVector;
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

void	cPuzzleImageUnitTriangulator::SetPointsVector(std::vector<Vector3>*e_pVector)
{
	this->m_bEdited = true;
	m_PointVector = *e_pVector;
	SetLOD(this->m_iLOD, true);
}

std::vector<Vector3>*	cPuzzleImageUnitTriangulator::GetPointsVector()
{
	return &m_PointVector;
}


int		cPuzzleImageUnitTriangulator::GetLOD()
{
	return m_iLOD;
}

bool cPuzzleImageUnitTriangulator::LODToPoints()
{
	m_PointVector = m_LODPointVector;
	return this->SetLOD(1, true);
}

int cPuzzleImageUnitTriangulator::GetNumTriangles()
{
	return (int)m_TriangleVector.size()/3;
}

std::string cPuzzleImageUnitTriangulator::GetInfo()
{
	std::string l_strInfo;
	l_strInfo = UT::ComposeMsgByFormat("Points:%d,Triangles:%d,LOD:%d", (int)m_PointVector.size(),GetNumTriangles(),this->m_iLOD);
	return l_strInfo;
}

bool cPuzzleImageUnitTriangulator::IsVertexIndexChanged()
{
	return m_BeforeAndAfterVertexIndexMap.size()?true:false;
}

bool cPuzzleImageUnitTriangulator::ToTixmlElementWithBinaryData(ATG::XMLWriter*e_pXMLWriter,Vector2 e_vPISize, Vector2 e_vTextureSize, Vector2 e_vImagePos, std::vector<Vector3>& e_PosVector, std::vector<Vector2>& e_UVVector, std::vector<int>& e_iIndexBufferVector)
{
	if(!m_pTrianglesToDrawIndicesBuffer)
		return false;
	return m_pTrianglesToDrawIndicesBuffer->ToTixmlElementWithBinaryData(e_pXMLWriter,e_vPISize,e_vTextureSize,e_vImagePos,e_PosVector,e_UVVector,e_iIndexBufferVector,this->m_vCenterOffset);
}

TiXmlElement * cPuzzleImageUnitTriangulator::MorphingAnimationToTiXmlElement(cBinaryFile*e_pMorphingData, cBinaryFile*e_pOptmizeMorphingData)
{
	if (this->m_pEditor_MorphingAnimation)
	{
		return m_pEditor_MorphingAnimation->ToTiXmlElement(e_pMorphingData,e_pOptmizeMorphingData);
	}
	return nullptr;
}

Vector2 cPuzzleImageUnitTriangulator::CalculateCenter()
{
	auto l_vPosCenter = Vector3::Zero;
	Vector3 l_vTotalPos = Vector3::Zero;
	size_t l_uiSize = m_s2DVertex.vPosVector.size();
	for (size_t i = 0; i < l_uiSize; ++i)
	{
		l_vTotalPos += m_s2DVertex.vPosVector[i].vPos[0];
		l_vTotalPos += m_s2DVertex.vPosVector[i].vPos[1];
		l_vTotalPos += m_s2DVertex.vPosVector[i].vPos[2];
	}
	l_uiSize *= 3;
	l_vPosCenter.x = l_vTotalPos.x / l_uiSize;
	l_vPosCenter.y = l_vTotalPos.y / l_uiSize;
	l_vPosCenter.z = l_vTotalPos.z / l_uiSize;
	
	return Vector2(l_vPosCenter.x, l_vPosCenter.y);
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
		//find a way to make LOD perfect here...?
		//if(m_iLOD != 1)
		//	m_LODPointVector.insert(m_LODPointVector.end(),m_PointVector.begin(), m_PointVector.end());
		GenerateTriangle();
		if (m_iLOD != 1)
		{
			for (int i = 1; i < m_iLOD; ++i)
			{
				std::vector<Vector3>		l_LODTriangleVector;
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
		if (m_pTrianglesToDrawIndicesBuffer)
		{
			m_pTrianglesToDrawIndicesBuffer->ParseVertices((Vector3*)&m_s2DVertex.vPosVector[0], (Vector2*)&m_s2DVertex.vUVVector[0], (int)m_s2DVertex.vPosVector.size());
		}
		return true;
	}
	return false;
}

void	VectorFunctionForVector3ToVector2(std::vector<Vector3>&e_Src, std::vector<Vector2>&e_Dest)
{
	e_Dest.resize(e_Src.size());
	int l_iIndex = 0;
	for (auto l_vV3 : e_Src)
	{
		e_Dest[l_iIndex] = e_Src[l_iIndex];
		++l_iIndex;
	}
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
		std::vector<Vector2> l_TempVector;
		VectorFunctionForVector3ToVector2(m_LODPointVector, l_TempVector);
		const std::vector<Triangle<float> > l_Triangles = triangulation.triangulate((std::vector<DelaunayVector2<float> >*)&l_TempVector);
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
			m_TriangleVector.push_back(Vector3(l_v3Pos.vPos[0].x, l_v3Pos.vPos[0].y,0.f));
			m_TriangleVector.push_back(Vector3(l_v3Pos.vPos[1].x, l_v3Pos.vPos[1].y, 0.f));
			m_TriangleVector.push_back(Vector3(l_v3Pos.vPos[2].x, l_v3Pos.vPos[2].y, 0.f));
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
			SetLOD(this->m_iLOD, true);
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
			m_PointVector[m_iFocusPoint] = Vector3((float)e_iPosX, (float)e_iPosY,0.f);
		}
		break;
	case eMB_UP:
		if (m_iFocusPoint != -1)
		{
			m_PointVector[m_iFocusPoint] = Vector3((float)e_iPosX, (float)e_iPosY, 0.f);
		}
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
		std::vector<Vector3>*l_pPointsVector = e_pPI->GetImageShapePointVector(l_iIndex);
		int	l_iLOD = e_pPI->GetImageShapePointLOD(l_iIndex);
		auto l_vCenterPos = e_pPI->GetImageShapePointCenterOffsetPos(l_iIndex);
		if (l_pPointsVector)
		{
			l_pPuzzleImageUnitTriangulator->SetLOD(l_iLOD, false);
			l_pPuzzleImageUnitTriangulator->SetPointsVector(l_pPointsVector);
			l_pPuzzleImageUnitTriangulator->SetCenterOffset(l_vCenterPos);
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

bool cPuzzleImageUnitTriangulatorManager::ExportMorphingAnimation(const char * e_strFileName, const char*e_str2DMeshFileName, const char*e_strImageName)
{
	cBinaryFile l_MorphingData;
	cBinaryFile l_MorphingOptimizeData;
	std::string l_strEditorFileName = UT::ChangeFileExtensionName(e_strFileName, FM_MORPHING_EDITOR_FILE_BINARY_DATA_EXTENSION_NAME);
	std::string l_strOptimizeFileName = UT::ChangeFileExtensionName(e_strFileName, FM_MORPHING_FILE_BINARY_DATA_EXTENSION_NAME);
	std::string l_strPIFileName = UT::ChangeFileExtensionName(UT::GetFileNameWithoutFullPath(e_strFileName).c_str(), "pi");
	std::string l_strMXBinaryFileName = UT::ChangeFileExtensionName(UT::GetFileNameWithoutFullPath(e_strFileName).c_str(), FM_MORPHING_FILE_BINARY_DATA_EXTENSION_NAME);
	l_MorphingData.Writefile(l_strEditorFileName.c_str(), true, false, "w+");
	l_MorphingOptimizeData.Writefile(l_strOptimizeFileName.c_str(), true, false, "w+");
	TiXmlDocument l_Doc(UT::CharToWchar(e_strFileName).c_str());
	TiXmlElement*l_pMorphingAnimationElement = new TiXmlElement(MORPHING_ANIMATION_ROOT_NAME);
	l_pMorphingAnimationElement->SetAttribute(L"Version", FM_MORPHING_ANIMATION_VER);
	l_pMorphingAnimationElement->SetAttribute(CHAR_TO_WCHAR_DEFINE(PI_FILE_ELEMENT_NAME), ValueToStringW(l_strPIFileName.c_str()).c_str());
	auto l_str2DMeshFileName = UT::GetFileNameWithoutFullPath(e_str2DMeshFileName, false);
	l_pMorphingAnimationElement->SetAttribute(CHAR_TO_WCHAR_DEFINE(TI_ELEMENT_NAME), ValueToStringW(l_str2DMeshFileName).c_str());
	l_pMorphingAnimationElement->SetAttribute(L"ImageName", ValueToStringW(UT::GetFileNameWithoutFullPath(e_strImageName,false)).c_str());

	l_pMorphingAnimationElement->SetAttribute(CHAR_TO_WCHAR_DEFINE(MX_BINARY_FILE_ELEMENT_NAME), ValueToStringW(l_strMXBinaryFileName).c_str());
	
	l_Doc.LinkEndChild(l_pMorphingAnimationElement);
	auto l_iCount = this->Count();
	for (int i = 0; i < l_iCount; ++i)
	{
		auto*l_pObject = this->GetObject(i);
		auto l_pElement = l_pObject->MorphingAnimationToTiXmlElement(&l_MorphingData, &l_MorphingOptimizeData);
		l_pMorphingAnimationElement->LinkEndChild(l_pElement);
	}
	l_Doc.SaveFile(e_strFileName);
	return false;
}

bool cPuzzleImageUnitTriangulatorManager::ParseMorphingAnimation(const char * e_strFileName)
{
	cNodeISAX l_Parser;
	if (l_Parser.ParseDataIntoXMLNode(e_strFileName))
	{
		auto l_strBinaryFileName = UT::ChangeFileExtensionName(e_strFileName, FM_MORPHING_EDITOR_FILE_BINARY_DATA_EXTENSION_NAME);
		cBinaryFile l_TrianglesBinaryData;
		l_TrianglesBinaryData.Openfile(l_strBinaryFileName.c_str());
		char*l_pBinaryData = (char*)l_TrianglesBinaryData.GetDataFile(0);
		auto l_pRoot = l_Parser.GetRootElement();
		if (!wcscmp(l_pRoot->Value(), MORPHING_ANIMATION_ROOT_NAME))
		{
			auto l_strVersion = l_pRoot->Attribute(L"Version");
			if (l_strVersion)
			{
				if (GetInt(l_strVersion) != FM_MORPHING_ANIMATION_VER)
				{
					std::string l_strInfo = e_strFileName;
					l_strInfo += " version not matched";
					WARNING_MSG(gcnew String(l_strInfo.c_str()));
					return false;
				}
			}
			l_pRoot = l_pRoot->FirstChildElement();
			while (l_pRoot)
			{
				//cMorphingAnimation
				auto l_iCount = this->Count();
				for (int i = 0; i < l_iCount; ++i)
				{
					auto l_pObject = this->GetObject(i);
					//l_pObject->ParseMorphAnimationElement(l_pRoot, l_pBinaryData);
					if (l_pObject->m_pEditor_MorphingAnimation)
					{
						auto l_iBinaryStep = l_pObject->m_pEditor_MorphingAnimation->ParseMorphAnimationElement(l_pRoot, l_pBinaryData);
						l_pBinaryData += l_iBinaryStep;
					}
				}
				l_pRoot = l_pRoot->NextSiblingElement();
			}
			return true;
		}
	}
	return false;
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