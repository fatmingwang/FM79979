#include "StdAfx.h"
#include "TextEditorLoad.h"

#define IS_ELEMENT_NAME( x )if( _wcsicmp(m_CurrentElementDesc.strElementName, x)==0 )

//--------------------------------------------------------------------------------------
// Name: HandleElementData()
// Desc: This method gets "first crack" at a begin tag + content combo.
//       It identifies certain high-level tags and sets the loader state appropriately.
//--------------------------------------------------------------------------------------
VOID TextEditorLoad::HandleElementData()
{
	if( wcslen( m_CurrentElementDesc.strElementName ) == 0 )
		return;

	m_CurrentElementDesc.bEndElement = FALSE;

	IS_ELEMENT_NAME(L"Group")
	{
		m_eTextEditorLoadType = TELT_TREE_NODE_GROUP;
	}	
	//else
	IS_ELEMENT_NAME(L"Child")
	{
		m_eTextEditorLoadType = TELT_TREE_NODE_CHILD;
	}	
	//else
	//IS_ELEMENT_NAME(L"Content")
	//{
	//	m_eCommonXmlLoadeType = CXLT_CONTENT;
	//}
	else
	{
		m_eTextEditorLoadType = TELT_NONE;
	}

	// Distribute the data to the appropriate loader function.
	DistributeElementToLoaders();
}


//--------------------------------------------------------------------------------------
// Name: HandleElementEnd()
// Desc: This method gets "first crack" at an end tag.
//       It labels the tag state as "end tag" and passes the tag onto the loader
//       methods.
//--------------------------------------------------------------------------------------
VOID TextEditorLoad::HandleElementEnd()
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
VOID TextEditorLoad::DistributeElementToLoaders()
{
	switch(m_eTextEditorLoadType )
	{
		case TELT_TREE_NODE_GROUP:
		{
			ATG::TreeLoad* l_p = new TreeLoad();
			m_TreeLoadList.push_back(l_p);
		}
			break;
		case TELT_TREE_NODE_CHILD:	
			ProcessChild();
			break;
		//case CXLT_DESIGNATION:	
		//	ProcessDesignation();
		//	break;
		//default:
		//	::MessageBox(0,L"spell error or not support",L"waring",MB_OK);
		//	assert( FALSE );
		//	break;
	}
}


HRESULT TextEditorLoad::ElementBegin( const WCHAR* strName, UINT NameLen, const XMLAttribute *pAttributes, UINT NumAttributes )
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

HRESULT TextEditorLoad::ElementContent( const WCHAR* strData, UINT DataLen, BOOL More )
{	
	// Accumulate this element content into the current desc body content.
	if( DataLen>1024 )
		assert( 0 && "Content too large" );//wcsncat_s( m_CurrentElementDesc.strElementBody, L"Content too large", DataLen );
	else
		wcsncat_s( m_CurrentElementDesc.strElementBody, strData, DataLen );
	
	return S_OK;
}

HRESULT TextEditorLoad::ElementEnd( const WCHAR* strName, UINT NameLen )
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

VOID TextEditorLoad::CopyAttributes( const XMLAttribute* pAttributes, UINT uAttributeCount )
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

BOOL TextEditorLoad::FindAttribute( const WCHAR* strName, WCHAR* strDest, UINT uDestLength )
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

WCHAR* TextEditorLoad::FindAttribute( const WCHAR* strName )
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

void	TextEditorLoad::ProcessGroup()
{
	if(!m_CurrentElementDesc.bEndElement)
	{
		WCHAR*	l_pName = FindAttribute( L"Name");
		int	l_i = -1;
		//if( !IsExistCategoryContentList(l_pName,&l_i) )
		//{
		//	sCategoryContent*	l_psCategoryContent = new sCategoryContent;
		//	swprintf_s(l_psCategoryContent->m_sName,MAX_PATH,L"%s\0",FindAttribute( L"Name"));
		//	this->m_CategoryContentList.push_back(l_psCategoryContent);			
		//	m_psCategoryContent = l_psCategoryContent;
		//}
#ifdef _DEBUG
//		else
		{//usually it would be here but debug mode maybe not			
			//m_psCategoryContent = m_CategoryContentList[l_i];
		}
#endif
	}
	else
	{
	
	}
}

void	TextEditorLoad::ProcessChild()
{
	if(!m_CurrentElementDesc.bEndElement)
	{
		WCHAR*	l_pName = FindAttribute( L"Name");
		int	l_i = -1;
		//if( !IsExistCategoryContentList(l_pName,&l_i) )
		//{
		//	sCategoryContent*	l_psCategoryContent = new sCategoryContent;
		//	swprintf_s(l_psCategoryContent->m_sName,MAX_PATH,L"%s\0",FindAttribute( L"Name"));
		//	this->m_CategoryContentList.push_back(l_psCategoryContent);			
		//	m_psCategoryContent = l_psCategoryContent;
		//}
#ifdef _DEBUG
		//else
		{//usually it would be here but debug mode maybe not			
			//m_psCategoryContent = m_CategoryContentList[l_i];
		}
#endif
	}
	else
	{
	
	}
}