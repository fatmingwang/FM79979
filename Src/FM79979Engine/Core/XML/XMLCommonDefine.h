#pragma once


//here should using # instead
#define	COMPARE_VALUE_WITH_DEFINE(x)							if( !wcscmp(l_strValue, x) )
#define	COMPARE_NAME_WITH_DEFINE(x)								if( !wcscmp(l_strName, x) )
#define	COMPARE_VALUE(x)										if( !wcscmp(l_strValue,L## x) )
#define	COMPARE_NAME(x)											if( !wcscmp(l_strName,L## x) )

#define	COMPARE_NOT_SUPPORT(value,FunctionName)					else{std::wstring	l_strFunctionNameAndValue = FunctionName;l_strFunctionNameAndValue += value;UT::ErrorMsg(l_strFunctionNameAndValue.c_str(),L"not support format") ;	}

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
									const wchar_t*	l_strValue = nullptr;												\
									const wchar_t*	l_strName = nullptr;												\
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
#define	VALUE_TO_DOUBLE							_wtof(l_strValue)
#define	VALUE_TO_BOOLEAN						VALUE_TO_INT?true:false
#define	VALUE_TO_POINT  						GetPoint(l_strValue)
#define	VALUE_TO_CHAR_STRING					UT::WcharToChar(l_strValue)
#define	VALUE_TO_WCHAR_STRING					l_strValue
#define	VALUE_TO_VECTOR2						GetVector2(l_strValue)
#define	VALUE_TO_VECTOR3						GetVector3(l_strValue)
#define	VALUE_TO_VECTOR4						GetVector4(l_strValue)

#define	COMPARE_ASSIGN_INT(KeyString,Variable)		COMPARE_NAME(KeyString){Variable = VALUE_TO_INT;}
#define	COMPARE_ASSIGN_FLOAT(KeyString,Variable)	COMPARE_NAME(KeyString){Variable = VALUE_TO_FLOAT;}
#define	COMPARE_ASSIGN_DOUBLE(KeyString,Variable)	COMPARE_NAME(KeyString){Variable = VALUE_TO_DOUBLE;}
#define	COMPARE_ASSIGN_BOOLEAN(KeyString,Variable)	COMPARE_NAME(KeyString){Variable = VALUE_TO_BOOLEAN;}
#define	COMPARE_ASSIGN_POINT(KeyString,Variable)	COMPARE_NAME(KeyString){Variable = VALUE_TO_POINT;}

#define	COMPARE_ASSIGN_CHAR_STRING(KeyString,Variable)	COMPARE_NAME(KeyString){Variable = VALUE_TO_CHAR_STRING;}
#define	COMPARE_ASSIGN_WCHAR_STRING(KeyString,Variable)	COMPARE_NAME(KeyString){Variable = VALUE_TO_WCHAR_STRING;}

#define	COMPARE_ASSIGN_VECTOR2(KeyString,Variable)	COMPARE_NAME(KeyString){Variable = VALUE_TO_VECTOR2;}
#define	COMPARE_ASSIGN_VECTOR3(KeyString,Variable)	COMPARE_NAME(KeyString){Variable = VALUE_TO_VECTOR3;}
#define	COMPARE_ASSIGN_VECTOR4(KeyString,Variable)	COMPARE_NAME(KeyString){Variable = VALUE_TO_VECTOR4;}


#define	PARSE_FAILED_MESSAGE_BOX(p,FileName)	if(!p->Parse(FileName)){ UT::ErrorMsg(FileName,"parse failed"); }
#define	PARSEWITHMYPARSE_FAILED_MESSAGE_BOX(p,FileName)	if(!p->ParseWithMyParse(FileName)){ UT::ErrorMsg(FileName,"parse failed"); }

#define	ATTRIBUT_VALUE(Element,Name) Element->Attribute(L## Name)
#define	ATTRIBUT_VALUE_WITH_DEFINE(Element,Name) Element->Attribute(Name)

#define	FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(Element)Element = Element->FirstChildElement();while(Element){
#define FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(Element) Element = Element->NextSiblingElement();}


#define	LAZY_ADD_ATTRIBUTE(Element,Name,Data)Element->SetAttribute(Name,ValueToStringW(Data).c_str());
#define	VERY_LAZY_ADD_ATTRIBUTE(Element,Name)Element->SetAttribute(WSTRING(Name),ValueToStringW(m_##Name).c_str());

#define	VERY_LAZY_TO_VECTOR2(Element,Name){auto l_strTemp = Element->Attribute(WSTRING(Name));m_##Name = GetVector2(l_strTemp);}
#define	VERY_LAZY_TO_POINT(Element,Name){auto l_strTemp = Element->Attribute(WSTRING(Name));m_##Name = GetPoint(l_strTemp);}
#define	VERY_LAZY_TO_INT(Element,Name){auto l_strTemp = Element->Attribute(WSTRING(Name));m_##Name = GetInt(l_strTemp);}