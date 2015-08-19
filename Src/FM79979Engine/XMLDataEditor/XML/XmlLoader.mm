#include "../stdafx.h"
#include "XMLLoader.h"
#include "../Utility.h"
#include "../Image/GlyphFontRender.h"
#ifdef __IPHONE__
#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIImage.h>
#endif
LoadingProgress	g_pLoadingProgressCallBack = 0;

#ifdef __IPHONE__
//because iphone using bundle resource,so the resource store in its specific path.
void GetAppleBundelResourcePathByObjectPath(char*e_strSrc,char*e_strDest)
{
	char	l_strFileName[MAX_PATH];
	sprintf(l_strFileName,"%s\0",UT::GetFileNameWithoutFullPath(e_strSrc,false));
	char	l_strDirectory[MAX_PATH];
	sprintf(l_strDirectory,"%s\0",UT::GetDirectoryWithoutFileName(e_strSrc));
	NSString*l_strName = [[NSString alloc]initWithUTF8String:l_strFileName];
	NSString*l_NSDirectory = [[NSString alloc]initWithUTF8String:l_strDirectory];
	NSString* l_str = [[NSBundle mainBundle] pathForResource:l_strName ofType:nil inDirectory:l_NSDirectory ];
	const char* datechar = [l_str UTF8String];
	assert(e_strDest);
	sprintf(e_strDest,"%s\0",datechar);
}
#endif

char*	ISAXCallback::StringAddDirectory(const char*e_strValue)
{
	return	StringAddDirectory((char*)e_strValue);
}

char*	ISAXCallback::StringAddDirectory(char*e_strValue)
{
	static char	l_strValue[TEMP_SIZE];
	sprintf(l_strValue,"%s%s\0",m_strCurrentDirectory,e_strValue);
	return l_strValue;
}

bool    ISAXCallback::Parse(char*e_strFileName,bool e_bShowLoadingState)
{
		m_bShowLoadingState = e_bShowLoadingState;
#ifdef __IPHONE__
		char	l_strFileName[MAX_PATH];
		sprintf(l_strFileName,"%s\0",UT::GetFileNameWithoutFullPath(e_strFileName,false));
		char	l_strDirectory[MAX_PATH];
		sprintf(l_strDirectory,"%s\0",UT::GetDirectoryWithoutFileName(e_strFileName));
		NSString*l_strName = [[NSString alloc]initWithUTF8String:l_strFileName];
		NSString*l_NSDirectory = [[NSString alloc]initWithUTF8String:l_strDirectory];
		NSString* l_str = [[NSBundle mainBundle] pathForResource:l_strName ofType:nil inDirectory:l_NSDirectory ];
		const char* datechar = [l_str UTF8String];
		TiXmlDocument doc( datechar );
		[l_strName release];
		[l_NSDirectory release];
#else
		TiXmlDocument doc( e_strFileName );
#endif
        bool loadOkay = doc.LoadFile();
        if(!loadOkay)
		{
#ifdef WIN32
			OutputDebugString(L"encode error or file not exist or xml format error??");
#endif
                return false;
		}
		TiXmlNode* l_pFirstNode = doc.FirstChild();
        TiXmlElement* itemElement = 0;
        itemElement = l_pFirstNode->ToElement();
		while( !itemElement )
		{
			itemElement = l_pFirstNode->NextSibling()->ToElement();
			l_pFirstNode = l_pFirstNode->NextSibling();
			if( !l_pFirstNode )
				return false;
		}
        assert(itemElement&&"empty element,check name for none numeral");
//iphone does't need to translate it because all file have to translate by loader api...
        char*   l_pDirectory =  GetDirectoryWithoutFileName(e_strFileName);
		if( l_pDirectory )
			sprintf(m_strCurrentDirectory,"%s\0",l_pDirectory);
#ifdef WIN32
#ifdef _DEBUG
//	OutputDebugString(UT::CharToWchar(e_strFileName));
//	OutputDebugString(L"	Parsing Start\n");
#endif
#endif
        ParserAllElement(itemElement);
        return true;
}


void    XMLHandleElementDataDebugInfo(TiXmlElement*e_pTiXmlElement)
{
#ifdef WIN32
#ifdef _DEBUG
        const char*     l_str = e_pTiXmlElement->Value();
        TiXmlAttribute*l_pAttribute = e_pTiXmlElement->FirstAttribute();
        if(l_pAttribute)
        {
                const char*     l_Test;
                const char*     l_strAttribute = 0;
                while( l_pAttribute )
                {
                        l_Test = l_pAttribute->Value();
                        l_strAttribute = l_pAttribute->Name();
                        if(l_Test)
                        {
//                                OutputDebugString(L">>");
//                                OutputDebugString(UT::CharToWchar(l_strAttribute));
                        }
                        l_pAttribute = l_pAttribute->Next();
                }
        }
//        OutputDebugString(L"\n");
#endif
#endif
}

