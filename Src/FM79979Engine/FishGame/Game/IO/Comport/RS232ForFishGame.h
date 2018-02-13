//#ifdef RS232_FRO_FISH_GAME
#pragma once

#include "ComportData.h"

#define	REFRESH_8051_IO_TIME_OFFSET				0.03f

class	cFUThreadPool;
class	cIOControlInterface;
//the order for m_SendingComportDataVector AddObject
//it should't like this but I am lazy.
enum eComportDataIndex
{
	eCDI_CoinRefundResetSendingComportData = 0,
	eCDI_CoinRefundSendingComportData,
	eCDI_DefaultRecevedComportData,
	eCDI_GameStickerSendingComportData,
	eCDI_PrintRefundSendingComportData,
	eCDI_CountRegisterComportData,
	eCDI_COMPORT_SENDING_RESERVE5,
	eCDI_MAX
};

class cRS232ForFishGame:public cNodeISAX
{
	friend class		cIOControlInterface;
	void				ProcessComportDCB(TiXmlElement*e_pTiXmlElement);
	virtual	bool		MyParse(TiXmlElement*e_pRoot);
	int					m_iDebugFlag;//0for not start,1 for open comport,2,3,4(coin refund,coin reset,common) for poll comport data data.5 for receving comport data.
	//debug info
	WCHAR				m_strError[MAX_PATH];
	std::wstring		m_strErrorInfo;
	//
	HANDLE				m_pComportHandler;
	int					m_iComportErrorCount;
	//
	float				m_fRestTimeToUpdate;
	cFUThreadPool*		m_pThread;
	bool				m_bStopThread;
	//
	HANDLE				OpenComPort(DCB*e_pDCB);
	bool				IsCommStateOk(DCB*e_pDCB,HANDLE e_Handle);
	//
	void				ErrorProcess(HANDLE e_Handle);
	//data for receive,acctually dont need those only need m_SendingComportDataVector,but I am lazy.
	cComportData		m_GameStickerSendingComportData;
	cComportData		m_CoinRefundResetSendingComportData;
	cComportData		m_CoinRefundSendingComportData;
	int					m_iNumNewSendingComportDataBuffer;
	cComportData		m_NewSendingComportDataBuffer[10];
	cComportData		m_DefaultRecevedComportData;
	//
	bool				m_bSendingRefundAndRefundingReset[eCDI_MAX];
	cNamedTypedObjectVector<cComportData>	m_SendingComportDataVector;
	bool				ComportSingnalSending(eComportDataIndex e_eComportDataIndex);
	//
	//return 0 is error,1 is sending ,2 is receving
	int					SingnalProcess(float e_fElpaseTime);
	bool				WriteSignal();
	bool				ReadSignal();
	//bool				RefundSingnalSending();
	//bool				RefundResetSingnalSending();
	bool				RequireIOStatusSingnalSending();
	bool				m_bIOError;
	DCB					m_DCB;
	bool				m_bSendSignalFlag;
	//bool				m_bCoinRefundSingnal;
	//bool				m_bCoinRefundResetSingnal;

	void				DebugRender(cComportData&e_cComportData, int e_iStartPosX,int e_iStartPosY);
	//while data is fetched send message to comport.
	bool				m_bDoUpdate;
	//
	template<class Type>bool				WriteIOWithComportDataIndex(int e_iAddress, Type e_Value, int e_iComportDataIndex);
	//becasue some PC has more than one comport...and I have no idea why,so it need to find out which comport can receving message
	int					m_iCurrentComportIndexToFindOutWhichComportCanRecevingMessage;
	int					m_iRecevingMessageComportIndex;
	std::vector<int>	m_iAvailableComportVector;
	int					m_iCurrentIOMode;
public:
	cRS232ForFishGame(int e_iCurrentIOMode);
	~cRS232ForFishGame();
	//its possible IO refresh too slow or some problem with comport,so we have to clear coin and ticker count
	void				ClearRefundAndCoinData();
	//
	void				Init(bool e_bResetCoin);
	bool				ReadIOBit(int e_iBitAddress,bool*e_pbData, const WCHAR*e_strComportDataName);
	bool				ReadIOChar(int e_iAddress,char*e_pcValue, const WCHAR*e_strComportDataName);
	bool				ReadIOShort(int e_iAddress,short*e_pshValue, const WCHAR*e_strComportDataName);
	bool				ReadIOInt(int e_iAddress, int*e_piValue, const WCHAR*e_strComportDataName);

	bool				WriteIOChar(int e_iAddress, char e_cValue,const WCHAR*e_strComportDataName);
	bool				WriteIOShort(int e_iAddress,short e_shValue, const WCHAR*e_strComportDataName);
	bool				WriteIOInt(int e_iAddress, int e_iValue, const WCHAR*e_strComportDataName);
	bool				WriteIOBit(int e_iAddress, char e_cValue, const WCHAR*e_strComportDataName);
	//bool				WriteIOBit(int e_iAddress,bool e_bValue);
	virtual void		Update(float e_fElpaseTime);
	virtual void		Render();
	bool				IsIOError(){return m_bIOError;}
	bool				IsStopThread() { return m_bStopThread; }

	int					GetCurrentIOMode() { return m_iCurrentIOMode; }
	
};
//#endif//end RS232_FRO_FISH_GAME