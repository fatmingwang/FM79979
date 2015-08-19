#ifndef	_FISH_ROD_AND_FISH_THREAD_DATA_H_
#define	_FISH_ROD_AND_FISH_THREAD_DATA_H_

enum	eFishRodType
{
	eFRT_BEGINNER = 0,	//L"Begginer"
	eFRT_NONE
};

class	cFishRodData:public NamedTypedObject
{
public:
	struct	sFishLeaveProbability
	{
		std::wstring	strFishName;
		float			fLeaveProbability;
	};
	std::vector<sFishLeaveProbability>	m_FishLeaveProbabilityVector;
	struct	sLVData
	{
		//for upgrade
		int		iUpgradeMoneyCost;
		float	fMoneyProbability;
		int		iUpgradeGoldCost;
		float	fGoldProbability;
		//
		int		iLevelIndex;
		float	fBrakeSpeed;
		float	fPowerTime;
		int		iFishLeaveCost;
		float	fHookRisePercent;
		static sLVData*	ProcessData(TiXmlElement*e_pElement);
		sLVData(){  }
		~sLVData(){  }
	};
protected:
	//eFishRodType			m_eFishRodType;
	cRenderObject*	m_pImage;//image to show
	TiXmlElement*			m_pImageElement;
	std::vector<sLVData*>	m_LeaveDataVector;
	std::wstring			m_strDescription;
	int						m_iCurrentLV;
public:
	cFishRodData(TiXmlElement*e_pElement);
	~cFishRodData();
	sLVData*			GetLVData(int e_iLeavel);
	sLVData*			GetCurrentLVData();
	float				GetLeaveFishProbability(const WCHAR*e_strFishName);
	bool				IsFishLeaving(const WCHAR*e_strFishName);
};

class	cFishThread:public NamedTypedObject
{
public:
	cFishThread(TiXmlElement*e_pElement);
	~cFishThread();
	std::wstring			m_strDescription;
	float					m_fSinkTime;
	float					m_fSinkSpeed;
	float					m_fFloatTime;
	float					m_fFloatSpeed;
	float					m_fEnergy;
	float					m_fCost;
	float					GetTotalDepth(){ return m_fSinkTime*m_fSinkSpeed; }
	cRenderObject*	m_pImage;
};
//
//
//
class	cFishRodAndFishThreadData:public ISAXCallback
{
	virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
public:
	cNamedTypedObjectVector<cFishRodData>	m_FishRodDataVector;
	cNamedTypedObjectVector<cFishThread>	m_FishThreadVector;
	cFishRodData*							m_pCurrentSelectedFishRodData;
	cFishThread*							m_pCurrentSelectedcFishThread;

	cFishRodAndFishThreadData();
	~cFishRodAndFishThreadData();

};

#endif