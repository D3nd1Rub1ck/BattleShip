#include "battlegrid.h"
#include <random>
#include <iostream>

char stateSymbols[4] = {char(177), ' ', char(219), '*'};/* states:
    00 -  '?' - sea
    01 - '.' - shotten ship
    10 - '█' - ship
    11 - '*' - shotten ship
*/

int RandNum(int x, int y) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(x, y);
    return distribution(gen);
}

char Cell::GetStateSymbol(bool hideShips /*= false*/){
    if(hideShips)return stateSymbols[(((this->shotten) ? this->ship : 0) << 1) + this->shotten];// making bool ship false if they are undiscovered
    return stateSymbols[(this->ship << 1) + this->shotten];
}

void BattleGrid::writeShip(bool (&cur_field)[10][10], int x, int y, int ShipSize, bool isHorizontal){
    while(ShipSize--){
        cur_field[x][y] = 1;
        //cout << "write[" << ShipSize << "]: " << x << ' ' << y << '\t' << ((isHorizontal) ? "Horizontal" : "Vertical") << '\n';
        if(isHorizontal)x++;
        else y++;
    }
}


bool BattleGrid::isShipLocalePossible(bool (&cur_field)[10][10], int x, int y, int ShipSize, bool isHorizontal){
    while(ShipSize--){
        if((0 > x || x > 10) || (0 > y || y > 10))return false;
        if(cur_field[x][y])return false;
        for(int d = 0;d < 8;d++){
            int nx = x + dx[d];
            int ny = y + dy[d];
            if((0 > nx || nx > 9) || (0 > ny || ny > 9))continue;
            if(cur_field[nx][ny])return false;
        }
        //cout << "test[" << ShipSize << "]: " << x << ' ' << y << '\t' << ((isHorizontal) ? "Horizontal" : "Vertical") << '\n';
        if(isHorizontal)x++;
        else y++;
    }
    return true;
}

bool BattleGrid::isShip(int x, int y){
    if((0 > x || x > 9) || (0 > y || y > 9))return false;
    return grid[x][y].ship;
}
    

BattleGrid::BattleGrid(){
    bool cur_field[10][10];
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            cur_field[i][j] = 0;
    for(int ShipSize = 4; ShipSize >= 1;ShipSize--){
        for(int i = ShipSize;i < 5;i++){
            bool isHorizontal;
            int x, y;
            do{
                isHorizontal = (bool)RandNum(0, 1);
                x = RandNum(0, 9 - ((!isHorizontal) ? 0 : (ShipSize - 1)));
                y = RandNum(0, 9 - ((isHorizontal) ? 0 : (ShipSize - 1)));
            }while(!(isShipLocalePossible(cur_field, x, y, ShipSize, isHorizontal)));
            writeShip(cur_field, x, y, ShipSize, isHorizontal);
            //cout << ShipSize << '[' << i << "]\t" << x << ' ' << y << '\t' << ((isHorizontal) ? "Horizontal" : "Vertical") << "\n\n";
        }
    }
    this->ship[0] = this->ship[1] = this->ship[2] = this->ship[3] = 0;
    for(int i = 0;i < 10;i++){
        for(int j = 0;j < 10;j++){
            this->grid[i][j].ship = cur_field[i][j];
            //this->grid[i][j].shotten = RandNum(0, 1);
            this->grid[i][j].shotten = 0;
        }
    }
}

bool BattleGrid::isShotten(int x, int y){
    //if((0 > x || x > 9) || (0 > y || y > 9))printf("assdklfhashdgakjdfasdf");
    if((0 > x || x > 9) || (0 > y || y > 9))return false;
    return grid[x][y].shotten;
}

void BattleGrid::printGrid(bool hideShips /*= false*/){// hideShips displays unhitten ships as sea if we do not wanna see them
    int x = -1;
    //using std::cout;
    int n = 10;
    int length = 1;
    int width = 2;
    int m = n * width;
    n = n * length;
    std::cout << " ";
    for (int i = 0;i < 10;i++)std::cout << ' ' << i;
    std::cout << '\n';
    for(int i = -1; i < n + 1;i++){
        for(int j = -1; j < m + 1;j++){
            bool BottomWall = (i == n), CeilWall = (i == -1), LeftWall = (j == -1), RightWall = (j == m);
            char symbol = ' ';
            if(CeilWall && LeftWall)symbol = char(218);
            else if(CeilWall && RightWall)symbol = char(191);
            else if(BottomWall && LeftWall)symbol = char(192);
            else if(BottomWall && RightWall)symbol = char(217);
            else if(BottomWall || CeilWall)symbol = char(196);
            else if(LeftWall || RightWall)symbol = char(179);
            if(LeftWall)std::cout << ((0 <= x && x < 10) ? char(int('a') + x) : ' ');
            //if(!(symbol == ' '))cout << symbol;
            if(symbol != ' ')printf("\033[1;35m%c\033[0m", symbol);
            //else cout << this->grid[i / length][j / width].GetStateSymbol(hideShips);
            else if(isShip(i / length, j / width) && (!hideShips)) printf("\033[1;37m%c\033[0m", this->grid[i / length][j / width].GetStateSymbol(hideShips));
            else if(j & 2) printf("\033[1;33m%c\033[0m", this->grid[i / length][j / width].GetStateSymbol(hideShips));
            else printf("\033[1;32m%c\033[0m", this->grid[i / length][j / width].GetStateSymbol(hideShips));
            
            //else cout << (field[(i / length)][(j / width)] ? char(219) : ' '); 
            //cout << i << ' ' << j << '\t';
        }
        x++;
        std::cout << '\n';
    }
}
    
bool BattleGrid::shoot(short int x, short int y){// returns was there ship or not 
    Cell & curCell = this->grid[x][y];
    curCell.shotten = true;
    return curCell.ship;
}

int BattleGrid::isShipAlive(int x, int y, int px/* = -1*/, int py/*= -1*/){// [0 - Dead]; [1 - Alive]; [2 - not a Ship]; [3 - error]; (px and py is parent of recurcion;)
    if(!isShip(x, y))return 2;
    //printf("I on x:%c\ty:%d\n", char(int('a') + x), y);
    if(!isShotten(x, y))return 1;
    int next = false;
    for(int d = 0;d < 4;d++){
        int nx = x + dx[d];
        int ny = y + dy[d];
        if((0 > nx || nx > 9) || (0 > ny || ny > 9) || (nx == px && ny == py))continue;
        if(isShip(nx, ny))next = isShipAlive(nx, ny, x, y);
        if(next == 1)return true;
    }
    return false;
}

void BattleGrid::surroundCorpse(int x, int y, int px/* = -1*/, int py/* = -1*/){
    this->grid[x][y].shotten = true;
    for(int d = 0;d < 8;d++){
        int nx = x + dx[d];
        int ny = y + dy[d];
        if((0 > nx || nx > 9) || (0 > ny || ny > 9))continue;
        this->grid[nx][ny].shotten = true;
        if(nx == px && ny == py)continue;
        if(isShip(nx, ny))surroundCorpse(nx, ny, x, y);   
    }
}

bool BattleGrid::haveAliveShip(){
    for(int i = 0;i < 10;i++)
        for(int j = 0;j < 10;j++)
            if(this->grid[i][j].ship)
                if(!this->grid[i][j].shotten)
                    return true;
    return false;
}

int BattleGrid::registerShoot(int & x, int & y){// [0 - Miss], [1 - Hit], [2 - KILL!];
if(this->shoot(x, y)){
    if(!this->isShipAlive(x, y)){
        this->surroundCorpse(x, y);
        return 2;
    }
    else return 1;
}
return 0;
}