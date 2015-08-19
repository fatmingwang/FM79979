#ifndef	_PLAYER_DATA_H_
#define	_PLAYER_DATA_H_
class	cPlayerData;
//<Item Name="Clairvoyant" Maxium="99" Cosumble="1">
//	<cImageButton PI="Image/Item/Item.pi" Image="Clairvoyant" Pos="900,0">
//</Item>
class	cCosumbleItem:public cClickMouseBehavior
{
	cTimeNumerialImage*	m_pTimeCounter;
	friend class		cPlayerData;
	int					m_iCount;
	int					m_iMaximumCount;
	float				m_fRecoverTime;
	float				m_fRestRecoverTime;	//if -1 no recover time.
	timeval				m_LastTimeGetItem;
	int					m_iRecoverReachedTimeAddItemCount;
	//if false button cannot be touched
	bool				m_bCosumbleable;
	cImageButton*		m_pImageButton;
	cNumeralImage*		m_pNumeralImage;
	//if true 
	bool				m_bItemChanged;
	int					m_iLessThanToRecovery;//while count is less than this value recovery item count
public:
	cCosumbleItem(TiXmlElement*e_pelement);
	cCosumbleItem(int e_iMaximumCount,float e_fRecoverTime);
	~cCosumbleItem();
	virtual void		Init();
	//if true write time stamp
	void				Update(float e_fElpaseTime);
	void				Render();
	void				RenderRestTime();
    virtual void		MouseMove(int e_iPosX,int e_iPosY);
    virtual void		MouseDown(int e_iPosX,int e_iPosY);
    virtual void		MouseUp(int e_iPosX,int e_iPosY);
	bool				ItemAdd(int e_iCount,bool e_bItemChangeByTime);
	bool				ItemMinus(int e_iCount,bool e_bItemChangeByTime);
	bool				IsAddable(int e_iCount);
	bool				IsMinusable(int e_iCount);
};

//<PlayerData>
//	<Item Maxium="" RecoverTime="">
//</PlayerData>

class	cPlayerData:public ISAXCallback
{
	void			ProcessItemData(TiXmlElement*e_pTiXmlElement);
	virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
	//Calendar c = Calendar.getInstance(); 
	//int seconds = c.get(Calendar.SECOND);
	//http://stackoverflow.com/questions/13064750/how-to-get-current-time-from-internet-in-android
	cCosumbleItem*								m_pHeart;
	cCosumbleItem*								m_pClairvoynat;
	cCosumbleItem*								m_pShockTime;
	cClickMouseBehaviorVector<cCosumbleItem>	m_AllItems;
	//
	bool										m_bUsingClairvoyant;

	bool										ParsePlayerSaveFile();
	void										Write();
	cPuzzleImage*								m_pPI;
	//
	GET_SET_DEC(bool,m_bItemClickEnable,IsItemClickEnable,SetItemClickEnable);
	//
	cSkipHeaderBinaryFile*					m_pItemFile;
	std::string								m_strPlayerSaveFileName;
	cMPDI*									m_pUIBar;
public:
	cPlayerData(char*e_strPlayerDataFileName,char*e_strPlayerSaveFileName);
	~cPlayerData();
	void			Init();
	void			Update(float e_fElpaseTime);
	void			Render();
	void			RenderBG();

	void			MouseDown(int e_iX,int e_iY);
	void			MouseUp(int e_iX,int e_iY);
	void			MouseMove(int e_iX,int e_iY);

	bool			SaveFile();
	bool			HeartMinus();
	bool			HeartAdd(int e_iCount,bool e_bWriteTimeStame);
	int				HeartCount();

	bool			ShockTimeMinus();

	bool			IsUsingClairvoyant();
	int				ClairvoyantCount();
	bool			ClairvoyantAdd(int e_iCount);
	bool			UsingClairvoyant();
	bool			m_bEnableRender;
};

#endif