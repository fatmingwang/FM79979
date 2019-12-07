#include "stdafx.h"
#include "FishRodAndFishThreadData.h"
#include "FishingRodCamera.h"
#include "FishBehavior.h"
#include "StageData.h"
#include "FishShowProbability.h"
#include "GameApp.h"
//<Hook>
//	<AxisController MinValue="2.5" NextLevelStep="3" RiseSpeed="1.5" />
//	<Collision />
//	<cMPDI />
//</Hook>
//
cHook::cHook(TiXmlElement*e_pElement)
{
	m_fAxisMinValue = 2.5f;
	m_fNextLevelStep = 3.f;
	m_fRiseSpeed = 1.5f;
	m_fMovingSpeed = 5.f;
	m_vMovingPos = Vector3::Zero;
	m_pbtShapeCollision = 0;
	m_pHookImage = 0;
	e_pElement = e_pElement->FirstChildElement();
	while( e_pElement )
	{
		const WCHAR*l_strValue = e_pElement->Value();
		COMPARE_VALUE("cbtShapeCollision")
		{
			m_pbtShapeCollision = cbtShapeCollision::GetMe(e_pElement);
		}
		else
		COMPARE_VALUE("Image")
		{
			TiXmlElement*l_pElement = e_pElement->FirstChildElement();
			m_pHookImage = GetRenderObjectBehavior(l_pElement);
		}
		else
		COMPARE_VALUE("AxisController")
		{
			m_fAxisMinValue = GetFloat(e_pElement->Attribute(L"MinValue"));
			m_fNextLevelStep = GetFloat(e_pElement->Attribute(L"NextLevelStep"));
			m_fRiseSpeed = GetFloat(e_pElement->Attribute(L"RiseSpeed"));
			m_fMovingSpeed = GetFloat(e_pElement->Attribute(L"MovingSpeed"));
		}
		e_pElement = e_pElement->NextSiblingElement();
	}
}

cHook::~cHook()
{
	SAFE_DELETE(m_pHookImage);
	m_pbtShapeCollision = 0;
}


void	cHook::Update(float e_fElpaseTime,cOrthogonalCamera*e_pOrthogonalCamera)
{
#ifdef WIN32
	if( cGameApp::m_sucKeyData[37] )
	{
		m_vMovingPos.x -= m_fMovingSpeed;
		if( m_vMovingPos.x < e_pOrthogonalCamera->GetViewRect().x )
			m_vMovingPos.x = e_pOrthogonalCamera->GetViewRect().x;
	}
	else
	if( cGameApp::m_sucKeyData[39] )
	{
		float	l_fOffset = 0.f;
		if(m_pHookImage)
		{
			cBaseImage*l_pImage = (cBaseImage*)m_pHookImage;
			l_fOffset = l_pImage->GetWidth();
		}
		m_vMovingPos.x += m_fMovingSpeed;
		if( m_vMovingPos.x+l_fOffset > e_pOrthogonalCamera->GetViewRect().z )
			m_vMovingPos.x -= m_fMovingSpeed;
	}
#else
	if( abs(cGameApp::m_svAccelerometer.x) > m_fAxisMinValue )
	{
		int l_iStrideLevel = (int)((abs(cGameApp::m_svAccelerometer.x)-m_fAxisMinValue)/m_fNextLevelStep);
		float l_fMoveSpeed = m_fMovingSpeed;
		if( l_iStrideLevel > 0 )
			l_fMoveSpeed *= (l_iStrideLevel*m_fRiseSpeed);
		if( cGameApp::m_svAccelerometer.x > m_fAxisMinValue )//left
		{
			m_vMovingPos.x -= l_fMoveSpeed;
			if( m_vMovingPos.x < e_pOrthogonalCamera->GetViewRect().x )
				m_vMovingPos.x = e_pOrthogonalCamera->GetViewRect().x;
		}
		else
		if( cGameApp::m_svAccelerometer.x < -m_fAxisMinValue )//right
		{
			float	l_fOffset = 0.f;
			if(m_pHookImage)
			{
				cBaseImage*l_pImage = (cBaseImage*)m_pHookImage;
				l_fOffset = (float)l_pImage->GetWidth();
			}
			m_vMovingPos.x += l_fMoveSpeed;
			if( m_vMovingPos.x+l_fOffset > e_pOrthogonalCamera->GetViewRect().z )
				m_vMovingPos.x -= l_fMoveSpeed;
		}
	}

#endif
	if(m_pHookImage)
	{
		m_pHookImage->Update(e_fElpaseTime);
		m_pHookImage->SetPos(e_pOrthogonalCamera->GetCameraPos()+m_vMovingPos);
		if( m_pbtShapeCollision )
		{
			m_pbtShapeCollision->SetTransform(cMatrix44::TranslationMatrix(m_pHookImage->GetPos()));
		}
	}
}

