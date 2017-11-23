#include "../stdafx.h"
/*
www.sourceforge.net/projects/tinyxml
Original code (2.0 and earlier )copyright (c) 2000-2002 Lee Thomason (www.grinninglizard.com)

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
#include <stddef.h>

#include "tinyxml.h"

//#define DEBUG_PARSER
#if defined( DEBUG_PARSER )
#	if defined( DEBUG ) && defined( _MSC_VER )
#		include <windows.h>
#		define TIXML_LOG OutputDebugString
#	else
#		define TIXML_LOG printf
#	endif
#endif

// Note tha "PutString" hardcodes the same list. This
// is less flexible than it appears. Changing the entries
// or order will break putstring.
TiXmlBase::Entity TiXmlBase::entity[ NUM_ENTITY ] =
{
	{ L"&amp;",  5, L'&' },
	{ L"&lt;",   4, L'<' },
	{ L"&gt;",   4, L'>' },
	{ L"&quot;", 6, L'\"' },
	{ L"&apos;", 6, L'\'' }
};

// Bunch of unicode info at:
//		http://www.unicode.org/faq/utf_bom.html
// Including the basic of this table, which determines the #bytes in the
// sequence from the lead byte. 1 placed for invalid sequences --
// although the result will be junk, pass it through as much as possible.
// Beware of the non-WCHARacters in UTF-8:
//				ef bb bf (Microsoft "lead bytes")
//				ef bf be
//				ef bf bf

const wchar_t TIXML_UTF_LEAD_0 = 0xefU;
const wchar_t TIXML_UTF_LEAD_1 = 0xbbU;
const wchar_t TIXML_UTF_LEAD_2 = 0xbfU;


void TiXmlBase::ConvertUTF32ToUTF8( unsigned long input, wchar_t* output, int* length )
{
	const unsigned long BYTE_MASK = 0xBF;
	const unsigned long BYTE_MARK = 0x80;
	const unsigned long FIRST_BYTE_MARK[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

	if (input < 0x80)
		*length = 1;
	else if ( input < 0x800 )
		*length = 2;
	else if ( input < 0x10000 )
		*length = 3;
	else if ( input < 0x200000 )
		*length = 4;
	else
		{ *length = 0; return; }	// This code won't covert this correctly anyway.

	output += *length;

	// Scary scary fall throughs.
	switch (*length)
	{
		case 4:
			--output;
			*output = (wchar_t)((input | BYTE_MARK) & BYTE_MASK);
			input >>= 6;
		case 3:
			--output;
			*output = (wchar_t)((input | BYTE_MARK) & BYTE_MASK);
			input >>= 6;
		case 2:
			--output;
			*output = (wchar_t)((input | BYTE_MARK) & BYTE_MASK);
			input >>= 6;
		case 1:
			--output;
			*output = (wchar_t)(input | FIRST_BYTE_MARK[*length]);
	}
}


int TiXmlBase::IsAlpha( wchar_t anyByte )
{
	// This will only work for low-ascii, everything else is assumed to be a valid
	// letter. I'm not sure this is the best approach, but it is quite tricky trying
	// to figure out alhabetical vs. not across encoding. So take a very
	// conservative approach.

	return isalpha( anyByte );
}


int TiXmlBase::IsAlphaNum( wchar_t anyByte )
{
	// This will only work for low-ascii, everything else is assumed to be a valid
	// letter. I'm not sure this is the best approach, but it is quite tricky trying
	// to figure out alhabetical vs. not across encoding. So take a very
	// conservative approach.

//	if ( encoding == TIXML_ENCODING_UTF8 )
//	{
		if ( anyByte < 127 )
			return isalnum( anyByte );
		else
			return 1;	// What else to do? The unicode set is huge...get the english ones right.
//	}
//	else
//	{
//		return isalnum( anyByte );
//	}
}


class TiXmlParsingData
{
	friend class TiXmlDocument;
  public:
	void Stamp( const wchar_t* now);

	const TiXmlCursor& Cursor()	{ return cursor; }

  private:
	// Only used by the document!
	TiXmlParsingData( const wchar_t* start, int _tabsize, int row, int col )
	{
		assert( start );
		stamp = start;
		tabsize = _tabsize;
		cursor.row = row;
		cursor.col = col;
	}

	TiXmlCursor		cursor;
	const wchar_t*		stamp;
	int				tabsize;
};


void TiXmlParsingData::Stamp( const wchar_t* now )
{
	assert( now );

	// Do nothing if the tabsize is 0.
	if ( tabsize < 1 )
	{
		return;
	}

	// Get the current row, column.
	int row = cursor.row;
	int col = cursor.col;
	const wchar_t* p = stamp;
	assert( p );

	while ( p < now )
	{
		// Treat p as unsigned, so we have a happy compiler.
		const wchar_t* pU = (const wchar_t*)p;

		// Code contributed by Fletcher Dunn: (modified by lee)
		switch (*pU) {
			case 0:
				// We *should* never get here, but in case we do, don't
				// advance past the terminating null WCHARacter, ever
				return;

			case '\r':
				// bump down to the next line
				++row;
				col = 0;
				// Eat the WCHARacter
				++p;

				// Check for \r\n sequence, and treat this as a single WCHARacter
				if (*p == '\n') {
					++p;
				}
				break;

			case '\n':
				// bump down to the next line
				++row;
				col = 0;

				// Eat the WCHARacter
				++p;

				// Check for \n\r sequence, and treat this as a single
				// WCHARacter.  (Yes, this bizarre thing does occur still
				// on some arcane platforms...)
				if (*p == '\r') {
					++p;
				}
				break;

			case '\t':
				// Eat the WCHARacter
				++p;

				// Skip to next tab stop
				col = (col / tabsize + 1) * tabsize;
				break;

			case TIXML_UTF_LEAD_0:
					++p;
					++col;
				break;

			default:
				++p;
				++col;
				break;
		}
	}
	cursor.row = row;
	cursor.col = col;
	assert( cursor.row >= -1 );
	assert( cursor.col >= -1 );
	stamp = p;
	assert( stamp );
}


const wchar_t* TiXmlBase::SkipWhiteSpace( const wchar_t* p)
{
	if ( !p || !*p )
	{
		return 0;
	}

	wchar_t lp = 65279;
	while ( *p && IsWhiteSpace( *p ) || *p == L'\n' || *p == L'\r'|| *p== lp || *p == L'\t' )
		++p;

	return p;
}

// One of TinyXML's more performance demanding functions. Try to keep the memory overhead down. The
// "assign" optimization removes over 10% of the execution time.
//
const wchar_t* TiXmlBase::ReadName( const wchar_t* p, TIXML_STRING * name )
{
	// Oddly, not supported on some comilers,
	//name->clear();
	// So use this:
	*name = L"";
	assert( p );

	// Names start with letters or underscores.
	// Of course, in unicode, tinyxml has no idea what a letter *is*. The
	// algorithm is generous.
	//
	// After that, they can be letters, underscores, numbers,
	// hyphens, or colons. (Colons are valid ony for namespaces,
	// but tinyxml can't tell namespaces from names.)
        if( p[1] == L'C'&&
                p[2] == L'o'&&
                p[3] == L'u'&&
                p[4] == L'n'&&
                p[5] == L't')
        {
            int a=0;
        }
	if (    p && *p
		 && ( IsAlpha( (wchar_t) *p) || *p == L'_' ) )
	{
		const wchar_t* start = p;
		while(		p && *p
				&&	(		IsAlphaNum( (wchar_t ) *p )
						 || *p == L'_'
						 || *p == L'-'
						 || *p == L'.'
						 || *p == L':' ) )
		{
			//(*name) += *p; // expensive
			++p;
		}
		if ( p-start > 0 ) {
			name->assign( start, p-start );
		}
		return p;
	}
	return 0;
}

const wchar_t* TiXmlBase::GetEntity( const wchar_t* p, wchar_t* value, int* length )
{
	// Presume an entity, and pull it out.
    TIXML_STRING ent;
	int i;
	*length = 0;

	if ( *(p+1) && *(p+1) == '#' && *(p+2) )
	{
		unsigned long ucs = 0;
		ptrdiff_t delta = 0;
		unsigned mult = 1;

		if ( *(p+2) == 'x' )
		{
			// Hexadecimal.
			if ( !*(p+3) ) return 0;

			const wchar_t* q = p+3;
			q = ::wcschr( q, ';' );

			if ( !q || !*q ) return 0;

			delta = q-p;
			--q;

			while ( *q != 'x' )
			{
				if ( *q >= '0' && *q <= '9' )
					ucs += mult * (*q - '0');
				else if ( *q >= 'a' && *q <= 'f' )
					ucs += mult * (*q - 'a' + 10);
				else if ( *q >= 'A' && *q <= 'F' )
					ucs += mult * (*q - 'A' + 10 );
				else
					return 0;
				mult *= 16;
				--q;
			}
		}
		else
		{
			// Decimal.
			if ( !*(p+2) ) return 0;

			const wchar_t* q = p+2;
			q = ::wcschr( q, ';' );

			if ( !q || !*q ) return 0;

			delta = q-p;
			--q;

			while ( *q != '#' )
			{
				if ( *q >= '0' && *q <= '9' )
					ucs += mult * (*q - '0');
				else
					return 0;
				mult *= 10;
				--q;
			}
		}
		*value = (wchar_t)ucs;
		*length = 1;
		return p + delta + 1;
	}

	// Now try to match it.
	for( i=0; i<NUM_ENTITY; ++i )
	{
		if ( wcscmp( entity[i].str, p) == 0 )
		{
			assert( wcslen( entity[i].str ) == entity[i].strLength );
			*value = entity[i].chr;
			*length = 1;
			return ( p + entity[i].strLength );
		}
	}

	// So it wasn't an entity, its unrecognized, or something like that.
	*value = *p;	// Don't put back the last one, since we return it!
	//*length = 1;	// Leave unrecognized entities - this doesn't really work.
					// Just writes strange XML.
	return p+1;
}


bool TiXmlBase::StringEqual( const wchar_t* p,
							 const wchar_t* tag,
							 bool ignoreCase)
{
	assert( p );
	assert( tag );
	if ( !p || !*p )
	{
		assert( 0 );
		return false;
	}

	const wchar_t* q = p;

	if ( ignoreCase )
	{
		while ( *q && *tag && ToLower( *q ) == ToLower( *tag ) )
		{
			++q;
			++tag;
		}

		if ( *tag == 0 )
			return true;
	}
	else
	{
		while ( *q && *tag && *q == *tag )
		{
			++q;
			++tag;
		}

		if ( *tag == 0 )		// Have we found the end of the tag, and everything equal?
			return true;
	}
	return false;
}

const wchar_t* TiXmlBase::ReadText(	const wchar_t* p,
									TIXML_STRING * text,
									bool trimWhiteSpace,
									const wchar_t* endTag,
									bool caseInsensitive)
{
    *text = L"";
	if (    !trimWhiteSpace			// certain tags always keep whitespace
		 || !condenseWhiteSpace )	// if true, whitespace is always kept
	{
		// Keep all the white space.
		while (	   p && *p
				&& !StringEqual( p, endTag, caseInsensitive )
			  )
		{
			int len;
			wchar_t cArr[4] = { 0, 0, 0, 0 };
			p = GetWchar( p, cArr, &len );
			text->append( cArr, len );
		}
	}
	else
	{
		bool whitespace = false;

		// Remove leading white space:
		p = SkipWhiteSpace( p);
		while (	   p && *p
				&& !StringEqual( p, endTag, caseInsensitive) )
		{
			if ( *p == '\r' || *p == '\n' )
			{
				whitespace = true;
				++p;
			}
			else if ( IsWhiteSpace( *p ) )
			{
				whitespace = true;
				++p;
			}
			else
			{
				// If we've found whitespace, add it before the
				// new WCHARacter. Any whitespace just becomes a space.
				if ( whitespace )
				{
					(*text) += ' ';
					whitespace = false;
				}
				int len;
				wchar_t cArr[4] = { 0, 0, 0, 0 };
				p = GetWchar( p, cArr, &len);
				if ( len == 1 )
					(*text) += cArr[0];	// more efficient
				else
					text->append( cArr, len );
			}
		}
	}
	if ( p )
		p += wcslen( endTag );
	return p;
}

const wchar_t* TiXmlDocument::Parse( const wchar_t* p, TiXmlParsingData* prevData )
{
	ClearError();

	// Parse away, at the document level. Since a document
	// contains nothing but other tags, most of what happens
	// here is skipping white space.
	if ( !p || !*p )
	{
		SetError( TIXML_ERROR_DOCUMENT_EMPTY, 0, 0);
		return 0;
	}

	// Note that, for a document, this needs to come
	// before the while space skip, so that parsing
	// starts from the pointer we are given.
	location.Clear();
	if ( prevData )
	{
		location.row = prevData->cursor.row;
		location.col = prevData->cursor.col;
	}
	else
	{
		location.row = 0;
		location.col = 0;
	}
	TiXmlParsingData data( p, TabSize(), location.row, location.col );
	location = data.Cursor();


    //p = SkipWhiteSpace( p, encoding );
	if ( !p )
	{
		SetError( TIXML_ERROR_DOCUMENT_EMPTY, 0, 0);
		return 0;
	}

	while ( p && *p )
	{
		TiXmlNode* node = Identify( p);
		if ( node )
		{
			p = node->Parse( p, &data);
			LinkEndChild( node );
		}
		else
		{
			break;
		}

		p = SkipWhiteSpace( p);
	}

	// Was this empty?
	if ( !firstChild ) {
		SetError( TIXML_ERROR_DOCUMENT_EMPTY, 0, 0 );
		return 0;
	}

	// All is well.
	return p;
}

void TiXmlDocument::SetError( int err, const wchar_t* pError, TiXmlParsingData* data )
{
	// The first error in a chain is more accurate - don't set again!
	if ( error )
		return;
#ifdef WIN32
#ifdef DEBUG	
	OutputDebugString(L"TiXmlDocument::SetError:");
	OutputDebugString(FATMING_CORE::ValueToStringW(err).c_str());
#endif
#endif
	assert( err > 0 && err < TIXML_ERROR_STRING_COUNT );
	error   = true;
	errorId = err;
	errorDesc = errorString[ errorId ];

	errorLocation.Clear();
	if ( pError && data )
	{
		data->Stamp( pError );
		errorLocation = data->Cursor();
	}
}


TiXmlNode* TiXmlNode::Identify( const wchar_t* p )
{
	TiXmlNode* returnNode = 0;
	p = SkipWhiteSpace( p );
	if( !p || !*p || *p != L'<' )
	{
		return 0;
	}

	TiXmlDocument* doc = GetDocument();
	p = SkipWhiteSpace( p );

	if ( !p || !*p )
	{
		return 0;
	}

	// What is this thing?
	// - Elements start with a letter or underscore, but xml is reserved.
	// - Comments: <!--
	// - Decleration: <?xml
	// - Everthing else is unknown to tinyxml.
	//

	const wchar_t* xmlHeader = { L"<?xml" };
	const wchar_t* commentHeader = { L"<!--" };
	const wchar_t* dtdHeader = { L"<!" };
	const wchar_t* cdataHeader = { L"<![CDATA[" };

	if ( StringEqual( p, xmlHeader, true) )
	{
		#ifdef DEBUG_PARSER
			TIXML_LOG( "XML parsing Declaration\n" );
		#endif
		returnNode = new TiXmlDeclaration();
	}
	else if ( StringEqual( p, commentHeader, false) )
	{
		#ifdef DEBUG_PARSER
			TIXML_LOG( "XML parsing Comment\n" );
		#endif
		returnNode = new TiXmlComment();
	}
	else if ( StringEqual( p, cdataHeader, false) )
	{
		#ifdef DEBUG_PARSER
			TIXML_LOG( "XML parsing CDATA\n" );
		#endif
		TiXmlText* text = new TiXmlText( L"" );
		text->SetCDATA( true );
		returnNode = text;
	}
	else if ( StringEqual( p, dtdHeader, false) )
	{
		#ifdef DEBUG_PARSER
			TIXML_LOG( "XML parsing Unknown(1)\n" );
		#endif
		returnNode = new TiXmlUnknown();
	}
	else if (    IsAlpha( *(p+1) )
			  || *(p+1) == '_' )
	{
		#ifdef DEBUG_PARSER
			TIXML_LOG( "XML parsing Element\n" );
		#endif
		returnNode = new TiXmlElement( L"" );
	}
	else
	{
		#ifdef DEBUG_PARSER
			TIXML_LOG( "XML parsing Unknown(2)\n" );
		#endif
		returnNode = new TiXmlUnknown();
	}

	if ( returnNode )
	{
		// Set the parent, so it can report errors
		returnNode->parent = this;
	}
	else
	{
		if ( doc )
			doc->SetError( TIXML_ERROR_OUT_OF_MEMORY, 0, 0);
	}
	return returnNode;
}

const wchar_t* TiXmlElement::Parse( const wchar_t* p, TiXmlParsingData* data )
{
	p = SkipWhiteSpace( p );
	TiXmlDocument* document = GetDocument();

	if ( !p || !*p )
	{
		if ( document ) document->SetError( TIXML_ERROR_PARSING_ELEMENT, 0, 0 );
		return 0;
	}

	if ( data )
	{
		data->Stamp( p );
		location = data->Cursor();
	}

	if ( *p != '<' )
	{
		if ( document ) document->SetError( TIXML_ERROR_PARSING_ELEMENT, p, data );
		return 0;
	}

	p = SkipWhiteSpace( p+1 );

	// Read the name.
	const wchar_t* pErr = p;

    p = ReadName( p, &value);
	if ( !p || !*p )
	{
		if ( document )	document->SetError( TIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME, pErr, data );
		return 0;
	}

    TIXML_STRING endTag (L"</");
	endTag += value;
	endTag += L">";

	// Check for and read attributes. Also look for an empty
	// tag or an end tag.
	while ( p && *p )
	{
		pErr = p;
		p = SkipWhiteSpace( p );
		if ( !p || !*p )
		{
			if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES, pErr, data );
			return 0;
		}
		if ( *p == L'/' )
		{
			++p;
			// Empty tag.
			if ( *p  != L'>' )
			{
				if ( document ) document->SetError( TIXML_ERROR_PARSING_EMPTY, p, data);
				return 0;
			}
			return (p+1);
		}
		else if ( *p == L'>' )
		{
			// Done with attributes (if there were any.)
			// Read the value -- which can include other
			// elements -- read the end tag, and return.
			++p;
			p = ReadValue( p, data );		// Note this is an Element method, and will set the error if one happens.
			if ( !p || !*p ) {
				// We were looking for the end tag, but found nothing.
				// Fix for [ 1663758 ] Failure to report error on bad XML
				if ( document ) document->SetError( TIXML_ERROR_READING_END_TAG, p, data );
				return 0;
			}

			// We should find the end tag now
			if ( StringEqual( p, endTag.c_str(), false) )
			{
				p += endTag.length();
				return p;
			}
			else
			{
				if ( document ) document->SetError( TIXML_ERROR_READING_END_TAG, p, data );
				return 0;
			}
		}
		else
		{
			// Try to read an attribute:
			TiXmlAttribute* attrib = new TiXmlAttribute();
			if ( !attrib )
			{
				if ( document ) document->SetError( TIXML_ERROR_OUT_OF_MEMORY, pErr, data );
				return 0;
			}

			attrib->SetDocument( document );
			pErr = p;
			p = attrib->Parse( p, data);

			if ( !p || !*p )
			{
				if ( document ) document->SetError( TIXML_ERROR_PARSING_ELEMENT, pErr, data );
				delete attrib;
				return 0;
			}

			// Handle the strange case of double attributes:
			#ifdef TIXML_USE_STL
			TiXmlAttribute* node = attributeSet.Find( attrib->NameTStr() );
			#else
			TiXmlAttribute* node = attributeSet.Find( attrib->Name() );
			#endif
			if ( node )
			{
				node->SetValue( attrib->Value() );
				delete attrib;
				return 0;
			}

			attributeSet.Add( attrib );
		}
	}
	return p;
}


const wchar_t* TiXmlElement::ReadValue( const wchar_t* p, TiXmlParsingData* data )
{
	TiXmlDocument* document = GetDocument();

	// Read in text and elements in any order.
	const wchar_t* pWithWhiteSpace = p;
	p = SkipWhiteSpace( p);

	while ( p && *p )
	{
		if ( *p != L'<' )
		{
			// Take what we have, make a text element.
			TiXmlText* textNode = new TiXmlText( L"" );

			if ( !textNode )
			{
				if ( document ) document->SetError( TIXML_ERROR_OUT_OF_MEMORY, 0, 0 );
				    return 0;
			}

			if ( TiXmlBase::IsWhiteSpaceCondensed() )
			{
				p = textNode->Parse( p, data );
			}
			else
			{
				// Special case: we want to keep the white space
				// so that leading spaces aren't removed.
				p = textNode->Parse( pWithWhiteSpace, data );
			}

			if ( !textNode->Blank() )
				LinkEndChild( textNode );
			else
				delete textNode;
		}
		else
		{
			// We hit a '<'
			// Have we hit a new element or an end tag? This could also be
			// a TiXmlText in the "CDATA" style.
			if ( StringEqual( p, L"</", false) )
			{
				return p;
			}
			else
			{
				TiXmlNode* node = Identify( p );
				if ( node )
				{
					p = node->Parse( p, data);
					LinkEndChild( node );
				}
				else
				{
					return 0;
				}
			}
		}
		pWithWhiteSpace = p;
		p = SkipWhiteSpace( p);
	}

	if ( !p )
	{
		if ( document ) document->SetError( TIXML_ERROR_READING_ELEMENT_VALUE, 0, 0 );
	}
	return p;
}


#ifdef TIXML_USE_STL
void TiXmlUnknown::StreamIn( std::istream * in, TIXML_STRING * tag )
{
	while ( in->good() )
	{
		int c = in->get();
		if ( c <= 0 )
		{
			TiXmlDocument* document = GetDocument();
			if ( document )
				document->SetError( TIXML_ERROR_EMBEDDED_NULL, 0, 0);
			return;
		}
		(*tag) += (wchar_t) c;

		if ( c == '>' )
		{
			// All is well.
			return;
		}
	}
}
#endif


const wchar_t* TiXmlUnknown::Parse( const wchar_t* p, TiXmlParsingData* data )
{
	TiXmlDocument* document = GetDocument();
	p = SkipWhiteSpace( p);

	if ( data )
	{
		data->Stamp( p );
		location = data->Cursor();
	}
	if ( !p || !*p || *p != L'<' )
	{
		if ( document ) document->SetError( TIXML_ERROR_PARSING_UNKNOWN, p, data );
		return 0;
	}
	++p;
    value = L"";

	while ( p && *p && *p != L'>' )
	{
		value += *p;
		++p;
	}

	if ( !p )
	{
		if ( document )	document->SetError( TIXML_ERROR_PARSING_UNKNOWN, 0, 0 );
	}
	if ( *p == '>' )
		return p+1;
	return p;
}

#ifdef TIXML_USE_STL
void TiXmlComment::StreamIn( std::istream * in, TIXML_STRING * tag )
{
	while ( in->good() )
	{
		int c = in->get();
		if ( c <= 0 )
		{
			TiXmlDocument* document = GetDocument();
			if ( document )
				document->SetError( TIXML_ERROR_EMBEDDED_NULL, 0, 0);
			return;
		}

		(*tag) += (wchar_t) c;

		if ( c == '>'
			 && tag->at( tag->length() - 2 ) == '-'
			 && tag->at( tag->length() - 3 ) == '-' )
		{
			// All is well.
			return;
		}
	}
}
#endif


const wchar_t* TiXmlComment::Parse( const wchar_t* p, TiXmlParsingData* data )
{
	TiXmlDocument* document = GetDocument();
	value = L"";

	p = SkipWhiteSpace( p);

	if ( data )
	{
		data->Stamp( p);
		location = data->Cursor();
	}
	const wchar_t* startTag = L"<!--";
	const wchar_t* endTag   = L"-->";

	if ( !StringEqual( p, startTag, false) )
	{
		document->SetError( TIXML_ERROR_PARSING_COMMENT, p, data );
		return 0;
	}
	p += wcslen( startTag );

	// [ 1475201 ] TinyXML parses entities in comments
	// Oops - ReadText doesn't work, because we don't want to parse the entities.
	// p = ReadText( p, &value, false, endTag, false, encoding );
	//
	// from the XML spec:
	/*
	 [Definition: Comments may appear anywhere in a document outside other markup; in addition,
	              they may appear within the document type declaration at places allowed by the grammar.
				  They are not part of the document's WCHARacter data; an XML processor MAY, but need not,
				  make it possible for an application to retrieve the text of comments. For compatibility,
				  the string "--" (double-hyphen) MUST NOT occur within comments.] Parameter entity
				  references MUST NOT be recognized within comments.

				  An example of a comment:

				  <!-- declarations for <head> & <body> -->
	*/

    value = L"";
	// Keep all the white space.
	while (	p && *p && !StringEqual( p, endTag, false ) )
	{
		value.append( p, 1 );
		++p;
	}
	if ( p )
		p += wcslen( endTag );

	return p;
}


