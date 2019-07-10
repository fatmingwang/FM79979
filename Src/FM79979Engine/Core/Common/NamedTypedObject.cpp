#include "../stdafx.h"
#include "NamedTypedObject.h"
#include "../GameplayUT/GameApp.h"
#include "../GameplayUT/Log/FMLog.h"
uint64				g_ui64GlobalUniqueID = 0;
uint64				g_ui64TimeStamp = 0;
NamedTypedObject*	g_pLatestNamedTypedObject = 0;
uint64	GetGlobalUniqueID()
{
	return ++g_ui64GlobalUniqueID;
}

uint64	GetTimeStampID()
{
	return ++g_ui64TimeStamp;
}


TYPDE_DEFINE_MARCO(NamedTypedObject);

#ifdef DEBUG

	bool	g_bDumpUsing = true;
	//0,1,2 no use
#ifdef WASM
	int		g_iDumpUsing = 1;
#else
	int		g_iDumpUsing = 0;
#endif
	NameIndexedCollection*g_pNameIndexedCollection = 0;
	void	DebugResourceInfoAdd(NamedTypedObject*e_pNamedTypedObject)
	{
		if( g_iDumpUsing == 0 )
		{
			if( !g_pNameIndexedCollection )
				g_pNameIndexedCollection = new NameIndexedCollection;
			g_pNameIndexedCollection->Add(e_pNamedTypedObject);
		}
	}

	void	DebugResourceDelete(NamedTypedObject*e_pNamedTypedObject)
	{
		if( g_iDumpUsing == 0  && g_pNameIndexedCollection)
		{
			g_pNameIndexedCollection->Remove(e_pNamedTypedObject);
			if( g_pNameIndexedCollection->Size() == 0 )
			{
				SAFE_DELETE(g_pNameIndexedCollection);
			}
		}
	}
	//if here occur crush,please check the virtual is added before destruct function(~ooxx())
	void	DumpUnReleaseInfo()
	{
		if( g_iDumpUsing == 0 )
		{
			if( g_pNameIndexedCollection )
			{
				unsigned long	l_ulSize = g_pNameIndexedCollection->Size();
				if( l_ulSize != 0 )
				{
					NameIndexedCollection::iterator	i = g_pNameIndexedCollection->begin();
					NameIndexedCollection::iterator	l_End = g_pNameIndexedCollection->end();
					for( ;i != l_End; i++  )
					{
						NamedTypedObject*l_pNamedTypedObject = (NamedTypedObject*)(*i);
						wchar_t	l_strData[MAX_PATH];
						swprintf(l_strData,MAX_PATH,L"unreleaseData---Type:%ls,Name:%ls,UniqueID:%llu\n",l_pNamedTypedObject->Type(),l_pNamedTypedObject->GetName(),l_pNamedTypedObject->GetUniqueID());
						FMLog::Log(l_strData, true);
					}
				}
				SAFE_DELETE(g_pNameIndexedCollection);
			}
		}
	}
	const UINT64	g_ui64MAGICID = 123456789;
	bool	NamedTypedObject::IsMagicIDMatch()
	{
		return m_ui64PointerVaildMagicID == g_ui64MAGICID;;
	}
#endif

NamedTypedObject::NamedTypedObject()
{
	SetupInternalData();
}

NamedTypedObject::~NamedTypedObject()
{
#ifdef DEBUG
	DebugResourceDelete(this);
#endif
	//Destroy();
}


void	NamedTypedObject::SetupInternalData()
{
	this->m_uiUniqueID = GetGlobalUniqueID();
	m_pWoner = 0;
#ifdef DEBUG
	//have now idea why it occur crash...if make a class like below.
	//class A1:virtual  public NamedTypedObject{	public:		A1(){}	};
	//class A2:virtual public A1	{		public:		A2(){}	};
	//class	A3:public A2	{	public:		A3()		{}		~A3(){}	};
	//A3 l_cEditorMode22;
	m_i64TimeStamp = GetTimeStampID();
	m_ui64PointerVaildMagicID = g_ui64MAGICID;
	DebugResourceInfoAdd(this);
	//if( m_uiUniqueID == 1 || m_uiUniqueID == 27|| m_uiUniqueID == 26 )
	if( m_uiUniqueID == 26 || m_uiUniqueID == 25 )
	{
		int a=0;
	}
#endif
}

//void	NamedTypedObject::UpdateTimeStamp()
//{
//	g_pLatestNamedTypedObject = this;
//#ifdef DEBUG
//	m_i64TimeStamp = GetTimeStampID();
//#endif
//}

