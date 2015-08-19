#ifndef	_PLAYER_DATA_H_
#define	_PLAYER_DATA_H_

//#include "FishCollection.h"
#include "FishRodAndFishThreadData.h"

class	cPlayerData
{
	int64				m_i64Money;
	int64				m_i64Gold;
	int64				m_i64Reel;
	//
	Vector3				m_vGoldShowPos;
	Vector3				m_vMonetShowPos;
	Vector3				m_vReelShowPos;
	cNumeralImage*		m_pNumeralImage;
public:
	//cNamedTypedObjectVector<cFishCollection>	m_FishCollectionVector;
	cPlayerData();
	~cPlayerData();
	void	SetRenderPos(Vector3 e_vGoldShowPos,Vector3	e_vMonetShowPos,Vector3	e_vReelShowPos,cNumeralImage*e_pNumeralImage);
	void	Render();
	bool	MoneyMinus(int e_iValue);
	void	MoneyAdded(int e_iValue);
	bool	GoldMinus(int e_iValue);
	void	GoldAdded(int e_iValue);
	bool	ReelMinus(int e_iValue);
	void	ReelAdded(int e_iValue);
	cNamedTypedObjectVector<cFishRodData>	m_FishRodDataVector;
	bool			ParsePlayerData();
};

#endif
