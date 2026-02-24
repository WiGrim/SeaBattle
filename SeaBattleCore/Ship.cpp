#include "pch.h"
#include "Ship.h"

Ship::Ship(int x, int y, int length, Orientation orientation)
    : startX(x), startY(y), length(length), orientation(orientation), hits(length, false)
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

bool Ship::registerHit(int x, int y)
{
    for (int i = 0; i < length; ++i)
    {
        int cx = (orientation == Orientation::Horizontal)
            ? startX + i
            : startX;

        int cy = (orientation == Orientation::Vertical)
            ? startY + i
            : startY;

        if (cx == x && cy == y)
        {
            hits[i] = true;
            return true;
        }
    }

    return false;
}

bool Ship::isDestroyed() const
{
    for (bool hit : hits)
    {
        if (!hit)
            return false;
    }

    return true;
}