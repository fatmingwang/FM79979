#pragma once
#include "../WriteFile/WriteFile.h"


class cFishManager;
class cFishBase;
//https://stackoverflow.com/questions/19842035/stdmap-how-to-sort-by-value-then-by-key
//std::vector<std::pair<V,K>> items;
//...
//std::sort(items.begin(), items.end());
class cSceneChangeFishGroup:public NamedTypedObject
{
	std::vector<std::pair<float,int> >	m_OriginalTimeAndFishIDPairVector;//init assign to current
	std::vector<std::pair<float, int> >	m_CurrentTimeAndFishIDPairVector;//for update
	float								m_fCurrentTime;
	std::vector<cFishBase*>				m_FishBaseVector;//the fishes required,while all fish is not at scene change scene
public:
	DEFINE_TYPE_INFO();
	cSceneChangeFishGroup(cMPDI*e_pMPDI,cFishManager*e_pFishManager);
	virtual ~cSceneChangeFishGroup();
	void	Init();
	//return true no more fish need out
	bool	Update(float e_fElpaseTime,cFishManager*e_pFishManager);
};
//
class cSceneChangeFishGroupManager :public cNamedTypedObjectVector<cSceneChangeFishGroup>
{
	cFishManager*								m_pFishManager;
	cSceneChangeFishGroup*						m_pCurrentFishGroup;
	//
	void										AddFishGroupByMPDIFile(const wchar_t* e_strFileName,cMPDIList*e_pMPDIList);
	void										PushSceneAndFishGroupData(std::wstring e_strName,std::vector<int> e_Data);
	UT::sTimeCounter							m_AllFinishNotInSceneChangeToSceneBTC;//5 second?
																	  //std::wstring is scene name,int is cSceneChangeFishGroup index of cNamedTypedObjectVector
	std::map<std::wstring, std::vector<int>>	m_FishGroupNameAndIndexOfNamedTypedObjectVectorVector;
public:
	DEFINE_TYPE_INFO();
	cSceneChangeFishGroupManager(cFishManager*e_pFishManager);
	~cSceneChangeFishGroupManager();
	bool	ProcessSceneChangeFishGroup(TiXmlElement*e_pRoot);
	void	Update(float e_fElpaseTime);
	//bool	IsAllowToChangeSceneWhenFishGroupFinish();//while m_pCurrentFishGroup is not empty and the fishes still working
	void	SetCurrentFishGroup(std::wstring e_strFishGroupName, int e_FishGroupIndexOfScene);
};