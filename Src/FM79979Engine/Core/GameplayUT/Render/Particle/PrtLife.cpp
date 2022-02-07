#include "ParticleBase.h"
#include "PrtLife.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cPrtLifeInitrSetLife);
	TYPDE_DEFINE_MARCO(cPrtLifeActDyingByGameTime);
#ifdef WIN32
	char*	cPrtLifeInitrSetLife::UsageExplanation =
	{
		"LifeMin for the base file,\nLifeRange will add with baselife,\nif Random is active the randge will fetch a random betwnn 0 to m_fLifeRange"

	};
	char*	cPrtLifeActDyingByGameTime::UsageExplanation =
	{
		"life time minuse elpase time"
	};
#else
	char*	cPrtLifeInitrSetLife::UsageExplanation =
	{
		"LifeMin for the base file,\nLifeRange will add with baselife,\nif Random is active the randge will fetch a random betwnn 0 to m_fLifeRange\n"

	};
	char*	cPrtLifeActDyingByGameTime::UsageExplanation =
	{
		"life time minuse elpase time"
	};
#endif
	NamedTypedObject*  cPrtLifeInitrSetLife::Clone()
	{
		cPrtLifeInitrSetLife* l_p = new cPrtLifeInitrSetLife; 
		l_p->SetLifeMin(this->m_fLifeMin);
		l_p->SetLifeRange(this->m_fLifeRange);
		l_p->SetRandom(this->m_bRandom);
		l_p->SetName(GetName());
		return l_p; 
	};

	std::string	cPrtLifeInitrSetLife::GetDataInfo()
	{

		sprintf(m_sTemp,"LifeMin:=%.2f\nLifeRange:%.2f\nRandom:%s",m_fLifeMin,m_fLifeRange,m_bRandom?"true":"false");
		return m_sTemp;
	}
	//this one for output data value by "," to separate value
	std::string	cPrtLifeInitrSetLife::GetOutputDataString()
	{
		sprintf(m_sTemp,"%.2f,%.2f,%d\0",m_fLifeMin,m_fLifeRange,m_bRandom?1:0);
		return m_sTemp;
	}
	//input the output data string,and analyze it
	bool	cPrtLifeInitrSetLife::SetDataByDataString(const char*e_pString)
	{
		char*	l_pForStrtok_s = nullptr;
		char*   l_strValue = (char*)alloca(strlen(e_pString));
		sprintf(l_strValue, "%s", e_pString);
		char* l_pString = strtok_s(l_strValue, ",",&l_pForStrtok_s);
		this->m_fLifeMin = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		this->m_fLifeRange = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		this->m_bRandom = atoi(l_pString)?true:false;
		if( strtok_s(nullptr,",", &l_pForStrtok_s) )
			return false;
		return true;
	}
}