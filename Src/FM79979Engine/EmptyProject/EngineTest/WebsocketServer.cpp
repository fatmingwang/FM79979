#include "stdafx.h"
#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_
#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <iostream>
#include "WebsocketServer.h"
typedef websocketpp::server<websocketpp::config::asio> server;


using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;


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
        unsigned int uLength = sInput.length() / sizeof(char);

        int l_iSize = *(int*)pDataArray;
        pDataArray += sizeof(int)*3;
        std::string l_strText = pDataArray;
        int a = 0;

        struct sBaseNetworkMessage
        {
            int		iSize1;
            int		iSize2;
            int		iMessage;
            char	strText[260];
            //sBaseNetworkMessage(int e_iID,int e_iSize) :iMessage(e_iID) { iSize= e_iSize;	}
            //-sizeof(int) because my lazy received data is doing this.
            sBaseNetworkMessage() { iSize1 = iSize2 = sizeof(sBaseNetworkMessage)-sizeof(int); iMessage = 2; }
        };
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

server echo_server;



bool    WebSocketInit()
{
    try 
    {
        // Set logging settings
        //echo_server.set_access_channels(websocketpp::log::alevel::all);
        echo_server.set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);
        echo_server.set_error_channels(websocketpp::log::elevel::all);
        // Register our handlers
        echo_server.set_open_handler([](websocketpp::connection_hdl hdl) 
        {
             printf("on open called.");
            //m_open = std::chrono::high_resolution_clock::now();
            //m_endpoint.send(hdl, request, websocketpp::frame::opcode::text);
        });
        echo_server.set_close_handler([](websocketpp::connection_hdl) 
        {
                printf("on_close called\n");
        });
        echo_server.set_fail_handler([](websocketpp::connection_hdl hdl) 
        {
            auto con = echo_server.get_con_from_hdl(hdl);

            std::cout << "Fail handler" << std::endl;
            std::cout << con->get_state() << std::endl;
            std::cout << con->get_local_close_code() << std::endl;
            std::cout << con->get_local_close_reason() << std::endl;
            std::cout << con->get_remote_close_code() << std::endl;
            std::cout << con->get_remote_close_reason() << std::endl;
            std::cout << con->get_ec() << " - " << con->get_ec().message() << std::endl;
        }
        );

        echo_server.set_http_handler([](websocketpp::connection_hdl hdl)
        {
                printf("http_handler");
        });

        //echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        //echo_server.set_access_channels(websocketpp::log::alevel::frame_header | websocketpp::log::alevel::frame_payload);
        //echo_server.clear_access_channels(websocketpp::log::alevel::all);
        // Initialize Asio
        echo_server.init_asio();

        // Register our message handler
        echo_server.set_message_handler(bind(&on_message, &echo_server, ::_1, ::_2));

        //echo_server.get_connection()->add_subprotocol("Sec-Websocket-Protocol");

        // Listen on port 9002
        echo_server.listen(9991);

        // Start the server accept loop
        echo_server.start_accept();


    }
    catch (websocketpp::exception const& e) 
    {
        printf(e.what());
    }
    catch (...) 
    {
        std::cout << "other exception" << std::endl;
    }

    std::thread l_Thread = std::thread
    (
        [=]
        {
        for (;;) 
        {
            try 
            {
                echo_server.run();
                break; // run() exited normally
            }
            catch (websocketpp::exception const& e) {
                //m_logger->error(e.what());
                //m_logger->error("Exception thrown from m_io_service->run(). Restarting m_io_service->run()");
                std::cout << e.what() << std::endl;
            }
            catch (...) 
            {
                //m_logger->critical("Hit critial error. Restarting m_io_service->run()");
                std::cout << "other exception" << std::endl;
            }
        }
            try
            {
                echo_server.run();
            }
            catch (websocketpp::exception const& e)
            {
                std::cout << e.what() << std::endl;
            }
            catch (...)
            {
                std::cout << "other exception" << std::endl;
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