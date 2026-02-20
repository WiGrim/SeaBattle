#include "pch.h"
#include "Board.h"
#include "ShipPlaceError.h"

Board::Board(int boardSize) : size(boardSize), grid(boardSize, std::vector<CellState>(boardSize, CellState::Empty)) {}

int Board::getSize() const {
    return size;
}

void Board::validateCoordinates(int x, int y) const
{
    if (x < 0 || y < 0 || x >= size || y >= size)
        throw std::out_of_range("Coordinates are outside the board");
}

CellState Board::getCell(int x, int y) const {
    validateCoordinates(x, y);
    return grid[y][x];
}

void Board::placeShip(int x, int y)
{
    validateCoordinates(x, y);

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && ny >= 0 && nx < size && ny < size) {
                if (grid[ny][nx] == CellState::Ship) {
                    throw ShipPlaceError("Cannot place ship near another ship");
                }
            }
        }
    }

    grid[y][x] = CellState::Ship;
}

void Board::shoot(int x, int y)
{
    validateCoordinates(x, y);

    if (grid[y][x] == CellState::Ship || grid[y][x] == CellState::Hit)
    {
        grid[y][x] = CellState::Hit;
    }
    else
    {
        grid[y][x] = CellState::Miss;
    }
}

bool Board::isGameOver() const {
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (grid[y][x] == CellState::Ship) {
                return false;
            }
        }
    }
    return true;
}

void Board::print(std::ostream& out, bool showShips) const
{
    out << "  ";
    for (int x = 0; x < size; x++)
        out << x << " ";
    out << "\n";

    for (int y = 0; y < size; y++) {
        out << y << " ";
        for (int x = 0; x < size; x++) {
            char c = '-';

            switch (grid[y][x]) {
            case CellState::Empty:
                c = '-';
                break;
            case CellState::Ship:
                c = showShips ? '0' : '-';
                break;
            case CellState::Hit:
                c = 'X';
                break;
            case CellState::Miss:
                c = '/';
                break;
            }

            out << c << " ";
        }
        out << "\n";
    }
}