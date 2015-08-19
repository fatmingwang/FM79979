#pragma once
#include ".\XML\AtgXmlParser.h"
#include ".\XML\TreeLoad.h"
using namespace ATG;

enum eTextEditorLoadType
{
	TELT_INFO = 0,
	TELT_TREE_NODE_GROUP,
	TELT_TREE_NODE_CHILD,
	TELT_NONE,
};

class TextEditorLoad :	public ISAXCallback
{
public:
	TextEditorLoad(){ m_pTreeLoad = new ATG::TreeLoad(); }
	~TextEditorLoad(){ DELETE_LIST(m_TreeLoadList,TreeLoad); delete(m_pTreeLoad); }
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

	WCHAR*		GetProtertyByCategoryAndDesignation(WCHAR*,WCHAR*);
	ATG::TreeLoad*	m_pTreeLoad;
protected:
	eTextEditorLoadType	m_eTextEditorLoadType;
	XMLElementDesc	m_CurrentElementDesc;
	std::vector<ATG::TreeLoad*>	m_TreeLoadList;
	
	VOID CopyAttributes( const XMLAttribute* pAttributes, UINT uAttributeCount );

	
	virtual VOID HandleElementData();
	VOID HandleElementEnd();
	
	virtual VOID DistributeElementToLoaders();

	WCHAR* FindAttribute( const WCHAR* strName );
	BOOL FindAttribute( const WCHAR* strName, WCHAR* strDest, UINT uDestLength );

	void	ProcessGroup();
	void	ProcessChild();
};