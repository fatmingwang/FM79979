#include "StdAfx.h"
#include "GlyphReader.h"


HRESULT GlyphReader::LoadFile( const WCHAR* strFileName)
{
    // Open the file for reading
    HANDLE hFile = CreateFile( strFileName, GENERIC_READ, 0, NULL, 
                               OPEN_EXISTING, 0, NULL );
    
    if( INVALID_HANDLE_VALUE == hFile )
        return E_HANDLE;

    DWORD dwFileSize = GetFileSize( hFile, NULL );
    m_pFontData = malloc( dwFileSize );

    if( NULL == m_pFontData )
    {
        CloseHandle( hFile );
        return E_OUTOFMEMORY;
    }

    DWORD dwBytesRead;
    ReadFile( hFile, (VOID*)m_pFontData, dwFileSize, &dwBytesRead, NULL );
    
    // Finished reading file
    CloseHandle( hFile ); 

    if( dwBytesRead != dwFileSize )
    {
        free( m_pFontData );
        return E_FAIL;
    }

    return S_OK;
}


GlyphReader::GlyphReader(void)
{
	m_wFontHeight = 0;
	m_dwMask = 0;//to alpha
    m_cMaxGlyph = 0;
    m_TranslatorTable = 0;
	m_dwNumGlyphs = 0;
	m_pGlyphs = 0;
}

GlyphReader::~GlyphReader(void)
{
	if(m_pFontData)
		free( m_pFontData );
}


//1.Version			size:DWORD*1
//2.Height			size:SHORT*1
//3.MaskColor		size:DWORD*1
//4.MaxGlyph		size:WORD*1
//5.TranslatorTable size:WORD*(MaxGlyph+1)
//6.m_dwNumGlyphs	size:DWORD*1
//7.m_pGlyphs		size:m_dwNumGlyphs*FILE_GLYPH_ATTR
HRESULT	GlyphReader::LoadFontDataFile(const WCHAR* strFileName)
{
	HRESULT hr = S_OK;	
	hr = LoadFile( strFileName);
	if( FAILED(hr) )
		assert(0&&"open file failed");

	BYTE*	l_pData = (BYTE*)m_pFontData;
    // Check version of file (to make sure it matches up with the FontMaker tool)
    DWORD dwFileVersion = *((DWORD*)(l_pData)); l_pData += sizeof(DWORD);    
    if( dwFileVersion == 0x00000005 )
    {
        // Parse the font data
		this->m_wFontHeight	= *((SHORT*)l_pData); l_pData += sizeof(SHORT);
		this->m_dwMask		= *((DWORD*)l_pData); l_pData += sizeof(DWORD);
        // Point to the translator string
        m_cMaxGlyph			= ((WORD*)l_pData)[0];   l_pData += sizeof(WORD);
        m_TranslatorTable	= (WORD*)l_pData;       l_pData += sizeof(WORD)*(m_cMaxGlyph+1);

        // Read the glyph attributes from the file
        m_dwNumGlyphs		= ((DWORD*)l_pData)[0];  l_pData += sizeof(DWORD);
		m_pGlyphs = (FILE_GLYPH_ATTR*)l_pData;
    }
    else
    {
        //ATG_PrintError( "Incorrect version number on font file!\n" );
        return E_FAIL;
    }
	return hr;
}
