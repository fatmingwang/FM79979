#ifndef	_DICE_TABLE_H_
#define	_DICE_TABLE_H_

struct	sDiceRule
{
	int		iMin;
	int		iMax;
	float	fPercentForSpecialNumeral1;
	int		iNumDice;
	int		iAllDiceCount;
	//<DiceRule Min="1" Max="6" OneDiceRule="0" 
	sDiceRule(TiXmlElement*e_pelement);
	~sDiceRule(){}
};

//game diffculy dice rand reference(more dice 1 more hard) (?%)
//the AI call Dice1 probability (?%0
//level to to call next dice?(1-n),ex: Dice 2 has 5 next to call Dice n has 5+n
class	cBluffingDiceAI;
class	cBluffingDiceGame;
class	cBluffingDiceMultiPlayerGame;
enum	eBehaviorType
{
	eBT_PLAYER = 0,
	eBT_AI,
	eBT_MAX,
};
class	cDiceTable
{
	eBehaviorType								m_eBehaviorType;
	struct	sCallBehavior
	{
		cSubMPDI*								pBGSubMPDI;
		cSubMPDI*								pNumberSubMPDI;
		cSubMPDI*								pDiceSubMPDI;
		std::wstring							strNumberPrefix;
		std::wstring							strDicePrefix;
		sCallBehavior();
		~sCallBehavior();
		void	Init();
		void	ChangeData(int e_iDiceCount,int e_iDicePoint);
		void	Render();
		void	Disable();
	};
	cMPDI*										m_pShowMPDI;
	//
	friend class								cBluffingDiceAI;
	friend class								cBluffingDiceGame;
	friend class								cBluffingDiceMultiPlayerGame;
	//
	cPuzzleImageUnit*							m_pSelectedNumeralImage;
	cPuzzleImageUnit*							m_pSelectedDiceImage;
	//
	sDiceRule*									m_pDiceRule;
	int											m_iCurrentSelectDiceCount;
	int											m_iCurrentSelectDicePoint;
	//
	void										ProcessImageData(TiXmlElement*e_pElement);
	void										ProcessPosData(TiXmlElement*e_pElement);
	void										ProcessCallHintData(TiXmlElement*e_pElement);
	//
	//
	cClickMouseBehaviorVector<cImageButton>		m_DiceImageButtonContainer;
	cClickMouseBehaviorVector<cImageButton>		m_NumeralImageButton;
	sCallBehavior								m_CallBehavior;
	//
	Vector3										m_vMyDiceImagePos;
	float										m_fMyDiceImageGap;
	std::wstring								m_strMyDiceImagePrefixName;
	std::wstring								m_strDiceImageButtonPrefixName;
	//
	std::vector<int>							m_MyDiceTable;
	cBehaviorObjectList<cBaseImage>				m_MyDiceTableImage;
	void										GenerateDiceTableImages();
	//
	cPuzzleImageUnit*							m_pBlockImage;
	cPuzzleImage*								m_pPI;
	//if true render will be m_DiceImageVector or m_pBlockImage
	bool										m_bReveal;
	//
	bool										IsCallConditionOk(int e_iCurrentDicePoint,int e_iCurrentDiceCount,int e_iNewDicePoint,int e_iNewDiceCount,int e_iTotalPlayer);
public:
	//int e_iNumDice,float e_fPrecent
	cDiceTable(TiXmlElement*e_pElement,sDiceRule*e_pDiceRule,eBehaviorType e_eBehaviorType);
	~cDiceTable();
	void										Init();
	void										Update(float e_fElpaseTime);
	void										Render();
	void										ClickButtonRender();
	void										DebugRender();
	void										MouseDown(int e_iX,int e_iY);
	void										MouseUp(int e_iX,int e_iY);
	void										MouseMove(int e_iX,int e_iY);
	int											GetCurrentSelectNumeral();
	int											GetCurrentSelectDice();
	void										SetReveal(bool e_bReveal){m_bReveal = e_bReveal;}
	bool										IsReveal(){return m_bReveal;}
	bool										IsConditionOk(int e_iCurrentDicePoint,int e_iCurrentDiceCount,int e_iTotalPlayer);
	void										SetDiceTable(std::vector<int> e_DiceContent);
	void										ChangeCallData(int e_iDiceCount,int e_iDicePoint);
	std::vector<int>							GetMyDiceContent(){return m_MyDiceTable;}
	void										SetMyDiceContent(std::vector<int> e_DiceTable){m_MyDiceTable = e_DiceTable;}
};

#endif