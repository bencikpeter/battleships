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
#include <vector>

namespace logic {


enum CellType { WATER_NOT_SHOT, WATER_SHOT, SHIP_NOT_SHOT, SHIP_SHOT };
enum GameEnd { WON, LOST, PLAY };
/**
 * @brief The Matrix class holds ships layout and always copies it when created so GUI can work with it whithout mutex locking
 */
class Matrix{
    CellType** matrix;
public:
    Matrix() = delete;
    Matrix(CellType** a): matrix(new CellType*[10]) {
        for (int i = 0; i < 10; i++){
            matrix[i] = new CellType[10];
            for (int j = 0; j < 10; j++){
                matrix[i][j] = a[i][j];
            }
        }
    }
    Matrix(const Matrix& o): matrix(new CellType*[10]) {
        for (int i = 0; i < 10; i++){
            matrix[i] = new CellType[10];
            for (int j = 0; j < 10; j++){
                matrix[i][j] = o.matrix[i][j];
            }
        }
    }

    CellType** get(){return matrix;}

    const Matrix& operator= (Matrix o){
        std::swap(o.matrix, matrix);
        return *this;
    }

    ~Matrix(){
        for (int i = 0; i < 10; i++){
            delete[] matrix[i];
        }
        delete[] matrix;
    }
};


class Logic {
private:
    CellType** myShips;
    CellType** enemyShips;
    int countMy;
    int countEnemy;
    network::NetworkManager* net;
    std::mutex mutexMy; // myShips, countMy
    std::mutex mutexEnemy; // enemyShips, countEnemy
    std::mutex insert; // ship counters for insert
    int count2;
    int count3;
    int count4;
    int count6;

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
    bool insertShip(int x, int y, bool horizontal, int length);

    /**
     * @brief sendMyLayout when finished layout this function is called with std::async and before that getEnemyShipLayout() is also called to get enemy layout
     */
    void sendMyLayout(){
        net->sender(encodeMyLayout());
    }

    /**
     * @brief shoot called with std::async and position of shot and before that getEnemyShot() is called
     * @param x index from left (0) to right (9)
     * @param y index from top (0) to bottom (9)
     * @return true if it was a hit false if you shot water
     */
    bool shoot(int x, int y){
        net->sender(encodeCoords(x,y));
        std::lock_guard<std::mutex> guard(mutexEnemy);
        if (enemyShips[x][y] >= SHIP_NOT_SHOT) {
            enemyShips[x][y] = SHIP_SHOT;
            countEnemy--;
            return true;
        }
        enemyShips[x][y] = WATER_SHOT;
        return false;
    }

    /**
     * @brief getEnemyShot called with std::async
     * @return pair.first is x coordinate pair.second is y
     */
    std::pair<int,int> getEnemyShot(){
        auto a = decodePair(net->listener());
        int x = a.first;
        int y = a.second;
        std::lock_guard<std::mutex> guard(mutexMy);
        if (myShips[x][y] >= SHIP_NOT_SHOT) {
            myShips[x][y] = SHIP_SHOT;
            countMy--;
        }
        else
            myShips[x][y] = WATER_SHOT;
        return a;
    }

    /**
     * @brief getEnemyShipLayout called only in beginning with std::async
     * @return grid of ships layout
     */
    Matrix getEnemyShipLayout(){
        decodeEnemyLayout(net->listener());
        std::lock_guard<std::mutex> guard(mutexEnemy);
        return Matrix(enemyShips);
    }

    /**
     * @brief getEnemyShipsWhithShots called during game whithout std::async
     * @return grid of enemy ships with positions shot at
     */
    Matrix getEnemyShipsWhithShots(){
        std::lock_guard<std::mutex> guard(mutexEnemy);
        return Matrix(enemyShips);
    }

    /**
     * @brief getMyShips called without std::async
     * @return my layout
     */
    Matrix getMyShips(){
        std::lock_guard<std::mutex> guard(mutexMy);
        return Matrix(myShips);
    }

    /**
     * @brief connect initialize connetction to ip calld std::async
     * @param ip ip adress of enemy
     * @return true if connection was succesfull
     */
    bool connect(std::string ip){
        net = new network::NetworkManager(ip);
        return net->initialize();
    }

