#pragma once
const unsigned int FONTINFOFILEVERSION = 0x00000005; // Texture file properties
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

class GlyphReader
{
	HRESULT	LoadFile( const WCHAR* strFileName);
public:
	VOID*			m_pFontData;
    WCHAR			m_cMaxGlyph;
    WORD*			m_TranslatorTable;
	DWORD			m_dwNumGlyphs;
	FILE_GLYPH_ATTR* m_pGlyphs;


	GlyphReader(void);
	virtual ~GlyphReader(void);

	SHORT			m_wFontHeight;
	DWORD			m_dwMask;//to alpha

	HRESULT	LoadFontDataFile(const WCHAR* e_pString);
	int	FindIndex( WCHAR e_dwChar ){ assert(m_TranslatorTable[e_dwChar]<m_dwNumGlyphs); return m_TranslatorTable[e_dwChar]; }
	FILE_GLYPH_ATTR	GetCharInfo( int e_iIndex ){ assert((int)m_dwNumGlyphs>e_iIndex);  return m_pGlyphs[e_iIndex]; }
	FILE_GLYPH_ATTR	GetCharInfo(WCHAR e_dwChar){ return GetCharInfo(FindIndex(e_dwChar)); }
};
