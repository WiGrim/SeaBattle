#include "pch.h"
#include "Game.h"
#include "CellState.h"
#include <algorithm>

Game::Game(const std::string& name1, const std::string& name2, int boardSize)
    : player1(name1, boardSize), player2(name2, boardSize),
    currentPlayer(&player1), opponent(&player2)
{
}

void Game::shootAtOpponent(int x, int y, std::ostream& out = std::cout)
{
    bool hit = opponent->board.shoot(x, y);
    if (hit)
    {
        out << "Hit at (" << x << ", " << y << ")!\n";
        for (auto& ship : opponent->getShips())
        {
            if (ship.registerHit(x, y))
                break;
        }
        if (isGameOver()) {
            winnerName = currentPlayer->name;
        }
    }
    else
    {
        out << "Miss at (" << x << ", " << y << ").\n";
        std::swap(currentPlayer, opponent);
    }
}

void Game::start() {
    if (!player1.allShipsPlaced() || !player2.allShipsPlaced())
        throw std::logic_error("Both players must place ships before starting");
    currentPlayer = &player1;
    opponent = &player2;
}

bool Game::isGameOver() const {
    return player1.allShipsDestroyed() || player2.allShipsDestroyed();
}