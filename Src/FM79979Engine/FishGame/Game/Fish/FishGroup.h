#pragma once
#include "../WriteFile/WriteFile.h"


class cFishManager;
class cFishBase;
//https://stackoverflow.com/questions/19842035/stdmap-how-to-sort-by-value-then-by-key
//std::vector<std::pair<V,K>> items;
//...
//std::sort(items.begin(), items.end());
class cFishGroup:public NamedTypedObject
{
	std::vector<std::pair<float,int> >	m_OriginalTimeAndFishIDPairVector;//init assign to current
	std::vector<std::pair<float, int> >	m_CurrentTimeAndFishIDPairVector;//for update
	float								m_fCurrentTime;
	std::vector<cFishBase*>				m_FishBaseVector;//the fishes required,while all fish is not at scene change scene
public:
	cFishGroup(cMPDI*e_pMPDI,cFishManager*e_pFishManager);
	virtual ~cFishGroup();
	void	Init();
	//return true no more fish need out
	bool	Update(float e_fElpaseTime,cFishManager*e_pFishManager);
};
//
class cFishGroupManager :public cNamedTypedObjectVector<cFishGroup>,public cNodeISAX,public cWriteBinaryFile
{
	struct sSceneData
	{
		int		iSceneIndex;
		int		iFishGroupIndex;
		bool	bSceneAOrB;//if true a or b,a fish group is play
		sSceneData() {	iSceneIndex = 0; iFishGroupIndex = 0; bSceneAOrB= true;	}
		~sSceneData(){}
	};
	sSceneData								m_CurrentSceneData;
	UT::sTimeCounter						m_AllFinishNotInSceneChangeToSceneBTC;//5 second?
	//std::wstring is scene name,int is cFishGroup index of cNamedTypedObjectVector
	std::map<std::wstring,std::vector<int>>	m_FishGroupVectorWithSceneName;
	//a quick find map for scene change,first in is how many scene,second is the fish gourp count
	std::map<size_t, size_t>						m_SceneIndexAndFishGroupCountMap;
	//
	//
	virtual	bool	MyParse(TiXmlElement*e_pRoot)override;
	//
	void			AddFishGroupByMPDIFile(const wchar_t* e_strFileName,cMPDIList*e_pMPDIList);
	void			PushSceneAndFishGroupData(std::wstring e_strName,std::vector<int> e_Data);
	//
	cFishManager*	m_pFishManager;
	cFishGroup*		m_pCurrentFishGroup;
	void			SetCurrentSceneName(const wchar_t*e_strSceneName);
public:
	cFishGroupManager(cFishManager*e_pFishManager);
	~cFishGroupManager();
	void	ToNextScene();
	void	Update(float e_fElpaseTime);
};