TiXmlElement*   g_pTiXmlElement;
TiXmlElement*   ISAXCallback::ParserAllElement(TiXmlElement*e_pTiXmlElement)
{
		if(this->m_bShowLoadingState&&g_pDebugFont)
		{
			if(g_pLoadingProgressCallBack)
				g_pLoadingProgressCallBack();
		}
#ifdef WIN32
#ifdef _DEBUG
        g_pTiXmlElement = e_pTiXmlElement;
        while(g_pTiXmlElement->Parent())
        {
                g_pTiXmlElement = (TiXmlElement*)g_pTiXmlElement->Parent();
//                OutputDebugString(L"    ");
        }
//        OutputDebugString(UT::CharToWchar(e_pTiXmlElement->Value()));
		XMLHandleElementDataDebugInfo(e_pTiXmlElement);
#endif
#endif
        //1st all chilren       element
        //2nd all nextslibing   element
        m_pCurrentTiXmlElement = e_pTiXmlElement;
        HandleElementData(e_pTiXmlElement);
        if( e_pTiXmlElement->FirstChildElement() )
        {
                ParserAllElement( e_pTiXmlElement->FirstChildElement());
        }
		//go back
		m_pCurrentTiXmlElement = e_pTiXmlElement;
		if( m_bSetRunBack )
		{
			if( !e_pTiXmlElement->m_bDone )
			{
#ifdef WIN32
#ifdef _DEBUG
        g_pTiXmlElement = e_pTiXmlElement;
        while(g_pTiXmlElement->Parent())
        {
                g_pTiXmlElement = (TiXmlElement*)g_pTiXmlElement->Parent();
//                OutputDebugString(L"    ");
        }
//        OutputDebugString(UT::CharToWchar(e_pTiXmlElement->Value()));
//		OutputDebugString(L"__End");
//		OutputDebugString(L"\n");
#endif
#endif
				e_pTiXmlElement->m_bDone = true;
				HandleElementData(e_pTiXmlElement);
			}
		}
        if( e_pTiXmlElement->NextSiblingElement() )
        {
                ParserAllElement( e_pTiXmlElement->NextSiblingElement());  
        }
        return e_pTiXmlElement;
}

//int	g_iLevel = 0;
//void	sXMLNode::ShowDebugInfo(sXMLNode *e_XMLNode)
//{
//#ifdef	WIN32
//	for( int i=0;i<g_iLevel;++i )
//		OutputDebugString(L"	");
//	OutputDebugString(L"TagName:");
//	OutputDebugString(UT::CharToWchar(e_XMLNode->strTagName));
//	OutputDebugString(L">>");
//	for( UINT i=0;i<e_XMLNode->AttributeList.size();++i )
//	{
//		OutputDebugString(UT::CharToWchar(e_XMLNode->AttributeList[i].strAttribute));
//		OutputDebugString(L">>");
//	}
//	OutputDebugString(L"\n");
//	for( UINT i=0;i<e_XMLNode->ChildNodeList.size();++i )
//		ShowDebugInfo(e_XMLNode->ChildNodeList[i]);
//	for( UINT i=0;i<e_XMLNode->SiblingNodeList.size();++i )
//		ShowDebugInfo(e_XMLNode->SiblingNodeList[i]);
//#else
//	printf(L"TagName:");
//	printf(UT::CharToWchar(e_XMLNode.strTagName.c_str()));
//	for( UINT i=0;i<e_XMLNode.AttributeList.size();++i )
//	{
//		printf(L"Attribute:");
//		printf(UT::CharToWchar(e_XMLNode.AttributeList[i].strAttribute.c_str()));
//		OutputDebugString(L",");
//	}
//	printf(L"\n");
//	for( UINT i=0;i<e_XMLNode.ChildNodeList.size();++i )
//		ShowDebugInfo(e_XMLNode.ChildNodeList[i]);
//	for( UINT i=0;i<e_XMLNode.SiblingNodeList.size();++i )
//		ShowDebugInfo(e_XMLNode.SiblingNodeList[i]);	
//#endif
//}

