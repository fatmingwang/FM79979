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
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
    std::cout << "on_message called with hdl: " << hdl.lock().get()
        << " and message: " << msg->get_payload()
        << std::endl;

    // check for a special command to instruct the server to stop listening so
    // it can be cleanly exited.
    if (msg->get_payload() == "stop-listening") {
        s->stop_listening();
        return;
    }

    try {
        s->send(hdl, msg->get_payload(), msg->get_opcode());
    }
    catch (websocketpp::exception const& e) {
        std::cout << "Echo failed because: "
            << "(" << e.what() << ")" << std::endl;
    }
}

server echo_server;

bool    WebSocketInit()
{
    try 
    {
        // Set logging settings
        echo_server.set_access_channels(websocketpp::log::alevel::all);
        //echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        //echo_server.set_access_channels(websocketpp::log::alevel::frame_header | websocketpp::log::alevel::frame_payload);
        //echo_server.clear_access_channels(websocketpp::log::alevel::all);
        // Initialize Asio
        echo_server.init_asio();

        // Register our message handler
        echo_server.set_message_handler(bind(&on_message, &echo_server, ::_1, ::_2));

        // Listen on port 9002
        echo_server.listen(1000);

        // Start the server accept loop
        echo_server.start_accept();


    }
    catch (websocketpp::exception const& e) 
    {
        std::cout << e.what() << std::endl;
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