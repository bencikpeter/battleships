//
//  network_backend.h
//  battleships
//
//  Created by Peter Bencik on 27.04.16.
//  Copyright © 2016 Peter Bencik. All rights reserved.
//

#ifndef network_backend_h
#define network_backend_h

#define ASIO_STANDALONE

#include <iostream>
#include <vector>

#include "../asio-1.10.6/include/asio.hpp"

namespace network {
    
    namespace ip = asio::ip;
    
    
    class NetworkManager {
        
        asio::io_service io_service;
        ip::udp::socket socket;
        
        ip::udp::endpoint rec_endpoint ;
        ip::udp::endpoint send_endpoint;
        
        std::vector<char> send_buf;
        std::vector<char> rec_buf;
        
        
    public: /*API*/
        NetworkManager(std::string const & ip_address): io_service(), socket(io_service,ip::udp::endpoint(ip::udp::v4(), 0)) {
            
            ip::udp::resolver resolver(io_service);
            ip::udp::resolver::query query(ip::udp::v4(), ip_address, "1337");
            rec_endpoint = *resolver.resolve(query);
            //socket = ip::udp::socket(io_service, rec_endpoint);
            //if(!socket.is_open()) socket.open(ip::udp::v4());
            //JUST TEST
        }
        
        ~NetworkManager(){
            socket.close();
        }
        
        void initialize() {
            send_buf = {0}; //define some kind of init message
            socket.send_to(asio::buffer(send_buf), rec_endpoint);
        }
        
        void sender(std::vector<char> message){
            send_buf = message;
            socket.send_to(asio::buffer(send_buf), rec_endpoint);
        }
        
        std::vector<char> listener(){
            size_t len = socket.receive_from(asio::buffer(rec_buf), send_endpoint);
            if (isInitMessage(rec_buf)) {
                return rec_buf;
            } else{
                return rec_buf;
            }
        }
    private:
        bool isInitMessage(std::vector<char> message){
            return message.size() == 1 && message[0] == 0;
        }
        
    };
}

#endif /* network_backend_h */
