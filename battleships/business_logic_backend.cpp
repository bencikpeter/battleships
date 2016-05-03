#include "business_logic_backend.h"


logic::Logic::~Logic(){
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

logic::Logic::Logic(): myShips(new CellType*[10]), enemyShips(new CellType*[10]), countMy(31), countEnemy(31), net(nullptr){
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

bool logic::Logic::insertShip(int x, int y, bool horizontal, int length){//doplnit pocet
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

/*
void logic::Logic::sendMyLayout(){
    net->sender(encodeMyLayout());
}

bool logic::Logic::shoot(int x, int y){
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

std::pair<int, int> logic::Logic::getEnemyShot(){
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

logic::Matrix logic::Logic::getEnemyShipLayout(){
    decodeEnemyLayout(net->listener());
    std::lock_guard<std::mutex> guard(mutexEnemy);
    return Matrix(enemyShips);
}

logic::Matrix logic::Logic::getEnemyShipsWhithShots(){
    std::lock_guard<std::mutex> guard(mutexEnemy);
    return Matrix(enemyShips);
}

logic::Matrix logic::Logic::getMyShips(){
    std::lock_guard<std::mutex> guard(mutexMy);
    return Matrix(myShips);
}

bool logic::Logic::connect(std::__cxx11::string ip){
    net = new network::NetworkManager(ip);
    return net->initialize();
}

bool logic::Logic::host(std::atomic<bool> &a){
    net = new network::NetworkManager();
    return net->waitForIinit(a);
}

bool logic::Logic::isIpValid(std::__cxx11::string ip){
    return true;
}*/

logic::GameEnd logic::Logic::checkIfGameEnds(){
    std::lock_guard<std::mutex> guard1(mutexMy);
    std::lock_guard<std::mutex> guard2(mutexEnemy);
    if (countMy == 0) return LOST;
    if (countEnemy == 0) return WON;
    return PLAY;
}

std::vector<char> logic::Logic::encodeCoords(int x, int y){
    std::vector<char> vec;
    vec.push_back(static_cast<char>(x));
    vec.push_back(static_cast<char>(y));
    return vec;
}

std::vector<char> logic::Logic::encodeMyLayout(){
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

std::pair<int, int> logic::Logic::decodePair(const std::vector<char> &vec){
    std::pair<int,int> pair;
    pair.first = static_cast<int>(vec[0]);
    pair.second = static_cast<int>(vec[1]);
    return pair;
}

void logic::Logic::decodeEnemyLayout(const std::vector<char> &vec){
    std::lock_guard<std::mutex> guard(mutexEnemy);
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            enemyShips[i][j] = static_cast<CellType>(vec[i*10+j]);
        }
    }
}

bool logic::Logic::checkWaterAroundCell(int x, int y){
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
