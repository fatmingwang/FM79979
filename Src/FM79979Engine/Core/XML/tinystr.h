/*
www.sourceforge.net/projects/tinyxml
Original file by Yves Berquin.

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

/*
 * THIS FILE WAS ALTERED BY Tyge Lovset, 7. April 2005.
 *
 * - completely rewritten. compact, clean, and fast implementation.
 * - sizeof(TiXmlString) = pointer size (4 bytes on 32-bit systems)
 * - fixed reserve() to work as per specification.
 * - fixed buggy compares operator==(), operator<(), and operator>()
 * - fixed operator+=() to take a const ref argument, following spec.
 * - added "copy" constructor with length, and most compare operators.
 * - added swap(), clear(), size(), capacity(), operator+().
 */

#ifndef TIXML_USE_STL

#ifndef TIXML_STRING_INCLUDED
#define TIXML_STRING_INCLUDED

#include <assert.h>
#include <string.h>

#define TiXmlString	std::wstring

inline bool operator == (const TiXmlString & a, const TiXmlString & b)
{
	return    ( a.length() == b.length() )				// optimization on some platforms
	       && ( wcscmp(a.c_str(), b.c_str()) == 0 );	// actual compare
}
inline bool operator < (const TiXmlString & a, const TiXmlString & b)
{
	return wcscmp(a.c_str(), b.c_str()) < 0;
}

inline bool operator != (const TiXmlString & a, const TiXmlString & b) { return !(a == b); }
inline bool operator >  (const TiXmlString & a, const TiXmlString & b) { return b < a; }
inline bool operator <= (const TiXmlString & a, const TiXmlString & b) { return !(b < a); }
inline bool operator >= (const TiXmlString & a, const TiXmlString & b) { return !(a < b); }

inline bool operator == (const TiXmlString & a, const wchar_t* b) { return wcscmp(a.c_str(), b) == 0; }
inline bool operator == (const wchar_t* a, const TiXmlString & b) { return b == a; }
inline bool operator != (const TiXmlString & a, const wchar_t* b) { return !(a == b); }
inline bool operator != (const wchar_t* a, const TiXmlString & b) { return !(b == a); }

TiXmlString operator + (const TiXmlString & a, const TiXmlString & b);
TiXmlString operator + (const TiXmlString & a, const wchar_t* b);
TiXmlString operator + (const wchar_t* a, const TiXmlString & b);



#endif	// TIXML_STRING_INCLUDED
#endif	// TIXML_USE_STL
