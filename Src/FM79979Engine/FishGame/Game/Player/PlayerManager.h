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
	cMPDI * m_pSlimeCrown;  //�E�_�ֱM��
	cMPDI*			m_pSlimeCrownPos;  //�E�_�ֱM��
	int				m_nIdxPlayer;	//�E�_�ֱM��
	cMatrix44		m_Mat44Tmp1;	//�E�_�ֱM��
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