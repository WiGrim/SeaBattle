#pragma once
#include <vector>
#include <utility>

enum class Orientation { Horizontal, Vertical };

class Ship
{
public:
    Ship(int x, int y, int length, Orientation orientation);

    std::vector<std::pair<int, int>> getCoordinates() const;

private:
    int startX;
    int startY;
    int length;
    Orientation orientation;
};