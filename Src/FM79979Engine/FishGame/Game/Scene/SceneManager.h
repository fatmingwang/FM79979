#pragma once

class cCameraShake;
class cSceneChangeFishGroupManager;
class cFishManager;

//it contain BaseScene(s) which it should has a Fishgroup and a common scene(time up to turn to next scene).
typedef cPhaseManager cPhaseManagerInPhaseManager;
//cPhaseManager contain a cPhaseManager and it own SceneBase
class cSceneManager : public cNodeISAX,public cPhaseManagerInPhaseManager, public cWriteFileWithThread,public cMessageSender
{
	//this is the data write into file and control scene change
	struct sSceneData
	{
		int		iSceneIndex;
		int		iSubSceneIndex;
		int		iFishGroupIndex;
		sSceneData() {	iSceneIndex = 0; iFishGroupIndex = 0; iSubSceneIndex= 0;}
		~sSceneData() {}
	};
	sSceneData						m_CurrentSceneData;
	int								m_iFishGroupCount;
	virtual bool					OpenFileGetData(int e_iDataSizeWithOutFileExtension, char*e_pData, sRegister_Header_Struct*e_pRegister_Header_Struct)override;
	//
	bool							SceneChangeEvent(void*e_pFishGroupName);
	bool							FishGroupGoEvent(void*e_pFishGroupName);
	//
	cCameraShake*					m_pCameraShake;
	cSceneChangeFishGroupManager*	m_pSceneChangeFishGroupManager;
	cFishManager*					m_pFishManagerReference;
	virtual	bool					MyParse(TiXmlElement*e_pRoot)override;
	void							ProcessSceneContent(TiXmlElement*e_pTiXmlElement);
	void							ProcessFishGroup(TiXmlElement*e_pTiXmlElement);
	virtual void					WriteFileUpdate(float e_fElpaseTime)override;
public:	
	cSceneManager(cFishManager*e_pFishManager);
	~cSceneManager();
	void	LastRender();
	void	DebugRender();
	virtual void	Update(float e_fElpaseTime)override;
};