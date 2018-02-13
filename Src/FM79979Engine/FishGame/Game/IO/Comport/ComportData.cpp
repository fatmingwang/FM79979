#include "stdafx.h"
#include "ComportData.h"
#include "IOControlInterface.h"
cComportData::cComportData()
{
	m_iTotalComportBufferSize = 0;
	m_iDataStartIndex = -1;
	m_iEachDataSize = -1;
	m_iCheckSumIndexOfDataArray = -1;
	m_iEndCommandSize = -1;
	m_iStartCommandSize = -1;
	m_iEndCommandBufferIndex = -1;
	m_pSpecificAddressToSetZero = nullptr;
	m_pSpecificBitAddressToSetOne = nullptr;
}
//first 2 is command,10 players game sticker status(10bytes),check sum 1 byte
//0xAA,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0x68,0x16
//<ComportData TotalDataSize="25" DefaultComportBuffer="0xAA,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0x68,0x16" m_iDataStartIndex="2" m_iEachDataSize="1" m_iCheckSumIndexOfDataArray="12" StartCommandBuffer="" EndCommandBuffer="" EndCommandBufferIndex="13"/>
bool	cComportData::ParseData(TiXmlElement*e_pTiXmlElement)
{
	this->SetName(e_pTiXmlElement->Value());
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("TotalDataSize")
		{
			m_iTotalComportBufferSize = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("DefaultComportBuffer")
		{
			if (m_iTotalComportBufferSize < 1)
			{
				UT::ErrorMsg("Error", "DefaultComportBuffer data not correct!");
				return false;
			}
			char*l_pComportBuffer = ParseDataToGenerateIntPointer<char>(l_strValue, m_iTotalComportBufferSize);
			memcpy(m_ComportBuffer, l_pComportBuffer, m_iTotalComportBufferSize);
			memcpy(m_DefaultComportData, l_pComportBuffer, m_iTotalComportBufferSize);
			SAFE_DELETE(l_pComportBuffer);
		}
		else
		COMPARE_NAME("DataStartIndex")
		{
			m_iDataStartIndex = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("EachDataSize")
		{
			m_iEachDataSize = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("CheckSumIndexOfDataArray")
		{
			m_iCheckSumIndexOfDataArray = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("StartCommandBuffer")
		{
			std::vector<unsigned char> l_ResultVector = GetValueListByCommaDivide<unsigned char>(l_strValue);
			CommandAssign(m_iStartCommandSize, m_StartCommandBuffer, l_ResultVector);
			assert(m_iStartCommandSize < START_COMMAND_SIZE);
		}
		else
		COMPARE_NAME("EndCommandBuffer")
		{
			std::vector<unsigned char> l_ResultVector = GetValueListByCommaDivide<unsigned char>(l_strValue);
			CommandAssign(m_iEndCommandSize, m_EndCommandBuffer, l_ResultVector);
			assert(m_iEndCommandSize < START_COMMAND_SIZE);
		}
		else
		COMPARE_NAME("EndCommandBufferIndex")
		{
			m_iEndCommandBufferIndex = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("SpecificAddressToSetZero")
		{
			m_pSpecificAddressToSetZero = new std::vector<int>;
			*m_pSpecificAddressToSetZero = GetIntegerListByCommaDivide(l_strValue,10);
		}
		else
		COMPARE_NAME("SpecificBitAddressToSetOne")
		{
			m_pSpecificBitAddressToSetOne = new std::vector<int>;
			*m_pSpecificBitAddressToSetOne = GetIntegerListByCommaDivide(l_strValue, 20);
		}
		PARSE_NAME_VALUE_END
		for (int i = 0; i < m_iStartCommandSize; ++i)
		{
			this->m_ComportBuffer[i] = m_StartCommandBuffer[i];
			this->m_DefaultComportData[i] = m_StartCommandBuffer[i];
		}
		int l_iIndex = 0;
		assert((m_iTotalComportBufferSize- m_iEndCommandBufferIndex) == m_iEndCommandSize &&"end buffer size wrong!");
		for (int i = m_iEndCommandBufferIndex; i < this->m_iTotalComportBufferSize; ++i)
		{
			this->m_ComportBuffer[i] = m_EndCommandBuffer[l_iIndex];
			if (m_DefaultComportData)
				this->m_DefaultComportData[i] = m_EndCommandBuffer[l_iIndex];
			++l_iIndex;
		}
		this->DoAssignCheckSum();
		return true;
}
cComportData::~cComportData()
{
	SAFE_DELETE(m_pSpecificAddressToSetZero);
	SAFE_DELETE(m_pSpecificBitAddressToSetOne);
}

void	cComportData::CommandAssign(int&e_iSize,unsigned char*e_pcCommandBuffer, std::vector<unsigned char>&e_Vector)
{
	e_iSize = (int)e_Vector.size();
	for (int i = 0; i < e_iSize; ++i)
	{
		e_pcCommandBuffer[i] = e_Vector[i];
	}
}

void	cComportData::SetupData(int e_m_iTotalComportBufferSize, char*e_pDefaultData, int e_m_iDataStartIndex, int e_m_iEachDataSize, int e_iCheckSumIndexOfComportBuffer)
{
	SetDefaultBufferData(e_m_iTotalComportBufferSize, e_pDefaultData);
	m_iDataStartIndex = e_m_iDataStartIndex;
	m_iEachDataSize = e_m_iEachDataSize;
	m_iCheckSumIndexOfDataArray = e_iCheckSumIndexOfComportBuffer;
}
void	cComportData::SetDefaultBufferData(int e_m_iTotalComportBufferSize, char*	e_pDefaultDataArray)
{
	m_iTotalComportBufferSize = e_m_iTotalComportBufferSize;
	memcpy(m_ComportBuffer, e_pDefaultDataArray, e_m_iTotalComportBufferSize);
}

bool    cComportData::DoAssignCheckSum()
{
	unsigned char l_cResult = 0;
	if (CalculateCheckSum(m_ComportBuffer, m_iCheckSumIndexOfDataArray, l_cResult))
	{
		m_ComportBuffer[m_iCheckSumIndexOfDataArray] = l_cResult;
		return true;
	}
	return false;
}

bool	cComportData::IsCheckSumIsLegal()
{
	if (!IsStartCommandCorrect() || !IsCloseCommandCorrect() )
		return false;
	unsigned char l_cResult = 0;
	if (CalculateCheckSum(m_ComportBuffer, m_iCheckSumIndexOfDataArray, l_cResult))
	{
		if(m_ComportBuffer[m_iCheckSumIndexOfDataArray] == l_cResult)
			return true;
	}
	return false;
}


bool	cComportData::IsStartCommandCorrect()
{
	for (int i = 0; i < this->m_iStartCommandSize; ++i)
	{
		if (this->m_ComportBuffer[i] != this->m_StartCommandBuffer[i])
			return false;
	}
	return true;
}

bool	cComportData::IsCloseCommandCorrect()
{
	int l_iEndIndex = m_iEndCommandBufferIndex+this->m_iEndCommandSize;
	int l_iIndex = 0;
	for (int i = this->m_iEndCommandBufferIndex; i < l_iEndIndex; ++i)
	{
		if (this->m_ComportBuffer[i] != this->m_EndCommandBuffer[l_iIndex])
			return false;
		++l_iIndex;
	}
	return true;
}

void	cComportData::CopyDefaultBufferToComportBuffer()
{
	memcpy(this->m_ComportBuffer, this->m_DefaultComportData, this->m_iTotalComportBufferSize);
}

void	cComportData::SetSpecificBufferDataToZeroAvoidCoinOrTickerNotCorrect()
{
	if (m_pSpecificAddressToSetZero)
	{
		size_t l_uiSize = m_pSpecificAddressToSetZero->size();
		for (size_t i = 0; i < l_uiSize; ++i)
			this->m_ComportBuffer[(*m_pSpecificAddressToSetZero)[i]] = 0;
	}
}

void	cComportData::SetSpecificBufferBitDataToForResetAllPlayerRefundData()
{
	if (m_pSpecificBitAddressToSetOne)
	{
		size_t l_uiSize = m_pSpecificBitAddressToSetOne->size();
		for (size_t i = 0; i < l_uiSize; ++i)
		{
			int l_iBitAddress = (*m_pSpecificBitAddressToSetOne)[i];
			int	l_iByte = l_iBitAddress / BIT_COUNT;
			int	l_iBit = l_iBitAddress % BIT_COUNT;
			this->m_ComportBuffer[l_iByte] |= g_iBitAddressValue[l_iBit];
		}
	}
	
}