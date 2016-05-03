//
// blocking_udp_echo_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
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
        
        std::vector<char> message = {'a','l','o','h','a'};
        while (true) {
            s.send_to(asio::buffer(message), endpoint);
            std::cout << "message sent" << std::endl;
            getchar();
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    
    return 0;
}
