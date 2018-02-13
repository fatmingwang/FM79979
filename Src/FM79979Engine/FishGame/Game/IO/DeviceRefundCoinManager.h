#pragma once
//
#include "DeviceRefundCoinBase.h"
//

class cPlayerManager;

class	cDeviceRefoundCoinManager : public cNamedTypedObjectVector<cDeviceRefoundCoinBase>
{
private:
	//std::vector<sCoinRefundIOName*>		m_CoinRefundIONameVector;
	std::vector<sScoreBehaviorStruct*>	m_ScoreBehaviorStructVector;//pass to base
public:
	cDeviceRefoundCoinManager();
	~cDeviceRefoundCoinManager();
#ifdef IO_EDITOR
	//e_bRefundTicketOrCoin true for coin
	void	TestInit( const char *filename , cIOControlInterface*e_pIOControlInterface);
#endif
	//0 for ticket 1 for coin 2 for print
	void	GameInit(const char *filename, int e_iScoreToCoinOrTicketExchangeRate, int e_iRefundTicketOrCoinOrPrint, int e_iNumPlayer, cIOControlInterface*e_pIOControlInterface,cPlayerManager*e_pPlayerManager);

	void	Update( float e_fElpaseTime );
	void	Render();
	void	Destroy();

	bool	RefundTest(int e_iPlayerIndex,int e_iCount);
	bool	IsRefundCoin();
};