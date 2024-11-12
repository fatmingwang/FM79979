#include "stdafx.h"
#include "GUIForFileTransfer.h"
#include "json.hpp"

const char* g_strEnvJSONKey_LocalSource = "LocalSource";
const char* g_strEnvJSONKey_BackupSource = "BackupSource";
const char* g_strEnvJSONKey_TargetSourceIP = "TargetIP";
const char* g_strEnvJSONKey_DeployEnvironmentData = "Deploy";
const char* g_strEnvJSONKey_SFTPDirectory = "SFTPDirectory";
const char* g_strEnvJSONKey_TargetIP = "TargetIP";
const char* g_strEnvJSONKey_UserName = "user";
const char* g_strEnvJSONKey_Password = "pw";
const char* g_strEnvJSONKey_Port = "port";
const char* g_strEnvJSONKey_TargetFolder = "TargetFolder";
const char* g_strEnvJSONKey_GoogleSheetName = "GoogleSheetName";
const char* g_strEnvJSONKey_VersionFileDirectory = "VersionFileDirectory";
const char* g_strEnvJSONKey_UploadGoogleSheetExeFileDirectory = "UploadGoogleSheetExeFileDirectory";

std::string	g_strLocalFolder;
std::string	g_strPullCommand;
std::string	g_strPushCommand;

const char* g_strThisPullFileName = "ThisIsPull.bat";
const char* g_strThisPushFileName = "ThisIsCommitAndPush.bat";

void cGUIForFileTransfer::FetchVersionFileList()
{
}

void cGUIForFileTransfer::ParseEnvData(const char* e_strFileName)
{
}

cGUIForFileTransfer::cGUIForFileTransfer()
{
}

cGUIForFileTransfer::~cGUIForFileTransfer()
{
}

void cGUIForFileTransfer::Init()
{
}

void cGUIForFileTransfer::Update(float e_fElpaseTime)
{
}

void cGUIForFileTransfer::Render()
{
}

void cGUIForFileTransfer::Destory()
{
}
