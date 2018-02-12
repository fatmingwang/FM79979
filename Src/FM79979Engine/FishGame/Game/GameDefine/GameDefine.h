#pragma once

#include "FishGameEventMessageID.h"
#include "GameParameterDefine.h"


enum eFishGameFileName
{
	eFGFN_MESSAGE_MPDILIST = 0,
	eFGFN_LANGUAGE,
	eFGFN_SCENE_MANAGER_FILE_NAME,
	eFGFN_MAX
};
std::string		GetFishGameFileName(eFishGameFileName e_eFishGameFileName);
std::wstring	GetFishGameFileNameW(eFishGameFileName e_eFishGameFileName);