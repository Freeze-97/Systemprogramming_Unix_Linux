/*
 * toya1800
 * Laboration 5 - TCP
 */

#include <iostream>
#include <limits>
#include <random>
#include <sys/poll.h>
#include <iomanip>
#include "TCPsocket.h"
#define BUFSIZE 512

void startGame(TCPsocket *tcPsocket);
std::string chooseMove();
int performCommand(std::string lastMove, std::string &commands, char allCells[15][15]);
void printArea(char cells[15][15]);
/*
 * Function which splits string by delim and returns it in a vector
 * From: https://stackoverflow.com/a/65075284
 */
std::vector<std::string> split(std::string  &text, char delim);

int main() {
    // Connect to server
    TCPsocket tcPsocket;
    tcPsocket.connect("luffarserv.miun.se", 6500);

    // Start the game
    startGame(&tcPsocket);

    // Close connection/socket
    tcPsocket.close();
    return 0;
}

void startGame(TCPsocket *tcPsocket) {
    // The game area with 15x15 cells
    const int ROW = 15;
    const int COL = 15;
    char gameCells[ROW][COL];

    // Give every cell a neutral value '-'
    for (auto &gameCell: gameCells) {
        for (char &j: gameCell) {
            j = '-';
        }
    }

    // Ask for users name
    std::cout << "Name: " << std::endl;
    std::string name;
    std::cin >> name;
    std::cin.clear();

    // If the name is smaller than 10, give it a 0 as the first num
    std::string nameSize;
    if(name.length() < 10) {
        nameSize.append("0");
    }
    nameSize.append(std::to_string(name.length()));

    /*
     * Ask user if he wants to play vs
     * striker(0)
     * or
     * defender(1)
     */
    std::cout << "Which opponent do you wish to play against? "
                 "Striker(0) or Defender(1)" << std::endl;
    int opponent = -1;
    bool opponentOK = false;
    do {
        while (!(std::cin >> opponent)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input." << std::endl;
        }
        // Only valid input is 0 or 1
        if(opponent == 0 || opponent == 1) {
            opponentOK = true;
        }
    } while(!opponentOK);

    // Randomize the starter - 0 is player and 1 is AI
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0,1);
    int startingPlayer = dist(rd);

    // Send challenge message
    std::string chaMes = "CHA:" +
            std::to_string(opponent) +
            ":" + std::to_string(startingPlayer) +
            ":" + nameSize +
            ":" + name + "\n";

    if(r_write(tcPsocket->getDescriptor(), (void *) chaMes.c_str(), chaMes.size()) == -1) {
        perror("Could not send message to server!");
        return;
    }

    // Store the last move made
    std::string lastMove;

    // The game starts here
    bool continueGame = true;
    while(continueGame) {
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
        } else if(response == 0) {
            // Pick move to send to server
            chaMes = chooseMove();
            lastMove = chaMes;
            std::cout << "LAST MOVE 1: " << lastMove << std::endl;

            // Sending the move to the server
            if(r_write(tcPsocket->getDescriptor(), (void *) chaMes.c_str(), chaMes.size()) == -1) {
                perror("Could not send message to server!");
                continueGame = false;
                return;
            }
            if(chaMes == "QUI\n") {
                continueGame = false;
            }
        } else { // Getting server data
            char readBUF[BUFSIZE]; // 512
            ssize_t bytesRead = recv(tcPsocket->getDescriptor(), readBUF, BUFSIZE, 0); // Getting data

            // -1 means error
            if(bytesRead == -1) {
                perror("Error when trying to read from server");
                continueGame = false;
                return;
            }
            std::string command; // Command gotten from the server
            if(bytesRead == 0) {  // 0 means got nothing from server
                if (lastMove == "QUI\n") { // Last command is quit from user
                    command = "QUI\n";
                }
            } else {
                // Put all data into the command variable
               for(int index = 0; index < bytesRead; index++) {
                   command += readBUF[index];
               }
            }
            /*
             * Perform the command.
             * Depedning on what the command is.
             * Returning 0 means continue.
             * Returning -1 means some sort of error occurred
             * and the game will be cancelled.
             */
            int returnValue = performCommand(lastMove, command, gameCells);
            if(returnValue == -1 && !command.empty()) { // -1 means end/cancel
                continueGame = false;
            }
            lastMove = "";
        }
    }
}

std::string chooseMove() {
    int input = -1;
    bool inputOK = false;
    do {
        std::cout << "Do you want to quit?" << std::endl;
        std::cout << "0 - No" << std::endl;
        std::cout << "1 - Yes" << std::endl;
        while (!(std::cin >> input)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input" << std::endl << std::endl;
        }
        std::cin.clear();
        // Only valid input is 0 or 1
        if(input == 0 || input == 1) {
            inputOK = true;
        }
    } while(!inputOK);

    std::string move; // The full move

    if(input == 1) { // Quit the game
        move = "QUI\n";
    } else { // Continue with move
        // Ask for x cord
        int x;
        inputOK = false;
        do {
            std::cout << "Move X-cord (0-14): " << std::endl;
            while (!(std::cin >> x)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input" << std::endl << std::endl;
            }
            std::cin.clear();
            // Only valid input is 0 or 1
            if(x >= 0 && x <= 14) {
                inputOK = true;
            }
        } while(!inputOK);

        // Ask for y cord
        int y;
        inputOK = false;
        do {
            std::cout << "Move Y-cord (0-14): " << std::endl;
            while (!(std::cin >> y)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input" << std::endl << std::endl;
            }
            std::cin.clear();
            // Only valid input is 0 or 1
            if(y >= 0 && y <= 14) {
                inputOK = true;
            }
        } while(!inputOK);

        // Get numbers as XX and YY
        std::string xCord;
        std::string yCord;

        if(x < 10) {
            xCord.append("0");
        }
        xCord.append(std::to_string(x));

        if(y < 10) {
            yCord.append("0");
        }
        yCord.append(std::to_string(y));

        // Create full message for the move command
        move = "MOV:"
                + xCord + ":"
                + yCord
                + "\n";
    }
    return move;
}

