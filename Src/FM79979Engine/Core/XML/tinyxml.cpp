#include "../stdafx.h"
#include "../CommonDefine.h"
#include "../CommonDefine.h"
#include "../GameplayUT/StringCompress.h"
#include "StringToStructure.h"
#include "../GameplayUT/GameApp.h"
#include "../Utility.h"
using namespace FATMING_CORE;
/*
www.sourceforge.net/projects/tinyxml
Original code (2.0 and earlier )copyright (c) 2000-2006 Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include <ctype.h>

#ifdef TIXML_USE_STL
#include <sstream>
#include <iostream>
#endif

#include "tinyxml.h"
#include "../Utility.h"

bool TiXmlBase::condenseWhiteSpace = true;
// Microsoft compiler security
NvFile* TiXmlFOpen( const char* filename, const char* mode )
{
	return MyFileOpen( filename, mode );
}

void TiXmlBase::EncodeString( const TIXML_STRING& str, TIXML_STRING* outString )
{
	int i=0;

	while( i<(int)str.length() )
	{
		wchar_t c = (wchar_t) str[i];

		if (    c == L'&'
		     && i < ( (int)str.length() - 2 )
			 && str[i+1] == L'#'
			 && str[i+2] == L'x' )
		{
			// Hexadecimal WCHARacter reference.
			// Pass through unchanged.
			// &#xA9;	-- copyright symbol, for example.
			//
			// The -1 is a bug fix from Rob Laveaux. It keeps
			// an overflow from happening if there is no ';'.
			// There are actually 2 ways to exit this loop -
			// while fails (error case) and break (semicolon found).
			// However, there is no mechanism (currently) for
			// this function to return an error.
			while ( i<(int)str.length()-1 )
			{
				outString->append( str.c_str() + i, 1 );
				++i;
				if ( str[i] == L';' )
					break;
			}
		}
		else if ( c == L'&' )
		{
			outString->append( entity[0].str, entity[0].strLength);
			++i;
		}
		else if ( c == L'<' )
		{
			outString->append( entity[1].str, entity[1].strLength);
			++i;
		}
		else if ( c == L'>' )
		{
			outString->append( entity[2].str, entity[2].strLength);
			++i;
		}
		else if ( c == L'\"' )
		{
			outString->append( entity[3].str, entity[3].strLength);
			++i;
		}
		else if ( c == L'\'' )
		{
			outString->append( entity[4].str, entity[4].strLength);
			++i;
		}
		else if ( c < 32 )
		{
			// Easy pass at non-alpha/numeric/symbol
			// Below 32 is symbolic.
			wchar_t buf[ 32 ];
			char l_cbuf[ 32 ];
			sprintf( l_cbuf, "&#x%02X;", (unsigned) ( c & 0xff ) );
			int	l_iLength = strlen(l_cbuf);
			for( int ii=0;ii<l_iLength;++ii )
			{
				buf[ii] = l_cbuf[ii];
			}
			buf[l_iLength] = L'\0'; 
			//*ME:	warning C4267: convert 'size_t' to 'int'
			//*ME:	Int-Cast to make compiler happy ...
			outString->append( buf, l_iLength );
			++i;
		}
		else
		{
			//wchar_t realc = (wchar_t) c;
			//outString->append( &realc, 1 );
			*outString += (wchar_t) c;	// somewhat more efficient function call.
			++i;
		}
	}
}


void	DumpInfo(TiXmlElement*e_pTiXmlElement)
{
#ifdef WIN32
	for(TiXmlNode*l_pFirstNode = e_pTiXmlElement;l_pFirstNode;l_pFirstNode = l_pFirstNode->Parent())
	{
		cGameApp::OutputDebugInfoString(L"\t");
	}
	cGameApp::OutputDebugInfoString(e_pTiXmlElement->Value());
	cGameApp::OutputDebugInfoString(L"__");
	for( TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();l_pTiXmlAttribute;l_pTiXmlAttribute = l_pTiXmlAttribute->Next() )
	{
		cGameApp::OutputDebugInfoString(l_pTiXmlAttribute->Name());
		cGameApp::OutputDebugInfoString(L":");
		cGameApp::OutputDebugInfoString(l_pTiXmlAttribute->Value());
		cGameApp::OutputDebugInfoString(L",");
	}
	for(TiXmlElement*l_pFirstNode = e_pTiXmlElement->FirstChildElement();l_pFirstNode;l_pFirstNode = l_pFirstNode->NextSiblingElement())
	{
		cGameApp::OutputDebugInfoString(L"\n");
		DumpInfo(l_pFirstNode);
	}
#endif
}

TiXmlNode::TiXmlNode( NodeType _type ) : TiXmlBase()
{
	parent = 0;
	type = _type;
	firstChild = 0;
	lastChild = 0;
	prev = 0;
	next = 0;
}


TiXmlNode::~TiXmlNode()
{
	TiXmlNode* node = firstChild;
	TiXmlNode* temp = 0;

	while ( node )
	{
		temp = node;
		node = node->next;
		delete temp;
	}
}

int	TiXmlNode::ChildElementCount()
{
	int	l_iCount = 0;
	TiXmlElement*l_pChildElement = this->FirstChildElement();
	while(l_pChildElement)
	{
		l_pChildElement = l_pChildElement->NextSiblingElement();
		++l_iCount;
	}
	return l_iCount;
}

void TiXmlNode::CopyTo( TiXmlNode* target ) const
{
	target->SetValue (value.c_str() );
	target->userData = userData;
}


void TiXmlNode::Clear()
{
	TiXmlNode* node = firstChild;
	TiXmlNode* temp = 0;

	while ( node )
	{
		temp = node;
		node = node->next;
		delete temp;
	}

	firstChild = 0;
	lastChild = 0;
}


TiXmlNode* TiXmlNode::LinkEndChild( TiXmlNode* node )
{
	assert( node->parent == 0 || node->parent == this );
	assert( node->GetDocument() == 0 || node->GetDocument() == this->GetDocument() );

	if ( node->Type() == TiXmlNode::DOCUMENT )
	{
		delete node;
		if ( GetDocument() ) GetDocument()->SetError( TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0 );
		return 0;
	}

	node->parent = this;

	node->prev = lastChild;
	node->next = 0;

	if ( lastChild )
		lastChild->next = node;
	else
		firstChild = node;			// it was an empty list.

	lastChild = node;
	return node;
}

TiXmlNode* TiXmlNode::LinkFirstChild( TiXmlNode* node )
{
	assert( node->parent == 0 || node->parent == this || node->prev == 0 );

	assert( node->GetDocument() == 0 || node->GetDocument() == this->GetDocument() );

	if ( node->Type() == TiXmlNode::DOCUMENT )
	{
		delete node;
		if ( GetDocument() ) GetDocument()->SetError( TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0 );
		return 0;
	}

	node->parent = this;

	node->prev = 0;
	node->next = this->firstChild;

	if ( firstChild )
		firstChild->prev = node;
	else
		lastChild = node;			// it was an empty list.

	firstChild = node;
	return node;
}


TiXmlNode* TiXmlNode::InsertEndChild( const TiXmlNode& addThis )
{
	if ( addThis.Type() == TiXmlNode::DOCUMENT )
	{
		if ( GetDocument() ) GetDocument()->SetError( TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0 );
		return 0;
	}
	TiXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;

	return LinkEndChild( node );
}


TiXmlNode* TiXmlNode::InsertBeforeChild( TiXmlNode* beforeThis, const TiXmlNode& addThis )
{
	if ( !beforeThis || beforeThis->parent != this ) {
		return 0;
	}
	if ( addThis.Type() == TiXmlNode::DOCUMENT )
	{
		if ( GetDocument() ) GetDocument()->SetError( TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0 );
		return 0;
	}

	TiXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;
	node->parent = this;

	node->next = beforeThis;
	node->prev = beforeThis->prev;
	if ( beforeThis->prev )
	{
		beforeThis->prev->next = node;
	}
	else
	{
		assert( firstChild == beforeThis );
		firstChild = node;
	}
	beforeThis->prev = node;
	return node;
}


TiXmlNode* TiXmlNode::InsertAfterChild( TiXmlNode* afterThis, const TiXmlNode& addThis )
{
	if ( !afterThis || afterThis->parent != this ) {
		return 0;
	}
	if ( addThis.Type() == TiXmlNode::DOCUMENT )
	{
		if ( GetDocument() ) GetDocument()->SetError( TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0 );
		return 0;
	}

	TiXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;
	node->parent = this;

	node->prev = afterThis;
	node->next = afterThis->next;
	if ( afterThis->next )
	{
		afterThis->next->prev = node;
	}
	else
	{
		assert( lastChild == afterThis );
		lastChild = node;
	}
	afterThis->next = node;
	return node;
}


TiXmlNode* TiXmlNode::ReplaceChild( TiXmlNode* replaceThis, const TiXmlNode& withThis )
{
	if ( replaceThis->parent != this )
		return 0;

	TiXmlNode* node = withThis.Clone();
	if ( !node )
		return 0;

	node->next = replaceThis->next;
	node->prev = replaceThis->prev;

	if ( replaceThis->next )
		replaceThis->next->prev = node;
	else
		lastChild = node;

	if ( replaceThis->prev )
		replaceThis->prev->next = node;
	else
		firstChild = node;

	delete replaceThis;
	node->parent = this;
	return node;
}


bool TiXmlNode::RemoveChild( TiXmlNode* removeThis )
{
	if ( removeThis->parent != this )
	{
		assert( 0 );
		return false;
	}

	if ( removeThis->next )
		removeThis->next->prev = removeThis->prev;
	else
		lastChild = removeThis->prev;

	if ( removeThis->prev )
		removeThis->prev->next = removeThis->next;
	else
		firstChild = removeThis->next;

	delete removeThis;
	return true;
}

const TiXmlNode* TiXmlNode::FirstChild( const wchar_t* _value ) const
{
	const TiXmlNode* node;
	for ( node = firstChild; node; node = node->next )
	{
		if ( wcscmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


const TiXmlNode* TiXmlNode::LastChild( const wchar_t* _value ) const
{
	const TiXmlNode* node;
	for ( node = lastChild; node; node = node->prev )
	{
		if ( wcscmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


const TiXmlNode* TiXmlNode::IterateChildren( const TiXmlNode* previous ) const
{
	if ( !previous )
	{
		return FirstChild();
	}
	else
	{
		assert( previous->parent == this );
		return previous->NextSibling();
	}
}


const TiXmlNode* TiXmlNode::IterateChildren( const wchar_t* val, const TiXmlNode* previous ) const
{
	if ( !previous )
	{
		return FirstChild( val );
	}
	else
	{
		assert( previous->parent == this );
		return previous->NextSibling( val );
	}
}


const TiXmlNode* TiXmlNode::NextSibling( const wchar_t* _value ) const
{
	const TiXmlNode* node;
	for ( node = next; node; node = node->next )
	{
		if ( wcscmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


const TiXmlNode* TiXmlNode::PreviousSibling( const wchar_t* _value ) const
{
	const TiXmlNode* node;
	for ( node = prev; node; node = node->prev )
	{
		if ( wcscmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


void TiXmlElement::RemoveAttribute( const wchar_t* name )
{
    #ifdef TIXML_USE_STL
	TIXML_STRING str( name );
	TiXmlAttribute* node = attributeSet.Find( str );
	#else
	TiXmlAttribute* node = attributeSet.Find( name );
	#endif
	if ( node )
	{
		attributeSet.Remove( node );
		delete node;
	}
}

const TiXmlElement* TiXmlNode::FirstChildElement() const
{
	const TiXmlNode* node;

	for (	node = FirstChild();
			node;
			node = node->NextSibling() )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const TiXmlElement* TiXmlNode::FirstChildElement( const wchar_t* _value ) const
{
	const TiXmlNode* node;

	for (	node = FirstChild( _value );
			node;
			node = node->NextSibling( _value ) )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const TiXmlElement* TiXmlNode::NextSiblingElement() const
{
	const TiXmlNode* node;

	for (	node = NextSibling();
			node;
			node = node->NextSibling() )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const TiXmlElement* TiXmlNode::NextSiblingElement( const wchar_t* _value ) const
{
	const TiXmlNode* node;

	for (	node = NextSibling( _value );
			node;
			node = node->NextSibling( _value ) )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const TiXmlDocument* TiXmlNode::GetDocument() const
{
	const TiXmlNode* node;

	for( node = this; node; node = node->parent )
	{
		if ( node->ToDocument() )
			return node->ToDocument();
	}
	return 0;
}


TiXmlElement::TiXmlElement (const wchar_t* _value)
	: TiXmlNode( TiXmlNode::ELEMENT )
{
	m_bDone = false;
	firstChild = lastChild = 0;
	value = _value;
}


#ifdef TIXML_USE_STL
TiXmlElement::TiXmlElement( const std::wstring& _value )
	: TiXmlNode( TiXmlNode::ELEMENT )
{
	firstChild = lastChild = 0;
	value = _value;
}
#endif


TiXmlElement::TiXmlElement( const TiXmlElement& copy)
	: TiXmlNode( TiXmlNode::ELEMENT )
{
	m_bDone = false;
	firstChild = lastChild = 0;
	copy.CopyTo( this );
}


void TiXmlElement::operator=( const TiXmlElement& base )
{
	ClearThis();
	base.CopyTo( this );
}


TiXmlElement::~TiXmlElement()
{
	ClearThis();
}


void TiXmlElement::ClearThis()
{
	Clear();
	while( attributeSet.First() )
	{
		TiXmlAttribute* node = attributeSet.First();
		attributeSet.Remove( node );
		delete node;
	}
}


const wchar_t* TiXmlElement::Attribute( const wchar_t* name ) const
{
	const TiXmlAttribute* node = attributeSet.Find( name );
	if ( node )
		return node->Value();
	return 0;
}


#ifdef TIXML_USE_STL
const std::wstring* TiXmlElement::Attribute( const std::wstring& name ) const
{
	const TiXmlAttribute* node = attributeSet.Find( name );
	if ( node )
		return &node->ValueStr();
	return 0;
}
#endif


const wchar_t* TiXmlElement::Attribute( const wchar_t* name, int* i ) const
{
	const wchar_t* s = Attribute( name );
	if ( i )
	{
		if ( s ) {
			*i = _wtoi( s );
		}
		else {
			*i = 0;
		}
	}
	return s;
}


#ifdef TIXML_USE_STL
const std::wstring* TiXmlElement::Attribute( const std::wstring& name, int* i ) const
{
	const std::wstring* s = Attribute( name );
	if ( i )
	{
		if ( s ) {
			*i = _wtoi( s->c_str() );
		}
		else {
			*i = 0;
		}
	}
	return s;
}
#endif


const wchar_t* TiXmlElement::Attribute( const wchar_t* name, double* d ) const
{
	const wchar_t* s = Attribute( name );
	if ( d )
	{
		if ( s ) {
			*d = _wtof( s );
		}
		else {
			*d = 0;
		}
	}
	return s;
}


#ifdef TIXML_USE_STL
const std::wstring* TiXmlElement::Attribute( const std::wstring& name, double* d ) const
{
	const std::wstring* s = Attribute( name );
	if ( d )
	{
		if ( s ) {
			*d = _wtof( s->c_str() );
		}
		else {
			*d = 0;
		}
	}
	return s;
}
#endif


int TiXmlElement::QueryIntAttribute( const wchar_t* name, int* ival ) const
{
	const TiXmlAttribute* node = attributeSet.Find( name );
	if ( !node )
		return TIXML_NO_ATTRIBUTE;
	return node->QueryIntValue( ival );
}


#ifdef TIXML_USE_STL
int TiXmlElement::QueryIntAttribute( const std::wstring& name, int* ival ) const
{
	const TiXmlAttribute* node = attributeSet.Find( name );
	if ( !node )
		return TIXML_NO_ATTRIBUTE;
	return node->QueryIntValue( ival );
}
#endif


int TiXmlElement::QueryDoubleAttribute( const wchar_t* name, double* dval ) const
{
	const TiXmlAttribute* node = attributeSet.Find( name );
	if ( !node )
		return TIXML_NO_ATTRIBUTE;
	return node->QueryDoubleValue( dval );
}


#ifdef TIXML_USE_STL
int TiXmlElement::QueryDoubleAttribute( const std::wstring& name, double* dval ) const
{
	const TiXmlAttribute* node = attributeSet.Find( name );
	if ( !node )
		return TIXML_NO_ATTRIBUTE;
	return node->QueryDoubleValue( dval );
}
#endif


void TiXmlElement::SetAttribute( const wchar_t* name, int val )
{
	SetAttribute( name, ValueToStringW(val) );
}

void TiXmlElement::SetAttribute( const wchar_t* name, float value )
{
	SetAttribute( name, ValueToStringW(value) );
}


#ifdef TIXML_USE_STL
void TiXmlElement::SetAttribute( const std::wstring& name, int val )
{
   //std::ostringstream oss;
   //oss << val;
   SetAttribute( name, ValueToStringW(val) );
}
#endif


void TiXmlElement::SetDoubleAttribute( const wchar_t* name, double val )
{
	SetAttribute( name, ValueToStringW(val) );
}


void TiXmlElement::SetAttribute( const wchar_t* e_strName, std::wstring e_strValue )
{
	SetAttribute( e_strName, e_strValue.c_str() );
}
void TiXmlElement::SetAttribute( std::wstring e_strName, const wchar_t* e_strValue )
{
	SetAttribute( e_strName.c_str(), e_strValue );
}
void TiXmlElement::SetAttribute( std::wstring e_strName, std::wstring e_strValue )
{
	SetAttribute( e_strName.c_str(), e_strValue.c_str() );
}

void TiXmlElement::SetAttribute( const wchar_t* cname, const wchar_t* cvalue )
{
    #ifdef TIXML_USE_STL
	TIXML_STRING _name( cname );
	TIXML_STRING _value( cvalue );
	#else
	const wchar_t* _name = cname;
	const wchar_t* _value = cvalue;
	#endif

	TiXmlAttribute* node = attributeSet.Find( _name );
	if ( node )
	{
		node->SetValue( _value );
		return;
	}

	TiXmlAttribute* attrib = new TiXmlAttribute( cname, cvalue );
	if ( attrib )
	{
		attributeSet.Add( attrib );
	}
	else
	{
		TiXmlDocument* document = GetDocument();
		if ( document ) document->SetError( TIXML_ERROR_OUT_OF_MEMORY, 0, 0 );
	}
}


#ifdef TIXML_USE_STL
void TiXmlElement::SetAttribute( const std::wstring& name, const std::wstring& _value )
{
	TiXmlAttribute* node = attributeSet.Find( name );
	if ( node )
	{
		node->SetValue( _value );
		return;
	}

	TiXmlAttribute* attrib = new TiXmlAttribute( name, _value );
	if ( attrib )
	{
		attributeSet.Add( attrib );
	}
	else
	{
		TiXmlDocument* document = GetDocument();
		if ( document ) document->SetError( TIXML_ERROR_OUT_OF_MEMORY, 0, 0 );
	}
}
#endif

#ifdef ANDROID

void TiXmlUnknown::Print( FILE* cfile, int depth ) const
{
	for ( int i=0; i<depth; i++ )
	{
		fprintf( cfile, "    " );
	}
	fprintf( cfile, "<%s>", UT::WcharToChar(value.c_str()).c_str() );
}


void TiXmlDeclaration::Print( FILE* cfile, int /*depth*/, TIXML_STRING* str ) const
{
	if ( cfile ) fprintf( cfile, "?xml " );
	if ( str )	 (*str) += L"<?xml ";

	if ( !version.empty() )
	{
		if ( cfile )
			fprintf (cfile, "version=\"%s\" ", UT::WcharToChar(version.c_str ()).c_str());
		if ( str )
		{
		    (*str) += L"version=\"";
		    (*str) += version;
		    (*str) += L"\" "; 
		}
	}
	if ( !encoding.empty() )
	{
		if ( cfile )
			fprintf (cfile, "encoding=\"%s\" ", UT::WcharToChar(encoding.c_str ()).c_str());
		if ( str )
		{
		    (*str) += L"encoding=\"";
		    (*str) += encoding; (*str) += L"\" ";
		 }
	}
	if ( !standalone.empty() )
	{
		if ( cfile ) 
			fprintf (cfile, "standalone=\"%s\" ", UT::WcharToChar(standalone.c_str ()).c_str());
		if ( str )
		{
		    (*str) += L"standalone=\"";
		    (*str) += standalone;
		    (*str) += L"\" ";
		}
	}
	if ( cfile )
	    fprintf( cfile, "?>" );
	if ( str )
	    (*str) += L"?>";
}


