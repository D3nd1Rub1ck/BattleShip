#include <bits/stdc++.h>
#include <thread>
#include <chrono>
#include "battlegrid.h"

int reformat_input(char c){
    //int n = int(c);
    if(c >= '0' && c <= '9')return int(c - 48);
    if(c >= 'a' && c <= 'j')return int(c -int('a'));
    //std::cout << "Invalid format!" << c << '\n';
    return -1;
}

void getInput(BattleGrid & battlefield, int & x, int & y, bool random = false){
    char input_x, input_y;
    do{
        if(random){
            x = RandNum(0, 9);
            y = RandNum(0, 9);
        }
        else{
            std::cout << "Your turn enter cell in format: \"a0\", \"e3\" or \"j9\"\n";
            std::cin >> input_x >> input_y;
            x = reformat_input(input_x);
            y = reformat_input(input_y);
        }
    }while(((x == -1) || (y == -1)) || battlefield.isShotten(x, y));
}

int main(){
    BattleGrid a, b;

    bool turn = true;//[true a];[false b];
    while( a.haveAliveShip() && b.haveAliveShip() ){
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        if(turn){
            system("cls");
            a.printGrid(turn);
            b.printGrid(!turn);
        }
        int x, y;
        getInput( ((turn) ? a : b) , x, y, !turn);
        //getInput( ((turn) ? a : b) , x, y, true);
        int shootResult = ((turn) ? a.registerShoot(x, y) : b.registerShoot(x, y));
        if(turn){
            system("cls");
            a.printGrid(turn);
            b.printGrid(!turn);
            std::cout << ((turn) ? "You" : "Bot") << " shot on [" << char(int('a') + x) << y << "] and got: ";
            if (shootResult == 2)std::cout << "KILL!\n";
            else if (shootResult == 1)std::cout << "Hit!\n";
            else std::cout << "Miss.\n";
        }
        if(shootResult == 0)turn = !turn;
    }
    
    system("cls");
    std::cout << "Congrants! The Winner is:" << ((b.haveAliveShip()) ? "You" : "Bot") << '\n';
    a.printGrid();
    b.printGrid();
    system("pause");
    
    return 0;
}