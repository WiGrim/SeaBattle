#pragma once
#include "Player.h"

class Game {
public:
    Player player1;
    Player player2;

    Player* currentPlayer;
    Player* opponent;

    Game(const std::string& name1, const std::string& name2, int boardSize);

    void shootAtOpponent(int x, int y);

    void start();
    bool isGameOver() const;

    std::string getWinnerName() const {
        return winnerName;
    }

private:
    std::string winnerName;
};