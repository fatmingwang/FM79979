#pragma once
#include "../XML/XMLLoader.h"
#include "../NamedTypedObject.h"
#include "../TemplateClass.h"
namespace mu
{
	class Parser;
}

namespace FATMING_CORE
{
//<cMathFormulaXML>
//	<Equation Data=""/>
//	<Variable Name="" DefaultValue="" />
//	<Variable Name="" DefaultValue="" />
//	<Variable Name="" DefaultValue="" />
//</cMathFormulaXML>

	class cVariable:public NamedTypedObject
	{
	public:
		cVariable();
		cVariable(TiXmlElement*e_pElement);
		virtual ~cVariable();
		DEFINE_TYPE_INFO();
		virtual	TiXmlElement*		ToTiXmlElement();
		double m_dbValue;
	};
	//===============
	//do not use type postfix 
	//ex:1.f
	//===============
	class cMathFormulaXML:public cNodeISAX,public cNamedTypedObjectVector<cVariable>
	{
		void			ProcessVariable(TiXmlElement*e_pElement);
		virtual	bool	MyParse(TiXmlElement*e_pRoot);
		mu::Parser*		m_pMUParser;
	public:
		cMathFormulaXML();
		~cMathFormulaXML();
		//mu::Parser*		GetParser(){return &m_MUParser;}
		double			GetResult();
		//get variable value.
		double*			AddDefineVariable(std::wstring e_strVariableName,double e_dbValue);
		void			SetExpr(const std::wstring e_strExpr);
		bool			Export(const char*e_strFileName,bool e_bBinary = false);
	};

	//inline double MySqr(double a_fVal) 
	//{ 
	//  return a_fVal*a_fVal; 
	//}
	//inline void MathParserTest()
	//{
	//	try
	//	{
	//		double fVal = 1;
	//		mu::Parser p;
	//		p.DefineVar(L"a", &fVal); 
	//		p.DefineFun(L"MySqr", MySqr); 
	//		p.SetExpr(L"MySqr(a)*_pi+min(10,a)");
	//		//p.GetExpr();

	//		for (std::size_t a=0; a<100; ++a)
	//		{
	//		  fVal = a;  // Change value of variable a
	//		  double ll = p.Eval();
	//		  int aa=0;
	//		}
	//	}
	//	catch (mu::Parser::exception_type &e)
	//	{
	//		  std::wstring ll = e.GetMsg().c_str();
	//		  int a=0;
	//	}	
	//}
}