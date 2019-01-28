#pragma once

#include "../../Common/NamedTypedObject.h"
#include <map>
namespace FATMING_CORE
{
	enum eRenderCommandType;
	class cRenderCommand;
	class cRenderQueue;
	class cRenderQueueManager :public NamedTypedObject
	{
		//swap one for work one for wait
		cRenderQueue*	m_pRenderQueue[2];
		cRenderQueue*	m_pCurrentRenderQueue;
		bool			m_bUpdateFinish;
		//
		std::map<size_t, cRenderCommand*>	m_RenderCommandResourceMap;
		//after RequireRenderCommand called wait for Render finish, add into m_pRenderQueue
		std::list<cRenderCommand*>			m_NewRequireRenderCommandWaitForAdd;
	public:
		cRenderQueueManager();
		~cRenderQueueManager();
		void			Render();

		cRenderCommand*	RequireRenderCommand(size_t e_uiObjectAddress, eRenderCommandType*e_pRenderCommandType);
		cRenderCommand*	DeleteRenderCommand(size_t e_uiObjectAddress, eRenderCommandType*e_pRenderCommandType);
	};
}