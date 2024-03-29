/*
  SDL_net:  An example cross-platform network library for use with SDL
  Copyright (C) 1997-2016 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/* $Id$ */

#include "SDLnetsys.h"
#include "SDL_net.h"
#include <exception>
#include "../Common/Log/FMLog.h"
/* The network API for TCP sockets */

/* Since the UNIX/Win32/BeOS code is so different from MacOS,
   we'll just have two completely different sections here.
*/

/* Open a TCP network socket
   If 'remote' is NULL, this creates a local server socket on the given port,
   otherwise a TCP connection to the remote host and port is attempted.
   The newly created socket is returned, or NULL if there was an error.
*/
TCPsocket SDLNet_TCP_Open(IPaddress *ip)
{
    TCPsocket sock;
    struct sockaddr_in sock_addr;

    /* Allocate a TCP socket structure */
    sock = (TCPsocket)malloc(sizeof(*sock));
    if ( sock == NULL ) 
    {
        SDLNet_SetError("Out of memory");
        goto error_return;
    }

    /* Open the socket */
#ifdef WASM
    //https://blog.squareys.de/emscripten-sockets/
    printf("call socket start");
    //	sock->channel = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sock->channel = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);// 
    //sock->channel = socket(AF_INET, SOCK_STREAM, 0);
    printf("call socket finish");
    //fcntl(sock->channel, F_SETFL, O_NONBLOCK);
#else
	sock->channel = socket(AF_INET, SOCK_STREAM, 0);
#endif

    if ( sock->channel == INVALID_SOCKET ) 
    {
        SDLNet_SetError("Couldn't create socket");
        goto error_return;
    }
    /* Connect to remote, or bind locally, as appropriate */
    if ( (ip->host != INADDR_NONE) && (ip->host != INADDR_ANY) ) 
    {

    // #########  Connecting to remote

        memset(&sock_addr, 0, sizeof(sock_addr));
        sock_addr.sin_family = AF_INET;
        sock_addr.sin_addr.s_addr = ip->host;
        sock_addr.sin_port = ip->port;
#ifdef WASM
        printf("call connect start");
        //auto l_strHost = SDLNet_ResolveIP(ip);
        //printf("do SDLNet_ResolveIP get ");
        //printf(l_strHost);
        //printf("\n");
        //if (inet_pton(AF_INET, l_strHost, &sock_addr.sin_addr) != 1)
        //{
        //    SDLNet_SetError("Socket::connect(): inet_pton failed");
        //    goto error_return;
        //}
#endif
        /* Connect to the remote host */
        SDLNet_SetError("try SDLTCP connect 1");
        if ( connect(sock->channel, (struct sockaddr *)&sock_addr,sizeof(sock_addr)) == SOCKET_ERROR ) 
        {
            //SDLNet_SetError("try SDLTCP connect 2");
            if (errno == EINPROGRESS)
            {

                /* Wait for connection to complete */
                fd_set sockets;
                FD_ZERO(&sockets);
                FD_SET(sock->channel, &sockets);

            //    /* You should probably do other work instead of busy waiting on this...
            //       or set a timeout or something */
                SDLNet_SetError("wait select");
            //    while (select(sock->channel + 1, nullptr, &sockets, nullptr, nullptr) <= 0) {}
                timeval timeout_;
#define SELECT_TIME_OUT 1000
                timeout_.tv_sec = SELECT_TIME_OUT / 1000;
                timeout_.tv_usec = (SELECT_TIME_OUT % 1000) * 1000;
                //int selret = ::select(mSocket + 1, NULL, &writefs, &exceptfs, &timeout_);
                select(sock->channel + 1, nullptr, &sockets, nullptr, nullptr);// 
                SDLNet_SetError("select finish");
                Sleep(100);
            }
            else
            {
                SDLNet_SetError("Couldn't connect to remote host");
                goto error_return;
            }
        }
        sock->iServerFlag = 0;
    }
    else 
    {
    // ##########  Binding locally

        memset(&sock_addr, 0, sizeof(sock_addr));
        sock_addr.sin_family = AF_INET;
        sock_addr.sin_addr.s_addr = INADDR_ANY;
        sock_addr.sin_port = ip->port;

/*
 * Windows gets bad mojo with SO_REUSEADDR:
 * http://www.devolution.com/pipermail/sdl/2005-September/070491.html
 *   --ryan.
 */
#ifndef WIN32
        printf("call connect finish");
        /* allow local address reuse */
        { int yes = 1;
            setsockopt(sock->channel, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes));
        }
#endif
        /* Bind the socket for listening */
        if ( bind(sock->channel, (struct sockaddr *)&sock_addr,
                sizeof(sock_addr)) == SOCKET_ERROR ) {
            SDLNet_SetError("Couldn't bind to local port");
            goto error_return;
        }
        if ( listen(sock->channel, 5) == SOCKET_ERROR ) {
            SDLNet_SetError("Couldn't listen to local port");
            goto error_return;
        }

        /* Set the socket to non-blocking mode for accept() */
#if defined(__BEOS__) && defined(SO_NONBLOCK)
        /* On BeOS r5 there is O_NONBLOCK but it's for files only */
        {
            long b = 1;
            setsockopt(sock->channel, SOL_SOCKET, SO_NONBLOCK, &b, sizeof(b));
        }
#elif defined(O_NONBLOCK)
        {
            fcntl(sock->channel, F_SETFL, O_NONBLOCK);
        }
#elif defined(WIN32)
        {
            unsigned long mode = 1;
            ioctlsocket (sock->channel, FIONBIO, &mode);
        }
#elif defined(__OS2__)
        {
            int dontblock = 1;
            ioctl(sock->channel, FIONBIO, &dontblock);
        }
#else
#warning How do we set non-blocking mode on other operating systems?
#endif
        sock->iServerFlag = 1;
    }
    sock->ready = 0;

