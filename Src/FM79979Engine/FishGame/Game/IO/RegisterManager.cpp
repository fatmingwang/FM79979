#include "stdafx.h"
#include "RegisterManager.h"
#include "Comport/IOControlInterface.h"
#include "DeviceRefundCoinBase.h"
#include "../GameDefine/FishGameEventMessageID.h"
#include "../ControlPanel/ControlSettingParameter.h"
#include "../GameDefine/GameParameterDefine.h"

#define	REGISTER_SAFE_COUNT_VALUE			60000
#define	IO_REGISTER_FILE_NAME				"Register.bi"
cRegisterManager*	cRegisterManager::m_pRegisterManager = nullptr;


cRegisterValueChecker::cRegisterValueChecker(cIOPackData*e_pSendToRegister, cIOPackData*e_pRecevedFromeRegister)
{
	assert(e_pSendToRegister && "e_pSendToRegister no exists");
	assert(e_pRecevedFromeRegister && "e_pRecevedFromeRegister no exists");
	m_iWaitForAddToRegisterValue = 0;
	m_iCurrentToRegisterValue = 0;
	m_iWaitIOWriteReadReadyFlag = 0;
	m_pSendToRegister = e_pSendToRegister;
	m_pRecevedFromeRegister = e_pRecevedFromeRegister;
	m_pRecevedFromeRegister->m_IOSignalReadedFunction = std::bind(&cRegisterValueChecker::f_IOSignalReadedFunction, this);
	m_pSendToRegister->m_IOSignalWriteFunction = std::bind(&cRegisterValueChecker::f_IOSignalWriteFunction, this);
}
cRegisterValueChecker::~cRegisterValueChecker()
{
}

bool	cRegisterValueChecker::f_IOSignalReadedFunction()
{
	if (m_iWaitIOWriteReadReadyFlag >= 1)
	{
		//OutputDebugString(L"m_iWaitIOWriteReadReadyFlag:2  ");
		short l_shValue = m_pRecevedFromeRegister->GetDataByShort();
		//std::wstring l_str = ValueToStringW(l_shValue);
		//OutputDebugString(L"target refund value ");
		//OutputDebugString(l_str.c_str());
		//OutputDebugString(L"\n");
		//if(l_shValue != 0)
		//	m_iWaitIOWriteReadReadyFlag = 2;
		m_iWaitIOWriteReadReadyFlag++;
	}
	return true;
}
bool	cRegisterValueChecker::f_IOSignalWriteFunction()
{
	if (m_iWaitIOWriteReadReadyFlag == 0)
	{
		m_iWaitIOWriteReadReadyFlag = 1;
		//OutputDebugString(L"m_iWaitIOWriteReadReadyFlag:1\n");
	}
	return true;
}
void	cRegisterValueChecker::Update(float e_fElpaseTime)
{
	if (m_iCurrentToRegisterValue != 0)
	{
		if (m_iWaitIOWriteReadReadyFlag >= START_REFUND_FLAG_VALUE && m_pRecevedFromeRegister)
		{
			short l_shValue = m_pRecevedFromeRegister->GetDataByShort();
			if (l_shValue != m_iCurrentToRegisterValue)
			{
				m_iCurrentToRegisterValue = l_shValue;
			}
		}
	}
	else
	if (m_iWaitForAddToRegisterValue != 0)
	{
		const int l_ciSafeToRegisterValue = REGISTER_SAFE_COUNT_VALUE;
		if (m_iWaitForAddToRegisterValue >= l_ciSafeToRegisterValue)
		{
			m_iWaitForAddToRegisterValue -= l_ciSafeToRegisterValue;
			m_iCurrentToRegisterValue = l_ciSafeToRegisterValue;
		}
		else
		{
			m_iCurrentToRegisterValue = m_iWaitForAddToRegisterValue;
			m_iWaitForAddToRegisterValue = 0;
		}
		m_iWaitIOWriteReadReadyFlag = 0;
		if (!m_pSendToRegister->SetData((short)m_iCurrentToRegisterValue))
		{
			assert(0 && "m_pSendToRegister->SetData((short)m_iCurrentToRegisterValue");
		}
		//std::wstring l_str = ValueToStringW(m_iCurrentToRegisterValue);
		//OutputDebugString(L"going to refund");
		//OutputDebugString(l_str.c_str());
		//OutputDebugString(L"\n");
	}
}

int		cRegisterValueChecker::AddValueToWaitWaitForAddToRegisterValue(int e_iValue)
{
	m_iWaitForAddToRegisterValue += e_iValue;
	return m_iWaitForAddToRegisterValue;
}

int		cRegisterValueChecker::GetRestValueToRegister()
{
	int l_iResult = m_iCurrentToRegisterValue + m_iWaitForAddToRegisterValue;
	return l_iResult;
}

