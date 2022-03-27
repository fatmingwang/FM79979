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
//https://gist.github.com/nus/564e9e57e4c107faa1a45b8332c265b9
//https://emscripten.org/docs/porting/networking.html
//https://blog.csdn.net/qq_42956179/article/details/118576680

//https://gist.github.com/floooh/360e884ea45c9868039c9ddb1343750d

class	cNetworkSample :public FATMING_CORE::cGameNetwork, public cMessageSender
{
	void								LostConnection();
	bool								CurrentBonsNetworkEventProcess(FATMING_CORE::sNetworkReceivedPacket*e_pData);
	bool								LoginNetworkEventProcess(FATMING_CORE::sNetworkReceivedPacket*e_pData);
public:
	DEFINE_TYPE_INFO();
	cNetworkSample();
	~cNetworkSample();
	virtual	void						Init()override;
	virtual	void						Update(float e_fElpaseTime)override;
	virtual	void						Destroy()override;
};