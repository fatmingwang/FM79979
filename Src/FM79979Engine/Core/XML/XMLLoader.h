#ifndef XMLLOADER_H
#define XMLLOADER_H
#include <map>
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
//=============================================
//for iphone xml chinese parse
//http://stackoverflow.com/questions/2041238/xml-parsing-problem-in-iphone
//<?xml version="1.0" encoding="gb2312"?>
//NSString stringWithContentsOfURL:@"http://..."
//                         encoding:CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_2312_80)
//                            error:nullptr];
// ()parser:(NSXMLParser *)parser parseErrorOccurred:(NSError *)parseError {
//    NSString * errorString = [NSString stringWithFormat:@"Unable to download story feed from web site (Error code %i )", [parseError code]];
//    NSLog(@"error parsing XML: %@", errorString);
//
//    UIAlertView * errorAlert = [[UIAlertView alloc] initWithTitle:@"Error loading content" message:errorString delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil];
//    [errorAlert show];
//http://www.iphonesdkarticles.com/2008/11/localizing-iphone-apps-part-1.html
//NSLocale *currentUsersLocale = [NSLocale currentLocale];
//NSLog(@"Current Locale: %@", [currentUsersLocale localeIdentifier]);
// 
////Output
//Current Locale: en_US
//}
//
//
//
//PARSE_CURRENT_ELEMENT_START
//	COMPARE_NAME("")
//	{
//
//	}
//	else
//	COMPARE_NAME("")
//	{

//	}
//PARSE_NAME_VALUE_END
//
//
//
#include "tinyxml.h"
namespace FATMING_CORE
{
	//here should using # instead
	#define	COMPARE_VALUE_WITH_DEFINE(x)							if( !wcscmp(l_strValue, x) )
	#define	COMPARE_NAME_WITH_DEFINE(x)								if( !wcscmp(l_strName, x) )
	#define	COMPARE_VALUE(x)										if( !wcscmp(l_strValue,L## x) )
	#define	COMPARE_NAME(x)											if( !wcscmp(l_strName,L## x) )

	#define	COMPARE_NOT_SUPPORT(value,FunctionName)					else{std::wstring	l_strFunctionNameAndValue = FunctionName;l_strFunctionNameAndValue += value;UT::ErrorMsg(l_strFunctionNameAndValue.c_str(),L"not support format") ;	}
	//
	//is strcmp should instead by strcmpi?
	#define	COMPARE_ELEMENT_VALUE(str)								if( !wcscmp(m_pCurrentTiXmlElement->Value(),L## str) )
	#define	COMPARE_TARGET_ELEMENT_VALUE(element,str)				if( !wcscmp(element->Value(),L## str) )

	#define	COMPARE_ELEMENT_VALUE_WITH_DEFINE(str)					if( !wcscmp(m_pCurrentTiXmlElement->Value(),str) )
	#define	COMPARE_TARGET_ELEMENT_VALUE_WITH_DEFINE(element,str)	if( !wcscmp(element->Value(),str) )

	#define	ASSERT_ELEMENT_VALUE(str)	assert(!wcscmp(m_pCurrentTiXmlElement->Value(),L## str));
	#define	ASSERT_TARGET_ELEMENT_VALUE(element,str)	assert(!wcscmp(element->Value(),L## str) );

	#define	ASSERT_ELEMENT_VALUE_WITH_DEFINE(str)					assert( !wcscmp(m_pCurrentTiXmlElement->Value(),str) );
	#define	ASSERT_TARGET_ELEMENT_VALUE_WITH_DEFINE(element,str)	assert( !wcscmp(element->Value(),str) );

