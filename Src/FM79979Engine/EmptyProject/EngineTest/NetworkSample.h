#pragma once
#include "../../Core/Network/GameNetWork.h"
//below code should be shared protocal
enum eGameNetworkMessageID
{
	eGNMI_C2S_LOGIN = 0,
	eGNMI_S2C_LOGIN_RESULT,
	eGNMI_MAX,
};

struct sGNMI_C2S_LOGIN
{
	char	strID[20];
	char	strPWD[20];
};

class	cNetworkSample :public FATMING_CORE::cGameNetwork,public NamedTypedObject, public cMessageSender
{
	void								LostConnection();
	bool								CurrentBonsNetworkEventProcess(FATMING_CORE::sNetworkReceivedPacket*e_pData);
	bool								LoginNetworkEventProcess(FATMING_CORE::sNetworkReceivedPacket*e_pData);
	cNetworkSample();
	~cNetworkSample();
public:
	DEFINE_TYPE_INFO();
	virtual	void						Init()override;
	virtual	void						Update(float e_fElpaseTime)override;
	virtual	void						Destroy()override;
};