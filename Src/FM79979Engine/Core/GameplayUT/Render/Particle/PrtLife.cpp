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

	char*	cPrtLifeInitrSetLife::GetDataInfo()
	{

		sprintf(m_sTemp,"LifeMin:=%.2f\nLifeRange:%.2f\nRandom:%s\0",m_fLifeMin,m_fLifeRange,m_bRandom?"true":"false");
		return m_sTemp;
	}
	//this one for output data value by "," to separate value
	char*	cPrtLifeInitrSetLife::GetOutputDataString()
	{
		sprintf(m_sTemp,"%.2f,%.2f,%d\0",m_fLifeMin,m_fLifeRange,m_bRandom?1:0);
		return m_sTemp;
	}
	//input the output data string,and analyze it
	bool	cPrtLifeInitrSetLife::SetDataByDataString(char*e_pString)
	{
		char*Nexttoken = 0;
		char* l_pString = strtok(e_pString,",");
		this->m_fLifeMin = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_fLifeRange = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_bRandom = atoi(l_pString)?true:false;
		if( strtok(0,",") )
			return false;
		return true;
	}
}