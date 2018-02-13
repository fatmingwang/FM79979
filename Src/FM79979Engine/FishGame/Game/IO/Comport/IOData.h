#ifndef _IOSM_IO_DATA_H_
#define _IOSM_IO_DATA_H_



class cIODataManager;
class cIOControlInterface;
//====================
//<IOData MemoryAdress="" BitAdress="" Bounce="" />
//
//cIOSMBaseDevice will update it's child behavior,then update IOData,so even change IOData's current singnal,it doesn't mean to write IO,
//it will check by behavior's update,if condition is full,singnal will be changed.
//====================
enum	eIODataType
{
	eIODT_READ = 0,
	eIODT_WRITE,
	eIODT_MAX
};

typedef std::function<bool()>	IOSignalReadedFunction;
typedef std::function<bool()>	IOSignalWriteFunction;

class cIOBitData:public NamedTypedObject
{
	friend	class	cIODataManager;
protected:
	bool	m_bDataReaded;
	void*	m_pSingnalData;
	//
	GET_SET_DEC( int, m_iBitAddress, GetBitAddress, SetBitAddress);
	//key data,update cGameApp::m_sucKeyData
	GET_SET_DEC(unsigned char*,m_pucKeyData,GetKeyData,SetKeyData);
	GET_SET_DEC(eIODataType, m_eIODataType, GeteIODataType, SeteIODataType);
	bool	m_bWaitForWrite;
	//it could be have many different comport data,default value is
	std::wstring	m_strTargetComportName;
public:
	cIOBitData();
	~cIOBitData();
	DEFINE_TYPE_INFO();
	bool	IsDataReaded() { return m_bDataReaded; }
	//set start status
	void	Init();
	//this update will setup to write data into memory,it'd called in the last of Device's update to ensure it's time to write data
	virtual	void	Update(float e_fElpaseTime);
	//
	bool			SetDataByBoolean(bool e_b);
	bool			IsSingnalActive();
	//I am lazy to make this as function...
	IOSignalReadedFunction	m_IOSignalReadedFunction;
	IOSignalWriteFunction	m_IOSignalWriteFunction;
	//void	SetCurrentSingnal(bool e_b);
};

//io noly has 1bit(1/0),but some io is a device has more than 1 bit,so we has this now
//this one only for get data
//size as byte
//    <IOPackData Name="Spin" Read="false"	KeyData="S" MemoryAddress="5" Size="1" />
class cIOPackData:public cIOBitData
{
	friend	class	cIODataManager;
	//
	int				m_iDataSize;
public:
	cIOPackData();
	~cIOPackData();
	DEFINE_TYPE_INFO();
	void			Init();
	virtual	void	Update(float e_fElpaseTime);
	//
	void			SetDataSize(int	e_iSize);
	int				GetDataSize() {return m_iDataSize;}
	//
	int				GetDataByInt();
	short			GetDataByShort();
	unsigned char	GetDataByByte();
	//
	void*			GetData(){ return m_pSingnalData; }
	bool			SetData(unsigned char	e_ucData);
	bool			SetData(short e_shValue);
	bool			SetData(int e_iValue);
	void			SetToZero();
};

//<IODataInfoList Count="2">
//    <IOData Name="BetMax" 	KeyData="G" IOType="Input" SetStartSingnal="0" StartSingnal="0" MemoryAddress="6" BitAdress="0" HighCircle="0.000000" LowCircle="0.000000" ActiveDelay="0.000000" InactiveDelay="0.000000" WorkingSingnal="0" Enable="1" />
//    <IOData Name="Spin" 	KeyData="S" IOType="Input" SetStartSingnal="0" StartSingnal="0" MemoryAddress="5" BitAdress="0" HighCircle="0.000000" LowCircle="0.000000" ActiveDelay="0.000000" InactiveDelay="0.000000" WorkingSingnal="0" Enable="1" />
//</IODataInfoList>
class	cIODataManager:public cNamedTypedObjectVector<cIOBitData>,public cNodeISAX
{
	void			ProcessIOData(TiXmlElement*e_pTiXmlElement);
	void			ProcessIOPackData(TiXmlElement*e_pTiXmlElement);
	virtual	bool	MyParse(TiXmlElement*e_pRoot);
	//
	void			SetKeyASCII(cIOBitData*e_pIOBitData,const wchar_t*e_strValue);
	void			SetKeyValue(cIOBitData*e_pIOBitData, const wchar_t*e_strValue);
public:
	cIODataManager();
	~cIODataManager();
	void			Init();
	cIOBitData*		GetObjectByKeyData(unsigned char e_ucKey);
#ifdef WIN32
	bool	Export(char*e_strFileName);
	void	DumpOccupiedKeyValue();
#endif
};




#endif