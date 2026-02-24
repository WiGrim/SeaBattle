#include "pch.h"
#include "Ship.h"

Ship::Ship(int x, int y, int length, Orientation orientation)
    : startX(x), startY(y), length(length), orientation(orientation)
{
}

std::vector<std::pair<int, int>> Ship::getCoordinates() const
{
    std::vector<std::pair<int, int>> coords;
    for (int i = 0; i < length; i++)
    {
        if (orientation == Orientation::Horizontal)
            coords.push_back({ startX + i, startY });
        else
            coords.push_back({ startX, startY + i });
    }
    return coords;
}