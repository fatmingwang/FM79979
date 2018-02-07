#pragma once
//
enum	eMonsterStatus
{
	eMS_NONE = 0,				//not on the scene
	eMS_ALIVE,					//on the scene,do AIMachine
	eMS_STRUGGLE,				//while fish is hitted it will has a fake probability to play this,make speed slower
	eMS_DIED_SHOW,				//
	eMS_WAITING_FOR_CLEAN,		//
	eMS_MAX,
};

enum	eBodyType
{
	eBT_Small = 0,
	eBT_Medium,
	eBT_Big,
	eBT_Enormous,
	eBT_Total,
};

eBodyType	GetBodyType(const wchar_t *e_str);

class cMonsterBase : public cRenderObject
{
private:
	virtual	void				ProcessCollisionlData(TiXmlElement*e_pElement) = 0;
	void						ProcessStatusData(TiXmlElement*e_pTiXmlElement);
protected:
	eBodyType					m_eBodyType;
	cMPDI*						m_pMonsterAnimation[eMS_MAX];
	//
	static sMinMaxData<float>	m_StruggleTime;
	UT::sTimeCounter			m_StruggleTC;
	int							m_iID;
	float						m_fScale;
	float						m_fRadius;
	eMonsterStatus				m_eMonsterStatus;
	//internal 
	virtual	void				InternalInit() = 0;
	virtual	void				InternalUpdate(float e_fElpaseTime) = 0;
	virtual void				StatusChange(eMonsterStatus e_eMonsterStatus);
public:
	//
	cMonsterBase();
	cMonsterBase(cMonsterBase*e_pMonsterBase);
	virtual ~cMonsterBase();
	//static GetMe(TiXmlElement*e_pElement);
	eBodyType					GetBodyType();
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
