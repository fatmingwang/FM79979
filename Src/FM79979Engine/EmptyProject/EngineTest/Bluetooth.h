#pragma once
//https://code.msdn.microsoft.com/windowsdesktop/Bluetooth-Connection-e3263296
#include <mutex>

DEFINE_GUID(GUID_DEVCLASS_BLUETOOTH, { 0xe0cbf06cL, 0xcd8b, 0x4647,{ 0xbb, 0x8a, 0x26, 0x3b, 0x43, 0xf0, 0xf9, 0x74 } });

struct sBluetoothPacket
{
	int			iSize;
	char*		pData;
	sBluetoothPacket();
	~sBluetoothPacket();
};

class cBluetoothSinglton:public cSingltonTemplate<cBluetoothSinglton>,public cCPP11Thread,public NamedTypedObject
{
	//for server
	std::vector<sBluetoothPacket*>	m_BluetoothPacketVector;
	std::mutex						m_BluetoothPacketVectorMutex;
	std::mutex						m_BluetoothSocketVectorMutex;
	std::vector<SOCKET>				m_ConnectedSocketVector;
	//
	bool							m_bLostConnection;
	SOCKET							m_LocalSocket;
	bool							SetDeviceName(const wchar_t*e_strName);
	void							ServerUpdate(float e_fElpaseTime);
	void							ClientUpdate(float e_fElpaseTime);
	void							ConnectToServerUpdate(float e_fElpaseTime);
	//
	cBluetoothSinglton();
	~cBluetoothSinglton();
public:
	SINGLETON_BASIC_FUNCTION(cBluetoothSinglton);
	DEFINE_TYPE_INFO();
	void	Init();
	void	Update(float e_fElpaseTime);
	//
	bool	CreateAsServer(const wchar_t*e_strServerName);
	bool	CreateAsClient(const wchar_t*e_strServerName);
	void	Disconnect();
	void	GetReceivedData(std::vector<sBluetoothPacket*>*e_pDataVector);
	void	FindDevices();
	void	SendDataToAllClient(int e_iLength,char*e_pData);
	void	SendDataToServer(int e_iLength, char*e_pData);
};