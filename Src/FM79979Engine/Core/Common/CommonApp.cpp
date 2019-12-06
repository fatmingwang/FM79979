#include "CommonApp.h"
#include "EventSender/MessageSender.h"
namespace	FATMING_CORE
{
	bool  cCommonApp::m_sbGamePause = false;

	void cCommonApp::Update(float e_fElpaseTime)
	{
		if (m_spMessageSenderManager)
			m_spMessageSenderManager->Update(e_fElpaseTime);
	}

}