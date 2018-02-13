#include "stdafx.h"
#include "IOData.h"
#include "IOControlInterface.h"


const wchar_t*         cIOBitData::TypeID( L"cIOBitData" );
const wchar_t*         cIOPackData::TypeID( L"cIOPackData" );
//from the cpp you implement,ex:RS232ForFishGame.cpp
extern cIOControlInterface*g_pIOSMDriverInterface;
cIOBitData::cIOBitData()
{
	m_pucKeyData = nullptr;
	m_iBitAddress = -1;
	m_pSingnalData = nullptr;
	m_eIODataType = eIODT_READ;
	m_bWaitForWrite = false;
	m_strTargetComportName = L"DefaultRecevedComportData";
	m_bDataReaded = false;
	m_IOSignalReadedFunction = nullptr;
	m_IOSignalWriteFunction = nullptr;
}

cIOBitData::~cIOBitData()
{
	SAFE_DELETE(m_pucKeyData);
	SAFE_DELETE(m_pSingnalData);
	m_IOSignalReadedFunction = nullptr;
	m_IOSignalWriteFunction = nullptr;
}


bool	cIOBitData::SetDataByBoolean(bool e_b)
{
	*(bool*)(this->m_pSingnalData) = e_b;
	m_bWaitForWrite = true;
	if (g_pIOSMDriverInterface)
	{
		return true;
	}
	return false;
}

bool	cIOBitData::IsSingnalActive()
{
	return *(bool*)(this->m_pSingnalData);
}

void	cIOBitData::Init()
{
	if (m_pSingnalData == nullptr)
		m_pSingnalData = new bool;
	m_bWaitForWrite = false;
}

void	cIOBitData::Update(float e_fElpaseTime)
{
	if (g_pIOSMDriverInterface)
	{
		if (this->m_eIODataType == eIODT_READ)
		{
			if (g_pIOSMDriverInterface->ReadIOBit(m_iBitAddress, (bool*)(this->m_pSingnalData), this->m_strTargetComportName.c_str()))
			{
				m_bDataReaded = true;
				if (m_IOSignalReadedFunction)
				{
					m_IOSignalReadedFunction();
				}
				if (m_pucKeyData)
				{
					cGameApp::m_sucKeyData[*m_pucKeyData] = *(bool*)(this->m_pSingnalData);
				}
			}
		}
		else
		if(m_bWaitForWrite && this->m_eIODataType == eIODT_WRITE)
		{
			g_pIOSMDriverInterface->WriteIOBit(m_iBitAddress, *(bool*)(this->m_pSingnalData),this->m_strTargetComportName.c_str());
			m_bWaitForWrite = false;
			if (m_IOSignalWriteFunction)
			{
				m_IOSignalWriteFunction();
			}
		}
	}
}


cIOPackData::cIOPackData()
{
	m_iDataSize = 0;
}

cIOPackData::~cIOPackData()
{
}


void	cIOPackData::SetDataSize(int	e_iSize)
{
	if(m_pSingnalData)
		delete m_pSingnalData;
	m_pSingnalData = malloc(e_iSize);
	memset(m_pSingnalData,0,e_iSize);
	this->m_iDataSize = e_iSize;
}

void	cIOPackData::Init()
{
	if (m_pSingnalData == nullptr && m_iDataSize > 0)
	{
		SetDataSize(m_iDataSize);
	}
}

