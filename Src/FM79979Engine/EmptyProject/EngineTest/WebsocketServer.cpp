#include "stdafx.h"
#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_
#include <set>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include "WebsocketServer.h"

typedef websocketpp::server<websocketpp::config::asio> server;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using websocketpp::connection_hdl;

struct sBaseNetworkMessage
{
    int		iSize1;
    int		iSize2;
    int		iMessage;
    char	strText[260];
    //sBaseNetworkMessage(int e_iID,int e_iSize) :iMessage(e_iID) { iSize= e_iSize;	}
    //-sizeof(int) because my lazy received data is doing this.
    sBaseNetworkMessage() { iSize1 = iSize2 = sizeof(sBaseNetworkMessage) - sizeof(int); iMessage = 2; }
};

class cSimpleWebSocketServer
{
public:
    cSimpleWebSocketServer()
    {
        //m_server.init_asio();
        //m_server.set_open_handler(bind(&cSimpleWebSocketServer::on_open, this, ::_1));
        //m_server.set_close_handler(bind(&cSimpleWebSocketServer::on_close, this, ::_1));
        //m_server.set_message_handler(bind(&cSimpleWebSocketServer::on_message, this, ::_1, ::_2));

        try
        {
            // Set logging settings
            //m_server.set_access_channels(websocketpp::log::alevel::all);
            m_server.set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);
            m_server.set_error_channels(websocketpp::log::elevel::all);
            // Register our handlers
            m_server.set_open_handler([this](websocketpp::connection_hdl hdl)
            {
                    auto con = m_server.get_con_from_hdl(hdl);
                    auto l_EndPoint = con->get_socket().remote_endpoint();
                    std::string sClientIp = l_EndPoint.address().to_string();
                    unsigned short uiClientPort = l_EndPoint.port();
                    FMLog::Log("on open called.", false);
                    //m_open = std::chrono::high_resolution_clock::now();
                    //m_endpoint.send(hdl, request, websocketpp::frame::opcode::text);
            });
            m_server.set_close_handler([](websocketpp::connection_hdl hdl)
                {
                    FMLog::Log("on_close called\n", false);
                });
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
        //auto con = m_server.get_con_from_hdl(hdl);
        //auto l_pSocket = con->get_socket();
        m_connections.insert(hdl);
    }

    void on_close(connection_hdl hdl)
    {
        m_connections.erase(hdl);
    }

    void on_message(connection_hdl hdl, server::message_ptr msg)
    {
        if (msg->get_opcode() == websocketpp::frame::opcode::BINARY)
        {
            const std::string& sInput = msg->get_payload();
            const char* pDataArray = reinterpret_cast<const char*>(sInput.data());
            auto uLength = sInput.length() / sizeof(char);
            int l_iSize = *(int*)pDataArray;
            if (l_iSize != uLength - 4)
            {
                FMLog::Log("websocket on_Message size not match",true);
            }
            else
            {
                pDataArray += sizeof(int);
                struct seCDNM_C2S_CAR_STATUS
                {
                    int		iSize;
                    int		iMessage;
                    int     a;
                    char	strText[260];
                    int     b;
                };
                seCDNM_C2S_CAR_STATUS* l_pseCDNM_C2S_CAR_STATUS = (seCDNM_C2S_CAR_STATUS*)pDataArray;
                std::shared_ptr<sNetworkReceivedPacket>l_pNetworkSendPacket = std::make_shared<sNetworkReceivedPacket>();
                l_pNetworkSendPacket->WebSocketReceiveDataWithoutHeaderSize((void*)&hdl, l_iSize, pDataArray);
                m_WaitProcessDataVector.push_back(l_pNetworkSendPacket);
            }
        }
    }

    void run(uint16_t port)
    {
        m_server.listen(port);
        m_server.start_accept();
        m_server.run();
    }
    typedef std::set<connection_hdl, std::owner_less<connection_hdl>> ConnectSet;

