/*
 * toya1800
 * TCP socket class file
 */
#include "TCPsocket.h"

TCPsocket::TCPsocket() = default;

TCPsocket::~TCPsocket() = default;

void TCPsocket::connect(const char *ipaddress, in_port_t port) {
    // Create socket with domain, type and protcol
    if((iSockDesc = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        throw std::runtime_error("ERROR: could not create socket!");
    }

    struct sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    // Check if hostname or dotted
    if(checkFormat(const_cast<char *>(ipaddress), &(serverAddr.sin_addr.s_addr)) == -1) {
        throw std::invalid_argument("ERROR: invalid input. "
                                    "Either hostname or dotted-decimals can be used");
    }

    // Connect to the server (Not the function I created but the one from <sys/socket.h>)
    if((::connect(iSockDesc, (const sockaddr*) &serverAddr, sizeof (serverAddr))) == -1) {
        ::close(iSockDesc); // <sys/socket.h>
        throw std::runtime_error("ERROR: Could not connect to server!");
    }
}

void TCPsocket::connect(const std::string& ipaddress, in_port_t port) {
    connect(ipaddress.c_str(), port);
}

int TCPsocket::checkFormat(char *name, in_addr_t *addrp) {
    struct hostent *hp;

    if(std::isdigit((int)(*name))) {
        *addrp = inet_addr(name);
    } else {
        hp = gethostbyname(name);
        if(hp == nullptr) {
            return -1;
        }
        memcpy((char *) addrp, hp->h_addr_list[0], hp->h_length);
    }
    return 0;
}

void TCPsocket::close() const {
    r_close(iSockDesc);
}
