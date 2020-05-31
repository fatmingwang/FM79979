#pragma once

#include "2DMesh.h"
#define	FM_MORPHING_ANIMATION_VER	20200522

//morphing xml
#define FM_MORPHING_XML_FILE_EXTENSION_NAME					"mx"

#define	MX_ELEMENT_NAME										"PI_morphing"

#define	MX_BINARY_FILE_ELEMENT_NAME							"MXBinary"
//morphing binary
#define FM_MORPHING_FILE_BINARY_DATA_EXTENSION_NAME			"mb"

#define	FM_MORPHING_EDITOR_FILE_BINARY_DATA_EXTENSION_NAME	"editor_mb"

namespace FATMING_CORE
{
	class cFMMorphingAnimation :public cFMTimeLineAnimationRule
	{
		friend class cFMMorphingAnimationVector;
		friend class cFMMorphingAnimationManager;
		struct sTimeAndPosAnimationData
		{
			typedef std::map<float, Vector3> FloatTocVector3Map;
			//time and 
			FloatTocVector3Map	m_FormKeyFrames;
			Vector3				UpdateAnimationByGlobalTime(float e_fGlobalTime);
			void				AssignData(int e_iCount, float*e_pfTimeArray, Vector3*e_pfPosArray);
		};
		float									m_fEndTime;
		std::vector<sTimeAndPosAnimationData>*	m_pTimeAndPosAnimationVector;
		//for draw indices
		c2DMeshObject::sMeshBuffer*				m_pMeshBuffer;
		//final render pos,sTimeAndPosAnimationData::UpdateAnimationByGlobalTime
		std::vector<Vector3>					m_RenderVertex;
		float									m_fSpeed;
		bool									m_bInvertOrder;
		//
		virtual	void	InternalInit()override;
		virtual	void	InternalUpdate(float e_fElpaseTime)override;
		virtual	void	InternalRender()override;
	public:
		cFMMorphingAnimation(c2DMeshObject::sMeshBuffer*e_pMeshBuffer,std::vector<sTimeAndPosAnimationData>*e_pTimeAndPosAnimationVector);
		cFMMorphingAnimation(cFMMorphingAnimation*e_pFMMorphingAnimation);
		virtual ~cFMMorphingAnimation();
		DEFINE_TYPE_INFO();
		CLONE_MYSELF(cFMMorphingAnimation);
		virtual	void	UpdateByGlobalTime(float e_fGlobalTime)override;
		virtual	void	RenderByGlobalTime(float e_fTime)override;
		virtual	void	RearrangeTime(float e_fNewTime)override;
		virtual	void	RearrangeTimeByPercent(float e_fPercenttage)override;
		virtual	void	InvertOrder()override;
		virtual	float	GetEndTime()override;
	};


	class cFMMorphingAnimationVector :public cNamedTypedObjectVector<cFMMorphingAnimation>, public cNodeISAX
	{
		friend class cFMMorphingAnimationManager;
		class cAnimationDataMap :public cSmartObject
		{
		public:
			cAnimationDataMap(NamedTypedObject*e_pObject);
			virtual ~cAnimationDataMap();
			std::map<cFMMorphingAnimation*, std::vector<cFMMorphingAnimation::sTimeAndPosAnimationData>*> m_AnimationMap;
		};
		cAnimationDataMap*		m_pAnimationDataMap;
		c2DMeshObjectVector*	m_p2DMeshObjectManager;
	public:
		cFMMorphingAnimationVector(c2DMeshObjectVector*e_p2DMeshObjectVector);
		virtual ~cFMMorphingAnimationVector();
		DEFINE_TYPE_INFO();
		c2DMeshObjectVector*		Get2DMeshObjectManager() { return m_p2DMeshObjectManager; }
	};

	class cFMMorphingAnimationManager:public cNamedTypedObjectVector<cFMMorphingAnimationVector>, public cNodeISAX
	{
		//morphing binary file
		cBinaryFile*				m_pMBFile;
		unsigned int				m_uiCurrentDataPos;
		cFMMorphingAnimationVector*	m_pCurrentFMMorphingAnimationVector;
		c2DMeshObjectVector*		m_pCurrent2DMeshObjectVector;
		virtual	bool				MyParse(TiXmlElement*e_pRoot)override;
		bool						Process_MorphingAnimationData(TiXmlElement*e_pTiXmlElement);
		bool						Process_TRIANGLE_AND_DRAW_INDEX_ElementData(const wchar_t*e_strTIFileName);
		//
		c2DMeshObjectManager		m_2DMeshObjectManager;
	public:
		cFMMorphingAnimationManager();
		~cFMMorphingAnimationManager();
		virtual NamedTypedObject*	GetObjectByFileName(const char*e_strFileName)override;
		c2DMeshObjectManager*		Get2DMeshObjectManager() { return &m_2DMeshObjectManager; }
	};
}