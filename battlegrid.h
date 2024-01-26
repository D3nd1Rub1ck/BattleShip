#pragma once 

const int dx[] = {1, -1, 0, 0, 1, 1, -1, -1};
const int dy[] = {0, 0, 1, -1, 1, -1, 1, -1};

int RandNum(int x, int y);

class Cell{ // is it ship, is it shotten and GetState().
public:    
    bool ship;
    bool shotten;

    char GetStateSymbol(bool hideShips = false);
};

class BattleGrid{ // grid it self and ALL methods.
private:
    Cell grid[10][10]; // the grid it self (table)
    short int ship[4];
 
    void writeShip(bool (&cur_field)[10][10], int x, int y, int ShipSize, bool isHorizontal);

    bool isShipLocalePossible(bool (&cur_field)[10][10], int x, int y, int ShipSize, bool isHorizontal);

    bool isShip(int x, int y);

public:
    BattleGrid();

    bool isShotten(int x, int y);

    void printGrid(bool hideShips = false);// hideShips displays unhitten ships as sea if we do not wanna see them

    bool shoot(short int x, short int y);// returns was there ship or not 

    int isShipAlive(int x, int y, int px = -1, int py = -1);// [0 - Dead]; [1 - Alive]; [2 - not a Ship]; [3 - error]; (px and py is parent of recurcion;)

    void surroundCorpse(int x, int y, int px = -1, int py = -1);

    bool haveAliveShip();

    int registerShoot(int & x, int & y);// [0 - Miss], [1 - Hit], [2 - KILL!];
};