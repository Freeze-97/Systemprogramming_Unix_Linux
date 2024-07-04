/*
 * toya1800
 * TCP socket class header
 */

#ifndef DT065G_TOYA1800_LABB5_TCPSOCKET_H
#define DT065G_TOYA1800_LABB5_TCPSOCKET_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <stdexcept>
#include <cstring>
#include <sys/socket.h>
#include "Restart.h"

class TCPsocket {
private:
    int iSockDesc; // Socket descriptor

public:
    // Constructor
    TCPsocket();

    // Destructor
    ~TCPsocket();

    // Connect to the server
    // Either with hostname or a dotted decimal address
    void connect(const char *ipaddress, in_port_t port);
    void connect(const std::string& ipaddress, in_port_t port);

    // Return socket descriptor
    int getDescriptor() const { return iSockDesc; }

    /*
     * This function is directly taken from the book
     * to solve the problem around if the user
     * is using hostname or dotted-decimal
     */
    static int checkFormat(char *name, in_addr_t *addrp);

    // Close socket
    void close() const;
};

#endif //DT065G_TOYA1800_LABB5_TCPSOCKET_H
