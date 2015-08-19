#pragma once
#include "EventTemplate.h"
using namespace EVENT;

enum eCommonXmlLoadeType
{
	CXLT_INFO = 0,
	CXLT_CATEGORY,
	CXLT_DESIGNATION,
	CXLT_CONTENT,
	CXLT_NONE,
};
class cCommonXmlFileParser : public ISAXCallback
{
public:
	static const CHAR* GetParseErrorMessage();

	virtual HRESULT  StartDocument() { return S_OK; }
	virtual HRESULT  EndDocument() { return S_OK; }

	virtual HRESULT  ElementBegin( const WCHAR* strName, UINT NameLen, 
								   const XMLAttribute *pAttributes, UINT NumAttributes );
	virtual HRESULT  ElementContent( const WCHAR *strData, UINT DataLen, BOOL More );
	virtual HRESULT  ElementEnd( const WCHAR *strName, UINT NameLen );

	virtual HRESULT  CDATABegin() { return S_OK; }
	virtual HRESULT  CDATAData( const WCHAR *strCDATA, UINT CDATALen, BOOL bMore ) { return S_OK; }
	virtual HRESULT  CDATAEnd() { return S_OK; }

	virtual VOID     SetParseProgress( DWORD dwProgress ){}

	virtual VOID     Error( HRESULT hError, const CHAR *strMessage ){}
	EventTemplateList	m_EventTemplateList;
	WCHAR*		GetProtertyByCategoryAndDesignation(WCHAR*,WCHAR*);
protected:	
	eCommonXmlLoadeType	m_eCommonXmlLoadeType;
	XMLElementDesc	m_CurrentElementDesc;		
	
	VOID CopyAttributes( const XMLAttribute* pAttributes, UINT uAttributeCount );

	
	virtual VOID HandleElementData();
	VOID HandleElementEnd();

	
	virtual VOID DistributeElementToLoaders();

	WCHAR* FindAttribute( const WCHAR* strName );
	BOOL FindAttribute( const WCHAR* strName, WCHAR* strDest, UINT uDestLength );

	//BOOL SetObjectNameFromAttribute( NamedTypedObject* pNTO );

	VOID ProcessCategory();
	VOID ProcessDesignation();
	VOID ProcessContent();
	VOID ProcessLoadInfo();


};