void TiXmlText::Print( FILE* cfile, int depth ) const
{
	assert( cfile );
	if ( cdata )
	{
		int i;
		fprintf( cfile, "\n" );
		for ( i=0; i<depth; i++ ) 
		{
			fprintf( cfile, "    " );
		}
		fprintf( cfile, "<![CDATA[%s]]>\n", UT::WcharToChar(value.c_str()).c_str());	// unformatted output
	}
	else
	{
		TIXML_STRING buffer;
		EncodeString( value, &buffer );
		fprintf( cfile, "%s", UT::WcharToChar(buffer.c_str()).c_str() );
	}
}

void TiXmlComment::Print( FILE* cfile, int depth ) const
{
	assert( cfile );
	for ( int i=0; i<depth; i++ )
	{
		fprintf( cfile,  "    " );
	}
	fprintf( cfile, "<!--%s-->", UT::WcharToChar(value.c_str()).c_str());
}


void TiXmlAttribute::Print( FILE* cfile, int /*depth*/, TIXML_STRING* str ) const
{
	TIXML_STRING n, v;

	EncodeString( name, &n );
	EncodeString( value, &v );

	if (value.find ('\"') == TIXML_STRING::npos) 
	{
		if ( cfile ) 
		{
			fprintf (cfile, "%s=\"%s\"",n.c_str() ,v.c_str());
		}
		if ( str ) 
		{
			(*str) += n; (*str) += L"=\""; (*str) += v; (*str) += L"\"";
		}
	}
	else {
		if ( cfile ) 
		{
			fprintf (cfile, "%s='%s'", n.c_str() ,v.c_str());
		}
		if ( str ) 
		{
			(*str) += n; (*str) += L"='"; (*str) += v; (*str) += L"'";
		}
	}
}

