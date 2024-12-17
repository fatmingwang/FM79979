#pragma once
#include "json.hpp"
#include "openssl/include/libssh2.h"
#include "openssl/include/libssh2_sftp.h"
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

std::string GetEnvName(eEnv e_eEnv);

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


typedef std::function<void(std::string)>				f_CompleteFunction;
typedef std::function<void(std::vector<std::string>, std::vector<std::string>)>	f_GetDirectoryContentCompleteFunction;
void	AssignEnvData(const eEnv e_eEnvType, sEnvData e_EnvData);
void	LibSSH2Init();
bool	UploadFileOrDirectory(const std::string& e_strLocalPath, const std::string& e_strRemotePath,std::vector<eEnv>& e_eEnvTypeVector, f_CompleteFunction e_fCompleteFunction);
bool	DownloadFileOrDirectory(const std::string& e_strRemotePath, const std::string& e_strLocalFilePath, std::vector<eEnv>& e_eEnvTypeVector, f_CompleteFunction e_fCompleteFunction);
bool	ListRemoteDirectory(eEnv e_EnvType,const std::string& e_strRemotePath, f_GetDirectoryContentCompleteFunction e_fGetDirectoryContentCompleteFunction);
void	LibSSH2ShutDown();



struct sLIBSSH2SocketData
{
	int m_iSock;
	sockaddr_in			m_ServerAddr;
	LIBSSH2_SESSION*	m_pSession = nullptr;
	LIBSSH2_SFTP*		m_pSFTPSession = nullptr;
	sEnvData			m_EnvData;
	bool				m_bConnectedOk = false;
	f_CompleteFunction	m_fCompleteFunction;
	sLIBSSH2SocketData();
	~sLIBSSH2SocketData();
	void	Close();
	bool	DoUploadFile(const std::string& local_path, const std::string& remote_path);
	bool	DoUploadDirectory(const std::string& local_dir, const std::string& remote_dir);
	bool	DoDownloadFile(const std::string& remote_dir, const std::string& local_dir);
	bool	DoListRemoteDirectory(const std::string& remote_dir, std::vector<std::string>& e_FilesVector, std::vector<std::string>& e_DirectoriesVector);
	bool	DoDownloadDirectory(const std::string& remote_dir, const std::string& local_dir);
	bool	DoConnect();
	static std::map<eEnv, sLIBSSH2SocketData*>* g_pLIBSSH2SocketDataMap;
	static sLIBSSH2SocketData* GetDataByEnv(eEnv e_eEnv);
};