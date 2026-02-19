#pragma once
#include <vector>
#include "CellState.h"

class Board
{
public:
    Board(int size);

    CellState getCell(int x, int y) const;
    int getSize() const;

private:
    int size;
    std::vector<std::vector<CellState>> grid;
};