#pragma once


void	AssignEnvData(const char* e_strUserName, const char* e_strPW, const char* e_strIP, const int e_iPort);
void	LibSSH2Init();
bool	UploadFileOrDirectory(const char*e_strEnvName,const std::string& e_strLocalPath, const std::string& e_strRemotePath);
bool	DownloadFileOrDirectory(const char* e_strEnvName,const std::string& e_strLocalPath, const std::string& e_strRemotePath);
void	LibSSH2ShutDown();