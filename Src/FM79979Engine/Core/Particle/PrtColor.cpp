#include "../stdafx.h"
#include "ParticleBase.h"
#include "PrtColor.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cPrtColorInitrSetColor);
	TYPDE_DEFINE_MARCO(cPrtColorInitrSetRandomColor);
	TYPDE_DEFINE_MARCO(cPrtColorActBlending);
	TYPDE_DEFINE_MARCO(cPrtColorActBlendingByLife);
	TYPDE_DEFINE_MARCO(cPrtColorActBlendingBy2Color);

	char*	cPrtColorInitrSetColor::UsageExplanation =
	{
		"RGBA for color from 0 to 1"
	};
#ifdef WIN32
	char*	cPrtColorInitrSetRandomColor::UsageExplanation =
	{
		"as RandomValue close 1 as effective\nRandomValue �V�a��1�C���ܤƶV�j"
	};
	char*	cPrtColorActBlending::UsageExplanation =
	{
		"if m_bFade is true per sceond will minus color u input else add\n�p�Gfade�O����C�����N�|�����A�ҿ�J���C��Ϥ����W�["
	};
	char*	cPrtColorActBlendingByLife::UsageExplanation =
	{
		"color will become what u specific when time passing\n�C��|�C�C�ܦ����w���ƭȮɶ��������ɭԷ|�ܦ��A���w���ˤl"
	};
	char*	cPrtColorActBlendingBy2Color::UsageExplanation =
	{
		"2 setp color changing\n�ⶥ�q���C���ܴ�"
	};
#else
	char*	cPrtColorInitrSetRandomColor::UsageExplanation =
	{
		"as RandomValue close 1 as effective\nRandomValue"
	};
	char*	cPrtColorActBlending::UsageExplanation =
	{
		"if m_bFade is true per sceond will minus color u input else add\n"
	};
	char*	cPrtColorActBlendingByLife::UsageExplanation =
	{
		"color will become what u specific when time passing\n"
	};
	char*	cPrtColorActBlendingBy2Color::UsageExplanation =
	{
		"2 setp color changing\n"
	};
