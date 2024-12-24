#pragma once


enum eAPGameType
{
	eAPGT_SLOT = 0,
	eAPGT_MINI_GAME,
	eAPGT_ORIGINAL_GAME,
	eAPGT_MAX
};

class cCURLUI :public NamedTypedObject
{
	cMyGuiEditBox*			m_pJsonEditBox;
	std::string				m_strGameCode;
	std::string				m_strGameName;
	std::string				m_strLineType;
	std::string				m_strProvider = "AP";
	std::string				m_strReleaseDate = "01-10-2025";
	bool					m_bJP;
	bool					m_bFreeGamePickUpFeature;
	bool					m_bCollectionFeature;
	std::vector<int>		m_BuyFeatureRateVector;
	float					m_fDoubleChanceRate = 1.2f;
	eAPGameType				m_eGameCategoryIds;
	std::string				m_strSpecialPlatformRtp = "";

	class cMyGuiRootNode* m_pMainUIRoot;
	//std::map<eEnv, sEnvData>		m_EnvDataMap;
	void							ParseData(const char* e_strFileName);
	//
	std::vector<std::string>		m_strEvnNameVector;
	void							JsonCompose();
	void							ParseUIFile(cImGuiNode* e_pNode);
	public:
	cCURLUI();
	virtual ~cCURLUI();
	void	Init();
	void	Update(float e_fElpaseTime);
	void	Render();
};