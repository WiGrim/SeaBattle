#pragma once
#include <string>
#include "Board.h"

class Player {
public:
    std::string name;
    Board board;

    Player(const std::string& n, int boardSize);
};