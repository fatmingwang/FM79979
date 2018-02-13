#pragma once


inline bool    CalculateCheckSum(unsigned char*e_pData, int e_iCheckSumdIndex,unsigned char&e_cResult)
{
	if (e_iCheckSumdIndex < 1)
		return false;
	unsigned char l_CheckSum = 0;
	for (int i = 0; i < e_iCheckSumdIndex; ++i)
	{
		l_CheckSum += e_pData[i];
	}
	e_cResult = l_CheckSum;
	return  true;
}

#define	MAX_COMPORT_BUFFER	1024
#define	START_COMMAND_SIZE	10
#define	END_COMMAND_SIZE	10

class cComportData:public NamedTypedObject
{
	void	SetDefaultBufferData(int e_iPollDataSize, char*	e_pPollDataBuffer);
	bool	IsStartCommandCorrect();
	bool	IsCloseCommandCorrect();
	void	CommandAssign(int&e_iSize, unsigned char*e_pcCommandBuffer, std::vector<unsigned char>&e_Vector);
	//
	int				m_iStartCommandSize;
	unsigned char	m_StartCommandBuffer[START_COMMAND_SIZE];
	int				m_iEndCommandBufferIndex;
	int				m_iEndCommandSize;
	unsigned char	m_EndCommandBuffer[START_COMMAND_SIZE];
	//
	int				m_iTotalComportBufferSize;
	unsigned char	m_ComportBuffer[MAX_COMPORT_BUFFER];
	unsigned char	m_DefaultComportData[MAX_COMPORT_BUFFER];
	//
	int				m_iDataStartIndex;
	int				m_iEachDataSize;
	//
	std::vector<int>* m_pSpecificAddressToSetZero;
	std::vector<int>* m_pSpecificBitAddressToSetOne;
	//
	int		m_iCheckSumIndexOfDataArray;
public:
	cComportData();
	~cComportData();
	unsigned char*GetComportBuffer() { return m_ComportBuffer; }
	bool	ParseData(TiXmlElement*e_pTiXmlElement);
	void	SetupData(int e_iSize,char*e_pDefaultData,int e_m_iDataStartIndex,int e_iDataSize,int e_iCheckSumIndexOfPollDataBuffer);
	bool    DoAssignCheckSum();
	bool	IsCheckSumIsLegal();
	void	CopyDefaultBufferToComportBuffer();
	void	SetSpecificBufferDataToZeroAvoidCoinOrTickerNotCorrect();
	void	SetSpecificBufferBitDataToForResetAllPlayerRefundData();
	int		GetiTotalComportBufferSize() { return m_iTotalComportBufferSize; }
	//bool	Parse(const char*e_strFileName);
};