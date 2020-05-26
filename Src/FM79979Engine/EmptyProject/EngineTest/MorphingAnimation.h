#pragma once

#include "2DMesh.h"

namespace FATMING_CORE
{
	class cFMMorphingAnimation :public cFMTimeLineAnimationRule
	{
		friend class cFMMorphingAnimationManager;
		struct sTimeAndPosAnimationData
		{
			typedef std::map<float, Vector3> FloatTocVector3Map;
			//time and 
			FloatTocVector3Map	m_FormKeyFrames;
			Vector3				UpdateAnimationByGlobalTime(float e_fGlobalTime);
			void				RearrangeTime(float e_fScale);
			void				RenderPointByTime(float e_fTime, Vector4 e_vColor, float e_fPointSize);
		};
		std::vector<sTimeAndPosAnimationData>	m_TimeAndPosAnimationVector;
		//for draw indices
		c2DMeshObject::sMeshBuffer*				m_pMeshBuffer;
		//final render pos,sTimeAndPosAnimationData::UpdateAnimationByGlobalTime
		std::vector<Vector3>					m_RenderVertex;
		//
		virtual	void	InternalInit()override;
		virtual	void	InternalUpdate(float e_fElpaseTime)override;
		virtual	void	InternalRender()override;
	public:
		cFMMorphingAnimation(c2DMeshObject::sMeshBuffer*e_pMeshBuffer);
		cFMMorphingAnimation(cFMMorphingAnimation*e_pFMMorphingAnimation);
		~cFMMorphingAnimation();
		DEFINE_TYPE_INFO();
		CLONE_MYSELF(cFMMorphingAnimation);
		virtual	void	UpdateByGlobalTime(float e_fGlobalTime)override;
		virtual	void	RenderByGlobalTime(float e_fTime)override;
		virtual	void	RearrangeTime(float e_fNewTime)override;
		virtual	void	RearrangeTimeByPercent(float e_fPercenttage)override;
		virtual	void	InvertOrder()override;
		virtual	float	GetEndTime()override;
	};


	class cFMMorphingAnimationManager :public cNamedTypedObjectVector<cFMMorphingAnimation>, public cNodeISAX
	{
		class cAnimationDataMap :public cSmartObject
		{
		public:
			cAnimationDataMap(NamedTypedObject*e_pObject);
			virtual ~cAnimationDataMap();
			std::map<cFMMorphingAnimation*, cFMMorphingAnimation::sTimeAndPosAnimationData*> m_BufferMap;
		};
		cAnimationDataMap*	m_pAnimationDataMap;
		virtual	bool	MyParse(TiXmlElement*e_pRoot)override;
	public:
		cFMMorphingAnimationManager();
		~cFMMorphingAnimationManager();
		DEFINE_TYPE_INFO();
	};
}