const wchar_t* TiXmlAttribute::Parse( const wchar_t* p, TiXmlParsingData* data )
{
	p = SkipWhiteSpace( p);
	if ( !p || !*p ) return 0;

//	int tabsize = 4;
//	if ( document )
//		tabsize = document->TabSize();

	if ( data )
	{
		data->Stamp( p );
		location = data->Cursor();
	}
	// Read the name, the '=' and the value.
	const wchar_t* pErr = p;
	p = ReadName( p, &name);
	if ( !p || !*p )
	{
		if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES, pErr, data );
		return 0;
	}
	p = SkipWhiteSpace( p);
	if ( !p || !*p || *p != '=' )
	{
		if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES, p, data );
		return 0;
	}

	++p;	// skip '='
	p = SkipWhiteSpace( p );
	if ( !p || !*p )
	{
		if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES, p, data );
		return 0;
	}

	const wchar_t* end;
	const wchar_t SINGLE_QUOTE = L'\'';
	const wchar_t DOUBLE_QUOTE = L'\"';

	if ( *p == SINGLE_QUOTE )
	{
		++p;
		end = L"\'";		// single quote in string
		p = ReadText( p, &value, false, end, false);
	}
	else if ( *p == DOUBLE_QUOTE )
	{
		++p;
		end = L"\"";		// double quote in string
		p = ReadText( p, &value, false, end, false);
	}
	else
	{
		// All attribute values should be in single or double quotes.
		// But this is such a common error that the parser will try
		// its best, even without them.
		value = L"";
		while (    p && *p											// existence
				&& !IsWhiteSpace( *p ) && *p != L'\n' && *p != L'\r'	// whitespace
				&& *p != L'/' && *p != L'>' )							// tag end
		{
			if ( *p == SINGLE_QUOTE || *p == DOUBLE_QUOTE ) {
				// [ 1451649 ] Attribute values with trailing quotes not handled correctly
				// We did not have an opening quote but seem to have a
				// closing one. Give up and throw an error.
				if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES, p, data );
				return 0;
			}
			value += *p;
			++p;
		}
	}
	return p;
}

