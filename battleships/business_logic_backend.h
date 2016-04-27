//
//  business_logic_backend.h
//  battleships
//
//  Created by Peter Bencik on 27.04.16.
//  Copyright © 2016 Peter Bencik. All rights reserved.
//

#ifndef business_logic_backend_h
#define business_logic_backend_h
#include "network_backend.h"
#include <utility>
#include <string>
#include <mutex>

namespace logic {
    class Logic {
    private:
        int myShips[10][10];
        int enemyShips[10][10];
        network::NetworkManager net;
        std::mutex mutexMy;
        std::mutex mutexEnemy;
        std::mutex mutexNet;

        /**
         * @brief chceckShipsLayout checks white spaces around ships
         * @return true if layout is OK
         */
        bool chceckShipsLayout();
    public:

        /**
         * @brief insertShip
         * @param x index from top (0) to bottom (9)
         * @param y index from left (0) to right (9)
         * @param horizontal true if ship is in horizontal orientation false for vertical
         * @param length length of ship
         * @return true if ship was legaly placed
         */
        bool insertShip(int x, int y, bool horizontal, int length);

        /**
         * @brief sendMyLayout when finished layout this function is called with std::async and before that getEnemyShipLayout() is also called to get enemy layout
         */
        void sendMyLayout();

        /**
         * @brief shoot called with std::async and position of shot and before that getEnemyShot() is called
         * @param x index from top (0) to bottom (9)
         * @param y index from left (0) to right (9)
         * @return true if it was a hit false if you shot water
         */
        bool shoot(int x, int y);

        /**
         * @brief getEnemyShot called with std::async
         * @return pair.first is x coordinate pair.second is y
         */
        std::pair<int,int> getEnemyShot();

        /**
         * @brief getEnemyShipLayout called only in beginning
         * @return grid of ships layout
         */
        int** getEnemyShipLayout();

        /**
         * @brief getMyShips called without threads
         * @return my layout
         */
        int** getMyShips();

        /**
         * @brief connect initialize connetction
         * @param ip ip adress of enemy
         * @return true if connection was succesfull
         */
        bool connect(std::string ip);
    };
}

#endif /* business_logic_backend_h */
