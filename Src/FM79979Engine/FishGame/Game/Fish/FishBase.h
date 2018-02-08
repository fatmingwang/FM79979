#pragma once
//
enum	eFishStatus
{
	eFS_NONE = 0,				//not on the scene
	eFS_ALIVE,					//on the scene,do AIMachine
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

class cFishBase : public cRenderObject
{
private:
	virtual	void				ProcessCollisionlData(TiXmlElement*e_pElement) = 0;
	void						ProcessStatusData(TiXmlElement*e_pTiXmlElement);
protected:
	eFishBodyType					m_eFishBodyType;
	cMPDI*						m_pMonsterAnimation[eFS_MAX];
	//
	static sMinMaxData<float>	m_StruggleTime;
	UT::sTimeCounter			m_StruggleTC;
	int							m_iID;
	float						m_fScale;
	float						m_fRadius;
	eFishStatus					m_eFishStatus;
	//internal 
	virtual	void				InternalInit() = 0;
	virtual	void				InternalUpdate(float e_fElpaseTime) = 0;
	virtual void				StatusChange(eFishStatus e_eFishStatus);
public:
	//
	cFishBase();
	cFishBase(cFishBase*e_pMonsterBase);
	virtual ~cFishBase();
	//static GetMe(TiXmlElement*e_pElement);
	eFishBodyType					GetFishBodyType();
	float						GetRadius() { return m_fRadius; }
	int							GetID() { return m_iID; }
	void						Init();
	virtual	void				Update(float e_fElpaseTime);
	virtual	void				Render();
	virtual	bool				IsCollide(cbtShapeCollision*e_pbtShapeCollision) = 0;
	//while monster status is wait for clear or control by mini game cannot be died
	virtual bool				IsStatusAllowToDied();
	float						GetScale() { return m_fScale; }
};
