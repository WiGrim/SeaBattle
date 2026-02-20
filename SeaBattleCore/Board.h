#pragma once
#include <vector>
#include "CellState.h"
#include <iostream>
#include <ostream>

class Board
{
public:
    Board(int size);

    void placeShip(int x, int y);
    bool shoot(int x, int y);
    void print(std::ostream& out, bool showShips) const;

    void validateCoordinates(int x, int y) const;
    CellState getCell(int x, int y) const;
    int getSize() const;
    bool isGameOver() const;

private:
    int size;
    std::vector<std::vector<CellState>> grid;
};