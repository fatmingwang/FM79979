//-------------------------------------------------------------------------------------
//  AtgXmlWriter.h
//  
//  A simple XML writer.
//  
//  Xbox Advanced Technology Group
//  Copyright ?Microsoft Corporation. All rights reserved.
//-------------------------------------------------------------------------------------
#ifdef WIN32

#include "../stdafx.h"
#include "AtgXmlWriter.h"
#include "tinyxml.h"
#include "XMLLoader.h"
#pragma warning( disable:4793 )

#define WRITE_BUFFER_SIZE 16384

void	WriteElementToFile(ATG::XMLWriter*e_pXMLWriter,TiXmlElement*e_pTiXmlElement)
{
	e_pXMLWriter->StartElement(e_pTiXmlElement->Value());
	TiXmlAttribute*	l_pAttribute = e_pTiXmlElement->FirstAttribute();
	const wchar_t*	l_strValue = 0;
	const wchar_t*	l_strName = 0;
	while(l_pAttribute)
	{
		l_strValue = l_pAttribute->Value();
		l_strName = l_pAttribute->Name();

		l_pAttribute = l_pAttribute->Next();
	}
	TiXmlElement*l_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while( l_pTiXmlElement )
	{
		WriteElementToFile(e_pXMLWriter,l_pTiXmlElement);
		l_pTiXmlElement = l_pTiXmlElement->FirstChildElement();
	}
	e_pXMLWriter->EndElement();
	l_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
	while( l_pTiXmlElement )
	{
		WriteElementToFile(e_pXMLWriter,l_pTiXmlElement);
		l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
	}
}

void	SaveTinyXMLToFile(char*e_strFileName,TiXmlElement*e_pTiXmlElement)
{
	ATG::XMLWriter	l_XMLWriter(e_strFileName);
	WriteElementToFile(&l_XMLWriter,e_pTiXmlElement);
}

