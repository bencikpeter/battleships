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

logic::Logic::Logic(): myShips(new CellType*[10]), enemyShips(new CellType*[10]), countMy(31), countEnemy(31), net(nullptr),
                       count2(4), count3(3), count4(2), count6(1){
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

bool logic::Logic::insertShip(int x, int y, bool horizontal, int length){
    bool a = true;
    std::lock_guard<std::mutex> guard1(mutexMy);
    if (horizontal){
        if (y + length-1 < 10){
            for (int i = y; i < y+length; i++)
                a = a && checkWaterAroundCell(x,i);
        }
        else a = false;
        if (a){
            for (int i = y; i < y+length; i++)
                myShips[x][i] = SHIP_NOT_SHOT;

            if (x-1 >= 0)
                for (int i = y; i < y+length; i++ )
                    myShips[x-1][i] = NOT_CLICKABLE;
            if (x+1 < 10)
                for (int i = y; i < y+length; i++ )
                    myShips[x+1][i] = NOT_CLICKABLE;
            if (y-1 >= 0){
                if (x-1 >= 0) myShips[x-1][y-1] = NOT_CLICKABLE;
                if (x+1 < 10) myShips[x+1][y-1] = NOT_CLICKABLE;
                myShips[x][y-1] = NOT_CLICKABLE;
            }
            if (y+length < 10){
                if (x-1 >= 0) myShips[x-1][y+length] = NOT_CLICKABLE;
                if (x+1 < 10) myShips[x+1][y+length] = NOT_CLICKABLE;
                myShips[x][y+length] = NOT_CLICKABLE;
            }
        }
    }
    else{
        if (x + length-1 < 10){
            for (int i = x; i < x+length; i++)
                a = a && checkWaterAroundCell(i,y);
        }
        else a = false;
        if (a){
            for (int i = x; i < x+length; i++)
                myShips[i][y] = SHIP_NOT_SHOT;

            if (y-1 >= 0)
                for (int i = x; i < x+length; i++ )
                    myShips[i][y-1] = NOT_CLICKABLE;
            if (y+1 < 10)
                for (int i = x; i < x+length; i++ )
                    myShips[i][y+1] = NOT_CLICKABLE;
            if (x-1 >= 0){
                if (y-1 >= 0) myShips[x-1][y-1] = NOT_CLICKABLE;
                if (y+1 < 10) myShips[x-1][y+1] = NOT_CLICKABLE;
                myShips[x-1][y] = NOT_CLICKABLE;
            }
            if (x+length < 10){
                if (y-1 >= 0) myShips[x+length][y-1] = NOT_CLICKABLE;
                if (y+1 < 10) myShips[x+length][y+1] = NOT_CLICKABLE;
                myShips[x+length][y] = NOT_CLICKABLE;
            }
        }
    }
    if (a) {
        std::lock_guard<std::mutex> guard2(mutexInsert);
        switch(length){
        case 6: count6--; break;
        case 4: count4--; break;
        case 3: count3--; break;
        case 2: count2--; break;
        }
    }
    return a;
}
/*
void logic::Logic::sendMyLayout(){
    std::lock_guard<std::mutex> guard(mutexMy);
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            if (myShips[i][j] == CLICKABLE || myShips[i][j] == NOT_CLICKABLE) myShips[i][j] = WATER_NOT_SHOT;
        }
    }
    net->sender(encodeMyLayout());
}

void logic::Logic::shootSend(int x, int y){
    net->sender(encodeCoords(x,y));
    std::lock_guard<std::mutex> guard(mutexEnemy);
    if (enemyShips[x][y] == SHIP_NOT_SHOT) {
        enemyShips[x][y] = SHIP_SHOT;
        countEnemy--;
    }
    else
        enemyShips[x][y] = WATER_SHOT;
}

bool logic::Logic::shootCheck(int x, int y){
    std::lock_guard<std::mutex> guard(mutexEnemy);
    if (enemyShips[x][y] == SHIP_NOT_SHOT) {
        return true;
    }
    return false;
}

std::pair<int, int> logic::Logic::getEnemyShot(){
    auto a = decodePair(net->listener());
    int x = a.first;
    int y = a.second;
    std::lock_guard<std::mutex> guard(mutexMy);
    if (myShips[x][y] == SHIP_NOT_SHOT) {
        myShips[x][y] = SHIP_SHOT;
        countMy--;
    }
    else
        myShips[x][y] = WATER_SHOT;
    auto e = LogicEvent(ENEMY_SHOT);
    return a;
}

logic::Matrix logic::Logic::getEnemyShipLayout(){
    decodeEnemyLayout(net->listener());
    std::lock_guard<std::mutex> guard(mutexEnemy);
    auto e = LogicEvent(GET_LAYOUT);
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

bool logic::Logic::connect(std::string ip){
    net = new network::NetworkManager(ip);
    auto e = LogicEvent(CONNECT);
    return net->initialize();
}

bool logic::Logic::host(std::atomic<bool> &a){
    net = new network::NetworkManager();
    auto e = LogicEvent(HOST);
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

logic::Matrix logic::Logic::getClickableMatrix(int x, int y){
    std::lock_guard<std::mutex> guard(mutexMy);
    auto m = Matrix(myShips);
    guard.~lock_guard();
    auto ships = m.get();
    bool a[4];
    for (int i = 0; i < 4; i++)
        a[i] = true;
    int* pointer;
    std::lock_guard<std::mutex> guard2(mutexInsert);
    for (int i = 1; i < 6; i++){
        switch(i){
                case 5: pointer = &count6; break;
                case 3: pointer = &count4; break;
                case 2: pointer = &count3; break;
                case 1: pointer = &count2; break;
                }
        //right
        if (ships[x+i][y] != WATER_NOT_SHOT)
            a[0] = false;
        //left
        if (ships[x-i][y] != WATER_NOT_SHOT)
            a[1] = false;
        //down
        if (ships[x][y+i] != WATER_NOT_SHOT)
            a[2] = false;
        //up
        if (ships[x][y-i] != WATER_NOT_SHOT)
            a[3] = false;
        if ((i != 4) && (*pointer != 0)){
            if (a[0]) ships[x+i][y] = CLICKABLE;
            if (a[1]) ships[x-i][y] = CLICKABLE;
            if (a[2]) ships[x][y+i] = CLICKABLE;
            if (a[3]) ships[x][y-i] = CLICKABLE;
        }
    }
    return m;

}

bool logic::Logic::checkIfAllShipsPlaced()
{
    return (count2 == 0) && (count3 == 0) && (count4 == 0) && (count6 == 0);
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
    bool a = myShips[x][y] >= WATER_NOT_SHOT;
    if (x + 1 < 10) a = a && myShips[x+1][y] >= WATER_NOT_SHOT;
    if (x - 1 >= 0) a = a && myShips[x-1][y] >= WATER_NOT_SHOT;
    if (y + 1 < 10) a = a && myShips[x][y+1] >= WATER_NOT_SHOT;
    if (y - 1 >= 0) a = a && myShips[x][y-1] >= WATER_NOT_SHOT;
    if (x + 1 < 10 && y + 1 < 10) a = a && myShips[x+1][y+1] >= WATER_NOT_SHOT;
    if (x + 1 < 10 && y - 1 >= 0) a = a && myShips[x+1][y-1] >= WATER_NOT_SHOT;
    if (x - 1 >= 0 && y + 1 < 10) a = a && myShips[x-1][y+1] >= WATER_NOT_SHOT;
    if (x - 1 >= 0 && y - 1 >= 0) a = a && myShips[x-1][y-1] >= WATER_NOT_SHOT;
    return a;
}
