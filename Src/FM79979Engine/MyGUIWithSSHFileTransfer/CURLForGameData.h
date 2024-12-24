#pragma once

#include "curl/curl.h"
#include "../../include/json.hpp"
enum eDeployEvn
{
    eDE_DEV = 0,
    eDE_UAT_QA,
    eDE_SIT,
    eDE_FUN,
    eDE_PROD,
    eDE_MAX
};

void            CurlInit();
void            CurlExit();
bool            Post(eDeployEvn e_eDeployEvn,const char* e_strJSONString, nlohmann::json& e_OutResult);
std::string     GetDeployEvnToString(eDeployEvn e_eDeployEvn);