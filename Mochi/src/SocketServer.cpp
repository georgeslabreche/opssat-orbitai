#include <sys/socket.h> /* For socket functions */
#include <netinet/in.h> /* For sockaddr_in */
#include <unistd.h>

#include "Constants.hpp"
#include "SocketServer.hpp"

/**
 * Initialize the socket server and start listening for a connection.
 */
int SocketServer::initSocketServer(int portNumber, int *pSockfd, int *pConnection)
{
    /* Create a socket (IPv4, TCP) */
    *pSockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (*pSockfd == -1)
    {
        /* Exit with error code. */
        return ERROR_CREATE_SOCKET;
    }

    /* Listen to port on any address */
    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;

    /* Invoking htons is necessary to convert a number to network byte order. */
    sockaddr.sin_port = htons(portNumber);

    /* Bind to the given port. */
    if (bind(*pSockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
    {
        /* Exit with error code. */
        return ERROR_BIND_PORT;
    }

    /* Start listening. Hold at most 1 connection in the queue. */
    if (listen(*pSockfd, 1) < 0)
    {
        /* Exit with error code. */
        return ERROR_LISTEN_SOCKET;
    }

    /* Grab a connection from the queue. */
    auto addrlen = sizeof(sockaddr);
    *pConnection = accept(*pSockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);

    if (*pConnection < 0)
    {
        /* Exit with error code. */
        return ERROR_GRAB_CONNECTION;
    }

    return NO_ERROR;
}

/**
 * Close server socket and connection.
 */
void SocketServer::shutdownSocketServer(int *pSockfd, int *pConnection)
{
    /* Close the connection */
    if(*pConnection)
    {
        close(*pConnection);
    }

    /* Close the socket. */
    if(*pSockfd)
    {
        close(*pSockfd);
    }
}