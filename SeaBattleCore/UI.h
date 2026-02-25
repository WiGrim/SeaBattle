#pragma once
#include <ostream>
#include "Board.h"
#include "Player.h"

void printBoardsSideBySide(const Board& b1, const Board& b2, bool showShips1, bool showShips2, std::ostream& out);
void printSwitchScreen(const Player& player1, const Player& player2, const Player& currentPlayer, std::ostream& out);

void showSwitchMessage(std::ostream& out);
void showBoardsForSwitch(const Player& player1, const Player& player2, const Player& currentPlayer, std::ostream& out);