void TiXmlDocument::Print( FILE* cfile, int depth ) const
{
	assert( cfile );
	for ( const TiXmlNode* node=FirstChild(); node; node=node->NextSibling() )
	{
		node->Print( cfile, depth );
		fprintf( cfile, "\n" );
	}
}

void TiXmlElement::Print( FILE* cfile, int depth ) const
{
	int i;
	assert( cfile );
	for ( i=0; i<depth; i++ ) 
	{
		fprintf( cfile, "    " );
	}

	fprintf( cfile, "<%s", UT::WcharToChar(value.c_str()).c_str());

	const TiXmlAttribute* attrib;
	for ( attrib = attributeSet.First(); attrib; attrib = attrib->Next() )
	{
		fprintf( cfile, " " );
		attrib->Print( cfile, depth );
	}

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	TiXmlNode* node;
	if ( !firstChild )
	{
		fprintf( cfile, " />" );
	}
	else if ( firstChild == lastChild && firstChild->ToText() )
	{
		fprintf( cfile, ">" );
		firstChild->Print( cfile, depth + 1 );
		fprintf( cfile, "</%s>", UT::WcharToChar(value.c_str()).c_str());
	}
	else
	{
		fprintf( cfile, ">" );

		for ( node = firstChild; node; node=node->NextSibling() )
		{
			if ( !node->ToText() )
			{
				fprintf( cfile, "\n" );
			}
			node->Print( cfile, depth+1 );
		}
		fprintf( cfile, "\n" );
		for( i=0; i<depth; ++i ) 
		{
			fprintf( cfile,"    " );
		}
		fprintf( cfile, "</%s>", UT::WcharToChar(value.c_str()).c_str());
	}
}
#else

