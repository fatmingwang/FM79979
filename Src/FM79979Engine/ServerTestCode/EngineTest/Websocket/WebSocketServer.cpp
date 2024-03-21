/** --------------------------------------------------------------------------
 *  WebSocketServer.cpp
 *
 *  Base class that WebSocket implementations must inherit from.  Handles the
 *  client connections and calls the child class callbacks for connection
 *  events like onConnect, onMessage, and onDisconnect.
 *
 *  Author    : Jason Kruse <jason@jasonkruse.com> or @mnisjk
 *  Copyright : 2014
 *  License   : BSD (see LICENSE)
 *  --------------------------------------------------------------------------
 **/
#include "../stdafx.h"
#include <stdlib.h>
#include <string>
#include <cstring>
//#include <sys/time.h>
#include <fcntl.h>
#include "libwebsockets.h"
//#include "Util.h"
#include "WebSocketServer.h"

using namespace std;

// 0 for unlimited
#define MAX_BUFFER_SIZE 0
#define LWS_NO_EXTENSIONS
// Nasty hack because certain callbacks are statically defined
WebSocketServer* g_pWebSocketServer = nullptr;
static int callback_main(   struct lws *wsi,
                            enum lws_callback_reasons reason,
                            void *user,
                            void *in,
                            size_t len )
{
    int fd;
    unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + 512 + LWS_SEND_BUFFER_POST_PADDING];
    unsigned char *p = &buf[LWS_SEND_BUFFER_PRE_PADDING];

    switch( reason ) 
    {
        case LWS_CALLBACK_ESTABLISHED:
            g_pWebSocketServer->onConnectWrapper( lws_get_socket_fd( wsi ) );
            lws_callback_on_writable( wsi );
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE:
        {
            fd = lws_get_socket_fd(wsi);
            auto l_IT = g_pWebSocketServer->connections.find(fd);
            if (l_IT != g_pWebSocketServer->connections.end())
            {
                while (!g_pWebSocketServer->connections[fd]->buffer.empty())
                {
                    auto l_Data = g_pWebSocketServer->connections[fd]->buffer.front();
                    int msgLen = l_Data.size();
                    int charsSent = lws_write(wsi, (unsigned char*)&l_Data[0], msgLen, LWS_WRITE_TEXT);
                    if (charsSent < msgLen)
                    {
                        g_pWebSocketServer->onErrorWrapper(fd, string("Error writing to socket"));
                    }
                    else
                    {
                        // Only pop the message if it was sent successfully.
                        g_pWebSocketServer->connections[fd]->buffer.pop_front();
                    }
                }
                lws_callback_on_writable(wsi);
            }
        }
            break;

        case LWS_CALLBACK_RECEIVE:
            g_pWebSocketServer->onMessage( lws_get_socket_fd( wsi ), (char*)in, len );
            break;

        case LWS_CALLBACK_CLOSED:
            g_pWebSocketServer->onDisconnectWrapper( lws_get_socket_fd( wsi ) );
            break;

        default:
            break;
    }
    return 0;
}
struct lws_protocols protocols[] = {
{
    "/",
    callback_main,
    0, // user data struct not used
    MAX_BUFFER_SIZE,
},
{
    nullptr, nullptr, 0, 0 }
};
WebSocketServer::WebSocketServer( int port, const string certPath, const string& keyPath )
{
    g_pWebSocketServer = this;
    this->_port     = port;
    this->_certPath = certPath;
    this->_keyPath  = keyPath;

    lws_set_log_level( 0, lwsl_emit_syslog ); // We'll do our own logging, thank you.
    struct lws_context_creation_info info;
    memset( &info, 0, sizeof info );
    info.port = this->_port;
    info.iface = NULL;
    info.protocols = protocols;
#ifndef LWS_NO_EXTENSIONS
    info.extensions = lws_get_internal_extensions( );
#endif

    if( !this->_certPath.empty( ) && !this->_keyPath.empty( ) )
    {
        FMLOG("Using SSL certPath=%s . keyPath=%s",this->_certPath.c_str(), this->_keyPath.c_str());
        //Util::log( "Using SSL certPath=" + this->_certPath + ". keyPath=" + this->_keyPath + "." );
        info.ssl_cert_filepath        = this->_certPath.c_str( );
        info.ssl_private_key_filepath = this->_keyPath.c_str( );
    }
    else
    {
        FMLOG( "Not using SSL" );
        info.ssl_cert_filepath        = NULL;
        info.ssl_private_key_filepath = NULL;
    }
    info.gid = -1;
    info.uid = -1;
    info.options = 0;

    // keep alive
    info.ka_time = 60; // 60 seconds until connection is suspicious
    info.ka_probes = 10; // 10 probes after ^ time
    info.ka_interval = 10; // 10s interval for sending probes
    this->_context = lws_create_context( &info );
    if( !this->_context )
        throw "libwebsocket init failed";
    FMLOG( "Server started on port %d ",this->_port);

    // Some of the libwebsocket stuff is define statically outside the class. This
    // allows us to call instance variables from the outside.  Unfortunately this
    // means some attributes must be public that otherwise would be private.
}

