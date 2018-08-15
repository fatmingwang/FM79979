#pragma once

#include <mutex>
#include "../../Core/Network/SDL_net.h"
#ifdef WIN32
//https://code.msdn.microsoft.com/windowsdesktop/Bluetooth-Connection-e3263296
DEFINE_GUID(GUID_DEVCLASS_BLUETOOTH, { 0xe0cbf06c, 0xcd8b, 0x4647,{ 0xbb, 0x8a, 0x26, 0x3b, 0x43, 0xf0, 0xf9, 0x74 } });
#endif

enum eBluetoothConnectionStatus
{
	eBTCS_NONE = 0,
	eBTCS_CONNECTED,
	eBTCS_CONNECT_FAILED,
	eBTCS_LOST_CONNECTION,
	eBTCS_TRYING_TO_CONNECT,
	eBTCS_SELECT_DEVICE,
	eBTCS_MAX,
};

struct sBluetoothPacket
{
	int			iSize;
	char*		pData;
	sBluetoothPacket(int e_iSize,char*e_pData);
	~sBluetoothPacket();
};

class cBluetoothSinglton:public cSingltonTemplate<cBluetoothSinglton>,public cCPP11Thread,public NamedTypedObject
{
	eBluetoothConnectionStatus		m_eBluetoothConnectionStatus;
	//for server
	bool							CreateSocksetToListenData();
	std::vector<sBluetoothPacket*>	m_BluetoothPacketVector;
	std::mutex						m_BluetoothPacketVectorMutex;
	std::mutex						m_BluetoothSocketVectorMutex;
	//
	_SDLNet_SocketSet*				m_pAllSocketToListenClientMessage;
	std::vector<SDLNet_Socket*>		m_ConnectedSocketVector;
	//
	bool							m_bLostConnection;
	SDLNet_Socket					m_LocalSocket;
	bool							SetDeviceName(const wchar_t*e_strName);
	void							ServerUpdate(float e_fElpaseTime);
	void							ClientUpdate(float e_fElpaseTime);
	//
	cBluetoothSinglton();
	~cBluetoothSinglton();
public:
	SINGLETON_BASIC_FUNCTION(cBluetoothSinglton);
	DEFINE_TYPE_INFO();
	void	Init();
	void	Update(float e_fElpaseTime);
	void	Render(int e_iPosX, int e_iPosY);
	//
	bool	CreateAsServer(const wchar_t*e_strServerName);
	bool	CreateAsClient(const wchar_t*e_strServerName);
	void	Disconnect();
	void	GetReceivedData(std::vector<sBluetoothPacket*>*e_pDataVector);
	void	FindDevices();
	void	SendDataToAllClient(int e_iLength,char*e_pData);
	void	SendDataToServer(int e_iLength, char*e_pData);
	void	DebugRender();
	void	SetBluetoothConnectionStatus(eBluetoothConnectionStatus e_eBluetoothConnectionStatus) { m_eBluetoothConnectionStatus = e_eBluetoothConnectionStatus; }
#ifdef ANDROID
	void	Android_AddBluetoothPacket(int e_iSize,char*e_pData);
#endif
};