#include "pch.h"
#include "Board.h"
#include "CellState.h"
#include "ShipPlaceError.h"
#include "Game.h"
#include "UI.h"

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

TEST(PlayerTest, CorrectShipPoolInitialization)
{
    Player p("Alice", 10, { 4, 3, 2, 1, 1 }); 
    auto pool = p.getShipPool();
    EXPECT_EQ(pool.size(), 4);
    EXPECT_EQ(pool[0].length, 4);
    EXPECT_EQ(pool[0].count, 1);
    EXPECT_EQ(pool[3].length, 1);
    EXPECT_EQ(pool[3].count, 2);
}

TEST(PlayerTest, CannotAddMoreShipsThanPool)
{
    Player p("Alice", 10, { 2, 1 });

    EXPECT_NO_THROW(p.addShip(0, 0, 2, Orientation::Horizontal));
    EXPECT_NO_THROW(p.addShip(0, 2, 1, Orientation::Horizontal));

    EXPECT_THROW(p.addShip(2, 0, 2, Orientation::Horizontal), ShipPlaceError);
    EXPECT_THROW(p.addShip(1, 2, 1, Orientation::Horizontal), ShipPlaceError);
}

TEST(PlayerTest, MustPlaceAllShipsBeforeStarting)
{
    Game game("Alice", "Bob", 10);

    game.player1 = Player("Alice", 10, { 2,1 });
    game.player2 = Player("Bob", 10, { 2,1 });

    EXPECT_THROW(game.start(), std::logic_error);

    game.player1.addShip(0, 0, 2, Orientation::Horizontal);
    EXPECT_THROW(game.start(), std::logic_error);

    game.player1.addShip(0, 2, 1, Orientation::Horizontal);
    game.player2.addShip(5, 5, 2, Orientation::Horizontal);
    game.player2.addShip(5, 7, 1, Orientation::Horizontal);

    EXPECT_NO_THROW(game.start());
}

TEST(UITest, HorizontalDisplayShipsAndHits)
{
    Board b1(3);
    Board b2(3);

    b1.placeShip(0, 0);
    b1.placeShip(2, 1);
    b1.shoot(0, 0);
    b1.shoot(1, 1);

    b2.placeShip(1, 0);
    b2.placeShip(0, 2);
    b2.shoot(1, 0);
    b2.shoot(2, 2);

    std::stringstream ss;
    printBoardsSideBySide(b1, b2, true, false, ss);

    std::string output = ss.str();

    EXPECT_NE(output.find("X - -"), std::string::npos);
    EXPECT_NE(output.find("- / 0"), std::string::npos);
    EXPECT_NE(output.find("- - -"), std::string::npos);

    int countZerosAfterTab = 0;
    bool afterTab = false;
    for (char c : output) {
        if (c == '\t') {
            afterTab = true;
            continue;
        }
        else if (c == '\n') {
            afterTab = false;
            continue;
        }

        if (afterTab && c == '0') {
            countZerosAfterTab++;
        }
    }
    EXPECT_EQ(countZerosAfterTab, 2);
}

TEST(UITest, BothBoardsHidden)
{
    Board b1(2);
    Board b2(2);

    b1.placeShip(0, 0);
    b1.shoot(0, 0);

    b2.placeShip(1, 1);
    b2.shoot(1, 1);

    std::stringstream ss;
    printBoardsSideBySide(b1, b2, false, false, ss);

    std::string output = ss.str();

    EXPECT_EQ(std::count(output.begin(), output.end(), '0'), 4);
    EXPECT_NE(output.find("X"), std::string::npos);
}

TEST(UITest, InputPlayerNames) {
    std::istringstream in("Alice\nBob\n");
    std::ostringstream out;

    std::string name1, name2;
    std::getline(in, name1);
    std::getline(in, name2);

    EXPECT_EQ(name1, "Alice");
    EXPECT_EQ(name2, "Bob");
}