void TiXmlUnknown::Print( FILE* cfile, int depth ) const
{
	for ( int i=0; i<depth; i++ )
		fwprintf( cfile, L"    " );
	fwprintf( cfile, L"<%s>", value.c_str() );
}


void TiXmlDeclaration::Print( FILE* cfile, int /*depth*/, TIXML_STRING* str ) const
{
	if ( cfile ) fwprintf( cfile, L"<?xml " );
	if ( str )	 (*str) += L"<?xml ";

	if ( !version.empty() )
	{
		if ( cfile )
		    fwprintf (cfile, L"version=\"%s\" ", version.c_str ());
		if ( str )
		{
		    (*str) += L"version=\"";
		    (*str) += version;
		    (*str) += L"\" "; 
		}
	}
	if ( !encoding.empty() )
	{
		if ( cfile )
		    fwprintf (cfile, L"encoding=\"%s\" ", encoding.c_str ());
		if ( str )
		{
		    (*str) += L"encoding=\"";
		    (*str) += encoding; (*str) += L"\" ";
		 }
	}
	if ( !standalone.empty() )
	{
		if ( cfile ) 
		    fwprintf (cfile, L"standalone=\"%s\" ", standalone.c_str ());
		if ( str )
		{
		    (*str) += L"standalone=\"";
		    (*str) += standalone;
		    (*str) += L"\" ";
		}
	}
	if ( cfile )
	    fwprintf( cfile, L"?>" );
	if ( str )
	    (*str) += L"?>";
}


void TiXmlText::Print( FILE* cfile, int depth ) const
{
	assert( cfile );
	if ( cdata )
	{
		int i;
		fwprintf( cfile, L"\n" );
		for ( i=0; i<depth; i++ ) {
			fwprintf( cfile, L"    " );
		}
		fwprintf( cfile, L"<![CDATA[%s]]>\n", value.c_str() );	// unformatted output
	}
	else
	{
		TIXML_STRING buffer;
		EncodeString( value, &buffer );
		fwprintf( cfile, L"%s", buffer.c_str() );
	}
}