#ifdef TIXML_USE_STL
void TiXmlText::StreamIn( std::istream * in, TIXML_STRING * tag )
{
	while ( in->good() )
	{
		int c = in->peek();
		if ( !cdata && (c == '<' ) )
		{
			return;
		}
		if ( c <= 0 )
		{
			TiXmlDocument* document = GetDocument();
			if ( document )
				document->SetError( TIXML_ERROR_EMBEDDED_NULL, 0, 0);
			return;
		}

		(*tag) += (wchar_t) c;
		in->get();	// "commits" the peek made above

		if ( cdata && c == '>' && tag->size() >= 3 ) {
			size_t len = tag->size();
			if ( (*tag)[len-2] == ']' && (*tag)[len-3] == ']' ) {
				// terminator of cdata.
				return;
			}
		}
	}
}
#endif

const wchar_t* TiXmlText::Parse( const wchar_t* p, TiXmlParsingData* data )
{
	value = L"";
	TiXmlDocument* document = GetDocument();

	if ( data )
	{
		data->Stamp( p );
		location = data->Cursor();
	}

	const wchar_t* const startTag = L"<![CDATA[";
 	const wchar_t* const endTag   = L"]]>";

	if ( cdata || StringEqual( p, startTag, false) )
	{
		cdata = true;

		if ( !StringEqual( p, startTag, false ) )
		{
			document->SetError( TIXML_ERROR_PARSING_CDATA, p, data );
			return 0;
		}
		p += wcslen( startTag );

		// Keep all the white space, ignore the encoding, etc.
		while (	   p && *p
				&& !StringEqual( p, endTag, false )
			  )
		{
			value += *p;
			++p;
		}

		TIXML_STRING dummy;
		p = ReadText( p, &dummy, false, endTag, false);
		return p;
	}
	else
	{
		bool ignoreWhite = true;

		const wchar_t* end = L"<";
		p = ReadText( p, &value, ignoreWhite, end, false);
		if ( p )
			return p-1;	// don't truncate the '<'
		return 0;
	}
}

