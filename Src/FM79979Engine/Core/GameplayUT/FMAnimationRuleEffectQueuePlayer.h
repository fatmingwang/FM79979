#ifndef _FM_ANIMATION_RULE_QUEUE_PLAYER_H_
#define _FM_ANIMATION_RULE_QUEUE_PLAYER_H_
//get any kind of effect(MPDI,particle) play on the screen
#include "../Image/FMAnimationRule.h"
namespace FATMING_CORE
{
	class	cFMARQueuePlayer:public cFatmingGroupBehaviorList<cFMTimeLineAnimationRule>
	{
		cFMTimeLineAnimationRule*	m_pFMAnimationRule;
		//
		void				ProcssMPDIEffect(TiXmlElement*e_pElement);
		void				ProcssParticleEffect(TiXmlElement*e_pElement);
		//
	    virtual	void		InternalInit();
	    virtual	void		InternalUpdate(float e_fElpaseTime);
	    virtual	void		InternalRender();
	public:
		cFMARQueuePlayer(TiXmlElement*e_pElement);
		cFMARQueuePlayer(cFMARQueuePlayer*e_pFARQueuePlayer);
		virtual ~cFMARQueuePlayer();
		CLONE_MYSELF(cFMARQueuePlayer);
		DEFINE_TYPE_INFO()
		//cNamedTypedObjectVector<cMultiPathDynamicImage>		m_MPDIQueue;
		//
		virtual	void	RearrangeTime(float e_fNewTime);
		virtual	void	RearrangeTimeByPercent(float e_fPercenttage);
		virtual	void	RenderByGlobalTime(float e_fTime);
		virtual	void	InvertOrder();
		//
		virtual	void	SetData(void*e_pData);
	};
//end namespace
}
#endif