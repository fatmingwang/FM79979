#ifndef _NAMED_TYPED_OBJECT_H_
#define _NAMED_TYPED_OBJECT_H_
#include "CommonDefine.h"
// C4127: conditional expression is constant
// this shows up when using STL without exception handling

#include <list>
#include <string.h>
//#include "StringID.h"
#include <mutex>
//-----------------------------------------------------------------------------
// Name: DEFINE_TYPE_INFO
// Desc: Creates a type based on the NamedTypedObject class.  Use this in any
//       classes derived from NamedTypedObject to get the IsDerived() functionality.
//-----------------------------------------------------------------------------
//#define Conn(x,y) x##y
//#define ToChar(x) #@x		@ = ''
//#define ToString(x) #x	# = 


#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif
#define CAT(A, B)   A##B
#define WSTRING(A)  CAT(L, #A)

#define CHAR_TO_WCHAR_DEFINE(A)  CAT(L, A)



//fuck only works for windows
#define TYPDE_DEFINE_MARCO(TYPE) const wchar_t*         TYPE::TypeID( WSTRING(TYPE) );


#ifndef DEFINE_TYPE_INFO
#define DEFINE_TYPE_INFO() \
    public: \
	virtual const wchar_t*        Type() const override { return TypeID; }\
	static const  wchar_t*        TypeID;
#endif
	//virtual bool                IsDerivedFrom( const wchar_t* _TypeID ) const { return ( _TypeID == TypeID ); }
	//virtual bool IsDerivedFrom( const wchar_t* e_strTypeID ) const  { if( e_strTypeID == TypeID ) return true;  return __super::IsDerivedFrom( e_strTypeID ); 

#define	CLONE_MYSELF(TYPE)	virtual NamedTypedObject*	Clone()override{ return new TYPE(this); }

//! class name="NamedTypedObject" Desc="Base class for exposed objects"
//!    property name="Name" Desc="Name of the Object" Get=GetName Set=SetName
/**
	A quaternion.
	Used to represent rotations: quaternions have the sole advantage of reduced
	floating-point error when concatenating rotations.
*/
class	TiXmlElement;
class NamedTypedObject
{
	void			SetupInternalData();
// Because NamedTypedObject has no base class we explicitly define the type info here- most classes
// will us the DEFINE_TYPE_INFO macro, with an explicit callout in the .cpp file.
// TypeID is public so you can use NamedTypedObject::TypeID for comparisons without an instance.
protected:
	//not necessary but quit helpful to find out who contain this
	//if not calling cNamedTypedObjectVector::AddObject...AddObjectNeglectExist...
	//the owner might be nullptr
	//but cNamedTypedObjectVector::CopyListPointer and cNamedTypedObjectVector::DumpListPointer will occur multi parents problem...be carfful!
	//if the object is cloned!the parent may not real parent.....it might be a reference.
	GET_SET_DEC(NamedTypedObject*,m_pWoner,GetOwner,SetOwner);
	//name coudl be same but unique won't,but it still has a limit for 2^64
	uint64	m_uiUniqueID;
public:
	NamedTypedObject();
	virtual ~NamedTypedObject();
	virtual bool                IsDerivedFrom( const wchar_t* _TypeID ) const { return ( _TypeID == TypeID ); }
	virtual const wchar_t*      Type() const { return TypeID; }
	static const  wchar_t*      TypeID;

public:
    //const wchar_t*	            GetName() const { return m_sObjectName; };
	const wchar_t*				GetName(){ return m_sObjectName.c_str(); };
	std::string					GetCharName();
	bool						IsSameName(const wchar_t*e_strName){ return wcscmp(e_strName,m_sObjectName.c_str())==0?true:false; }
	bool						IsSameName(NamedTypedObject*e_pNamedTypedObject){ return IsSameName(e_pNamedTypedObject->GetName()); }

	bool						IsSameType( NamedTypedObject*e_pNamedTypedObject ){ if(e_pNamedTypedObject->Type() == this->Type())return true;return false; }
	bool						IsSameType( const wchar_t*e_strTypeID ){ if(e_strTypeID == this->Type())return true;return false; }
	
    void                        SetName( wchar_t*e_pString ){m_sObjectName = e_pString;}
	void                        SetName( std::wstring e_strName ){m_sObjectName = e_strName.c_str();}
	void                        SetName( const wchar_t*e_pString ){	m_sObjectName = e_pString;}
    void                        SetName( const char*e_pString );
	//dump all un release data
	static void					DumpUnReleaseInfo();
	//haha......I am lazy to implement this virtual function now,because too many class have to do...so it only work at cTexture for now!
	//virtual wchar_t*              DescriptionInfo(){return L"I am NamedTypedObject";}
	//while u are assign object is not pointer it's possible get same unique ID
	uint64						GetUniqueID(){ return m_uiUniqueID; }
	//
	//virtual	void				Init(){}
	//virtual	void				Destroy(){}
	//virtual	void				Update(float e_fElpaseTime){}
	//virtual	void				Render(){}
	virtual	NamedTypedObject*	Clone();
	virtual	TiXmlElement*		ToTiXmlElement();
	NamedTypedObject&			operator=(const NamedTypedObject&e_Other);
private:
	std::wstring				m_sObjectName;
#ifdef DEBUG
public:
	uint64						m_i64TimeStamp;
	//please don not chang this value.
	UINT64						m_ui64PointerVaildMagicID;
	bool						IsMagicIDMatch();//if not match pointer might delete at somewhere
#endif
	virtual	void				UpdateTimeStamp(){}
};

const int DEFAULT_COLLECTION_HASHSIZE = 61;
class NameIndexedCollection
{
public:
    // iterator for the collection - it is made so you can 
    // delete the object at the current position and STILL do a ++
    // safely afterwards
    class iterator
    {
    friend class NameIndexedCollection;
    public:
        iterator();      

        NamedTypedObject*		operator*();
        iterator&               operator++( int ); // only postfix defined 
        iterator&               operator=( const iterator& iRHS );
        bool                    operator==( const iterator& iRHS );
        bool                    operator!=( const iterator& iRHS );
    private:   
        NameIndexedCollection*						m_pCollection;
        std::vector<NamedTypedObject*>::iterator	m_iter;
        int                                         m_iCurBucket;
    };
    friend class NameIndexedCollection::iterator;
	NameIndexedCollection();
	~NameIndexedCollection();
    void    Add( NamedTypedObject *pObjectToAdd );        // Add a NamedTypedObject to the collection        
    void    Remove( NamedTypedObject *pObjectToRemove );  // Remove a NamedTypedObject from the collection        
    //NamedTypedObject* Find( const wchar_t* strName );       // Find a NamedTypedObject in the collection
	NamedTypedObject* Find( NamedTypedObject*e_pNamedTypedObject );
    NamedTypedObject* FindTyped( const wchar_t* strName, const wchar_t* TypeID );       // Find a NamedTypedObject of a certain type in the collection
    unsigned long   Size();
       
    iterator        begin();
    iterator        end();
private:
    std::vector<NamedTypedObject *> s_Lists[ DEFAULT_COLLECTION_HASHSIZE ];     
	std::mutex m_Mutex;
};

uint64	GetTimeStampID();
uint64	GetGlobalUniqueID();

#endif // NamedTypedObject_H