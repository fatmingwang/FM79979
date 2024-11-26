#pragma once
#include "json.hpp"
using json = nlohmann::json;

//a Deploy.json for file content
//a combobox for src env
//a listbox for target env


//a button dor fetch env Rule.json file and all version
// 
//a new panel show default using version and a list box for using special rule game list(game ID and shared code version)
//
//a listbox show all verion folder
// 
//a listbox to select game
//a drop box for set target game for use specific version
//a button to combine 

class cGUIForFileTransfer:NamedTypedObject
{
	enum eEnv
	{
		eE_DEV = 0,
		eE_UAT,
		eE_SIT,
		eE_PLAY_FOR_FUN,
		eE_PROD,
		eE_MAX
	};
	std::string						GetEnvName(eEnv e_eEnv);
					
	struct sEnvData
	{
		std::string					m_steVersionFileName;
		std::vector<std::string>	m_VersionVector;
		//
		std::string					m_strBackupDirectory;
		//
		std::string					m_strTargetIP;
		std::string					m_strRemoteUserName;
		std::string					m_strRemotePassword;
		int							m_iPort;
		sEnvData(json e_json);
		sEnvData(){}
	};
	std::map<eEnv, sEnvData>		m_EnvDataMap;
	std::vector<std::string>		m_VersionVector;
	bool							m_bDoingUpdate;
	//
	class cMyGuiRootNode*	m_pRoot;
	class cMyGuiListBox*	m_pMyGuiListBox;
	class cMyGuiComboBox*	m_pMyGuiComboBox;
	class cMyGuiButton*		m_pMyGuiButton;
	//
	void							FetchVersionFileList();
	void							ParseEnvData(const char*e_strFileName);
	void							RenderMainUI();
	void							RenderMenu(class cImGuiNode*e_pNode);
	//
	std::vector<std::string>		m_strEvnNameVector;
public:
	cGUIForFileTransfer();
	virtual ~cGUIForFileTransfer();
	void	Init();
	void	Update(float e_fElpaseTime);
	void	Render();
	void	Destory();
};