    void    SetDataTo(connection_hdl e_connection_hdl)
    {
        char l_Data[6];
        m_server.send(e_connection_hdl, &l_Data[0],sizeof(l_Data), websocketpp::frame::opcode::binary);
    }
    void    Update(float e_fElpaseTime)
    {
        std::list<std::shared_ptr<sNetworkReceivedPacket>>l_WaitProcessDataVector;
        if(m_WaitProcessDataVector.size())
        {
            MUTEX_PLACE_HOLDER(m_NetworkReceivedPacketMutex,"NetworkReceivedPacketMutex");
            l_WaitProcessDataVector = m_WaitProcessDataVector;
            m_WaitProcessDataVector.clear();
        }
        while (l_WaitProcessDataVector.size())
        {
            auto l_pData = l_WaitProcessDataVector.front();
            sBaseNetworkMessage*l_pBaseNetworkMessage = (sBaseNetworkMessage*)l_pData->pData;
            cGameApp::m_spMessageSenderManager->NetworkMessageShot(l_pBaseNetworkMessage->iMessage, l_pData.get());
            l_WaitProcessDataVector.pop_front();
        }
    }
private:
    server m_server;
    ConnectSet m_connections;
    std::mutex m_NetworkReceivedPacketMutex;
    std::list<std::shared_ptr<sNetworkReceivedPacket>>m_WaitProcessDataVector;
};


//https://github.com/zaphoyd/websocketpp/blob/master/examples/simple_broadcast_server/simple_broadcast_server.cpp
bool    SendMessageToSocket(std::shared_ptr<websocketpp::connection<websocketpp::config::asio>> e_connection_ptr)
{

    return true;
}

//class echo_handler : public server::handler {
//    void on_message(connection_ptr con, std::string msg) {
//        con->write(msg);
//    }
//};

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

// Define a callback to handle incoming messages
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) 
{
    std::cout << "on_message called with hdl: " << hdl.lock().get()
        << " and message: " << msg->get_payload()
        << std::endl;

    //https://kheresy.wordpress.com/2013/04/11/transfer-binary-data-via-websocket-arraybuffer-p2/
    if (msg->get_opcode() == websocketpp::frame::opcode::BINARY)
    {
        const std::string& sInput = msg->get_payload();

        const char* pDataArray = reinterpret_cast<const char*>(sInput.data());
        auto uLength = sInput.length() / sizeof(char);

        int l_iSize = *(int*)pDataArray;
        pDataArray += sizeof(int)*3;
        std::string l_strText = pDataArray;
        int a = 0;
        sBaseNetworkMessage l_sBaseNetworkMessage;
        sprintf(l_sBaseNetworkMessage.strText,"9876543210");
        s->send(hdl,&l_sBaseNetworkMessage, sizeof(l_sBaseNetworkMessage), websocketpp::frame::opcode::binary);
    }
    else
    {
        // check for a special command to instruct the server to stop listening so
        // it can be cleanly exited.
        if (msg->get_payload() == "stop-listening") 
        {
            s->stop_listening();
            return;
        }

        try
        {
            //s->send(hdl, msg->get_payload(), msg->get_opcode());
            uint8_t buffer[4] = { 1, 2, 3, 4 };
            // This uses the send overload that takes a void * and length
            s->send(hdl, buffer, sizeof(buffer), websocketpp::frame::opcode::binary);
        }
        catch (websocketpp::exception const& e) {
            std::cout << "Echo failed because: "
                << "(" << e.what() << ")" << std::endl;
        }
    }
}





bool    WebSocketInit()
{
    std::thread l_Thread = std::thread
    (
        [=]
        {
            cSimpleWebSocketServer l_cSimpleWebSocketServer;
            for (;;) 
            {
                try 
                {
                    l_cSimpleWebSocketServer.run(9991);
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

int WebSocketUpdate() 
{
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
    return 1;
}