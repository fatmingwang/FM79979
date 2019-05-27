#include "StdAfx.h"
#include "GlyphWriter.h"
#pragma warning( disable:4996 )

//#define BIG_ENDIAN
//
//#ifdef BIG_ENDIAN
//    enum { D0 = 3, D1 = 2, D2 = 1, D3 = 0 };
//    enum { W0 = 1, W1 = 0,  };
//#else
//    enum { D0 = 0, D1 = 1, D2 = 2, D3 = 3 };
//    enum { W0 = 0, W1 = 1,  };
//#endif
int D0 = -1,D1 = -1,D2 = -1,D3 = -1;
int W0 = -1,W1 = -1;
void SwapToPc()
{
	D0 = 0;D1 = 1;D2 = 2;D3 = 3;
	W0 = 0;W1 = 1;
}

void SwapToXBOX()
{
	D0 = 3;D1 = 2;D2 = 1;D3 = 0;
	W0 = 1;W1 = 0;
}
VOID WriteToFile( FILE* file, FLOAT val )
{
    CHAR data[4] = { ((CHAR*)&val)[D0], ((CHAR*)&val)[D1],
                     ((CHAR*)&val)[D2], ((CHAR*)&val)[D3] };
    fwrite( data, sizeof(data), 1, file ); 
}


VOID WriteToFile( FILE* file, unsigned int val )
{
    CHAR data[4] = { ((CHAR*)&val)[D0], ((CHAR*)&val)[D1],
                     ((CHAR*)&val)[D2], ((CHAR*)&val)[D3] };
    fwrite( data, sizeof(data), 1, file ); 
}


VOID WriteToFile( FILE* file, SHORT val )
{
    CHAR data[2] = { ((CHAR*)&val)[W0], ((CHAR*)&val)[W1] };
    fwrite( data, sizeof(data), 1, file ); 
}


VOID WriteToFile( FILE* file, WORD val )
{
    CHAR data[2] = { ((CHAR*)&val)[W0], ((CHAR*)&val)[W1] };
    fwrite( data, sizeof(data), 1, file ); 
}


GlyphWriter::GlyphWriter(void)
{
	m_wFontHeight = 0;
    m_ValidGlyphs = 0;
    m_cMaxGlyph = 0;
    m_TranslatorTable = 0;
	m_dwNumGlyphs = -1;
	m_dwMask = 0;
	m_pGlyphs = 0;
}

GlyphWriter::~GlyphWriter(void)
{
	DestroyObjects();
}
void	GlyphWriter::SetFontInfo(SHORT wHeight,DWORD e_dwColor,DWORD e_dwNumGlyph,WCHAR e_dwMaxGlyph)
{
	m_wFontHeight = wHeight;
	m_dwMask = e_dwColor;
	m_dwNumGlyphs = e_dwNumGlyph; m_pGlyphs = new FILE_GLYPH_ATTR[e_dwNumGlyph];
	m_cMaxGlyph = e_dwMaxGlyph;
}

HRESULT GlyphWriter::BuildTranslatorTable()
{
    if( m_TranslatorTable )
        delete[] m_TranslatorTable;

    //// Insure the \0 is there
    //if( m_bIncludeNullCharacter && 0 == m_ValidGlyphs['\0'] )
    //    m_dwNumGlyphs++, m_ValidGlyphs['\0'] = 1;

    // Fill the string of all valid glyphs and build the translator table
    m_TranslatorTable = new WORD[m_cMaxGlyph+1];
    ZeroMemory( m_TranslatorTable, sizeof(WORD)*(m_cMaxGlyph+1) );
    DWORD dwGlyph = 0;
    for( DWORD i=0; i<65536; i++ )
    {
        if( m_ValidGlyphs[i] )
        {
            m_TranslatorTable[i] = (WORD)dwGlyph;
            dwGlyph++;
        }
    }
    
    return S_OK;
}

HRESULT	GlyphWriter::ExtractValidGlyphsFromWcharPointer( WCHAR *e_pString)
{
    if( m_TranslatorTable )
        delete[] m_TranslatorTable;

    // Fill the string of all valid glyphs and build the translator table
	m_TranslatorTable = new WORD[this->m_cMaxGlyph+1];
	//set default value is invalid
	memset(m_TranslatorTable,m_dwNumGlyphs,sizeof(WORD)*m_cMaxGlyph+1);
	WORD	j = 0;
	for( WCHAR i=0;i<(WCHAR)(m_cMaxGlyph+1);i++ )
	{
		if( e_pString[j] == i )
		{
			m_TranslatorTable[i] = j;
			j++;
		}
	}
	assert(j==this->m_dwNumGlyphs&&"impossible data set not correct");
	return true;
}

//============================================
//
//============================================
HRESULT GlyphWriter::ExtractValidGlyphsFromRange( WORD wStartGlyph, WORD wEndGlyph )
{
	if(!this->m_pGlyphs)
		assert(0&&"no font info can't write any info");
    // Cleanup any previous entries
    if( m_ValidGlyphs )
        delete[] m_ValidGlyphs;

    m_cMaxGlyph       = 0;
    m_dwNumGlyphs     = 0;
    m_ValidGlyphs     = NULL;

    // Allocate memory for the array of vaild glyphs
    m_ValidGlyphs = new BYTE[65536];
    ZeroMemory( m_ValidGlyphs, 65536 );

    for( DWORD c=(DWORD)wStartGlyph; c<=(DWORD)wEndGlyph; c++ )
    {
        m_ValidGlyphs[c] = 1;
        m_dwNumGlyphs++;
    }

    m_cMaxGlyph = wEndGlyph;

    BuildTranslatorTable();

    return S_OK;
}

