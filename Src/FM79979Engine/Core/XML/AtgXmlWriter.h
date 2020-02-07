//-------------------------------------------------------------------------------------
//  AtgXmlWriter.h
//  
//  A simple XML writer.
//  
//  Xbox Advanced Technology Group
//  Copyright ?Microsoft Corporation. All rights reserved.
//-------------------------------------------------------------------------------------
//tinu xml version
//http://www.grinninglizard.com/tinyxmldocs/tutorial0.html
//Writing a document to a file
//
//Writing a pre-built DOM to a file is trivial:
//
//doc.SaveFile( saveFilename );  
//Recall, for example, example 4:
//
//<?xml version="1.0" ?>
//<MyApp>
//    <!-- Settings for MyApp -->
//    <Messages>
//        <Welcome>Welcome to MyApp</Welcome>
//        <Farewell>Thank you for using MyApp</Farewell>
//    </Messages>
//    <Windows>
//        <Window name="MainFrame" x="5" y="15" w="400" h="250" />
//    </Windows>
//    <Connection ip="192.168.0.1" timeout="123.456000" />
//</MyApp>
//The following function builds this DOM and writes the file "appsettings.xml":
//
//void write_app_settings_doc( )  
//{  
//	TiXmlDocument doc;
//	TiXmlElement* msg;
// 	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
//	doc.LinkEndChild( decl );  
// 
//	TiXmlElement * root = new TiXmlElement( "MyApp" );  
//	doc.LinkEndChild( root );  
//
//	TiXmlComment * comment = new TiXmlComment();
//	comment->SetValue(" Settings for MyApp " );  
//	root->LinkEndChild( comment );  
// 
//	TiXmlElement * msgs = new TiXmlElement( "Messages" );  
//	root->LinkEndChild( msgs );  
// 
//	msg = new TiXmlElement( "Welcome" );  
//	msg->LinkEndChild( new TiXmlText( "Welcome to MyApp" ));  
//	msgs->LinkEndChild( msg );  
// 
//	msg = new TiXmlElement( "Farewell" );  
//	msg->LinkEndChild( new TiXmlText( "Thank you for using MyApp" ));  
//	msgs->LinkEndChild( msg );  
// 
//	TiXmlElement * windows = new TiXmlElement( "Windows" );  
//	root->LinkEndChild( windows );  
//
//	TiXmlElement * window;
//	window = new TiXmlElement( "Window" );  
//	windows->LinkEndChild( window );  
//	window->SetAttribute("name", "MainFrame");
//	window->SetAttribute("x", 5);
//	window->SetAttribute("y", 15);
//	window->SetAttribute("w", 400);
//	window->SetAttribute("h", 250);
//
//	TiXmlElement * cxn = new TiXmlElement( "Connection" );  
//	root->LinkEndChild( cxn );  
//	cxn->SetAttribute("ip", "192.168.0.1");
//	cxn->SetDoubleAttribute("timeout", 123.456); // floating point attrib
//	
//	dump_to_stdout( &doc );
//	doc.SaveFile( "appsettings.xml" );  
//} 
#ifdef WIN32

#pragma once
#ifndef ATGXMLWRITER_H
#define ATGXMLWRITER_H

#include <vector>
#ifdef _XBOX360
#include <xtl.h>
#endif

namespace ATG
{

	#define XMLWRITER_NAME_STACK_SIZE 255

	class XMLWriter
	{
	public:
		XMLWriter();
		XMLWriter( CHAR* strBuffer, UINT uBufferSize );
		XMLWriter( const CHAR* strFileName );
		~XMLWriter();

		void Initialize( CHAR* strBuffer, UINT uBufferSize );
		void Initialize( const CHAR* strFileName );
		void Close();

		void SetIndentCount( UINT uSpaces );
		void EnableNewlines( BOOL bWriteNewLines ) { m_bWriteNewlines = bWriteNewLines; }

		BOOL StartElement( const CHAR* strName );
		BOOL StartElement( const wchar_t* strName );
		BOOL EndElement();
		BOOL WriteElement( const CHAR* strName, const CHAR* strBody );
		BOOL WriteElement( const CHAR* strName, INT iBody );
		BOOL WriteElement( const CHAR* strName, FLOAT fBody );
		BOOL WriteElementFormat( const CHAR* strName, const CHAR* strFormat, ... );

		BOOL StartCDATA();
		BOOL EndCDATA();
		BOOL WriteCDATA( const CHAR* strData, unsigned long dwDataLength );

		BOOL StartComment( BOOL bInline = FALSE );
		BOOL EndComment();
		BOOL WriteComment( const CHAR* strComment, BOOL bInline = FALSE );

		BOOL AddAttributeFormat( const CHAR* strName, const CHAR* strFormat, ... );
		BOOL AddAttribute( const CHAR* strName, const CHAR* strValue );
		BOOL AddAttribute( const CHAR* strName, std::string strValue );
		BOOL AddAttribute( const CHAR* strName, const wchar_t* wstrValue );
		BOOL AddAttribute( const CHAR* strName, std::wstring	strValue );
		BOOL AddAttribute( const CHAR* strName, INT iValue );
		BOOL AddAttribute( const CHAR* strName, FLOAT fValue );

		BOOL WriteString( const CHAR* strText );
		BOOL WriteStringFormat( const CHAR* strFormat, ... );

	private:

		void PushName( const CHAR* strName );
		const CHAR* PopName();

		inline BOOL EndOpenTag();
		inline BOOL WriteNewline();
		inline BOOL WriteIndent();

		inline BOOL OutputString( const CHAR* strText );
		inline BOOL OutputStringFast( const CHAR* strText, UINT uLength );
		void FlushBufferToFile();

		HANDLE          m_hFile;
		CHAR*           m_strBuffer;
		CHAR*           m_strBufferStart;
		UINT            m_uBufferSizeRemaining;

		CHAR            m_strNameStack[XMLWRITER_NAME_STACK_SIZE];
		CHAR*           m_strNameStackTop;
		UINT            m_uNameStackSize;
		std::vector<UINT>    m_NameStackPositions;
		UINT            m_uIndentCount;
		CHAR            m_strIndent[9];
		BOOL            m_bWriteNewlines;

		BOOL            m_bOpenTagFinished;
		BOOL            m_bWriteCloseTagIndent;
		BOOL            m_bInlineComment;
	};

} // namespace ATG

class TiXmlElement;
void	SaveTinyXMLToFile(char*e_strFileName,TiXmlElement*e_pTiXmlElement);
#endif
//win32
#endif