std::string	NamedTypedObject::GetCharName()
{
	std::string	l_str = ValueToString(m_sObjectName);
	return l_str;
}

void NamedTypedObject::SetName( const char*e_pString )
{
	this->m_sObjectName = ValueToStringW(e_pString).c_str();
}


void	NamedTypedObject::DumpUnReleaseInfo()
{
#ifdef DEBUG
	::DumpUnReleaseInfo();
#endif
}


NamedTypedObject*	NamedTypedObject::Clone()
{
//	UT::ErrorMsg(this->Type(),L"not implement clone");
	return 0;
}

TiXmlElement*		NamedTypedObject::ToTiXmlElement()
{
	return 0;
}

NamedTypedObject&			NamedTypedObject::operator=(const NamedTypedObject&e_Other)
{
	this->m_pWoner = nullptr;
	m_sObjectName = e_Other.m_sObjectName;
#ifdef DEBUG
	m_i64TimeStamp = 0;
	m_ui64PointerVaildMagicID = e_Other.m_ui64PointerVaildMagicID;
#endif
	return *this;
}


//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::iterator::iterator()
//-----------------------------------------------------------------------------
NameIndexedCollection::iterator::iterator()
{
    m_pCollection = nullptr;    
    m_iCurBucket = 0;
}

//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::iterator::operator*()
// Desc: iterator dereference
//-----------------------------------------------------------------------------
NamedTypedObject* NameIndexedCollection::iterator::operator*()
{
    return *m_iter;
}


//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::iterator::operator=()
// Desc: iterator assignment operator
//-----------------------------------------------------------------------------
NameIndexedCollection::iterator& 
NameIndexedCollection::iterator::operator=( const NameIndexedCollection::iterator& iRHS)
{
    m_pCollection = iRHS.m_pCollection;
    m_iter = iRHS.m_iter;
    m_iCurBucket = iRHS.m_iCurBucket;
    return (*this);
}


//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::iterator::operator==()
// Desc: iterator comparison operator
//-----------------------------------------------------------------------------
bool NameIndexedCollection::iterator::operator==( const NameIndexedCollection::iterator& iRHS)
{
    if ( ( m_pCollection == iRHS.m_pCollection ) &&
         ( m_iCurBucket == iRHS.m_iCurBucket ) && 
         ( m_iter == iRHS.m_iter ) )   
        return TRUE;
    return FALSE;
}


//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::iterator::operator!=()
// Desc: iterator comparison operator
//-----------------------------------------------------------------------------
bool NameIndexedCollection::iterator::operator!=( const NameIndexedCollection::iterator& iRHS)
{
    if ( ( m_pCollection == iRHS.m_pCollection ) &&
         ( m_iCurBucket == iRHS.m_iCurBucket ) && 
         ( m_iter == iRHS.m_iter ) )    
        return FALSE;
    return TRUE;
}


//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::iterator::operator++()
// Desc: iterator increment
//-----------------------------------------------------------------------------
NameIndexedCollection::iterator& 
NameIndexedCollection::iterator::operator++( int )
{    
    assert( m_pCollection );

    // increment the iterator
    m_iter++;    

    // if we are at the end of a bucket, move to the next bucket
    while ( ( m_iter == m_pCollection->s_Lists[ m_iCurBucket ].end() ) &&
            ( m_iCurBucket < DEFAULT_COLLECTION_HASHSIZE - 1 ) )
    {        
        m_iCurBucket++;        
        m_iter = m_pCollection->s_Lists[ m_iCurBucket ].begin();
    }
        
    return (*this);
}


//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::Add()
//-----------------------------------------------------------------------------
void NameIndexedCollection::Add( NamedTypedObject *pObjectToAdd )
{
    assert( pObjectToAdd );
	//int iBucket = pObjectToAdd->GetUniqueID()%DEFAULT_COLLECTION_HASHSIZE;
    //int iBucket = StringID::HashString( pObjectToAdd->GetName() ) % DEFAULT_COLLECTION_HASHSIZE;    
	//int iBucket = StringID::HashString( pObjectToAdd->Type() ) % DEFAULT_COLLECTION_HASHSIZE;    
	size_t iBucket = ((size_t)pObjectToAdd)%DEFAULT_COLLECTION_HASHSIZE;
    // $OPTIMIZE: use a fixed size allocator here
	if( pObjectToAdd->GetUniqueID() == 617 )
	{
		int a=0;
	}
    s_Lists[ iBucket ].push_back( pObjectToAdd );
}


