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
/*
#include <iostream>
#include "network_backend.h"

using asio::ip::udp;

class UDPClient
{
public:
    UDPClient(
              asio::io_service& io_service,
              const std::string& host,
              const std::string& port
              ) : io_service_(io_service), socket_(io_service, udp::endpoint(udp::v4(), 0)) {
        udp::resolver resolver(io_service_);
        udp::resolver::query query(udp::v4(), host, port);
        udp::resolver::iterator iter = resolver.resolve(query);
        endpoint_ = *iter;
    }
    
    ~UDPClient()
    {
        socket_.close();
    }
    
    void send(const std::string& msg) {
        socket_.send_to(asio::buffer(msg, msg.size()), endpoint_);
    }
    
private:
    asio::io_service& io_service_;
    udp::socket socket_;
    udp::endpoint endpoint_;
};

int main()
{
    asio::io_service io_service;
    UDPClient client(io_service, "localhost", "12345");
    
    client.send("Hello, World!");
}*/


//
// blocking_udp_echo_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
/*
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "network_backend.h"

using asio::ip::udp;

enum { max_length = 1024 };

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: blocking_udp_echo_client <host> <port>\n";
            return 1;
        }
        
        asio::io_service io_service;
        
        udp::socket s(io_service, udp::endpoint(udp::v4(), 0));
        
        udp::resolver resolver(io_service);
        udp::endpoint endpoint = *resolver.resolve({udp::v4(), argv[1], argv[2]});
        
        std::cout << "Enter message: ";
        char request[max_length];
        std::cin.getline(request, max_length);
        size_t request_length = std::strlen(request);
        s.send_to(asio::buffer(request, request_length), endpoint);
        
        char reply[max_length];
        udp::endpoint sender_endpoint;
        size_t reply_length = s.receive_from(
                                             asio::buffer(reply, max_length), sender_endpoint);
        std::cout << "Reply is: ";
        std::cout.write(reply, reply_length);
        std::cout << "\n";
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    
    return 0;
}
*/