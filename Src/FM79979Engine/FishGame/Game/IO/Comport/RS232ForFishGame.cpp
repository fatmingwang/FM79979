#include "stdafx.h"
#include "RS232ForFishGame.h"
#include "IOControlInterface.h"
#include "../RegisterManager.h"
cRS232ForFishGame*g_pRS232ForFishGame = nullptr;
cIOControlInterface*g_pIOSMDriverInterface = nullptr;


void	IOWorkThread( size_t _workParameter, size_t _pUri )
{
	cRS232ForFishGame *l_pRS232ForFishGame = (cRS232ForFishGame*)_workParameter;
	UT::sTimeAndFPS	l_sTimeAndFPS;
	l_sTimeAndFPS.Update();
	while( !l_pRS232ForFishGame->IsStopThread())
	{
		l_sTimeAndFPS.Update();
		l_pRS232ForFishGame->Update(l_sTimeAndFPS.fElpaseTime);
		cRegisterManager::GetInstanceCallUpdateForWriteFile(l_sTimeAndFPS.fElpaseTime);
	}
}

void	IOWorkThreadDone(size_t _workParameter, size_t _pUri)
{
	cGameApp::OutputDebugInfoString("cRS232ForFishGame thread Stop");
}

cRS232ForFishGame::cRS232ForFishGame(int e_iCurrentIOMode)
{
	m_iCurrentIOMode = e_iCurrentIOMode;
	m_iCurrentComportIndexToFindOutWhichComportCanRecevingMessage = -1;
	m_iRecevingMessageComportIndex = -1;
	cGameApp::OutputDebugInfoString("cRS232ForFishGame created");
	m_iNumNewSendingComportDataBuffer = 0;
	m_SendingComportDataVector.SetFromResource(true);
	for (int i = 0; i < eCDI_MAX; ++i)
		m_bSendingRefundAndRefundingReset[i] = false;
	m_pThread = nullptr;
	m_iDebugFlag = 0;
	m_bIOError = false;
	m_pComportHandler = nullptr;
	this->m_fRestTimeToUpdate = 0.0f;
	m_bDoUpdate = true;
	m_iComportErrorCount = 0;
	m_bSendSignalFlag = true;
	m_bStopThread = true;
	SecureZeroMemory(&m_DCB, sizeof(DCB));

	for (int i = 0; i < 7; ++i)
	{
		std::wstring l_strComportName = L"COM";
		l_strComportName += ValueToStringW(i);
		HANDLE l_Handle = CreateFile(l_strComportName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (l_Handle != INVALID_HANDLE_VALUE)
		{
			m_iAvailableComportVector.push_back(i);
			CloseHandle(l_Handle);
		}
	}
}

cRS232ForFishGame::~cRS232ForFishGame()
{
	cGameApp::OutputDebugInfoString(L"~cRS232ForFishGame()");
	this->m_bStopThread = true;
	//wait thread stop!.
	Sleep(500);
	SAFE_DELETE(m_pThread);
	if (this->m_pComportHandler)
	{
		CloseHandle(m_pComportHandler);
		this->m_pComportHandler = nullptr;
	}
	cRegisterManager::DeleteInstance();
}
//<IODataSetup ByteSize="8" BaudRate="115200">
//	<RecevedComportData TotalDataSize="56" DefaultComportBuffer="0XAA,0xF0,0,1,0,0,0,0,2,0,0,0,0,3,0,0,0,0,4,0,0,0,0,5,0,0,0,0,6,0,0,0,0,7,0,0,0,0,8,0,0,0,0,9,0,0,0,0,10,0,0,0,0,0,0x68 0x16" m_iDataStartIndex="3" m_iEachDataSize="5" m_iCheckSumIndexOfDataArray="52" StartCommandBuffer="0Xaa,0xf0" EndCommandBuffer="0x68,0x16" EndCommandBufferIndex="52"/>
//	<GameStickerSendingComportData TotalDataSize="15" DefaultComportBuffer="0xAA,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0x68,0x16" m_iDataStartIndex="2" m_iEachDataSize="1" m_iCheckSumIndexOfDataArray="12" StartCommandBuffer="0xaa,0xf0" EndCommandBuffer="0x68,0x16" EndCommandBufferIndex="13"/>
//	<CoinRefundResetSendingComportData TotalDataSize="15" DefaultComportBuffer="0xaa,0xf2,0,0,0,0,0,0,0,0,0,0,0,0x68 0x16" m_iDataStartIndex="2" m_iEachDataSize="1" m_iCheckSumIndexOfDataArray="12" StartCommandBuffer="2" EndCommandBuffer="0x68,0x16" EndCommandBufferIndex="13"/>
//	<CoinRefundSendingComportData TotalDataSize="25" DefaultComportBuffer="0xAA,0xF1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x68 0x16" m_iDataStartIndex="2" m_iEachDataSize="2" m_iCheckSumIndexOfDataArray="22" StartCommandBuffer="2" EndCommandBuffer="0x68,0x16" EndCommandBufferIndex="21"/>
//</IODataSetup>

void	cRS232ForFishGame::ProcessComportDCB(TiXmlElement*e_pTiXmlElement)
{
	m_DCB.DCBlength = sizeof(DCB);
	m_DCB.StopBits = ONESTOPBIT;
	m_DCB.Parity = NOPARITY;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("ByteSize")
		{
			m_DCB.ByteSize = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("BaudRate")
		{
			m_DCB.BaudRate = GetBudaRate(VALUE_TO_INT);
		}
		else
		COMPARE_NAME("StopBits")
		{
			m_DCB.StopBits = VALUE_TO_INT;
		}
	PARSE_NAME_VALUE_END
}

bool	cRS232ForFishGame::MyParse(TiXmlElement*e_pRoot)
{
	ProcessComportDCB(e_pRoot);
	e_pRoot = e_pRoot->FirstChildElement();
	m_iNumNewSendingComportDataBuffer = 0;
	m_SendingComportDataVector.Destroy();
	while (e_pRoot)
	{
		const wchar_t*l_strName = e_pRoot->Value();
		COMPARE_NAME("DefaultRecevedComportData")
		{
			m_DefaultRecevedComportData.ParseData(e_pRoot);
		}
		else
		COMPARE_NAME("GameStickerSendingComportData")
		{
			m_GameStickerSendingComportData.ParseData(e_pRoot);
			m_GameStickerSendingComportData.DoAssignCheckSum();
		}
		else
		COMPARE_NAME("CoinRefundResetSendingComportData")
		{
			m_CoinRefundResetSendingComportData.ParseData(e_pRoot);

		}
		else
		COMPARE_NAME("CoinRefundSendingComportData")
		{
			m_CoinRefundSendingComportData.ParseData(e_pRoot);
		}
		else
		{
			//OutputDebugString(l_strName);
			//OutputDebugString(L":");
			//OutputDebugString(ValueToStringW(m_iNumNewSendingComportDataBuffer).c_str());
			//OutputDebugString(L"\n");
			m_NewSendingComportDataBuffer[m_iNumNewSendingComportDataBuffer].ParseData(e_pRoot);
			++m_iNumNewSendingComportDataBuffer;
		}
		e_pRoot = e_pRoot->NextSiblingElement();
	}
	m_SendingComportDataVector.AddObjectNeglectExist(&m_CoinRefundResetSendingComportData);
	m_SendingComportDataVector.AddObjectNeglectExist(&m_CoinRefundSendingComportData);
	m_SendingComportDataVector.AddObjectNeglectExist(&m_DefaultRecevedComportData);
	m_SendingComportDataVector.AddObjectNeglectExist(&m_GameStickerSendingComportData);
	assert(m_DefaultRecevedComportData.GetiTotalComportBufferSize()>0&&"CoinRefundSendingComportData data error");
	assert(m_GameStickerSendingComportData.GetiTotalComportBufferSize()>0 && "GameStickerSendingComportData data error");
	assert(m_CoinRefundResetSendingComportData.GetiTotalComportBufferSize()>0 && "CoinRefundResetSendingComportData data error");
	assert(m_CoinRefundSendingComportData.GetiTotalComportBufferSize()>0 && "CoinRefundSendingComportData data error");
	for (int i = 0; i<m_iNumNewSendingComportDataBuffer; ++i)
		m_SendingComportDataVector.AddObjectNeglectExist(&m_NewSendingComportDataBuffer[i]);
	g_pIOSMDriverInterface->Init();
	return true;
}

void	cRS232ForFishGame::Init(bool e_bResetCoin)
{
	if (!cGameApp::m_sbDeviceExist)
		return;	
	if (this->m_pComportHandler)
	{
		Sleep(100);
		CloseHandle(m_pComportHandler);
	}
	this->m_pComportHandler = nullptr;
	m_pComportHandler = OpenComPort(&m_DCB);
	if( !m_pComportHandler)
	{
		m_bIOError = true;
		std::wstring	l_strError;
		l_strError += L"comport pen failed , ";
		l_strError += this->m_strError;
		cGameApp::OutputDebugInfoString(l_strError);
		Sleep(1000);
	}
	else
	if( e_bResetCoin )
	{
		m_bSendingRefundAndRefundingReset[eCDI_CoinRefundResetSendingComportData] = true;
		this->m_CoinRefundResetSendingComportData.SetSpecificBufferBitDataToForResetAllPlayerRefundData();
		for (int i = 0; i < m_iNumNewSendingComportDataBuffer; ++i)
			m_NewSendingComportDataBuffer[i].SetSpecificBufferBitDataToForResetAllPlayerRefundData();
	}
	if( !m_pThread )
	{
		m_pThread = new cFUThreadPool();
		m_pThread->Spawn(1);
		m_bStopThread = false;
		FUStaticFunctor2<size_t, size_t, void>* workFunctor = new FUStaticFunctor2<size_t, size_t, void>(&IOWorkThread);
		FUStaticFunctor2<size_t, size_t, void>* doneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&IOWorkThreadDone);
		m_pThread->ExecuteWork(workFunctor,doneFunctor,(size_t)this,0);
	}
}

bool	cRS232ForFishGame::IsCommStateOk(DCB*e_pDCB,HANDLE e_Handle)
{
	//  Initialize the DCB structure.
	BOOL fSuccess = GetCommState(e_Handle,e_pDCB);
	if (!fSuccess)
	{
		//  Handle the error.
		swprintf (m_strError,MAX_PATH, L"GetCommState failed with error %d.\n", GetLastError());
		return false;
	}
	return true;
}

HANDLE	cRS232ForFishGame::OpenComPort(DCB*e_pDCB)
{
	m_iDebugFlag = 1;
	HANDLE	l_pFileHandler = nullptr;
	//if comport receved message we know correct index,so only need this comport
	//if (m_iRecevingMessageComportIndex != -1)
	//{
	//	m_iAvailableComportVector.clear();
	//	m_iAvailableComportVector.push_back(m_iRecevingMessageComportIndex);
	//}
	//find out all possible comport and if available comport count is bigger than 1 check all possible comport index.
	int l_iSize = (int)m_iAvailableComportVector.size();
	for (int i = 0; i < l_iSize; ++i)
	{
		std::wstring l_strComportName = L"COM";
		int l_iComportIndex = m_iAvailableComportVector[i];
		l_strComportName += ValueToStringW(l_iComportIndex);
		HANDLE l_Handle  = CreateFile(l_strComportName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (l_Handle != INVALID_HANDLE_VALUE)
		{
			if (l_iSize >= 2)
			{
				if (m_iCurrentComportIndexToFindOutWhichComportCanRecevingMessage != l_iComportIndex)
				{
					m_iCurrentComportIndexToFindOutWhichComportCanRecevingMessage = l_iComportIndex;
					l_pFileHandler = l_Handle;
					break;
				}
				else
				{
					CloseHandle(l_Handle);
				}
			}
			else
			{
				l_pFileHandler = l_Handle;
				m_iCurrentComportIndexToFindOutWhichComportCanRecevingMessage = l_iComportIndex;
				break;
			}
		}
	}
	//OutputDebugString(L"comport index:");
	//OutputDebugString(ValueToStringW(m_iCurrentComportIndexToFindOutWhichComportCanRecevingMessage).c_str());
	//OutputDebugString(L"\n");
	if( !l_pFileHandler || l_pFileHandler == INVALID_HANDLE_VALUE)
	{
		swprintf(m_strError, MAX_PATH, L"CreateFile failed with error %d.", GetLastError());
		return nullptr;
	}
	SecureZeroMemory(e_pDCB, sizeof(DCB));
	e_pDCB->DCBlength = sizeof(DCB);
	if (!GetCommState(l_pFileHandler,e_pDCB))
	{
		//  Handle the error.
		swprintf(m_strError, MAX_PATH, L"GetCommState failed with error %d.\n", GetLastError());
		return 0;
	}
	e_pDCB->BaudRate = CBR_115200;     //  baud rate
	e_pDCB->ByteSize = 8;             //  data size, xmit and rcv
	e_pDCB->Parity = NOPARITY;      //  parity bit
	e_pDCB->StopBits = ONESTOPBIT;    //  stop bit
	//  Fill in some DCB values and set the com state: 
	//  38400 bps, 8 data bits, no parity, and 1 stop bit.
	BOOL fSuccess = SetCommState(l_pFileHandler, e_pDCB);
	if (!fSuccess) 
	{
	  //  Handle the error.
		swprintf (m_strError,MAX_PATH, L"SetCommState failed with error %d.\n", GetLastError());
		return nullptr;
	}
	////  Get the comm config again.
	fSuccess = GetCommState(l_pFileHandler, e_pDCB);
	if (!fSuccess) 
	{
		//  Handle the error.
		swprintf (m_strError,MAX_PATH, L"GetCommState failed with error %d.\n", GetLastError());
		return nullptr;
	}
	//swprintf(m_strReceive, MAX_PATH, L"");
	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = 1;
	timeouts.ReadTotalTimeoutMultiplier = 1;
	timeouts.ReadTotalTimeoutConstant = 1;
	timeouts.WriteTotalTimeoutMultiplier = 1;
	timeouts.WriteTotalTimeoutConstant = 1;
	if (!SetCommTimeouts(l_pFileHandler, &timeouts))
	{
		swprintf(m_strError,MAX_PATH,L"setting port time-outs.(%d)", GetLastError());
		return nullptr;
	}
	swprintf(m_strError,MAX_PATH,L"Open Com1 Success.(%d)", GetLastError());
	//clear comport data
	PurgeComm(l_pFileHandler,PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
	//wait for a while avoid too fast.
	Sleep(100);
	return l_pFileHandler;
}

void	cRS232ForFishGame::ErrorProcess(HANDLE e_Handle)
{
	PurgeComm(e_Handle,PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
	++m_iComportErrorCount;
	//if(m_iComportErrorCount >= 5 )
	m_bIOError = true;
	if (m_iComportErrorCount % 4 == 0)
	{
		this->Init(false);
	}
}

void	cRS232ForFishGame::ClearRefundAndCoinData()
{
	//m_DefaultRecevedComportData.SetSpecificBufferDataToZeroAvoidCoinOrTickerNotCorrect();
	m_CoinRefundResetSendingComportData.CopyDefaultBufferToComportBuffer();
	m_CoinRefundSendingComportData.CopyDefaultBufferToComportBuffer();
	for (int i = 0; i < m_iNumNewSendingComportDataBuffer; ++i)
	{
		m_NewSendingComportDataBuffer[i].CopyDefaultBufferToComportBuffer();
		m_NewSendingComportDataBuffer[i].SetSpecificBufferDataToZeroAvoidCoinOrTickerNotCorrect();
	}
}

bool	cRS232ForFishGame::WriteSignal()
{
	return true;
}

bool	cRS232ForFishGame::ReadSignal()
{
	return true;
}

int	cRS232ForFishGame::SingnalProcess(float e_fElpaseTime)
{
	//IO Hopper	Require	serial Data		
	HANDLE	l_Handle = m_pComportHandler;
	bool	l_bSignalProcessResult = true;
	int		l_iProcessResult = 0;
	BOOL	l_bRecevingCheckSumOk = true;
	//polling write
	if(!IsCommStateOk(&this->m_DCB,l_Handle))
	{
		ErrorProcess(l_Handle);
		return l_iProcessResult;
	}
	if (l_Handle)
	{
		if (m_bSendSignalFlag)
		{
			l_iProcessResult = 1;
			m_iDebugFlag = 2;
			for (int i = 0; i < eCDI_MAX; ++i)
			{
				m_iDebugFlag = 3;
				if (m_bSendingRefundAndRefundingReset[i])
				{
					if (!ComportSingnalSending((eComportDataIndex)i))
					{
						l_bSignalProcessResult = false;
						break;
					}
					Sleep(10);
				}
			}
			if (l_bSignalProcessResult)
			{
				ClearRefundAndCoinData();
				l_bSignalProcessResult = RequireIOStatusSingnalSending();
				m_iDebugFlag = 4;
			}
		}
		else
		{
			m_iDebugFlag = 5;
			l_iProcessResult = 2;
			BOOL	l_bReadOk = false;
			bool	l_bLengthSame = false;
			DWORD	dwBytesReceived = 0;//start bit value
			//polling read
			l_bReadOk = ReadFile(l_Handle, m_DefaultRecevedComportData.GetComportBuffer(), m_DefaultRecevedComportData.GetiTotalComportBufferSize(), &dwBytesReceived, NULL);
			if (dwBytesReceived == m_DefaultRecevedComportData.GetiTotalComportBufferSize())
				l_bLengthSame = true;
			l_bRecevingCheckSumOk = m_DefaultRecevedComportData.IsCheckSumIsLegal();
			//not necessary
			if (!l_bLengthSame || !l_bRecevingCheckSumOk)
			{
				l_bSignalProcessResult = false;
				l_iProcessResult = 0;
				m_strErrorInfo = L"SingnalProcess ReadFile m_DefaultRecevedComportData length not correct!";
				cGameApp::OutputDebugInfoString(m_strErrorInfo);
			}
			else
			if (l_bReadOk)
			{
				if (l_bRecevingCheckSumOk)
				{
					m_strErrorInfo[0] = 0;
					m_iRecevingMessageComportIndex = m_iCurrentComportIndexToFindOutWhichComportCanRecevingMessage;
				}
				else
				{
					m_strErrorInfo = L"SingnalProcess ReadFile m_DefaultRecevedComportData chceck sum not correct!";
					cGameApp::OutputDebugInfoString(m_strErrorInfo);
					l_bSignalProcessResult = false;
				}
			}
			else
			{
				m_strErrorInfo = L"SingnalProcess ReadFile m_DefaultRecevedComportData failed!";
				cGameApp::OutputDebugInfoString(m_strErrorInfo);
				l_bSignalProcessResult = false;
			}
		}
		//
		if (l_bSignalProcessResult )
		{
			//only check receieve message
			if (l_bRecevingCheckSumOk )
			{
				if (!m_bSendSignalFlag)
				{
					m_iComportErrorCount = 0;
					this->m_strError[0] = 0;
					this->m_bIOError = false;
				}
			}
			else
			{
				ErrorProcess(l_Handle);
			}
			m_bSendSignalFlag = !m_bSendSignalFlag;
		}
		else
		{//send fail so always go sending.
			m_bSendSignalFlag = true;
			ErrorProcess(l_Handle);
		}
	}
	return l_iProcessResult;
}

bool	cRS232ForFishGame::RequireIOStatusSingnalSending()
{
	DWORD dwStartWritten = 0;//start bit value
	HANDLE	l_Handle = l_Handle = this->m_pComportHandler;
	BOOL l_bWriteOk = WriteFile(l_Handle, m_GameStickerSendingComportData.GetComportBuffer(), m_GameStickerSendingComportData.GetiTotalComportBufferSize(), &dwStartWritten, NULL);
	if (!l_bWriteOk)
	{
		m_strErrorInfo = L"SingnalProcess WriteFile GameStickerSendingComportData failed!";
		cGameApp::OutputDebugInfoString(m_strErrorInfo);
		return false;
	}
	return true;
}


bool	cRS232ForFishGame::ComportSingnalSending(eComportDataIndex e_eComportDataIndex)
{
	if (e_eComportDataIndex == eCDI_MAX)
		return false;
	auto l_pComportData = m_SendingComportDataVector[e_eComportDataIndex];
	DWORD dwStartWritten = 0;//start bit value
	DWORD dwBytesReceived = 0;//start bit value
	HANDLE	l_Handle = l_Handle = this->m_pComportHandler;
	l_pComportData->DoAssignCheckSum();
	BOOL l_bWriteOk = WriteFile(l_Handle, l_pComportData->GetComportBuffer(), l_pComportData->GetiTotalComportBufferSize(), &dwStartWritten, NULL);
	if(l_bWriteOk)
	{
		if(l_pComportData->GetiTotalComportBufferSize() == dwStartWritten )
		{
			m_bSendingRefundAndRefundingReset[e_eComportDataIndex] = false;
			return true;
		}
		else
		{
			m_strErrorInfo = L"SingnalProcess ComportSingnalSending written size not same!";
			cGameApp::OutputDebugInfoString(m_strErrorInfo);
			return false;
		}
	}
	else
	{
		m_strErrorInfo = L"SingnalProcess ComportSingnalSending WriteFile failed!";
		cGameApp::OutputDebugInfoString(m_strErrorInfo);
		return false;
	}
	return false;
}


void	cRS232ForFishGame::Update(float e_fElpaseTime)
{
	//avoid game lag cause frame skip
	if( e_fElpaseTime >= 0.016f )
		e_fElpaseTime = 0.016f;
	//avoid frame skip
	m_fRestTimeToUpdate += e_fElpaseTime;
	float	l_fTargetTime = REFRESH_8051_IO_TIME_OFFSET;
	if(m_fRestTimeToUpdate >= l_fTargetTime )
	{
		if( m_bDoUpdate )
		{
			m_fRestTimeToUpdate = UT::GetFloatModulus(m_fRestTimeToUpdate,l_fTargetTime);
			if(m_pComportHandler == nullptr )
			{
				this->m_bIOError = true;
				this->Init(false);
				return;
			}
			if(SingnalProcess(e_fElpaseTime) == 2)
				m_bDoUpdate = false;
		}
	}
}
//std::wstring	g_str = L"";

bool cRS232ForFishGame::ReadIOBit(int e_iBitAddress,bool*e_pbData, const WCHAR*e_strComportDataName)
{
	//this game only require 1 receving comport data
	int	l_iByte = e_iBitAddress / BIT_COUNT;
	int	l_iBit = e_iBitAddress % BIT_COUNT;
	*e_pbData = (this->m_DefaultRecevedComportData.GetComportBuffer()[l_iByte] & g_iBitAddressValue[l_iBit]) ? true : false;
	return true;
}

bool	cRS232ForFishGame::ReadIOShort(int e_iAddress, short*e_pshValue, const WCHAR*e_strComportDataName)
{
	//this game only require 1 receving comport data
	int	l_iByte = e_iAddress;
	memcpy(e_pshValue, &m_DefaultRecevedComportData.GetComportBuffer()[l_iByte], sizeof(short));
	return true;
}

bool	cRS232ForFishGame::ReadIOInt(int e_iAddress, int*e_piValue, const WCHAR*e_strComportDataName)
{
	//this game only require 1 receving comport data
	int	l_iByte = e_iAddress;
	memcpy(e_piValue, &m_DefaultRecevedComportData.GetComportBuffer()[l_iByte], sizeof(int));
	return true;
}

bool cRS232ForFishGame::ReadIOChar(int e_iAddress,char*e_pcValue, const WCHAR*e_strComportDataName)
{
	//this game only require 1 receving comport data
	int	l_iByte = e_iAddress;
	*e_pcValue = m_DefaultRecevedComportData.GetComportBuffer()[l_iByte];
	return true;
}


template<class Type>bool	cRS232ForFishGame::WriteIOWithComportDataIndex(int e_iAddress, Type e_Value, int e_iComportDataIndex)
{
	int	l_iByte = e_iAddress;
	cComportData*l_pComportData = m_SendingComportDataVector.GetObject(e_iComportDataIndex);
	if (!l_pComportData)
		return false;
	m_bSendingRefundAndRefundingReset[e_iComportDataIndex] = true;
	if (e_iComportDataIndex == eComportDataIndex::eCDI_CountRegisterComportData ||
		e_iComportDataIndex == eComportDataIndex::eCDI_PrintRefundSendingComportData)
	{
		char l_ToBigEndian[2]{ ((char*)&e_Value)[1],((char*)&e_Value)[0] };
		memcpy(&l_pComportData->GetComportBuffer()[l_iByte], &l_ToBigEndian, sizeof(Type));//F12 use
	}
	else 
	{
		//only allow one player to refund.
		memcpy(&l_pComportData->GetComportBuffer()[l_iByte], &e_Value, sizeof(Type));		
	}
	l_pComportData->DoAssignCheckSum();
	return true;
}

bool	cRS232ForFishGame::WriteIOChar(int e_iAddress,char e_cValue, const WCHAR*e_strComportDataName)
{
	int l_iIndex = m_SendingComportDataVector.GetObjectIndexByName(e_strComportDataName);
	assert(l_iIndex != -1 && "WriteIOChar wrong name!");
	return WriteIOWithComportDataIndex<char>(e_iAddress, e_cValue, l_iIndex);
}

bool	cRS232ForFishGame::WriteIOShort(int e_iAddress, short e_shValue, const WCHAR*e_strComportDataName)
{
	int l_iIndex = m_SendingComportDataVector.GetObjectIndexByName(e_strComportDataName);
	assert(l_iIndex != -1 && "WriteIOChar wrong name!");
	return WriteIOWithComportDataIndex<short>(e_iAddress, e_shValue, l_iIndex);
	return true;
}

bool	cRS232ForFishGame::WriteIOInt(int e_iAddress, int e_iValue, const WCHAR*e_strComportDataName)
{
	int l_iIndex = m_SendingComportDataVector.GetObjectIndexByName(e_strComportDataName);
	assert(l_iIndex != -1 && "WriteIOChar wrong name!");
	return WriteIOWithComportDataIndex<int>(e_iAddress, e_iValue, l_iIndex);
}

bool	cRS232ForFishGame::WriteIOBit(int e_iAddress, char e_cValue, const WCHAR*e_strComportDataName)
{
	int	l_iByte = e_iAddress / BIT_COUNT;
	int	l_iBit = e_iAddress % BIT_COUNT;
	int	l_iValue = e_iAddress;
	//only allow one player to refund.
	int l_iIndex = m_SendingComportDataVector.GetObjectIndexByName(e_strComportDataName);
	auto l_pData = m_SendingComportDataVector.GetObject(l_iIndex);
	l_pData->GetComportBuffer()[l_iByte] |= g_iBitAddressValue[l_iBit];
	assert(l_iIndex != -1 && "WriteIOBit wrong name");
	m_bSendingRefundAndRefundingReset[l_iIndex] = true;
	return true;
}

void	cRS232ForFishGame::DebugRender(cComportData&e_cComportData, int e_iStartPosX, int e_iStartPosY)
{
	WCHAR	l_str[MAX_PATH];
	int l_iPosX = e_iStartPosX;
	for (int j = 0; j<e_cComportData.GetiTotalComportBufferSize(); j++)
	{
		int l_iValue = (unsigned char)e_cComportData.GetComportBuffer()[j];
		swprintf(l_str, MAX_PATH, L"%x", l_iValue);
		cGameApp::m_spGlyphFontRender->RenderFont(l_iPosX, e_iStartPosY, l_str);
		l_iPosX += 60;
		if ((j+1) % 10 == 0)
		{
			l_iPosX = e_iStartPosX;
			e_iStartPosY += 40;
		}
	}
}

void	cRS232ForFishGame::Render()
{
	//cGameApp::m_spGlyphFontRender->SetScale(0.5);
	//cGameApp::RenderFont(10, 20, L"Poll Data");
	//DebugRender(m_GameStickerSendingComportData, 100);
	//cGameApp::RenderFont(10, 220, L"refund reset");
	//DebugRender(m_CoinRefundResetSendingComportData, 300);
	for (int i = 0; i < m_iNumNewSendingComportDataBuffer; ++i)
	{
		int l_iPosY = i * 300 + 200;
		int l_iPosX = 800;
		cGameApp::RenderFont(800, l_iPosY, m_NewSendingComportDataBuffer[i].GetName());
		DebugRender(m_NewSendingComportDataBuffer[i], l_iPosX, l_iPosY + 20);

	}

	cGameApp::RenderFont(100, 200, L"receving");
	DebugRender(m_DefaultRecevedComportData, 100, 220);


	cGameApp::RenderFont(150, 200, L"receving");
	DebugRender(m_DefaultRecevedComportData, 150, 220);
	std::wstring l_strInfo = L"None";
	switch(m_iDebugFlag)
	{
		case 0:l_strInfo = L"not init";	break;
		case 1:l_strInfo = L"open comport";	break;
		case 2:l_strInfo = L"coin refund";	break;
		case 3:l_strInfo = L"coin refund reset";	break;
		case 4:l_strInfo = L"common poll";	break;
		case 5:l_strInfo = L"receving data";	break;
		default:break;
	}
	cGameApp::RenderFont(0, 950, l_strInfo);
	cGameApp::RenderFont(600, 950, m_strErrorInfo);
	if (m_bIOError)
	{
		//cGameApp:m_spGlyphFontRender->SetFontColor(Vector4::Red);
		cGameApp::RenderFont(650, 30, L"comport error!");
		//cGameApp:m_spGlyphFontRender->SetFontColor(Vector4::One);

	}
	m_strError[0] = 0;
	
	//cGameApp::m_spGlyphFontRender->SetScale(1.f);
}

//======================================================
//
//======================================================


cIOControlInterface::cIOControlInterface(int e_iCurrentIOMode)
{
	assert(g_pIOSMDriverInterface == nullptr&&"not allow have multi instance!");
	g_pIOSMDriverInterface = this;
	m_bIOError = false;
	if( !g_pRS232ForFishGame)
		g_pRS232ForFishGame = new cRS232ForFishGame(e_iCurrentIOMode);
}

cIOControlInterface::~cIOControlInterface()
{
	SAFE_DELETE(g_pRS232ForFishGame);
	g_pIOSMDriverInterface = nullptr;
}
void	cIOControlInterface::Destroy(){}
void	cIOControlInterface::Init(const char*e_strComportDataSetupFileName, const char*e_strIODeviceFileName)
{
	if (this->ParseWithMyParse(e_strIODeviceFileName))
	{
		int l_iCount = this->Count();
		for (int i = 0; i < l_iCount; ++i)
		{
			auto*l_pData = (*this)[i];
			std::wstring l_strName = l_pData->GetName();
			int l_iAddress = l_pData->GetBitAddress();
			m_OriginalIONameAndAddressMap.insert(std::make_pair(l_strName, l_iAddress));
		}
		if (g_pRS232ForFishGame)
		{
			g_pRS232ForFishGame->ParseWithMyParse(e_strComportDataSetupFileName);
			g_pRS232ForFishGame->Init(true);
#ifndef RETAILER
			DumpOccupiedKeyValue();
#endif
		}
	}
}

void	cIOControlInterface::ClearIOData(float e_fElpaseTime)
{
	if (!cGameApp::m_sbDeviceExist)
		return;
	//force IO update one time
	if (g_pRS232ForFishGame->m_bDoUpdate)
		Sleep(200);
	assert(!g_pRS232ForFishGame->m_bDoUpdate && "what's wrong with IO!?");
	if (g_pRS232ForFishGame && !g_pRS232ForFishGame->m_bDoUpdate)
	{
		g_pRS232ForFishGame->m_bDoUpdate = true;
	}
	Sleep(200);
	this->m_bIOError = g_pRS232ForFishGame->IsIOError();
}

void	cIOControlInterface::Update(float e_fElpaseTime)
{
	if(!cGameApp::m_sbDeviceExist)
		return;
	if(g_pRS232ForFishGame && !g_pRS232ForFishGame->m_bDoUpdate)
	{
		if (!g_pRS232ForFishGame->m_bIOError)
		{
			int	l_iCount = this->Count();
			for (int i = 0; i < l_iCount; ++i)
			{
				this->m_ObjectList[i]->Update(e_fElpaseTime);
			}
		}
		g_pRS232ForFishGame->m_bDoUpdate = true;
	}
	this->m_bIOError = g_pRS232ForFishGame->IsIOError();
}
void	cIOControlInterface::Render()
{
	if(g_pRS232ForFishGame)
		g_pRS232ForFishGame->Render();
}

bool	cIOControlInterface::ReadIOBit(int e_iAddress, bool*e_pbData, const WCHAR*e_strComportDataName)
{
	if (g_pRS232ForFishGame)
		return g_pRS232ForFishGame->ReadIOBit(e_iAddress,e_pbData, e_strComportDataName);
	return false;
}

bool	cIOControlInterface::ReadIOChar(int e_iAddress, char*e_pcData, const WCHAR*e_strComportDataName)
{
	if (g_pRS232ForFishGame)
		return g_pRS232ForFishGame->ReadIOChar(e_iAddress, e_pcData, e_strComportDataName);
	return false;
}

bool	cIOControlInterface::ReadIOShort(int e_iAddress, short*e_pshData, const WCHAR*e_strComportDataName)
{
	if (g_pRS232ForFishGame)
		return g_pRS232ForFishGame->ReadIOShort(e_iAddress, e_pshData, e_strComportDataName);
	return false;
}

bool	cIOControlInterface::ReadIOInt(int e_iAddress, int*e_piData, const WCHAR*e_strComportDataName)
{
	if (g_pRS232ForFishGame)
		return g_pRS232ForFishGame->ReadIOInt(e_iAddress, e_piData, e_strComportDataName);
	return false;
}


bool	cIOControlInterface::WriteIOShort(int e_iAddress, short e_cData, const WCHAR*e_strComportDataName)
{
	if (g_pRS232ForFishGame)
		return g_pRS232ForFishGame->WriteIOShort(e_iAddress, e_cData, e_strComportDataName);
	return false;
}
bool	cIOControlInterface::WriteIOInt(int e_iAddress, int e_cData, const WCHAR*e_strComportDataName)
{
	if (g_pRS232ForFishGame)
		return g_pRS232ForFishGame->WriteIOInt(e_iAddress, e_cData, e_strComportDataName);
	return false;
}

bool	cIOControlInterface::WriteIOChar(int e_iAddress, char e_cData, const WCHAR*e_strComportDataName)
{
	if (g_pRS232ForFishGame)
		return g_pRS232ForFishGame->WriteIOChar(e_iAddress, e_cData, e_strComportDataName);
	return false;
}

bool	cIOControlInterface::WriteIOBit(int e_iAddress, bool e_bData, const WCHAR*e_strComportDataName)
{
	if (g_pRS232ForFishGame)
		return g_pRS232ForFishGame->WriteIOBit(e_iAddress, e_bData, e_strComportDataName);
	return false;
}

void*	cIOControlInterface::GetIOInstance()
{
	return (void*)g_pRS232ForFishGame;
}

void	cIOControlInterface::SwapByName(bool e_bSwap, std::wstring e_strIO1Name, std::wstring e_strIO2Name, int e_iPlayerCount)
{
	for (int i = 0; i < e_iPlayerCount; ++i)
	{
		std::wstring l_strIO1 = e_strIO1Name;
		std::wstring l_strIO2 = e_strIO2Name;
		l_strIO1 += ValueToStringW(i);
		l_strIO2 += ValueToStringW(i);
		auto l_pIO1Iterator = m_OriginalIONameAndAddressMap.find(l_strIO1);
		auto l_pIO2Iterator = m_OriginalIONameAndAddressMap.find(l_strIO2);
		auto l_pIO1 = this->GetObject(l_strIO1);
		auto l_pIO2 = this->GetObject(l_strIO2);
		if (l_pIO1Iterator != m_OriginalIONameAndAddressMap.end() &&
			l_pIO2Iterator != m_OriginalIONameAndAddressMap.end() &&
			l_pIO1 && l_pIO2)
		{
			if (e_bSwap)
			{
				l_pIO1->SetBitAddress(l_pIO2Iterator->second);
				l_pIO2->SetBitAddress(l_pIO1Iterator->second);
			}
			else
			{
				l_pIO2->SetBitAddress(l_pIO2Iterator->second);
				l_pIO1->SetBitAddress(l_pIO1Iterator->second);
			}
		}
	}
}

////IGS
//Right_0;
//Left_0;
////Ar Dong
//Up_0;
//Down_0;
//SendingCoinRefund_0;
//SendingTicketRefund_0;
void	cIOControlInterface::IOMode0(bool e_bSwap, int e_iPlayerCount)
{
	std::wstring l_strLeft	= L"Left_";
	std::wstring l_strRight = L"Right_";
	SwapByName(e_bSwap, l_strLeft, l_strRight,e_iPlayerCount);
}

void	cIOControlInterface::IOMode1(bool e_bSwap, int e_iPlayerCount)
{
	std::wstring l_strUp = L"Up_";
	std::wstring l_strDown = L"Down_";
	SwapByName(e_bSwap, l_strUp, l_strDown, e_iPlayerCount);
	std::wstring l_strSendingCoinRefund = L"SendingCoinRefund_";
	std::wstring l_strSendingTicketRefund = L"SendingTicketRefund_";
	SwapByName(e_bSwap, l_strSendingCoinRefund, l_strSendingTicketRefund, e_iPlayerCount);

}
void	cIOControlInterface::IOMode2(bool e_bSwap,int e_iPlayerCount)
{

}

void	cIOControlInterface::SwitchIOMode(int e_iType, int e_iPlayerCount)
{
	assert(m_OriginalIONameAndAddressMap.size()&&"how come!");
	bool l_bSwapMode0 = e_iType == 1 ? true : false;
	bool l_bSwapMode1 = e_iType == 3 ? true : false;
	//bool l_bSwapMode2 = e_iType == 4 ? true : false;
	IOMode0(l_bSwapMode0, e_iPlayerCount);
	IOMode1(l_bSwapMode1, e_iPlayerCount);
	//IOMode2(l_bSwapMode2);
}

void	cIOControlInterface::Reset()
{
	cGameApp::OutputDebugInfoString(L"Reset()");
	if (g_pRS232ForFishGame)
	{
		g_pRS232ForFishGame->m_bDoUpdate = true;
		Sleep(200);
	}
	PurgeComm(g_pRS232ForFishGame->m_pComportHandler, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	Sleep(200);
	g_pRS232ForFishGame->Init(true);
	cGameApp::OutputDebugInfoString(L"Reset() Finish");
}