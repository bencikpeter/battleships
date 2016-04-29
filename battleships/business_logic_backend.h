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

/**
 * @brief The Matrix class holds ships layout and always copies it when created so GUI can work with it whithout mutex locking
 */
class Matrix{
    char** matrix;
public:
    Matrix() = delete;
    Matrix(char** a): matrix(new char*[10]) {
        for (int i = 0; i < 10; i++){
            matrix[i] = new char[10];
            for (int j = 0; j < 10; j++){
                matrix[i][j] = a[i][j];
            }
        }
    }
    Matrix(const Matrix& o): matrix(new char*[10]) {
        for (int i = 0; i < 10; i++){
            matrix[i] = new char[10];
            for (int j = 0; j < 10; j++){
                matrix[i][j] = o.matrix[i][j];
            }
        }
    }

    char** get(){return matrix;}

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
    char** myShips;
    char** enemyShips;
    network::NetworkManager* net;
    std::mutex mutexMy;
    std::mutex mutexEnemy;

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
    bool insertShip(char x, char y, bool horizontal, char length);//is header ok?

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
    bool shoot(char x, char y){//should i save where was it shot
        net->sender(encodeCoords(x,y));
    }

    /**
     * @brief getEnemyShot called with std::async
     * @return pair.first is x coordinate pair.second is y
     */
    std::pair<char,char> getEnemyShot(){//same question
        auto a = decodePair(net->listener());
        return a;
    }

    /**
     * @brief getEnemyShipLayout called only in beginning with std::async
     * @return grid of ships layout
     */
    Matrix getEnemyShipLayout(){
        decodeEnemyLayout(net->listener());
        std::lock_guard<std::mutex> guard1(mutexEnemy);
        return Matrix(enemyShips);
    }

    /**
     * @brief getMyShips called without std::async
     * @return my layout
     */
    Matrix getMyShips(){
        std::lock_guard<std::mutex> guard1(mutexMy);
        return Matrix(myShips);
    }

    /**
     * @brief connect initialize connetction to ip calld std::async
     * @param ip ip adress of enemy
     * @return true if connection was succesfull
     */
    bool connect(std::string ip){
        net = new network::NetworkManager(ip);
        net->initialize();
    }


private:
    std::vector<char> encodeCoords(char x, char y){
        std::vector<char> vec;
        vec.push_back(x);
        vec.push_back(y);
        return vec;
    }

    std::vector<char> encodeMyLayout(){
        std::vector<char> vec;
        vec.resize(100);
        std::lock_guard<std::mutex> guard1(mutexMy);
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 10; j++){
                vec[i*10+j] = myShips[i][j];
            }
        }
        return vec;
    }

    std::pair<char,char> decodePair(const std::vector<char> &vec){
        std::pair<char,char> pair;
        pair.first = vec[0];
        pair.second = vec[1];
        return pair;
    }

    void decodeEnemyLayout(const std::vector<char> &vec){
        std::lock_guard<std::mutex> guard1(mutexEnemy);
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 10; j++){
                enemyShips[i][j] = vec[i*10+j];
            }
        }
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

Logic::Logic(): myShips(new char*[10]), enemyShips(new char*[10]){
    std::lock_guard<std::mutex> guard1(mutexMy);
    std::lock_guard<std::mutex> guard2(mutexEnemy);
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