void	cHook::Render()
{
	if(m_pHookImage)
	{
		m_pHookImage->Render();
	}
}

void	cHook::DebugRender()
{
	if( m_pbtShapeCollision )
	{
		m_pbtShapeCollision->DebugRender();
	}
}

Frame*	cHook::GetFrame()
{
	if(m_pHookImage)
	{
		return m_pHookImage;
	}
	return 0;
}

cBrakeImage::cBrakeImage(TiXmlElement*e_pImageButtonXML,float e_fPowerTime,float e_fBrakeSpeed)
{
	m_pButton = 0;
	TiXmlElement*l_pButtonElement = e_pImageButtonXML->FirstChildElement();
	if( l_pButtonElement )
		m_pButton = dynamic_cast<cImageButton*>(GetClickMouseBehavior(l_pButtonElement));
	m_fBrakeSpeed = e_fBrakeSpeed;
	m_PowerTimeTC.SetTargetTime(e_fPowerTime);
}

cBrakeImage::~cBrakeImage()
{
	SAFE_DELETE(m_pButton);
}

void	cBrakeImage::Update(float e_fElpaseTime)
{
	m_bWorking = false;
	if(!m_pButton || m_PowerTimeTC.bTragetTimrReached )
		return;
#ifdef	WIN32
	if(cGameApp::m_sbTouched && m_pButton->Collide(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y))
	{
		m_bWorking = true;
	}
#else
	std::vector<int>l_WorkingIndex = cGameApp::m_spMultiTouchPoints->TouchPointWorkingIndex;
	for(size_t i=0;i<l_WorkingIndex.size();++i)
	{
		if(cGameApp::m_spMultiTouchPoints->TouchPointData[i].bTouched)
		{
			POINT	l_Pos = cGameApp::m_spMultiTouchPoints->TouchPointData[i].Pos;
			if(m_pButton->Collide(l_Pos.x,l_Pos.y))
			{
				m_bWorking = true;
			}
		}
	}
#endif
	if( m_bWorking )
		m_PowerTimeTC.Update(e_fElpaseTime);
}

void	cBrakeImage::Render()
{
	if(m_pButton)
	{
		Vector3	l_vPos = m_pButton->GetPos();		
		float	l_fRestTime = m_PowerTimeTC.fRestTime;
		if( l_fRestTime < 0.f )
			l_fRestTime = 0.f;
		cGameApp::RenderFont(l_vPos.x,l_vPos.y-30,ValueToStringW(l_fRestTime));
		m_pButton->Render();
	}
}

cStageData::cStageData(cFishRodData*e_pFishRodData,cFishThread*e_pFishThread)
{
	//m_pBGMPDI = 0;
	m_pFishDataButton = 0;
	m_bActiveRenderInfo = false;
	m_pUVAnimationImage = 0;
	m_fQuickLeaveSpeed = 0.5f;
	m_pQuickLeaveButton = 0;
	m_bQuickLeave = false;
	m_pBrakeImage = 0;
	m_pFishShowProbability = 0;
	m_bActiveRenderInfo = false;
	m_pFishingRodCamera = 0;
	m_pFishBehaviorManager = 0;
	m_pFishRodData = e_pFishRodData;
	m_pFishThread = e_pFishThread;
	m_pHook = 0;
	if( m_pFishRodData == 0 )
	{
		m_pFishRodData = cFishingFunApp::m_spFishRodAndFishThreadData->m_FishRodDataVector[0];
	}
	if( m_pFishThread == 0 )
	{
		m_pFishThread = cFishingFunApp::m_spFishRodAndFishThreadData->m_FishThreadVector[0];
	}
	//m_pFishingRodCamera = new cFishingRodCamera();
}

