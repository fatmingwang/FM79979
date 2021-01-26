#include "ParticleBase.h"
#include "PrtSize.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cPrtSizeActBlending);
	TYPDE_DEFINE_MARCO(cPrtSizeInitSetSize);

#ifdef WIN32
	char*	cPrtSizeInitSetSize::UsageExplanation =
	{
		"size will decide by size *random(0,RandomValue),RandomValue"
	};
	char*	cPrtSizeActBlending::UsageExplanation =
	{
		"while Add is active,size is going to add specific,I.E\nAdd"
	};
#else
	char*	cPrtSizeInitSetSize::UsageExplanation =
	{
		"size will decide by size *random(0,RandomValue)"
	};
	char*	cPrtSizeActBlending::UsageExplanation =
	{
		"while Add is active,size is going to add specific,I.E\n"
	};
#endif
	NamedTypedObject*  cPrtSizeInitSetSize::Clone()
	{
		cPrtSizeInitSetSize* l_p = new cPrtSizeInitSetSize; 
		l_p->SetName(GetName());
		l_p->SetSize(x,y);
		l_p->m_fRandomValue = m_fRandomValue;
		return l_p; 
	};
	char*	cPrtSizeInitSetSize::GetDataInfo()
	{
		sprintf(m_sTemp,"Size:x=%.2f,y=%.2f,rand:%.3f\0",x,y,m_fRandomValue);
		return m_sTemp;
	}
	char*	cPrtSizeInitSetSize::GetOutputDataString()
	{
		sprintf(m_sTemp,"%.2f,%.2f,%.3f\0",x,y,m_fRandomValue);
		return m_sTemp;
	}
	//input the output data string,and analyze it
	bool	cPrtSizeInitSetSize::SetDataByDataString(char*e_pString)
	{
		char*   l_strValue = (char*)alloca(strlen(e_pString));
		sprintf(l_strValue, "%s", e_pString);
		char* l_pString = strtok(l_strValue, ",");
		x = (float)atof(l_pString);
		l_pString = strtok(0,",");
		y = (float)atof(l_pString);
		l_pString = strtok(0,",");
		if( l_pString )
		{
			m_fRandomValue = (float)atof(l_pString);
		}
		if( strtok(0,",") )
			return false;
		return true;
	}

	NamedTypedObject*  cPrtSizeActBlending::Clone()
	{
		cPrtSizeActBlending* l_p = new cPrtSizeActBlending; 
		l_p->SetAddOrSubtract(m_bAdd);
		l_p->SetSize(x,y);
		l_p->SetName(GetName());
		return l_p; 
	};
	char*	cPrtSizeActBlending::GetDataInfo()
	{

		sprintf(m_sTemp,"Add:%s\n:x=%.2f,y=%.2f\0",m_bAdd?"true":"false",x,y);
		return m_sTemp;
	}
	char*	cPrtSizeActBlending::GetOutputDataString()
	{
		sprintf(m_sTemp,"%.2f,%.2f,%d\0",x,y,m_bAdd?1:0);
		return m_sTemp;
	}
	//input the output data string,and analyze it
	bool	cPrtSizeActBlending::SetDataByDataString(char*e_pString)
	{
		char*   l_strValue = (char*)alloca(strlen(e_pString));
		sprintf(l_strValue, "%s", e_pString);
		char* l_pString = strtok(l_strValue, ",");
		x = (float)atof(l_pString);
		l_pString = strtok(0,",");
		y = (float)atof(l_pString);
		l_pString = strtok(0,",");
		m_bAdd = atoi(l_pString)?true:false;
		if( strtok(0,",") )
			return false;
		return true;
	}
}