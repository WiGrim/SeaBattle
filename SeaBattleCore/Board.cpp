#include "pch.h"
#include "Board.h"

Board::Board(int boardSize) : size(boardSize), grid(boardSize, std::vector<CellState>(boardSize, CellState::Empty)) {}

int Board::getSize() const {
    return size;
}

CellState Board::getCell(int x, int y) const {
    return grid[y][x];
}

void Board::validateCoordinates(int x, int y) const
{
    if (x < 0 || y < 0 || x >= size || y >= size)
        throw std::out_of_range("Coordinates are outside the board");
}

void Board::placeShip(int x, int y)
{
    validateCoordinates(x, y);

    grid[x][y] = CellState::Ship;
}

void Board::shoot(int x, int y)
{
    validateCoordinates(x, y);

    if (grid[x][y] == CellState::Ship)
    {
        grid[x][y] = CellState::Hit;
    }
}