#ifndef Gluph_Reader_H
#define Gluph_Reader_H

#include "../Common/SmartPointer.h"

const unsigned long FONTINFOFILEVERSION = 0x00000005; // Texture file properties
//==========================================
//reference from xbox360 FontMake tool
//==========================================
//if the data size is different,get the data from binary data will pointer to wrong data.......I donno why sux.....
struct FILE_GLYPH_ATTR
{
    float	fLeft;
	float	fTop;
	float	fRight;
	float	fBottom;
    float   fWidth;
	float   fHeight;
};

class GlyphReader:public cSmartObject,public NamedTypedObject
{
	bool	LoadFile( const char* strFileName);
public:
	unsigned char*	m_pFontData;
    wchar_t			m_cMaxGlyph;
    WORD*			m_pTranslatorTable;
	unsigned long			m_dwNumGlyphs;
	FILE_GLYPH_ATTR* m_pGlyphs;


	GlyphReader();
	virtual ~GlyphReader();

	SHORT			m_wFontHeight;
	unsigned long			m_dwMask;//to alpha

	bool	LoadFontDataFile(const char* e_pString);
	inline	int	FindIndex( wchar_t e_dwChar )
	{
		assert(m_pTranslatorTable[e_dwChar]<m_dwNumGlyphs);
		return m_pTranslatorTable[e_dwChar]; 
	}
	bool	IsLegalCharacter(wchar_t e_Character);
	inline FILE_GLYPH_ATTR	GetCharInfo( int e_iIndex )
	{
		assert((int)m_dwNumGlyphs>e_iIndex);
		return m_pGlyphs[e_iIndex]; 
	}
	FILE_GLYPH_ATTR	GetCharInfo(wchar_t e_dwChar){ return GetCharInfo(FindIndex(e_dwChar)); }
};

#endif