#include "stdafx.h"
#include "Proto/addressbook.pb.h"
#ifdef WIN32

#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_
//#define USE_SSL
#include <set>
#ifdef USE_SSL
#include <websocketpp/config/asio_client.hpp>
#else
#include <websocketpp/config/asio_no_tls.hpp>
#endif
#include <websocketpp/server.hpp>
////https://blog.csdn.net/byxdaz/article/details/84645586
//// for ws
//#include <websocketpp/config/asio_no_tls_client.hpp>
//#include <websocketpp/client.hpp>
//// 
////for wws
//#include <websocketpp/config/asio_client.hpp>
//#include <websocketpp/client.hpp>
#include <iostream>


//
typedef websocketpp::server<websocketpp::config::asio> server;
typedef websocketpp::config::asio::message_type::ptr message_ptr;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;



using websocketpp::connection_hdl;
// See https://wiki.mozilla.org/Security/Server_Side_TLS for more details about
// the TLS modes. The code below demonstrates how to implement both the modern
enum tls_mode
{
    MOZILLA_INTERMEDIATE = 1,
    MOZILLA_MODERN = 2
};
std::string get_password()
{
    return "test";
}

//typedef websocketpp::client<websocketpp::config::asio_client> client;
////https://github.com/zaphoyd/websocketpp/blob/master/tutorials/utility_client/utility_client.md
////
//class connection_metadata 
//{
//public:
//    typedef websocketpp::lib::shared_ptr<connection_metadata> ptr;
//    connection_metadata(int id, websocketpp::connection_hdl hdl, std::string uri)
//        : m_id(id)
//        , m_hdl(hdl)
//        , m_status("Connecting")
//        , m_uri(uri)
//        , m_server("N/A")
//    {}
//
//    void on_open(client* c, websocketpp::connection_hdl hdl)
//    {
//        m_status = "Open";
//
//        client::connection_ptr con = c->get_con_from_hdl(hdl);
//        m_server = con->get_response_header("Server");
//    }
//
//    void on_fail(client* c, websocketpp::connection_hdl hdl)
//    {
//        m_status = "Failed";
//
//        client::connection_ptr con = c->get_con_from_hdl(hdl);
//        m_server = con->get_response_header("Server");
//        m_error_reason = con->get_ec().message();
//    }
//
//    friend std::ostream& operator<< (std::ostream& out, connection_metadata const& data);
//private:
//    int m_id;
//    websocketpp::connection_hdl m_hdl;
//    std::string m_status;
//    std::string m_uri;
//    std::string m_server;
//    std::string m_error_reason;
//};


class cSimpleWebSocketServer
{

    //context_ptr on_tls_init(tls_mode mode, websocketpp::connection_hdl hdl) 
    //{
    //    namespace asio = websocketpp::lib::asio;
    //    std::cout << "on_tls_init called with hdl: " << hdl.lock().get() << std::endl;
    //    std::cout << "using TLS mode: " << (mode == MOZILLA_MODERN ? "Mozilla Modern" : "Mozilla Intermediate") << std::endl;
    //    context_ptr ctx = websocketpp::lib::make_shared<asio::ssl::context>(asio::ssl::context::sslv23);

    //    try 
    //    {
    //        if (mode == MOZILLA_MODERN) 
    //        {
    //            // Modern disables TLSv1
    //            ctx->set_options(asio::ssl::context::default_workarounds |
    //                asio::ssl::context::no_sslv2 |
    //                asio::ssl::context::no_sslv3 |
    //                asio::ssl::context::no_tlsv1 |
    //                asio::ssl::context::single_dh_use);
    //        }
    //        else 
    //        {
    //            ctx->set_options(asio::ssl::context::default_workarounds |
    //                asio::ssl::context::no_sslv2 |
    //                asio::ssl::context::no_sslv3 |
    //                asio::ssl::context::single_dh_use);
    //        }
    //        ctx->set_password_callback(bind(&get_password));
    //        ctx->use_certificate_chain_file("server.pem");
    //        ctx->use_private_key_file("server.pem", asio::ssl::context::pem);

    //        // Example method of generating this file:
    //        // `openssl dhparam -out dh.pem 2048`
    //        // Mozilla Intermediate suggests 1024 as the minimum size to use
    //        // Mozilla Modern suggests 2048 as the minimum size to use.
    //        ctx->use_tmp_dh_file("dh.pem");