void	cIOPackData::Update(float e_fElpaseTime)
{
	assert((this->m_iDataSize == 1|| this->m_iDataSize == 2||this->m_iDataSize == 4)&&"ask fatming to do more!cIOPackData::Update");
	if (g_pIOSMDriverInterface)
	{
		if ( this->m_eIODataType == eIODT_READ)
		{
			bool l_bResult = false;
			if (this->m_iDataSize == 1)
			{
				l_bResult = g_pIOSMDriverInterface->ReadIOChar(this->m_iBitAddress, (char*)m_pSingnalData, this->m_strTargetComportName.c_str());
			}
			else
			if (this->m_iDataSize == 2)
			{
				l_bResult = g_pIOSMDriverInterface->ReadIOShort(this->m_iBitAddress, (short*)m_pSingnalData, this->m_strTargetComportName.c_str());
			}
			else
			if (this->m_iDataSize == 4)
			{
				l_bResult = g_pIOSMDriverInterface->ReadIOInt(this->m_iBitAddress, (int*)m_pSingnalData, this->m_strTargetComportName.c_str());
			}
			if(l_bResult)
			{
				m_bDataReaded = true;
				if (m_IOSignalReadedFunction)
				{
					m_IOSignalReadedFunction();
				}
			}
		}
		else
		if (m_bWaitForWrite && this->m_eIODataType == eIODT_WRITE)
		{
			if (this->m_iDataSize == 1)
			{
				g_pIOSMDriverInterface->WriteIOChar(m_iBitAddress, *(char*)(m_pSingnalData), this->m_strTargetComportName.c_str());
			}
			else
			if (this->m_iDataSize == 2)
			{
				g_pIOSMDriverInterface->WriteIOShort(m_iBitAddress, *(short*)(m_pSingnalData), this->m_strTargetComportName.c_str());
				*(short*)(m_pSingnalData) = 0;//reset data ,now only use by ButtonCounterRegister
			}
			else
			if (this->m_iDataSize == 4)
			{
				g_pIOSMDriverInterface->WriteIOInt(m_iBitAddress, *(int*)(m_pSingnalData), this->m_strTargetComportName.c_str());
			}
			m_bWaitForWrite = false;
			if (m_IOSignalWriteFunction)
			{
				m_IOSignalWriteFunction();
			}
		}
	}
}

short	cIOPackData::GetDataByShort()
{
	assert(this->m_iDataSize == 2 && "DataSize is not short");
	char*l_pData = (char*)m_pSingnalData;
	char l_cValue[2] = { l_pData[1],l_pData[0] };
	return *(short*)l_cValue;
}

int		cIOPackData::GetDataByInt()
{
	assert(this->m_iDataSize == 4 && "DataSize is not Int");
	return *(int*)m_pSingnalData;
}

unsigned char	cIOPackData::GetDataByByte()
{
	return *(unsigned char*)m_pSingnalData;
}

bool	cIOPackData::SetData(short e_shValue)
{
	if (this->m_iDataSize == 2)
	{
		*(short*)m_pSingnalData = e_shValue;
		this->m_bWaitForWrite = true;
		return true;
	}
	return false;
}

bool	cIOPackData::SetData(int e_iValue)
{
	if (this->m_iDataSize == 4)
	{
		*(int*)m_pSingnalData = e_iValue;
		this->m_bWaitForWrite = true;
		return true;
	}
	return false;
}

bool	cIOPackData::SetData(unsigned char	e_ucData)
{
	*(unsigned char*)m_pSingnalData = e_ucData;
	unsigned char l_uc = *(unsigned char*)m_pSingnalData;
	this->m_bWaitForWrite = true;
	//return g_pIOSMDriverInterface->WriteIOChar(m_iBitAddress, e_ucData);
	if (g_pIOSMDriverInterface)
	{
		return true;
	}
	return false;
}

void	cIOPackData::SetToZero()
{
	int l_iSize = this->GetDataSize();
	if (l_iSize == 1)
	{
		this->SetData((unsigned char)0);
	}
	else
	if (l_iSize == 2)
	{
		this->SetData((short)0);
	}
	else
	if (l_iSize == 4)
	{
		this->SetData((int)0);
	}
}

//====================
//<IOData MemoryAdress="" BitAdress="" Bounce="" />
//====================

	//bool	m_bOn;
	//GET_SET_DEC( DWORD, m_iBitAddress, GetMemoryAddress, SetBitAddress );
	//GET_SET_DEC( BYTE, m_byBitAddress, GetBitAddress, SetBitAddress );
	//GET_SET_DEC( float, m_fBounce, GetBounce, SetBounce );


