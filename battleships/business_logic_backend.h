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
#include "business_logic_matrix.h"
#include <utility>
#include <string>
#include <mutex>
#include <vector>
#include <future>
#include "logicevent.h"

namespace logic {
//enum CellType implemented in bussunes_logic_matrix.h
//enum CellType { SHIP_NOT_SHOT, SHIP_SHOT, WATER_NOT_SHOT, WATER_SHOT, CLICKABLE, NOT_CLICKABLE };
enum GameEnd { WON, LOST, PLAY };

class Logic {
private:
    CellType** myShips;
    CellType** enemyShips;
    int countMy;
    int countEnemy;
    network::NetworkManager* net;
    std::mutex mutexMy; // myShips, countMy
    std::mutex mutexEnemy; // enemyShips, countEnemy
    std::mutex mutexInsert; // ship counters for insert
    int count2;
    int count3;
    int count4;
    int count6;

public:
    Logic();
    ~Logic();
    Logic(Logic &&);
    Logic& operator=(Logic &&);

    /**
     * @brief insertShip
     * @param x1 index from left (0) to right (9)
     * @param y1 index from top (0) to bottom (9)
     * @param x2 other end of ship
     * @param y2 other end of ship
     * @return true if ship was legaly placed
     */
    bool insertShip(int x1, int y1, int x2, int y2);

    /**
     * @brief sendMyLayout when finished layout this function is called with std::async and before that getEnemyShipLayout() is also called to get enemy layout
     */
    void sendMyLayout();//async

    /**
     * @brief shoot called with std::async and position of shot and before that getEnemyShot() is called
     * @param x index from left (0) to right (9)
     * @param y index from top (0) to bottom (9)
     */
    void shootSend(int x, int y);//async

    /**
     * @brief checks if shot was to water or ship
     * @param x index from left (0) to right (9)
     * @param y index from top (0) to bottom (9)
     * @return true if it was a hit false if you shot water
     */
    bool shootCheck(int x, int y);

    /**
     * @brief getEnemyShot called with std::async
     * @return pair.first is x coordinate pair.second is y
     */
    std::pair<int,int> getEnemyShot();//async

    /**
     * @brief getEnemyShipLayout called only in beginning
     * @return grid of ships layout
     */
    Matrix getEnemyShipLayout(){//async
        decodeEnemyLayout(net->listener());
        std::lock_guard<std::mutex> guard(mutexEnemy);
        auto e = LogicEvent(GET_LAYOUT);
        return Matrix(enemyShips);
    }

    /**
     * @brief getEnemyShipsWhithShots called during game
     * @return grid of enemy ships with positions shot at
     */
    Matrix getEnemyShipsWhithShots(){
        std::lock_guard<std::mutex> guard(mutexEnemy);
        return Matrix(enemyShips);
    }

    /**
     * @brief getMyShips
     * @return my layout
     */
    Matrix getMyShips(){
        std::lock_guard<std::mutex> guard(mutexMy);
        return Matrix(myShips);
    }

    /**
     * @brief connect initialize connetction to ip
     * @param ip ip adress of enemy
     * @return true if connection was succesfull
     */
    bool connect(std::string ip){//async
        net = new network::NetworkManager(ip);
        auto e = LogicEvent(CONNECT);
        return net->initialize();
    }

    /**
     * @brief host
     * @return true if connection succesful
     */
    bool host(){//async
        net = new network::NetworkManager();
        auto e = LogicEvent(HOST);
        bool tmp = net->waitForIinit();
        return tmp;
    }

    /**
     * @brief resetSocket resets socket when host function needs to be cancelled
     */
    void resetSocket()
    {
        try{
            net->socketReset();
        } catch(std::exception ex) {
            std::cout << "thrown exception" << ex.what() << std::endl;
        }
    }

    /**
     * @brief isIpValid
     * @return true if ip parametr is valid IPv4 adress
     */
    bool isIpValid(std::string ip){
        asio::error_code ec;
        asio::ip::address::from_string(ip, ec);
        if (ec){
            return false;
        }
        return true;
    }

    /**
     * @brief checkIfGameEnds
     * @return what state is game in (who won/lost or game should continue)
     */
    GameEnd checkIfGameEnds(){
        std::lock_guard<std::mutex> guard1(mutexMy);
        std::lock_guard<std::mutex> guard2(mutexEnemy);
        if (countMy == 0) return LOST;
        if (countEnemy == 0) return WON;
        return PLAY;
    }

    /**
     * @brief getClickableMatrix when user clicks on water creates array (does not modify myShips) where he can click next (other end of ship)
     * @return Matrix copy
     */
    Matrix getClickableMatrix(int x, int y);

    /**
     * @brief getClickableMatrix
     * @return layout of all places that ship can be placed
     */
    Matrix getClickableMatrix();

    /**
     * @brief checkIfAllShipsPlaced
     * @return true if all ships were placed
     */
    bool checkIfAllShipsPlaced()
    {
        return (count2 == 0) && (count3 == 0) && (count4 == 0) && (count6 == 0);
    }

    /**
     * @brief resetLayout clears all placed ships from layout, counters reset
     */
    void resetLayout();


private:
    std::vector<char> encodeCoords(int x, int y){
        std::vector<char> vec;
        vec.push_back(x);
        vec.push_back(y);
        return vec;
    }

    std::vector<char> encodeMyLayout();

    std::pair<int, int> decodePair(const std::vector<char> &vec){
        std::pair<int,int> pair;
        pair.first = vec[0];
        pair.second = vec[1];
        return pair;
    }

    void decodeEnemyLayout(const std::vector<char> &vec){
        std::lock_guard<std::mutex> guard(mutexEnemy);
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 10; j++){
                enemyShips[i][j] = static_cast<CellType>(vec[i*10+j]);
            }
        }
    }

    bool checkWaterAroundCell(int x, int y);
};

}
#endif /* business_logic_backend_h */
