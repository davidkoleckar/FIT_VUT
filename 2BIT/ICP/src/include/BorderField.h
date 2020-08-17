#ifndef BORDERFIELD_H
#define BORDERFIELD_H

#include "Field.h"

/**
 * Třída reprezentující okrajová (neaktivní) pole desky.
 * Pole neuchovává znalost o svém okolí a nelze ne něj umístit kámen.
 * 
 * @author Ondrej Janota - xjanot02
 * @author David Koleckar - xkolec07
 */
class BorderField : public Field
{
    public:
        BorderField() {};
        Disk *getDisk()
        {
        	return nullptr;
        };
        bool canPutDisk()
        {
        	return false;
        };
        void addNextField(Direction dirs, Field *new_field)
        {
        	(dirs = Direction::COUNT) ? new_field = nullptr : new_field = nullptr;
        };
        Field *nextField(Direction dirs)
        {
        	return (dirs = Direction::COUNT) ? nullptr : nullptr;
        };
        bool putDisk(Disk *new_disk)
        {
        	return (new_disk = nullptr) ? false : true;
        };
    protected:
    private:
};

#endif // BORDERFIELD_H
