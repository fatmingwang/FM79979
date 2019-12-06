#include "ParticleBase.h"
#include "PrtRotate.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cPrtRotateActRotate);
	TYPDE_DEFINE_MARCO(cPrtRotateInitRotate);

	char*	cPrtRotateActRotate::UsageExplanation =
	{
		//"while ParticleRotateMode is none this should be erase,random value as big as effective,if Negative is true the random will decide by -m_fRandomOffset and m_fRandomOffset,or 0,m_fRandomOffset\n當ParticleRotateMode 為 none 此物件應該刪除,亂數值越大旋轉越快,當Negative 為真那亂數數值將會介於-m_fRandomOffset,m_fRandomOffset否則為0,m_fRandomOffset"
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
	char*	cPrtRotateInitRotate::GetDataInfo()
	{
		sprintf(m_sTemp,"Angle:x=%.4f,y=%.4f,z=%.4f\0",m_vAngle.x,m_vAngle.y,m_vAngle.z);
		return m_sTemp;
	}
	char*	cPrtRotateInitRotate::GetOutputDataString()
	{
		sprintf(m_sTemp,"%.4f,%.4f,%.4f\0",m_vAngle.x,m_vAngle.y,m_vAngle.z);
		return m_sTemp;
	}
	//input the output data string,and analyze it
	bool	cPrtRotateInitRotate::SetDataByDataString(char*e_pString)
	{
		char*Nexttoken = 0;
		char* l_pString = strtok(e_pString,",");
		this->m_vAngle.x = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_vAngle.y = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_vAngle.z = (float)atof(l_pString);
		if( strtok(0,",") )
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
	char*	cPrtRotateActRotate::GetDataInfo()
	{
		sprintf(m_sTemp,"Angle:x=%.4f,y=%.4f,z=%.4f\nRandom:%d\nRandomOffset:%.4f,Mode:%d\0",m_vAngle.x,m_vAngle.y,m_vAngle.z,m_bRandom?1:0,m_fRandomOffset,(int)this->m_ePRM);
		return m_sTemp;
	}
	char*	cPrtRotateActRotate::GetOutputDataString()
	{
		sprintf(m_sTemp,"%.4f,%.4f,%.4f,%d,%.4f,%d\0",m_vAngle.x,m_vAngle.y,m_vAngle.z,m_bRandom?1:0,m_fRandomOffset,(int)this->m_ePRM);
		return m_sTemp;
	}
	//input the output data string,and analyze it
	bool	cPrtRotateActRotate::SetDataByDataString(char*e_pString)
	{
		char*Nexttoken = 0;
		char* l_pString = strtok(e_pString,",");
		this->m_vAngle.x = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_vAngle.y = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_vAngle.z = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_bRandom = atoi(l_pString)?true:false;
		l_pString = strtok(0,",");
		this->m_fRandomOffset = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_ePRM = (eParticleRotateMode)atoi(l_pString);
		if( strtok(0,",") )
			return false;
		return true;
	}
//end namespace FATMING_CORE
}