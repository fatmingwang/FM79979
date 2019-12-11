#pragma once
#ifndef WIN32

#ifndef VS_GO_GNN
#define VS_GO_GNN
#include <stdio.h>
#include <wchar.h>
#include <ctype.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
//inline wchar_t* WeiredWcstok(wchar_t*e_str1, const wchar_t*e_str2)
//{
//	wchar_t* l_pStringForWCSTOK = nullptr;
//	return ::wcstok(e_str1, e_str2, &l_pStringForWCSTOK);
//}
extern wchar_t* g_pStringForWCSTOK;
inline wchar_t* wcstok(wchar_t*e_str1, const wchar_t*e_str2)
{
	return ::wcstok(e_str1, e_str2, &g_pStringForWCSTOK);
}
//http://source.winehq.org/source/dlls/ntdll/wcstring.c
/*********************************************************************
*      _wtol    (NTDLL.@)
*
* Converts a unicode string to a long integer.
*
* PARAMS
*  str [I] Wstring to be converted
*
* RETURNS
*  On success it returns the integer value otherwise it returns 0.
*
* NOTES
*  Accepts: {whitespace} [+|-] {digits}
*  No check is made for value overflow, only the lower 32 bits are assigned.
*  If str is nullptr it crashes, as the native function does.
*/
	inline  long _wtol( wchar_t* str )
	{
		long RunningTotal = 0;
		bool bMinus = false;

		while (isspace(*str))
		{
			str++;
		}
		if (*str == '+')
		{
			str++;
		}
		else
		if (*str == '-')
		{
			bMinus = true;
			str++;
	    }
		while (*str >= L'0' && *str <= '9')
		{
			RunningTotal = RunningTotal * 10 + *str - '0';
			str++;
		} /* while */
		return bMinus ? -RunningTotal : RunningTotal;
	}


//        inline  float _wtof( const wchar_t* e_str )
//        {
//            return _wtof( (wchar_t*)e_str );
//        }