namespace ATG
{

//----------------------------------------------------------------------------------
// Name: XMLWriter
// Desc: constructor for the XML writer class.
//----------------------------------------------------------------------------------
XMLWriter::XMLWriter()
    : m_strBuffer( nullptr ),
    m_uBufferSizeRemaining( 0 ),
    m_hFile( INVALID_HANDLE_VALUE ),
    m_strNameStackTop( m_strNameStack ),
    m_uIndentCount( 0 )
{
}


//----------------------------------------------------------------------------------
// Name: XMLWriter
// Desc: constructor for the XML writer class.
//----------------------------------------------------------------------------------
XMLWriter::XMLWriter( const CHAR* strFileName )
{
	m_hFile = 0;
    Initialize( strFileName );
}


//----------------------------------------------------------------------------------
// Name: XMLWriter
// Desc: constructor for the XML writer class.
//----------------------------------------------------------------------------------
XMLWriter::XMLWriter( CHAR* strBuffer, UINT uBufferSize )
{
    Initialize( strBuffer, uBufferSize );
}


//----------------------------------------------------------------------------------
// Name: ~XMLWriter
// Desc: Destructor for the XML writer class.
//----------------------------------------------------------------------------------
XMLWriter::~XMLWriter()
{
    Close();
}


//----------------------------------------------------------------------------------
// Name: Initialize
// Desc: Sets up the XML writer to write to a file.
//----------------------------------------------------------------------------------
void XMLWriter::Initialize( const CHAR* strFileName )
{
    m_strBuffer = new CHAR[WRITE_BUFFER_SIZE];
    m_strBufferStart = m_strBuffer;
    m_uBufferSizeRemaining = WRITE_BUFFER_SIZE;
    m_bOpenTagFinished = TRUE;
    m_bWriteCloseTagIndent = FALSE;
	wchar_t strName[512];
	MultiByteToWideChar( CP_ACP, 0, strFileName, -1, strName, 512 );
    m_hFile = CreateFile( strName, FILE_WRITE_DATA, 0, nullptr, CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, nullptr );
	if( INVALID_HANDLE_VALUE == m_hFile)
	{
		UT::ErrorMsg(strFileName,"Creatf file failed!");
	}
    m_strNameStack[0] = '\0';
    m_strNameStackTop = m_strNameStack;
    m_NameStackPositions.clear();
    SetIndentCount( 4 );
    m_bWriteNewlines = TRUE;
}


//----------------------------------------------------------------------------------
// Name: Initialize
// Desc: Sets up the XML writer to write to a string buffer.
//----------------------------------------------------------------------------------
void XMLWriter::Initialize( CHAR* strBuffer, UINT uBufferSize )
{
    m_strBuffer = strBuffer;
    m_strBufferStart = m_strBuffer;
    m_uBufferSizeRemaining = uBufferSize;
    m_hFile = INVALID_HANDLE_VALUE;
    m_bOpenTagFinished = TRUE;
    m_bWriteCloseTagIndent = FALSE;
    m_strNameStack[0] = '\0';
    m_strNameStackTop = m_strNameStack;
    m_NameStackPositions.clear();
    SetIndentCount( 0 );
    m_bWriteNewlines = FALSE;
}


//----------------------------------------------------------------------------------
// Name: Close
// Desc: Cleans up the output of the XML writing operation.
//----------------------------------------------------------------------------------
void XMLWriter::Close()
{
    if( m_hFile != INVALID_HANDLE_VALUE )
    {
        FlushBufferToFile();
        CloseHandle( m_hFile );
        delete[] m_strBufferStart;
        m_strBufferStart = nullptr;
        m_strBuffer = nullptr;
        m_hFile = INVALID_HANDLE_VALUE;
    }
    if( m_strBuffer != nullptr )
    {
        m_strBuffer = nullptr;
        m_strBufferStart = nullptr;
        m_uBufferSizeRemaining = 0;
    }
}


void XMLWriter::FlushBufferToFile()
{
    if( m_uBufferSizeRemaining >= WRITE_BUFFER_SIZE || m_hFile == INVALID_HANDLE_VALUE )
        return;
    unsigned long dwBytesWritten = 0;
    WriteFile( m_hFile, m_strBufferStart, WRITE_BUFFER_SIZE - m_uBufferSizeRemaining, &dwBytesWritten, nullptr );
    m_uBufferSizeRemaining = WRITE_BUFFER_SIZE;
    m_strBuffer = m_strBufferStart;
}


//----------------------------------------------------------------------------------
// Name: SetIndentCount
// Desc: Builds a string with the correct amount of indentation spaces.
//----------------------------------------------------------------------------------
void XMLWriter::SetIndentCount( UINT uSpaces )
{
    m_uIndentCount = ( uSpaces > 8 ) ? 8 : uSpaces;
    if( m_uIndentCount > 0 )
        memset( m_strIndent, ' ', m_uIndentCount );
    m_strIndent[ m_uIndentCount ] = '\0';
}


//----------------------------------------------------------------------------------
// Name: StartElement
// Desc: Writes the beginning of an XML open tag.
//----------------------------------------------------------------------------------
BOOL XMLWriter::StartElement( const CHAR* strName )
{
    if( !m_bOpenTagFinished )
    {
        if( !EndOpenTag() ) 
            return FALSE;
        if( !WriteNewline() )
            return FALSE;
    }
    BOOL result = TRUE;
    result &= WriteIndent();
    PushName( strName );
    result &= OutputStringFast( "<", 1 );
    result &= OutputString( strName );
    m_bOpenTagFinished = FALSE;
    m_bWriteCloseTagIndent = FALSE;
    return result;
}

BOOL XMLWriter::StartElement( const wchar_t* strName )
{
	return StartElement(UT::WcharToChar(strName).c_str());
}
//----------------------------------------------------------------------------------
// Name: EndElement
// Desc: Writes an element close tag corresponding with the most recent open tag.
//----------------------------------------------------------------------------------
BOOL XMLWriter::EndElement()
{
    const CHAR* strName = PopName();
    if( strName == nullptr )
        return FALSE;
    BOOL result = TRUE;
    if( !m_bOpenTagFinished )
    {
        m_bOpenTagFinished = TRUE;
        result &= OutputStringFast( " />", 3 );
        result &= WriteNewline();
        m_bWriteCloseTagIndent = TRUE;
        return result;
    }
    if( m_bWriteCloseTagIndent )
        result &= WriteIndent();
    result &= OutputStringFast( "</", 2 );
    result &= OutputString( strName );
    result &= OutputStringFast( ">", 1 );
    result &= WriteNewline();
    m_bWriteCloseTagIndent = TRUE;
    return result;
}


//----------------------------------------------------------------------------------
// Name: WriteElement
// Desc: Convenience function to write an XML element with a body and no attributes.
//----------------------------------------------------------------------------------
BOOL XMLWriter::WriteElement( const CHAR* strName, const CHAR* strBody )
{
    BOOL result = TRUE;
    result &= StartElement( strName );
    result &= WriteString( strBody );
    result &= EndElement();
    return result;
}


//----------------------------------------------------------------------------------
// Name: WriteElement
// Desc: Convenience function to write an XML element with a body and no attributes.
//----------------------------------------------------------------------------------
BOOL XMLWriter::WriteElement( const CHAR* strName, INT iBody )
{
    BOOL result = TRUE;
    result &= StartElement( strName );
    CHAR strTemp[32];
    _itoa_s( iBody, strTemp, 10 );
    result &= WriteString( strTemp );
    result &= EndElement();
    return result;
}


//----------------------------------------------------------------------------------
// Name: WriteElement
// Desc: Convenience function to write an XML element with a body and no attributes.
//----------------------------------------------------------------------------------
BOOL XMLWriter::WriteElement( const CHAR* strName, FLOAT fBody )
{
    BOOL result = TRUE;
    result &= StartElement( strName );
    CHAR strTemp[32];
    sprintf_s( strTemp, "%f", fBody );
    result &= WriteString( strTemp );
    result &= EndElement();
    return result;
}


//----------------------------------------------------------------------------------
// Name: WriteElementFormat
// Desc: Convenience function to write an XML element with a body and no attributes.
//----------------------------------------------------------------------------------
BOOL XMLWriter::WriteElementFormat( const CHAR* strName, const CHAR* strFormat, ... )
{
    BOOL result = TRUE;
    result &= StartElement( strName );
    CHAR strTemp[512];
    va_list args;
    va_start( args, strFormat );
    vsprintf_s( strTemp, strFormat, args );
    result &= WriteString( strTemp );
    result &= EndElement();
    return result;
}


//----------------------------------------------------------------------------------
// Name: StartCDATA
// Desc: Starts a CDATA block.
//----------------------------------------------------------------------------------
BOOL XMLWriter::StartCDATA()
{
    BOOL bResult = TRUE;
    if( !m_bOpenTagFinished )
    {
        if( !EndOpenTag() ) 
            return FALSE;
        if( !WriteNewline() )
            return FALSE;
    }
    bResult &= WriteIndent();
    bResult &= OutputStringFast( "<![CDATA[", 9 );
    return bResult;
}


//----------------------------------------------------------------------------------
// Name: EndCDATA
// Desc: Ends a CDATA block.
//----------------------------------------------------------------------------------
BOOL XMLWriter::EndCDATA()
{
    BOOL bResult = TRUE;
    bResult &= OutputStringFast( "]]>", 3 );
    bResult &= WriteNewline();
    m_bWriteCloseTagIndent = TRUE;
    return bResult;
}


//----------------------------------------------------------------------------------
// Name: WriteCDATA
// Desc: Writes a CDATA block.
//----------------------------------------------------------------------------------
BOOL XMLWriter::WriteCDATA( const CHAR* strData, unsigned long dwDataLength )
{
    BOOL bResult = StartCDATA();
    bResult &= OutputStringFast( strData, dwDataLength );
    bResult &= EndCDATA();
    return bResult;
}


//----------------------------------------------------------------------------------
// Name: StartComment
// Desc: Writes the beginning of an XML comment tag.
//----------------------------------------------------------------------------------
BOOL XMLWriter::StartComment( BOOL bInline )
{
    if( !m_bOpenTagFinished )
    {
        if( !EndOpenTag() ) 
            return FALSE;
        if( !bInline && !WriteNewline() )
            return FALSE;
    }
    BOOL result = TRUE;
    if( !bInline )
        result &= WriteIndent();
    result &= OutputStringFast( "<!-- ", 5 );
    m_bOpenTagFinished = TRUE;
    m_bWriteCloseTagIndent = FALSE;
    m_bInlineComment = bInline;
    return result;
}


//----------------------------------------------------------------------------------
// Name: EndComment
// Desc: Writes a comment close tag.
//----------------------------------------------------------------------------------
BOOL XMLWriter::EndComment()
{
    BOOL result = TRUE;
    result &= OutputStringFast( " -->", 4 );
    if( !m_bInlineComment )
        result &= WriteNewline();
    m_bWriteCloseTagIndent = !m_bInlineComment;
    return result;
}


//----------------------------------------------------------------------------------
// Name: WriteComment
// Desc: Convenience function to write an entire comment.
//----------------------------------------------------------------------------------
BOOL XMLWriter::WriteComment( const CHAR* strComment, BOOL bInline )
{
    BOOL result = TRUE;
    result &= StartComment( bInline );
    result &= WriteString( strComment );
    result &= EndComment();
    return result;
}


//----------------------------------------------------------------------------------
// Name: AddAttributeFormat
// Desc: Adds a key-value attribute pair to an XML open tag.  This must be called
//       after calling StartElement(), but before calling WriteString() or
//       EndElement().
//----------------------------------------------------------------------------------
BOOL XMLWriter::AddAttributeFormat( const CHAR* strName, const CHAR* strFormat, ... )
{
    if( m_bOpenTagFinished )
        return FALSE;
    BOOL result = TRUE;
    result &= OutputStringFast( " ", 1 );
    result &= OutputString( strName );
    result &= OutputStringFast( "=\"", 2 );
    CHAR strTemp[256];
    va_list args;
    va_start( args, strFormat );
    vsprintf_s( strTemp, strFormat, args );
    result &= OutputString( strTemp );
    result &= OutputStringFast( "\"", 1 );
    return result;
}

BOOL XMLWriter::AddAttribute( const CHAR* strName, std::string strValue )
{
	return AddAttribute( strName, strValue.c_str() );
}
//----------------------------------------------------------------------------------
// Name: AddAttribute
// Desc: Adds a key-value attribute pair to an XML open tag.  This must be called
//       after calling StartElement(), but before calling WriteString() or
//       EndElement().
//----------------------------------------------------------------------------------
BOOL XMLWriter::AddAttribute( const CHAR* strName, const CHAR* strValue )
{
    if( m_bOpenTagFinished )
        return FALSE;
    BOOL result = TRUE;
    result &= OutputStringFast( " ", 1 );
    result &= OutputString( strName );
    result &= OutputStringFast( "=\"", 2 );
    result &= OutputString( strValue );
    result &= OutputStringFast( "\"", 1 );
    return result;
}

BOOL XMLWriter::AddAttribute( const CHAR* strName, std::wstring	strValue )
{
	return AddAttribute( strName,strValue.c_str() );
}
//----------------------------------------------------------------------------------
// Name: AddAttribute
// Desc: Adds a key-value attribute pair to an XML open tag.  This must be called
//       after calling StartElement(), but before calling WriteString() or
//       EndElement().
//----------------------------------------------------------------------------------
BOOL XMLWriter::AddAttribute( const CHAR* strName, const wchar_t* wstrValue )
{
    CHAR strTemp[4096];
    WideCharToMultiByte( CP_ACP, 0, wstrValue, (INT)wcslen( wstrValue ) + 1, strTemp, 4096, nullptr, nullptr );
    return AddAttribute( strName, strTemp );
}


//----------------------------------------------------------------------------------
// Name: AddAttribute
// Desc: Adds a key-value attribute pair to an XML open tag.  This must be called
//       after calling StartElement(), but before calling WriteString() or
//       EndElement().
//----------------------------------------------------------------------------------
BOOL XMLWriter::AddAttribute( const CHAR* strName, INT iValue )
{
    CHAR strTemp[20];
    _itoa_s( iValue, strTemp, 10 );
    return AddAttribute( strName, strTemp );
}


//----------------------------------------------------------------------------------
// Name: AddAttribute
// Desc: Adds a key-value attribute pair to an XML open tag.  This must be called
//       after calling StartElement(), but before calling WriteString() or
//       EndElement().
//----------------------------------------------------------------------------------
BOOL XMLWriter::AddAttribute( const CHAR* strName, FLOAT fValue )
{
    CHAR strTemp[20];
    sprintf_s( strTemp, "%f", fValue );
    return AddAttribute( strName, strTemp );
}


//----------------------------------------------------------------------------------
// Name: WriteString
// Desc: Writes a string after an XML open tag.
//----------------------------------------------------------------------------------
BOOL XMLWriter::WriteString( const CHAR* strText )
{
    if( strText == nullptr )
        strText = "";
    if( !m_bOpenTagFinished )
    {
        if( !EndOpenTag() )
            return FALSE;
    }
    return OutputString( strText );
}


//----------------------------------------------------------------------------------
// Name: WriteStringFormat
// Desc: Writes a formatted string after an XML open tag.
//----------------------------------------------------------------------------------
BOOL XMLWriter::WriteStringFormat( const CHAR* strFormat, ... )
{
    if( !m_bOpenTagFinished )
    {
        if( !EndOpenTag() )
            return FALSE;
    }
    CHAR strTemp[1024];
    va_list args;
    va_start( args, strFormat );
    vsprintf_s( strTemp, strFormat, args );
    return OutputString( strTemp );
}


//----------------------------------------------------------------------------------
// Name: EndOpenTag
// Desc: Writes the closing angle bracket of an XML open tag, and sets the proper
//       state.
//----------------------------------------------------------------------------------
BOOL XMLWriter::EndOpenTag()
{
    assert( !m_bOpenTagFinished );
    OutputStringFast( ">", 1 );
    m_bOpenTagFinished = TRUE;
    return TRUE;
}


//----------------------------------------------------------------------------------
// Name: WriteNewline
// Desc: Writes a new line, if that option is enabled.  Writes a hard return to
//       files, and a soft return to buffers.
//----------------------------------------------------------------------------------
BOOL XMLWriter::WriteNewline()
{
    if( !m_bWriteNewlines )
        return TRUE;
    if( m_hFile != INVALID_HANDLE_VALUE )
        return OutputStringFast( "\r\n", 2 );
    return OutputStringFast( "\n", 1 );
}


//----------------------------------------------------------------------------------
// Name: WriteIndent
// Desc: Writes an indentation using spaces if indentation is enabled.
//----------------------------------------------------------------------------------
BOOL XMLWriter::WriteIndent()
{
    if( m_uIndentCount == 0 )
        return TRUE;
    for( UINT i = 0; i < m_NameStackPositions.size(); i++ )
    {
        if( !OutputStringFast( m_strIndent, m_uIndentCount ) )
            return FALSE;
    }
    return TRUE;
}


//----------------------------------------------------------------------------------
// Name: PushName
// Desc: Pushes an XML tag name onto the stack.  This is used to write an open tag.
//----------------------------------------------------------------------------------
void XMLWriter::PushName( const CHAR* strName )
{
    UINT uLen = (UINT)strlen( strName );
    if( ( m_strNameStackTop - m_strNameStack + uLen ) >= XMLWRITER_NAME_STACK_SIZE )
    {
        assert( false );
        return;
    }
    m_NameStackPositions.push_back( (UINT)( m_strNameStackTop - m_strNameStack ) );
    auto dwBufSize = ARRAYSIZE( m_strNameStack ) - ( m_strNameStackTop - m_strNameStack );
    strcpy_s( m_strNameStackTop, dwBufSize, strName );
    m_strNameStackTop += uLen;
}


//----------------------------------------------------------------------------------
// Name: PopName
// Desc: Pops an XML tag name off the stack.  This is used to write a close tag.
//----------------------------------------------------------------------------------
const CHAR* XMLWriter::PopName()
{
    if( m_NameStackPositions.size() == 0 )
        return nullptr;
    UINT uPos = m_NameStackPositions.back();
    m_NameStackPositions.pop_back();
    *m_strNameStackTop = '\0';
    m_strNameStackTop = m_strNameStack + uPos;
    return m_strNameStackTop;
}


//----------------------------------------------------------------------------------
// Name: OutputString
// Desc: Sends a null-terminated string to the output.
//----------------------------------------------------------------------------------
BOOL XMLWriter::OutputString( const CHAR* strText )
{
    assert( strText != nullptr );
    return OutputStringFast( strText, (UINT)strlen( strText ) );
}


//----------------------------------------------------------------------------------
// Name: OutputStringFast
// Desc: Sends a string with a supplied length to the output.
//----------------------------------------------------------------------------------
BOOL XMLWriter::OutputStringFast( const CHAR* strText, UINT uLength )
{
    if( m_hFile != INVALID_HANDLE_VALUE )
    {
        while( uLength >= m_uBufferSizeRemaining )
        {
            memcpy( m_strBuffer, strText, m_uBufferSizeRemaining );
            m_strBuffer += m_uBufferSizeRemaining;
            strText += m_uBufferSizeRemaining;
            uLength -= m_uBufferSizeRemaining;
            m_uBufferSizeRemaining = 0;
            FlushBufferToFile();
        }
        memcpy( m_strBuffer, strText, uLength );
        m_uBufferSizeRemaining -= uLength;
        m_strBuffer += uLength;
        return TRUE;
    }
    else if( m_strBuffer != nullptr )
    {
        if( ( uLength + 1 ) > m_uBufferSizeRemaining )
            return FALSE;
        memcpy( m_strBuffer, strText, uLength + 1 );
        m_uBufferSizeRemaining -= uLength;
        m_strBuffer += uLength;
        return TRUE;
    }
    return FALSE;
}

} // namespace ATG

//win32
#endif