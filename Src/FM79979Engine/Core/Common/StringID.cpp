#include "StringID.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#ifdef WIN32
#pragma warning( disable : 4996 )
#endif
const wchar_t*				StringID::s_EmptyString = L"";
std::list<const wchar_t *>*	StringID::m_spStringLists = nullptr;
unsigned long long	g_ui64NumObjectInUsing = 0;

StringID::StringID()                          
{
	m_strString = s_EmptyString; 
	++g_ui64NumObjectInUsing;
}

StringID::StringID( const wchar_t* strString )
{
	++g_ui64NumObjectInUsing;
	m_strString = AddString( strString ); 
}

StringID::StringID( const StringID& other )
{
	++g_ui64NumObjectInUsing;
	m_strString = other.m_strString; 
}

StringID::~StringID()
{
	--g_ui64NumObjectInUsing;
	if( g_ui64NumObjectInUsing == 0 )
		StringID::DestoryStringTable();
}
//-----------------------------------------------------------------------------
// Name: StringID::GetStringTable
// Desc: returns static string table data- used to ensure initialization is done
//-----------------------------------------------------------------------------
std::list<const wchar_t *>* StringID::GetStringTable() 
{
	if(!StringID::m_spStringLists)
	{
		StringID::m_spStringLists = new std::list<const wchar_t *>[StringID_HASHSIZE];
		printf("StringID::m_spStringLists = new std::list<const wchar_t *>[StringID_HASHSIZE];");
	}
    return StringID::m_spStringLists;
}


void	StringID::DestoryStringTable()
{
	for( int i=0;i<StringID_HASHSIZE;++i )
	{
		std::list<const wchar_t*>& CurrentList = GetStringTable()[ i ];
		std::list<const wchar_t*>::iterator iter = CurrentList.begin();
		std::list<const wchar_t*>::iterator end = CurrentList.end();
		while( iter != end )
		{
			const wchar_t* strTest = *iter;
			++iter;
			delete strTest;
		}
		CurrentList.clear();
	}
#ifdef DEBUG
	printf("SAFE_DELETE_ARRAY(m_spStringLists)");
#endif
	if (m_spStringLists)
	{
		delete[]  m_spStringLists;
		m_spStringLists = nullptr;
	}
}
//-----------------------------------------------------------------------------
// Name: StringID::operator==
// Desc: compare a string with a wchar_t 
//-----------------------------------------------------------------------------
bool StringID::operator== ( const wchar_t* strRHS ) const
{
    if( strRHS == nullptr )
    {
        if( m_strString == s_EmptyString )
            return true;
        return false;
    }

    if( m_strString == strRHS )
        return true;

    return ( wcscmp( m_strString, strRHS ) == 0 );
}

//-----------------------------------------------------------------------------
// Name: StringID::AddString 
// Desc: Add a string to the string table
//-----------------------------------------------------------------------------
const wchar_t* StringID::AddString( const wchar_t* strString )
{
    if( strString == nullptr )
        return nullptr;
    if( wcslen(strString) == 0 )
        return s_EmptyString;

    int uBucketIndex = HashString( strString ) % StringID_HASHSIZE;
	auto l_pTable = GetStringTable();
    std::list<const wchar_t*>* CurrentList = &l_pTable[ uBucketIndex ];

    std::list<const wchar_t*>::iterator iter = CurrentList->begin();
    std::list<const wchar_t*>::iterator end = CurrentList->end();

    while( iter != end )
    {
        const wchar_t* strTest = *iter;
        if( wcscmp( strTest, strString ) == 0 )
            return strTest;
        ++iter;
    }
    
    // $OPTIMIZE: use a fixed size allocator here
    unsigned long bufferLength = (unsigned long)(wcslen( strString ) + 1);
    wchar_t* strCopy = new wchar_t[ bufferLength ];
	wcscpy(strCopy, strString);
    //wcscpy_s( strCopy, bufferLength, strString );
    CurrentList->push_back( strCopy );
    return strCopy;
}


//-----------------------------------------------------------------------------
// Name: StringID::HashString
// Desc: Create a hash value from a string
//-----------------------------------------------------------------------------
unsigned long StringID::HashString( const wchar_t* strString )
{
    unsigned long HashVal = 0;        
    const wchar_t *pChar;

    for ( pChar = strString; *pChar; pChar++ )
    {
        HashVal += *pChar * 193951;
        HashVal *= 399283;
    }
    return HashVal;
}
