#include "pch.h"
#include "Game.h"
#include "CellState.h"
#include <algorithm>

Game::Game(const std::string& name1, const std::string& name2, int boardSize)
    : player1(name1, boardSize), player2(name2, boardSize),
    currentPlayer(&player1), opponent(&player2)
{
}

void Game::shootAtOpponent(int x, int y)
{
    bool hit = opponent->board.shoot(x, y);
    if (!hit) {
        std::swap(currentPlayer, opponent);
    }
}