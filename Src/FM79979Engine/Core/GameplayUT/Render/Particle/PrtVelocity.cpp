#include "ParticleBase.h"
#include "PrtVelocity.h"
namespace FATMING_CORE
{

	TYPDE_DEFINE_MARCO(cPrtVelocityInitSetVelocity);
	TYPDE_DEFINE_MARCO(cPrtVelocityActAcceleration);
	TYPDE_DEFINE_MARCO(cPrtVelocityActBySatelliteAction);
	TYPDE_DEFINE_MARCO(cPrtVelocityActDircctionChange);
#ifdef WIN32
	char*	cPrtVelocityInitSetVelocity::UsageExplanation =
	{
		"particle velocity"
	};
	char*	cPrtVelocityActAcceleration::UsageExplanation =
	{
		"current velocity = velocity+AccelerationSpeed*Direction+OriginalSpeed"
	};
#else
	char*	cPrtVelocityInitSetVelocity::UsageExplanation =
	{
		"particle velocity\n"
	};
	char*	cPrtVelocityActAcceleration::UsageExplanation =
	{
		"current velocity = velocity+AccelerationSpeed*Direction+OriginalSpeed\n"
	};
#endif
	char*	cPrtVelocityActBySatelliteAction::UsageExplanation =
	{
		"lazy to do"
	};


	char*	cPrtVelocityActDircctionChange::UsageExplanation =
	{
		"stop is reached particle's velocity would be zero,and new Speed will be apply,and if Accesleration is checked it would be true"
	};

	NamedTypedObject*  cPrtVelocityInitSetVelocity::Clone()
	{
		cPrtVelocityInitSetVelocity* l_p = new cPrtVelocityInitSetVelocity; 
		l_p->SetVelocity(this->m_vVeolcity);
		l_p->SetRandom(m_bRandom);
		l_p->SetRandomOffSet(m_fRandomOffSet);
		l_p->SetName(GetName());
		return l_p; 
	};

	std::string	cPrtVelocityInitSetVelocity::GetOutputDataString()
	{
		sprintf(m_sTemp,"%.2f,%.2f,%.2f,%d,%.2f\0",m_vVeolcity.x,m_vVeolcity.y,m_vVeolcity.z,m_bRandom?1:0,m_fRandomOffSet);
		return m_sTemp;
	}

	std::string	cPrtVelocityInitSetVelocity::GetDataInfo()
	{
		sprintf(m_sTemp,"Velocity:x=%.2f,y=%.2f,z=%.2f\nRandom:%s\nRandomOffset:%.2f\0",m_vVeolcity.x,m_vVeolcity.y,m_vVeolcity.z,m_bRandom?"true":"false",m_fRandomOffSet);
		return m_sTemp;
	}

	//input the output data string,and analyze it
	bool	cPrtVelocityInitSetVelocity::SetDataByDataString(const char*e_pString)
	{
		char*	l_pForStrtok_s = nullptr;
		char*   l_strValue = (char*)alloca(strlen(e_pString));
		sprintf(l_strValue, "%s", e_pString);
		char* l_pString = strtok_s(l_strValue, ",", &l_pForStrtok_s);
		m_vVeolcity.x = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		m_vVeolcity.y = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		m_vVeolcity.z = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		m_bRandom = atoi(l_pString)?true:false;
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		m_fRandomOffSet = (float)atof(l_pString);
		if( strtok_s(nullptr,",", &l_pForStrtok_s) )
			return false;
		return true;
	}



	//NamedTypedObject*  cPrtVelocityInitRandomFly::Clone()
	//{
	//	cPrtVelocityInitRandomFly* l_p = new cPrtVelocityInitRandomFly; 
	//	l_p->SetDir(this->m_vDir);
	//	l_p->SetDistance(this->m_fDistance);
	//	l_p->SetName(GetName());
	//	l_p->SetRandom(m_bRandom);
	//	return l_p; 
	//}
	//std::string	cPrtVelocityInitRandomFly::GetDataInfo()
	//{
	//	sprintf_s(m_sTemp,MAX_PATH,"Dir:x=%.2f,y=%.2f,z=%.2f\nRandom:%s\nDistance:%.2f\0",m_vDir.x,m_vDir.y,m_vDir.z,m_bRandom?"true":"false",m_fDistance);
	//	return m_sTemp;
	//}
	//char*	cPrtVelocityInitRandomFly::GetOutputDataString()
	//{
	//	sprintf_s(m_sTemp,MAX_PATH,"%.2f,%.2f,%.2f,%d,%.2f\0",m_vDir.x,m_vDir.y,m_vDir.z,m_bRandom?1:0,m_fDistance);
	//	return m_sTemp;
	//}
	////input the output data string,and analyze it
	//bool	cPrtVelocityInitRandomFly::SetDataByDataString(const char*e_pString)
	//{
	//	char *Nexttoken = 0;
	//	char* l_pString = strtok_s(e_pString,",");
	//	m_vDir.x = (float)atof(l_pString);
	//	l_pString = strtok_s(nullptr,",");
	//	m_vDir.y = (float)atof(l_pString);
	//	l_pString = strtok_s(nullptr,",");
	//	m_vDir.z = (float)atof(l_pString);
	//	l_pString = strtok_s(nullptr,",");
	//	m_bRandom = atoi(l_pString)?true:false;
	//	l_pString = strtok_s(nullptr,",");
	//	m_fDistance = (float)atof(l_pString);
	//	if( strtok_s(nullptr,",") )
	//		return false;
	//	return true;
	//}


