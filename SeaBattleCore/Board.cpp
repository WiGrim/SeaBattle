#include "pch.h"
#include "Board.h"

Board::Board(int size) : size(size)
{
}

int Board::getSize() const
{
    return size;
}