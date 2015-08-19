#include "stdafx.h"
#include "CommonXmlLoad.h"

#define IS_ELEMENT_NAME( x )if( _wcsicmp(m_CurrentElementDesc.strElementName, x)==0 )

//--------------------------------------------------------------------------------------
// Name: HandleElementData()
// Desc: This method gets "first crack" at a begin tag + content combo.
//       It identifies certain high-level tags and sets the loader state appropriately.
//--------------------------------------------------------------------------------------
VOID cCommonXmlFileParser::HandleElementData()
{
	if( wcslen( m_CurrentElementDesc.strElementName ) == 0 )
		return;

	m_CurrentElementDesc.bEndElement = FALSE;

	IS_ELEMENT_NAME(L"Category")
	{
		m_eCommonXmlLoadeType = CXLT_CATEGORY;
	}	
	else
	IS_ELEMENT_NAME(L"Designation")
	{
		m_eCommonXmlLoadeType = CXLT_DESIGNATION;	
	}	
	else
	IS_ELEMENT_NAME(L"Content")
	{
		m_eCommonXmlLoadeType = CXLT_CONTENT;
	}
	else
		m_eCommonXmlLoadeType = CXLT_NONE;

	// Distribute the data to the appropriate loader function.
	DistributeElementToLoaders();
}


//--------------------------------------------------------------------------------------
// Name: HandleElementEnd()
// Desc: This method gets "first crack" at an end tag.
//       It labels the tag state as "end tag" and passes the tag onto the loader
//       methods.
//--------------------------------------------------------------------------------------
VOID cCommonXmlFileParser::HandleElementEnd()
{
	// We are processing an end tag.
	m_CurrentElementDesc.bEndElement = TRUE;

	// Distribute the end tag to the appropriate loader function.
	DistributeElementToLoaders();
}


//--------------------------------------------------------------------------------------
// Name: DistributeElementToLoaders()
// Desc: This method calls the correct loader method based on the type of the current
//       object that we're parsing.  In general, a high level tag sets the object type,
//       and then this method sends all child tags of that high level tag to the right
//       loader method.
//--------------------------------------------------------------------------------------
VOID cCommonXmlFileParser::DistributeElementToLoaders()
{
	int a=0;
	int bb=a;
	switch(m_eCommonXmlLoadeType )
	{
		case CXLT_CATEGORY:	
			ProcessCategory();
			break;
		case CXLT_CONTENT:	
			ProcessContent();
			break;
		case CXLT_DESIGNATION:	
			ProcessDesignation();
			break;
		//default:
		//	::MessageBox(0,L"spell error or not support",L"waring",MB_OK);
		//	assert( FALSE );
		//	break;
	}
}


HRESULT cCommonXmlFileParser::ElementBegin( const WCHAR* strName, UINT NameLen, const XMLAttribute *pAttributes, UINT NumAttributes )
{

	// Distribute an accumulated begin+content package if one exists.
	HandleElementData();

	// Start a new begin+content package.
	// Copy the begin tag name to the current element desc.
	wcsncpy_s( m_CurrentElementDesc.strElementName, strName, NameLen );

	// Clear out the accumulated element body.
	m_CurrentElementDesc.strElementBody[0] = L'\0';

	// Copy all attributes from the begin tag into the current element desc.
	CopyAttributes( pAttributes, NumAttributes );
	return S_OK;
}

HRESULT cCommonXmlFileParser::ElementContent( const WCHAR* strData, UINT DataLen, BOOL More )
{	
	// Accumulate this element content into the current desc body content.
	if( DataLen>1024 )
		assert( 0 && "Content too large" );//wcsncat_s( m_CurrentElementDesc.strElementBody, L"Content too large", DataLen );
	else
		wcsncat_s( m_CurrentElementDesc.strElementBody, strData, DataLen );
	
	return S_OK;
}

HRESULT cCommonXmlFileParser::ElementEnd( const WCHAR* strName, UINT NameLen )
{
	// Distribute an accumulated begin+content package if one exists.
	HandleElementData();

	// Copy the end tag name into the current element desc.
	wcsncpy_s( m_CurrentElementDesc.strElementName, strName, NameLen );
	// Clear out the element body.
	m_CurrentElementDesc.strElementBody[0] = L'\0';
	// Distribute the end tag.
	HandleElementEnd();
	// Clear out the element name.
	m_CurrentElementDesc.strElementName[0] = L'\0';
	return S_OK;
}

