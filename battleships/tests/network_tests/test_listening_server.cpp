//
//  main2.cpp
//  battleships
//
//  Created by Peter Bencik on 28.04.16.
//  Copyright © 2016 Peter Bencik. All rights reserved.
//

//
// blocking_udp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <cstdlib>
#include <iostream>
#include "network_backend.h"

using asio::ip::udp;

enum { max_length = 1024 };

void server(asio::io_service& io_service, unsigned short port)
{
    udp::socket sock(io_service, udp::endpoint(udp::v4(), port));
    for (;;)
    {
        char data[max_length];
        udp::endpoint sender_endpoint;
        size_t length = sock.receive_from(
                                          asio::buffer(data, max_length), sender_endpoint);
        for(int i = 0; i < length; i++){
            std::cout << data[i];
        }
        std::cout << std::endl;
        //sock.send_to(asio::buffer(data, length), sender_endpoint);
    }
}

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: blocking_udp_echo_server <port>\n";
            return 1;
        }
        
        asio::io_service io_service;
        
        server(io_service, std::atoi(argv[1]));
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    
    return 0;
}

