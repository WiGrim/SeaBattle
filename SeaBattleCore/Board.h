#pragma once
#include <vector>
#include "CellState.h"

class Board
{
public:
    Board(int size);

    void placeShip(int x, int y);
    void shoot(int x, int y);

    void validateCoordinates(int x, int y) const;
    CellState getCell(int x, int y) const;
    int getSize() const;
    bool isGameOver() const;

private:
    int size;
    std::vector<std::vector<CellState>> grid;
};