VOID cCommonXmlFileParser::CopyAttributes( const XMLAttribute* pAttributes, UINT uAttributeCount )
{
	m_CurrentElementDesc.Attributes.clear();
	for( UINT i = 0; i < uAttributeCount; i++ )
	{
		XMLElementAttribute Attribute;
		wcsncpy_s( Attribute.strName, pAttributes[i].strName, pAttributes[i].NameLen );
		wcsncpy_s( Attribute.strValue, pAttributes[i].strValue, pAttributes[i].ValueLen );
		m_CurrentElementDesc.Attributes.push_back( Attribute );
	}
}

BOOL cCommonXmlFileParser::FindAttribute( const WCHAR* strName, WCHAR* strDest, UINT uDestLength )
{
	const WCHAR* strValue = FindAttribute( strName );
	if( strValue != NULL )
	{
		wcscpy_s( strDest, uDestLength, strValue );
		return TRUE;
	}
	strDest[0] = L'\0';
	return FALSE;
}

WCHAR* cCommonXmlFileParser::FindAttribute( const WCHAR* strName )
{
	for( UINT i = 0; i < m_CurrentElementDesc.Attributes.size(); i++ )
	{
		const XMLElementAttribute& Attribute = m_CurrentElementDesc.Attributes[i];
		if( _wcsicmp( Attribute.strName, strName ) == 0 )
		{
			return (WCHAR*)Attribute.strValue;
		}
	}
	return NULL;
}

VOID	cCommonXmlFileParser::ProcessCategory()
{
	if(!m_CurrentElementDesc.bEndElement)
	{
		WCHAR*	l_pName = FindAttribute( L"Name");
		for(UINT i=0;i<m_EventTemplateList.size();i++)
		{
			if( _wcsicmp(m_EventTemplateList[i]->m_sName, l_pName)==0 )
				return;
		}
		sEventTemplate	*l_pEventTemplateToPush = new sEventTemplate();
		swprintf_s(l_pEventTemplateToPush->m_sName,MAX_PATH,L"%s\0",l_pName);
		m_EventTemplateList.push_back(l_pEventTemplateToPush);
	}
	else
	{
	
	}
}

VOID cCommonXmlFileParser::ProcessDesignation()
{
	if(!m_CurrentElementDesc.bEndElement)
	{
		WCHAR*	l_pName = FindAttribute( L"Name");
		int	l_iCurrentIndex = m_EventTemplateList.size()-1;
		sEventTemplate	*l_pEventTemplateToPush = m_EventTemplateList[l_iCurrentIndex];
		assert(l_pEventTemplateToPush&&"no such category impossible");
		sAttributeAndValue	*l_pAttributeAndValue = new sAttributeAndValue();
		swprintf_s(l_pAttributeAndValue->m_sAttributeName,MAX_PATH,L"%s\0",l_pName);
		l_pEventTemplateToPush->m_AttributeAndValueList.push_back(l_pAttributeAndValue);

	}
	else
	{
	
	}
}

VOID cCommonXmlFileParser::ProcessContent()
{
	if(!m_CurrentElementDesc.bEndElement)
	{
		WCHAR*	l_pContent = FindAttribute( L"Property");
		sEventTemplate	*l_pEventTemplateToPush = m_EventTemplateList[m_EventTemplateList.size()-1];
		assert(l_pEventTemplateToPush&&"no such category impossible");
		//get last 
		sAttributeAndValue	*l_pAttributeAndValue = l_pEventTemplateToPush->m_AttributeAndValueList[l_pEventTemplateToPush->m_AttributeAndValueList.size()-1];
		assert(wcslen(l_pAttributeAndValue->m_sAttributeName)!=0&&"no designation error");
		swprintf_s(l_pAttributeAndValue->m_sValue,MAX_PATH,L"%s\0",l_pContent);
	}
	else
	{
	
	}
}


WCHAR*	cCommonXmlFileParser::GetProtertyByCategoryAndDesignation(WCHAR*e_pStringForCateGory ,WCHAR*e_pStringForDesignation)
{
	for(UINT i = 0;i<m_EventTemplateList.size();i++)
	{
		if(wcscmp(m_EventTemplateList[i]->m_sName,e_pStringForCateGory)==0)
		{
			for(UINT j=0;j<m_EventTemplateList[i]->m_AttributeAndValueList.size();i++)
			{
				if(wcscmp(m_EventTemplateList[i]->m_AttributeAndValueList[j]->m_sAttributeName,e_pStringForDesignation)==0)
				{
					return m_EventTemplateList[i]->m_AttributeAndValueList[j]->m_sValue;
				}
			}
		}	
	}
	return NULL;
}