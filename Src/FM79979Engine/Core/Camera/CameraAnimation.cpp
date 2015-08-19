#include "../stdafx.h"
#include "../Math/LinerTemplateDataProcess.h"
#include "CameraAnimation.h"
#include "../GameplayUT/GameApp.h"

namespace FATMING_CORE
{
	const wchar_t*     cCameraAnimation::TypeID( L"cCameraAnimation" );
	cCameraAnimation::cCameraAnimation()
	{
		m_eProjectionType = Projection::Orthographic;
		m_bSetRenderStatusBack = true;
		//m_pViewPortAnimationData = new cLinerTemplateDataProcess<Vector4>;
		m_pViewWidthHeightLinerDataProcessor = new cLinerDataProcessor<Vector2>;
		m_pAngleLinerDataProcessor = new cLinerDataProcessor<Vector3>;
		m_pPosLinerDataProcessor = new cLinerDataProcessor<Vector3>;
		m_pContainer = new cLinerDataContainer();
		m_pContainer->AddData(m_pViewWidthHeightLinerDataProcessor);
		m_pContainer->AddData(m_pAngleLinerDataProcessor);
		m_pContainer->AddData(m_pPosLinerDataProcessor);
		//m_pContainer->AddData(m_pViewPortAnimationData);
		//m_bViewPortSet = false;
		m_bViewProjectionMatrixSet = false;
		m_vCameraViewSize = cGameApp::m_svGameResolution;
		m_vCameraPos = Vector3::Zero;
		m_vCameraAngle = Vector3::Zero;
	}
	//<cCameraAnimation CameraType="2">
	//	<GameResolution>
	//		<Time Data="0,1" />
	//		<AnimationData Type="Vector4" Data0="" Data1="" />
	//	/<GameResolution>
	//	<AngleAnimationData>
	//		<Time Data="0,1" />
	//		<AnimationData Type="Vector3" Data0="" Data1="" />
	//	</AngleAnimationData>
	//</cCameraAnimation>
	cCameraAnimation::cCameraAnimation(TiXmlElement*e_pTiXmlElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cCameraAnimation::TypeID);
		const wchar_t*l_strName = e_pTiXmlElement->Attribute(L"Name");
		if( l_strName )
			this->SetName(l_strName);
		e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		while( e_pTiXmlElement )
		{
			const wchar_t*l_strValue = e_pTiXmlElement->Value();
			COMPARE_VALUE("GameResolution")
			{
				ProcessGameResolution(e_pTiXmlElement);	
			}
			else
			COMPARE_VALUE("AngleAnimationData")
			{
				//ProcessAngle(e_pTiXmlElement);	
			}
			e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
		}
	}

	cCameraAnimation::cCameraAnimation(cCameraAnimation*e_p2DCameraAnimation)
	{
	
	}

	cCameraAnimation::~cCameraAnimation()
	{
		//SAFE_DELETE(m_pViewPortAnimationData);
		//SAFE_DELETE(m_pPosLinerDataProcessor);
		//SAFE_DELETE(m_pViewWidthHeightLinerDataProcessor);
		//SAFE_DELETE(m_pAngleLinerDataProcessor);
		SAFE_DELETE(m_pContainer);
	}

	TiXmlElement*	cCameraAnimation::Export()
	{
		std::vector<Vector2>* l_pViewWidthHeightAnimationData = m_pViewWidthHeightLinerDataProcessor->GetLinerDataVector();
		std::vector<Vector3>* l_pPosAnimationData = m_pPosLinerDataProcessor->GetLinerDataVector();
		std::vector<Vector3>* l_pAngleAnimationData = m_pAngleLinerDataProcessor->GetLinerDataVector();

		int	l_iViewWidthHeightAnimationDataSize = (int)l_pViewWidthHeightAnimationData->size();
		int	l_iPosAnimationDataSize = (int)l_pPosAnimationData->size();
		int	l_iAngleAnimationDataSize = (int)l_pAngleAnimationData->size();

		if( l_iAngleAnimationDataSize == 0 &&
			l_iPosAnimationDataSize == 0 &&
			l_iViewWidthHeightAnimationDataSize == 0)
		{//no data
			return 0;
		}
		TiXmlElement*l_pTiXmlElement = new TiXmlElement(cCameraAnimation::TypeID);
		l_pTiXmlElement->SetAttribute(L"CameraType",m_eProjectionType == Projection::Orthographic?2:3);
		if(m_bSetRenderStatusBack)
		{
			l_pTiXmlElement->SetAttribute(L"SetRenderStatusBack",L"1");
		}
		if( l_iViewWidthHeightAnimationDataSize > 0 )
		{
			TiXmlElement*l_pViewWidthHeight = new TiXmlElement(L"ViewWidthHeight");
			TiXmlElement*l_pXmlElementData = m_pViewWidthHeightLinerDataProcessor->ToTiXmlElement();
			if( l_pXmlElementData )
				l_pViewWidthHeight->LinkEndChild(l_pTiXmlElement);
			l_pTiXmlElement->LinkEndChild(l_pViewWidthHeight);
		}
		if( l_iPosAnimationDataSize > 0 )
		{
			TiXmlElement*l_pPosElement = new TiXmlElement(L"Pos");
			TiXmlElement*l_pXmlElementData = m_pPosLinerDataProcessor->ToTiXmlElement();
			if( l_pXmlElementData )
				l_pPosElement->LinkEndChild(l_pXmlElementData);
			l_pTiXmlElement->LinkEndChild(l_pPosElement);
		}
		if( l_iAngleAnimationDataSize > 0 )
		{
			TiXmlElement*l_pAngle = new TiXmlElement(L"Angle");
			TiXmlElement*l_pXmlElementData = m_pAngleLinerDataProcessor->ToTiXmlElement();
			if( l_pXmlElementData )
				l_pAngle->LinkEndChild(l_pXmlElementData);
			l_pTiXmlElement->LinkEndChild(l_pAngle);
		}
		return l_pTiXmlElement;
	}

	void	cCameraAnimation::ProcessGameResolution(TiXmlElement*e_pTiXmlElement)
	{
		//	<GameResolution>
		//		<Time Data="0,1" />
		//		<AnimationData Type="Vector4" Data0="" Data1="" />
		//	/<GameResolution>	
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("Time")
			{
				//std::vector<float>	l_TimeData = GetValueListByCommaDivide(l_strValue);
				//m_pGameResolutionAnimationData->SetTimeVector(l_TimeData);
			}
			else
			COMPARE_NAME("AnimationData")
			{
				//std::vector<float>	l_TimeData = GetValueListByCommaDivide(l_strValue);
				//m_pGameResolutionAnimationData->SetLinerDataVector(l_TimeData);
			}
		PARSE_NAME_VALUE_END
	}

	void	cCameraAnimation::ProcessAngle(TiXmlElement*e_pTiXmlElement)
	{
	//	<AngleAnimationData>
	//		<Time Data="0,1" />
	//		<AnimationData Type="Vector3" Data0="" Data1="" />
	//	</AngleAnimationData>
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("Time")
			{
		
			}
			else
			COMPARE_NAME("AnimationData")
			{
		
			}
		PARSE_NAME_VALUE_END
	}

	void	cCameraAnimation::InternalInit()
	{
		m_pContainer->Init();
	}

	void	cCameraAnimation::Update2DCamera()
	{
		Projection	l_Projection;
		l_Projection.SetOrthographic(m_vCameraViewSize.x,m_vCameraViewSize.y,-1000,1000);
		Vector2	l_vHalfSize = m_vCameraViewSize/2;
		Vector4	l_vViewZone(m_vCameraPos.x-l_vHalfSize.x,m_vCameraPos.y-l_vHalfSize.y,m_vCameraPos.x+l_vHalfSize.x,m_vCameraPos.y+l_vHalfSize.y);
		cMatrix44	l_matCameraView;
		glhOrthof2((float*)l_matCameraView.m,l_vViewZone.x,l_vViewZone.z,l_vViewZone.y,l_vViewZone.w, -10000, 10000);
		m_matOriginalCameraView = cMatrix44(GetCurrentViewProjectionMatrix());
		SetupShaderViewProjectionMatrix(l_matCameraView,true);
		m_bViewProjectionMatrixSet = true;
	}

	void	cCameraAnimation::Update3DCamera()
	{
		Projection	l_Projection;
		l_Projection.SetFovXFovY(m_vCameraViewSize.x,m_vCameraViewSize.y,0,100);
		cMatrix44	l_matCameraView = cMatrix44::TranslationMatrix(m_vCameraPos)*cMatrix44::RotationMatrix(m_vCameraAngle)*l_Projection.GetMatrix();
		m_matOriginalCameraView = cMatrix44(GetCurrentViewProjectionMatrix());
		SetupShaderViewProjectionMatrix(l_matCameraView,true);
		m_bViewProjectionMatrixSet = true;
	}

	void	cCameraAnimation::InternalUpdate(float e_fElpaseTime)
	{
		m_pContainer->Update(e_fElpaseTime);
		m_bViewProjectionMatrixSet = false;
		bool	l_bUpdateViewProjectionMatrix = false;
		if( m_pViewWidthHeightLinerDataProcessor && m_pViewWidthHeightLinerDataProcessor->IsDuringWorking() )
		{
			l_bUpdateViewProjectionMatrix = true;
			m_vCameraViewSize = m_pViewWidthHeightLinerDataProcessor->GetCurrentData();
		}

		if( m_pPosLinerDataProcessor && m_pPosLinerDataProcessor->IsDuringWorking() )
		{
			l_bUpdateViewProjectionMatrix = true;
			m_vCameraPos = m_pPosLinerDataProcessor->GetCurrentData();
		}

		if(m_pAngleLinerDataProcessor && m_pAngleLinerDataProcessor->IsDuringWorking())
		{
			l_bUpdateViewProjectionMatrix = true;
			m_vCameraAngle = m_pAngleLinerDataProcessor->GetCurrentData();
		}
		if( l_bUpdateViewProjectionMatrix )
		{
			if( Projection::Perspective == m_eProjectionType )
				Update3DCamera();
			else
				Update2DCamera();
		}
	}

	void	cCameraAnimation::InternalRender()
	{
		if( m_eProjectionType == Projection::Orthographic )
		{
			Update2DCamera();
		}
		else
		{
			Update3DCamera();
		}
	}

	void	cCameraAnimation::SetRenderStatusBack()
	{
		if( this->m_bSetRenderStatusBack )
		{
			//if(m_bViewPortSet)
			//{
			//	Vector4	l_vViewport = m_pViewPortAnimationData->GetCurrentData();
			//	if( l_vViewport != cGameApp::m_svViewPortSize )
			//	{
			//		glViewport((int)cGameApp::m_svViewPortSize.x,(int)cGameApp::m_svViewPortSize.y,(int)cGameApp::m_svViewPortSize.z,(int)cGameApp::m_svViewPortSize.w);
			//	}
			//}
			if( m_bViewProjectionMatrixSet )
			{
				SetupShaderViewProjectionMatrix(m_matOriginalCameraView,true);
			}
		}
	}

	void	cCameraAnimation::RenderLastFrame()
	{
		RenderByGlobalTime(this->m_pContainer->GetEndTime()+0.1f);
	}

    void	cCameraAnimation::RearrangeTime(float e_fNewTime)
	{
		m_pContainer->Rearrange(e_fNewTime);
	}

	void	cCameraAnimation::RearrangeTimeByPercent(float e_fPercenttage)
	{
		m_pContainer->Rearrange(e_fPercenttage*this->GetEndTime());
	}

	void	cCameraAnimation::UpdateByGlobalTime(float e_fGlobalTime)
	{
		m_pContainer->UpdateByGlobalTime(e_fGlobalTime);
	}

	void	cCameraAnimation::RenderByGlobalTime(float e_fTime)
	{
		UpdateByGlobalTime(e_fTime);
		this->Render();

	}

	void	cCameraAnimation::InvertOrder()
	{
		m_pContainer->InvertOrder();
	}

	float	cCameraAnimation::GetEndTime()
	{
		return m_pContainer->GetEndTime();
	}

	void	cCameraAnimation::Clear()
	{
		m_pContainer->Clear();
	}

	void	cCameraAnimationManager::InternalParse()
	{
		this->Destroy();
	}

	void	cCameraAnimationManager::ProcesscCameraAnimation(TiXmlElement*e_pTiXmlElement)
	{
		cCameraAnimation*l_p2DCameraAnimation = new cCameraAnimation(e_pTiXmlElement);
		if(!this->AddObject(l_p2DCameraAnimation))
		{
			UT::ErrorMsg(l_p2DCameraAnimation->GetName(),L"exists");
		}
	}

	void	cCameraAnimationManager::HandleElementData(TiXmlElement*e_pTiXmlElement)
	{
		const wchar_t*l_strValue = e_pTiXmlElement->Value();
		COMPARE_VALUE_WITH_DEFINE( cCameraAnimation::TypeID )
		{
			ProcesscCameraAnimation(e_pTiXmlElement);
		}
	}
	//<CameraAnimation>
	//	<cCameraAnimation Name="aabbcc">
	//		<GameResolution>
	//			<Time Data="0,1" />
	//			<AnimationData Type="Vector4" Data0="" Data1="" />
	//		/<GameResolution>
	//		<AngleAnimationData>
	//			<Time Data="0,1" />
	//			<AnimationData Type="Vector3" Data0="" Data1="" />
	//		</AngleAnimationData>
	//	</cCameraAnimation>
	//</CameraAnimation>
	void	cCameraAnimationManager::Export(char*e_strFileName)
	{
		int	l_iCount = Count();
		if( l_iCount > 0 )
		{
			TiXmlDocument l_Doc(UT::CharToWchar(e_strFileName).c_str());
			TiXmlElement*l_pCameraAnimationTiXmlElement = new TiXmlElement(L"CameraAnimation");
			l_Doc.LinkEndChild(l_pCameraAnimationTiXmlElement);
			for( int i=0;i<l_iCount;++i )
			{
				TiXmlElement*l_pData = 0;
				cFMTimeLineAnimationRule*l_pFatmingAnimationRule = this->GetObject(i);
				if( l_pFatmingAnimationRule->Type() == cCameraAnimation::TypeID )
				{
					cCameraAnimation*l_p2DCameraAnimation = (cCameraAnimation*)l_pFatmingAnimationRule;
					l_pData = l_p2DCameraAnimation->Export();
				}
				else
				{
					UT::ErrorMsg(l_pFatmingAnimationRule->Type(),L"Error");
				}
				if( l_pData )
					l_pCameraAnimationTiXmlElement->LinkEndChild(l_pData);
			}
			l_Doc.SaveFile(e_strFileName);
		}	
	}
}