	NamedTypedObject*  cPrtVelocityActAcceleration::Clone()
	{
		cPrtVelocityActAcceleration*l_pPrtVelocityActAcceleration = new cPrtVelocityActAcceleration();
		l_pPrtVelocityActAcceleration->SetAccelerationSpeed(m_fAccelerationSpeed);
		return l_pPrtVelocityActAcceleration;
	}

	std::string	cPrtVelocityActAcceleration::GetOutputDataString()
	{
		sprintf(m_sTemp,"%.5f\0",m_fAccelerationSpeed);
		return m_sTemp;
	}

	std::string	cPrtVelocityActAcceleration::GetDataInfo()
	{
		sprintf(m_sTemp,"Acceleration:%.5f\0",m_fAccelerationSpeed);
		return m_sTemp;		
	}

	//input the output data string,and analyze it
	bool	cPrtVelocityActAcceleration::SetDataByDataString(const char*e_pString)
	{
		char*	l_pForStrtok_s = nullptr;
		char*   l_strValue = (char*)alloca(strlen(e_pString));
		sprintf(l_strValue, "%s", e_pString);
		char* l_pString = strtok_s(l_strValue, ",",&l_pForStrtok_s);
		m_fAccelerationSpeed = (float)atof(l_pString);
		if( strtok_s(nullptr,",", &l_pForStrtok_s) )
			return false;
		return true;		
	}

	NamedTypedObject*  cPrtVelocityActBySatelliteAction::Clone()
	{
		cPrtVelocityActBySatelliteAction* l_p = new cPrtVelocityActBySatelliteAction; 
		l_p->SetPivot(this->m_vPivot);
		l_p->SetRadius(m_fRadius);
		l_p->SetSpeed(this->m_fSpeed);
		l_p->SetTrackLength(this->m_fTrackLength);
		l_p->SetName(GetName());
		return l_p;
	};



		//
	NamedTypedObject*  cPrtVelocityActDircctionChange::Clone()
	{
		cPrtVelocityActDircctionChange* l_p = new cPrtVelocityActDircctionChange; 
		l_p->SetStopTime(m_vStopTime);
		l_p->SetNewSpeed(m_vNewSpeed);
		l_p->SetAccesleration(m_bAccesleration);
		l_p->SetName(GetName());
		return l_p;	
	}
	//
	std::string	cPrtVelocityActDircctionChange::GetOutputDataString()
	{
		sprintf(m_sTemp,"%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%d\0",m_vStopTime.x,m_vStopTime.y,m_vStopTime.z,m_vNewSpeed.x,m_vNewSpeed.y,m_vNewSpeed.z,m_bAccesleration?1:0);
		return m_sTemp;
	}
	//
	std::string	cPrtVelocityActDircctionChange::GetDataInfo()
	{
		sprintf(m_sTemp,"StopTime:%.3f,%.3f,%.3f\n,NewSpeed:%.3f,%.3f,%.3f\nAccesleration:%d\0",m_vStopTime.x,m_vStopTime.y,m_vStopTime.z,m_vNewSpeed.x,m_vNewSpeed.y,m_vNewSpeed.z,m_bAccesleration?1:0);
		return m_sTemp;	
	}
	//input the output data string,and analyze it
	bool	cPrtVelocityActDircctionChange::SetDataByDataString(const char*e_pString)
	{
		char*	l_pForStrtok_s = nullptr;
		char*   l_strValue = (char*)alloca(strlen(e_pString));
		sprintf(l_strValue, "%s", e_pString);
		char* l_pString = strtok_s(l_strValue, ",", &l_pForStrtok_s);
		m_vStopTime.x = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		m_vStopTime.y = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		m_vStopTime.z = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		m_vNewSpeed.x = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		m_vNewSpeed.y = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		m_vNewSpeed.z = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		m_bAccesleration = atoi(l_pString)?true:false;
		if( strtok_s(nullptr,",", &l_pForStrtok_s) )
			return false;
		return true;	
	}



	std::string	cPrtVelocityActBySatelliteAction::GetDataInfo()
	{
		sprintf(m_sTemp,"Radius:%.2f\nSpeed:%.2f\nTrackLength:%.2f\nPivot:x=%.2f,y=%.2f,z=%.2f\0",m_fRadius,m_fSpeed,m_fTrackLength,m_vPivot.x,m_vPivot.y,m_vPivot.z);
		return m_sTemp;
	}

	std::string	cPrtVelocityActBySatelliteAction::GetOutputDataString()
	{
		sprintf(m_sTemp,"%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\0",m_fRadius,m_fSpeed,m_fTrackLength,m_vPivot.x,m_vPivot.y,m_vPivot.z);
		return m_sTemp;
	}
	//input the output data string,and analyze it
	bool	cPrtVelocityActBySatelliteAction::SetDataByDataString(const char*e_pString)
	{
		char* l_pForStrtok_s = nullptr;
		char*   l_strValue = (char*)alloca(strlen(e_pString));
		sprintf(l_strValue, "%s", e_pString);
		char* l_pString = strtok_s(l_strValue, "," ,&l_pForStrtok_s);
		this->m_fRadius = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		this->m_fSpeed = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		this->m_fTrackLength = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		this->m_vPivot.x = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		this->m_vPivot.y = (float)atof(l_pString);
		l_pString = strtok_s(nullptr,",", &l_pForStrtok_s);
		this->m_vPivot.z = (float)atof(l_pString);
		if( strtok_s(nullptr,",", &l_pForStrtok_s) )
			return false;
		return true;
	}
//end namespace FATMING_CORE
}