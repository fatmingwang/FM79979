#pragma once

#include "../Render/UI/ImageButton.h"
#include "../Render/UI/Mouse_Touch.h"

namespace FATMING_CORE
{
	//class	cImageButton;
	cRenderObject* GetRenderObjectBehavior(TiXmlElement* e_pElement, bool e_bClone = true);

//<cClickMouseBehaviorVector Name="aabb">
//	<cImageButton Name="TestButton" PI="Image/Dice/Dice.pi" Image="Call" Pos="" />
//	<cImageButton Name="TestButton" MPDIList="Image/Dice/Dice.mpdi" MPDI="" />
//</cClickMouseBehaviorVector>
	cClickMouseBehaviorVector<cImageButton>*	GetClickMouseBehaviorList(TiXmlElement*e_pElement);
	cClickMouseBehavior*						GetClickMouseBehavior(TiXmlElement*e_pElement);
//end namespace FATMING_CORE
}
//end _RENDER_OBJECT_XML_PARSER_H_