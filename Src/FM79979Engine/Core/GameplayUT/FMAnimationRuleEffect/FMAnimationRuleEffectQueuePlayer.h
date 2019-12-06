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
	    virtual	void		InternalInit()override;
	    virtual	void		InternalUpdate(float e_fElpaseTime)override;
	    virtual	void		InternalRender()override;
	public:
		cFMARQueuePlayer(TiXmlElement*e_pElement);
		cFMARQueuePlayer(cFMARQueuePlayer*e_pFARQueuePlayer);
		virtual ~cFMARQueuePlayer();
		CLONE_MYSELF(cFMARQueuePlayer);
		DEFINE_TYPE_INFO()
		//cNamedTypedObjectVector<cMultiPathDynamicImage>		m_MPDIQueue;
		//
		virtual	void	RearrangeTime(float e_fNewTime)override;
		virtual	void	RearrangeTimeByPercent(float e_fPercenttage)override;
		virtual	void	RenderByGlobalTime(float e_fTime)override;
		virtual	void	InvertOrder()override;
		//
		virtual	void	SetData(void*e_pData)override;
	};
//end namespace
}
#endif