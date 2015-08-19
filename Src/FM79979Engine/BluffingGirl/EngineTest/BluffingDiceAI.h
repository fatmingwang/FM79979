#ifndef	_DICE_AI_H_
#define	_DICE_AI_H_

class	cDiceTable;
struct	sDiceRule;
class	cBluffingDiceGame;

class	cBluffingDiceAI
{
	friend class				cAIDiceTableCallResult;
	friend class				cBluffingDiceGame;
	//
	cMPDI*						m_pUseClairvoyantMPDI;
	std::vector<cDiceTable*>	m_DiceTableVector;
	cPuzzleImage*				m_pPI;
	//call by most same of dices
	sMinMaxData<float>			m_CallByAmount;
	sMinMaxData<float>			m_CheatToWin;
	bool						m_bEnableToCheat;//it use special item,cheat is disable
	sMinMaxData<int>			m_SkipStep;
	sMinMaxData<int>			m_ExtraVisibleDice;
	sMinMaxData<float>			m_SafeToCall;//although m_SkipStep is assign but we have to check is new call safe enough
	float						m_fWinCallPercentage;
	sDiceRule*					m_pDiceRule;
	//
	void						ProcessAIData(TiXmlElement*e_pElement);
	//
	int							GetVisibleMatchCount(int e_iCurrentDicePoint,bool e_bSpecialOne,int e_iDiceAIIndex,int*e_piMyMatchCount,int*e_piVisibleCount);
	float						CurrentCatchToWinPercentage(int e_iCurrentDiceCount,int e_iCurrentDicePoint,bool e_bSpecialOne,int e_iDiceAIIndex);
	int							MatchDicePointCount(int e_iPlayerIndex,int e_iCurrentDicePoint,bool e_bSpecialOne,int e_iVisibleCount);
	void						NewDiceData(int e_iMoveGap,int e_iCurrentDicePoint,int e_iCurrentDiceCount,int*e_piNewDiceCount,int*e_piNewDicePoint);
	bool						NewDiceDataByMostSameDices(bool e_bSpecialOneRule,int e_iDiceAIIndex,int e_iCurrentDicePoint,int e_iCurrentDiceCount,int*e_piNewDiceCount,int*e_piNewDicePoint);
public:
	cBluffingDiceAI(TiXmlElement*e_pElement,sDiceRule*e_pDiceRule);
	~cBluffingDiceAI();
	bool	IsCatchWin(int e_iCount,int e_iPoint,bool e_bSpecialDiceRule);
	void	AssignTableData(std::vector<cDiceTable*>*e_DiceTableVector);
	void	Update(int e_iCurrentDiceCount,int e_iCurrentDicePoint,bool e_bSpecialOneRule,int e_iDiceAIIndex);
	int		m_iDiceCountCallResult;
	int		m_iDicePointCallResult;
	bool	m_bCallOrCatch;//if true is call next round,if false catch last call.
	void	SetCheatEnable(bool e_b){ m_bEnableToCheat = e_b; }
	bool	IsCheatEnable(){ return m_bEnableToCheat; }
	std::vector<cDiceTable*>*	GetDiceTableVector(){return &m_DiceTableVector;}
};
//
class	cAIDiceTableCallResult
{
	//must delete!because the data is store at here
	cDiceTable*			m_pMyDiceTable;
	//
	cBluffingDiceAI*	m_pBluffingDiceAI;
	//
	std::vector<int>	m_iCallDicePointHistory;
	std::vector<int>	m_iLastCallDiceCountHistory;
	//
	cPuzzleImage*		m_pPI;
	cPuzzleImageUnit*	m_pDicePointImage;
	cPuzzleImageUnit*	m_pDiceCountImage;
	std::wstring		m_strDicePointImagePrefixName;
	std::wstring		m_strDiceCountPrefixName;
	//
	Vector3				m_vDicePointImagePos;
	Vector3				m_vDiceCountImagePos;
	bool				m_bWaitForAI;
	//index of dicetable
	int					m_iAIIndex;
	//
	cMPDI*				m_pWaitingAIRespond;
public:
	cAIDiceTableCallResult(cBluffingDiceAI*e_pBluffingDiceAI,TiXmlElement*e_pTiXmlElement,int e_iAIIndex);
	~cAIDiceTableCallResult();
	void				AIUpdate(int e_iCurrentDiceCount,int e_iCurrentDicePoint,bool e_bSpecialRule,float e_fElpaseTime);
	void				Render();
	void				Init();
	cDiceTable*			GetDiceTable(){return m_pMyDiceTable;}
	bool				IsWaitForAI(){return m_bWaitForAI;}
	void				SetWaitAI(bool e_b){m_bWaitForAI = e_b;}
	bool				IsCall();
	bool				IsCatch();
};

#endif