#ifndef	_CAMERA_ANIMATION_H_
#define	_CAMERA_ANIMATION_H_

#include "../Image/FMAnimationRule.h"
#include "SimpleCamera.h"

namespace FATMING_CORE
{
	//template<class T>class	cLinerTemplateDataProcess;
	
	class	cCameraAnimation:public cFMTimeLineAnimationRule
	{
		Vector2								m_vCameraViewSize;
		Vector3								m_vCameraPos;
		Vector3								m_vCameraAngle;
		//cLinerTemplateDataProcess<Vector4>*	m_pViewPortAnimationData;			//viewport
		//bool								m_bViewPortSet;

		Projection::ProjectionType			m_eProjectionType;
		void								ProcessGameResolution(TiXmlElement*e_pTiXmlElement);
		void								ProcessAngle(TiXmlElement*e_pTiXmlElement);
		//
		bool								m_bViewProjectionMatrixSet;
		cLinerDataContainer*				m_pContainer;
		cLinerDataProcessor<Vector3>*		m_pPosLinerDataProcessor;						//Position
		cLinerDataProcessor<Vector2>*		m_pViewWidthHeightLinerDataProcessor;			//for
		cLinerDataProcessor<Vector3>*		m_pAngleLinerDataProcessor;					//angle
		//
		cMatrix44		m_matCameraViewMatrix;
		void			Update2DCamera();
		void			Update3DCamera();
		//
		virtual	void	InternalInit()override;
	    virtual	void	InternalUpdate(float e_fElpaseTime)override;
	    virtual	void	InternalRender()override;
		virtual	void	RenderLastFrame();
		GET_SET_DEC(bool,m_bSetRenderStatusBack,IsSetRenderStatusBack,SetSetRenderStatusBack);
		cMatrix44		m_matOriginalCameraView;
	public:
		DEFINE_TYPE_INFO()
		cCameraAnimation();
		cCameraAnimation(TiXmlElement*e_pTiXmlElement);
		cCameraAnimation(cCameraAnimation*e_p2DCameraAnimation);
		virtual ~cCameraAnimation();
		CLONE_MYSELF(cCameraAnimation);
		TiXmlElement*	Export();
	    virtual	void	RearrangeTime(float e_fNewTime)override;
		virtual	void	RearrangeTimeByPercent(float e_fPercenttage)override;
		virtual	void	UpdateByGlobalTime(float e_fGlobalTime)override;
	    virtual	void	RenderByGlobalTime(float e_fTime)override;
	    virtual	void	InvertOrder()override;
	    virtual	float	GetEndTime()override;
		virtual	void	Clear();
		void			SetRenderStatusBack();

		//cLinerTemplateDataProcess<Vector4>*		GetGameResolutionAnimationData(){return m_pGameResolutionAnimationData;}
		cLinerDataProcessor<Vector3>*		GetAngleAnimationData(){return m_pAngleLinerDataProcessor;}
		cLinerDataProcessor<Vector2>*		GetViewWidthHeightAnimationData(){return m_pViewWidthHeightLinerDataProcessor;}
		cLinerDataProcessor<Vector3>*		GetPosAnimationData(){return m_pPosLinerDataProcessor;}
	};
	class	c3DCameraAnimation:public cFMTimeLineAnimationRule
	{
	
	};
	//
	//
	//
	class	cCameraAnimationManager:public cNamedTypedObjectVector<cFMTimeLineAnimationRule>,public cSmartObject,public ISAXCallback
	{
		virtual	void	InternalParse();
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
		void			ProcesscCameraAnimation(TiXmlElement*e_pTiXmlElement);
	public:
		cCameraAnimationManager();
		virtual ~cCameraAnimationManager();
		void	Export(char*e_strFileName);
	};

//end namespace FATMING_CORE
}
//end _CAMERA_ANIMATION_H_
#endif