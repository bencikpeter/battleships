//
//  network_backend.h
//  battleships
//
//  Created by Peter Bencik on 27.04.16.
//  Copyright © 2016 Peter Bencik. All rights reserved.
//

#ifndef network_backend_h
#define network_backend_h

#include <stdio>
#include <vector>

#include "asio-1.10.6/include/asio.hpp"

namespace network {
    
    namespace ip = boost::asio::ip;
    
    
    class NetworkManager {
        
        
        asio::io_service io_service;
        ip::udp::resolver resolver;
        ip::udp::resolver::query query;
        ip::udp::socket socket;
        
        ip::udp::rec_endpoint;
        ip::udp::send_endpoint;
        
        std::vector<char> send_buf;
        std::vector<char> rec_buf;
        
        
    public: /*API*/
        NetworkManager(std::string ip_address): io_service(), resolver(io_service), socket(io_service) {
            
            ip::udp::resolver::query query(ip::udp::v4(), ip_address, "battleships");
            rec_endpoint = *resolver.resolve(query);
            
            socket.open(udp::v4()));
        }
        
        void initialize() {
            send_buf = {0};
            socket.send_to(asio::buffer(send_buf), rec_endpoint);
        }
        
        void sender(std::vector<char> message){
            send_buf = message;
            socket.send_to(asio::buffer(send_buf), rec_endpoint);
        }
        
        std::vector<char> listener(){
            size_t len = socket.receive_from(asio::buffer(recv_buf), sender_endpoint);
            if (/* INIT message */) {
                <#statements#>
            } else /*somtehing else*/
        }
        
    };
}

#endif /* network_backend_h */
