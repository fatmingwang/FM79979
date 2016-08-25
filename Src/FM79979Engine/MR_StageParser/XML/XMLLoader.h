#ifndef XMLLOADER_H
#define XMLLOADER_H

#ifdef TIXML_USE_STL
	#include <iostream>
	#include <sstream>
	using namespace std;
#else
	#include <stdio.h>
#endif

#if defined( WIN32 ) && defined( TUNE )
	#include <crtdbg.h>
	_CrtMemState startMemState;
	_CrtMemState endMemState;
#endif

#include "tinyxml.h"

//
#define	COMCARP_STRING(x)		if( !wcscmp(l_strName,x) )
//
//is strcmp should instead by strcmpi?
#define	COMPARE_ELEMENTVALUE(str)	if( !strcmp(m_pCurrentTiXmlElement->Value(),str) )
//
#define PARSE_NAME_VALUE_START	TiXmlAttribute*	l_pAttribute = m_pCurrentTiXmlElement->FirstAttribute();	\
								const WCHAR*	l_strValue = 0;													\
								const WCHAR*	l_strName = 0;													\
								while(l_pAttribute)															\
								{																			\
									l_strValue = l_pAttribute->Value();										\
									l_strName = l_pAttribute->Name();
//
#define PARSE_NAME_VALUE_END	else{UT::ErrorMsg((WCHAR*)l_strName,L"unsupport Attribute");}l_pAttribute = l_pAttribute->Next();}
//go to next without query
#define TO_NEXT_VALUE			l_pAttribute = l_pAttribute->Next();}

#include "../Utility.h"
#ifdef WIN32
#include "../../../Include/iconv/iconv.h"
#else
#include "iconv.h"
#endif
void    XMLHandleElementDataDebugInfo (TiXmlElement*e_pTiXmlElement);
//u could set it into xml,so it sould be called in each element is loaded
typedef void    (*LoadingProgress)( );
//
extern LoadingProgress	g_pLoadingProgressCallBack;
//=============
//parse and loading data class
//=============
class ISAXCallback
{
	bool	m_bShowLoadingState;
	virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement){};
protected:
	char m_strCurrentDirectory[TEMP_SIZE];
	//if true,the xml parser will go as
	//1->2->3->4->5->6->
	//6->5->4->3->2->1
	bool	m_bSetRunBack;
protected:
	char*	StringAddDirectory(char*e_strValue);
	char*	StringAddDirectory(const char*e_strValue);
	//get current xml element
	TiXmlElement*	m_pCurrentTiXmlElement;
//================
//further goal,automatic to kill and replace data or else from old name vector
//================
	//kill all old imageresource
	//#define KILL_OLD_IMAGE				
	//don't kill old image resource,if same name don't loading new image resource
	//#define KEEP_AND_MERGE_OLD_IMAGE
	//don't kill old image resource,if same name loading new image resource
	//#define KEEP_AND_REPLACE_OLD_IMAGE
	//idea for store old object name list to compare new list and old list to kill or don't loading same object
	//protected:
	//	std::vector<WCHAR*>	m_OldObjectNameVector;
	TiXmlElement*	ParserAllElement(TiXmlElement*e_pTiXmlElement);
public:
	ISAXCallback(bool e_bSetRunBack = false) { m_pCurrentTiXmlElement = 0; m_strCurrentDirectory[0] = '\0';  m_bSetRunBack = e_bSetRunBack; }
    ~ISAXCallback() { }
	virtual	bool	Parse(char*e_strFileName,bool e_bShowLoadingState = false);
	
};

//cXMLloader	l_l("C:/Documents and Settings/Fatming/Æ‡≠±/MT_Tank/tinyxml_2_5_3/tinyxml/Fire.fat");
//cTestXMLSAX	l_cTestXMLSAX;
//l_l.ParseDataIntoXMLNode(&l_cTestXMLSAX);

//=============
//get whole data into sXMLNode by ParseDataIntoXMLNode
//=============
TiXmlElement*	SearchTiXmlElement(TiXmlElement* e_pNode,void* e_pPointer,TiXmlElement*(*callback)( TiXmlElement*e_pCallbackFrame ,void* e_pcallbackPointer ) );


TiXmlElement	*FindElementByName( TiXmlElement*e_pCallbackFrame ,void* e_pcallbackPointer );

	//cNodeISAX	*l_pNodeISAX = new cNodeISAX();
	//bool	l_b = l_pNodeISAX->ParseDataIntoXMLNode("Media/Resolution.fat");
class cNodeISAX:public ISAXCallback
{
	GET_SET_DEC(TiXmlElement*,m_pRootElement,GetRootElement,SetRootElement);
	GET_SET_DEC(TiXmlDocument*,m_pTiXmlDoc,GetTiXmlDoc,SetXmlDoc);
public:
	//call this will store m_pTiXmlDoc,so we could use it.
	virtual	bool	ParseDataIntoXMLNode(char*e_strFileName);
	~cNodeISAX(){ SAFE_DELETE(m_pTiXmlDoc); }

	TiXmlElement*	GetXmlElementByNameFromRoot( char*e_pString ){ return SearchTiXmlElement(m_pRootElement,(void*)e_pString,FindElementByName);  }
	//only get root's child
	TiXmlElement*	GetChildOfRootXmlElementByName( char*e_pString );
	TiXmlElement*	GetXmlElementByNameFromElement( char*e_pString,TiXmlElement*e_pTiXmlElement ){ return SearchTiXmlElement(e_pTiXmlElement,(void*)e_pString,FindElementByName); }
	TiXmlElement*	GetXmlElementByNameFromElement( WCHAR*e_pString,TiXmlElement*e_pTiXmlElement )
	{
		char	l_strName[MAX_PATH];
		UT::WcharToChar(e_pString,l_strName);
		return GetXmlElementByNameFromElement(l_strName,e_pTiXmlElement);
	}
};

	//const char*l_strFulscreen = l_pTiXmlElement->Attribute("FullScreen");
	//iconv_t cd = libiconv_open("wchar_t", "UTF-8");
 //   iconv(cd, 0, 0, 0, 0);
	//WCHAR	l_str[260];
	//memset(l_str,0,sizeof(WCHAR)*260);
 //   char *outptr = (char*)l_str;
 //   size_t outsize = 260 * sizeof(wchar_t);
 //   char *inptr = (char*)l_strFileName;
 //   size_t insize = strlen(l_strFileName);
 //   size_t res = iconv(cd, (char**) & inptr, &insize, &outptr, &outsize);
	//MessageBox(0,L"",l_str,MB_OK);
	//libiconv_close(cd);

//===================
//the rule here,
//one element only could one attribute or occor error maybe.
//ex:<ControrLineCount Pos="50,200"/>
//===================
class cPositionParser:public ISAXCallback
{
	cObjectListByName<NamedTypedObject*>	m_AllObject;	//this is connect with below AllPositionInfo
	std::map<NamedTypedObject*,Vector3>	m_AllPositionInfo;
	virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
public:
	cPositionParser(void);
	~cPositionParser(void);
	Vector3	*GetPositionByName(WCHAR*e_strPosition);
	Vector3	*GetPositionByName(NamedTypedObject*);
	void	Clear(){ m_AllObject.Clear(); }
};


#ifdef __IPHONE__
//because iphone using bundle resource,so the resource store in its specific path.
void GetAppleBundelResourcePathByObjectPath(char*e_strSrc,char*e_strDest);
#endif
#endif