cRegisterManager::cRegisterManager()
{
	m_fFileVersion = 1.0f;
	m_LastTimeValue.iExchangeInValue = 0;
	m_LastTimeValue.iExchangeOutValue = 0;
	m_fRestTimeToWriteDataIntoFile = m_fTimeForWriteDataIntoFile = 0.05f;
	m_pExchangeInRegisterValueChecker = nullptr;
	m_pExchangeOutRegisterValueChecker = nullptr;
	REG_EVENT(eFGEMI_REGISTER_IN, &cRegisterManager::ExchangeInEventFuntion);
	REG_EVENT(eFGEMI_REGISTER_OUT, &cRegisterManager::ExchangeOutEventFuntion);
//	if (cFishApp::m_spIOControlInterface)
	{
		//cIOPackData*l_pExchangeInCounterRegisterIOPackData = (cIOPackData*)cFishApp::m_spIOControlInterface->GetObject(L"ExchangeInCounterRegister_0");
		//cIOPackData*l_pRecevingInCounterRegisterRefundIOPackData = (cIOPackData*)cFishApp::m_spIOControlInterface->GetObject(L"RecevingInCounterRegisterRefund_0");
		//cIOPackData*l_pExchangeOutCounterRegisterIOPackData = (cIOPackData*)cFishApp::m_spIOControlInterface->GetObject(L"ExchangeOutCounterRegister_0");
		//cIOPackData*l_pRecevingOutCounterRegisterRefundIOPackData = (cIOPackData*)cFishApp::m_spIOControlInterface->GetObject(L"RecevingOutCounterRegisterRefund_0");
		//m_pExchangeInRegisterValueChecker = new cRegisterValueChecker(l_pExchangeInCounterRegisterIOPackData, l_pRecevingInCounterRegisterRefundIOPackData);
		//m_pExchangeOutRegisterValueChecker = new cRegisterValueChecker(l_pExchangeOutCounterRegisterIOPackData, l_pRecevingOutCounterRegisterRefundIOPackData);
		FileOpen();
	}
}

cRegisterManager::~cRegisterManager()
{
	SAFE_DELETE(m_pExchangeInRegisterValueChecker);
	SAFE_DELETE(m_pExchangeOutRegisterValueChecker);
}

bool	cRegisterManager::FileOpen()
{
	if (!Writefile(IO_REGISTER_FILE_NAME, true, true, "rb+"))
	{
		assert(0 && "it's imsspoible...not enough disk spacr?");
		return false;
	}
	long l_lFileSize = UT::GetFileSize(this->m_pFile);
	if (l_lFileSize == 0)
	{//new file write header
		WriteNewDataIntoFile();
	}
	else
	{
		char*l_pData = (char*)GetDataFile(0);
		//first check header
		Register_Header_Struct*l_pRegister_Header_Struct = (Register_Header_Struct*)l_pData;
		if (l_pRegister_Header_Struct->chID[0] == 'R' &&
			l_pRegister_Header_Struct->chID[1] == 'E' &&
			l_pRegister_Header_Struct->chID[2] == 'G')
		{
			if (l_pRegister_Header_Struct->fVersion == m_fFileVersion)
			{
				l_pData += sizeof(Register_Header_Struct);
				sVersion_1_0_Data*l_pVersion_1_0_Data = (sVersion_1_0_Data*)l_pData;
				if (l_pVersion_1_0_Data->iExchangeInValue != 0 && m_pExchangeInRegisterValueChecker)
				{
					//OutputDebugString(L"iExchangeInValue:");
					//OutputDebugString(ValueToStringW(l_pVersion_1_0_Data->iExchangeInValue).c_str());
					//OutputDebugString(L"\n");
					m_pExchangeInRegisterValueChecker->AddValueToWaitWaitForAddToRegisterValue(l_pVersion_1_0_Data->iExchangeInValue);
				}
				if (l_pVersion_1_0_Data->iExchangeOutValue != 0)
				{
					//OutputDebugString(L"iExchangeOutValue:");
					//OutputDebugString(ValueToStringW(l_pVersion_1_0_Data->iExchangeOutValue).c_str());
					//OutputDebugString(L"\n");
					m_pExchangeOutRegisterValueChecker->AddValueToWaitWaitForAddToRegisterValue(l_pVersion_1_0_Data->iExchangeOutValue);
				}
			}
			else
			{//different version,do convert file.
				//now should always same.
				WriteNewDataIntoFile();
			}
		}
		else
		{//incorrect file
			WriteNewDataIntoFile();
		}
	}
	return true;
}

