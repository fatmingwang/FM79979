#pragma once

#include "FMAnimationRule.h"
#include "../../../XML/XMLLoader.h"
//=========================================
//this is a FMAnimationRule parser
//what ever it is,just require your data and it will auto update
//
//=========================================

namespace FATMING_CORE
{
	typedef cFMTimeLineAnimationRule*    (*FatmingAnimationRuleParseFunction)(TiXmlElement*e_pElement);
	class cFMTimeLineAnimationRuleManager : public cNamedTypedObjectVector< cFMTimeLineAnimationRule >,public ISAXCallback
	{
		//
		FatmingAnimationRuleParseFunction	m_pFatmingAnimationRuleParseFunction;
		//
		virtual	void						HandleElementData(TiXmlElement*e_pTiXmlElement);
		void								ProcessFMARQueuePlayer(TiXmlElement*e_pTiXmlElement);
		void								ProcessFatmingAnimationRule(TiXmlElement*e_pTiXmlElement);
								
		cTempContainer<cFMTimeLineAnimationRule>										m_WorkingFatmingAnimationRuleTempContainer;
		cNamedTypedObjectVector< cNamedTypedObjectVector< cFMTimeLineAnimationRule > >	m_AllFatmingAnimationRuleList;
		//not wait for calling,it's a always running game effect
		cNamedTypedObjectVector<cFMTimeLineAnimationRule>								m_AlwaysRunFatmingAnimationRuleList;
	public:

		cFMTimeLineAnimationRuleManager();
		virtual ~cFMTimeLineAnimationRuleManager();
		//after call this please call cFMTimeLineAnimationRule::SetData
		cFMTimeLineAnimationRule* RequirePrizeEffect( const wchar_t* e_strEffectTypeName,void*e_pData = 0,bool e_bPushIntoTempContainer = true);

		void Init(char*e_strFileName);
		void Render();
		void Update( float ElapseTime );
	};

//end namespace FATMING_CORE
}