#ifndef _IOCD_DRIVER_INTERFACE_H
#define _IOCD_DRIVER_INTERFACE_H

#include "IOData.h"

#define	BIT_COUNT								8

class	cIOSMIOManager;
//%x for hex value to decimal
class	cIOControlInterface:public cIODataManager
{
	bool	m_bIOError;
	friend class cIOSMIOManager;
	void	Destroy();
	std::map<std::wstring, int>	m_OriginalIONameAndAddressMap;
	void	SwapByName(bool e_bSwap,std::wstring e_strIO1Name, std::wstring e_strIO2Name,int e_iPlayerCount);
	//IGS	左右相反
	void	IOMode0(bool e_bSwap, int e_iPlayerCount);
	//A-Dong  上下相反, hopper & 彩票信號相反
	void	IOMode1(bool e_bSwap, int e_iPlayerCount);
	//
	void	IOMode2(bool e_bSwap, int e_iPlayerCount);
public:
	cIOControlInterface(int e_iCurrentIOMode);
	~cIOControlInterface();
	//
	using	cIODataManager::Init;
	void	Init(const char*e_strComportDataSetupFileName, const char*e_strIODeviceFileName);
	//JMGB
	//void	Init(GUID e_GUID,int e_iStartAddress,int e_iDataLength);
	//bool	WriteInputData(int e_iAddress,bool e_bOn);
	//update current IOData status
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	ClearIOData(float e_fElpaseTime);
	virtual	void	Render();
	//void	WriteGameKey();
	//void	WriteInputIOSetup();
	bool	ReadIOBit(int e_iAddress, bool*e_pbData, const WCHAR*e_strComportDataName);
	bool	ReadIOChar(int e_iAddress, char*e_pcData, const WCHAR*e_strComportDataName);
	bool	ReadIOShort(int e_iAddress, short*e_pshData, const WCHAR*e_strComportDataName);
	bool	ReadIOInt(int e_iAddress, int*e_piData, const WCHAR*e_strComportDataName);
	bool	WriteIOChar(int e_iAddress, char e_cData, const WCHAR*e_strComportDataName);
	bool	WriteIOShort(int e_iAddress, short e_cData, const WCHAR*e_strComportDataName);
	bool	WriteIOInt(int e_iAddress, int e_cData, const WCHAR*e_strComportDataName);
	bool	WriteIOBit(int e_iAddress, bool e_bData, const WCHAR*e_strComportDataName);
	bool	IsIOError(){return m_bIOError;}
	void	SetIOError(bool e_bIOError){m_bIOError = e_bIOError;}
	//basibly you won't use this,this is for editor
	void*	GetIOInstance();
	//some bit are different between manufacturers
	void	SwitchIOMode(int e_iType, int e_iPlayerCount);
	//avoid any action before game start
	void	Reset();
};

const int	g_iBitAddressValue[8] = { 1 << 0,1 << 1,1 << 2,1 << 3,1 << 4,1 << 5,1 << 6,1 << 7 };

inline DWORD	GetBudaRate(int e_iValue)
{
	DWORD l_dwResult = -1;
	switch (e_iValue)
	{
		case 110:		l_dwResult = CBR_110;		break;
		case 300:		l_dwResult = CBR_300;		break;
		case 600:		l_dwResult = CBR_600;		break;
		case 1200:		l_dwResult = CBR_1200;		break;
		case 2400:		l_dwResult = CBR_2400;		break;
		case 4800:		l_dwResult = CBR_4800;		break;
		case 9600:		l_dwResult = CBR_9600;		break;
		case 14400:		l_dwResult = CBR_14400;		break;
		case 19200:		l_dwResult = CBR_19200;		break;
		case 38400:		l_dwResult = CBR_38400;		break;
		case 56000:		l_dwResult = CBR_56000;		break;
		case 57600:		l_dwResult = CBR_57600;		break;
		case 115200:	l_dwResult = CBR_115200;	break;
		case 128000:	l_dwResult = CBR_128000;	break;
		case 256000:	l_dwResult = CBR_256000;	break;
		default:									break;
	}
	return l_dwResult;
}

#endif