void	cIODataManager::ProcessIOPackData(TiXmlElement*e_pTiXmlElement)
{
	cIOPackData*l_pIOSMIOData = new cIOPackData();
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("Name")
		{
			l_pIOSMIOData->SetName(l_strValue);
		}
		else
		COMPARE_NAME("BitAddress")
		{
			l_pIOSMIOData->SetBitAddress(_wtoi(l_strValue));
		}
		else
		COMPARE_NAME("ByteAddress")
		{
			l_pIOSMIOData->SetBitAddress(_wtoi(l_strValue));
		}
		else
		COMPARE_NAME("KeyData")
		{
			SetKeyASCII(l_pIOSMIOData, l_strValue);
		}
		else
		COMPARE_NAME("KeyValue")
		{
			SetKeyValue(l_pIOSMIOData, l_strValue);
		}
		else
		COMPARE_NAME("Size")
		{
			l_pIOSMIOData->SetDataSize(VALUE_TO_INT);
		}
		else
		COMPARE_NAME("IOType")
		{
			l_pIOSMIOData->SeteIODataType((eIODataType)(VALUE_TO_BOOLEAN));
		}
		else
		COMPARE_NAME("TargetComportName")
		{
			l_pIOSMIOData->m_strTargetComportName = l_strValue;
		}
	PARSE_NAME_VALUE_END
	if(!this->AddObject(l_pIOSMIOData))
	{
		UT::ErrorMsg(L"io name exists",l_pIOSMIOData->GetName());
	}
	//cGameApp::OutputDebugInfoString(l_pIOSMIOData->GetName());
	if (wcscmp(L"Ticket_9", l_pIOSMIOData->GetName()) == 0)
	{

		int a = 0;
	}
		
}

void	cIODataManager::ProcessIOData(TiXmlElement*e_pTiXmlElement)
{
	cIOBitData*l_pIOSMIOData = new cIOBitData();
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("Name")
		{
			l_pIOSMIOData->SetName(l_strValue);
		}
		else
		COMPARE_NAME("BitAddress")
		{
			l_pIOSMIOData->SetBitAddress(_wtoi(l_strValue));
		}
		else
		COMPARE_NAME("KeyData")
		{
			SetKeyASCII(l_pIOSMIOData, l_strValue);
		}
		else
		COMPARE_NAME("KeyValue")
		{
			SetKeyValue(l_pIOSMIOData, l_strValue);
		}
		else
		COMPARE_NAME("IOType")
		{
			l_pIOSMIOData->SeteIODataType((eIODataType)(VALUE_TO_BOOLEAN));
		}
		else
		COMPARE_NAME("TargetComportName")
		{
			l_pIOSMIOData->m_strTargetComportName = l_strValue;
		}
	PARSE_NAME_VALUE_END
	if(!this->AddObject(l_pIOSMIOData))
	{
		UT::ErrorMsg(L"io name exists",l_pIOSMIOData->GetName());
	}
	//cGameApp::OutputDebugInfoString(l_pIOSMIOData->GetName());
}

bool	cIODataManager::MyParse(TiXmlElement*e_pRoot)
{
	if (e_pRoot)
	{
		e_pRoot = e_pRoot->FirstChildElement();
		//COMPARE_VALUE("IODataInfoList")
		//{
		//	const WCHAR*l_strCount = e_pRoot->Attribute(L"Count");
		//	this->m_ObjectList.reserve(_wtoi(l_strCount));
		//}
	}
	if (!e_pRoot)
		return false;
	const WCHAR*l_strValue = e_pRoot->Value();
	while (e_pRoot)
	{
		COMPARE_VALUE("IOData")
		{
			ProcessIOData(e_pRoot);
		}
		else
		COMPARE_VALUE("IOPackData")
		{
			ProcessIOPackData(e_pRoot);
		}
		e_pRoot = e_pRoot->NextSiblingElement();
		if(e_pRoot)
			l_strValue = e_pRoot->Value();
	}
	cGameApp::OutputDebugInfoString(L"IO Parse Finished!");
	return true; 
}

void	cIODataManager::SetKeyValue(cIOBitData*e_pIOBitData, const wchar_t*e_strValue)
{
	unsigned char*l_pucKeyData = new unsigned char;
	*l_pucKeyData = (unsigned char)GetInt(e_strValue);
	int	l_iValue = (int)(*l_pucKeyData);
	e_pIOBitData->SetKeyData(l_pucKeyData);
	//#ifdef _DEBUG
	//#ifdef WIN32
	//				OutputDebugString(l_pIOSMIOData->GetName());
	//				OutputDebugString(L" Value:");
	//				OutputDebugString(ValueToStringW(l_iValue));
	//				OutputDebugString(L",");
	//				OutputDebugString(l_strValue);
	//				OutputDebugString(L"\n");
	//#endif
	//#endif	
}
void	cIODataManager::SetKeyASCII(cIOBitData*e_pIOBitData, const wchar_t*e_strValue)
{
	unsigned char*l_pucKeyData = new unsigned char;
	if (wcslen(e_strValue) == 1)
	{
		*l_pucKeyData = (unsigned char)e_strValue[0];
	}
	else
	{
		UT::ErrorMsg("ERROR!","Key Data only support character");
	}
	int	l_iValue = (int)(*l_pucKeyData);
	e_pIOBitData->SetKeyData(l_pucKeyData);
	//#ifdef _DEBUG
	//#ifdef WIN32
	//				OutputDebugString(l_pIOSMIOData->GetName());
	//				OutputDebugString(L" Value:");
	//				OutputDebugString(ValueToStringW(l_iValue));
	//				OutputDebugString(L",");
	//				OutputDebugString(l_strValue);
	//				OutputDebugString(L"\n");
	//#endif
	//#endif
}

