#include "tinystr.h"
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
 * THIS FILE WAS ALTERED BY Tyge L�vset, 7. April 2005.
 */


#ifndef TIXML_USE_STL

#include "tinystr.h"




TiXmlString operator + (const TiXmlString & a, const TiXmlString & b)
{
	TiXmlString tmp;
	tmp.reserve(a.length() + b.length());
	tmp += a;
	tmp += b;
	return tmp;
}

TiXmlString operator + (const TiXmlString & a, const wchar_t* b)
{
	TiXmlString tmp;
	TiXmlString::size_type b_len = static_cast<TiXmlString::size_type>( wcslen(b) );
	tmp.reserve(a.length() + b_len);
	tmp += a;
	tmp.append(b, b_len);
	return tmp;
}

TiXmlString operator + (const wchar_t* a, const TiXmlString & b)
{
	TiXmlString tmp;
	TiXmlString::size_type a_len = static_cast<TiXmlString::size_type>( wcslen(a) );
	tmp.reserve(a_len + b.length());
	tmp.append(a, a_len);
	tmp += b;
	return tmp;
}


#endif	// TIXML_USE_STL

