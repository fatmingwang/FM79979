#include "stdafx.h"
#include "GamePlayAttackPath.h"

cGamePlayAttackPath::sShowKeyPoints::sShowKeyPoints()
{
	m_pNumeralImage = nullptr;
	m_pNumeralImage = cGameApp::m_spGameResourceManager->GetObjectByDefaultResourceParser<cNumeralImage>(L"PathNumeralImage");
}