VOID GlyphWriter::DestroyObjects()
{
	SAFE_DELETE_ARRAY(m_pGlyphs);
	SAFE_DELETE_ARRAY(m_TranslatorTable);
	SAFE_DELETE_ARRAY(m_ValidGlyphs);

    m_cMaxGlyph       = 0;
    m_dwNumGlyphs     = 0;
    m_pGlyphs         = NULL;
    m_ValidGlyphs     = NULL;
    m_TranslatorTable = NULL;
}

//--------------------------------------------------------------------------------------
// Name: ExtractValidGlyphsFromRange()
// Desc: Set global variables to indicate we will be drawing all glyphs that
//       are present in the specified text file.
//--------------------------------------------------------------------------------------
HRESULT GlyphWriter::ExtractValidGlyphsFromFile( const CHAR* strFileName )
{
    // Open the file
    FILE* file = fopen( strFileName, "rb" );
    if( NULL == file )
        return E_FAIL;

    // Cleanup any previous entries
    if( m_ValidGlyphs )
        delete[] m_ValidGlyphs;

    m_cMaxGlyph       = 0;
    m_dwNumGlyphs     = 0;
    m_ValidGlyphs     = NULL;

    // Allocate memory for the array of vaild glyphs
    m_ValidGlyphs = new BYTE[65536];
    ZeroMemory( m_ValidGlyphs, 65536 );

    // Skip the unicode marker
    BOOL bIsUnicode = (fgetwc(file) == 0xfeff) ? TRUE : FALSE;

    if( bIsUnicode == FALSE )
        rewind( file );

    // Record which glyphs are valid
    WCHAR c;
    while( (WCHAR)EOF != ( c = bIsUnicode ? fgetwc(file) : fgetc(file) ) )
    {
        while( c == L'\\' )
        {
            c = bIsUnicode ? fgetwc(file) : fgetc(file);

            // Handle octal-coded characters
            if( isdigit(c) )
            {
                int code = (c - L'0');
                c = bIsUnicode ? fgetwc(file) : fgetc(file);
                
                if( isdigit(c) )
                {
                    code = code*8 + (c - L'0');
                    c = bIsUnicode ? fgetwc(file) : fgetc(file);

                    if( isdigit(c) )
                    {
                        code = code*8 + (c - L'0');
                        c = bIsUnicode ? fgetwc(file) : fgetc(file);
                    }
                }

                if( m_ValidGlyphs[code] == 0 )
                {
                    if( code > m_cMaxGlyph )
                        m_cMaxGlyph = (WCHAR)code;

                    m_dwNumGlyphs++;
                    m_ValidGlyphs[code] = 2;
                }
            }
            else
            { 
                // Add the backslash character
                if( L'\\' > m_cMaxGlyph )
                    m_cMaxGlyph = L'\\';

                if( m_ValidGlyphs[L'\\'] == 0 )
                {
                    m_dwNumGlyphs++;
                    m_ValidGlyphs[L'\\'] = 1;
                }
            }
        }

        if( m_ValidGlyphs[c] == 0 )
        {
            // If the character is a printable one, add it
            if( c != L'\n' && c != L'\r' && c != 0xffff )
            {
                m_dwNumGlyphs++;
                m_ValidGlyphs[c] = 1;

                if( c > m_cMaxGlyph )
                    m_cMaxGlyph = c;
            }
        }
    }

    // Done with the file
    fclose( file );

    BuildTranslatorTable();

    return S_OK;
}
//1.Version			size:DWORD*1
//2.Height			size:SHORT*1
//3.MaskColor		size:DWORD*1
//4.MaxGlyph		size:WORD*1
//5.TranslatorTable size:WORD*(MaxGlyph)
//6.m_dwNumGlyphs	size:DWORD*1
//7.m_pGlyphs		size:m_dwNumGlyphs*FILE_GLYPH_ATTR
HRESULT	GlyphWriter::SaveFile(char*strFileName,bool e_bPC)
{
	if( e_bPC )
		SwapToPc();
	else
		SwapToXBOX();
	assert(m_wFontHeight&&"Font Height can't be 0");
	HRESULT hr = S_OK;
    // Create the info file
    FILE* file = fopen( strFileName, "wb" );
    if( NULL == file )
        return E_FAIL;

    // Write out the file version
	//1
    WriteToFile( file, FONTINFOFILEVERSION );

    // Write out the font height
	SHORT wFontHeight = this->m_wFontHeight;
	//2
    WriteToFile( file, wFontHeight ); 
	//mask color for alpha
	auto	l_dwMaskColor = m_dwMask;
	//3
    WriteToFile( file,l_dwMaskColor);
    // Write the translator table
    {
		//4
        WriteToFile( file, (WORD)m_cMaxGlyph ); 
		//5
        for( DWORD i=0; i<(DWORD)(m_cMaxGlyph+1); i++ )
            WriteToFile( file, m_TranslatorTable[i] ); 
    }

    // Write the glyph attributes to the file
    {
		//6
        WriteToFile( file, m_dwNumGlyphs );
		//7
        for( DWORD i=0; i<m_dwNumGlyphs; i++ )
        {
            WriteToFile( file, m_pGlyphs[i].fLeft );
            WriteToFile( file, m_pGlyphs[i].fTop );
            WriteToFile( file, m_pGlyphs[i].fRight );
            WriteToFile( file, m_pGlyphs[i].fBottom );
			assert(m_pGlyphs[i].fHeight>0&&"Widtht can't be zero");
			WriteToFile( file, m_pGlyphs[i].fWidth );
			assert(m_pGlyphs[i].fHeight>0&&"Height can't be zero");
			WriteToFile( file, m_pGlyphs[i].fHeight );
        }
    }

    // Close the file and return okay
    fclose( file );
	return hr;
}