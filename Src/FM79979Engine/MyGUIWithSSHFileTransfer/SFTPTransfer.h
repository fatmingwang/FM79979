#pragma once


enum eEnvType
{
	eET_DEV = 0,
	eET_UAT,
	eET_SIT,
	eET_FUN,
	eET_PROD,
	eET_MAX,
};


typedef std::function<void()> f_CompleteFunction;
void	AssignEnvData(const char* e_strUserName, const char* e_strPW, const char* e_strIP, const int e_iPort, const eEnvType e_eEnvType);
void	LibSSH2Init();
bool	UploadFileOrDirectory(const std::string& e_strLocalPath, const std::string& e_strRemotePath,std::vector<eEnvType>& e_eEnvTypeVector, f_CompleteFunction e_fCompleteFunction);
bool	DownloadFileOrDirectory(const std::string& e_strLocalPath, const std::string& e_strRemotePath, std::vector<eEnvType>& e_eEnvTypeVector, f_CompleteFunction e_fCompleteFunction);
void	LibSSH2ShutDown();