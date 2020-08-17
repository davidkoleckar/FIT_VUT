#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "Rules.h"
#include "Field.h"
#include "BorderField.h"
#include "BoardField.h"

using namespace std;
/**
 * Třída reprezentující hrací desku.
 * Deska má velikost (rozměr) N a rozlišuje aktivní a neaktivní pole.
 * Aktivní pole jsou umístěna na řádcích a sloupcích, 
 * které jsou číslovány od 1 do N. 
 * Neaktivní (okrajová) pole jsou na sloupcích a řádcích 
 * indexovaných hodnotami 0 a N+1.
 * @author Ondrej Janota - xjanot02
 * @author David Koleckar - xkolec07
 */
class Board
{
    public:
        Board(Rules *new_rules);
        Rules *getRules();
        Field *getField(int row, int col);
    protected:
        Rules *rules;
        vector<vector<Field*>> field;
    private:
        void setBorder(int row, int col);
};

#endif // BOARD_H
