#include "../stdafx.h"
#include "XMLLoader.h"
#include "../Utility.h"
#include "../Image/GlyphFontRender.h"
#include "../GameplayUT/GameApp.h"
#include "../GameplayUT/BinaryFile.h"
#include "../Math/MathUtility.h"
#include "../Sound/SoundManager.h"
namespace FATMING_CORE
{
	bool	g_bDumpXmlContent = false;
	const char*ISAXCallback::ExtensionNameID( "Fuck" );

	LoadingProgress	g_pLoadingProgressCallBack = 0;
	void	DefaultLoadingProgress()
	{
		cGameApp::m_sTimeAndFPS.Update();
		if(cGameApp::m_spSoundParser)
			cGameApp::m_spSoundParser->Update(cGameApp::m_sTimeAndFPS.fElpaseTime);
		static float	l_sfElpaseTime = 0.f;
		l_sfElpaseTime += cGameApp::m_sTimeAndFPS.fElpaseTime;
		if( l_sfElpaseTime >= 0.1f )
		{
			l_sfElpaseTime = 0.f;
		}
		else
			return;
		UseShaderProgram(DEFAULT_SHADER);
		glViewport(0,0,(int)cGameApp::m_svViewPortSize.Width(),(int)cGameApp::m_svViewPortSize.Height());
		//glScissor(0,0,(int)m_svViewPortSize.x,(int)m_svViewPortSize.y);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
		float	l_fPosX = cGameApp::m_svGameResolution.x/2-50;
		float	l_fPosY = cGameApp::m_svGameResolution.x/2;
		const wchar_t*l_strLoadingInfo = L"Loading";
		int	l_iLength = (int)wcslen(l_strLoadingInfo);
		int	l_iTemp[] = {0,0,0,0,0,0,0};
		vector<int>	l_uiAllMagicIDVector1 = UT::GenerateRandomTable(l_iLength,l_iLength);
		std::wstring	l_strInfo;
		for( int i=0;i<l_iLength;++i )
		{
			for( int j=0;j<l_iLength;++j )
			{
				if( l_uiAllMagicIDVector1[j] == i)
				{
					l_strInfo += l_strLoadingInfo[j];
				}
				else
					l_strInfo += L"    ";
			}
			l_strInfo += L"\n";
		}
		cGameApp::RenderFont(l_fPosX,l_fPosY,l_strInfo.c_str());
#ifdef ANDROID
		cGameApp::m_pNvEGLUtil->swap();
#endif
#ifdef WIN32
		SwapBuffers(cGameApp::m_sHdc);
#endif
	}
	ISAXCallback::ISAXCallback(bool e_bSetRunBack)
	{
		m_pCurrentTiXmlElement = 0;
		m_strFileName[0] = '\0';
		m_strCurrentDirectory[0] = '\0';
		m_bSetRunBack = e_bSetRunBack;
		m_bShowErrorMsg = false;
		m_pDoc = 0;
		#if defined(WIN32)
			m_bShowErrorMsg = true; 
		#elif defined(DEBUG)
			m_bShowErrorMsg = true;
		#endif
	}
	
	std::string	ISAXCallback::StringAddDirectory(std::string e_strValue)
	{
		return	StringAddDirectory((char*)e_strValue.c_str());
	}

	std::string	ISAXCallback::StringAddDirectory(const char*e_strValue)
	{
		return	StringAddDirectory((char*)e_strValue);
	}

	std::string	ISAXCallback::StringAddDirectory(char*e_strValue)
	{
		char	l_strValue[TEMP_SIZE];
		sprintf(l_strValue,"%s%s\0",m_strCurrentDirectory,e_strValue);
		std::string	l_str = l_strValue;
		return l_str;
	}

