#pragma once
#include "Comport/IOData.h"

class cRegisterValueChecker:public NamedTypedObject
{
	//
	cIOPackData*	m_pSendToRegister;
	cIOPackData*	m_pRecevedFromeRegister;
	//
	int				m_iWaitForAddToRegisterValue;
	int				m_iCurrentToRegisterValue;
	//write IO and read then start to check Register value
	int				m_iWaitIOWriteReadReadyFlag;
	//
	bool			f_IOSignalReadedFunction();
	bool			f_IOSignalWriteFunction();
public:
	cRegisterValueChecker(cIOPackData*e_pSendToRegister, cIOPackData*e_pRecevedFromeRegister);
	virtual ~cRegisterValueChecker();
	void	Update(float e_fElpaseTime);
	//m_iWaitForAdd+m_iWaitForAdd,get this file and writoe into file.
	int		GetRestValueToRegister();
	//e_iValue from file or event
	int		AddValueToWaitWaitForAddToRegisterValue(int e_iValue);

};

class cRegisterManager : public cMessageSender, public cBinaryFile
{
	struct Register_Header_Struct
	{
		char				chID[4];//REG,because size of Register_Header_Struct is 8 not 7 so I add the fourth and ignore its data.
		float				fVersion;//1.0
	};
	struct sVersion_1_0_Data
	{
		int	iExchangeInValue;
		int	iExchangeOutValue;
	};
	//if no file exists create a new file
	float					m_fFileVersion;
	bool					FileOpen();
	sVersion_1_0_Data		m_LastTimeValue;
	bool					WriteNewDataIntoFile();
	cRegisterValueChecker*	m_pExchangeInRegisterValueChecker;
	cRegisterValueChecker*	m_pExchangeOutRegisterValueChecker;
	//
	static cRegisterManager* m_pRegisterManager;
	cRegisterManager();
	virtual ~cRegisterManager();
	bool					ExchangeInEventFuntion(void *e_pData);
	bool					ExchangeOutEventFuntion(void *e_pData);
	float					m_fTimeForWriteDataIntoFile;
	float					m_fRestTimeToWriteDataIntoFile;
public:
	static cRegisterManager*	GetInstance();
	static void					DeleteInstance();
	static void					GetInstanceCallUpdateForWriteFile(float e_fElpaseTime);
	//do write file here
	void					Update(float e_fElpaseTime);
	void					UpdateForWriteFile(float e_fElpaseTime);
	bool					IsRegisterWorking();
	//
};