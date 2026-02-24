#pragma once
#include <string>
#include "Board.h"
#include "Ship.h"

class Player {
public:
    std::string name;
    Board board;

    Player(const std::string& n, int boardSize);

    bool addShip(int x, int y, int length, Orientation orientation);

    bool allShipsDestroyed() const;

    std::vector<Ship>& getShips() {
        return ships;
    }

private:
    std::vector<Ship> ships;
    int maxShips = 1; //test
};