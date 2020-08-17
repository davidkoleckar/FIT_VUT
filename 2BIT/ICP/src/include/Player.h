#ifndef PLAYER_H
#define PLAYER_H

#include "Field.h"
#include "Board.h"
#include "Disk.h"
/**
 * Reprezentuje hráče hry. Hráč má bílou nebo černou barvu. 
 * Po vytvoření reprezentuje volného hráče.
 * Součástí hráče je sada kamenů, které má k dispozici pro vkládání na desku.
 * Volný hráč musí být inicializován v rámci hrací desky
 * @author Ondrej Janota - xjanot02
 * @author David Koleckar - xkolec07
 */
class Player
{
    public:
        Player(bool is_white);
        void initialize(Board *board);
        bool is_white();
        int getCount();

        bool canPutDisk(Field *field);
        bool testBorder(Field *field, Field::Direction dirs, bool canTurn, bool first);
        bool putDisk(Field *field);
        bool checkMoves();
        
        pair<Field *, vector<Field *>> *getUndoMove();
        pair<Field *, vector<Field *>> *getRedoMove();
        
        vector<pair<Field*, vector<Field*>>> getUndo();
        void setUndo(vector<pair<Field*, vector<Field*>>> new_undo);
        void removeRedo();
        int getIndex();
        void setIndex(int new_index);

        int disks;
    protected:
    private:
        bool border(Field *field, bool canTurn);

        bool isWhite;
        Board *board;
        
        vector<pair<Field*, vector<Field*>>> undo;
        vector<Field *> turned;
        
        unsigned int index;
};

#endif // PLAYER_H
