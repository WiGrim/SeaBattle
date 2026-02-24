#pragma once
#include <string>
#include "Board.h"
#include "Ship.h"

struct ShipPool {
    int length;
    int count;
    int placed = 0;
};

class Player {
public:
    std::string name;
    Board board;

    Player(const std::string& n, int boardSize);
    Player(const std::string& n, int boardSize, std::initializer_list<int> shipLengths);

    bool addShip(int x, int y, int length, Orientation orientation);

    bool allShipsDestroyed() const;

    bool allShipsPlaced() const;

    std::vector<Ship>& getShips() {
        return ships;
    }

    const std::vector<ShipPool>& getShipPool() const
    {
        return shipPool;
    }

private:
    std::vector<Ship> ships;

    std::vector<ShipPool> shipPool;
};