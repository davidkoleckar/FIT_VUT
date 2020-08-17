#ifndef BOARDFIELD_H
#define BOARDFIELD_H

#include "Field.h"
#include <iostream>

using namespace std;
/**
 * Třída reprezentující aktivní pole na hrací desce.
 * @author Ondrej Janota - xjanot02
 * @author David Koleckar - xkolec07
 */
class BoardField : public Field
{
    public:
        BoardField(int rows, int cols);
        Disk *getDisk();
        bool canPutDisk();
        void addNextField(Direction dirs, Field *new_field);
        Field *nextField(Direction dirs);
        bool putDisk(Disk *new_disk);
        void removeDisk();
        pair<int, int> getPosition();
    protected:
        Field *border[Field::Direction::COUNT];
    private:
        int row;
        int col;
        Disk *disk;
};

#endif // BOARDFIELD_H