TiXmlElement*	SearchTiXmlElement(TiXmlElement* e_pNode,void* e_pPointer,TiXmlElement*(*callback)( TiXmlElement*e_pCallbackFrame ,void* e_pcallbackPointer ) )
{
	TiXmlElement*	l_pTiXmlElement = (*callback)(e_pNode,e_pPointer);
	if( l_pTiXmlElement )
		return l_pTiXmlElement;
	if( e_pNode->NextSiblingElement() != NULL)
	{		
		return SearchTiXmlElement( e_pNode->NextSiblingElement(),e_pPointer,callback);
	}
	if (e_pNode->FirstChildElement() != NULL)
	{
		return SearchTiXmlElement( e_pNode->FirstChildElement(),e_pPointer,callback);
	}

	return 0;
}

TiXmlElement	*FindElementByName( TiXmlElement*e_pCallbackFrame ,void* e_pcallbackPointer )
{
	char*	e_strName = (char*)e_pcallbackPointer;
	const char*l_strNodeNAme = e_pCallbackFrame->Value();
	if( !strcmp(l_strNodeNAme,e_strName) )
		return e_pCallbackFrame;
	return 0;
}

bool	cNodeISAX::ParseDataIntoXMLNode(char*e_strFileName)
{
#ifdef __IPHONE__
	char	l_strFileName[MAX_PATH];
	GetAppleBundelResourcePathByObjectPath(e_strFileName,l_strFileName);
    TiXmlDocument *l_pDoc = new TiXmlDocument(l_strFileName);
#else
	TiXmlDocument *l_pDoc = new TiXmlDocument(e_strFileName);
#endif
    bool loadOkay = l_pDoc->LoadFile();
    if(!loadOkay)
	{
		ErrorMsg(L"Open file falied",UT::CharToWchar(e_strFileName));
        return false;
	}
//iphone does't need to translate it because all file have to translate by loader api...
    char*   l_pDirectory =  GetDirectoryWithoutFileName(e_strFileName);
	if( l_pDirectory )
		sprintf(m_strCurrentDirectory,"%s\0",l_pDirectory);
    m_pTiXmlDoc = l_pDoc;
    TiXmlNode* node = 0;
    node  = m_pTiXmlDoc->FirstChild();
    m_pRootElement = node->ToElement();
	if( !m_pRootElement )
		m_pRootElement = m_pTiXmlDoc->FirstChild()->NextSibling()->ToElement();
    assert(m_pRootElement&&"empty element,check name for none numeral");
	return true;
}

TiXmlElement*	cNodeISAX::GetChildOfRootXmlElementByName( char*e_pString )
{
	TiXmlElement*l_pTiXmlElement = m_pRootElement->FirstChild()->ToElement();
	while(l_pTiXmlElement)
	{
		if(!strcmp(l_pTiXmlElement->Value(),e_pString))
		{
			return l_pTiXmlElement;
		}
		l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
	}
	return 0;
}

cPositionParser::cPositionParser(void)
{
}

cPositionParser::~cPositionParser(void)
{
}

//<ControrLineCount Pos="50,200"/>
void	cPositionParser::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	if( e_pTiXmlElement->FirstAttribute() )
	{
		const char*	l_strElementName = e_pTiXmlElement->Value();
		const char*	l_strPos = e_pTiXmlElement->FirstAttribute()->Value();
		NamedTypedObject*l_pCurrentObject = m_AllObject.GetObject(UT::CharToWchar(l_strPos));
		Vector3	l_vPos = GetVector3((char*)l_strPos);
		if( l_pCurrentObject )
		{
			Vector3*l_pvPos =&m_AllPositionInfo[l_pCurrentObject];
			*l_pvPos = l_vPos;
		}
		else
		{
			NamedTypedObject	*l_pNamedTypedObject = new NamedTypedObject;
			l_pNamedTypedObject->SetName(UT::CharToWchar(l_strElementName));
			m_AllObject.AddObject(l_pNamedTypedObject);
			m_AllPositionInfo[l_pNamedTypedObject] = l_vPos;
		}
	}
}


Vector3* cPositionParser::GetPositionByName(WCHAR*e_str)
{
	NamedTypedObject*l_pNamedTypedObject = m_AllObject[e_str];
	if(!l_pNamedTypedObject)
		return 0;
	return &m_AllPositionInfo[l_pNamedTypedObject];
}

Vector3* cPositionParser::GetPositionByName(NamedTypedObject*e_pNamedTypedObject)
{
	return GetPositionByName(e_pNamedTypedObject->GetName());
}