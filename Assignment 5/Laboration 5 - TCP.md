# Laboration 5 - TCP/IP
### Envoirment & Tools / Utvecklingsmiljö & Verktyg
Operating system: Ubuntu 20.04.1 LTS
IDE: CLion
Compiler: GCC

### Purpose / Syfte
Purpose of this laboration is to implement sockets in order to create a connection between a client and a server.
The assignment is divided into 2 parts. In the first part we only have to create a very simple socket-class named
"TCPsocket", which will be used to create a connection to the server. 
In the second part we have to implement the logic to connect to the server and play a game of "luffarschack".
When connected, the host and client will send commands to each other to start and play the game.

### Procedures / Genomförande
First we create the TCPsocket with the help of the template provided by the assigment paper.
```cpp
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
```
We create socket with standard values for the sockaddr_in like AF_INET, port and also format since 2 different ways can be used with just decimals or plain text. We then used the TCPsocket to connect to "luffarserv.miun.se" in port 6500.
When starting the game we ask for user input to start the connection with the challenge message format CHA:X:Y:NN:name.

I used the r_write function from the Restart.h file to send the message to the server.
```cpp
   if(r_write(tcPsocket->getDescriptor(), (void *) chaMes.c_str(), chaMes.size()) == -1) {
        perror("Could not send message to server!");
        return;
    }
```
Sending the descriptor, message and the size of the message to the server.
To listen to the server I used the poll function. Using a 1 second timeout. It's very simple from here since the descriptor is already handled and you don't have to wait forever if the server for some reason can't respond.
```cpp
        /*
         * Using poll to check if server
         * has terminated the connection or
         * users turn to send data
         */
        struct pollfd pollfd{};
        pollfd.fd = tcPsocket->getDescriptor();
        pollfd.events = POLLIN;

        int response;
        if((response = poll(&pollfd, 1, 1000)) == -1) {
            perror("Server has terminated the session!");
            continueGame = false;
        }
```
I then used the same r_write function to send the moves to the server with the format MOV:XX:YY(\n).
And finally to get the data from the server, which is written in the same way I send them, I used the function recv.
```cpp
char readBUF[BUFSIZE]; // 512
ssize_t bytesRead = recv(tcPsocket->getDescriptor(), readBUF, BUFSIZE, 0); // Getting data
```
The rest is mostly just if and else statements to tell the program what to do 
depending on what the server sends to the client like MOV, NAP, ILM etc.
### Discussion / Diskussion
The purpose has been fullfilled. All the areas of sockets, client and server have been used
to establish a connection and play a game of "luffarschak". It was not an easy aassignment, 
it was both tricky and pretty big. But I learned a lot 
about sockets in C/C++ and could also use my prior knowledge of it from Java
to compare and see the similarities. Since the assigment was pretty big some more
help/guide could have been used regarding how we should impement the logic
with recieving the data from the server and which library functions could 
be used to help. What I could have done better is probably using more functions
when using a lot of code which goes "off topic" like tranforming from int to string
or the other way around to make the code look cleaner. 
