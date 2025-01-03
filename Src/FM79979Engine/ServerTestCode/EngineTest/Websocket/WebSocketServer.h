/** --------------------------------------------------------------------------
 *  WebSocketServer.h
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

#ifndef _WEBSOCKETSERVER_H
#define _WEBSOCKETSERVER_H
#include <stdint.h>
#include <map>
#include <string>
#include <list>
#include <stdio.h>
#include <ctime>
//#include <sys/time.h>
#include <iostream>
#include <sstream>
#include "libwebsockets.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/// WebSocketServer
/// ---------
class WebSocketServer:public cCPP11Thread
{
    std::mutex m_ConnectionapMutex;
    void        ServiceUpdate(float e_fElpaseTime);
    uint64_t    m_ui64Timeout;
public:
    // Represents a client connection
    struct Connection
    {
        list<std::vector<char>>       buffer;     // Ordered list of pending messages to flush out when socket is writable
        //map<string,string> keyValueMap;
        time_t             createTime;
    };

    // Manages connections. Unfortunately this is public because static callback for
    // libwebsockets is defined outside the instance and needs access to it.
    map<int,Connection*> connections;

    // Constructor / Destructor
    WebSocketServer( int port, const string certPath = "", const string& keyPath = "" );
    ~WebSocketServer( );

    void run(       uint64_t timeout = 50     );
    void send(      int socketID, char* e_pData, size_t e_uiDataLen);
    void broadcast(char* e_pData, size_t e_uiDataLen);

    // Key => value storage for each connection
    //string getValue( int socketID, const string& name );
    //void   setValue( int socketID, const string& name, const string& value );
    int    getNumberOfConnections( );

    // Overridden by children
    virtual void onConnect(    int socketID                                 ) = 0;
    virtual void onMessage(    int socketID, char*e_pData,size_t e_uiDataLen) = 0;
    virtual void onDisconnect( int socketID                                 ) = 0;
    virtual void   onError(    int socketID, const string& message          ) = 0;


    // Wrappers, so we can take care of some maintenance
    void onConnectWrapper(    int socketID );
    void onDisconnectWrapper( int socketID );
    void onErrorWrapper( int socketID, const string& message );

protected:
    // Nothing, yet.

private:
    int                  _port;
    string               _keyPath;
    string               _certPath;
    struct lws_context  *_context;

    void _removeConnection( int socketID );
};

// WebSocketServer.h
#endif
