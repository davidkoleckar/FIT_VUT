#ifndef REVERSIRULES_H
#define REVERSIRULES_H

#include "Rules.h"
#include "Field.h"
#include "BoardField.h"
/**
 * Třída implementující rozhraní Rules.
 * Reprezentuje pravidla inicializace hry Reversi.
 * @author Ondrej Janota - xjanot02
 * @author David Koleckar - xkolec07
 */

class ReversiRules : public Rules
{
    public:
        ReversiRules(int new_size);
        int getSize();
        Field *createField(int row, int col);
    protected:
    private:
        int size;
};

#endif // REVERSIRULES_H