	TiXmlElement*	ISAXCallback::LoadingXML(const char*e_strFileName)
	{
	    SAFE_DELETE(m_pDoc);
		m_strErrorMsg.clear();
		sprintf(m_strFileName,"%s\0",e_strFileName);
		m_pDoc = new TiXmlDocument( UT::CharToWchar(e_strFileName).c_str() );
		bool loadOkay = m_pDoc->LoadFile();
		if(!loadOkay)
		{
		    SAFE_DELETE(m_pDoc);
			m_strErrorMsg = L"load file error!utf8?!!!fuck\n";
			std::string	l_str = UT::ComposeMsgByFormat("file %s open failed! uft8 or file is not exist!?",e_strFileName);
			if( cGameApp::m_spLogFile )
				cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
#ifdef DEBUG
			std::string	l_strErrorMsg = e_strFileName;
			l_strErrorMsg += " encode error or file not exist or xml format error??\n";
			cGameApp::OutputDebugInfoString(l_strErrorMsg);
			std::string l_strContent = UT::GetTxtFileContent(e_strFileName);
			cGameApp::OutputDebugInfoString(l_strContent);
#endif
				return 0;
		}
		TiXmlNode* l_pFirstNode = m_pDoc->FirstChild();
		TiXmlElement* itemElement = 0;
		itemElement = l_pFirstNode->ToElement();
		while( !itemElement )
		{
			itemElement = l_pFirstNode->NextSibling()->ToElement();
			l_pFirstNode = l_pFirstNode->NextSibling();
			if( !l_pFirstNode )
			{
				m_strErrorMsg += L"there is no FirstNode\n";
				return 0;
			}
		}
		assert(itemElement&&"empty element,check name for none numeral");
//iphone does't need to translate it because all file have to translate by loader api...
		std::string   l_strDirectory =  GetDirectoryWithoutFileName(e_strFileName);
		if( l_strDirectory.length() )
			sprintf(m_strCurrentDirectory,"%s\0",l_strDirectory.c_str());

#ifdef DEBUG
	cGameApp::OutputDebugInfoString(UT::CharToWchar(e_strFileName));
	cGameApp::OutputDebugInfoString(L"	Parsing Start\n");
#endif
		InternalParse();
		return itemElement;
	}

	bool	ISAXCallback::ParseText(const wchar_t*e_strContext)
	{
		SAFE_DELETE(m_pDoc);
		m_pDoc = new TiXmlDocument( );
		m_pDoc->Parse(e_strContext, 0);
		if( m_pDoc->Error() )
		{
			delete m_pDoc;
			return false;
		}
		TiXmlNode* l_pFirstNode = m_pDoc->FirstChild();
		TiXmlElement* itemElement = 0;
		itemElement = l_pFirstNode->ToElement();
		while( !itemElement )
		{
			itemElement = l_pFirstNode->NextSibling()->ToElement();
			l_pFirstNode = l_pFirstNode->NextSibling();
			if( !l_pFirstNode )
			{
				m_strErrorMsg += L"there is no FirstNode\n";
				return 0;
			}
		}
		assert(itemElement&&"empty element,check name for none numeral");
		InternalParse();
		if(!itemElement)
			return false;
		ParserAllElement(itemElement);
		delete m_pDoc;
		return true;
	}

	bool	ISAXCallback::Parse(const wchar_t*e_strFileName,bool e_bShowLoadingState)
	{
		return Parse(UT::WcharToChar(e_strFileName),e_bShowLoadingState);
	}

	bool	ISAXCallback::Parse(std::string e_strFileName,bool e_bShowLoadingState)
	{
		return Parse(e_strFileName.c_str(),e_bShowLoadingState);
	}

