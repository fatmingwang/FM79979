#pragma once
#include <vector>


#define CONCAT2(a, b) a ## b
//#define CONCAT(a, b) CONCAT2(a, b)

#define		GET_VARIABLE_NAME(VARIABLE)std::wstring CONCAT2(Get,VARIABLE)Name(){ return L#VARIABLE; }

#define	GET_FLOAT_DATA_FROM_ATTRIBUTE(VariableName,Attribute)\
private:	float m_f##VariableName;\
public: float	Get##VariableName()\
{\
	if( m_f##VariableName == -1.f )\
	{\
		std::wstring l_strValue = this->getValue(Attribute);\
		if( l_strValue.length() > 0 )\
		{\
			m_f##VariableName = GetFloat(l_strValue);\
		}\
	}\
	return m_f##VariableName;\
}\
GET_VARIABLE_NAME(VariableName)


#define	GET_INT_DATA_FROM_ATTRIBUTE(VariableName,Attribute)\
private:	int m_i##VariableName;\
public: int	Get##VariableName()\
{\
	if( m_i##VariableName == -1 )\
	{\
		std::wstring l_strValue = this->getValue(Attribute);\
		if( l_strValue.length() > 0 )\
		{\
			m_i##VariableName = GetInt(l_strValue);\
		}\
	}\
	return m_i##VariableName;\
}\
GET_VARIABLE_NAME(VariableName)



struct sXmlNode
{
	cLinerDataProcessor<Vector3>*	m_pLineData;
	sXmlNode(const sXmlNode &e_pXmlNode)
	{
		m_pLineData = new cLinerDataProcessor<Vector3>(e_pXmlNode.m_pLineData);
		strAttributeVector = e_pXmlNode.strAttributeVector;
		strValueVector = e_pXmlNode.strValueVector;
	}
	sXmlNode()
	{
		m_pLineData = new cLinerDataProcessor<Vector3>();
	}
	~sXmlNode()
	{
		SAFE_DELETE(m_pLineData); 
	}
	std::vector<std::wstring> strAttributeVector;
	std::vector<std::wstring> strValueVector;
	std::wstring	getNameValue();
	std::wstring	getValue(const wchar_t*e_str);
	int				getInt(const wchar_t*e_str);
	void			StroeAllAttribute(TiXmlElement*e_pRoot);
	//if same attribute new one will be ingore
	void			Merge(TiXmlElement*e_pRoot);
};

class cChartWithName
{
public:
	Vector2 m_vMaxValue;
	bool	m_bShowValue;
	int		m_ObjectIndexDistance;
	float	m_fHeightScale;
	cChartWithName();
	~cChartWithName();
	struct sNameAndData:public sXmlNode
	{
		std::wstring					m_Name;
		Vector2							m_vMaxValue;
		sNameAndData(){m_vMaxValue.x = m_vMaxValue.y = 0.f;}
	};
	std::vector<int>					m_RenderIndex;
	std::vector<sNameAndData*>			m_LineDataVector;
	void								AddData(sNameAndData* e_pNameAndData);
	void								Render();
	Vector2								GetMaxValue(int e_iIndex);
	Vector2								GetMaxValue();
};