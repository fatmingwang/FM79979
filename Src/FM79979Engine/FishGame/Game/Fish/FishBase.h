#pragma once
//
enum	eFishStatus
{
	eFS_NONE = 0,				//not on the scene
	eFS_MOVING,					//on the scene,do AIMachine
	eFS_STRUGGLE,				//while fish is hitted it will has a fake probability to play this,make speed slower
	eFS_DIED_SHOW,				//
	eFS_WAITING_FOR_CLEAN,		//
	eFS_MAX,
};

enum	eFishBodyType
{
	eFBT_Small = 0,
	eFBT_Medium,
	eFBT_Big,
	eFBT_Enormous,
	eFBT_Total,
};

eFishBodyType	GetFishBodyType(const wchar_t *e_str);
class cPIUnitAndPointsVector;

class cFishBase : public cRenderObject
{
protected:
	void									ProcessCollisionlData(TiXmlElement*e_pElement);
	void									ProcessStatusAnimationData(TiXmlElement*e_pTiXmlElement);
	//
	eFishBodyType							m_eFishBodyType;
	cMPDIList*								m_pAnimationMPDIList;
	cMPDI*									m_pStatusAnimation[eFS_MAX];
	//its possible has more than one PI in the animation
	//key is PuzzleImage index of MPDIList(should as same as cMPDI)
	std::map<int,c2DImageCollisionData*>	m_CollisionDataMap;
	cbtShapeCollision*						m_pCurrentbtShapeCollision;//data from current status animation

	cPIUnitAndPointsVector*					m_pPIUnitAndPointsVector;
	//
	static sMinMaxData<float>				m_StruggleTime;
	UT::sTimeCounter						m_StruggleTC;
	int										m_iID;
	float									m_fScale;
	float									m_fRadius;
	eFishStatus								m_eFishStatus;
	//internal 
	virtual	void							InternalInit() = 0;
	virtual	void							InternalUpdate(float e_fElpaseTime) = 0;
	virtual void							StatusChange(eFishStatus e_eFishStatus) ;
	virtual	void							SetTransform(Vector3 e_vPos,float e_fAngle,Vector3 e_vOffsetPos);
	virtual	void							SetTransformCollision(cMatrix44 e_mat);
public:
	//
	cFishBase();
	cFishBase(cFishBase*e_pFishBase);
	virtual ~cFishBase();
	//static GetMe(TiXmlElement*e_pElement);
	eFishBodyType							GetFishBodyType();
	eFishStatus								GetFishStatus();
	float									GetRadius() { return m_fRadius; }
	int										GetID() { return m_iID; }
	void									Init();
	virtual	void							Update(float e_fElpaseTime);
	virtual	void							Render();
	virtual	void							DebugRender();
	virtual	bool							IsCollide(cbtShapeCollision*e_pbtShapeCollision);
	virtual	bool							IsVisible(Vector4 e_vWall);
	//while monster status is wait for clear or control by mini game cannot be died
	virtual bool							IsStatusAllowToDied();
	float									GetScale() { return m_fScale; }
};


#define	FISH_LAZY_GET_ME_START(e_pElement,TYPE,DO_YOUR_THING)		\
													TYPE*l_pObject = nullptr;												\
													COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pElement, TYPE::TypeID)		\
													{																		\
														const wchar_t*l_strID = e_pElement->Attribute(L"ID");				\
														if (l_strID)														\
														{																	\
															l_pObject = new TYPE();								\
															l_pObject->m_iID = GetInt(l_strID);								\
															FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pElement)			\
															COMPARE_TARGET_ELEMENT_VALUE(e_pElement, "StatusAnimation")		\
															{																\
																l_pObject->ProcessStatusAnimationData(e_pElement);			\
															}																\
															else															\
															COMPARE_TARGET_ELEMENT_VALUE(e_pElement, "Collision")			\
															{																\
																l_pObject->ProcessCollisionlData(e_pElement);				\
															}																\
															else															\
															{DO_YOUR_THING}													\
															FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pElement)				\
 														}																	\
													}																		\
													else																	\
													{																		\
														UT::ErrorMsg(TYPE::TypeID, L"ID donest't exist!");					\
													}																		\
													return l_pObject;