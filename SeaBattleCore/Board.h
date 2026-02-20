#pragma once
#include <vector>
#include "CellState.h"

class Board
{
public:
    Board(int size);

    void placeShip(int x, int y);

    CellState getCell(int x, int y) const;
    int getSize() const;

private:
    int size;
    std::vector<std::vector<CellState>> grid;
};