	//
	#define PARSE_CURRENT_ELEMENT_START	TiXmlAttribute*	l_pAttribute = m_pCurrentTiXmlElement->FirstAttribute();	\
									const wchar_t*	l_strValue = 0;												\
									const wchar_t*	l_strName = 0;												\
									while(l_pAttribute)															\
									{																			\
										l_strValue = l_pAttribute->Value();										\
										l_strName = l_pAttribute->Name();

	#define PARSE_ELEMENT_START(p)	TiXmlAttribute*	l_pAttribute = p->FirstAttribute();	\
									const wchar_t*	l_strValue = 0;												\
									const wchar_t*	l_strName = 0;												\
									while(l_pAttribute)															\
									{																			\
										l_strValue = l_pAttribute->Value();										\
										l_strName = l_pAttribute->Name();

	//
	#define PARSE_NAME_VALUE_END		else{UT::ErrorMsg((wchar_t*)l_strName,L"unsupport Attribute");}l_pAttribute = l_pAttribute->Next();}
	//go to next without query
	#define TO_NEXT_VALUE				l_pAttribute = l_pAttribute->Next();}

	#define	VALUE_TO_INT							_wtoi(l_strValue)
	#define	VALUE_TO_FLOAT							(float)_wtof(l_strValue)
	#define	VALUE_TO_BOOLEAN						VALUE_TO_INT?true:false
	#define	VALUE_TO_POINT  						GetPoint(l_strValue)
	#define	VALUE_TO_VECTOR2						GetVector2(l_strValue)
	#define	VALUE_TO_VECTOR3						GetVector3(l_strValue)
	#define	VALUE_TO_VECTOR4						GetVector4(l_strValue)

	#define	PARSE_FAILED_MESSAGE_BOX(p,FileName)	if(!p->Parse(FileName)){ UT::ErrorMsg(FileName,"parse failed"); }

	#define	ATTRIBUT_VALUE(Element,Name) Element->Attribute(L## Name)
	#define	ATTRIBUT_VALUE_WITH_DEFINE(Element,Name) Element->Attribute(Name)

	#define	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(Element)Element = Element->FirstChildElement();while(Element){
	#define FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(Element) Element = Element->NextSiblingElement();}


	inline	float	ElementToFloat(TiXmlElement*e_pElement,const WCHAR*e_strAttributeName)
	{
		const wchar_t*l_strAttribute = e_pElement->Attribute(e_strAttributeName);
		if( l_strAttribute )return GetFloat(l_strAttribute);
		UT::ErrorMsg(e_strAttributeName,L"this attribute is not exists!");
		return -1.f;
	}


#ifdef DEBUG
#define	ELEMENT_VALUE_ASSERT_CHECK(Element,CompareValue){const wchar_t*l_strValue__79979 = Element->Value();if( wcscmp(CompareValue,l_strValue__79979) )assert(0&&"element value is not match");}
#else
#define	ELEMENT_VALUE_ASSERT_CHECK(Element,Value)
#endif

	//#include "../Utility.h"
	#ifdef WIN32
	//#include "../../../Include/iconv/iconv.h"
	#else
	//#include "iconv.h"
	#endif
	void    XMLHandleElementDataDebugInfo (TiXmlElement*e_pTiXmlElement);
	//u could set it into xml,so it sould be called in each element is loaded
	typedef void    (*LoadingProgress)( );
	//
	extern LoadingProgress	g_pLoadingProgressCallBack;
	//=============
	//parse and loading data class
	//=============
#ifndef DEFINE_FILE_EXTENSION_NAME_INFO
#define DEFINE_FILE_EXTENSION_NAME_INFO() \
    public: \
	virtual const char*        ExtensionName() const { return ExtensionNameID; }\
	static const  char*        ExtensionNameID;
#endif
	TiXmlElement	*FindElementByName( TiXmlElement*e_pCallbackFrame ,void* e_pcallbackPointer );
	//=============
	//get whole data into sXMLNode by ParseDataIntoXMLNode
	//=============
	TiXmlElement*	SearchTiXmlElement(TiXmlElement* e_pNode,void* e_pPointer,TiXmlElement*(*callback)( TiXmlElement*e_pCallbackFrame ,void* e_pcallbackPointer ) );
	void			SearchTiXmlElementAndSaveIt(TiXmlElement* e_pNode,std::vector<TiXmlElement*>*e_pAllMatchedElementVector,void* e_pPointer,TiXmlElement*(*callback)( TiXmlElement*e_pCallbackFrame ,void* e_pcallbackPointer ) );
    //fuck only works for windows
    #define EXTENSION_DEFINE_MARCO(TYPE,MyExtensionName) const char*##TYPE::ExtensionNameID( #MyExtensionName );
    //locale = setlocale(LC_ALL, "");
    //make ansi charater to unicode??
	class ISAXCallback
	{
	protected:
	    GET_SET_DEC(TiXmlDocument*,m_pDoc,GetDoc,SetDoc);
		//parse xml file and return a root element
		TiXmlElement*	LoadingXML(const char*e_strFileName);

		bool	m_bShowLoadingState;
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement){}
		//do some resource organsize or data arrange if Parse is called
		virtual	void	InternalParse(){}
	protected:
		std::wstring	m_strErrorMsg;	
		char m_strCurrentDirectory[TEMP_SIZE];
		char m_strFileName[TEMP_SIZE];
		//if true,the xml parser will go as
		//1->2->3->4->5->6->
		//6->5->4->3->2->1
		bool	m_bSetRunBack;
	protected:
		std::string	StringAddDirectory(std::string e_strValue);
		std::string	StringAddDirectory(char*e_strValue);
		std::string	StringAddDirectory(const char*e_strValue);
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
		//	std::vector<wchar_t*>	m_OldObjectNameVector;
		TiXmlElement*	ParserAllElement(TiXmlElement*e_pTiXmlElement);
	public:
		//if true the class implement ISAXCallback could decide the error message show or not
		bool	m_bShowErrorMsg;
		//DEFINE_TYPE_INFO();
		DEFINE_FILE_EXTENSION_NAME_INFO();
		ISAXCallback(bool e_bSetRunBack = false);
		~ISAXCallback(){SAFE_DELETE(m_pDoc);}
		//if e_bRemoveOldResource is true,u have to implement InternalParse to remove the resource u don't want keep
		virtual	bool	Parse(const char*e_strFileName,bool e_bShowLoadingState = false);
		virtual	bool	Parse(std::string e_strFileName,bool e_bShowLoadingState = false);
		virtual	bool	Parse(const wchar_t*e_strFileName,bool e_bShowLoadingState = false);
		virtual	bool	ParseText(const char*e_strFileName);
		char*	GetFileName(){ return m_strFileName; }
		bool	HasErrorMessage(){ if( wcslen(m_strErrorMsg.c_str()) )return true;return false; }
		const   wchar_t*GetErrorMsg(){ return m_strErrorMsg.c_str(); }
		bool	Export(const char*e_strFileName,bool e_bBinary = false);
		bool	Export(const wchar_t*e_strFileName,bool e_bBinary = false);
	};
	TiXmlElement*	GetXmlElementByNameFromElement( const wchar_t*e_pString,TiXmlElement*e_pTiXmlElement );
	//cXMLloader	l_l("C:/Documents and Settings/Fatming/A?≮∮/MT_Tank/tinyxml_2_5_3/tinyxml/Fire.fat");
	//cTestXMLSAX	l_cTestXMLSAX;
	//l_l.ParseDataIntoXMLNode(&l_cTestXMLSAX);

	//cNodeISAX	l_NodeISAX;
	//bool	l_b = l_NodeISAX.ParseDataIntoXMLNode(e_strEffectSetupData);
	//if( l_b )
	//{
	//	TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
	//	const wchar_t*l_strMaximumObject = l_pElement->Attribute(L"MaximumObject");
	//	const wchar_t*l_strDistanceToDisappear = l_pElement->Attribute(L"DistanceToDisappear");
	//	const wchar_t*l_strMPDIFileName = l_pElement->Attribute(L"MPDIFileName");
	//	if( l_strMaximumObject )
	//	{
	//	
	//	}
	//	if( l_strDistanceToDisappear )
	//	{
	//	
	//	}
	//	if( l_strMPDIFileName )
	//	{
	//	
	//	}
	//}
	//this one is not parse recursively,you have to find the element that u want,and analyize it to generate the data u want
	//internal call MyParse,this one do not support parse animation(e_bShowLoadingState)
	class cNodeISAX:public ISAXCallback
	{
		//find need element and parse element not recursively!
		//you should overwrite this if u call ParseWithMyParse
		virtual	bool	MyParse(TiXmlElement*e_pRoot){return true;}
		//if GetRootElement is failed ensure ParseDataIntoXMLNode is called
		GET_SET_DEC(TiXmlElement*,m_pRootElement,GetRootElement,SetRootElement);
	public:
	    //DEFINE_TYPE_INFO();
		cNodeISAX();
		virtual ~cNodeISAX();
		//call this will store m_pTiXmlDoc,so we could use it.
		virtual	bool	ParseDataIntoXMLNode(const char*e_strText);
		virtual	bool	ParseDataIntoXMLNode(const wchar_t*e_strText);
		virtual	bool	ParseTextIntoXMLNode(const char*e_str);
		virtual	bool	ParseWithMyParse(const char*e_strFileName,bool e_bDeleteXLDocument = true);
		virtual	bool	ParseTextWithMyParse(const char*e_strFileName,bool e_bDeleteXLDocument = true);
		TiXmlElement*	GetXmlElementByNameFromRoot( wchar_t*e_pString ){ return SearchTiXmlElement(m_pRootElement,(void*)e_pString,FindElementByName);  }
		//only get root's child
		TiXmlElement*	GetChildOfRootXmlElementByName( const wchar_t*e_pString );
		TiXmlElement*	GetChildOfRootXmlElementByAttribueName(const  wchar_t*e_pElementName,const wchar_t*e_strAttributeKeyWord,const wchar_t*e_strAttributeName=L"Name" );
		TiXmlElement*	GetRootChildElementByIndex( int e_iIndex );
		int				RootChildrenCount();
		//if element's child contain same element name it will crush.
		static bool		ReplaceNodeDataByTemplateNode(const char*e_strTargetFileName,const char*e_strTemplateFileName,const char*e_strExportName);
	};

		//const char*l_strFulscreen = l_pTiXmlElement->Attribute("FullScreen");
		//iconv_t cd = libiconv_open("wchar_t", "UTF-8");
	 //   iconv(cd, 0, 0, 0, 0);
		//wchar_t	l_str[260];
		//memset(l_str,0,sizeof(wchar_t)*260);
	 //   char *outptr = (char*)l_str;
	 //   size_t outsize = 260 * sizeof(wchar_t);
	 //   char *inptr = (char*)l_strFileName;
	 //   size_t insize = strlen(l_strFileName);
	 //   size_t res = iconv(cd, (char**) & inptr, &insize, &outptr, &outsize);
		//MessageBox(0,L"",l_str,MB_OK);
		//libiconv_close(cd);


	#ifdef IOS
	//because iphone using bundle resource,so the resource store in its specific path.
	void GetAppleBundelResourcePathByObjectPath(const char*e_strSrc,char*e_strDest);
	void GetIphoneAppFilePath(char* o_buffer, char* filename);
	//http://svn.openscenegraph.org/osg/OpenSceneGraph/tags/OpenSceneGraph-2.3.8/src/osgDB/FileUtils.cpp
    // returns an absolute (POSIX on MacOS X) path from a CFURLRef
    std::string GetPathFromCFURLRef(CFURLRef urlRef)
    {
        char buffer[1024];
        std::string path;
        if(CFURLGetFileSystemRepresentation(urlRef, true, (UInt8*)buffer, 1024))
            path = std::string(buffer);
        return path;
    }

    // returns the absolute path to the main bundle
    std::string GetApplicationBundlePath(CFBundleRef mainBundle)
    {
        std::string path;
        CFURLRef urlRef = CFBundleCopyBundleURL(mainBundle);
        if(urlRef)
        {
            path = GetPathFromCFURLRef(urlRef);
            CFRelease(urlRef); // docs say we are responsible for releasing CFURLRef
        }
        return path;
        
    }
	#endif
}
#endif