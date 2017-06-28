#include "../stdafx.h"
#include "CurveManager.h"
#ifdef WIN32
#include "../XML/AtgXmlWriter.h"
#endif
#include "../Image/SimplePrimitive.h"
#include "../GLSL/Shader.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cCurveManager);
	const char*cCurveManager::ExtensionNameID( ".path" );
	

	//W axis won't be transited
	Vector3	SwapPosAsAxisView(Vector3 e_vPos,MyMath::AXIS e_Axis)
	{
		switch(e_Axis)
		{
			case MyMath::X:
				e_vPos.x = 0.f;
				break;
			case MyMath::Y:
				e_vPos.y = 0.f;
				break;
			case MyMath::Z:
				e_vPos.z = 0.f;
				break;
		}
		return e_vPos;
	}

	Vector3	RetainPosAxisAsAxisView(Vector3 e_vSrcPos,Vector3 e_vDestSrc,MyMath::AXIS e_Axis)
	{
		switch(e_Axis)
		{
			case MyMath::X:
				e_vDestSrc.x = e_vSrcPos.x;
				break;
			case MyMath::Y:
				e_vDestSrc.y = e_vSrcPos.y;
				break;
			case MyMath::Z:
				e_vDestSrc.z = e_vSrcPos.z;
				break;
		}
		return e_vDestSrc;
	}

	cCurveManager::cCurveManager()
	{
		memset(m_strForCollidedpiFile,0,sizeof(wchar_t)*MAX_PATH);
		m_vRotationCenter.x = m_vRotationCenter.y = 0.f;
		m_ePathMode = ePM_EDIT;
		m_iFocusPoint = -1;
		m_iCurrentSelectedIndex = -1;
		m_pBasemageForShow = 0;
		m_pCurrentCurve = 0;//depend on m_iCurrentSelectedIndex,if m_iCurrentSelectedIndex = -1,null
		m_bBaseImageFromResource = true;
	}

	cCurveManager::~cCurveManager()
	{
		if(!m_bBaseImageFromResource)
			SAFE_DELETE(m_pBasemageForShow);
	}

	//<CurveData Name="79979" Count ="3" LOD="1" />
	void	cCurveManager::ProcessCurveData()
	{
		cCurveWithTime*l_pCurvesWithTime = new cCurveWithTime();
		this->AddObject(l_pCurvesWithTime);
		PARSE_CURRENT_ELEMENT_START
			COMPARE_NAME("Name")
			{
				l_pCurvesWithTime->SetName(l_strValue);
			}
			else
			COMPARE_NAME("Count")
			{
				//atoi(l_strValue);
			}
			else
			COMPARE_NAME("LOD")
			{
				int	l_i = _wtoi(l_strValue);
				l_pCurvesWithTime->SetLOD(l_i);
			}
		PARSE_NAME_VALUE_END
	}
	//<CurvePoint  P1="0,0" P2="10,20" P3="20,20"  />
	void	cCurveManager::ProcessCurvePoint()
	{
		assert(this->GetObject(this->Count()-1) && "data order is error" );
		assert(this->GetObject(this->Count()-1)->GetPointList().size()==0&&"data alread existed ");
		cCurveWithTime*l_pCurvesWithTime = (*this)[Count()-1];
		std::vector< Vector3 >	l_vList;
		PARSE_CURRENT_ELEMENT_START
			Vector3 l_v = GetVector3(l_strValue);
			l_vList.push_back(l_v);
			l_pAttribute = l_pAttribute->Next();
		}
		l_pCurvesWithTime->SetOriginalPointList(l_vList);
	}
	//<CurveTime   />
	void	cCurveManager::ProcessCurveTime()
	{
		assert(this->GetObject(this->Count()-1) && "data order is error" );
		assert(this->GetObject(this->Count()-1)->GetTimeList().size()==0&&"data alread existed ");
		cCurveWithTime*l_pCurvesWithTime = (*this)[Count()-1];
		std::vector< float >	l_fList;
		PARSE_CURRENT_ELEMENT_START
			float	l_f = (float)_wtof(l_strValue);
			l_fList.push_back(l_f);
			l_pAttribute = l_pAttribute->Next();
		}
		l_pCurvesWithTime->SetOriginalTimeList(l_fList);
		l_pCurvesWithTime->DoLOD();
	}

	void	cCurveManager::HandleElementData(TiXmlElement*e_pTiXmlElement)
	{
		const wchar_t*	l_strForElementName = e_pTiXmlElement->Value();
		if( !wcslen(l_strForElementName) )
			return;
		if( !wcscmp(l_strForElementName,L"CurveData") )
			ProcessCurveData();
		else
		if( !wcscmp(l_strForElementName,L"CurvePoint") )
			ProcessCurvePoint();
		else
		if( !wcscmp(l_strForElementName,L"CurveTime") )
			ProcessCurveTime();
		else
		if( !wcscmp(l_strForElementName,L"PIFile") )
			ProcessPIFileName();
	}


	void	cCurveManager::SetCurrentSelectedIndex(int e_iIndex)
	{
		m_iCurrentSelectedIndex = e_iIndex;

		if(e_iIndex>=Count())
			m_iCurrentSelectedIndex = -1;
		if( m_iCurrentSelectedIndex == -1 )
			m_pCurrentCurve = 0;
		else
			m_pCurrentCurve = (*this)[m_iCurrentSelectedIndex];
	}


	void	cCurveManager::Render(Vector4 e_vColor)
	{
		if( m_iCurrentSelectedIndex==-1 )
			return;
		switch(m_ePathMode)
		{
			case ePM_EDIT:
				this->m_iFocusPoint = -1;
				break;
			case ePM_FIX:
				break;
			case ePM_FIX_WITH_MOUSE_DRAG:
				break;
			case ePM_ALL_PATH_ROTATION_DRAG:
			case ePM_ALL_PATH_ROTATION:
			case ePM_PATH_MOVING_ALL:
			case ePM_ALL_PATH_WITH_MOUSE_DRAG:
			{
				int	l_iNum = (int)m_ObjectList.size();
				for( int i=0;i<l_iNum;++i )
				{
					cCurveWithTime*l_pCurvesWithTime = (*this)[i];
					l_pCurvesWithTime->RenderCurve(e_vColor);
					l_pCurvesWithTime->RenderPoint();
				}
			}
				return;
		}
		if( m_iCurrentSelectedIndex>= this->Count() )
			return;
		//GLboolean	l_bEnbleTexture = false;
		//glGetBooleanv(GL_TEXTURE_2D,&l_bEnbleTexture);
		cCurveWithTime*l_pCurvesWithTime = (*this)[m_iCurrentSelectedIndex];
		l_pCurvesWithTime->RenderCurve(e_vColor);
		//if( !l_bEnbleTexture )
		l_pCurvesWithTime->RenderPointIndex();
		//if( !l_bEnbleTexture )
			//glDisable(GL_TEXTURE_2D);
		if( m_iFocusPoint!=-1 )
		{
			Vector3	l_v = l_pCurvesWithTime->GetOriginalPointList()[this->m_iFocusPoint];
			RenderPoint(l_v,5,Vector4(1,0,0,1));
		}
		if( m_ePathMode == ePM_ALL_PATH_ROTATION || m_ePathMode == ePM_ALL_PATH_ROTATION_DRAG )
		{
			RenderPoint(m_vRotationCenter,5,Vector4(1,0,0,1));
		}
	}



	void	cCurveManager::RenderALLCurve()
	{
		int	l_iNum = (int)this->m_ObjectList.size();
		for( int i=0;i<l_iNum;++i )
		{
			cCurveWithTime*l_pCurvesWithTime = (*this)[i];
			float	l_fLERP = (l_iNum-i)/(float)l_iNum;
			float	l_fR = l_fLERP;
			float	l_fG = 1-l_fLERP;
			l_pCurvesWithTime->RenderCurve(Vector4(l_fG,l_fR,1,1));
		}
		for( int i=0;i<l_iNum;++i )
		{
			cCurveWithTime*l_pCurvesWithTime = (*this)[i];
			l_pCurvesWithTime->RenderPointIndex();
		}
	}

	void	cCurveManager::Update(float e_fElpase)
	{
		if( m_iCurrentSelectedIndex==-1 )
			return;
		(*this)[m_iCurrentSelectedIndex]->Update(e_fElpase);
	}

	void	cCurveManager::LoopUpdate(float e_fElpase)
	{
		if( m_iCurrentSelectedIndex==-1 )
			return;
	}

	void	cCurveManager::RenderWithImage()
	{
		if( m_iCurrentSelectedIndex==-1 )
			return;
		if( m_pBasemageForShow )
		{
			Vector3	l_v = m_pCurrentCurve->GetCurrentPosition();
			POINT	l_Pos = {(int)l_v.x,(int)l_v.y};	
			l_Pos.x-=(m_pBasemageForShow->GetWidth()/2);
			l_Pos.y-=(m_pBasemageForShow->GetHeight()/2);
			m_pBasemageForShow->Render(l_Pos.x,l_Pos.y);
		}
	}

	void	cCurveManager::MouseMove(Vector3 e_vPos,MyMath::AXIS e_Axis,float*e_pvProjectionMatrix,float*e_pfViewMatrix,int *e_piViewPort)
	{
		if( this->m_iCurrentSelectedIndex == -1 )
			return;
		e_vPos = SwapPosAsAxisView(e_vPos,e_Axis);
		if( m_ePathMode == ePM_FIX )
		{
			this->m_iFocusPoint = -1;
			Vector3	l_vMousePos = e_vPos;
			std::vector<Vector3> l_vPointList = m_pCurrentCurve->GetOriginalPointList();
			float	l_fClosetDistance = 20;
			int	l_iLength = (int)l_vPointList.size();
			for( int i=0;i<l_iLength;++i )
			{
				Vector3 l_vPos = l_vPointList[i];
				l_vPos = SwapPosAsAxisView(l_vPos,e_Axis);

				if( e_Axis == MyMath::W&&e_pvProjectionMatrix )//try screen space
				{
					//float	l_fProjectionMatrix[16];
					//float	l_fViewMatrix[16];
					//for( int J=0;J<16;++J )
					//{
					//	l_fProjectionMatrix[J] = (double)e_pvProjectionMatrix[J];
					//	l_fViewMatrix[J] = (double)e_pfViewMatrix[J];
					//}
					Vector3	l_vp1 = UT::WorldToScreen(l_vPos.x,l_vPos.y,l_vPos.z,e_pvProjectionMatrix,e_pfViewMatrix,e_piViewPort);
					l_vp1.z = 0.f;
					Vector3	l_vp2 = e_vPos;
					float	l_fDis = (l_vp1-l_vp2).Length();
					if( l_fClosetDistance>l_fDis )
					{
						l_fClosetDistance = l_fDis;
						m_iFocusPoint = i;
					}
				}
				else
				{
					Vector3	l_vDis = l_vPos-l_vMousePos;
					float	l_fDis = l_vDis.Length();
					if( l_fClosetDistance>l_fDis )
					{
						l_fClosetDistance = l_fDis;
						m_iFocusPoint = i;
					}
				}
			}
		}
		else
		if( m_ePathMode == ePM_FIX_WITH_MOUSE_DRAG )
		{
			if( m_pCurrentCurve )
			{
				if( m_iFocusPoint !=-1 )
				{
					Vector3	l_vTargetPos = RetainPosAxisAsAxisView(m_pCurrentCurve->GetOriginalPointList()[m_iFocusPoint],e_vPos,e_Axis);
					m_pCurrentCurve->FixPoint(this->m_iFocusPoint,l_vTargetPos);
				}
			}
		}
		else
		if( m_ePathMode == ePM_PATH_WITH_MOUSE_DRAG )
		{
			if( !m_pCurrentCurve )
				return;
			std::vector<Vector3>	l_PosList = m_pCurrentCurve->GetOriginalPointList();
			int	l_iNum = (int)m_SelectedNodes.size();
			for( int i=0;i<l_iNum;++i )
			{
				int	l_iIndex = m_SelectedNodes[i];
				Vector3	*l_vPos = &l_PosList[l_iIndex];
				l_vPos->x+=(e_vPos.x-m_vMouseClickedPosition.x);
				l_vPos->y+=(e_vPos.y-m_vMouseClickedPosition.y);
			}
			m_vMouseClickedPosition = e_vPos;
			m_pCurrentCurve->SetOriginalPointList(l_PosList);
			m_pCurrentCurve->DoLOD();
		}
		else
		if( m_ePathMode == 	ePM_ALL_PATH_WITH_MOUSE_DRAG )
		{
			size_t	l_uiNumSelected = m_SelectedIndicesToMovingOrRotating.size();//if the selected count is not zero just move the path  are selected
			assert(m_SelectedIndicesToMovingOrRotating.size()<=(UINT)this->Count()&&"forgot to release old data!!");
			if( l_uiNumSelected>0 )
			{
				for( size_t i=0;i<l_uiNumSelected;++i )
				{
					cCurveWithTime*l_pCurvesWithTime = (*this)[m_SelectedIndicesToMovingOrRotating[i]];
					std::vector<Vector3>	l_PosList = l_pCurvesWithTime->GetOriginalPointList();
					int	l_iNumPoint = (int)l_PosList.size();
					for( int j=0;j<l_iNumPoint;++j )
					{
						Vector3	*l_pvPos = &l_PosList[j];
						l_pvPos->x+=(e_vPos.x-m_vMouseClickedPosition.x);
						l_pvPos->y+=(e_vPos.y-m_vMouseClickedPosition.y);
					}
					l_pCurvesWithTime->SetOriginalPointList(l_PosList);
					l_pCurvesWithTime->DoLOD();	
				}
				m_vMouseClickedPosition = e_vPos;
				return;
			}
			int	l_iNum = (int)m_ObjectList.size();
			for( int i=0;i<l_iNum;++i )
			{
				cCurveWithTime*l_pCurvesWithTime = (*this)[i];
				std::vector<Vector3>	l_PosList = l_pCurvesWithTime->GetOriginalPointList();
				int	l_iNumPoint = (int)l_PosList.size();
				for( int j=0;j<l_iNumPoint;++j )
				{
					Vector3	*l_pvPos = &l_PosList[j];
					l_pvPos->x+=(e_vPos.x-m_vMouseClickedPosition.x);
					l_pvPos->y+=(e_vPos.y-m_vMouseClickedPosition.y);
				}
				l_pCurvesWithTime->SetOriginalPointList(l_PosList);
				l_pCurvesWithTime->DoLOD();	
			}
			m_vMouseClickedPosition = e_vPos;
		}
		else
		if( m_ePathMode == ePM_PATH_ROTATION_DRAG )
		{
			if( !m_pCurrentCurve )
				return;
			std::vector<Vector3>	l_PosList = m_pCurrentCurve->GetOriginalPointList();
			float	l_fRotateAngle = GetAngleBy2Point(e_vPos.x,e_vPos.y,m_vRotationCenter.x,m_vRotationCenter.y);
			Vector3	l_vRotationCenter(m_vRotationCenter.x,m_vRotationCenter.y,0.f);
			int	l_iNum = (int)m_SelectedNodes.size();
			for( int i=0;i<l_iNum;++i )
			{
				int	l_iIndex = m_SelectedNodes[i];
				Vector3	*l_pvPos = &l_PosList[l_iIndex];
				Vector3	l_vOriginalPos = m_fOriginalPosForRotatePointAroundRotationCenter[i];
				float	l_Radiun = 180-((l_fRotateAngle) * (D3DX_PI / 180.0f));
 				cMatrix44	l_mat = cMatrix44::ZAxisRotationMatrix(l_Radiun)*cMatrix44::TranslationMatrix(Vector3(l_vOriginalPos.x,l_vOriginalPos.y,0.f));
				Vector3	l_vFinalPos = l_mat.GetAxis(MyMath::W)+l_vRotationCenter;
				l_pvPos->x = l_vFinalPos.x;
				l_pvPos->y = l_vFinalPos.y;
			}
			m_pCurrentCurve->SetOriginalPointList(l_PosList);
			m_pCurrentCurve->DoLOD();
		}
		else
		if( m_ePathMode == ePM_ALL_PATH_ROTATION_DRAG )
		{
			float	l_fRotateAngle = GetAngleBy2Point(e_vPos.x,e_vPos.y,m_vRotationCenter.x,m_vRotationCenter.y);
			Vector3	l_vRotationCenter(m_vRotationCenter.x,m_vRotationCenter.y,0.f);
			size_t	l_uiSelectedIndex = m_SelectedIndicesToMovingOrRotating.size();
			int	l_iNum = (int)m_ObjectList.size();
			int	l_iTotalStep = 0;//to fetch correspond index data
			if( l_uiSelectedIndex>0 )
			{
				for( int i=0;i<l_iNum;++i )
				{
					bool	l_bContain = false;//check if the selectedIndices has been selected,then contain is true,then rotate the angle as wish or skip.
					for( size_t k=0;k<m_SelectedIndicesToMovingOrRotating.size();++k )
					{
						if( m_SelectedIndicesToMovingOrRotating[k] == i )
						{
							l_bContain = true;
							k = m_SelectedIndicesToMovingOrRotating.size();
						}
					}
					cCurveWithTime*l_pCurvesWithTime = (*this)[i];
					std::vector<Vector3>	l_PosList = l_pCurvesWithTime->GetOriginalPointList();
					int	l_iNumPoint = (int)l_PosList.size();
					float	l_Radiun = ((l_fRotateAngle) * (D3DX_PI / 180.0f));
					for( int j=0;j<l_iNumPoint;++j )
					{
						if( l_bContain )
						{
							Vector3	*l_pvPos = &l_PosList[j];
							Vector3	l_vOriginalPos = m_fOriginalPosForRotatePointAroundRotationCenter[l_iTotalStep];
 							cMatrix44	l_mat = cMatrix44::ZAxisRotationMatrix(l_Radiun)*cMatrix44::TranslationMatrix(Vector3(l_vOriginalPos.x,l_vOriginalPos.y,0.f));
							Vector3	l_vFinalPos = l_mat.GetAxis(MyMath::W)+l_vRotationCenter;
							l_pvPos->x = l_vFinalPos.x;
							l_pvPos->y = l_vFinalPos.y;
						}
						++l_iTotalStep;
					}
					if( l_bContain )
					{
						l_pCurvesWithTime->SetOriginalPointList(l_PosList);
						l_pCurvesWithTime->DoLOD();
					}
				}
			}
			else
			{
				for( int i=0;i<l_iNum;++i )
				{
					cCurveWithTime*l_pCurvesWithTime = (*this)[i];
					std::vector<Vector3>	l_PosList = l_pCurvesWithTime->GetOriginalPointList();
					int	l_iNumPoint = (int)l_PosList.size();
					float	l_Radiun = ((l_fRotateAngle) * (D3DX_PI / 180.0f));
					for( int j=0;j<l_iNumPoint;++j )
					{
						Vector3	*l_pvPos = &l_PosList[j];
						Vector3	l_vOriginalPos = m_fOriginalPosForRotatePointAroundRotationCenter[l_iTotalStep];
 						cMatrix44	l_mat = cMatrix44::ZAxisRotationMatrix(l_Radiun)*cMatrix44::TranslationMatrix(Vector3(l_vOriginalPos.x,l_vOriginalPos.y,0.f));
						Vector3	l_vFinalPos = l_mat.GetAxis(MyMath::W)+l_vRotationCenter;
						l_pvPos->x = l_vFinalPos.x;
						l_pvPos->y = l_vFinalPos.y;
						++l_iTotalStep;
					}
					l_pCurvesWithTime->SetOriginalPointList(l_PosList);
					l_pCurvesWithTime->DoLOD();	
				}
			}
			m_vMouseClickedPosition = e_vPos;	
		}
	}

	void	cCurveManager::MouseDown(Vector3 e_vPos,MyMath::AXIS e_Axis,float*e_pvProjectionMatrix,float*e_pfViewMatrix,int *e_piViewPort)
	{
		if( m_ePathMode == ePM_FIX )
		{
			m_ePathMode = ePM_FIX_WITH_MOUSE_DRAG;
		}
		else
		if( m_ePathMode == ePM_PATH_MOVING )
		{
			m_ePathMode = ePM_PATH_WITH_MOUSE_DRAG;
		}
		else
		if( m_ePathMode == ePM_PATH_MOVING_ALL )
		{
			m_ePathMode = ePM_ALL_PATH_WITH_MOUSE_DRAG;
		}
		else
		if( m_ePathMode == ePM_PATH_ROTATION )
		{
			m_ePathMode = ePM_PATH_ROTATION_DRAG;
			std::vector<Vector3>	l_PosList = m_pCurrentCurve->GetOriginalPointList();
			int	l_iNum = (int)m_SelectedNodes.size();
			if( l_iNum )
			{
				m_fOriginalPosForRotatePointAroundRotationCenter.clear();
				FinoutCurrentPointsCenter();
				//assign original distance let point could rotate aound m_vRotationCenter
				int	l_iNum = (int)m_SelectedNodes.size();
				for( int i=0;i<l_iNum;++i )
				{
					int	l_iIndex = m_SelectedNodes[i];
					Vector3	*l_pvPos = &l_PosList[l_iIndex];
					m_fOriginalPosForRotatePointAroundRotationCenter.push_back(m_vRotationCenter-*l_pvPos);
				}
			}
		}
		else
		if( m_ePathMode == ePM_ALL_PATH_ROTATION  )
		{
			m_ePathMode = ePM_ALL_PATH_ROTATION_DRAG;
			m_fOriginalPosForRotatePointAroundRotationCenter.clear();
			FinoutAllPointsCenter();
			if( Count() )
			{
				for(int i=0;i<Count();++i)
				{
					std::vector<Vector3>	l_PosList = (*this)[i]->GetOriginalPointList();
					for( UINT i=0;i<l_PosList.size();++i )
					{
						Vector3	*l_vPos = &l_PosList[i];
						m_fOriginalPosForRotatePointAroundRotationCenter.push_back(m_vRotationCenter-*l_vPos);
					}
				}
			}
		}
		m_vMouseClickedPosition = e_vPos;
	}

	void	cCurveManager::MouseUp(Vector3 e_vPos,MyMath::AXIS e_Axis,float*e_pvProjectionMatrix,float*e_pfViewMatrix,int *e_piViewPort)
	{
		if( m_ePathMode == ePM_FIX_WITH_MOUSE_DRAG)
			m_ePathMode = ePM_FIX ;
		else
		if( m_ePathMode == ePM_PATH_WITH_MOUSE_DRAG)
			m_ePathMode = ePM_PATH_MOVING;
		else
		if( m_ePathMode == ePM_ALL_PATH_WITH_MOUSE_DRAG)
			m_ePathMode = ePM_PATH_MOVING_ALL;
		else
		if( m_ePathMode == ePM_PATH_ROTATION_DRAG )
			m_ePathMode = ePM_PATH_ROTATION;
		else
		if( m_ePathMode == ePM_ALL_PATH_ROTATION_DRAG )
			m_ePathMode = ePM_ALL_PATH_ROTATION;
	}

	void	cCurveManager::FinoutAllPointsCenter()
	{
		if( Count() )
		{
			Vector3	l_veLeftUp = (*this)[0]->GetOriginalPointList()[0];
			Vector3	l_veRightDown = l_veLeftUp;
			for(int i=0;i<Count();++i)
			{
				std::vector<Vector3>	l_PosList = (*this)[i]->GetOriginalPointList();
				for( UINT i=0;i<l_PosList.size();++i )
				{
					Vector3	*l_vPos = &l_PosList[i];
					if( l_veLeftUp.x>l_vPos->x )
						l_veLeftUp.x = l_vPos->x;
					if( l_veLeftUp.y>l_vPos->y )//2d so y up is getting small and small
						l_veLeftUp.y = l_vPos->y;
					if( l_veRightDown.x<l_vPos->x )
						l_veRightDown.x = l_vPos->x;
					if( l_veRightDown.y<l_vPos->y )
						l_veRightDown.y = l_vPos->y;
				}
			}
			m_vRotationCenter = Vector2((l_veLeftUp.x+l_veRightDown.x)/2,(l_veLeftUp.y+l_veRightDown.y)/2);
		}
	}

	void	cCurveManager::FinoutCurrentPointsCenter()
	{
		std::vector<Vector3>	l_PosList = m_pCurrentCurve->GetOriginalPointList();
		int	l_iNum = (int)m_SelectedNodes.size();
		if( l_iNum )
		{
			std::vector<Vector3>	l_PosList = m_pCurrentCurve->GetOriginalPointList();
			Vector3	l_veLeftUp = l_PosList[m_SelectedNodes[0]];
			Vector3	l_veRightDown = l_veLeftUp;
			for( int i=1;i<l_iNum;++i )
			{
				int	l_iIndex = m_SelectedNodes[i];
				Vector3	*l_vPos = &l_PosList[l_iIndex];
				if( l_veLeftUp.x>l_vPos->x )
					l_veLeftUp.x = l_vPos->x;
				if( l_veLeftUp.y>l_vPos->y )//2d so y up is getting small and small
					l_veLeftUp.y = l_vPos->y;
				if( l_veLeftUp.z>l_vPos->z )
					l_veLeftUp.z = l_vPos->z;
				if( l_veRightDown.x<l_vPos->x )
					l_veRightDown.x = l_vPos->x;
				if( l_veRightDown.y<l_vPos->y )
					l_veRightDown.y = l_vPos->y;
				if( l_veRightDown.z<l_vPos->z )
					l_veRightDown.z = l_vPos->z;
			}
			m_vRotationCenter = (l_veLeftUp+l_veRightDown)/2;
		}
	}

	void	cCurveManager::PathMoviing(bool e_bClicked,POINT e_MousePos)
	{
		
	}	

	void	cCurveManager::ProcessPIFileName()
	{
#ifdef ANDROID
		char	l_str[MAX_PATH];
		sprintf(l_str,"%s_mpdi",UT::WcharToChar(this->m_pCurrentTiXmlElement->FirstAttribute()->Value()).c_str());
		CHAR_TO_WCHAR(l_str,m_strForCollidedpiFile);
#else
		swprintf(m_strForCollidedpiFile,MAX_PATH,L"%ls\0",this->m_pCurrentTiXmlElement->FirstAttribute()->Value());
#endif
	}

	void	cCurveManager::DoScale(Vector2 e_vScale)
	{
		if( e_vScale.x == 1.f && e_vScale.y == 1.f )
			return;
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			cCurveWithTime*l_pCurveWithTime = this->GetObject(i);
			std::vector<Vector3>	l_vOriginale = l_pCurveWithTime->GetOriginalPointList();
			int	l_iSize = (int)l_vOriginale.size();
			for( int j=0;j<l_iSize;++j )
			{
				l_vOriginale[j].x *= e_vScale.x;
				l_vOriginale[j].y *= e_vScale.y;
			}
			l_pCurveWithTime->SetOriginalPointList(l_vOriginale);
			l_pCurveWithTime->DoLOD();
		}
		
	}

	void	cCurveManager::ExportData(char*e_strFileName,char*e_strForCollidedpiFile,char*e_pstrAutorData)
	{
#ifdef WIN32
		ATG::XMLWriter*     l_pXMLWriter = new ATG::XMLWriter(e_strFileName);
		char	l_temp[MAX_PATH];
		if( e_pstrAutorData )
		{
			l_pXMLWriter->WriteComment(e_pstrAutorData);
			l_pXMLWriter->WriteComment("T is time,P is Point,Count for how many point,LOD could change detail");
		}
		l_pXMLWriter->StartElement( "PathEditData" );
		l_pXMLWriter->AddAttribute("Version","1.0");		
		if(e_strForCollidedpiFile)
		{
			l_pXMLWriter->StartElement( "PIFile" );
			std::string	l_strForPIFile = e_strForCollidedpiFile;
			l_strForPIFile+=".pi";
			l_pXMLWriter->AddAttribute("FileName",l_strForPIFile.c_str());
			l_pXMLWriter->EndElement();
		}
		for(int i=0;i<this->Count();++i )
		{
			cCurveWithTime*l_pCurvesWithTime = (*this)[i];
			std::vector<Vector3> l_PosList = l_pCurvesWithTime->GetOriginalPointList();
			std::vector<float> l_TimeList = l_pCurvesWithTime->GetOriginalTimeList();
			l_pXMLWriter->StartElement( UT::WcharToChar(cCurveWithTime::TypeID).c_str() );
			l_pXMLWriter->AddAttribute("Name",UT::WcharToChar(l_pCurvesWithTime->GetName()).c_str());
			int	l_iCount = (int)l_TimeList.size();
			l_pXMLWriter->AddAttribute("Count",l_iCount);
			l_pXMLWriter->AddAttribute("LOD",l_pCurvesWithTime->GetLOD());
				l_pXMLWriter->StartElement( "CurvePoint" );
				for( int j=0;j<l_iCount;++j )
				{
					Vector3	l_v = l_PosList[j];
					std::string l_CurvePoins = ValueToString(l_v);
					std::string l_PosID = "P";
					sprintf(l_temp,"%d\0",j);
					l_PosID += l_temp;
					l_pXMLWriter->AddAttribute(l_PosID.c_str(),l_CurvePoins.c_str());
				}
				l_pXMLWriter->EndElement();//end CurvePoint
				l_pXMLWriter->StartElement( "CurveTime" );
				char	l_strTime[MAX_PATH];
				for( int j=0;j<l_iCount;++j )
				{
					sprintf(l_strTime,"%.3f\0",l_TimeList[j]);
					std::string l_CurvePoins = l_strTime;
					std::string l_TimeID = "T";
					sprintf(l_temp,"%d\0",j);
					l_TimeID += l_temp;
					l_pXMLWriter->AddAttribute(l_TimeID.c_str(),l_CurvePoins.c_str());
				}
				l_pXMLWriter->EndElement();//end CurveTime
			l_pXMLWriter->EndElement();//end CurveData
		}
		l_pXMLWriter->EndElement();//end PathEditData
		l_pXMLWriter->Close();
		delete l_pXMLWriter;
#endif
	}
}