cStageData::~cStageData()
{
	this->Destroy();
}

void	cStageData::Destroy()
{
	SAFE_DELETE(m_pFishDataButton);
	SAFE_DELETE(m_pUVAnimationImage);
	//SAFE_DELETE(m_pBGMPDI);
	SAFE_DELETE(m_pQuickLeaveButton);
	SAFE_DELETE(m_pBrakeImage);
	SAFE_DELETE(m_pFishShowProbability);
	SAFE_DELETE(m_pHook);
	//SAFE_DELETE(m_pFishRodData);
	//SAFE_DELETE(m_pFishThread);
	SAFE_DELETE(m_pFishBehaviorManager);
	SAFE_DELETE(m_pFishingRodCamera);
}
//<StageData>
//	<FishThread Data="FishThread1,FishThread2" />
//	<FishingHook>
//		<Collision File="Image/FishingTool.collsision" Name="FishHook"/>
//		<Image>
//			<cPuzzleImageUnit PI="Image/FishingTool.pi" PIUnit="FishHook"/>
//		</Image>
//	</FishingHook>
//	<CameraData SinkingTime="50" FloatTime="25" CameraViewPort="720,1280" StartPos="360,640"/>
//	<ShowFish TotalCount="">
//		<Data FileName="" Probability="">
//		<Data FileName="" Probability="">
//	</ShowFish>
//	<Image>
//	</Image>
//</StageData>

