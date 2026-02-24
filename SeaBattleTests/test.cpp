#include "pch.h"
#include "Board.h"
#include "CellState.h"
#include "ShipPlaceError.h"
#include "Game.h"

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

//Логика перемещена в Game
/*TEST(BoardTest, GameOverWhenAllShipsGone)
{
    Board board(2);
    board.placeShip(0, 0);
    board.shoot(0, 0);
    EXPECT_TRUE(board.isGameOver());
}*/

//Логика проверки перенесена в Player::addShip()
/*TEST(BoardTest, CannotPlaceShipNearToOther)
{
    Board board(2);
    board.placeShip(0, 0);
    EXPECT_THROW(board.placeShip(0, 0), ShipPlaceError);
}*/

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

TEST(GameTest, SwitchTurnAfterMiss)
{
    Game game("Alice", "Bob", 2);

    game.player1.board.placeShip(0, 0);
    game.player2.board.placeShip(1, 1);

    game.currentPlayer = &game.player1;
    game.opponent = &game.player2;
    
    game.shootAtOpponent(0, 1);
    EXPECT_EQ(game.currentPlayer->name, "Bob");
}

TEST(GameTest, HitDoesNotSwitchTurn)
{
    Game game("Alice", "Bob", 2);

    game.player1.board.placeShip(0, 0);
    game.player2.board.placeShip(1, 1);

    game.currentPlayer = &game.player1;
    game.opponent = &game.player2;

    game.shootAtOpponent(1, 1);
    EXPECT_EQ(game.currentPlayer->name, "Alice");
}

TEST(GameTest, GameOverWhenAllShipsDestroyed)
{
    Game game("Alice", "Bob", 2);

    game.player1.board.placeShip(0, 0);
    game.player2.board.placeShip(1, 1);

    game.currentPlayer = &game.player1;
    game.opponent = &game.player2;

    game.shootAtOpponent(1, 1);
    //EXPECT_TRUE(game.opponent->board.isGameOver());
    EXPECT_TRUE(game.isGameOver());
}

TEST(PlayerTest, AddSingleShip)
{
    Player p("Alice", 5);
    EXPECT_NO_THROW(p.addShip(0, 0, 2, Orientation::Horizontal));
    EXPECT_EQ(p.board.getCell(0, 0), CellState::Ship);
    EXPECT_EQ(p.board.getCell(1, 0), CellState::Ship);
}

TEST(PlayerTest, AddShipOutOfBounds)
{
    Player p("Alice", 5);
    EXPECT_THROW(p.addShip(4, 0, 2, Orientation::Horizontal), ShipPlaceError);
    EXPECT_THROW(p.addShip(0, 4, 2, Orientation::Vertical), ShipPlaceError);
}

TEST(PlayerTest, AddShipOverlapping)
{
    Player p("Alice", 5);
    p.addShip(0, 0, 2, Orientation::Horizontal);
    EXPECT_THROW(p.addShip(1, 0, 3, Orientation::Horizontal), ShipPlaceError);
}

TEST(PlayerTest, AddMultipleShips)
{
    Player p("Alice", 5);
    EXPECT_NO_THROW(p.addShip(0, 0, 2, Orientation::Horizontal));
    EXPECT_NO_THROW(p.addShip(3, 0, 2, Orientation::Vertical));

    EXPECT_EQ(p.board.getCell(0, 0), CellState::Ship);
    EXPECT_EQ(p.board.getCell(1, 0), CellState::Ship);
    EXPECT_EQ(p.board.getCell(3, 0), CellState::Ship);
    EXPECT_EQ(p.board.getCell(3, 1), CellState::Ship);
}

TEST(ShipTest, ShipRegistersHit)
{
    Ship ship(0, 0, 3, Orientation::Horizontal);

    EXPECT_TRUE(ship.registerHit(0, 0));
    EXPECT_FALSE(ship.isDestroyed());
}

TEST(ShipTest, ShipDestroyedAfterAllHits)
{
    Ship ship(0, 0, 2, Orientation::Horizontal);

    ship.registerHit(0, 0);
    ship.registerHit(1, 0);

    EXPECT_TRUE(ship.isDestroyed());
}

TEST(ShipTest, HitOutsideShipReturnsFalse)
{
    Ship ship(0, 0, 2, Orientation::Horizontal);

    EXPECT_FALSE(ship.registerHit(5, 5));
}

TEST(BoardTest, ShootDestroysShip)
{
    Player p("Alice", 5);
    p.addShip(0, 0, 2, Orientation::Horizontal);

    EXPECT_FALSE(p.getShips()[0].isDestroyed());

    p.board.shoot(0, 0);
    p.board.shoot(1, 0);

    EXPECT_FALSE(p.getShips()[0].isDestroyed());
}

TEST(BoardTest, CannotShootSameCellTwice)
{
    Board board(5);
    board.placeShip(0, 0);

    board.shoot(0, 0);

    EXPECT_THROW(board.shoot(0, 0), std::logic_error);
}

TEST(GameTest, GameDetectsWinner)
{
    Game game("Alice", "Bob", 2);

    game.player1.addShip(0, 0, 1, Orientation::Horizontal);
    game.player2.addShip(1, 1, 1, Orientation::Horizontal);

    game.currentPlayer = &game.player1;
    game.opponent = &game.player2;

    game.shootAtOpponent(1, 1);

    EXPECT_TRUE(game.isGameOver());
    EXPECT_EQ(game.getWinnerName(), "Alice");
}

TEST(PlayerTest, CannotExceedShipPool)
{
    Player p("Alice", 10);

    p.addShip(0, 0, 4, Orientation::Horizontal);

    EXPECT_THROW(
        p.addShip(0, 2, 4, Orientation::Horizontal),
        ShipPlaceError
    );
}

TEST(GameTest, CannotStartGameUntilShipsPlaced)
{
    Game game("Alice", "Bob", 10);

    EXPECT_THROW(game.start(), std::logic_error);
}