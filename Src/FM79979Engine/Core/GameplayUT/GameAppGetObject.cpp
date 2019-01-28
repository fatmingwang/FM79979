#include "../stdafx.h"
#include "GameApp.h"
#include "../Particle/AllParticleInclude.h"
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
		std::wstring	l_strName = UT::GetFileNameWithoutFullPath(e_strFileName);
		cCurveManager*l_pCurveManager = m_spPathFileList->GetObject(l_strName.c_str());
		if (l_pCurveManager)
			return l_pCurveManager;
		l_pCurveManager = new cCurveManager();
		if (l_pCurveManager->Parse(UT::WcharToChar(e_strFileName).c_str()))
		{
			l_pCurveManager->SetName(l_strName);
			m_spPathFileList->AddObjectNeglectExist(l_pCurveManager);
			l_pCurveManager->DoScale(cGameApp::m_svGameScale);
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