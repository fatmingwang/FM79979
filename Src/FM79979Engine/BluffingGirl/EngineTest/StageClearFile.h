#ifndef _STAGE_CLEAR_FILE_H_
#define _STAGE_CLEAR_FILE_H_

//if game is clear save to a file then player could watch video


class	cStageClearFile
{
	class	cStageClearCheck:public NamedTypedObject
	{
	public:
		cStageClearCheck(){m_bStageCleard = false;}
		~cStageClearCheck(){}
		bool	m_bStageCleard;
	};
	static	cNamedTypedObjectVector<cStageClearCheck>*	m_pStageClearCheckVector;
public:
	cStageClearFile();
	~cStageClearFile();
	static	bool	IsStageClear(const WCHAR*e_strStageName);
	static	bool	StageClear(const WCHAR*e_strStageName);

	static	bool	IsGirlUnlock(const WCHAR*e_strStageName);
	static	bool	GirlUnlock(const WCHAR*e_strStageName);
};

#endif