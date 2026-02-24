#include "pch.h"
#include "Player.h"
#include "ShipPlaceError.h"

Player::Player(const std::string& n, int boardSize) : name(n), board(boardSize) {}

bool Player::addShip(int x, int y, int length, Orientation orientation)
{
    Ship newShip(x, y, length, orientation);

    for (auto [sx, sy] : newShip.getCoordinates())
    {
        if (sx < 0 || sy < 0 || sx >= board.getSize() || sy >= board.getSize())
            throw ShipPlaceError("Ship goes out of bounds");

        if (board.getCell(sx, sy) == CellState::Ship)
            throw ShipPlaceError("Ship overlaps another ship");

        for (int dy = -1; dy <= 1; dy++)
        {
            for (int dx = -1; dx <= 1; dx++)
            {
                int nx = sx + dx;
                int ny = sy + dy;

                if (nx >= 0 && ny >= 0 &&
                    nx < board.getSize() &&
                    ny < board.getSize())
                {
                    if (board.getCell(nx, ny) == CellState::Ship)
                        throw ShipPlaceError("Ship is too close to another ship");
                }
            }
        }
    }

    for (auto [sx, sy] : newShip.getCoordinates())
        board.placeShip(sx, sy);

    ships.push_back(newShip);
    return true;
}