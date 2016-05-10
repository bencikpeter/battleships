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
    std::vector<char> message = {'m','e','s','s','a','g','e'};
    //manager.initialize();
    //manager.sender(message);
    
    //std::vector<char> recieved = manager.listener();
    //for(int i =0; i< recieved.size(); ++i) std::cout << recieved[i];
    
    while (true) {
        message = manager.listener();
        //std::cout << "Message: " << message << std:: endl;
    }
    
    //std::cout << manager.getMyIPAddress() << std::endl;;
}