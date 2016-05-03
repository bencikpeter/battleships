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
#include <future>

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
    void sendMyLayout();

    /**
     * @brief shoot called with std::async and position of shot and before that getEnemyShot() is called
     * @param x index from left (0) to right (9)
     * @param y index from top (0) to bottom (9)
     * @return true if it was a hit false if you shot water
     */
    bool shoot(int x, int y);

    /**
     * @brief getEnemyShot called with std::async
     * @return pair.first is x coordinate pair.second is y
     */
    std::pair<int,int> getEnemyShot();

    /**
     * @brief getEnemyShipLayout called only in beginning with std::async
     * @return grid of ships layout
     */
    Matrix getEnemyShipLayout();

    /**
     * @brief getEnemyShipsWhithShots called during game whithout std::async
     * @return grid of enemy ships with positions shot at
     */
    Matrix getEnemyShipsWhithShots();

    /**
     * @brief getMyShips called without std::async
     * @return my layout
     */
    Matrix getMyShips();

    /**
     * @brief connect initialize connetction to ip calld std::async
     * @param ip ip adress of enemy
     * @return true if connection was succesfull
     */
    bool connect(std::string ip);

    bool host(std::atomic<bool> &a);

    bool isIpValid(std::string ip);

    GameEnd checkIfGameEnds();


private:
    std::vector<char> encodeCoords(int x, int y);

    std::vector<char> encodeMyLayout();

    std::pair<int,int> decodePair(const std::vector<char> &vec);

    void decodeEnemyLayout(const std::vector<char> &vec);

    bool checkWaterAroundCell(int x, int y);
};

}
#endif /* business_logic_backend_h */