    //        std::string ciphers;

    //        if (mode == MOZILLA_MODERN) 
    //        {
    //            ciphers = "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256:DHE-DSS-AES128-GCM-SHA256:kEDH+AESGCM:ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA256:DHE-RSA-AES256-SHA256:DHE-DSS-AES256-SHA:DHE-RSA-AES256-SHA:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!3DES:!MD5:!PSK";
    //        }
    //        else 
    //        {
    //            ciphers = "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256:DHE-DSS-AES128-GCM-SHA256:kEDH+AESGCM:ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA256:DHE-RSA-AES256-SHA256:DHE-DSS-AES256-SHA:DHE-RSA-AES256-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:AES128-SHA256:AES256-SHA256:AES128-SHA:AES256-SHA:AES:CAMELLIA:DES-CBC3-SHA:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!MD5:!PSK:!aECDH:!EDH-DSS-DES-CBC3-SHA:!EDH-RSA-DES-CBC3-SHA:!KRB5-DES-CBC3-SHA";
    //        }

    //        if (SSL_CTX_set_cipher_list(ctx->native_handle(), ciphers.c_str()) != 1) 
    //        {
    //            std::cout << "Error setting cipher list" << std::endl;
    //        }
    //    }
    //    catch (std::exception& e) 
    //    {
    //        std::cout << "Exception: " << e.what() << std::endl;
    //    }
    //    return ctx;
    //}
public:
    cSimpleWebSocketServer()
    {
        m_uiNumUser = 0;
        //m_server.init_asio();
        m_server.set_open_handler(bind(&cSimpleWebSocketServer::on_open, this, ::_1));
        m_server.set_close_handler(bind(&cSimpleWebSocketServer::on_close, this, ::_1));
        m_server.set_message_handler(bind(&cSimpleWebSocketServer::on_message, this, ::_1, ::_2));
        //m_server.set_tls_init_handler(bind(&cSimpleWebSocketServer::on_tls_init, MOZILLA_INTERMEDIATE, ::_1));

        try
        {
            // Set logging settings
            //m_server.set_access_channels(websocketpp::log::alevel::all);
            m_server.set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);
            m_server.set_error_channels(websocketpp::log::elevel::all);
            // Register our handlers
            m_server.set_fail_handler([this](websocketpp::connection_hdl hdl)
                {
                    auto con = m_server.get_con_from_hdl(hdl);

                    std::cout << "Fail handler" << std::endl;
                    std::cout << con->get_state() << std::endl;
                    std::cout << con->get_local_close_code() << std::endl;
                    std::cout << con->get_local_close_reason() << std::endl;
                    std::cout << con->get_remote_close_code() << std::endl;
                    std::cout << con->get_remote_close_reason() << std::endl;
                    std::cout << con->get_ec() << " - " << con->get_ec().message() << std::endl;
                }
            );

            m_server.set_http_handler([](websocketpp::connection_hdl hdl)
                {
                    printf("http_handler");
                });

            //m_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

            //m_server.set_access_channels(websocketpp::log::alevel::frame_header | websocketpp::log::alevel::frame_payload);
            //m_server.clear_access_channels(websocketpp::log::alevel::all);
            // Initialize Asio
            m_server.init_asio();

            // Register our message handler
            m_server.set_message_handler(bind(&cSimpleWebSocketServer::on_message, this, ::_1, ::_2));

            //m_server.get_connection()->add_subprotocol("Sec-Websocket-Protocol");

            //// Listen on port 9002
            //m_server.listen(9991);

            //// Start the server accept loop
            //m_server.start_accept();


        }
        catch (websocketpp::exception const& e)
        {
            FMLog::Log(e.what(), false);
        }
        catch (std::exception e)
        {
            FMLog::Log(e.what(), false);
        }

    }

    void on_open(connection_hdl hdl)
    {
        FMLog::Log("on_open called", false);
        m_connections.insert(hdl);
        SDLNetSocket l_SDLNetSocket = GetSDLNetSocket(m_uiNumUser);
        m_ConnectionAndUserIDMap.insert(std::make_pair(hdl, l_SDLNetSocket));
        ++m_uiNumUser;
    }

    void on_close(connection_hdl hdl)
    {
        FMLog::Log("on_close called", false);
        auto l_IT = m_ConnectionAndUserIDMap.find(hdl);
        if (l_IT != m_ConnectionAndUserIDMap.end())
        {
            m_ConnectionAndUserIDMap.erase(l_IT);
        }
        m_connections.erase(hdl);
    }

    void on_message(connection_hdl hdl, server::message_ptr msg)
    {
        FMLog::Log("on_message called", true);
        if (msg->get_opcode() == websocketpp::frame::opcode::BINARY)
        {
            const std::string& sInput = msg->get_payload();
            const char* pDataArray = reinterpret_cast<const char*>(sInput.data());
            auto uLength = sInput.length() / sizeof(char);
            bool l_bDoEchoTest = true;
            if (l_bDoEchoTest)
            {
                this->Boardcast(uLength, pDataArray);
            }
            else
            {
                int l_iSize = *(int*)pDataArray;
                if (l_iSize != uLength - 4)
                {
                    FMLog::Log("websocket on_Message size not match", true);
                }
                else
                {
                    pDataArray += sizeof(int);

                    websocketpp::connection_hdl l_connection_hdl;
                    auto l_IT = m_ConnectionAndUserIDMap.find(hdl);
                    if (l_IT != m_ConnectionAndUserIDMap.end())
                    {
                        std::shared_ptr<sNetworkReceivedPacket>l_pNetworkSendPacket = std::make_shared<sNetworkReceivedPacket>();
                        l_pNetworkSendPacket->WebSocketReceiveDataWithoutHeaderSize(l_IT->second, l_iSize, pDataArray);
                        m_WaitProcessDataVector.push_back(l_pNetworkSendPacket);
                    }
                }
            }
        }
    }

    void run(uint16_t port)
    {
        m_server.listen(port);
        m_server.start_accept();
        m_server.run();
    }
    void    stop()
    {
        m_server.stop();
    }

    typedef std::set<connection_hdl, std::owner_less<connection_hdl>> ConnectSet;


    bool    SetDataTo(SDLNetSocket e_SDLNetSocket, int e_iDataLength, char* e_pData)
    {
        websocketpp::connection_hdl l_connection_hdl;
        if (GetConnection__hdl(e_SDLNetSocket->Socket.uiWebSocketHandlerAddress, l_connection_hdl))
        {
            auto l_pData = (char*)alloca(e_iDataLength + sizeof(int));
            memcpy(l_pData, &e_iDataLength, sizeof(int));
            memcpy(&l_pData[sizeof(int)], e_pData, e_iDataLength);
            m_server.send(l_connection_hdl, l_pData, e_iDataLength + sizeof(int), websocketpp::frame::opcode::binary);
            return true;
        }
        return false;
    }

    bool    Boardcast(int e_iDataLength, const char* e_pData)
    {
        for (auto l_IT : m_ConnectionAndUserIDMap)
        {
            m_server.send(l_IT.first, e_pData, e_iDataLength, websocketpp::frame::opcode::binary);
        }
        return true;
    }
    void    Update(float e_fElpaseTime)
    {
        std::list<std::shared_ptr<sNetworkReceivedPacket>>l_WaitProcessDataVector;
        if (m_WaitProcessDataVector.size())
        {
            MUTEX_PLACE_HOLDER(m_NetworkReceivedPacketMutex, "NetworkReceivedPacketMutex");
            l_WaitProcessDataVector = m_WaitProcessDataVector;
            m_WaitProcessDataVector.clear();
        }
        while (l_WaitProcessDataVector.size())
        {
            auto l_pData = l_WaitProcessDataVector.front();
            if (1)
            {
                //https://krpc.github.io/krpc/communication-protocols/tcpip.html
                tutorial::AddressBook l_AddressBook;// = (tutorial::AddressBook::ParseFrom(()*)l_pData->pData;
                if (l_AddressBook.ParseFromArray(l_pData->pData, l_pData->iSize))
                {
                    int l_iNumPeople = l_AddressBook.people_size();
                    auto l_PeopleSize = l_AddressBook.people_size();
                    for (int i = 0; i < l_PeopleSize; ++i)
                    {
                        auto l_People = l_AddressBook.people(i);
                        int ID = l_People.id();
                        auto l_strName = l_People.name();
                        auto l_stremail = l_People.email();
                        int a = 0;
                    }
                    int a = 0;
                }
                //l_AddressBook.ParseFromString(()
                //fstream input(l_pData->pData, ios::in | ios::binary);
                //if (!l_AddressBook.ParseFromIstream(&input))
                //{
                //}
            }

            //sBaseNetworkMessage* l_pBaseNetworkMessage = (sBaseNetworkMessage*)l_pData->pData;
            //cGameApp::m_spMessageSenderManager->NetworkMessageShot(l_pBaseNetworkMessage->iMessage, l_pData.get());
            l_WaitProcessDataVector.pop_front();
        }
    }
