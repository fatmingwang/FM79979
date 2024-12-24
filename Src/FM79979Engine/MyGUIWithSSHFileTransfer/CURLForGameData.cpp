#include "stdafx.h"
#include "CURLForGameData.h"


const char* g_strURLArray[eDeployEvn::eDE_MAX] = 
{
    "https://dev.diresoft.net/api/game/update",
    "https://qa.diresoft.net/api/game/update",
    "https://sit.diresoft.net/api/game/update",
    "https://fun.diresoft.net/api/game/update",
    "https://prod.diresoft.net/api/game/update"
};



class cCURL
{
public:
    void* m_pCurl = nullptr;
    cCURL()
    {
        auto l_uiCode = curl_global_init(CURL_GLOBAL_ALL);
        m_pCurl = curl_easy_init();
    }
    ~cCURL()
    {
        curl_easy_cleanup((CURL*)m_pCurl);
        curl_global_cleanup();
    }
};

cCURL* g_pCURL = nullptr;
void    CurlInit()
{
    g_pCURL = new cCURL();
}

void    CurlExit()
{
    delete g_pCURL;
}



size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool Post(eDeployEvn e_eDeployEvn,const char* e_strJSONString, nlohmann::json& e_OutResult)
{
    CURL* curl = g_pCURL->m_pCurl;
    CURLcode res;
    auto body = e_strJSONString;
    auto l_uiCode = curl_easy_setopt(curl, CURLOPT_URL, g_strURLArray[e_eDeployEvn]);
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    l_uiCode = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    l_uiCode = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);

    std::string resultJsonStr;
    l_uiCode = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    l_uiCode = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resultJsonStr);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    //
    //curl_easy_cleanup(l_pCurl);
    /* Check for errors */
    e_OutResult = nlohmann::json::parse(resultJsonStr);
    if (res != CURLE_OK)
    {
        //FMLog::Log(UT::ComposeMsgByFormat("curl_easy_perform() failed: %s", curl_easy_strerror(res)).c_str(), true);
        return false;
    }
    else
    {
        //try 
        //{
        e_OutResult = nlohmann::json::parse(resultJsonStr);
        return true;
        //}
        //catch (nlohmann::detail::parse_error& error)
        //{
        //    //cout << "json parse error" << endl;
        //}
        //catch (nlohmann::detail::type_error& error)
        //{
        //    //cout << "json type error" << endl;
        //}
        return nullptr;
    }
    return nullptr;
}


//
//"{
//""Token"": ""hhuWRcPdsAvBdPPMtuONqnYrkrAPkVreWYyOSDQqDqzJon"",
//""GameInfo"" : {
//    ""GameCode"": ""10099"",
//        ""GameName"" : ""Squid Game : Mukunghwaasd"",
//        ""LineType"" : ""Line"",
//        ""Provider"" : ""AP"",
//        ""ReleaseDate"" : ""2020 - 01 - 01"",
//        ""IsEgret"" : false,
//        ""HasJackpot"" : false,
//        ""HasFreeGame"" : true,
//        ""HasFreeGameCustom"" : false,
//        ""HasCollection"" : false,
//        ""BF"" : [
//            ""50""
//        ] ,
//        ""DC"" : [
//            ""1.2""
//        ] ,
//        ""GameCategoryIds"" : [
//            ""1""
//        ] ,
//        ""LanguageInfo"" : [
//    {
//        ""Language"": ""cn"",
//            ""Name"" : ""Squid Game : Mukunghwa""
//    },
//            {
//                ""Language"": ""en"",
//                ""Name"" : ""Squid Game : Mukunghwa""
//            },
//            {
//                ""Language"": ""hi"",
//                ""Name"" : ""Squid Game : Mukunghwa""
//            },
//            {
//                ""Language"": ""id"",
//                ""Name"" : ""Squid Game : Mukunghwa""
//            },
//            {
//                ""Language"": ""ja"",
//                ""Name"" : ""Squid Game : Mukunghwa""
//            },
//            {
//                ""Language"": ""kh"",
//                ""Name"" : ""Squid Game : Mukunghwa""
//            },
//            {
//                ""Language"": ""kr"",
//                ""Name"" : ""Squid Game : Mukunghwa""
//            },
//            {
//                ""Language"": ""my"",
//                ""Name"" : ""Squid Game : Mukunghwa""
//            },
//            {
//                ""Language"": ""th"",
//                ""Name"" : ""Squid Game : Mukunghwa""
//            },
//            {
//                ""Language"": ""vi"",
//                ""Name"" : ""Squid Game : Mukunghwa""
//            }
//        ],
//        ""SpecialPlatformRtp"": ""97.01 % ""
//}
//}"