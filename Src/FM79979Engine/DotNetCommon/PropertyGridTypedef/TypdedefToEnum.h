#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Globalization;

ref class EnumListConvert :StringConverter
{
public:System::Collections::ArrayList^m_sNameList;
public:EnumListConvert()
		{
			m_sNameList = gcnew System::Collections::ArrayList();
		}
public:~EnumListConvert()
		{

		}
	   bool	AddString(String^e_pString)
	   {		   
			for each(String^l_pString in m_sNameList)
			{
				if(l_pString->Equals(e_pString))
					return false;
			}
			m_sNameList->Add(e_pString);
			return true;
	   }
	public:virtual bool GetStandardValuesSupported(ITypeDescriptorContext^ context) override
	{
		return true;
	}
	public:virtual  StandardValuesCollection^ GetStandardValues(ITypeDescriptorContext^ context) override
	{
		array<String^>^l_pStringList = gcnew array<String^>(m_sNameList->Count);
		for( int i=0;i<m_sNameList->Count;i++ )
		{
			l_pStringList->SetValue(m_sNameList[i],i);
		}		
		return gcnew StandardValuesCollection(m_sNameList);
	}
	public:virtual bool GetStandardValuesExclusive(ITypeDescriptorContext^ context) override
	{
		return false;
	 }	
};

#define ROTATE_ENUM	gcnew array<String^>{"Local", "World"}

#define D3DBLEND_ENUM	gcnew array<String^>{	\
    "D3DBLEND_ZERO = 1",			\
    "D3DBLEND_ONE = 2",				\
    "D3DBLEND_SRCCOLOR = 3",		\
    "D3DBLEND_INVSRCCOLOR = 4",		\
    "D3DBLEND_SRCALPHA = 5",		\
    "D3DBLEND_INVSRCALPHA = 6",		\
    "D3DBLEND_DESTALPHA = 7",		\
    "D3DBLEND_INVDESTALPHA = 8",	\
    "D3DBLEND_DESTCOLOR = 9",		\
    "D3DBLEND_INVDESTCOLOR = 10",	\
    "D3DBLEND_SRCALPHASAT = 11",	\
    "D3DBLEND_BOTHSRCALPHA = 12",	\
    "D3DBLEND_BOTHINVSRCALPHA = 13",\
    "D3DBLEND_BLENDFACTOR = 14",	\
    "D3DBLEND_INVBLENDFACTOR = 15",	\
    "D3DBLEND_SRCCOLOR2 = 16",		\
	"D3DBLEND_INVSRCCOLOR2 = 17"}	



#define NEW_ENUM(Name,enumArray)	\
ref class Name :StringConverter	\
{	\
	public:virtual bool GetStandardValuesSupported(ITypeDescriptorContext^ context) override	\
	{	\
		return true;	\
	}	\
	public:virtual  StandardValuesCollection^ GetStandardValues(ITypeDescriptorContext^ context) override	\
	{	\
		return gcnew StandardValuesCollection(enumArray);	\
	}	\
	public:virtual bool GetStandardValuesExclusive(ITypeDescriptorContext^ context) override	\
	{	\
		return false;	\
	 }	\
};
//NEW_ENUM(EnumString,TEST_ENUM)
NEW_ENUM(EnumString,ROTATE_ENUM);
NEW_ENUM(D3DBLEND_EnumString,D3DBLEND_ENUM);



#define ATTACH_TYPE_ENUM	gcnew array<String^>{"CAMERA","MODEL","FRAME_OF_FRAME","FRAME_OF_MESH"}

NEW_ENUM(AttachEnumString,ATTACH_TYPE_ENUM);