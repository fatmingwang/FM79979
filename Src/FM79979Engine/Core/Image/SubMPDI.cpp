#include "../stdafx.h"
#include "SimplePrimitive.h"
//#include "AnimationParser.h"
#include "SubMPDI.h"
#include "MPDI.h"
#include "../GLSL/Shader.h"
#include "../GameplayUT/GameApp.h"
#include "../Sound/SoundManager.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cSubMPDI);
	cCueToStartCurveWithTime::cCueToStartCurveWithTime(cMulti_PI_Image*e_pMulti_PI_Image) :cMulti_PI_Image(e_pMulti_PI_Image)
	{
		m_eAnchorType = eIAT_CENTER_CENTER;
		m_iCurrentHintPointDataIndex = -1;
		m_vRotationAnglePosOffset = Vector3::Zero;
		m_pCurrentPointData = new sTexBehaviorDataWithImageIndexData();
		m_bBehaviorUpdateActive = false;
		m_fCurrentTime = 0.f;
		m_bColorBlending = false;
		m_SrcBlendingMode = GL_SRC_ALPHA;
		m_DestBlendingMode = GL_ONE_MINUS_SRC_ALPHA;
		m_bHintPoint = false;
		m_bStayAtLastFrame = false;
	}

	cCueToStartCurveWithTime::cCueToStartCurveWithTime(cCueToStartCurveWithTime*e_pCueToStartCurvesWithTime)
		:cMulti_PI_Image(e_pCueToStartCurvesWithTime), cCurveWithTime(e_pCueToStartCurvesWithTime), Frame(e_pCueToStartCurvesWithTime)
	{
		if (e_pCueToStartCurvesWithTime->IsStayAtLastFrame())
		{
			int a = 0;
		}
		m_eAnchorType = e_pCueToStartCurvesWithTime->m_eAnchorType;

		m_vRotationAnglePosOffset = e_pCueToStartCurvesWithTime->m_vRotationAnglePosOffset;
		//m_pCurrentPointData = new sTexBehaviorDataWithImageIndexData(e_pCueToStartCurvesWithTime->m_pCurrentPointData);
		m_pCurrentPointData = new sTexBehaviorDataWithImageIndexData();
		size_t l_iSize = e_pCueToStartCurvesWithTime->m_PointDataList.size();
		m_PointDataList.reserve(l_iSize);
		for (size_t i = 0; i<l_iSize; ++i)
		{
			sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = new sTexBehaviorDataWithImageIndexData(e_pCueToStartCurvesWithTime->m_PointDataList[i]);
			m_PointDataList.push_back(l_pTexBehaviorDataWithImageIndexData);
		}
		m_bBehaviorUpdateActive = e_pCueToStartCurvesWithTime->m_bBehaviorUpdateActive;
		m_fCurrentTime = e_pCueToStartCurvesWithTime->m_fCurrentTime;
		m_bColorBlending = e_pCueToStartCurvesWithTime->m_bColorBlending;
		m_SrcBlendingMode = e_pCueToStartCurvesWithTime->m_SrcBlendingMode;
		m_DestBlendingMode = e_pCueToStartCurvesWithTime->m_DestBlendingMode;
		m_bHintPoint = e_pCueToStartCurvesWithTime->m_bHintPoint;
		m_iCurrentHintPointDataIndex = e_pCueToStartCurvesWithTime->m_iCurrentHintPointDataIndex;
		m_HintPointVector = e_pCueToStartCurvesWithTime->m_HintPointVector;
	}

	cCueToStartCurveWithTime::cCueToStartCurveWithTime()
	{
		m_eAnchorType = eIAT_CENTER_CENTER;
		m_iCurrentHintPointDataIndex = -1;
		m_vRotationAnglePosOffset = Vector3::Zero;
		m_pCurrentPointData = new sTexBehaviorDataWithImageIndexData();
		m_bBehaviorUpdateActive = false;
		m_fCurrentTime = 0.f;
		m_bColorBlending = false;
		m_SrcBlendingMode = GL_SRC_ALPHA;
		m_DestBlendingMode = GL_ONE_MINUS_SRC_ALPHA;
		m_bHintPoint = false;
	}

	cCueToStartCurveWithTime::~cCueToStartCurveWithTime()
	{
		DELETE_VECTOR(m_PointDataList, sTexBehaviorDataWithImageIndexData*);
		SAFE_DELETE(m_pCurrentPointData);
	}

	cMatrix44			cCueToStartCurveWithTime::GetConvertedWorldTransformIfParentRequireDoPositionOffsetToCenter()
	{
		cMPDI*l_pParent = dynamic_cast<cMPDI*>(this->GetParent());
//		cMPDI*l_pParent = reinterpret_cast<cMPDI*>(this->GetParent());
		if (l_pParent && l_pParent->IsDoPositionOffsetToCenter())
		{
			Vector2 l_vDrawSize = l_pParent->GetDrawSize() / 2;
			cMatrix44 l_matParentMatrix = l_pParent->GetWorldTransform()*cMatrix44::TranslationMatrix(-Vector3(l_vDrawSize.x, l_vDrawSize.y, 0));
			l_matParentMatrix *= this->GetLocalTransform();
			return l_matParentMatrix;
		}
		return this->GetWorldTransform();
	}

	void	cCueToStartCurveWithTime::UpdateData()//to detec current point index and image index
	{
		if (!m_PointDataList.size())
			return;
		int	l_iCurrentOriginalPointIndex = (int)(m_PointDataList.size() - 1);
		if (!this->IsAnimationDone())
		{
			l_iCurrentOriginalPointIndex = CalculateCurrentPointIndex();
#ifdef DEBUG
			if (l_iCurrentOriginalPointIndex <0)
			{
				int a = 0;
			}
#endif
			float l_fEndTime = this->GetEndTime();
			//if(l_iCurrentOriginalPointIndex+1<(int)m_PointDataList.size() &&
			//	(this->m_fPastTime<l_fEndTime||this->m_bAnimationLoop) )
			if (this->m_fPastTime<this->GetEndTime() || this->IsAnimationLoop())
			{
				if (m_bBehaviorUpdateActive && l_iCurrentOriginalPointIndex + 1<(int)m_PointDataList.size())
				{
					m_PointDataList[l_iCurrentOriginalPointIndex]->ToLerp(m_PointDataList[l_iCurrentOriginalPointIndex + 1], this->m_fCurrentLERPTime, m_pCurrentPointData);
				}
				else
					//because fucking dot net is stupid about momory stack,it will get old memory,and I dnno why,so I have to recheck again here...
					if ((int)m_PointDataList.size() > l_iCurrentOriginalPointIndex)
					{
						m_PointDataList[l_iCurrentOriginalPointIndex]->DumpData(m_pCurrentPointData);
					}
			}
			else
			{
				if (!this->m_bCurveLoop)
				{
					this->SetAnimationDone(true);
				}
				m_PointDataList[l_iCurrentOriginalPointIndex]->DumpData(m_pCurrentPointData);
			}
		}
		else
		{
			if (l_iCurrentOriginalPointIndex >= (int)m_PointDataList.size())
				l_iCurrentOriginalPointIndex = (int)m_PointDataList.size() - 1;
			*m_pCurrentPointData = *this->m_PointDataList[l_iCurrentOriginalPointIndex];
		}
		//update to latest pi.
		m_pPuzzleImage = m_pCurrentPointData->pPI;
#ifdef	DEBUG 
		if (m_pCurrentPointData->iImageIndex == -1)
		{
			std::wstring	l_str = UT::ComposeMsgByFormat(L"cCueToStartCurveWithTime image error,Name:%s", this->GetName());
			cGameApp::OutputDebugInfoString(l_str.c_str());
			//UT::ErrorMsg(this->GetName(),L"image error");
		}
#endif
		//hint point detect
		UINT	l_iNumHintPoint = (UINT)m_HintPointVector.size();
		if (l_iNumHintPoint)
		{
			sHintPoint*l_pHintPoint = &m_HintPointVector[0];
			for (UINT i = 0; i<l_iNumHintPoint; ++i)
			{
				if (l_pHintPoint[i].iPointIndex == l_iCurrentOriginalPointIndex)
				{
					if (m_iCurrentHintPointDataIndex != i)
					{
						m_bHintPoint = true;
						m_iCurrentHintPointDataIndex = i;
						if (l_pHintPoint->AnimationEvent == eAE_SOUND)
						{
							cBasicSound*l_pSound = cGameApp::m_spSoundParser->GetObject(UT::GetFileNameWithoutFullPath(l_pHintPoint->strEventName));
							if (l_pSound)
								l_pSound->Play(true);
						}
						//else
						//{
						//cPrtEmitter*l_pPrtEmitter = cGameApp::m_spPaticleManager->GetObject(l_pHintPoint->strEventName);
						//if( l_pPrtEmitter )
						//	l_pPrtEmitter->
						//}
						return;
					}
				}
			}
			m_bHintPoint = false;
		}
	}

	void	cCueToStartCurveWithTime::AddPoint(Vector3 e_vPos, float e_fTime, Vector2 e_Size, Vector3 e_vAngle, Vector4 e_vColor, int e_iImageIndex, bool e_bMirror, cPuzzleImage*e_pPI, bool e_bCheckOptmize)
	{
		cCurveWithTime::AddPoint(e_vPos, e_fTime);
		sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = new sTexBehaviorDataWithImageIndexData(e_Size, e_vAngle, e_vColor, e_iImageIndex, e_bMirror, e_pPI);
		m_PointDataList.push_back(l_pTexBehaviorDataWithImageIndexData);
		if (e_bCheckOptmize&&this->GetOwner())
		{
			cMultiPathDynamicImage*l_pMultiPathDynamicImage = dynamic_cast<cMultiPathDynamicImage*>(this->GetOwner());
			l_pMultiPathDynamicImage->CheckRenderOptmize();
		}
	}

	void	cCueToStartCurveWithTime::AddPoint(Vector3 e_vPos, float e_fTime, Vector2 e_Size, float e_fAngle, Vector4 e_vColor, int e_iImageIndex, bool e_bMirror, cPuzzleImage*e_pPI, bool e_bCheckOptmize)
	{
		cCurveWithTime::AddPoint(e_vPos, e_fTime);
		sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = new sTexBehaviorDataWithImageIndexData(e_Size, Vector3(0, 0, e_fAngle), e_vColor, e_iImageIndex, e_bMirror, e_pPI);
		m_PointDataList.push_back(l_pTexBehaviorDataWithImageIndexData);
		if (e_bCheckOptmize&&this->GetOwner())
		{
			cMultiPathDynamicImage*l_pMultiPathDynamicImage = dynamic_cast<cMultiPathDynamicImage*>(this->GetOwner());
			l_pMultiPathDynamicImage->CheckRenderOptmize();
		}
	}
	//
	void	cCueToStartCurveWithTime::DelPoint(int e_iIndex)
	{
		cCurveWithTime::DelPoint(e_iIndex);
		sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = m_PointDataList[e_iIndex];
		delete l_pTexBehaviorDataWithImageIndexData;
		m_PointDataList.erase(m_PointDataList.begin() + e_iIndex);
	}

	void	cCueToStartCurveWithTime::ClearPoint()
	{
		while (m_PointDataList.size())
		{
			DelPoint(0);
		}
	}

	sTexBehaviorDataWithImageIndexData*cCueToStartCurveWithTime::GetPointData(int e_iIndex)
	{
		if ((int)m_PointDataList.size() <= e_iIndex)
			return 0;
		return m_PointDataList[e_iIndex];
	}

	cBaseImage*		cCueToStartCurveWithTime::PointDataToBaseImage(int e_iIndex)
	{
		sTexBehaviorDataWithImageIndexData*l_pData = GetPointData(e_iIndex);
		if (!l_pData)
			return 0;
		cPuzzleImageUnit*l_pPIUnit = l_pData->pPI->GetObject(l_pData->iImageIndex);
		if (!l_pPIUnit)
			return 0;
		cBaseImage*l_pBaseImage = new cBaseImage(l_pPIUnit);
		l_pBaseImage->SetWidth((int)l_pData->Size.x);
		l_pBaseImage->SetHeight((int)l_pData->Size.y);
		Vector3	l_vPos = this->GetOriginalPointList()[e_iIndex];
		l_pBaseImage->SetPos(l_vPos);
		return l_pBaseImage;
	}

	void	cCueToStartCurveWithTime::InsertPoint(Vector3 e_Pos, float e_fTime, sTexBehaviorDataWithImageIndexData* e_pTexBehaviorDataWithImageIndexData, int e_iIndex)
	{
		cCurveWithTime::InsertPoint(e_Pos, e_fTime, e_iIndex);
		sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = new sTexBehaviorDataWithImageIndexData(e_pTexBehaviorDataWithImageIndexData);
		this->m_PointDataList.insert(m_PointDataList.begin() + e_iIndex, l_pTexBehaviorDataWithImageIndexData);
	}

	//void cCueToStartCurveWithTime::ReplacePointsByCount(int e_iNumPoint)
	//{
	//	cCurveWithTime::ReplacePointsByCount(e_iNumPoint);
	//	if( !e_iNumPoint )
	//		return;
	//	std::vector<sTexBehaviorDataWithImageIndexData>l_PointDataList = m_PointDataList;
	//	int	l_iNum = l_PointDataList.size();
	//	float	l_fStep = (float)l_iNum/e_iNumPoint;
	//	m_PointDataList.clear();
	//	for( int i=0;i<e_iNumPoint;++i )
	//	{
	//		m_PointDataList.push_back(l_PointDataList[(int)(l_fStep*i)]);
	//	}
	//}

	void cCueToStartCurveWithTime::SetOriginalToFinal()
	{
		std::vector<sTexBehaviorDataWithImageIndexData*> l_PointDataList;
		size_t	l_uiSize = this->m_FinallyPointList.size();
		size_t	l_iStepSize = l_uiSize / this->m_OriginalPointList.size();
		//	int	l_iLod = 1;	
		for (UINT i = 0; i<l_uiSize; ++i)
		{
			sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = new sTexBehaviorDataWithImageIndexData(m_PointDataList[i / l_iStepSize]);
			l_PointDataList.push_back(l_pTexBehaviorDataWithImageIndexData);
		}
		DELETE_VECTOR(m_PointDataList, sTexBehaviorDataWithImageIndexData*);
		m_PointDataList.reserve(l_uiSize);
		for (UINT i = 0; i<l_uiSize; ++i)
		{
			m_PointDataList.push_back(l_PointDataList[i]);
		}
		l_PointDataList.clear();
		cCurveWithTime::SetOriginalToFinal();
		assert(m_PointDataList.size() == this->m_OriginalPointList.size());
	}

	void	cCueToStartCurveWithTime::RenderPuzzleData(sPuzzleData*e_pPuzzleData)
	{
		if (m_pCurrentPointData->vColor.a == 0)//totaly transparent
			return;
		sBlendfunctionRestore l_BlendfunctionRestore;
		if (this->m_bColorBlending)
		{
			l_BlendfunctionRestore.GetStatus();
			glBlendFunc(m_SrcBlendingMode, m_DestBlendingMode);
		}
		GetRenderPuzzleDataAndMatrix(m_pCurrentPointData, 3, (float*)this->m_2DVertices.vPos, (float*)m_2DVertices.fUV, e_pPuzzleData);
		cMatrix44 l_mat = GetConvertedWorldTransformIfParentRequireDoPositionOffsetToCenter();
		DrawQuadWithMatrix((float*)&this->m_2DVertices.vPos, (float*)m_2DVertices.fUV, m_pCurrentPointData->vColor, l_mat, 3, 1);
		if (this->m_bColorBlending)
		{
			l_BlendfunctionRestore.Restore();
		}
	}


	void	cCueToStartCurveWithTime::SetColor(Vector4 e_vColor)
	{
		UINT	l_uiSize = (UINT)this->m_FinallyPointList.size();
		for (UINT i = 0; i<l_uiSize; ++i)
		{
			m_PointDataList[i]->vColor = e_vColor;
		}
	}

	void	cCueToStartCurveWithTime::SetColorScale(Vector4 e_vColor)
	{
		UINT	l_uiSize = (UINT)this->m_FinallyPointList.size();
		for (UINT i = 0; i<l_uiSize; ++i)
		{
			Vector4	l_vColor = m_PointDataList[i]->vColor;
			l_vColor.x *= e_vColor.x;
			l_vColor.y *= e_vColor.y;
			l_vColor.z *= e_vColor.z;
			l_vColor.w *= e_vColor.w;
			m_PointDataList[i]->vColor = l_vColor;
		}
	}

	void	cCueToStartCurveWithTime::SetCurrentColor(Vector4 e_vColor)
	{
		this->m_pCurrentPointData->vColor = e_vColor;
	}


	void	cCueToStartCurveWithTime::GetRenderPuzzleDataAndMatrix(sTexBehaviorDataWithImageIndexData*e_pImageData, int e_iStride, float*e_pOutVertices, float*e_pOutTextureUV, sPuzzleData*e_pPuzzleData)
	{
		sPuzzleData*l_pPuzzleData = e_pPuzzleData;
#ifdef DEBUG
		if (!e_pImageData || !l_pPuzzleData || l_pPuzzleData->Size.x == 0 || l_pPuzzleData->Size.y == 0)
		{//something wrong here.
			int a = 0;
		}
#endif
		Vector2	l_vScale(e_pImageData->Size.x / l_pPuzzleData->Size.x, e_pImageData->Size.y / l_pPuzzleData->Size.y);
		Vector2	l_vImageSize(e_pImageData->Size.x, e_pImageData->Size.y);
		Vector2	l_vImageHalfSize = l_vImageSize / 2.f;
		Vector2	l_vOffsetPos((float)l_pPuzzleData->OffsetPos.x, (float)l_pPuzzleData->OffsetPos.y);
		Vector2	l_vOriginalSize((float)l_pPuzzleData->OriginalSize.x, (float)l_pPuzzleData->OriginalSize.y);
		Vector2	l_vHalfOriginalSize = l_vOriginalSize / 2.f;
		Vector2	l_vCenterToOffset = l_vOffsetPos - l_vHalfOriginalSize;

		Vector2	l_vFinalOffsetPos = l_vOriginalSize / 2;
		//although this one is not correct but work for some function.
		Vector4	l_vEntendImagePoint = GetExtendImagePoint(m_eAnchorType,l_vImageSize, 1.f);
		Vector4	l_vDrawRect = l_vEntendImagePoint;
		if (m_eAnchorType == eIAT_CENTER_CENTER)
		{
			l_vCenterToOffset.x *= l_vScale.x;
			l_vCenterToOffset.y *= l_vScale.y;
			l_vDrawRect = Vector4(0, 0, l_vImageSize.x, l_vImageSize.y);
		}
		//set vertex
		if (e_pOutVertices)
		{
			if (e_iStride == 2)
			{
				//e_pOutVertices[0] = 0;				 e_pOutVertices[1] = 0;
				//e_pOutVertices[2] = l_vImageSize.x;  e_pOutVertices[3] = 0;
				//e_pOutVertices[4] = 0;				 e_pOutVertices[5] = l_vImageSize.y;
				//e_pOutVertices[6] = l_vImageSize.x;  e_pOutVertices[7] = l_vImageSize.y;
				e_pOutVertices[0] = l_vDrawRect.x;  e_pOutVertices[1] = l_vDrawRect.y;
				e_pOutVertices[2] = l_vDrawRect.z;  e_pOutVertices[3] = l_vDrawRect.y;
				e_pOutVertices[4] = l_vDrawRect.x;  e_pOutVertices[5] = l_vDrawRect.w;
				e_pOutVertices[6] = l_vDrawRect.z;  e_pOutVertices[7] = l_vDrawRect.w;
			}
			else
				if (e_iStride == 3)
				{
					e_pOutVertices[0] = 0;				e_pOutVertices[1] = 0;				e_pOutVertices[2] = 0.f;
					e_pOutVertices[3] = l_vImageSize.x;	e_pOutVertices[4] = 0;				e_pOutVertices[5] = 0.f;
					e_pOutVertices[6] = 0;				e_pOutVertices[7] = l_vImageSize.y; e_pOutVertices[8] = 0.f;
					e_pOutVertices[9] = l_vImageSize.x;	e_pOutVertices[10] = l_vImageSize.y; e_pOutVertices[11] = 0.f;
				}
		}
		if (e_pOutTextureUV)
			AssignUVDataToTriangleStrip(l_pPuzzleData->fUV, e_pOutTextureUV, e_pImageData->bMirror);

		//not quaterion is because quaterion has more complex calculation.
		Vector3	l_vPos = Vector3(l_vFinalOffsetPos.x, l_vFinalOffsetPos.y, 0.f);
		cMatrix44  l_mat2DTransform = cMatrix44::TranslationMatrix(l_vPos + this->GetCurrentPosition());
		//cMatrix44  l_mat2DTransform = cMatrix44::TranslationMatrix(l_vPos);
		l_mat2DTransform *= cMatrix44::RotationMatrix(this->m_pCurrentPointData->vAngle);
		l_mat2DTransform *= cMatrix44::TranslationMatrix(m_vRotationAnglePosOffset + l_vCenterToOffset);
		this->SetLocalTransform(l_mat2DTransform);
	}

	Vector4	cCueToStartCurveWithTime::GetCollideRectByIndex(int e_iIndex)
	{
		if (GetTransformedVerticesByIndex(g_fMPDIOptmizeRenderVertices, g_fMPDIOptmizeRenderUV, g_fMPDIOptmizeRenderColor, 0))
		{
			Vector4	l_vRect(g_fMPDIOptmizeRenderVertices[6], g_fMPDIOptmizeRenderVertices[7], g_fMPDIOptmizeRenderVertices[12], g_fMPDIOptmizeRenderVertices[13]);
			return l_vRect;
		}
		return Vector4::Zero;
		//return Vector4(l_Vertices[0].x+l_vPos.x,l_Vertices[0].y+l_vPos.y,l_Vertices[3].x+l_vPos.x,l_Vertices[3].y+l_vPos.y);
	}

	bool	cCueToStartCurveWithTime::AssignColorAndTransformToVertices(Vector4 e_vInputColor, float*e_pfVertices, float*e_pfColor)
	{
		if (e_pfColor)
		{
			for (int i = 0; i<6; ++i)
				memcpy(&e_pfColor[i * 4], &e_vInputColor, sizeof(Vector4));
		}
		cMatrix44 l_mat2DTransform = GetConvertedWorldTransformIfParentRequireDoPositionOffsetToCenter();
		Vector3 l_Vertices[4];
		memcpy(l_Vertices, &this->m_2DVertices.vPos, sizeof(Vector3) * 4);
		if (e_pfVertices)
		{
			for (int i = 0; i<4; ++i)
				l_Vertices[i] = l_mat2DTransform.TransformCoordinate(l_Vertices[i]);
			e_pfVertices[0] = l_Vertices[2].x;			//left down
			e_pfVertices[1] = l_Vertices[2].y;
			e_pfVertices[2] = l_Vertices[2].z;

			e_pfVertices[3] = l_Vertices[3].x;			//right down
			e_pfVertices[4] = l_Vertices[3].y;
			e_pfVertices[5] = l_Vertices[3].z;

			e_pfVertices[6] = l_Vertices[0].x;			//left up
			e_pfVertices[7] = l_Vertices[0].y;
			e_pfVertices[8] = l_Vertices[0].z;

			e_pfVertices[9] = l_Vertices[0].x;			//left up
			e_pfVertices[10] = l_Vertices[0].y;
			e_pfVertices[11] = l_Vertices[0].z;

			e_pfVertices[12] = l_Vertices[3].x;			//right down
			e_pfVertices[13] = l_Vertices[3].y;
			e_pfVertices[14] = l_Vertices[3].z;

			e_pfVertices[15] = l_Vertices[1].x;			//right up
			e_pfVertices[16] = l_Vertices[1].y;
			e_pfVertices[17] = l_Vertices[1].z;
		}
		return true;
	}

	bool	cCueToStartCurveWithTime::GetTransformedVerticesByIndex(float*e_pfVertices, float*e_pfUV, float*e_pfColor, int e_iIndex)
	{
		sTexBehaviorDataWithImageIndexData*l_pData = this->GetPointData(e_iIndex);
		sPuzzleData*l_pPuzzleData = l_pData->pPI->GetPuzzleData()[l_pData->iImageIndex];
		Vector3	l_Vertices[4];
		GetRenderPuzzleDataAndMatrix(l_pData, 3, (float*)l_Vertices, e_pfUV, l_pPuzzleData);
		return AssignColorAndTransformToVertices(l_pData->vColor, e_pfVertices, e_pfColor);
	}

	//0    14
	//23	5
	bool	cCueToStartCurveWithTime::GetTransformedTrianglesVertices(float*e_pfVertices, float*e_pfUV, float*e_pfColor, bool e_bForceToFetch)
	{
		if (!e_bForceToFetch)
		{
			if (this->IsAnimationDone())
			{
					return  false;
			}
		}
		if (m_pCurrentPointData->iImageIndex == -1)
			return false;
		if (!m_bStart || m_pCurrentPointData->vColor.a == 0)
			return false;

		sPuzzleData*l_pPuzzleData = this->m_pCurrentPointData->pPI->GetPuzzleData()[this->m_pCurrentPointData->iImageIndex];
		if (e_pfUV)
			AssignUVDataTo2Triangles(l_pPuzzleData->fUV, e_pfUV, m_pCurrentPointData->bMirror);
		AssignColorAndTransformToVertices(m_pCurrentPointData->vColor, e_pfVertices, e_pfColor);
		return true;
	}

	bool	cCueToStartCurveWithTime::IsUsingPuzzleImage(cPuzzleImage*e_pPI)
	{
		size_t	l_uiSize = this->m_PointDataList.size();
		for (size_t i = 0; i<l_uiSize; ++i)
		{
			if (e_pPI == m_PointDataList[i]->pPI)
				return true;
		}
		return false;
	}

	void cCueToStartCurveWithTime::Render(sPuzzleData*e_pPuzzleData)
	{
		if (!this->IsAnimationDone())
		{
			if (!m_bStart)
				return;
			this->m_pCurrentPointData->pPI->ApplyImage();
			RenderPuzzleData(e_pPuzzleData);
		}
	}

	int	cCueToStartCurveWithTime::CalculateCurrentPointIndex()
	{
		if (m_iCurrentPointIndex == -1)//time has changed,it should't happen but for edit is fine
			m_iCurrentPointIndex = 0;
		if (this->m_iLOD == 1)
			return m_iCurrentPointIndex;
		float	l_fCurrentStep = m_iCurrentPointIndex / (float)this->m_FinalTimeList.size();
		l_fCurrentStep *= this->m_OriginalTimeList.size();
#ifdef DEBUG
		if ((unsigned int)l_fCurrentStep >= m_PointDataList.size())
		{
			assert(0 && "call fatming - MDPI.h");
		}
#endif
		return (int)l_fCurrentStep;
	}

	void	cCueToStartCurveWithTime::InternalInit()
	{
		cCurveWithTime::InternalInit();
		this->SetAnimationDone(false);
		m_bHintPoint = false;
		m_iCurrentHintPointDataIndex = -1;
		this->Update(EPSIONAL);
	}
	void	cCueToStartCurveWithTime::InternalUpdate(float e_fElpaseTime)
	{
		//if (this->m_bCurveMoveDone)
			//return;
		cCurveWithTime::InternalUpdate(e_fElpaseTime);
		UpdateData();
		sPuzzleData*l_pPuzzleData = m_pCurrentPointData->pPI->GetPuzzleData()[this->m_pCurrentPointData->iImageIndex];
		GetRenderPuzzleDataAndMatrix(m_pCurrentPointData, 3, (float*)this->m_2DVertices.vPos, (float*)m_2DVertices.fUV, l_pPuzzleData);
	}

	void cCueToStartCurveWithTime::InternalRender()
	{
		if (this->IsAnimationDone())
		{
			if (m_bStayAtLastFrame)
			{
				this->RenderLastFrame();
			}
			return;
		}
		if (!m_pCurrentPointData->pPI)
			return;
		m_pCurrentPointData->pPI->ApplyImage();

		if (m_pCurrentPointData->iImageIndex == -1)
		{
#ifdef DEBUG
			std::wstring	l_str = UT::ComposeMsgByFormat(L"image error,Name:", this->GetName());
			cGameApp::OutputDebugInfoString(l_str.c_str());
#endif
			return;
		}
		if (m_pCurrentPointData->vColor.a == 0)//totaly transparent
			return;
		GLenum	l_OriginalSrc, l_OriginalDest;
		if (this->m_bColorBlending)
		{
			MyGLGetIntegerv(GL_BLEND_SRC, (GLint*)&l_OriginalSrc);
			MyGLGetIntegerv(GL_BLEND_DST, (GLint*)&l_OriginalDest);
			glBlendFunc(m_SrcBlendingMode, m_DestBlendingMode);
		}
		cMatrix44 l_mat = GetConvertedWorldTransformIfParentRequireDoPositionOffsetToCenter();
		DrawQuadWithMatrix((float*)this->m_2DVertices.vPos, (float*)m_2DVertices.fUV, m_pCurrentPointData->vColor, l_mat, 3, 1);
		if (this->m_bColorBlending)
		{
			glBlendFunc(l_OriginalSrc, l_OriginalDest);
		}
	}

	void	cCueToStartCurveWithTime::InvertOrder()
	{
		cCurveWithTime::InvertOrder();
		InvertVectorDateOrder(&m_PointDataList);
	}

	void	cCueToStartCurveWithTime::RenderLastFrame()
	{
		if (m_PointDataList.size() == 0 || !m_pCurrentPointData->pPI)
			return;
		float	l_fLastTime = this->GetEndTime() - EPSIONAL;
		if (fabs(l_fLastTime - m_fPastTime) > 0.f)
		{
			GetPositionByTime(l_fLastTime - EPSIONAL);
		}
		*m_pCurrentPointData = *this->m_PointDataList[m_PointDataList.size() - 1];
		int l_iLastFrameImageIndex = this->m_PointDataList[m_PointDataList.size() - 1]->iImageIndex;
		if (l_iLastFrameImageIndex == -1)
		{
#ifdef DEBUG
			std::wstring	l_str = UT::ComposeMsgByFormat(L"image error,Name:", this->GetName());
			UT::ErrorMsg(l_str.c_str(), L"Error RenderLastFrame");
			//cGameApp::OutputDebugInfoString(l_str.c_str());
#endif
			return;
		}
		sPuzzleData*l_pPuzzleData = m_pCurrentPointData->pPI->GetPuzzleData()[l_iLastFrameImageIndex];
		m_pCurrentPointData->pPI->ApplyImage();
		RenderPuzzleData(l_pPuzzleData);
	}

	void	cCueToStartCurveWithTime::SetAnimationLoop(bool e_bLoop)
	{
		cFMTimeLineAnimationRule::SetAnimationLoop(e_bLoop);
		this->m_bCurveLoop = e_bLoop;
	}

	void	cCueToStartCurveWithTime::RenderByGlobalTime(float e_fTime)
	{
		this->SetCurrentTime(e_fTime);
		int	l_iLastImageIndex = -1;
		UINT	l_uiSize = (UINT)this->GetTimeList().size();
		if (!l_uiSize)
			return;
		float	l_fEndTime = this->GetTimeList()[l_uiSize - 1];
		//ensure start time
		if (this->GetStartTime()>m_fCurrentTime)
			return;
		//it's over start time and end time,it's possible using in the editor ,so it is possible not set end time yet,so do safe way to get right time
		float	l_fStartAndEndTime = this->GetStartTime() + l_fEndTime;
		float	l_fTargetTime = m_fCurrentTime;
		if (m_fCurrentTime > l_fStartAndEndTime)
		{//it's loop so we have to loop it,current time minus it's own play time and play it
			if (!this->IsAnimationLoop())
			{
				return;
			}
			else
			{
				l_fTargetTime = UT::GetFloatModulus(e_fTime - GetStartTime(), l_fEndTime);
				l_fTargetTime += GetStartTime();
			}
		}
		//render by global time so animation won't be done
		this->SetAnimationDone(false);
		GetPositionByTime(l_fTargetTime);
		//l_iLastImageIndex = this->GetCurrentImageIndex();
		l_iLastImageIndex = this->m_pCurrentPointData->iImageIndex;
		if (l_iLastImageIndex != -1)
		{
			if (m_pCurrentPointData->pPI)
			{
				m_pCurrentPointData->pPI->ApplyImage();
				sPuzzleData*l_pPuzzleData = m_pCurrentPointData->pPI->GetPuzzleData()[m_pCurrentPointData->iImageIndex];
				RenderPuzzleData(l_pPuzzleData);
			}
		}
	}

	sHintPoint*	cCueToStartCurveWithTime::GetCurrentHintPointData(int *e_piNumRelated)//it could be none if it doesn't occur any hitpoint
	{
		if (m_bHintPoint)
		{
			if (e_piNumRelated)
			{
				*e_piNumRelated = 1;
				int	l_iHintPointIndex = m_HintPointVector[m_iCurrentHintPointDataIndex].iPointIndex;
				for (UINT i = m_iCurrentHintPointDataIndex + 1; i<m_HintPointVector.size(); ++i)
				{
					if (l_iHintPointIndex == m_HintPointVector[i].iPointIndex)
						*e_piNumRelated = *e_piNumRelated + 1;
				}
			}
			return &m_HintPointVector[m_iCurrentHintPointDataIndex];
		}
		return 0;
	}

	void	cCueToStartCurveWithTime::ChangeAllImageByImageIndex(int e_iIndex, bool e_bScalingImage)
	{
		for (size_t i = 0; i<m_PointDataList.size(); ++i)
		{
			if (e_bScalingImage)
			{
				sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = m_PointDataList[i];
				cPuzzleImageUnit*l_pPIUnuit = (*l_pTexBehaviorDataWithImageIndexData->pPI)[l_pTexBehaviorDataWithImageIndexData->iImageIndex];
				if (l_pPIUnuit)
				{
					float	l_fScaleX = l_pTexBehaviorDataWithImageIndexData->Size.x / (l_pPIUnuit->GetHeight());
					float	l_fScaleY = l_pTexBehaviorDataWithImageIndexData->Size.y / (l_pPIUnuit->GetHeight());
					l_pPIUnuit = (*l_pTexBehaviorDataWithImageIndexData->pPI)[e_iIndex];
					m_PointDataList[i]->Size.x = l_pPIUnuit->GetWidth()*l_fScaleX;
					m_PointDataList[i]->Size.y = l_pPIUnuit->GetHeight()*l_fScaleY;
				}
			}
			m_PointDataList[i]->iImageIndex = e_iIndex;
		}
		if (m_pCurrentPointData)
			this->m_pCurrentPointData->iImageIndex = e_iIndex;
	}

	void	cCueToStartCurveWithTime::ImageRotation(Vector3 e_vAngle)
	{
		if (m_PointDataList.size())
		{
			//float	l_fOffsetAngle = e_fAngle-m_PointDataList[0]->fAngle;
			for (size_t i = 0; i<m_PointDataList.size(); ++i)
			{
				//m_PointDataList[i]->fAngle += l_fOffsetAngle;
				m_PointDataList[i]->vAngle.x = GetFloatModulus(m_PointDataList[i]->vAngle.x + e_vAngle.x, 360);
				m_PointDataList[i]->vAngle.y = GetFloatModulus(m_PointDataList[i]->vAngle.y + e_vAngle.y, 360);
				m_PointDataList[i]->vAngle.z = GetFloatModulus(m_PointDataList[i]->vAngle.z + e_vAngle.z, 360);
			}
		}
	}

	void	cCueToStartCurveWithTime::ImageRelativeRotation(Vector3 e_vAngle)
	{
		if (m_PointDataList.size())
		{
			for (size_t i = 0; i<m_PointDataList.size(); ++i)
			{
				m_PointDataList[i]->vAngle += e_vAngle;
			}
		}
	}

	void	cCueToStartCurveWithTime::RotateCurveWithCurveCenter(Quaternion e_Quaternion, bool e_bRotateImage)
	{
		cCurveWithTime::RotateCurveWithCurveCenter(e_Quaternion.ToMatrix());
		if (e_bRotateImage)
		{
			Vector3 l_vAngle = e_Quaternion.ToEuler();
			Vector3	l_vDegreeAngle(D3DXToDegree(l_vAngle.x), D3DXToDegree(l_vAngle.y), D3DXToDegree(l_vAngle.z));
			ImageRelativeRotation(l_vDegreeAngle);
		}
	}

	void	cCueToStartCurveWithTime::RotateCurveWithSpecificCenterPosition(Vector3 e_vCenterPos, Quaternion e_Quaternion, bool e_bRotateImage)
	{
		cCurveWithTime::RotateCurveWithSpecificCenterPosition(e_vCenterPos, e_Quaternion);
		Vector3 l_vAngle = e_Quaternion.ToEuler();
		if (e_bRotateImage)
		{
			Vector3	l_vDegreeAngle(D3DXToDegree(l_vAngle.x), D3DXToDegree(l_vAngle.y), D3DXToDegree(l_vAngle.z));
			ImageRotation(l_vDegreeAngle);
		}
	}


	void	cCueToStartCurveWithTime::SetScaleForPointData(float e_fScale)
	{
		if (m_PointDataList.size())
		{
			for (size_t i = 0; i<m_PointDataList.size(); ++i)
			{
				m_PointDataList[i]->Size *= e_fScale;
			}
		}
	}

	bool    cCueToStartCurveWithTime::Collide(int e_iPosX, int e_iPosY)
	{
		int l_iTargetIndex = CalculateCurrentPointIndex();
		Vector2 l_vSize = this->m_PointDataList[l_iTargetIndex]->Size;
		Vector3	l_vPos = GetPos();
		return CollideRectWithTransform(e_iPosX, e_iPosY,
			(int)(l_vPos.x),
			(int)(l_vPos.y),
			this->m_PointDataList[l_iTargetIndex]->vAngle,
			(int)l_vSize.x, (int)l_vSize.y);
	}

	bool	cCueToStartCurveWithTime::Collide(Vector4 e_vViewRect)
	{
		if (m_pCurrentPointData)
		{
			Vector3	l_vPos = this->GetPos();
			Vector4	l_vViewRect(l_vPos.x, m_vCurrentPosition.y, l_vPos.x + this->m_pCurrentPointData->Size.x, l_vPos.y + this->m_pCurrentPointData->Size.y);
			return l_vViewRect.Collide(e_vViewRect);
		}
		return false;
	}

	void	cCueToStartCurveWithTime::RenderByCollide(Vector4 e_vViewRect)
	{
		if (Collide(e_vViewRect))
			Render();
	}

	void    cCueToStartCurveWithTime::RenderFirsttFrameImage(bool e_bBlending, Vector4 e_vColor)
	{
		int	l_iLastImageIndex = -1;
		UINT	l_uiSize = (UINT)this->GetTimeList().size();
		if (!l_uiSize)
			return;
		float	l_fEndTime = this->GetTimeList()[l_uiSize - 1];
		//ensure start time
		if (this->GetStartTime()>0.000001f)
			return;
		//it's over start time and end time
		if (this->GetStartTime() + l_fEndTime <= 0.0000001f)
		{//it's loop so we have to loop it,current time minus it's own play time and play it
			if (!this->IsAnimationLoop())
				return;
		}
		GetPositionByTime(0.00000001f);
		Vector4 l_vOriginalColor = m_pCurrentPointData->vColor;
		l_iLastImageIndex = m_pCurrentPointData->iImageIndex;
		if (l_iLastImageIndex != -1)
		{
			if (e_bBlending)
				m_pCurrentPointData->vColor = e_vColor;
			m_pCurrentPointData->pPI->ApplyImage();
			//sPuzzleData*l_pPuzzleData = this->m_ppPuzzleData[this->GetCurrentImageIndex()];
			sPuzzleData*l_pPuzzleData = m_pCurrentPointData->pPI->GetPuzzleData()[l_iLastImageIndex];
			RenderPuzzleData(l_pPuzzleData);
			if (e_bBlending)
				m_pCurrentPointData->vColor = l_vOriginalColor;
		}
	}

	Vector3 cCueToStartCurveWithTime::GetPositionByTime(float e_fGlobalTime)
	{
		if (m_fPastTime != e_fGlobalTime)
		{
			m_fPastTime = e_fGlobalTime;
			float	l_fPastTime = e_fGlobalTime - m_fStartTime;
			if (l_fPastTime >= 0.f)
			{
				this->m_vCurrentPosition = cCurveWithTime::GetPositionByTime(l_fPastTime);
				CalculateCurrentPointIndex();
				UpdateData();
			}
		}
		return m_vCurrentPosition;
	}

	void    cCueToStartCurveWithTime::RenderCollide()
	{
		if (!m_pCurrentPointData || m_pCurrentPointData->iImageIndex == -1)
			return;
		cMatrix44 l_mat = this->GetConvertedWorldTransformIfParentRequireDoPositionOffsetToCenter();
		GLRender::RenderRectangle( m_pCurrentPointData->Size.x, m_pCurrentPointData->Size.y, l_mat, m_pCurrentPointData->vColor);

		//GLRender::RenderRectangle(GetPos() + this->m_vRotationAnglePosOffset, m_pCurrentPointData->Size.x, m_pCurrentPointData->Size.y, m_pCurrentPointData->vColor, m_pCurrentPointData->vAngle);
	}
	//<SubMPDI cMPDIList="" cMPDI="" cSubMPDI=""/>
	cCueToStartCurveWithTime*		cCueToStartCurveWithTime::GetMe(TiXmlElement*e_pElement, bool e_bClone)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pElement, cCueToStartCurveWithTime::TypeID);
		cSubMPDI*l_pSubMPDI = 0;
		cSubMPDI*l_pCloneSubMPDI = 0;
		bool*	l_pbLoop = 0;
		cMPDI*l_pMPDI = 0;
		cMPDIList*l_pMPDIList = 0;
		const float	l_fMagicValue = -79979.f;
		Vector4	l_vColor(l_fMagicValue, l_fMagicValue, l_fMagicValue, l_fMagicValue);
		Vector3	l_vPos(l_fMagicValue, l_fMagicValue, l_fMagicValue);
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME_WITH_DEFINE(cMPDIList::TypeID)
		{
			l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strValue);
		}
			else
				COMPARE_NAME_WITH_DEFINE(cMPDI::TypeID)
			{
			if (l_pMPDIList)
				l_pMPDI = l_pMPDIList->GetObject(l_strValue);
			}
			else
				COMPARE_NAME_WITH_DEFINE(cSubMPDI::TypeID)
			{
			if (l_pMPDI)
				l_pSubMPDI = l_pMPDI->GetObject(l_strValue);
			}
			else
				COMPARE_NAME("Color")
			{
			l_vColor = VALUE_TO_VECTOR4;
			}
			else
				COMPARE_NAME("Pos")
			{
			l_vPos = VALUE_TO_VECTOR3;
			}
			else
				COMPARE_NAME("Loop")
			{
			l_pbLoop = new bool;
			*l_pbLoop = VALUE_TO_BOOLEAN;
			}
			PARSE_NAME_VALUE_END
				if (l_pSubMPDI)
				{
					if (e_bClone)
						l_pCloneSubMPDI = dynamic_cast<cSubMPDI*>(l_pSubMPDI->Clone());
					else
						l_pCloneSubMPDI = l_pSubMPDI;
				}
			if (l_vColor.x != l_fMagicValue ||
				l_vColor.y != l_fMagicValue ||
				l_vColor.z != l_fMagicValue ||
				l_vColor.w != l_fMagicValue)
			{
				l_pCloneSubMPDI->SetColor(l_vColor);
			}
			if (l_vPos.x != l_fMagicValue ||
				l_vPos.y != l_fMagicValue ||
				l_vPos.z != l_fMagicValue)
			{
				l_pCloneSubMPDI->SetPos(l_vPos);
			}
			if (l_pbLoop)
			{
				l_pCloneSubMPDI->SetAnimationLoop(*l_pbLoop);
				SAFE_DELETE(l_pbLoop);
			}
			return l_pCloneSubMPDI;
	}
	//end namespace FATMING_CORE
}