void TiXmlComment::Print( FILE* cfile, int depth ) const
{
	assert( cfile );
	for ( int i=0; i<depth; i++ )
	{
		fwprintf( cfile,  L"    " );
	}
	fwprintf( cfile, L"<!--%s-->", value.c_str() );
}


void TiXmlAttribute::Print( FILE* cfile, int /*depth*/, TIXML_STRING* str ) const
{
	TIXML_STRING n, v;

	EncodeString( name, &n );
	EncodeString( value, &v );

	if (value.find ('\"') == TIXML_STRING::npos) {
		if ( cfile ) {
		fwprintf (cfile, L"%s=\"%s\"", n.c_str(), v.c_str() );
		}
		if ( str ) {
			(*str) += n; (*str) += L"=\""; (*str) += v; (*str) += L"\"";
		}
	}
	else {
		if ( cfile ) {
		fwprintf (cfile, L"%s='%s'", n.c_str(), v.c_str() );
		}
		if ( str ) {
			(*str) += n; (*str) += L"='"; (*str) += v; (*str) += L"'";
		}
	}
}

void TiXmlDocument::Print( FILE* cfile, int depth ) const
{
	assert( cfile );
	for ( const TiXmlNode* node=FirstChild(); node; node=node->NextSibling() )
	{
		node->Print( cfile, depth );
		fwprintf( cfile, L"\n" );
	}
}

void TiXmlElement::Print( FILE* cfile, int depth ) const
{
	int i;
	assert( cfile );
	for ( i=0; i<depth; i++ ) 
	{
		fwprintf( cfile, L"    " );
	}

	fwprintf( cfile, L"<%s", value.c_str() );

	const TiXmlAttribute* attrib;
	for ( attrib = attributeSet.First(); attrib; attrib = attrib->Next() )
	{
		fwprintf( cfile, L" " );
		attrib->Print( cfile, depth );
	}

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	TiXmlNode* node;
	if ( !firstChild )
	{
		fwprintf( cfile, L" />" );
	}
	else if ( firstChild == lastChild && firstChild->ToText() )
	{
		fwprintf( cfile, L">" );
		firstChild->Print( cfile, depth + 1 );
		fwprintf( cfile, L"</%s>", value.c_str() );
	}
	else
	{
		fwprintf( cfile, L">" );

		for ( node = firstChild; node; node=node->NextSibling() )
		{
			if ( !node->ToText() )
			{
				fwprintf( cfile, L"\n" );
			}
			node->Print( cfile, depth+1 );
		}
		fwprintf( cfile, L"\n" );
		for( i=0; i<depth; ++i ) {
			fwprintf( cfile, L"    " );
		}
		fwprintf( cfile, L"</%s>", value.c_str() );
	}
}
#endif

void TiXmlElement::CopyTo( TiXmlElement* target ) const
{
	// superclass:
	TiXmlNode::CopyTo( target );

	// Element class:
	// Clone the attributes, then clone the children.
	const TiXmlAttribute* attribute = 0;
	for(	attribute = attributeSet.First();
	attribute;
	attribute = attribute->Next() )
	{
		target->SetAttribute( attribute->Name(), attribute->Value() );
	}

	TiXmlNode* node = 0;
	for ( node = firstChild; node; node = node->NextSibling() )
	{
		target->LinkEndChild( node->Clone() );
	}
}

bool TiXmlElement::Accept( TiXmlVisitor* visitor ) const
{
	if ( visitor->VisitEnter( *this, attributeSet.First() ) )
	{
		for ( const TiXmlNode* node=FirstChild(); node; node=node->NextSibling() )
		{
			if ( !node->Accept( visitor ) )
				break;
		}
	}
	return visitor->VisitExit( *this );
}