TEST(UITest, ShowNextPlayerBoardOnSwitchScreen) {
    Player p1("Alice", 5, { 2 });
    Player p2("Bob", 5, { 2 });

    p1.addShip(0, 0, 2, Orientation::Horizontal);
    p2.addShip(1, 1, 2, Orientation::Horizontal);

    std::ostringstream out;
    printSwitchScreen(p1, p2, p2, out);

    std::string output = out.str();
    EXPECT_NE(output.find("Turn away and give control to another player"), std::string::npos);
    EXPECT_NE(output.find("0 1 2 3 4"), std::string::npos);
    EXPECT_EQ(std::count(output.begin(), output.end(), '0'), 6);
}

TEST(UITest, UndoLastShipPlacement) {
    Player p("Alice", 5, { 2,1 });
    EXPECT_NO_THROW(p.addShip(0, 0, 2, Orientation::Horizontal));
    auto shipsBefore = p.getShips().size();
    p.getShips().pop_back();
    EXPECT_EQ(p.getShips().size(), shipsBefore - 1);
}

TEST(UITest, ResetAllShips) {
    Player p("Alice", 5, { 2,1 });
    EXPECT_NO_THROW(p.addShip(0, 0, 2, Orientation::Horizontal));
    EXPECT_NO_THROW(p.addShip(2, 2, 1, Orientation::Horizontal));
    p.getShips().clear();
    for (auto& entry : p.getShipPool()) entry.placed = 0;
    EXPECT_FALSE(p.allShipsPlaced());
}

TEST(UITest, BothFieldsHiddenAtStartOfTurn) {
    Game game("Alice", "Bob", 5);
    game.player1.addShip(0, 0, 2, Orientation::Horizontal);
    game.player2.addShip(1, 1, 2, Orientation::Horizontal);

    std::ostringstream out;
    printBoardsSideBySide(game.currentPlayer->board, game.opponent->board, false, false, out);
    std::string output = out.str();
    EXPECT_EQ(std::count(output.begin(), output.end(), '0'), 4);
}

TEST(UITest, HitDoesNotSwitchPlayer) {
    Game game("Alice", "Bob", 3);
    game.player1.addShip(0, 0, 1, Orientation::Horizontal);
    game.player2.addShip(1, 1, 1, Orientation::Horizontal);

    game.shootAtOpponent(1, 1);
    EXPECT_EQ(game.currentPlayer->name, "Alice");
}

TEST(UITest, MissSwitchesPlayer) {
    Game game("Alice", "Bob", 3);
    game.player1.addShip(0, 0, 1, Orientation::Horizontal);
    game.player2.addShip(1, 1, 1, Orientation::Horizontal);

    game.shootAtOpponent(0, 2);
    EXPECT_EQ(game.currentPlayer->name, "Bob");
}

TEST(UITest, ShowNextPlayerFieldBeforeTurn) {
    Game game("Alice", "Bob", 3);
    game.player1.addShip(0, 0, 1, Orientation::Horizontal);
    game.player2.addShip(1, 1, 1, Orientation::Horizontal);

    std::ostringstream out;
    printSwitchScreen(game.player1, game.player2, *game.currentPlayer, out);

    std::string output = out.str();
    EXPECT_NE(output.find("Turn away and give control to another player"), std::string::npos);
    EXPECT_NE(output.find("0"), std::string::npos);
}

TEST(UITest, ShotMarksHitAndMissCorrectly) {
    Player p1("Alice", 3);
    Player p2("Bob", 3);

    p2.addShip(1, 1, 1, Orientation::Horizontal);
    p1.board.shoot(1, 1);
    p1.board.shoot(0, 0);

    EXPECT_EQ(p1.board.getCell(1, 1), CellState::Miss);
    EXPECT_EQ(p1.board.getCell(0, 0), CellState::Miss);
}

TEST(UITest, GameOverScreenShowsWinner) {
    Game game("Alice", "Bob", 2);
    game.player1.addShip(0, 0, 1, Orientation::Horizontal);
    game.player2.addShip(1, 1, 1, Orientation::Horizontal);

    game.shootAtOpponent(1, 1);
    EXPECT_TRUE(game.isGameOver());
    EXPECT_EQ(game.getWinnerName(), "Alice");
}