//inline const char* fast_atof_move( const char* c, float& out)
//{
//	float f;
//
//	bool inv = (*c=='-');
//	if (inv || *c=='+')
//		++c;
//
//	f = (float) strtol10_64 ( c, &c);
//	if (*c == '.' || (c[0] == ',' && (c[1] >= '0' || c[1] <= '9'))) // allow for commas, too
//	{
//		++c;
//
//		// NOTE: The original implementation is highly unaccurate here. The precision of a single
//		// IEEE 754 float is not high enough, everything behind the 6th digit tends to be more 
//		// inaccurate than it would need to be. Casting to double seems to solve the problem.
//		// strtol_64 is used to prevent integer overflow.
//
//		// Another fix: this tends to become 0 for long numbers if we don't limit the maximum 
//		// number of digits to be read. AI_FAST_ATOF_RELAVANT_DECIMALS can be a value between
//		// 1 and 15.
//		unsigned int diff = AI_FAST_ATOF_RELAVANT_DECIMALS;
//		double pl = (double) strtol10_64 ( c, &c, &diff );
//
//		pl *= fast_atof_table[diff];
//		f += (float)pl;
//	}
//
//	// A major 'E' must be allowed. Necessary for proper reading of some DXF files.
//	// Thanks to Zhao Lei to point out that this if() must be outside the if (*c == '.' ..)
//	if (*c == 'e' || *c == 'E')
//	{
//		++c;
//		bool einv = (*c=='-');
//		if (einv || *c=='+')
//			++c;
//
//		float exp = (float)strtol10_64(c, &c);
//		if (einv)
//			exp *= -1.0f;
//
//		f *= pow(10.0f, exp);
//	}
//
//	if (inv)
//		f *= -1.0f;
//	
//	out = f;
//	return c;
//}
//
// Just in case you haven't got an atof() around...
//   This one doesn't check for bad syntax or overflow,
//   and is slow and inaccurate.
//   But it's good enough for the occasional string literal... 
//
//#include "pyconfig.h"
//
//#include <ctype.h>
//
//double atof(char *s)
//{
//        double a = 0.0;
//        int e = 0;
//        int c;
//        while ((c = *s++) != '\0' && isdigit(c)) {
//                a = a*10.0 + (c - '0');
//        }
//        if (c == '.') {
//                while ((c = *s++) != '\0' && isdigit(c)) {
//                        a = a*10.0 + (c - '0');
//                        e = e-1;
//                }
//        }
//        if (c == 'e' || c == 'E') {
//                int sign = 1;
//                int i = 0;
//                c = *s++;
//                if (c == '+')
//                        c = *s++;
//                else if (c == '-') {
//                        c = *s++;
//                        sign = -1;
//                }
//                while (isdigit(c)) {
//                        i = i*10 + (c - '0');
//                        c = *s++;
//                }
//                e += i*sign;
//        }
//        while (e > 0) {
//                a *= 10.0;
//                e--;
//        }
//        while (e < 0) {
//                a *= 0.1;
//                e++;
//        }
//        return a;
//}

	inline double _wtof( const wchar_t* e_str )
	{
            bool bMinus = false;
            if (*e_str == '+')
            {
               e_str++;
            }
            else
            if (*e_str == '-')
            {
                bMinus = true;
                e_str++;
            }
            double  l_dbValue = 0.f;
            int     l_dotStart = -1;
            int l_iLength = (int)wcslen(e_str);
            for( int i=0;i<l_iLength;++i )
            {
                if( e_str[i] == L'.' )
                {
                    l_dotStart = i;
                }
                else
                if( e_str[i] >= L'0' && e_str[i] <= L'9' )
                {
                    l_dbValue = (l_dbValue*10) +(e_str[i]-L'0');
                }
				else
					if( e_str[i] == L'e'||e_str[i] == L'E' )
				{
					int	l_ie = e_str[i+2]-L'0';
					for( int j=1;j<l_ie;++j )
						l_dbValue /= 10;
				}

            }

            if( l_dotStart != -1 )
            {
                float	l_fDotOffset = 1.f;
                for( int j=1;j<l_iLength-l_dotStart;++j )
                    l_fDotOffset*=10;
                l_dbValue/=l_fDotOffset;
            }
            return bMinus?-l_dbValue:l_dbValue;
	}

/*********************************************************************
*      _wtoi    (NTDLL.@)
*
* Converts a unicode string to an integer.
*
* PARAMS
*  str [I] Wstring to be converted
*
* RETURNS
*  On success it returns the integer value otherwise it returns 0.
*
* NOTES
*  Accepts: {whitespace} [+|-] {digits}
*  No check is made for value overflow, only the lower 32 bits are assigned.
*  If str is nullptr it crashes, as the native function does.
*/
	inline int _wtoi( wchar_t* str )
	{
		return (int)_wtol(str);
	}

	inline int _wtoi( const wchar_t* str )
	{
		return (int)_wtol((wchar_t*)str);
	}

	inline	void	_itow(int val,wchar_t*e_strBuffer,int base)
	{
		wchar_t buf[32] = {0};
		memset(buf,0,sizeof(wchar_t)*30);
		if( val!=0 )
		{
			int i = 30;
			for(; val && i ; --i, val /= base)
				buf[i] = L"0123456789abcdef"[val % base];
			memcpy(e_strBuffer,&buf[i+1],sizeof(wchar_t)*32-i-1);
		}
		else
		{
			e_strBuffer[0] = L'0';
			e_strBuffer[1] = L'\0';
		}
	}

	inline	void    itoa(int val,char*e_strBuffer, int base)
	{
		char buf[32] = {0};
		memset(buf,0,sizeof(char)*30);
		if( val!=0 )
		{
			int i = 30;

			for(; val && i ; --i, val /= base)

				buf[i] = "0123456789abcdef"[val % base];

			memcpy(e_strBuffer,&buf[i+1],sizeof(char)*32-i-1);
		}
		else
		{
			e_strBuffer[0] = '0';
			e_strBuffer[1] = '\0';
		}
	}


#endif//VS_GO_GNN
#endif//win32