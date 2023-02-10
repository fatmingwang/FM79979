#include "GameApp.h"
#include "../Render/PuzzleImage/PuzzleImage.h"
#include "../Render/MPDI/MPDI.h"
#include "../Render/Parser/AnimationParser.h"
#include "../Render/Particle/AllParticleInclude.h"
namespace	FATMING_CORE
{
	cPuzzleImage*	cGameApp::GetPuzzleImage(const wchar_t*e_strName)
	{
		if(m_spImageParser)
			return dynamic_cast<cPuzzleImage*>(m_spImageParser->GetObject(e_strName)); 
		return nullptr;
	}
	cBaseImage*		cGameApp::GetBaseImage(const wchar_t*e_strName)
	{
		if (m_spImageParser)
			return dynamic_cast<cBaseImage*>(m_spImageParser->GetObject(e_strName)); 
		return nullptr;
	}
	cPuzzleImage*	cGameApp::GetPuzzleImageByFileName(std::wstring e_strFileName)
	{
		if (m_spImageParser)
			return GetPuzzleImageByFileName(e_strFileName.c_str()); 
		return nullptr;
	}

	//<cNumeralImage PIFileName="" NumerialImageName=""/>
	//<cNumeralImage PIFileName="" StartPIUnitName="" EndPIUnitName=""/>
	cNumeralImage*	cGameApp::GetNumerialImageByXmlElement(TiXmlElement*e_pTiXmlElement)
	{
		if( e_pTiXmlElement )
		{
			const WCHAR*l_strTempString = 0;
			cPuzzleImage*l_pPI = 0;
			PARSE_ELEMENT_START(e_pTiXmlElement)
				COMPARE_NAME("PIFileName")
				{
					l_pPI = cGameApp::GetPuzzleImageByFileName(l_strValue);
				}
				else
				COMPARE_NAME("NumerialImageName")
				{
					return l_pPI->GetNumeralImageByName(l_strValue);
				}
				else
				COMPARE_NAME("StartPIUnitName")
				{
					l_strTempString = l_strValue;
				}
				else
				COMPARE_NAME("EndPIUnitName")
				{
					if( l_pPI )
						return l_pPI->GetNumeralImageByName(l_strTempString,l_strValue);
				}
			PARSE_NAME_VALUE_END
			//cNumeralImage*l_pNumerialImage;
		}
		return 0;
	}

	cPuzzleImage*	cGameApp::GetPuzzleImageByFileName(const wchar_t*e_strFileName)
	{
		if (m_spImageParser)
			return m_spImageParser->GetPuzzleImageByFileName(e_strFileName); 
		return nullptr;
	}
	cMPDIList*		cGameApp::GetMPDIList(const wchar_t*e_strName)
	{
		if (m_spImageParser)
			return m_spAnimationParser->GetMPDIList(e_strName); 
		return nullptr;
	}
	cMPDIList*		cGameApp::GetMPDIListByFileName(const wchar_t*e_strFileName, bool e_bShowErrorMsg)
	{
		if (m_spImageParser)
			return m_spAnimationParser->GetMPDIListByFileName(e_strFileName, e_bShowErrorMsg); 
		return nullptr;
	}
	cMPDIList*		cGameApp::GetMPDIList(const char*e_strFileName, const wchar_t*e_strName)
	{
		if (m_spImageParser)
			return m_spAnimationParser->GetMPDIList(e_strFileName, e_strName); 
		return nullptr;
	}
	cMPDIList*		cGameApp::GetMPDIList(int e_iIndex)
	{
		if (m_spImageParser)
			return m_spAnimationParser->GetMPDIList(e_iIndex); 
		return nullptr;
	}

	cCurveManager*	cGameApp::GetCurveManagerByFileName(const wchar_t*e_strFileName)
	{
		cCurveManager*l_pCurveManager = m_spPathFileList->GetObject(e_strFileName);
		if (l_pCurveManager)
			return l_pCurveManager;
		l_pCurveManager = new cCurveManager();
		if (l_pCurveManager->Parse(UT::WcharToChar(e_strFileName).c_str()))
		{
			l_pCurveManager->SetName(e_strFileName);
			m_spPathFileList->AddObjectNeglectExist(l_pCurveManager);
			if(cGameApp::m_spOpenGLRender->m_vGameScale.x != 1.f || cGameApp::m_spOpenGLRender->m_vGameScale.y != 1.f)
				l_pCurveManager->DoScale(cGameApp::m_spOpenGLRender->m_vGameScale);
			return l_pCurveManager;
		}
		delete l_pCurveManager;
		return nullptr;
	}

	cParticleEmitterGroup*cGameApp::GetPRTG(const wchar_t*e_strFileName, const wchar_t*e_strName)
	{
		return GetPRTG(UT::WcharToChar(e_strFileName).c_str(), e_strName); 
	}
	cParticleEmitterGroup*cGameApp::GetPRTG(const char*e_strFileName, const wchar_t*e_strName)
	{
		if(m_spBehaviorPaticleManager)
			return m_spBehaviorPaticleManager->GetPRTG(e_strFileName, e_strName); 
		return nullptr;
	}
	cParticleEmitterGroup*cGameApp::GetPRTG(const wchar_t*e_strName)
	{
		if (m_spBehaviorPaticleManager)
			return m_spBehaviorPaticleManager->GetPRTG(e_strName); 
		return nullptr;
	}

	cPrtEmitter*cGameApp::GetPrtEmitter(const char*e_strFileName, const wchar_t*e_strName)
	{
		if (m_spBehaviorPaticleManager)
			return m_spBehaviorPaticleManager->GetPaticleManager()->GetEmitter(e_strFileName, e_strName); 
		return nullptr;
	}
	cPrtEmitter*cGameApp::GetPrtEmitter(const wchar_t*e_strFileName, const wchar_t*e_strName)
	{
		return GetPrtEmitter(UT::WcharToChar(e_strFileName).c_str(), e_strName); 
	}
	cPrtEmitter*cGameApp::GetPrtEmitter(const wchar_t*e_strName)
	{
		if(m_spBehaviorPaticleManager)
			return m_spBehaviorPaticleManager->GetPaticleManager()->GetObject(e_strName); 
		return nullptr;
	}

}