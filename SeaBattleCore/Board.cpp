#include "pch.h"
#include "Board.h"

Board::Board(int boardSize) : size(boardSize), grid(boardSize, std::vector<CellState>(boardSize, CellState::Empty)) {}

int Board::getSize() const {
    return size;
}

CellState Board::getCell(int x, int y) const {
    return grid[y][x];
}

void Board::placeShip(int x, int y)
{
    grid[x][y] = CellState::Ship;
}