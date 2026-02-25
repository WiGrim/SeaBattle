#include "pch.h"
#include "Player.h"
#include "ShipPlaceError.h"

Player::Player(const std::string& n, int boardSize)
    : name(n), board(boardSize)
{
    shipPool = {
        {4, 1, 0},
        {3, 2, 0},
        {2, 3, 0},
        {1, 4, 0}
    };
}

Player::Player(const std::string& n, int boardSize, std::initializer_list<int> shipLengths)
    : name(n), board(boardSize)
{
    for (int length : shipLengths)
    {
        if (length <= 0)
            throw std::invalid_argument("Ship length must be positive");
        bool found = false;

        for (auto& entry : shipPool)
        {
            if (entry.length == length)
            {
                entry.count++;
                found = true;
                break;
            }
        }

        if (!found)
        {
            shipPool.push_back({ length, 1, 0 });
        }
    }
}

bool Player::addShip(int x, int y, int length, Orientation orientation)
{
    bool found = false;

    for (auto& entry : shipPool)
    {
        if (entry.length == length)
        {
            found = true;

            if (entry.placed >= entry.count)
                throw ShipPlaceError("Ship limit exceeded");

            break;
        }
    }

    if (!found)
        throw ShipPlaceError("Invalid ship length");

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

    for (auto& entry : shipPool)
    {
        if (entry.length == length)
        {
            entry.placed++;
            break;
        }
    }

    return true;
}

bool Player::allShipsDestroyed() const {
    for (const auto& ship : ships) {
        if (!ship.isDestroyed())
            return false;
    }
    return true;
}

bool Player::allShipsPlaced() const
{
    for (const auto& entry : shipPool)
    {
        if (entry.placed != entry.count)
            return false;
    }
    return true;
}

bool Player::undoLastShipPlacement() {
    if (ships.empty()) return false;

    Ship last = ships.back();
    ships.pop_back();

    for (auto [x, y] : last.getCoordinates())
        board.setCell(x, y, CellState::Empty);

    int shipLength = static_cast<int>(last.getCoordinates().size());

    for (auto& entry : shipPool)
        if (entry.length == shipLength) {
            entry.placed--;
            break;
        }

    return true;
}

void Player::resetAllShips() {
    for (auto& ship : ships)
        for (auto [x, y] : ship.getCoordinates())
            board.setCell(x, y, CellState::Empty);

    ships.clear();
    for (auto& entry : shipPool)
        entry.placed = 0;
}