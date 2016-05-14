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

bool logic::Logic::insertShip(int x1, int y1, int x2, int y2){
    bool a = true;
    std::lock_guard<std::mutex> guard1(mutexMy);
    int length,x,y;
    if (x1 == x2){
        x = x2;
        if (y1 - y2 > 0){
            y = y2;
            length = y1 - y2 + 1;
        }
        else{
            y = y1;
            length = y2 - y1 + 1;
        }
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
        y = y2;
        if (x1 - x2 > 0){
            x = x2;
            length = x1 - x2 + 1;
        }
        else{
            x = x1;
            length = x2 - x1 + 1;
        }
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

void logic::Logic::sendMyLayout(){
    std::lock_guard<std::mutex> guard(mutexMy);
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            if (myShips[i][j] == CLICKABLE || myShips[i][j] == NOT_CLICKABLE) myShips[i][j] = WATER_NOT_SHOT;
        }
    }
    guard.~lock_guard();
    auto a = encodeMyLayout();
    while (!net->sender(a));
    auto e = LogicEvent(SEND_LAYOUT);
}

void logic::Logic::shootSend(int x, int y){
    auto a = encodeCoords(x,y);
    while (!net->sender(a));
    std::lock_guard<std::mutex> guard(mutexEnemy);
    if (enemyShips[x][y] == SHIP_NOT_SHOT) {
        enemyShips[x][y] = SHIP_SHOT;
        countEnemy--;
    }
    else
        enemyShips[x][y] = WATER_SHOT;
    auto e = LogicEvent(SHOOTSEND);
}

bool logic::Logic::shootCheck(int x, int y){
    std::lock_guard<std::mutex> guard(mutexEnemy);
    std::cout << Matrix(enemyShips) << std::endl;
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

bool logic::Logic::host(){
    net = new network::NetworkManager();
    auto e = LogicEvent(HOST);
    bool tmp = net->waitForIinit();
    return tmp;
}

void logic::Logic::resetSocket()
{
    try{
        net->socketReset();
    } catch(std::exception ex) {
        std::cout << "thrown exception" << ex.what() << std::endl;
    }
}

bool logic::Logic::isIpValid(std::string ip){
    asio::error_code ec;
    asio::ip::address::from_string(ip, ec);
    if (ec){
        return false;
    }
    return true;
}

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
        if (x+i < 10 && ships[x+i][y] != WATER_NOT_SHOT)
            a[0] = false;
        //left
        if (x-i >= 0 && ships[x-i][y] != WATER_NOT_SHOT)
            a[1] = false;
        //down
        if (y+i < 10 && ships[x][y+i] != WATER_NOT_SHOT)
            a[2] = false;
        //up
        if (y-i >= 0 && ships[x][y-i] != WATER_NOT_SHOT)
            a[3] = false;
        if ((i != 4) && (*pointer != 0)){
            if (a[0] && x+i < 10) ships[x+i][y] = CLICKABLE;
            if (a[1] && x-i >= 0) ships[x-i][y] = CLICKABLE;
            if (a[2] && y+i < 10) ships[x][y+i] = CLICKABLE;
            if (a[3] && y-i >= 0) ships[x][y-i] = CLICKABLE;
        }
    }
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            if (ships[i][j] == WATER_NOT_SHOT)
            ships[i][j] = NOT_CLICKABLE;
        }
    }
    return m;
}

logic::Matrix logic::Logic::getClickableMatrix()
{
    std::lock_guard<std::mutex> guard(mutexMy);
    auto m = Matrix(myShips);
    guard.~lock_guard();
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            if (m.get()[i][j] == WATER_NOT_SHOT)
            m.get()[i][j] = CLICKABLE;
        }
    }
    return m;
}

bool logic::Logic::checkIfAllShipsPlaced()
{
    return (count2 == 0) && (count3 == 0) && (count4 == 0) && (count6 == 0);
}

void logic::Logic::resetLayout()
{
    std::lock_guard<std::mutex> guard1(mutexInsert);
    count2 = 4;
    count3 = 3;
    count4 = 2;
    count6 = 1;
    guard1.~lock_guard();
    std::lock_guard<std::mutex> guard2(mutexMy);
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            myShips[i][j] == WATER_NOT_SHOT;
        }
    }
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