#endif
	//================================================================
	//here divide two part one part is implement data,
	//another part is interface for transform the current interface to wanted by ChangeProperty(  Data.p and Data.cpp )
	//================================================================
	NamedTypedObject*  cPrtColorInitrSetColor::Clone()
	{
		cPrtColorInitrSetColor* l_p = new cPrtColorInitrSetColor; 
		l_p->SetName(GetName());
		l_p->SetColor(m_vColor);
		return l_p; 
	};
	char*	cPrtColorInitrSetColor::GetDataInfo()
	{
		sprintf(m_sTemp,"Color:r=%.2f,g=%.2f,b=%.2f,a=%.2f\0",m_vColor.r,m_vColor.g,m_vColor.b,m_vColor.a);
		return m_sTemp;
	}
	//this one for output data value by "," to separate value
	char*	cPrtColorInitrSetColor::GetOutputDataString()
	{
		sprintf(m_sTemp,"%.5f,%.5f,%.5f,%.5f\0",m_vColor.r,m_vColor.g,m_vColor.b,m_vColor.a);
		return m_sTemp;
	}
	//input the output data string,and analyze it
	bool	cPrtColorInitrSetColor::SetDataByDataString(char*e_pString)
	{
		char* l_pString = strtok(e_pString,",");
		this->m_vColor.r = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_vColor.g = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_vColor.b = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_vColor.a = (float)atof(l_pString);
		if( strtok(0,",") )
			return false;
		return true;
	}
	
	//RandomwValue = FloatValue
	NamedTypedObject*  cPrtColorInitrSetRandomColor::Clone()
	{
		cPrtColorInitrSetRandomColor* l_p = new cPrtColorInitrSetRandomColor; 
		l_p->SetColor(m_vColor);
		l_p->SetName(GetName());
		return l_p; 
	};
	char*	cPrtColorInitrSetRandomColor::GetDataInfo()
	{
		sprintf(m_sTemp,"Color:r=%.5f,g=%.5f,b=%.5f,a=%.5f\n\0",m_vColor.r,m_vColor.g,m_vColor.b,m_vColor.a);
		return m_sTemp;
	}
	//this one for output data value by "," to separate value
	char*	cPrtColorInitrSetRandomColor::GetOutputDataString()
	{
		sprintf(m_sTemp,"%.5f,%.5f,%.5f,%.5f\0",m_vColor.r,m_vColor.g,m_vColor.b,m_vColor.a);
		return m_sTemp;
	}
	//input the output data string,and analyze it
	bool	cPrtColorInitrSetRandomColor::SetDataByDataString(char*e_pString)
	{
		char* l_pString = strtok(e_pString,",");
		this->m_vColor.r = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_vColor.g = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_vColor.b = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_vColor.a = (float)atof(l_pString);
		if( strtok(0,",") )
			return false;
		return true;
	}

	NamedTypedObject*  cPrtColorActBlending::Clone()
	{
		cPrtColorActBlending* l_p = new cPrtColorActBlending; 
		l_p->SetColor(m_vColor);
		l_p->SetFade(this->m_bFade);
		l_p->SetName(GetName());
		return l_p; 
	}

	NamedTypedObject*  cPrtColorActBlendingByLife::Clone()
	{
		cPrtColorActBlendingByLife* l_p = new cPrtColorActBlendingByLife;
		l_p->SetColor(m_vColor);
		l_p->SetFade(this->m_bFade);
		l_p->SetName(GetName());
		return l_p; 	
	}
    
	char*	cPrtColorActBlending::GetDataInfo()
	{
		sprintf(m_sTemp,"Color:r=%.5f,g=%.5f,b=%.5f,a=%.5f\nFade:%d\0",m_vColor.r,m_vColor.g,m_vColor.b,m_vColor.a,m_bFade?1:0);
		return m_sTemp;
	}
	//this one for output data value by "," to separate value
	char*	cPrtColorActBlending::GetOutputDataString()
	{
		sprintf(m_sTemp,"%.5f,%.5f,%.5f,%.5f,%d\0",m_vColor.r,m_vColor.g,m_vColor.b,m_vColor.a,m_bFade?1:0);
		return m_sTemp;
	}
	//input the output data string,and analyze it
	bool	cPrtColorActBlending::SetDataByDataString(char*e_pString)
	{
		char* l_pString = strtok(e_pString,",");
		this->m_vColor.r = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_vColor.g = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_vColor.b = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_vColor.a = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_bFade = atoi(l_pString)?true:false;
		if( strtok(0,",") )
			return false;
		return true;
	}

	NamedTypedObject*  cPrtColorActBlendingBy2Color::Clone()
	{
		cParticleBase*l_pParticleBase = new cPrtColorActBlendingBy2Color();
		l_pParticleBase->SetDataByDataString(this->GetOutputDataString());
		return l_pParticleBase;
	}

	char*	cPrtColorActBlendingBy2Color::GetDataInfo()
	{
		sprintf(m_sTemp,"Color1:%.5f,%.5f,%.5f,%.5f\nColor2:%.5f,%.5f,%.5f,%.5f\0",m_vColor1.r,m_vColor1.g,m_vColor1.b,m_vColor1.a,
			m_vColor2.r,m_vColor2.g,m_vColor2.b,m_vColor2.a);
		return m_sTemp;
	}

	//this one for output data value by "," to separate value
	char*	cPrtColorActBlendingBy2Color::GetOutputDataString()
	{
		sprintf(m_sTemp,"%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f\0",m_vColor1.r,m_vColor1.g,m_vColor1.b,m_vColor1.a,
			m_vColor2.r,m_vColor2.g,m_vColor2.b,m_vColor2.a);
		return m_sTemp;
	}

	//input the output data string,and analyze it
	bool	cPrtColorActBlendingBy2Color::SetDataByDataString(char*e_pString)
	{
		char* l_pString = strtok(e_pString,",");
		this->m_vColor1.r = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_vColor1.g = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_vColor1.b = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_vColor1.a = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_vColor2.r = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_vColor2.g = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_vColor2.b = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_vColor2.a = (float)atof(l_pString);
		if( strtok(0,",") )
			return false;
		return true;
	}
//end namespace FATMING_CORE
}