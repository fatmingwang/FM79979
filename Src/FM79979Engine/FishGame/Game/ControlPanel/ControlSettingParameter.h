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
};