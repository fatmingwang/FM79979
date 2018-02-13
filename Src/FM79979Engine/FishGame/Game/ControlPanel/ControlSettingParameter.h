#pragma once

#include "../WriteFile/WriteFile.h"
//the parameter here is write into file while data is changed
struct sControlSettingParameter :public cWriteBinaryFile
{
private:
	static	cWriteBinaryFile::sRegister_Header_Struct		m_sRegister_Header_Struct;
	virtual void	WriteFile()override;
	virtual bool	OpenFileGetData(int e_iDataSizeWithOutFileExtension, char*e_pData, sRegister_Header_Struct*e_pRegister_Header_Struct)override;
public:
	sControlSettingParameter();
	virtual ~sControlSettingParameter();
	static	bool	m_sbTimeUpAutoShotOneBullet;
	static	int		m_siLongPressedFireKeyShotBulletSpeed;
	static	int64	m_si64ScoreLimit;
	static	int64	m_si64ButtonAddScoreLimit;
	static	int		m_siIOMode;
	static	int		m_siScoreToMoneyIOMode;
	static  int     m_siCoinToScoreRate;
	//this one is fucking stupid,who ask to add this!?
	static  int		m_siAddScoreIOBehaviod;//1 for enable exchangein score button and toast coin,2 for disable  exchangein button,3for disable toast coin
};