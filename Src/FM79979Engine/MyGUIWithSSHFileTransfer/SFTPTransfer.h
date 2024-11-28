#pragma once
#include "json.hpp"
using json = nlohmann::json;

enum eEnv
{
	eE_DEV = 0,
	eE_UAT,
	eE_SIT,
	eE_FUN,
	eE_PROD,
	eE_MAX,
};

struct sEnvData
{
	std::string					m_steVersionFileName;
	std::vector<std::string>	m_VersionVector;
	//
	std::string					m_strBackupDirectory;
	std::string					m_strRemoteDirectory;
	//
	std::string					m_strTargetIP;
	std::string					m_strRemoteUserName;
	std::string					m_strRemotePassword;
	int							m_iPort;
	sEnvData(json e_json);
	sEnvData() {}
};


typedef std::function<void(std::string)> f_CompleteFunction;
void	AssignEnvData(const eEnv e_eEnvType, sEnvData e_EnvData);
void	LibSSH2Init();
bool	UploadFileOrDirectory(const std::string& e_strLocalPath, const std::string& e_strRemotePath,std::vector<eEnv>& e_eEnvTypeVector, f_CompleteFunction e_fCompleteFunction);
bool	DownloadFileOrDirectory(const std::string& e_strRemotePath, const std::string& e_strLocalFilePath, std::vector<eEnv>& e_eEnvTypeVector, f_CompleteFunction e_fCompleteFunction);
void	LibSSH2ShutDown();