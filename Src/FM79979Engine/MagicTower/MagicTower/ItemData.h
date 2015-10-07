#ifndef _MAGIC_TOWER_ITEM__DATA_H_
#define _MAGIC_TOWER_ITEM__DATA_H_

	//class	cStatus:public NamedTypedObject
	//{
	//public:
	//	int	m_iHP;					//
	//	int	m_iDefence;				//
	//	int	m_iStrgeth;				//
	//	int	m_iLucky;				//
	//	int	m_iAgile;				//
	//	cStatus(){ m_iHP = m_iDefence = m_iStrgeth = m_iLucky = m_iAgile = 0; }
	//};

	struct	cItemDataInfo
	{
		WCHAR	strName[MAX_PATH];
		WCHAR	strStatusName[MAX_PATH];
		WCHAR	strImageName[MAX_PATH];
	};

	class	cItemData:public NamedTypedObject
	{
		GET_SET_DEC(cFAICharacterStatus*,m_pStatus,GetStatus,SetStatus);
		GET_SET_DEC(cBaseImage*,m_pImage,GetImage,SetImage);
	public:
		cItemData(){ m_pStatus = 0; m_pImage = 0; }
		//void	Update(float e_fElpasedTime);
		void	Render(){ m_pImage->Render(); }
	};

	//struct	sTriggerCondition
	//{
	//	//after talk those ID condition would be trigger if else is done
	//	std::vector<int>	*piAfterTalkIDVector;
	//	std::vector<int>	*piHaveItemIDVector;
	//	int					*piCount;
	//	sTriggerCondition(){piAfterTalkIDVector = 0;piHaveItemIDVector = 0; piCount = 0;}
	//};

#endif