#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Board.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <sstream>

using namespace std;
/**
 * Reprezentuje hru. Při inicializaci se vždy přiřadí hrací deska.
 * @author Ondrej Janota - xjanot02
 * @author David Koleckar - xkolec07
 */
class Game
{
    public:
        Game(Board *new_board, bool isComp, int ai);
        Board *getBoard();
        Player *currentPlayer();
        Player *nextPlayer();
        Player *getNextPlayer();
        bool addPlayer(Player *new_player);
        void AImoves();
        void saveGame(string name);
        bool getEnd();
        void setEnd();
        int getDisksWhite();
        int getDisksBlack();
        void setDisks(bool isWhite, int count);
        void setCurrent(bool new_current);
        bool isAI;
    protected:
    private:
        void getDisks(bool isWhite, vector<pair<int,int>> *positions);
        Board *board;
        Player *player[2];
        int disksCount[2];
        bool current;
        int algorithm;
        bool isEnd;
};

#endif // GAME_H