	bool    ISAXCallback::Parse(const char*e_strFileName,bool e_bShowLoadingState)
	{
		m_strErrorMsg.clear();
		TiXmlElement*l_pRootElement = ISAXCallback::LoadingXML((char*)e_strFileName);
		if (!l_pRootElement)
		{
			return false;
		}
		if(cGameApp::m_spstrErrorMsgString)
		{
			if(cGameApp::m_spstrErrorMsgString->length())
			{
				UT::ErrorMsg(cGameApp::m_spstrErrorMsgString->c_str(),L"before parse xml file there are error message not clear...");
			}
			cGameApp::m_spstrErrorMsgString->clear();
		}
		bool	l_bErrorMsgStringExist = FATMING_CORE::cGameApp::m_spstrErrorMsgString?true:false;
		int	l_iWhoErrorMsg = FATMING_CORE::cGameApp::m_siShowErrorType;
#ifndef	DEBUG
		FATMING_CORE::cGameApp::m_siShowErrorType = 2;
#endif
		if(!l_bErrorMsgStringExist)
		{
			FATMING_CORE::cGameApp::m_spstrErrorMsgString = new std::wstring();
		}
		m_bShowLoadingState = e_bShowLoadingState;
		//try
		{
			ParserAllElement(l_pRootElement);
		}
		//catch(...)
		//{
		//	if(m_bShowErrorMsg&&wcslen(m_strErrorMsg.c_str()))
		//	{
		//		UT::ErrorMsg((wchar_t*)this->m_strErrorMsg.c_str(),L"Error message");
		//	}		
		//}
		if(m_bShowErrorMsg&&wcslen(m_strErrorMsg.c_str()))
		{
			UT::ErrorMsg((wchar_t*)this->m_strErrorMsg.c_str(),L"Error message");
		}
		SAFE_DELETE(m_pDoc);
		if( FATMING_CORE::cGameApp::m_spstrErrorMsgString->length() )
		{
			FATMING_CORE::cGameApp::m_siShowErrorType = 1;
			UT::ErrorMsg((wchar_t*)FATMING_CORE::cGameApp::m_spstrErrorMsgString->c_str(),L"Error message");
			FATMING_CORE::cGameApp::m_spstrErrorMsgString->clear();
		}
		if(!l_bErrorMsgStringExist)
		{
			SAFE_DELETE(FATMING_CORE::cGameApp::m_spstrErrorMsgString);
		}
		FATMING_CORE::cGameApp::m_siShowErrorType = l_iWhoErrorMsg;
		return true;
	}

	bool	ISAXCallback::Export(const wchar_t*e_strFileName,bool e_bBinary)
	{
		if(m_pDoc)
		{
			return m_pDoc->SaveFile(UT::WcharToChar(e_strFileName),e_bBinary);
		}
		return false;
	}

	bool	ISAXCallback::Export(const char*e_strFileName,bool e_bBinary)
	{
		if(m_pDoc)
		{
			return m_pDoc->SaveFile(e_strFileName,e_bBinary);
		}
		return false;
	}

	TiXmlElement*	GetXmlElementByNameFromElement( const wchar_t*e_pString,TiXmlElement*e_pTiXmlElement )
	{
		//return SearchTiXmlElement(e_pTiXmlElement,(void*)e_pString,FindElementByName); 
		return SearchTiXmlElement(e_pTiXmlElement->FirstChildElement(),(void*)e_pString,FindElementByName);
	}

	void    XMLHandleElementDataDebugInfo(TiXmlElement*e_pTiXmlElement)
	{
	#ifdef DEBUG
			const wchar_t*     l_str = e_pTiXmlElement->Value();
			TiXmlAttribute*l_pAttribute = e_pTiXmlElement->FirstAttribute();
			if(l_pAttribute)
			{
					const wchar_t*     l_Test;
					const wchar_t*     l_strAttribute = 0;
					while( l_pAttribute )
					{
							l_Test = l_pAttribute->Value();
							l_strAttribute = l_pAttribute->Name();
							if(l_Test)
							{
	                                cGameApp::OutputDebugInfoString(L">>");
	                                cGameApp::OutputDebugInfoString((l_strAttribute));
							}
							l_pAttribute = l_pAttribute->Next();
					}
			}
	        cGameApp::OutputDebugInfoString(L"\n");
	#endif
	}