WebSocketServer::~WebSocketServer( )
{
    // Free up some memory
    for( map<int,Connection*>::const_iterator it = this->connections.begin( ); it != this->connections.end( ); ++it )
    {
        Connection* c = it->second;
        this->connections.erase( it->first );
        delete c;
    }
}

void WebSocketServer::onConnectWrapper( int socketID )
{
    Connection* c = new Connection;
    c->createTime = time( 0 );
    {
        MUTEX_PLACE_HOLDER(m_ConnectionapMutex,"WebSocketServer::onConnectWrapper");
        this->connections[socketID] = c;
    }
    this->onConnect( socketID );
}

void WebSocketServer::onDisconnectWrapper( int socketID )
{
    this->onDisconnect( socketID );
    this->_removeConnection( socketID );
}

void WebSocketServer::onErrorWrapper( int socketID, const string& message )
{
    FMLOG( "Error:%s  on socketID:%d", message , socketID);
    this->onError( socketID, message );
    this->_removeConnection( socketID );
}

void WebSocketServer::send( int socketID, char* e_pData, size_t e_uiDataLen)
{
    // Push this onto the buffer. It will be written out when the socket is writable.
    auto l_IT = connections.find(socketID);
    if (l_IT != connections.end())
    {
        std::vector<char>l_DataVector;
        l_DataVector.resize(e_uiDataLen);
        memcpy(&l_DataVector[0], e_pData, e_uiDataLen);
        this->connections[socketID]->buffer.push_back(l_DataVector);
    }
}

void WebSocketServer::broadcast(char* e_pData, size_t e_uiDataLen)
{
    for (map<int, Connection*>::const_iterator it = this->connections.begin(); it != this->connections.end(); ++it)
    {
        this->send(it->first, e_pData, e_uiDataLen);
    }
}

//void WebSocketServer::setValue( int socketID, const string& name, const string& value )
//{
//    //this->connections[socketID]->keyValueMap[name] = value;
//}
//
//string WebSocketServer::getValue( int socketID, const string& name )
//{
//    //return this->connections[socketID]->keyValueMap[name];
//    return std::string();
//}

int WebSocketServer::getNumberOfConnections( )
{
    return this->connections.size( );
}

void  WebSocketServer::ServiceUpdate(float e_fElpaseTime)
{
    if (lws_service(this->_context, m_ui64Timeout) < 0)
    {
        //throw "Error polling for socket activity.";
        FMLOG("timeout WebSocketServer::wait polling for socket activity.");
    }
}

void WebSocketServer::run( uint64_t timeout )
{
    m_ui64Timeout = timeout;
    f_ThreadWorkingFunction l_f_ThreadWorkingFunction = std::bind(&WebSocketServer::ServiceUpdate, this, std::placeholders::_1);
    this->ThreadDetach(l_f_ThreadWorkingFunction, "WebSocketServer::run");
}

void WebSocketServer::_removeConnection( int socketID )
{
    Connection* c = this->connections[ socketID ];
    this->connections.erase( socketID );
    delete c;
}
