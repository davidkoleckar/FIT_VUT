#ifndef NEWGAME_H
#define NEWGAME_H

#include <fstream>
#include "Board.h"
#include "Game.h"
#include "Rules.h"
#include "ReversiRules.h"
#include <string>
/**
 * Třída reprezentuje novou hru. Vytvoří board, players, rules a game.
 * @author Ondrej Janota - xjanot02
 * @author David Koleckar - xkolec07
 */
class newGame
{
    public:
        newGame(int new_size, bool is_AI, int new_algorithm, pair<bool, string> load);
        void loadGame(string name);
        Game *game;
        void saveGame(string name);
        void doUndo();
        void doRedo();
    protected:
    private:
};

#endif // NEWGAME_H
