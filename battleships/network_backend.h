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
#include <exception>
#include <regex>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
        /**
         * Constructor for side knowing the other IP
         * inits all the network properties
         * bool initialize(std::string const & ip_address) must be called  with my local adress to establish connection
         * @param ip_address IP address of the other side
         */
        NetworkManager(std::string const & ip_address): io_service(), socket(io_service,ip::udp::endpoint(ip::udp::v4(), 1337)),send_endpoint(ip::udp::v4(), 1337) {
            
            ip::udp::resolver resolver(io_service);
            ip::udp::resolver::query query(ip::udp::v4(), ip_address, "1337");
            rec_endpoint = *resolver.resolve(query);
        }
        /**
         * Constructor for side not knowing the other IP
         * inits the necessary network properties
         * bool waitForIinit() must be called to establish conection
         */
        NetworkManager(): io_service(), socket(io_service,ip::udp::endpoint(ip::udp::v4(), 1337)), send_endpoint(ip::udp::v4(), 1337) { }
        
        ~NetworkManager(){
            socket.close();
        }
        
        /**
         * informing other party about my adress
         * @returns true if succesfull
         */
        bool initialize() {
            try {
                std::string ip_address = getMyIPAddress();
                /*send_buf.clear();
                send_buf = toCharVector(ip_address);
                socket.send_to(asio::buffer(send_buf), rec_endpoint);*/
                sender(toCharVector(ip_address));
                return true;
            }catch (std::exception& ex ) {
                return false;
            }
            
        }
        /**
         * waiting for other party to reveal it's IP address
         * @returns true of succesfull
         */
        bool waitForIinit(){
            listener();
            if (isInitMessage(rec_buf)) {
                ip::udp::resolver resolver(io_service);
                ip::udp::resolver::query query(ip::udp::v4(), toString(rec_buf), "1337");
                rec_endpoint = *resolver.resolve(query);
                return true;
            }
            return false;

        }
        /**
         * sends a message to other party
         * @param messasge
         * @returns true if sended succesfully
         */
        bool sender(std::vector<char> message){
            try {
                send_buf = message;
                socket.send_to(asio::buffer(send_buf), rec_endpoint);
                return true;
            } catch (std::exception& ex ) {
                return false;
            }
        }
        /**
         * waits for a message from the other party
         * @returns message recieved message
         */
        std::vector<char> listener(){
            rec_buf.clear();
            rec_buf.resize(1024);
            size_t len = socket.receive_from(asio::buffer(rec_buf), send_endpoint);
            return rec_buf;
        }
    //private: /*FOR TESTING PURPOSES*/
        bool isInitMessage(std::vector<char> message){
            std::string toBeTested = toString(message);
            //std::regex reg ("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
            std::regex reg("^(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)$"); //wont be sending numerical stuff, so this is sufficient
            return std::regex_match(toBeTested,reg);
        }
        
        std::vector<char> toCharVector(std::string const &_string){
            std::vector<char> message;
            for (char const & c : _string) {
                message.push_back(c);
            }
            return message;
        }
        
        std::string toString(std::vector<char> message){
            
            std::string _string;
            for(char &c : message){
                _string+=c;
            }
            return _string;
        }
        
        std::string getMyIPAddress(){
            
            struct ifaddrs * addresses = nullptr;
            struct ifaddrs * addressTemp=NULL;
            void * tmpAddrPtr=NULL;
            
            getifaddrs(&addresses);
            
            for(addressTemp = addresses; addressTemp != nullptr; addressTemp = addressTemp->ifa_next){
                if (!addressTemp->ifa_addr) {
                    continue;
                }
                if (addressTemp->ifa_addr->sa_family == AF_INET){ //if valid IPv4
                    tmpAddrPtr=&((struct sockaddr_in *)addressTemp->ifa_addr)->sin_addr;
                    char addressBuffer[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
                    std::string textAddress;
                    textAddress.assign(addressBuffer);
                    if (isLocalAddress(textAddress)) {
                        if (addresses!=NULL) freeifaddrs(addresses);
                        return textAddress;
                    }
                }
            }
            
            if (addresses!=NULL) freeifaddrs(addresses);
            return std::string();
            
        }
        
        bool isLocalAddress(std::string const& address){
<<<<<<< HEAD
            //std::regex reg("(^127\..*)|(^10\..*)|(^172\.1[6-9]\..*)|(^172\.2[0-9]\..*)|(^172\.3[0-1]\..*)|(^192\.168\..*)");
            std::regex reg("^147\\.251\\..*");
=======
            std::regex reg("^25\\.43\\..*");
          //  std::regex reg("^192\\.168\\..*");
         //   return true; std::regex_match(address, reg);
>>>>>>> master
            return std::regex_match(address, reg);
        }
        
    };
    
}//networtk

#endif /* network_backend_h */