TiXmlNode* TiXmlElement::Clone() const
{
	TiXmlElement* clone = new TiXmlElement( Value() );
	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


const wchar_t* TiXmlElement::GetText() const
{
	const TiXmlNode* child = this->FirstChild();
	if ( child ) {
		const TiXmlText* childText = child->ToText();
		if ( childText ) {
			return childText->Value();
		}
	}
	return 0;
}


TiXmlDocument::TiXmlDocument() : TiXmlNode( TiXmlNode::DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
	ClearError();
}

TiXmlDocument::TiXmlDocument( const wchar_t* documentName ) : TiXmlNode( TiXmlNode::DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
	value = documentName;
	ClearError();
}


#ifdef TIXML_USE_STL
TiXmlDocument::TiXmlDocument( const std::wstring& documentName ) : TiXmlNode( TiXmlNode::DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
    value = documentName;
	ClearError();
}
#endif


TiXmlDocument::TiXmlDocument( const TiXmlDocument& copy ) : TiXmlNode( TiXmlNode::DOCUMENT )
{
	copy.CopyTo( this );
}


void TiXmlDocument::operator=( const TiXmlDocument& copy )
{
	Clear();
	copy.CopyTo( this );
}


bool TiXmlDocument::LoadFile(  )
{
	// See STL_STRING_BUG below.
	//StringToBuffer buf( value );
	std::string	l_strFileName = UT::WcharToChar(Value());
	return LoadFile( l_strFileName.c_str());
}

bool TiXmlDocument::SaveFile() const
{
	// See STL_STRING_BUG below.
//	StringToBuffer buf( value );
//
//	if ( buf.buffer && SaveFile( buf.buffer ) )
//		return true;
//
//	return false;
    char    l_strFileName[MAX_PATH];
    WCHAR_TO_CHAR(Value(),l_strFileName);
    return SaveFile( l_strFileName );
	//return SaveFile( Value() );
}


bool TiXmlDocument::LoadFile( const char* _filename)
{
	// There was a really terrifying little bug here. The code:
	//		value = filename
	// in the STL case, cause the assignment method of the std::wstring to
	// be called. What is strange, is that the std::wstring had the same
	// address as it's c_str() method, and so bad things happen. Looks
	// like a bug in the Microsoft STL implementation.
	// Add an extra string to avoid the crash.
	value = UT::CharToWchar(_filename);
	NvFile* file = TiXmlFOpen( _filename, "rb" );

	if ( file )
	{
		bool result = LoadFile( file);
		NvFClose( file );
		return result;
	}
	else
	{
		SetError( TIXML_ERROR_OPENING_FILE, 0, 0 );
		return false;
	}
}

bool TiXmlDocument::LoadFile( char*e_strData,int e_iLength)
{
	if( e_iLength == 0 )
		return false;
	bool	l_bBinary = false;
	//binary file eheck
	int	l_iMagicID = *(int*)e_strData;
	int	l_iLength = e_iLength;
	if( l_iMagicID == HUFFMAN_MAGIC_ID )
	{
		if( !cGameApp::m_sbAllowParseBinaryFile )
		{
			return false;
		}
		l_iLength -= sizeof(int);
		e_strData += sizeof(int);
		l_bBinary = true;
	}
	// If we have a file, assume it is all one big XML file, and read it in.
	// The document parser may decide the document ends sooner than the entire file, however.
	//std::wstring data;
	//data.reserve( length );
    //int l_iSize = sizeof(wchar_t);
	wchar_t* buf = new wchar_t[ l_iLength+1 ];//assume all file is unicode
	memcpy(buf,e_strData,l_iLength);
	buf[l_iLength] = L'\0';
	if( l_bBinary )
	{
		unsigned char *pDes;
		int nDesLen;
		DecompressHuffman((unsigned char*)buf,l_iLength, pDes, nDesLen);
		delete buf;
		buf = (wchar_t*)pDes;
		l_iLength = nDesLen;
	}
	unsigned char*l_strEncoding = (unsigned char*)buf;
#if defined(ANDROID)
	if( buf[0] != 0xfeff )//this is ansii file,convert ansii file to unicode
#else
	if( l_strEncoding[0] != 0xff && l_strEncoding[1] != 0xfe )//this is ansii file,convert ansii file to unicode
#endif
	{
		char*l_strAnsii = (char*)buf;
		std::wstring l_strCharToWchar = UT::CharToWchar(l_strAnsii);
		int	l_iNewBufferSize = l_strCharToWchar.length();
		wchar_t* pNewbuf = new wchar_t[ l_iLength+1 ];
		memcpy(pNewbuf,&l_strCharToWchar[0],sizeof(wchar_t)*l_iLength);
		pNewbuf[l_iLength] = L'\0';
		//for( int i=0;i<l_iLength;++i )
		//{
		//	pNewbuf[i] = l_strAnsii[i];
		//}
		delete [] buf;
		//pNewbuf[l_iLength] = L'\0';
		buf = pNewbuf;
	}
	else
	if( buf[1] == 0 )
	{
		//it's utf 32...works for IOS and Android but windows,skip 8 byte to keept format correct
		if( l_iLength%2 == 0 && (l_strEncoding[0] == 0xff && l_strEncoding[1] == 0xfe) )
		{
			wchar_t* l_pNewBuffer = new wchar_t[ l_iLength/2+1 ];//assume all file is unicode
			int	l_iIndex = 0;
			for( int i=0;i<l_iLength;i+=2 )
			{
				l_pNewBuffer[l_iIndex] = buf[i];
				++l_iIndex;
			}
			delete [] buf;
			buf = 0;
			buf = l_pNewBuffer;
		}
		else
		{
			SystemErrorCheck();
			delete [] buf;
			buf = 0;
			SystemErrorCheck();
			UT::ErrorMsg(L"this is utf32 xml file,only support utf16 or ansii",(wchar_t*)this->Value());
			SystemErrorCheck();
			return false;
		}
	}
	Parse( buf, 0 );
	//fuck android sometimes crash here
	try
	{
		delete buf;
	}
	catch(const std::exception& ex)
	{
		const char*l_str = ex.what();
		int a=0;
	}
	buf = 0;
	if (  Error() )
        return false;
    else
		return true;
}

bool TiXmlDocument::LoadFile( NvFile* file)
{
	if ( !file )
	{
		SetError( TIXML_ERROR_OPENING_FILE, 0, 0 );
		return false;
	}
	// Delete the existing data:
	Clear();
	location.Clear();

	// Get the file size, so we can pre-allocate the string. HUGE speed impact.
	long length = 0;
#ifdef ANDROID
	length = NvFSize(file);
#else
	NvFSeek( file, 0, SEEK_END );
	length = NvFTell( file );
	NvFSeek( file, 0, SEEK_SET );
#endif
	// Strange case, but good to handle up front.
	if ( length <= 0 )
	{
		SetError( TIXML_ERROR_DOCUMENT_EMPTY, 0, 0 );
		return false;
	}
	wchar_t* buf = new wchar_t[ length ];//assume all file is unicode
	size_t l_siNumRead = NvFRead( buf, length, 1, file );
	//check compress file or not
	if ( l_siNumRead == 0 )
	{
		delete [] buf;
		SetError( TIXML_ERROR_OPENING_FILE, 0, 0 );
		return false;
	}
	bool	l_bResult = LoadFile( (char*)buf,length);
	delete [] buf;
	return l_bResult;
}

bool TiXmlDocument::SaveFile( const char * filename,bool e_bBinary ) const
{
//ANDROID
	//http://developer.android.com/reference/android/Manifest.permission.html#WRITE_EXTERNAL_STORAGE
	//<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/>
	//<uses-permission android:name="android.permission.READ_INTERNAL_STORAGE"/>
	//http://stackoverflow.com/questions/1992953/file-operations-in-android-ndk
	//All of the above is correct. You can open a file through the NDK using FILE and fopen. BUT... Dont forget to place a permission for it. in the Android manifest place a
	//<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/> 
	//int	l_ii = chdir("/sdcard/");
//

	//"r"Opens for reading. If the file does not exist or cannot be found, the fopen call fails.
	//"w"Opens an empty file for writing. If the given file exists, its contents are destroyed.
	//"a"Opens for writing at the end of the file (appending) without removing the EOF marker before writing new data to the file; creates the file first if it doesn't exist.
	//"r+"Opens for both reading and writing. (The file must exist.)
	//"w+"Opens an empty file for both reading and writing. If the given file exists, its contents are destroyed.

	//"a+"
	// The old c stuff lives on...
	NvFile* fp = MyFileOpen( filename, "w" );
	if ( fp )
	{
		FILE*l_pFile = NvGetFile(fp);
		bool result = SaveFile( l_pFile );
		NvFClose( fp );
		if( e_bBinary )
			result = FileCompressHuffman(filename,filename);
		return result;
	}
	return false;
}


bool TiXmlDocument::SaveFile( FILE* fp ) const
{
	if ( useMicrosoftBOM )
	{
		const unsigned char TIXML_UTF_LEAD_0 = 0xefU;
		const unsigned char TIXML_UTF_LEAD_1 = 0xbbU;
		const unsigned char TIXML_UTF_LEAD_2 = 0xbfU;

		fputc( TIXML_UTF_LEAD_0, fp );
		fputc( TIXML_UTF_LEAD_1, fp );
		fputc( TIXML_UTF_LEAD_2, fp );
	}
	Print( fp, 0 );
	return (ferror(fp) == 0);
}

void TiXmlDocument::CopyTo( TiXmlDocument* target ) const
{
	TiXmlNode::CopyTo( target );

	target->error = error;
	target->errorId = errorId;
	target->errorDesc = errorDesc;
	target->tabsize = tabsize;
	target->errorLocation = errorLocation;
	target->useMicrosoftBOM = useMicrosoftBOM;

	TiXmlNode* node = 0;
	for ( node = firstChild; node; node = node->NextSibling() )
	{
		target->LinkEndChild( node->Clone() );
	}
}


TiXmlNode* TiXmlDocument::Clone() const
{
	TiXmlDocument* clone = new TiXmlDocument();
	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}



bool TiXmlDocument::Accept( TiXmlVisitor* visitor ) const
{
	if ( visitor->VisitEnter( *this ) )
	{
		for ( const TiXmlNode* node=FirstChild(); node; node=node->NextSibling() )
		{
			if ( !node->Accept( visitor ) )
				break;
		}
	}
	return visitor->VisitExit( *this );
}


const TiXmlAttribute* TiXmlAttribute::Next() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( next->value.empty() && next->name.empty() )
		return 0;
	return next;
}

