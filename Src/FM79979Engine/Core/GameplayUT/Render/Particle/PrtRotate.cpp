#include "ParticleBase.h"
#include "PrtRotate.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cPrtRotateActRotate);
	TYPDE_DEFINE_MARCO(cPrtRotateInitRotate);

	char*	cPrtRotateActRotate::UsageExplanation =
	{
		//"while ParticleRotateMode is none this should be erase,random value as big as effective,if Negative is true the random will decide by -m_fRandomOffset and m_fRandomOffset,or 0,m_fRandomOffset\n��ParticleRotateMode �� none ���������ӧR��,�üƭȶV�j����V��,��Negative ���u���üƼƭȱN�|����-m_fRandomOffset,m_fRandomOffset�_�h��0,m_fRandomOffset"
		"while ParticleRotateMode is none this should be erase,random value as big as effective,if Negative is true the random will decide by -m_fRandomOffset and m_fRandomOffset,or 0,m_fRandomOffset\n"
	};
	char*	cPrtRotateInitRotate::UsageExplanation =
	{
		"adjust start angle as you like"
	};

	NamedTypedObject*  cPrtRotateInitRotate::Clone()
	{
		cPrtRotateInitRotate* l_p = new cPrtRotateInitRotate; 
		l_p->SetAngle(m_vAngle);
		l_p->SetName(GetName());
		return l_p; 
	};
	std::string	cPrtRotateInitRotate::GetDataInfo()
	{
		sprintf(m_sTemp,"Angle:x=%.4f,y=%.4f,z=%.4f\0",m_vAngle.x,m_vAngle.y,m_vAngle.z);
		return m_sTemp;
	}
	std::string	cPrtRotateInitRotate::GetOutputDataString()
	{
		sprintf(m_sTemp,"%.4f,%.4f,%.4f\0",m_vAngle.x,m_vAngle.y,m_vAngle.z);
		return m_sTemp;
	}
	//input the output data string,and analyze it
	bool	cPrtRotateInitRotate::SetDataByDataString(const char*e_pString)
	{
		char*	 l_pForStrtok_s = nullptr;
		char*   l_strValue = (char*)alloca(strlen(e_pString));
		sprintf(l_strValue, "%s", e_pString);
		char* l_pString = strtok_s(l_strValue, ",", &l_pForStrtok_s);
		this->m_vAngle.x = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		this->m_vAngle.y = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		this->m_vAngle.z = (float)atof(l_pString);
		if( strtok_s(nullptr,",", &l_pForStrtok_s) )
			return false;
		return true;
	}
	NamedTypedObject*  cPrtRotateActRotate::Clone()
	{
		cPrtRotateActRotate* l_p = new cPrtRotateActRotate; 
		l_p->SetRandom(m_bRandom);
		l_p->SetRandomOffset(m_fRandomOffset);
		l_p->SetAngle(m_vAngle);
		l_p->SetName(GetName());
		l_p->SetPRM( m_ePRM );
		return l_p; 
	};
	std::string	cPrtRotateActRotate::GetDataInfo()
	{
		sprintf(m_sTemp,"Angle:x=%.4f,y=%.4f,z=%.4f\nRandom:%d\nRandomOffset:%.4f,Mode:%d\0",m_vAngle.x,m_vAngle.y,m_vAngle.z,m_bRandom?1:0,m_fRandomOffset,(int)this->m_ePRM);
		return m_sTemp;
	}
	std::string	cPrtRotateActRotate::GetOutputDataString()
	{
		sprintf(m_sTemp,"%.4f,%.4f,%.4f,%d,%.4f,%d\0",m_vAngle.x,m_vAngle.y,m_vAngle.z,m_bRandom?1:0,m_fRandomOffset,(int)this->m_ePRM);
		return m_sTemp;
	}
	//input the output data string,and analyze it
	bool	cPrtRotateActRotate::SetDataByDataString(const char*e_pString)
	{
		char*	l_pForStrtok_s = nullptr;
		char*   l_strValue = (char*)alloca(strlen(e_pString));
		sprintf(l_strValue, "%s", e_pString);
		char* l_pString = strtok_s(l_strValue, ",",&l_pForStrtok_s);
		this->m_vAngle.x = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		this->m_vAngle.y = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		this->m_vAngle.z = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		this->m_bRandom = atoi(l_pString)?true:false;
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		this->m_fRandomOffset = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		this->m_ePRM = (eParticleRotateMode)atoi(l_pString);
		if( strtok_s(nullptr,",", &l_pForStrtok_s) )
			return false;
		return true;
	}
//end namespace FATMING_CORE
}