void	cStageData::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("FishingHook")
	{
		SAFE_DELETE(m_pHook);
		this->m_pHook = new cHook(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("FishThread")
	{
		m_strSelectableFishThreadNameVector.clear();
		m_strSelectableFishThreadNameVector = GetWStringListByCommaDivide(l_strValue);
	}
	else
	COMPARE_VALUE("CameraData")
	{
		SAFE_DELETE(m_pFishingRodCamera);
		m_pFishingRodCamera = new cFishingRodCamera(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("BG")
	{
		cMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),false);
		//SAFE_DELETE(m_pBGMPDI);
		//m_pBGMPDI = cMPDI::GetMe(e_pTiXmlElement->FirstChildElement(),true);
		//m_pBGMPDI->Init();
	}
	else
	COMPARE_VALUE("Image")
	{
	}
	else
	COMPARE_VALUE("FishesShowProbability")
	{
		if( m_pFishShowProbability )
			m_pFishShowProbability->ProcessFishesShowProbabilityData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("AreaData")
	{
		if( m_pFishShowProbability )
			m_pFishShowProbability->ProcessAreaData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("BrakeButton")
	{
		float	l_fPowerTime = 10.f;
		float	l_fSlowSpeed = 0.9f;
		if( m_pFishRodData )
		{
			cFishRodData::sLVData*l_pLVData = m_pFishRodData->GetCurrentLVData();
			l_fPowerTime = l_pLVData->fPowerTime;
			l_fSlowSpeed = l_pLVData->fBrakeSpeed;
		}
		SAFE_DELETE(m_pBrakeImage);
		m_pBrakeImage = new cBrakeImage(e_pTiXmlElement,l_fPowerTime,l_fSlowSpeed);
	}
	else
	COMPARE_VALUE("QuickLeaveButton")
	{
		m_pQuickLeaveButton = dynamic_cast<cImageButton*>(GetClickMouseBehavior(e_pTiXmlElement->FirstChildElement()));
		m_pQuickLeaveButton->Init();
		m_fQuickLeaveSpeed = ElementToFloat(e_pTiXmlElement,L"Speed");
	}
	else
	COMPARE_VALUE("FishDataButton")
	{
		assert(m_pFishDataButton == 0);
		m_pFishDataButton = dynamic_cast<cImageButton*>(GetClickMouseBehavior(e_pTiXmlElement->FirstChildElement()));
		m_pFishDataButton->Init();
	}
}

void	cStageData::Init(const char*e_strStageFileName)
{
	m_bQuickLeave = false;
	m_strSelectableFishThreadNameVector.clear();
	m_strStageFileName = e_strStageFileName;
	Destroy();
	m_pFishBehaviorManager = new cFishBehaviorManager();
	m_pFishShowProbability = new cFishShowProbability(m_pFishBehaviorManager);
	PARSE_FAILED_MESSAGE_BOX(this,e_strStageFileName);
	if( m_pFishingRodCamera )
		m_pFishShowProbability->AssignFishShowData(Vector2(m_pFishingRodCamera->m_pOrthogonalCamera->GetViewRect().Width(),m_pFishThread->GetTotalDepth()));
	else
		m_pFishShowProbability->AssignFishShowData(Vector2(1920,1080));
	if(m_pFishingRodCamera)
	{
		m_pFishingRodCamera->Init();
		m_pFishingRodCamera->SetTotalDistance(m_pFishThread->GetTotalDepth());
		m_pFishingRodCamera->ChangeSinkAndFloatTime(m_pFishThread->m_fSinkTime,m_pFishThread->m_fFloatTime);
	}
	cPuzzleImage*l_pPI = cGameApp::GetPuzzleImage(L"Scene_Bg_Vestige");
	cPuzzleImageUnit*l_pPIUnit = l_pPI->GetObject(L"Vestige_Bg_Limit");
	m_pUVAnimationImage = new cUVAnimationImage(l_pPIUnit,Vector2(720,1280),eMD_DOWN_TO_UP,Vector2::Zero);
}

void	cStageData::Update(float e_fElpaseTime)
{
#ifdef DEBUG
	if( cGameApp::m_sucKeyData[17] || cGameApp::m_spMultiTouchPoints->TouchPointWorkingIndex.size() == 6 )
	{
		Init(m_strStageFileName.c_str());
	}
#endif
	float	l_fCameraUpdateSpeed = 1.f;
	if( m_pQuickLeaveButton && !m_bQuickLeave )
	{
		m_pQuickLeaveButton->Update(e_fElpaseTime);
		if(m_pQuickLeaveButton->IsSatisfiedCondition())
		{
			if( cFishingFunApp::m_spPlayerData->ReelMinus(1) )
			{
				m_bQuickLeave = true;
				m_pFishingRodCamera->SetStatus(eFRS_FLOATING);
			}
			else
				m_pQuickLeaveButton->Init();
		}
	}
	if( m_pFishDataButton )
	{
		m_pFishDataButton->Update(e_fElpaseTime);
		if( m_pFishDataButton->IsSatisfiedCondition() )
		{
			m_bActiveRenderInfo = !m_bActiveRenderInfo;
			m_pFishDataButton->Init();
		}
	}
	if(this->m_pBrakeImage && !m_bQuickLeave)
	{
		m_pBrakeImage->Update(e_fElpaseTime);
		if(m_pBrakeImage->IsWorking())
			l_fCameraUpdateSpeed = m_pBrakeImage->GetBrakeSpeed();
	}
	if( m_pFishBehaviorManager && m_pFishingRodCamera )
	{
		if( m_bQuickLeave )
			l_fCameraUpdateSpeed = m_fQuickLeaveSpeed;
		m_pFishingRodCamera->Update(e_fElpaseTime*l_fCameraUpdateSpeed);
		Vector4	l_vCameraViewRect = m_pFishingRodCamera->m_pOrthogonalCamera->GetViewRect();
		m_pFishBehaviorManager->Update(e_fElpaseTime,m_pFishingRodCamera->GetViewRect(),m_pFishingRodCamera->IsSinking()?true:false);
		if(m_pHook )
		{
			m_pHook->Update(e_fElpaseTime,m_pFishingRodCamera->m_pOrthogonalCamera);
			if( !m_bQuickLeave )
				m_pFishBehaviorManager->DoRodCollide(m_pHook->m_pbtShapeCollision,m_pHook->GetFrame(),this->m_pFishRodData,m_pFishingRodCamera->GetRestTimeToTop(),m_pFishingRodCamera->m_pOrthogonalCamera,this->m_pFishRodData->GetCurrentLVData()->iFishLeaveCost);
			if( m_pFishBehaviorManager )
			{
				if(m_pFishBehaviorManager->m_BeenHookedFishes.Count() && m_pFishingRodCamera->GetStatus() != eFRS_FLOATING )
				{
						if( m_pFishingRodCamera->m_eFishRodStatus == eFRS_SINKING )
						{
								m_pFishingRodCamera->SetStatus(eFRS_FLOATING);
						}
				}
			}
		}
	}
}

void	cStageData::Render()
{
	UseShaderProgram(DEFAULT_SHADER);
	glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x*2,cGameApp::m_spOpenGLRender->m_vGameResolution.y*2);

		cMatrix44	l_matProjection;
		//glhOrthof2((float*)l_matProjection.m, -cGameApp::m_spOpenGLRender->m_vGameResolution.x/2, cGameApp::m_spOpenGLRender->m_vGameResolution.x+cGameApp::m_spOpenGLRender->m_vGameResolution.x/2, -cGameApp::m_spOpenGLRender->m_vGameResolution.y/2, cGameApp::m_spOpenGLRender->m_vGameResolution.y+cGameApp::m_spOpenGLRender->m_vGameResolution.y/2, -10000, 10000);
		//FATMING_CORE::SetupShaderViewProjectionMatrix(l_matProjection,true);
		glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y);

	if( m_pQuickLeaveButton )
	{
		if( m_pUVAnimationImage )
			m_pUVAnimationImage->RenderByPos(m_pFishingRodCamera->m_pOrthogonalCamera->GetCameraPos());
		m_pQuickLeaveButton->Render();
	}
	if( m_pFishDataButton )
	{
		m_pFishDataButton->Render();
	}
	if(m_pHook )
	{
		if( m_pFishingRodCamera )
		{
			UseShaderProgram(DEFAULT_SHADER);
			m_pFishingRodCamera->Render();
		}
		if( m_pHook )
			m_pHook->Render();
	}
	if( m_pFishBehaviorManager && m_pFishingRodCamera )
	{
		if( m_pFishingRodCamera )
		{
			UseShaderProgram(SKINNING_MESH_SHADER);
			m_pFishingRodCamera->Render();
		}
		if( m_pFishBehaviorManager )
		{
			m_pFishBehaviorManager->Render();
			if(this->m_bActiveRenderInfo)
				m_pFishBehaviorManager->RenderData();
		}
	}
	if(this->m_pBrakeImage)
	{
		UseShaderProgram();
		glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y);
		m_pBrakeImage->Render();
	}
}