/*
TiXmlAttribute* TiXmlAttribute::Next()
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( next->value.empty() && next->name.empty() )
		return 0;
	return next;
}
*/

const TiXmlAttribute* TiXmlAttribute::Previous() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( prev->value.empty() && prev->name.empty() )
		return 0;
	return prev;
}

/*
TiXmlAttribute* TiXmlAttribute::Previous()
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( prev->value.empty() && prev->name.empty() )
		return 0;
	return prev;
}
*/



int TiXmlAttribute::QueryIntValue( int* ival ) const
{
	if ( TIXML_SSCANF( value.c_str(), L"%d", ival ) == 1 )
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}

int TiXmlAttribute::QueryDoubleValue( double* dval ) const
{
	if ( TIXML_SSCANF( value.c_str(), L"%lf", dval ) == 1 )
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}

void TiXmlAttribute::SetIntValue( int _value )
{
	SetValue (ValueToStringW(_value));
}

void TiXmlAttribute::SetDoubleValue( double _value )
{
	SetValue (ValueToStringW(_value));
}

int TiXmlAttribute::IntValue() const
{
	return _wtoi (value.c_str ());
}

double  TiXmlAttribute::DoubleValue() const
{
	return _wtof (value.c_str ());
}


TiXmlComment::TiXmlComment( const TiXmlComment& copy ) : TiXmlNode( TiXmlNode::COMMENT )
{
	copy.CopyTo( this );
}


void TiXmlComment::operator=( const TiXmlComment& base )
{
	Clear();
	base.CopyTo( this );
}



void TiXmlComment::CopyTo( TiXmlComment* target ) const
{
	TiXmlNode::CopyTo( target );
}


bool TiXmlComment::Accept( TiXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}


TiXmlNode* TiXmlComment::Clone() const
{
	TiXmlComment* clone = new TiXmlComment();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}




void TiXmlText::CopyTo( TiXmlText* target ) const
{
	TiXmlNode::CopyTo( target );
	target->cdata = cdata;
}


bool TiXmlText::Accept( TiXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}


TiXmlNode* TiXmlText::Clone() const
{
	TiXmlText* clone = 0;
	clone = new TiXmlText( L"" );

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


TiXmlDeclaration::TiXmlDeclaration( const wchar_t* _version,
									const wchar_t* _encoding,
									const wchar_t* _standalone )
	: TiXmlNode( TiXmlNode::DECLARATION )
{
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}


#ifdef TIXML_USE_STL
TiXmlDeclaration::TiXmlDeclaration(	const std::wstring& _version,
									const std::wstring& _encoding,
									const std::wstring& _standalone )
	: TiXmlNode( TiXmlNode::DECLARATION )
{
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}
#endif


TiXmlDeclaration::TiXmlDeclaration( const TiXmlDeclaration& copy )
	: TiXmlNode( TiXmlNode::DECLARATION )
{
	copy.CopyTo( this );
}


void TiXmlDeclaration::operator=( const TiXmlDeclaration& copy )
{
	Clear();
	copy.CopyTo( this );
}



void TiXmlDeclaration::CopyTo( TiXmlDeclaration* target ) const
{
	TiXmlNode::CopyTo( target );

	target->version = version;
	target->encoding = encoding;
	target->standalone = standalone;
}


bool TiXmlDeclaration::Accept( TiXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}


TiXmlNode* TiXmlDeclaration::Clone() const
{
	TiXmlDeclaration* clone = new TiXmlDeclaration();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}



void TiXmlUnknown::CopyTo( TiXmlUnknown* target ) const
{
	TiXmlNode::CopyTo( target );
}


bool TiXmlUnknown::Accept( TiXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}


TiXmlNode* TiXmlUnknown::Clone() const
{
	TiXmlUnknown* clone = new TiXmlUnknown();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


TiXmlAttributeSet::TiXmlAttributeSet()
{
	sentinel.next = &sentinel;
	sentinel.prev = &sentinel;
}


TiXmlAttributeSet::~TiXmlAttributeSet()
{
	assert( sentinel.next == &sentinel );
	assert( sentinel.prev == &sentinel );
}


void TiXmlAttributeSet::Add( TiXmlAttribute* addMe )
{
    #ifdef TIXML_USE_STL
	assert( !Find( TIXML_STRING( addMe->Name() ) ) );	// Shouldn't be multiply adding to the set.
	#else
	assert( !Find( addMe->Name() ) );	// Shouldn't be multiply adding to the set.
	#endif

	addMe->next = &sentinel;
	addMe->prev = sentinel.prev;

	sentinel.prev->next = addMe;
	sentinel.prev      = addMe;
}

void TiXmlAttributeSet::Remove( TiXmlAttribute* removeMe )
{
	TiXmlAttribute* node;

	for( node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node == removeMe )
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
			node->next = 0;
			node->prev = 0;
			return;
		}
	}
	assert( 0 );		// we tried to remove a non-linked attribute.
}


#ifdef TIXML_USE_STL
const TiXmlAttribute* TiXmlAttributeSet::Find( const std::wstring& name ) const
{
	for( const TiXmlAttribute* node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node->name == name )
			return node;
	}
	return 0;
}

/*
TiXmlAttribute*	TiXmlAttributeSet::Find( const std::wstring& name )
{
	for( TiXmlAttribute* node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node->name == name )
			return node;
	}
	return 0;
}
*/
#endif