#ifdef TCP_NODELAY
    /* Set the nodelay TCP option for real-time games */
    { int yes = 1;
    setsockopt(sock->channel, IPPROTO_TCP, TCP_NODELAY, (char*)&yes, sizeof(yes));
    }
#else
#warning Building without TCP_NODELAY
#endif /* TCP_NODELAY */

    /* Fill in the channel host address */
    sock->remoteAddress.host = sock_addr.sin_addr.s_addr;
    sock->remoteAddress.port = sock_addr.sin_port;

    /* The socket is ready */
    return(sock);

error_return:
    SDLNet_SetError("SDLNet_TCP_Open open failed call close");
    SDLNet_TCP_Close(sock);
    return(NULL);
}

/* Accept an incoming connection on the given server socket.
   The newly created socket is returned, or NULL if there was an error.
*/
TCPsocket SDLNet_TCP_Accept(TCPsocket server)
{
    TCPsocket sock;
    struct sockaddr_in sock_addr;
    socklen_t sock_alen;

    /* Only server sockets can accept */
    if ( ! server->iServerFlag) {
        SDLNet_SetError("Only server sockets can accept()");
        return(NULL);
    }
    server->ready = 0;

    /* Allocate a TCP socket structure */
    sock = (TCPsocket)malloc(sizeof(*sock));
    if ( sock == NULL ) {
        SDLNet_SetError("Out of memory");
        goto error_return;
    }

    /* Accept a new TCP connection on a server socket */
    sock_alen = sizeof(sock_addr);
    sock->channel = accept(server->channel, (struct sockaddr *)&sock_addr,
                                &sock_alen);
    if ( sock->channel == INVALID_SOCKET ) {
        SDLNet_SetError("accept() failed");
        goto error_return;
    }
#ifdef WIN32
    {
        /* passing a zero value, socket mode set to block on */
        unsigned long mode = 0;
        ioctlsocket (sock->channel, FIONBIO, &mode);
    }
#elif defined(O_NONBLOCK)
    {
        int flags = fcntl(sock->channel, F_GETFL, 0);
        fcntl(sock->channel, F_SETFL, flags & ~O_NONBLOCK);
    }
#endif /* WIN32 */
    sock->remoteAddress.host = sock_addr.sin_addr.s_addr;
    sock->remoteAddress.port = sock_addr.sin_port;

    sock->iServerFlag = 0;
    sock->ready = 0;

    /* The socket is ready */
    return(sock);

error_return:
    SDLNet_TCP_Close(sock);
    return(NULL);
}

/* Get the IP address of the remote system associated with the socket.
   If the socket is a server socket, this function returns NULL.
*/
IPaddress *SDLNet_TCP_GetPeerAddress(TCPsocket sock)
{
    if ( sock->iServerFlag) {
        return(NULL);
    }
    return(&sock->remoteAddress);
}

/* Send 'len' bytes of 'data' over the non-server socket 'sock'
   This function returns the actual amount of data sent.  If the return value
   is less than the amount of data sent, then either the remote connection was
   closed, or an unknown socket error occurred.
*/
int SDLNet_TCP_Send(TCPsocket sock, const void *datap, int len)
{
    const Uint8 *data = (const Uint8 *)datap;   /* For pointer arithmetic */
    int sent, left;

    /* Server sockets are for accepting connections only */
    if ( sock->iServerFlag) {
        SDLNet_SetError("Server sockets cannot send");
        return(-1);
    }

    /* Keep sending data until it's sent or an error occurs */
    left = len;
    sent = 0;
    SDLNet_SetLastError(0);
    do {
        if (sock->ready != 0 && sock->ready != 1)
            break;
#ifdef DEBUG
        try
        {
#endif
            len = send(sock->channel, (const char*)data, left, 0);
#ifdef DEBUG
        }
        catch (std::exception e)
        {
            std::string l_str = "Error:SDLNet_TCP_Send:";
            l_str += e.what();
            FMLog::Log(l_str.c_str(),true);
        }
#endif
        if ( len > 0 ) {
            sent += len;
            left -= len;
            data += len;
        }
    } while ( (left > 0) && ((len > 0) || (SDLNet_GetLastError() == EINTR)) );

    return(sent);
}

/* Receive up to 'maxlen' bytes of data over the non-server socket 'sock',
   and store them in the buffer pointed to by 'data'.
   This function returns the actual amount of data received.  If the return
   value is less than or equal to zero, then either the remote connection was
   closed, or an unknown socket error occurred.
*/
int SDLNet_TCP_Recv(TCPsocket sock, void *data, int maxlen)
{
    int len;

    /* Server sockets are for accepting connections only */
    if ( sock->iServerFlag) {
        SDLNet_SetError("Server sockets cannot receive");
        return(-1);
    }

    SDLNet_SetLastError(0);
    do {
        len = recv(sock->channel, (char *) data, maxlen, 0);
    } while ( SDLNet_GetLastError() == EINTR );

    sock->ready = 0;
    return(len);
}

/* Close a TCP network socket */
void SDLNet_TCP_Close(TCPsocket sock)
{
    if ( sock != NULL ) {
        if ( sock->channel != INVALID_SOCKET ) {
            closesocket(sock->channel);
        }
        free(sock);
    }
}
