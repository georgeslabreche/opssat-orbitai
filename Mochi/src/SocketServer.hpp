#ifndef SOCKET_SERVER_H_
#define SOCKET_SERVER_H_

using namespace std;

class SocketServer
{
public:

    /* Constructor. */
    SocketServer() {}

    /* Init the socket server and start listening for a connection. */
    int initSocketServer(int portNumber, int *pSockfd, int *pConnection);

    /* Shutdown the socker server. */
    void shutdownSocketServer(int *pSockfd, int *pConnection);

};

#endif // SOCKET_SERVER_H_
