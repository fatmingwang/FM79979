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
	std::string	cPrtSizeInitSetSize::GetDataInfo()
	{
		sprintf(m_sTemp,"Size:x=%.2f,y=%.2f,rand:%.3f\0",x,y,m_fRandomValue);
		return m_sTemp;
	}
	std::string	cPrtSizeInitSetSize::GetOutputDataString()
	{
		sprintf(m_sTemp,"%.2f,%.2f,%.3f\0",x,y,m_fRandomValue);
		return m_sTemp;
	}
	//input the output data string,and analyze it
	bool	cPrtSizeInitSetSize::SetDataByDataString(const char*e_pString)
	{
		char*	l_pForStrtok_s = nullptr;
		char*   l_strValue = (char*)alloca(strlen(e_pString));
		sprintf(l_strValue, "%s", e_pString);
		char* l_pString = strtok_s(l_strValue, ",",&l_pForStrtok_s);
		x = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		y = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		if( l_pString )
		{
			m_fRandomValue = (float)atof(l_pString);
		}
		if( strtok_s(nullptr,",", &l_pForStrtok_s) )
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
	std::string	cPrtSizeActBlending::GetDataInfo()
	{

		sprintf(m_sTemp,"Add:%s\n:x=%.2f,y=%.2f\0",m_bAdd?"true":"false",x,y);
		return m_sTemp;
	}
	std::string	cPrtSizeActBlending::GetOutputDataString()
	{
		sprintf(m_sTemp,"%.2f,%.2f,%d\0",x,y,m_bAdd?1:0);
		return m_sTemp;
	}
	//input the output data string,and analyze it
	bool	cPrtSizeActBlending::SetDataByDataString(const char*e_pString)
	{
		char*	l_pForStrtok_s = nullptr;
		char*   l_strValue = (char*)alloca(strlen(e_pString));
		sprintf(l_strValue, "%s", e_pString);
		char* l_pString = strtok_s(l_strValue, ",",&l_pForStrtok_s);
		x = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		y = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		m_bAdd = atoi(l_pString)?true:false;
		if( strtok_s(nullptr,",", &l_pForStrtok_s) )
			return false;
		return true;
	}
}