#pragma once


void	AssignEnvData(const char* e_strUserName, const char* e_strPW, const char* e_strIP, const int e_iPort);
void	LibSSH2Init();
bool	UploadFile(const char*e_strEnvName,const std::string& local_path, const std::string& remote_path);
bool	DownloadFile(const char* e_strEnvName,const std::string& local_path, const std::string& remote_path);
void	LibSSH2ShutDown();