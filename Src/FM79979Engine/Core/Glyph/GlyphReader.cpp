#include "../stdafx.h"
#include "GlyphReader.h"
#include "../XML/XMLLoader.h"
#include "../GameplayUT/StringCompress.h"
#include "../GameplayUT/GameApp.h"
GlyphReader::GlyphReader()
{
	m_pFontData = 0;
	m_wFontHeight = 0;
	m_dwMask = 0;//to alpha
    m_cMaxGlyph = 0;
    m_pTranslatorTable = 0;
	m_dwNumGlyphs = 0;
	m_pGlyphs = 0;
}

GlyphReader::~GlyphReader()
{
	SAFE_DELETE(m_pFontData);
#if defined(ANDROID) || defined(IOS)
	SAFE_DELETE(m_pGlyphs);
#endif
}

bool GlyphReader::LoadFile( const char* strFileName)
{
	NvFile*	l_pFile = MyFileOpen(strFileName,"rb");
	if( !l_pFile )
		return false;
	long	l_lFileSize = GetFileSize(l_pFile);
	//fuck malloc is not proper for iOS and Android!?
	//it will crush if I fetch some data frome it...ex:m_pGlyphs
	m_pFontData = new unsigned char[l_lFileSize];
	if( nullptr == m_pFontData )
	{
		NvFClose(l_pFile );
		return false;
	}
	NvFRead(m_pFontData,sizeof(char),l_lFileSize,l_pFile);
	if( IsCompressFile(l_pFile) )
	{
		if( cGameApp::m_sbAllowParseBinaryFile )
		{
			unsigned char *l_pDes;
			int l_iDesLen = 0;
			unsigned char*l_pData = (unsigned char*)m_pFontData;
			l_pData += sizeof(int);
			if(!DecompressHuffman((unsigned char*)l_pData,l_lFileSize, l_pDes,l_iDesLen))
			{
				UT::ErrorMsg("font info binary file error",strFileName);
			}
			delete m_pFontData;
			m_pFontData = l_pDes;
		}
		else
		{
			NvFClose(l_pFile );
			return false;;
		}
	}
	NvFClose(l_pFile );
    return true;
}

bool	GlyphReader::IsLegalCharacter(wchar_t e_Character)
{
	if(e_Character >= this->m_cMaxGlyph)
		return false;
	if( m_pTranslatorTable[e_Character]<m_dwNumGlyphs )
	{
		return true;
	}
	return false;
}

//1.Version			size:unsigned long*1
//2.Height			size:SHORT*1
//3.MaskColor		size:unsigned long*1
//4.MaxGlyph		size:WORD*1
//5.TranslatorTable size:WORD*(MaxGlyph+1)
//6.m_dwNumGlyphs	size:unsigned long*1
//7.m_pGlyphs		size:m_dwNumGlyphs*FILE_GLYPH_ATTR
bool	GlyphReader::LoadFontDataFile(const char* strFileName)
{
	if( !LoadFile( strFileName) )
	{
		assert(0&&"open file failed");
		return false;
	}

	BYTE*	l_pData = (BYTE*)m_pFontData;
    // Check version of file (to make sure it matches up with the FontMaker tool)
    unsigned int dwFileVersion = *((unsigned int*)(l_pData)); l_pData += sizeof(unsigned int);
    if( dwFileVersion == 0x00000005 )
    {
        // Parse the font data
		this->m_wFontHeight	= *((SHORT*)l_pData); l_pData += sizeof(SHORT);
		this->m_dwMask		= *((unsigned int*)l_pData); l_pData += sizeof(unsigned int);
        // Point to the translator string
        m_cMaxGlyph			= ((WORD*)l_pData)[0];   l_pData += sizeof(WORD);
        m_pTranslatorTable	= (WORD*)l_pData;       l_pData += sizeof(WORD)*(m_cMaxGlyph+1);

        // Read the glyph attributes from the file
        m_dwNumGlyphs		= ((unsigned int*)l_pData)[0];  l_pData += sizeof(unsigned int);
#if defined(ANDROID) || defined(IOS)
		m_pGlyphs = new FILE_GLYPH_ATTR[m_dwNumGlyphs];
		memcpy(m_pGlyphs,l_pData,sizeof(FILE_GLYPH_ATTR)*m_dwNumGlyphs);
#else
		m_pGlyphs = (FILE_GLYPH_ATTR*)l_pData;
#endif
    }
    else
    {
        //ATG_PrintError( "Incorrect version number on font file!\n" );
        return false;
    }
	return true;
}
