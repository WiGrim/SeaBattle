#include "pch.h"
#include <ostream>
#include "Board.h"
#include "Player.h"

static char cellToChar(CellState state, bool showShips)
{
    switch (state)
    {
    case CellState::Empty: return '-';
    case CellState::Ship:  return showShips ? '0' : '-';
    case CellState::Hit:   return 'X';
    case CellState::Miss:  return '/';
    }
    return '-';
}

void printBoardsSideBySide(const Board& left, const Board& right, bool showShipsLeft, bool showShipsRight, std::ostream& out)
{
    int size = left.getSize();
    if (size != right.getSize())
        throw std::logic_error("Boards must have same size");

    out << "  ";
    for (int x = 0; x < size; ++x)
        out << x << " ";
    out << "\t  ";

    for (int x = 0; x < size; ++x)
        out << x << " ";
    out << "\n";

    for (int y = 0; y < size; ++y)
    {
        out << y << " ";
        for (int x = 0; x < size; ++x)
        {
            out << cellToChar(left.getCell(x, y), showShipsLeft) << " ";
        }
        out << "\t";

        out << y << " ";
        for (int x = 0; x < size; ++x)
        {
            out << cellToChar(right.getCell(x, y), showShipsRight) << " ";
        }
        out << "\n";
    }
}

void printSwitchScreen(const Player& player1, const Player& player2, const Player& currentPlayer, std::ostream& out)
{
    bool showLeft = (&currentPlayer == &player1);
    bool showRight = (&currentPlayer == &player2);

    printBoardsSideBySide(player1.board, player2.board, showLeft, showRight, out);
}

void showSwitchMessage(std::ostream& out)
{
    out << "Turn away and give control to another player\n";
    out << "Press any key to continue...\n";
    std::cin.get();
}

void showBoardsForSwitch(const Player& player1, const Player& player2, const Player& currentPlayer, std::ostream& out)
{
    bool showLeft = (&currentPlayer == &player1);
    bool showRight = (&currentPlayer == &player2);

    printBoardsSideBySide(player1.board, player2.board, showLeft, showRight, out);

    out << "Press any key to continue...\n";
    std::cin.get();
}