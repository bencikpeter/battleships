//
//  business_logic_backend.h
//  battleships
//
//  Created by Peter Bencik on 27.04.16.
//  Copyright © 2016 Peter Bencik. All rights reserved.
//

#ifndef business_logic_backend_h
#define business_logic_backend_h
//#include "network_backend.h"
#include <utility>
#include <string>
#include <mutex>
#include <vector>

namespace logic {
    class Logic {
    private:
        char** myShips;
        char** enemyShips;
        network::NetworkManager* net;
        std::mutex mutexMy;
        std::mutex mutexEnemy;
        std::mutex mutexNet;

    public:
        Logic();
        ~Logic();

        /**
         * @brief insertShip
         * @param x index from left (0) to right (9)
         * @param y index from top (0) to bottom (9)
         * @param horizontal true if ship is in horizontal orientation false for vertical
         * @param length length of ship
         * @return true if ship was legaly placed
         */
        bool insertShip(char x, char y, bool horizontal, char length);

        /**
         * @brief sendMyLayout when finished layout this function is called with std::async and before that getEnemyShipLayout() is also called to get enemy layout
         */
        void sendMyLayout();

        /**
         * @brief shoot called with std::async and position of shot and before that getEnemyShot() is called
         * @param x index from left (0) to right (9)
         * @param y index from top (0) to bottom (9)
         * @return true if it was a hit false if you shot water
         */
        bool shoot(char x, char y);

        /**
         * @brief getEnemyShot called with std::async
         * @return pair.first is x coordinate pair.second is y
         */
        std::pair<char,char> getEnemyShot();

        /**
         * @brief getEnemyShipLayout called only in beginning with std::async
         * @return grid of ships layout
         */
        const char** getEnemyShipLayout();

        /**
         * @brief getMyShips called without std::async
         * @return my layout
         */
        const char** getMyShips(){
            return myShips;
        }

        /**
         * @brief connect initialize connetction to ip calld std::async
         * @param ip ip adress of enemy
         * @return true if connection was succesfull
         */
        bool connect(std::string ip);


    private:
        std::vector<char> encodePair(std::pair<char,char> &pair){
            std::vector<char> vec;
            vec.push_back(pair.first);
            vec.push_back(pair.second);
            return vec;
        }

        std::vector<char> encodeMyLayout(){
            std::vector<char> vec;
            vec.resize(100);
            for(int i = 0; i < 10; i++){
                for(int j = 0; j < 10; j++){
                    vec[i*10+j] = myShips[i][j];
                }
            }
            return vec;
        }

        std::pair<char,char> decodePair(std::vector<char> &vec){
            std::pair<char,char> pair;
            pair.first = vec[0];
            pair.second = vec[1];
            return pair;
        }

        void decodeEnemyLayout(std::vector<char> &vec){
            for(int i = 0; i < 10; i++){
                for(int j = 0; j < 10; j++){
                    enemyShips[i][j] = vec[i*10+j];
                }
            }
        }
    };

    Logic::~Logic(){
        for (int i = 0; i < 10; i++){
            delete[] myShips[i];
            delete[] enemyShips[i];
        }
        delete[] myShips;
        delete[] enemyShips;
    }

    Logic::Logic(): myShips(new char*[10]), enemyShips(new char*[10]){
        for (int i = 0; i < 10; i++){
            myShips[i] = new char[10];
            enemyShips[i] = new char[10];
            for (int j = 0; j < 10; j++){
                myShips[i][j] = 0;
                enemyShips[i][j] = 0;
            }
        }
    }
}
#endif /* business_logic_backend_h */
