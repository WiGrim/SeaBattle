#include "pch.h"
#include "Board.h"
#include "CellState.h"
#include "ShipPlaceError.h"

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

TEST(BoardTest, ShootHitsShip)
{
    Board board(10);
    board.placeShip(0, 0);

    board.shoot(0, 0);

    EXPECT_EQ(board.getCell(0, 0), CellState::Hit);
}

TEST(BoardTest, ShootNotHitsShip)
{
    Board board(10);
    board.placeShip(0, 0);

    board.shoot(0, 1);

    EXPECT_EQ(board.getCell(0, 1), CellState::Miss);
}

TEST(BoardTest, GameOverWhenAllShipsGone)
{
    Board board(2);
    board.placeShip(0, 0);
    board.shoot(0, 0);
    EXPECT_TRUE(board.isGameOver());
}

TEST(BoardTest, CannotPlaceShipNearToOther)
{
    Board board(2);
    board.placeShip(0, 0);
    EXPECT_THROW(board.placeShip(0, 0), ShipPlaceError);
}

TEST(BoardTest, PrintEmptyBoard)
{
    Board board(2);

    std::ostringstream out;
    board.print(out, true);

    std::string expected =
        "  0 1 \n"
        "0 - - \n"
        "1 - - \n";

    EXPECT_EQ(out.str(), expected);
}

TEST(BoardTest, PrintBoardWithShipAndHit)
{
    Board board(2);
    board.placeShip(0, 0);
    board.shoot(0, 0);
    board.shoot(1, 0);

    std::ostringstream out;
    board.print(out, true);

    std::string expected =
        "  0 1 \n"
        "0 X / \n"
        "1 - - \n";

    EXPECT_EQ(out.str(), expected);
}

TEST(BoardTest, PrintBoardHideShips)
{
    Board board(2);
    board.placeShip(0, 0);

    std::ostringstream out;
    board.print(out, false);

    std::string expected =
        "  0 1 \n"
        "0 - - \n"
        "1 - - \n";

    EXPECT_EQ(out.str(), expected);
}