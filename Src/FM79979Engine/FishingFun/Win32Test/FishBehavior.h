#ifndef	_FISH_BEHAVIOR_H_
#define	_FISH_BEHAVIOR_H_

#include "FishRodAndFishThreadData.h"


struct	sDescription
{
	int				iChangeLineLength;
	std::wstring	strDescription;
};
//<Description ChangeLineLength="" Text="this is a fish" />

class	cFishBehavior:public cFAICharacterInterface
{
	float						m_fHookedAngle;
	//cMatrix44	m_matDummy;
	friend class				cFishBehaviorManager;
	friend class				cFishShowProbability;
	//std::vector<sProbabilityToLeave>	m_ProbabilityToLeaveVector;
	struct	sShowProbability
	{
		Vector2	fShowDistanceRange;
		Vector2	ShowProbability;
	};
	sShowProbability					m_ShowProbability;
	Vector2								m_HeightRange;
	Vector2								m_WidthRange;
	POINT								m_GiveMoneyRange;
	POINT								m_GiveGold;
	sDescription						m_Description;
	float								m_fStamina;
	//get from fish rod level
	float								m_fLeaveProbability;
	std::vector<std::wstring>			m_strShowPlace;
	cCurveWithTime						m_Path;
	bool								m_bStunningable;
	float								m_fCoseStamina;
	int									m_iIDInCollectBook;
	Vector4								m_vVisibleRect;
	UT::sTimeCounter					m_UnhookTC;//if fTargetTime is small than 0,fish won't unhook
public:
	enum	eFishBehavior
	{
		eFB_MOVING = 0,
		eFB_HOOKED,
		eFB_UNHOOKED,
		//eFB_LET_GO,
		eFB_STUNNING_TO_LEAVE,
		eFB_NONE
	};
	eFishBehavior				m_eFishBehavior;
	cbtShapeCollision*			m_pCollideShape;
	cAnimationMesh*				m_pAnimationMesh;
	bool						m_bEnableFishLeaving;//player touch object to mkae status change to eFB_STUNNING
	void						ProcessDaeModelData(TiXmlElement*e_pElement);
	void						ProcessCollisionData(TiXmlElement*e_pElement);
	void						ProcessXmlData(TiXmlElement*e_pElement,cNamedTypedObjectVector<cColladaParser>*e_pCollada,cNamedTypedObjectVector<c2DImageCollisionData>*e_pCollision);
	void						UpdateVisibleRect();
public:
	cFishBehavior(const char*e_strFileName,cNamedTypedObjectVector<cColladaParser>*e_pCollada,cNamedTypedObjectVector<c2DImageCollisionData>*e_pCollision);
//		cFishBehavior(TiXmlElement*e_pElement,cNamedTypedObjectVector<cColladaParser>*e_pCollada,cNamedTypedObjectVector<c2DImageCollisionData>*e_pCollision);
	cFishBehavior(cFishBehavior*e_pFishBehavior);
	~cFishBehavior();
	CLONE_MYSELF(cFishBehavior);
	void			Init();
	void			Update(float e_fElpaseTime);
	void			Render();
	void			RenderData();
	void			DebugRender();
	void			SetStatus(eFishBehavior e_eFishBehavior,Frame*e_pTarget = 0,float e_fRestTimeToLeave = 0.f);
	bool			IsCollide(cbtShapeCollision*e_pbtShapeCollision);
	bool			IsCollide(Vector2 e_vPos);
	bool			IsVisible(Vector4 e_vCameraViewRect);

	int				SellForGold();
	int				SellForCash();
	float			StruggleToLeaveUpdate();
	void			GenerateHorizontalPath(float e_fWidth,float e_fHeight);
	bool			IsAllowToHook();
	float			GetRandomHeight();
	float			GetRandomWidth();
	float			GetStamina();
	void			SetLeaveProbability(float e_fLeaveProbability);
		//use a formua let fish to sell.
	int				GetGiveMoneyByWidth(float e_fWidth);
	int				GetGiveGold();
	virtual void*	GetAnimationData(){ return (void*)m_pAnimationMesh; }
};
//
class	cFishShowProbability;
class	cStageData;
class	cFishBehaviorManager:public cNamedTypedObjectVector<cFishBehavior>,public ISAXCallback
{
	friend class	cFishShowProbability;
	friend class	cStageData;
	cNamedTypedObjectVector<cFishBehavior>			m_GeneratedFishes;

	cNamedTypedObjectVector<cFishBehavior>			m_VisibleFishes;//from resource
	cNamedTypedObjectVector<cFishBehavior>			m_BeenHookedFishes;//from resource
	int												m_iCurrentVisibleFishIndex;
	//cNamedTypedObjectVector<cColladaParser>*		m_pAllColladaData;//from FishingFunApp
	//cNamedTypedObjectVector<c2DImageCollisionData>*	m_pAllCollisionData;//from FishingFunApp
	virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
	//probability to do some game dwsigner ask
	void											DoFishGameBehaviorUpdate(float e_fElpaseTime);
public:
	cFishBehaviorManager();
	~cFishBehaviorManager();
	void	Init();
	void	Update(float e_fElpaseTime,Vector4 e_vCameraViewRect,bool e_bSinking);
	void	Render();
	void	RenderData();
	void	DebugRender();
	void	RenderInfo();
	void	DoRodCollide(cbtShapeCollision*e_pbtShapeCollision,Frame*e_pHookFrame,cFishRodData*e_pFishRodData,float e_fRestTimeToTop,cOrthogonalCamera*e_pOrthogonalCamera,int e_iMoneyToLeave);
	void	AddFishData(char*e_strFileName);
};

#endif