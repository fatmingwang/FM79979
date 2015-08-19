#include "stdafx.h"
#include "FishingRodCamera.h"

cFishingRodCamera::cFishingRodCamera(TiXmlElement*e_pTiXmlElement)
{
	m_eFishRodStatus = eFRS_NONE;
	m_fCurrentPos = 0.f;
	m_fTotalDistance = 1000.f;
	m_SinkingTime.SetTargetTime(50.f);
	m_FloatingTime.SetTargetTime(30.f);
	m_pOrthogonalCamera = 0;
	m_vStartPos = Vector2::Zero;
	if( e_pTiXmlElement )
	{
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("SinkingTime")
			{
				m_SinkingTime.SetTargetTime(VALUE_TO_FLOAT);
			}
			else
			COMPARE_NAME("FloatTime")
			{
				m_FloatingTime.SetTargetTime(VALUE_TO_FLOAT);
			}
			else
			COMPARE_NAME("CameraViewPort")
			{
				Vector2	l_vViewPort = VALUE_TO_VECTOR2;
				m_pOrthogonalCamera = new cOrthogonalCamera(l_vViewPort);
			}
			else
			COMPARE_NAME("StartPos")
			{
				m_vStartPos = VALUE_TO_VECTOR2;
				if( m_pOrthogonalCamera )
					m_pOrthogonalCamera->SetCameraPos(m_vStartPos);
			}
		PARSE_NAME_VALUE_END
	}
}

cFishingRodCamera::~cFishingRodCamera()
{
	SAFE_DELETE(m_pOrthogonalCamera);
}

void	cFishingRodCamera::Init()
{
	m_SinkingTime.Start();
	m_FloatingTime.Start();
	m_eFishRodStatus = eFRS_SINKING;
	if( m_pOrthogonalCamera )
		m_pOrthogonalCamera->SetCameraPos(m_vStartPos);
}

void	cFishingRodCamera::Update(float e_fElpaseTime)
{
	if( m_eFishRodStatus == eFRS_SINKING )
	{
		m_SinkingTime.Update(e_fElpaseTime);
		float	l_fLERP = m_SinkingTime.GetLERP();
		m_fCurrentPos = l_fLERP*m_fTotalDistance;
		if( l_fLERP >= 1.f )
			m_eFishRodStatus = eFRS_FLOATING;
	}
	else
	if( m_eFishRodStatus == eFRS_FLOATING )
	{
		m_FloatingTime.Update(e_fElpaseTime);
		float	l_fLERP = m_FloatingTime.GetLERP();
		m_fCurrentPos = (1-l_fLERP)*m_fTotalDistance;
		if( l_fLERP >= 1.f )
			m_eFishRodStatus = eFRS_DONE;
	}
	if( m_pOrthogonalCamera )
		this->m_pOrthogonalCamera->SetCameraPos(Vector2(0,m_fCurrentPos));
}

void	cFishingRodCamera::Render()
{
	if( !m_pOrthogonalCamera )
		return;
	m_pOrthogonalCamera->Render(m_pOrthogonalCamera->GetCameraPos());
}

void	cFishingRodCamera::DebugRender()
{
	UseShaderProgram(DEFAULT_SHADER);
	m_pOrthogonalCamera->Render(m_pOrthogonalCamera->GetCameraPos());
	int	l_iGap = (int)(m_pOrthogonalCamera->GetViewRect().Height()/100)+2;
	for( int i=0;i<l_iGap;++i )
	{
		int	l_iYPos = (int)(this->m_fCurrentPos+i*100)/100*100;
		cGameApp::RenderFont(0,l_iYPos,ValueToStringW(l_iYPos));
	}
	cGameApp::RenderFont(100,(int)m_fCurrentPos+200,ValueToStringW(m_fCurrentPos));	

	if( m_eFishRodStatus == eFRS_SINKING )
	{
		cGameApp::RenderFont(100,(int)m_fCurrentPos+250,L"sinking");	
		cGameApp::RenderFont(100,(int)m_fCurrentPos+300,ValueToStringW(m_SinkingTime.fRestTime));	
	}
	else
	{
		cGameApp::RenderFont(100,(int)m_fCurrentPos+250,L"floating");	
		cGameApp::RenderFont(100,(int)m_fCurrentPos+300,ValueToStringW(m_FloatingTime.fRestTime));	
	}
}

void	cFishingRodCamera::ChangeSinkAndFloatTime(float e_fSink,float e_fFloat)
{
	m_SinkingTime.SetTargetTime(e_fSink);
	m_FloatingTime.SetTargetTime(e_fFloat);
}

Vector4	cFishingRodCamera::GetViewRect()
{
	Vector4	l_v = this->m_pOrthogonalCamera->GetViewRect();
	l_v.y += m_fCurrentPos;
	l_v.w += m_fCurrentPos;
	return l_v;
}

void	cFishingRodCamera::SetStatus(eFishRodStatus e_eFishRodStatus)
{
	if( e_eFishRodStatus == eFRS_SINKING )
	{
		m_SinkingTime.Start();
		m_FloatingTime.Start();
		m_eFishRodStatus = e_eFishRodStatus;
	}
	else	
	if( e_eFishRodStatus == eFRS_FLOATING )
	{
		m_eFishRodStatus = e_eFishRodStatus;
		float	l_fLerp = 1-(m_fCurrentPos/m_fTotalDistance);
		m_FloatingTime.Start(l_fLerp*m_FloatingTime.fTargetTime);
	}
}