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
	void	Render(float*e_pfMatrix = nullptr);
	void	Destory();
};