	TiXmlElement*   g_pXmlDebugInfoTiXmlElement;
	TiXmlElement*   ISAXCallback::ParserAllElement(TiXmlElement*e_pTiXmlElement)
	{
			//if(this->m_bShowLoadingState&&cGameApp::m_spGlyphFontRender)
			{
				if(g_pLoadingProgressCallBack)
					g_pLoadingProgressCallBack();
			}
	#ifdef WIN32
	#ifdef DEBUG
			g_pXmlDebugInfoTiXmlElement = e_pTiXmlElement;
			if( g_bDumpXmlContent )
			{
				while(g_pXmlDebugInfoTiXmlElement->Parent())
				{
						g_pXmlDebugInfoTiXmlElement = (TiXmlElement*)g_pXmlDebugInfoTiXmlElement->Parent();
		                cGameApp::OutputDebugInfoString(L"    ");
				}
				cGameApp::OutputDebugInfoString(e_pTiXmlElement->Value());
				XMLHandleElementDataDebugInfo(e_pTiXmlElement);
			}
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
	#ifdef DEBUG
			g_pXmlDebugInfoTiXmlElement = e_pTiXmlElement;
			if( g_bDumpXmlContent )
			{
				while(g_pXmlDebugInfoTiXmlElement->Parent())
				{
					g_pXmlDebugInfoTiXmlElement = (TiXmlElement*)g_pXmlDebugInfoTiXmlElement->Parent();
					cGameApp::OutputDebugInfoString(L"    ");
				}
				cGameApp::OutputDebugInfoString(e_pTiXmlElement->Value());
				cGameApp::OutputDebugInfoString(L"__End");
				cGameApp::OutputDebugInfoString(L"\n");
			}
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
	//		cGameApp::OutputDebugInfoString(L"	");
	//	cGameApp::OutputDebugInfoString(L"TagName:");
	//	cGameApp::OutputDebugInfoString(UT::CharToWchar(e_XMLNode->strTagName));
	//	cGameApp::OutputDebugInfoString(L">>");
	//	for( UINT i=0;i<e_XMLNode->AttributeList.size();++i )
	//	{
	//		cGameApp::OutputDebugInfoString(UT::CharToWchar(e_XMLNode->AttributeList[i].strAttribute));
	//		cGameApp::OutputDebugInfoString(L">>");
	//	}
	//	cGameApp::OutputDebugInfoString(L"\n");
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
	//		cGameApp::OutputDebugInfoString(L",");
	//	}
	//	printf(L"\n");
	//	for( UINT i=0;i<e_XMLNode.ChildNodeList.size();++i )
	//		ShowDebugInfo(e_XMLNode.ChildNodeList[i]);
	//	for( UINT i=0;i<e_XMLNode.SiblingNodeList.size();++i )
	//		ShowDebugInfo(e_XMLNode.SiblingNodeList[i]);	
	//#endif
	//}

	void			SearchTiXmlElementAndSaveIt(TiXmlElement* e_pNode,std::vector<TiXmlElement*>*e_pAllMatchedElementVector,void* e_pPointer,TiXmlElement*(*callback)( TiXmlElement*e_pCallbackFrame ,void* e_pcallbackPointer ) )
	{
		TiXmlElement*	l_pTiXmlElement = (*callback)(e_pNode,e_pPointer);
		if( l_pTiXmlElement )
		{
			e_pAllMatchedElementVector->push_back(l_pTiXmlElement);
		}
		if( e_pNode->NextSiblingElement() != nullptr)
		{		
			SearchTiXmlElementAndSaveIt( e_pNode->NextSiblingElement(),e_pAllMatchedElementVector,e_pPointer,callback);
		}
		if (e_pNode->FirstChildElement() != nullptr)
		{
			SearchTiXmlElementAndSaveIt( e_pNode->FirstChildElement(),e_pAllMatchedElementVector,e_pPointer,callback);
		}
	}
	TiXmlElement*	SearchTiXmlElement(TiXmlElement* e_pNode,void* e_pPointer,TiXmlElement*(*callback)( TiXmlElement*e_pCallbackFrame ,void* e_pcallbackPointer ) )
	{
		if( e_pNode )
		{
			TiXmlElement*	l_pTiXmlElement = (*callback)(e_pNode,e_pPointer);
			if( l_pTiXmlElement )
				return l_pTiXmlElement;
			if( e_pNode->NextSiblingElement() != nullptr)
			{		
				//why return ?check GoThoughAllFrameFromaFirstToEnd
				return SearchTiXmlElement( e_pNode->NextSiblingElement(),e_pPointer,callback);
			}
			if (e_pNode->FirstChildElement() != nullptr)
			{
				//why return ?check GoThoughAllFrameFromaFirstToEnd
				return SearchTiXmlElement( e_pNode->FirstChildElement(),e_pPointer,callback);
			}
		}

		return 0;
	}

	TiXmlElement	*FindElementByName( TiXmlElement*e_pCallbackFrame ,void* e_pcallbackPointer )
	{
		if( e_pCallbackFrame && e_pcallbackPointer )
		{
			wchar_t*	e_strName = (wchar_t*)e_pcallbackPointer;
			const wchar_t*l_strNodeNAme = e_pCallbackFrame->Value();
			if( !wcscmp(l_strNodeNAme,e_strName) )
				return e_pCallbackFrame;
		}
		return 0;
	}

	cNodeISAX::cNodeISAX()
	{
		m_pRootElement = 0;
	}

	cNodeISAX::~cNodeISAX()
	{

	}


	bool	cNodeISAX::ParseTextIntoXMLNode(const char*e_strText)
	{
		SystemErrorCheck();
		m_strErrorMsg.c_str();
		SAFE_DELETE(m_pDoc);
		std::wstring l_strText = UT::CharToWchar(e_strText);
		m_pDoc = new TiXmlDocument( );
		m_pDoc->Parse(l_strText.c_str(), 0);
		if( m_pDoc->Error() )
		{
			delete m_pDoc;
			return false;
		}
		TiXmlNode* l_pFirstNode = m_pDoc->FirstChild();
		TiXmlElement* itemElement = 0;
		itemElement = l_pFirstNode->ToElement();
		while( !itemElement )
		{
			itemElement = l_pFirstNode->NextSibling()->ToElement();
			l_pFirstNode = l_pFirstNode->NextSibling();
			if( !l_pFirstNode )
			{
				m_strErrorMsg += L"there is no FirstNode\n";
				return 0;
			}
		}
		assert(itemElement&&"empty element,check name for none numeral");
		InternalParse();
		if(!itemElement)
			return false;
		m_pRootElement = itemElement;
		if( !m_pRootElement )
			m_pRootElement = m_pDoc->FirstChild()->NextSibling()->ToElement();
		assert(m_pRootElement&&"empty element,check name for none numeral");
		SystemErrorCheck();
		return true;	
	}
	bool	cNodeISAX::ParseDataIntoXMLNode(const wchar_t*e_strText)
	{
		return ParseDataIntoXMLNode(UT::WcharToChar(e_strText).c_str());
	}
	bool	cNodeISAX::ParseDataIntoXMLNode(const char*e_strFileName)
	{
		SystemErrorCheck();
		//m_strErrorMsg.c_str();
		SAFE_DELETE(m_pDoc);
		std::wstring	l_str = UT::CharToWchar(e_strFileName);
		m_pDoc = new TiXmlDocument(l_str.c_str());
		bool loadOkay = m_pDoc->LoadFile();
		if(!loadOkay)
		{
			SAFE_DELETE(m_pDoc);
			m_strErrorMsg += L"Open file falied ";
			m_strErrorMsg += UT::CharToWchar(e_strFileName);
			return false;
		}
	//iphone does't need to translate it because all file have to translate by loader api...
		std::string   l_strDirectory =  GetDirectoryWithoutFileName(e_strFileName);
		if( l_strDirectory.length() )
			sprintf(m_strCurrentDirectory,"%s",l_strDirectory.c_str());
		TiXmlNode* node = 0;
		node  = m_pDoc->FirstChild();
		m_pRootElement = node->ToElement();
		if( !m_pRootElement )
		{
			TiXmlNode* node2 = m_pDoc->FirstChild()->NextSibling();
			while( node2 && !m_pRootElement )
			{
				m_pRootElement = node2->ToElement();
				node2 = node2->NextSibling();
			}
		}
		assert(m_pRootElement&&"empty element,check name for none numeral");
		SystemErrorCheck();
		return true;
	}

	bool	cNodeISAX::ParseTextWithMyParse(const char*e_str,bool e_bDeleteXLDocument)
	{
		if(!ParseTextIntoXMLNode(e_str))
			return false;
		if( !this->m_pRootElement )
			return false;
		bool    l_b = MyParse(m_pRootElement);
		if( e_bDeleteXLDocument )
		    SAFE_DELETE(m_pDoc);
		return l_b;	
	}

	bool	cNodeISAX::ParseWithMyParse(const wchar_t*e_strFileName, bool e_bDeleteXLDocument)
	{
		std::string l_str = ValueToString(e_strFileName);
		return ParseWithMyParse(l_str.c_str(), e_bDeleteXLDocument);
	}

	bool	cNodeISAX::ParseWithMyParse(const char*e_strFileName,bool e_bDeleteXLDocument)
	{
		m_strErrorMsg.c_str();
	    SAFE_DELETE(m_pDoc);
		TiXmlElement*l_pRootElement = ISAXCallback::LoadingXML(e_strFileName);
		this->m_pRootElement = l_pRootElement;
		if( !l_pRootElement )
			return false;
		bool    l_b = MyParse(l_pRootElement);
		if( e_bDeleteXLDocument )
		    SAFE_DELETE(m_pDoc);
		return l_b;
	}

	TiXmlElement*	cNodeISAX::GetChildOfRootXmlElementByName( const wchar_t*e_pString )
	{
		TiXmlElement*l_pTiXmlElement = m_pRootElement->FirstChild()->ToElement();
		while(l_pTiXmlElement)
		{
			if(!wcscmp(l_pTiXmlElement->Value(),e_pString))
			{
				return l_pTiXmlElement;
			}
			l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
		}
		return 0;
	}
	
    TiXmlElement*	cNodeISAX::GetChildOfRootXmlElementByAttribueName( const wchar_t*e_pElementName,const wchar_t*e_strAttributeKeyWord,const wchar_t*e_strAttributeName )
    {
		if( m_pRootElement )
		{
			TiXmlElement*l_pTiXmlElement = m_pRootElement->FirstChildElement();
			while(l_pTiXmlElement)
			{
				if(!wcscmp(l_pTiXmlElement->Value(),e_pElementName))
				{
					const wchar_t*l_strAttributeKeyWord = l_pTiXmlElement->Attribute(e_strAttributeName);
					if( l_strAttributeKeyWord )
					{
						if( !wcscmp(l_strAttributeKeyWord,e_strAttributeKeyWord) )
							return l_pTiXmlElement;
					}
				}
				l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
			}
		}
		return 0;    
    }

	TiXmlElement*	cNodeISAX::GetRootChildElementByIndex( int e_iIndex )
	{
		if( m_pRootElement )
		{
			int	l_iIndex = 0;
			TiXmlElement*l_pTiXmlElement = m_pRootElement->FirstChildElement();
			while(l_pTiXmlElement)
			{
				if( l_iIndex == e_iIndex )
					return l_pTiXmlElement;
				l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
				++l_iIndex;

			}
		}
		return 0;
	}

	int		cNodeISAX::RootChildrenCount()
	{
		if( m_pRootElement )
		{
			int	l_iIndex = 0;
			TiXmlElement*l_pTiXmlElement = m_pRootElement->FirstChildElement();
			while(l_pTiXmlElement)
			{
				l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
				++l_iIndex;
			}
			return l_iIndex;
		}
		return 0;	
	}

	void	TiXmlElementReplace(TiXmlElement*e_pTiXmlElement1,TiXmlElement*e_pTiXmlElement2)
	{
		std::vector<std::wstring>	l_NameVector;
		//remove old xml not use node
		{
			PARSE_ELEMENT_START(e_pTiXmlElement1)
				if(!e_pTiXmlElement2->Attribute(l_strName))
				{
					l_NameVector.push_back(std::wstring(l_strName));
				}
			TO_NEXT_VALUE
			while( l_NameVector.size() )
			{
				e_pTiXmlElement1->RemoveAttribute(l_NameVector[0].c_str());
				l_NameVector.erase(l_NameVector.begin());
			}
		}
		//replace old node value to new value
		{
			PARSE_ELEMENT_START(e_pTiXmlElement2)
				if(!e_pTiXmlElement1->Attribute(l_strName))
				{
					e_pTiXmlElement1->SetAttribute(l_strName,l_strValue);
				}
			TO_NEXT_VALUE
		}
		//remove old data and set child's attribute value
		TiXmlElement*l_pTiXmlElement1Child = e_pTiXmlElement1->FirstChildElement();
		while( l_pTiXmlElement1Child )
		{
			TiXmlElement*l_pCompareTiXmlElement = e_pTiXmlElement2->FirstChildElement(l_pTiXmlElement1Child->Value());
			if( !l_pCompareTiXmlElement )
			{
				l_NameVector.push_back(std::wstring(l_pTiXmlElement1Child->Value()));
			}
			else
			{
				TiXmlElementReplace(l_pTiXmlElement1Child,l_pCompareTiXmlElement);
			}
			l_pTiXmlElement1Child = l_pTiXmlElement1Child->NextSiblingElement();
		}
		while( l_NameVector.size() )
		{
			e_pTiXmlElement1->RemoveChild(e_pTiXmlElement1->FirstChild(l_NameVector[0].c_str()));
			l_NameVector.erase(l_NameVector.begin());
		}
		//new data assign to old one
		TiXmlElement*l_pTiXmlElement2Child = e_pTiXmlElement2->FirstChildElement();
		while( l_pTiXmlElement2Child )
		{
			const wchar_t*l_strValue = l_pTiXmlElement2Child->Value();
			TiXmlElement*l_pCompareTiXmlElement = e_pTiXmlElement1->FirstChildElement(l_strValue);
			if( !l_pCompareTiXmlElement )
			{
				TiXmlElement*l_pNewTiXmlElement = new TiXmlElement(*l_pTiXmlElement2Child);
				e_pTiXmlElement1->LinkEndChild(l_pNewTiXmlElement);
			}
			l_pTiXmlElement2Child = l_pTiXmlElement2Child->NextSiblingElement();
		}
	}

	bool	cNodeISAX::ReplaceNodeDataByTemplateNode(const char*e_strTargetFileName,const char*e_strTemplateFileName,const char*e_strExportName)
	{
		cNodeISAX	l_cNodeISAX1;		l_cNodeISAX1.ParseDataIntoXMLNode(e_strTargetFileName);
		cNodeISAX	l_cNodeISAX2;		l_cNodeISAX2.ParseDataIntoXMLNode(e_strTemplateFileName);
		TiXmlElement*l_pTiXmlElement1 = l_cNodeISAX1.GetRootElement();
		TiXmlElement*l_pTiXmlElement2 = l_cNodeISAX2.GetRootElement();
		if( l_pTiXmlElement1 && l_pTiXmlElement2 )
		{
			const wchar_t*l_strCompareName = l_pTiXmlElement2->Value();
			std::vector<TiXmlElement*>	l_SameNameElement;
			SearchTiXmlElementAndSaveIt(l_pTiXmlElement1,&l_SameNameElement,(void*)l_strCompareName,FindElementByName);
			//remove old xml not use node
			for( size_t i=0;i<l_SameNameElement.size();++i )
			{
				TiXmlElement*l_pTiXmlElement = l_SameNameElement[i];
				TiXmlElementReplace(l_pTiXmlElement,l_pTiXmlElement2);
			}
			//replace old node value to new value
			l_cNodeISAX1.Export(e_strExportName);
			return true;
		}
		return false;
	}
}