bool	cRegisterManager::WriteNewDataIntoFile()
{
	if (!m_pFile)
		return false;
	Register_Header_Struct l_Register_Header_Struct;
	l_Register_Header_Struct.chID[0] = 'R';
	l_Register_Header_Struct.chID[1] = 'E';
	l_Register_Header_Struct.chID[2] = 'G';
	l_Register_Header_Struct.fVersion = m_fFileVersion;
	NvFWrite(&l_Register_Header_Struct, sizeof(Register_Header_Struct), 1, m_pFile);
	int	l_iExhcangeValue[2] = { 0,0 };
	NvFWrite(&l_iExhcangeValue, sizeof(int), 2, m_pFile);
	this->Flush();
	return true;
}


bool cRegisterManager::ExchangeInEventFuntion(void *e_pData)
{
	if (m_pExchangeInRegisterValueChecker)
	{
		int l_iValue = *(int*)e_pData;
		m_pExchangeInRegisterValueChecker->AddValueToWaitWaitForAddToRegisterValue(l_iValue);
		//WCHAR debug[100];
		//swprintf(debug, L"ExchangeInEventFuntion: %d\n", (int)l_iValue);
		//OutputDebugString(debug);
		return true;
	}
	return false;
}
bool cRegisterManager::ExchangeOutEventFuntion(void *e_pData)
{
	if (m_pExchangeOutRegisterValueChecker)
	{
		int l_iValue = *(int*)e_pData;
		m_pExchangeOutRegisterValueChecker->AddValueToWaitWaitForAddToRegisterValue(l_iValue);
		//WCHAR debug[100];
		//swprintf(debug, L"ExchangeOutEventFuntion: %d\n", (int)l_iValue);
		//OutputDebugString(debug);
		return true;
	}
	return true;
}

cRegisterManager* cRegisterManager::GetInstance()
{
	if (!m_pRegisterManager)
	{
		if (sControlSettingParameter::m_siIOMode == TWLEADSTREAM_IO_MODE && cGameApp::m_sbDeviceExist)
			m_pRegisterManager = new cRegisterManager();
	}
	return m_pRegisterManager;
}

void	cRegisterManager::DeleteInstance()
{
	SAFE_DELETE(m_pRegisterManager);
}

void	cRegisterManager::GetInstanceCallUpdateForWriteFile(float e_fElpaseTime)
{
	if (sControlSettingParameter::m_siIOMode == TWLEADSTREAM_IO_MODE)
	{
		cRegisterManager*l_pRegisterManager = cRegisterManager::GetInstance();
		if (l_pRegisterManager)
			l_pRegisterManager->UpdateForWriteFile(e_fElpaseTime);
	}
}


void cRegisterManager::Update(float e_fElpaseTime)
{
	if (m_pExchangeInRegisterValueChecker)
		m_pExchangeInRegisterValueChecker->Update(e_fElpaseTime);
	if (m_pExchangeOutRegisterValueChecker)
		m_pExchangeOutRegisterValueChecker->Update(e_fElpaseTime);
	
}

bool	cRegisterManager::IsRegisterWorking()
{
	int l_iValue = 0;
	if (m_pExchangeInRegisterValueChecker)
	{
		l_iValue += m_pExchangeInRegisterValueChecker->GetRestValueToRegister();
	}
	if (m_pExchangeOutRegisterValueChecker)
	{
		l_iValue += m_pExchangeOutRegisterValueChecker->GetRestValueToRegister();
	}
	return l_iValue>0 ? true : false;
}

void	cRegisterManager::UpdateForWriteFile(float e_fElpaseTime)
{
	m_fRestTimeToWriteDataIntoFile -= e_fElpaseTime;
	if (m_fRestTimeToWriteDataIntoFile <= 0.f)
	{
		m_fRestTimeToWriteDataIntoFile = m_fTimeForWriteDataIntoFile;
		if (m_pExchangeOutRegisterValueChecker && m_pExchangeInRegisterValueChecker)
		{
			int l_iExchangeOutValue = m_pExchangeOutRegisterValueChecker->GetRestValueToRegister();
			int l_iExchangeInValue = m_pExchangeInRegisterValueChecker->GetRestValueToRegister();
			if(m_LastTimeValue.iExchangeOutValue != l_iExchangeOutValue ||
				m_LastTimeValue.iExchangeInValue != l_iExchangeInValue)
			{
				m_LastTimeValue.iExchangeOutValue = l_iExchangeOutValue;
				m_LastTimeValue.iExchangeInValue = l_iExchangeInValue;

				int l_iHeaderSize = sizeof(Register_Header_Struct);
				this->WriteToFile((const char*)&m_LastTimeValue,sizeof(sVersion_1_0_Data), l_iHeaderSize);
				this->Flush();
			}
		}
	}
}