void	cStageData::DebugRender()
{
	if(this->m_pFishBehaviorManager)
	{
		UseShaderProgram(NO_TEXTURE_SHADER);
		glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y);
		if( m_pUVAnimationImage )
			m_pUVAnimationImage->DebugRender();
		if( m_pFishingRodCamera )
		{
			m_pFishingRodCamera->Render();
			m_pFishingRodCamera->DebugRender();
		}
		if( m_pFishBehaviorManager )
			m_pFishBehaviorManager->DebugRender();
		if( m_pHook )
			this->m_pHook->DebugRender();
	}
}

void	cStageData::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_pQuickLeaveButton )
	{
		m_pQuickLeaveButton->MouseDown(e_iPosX,e_iPosY);
	}
	if( m_pFishDataButton )
	{
		m_pFishDataButton->MouseDown(e_iPosX,e_iPosY);
	}
}

void	cStageData::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_pQuickLeaveButton )
	{
		m_pQuickLeaveButton->MouseMove(e_iPosX,e_iPosY);
	}
	if( m_pFishDataButton )
	{
		m_pFishDataButton->MouseMove(e_iPosX,e_iPosY);
	}
}

void	cStageData::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_pQuickLeaveButton )
	{
		m_pQuickLeaveButton->MouseUp(e_iPosX,e_iPosY);
	}
	if( m_pFishDataButton )
	{
		m_pFishDataButton->MouseUp(e_iPosX,e_iPosY);
	}
}

bool	cStageData::IsStageClear()
{
	if( m_pFishingRodCamera )
		return m_pFishingRodCamera->m_eFishRodStatus == eFRS_DONE?true:false;
	return false;
}

cNamedTypedObjectVector<cFishBehavior>*cStageData::GetBeenHookedFishes()
{
	return &this->m_pFishBehaviorManager->m_BeenHookedFishes;
}