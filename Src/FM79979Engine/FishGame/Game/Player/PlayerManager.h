#pragma once
	
class	cPlayerManager : public cNamedTypedObjectVector<cPlayerBehaviorBase>
{
	std::map<int, std::vector<cMatrix44>>	m_IndexAndPlayerMatrixMap;
	int					m_iDebugPlayerPosIndex;
	void				ProcessPlayerCountAndPlayerPosMap(TiXmlElement*e_pTiXmlElement);
private:
	int				m_iPlayerCount;
	//cEventReceiverKey	*m_pEvent[ePMK_Total];

public:
	cMPDI * m_pSlimeCrown;  //聚寶盆專用
	cMPDI*			m_pSlimeCrownPos;  //聚寶盆專用
	int				m_nIdxPlayer;	//聚寶盆專用
	cMatrix44		m_Mat44Tmp1;	//聚寶盆專用
									//
	cPlayerManager(void);
	~cPlayerManager(void);

	void	ProcessSet(TiXmlElement* e_pElement);
	//
	void	Init(void);
	void	Update(float e_fElpaseTime);
	void	Render(void);
	void	MessageRender();
	void	Destroy(void);
	void	RenderMissileDirTex();
	bool	IsAllowToFireBullet(int e_iIndex, int e_iLimitCount);
};