#pragma once

class Board
{
public:
    Board(int size);

    int getSize() const;

private:
    int size;
};