#pragma once
#include "./SFTPTransfer.h"

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
	std::map<eEnv, sEnvData>		m_EnvDataMap;
	std::vector<std::string>		m_VersionVector;
	bool							m_bDoingUpdate;
	//
	class cMyGuiRootNode*			m_pMainUIRoot;
	class cMyGuiListBox*			m_pVersionListBox;
	class cMyGuiListBox*			m_pTargetEnvListBox;
	class cMyGuiComboBox*			m_pSourceEnvComboBox;
	class cMyGuiButton*				m_pFetchRuleFileButton;
	class cMyGuiButton*				m_pUploadRuleFileButton;
	class cMyGuiForm*				m_pMyGuiForm;
	class cMyGuiEditBox*			m_pRuleJsonContentEditbox;
	//
	void							FetchVersionFileList();
	void							ParseEnvData(const char*e_strFileName);
	//
	std::vector<std::string>		m_strEvnNameVector;
public:
	cGUIForFileTransfer();
	virtual ~cGUIForFileTransfer();
	void	Init();
	void	Update(float e_fElpaseTime);
	void	Render(float*e_pfMatrix = nullptr,float*e_pfGameResolutoinSize = nullptr);
	cOrthogonalCamera* m_p2DCamera = nullptr;
};