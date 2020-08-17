#ifndef FIELD_H
#define FIELD_H

#include "Disk.h"
/**
 * Rozhraní k polím, které lze umístit na hrací desku.
 * Pole zná své okolí, tj. pole, která ho obklopují ve všech osmi směrech.
 * Implementující třída respektuje při vkládání kamenů pravidla dané hry.
 * @author Ondrej Janota - xjanot02
 * @author David Koleckar - xkolec07
 */
class Field
{
    public:
        enum Direction
        {
            U,
            RU,
            R,
            RD,
            D,
            LD,
            L,
            LU,
            COUNT
        };
        virtual ~Field() = default;
        virtual Disk *getDisk(){return nullptr;};
        virtual bool canPutDisk(){return false;};
        virtual void addNextField(Direction dirs, Field *new_field)
        {
            (dirs = Direction::COUNT) ? new_field = nullptr : new_field = nullptr;
        };
        virtual Field *nextField(Direction dirs)
        {
            return (dirs = Direction::U) ? nullptr : nullptr;
        };
        virtual bool putDisk(Disk *new_disk)
        {
            return (new_disk = nullptr) ? false : true;
        };
    protected:
    private:
};

#endif // FIELD_H
