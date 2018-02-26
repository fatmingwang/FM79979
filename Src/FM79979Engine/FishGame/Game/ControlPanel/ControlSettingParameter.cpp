#include "stdafx.h"
#include "ControlSettingParameter.h"


sControlSettingParameter::sRegister_Header_Struct		m_sRegister_Header_Struct;// = { 'q','o','o','o',1.f };
bool	sControlSettingParameter::m_sbTimeUpAutoShotOneBullet = false;
int		sControlSettingParameter::m_siLongPressedFireKeyShotBulletSpeed = 1;
int64	sControlSettingParameter::m_si64ScoreLimit = 100000000;
int64	sControlSettingParameter::m_si64ButtonAddScoreLimit = 10000000;
int		sControlSettingParameter::m_siIOMode = 1;
int		sControlSettingParameter::m_siCoinToScoreRate = 1;
int		sControlSettingParameter::m_siScoreToMoneyIOMode = 0;

int		sControlSettingParameter::m_siAddScoreIOBehaviod = 1;


sControlSettingParameter::sControlSettingParameter()
{

}

sControlSettingParameter::~sControlSettingParameter()
{

}

void	sControlSettingParameter::WriteFile()
{

}
bool	sControlSettingParameter::OpenFileGetData(int e_iDataSizeWithOutFileExtension, char*e_pData, sRegister_Header_Struct*e_pRegister_Header_Struct)
{
	return false;
}