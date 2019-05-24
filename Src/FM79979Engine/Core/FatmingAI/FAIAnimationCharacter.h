#ifndef _FAI_ANIMATION_CHARACTER_H_
#define _FAI_ANIMATION_CHARACTER_H_

#include "FAICharacterInterface.h"

namespace FATMING_AI
{

	//===============================================
	//auto change animation while character behivor status is not same
	//===============================================
	class	cFAIAnimationCharacter:public cFMWorkingObjectChanger<cFMTimeLineAnimationRule>,public cFAICharacterInterface
	{
		eFAICharacterBehaviorStatus	m_ePreviousBehaviodStatus;
	public:
		DEFINE_TYPE_INFO();
		cFAIAnimationCharacter(TiXmlElement*e_pElement);
		cFAIAnimationCharacter(cFAIAnimationCharacter*e_pFAIAnimationStatus);
		virtual ~cFAIAnimationCharacter();
		CLONE_MYSELF(cFAIAnimationCharacter);
		virtual	bool	IsCharacterBehaviorStatusLock()override;
		virtual void*   GetAnimationData()override;
		virtual	void	Init()override;
		virtual	void	Update(float e_fElpaseTime)override;
		virtual	void	Render()override;
	};
}

#endif