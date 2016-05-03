//
//  main.cpp
//  battleships
//
//  Created by Peter Bencik on 28.04.16.
//  Copyright © 2016 Peter Bencik. All rights reserved.
//



#include <iostream>
#include "network_backend.h"

int main(){
    network::NetworkManager manager("localhost");
    std::string message = "message";
    //manager.initialize();
    manager.sender(message);
    
    //std::vector<char> recieved = manager.listener();
    //for(int i =0; i< recieved.size(); ++i) std::cout << recieved[i];
    
    /*while (true) {
        message = manager.listener();
        std::cout << "Message: " << message << std:: endl;
    }*/
    
    //std::cout << manager.getMyIPAddress() << std::endl;;
}