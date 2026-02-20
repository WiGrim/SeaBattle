#include "pch.h"
#include "gtest/gtest.h"
#include "Board.h"
#include "CellState.h"

TEST(BoardTest, BoardCreation)
{
    Board board(10);
}

TEST(BoardTest, BoardSize)
{
    Board board(10);
    EXPECT_EQ(board.getSize(), 10);
}

TEST(BoardTest, CellInitiallyEmpty)
{
    Board board(10);
    EXPECT_EQ(board.getCell(0, 0), CellState::Empty);
}

TEST(BoardTest, PlaceShipOnCell)
{
    Board board(10);

    board.placeShip(0, 0);

    EXPECT_EQ(board.getCell(0, 0), CellState::Ship);
}

TEST(BoardTest, PlaceShipOutOfBounds)
{
    Board board(10);

    EXPECT_THROW(board.placeShip(20, 20), std::out_of_range);
}