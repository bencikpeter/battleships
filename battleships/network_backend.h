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

#include "../asio-1.10.6/include/asio.hpp"

namespace network {
    
    namespace ip = asio::ip;
    
    /*class NetworkException: public std::exception {
        
        const char* message;
        
        NetworkException(const std::exception& other): message(other.what()) {}
        
        virtual const char* what() const override {
            return message;
        }
    };*/
    
    
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
        NetworkManager(std::string const & ip_address): io_service(), socket(io_service,ip::udp::endpoint(ip::udp::v4(), 1337)) {
            
            ip::udp::resolver resolver(io_service);
            ip::udp::resolver::query query(ip::udp::v4(), ip_address, "1337");
            rec_endpoint = *resolver.resolve(query);
        }
        /**
         * Constructor for side not knowing the other IP
         * inits the necessary network properties
         * bool waitForIinit() must be called to establish conection
         */
        NetworkManager(): io_service(), socket(io_service,ip::udp::endpoint(ip::udp::v4(), 0)) { }
        
        ~NetworkManager(){
            socket.close();
        }
        
        /**
         * informing other party about my adress
         * @param ip_address my local adress
         * @returns true if succesfull
         */
        bool initialize(std::string const & ip_address) {
            try {
                send_buf = toCharVector(ip_address); //define some kind of init message
                socket.send_to(asio::buffer(send_buf), rec_endpoint);
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
        bool sender(std::string message){
            try {
                send_buf = toCharVector(message);
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
        std::string listener(){
            size_t len = socket.receive_from(asio::buffer(rec_buf), send_endpoint);
            return toString(rec_buf);
        }
    private:
        bool isInitMessage(std::vector<char> message){
            /*MEETS THE IP FORMAT */
            return true;
        }
        
        std::vector<char> toCharVector(std::string const &string){
            //parse string into vector of chars
            return std::vector<char>();
        }
        
        std::string toString(std::vector<char>){
            //combine vector<char> into string
            return std::string();
        }
        
        
        
    };
}

#endif /* network_backend_h */
