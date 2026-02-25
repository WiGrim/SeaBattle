#define NOMINMAX
#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>
#include "Game.h"
#include "UI.h"

void clearConsole() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = { 0, 0 };

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputCharacter(hConsole, ' ', cellCount, homeCoords, &count);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count);
    SetConsoleCursorPosition(hConsole, homeCoords);
}

void SwitchMessage() {
    clearConsole();
    showSwitchMessage(std::cout);
    clearConsole();
}

void placeShipsForPlayer(Player& player) {
    std::cout << "\n" << player.name << ", place your ships on the board.\n";
    player.board.print(std::cout, true);

    while (!player.allShipsPlaced()) {
        for (auto& entry : player.getShipPool()) {
            if (entry.placed >= entry.count)
                continue;

            std::cout << "\nPlacing ship of length " << entry.length
                << " (" << entry.count - entry.placed << " remaining)\n";
            std::cout << "Enter coordinates x y and orientation (H for horizontal, V for vertical, default H)\n";
            std::cout << "Or type 'U' to undo last ship, 'R' to reset all ships.\n";

            int x = 0, y = 0;
            char dir = 'H';

            while (true) {
                std::string line;
                std::getline(std::cin, line);

                if (line.empty()) {
                    std::cout << "No input. Try again.\n";
                    continue;
                }

                char first = std::toupper(line[0]);
                if (first == 'U') {
                    if (player.undoLastShipPlacement())
                        std::cout << "Last ship undone.\n";
                    else
                        std::cout << "No ships to undo.\n";
                    player.board.print(std::cout, true);
                    break;
                }
                if (first == 'R') {
                    player.resetAllShips();
                    clearConsole();
                    std::cout << "All ships reset.\n";
                    player.board.print(std::cout, true);
                    break;
                }

                std::istringstream iss(line);
                if (!(iss >> x >> y)) {
                    std::cout << "Invalid input. Enter two numbers.\n";
                    continue;
                }

                std::string rest;
                std::getline(iss, rest);
                dir = 'H';
                if (!rest.empty()) {
                    dir = std::toupper(rest[0]);
                    if (dir != 'H' && dir != 'V') {
                        std::cout << "Invalid orientation. Try again.\n";
                        continue;
                    }
                }

                if (x < 0 || x >= player.board.getSize() || y < 0 || y >= player.board.getSize()) {
                    std::cout << "Coordinates out of bounds. Try again.\n";
                    continue;
                }

                Orientation orientation = (dir == 'H') ? Orientation::Horizontal : Orientation::Vertical;

                try {
                    player.addShip(x, y, entry.length, orientation);
                    std::cout << "Ship placed successfully!\n";
                    player.board.print(std::cout, true);
                    break;
                }
                catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << "\nTry again.\n";
                }
            }
        }
    }
}

int main()
{
    std::cout << "Welcome to Sea Battle!\n\n";

    std::string name1, name2;
    std::cout << "Enter name for Player 1: ";
    std::getline(std::cin, name1);
    std::cout << "Enter name for Player 2: ";
    std::getline(std::cin, name2);

    const int boardSize = 10;
    Game game(name1, name2, boardSize);

    clearConsole();
    std::cout << "Turn away and give control to " << name1 << "\n";
    std::cout << "Press any key to continue...\n";
    std::cin.get();
    clearConsole();
    placeShipsForPlayer(game.player1);
    SwitchMessage();
    placeShipsForPlayer(game.player2);

    game.start();
    SwitchMessage();
    while (!game.isGameOver()) {
        std::cout << "\n--- " << game.currentPlayer->name << "'s turn ---\n";
        printBoardsSideBySide(game.player1.board, game.player2.board, false, false, std::cout);

        int x, y;
        std::string shooterName = game.currentPlayer->name;
        std::cout << "\nEnter coordinates x(->) y to fire: ";
        std::cin >> x >> y;

        try {
            game.shootAtOpponent(x, y, std::cout);
        }
        catch (const std::exception& e) {
            clearConsole();
            std::cout << "Error: " << e.what() << "\nTry again.\n";
        }
        if (game.currentPlayer->name != shooterName) {
            std::string input;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << game.currentPlayer->name <<  ", press 1 to view your field or enter to continue\n";
            std::getline(std::cin, input);
            if (!input.empty()) {
                SwitchMessage();
                printSwitchScreen(game.player1, game.player2, *game.currentPlayer, std::cout);
            }
        }
    }

    std::cout << "\nGame Over! Winner: " << game.getWinnerName() << "\n";
    return 0;
}