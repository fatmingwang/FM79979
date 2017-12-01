#pragma once
//#include <map>
#include "../Math/Frame.h"
#include "../XML/XMLLoader.h"
#include "../TemplateClass.h"
#include "FMTimelineAnimationObject.h"
#include "BaseUVImageData.h"
#include "SubMPDIRenderPointData.h"

namespace FATMING_CORE
{
	enum eMPDINodeType
	{
		eMPDI_NT_BUTTON = 0,//cClickMouseBehavior
		eMPDI_NT_TEXT,//it might be a position and don't need to render?
		eMPDI_NT_LAYER,//it will ate click event and no respond,because most time it just a BG to cover background.cClickMouseBehaviorVector
		eMPDI_NT_ANIMATION,//just a effect for visual.
		eMPDI_NT_MAX,
	};
	//
	class	cMPDINodeFile;
	class	cLinerDataContainer;
	class	cMPDINodeImageData;
	//MDINode,different is ,contain a start pose(bind pose could move(SRT) with children)
	class	cMPDINode:public cFMTimelineAnimationObject,public cMulti_PI_Image
	{
	private:
		//for internal render use(m_vWorkingRotation,m_vWorkingPosition).
		cMatrix44						m_matAnimationMatrix;
		virtual	void					UpdateCachedWorldTransformIfNeeded();
	protected:
		struct	s2DVertex
		{
			Vector3		vPos[4];
			Vector2		fUV[4];
		};
		s2DVertex						m_2DVertices;
		//related to m_pLinerDataContainer
		float							m_fMaxLinerDataPlayTime;
		//default is eMT_ANIMATION.
		eMPDINodeType					m_eMPDINodeType;
	    virtual	void					InternalInit();
	    virtual	void					InternalUpdate(float e_fElpaseTime);
	    virtual	void					InternalRender();
		virtual	void					InternalDestroy();
		virtual	void					InternalDebugRender();
		virtual	void					InternalUpdateByGlobalTime(float e_fGlobalTime);
		//
		cLinerDataContainer*			m_pLinerDataContainer;
		cLinerDataProcessor<Vector3>*	m_pPosLinerDataProcessor;
		cLinerDataProcessor<Vector4>*	m_pColorLinerDataProcessor;
		cLinerDataProcessor<Vector2>*	m_pSizeLinerDataProcessor;
		cLinerDataProcessor<Vector3>*	m_pRotationLinerDataProcessor;
		//
		cMPDINodeImageData*				m_pImageData;
		Vector4							m_vWorkingColor;
		Vector2							m_vWorkingSize;
		Vector3							m_vWorkingRotation;
		Vector3							m_vWorkingPosition;
		cBaseImage*						m_pWorkingpImage;
		//the pos to anchor image size change
		GET_SET_DEC(eImageAnchorType,m_eAnchorType,GetAnchorType,SetAnchorType);
		GET_SET_DEC(Vector3,m_vRotationAnglePosOffset,GetRotationAnglePosOffset,SetRotationAnglePosOffset);
		//blending data,if all cCueToStartCurveWithTime under its parent MPDI,it could optmize to speed up,but I am not doing it right now,fatming,fuck
		GET_SET_DEC(bool,m_bColorBlending,IsColorBlending,SetColorBlending);
		GET_SET_DEC(GLenum,m_SrcBlendingMode,GetSrcBlendingMode,SetSrcBlendingMode);
		GET_SET_DEC(GLenum,m_DestBlendingMode,GetDestBlendingMode,SetDestBlendingMode);
	public:
		cMPDINode();
		cMPDINode(cMPDINode*e_pMPDINode);
		CLONE_MYSELF(cMPDINode);
		virtual ~cMPDINode();
	    virtual	void					RearrangeTime(float e_fNewTime);
		virtual	void					RearrangeTimeByPercent(float e_fPercenttage);
	    virtual	void					RenderByGlobalTime(float e_fTime);
	    virtual	void					InvertOrder();
	    //start and last time.
	    virtual	float					GetEndTime();
		void							RefreshMaxLinerDataPlayTime();
		float							GetMaxLinerDataPlayTime();
		//for extra data
		//virtual	void					SetData(void*e_pData,bool e_bForceReplace);
		cLinerDataProcessor<Vector3>*	GetPosData();
		cLinerDataProcessor<Vector4>*	GetColorData();
		cLinerDataProcessor<Vector2>*	GetSizeData();
		cLinerDataProcessor<Vector3>*	GetRotationData();
		cMPDINodeImageData*				GetImageData();
		//cMatrix44						GetAnimationMatrix(){return m_matAnimationMatrix;}
	};

	class	cMPDINodeImageData:public cTimeAndDataLinerUpdateInterface
	{
		friend class cMPDINode;
		std::vector<cBaseImage*>	m_RenderObjectVector;
		cBaseImage*					m_pCurrentRenderObject;
		virtual	void				InternalInit();
	public:
		cMPDINodeImageData();
		cMPDINodeImageData(cMPDINodeImageData*e_pImageData);
		virtual		cTimeAndDataLinerUpdateInterface*	Clone();
		virtual ~cMPDINodeImageData();
		virtual	TiXmlElement*	ToTiXmlElement();
		virtual	void			Init();
		cBaseImage*				GetCurrentData();
		virtual	void			Update(float e_fElpaseTime);
		virtual	void			Clear();
		virtual	void			InvertOrder();
		void					AddData(cBaseImage*e_pBaseImage,float e_fTime);
		bool					InsertData(int e_iIndex,float e_fTime,cBaseImage*e_pBaseImage);
		bool					ChangeData(int e_iIndex,float e_fTime,cBaseImage*e_pBaseImage);
		bool					RemoveDtaa(int e_iIndex);
		void					SetData(std::vector<float>e_TimeVector,std::vector<cBaseImage*>*e_pPIDataVector);
		virtual	bool			InsertDataWithLiner(int e_iIndex,int e_iCount){ assert(0&&"not support"); return false; }
	};

	//an adaptor to quick fetch.
//	class	cMPDINodeFile
//	{
//		//if the nodes contain button,the mouse event must to be called.
//		bool	m_bContainButton;
//	private:
////#ifdef DEBUG
//		//cNamedTypedObjectVector<cMPDINode>*	m_pCreatedNodesVector;
////#endif
//		struct sQuickFetchNodeData
//		{
//			std::vector<int>	NodePos;//node tree position.
//			std::wstring		strNodeNmae;
//		};
//		cMPDINode*				m_pRoot;
//		//int could be eMPDINodeType or bigger user could assign the value as need.
//		//its s reference not a real data.
//		std::map<int,cNamedTypedObjectVector<cMPDINode>* >	m_UIObjectQuickSearchMap;//eUIObjectType
//	public:
//		cMPDINode*				GetObject(const wchar_t*e_strName,cMPDINode*e_pTargetNode);
//		cMPDINode*				GetObject(int e_iType,const wchar_t*e_strName);
//		cMPDINode*				GetCloneRootNode();
//		//input root node and use quick search to find the target.
//	};

//end namespace FATMING_CORE
}