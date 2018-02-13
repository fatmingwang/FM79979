//#ifdef __FUCK__
#include "stdafx.h"
//#include "IOSMIOData.h"
//#include "IOSMDeviceBehavior.h"
//#include "IOSMBaseDevice.h"
//#include "IOSMDriverInterface.h"
//
//namespace IOSM
//{
//	cIOSMIOData::cIOSMIOData()
//	{
//		m_pucKeyData = 0;
//		m_bSetStartSingnal = false;
//		m_bStartSingnal = false;
//		m_bCurrentSingnale = false;
//		m_bProviousCurrentSingnale = false;
//		m_bWorkingSingnal = true;
//		m_bFirstActive = false;;
//		m_bFirstInactive = false;
//		m_bEnable = true;
//		m_TimeCounterForActiveDelay.SetTargetTime(0);
//		m_TimeCounterForInactiveDelay.SetTargetTime(0);
//		m_TimeCounterForHighCircle.SetTargetTime(0);
//		m_TimeCounterForLowCircle.SetTargetTime(0);
//		m_pIOSMBaseDevice = 0;
//		m_dwMemoryAddress = 0;
//		m_byBitAddress = 0;
//		m_eIODataType = eIODT_INPUT;
//	}
//
//	cIOSMIOData::~cIOSMIOData()
//	{
//		SAFE_DELETE(m_pucKeyData);
//	}
//
//	void	cIOSMIOData::SetSingnal(bool e_bOn,cIOSMIOData*e_cIOSMIOData)
//	{
//		if( !m_pIOSMBaseDevice )
//			m_bCurrentSingnale = e_bOn;
//		else
//		{
//			m_pIOSMBaseDevice->SetSingnal(e_cIOSMIOData,e_bOn);
//		}
//	}
//
//	void	cIOSMIOData::Init()
//	{
//		if(!this->m_bSetStartSingnal)//if not force to set,keep it as original status,if it's simulation we should set it as randomly
//		{
//			m_bCurrentSingnale = m_bStartSingnal;
//			this->WriteIO(m_bCurrentSingnale);
//		}
//		else
//		{
//			if( g_bSimulation )
//			{
//				this->m_bCurrentSingnale = rand()%2?true:false;
//				this->WriteIO(m_bCurrentSingnale);
//			}
//			else
//			{
//				if(!g_pIOSMDriverInterface->ReadIOBitAddress(this->m_dwMemoryAddress,m_byBitAddress,&m_bCurrentSingnale))
//				{
//				
//				}
//			}
//		}
//		this->FetchSingnal(&m_bCurrentSingnale);
//		m_TimeCounterForHighCircle.Start();
//		m_TimeCounterForLowCircle.Start();
//		m_TimeCounterForActiveDelay.Start();
//		m_TimeCounterForInactiveDelay.Start();
//	}
//
//	void	cIOSMIOData::DoHignLowCircle(float e_fElpaseTime)
//	{
//		//if singnal is changed,write IO,and set time counter to start
//		if( this->m_bCurrentSingnale != this->m_bProviousCurrentSingnale )
//		{
//			this->WriteIO(m_bCurrentSingnale);
//			if(this->IsActive())
//				this->m_bFirstActive = true;
//			else
//				this->m_bFirstInactive = true;
//		}
//		m_bProviousCurrentSingnale = this->m_bCurrentSingnale;
//		if(this->m_bCurrentSingnale)
//		{
//			if(this->m_TimeCounterForHighCircle.fTargetTime	> 0.f)//if 0 it's infinity,do not change status
//			{
//				m_TimeCounterForHighCircle.Update(e_fElpaseTime);
//				if(m_TimeCounterForHighCircle.bTragetTimrReached)
//				{//delay if need
//					this->m_bCurrentSingnale = false;
//					this->m_TimeCounterForLowCircle.Start();
//				}
//			}
//			else//infinity
//			{
//			
//			}
//		}
//		else
//		{
//			if(this->m_TimeCounterForLowCircle.fTargetTime	> 0.f)//if 0 it's infinity,do not change status
//			{
//				m_TimeCounterForLowCircle.Update(e_fElpaseTime);
//				if(m_TimeCounterForLowCircle.bTragetTimrReached)
//				{//delay if need
//					this->m_bCurrentSingnale = true;
//					this->m_TimeCounterForHighCircle.Start();
//				}
//			}
//			else//infinity
//			{
//			
//			}
//		}	
//	}
//
//	void	cIOSMIOData::FetchSingnal(bool*e_pSingnal)
//	{
//		if( this->m_eIODataType == eIODT_OUTPUT)
//		{
//			g_pIOSMDriverInterface->ReadOutputData(m_dwMemoryAddress,e_pSingnal);
//		}
//		else
//		{
//			g_pIOSMDriverInterface->ReadInputData(m_dwMemoryAddress,e_pSingnal);
//		}
//	}
//
//	void	cIOSMIOData::Update(float e_fElpaseTime)
//	{
//		if( m_pucKeyData  )
//		{
//			FetchSingnal(&m_bCurrentSingnale);
//			//if( !l_bData )
//			//{
//			//	std::wstring	l_str = L"Input:";
//			//	l_str += ValueToStringW((int)m_dwMemoryAddress);;
//			//	OutputDebugString(l_str.c_str());
//			//}
//			//high working
//			//do io delay offset.
//			//if( l_bData != this->m_bCurrentSingnale )
//			//{
//			//	
//			//}
//			if( !m_bWorkingSingnal )
//			{
//				m_bCurrentSingnale = !m_bCurrentSingnale;
//				//if( !l_bData )
//				//{
//				//	printf("Input:%d\n",m_dwMemoryAddress);
//				//}
//			}
//			cGameApp::m_sucKeyData[*m_pucKeyData ] = m_bCurrentSingnale;
//		}
//	}
//
//	void	cIOSMIOData::SetCurrentSingnal(bool e_b)
//	{
//		if(this->m_bEnable)
//			this->m_bCurrentSingnale = e_b;
//	}
//
//	bool	cIOSMIOData::WriteIO(bool e_b)
//	{
//		if(this->m_eIODataType == eIODT_INPUT)
//		{
//			return g_pIOSMDriverInterface->WriteInputData(this->m_dwMemoryAddress,e_b);
//		}
//		else
//			return g_pIOSMDriverInterface->WriteOutputData(this->m_dwMemoryAddress,e_b);
//	}
//
//	bool	cIOSMIOData::ReadIO()
//	{
//		return m_bCurrentSingnale;
//	}
//
//	bool	cIOSMIOData::ForceReadIO(bool*e_pbOutData)
//	{
//		if(g_pIOSMDriverInterface->ReadIOBitAddress(this->m_dwMemoryAddress,m_byBitAddress,e_pbOutData))
//			return true;
//		return false;
//	}
//
//	void	cIOSMIOData::SetEnable(bool e_b)
//	{
//		if( e_b )
//		{
//			this->m_TimeCounterForActiveDelay.Start();
//		}
//		else
//		{
//			this->m_TimeCounterForInactiveDelay.Start();
//		}
//		//start from rest time do not start from target time.
//		//if( m_TimeCounterForHighCircle.fRestTime<0.f )
//		//	m_TimeCounterForHighCircle.Start();
//		//if( m_TimeCounterForLowCircle.fRestTime<0.f )
//		//	m_TimeCounterForLowCircle.Start();
//		this->m_bEnable = e_b;
//	}
//
//	WCHAR* cIOSMIOData::GetDeviceInfo()
//	{
//		static WCHAR	l_strInfo[MAX_PATH];
////	swprintf(l_strInfo,MAX_PATH,L"Name:%ls,Singnal:%d,BitAddress:%d,MemoryAdress:%d,Bounce:%f\0",GetName(),m_bCurrentSingnale?1:0,(int)m_byBitAddress,(int)m_dwMemoryAddress,this->m_fBounce);
//		return l_strInfo;
//	}
//	//====================
//	//<IOData MemoryAdress="" BitAdress="" Bounce="" />
//	//====================
//
//		//bool	m_bOn;
//		//GET_SET_DEC( DWORD, m_dwMemoryAddress, GetMemoryAddress, SetMemoryAddress );
//		//GET_SET_DEC( BYTE, m_byBitAddress, GetBitAddress, SetBitAddress );
//		//GET_SET_DEC( float, m_fBounce, GetBounce, SetBounce );
//
//
//	void	cIOSMIODataManager::ProcessIOData()
//	{
//		cIOSMIOData*l_pIOSMIOData = new cIOSMIOData();
//		PARSE_NAME_VALUE_START
//			COMPARE_NAME("Name")
//			{
//				l_pIOSMIOData->SetName(l_strValue);
//			}
//			else
//			COMPARE_NAME("SetStartSingnal")
//			{
//				l_pIOSMIOData->m_bSetStartSingnal = _wtoi(l_strValue)?true:false;
//			}
//			else
//			COMPARE_NAME("StartSingnal")
//			{
//				l_pIOSMIOData->m_bStartSingnal = _wtoi(l_strValue)?true:false;
//			}
//			else
//			COMPARE_NAME("MemoryAddress")
//			{
//				l_pIOSMIOData->SetMemoryAddress(_wtoi(l_strValue));
//			}
//			else
//			COMPARE_NAME("BitAdress")
//			{
//				l_pIOSMIOData->SetBitAddress((char)_wtoi(l_strValue));
//			}
//			else
//			COMPARE_NAME("HighCircle")
//			{
//				l_pIOSMIOData->SetHighCircle((float)_wtof(l_strValue));
//			}
//			else
//			COMPARE_NAME("LowCircle")
//			{
//				l_pIOSMIOData->SetLowCircle((float)_wtof(l_strValue));
//			}
//			else
//			COMPARE_NAME("ActiveDelay")
//			{
//				l_pIOSMIOData->SetDelayTimeForActive((float)_wtof(l_strValue));
//			}
//			else
//			COMPARE_NAME("InactiveDelay")
//			{
//				l_pIOSMIOData->SetDelayTimeForInactive((float)_wtof(l_strValue));
//			}
//			else
//			COMPARE_NAME("Enable")
//			{
//				bool	l_b = _wtoi(l_strValue)?true:false;
//				l_pIOSMIOData->SetEnable(l_b);
//				if( l_b )
//					l_pIOSMIOData->m_TimeCounterForActiveDelay.bTragetTimrReached = true;
//				else
//					l_pIOSMIOData->m_TimeCounterForInactiveDelay.bTragetTimrReached = true;
//			}
//			else
//			COMPARE_NAME("WorkingSingnal")
//			{
//				l_pIOSMIOData->SetWorkingSingnal(_wtoi(l_strValue));
//			}
//			else
//			COMPARE_NAME("KeyData")
//			{
//				unsigned char*l_pucKeyData = new unsigned char;
//				*l_pucKeyData = l_strValue[0];
//				int	l_iValue = (int)(*l_pucKeyData);
//				//OutputDebugString(L"Value:");
//				//OutputDebugString(ValueToStringW(l_iValue));
//				//OutputDebugString(L",");
//				//OutputDebugString(l_strValue);
//				//OutputDebugString(L"\n");
//				l_pIOSMIOData->SetKeyData(l_pucKeyData);
//			}
//			else
//			COMPARE_NAME("IOType")
//			{
//				COMPARE_VALUE("Input")
//				{
//					l_pIOSMIOData->SeteIODataType(eIODT_INPUT);
//				}
//				else
//				{
//					l_pIOSMIOData->SeteIODataType(eIODT_OUTPUT);
//				}
//			}
//		PARSE_NAME_VALUE_END
//		bool	l_b = this->AddObject(l_pIOSMIOData);
//		assert(l_b);
//	}
//
//	void	cIOSMIODataManager::HandleElementData(TiXmlElement*e_pTiXmlElement)
//	{
//		const WCHAR*l_strValue = e_pTiXmlElement->Value();
//		COMPARE_VALUE("IODataInfoList")
//		{
//			const WCHAR*l_strCount = e_pTiXmlElement->Attribute(L"Count");
//			this->m_ObjectList.reserve(_wtoi(l_strCount));
//		}
//		else
//		COMPARE_VALUE("IOData")
//		{
//			ProcessIOData();
//		}
//	}
//
//	cIOSMIODataManager::cIOSMIODataManager()
//	{
//	
//	}
//
//	cIOSMIODataManager::~cIOSMIODataManager()
//	{
//	
//	}
//
//	cIOSMIOData*	cIOSMIODataManager::GetObjectByKeyData(unsigned char e_ucKey)
//	{
//		int	l_iCount = this->Count();
//		for( int i=0;i<l_iCount;++i )
//		{
//			unsigned char*l_puc = this->m_ObjectList[i]->GetKeyData();
//			if( l_puc && *l_puc == e_ucKey )
//			{
//				return this->m_ObjectList[i];
//			}
//		}
//		return 0;
//	}
//
//#ifdef WIN32
//	bool	cIOSMIODataManager::Export(char*e_strFileName)
//	{
//		//if target time is 0.it won't switch status
//		sTimeCounter	m_TimeCounterForHighCircle;
//		sTimeCounter	m_TimeCounterForLowCircle;
//		//it's not necessary,and it will to determinate by m_bWorkingSingnal and m_bCurrentSingnale.
//		//IsActive
//		sTimeCounter	m_TimeCounterForActiveDelay;
//		sTimeCounter	m_TimeCounterForInactiveDelay;
//
//		int	l_iNum = this->Count();
//		if( l_iNum>0 )
//		{
//			ATG::XMLWriter	l_XMLWriter(e_strFileName);
//			l_XMLWriter.StartElement("IODataInfoList");
//			l_XMLWriter.AddAttribute("Count",l_iNum);
//			cIOSMIOData**l_ppIOSMIOData = &this->m_ObjectList[0];
//			cIOSMIOData*l_pIOSMIOData = 0;
//			for( int i=0;i<l_iNum;++i )
//			{
//				l_pIOSMIOData = l_ppIOSMIOData[i];
//				l_XMLWriter.StartElement("IOData");
//				l_XMLWriter.AddAttribute("Name",l_pIOSMIOData->GetName());
//
//				if(l_pIOSMIOData->GetKeyData())
//					l_XMLWriter.AddAttribute("KeyData",(char*)l_pIOSMIOData->GetKeyData());
//				if( l_pIOSMIOData->GeteIODataType() == eIODT_INPUT)
//					l_XMLWriter.AddAttribute("IOType","Input");
//				else
//					l_XMLWriter.AddAttribute("IOType","Output");
//
//				l_XMLWriter.AddAttribute("SetStartSingnal",l_pIOSMIOData->m_bSetStartSingnal?1:0);
//				l_XMLWriter.AddAttribute("StartSingnal",l_pIOSMIOData->m_bStartSingnal?1:0);
//				l_XMLWriter.AddAttribute("MemoryAddress",(int)l_pIOSMIOData->m_dwMemoryAddress);
//				l_XMLWriter.AddAttribute("BitAdress",(int)l_pIOSMIOData->m_byBitAddress);
//				l_XMLWriter.AddAttribute("HighCircle",l_pIOSMIOData->m_TimeCounterForHighCircle.fTargetTime);
//				l_XMLWriter.AddAttribute("LowCircle",l_pIOSMIOData->m_TimeCounterForLowCircle.fTargetTime);
//				l_XMLWriter.AddAttribute("ActiveDelay",l_pIOSMIOData->m_TimeCounterForActiveDelay.fTargetTime);
//				l_XMLWriter.AddAttribute("InactiveDelay",l_pIOSMIOData->m_TimeCounterForInactiveDelay.fTargetTime);
//				l_XMLWriter.AddAttribute("WorkingSingnal",l_pIOSMIOData->IsWorkingSingnal()?"1":"0");
//				l_XMLWriter.AddAttribute("Enable",l_pIOSMIOData->IsEnable()?"1":"0");
//				l_XMLWriter.EndElement();//end IOData.
//			}
//			l_XMLWriter.EndElement();//end IODataInfoList.
//			return true;
//		}
//		return false;
//	}
//#endif
////end namespace IOSM
//}
//ebd ebdif
//#endif