//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::Remove()
//-----------------------------------------------------------------------------
void NameIndexedCollection::Remove( NamedTypedObject *pObjectToRemove )
{
    assert( pObjectToRemove );
	size_t iBucket = ((size_t)pObjectToRemove)%DEFAULT_COLLECTION_HASHSIZE;
	std::vector<NamedTypedObject *>*l_pList = &s_Lists[ iBucket ];
	int	l_iSize = (int)l_pList->size();
	//if here crush check g_pNameIndexedCollection is null and void	NamedTypedObject::DumpUnReleaseInfo() not call before all object is delete
	//uint64	l_ui46ID = pObjectToRemove->GetUniqueID();
	for( int i=l_iSize-1;i>-1;--i )
	{
		if( (*l_pList)[i] == pObjectToRemove )
		{
			s_Lists[ iBucket ].erase( l_pList->begin()+i );
			return;
		}
	}
	NamedTypedObject*l_pp = Find( pObjectToRemove );
	if( l_pp )
	{
		l_pp->GetName();
	}

}

NamedTypedObject* NameIndexedCollection::Find( NamedTypedObject*e_pNamedTypedObject )
{
    std::vector<NamedTypedObject *>::iterator i;
    
    //int iBucket = StringID::HashString( strName ) % DEFAULT_COLLECTION_HASHSIZE;
	size_t iBucket = ((size_t)e_pNamedTypedObject)%DEFAULT_COLLECTION_HASHSIZE;

    for ( i = s_Lists[ iBucket ].begin(); i != s_Lists[ iBucket ].end(); ++i )
    {
        if ( (*i) == e_pNamedTypedObject )
            break;
    }
    
    if ( i == s_Lists[ iBucket ].end() )
        return nullptr;
    
    return (*i);
}
//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::Find()
//-----------------------------------------------------------------------------
//NamedTypedObject* NameIndexedCollection::Find( const wchar_t* strName )
//{
//    std::vector<NamedTypedObject *>::iterator i;
//    
//    int iBucket = StringID::HashString( strName ) % DEFAULT_COLLECTION_HASHSIZE;    
//
//    for ( i = s_Lists[ iBucket ].begin(); i != s_Lists[ iBucket ].end(); ++i )
//    {
//        if ( (*i)->GetName() == strName )
//            break;
//    }
//    
//    if ( i == s_Lists[ iBucket ].end() )
//        return nullptr;
//    
//    return (*i);
//}


//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::FindTyped()
//-----------------------------------------------------------------------------
NamedTypedObject* NameIndexedCollection::FindTyped( const wchar_t* strName, const StringID TypeID )
{
    std::vector<NamedTypedObject *>::iterator i;

    int iBucket = StringID::HashString( strName ) % DEFAULT_COLLECTION_HASHSIZE;    

    for ( i = s_Lists[ iBucket ].begin(); i != s_Lists[ iBucket ].end(); ++i )
    {
        NamedTypedObject* pNTO = (*i);
        if ( pNTO->IsDerivedFrom( TypeID ) && pNTO->GetName() == strName )
            break;
    }

    if ( i == s_Lists[ iBucket ].end() )
        return nullptr;

    return (*i);
}


unsigned long NameIndexedCollection::Size()
{
    unsigned long dwSize = 0;
    for( unsigned long i = 0; i < DEFAULT_COLLECTION_HASHSIZE; i++ )
    {
        dwSize += (unsigned long)s_Lists[i].size();
    }
    return dwSize;
}


//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::begin()
//-----------------------------------------------------------------------------
NameIndexedCollection::iterator NameIndexedCollection::begin()
{
    NameIndexedCollection::iterator rtn;

    rtn.m_pCollection = this;
    rtn.m_iCurBucket = 0;
    rtn.m_iter = s_Lists[ rtn.m_iCurBucket ].begin();
    
    // if we are at the end of a bucket, move to the next bucket
    while ( ( rtn.m_iter == s_Lists[ rtn.m_iCurBucket ].end() ) &&
            ( rtn.m_iCurBucket != DEFAULT_COLLECTION_HASHSIZE - 1 ) )
    {        
        rtn.m_iCurBucket++;
        rtn.m_iter = s_Lists[ rtn.m_iCurBucket ].begin();
    }
        
    return rtn;
}


//----------------------------------------------------------------------------
// Name: NameIndexedCollection::end()
//-----------------------------------------------------------------------------
NameIndexedCollection::iterator NameIndexedCollection::end()
{
    NameIndexedCollection::iterator rtn;

    rtn.m_pCollection = this;
    rtn.m_iCurBucket = DEFAULT_COLLECTION_HASHSIZE - 1;
    rtn.m_iter = s_Lists[ DEFAULT_COLLECTION_HASHSIZE - 1].end();    
    
    return rtn;
}