int performCommand(std::string lastMove, std::string &commands, char allCells[15][15]) {
    // Vectors with the players last commands and the AI
    std::vector<std::string> lastPlayMove;
    std::vector<std::string> AIMessage;

    // Split player move by ':' to get XX at [1] and YY at [2]
    lastPlayMove = split(lastMove, ':');

    // Split commands from AI
    for(std::string &com : split(commands, '\n')) { // For every command (Split by '\n'
        if(com.back() == '\n') {
            com.pop_back(); // Get rid of last element
        }

        /*
         * Split by ':'
         * [0] = Command like MOV, OKR etc
         * [1] = XX
         * [2] = YY
         * This way we can extract the different parts
         * of one command.
         */
        AIMessage = split(com, ':');

        // From here we handle all possible commands with if-statements
        if(AIMessage[0] == "OKR") {
            std::cout << std::endl;
            std::cout << "The game has started!" << std::endl;
            std::cout << "The player is 'X' and the AI i '0'" << std::endl;
            printArea(allCells);
        } else if(AIMessage[0] == "MOV") {
            std::cout << std::endl;

            // Get XX and YY from server and get it as int
            int x = std::stoi(AIMessage[1]);
            int y = std::stoi(AIMessage[2]);

            std::cout << "AI move: X = " << x << ", Y = " << y << std::endl;
            allCells[y][x] = '0';

            // Move from the player/client side
            if(!lastMove.empty()) {
                // Get XX and YY and get it as int
                x = std::stoi(lastPlayMove[1]);
                y = std::stoi(lastPlayMove[2]);

                std::cout << "Player move: X = " << x << ", Y = " << y << std::endl;
                allCells[y][x] = 'X';
            }
            printArea(allCells);
        } else if(AIMessage[0] == "WIN") {
            std::cout << "Winning row: [X = " + AIMessage[1] + ", Y = " + AIMessage[2] + "] " +
            "[X = " + AIMessage[3] + ", Y = " + AIMessage[4] + "] " +
                    "[X = " + AIMessage[5] + ", Y = " + AIMessage[6] + "] " +
                    "[X = " + AIMessage[7] + ", Y = " + AIMessage[8] + "] " +
                    "[X = " + AIMessage[9] + ", Y = " + AIMessage[10] + "] " << std::endl;

            return -1;
        } else if(AIMessage[0] == "ILM") {
            std::string errorMsg = "ILM = Illegal move made: [X = " + AIMessage[1] +
                                ", Y = " + AIMessage[2] + "]. "
                                                          "Connection terminated.";
            perror(errorMsg.c_str());

            return -1;
        } else if(AIMessage[0] == "NAP") {
            std::string errorMsg = "NAP = No Active Player, cannot send command before "
                                   "CHA or getting OKR from server! "
                                   "Connection terminated.";
            perror(errorMsg.c_str());

            return -1;
        } else if(AIMessage[0] == "ILC") {
            std::string errorMsg = "ILC = Illegal command: " +
                    lastMove + ", incomplete command! "
                                   "Connection terminated.";
            perror(errorMsg.c_str());

            return -1;
        } else if(AIMessage[0] == "TCL") {
            std::string errorMsg = "TCL = (Timout, closing), "
                                   "90 seconds since last message. "
                                   "Connection terminated.";
            perror(errorMsg.c_str());

            return -1;
        } else if(AIMessage[0] == "QUI") {
            std::string errorMsg = "QUI = Quit session. Connection terminated.";
            perror(errorMsg.c_str());

            return -1;
        }
     }

    return 0; // No failure occurred
}

std::vector<std::string> split(std::string  &text, char delim) {
    std::string line;
    std::vector<std::string> vec;
    std::stringstream ss(text);
    while(std::getline(ss, line, delim)) {
        vec.push_back(line);
    }
    return vec;
}

void printArea(char cells[15][15]) {
    for(int row = 0; row < 15; row++) {
        // Create space up top to the left
        if(row == 0) {
            std::cout << std::left << std::setw(4) << " ";
        } else { // Print row num
            std::cout << std::left << std::setw(4) << row - 1;
        }
        // Columns
        for(int col = 0; col < 15; col++) {
            // So the column looks nice
            if(row == 0) {
                std::cout << std::left << std::setw(4) << col;
            } else {
                std::cout << std::left << std::setw(4) << cells[row - 1][col];
            }
        }
        std::cout << '\n'; // Next row
    }
}