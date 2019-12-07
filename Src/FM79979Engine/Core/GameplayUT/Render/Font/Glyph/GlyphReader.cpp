#include "GlyphReader.h"
#include "../../../XML/XMLLoader.h"
#include "../../../Common/BinaryFile/StringCompress.h"
#include "../../../Common/Log/FMLog.h"
GlyphReader::GlyphReader():cSmartObject(this)
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
	SAFE_DELETE_ARRAY(m_pGlyphs);
	SAFE_DELETE_ARRAY(m_pTranslatorTable);
#endif
}

bool GlyphReader::LoadFile( const char* strFileName)
{
	NvFile*	l_pFile = MyFileOpen(strFileName,"rb");
	if( !l_pFile )
		return false;
	long	l_lFileSize = GetFileSize(l_pFile);
	//fuck malloc is not proper for iOS and Android!?it's because memory alligment.
	//it will crush if I fetch some data frome it...ex:m_pGlyphs
	SAFE_DELETE(m_pFontData);
	m_pFontData = new unsigned char[l_lFileSize];
	if(m_pFontData == nullptr )
	{
		NvFClose(l_pFile );
		return false;
	}
	NvFRead(m_pFontData,sizeof(char),l_lFileSize,l_pFile);
	if( IsCompressFile(l_pFile) )
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
	NvFClose(l_pFile );
    return true;
}

bool	GlyphReader::IsLegalCharacter(wchar_t e_Character)
{
	if(e_Character >= this->m_cMaxGlyph)
		return false;
	if( (unsigned int)m_pTranslatorTable[e_Character]<m_dwNumGlyphs )
	{
		return true;
	}
	return false;
}

//1.Version			size:unsigned long*1
//2.Height			size:short*1
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
	FMLog::LogWithFlag("LoadFontDataFile start\n", CORE_LOG_FLAG);
	BYTE*	l_pData = (BYTE*)m_pFontData;
    // Check version of file (to make sure it matches up with the FontMaker tool)
    unsigned int dwFileVersion = *((unsigned int*)(l_pData)); l_pData += sizeof(unsigned int);
    if( dwFileVersion == 0x00000005 )
    {
		unsigned short l_sHeight = 0;				memcpy(&l_sHeight, l_pData, sizeof(short));	l_pData += sizeof(unsigned short);
		unsigned int l_uiMask = 0;		    memcpy(&l_uiMask, l_pData, sizeof(unsigned int));	l_pData += sizeof(unsigned int);
		unsigned short	l_sMaxGlyph = 0;		    memcpy(&l_sMaxGlyph, l_pData, sizeof(unsigned short));	l_pData += sizeof(unsigned  short);
		//http://opass.logdown.com/posts/743054-about-memory-alignment
		//
#if defined(ANDROID) || defined(IOS) || defined(WASM) || defined(LINUX)
		short*	l_psTranslatorTable = new short[l_sMaxGlyph + 1];	    memcpy(l_psTranslatorTable, l_pData, sizeof(short)*(l_sMaxGlyph + 1));	l_pData += (sizeof(short)*(l_sMaxGlyph + 1));
#else
		short*	l_psTranslatorTable = (short*)l_pData;	l_pData += (sizeof(short)*(l_sMaxGlyph + 1));
#endif
		unsigned int l_uiNumGlyphs = 0;	    memcpy(&l_uiNumGlyphs, l_pData, sizeof(unsigned int));	l_pData += sizeof(unsigned int);
		m_wFontHeight	= l_sHeight;
		m_dwMask		= l_uiMask;
        m_cMaxGlyph			= l_sMaxGlyph;
        m_pTranslatorTable	= l_psTranslatorTable;
        m_dwNumGlyphs		= l_uiNumGlyphs;
		//FMLog::LogWithFlag(UT::ComposeMsgByFormat("font wierd new start:NumGlyphs%d\n)",(int)m_dwNumGlyphs), CORE_LOG_FLAG);
#if defined(ANDROID) || defined(IOS) || defined(WASM) ||defined (LINUX)
		m_pGlyphs = new FILE_GLYPH_ATTR[m_dwNumGlyphs];
		memcpy(m_pGlyphs,l_pData,sizeof(FILE_GLYPH_ATTR)*m_dwNumGlyphs);
#else
		m_pGlyphs = (FILE_GLYPH_ATTR*)l_pData;
#endif
    }
    else
    {
		FMLog::LogWithFlag("Font data Incorrect version number on font file!\n", CORE_LOG_FLAG);
        //ATG_PrintError( "Incorrect version number on font file!\n" );
        return false;
    }
	FMLog::LogWithFlag("Font data parse finish\n", CORE_LOG_FLAG);
	return true;
}
