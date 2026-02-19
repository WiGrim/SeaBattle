#include "pch.h"
#include "gtest/gtest.h"
#include "Board.h"

TEST(BoardTest, BoardCreation)
{
    Board board(10);
}

TEST(BoardTest, BoardSize)
{
    Board board(10);
    EXPECT_EQ(board.getSize(), 10);
}