cIODataManager::cIODataManager()
{
	
}

cIODataManager::~cIODataManager()
{
	
}

void		cIODataManager::Init()
{
	int l_iCount = this->Count();
	for (int i = 0; i < l_iCount; ++i)
	{
		auto l_pData = this->GetObject(i);
		if (l_pData)
			l_pData->Init();
	}
}

cIOBitData*	cIODataManager::GetObjectByKeyData(unsigned char e_ucKey)
{
	int	l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		unsigned char*l_puc = this->m_ObjectList[i]->GetKeyData();
		if( l_puc && *l_puc == e_ucKey )
		{
			return this->m_ObjectList[i];
		}
	}
	return nullptr;
}

#ifdef WIN32
bool	cIODataManager::Export(char*e_strFileName)
{
	//if target time is 0.it won't switch status
	sTimeCounter	m_TimeCounterForHighCircle;
	sTimeCounter	m_TimeCounterForLowCircle;
	//IsActive
	sTimeCounter	m_TimeCounterForActiveDelay;
	sTimeCounter	m_TimeCounterForInactiveDelay;

	int	l_iNum = this->Count();
	if( l_iNum>0 )
	{
		ATG::XMLWriter	l_XMLWriter(e_strFileName);
		l_XMLWriter.StartElement("IODataInfoList");
		l_XMLWriter.AddAttribute("Count",l_iNum);
		cIOBitData**l_ppIOSMIOData = &this->m_ObjectList[0];
		cIOBitData*l_pIOSMIOData = 0;
		for( int i=0;i<l_iNum;++i )
		{
			l_pIOSMIOData = l_ppIOSMIOData[i];
			l_XMLWriter.StartElement("IOData");
			l_XMLWriter.AddAttribute("Name",l_pIOSMIOData->GetName());

			if(l_pIOSMIOData->GetKeyData())
				l_XMLWriter.AddAttribute("KeyData",(char*)l_pIOSMIOData->GetKeyData());

			l_XMLWriter.AddAttribute("MemoryAddress",(int)l_pIOSMIOData->m_iBitAddress);
			l_XMLWriter.EndElement();//end IOData.
		}
		l_XMLWriter.EndElement();//end IODataInfoList.
		return true;
	}
	return false;
}
#endif

void	cIODataManager::DumpOccupiedKeyValue()
{
	bool l_bOccupiedKeyValue[MAX_PATH];
	memset(l_bOccupiedKeyValue, 0, sizeof(bool)*MAX_PATH);
	int	l_iNum = this->Count();
	for(int i=0;i<l_iNum;++i)
	{
		auto l_pObject = this->GetObject(i);
		if (l_pObject->GeteIODataType() == eIODT_READ)
		{
			if (l_pObject->GetKeyData())
			{
				if (l_bOccupiedKeyValue[*l_pObject->GetKeyData()])
				{
					OutputDebugString(l_pObject->GetName());
					OutputDebugString(L"    ERROR!!! key value same!\n");
				}
				l_bOccupiedKeyValue[*l_pObject->GetKeyData()] = true;
			}
			else
			{//should't happen
				
			}
		}
	}
	std::wstring l_strDebugInfo;
	int	l_iIOCount = 0;
	for (int i = 0; i < MAX_PATH; ++i)
	{
		if (l_bOccupiedKeyValue[i])
		{
			l_strDebugInfo += ValueToStringW(i);
			l_strDebugInfo += L",";
			++l_iIOCount;
			if (l_iIOCount % 10 == 0)
				l_strDebugInfo += L"\n";
		}
	}
	OutputDebugString(l_strDebugInfo.c_str());
}