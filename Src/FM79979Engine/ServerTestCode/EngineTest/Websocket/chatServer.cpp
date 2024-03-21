/** --------------------------------------------------------------------------
 *  chatServer.cpp
 *
 *  A basic example of how to implement a WebSocketServer.  Creats a WS server
 *  bound to port 8080 for basic chatting.  It listens for connections and
 *  assigns them a random handle based on their socket FD.  Whenever a message
 *  is received on the server, it is sent out to all connected clients.
 *  received, it echos the same message back to the client.
 *
 *  Author    : Jason Kruse <jason@jasonkruse.com> or @mnisjk
 *  Copyright : 2014
 *  License   : BSD (see LICENSE)
 *  -------------------------------------------------------------------------- 
 **/

#include "stdafx.h"

// Log to stdout for easy debugging.
#define LOG_TO_STDOUT 1


#include "WebSocketServer.h"
#include "../Proto/addressbook.pb.h"
using namespace std;

// For any real project this should be defined separately in a header file
class cEasyWebSocketServer : public WebSocketServer
{
public: 
    cEasyWebSocketServer( int port, const string certPath, const string& keyPath);
    ~cEasyWebSocketServer( );
    virtual void onConnect(    int socketID                        );
    virtual void onMessage(int socketID, char* e_pData, size_t e_uiDataLen);
    virtual void onDisconnect( int socketID                        );
    virtual void   onError(    int socketID, const string& message );
};

cEasyWebSocketServer*g_pEasyWebSocketServer = nullptr;

void cEasyWebSocketServerInit(const char*_strCetPath,const char*e_pKeyPath)
{
    g_pEasyWebSocketServer = new cEasyWebSocketServer(9992, _strCetPath, e_pKeyPath);
    g_pEasyWebSocketServer->run();
}

cEasyWebSocketServer::cEasyWebSocketServer( int port, const string certPath, const string& keyPath) : WebSocketServer( port, certPath,keyPath)
{
}

cEasyWebSocketServer::~cEasyWebSocketServer( )
{
}


void cEasyWebSocketServer::onConnect( int socketID )
{
    // Give this connection a random user ID
    FMLOG("User :%d connect",socketID );
    //Util::log( "New connection: " + handle );
    
    // Associate this handle with the connection
    //this->setValue( socketID, "handle", handle );

    // Let everyone know the new user has connected
    //this->broadcast( handle + " has connected." );
}

void cEasyWebSocketServer::onMessage(int socketID, char* e_pData, size_t e_uiDataLen)
{
    // Send the received message to all connected clients in the form of 'User XX: message...'
    //Util::log( "Received: " + data );
    //const string& message = this->getValue( socketID, "handle" ) + ": " + data;
    int l_iSize = e_uiDataLen+1;
    char* l_pData = new char[l_iSize];
    memcpy(l_pData, e_pData, l_iSize);
    l_pData[e_uiDataLen] = 0;
    std::string l_str = l_pData;
    FMLOG(l_pData, l_iSize);
    FMLOG("OnMessage:%d",l_iSize);
    if (0)
    {
        //https://krpc.github.io/krpc/communication-protocols/tcpip.html
        tutorial::AddressBook l_AddressBook;// = (tutorial::AddressBook::ParseFrom(()*)l_pData->pData;
        if (l_AddressBook.ParseFromArray(e_pData, e_uiDataLen))
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
    this->broadcast(e_pData,e_uiDataLen);
}

void cEasyWebSocketServer::onDisconnect( int socketID )
{
    FMLOG("disconnect");
    //const string& handle = this->getValue( socketID, "handle" );
    //Util::log( "Disconnected: " + handle );
    
    // Let everyone know the user has disconnected
    //const string& message = handle + " has disconnected.";
    //for (map<int, Connection*>::const_iterator it = this->connections.begin(); it != this->connections.end(); ++it)
    {
        //if (it->first != socketID)
        {
            // The disconnected connection gets deleted after this function runs, so don't try to send to it
            // (It's still around in case the implementing class wants to perform any clean up actions)
            //this->send(it->first, message);
        }
    }
}

void cEasyWebSocketServer::onError( int socketID, const string& message )
{
    FMLOG("OnError");
    //Util::log( "Error: " + message );
}

