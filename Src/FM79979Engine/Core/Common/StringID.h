#ifndef _STRING_ID_H_
#define _STRING_ID_H_
//#pragma warning(disable:4127)
#include <list>



//-----------------------------------------------------------------------------
// Name: StringID
// Desc: Memory management for strings- strings will be inserted into a hash
//       table uniquely, and can be referenced by pointer.  If you want to 
//       insert a string case-insensitively, use SetCaseInsensitive
//-----------------------------------------------------------------------------    

// This is the number of lists in the string hashtable - should be prime
const int StringID_HASHSIZE = 61;

class StringID
{
public:
    // Constructors
    StringID();
    StringID( const wchar_t* strString );
    StringID( const StringID& other );
	~StringID();

    // Assignment
    StringID& operator= ( const StringID& RHS ) { m_strString = RHS.m_strString; return *this; }    
    StringID& operator= ( const wchar_t* strRHS ) { m_strString = AddString( strRHS ); return *this; }

    // Comparison
    bool operator== ( const StringID& RHS ) const { return m_strString == RHS.m_strString; }    
    bool operator== ( const wchar_t* strRHS ) const;
    bool IsEmptyString() const { return m_strString == s_EmptyString; }

    // Casting
    operator const wchar_t* () const { return m_strString; }
    const wchar_t* GetSafeString() const { return ( m_strString ? m_strString : L"null" ); }
    
    // Hash lookup function
    static unsigned long        HashString( const wchar_t* strString );
	static std::list<const wchar_t *>* m_spStringLists;
	static void						 DestoryStringTable();
protected:
    static const wchar_t* AddString( const wchar_t* strString );
    static std::list<const wchar_t *>* GetStringTable();

protected:
    const wchar_t*                    m_strString;               
    static const wchar_t*             s_EmptyString;
};

#endif