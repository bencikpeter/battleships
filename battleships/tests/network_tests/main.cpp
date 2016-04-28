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
    manager.sender(message);
    
    //std::vector<char> recieved = manager.listener();
    //for(int i =0; i< recieved.size(); ++i) std::cout << recieved[i];
    
    
    
    
}
/*
    int main(int argc, char* argv[])
    {
        using asio::ip::udp;
        try
        {
            
            asio::io_service io_service;
            
            udp::resolver resolver(io_service);
            udp::resolver::query query(udp::v4(), "localhost", "54321");
            udp::endpoint receiver_endpoint = *resolver.resolve(query);
            
            udp::socket socket(io_service);
            socket.open(udp::v4());
            
            std::vector<char> send_buf  = {'a','h','o','j'};
            socket.send_to(asio::buffer(send_buf), receiver_endpoint);
            
            std::vector<char> recv_buf;
            udp::endpoint sender_endpoint;
            size_t len = socket.receive_from(asio::buffer(recv_buf), sender_endpoint);
            
            std::cout.write(recv_buf.data(), len);
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
        
        return 0;
    }*/