#ifdef TIXML_USE_STL
void TiXmlDeclaration::StreamIn( std::istream * in, TIXML_STRING * tag )
{
	while ( in->good() )
	{
		int c = in->get();
		if ( c <= 0 )
		{
			TiXmlDocument* document = GetDocument();
			if ( document )
				document->SetError( TIXML_ERROR_EMBEDDED_NULL, 0, 0);
			return;
		}
		(*tag) += (wchar_t) c;

		if ( c == '>' )
		{
			// All is well.
			return;
		}
	}
}
#endif

const wchar_t* TiXmlDeclaration::Parse( const wchar_t* p, TiXmlParsingData* data)
{
	p = SkipWhiteSpace( p );
	// Find the beginning, find the end, and look for
	// the stuff in-between.
	TiXmlDocument* document = GetDocument();
	if ( !p || !*p || !StringEqual( p, L"<?xml", true ) )
	{
		if ( document ) document->SetError( TIXML_ERROR_PARSING_DECLARATION, 0, 0);
		return 0;
	}
	if ( data )
	{
		data->Stamp( p );
		location = data->Cursor();
	}
	p += 5;

	version = L"";
	encoding = L"";
	standalone = L"";

	while ( p && *p )
	{
		if ( *p == L'>' )
		{
			++p;
			return p;
		}

		p = SkipWhiteSpace( p );
		if ( StringEqual( p, L"version", true ) )
		{
			TiXmlAttribute attrib;
			p = attrib.Parse( p, data );
			version = attrib.Value();
		}
		else if ( StringEqual( p, L"encoding", true ) )
		{
			TiXmlAttribute attrib;
			p = attrib.Parse( p, data );
			encoding = attrib.Value();
		}
		else if ( StringEqual( p, L"standalone", true ) )
		{
			TiXmlAttribute attrib;
			p = attrib.Parse( p, data );
			standalone = attrib.Value();
		}
		else
		{
			// Read over whatever it is.
			while( p && *p && *p != L'>' && !IsWhiteSpace( *p ) )
				++p;
		}
	}
	return 0;
}

bool TiXmlText::Blank() const
{
	for ( unsigned i=0; i<value.length(); i++ )
		if ( !IsWhiteSpace( value[i] ) )
			return false;
	return true;
}
