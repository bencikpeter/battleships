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

logic::Logic::Logic(logic::Logic && other) : myShips(other.myShips), enemyShips(other.enemyShips), countMy(other.countMy), countEnemy(other.countEnemy), net(other.net),
count2(other.count2), count3(other.count3), count4(other.count4), count6(other.count6) {
    other.myShips = nullptr;
    other.enemyShips = nullptr;
}

logic::Logic &logic::Logic::operator=(logic::Logic && other)
{
    using std::swap;
    swap(myShips,other.myShips);
    swap(enemyShips,other.enemyShips);
    swap(countMy,other.countMy);
    swap(countEnemy,other.countEnemy);
    swap(net,other.net);
    swap(count2,other.count2);
    swap(count3,other.count3);
    swap(count4,other.count4);
    swap(count6,other.count6);
    return *this;
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

void logic::Logic::resetLayout()
{
    {
        std::lock_guard<std::mutex> guard1(mutexInsert);
        count2 = 4;
        count3 = 3;
        count4 = 2;
        count6 = 1;
    }
    std::lock_guard<std::mutex> guard2(mutexMy);
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            myShips[i][j] = WATER_NOT_SHOT;
        }
    }
}


std::vector<char> logic::Logic::encodeMyLayout(){
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