    bool host(){
        net = new network::NetworkManager();
        return net->waitForIinit();
    }

    bool isIpValid(){
        return true;
    }

    GameEnd checkIfGameEnds(){
        std::lock_guard<std::mutex> guard1(mutexMy);
        std::lock_guard<std::mutex> guard2(mutexEnemy);
        if (countMy == 0) return LOST;
        if (countEnemy == 0) return WON;
        return PLAY;
    }


private:
    std::vector<char> encodeCoords(int x, int y){
        std::vector<char> vec;
        vec.push_back(static_cast<char>(x));
        vec.push_back(static_cast<char>(y));
        return vec;
    }

    std::vector<char> encodeMyLayout(){
        std::vector<char> vec;
        vec.resize(100);
        std::lock_guard<std::mutex> guard1(mutexMy);
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 10; j++){
                vec[i*10+j] = static_cast<char>(myShips[i][j]);
            }
        }
        return vec;
    }

    std::pair<int,int> decodePair(const std::vector<char> &vec){
        std::pair<int,int> pair;
        pair.first = static_cast<int>(vec[0]);
        pair.second = static_cast<int>(vec[1]);
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

    bool checkWaterAroundCell(int x, int y){
        bool a = myShips[x][y] == WATER_NOT_SHOT;
        if (x + 1 < 10) a = a && myShips[x+1][y] == WATER_NOT_SHOT;
        if (x - 1 >= 0) a = a && myShips[x-1][y] == WATER_NOT_SHOT;
        if (y + 1 < 10) a = a && myShips[x][y+1] == WATER_NOT_SHOT;
        if (y - 1 >= 0) a = a && myShips[x][y-1] == WATER_NOT_SHOT;
        if (x + 1 < 10 && y + 1 < 10) a = a && myShips[x+1][y+1] == WATER_NOT_SHOT;
        if (x + 1 < 10 && y - 1 >= 0) a = a && myShips[x+1][y-1] == WATER_NOT_SHOT;
        if (x - 1 >= 0 && y + 1 < 10) a = a && myShips[x-1][y+1] == WATER_NOT_SHOT;
        if (x - 1 >= 0 && y - 1 >= 0) a = a && myShips[x-1][y-1] == WATER_NOT_SHOT;
        return a;
    }
};

Logic::~Logic(){
    std::lock_guard<std::mutex> guard1(mutexMy);
    std::lock_guard<std::mutex> guard2(mutexEnemy);
    for (int i = 0; i < 10; i++){
        delete[] myShips[i];
        delete[] enemyShips[i];
    }
    delete[] myShips;
    delete[] enemyShips;
    delete net;
}

Logic::Logic(): myShips(new CellType*[10]), enemyShips(new CellType*[10]), countMy(31), countEnemy(31), net(nullptr){
    std::lock_guard<std::mutex> guard1(mutexMy);
    std::lock_guard<std::mutex> guard2(mutexEnemy);
    for (int i = 0; i < 10; i++){
        myShips[i] = new CellType[10];
        enemyShips[i] = new CellType[10];
        for (int j = 0; j < 10; j++){
            myShips[i][j] = WATER_NOT_SHOT;
            enemyShips[i][j] = WATER_NOT_SHOT;
        }
    }
}

bool Logic::insertShip(int x, int y, bool horizontal, int length){//doplnit pocet
    bool a = true;
    std::lock_guard<std::mutex> guard(mutexMy);
    if (horizontal){
        if (x + length < 10){
            for (int i = x; i < x+length; i++)
                a = a && checkWaterAroundCell(i,y);
        }
        else a = false;
        if (a)
            for (int i = x; i < x+length; i++)
                myShips[i][y] = SHIP_NOT_SHOT;

    }
    else{
        if (y + length < 10){
            for (int i = y; i < y+length; i++)
                a = a && checkWaterAroundCell(x,i);
        }
        else a = false;
        if (a)
            for (int i = y; i < y+length; i++)
                myShips[x][i] = SHIP_NOT_SHOT;
    }
    return a;
}
}
#endif /* business_logic_backend_h */