const TiXmlAttribute* TiXmlAttributeSet::Find( const wchar_t* name ) const
{
	for( const TiXmlAttribute* node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( wcscmp( node->name.c_str(), name ) == 0 )
			return node;
	}
	return 0;
}

/*
TiXmlAttribute*	TiXmlAttributeSet::Find( const wchar_t* name )
{
	for( TiXmlAttribute* node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( wcscmp( node->name.c_str(), name ) == 0 )
			return node;
	}
	return 0;
}
*/

#ifdef TIXML_USE_STL
std::istream& operator>> (std::istream & in, TiXmlNode & base)
{
	TIXML_STRING tag;
	tag.reserve( 8 * 1000 );
	base.StreamIn( &in, &tag );

	base.Parse( tag.c_str(), 0);
	return in;
}
#endif


#ifdef TIXML_USE_STL
std::ostream& operator<< (std::ostream & out, const TiXmlNode & base)
{
	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	base.Accept( &printer );
	out << printer.Str();

	return out;
}


std::wstring& operator<< (std::wstring& out, const TiXmlNode& base )
{
	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	base.Accept( &printer );
	out.append( printer.Str() );

	return out;
}
#endif


TiXmlHandle TiXmlHandle::FirstChild() const
{
	if ( node )
	{
		TiXmlNode* child = node->FirstChild();
		if ( child )
			return TiXmlHandle( child );
	}
	return TiXmlHandle( 0 );
}


TiXmlHandle TiXmlHandle::FirstChild( const wchar_t* value ) const
{
	if ( node )
	{
		TiXmlNode* child = node->FirstChild( value );
		if ( child )
			return TiXmlHandle( child );
	}
	return TiXmlHandle( 0 );
}


TiXmlHandle TiXmlHandle::FirstChildElement() const
{
	if ( node )
	{
		TiXmlElement* child = node->FirstChildElement();
		if ( child )
			return TiXmlHandle( child );
	}
	return TiXmlHandle( 0 );
}


TiXmlHandle TiXmlHandle::FirstChildElement( const wchar_t* value ) const
{
	if ( node )
	{
		TiXmlElement* child = node->FirstChildElement( value );
		if ( child )
			return TiXmlHandle( child );
	}
	return TiXmlHandle( 0 );
}


TiXmlHandle TiXmlHandle::Child( int count ) const
{
	if ( node )
	{
		int i;
		TiXmlNode* child = node->FirstChild();
		for (	i=0;
				child && i<count;
				child = child->NextSibling(), ++i )
		{
			// nothing
		}
		if ( child )
			return TiXmlHandle( child );
	}
	return TiXmlHandle( 0 );
}


TiXmlHandle TiXmlHandle::Child( const wchar_t* value, int count ) const
{
	if ( node )
	{
		int i;
		TiXmlNode* child = node->FirstChild( value );
		for (	i=0;
				child && i<count;
				child = child->NextSibling( value ), ++i )
		{
			// nothing
		}
		if ( child )
			return TiXmlHandle( child );
	}
	return TiXmlHandle( 0 );
}


TiXmlHandle TiXmlHandle::ChildElement( int count ) const
{
	if ( node )
	{
		int i;
		TiXmlElement* child = node->FirstChildElement();
		for (	i=0;
				child && i<count;
				child = child->NextSiblingElement(), ++i )
		{
			// nothing
		}
		if ( child )
			return TiXmlHandle( child );
	}
	return TiXmlHandle( 0 );
}


TiXmlHandle TiXmlHandle::ChildElement( const wchar_t* value, int count ) const
{
	if ( node )
	{
		int i;
		TiXmlElement* child = node->FirstChildElement( value );
		for (	i=0;
				child && i<count;
				child = child->NextSiblingElement( value ), ++i )
		{
			// nothing
		}
		if ( child )
			return TiXmlHandle( child );
	}
	return TiXmlHandle( 0 );
}


bool TiXmlPrinter::VisitEnter( const TiXmlDocument& )
{
	return true;
}

bool TiXmlPrinter::VisitExit( const TiXmlDocument& )
{
	return true;
}

bool TiXmlPrinter::VisitEnter( const TiXmlElement& element, const TiXmlAttribute* firstAttribute )
{
	DoIndent();
	buffer += L"<";
	buffer += element.Value();

	for( const TiXmlAttribute* attrib = firstAttribute; attrib; attrib = attrib->Next() )
	{
		buffer += L" ";
		attrib->Print( 0, 0, &buffer );
	}

	if ( !element.FirstChild() )
	{
		buffer += L" />";
		DoLineBreak();
	}
	else
	{
		buffer += L">";
		if (    element.FirstChild()->ToText()
			  && element.LastChild() == element.FirstChild()
			  && element.FirstChild()->ToText()->CDATA() == false )
		{
			simpleTextPrint = true;
			// no DoLineBreak()!
		}
		else
		{
			DoLineBreak();
		}
	}
	++depth;
	return true;
}


bool TiXmlPrinter::VisitExit( const TiXmlElement& element )
{
	--depth;
	if ( !element.FirstChild() )
	{
		// nothing.
	}
	else
	{
		if ( simpleTextPrint )
		{
			simpleTextPrint = false;
		}
		else
		{
			DoIndent();
		}
		buffer += L"</";
		buffer += element.Value();
		buffer += L">";
		DoLineBreak();
	}
	return true;
}


bool TiXmlPrinter::Visit( const TiXmlText& text )
{
	if ( text.CDATA() )
	{
		DoIndent();
		buffer += L"<![CDATA[";
		buffer += text.Value();
		buffer += L"]]>";
		DoLineBreak();
	}
	else if ( simpleTextPrint )
	{
		TIXML_STRING str;
		TiXmlBase::EncodeString( text.ValueTStr(), &str );
		buffer += str;
	}
	else
	{
		DoIndent();
		TIXML_STRING str;
		TiXmlBase::EncodeString( text.ValueTStr(), &str );
		buffer += str;
		DoLineBreak();
	}
	return true;
}


bool TiXmlPrinter::Visit( const TiXmlDeclaration& declaration )
{
	DoIndent();
	declaration.Print( 0, 0, &buffer );
	DoLineBreak();
	return true;
}


bool TiXmlPrinter::Visit( const TiXmlComment& comment )
{
	DoIndent();
	buffer += L"<!--";
	buffer += comment.Value();
	buffer += L"-->";
	DoLineBreak();
	return true;
}


bool TiXmlPrinter::Visit( const TiXmlUnknown& unknown )
{
	DoIndent();
	buffer += L"<";
	buffer += unknown.Value();
	buffer += L">";
	DoLineBreak();
	return true;
}

