#include "MathFormulaXML.h"
#ifdef USE_MU_PARSER
//#include "../GameplayUT/GameApp.h"
#include "./MathParser/muParser.h"
#include "../Common/StringToStructure.h"
//#include "muParser.h"
namespace FATMING_CORE
{
	//<cMathFormulaXML>
	//	<Equation Data=""/>
	//	<Variable Name="" DefaultValue="" />
	//	<Variable Name="" DefaultValue="" />
	//	<Variable Name="" DefaultValue="" />
	//</cMathFormulaXML>
	TYPDE_DEFINE_MARCO(cVariable)
	cVariable::cVariable()
	{
		this->m_dbValue = 1;
	}
	cVariable::~cVariable()
	{
	}
	cVariable::cVariable(TiXmlElement*e_pElement)
	{
		ASSERT_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pElement,cVariable::TypeID);
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME("Name")
			{
				this->SetName(l_strValue);
			}
			else
			COMPARE_NAME("DefaultValue")
			{
				this->m_dbValue = VALUE_TO_DOUBLE;
			}
		PARSE_NAME_VALUE_END

	}

	TiXmlElement*	cVariable::ToTiXmlElement()
	{
		TiXmlElement*l_pTiXmlElement = new TiXmlElement(cVariable::TypeID);
		l_pTiXmlElement->SetAttribute(L"Name",this->GetName());
		l_pTiXmlElement->SetAttribute(L"DefaultValue",ValueToStringW(this->m_dbValue));
		return l_pTiXmlElement;
	}

	cMathFormulaXML::cMathFormulaXML()
	{
		m_pMUParser = new mu::Parser();
	}
	cMathFormulaXML::~cMathFormulaXML()
	{
		SAFE_DELETE(m_pMUParser);
	}

	void	cMathFormulaXML::ProcessVariable(TiXmlElement*e_pElement)
	{
		cVariable*l_pVariable = new cVariable(e_pElement);
		assert(this->GetObject(l_pVariable->GetName())==false&&"object exists!");
		this->AddObject(l_pVariable);
		m_pMUParser->DefineVar(l_pVariable->GetName(),&l_pVariable->m_dbValue);
	}
	//===============
	//do not use type postfix 
	//ex:1.f
	//===============
	//<cMathFormulaXML>
	//    <Equation Data="(Dis/Speed)/(1+(((60-20)/20*0.1))) " />
	//    <cVariable Name="Dis" DefaultValue="1.00" />
	//    <cVariable Name="Speed" DefaultValue="20.00" />
	//</cMathFormulaXML>
	bool	cMathFormulaXML::MyParse(TiXmlElement*e_pRoot)
	{
		this->Destroy();
		ASSERT_TARGET_ELEMENT_VALUE_WITH_DEFINE(e_pRoot,L"cMathFormulaXML");
		e_pRoot = e_pRoot->FirstChildElement();
		while( e_pRoot )
		{
			const wchar_t*l_strValue = e_pRoot->Value();
			COMPARE_VALUE("Equation")
			{
				const wchar_t*l_strEquation = e_pRoot->Attribute(L"Data");
				if( l_strEquation == nullptr )
					return false;
				m_pMUParser->SetExpr(l_strEquation);
			}
			else
			COMPARE_VALUE_WITH_DEFINE(cVariable::TypeID)
			{
				ProcessVariable(e_pRoot);
			}
			e_pRoot = e_pRoot->NextSiblingElement();
		}
#ifdef WIN32
		const mu::varmap_type l_varmap_type = m_pMUParser->GetVar();
		auto l_It = l_varmap_type.begin();
		for (; l_It != l_varmap_type.end(); ++l_It)
		{
			auto l_First = l_It->first;
			auto l_Second = l_It->second;
			int a=0;
		}
		try
		{
			double ll = m_pMUParser->Eval();
		}
		catch (mu::Parser::exception_type &e)
		{
			  std::wstring ll = e.GetMsg().c_str();
			  UT::ErrorMsg(this->m_strFileName,"equation or data error");
		}
#endif
		return true;
	}

	void	cMathFormulaXML::SetExpr(const std::wstring e_strExpr)
	{
		m_pMUParser->SetExpr(e_strExpr);
	}

	double*	cMathFormulaXML::AddDefineVariable(std::wstring e_strVariableName,double e_dbValue)
	{
		cVariable*l_pVariable = this->GetObject(e_strVariableName.c_str());
		if( l_pVariable == nullptr )
		{
			l_pVariable = new cVariable();
			l_pVariable->SetName(e_strVariableName.c_str());
			this->m_pMUParser->DefineVar(e_strVariableName.c_str(),&l_pVariable->m_dbValue);
			this->AddObjectNeglectExist(l_pVariable);
		}
		l_pVariable->m_dbValue = e_dbValue;
		return &l_pVariable->m_dbValue;
	}

	double	cMathFormulaXML::GetResult()
	{
		double l_dbResult = 0;
		try
		{
			l_dbResult = m_pMUParser->Eval();
		}
		catch (mu::Parser::exception_type &e)
		{
			  std::wstring ll = e.GetMsg().c_str();
			  int a=0;		
		}
		return l_dbResult;
	}

	bool	cMathFormulaXML::Export(const char*e_strFileName,bool e_bBinary)
	{
		TiXmlDocument l_Doc(UT::CharToWchar(e_strFileName).c_str());
		TiXmlElement*l_pRoot = new TiXmlElement(L"cMathFormulaXML");
		l_Doc.LinkEndChild(l_pRoot);
		{
			TiXmlElement*l_pEquationElement = new TiXmlElement(L"Equation");
			l_pEquationElement->SetAttribute(L"Data",this->m_pMUParser->GetExpr().c_str());
			l_pRoot->LinkEndChild(l_pEquationElement);
			int	l_iCount = this->Count();
			for (int i = 0; i < l_iCount; i++)
			{
				TiXmlElement*l_pElement = this->m_ObjectList[i]->ToTiXmlElement();
				l_pRoot->LinkEndChild(l_pElement);
			}
		}
		l_Doc.SaveFile(e_strFileName);
		return true;
	}
}
#endif