//
//  main.cpp
//  battleships
//
//  Created by Peter Bencik on 28.04.16.
//  Copyright © 2016 Peter Bencik. All rights reserved.
//



#include <iostream>
#include "network_backend.h"

void printMessage(std::vector<char> message) {
    for (int i = 0; i < message.size(); ++i) {
        std::cout << message[i];
    }
}

std::vector<char> toVector(std::string const &_string){
    std::vector<char> message;
    for (char const & c : _string) {
        message.push_back(c);
    }
    return message;
}

void host(){
    network::NetworkManager manager;
    std::cout << "Waiting for init" << std::endl;
    manager.waitForIinit();
    std::cout << "Init completed" << std::endl;
    while (true) {
        std::cout << "Waiting for message: " << std::endl;
        std::vector<char> message = manager.listener();
        std::cout << "Message: ";
        printMessage(message);
        std::cout << std::endl << "Write your reply: ";
        std::string reply;
        std::cin >> reply;
        message = toVector(reply);
        manager.sender(message);
        std::cout << std::endl << "Reply sent" << std::endl;
    }
}

void connect(){
    network::NetworkManager manager("25.43.227.247");
    std::cout << "Initializing" << std::endl;
    manager.initialize();
    std::cout << "Initializing complete" << std::endl;
    while (true) {
        std::vector<char> message;
        std::cout << std::endl << "Write your message: ";
        std::string reply;
        std::cin >> reply;
        message = toVector(reply);
        manager.sender(message);
        std::cout << std::endl << "Message sent" << std::endl;
        std::cout << "Waiting for reply: " << std::endl;
        message = manager.listener();
        std::cout << "Message: ";
        printMessage(message);
    }
}


int main(int argc, char* argv[]){
    if (argc != 2)
    {
        std::cerr << "Usage: testing <mode>\n Modes: h & c";
        return 1;
    }
    
    if (!strcmp(argv[1], "h")) {
        host();
    } else {
        connect();
    }
}