private:
    server m_server;
    //std::vector<connection_hdl>m_connections;
    ConnectSet m_connections;
    size_t      m_uiNumUser;
    std::map<websocketpp::connection_hdl, SDLNetSocket, std::owner_less<connection_hdl>> m_ConnectionAndUserIDMap;
    std::mutex m_NetworkReceivedPacketMutex;
    std::list<std::shared_ptr<sNetworkReceivedPacket>>m_WaitProcessDataVector;
    bool GetConnection__hdl(size_t e_UserID, websocketpp::connection_hdl& e_connection_hdl)
    {
        websocketpp::connection_hdl l_Target;
        for (auto l_IT : m_ConnectionAndUserIDMap)
        {
            if (l_IT.second->Socket.uiWebSocketHandlerAddress == e_UserID)
            {
                e_connection_hdl = l_IT.first;
                return true;
            }
        }
        return false;
    }
};

cSimpleWebSocketServer* g_pSimpleWebSocketServer = nullptr;
bool                   g_bWebSocketThreadRunning = false;


//https://github.com/zaphoyd/websocketpp/blob/master/examples/simple_broadcast_server/simple_broadcast_server.cpp
bool    SendMessageToSocket(std::shared_ptr<websocketpp::connection<websocketpp::config::asio>> e_connection_ptr)
{

    return true;
}


