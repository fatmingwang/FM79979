#pragma once

#include "../CommonRender/RenderObject.h"
namespace FATMING_CORE
{
	//
	class cRenderCommand;
	class cRenderQueue:public NamedTypedObject
	{
		std::list<cRenderCommand*>	m_RenderCommandList;
	public:
	};
}