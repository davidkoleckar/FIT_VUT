#ifndef RULES_H
#define RULES_H

#include "Field.h"
/**
 * Rozhraní reprezentující pravidla inicializace hry.
 * @author Ondrej Janota - xjanot02
 * @author David Koleckar - xkolec07
 */
class Rules
{
    public:
        virtual ~Rules() = default;
        virtual int getSize()
        {
        	return 0;
        };
        virtual Field *createField(int row, int col)
        {

        	return row == col ? nullptr : nullptr;
        };
    protected:
    private:
};

#endif // RULES_H