bool    WebSocketInit(int e_iPoit)
{
    if (g_pSimpleWebSocketServer)
    {
        return false;
    }
    g_pSimpleWebSocketServer = new cSimpleWebSocketServer();
    std::thread l_Thread = std::thread
    (
        [=]
        {
            for (;;)
            {
                try
                {
                    g_bWebSocketThreadRunning = true;
                    g_pSimpleWebSocketServer->run(e_iPoit);
                    break;
                }
                catch (websocketpp::exception const& e)
                {

                    FMLog::Log(e.what(), false);
                }
                catch (std::exception e)
                {
                    FMLog::Log(e.what(), false);
                }
            }
        }
        );
    l_Thread.detach();
    return true;
}

int WebSocketUpdate(float e_fElpaseTime)
{
    if (!g_pSimpleWebSocketServer || !g_bWebSocketThreadRunning)
    {
        return false;
    }
    g_pSimpleWebSocketServer->Update(e_fElpaseTime);
    //// Create a server endpoint
    //try {        

    //    // Start the ASIO io_service run loop
    //    echo_server.run();
    //}
    //catch (websocketpp::exception const& e) {
    //    std::cout << e.what() << std::endl;
    //}
    //catch (...) {
    //    std::cout << "other exception" << std::endl;
    //}
    return true;
}

bool	WebSocketDestroy()
{
    if (!g_pSimpleWebSocketServer)
    {
        return false;
    }
    g_pSimpleWebSocketServer->stop();
    Sleep(100);
    delete g_pSimpleWebSocketServer;
    g_pSimpleWebSocketServer = nullptr;
    return true;
}

bool	WebSocketSendData(SDLNetSocket e_WebSocket, int e_iDataLength, char* e_pData)
{
    if (!g_pSimpleWebSocketServer)
    {
        return false;
    }
    return g_pSimpleWebSocketServer->SetDataTo(e_WebSocket, e_iDataLength, e_pData);
}

bool	WebSocketSendDataToAll(int e_iDataLength, char* e_pData)
{
    if (!g_pSimpleWebSocketServer)
    {
        return false;
    }
    return g_pSimpleWebSocketServer->Boardcast(e_iDataLength, e_pData);
}
#else
bool	WebSocketInit() { return false; }
int		WebSocketUpdate() { return 0; }
bool	WebSocketDestroy() { return false; }

bool	WebSocketSendData(SDLNetSocket e_pWebSocket, int e_iDataLength, char* e_pData) { return false; }
bool	WebSocketSendDataToAll(int e_iDataLength, char* e_pData) { return false; }
#endif