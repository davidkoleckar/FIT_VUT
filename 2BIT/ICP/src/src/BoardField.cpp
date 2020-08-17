#include "../include/BoardField.h"
/**
 * Třída reprezentující aktivní pole na hrací desce.
 * @author Ondrej Janota - xjanot02
 * @author David Koleckar - xkolec07
 */


/**
 * Inicializuje pole.
 * 
 * Pole zná informaci o svém umístění.
 * @param row Řádek, na kterém je pole umístěno.
 * @param col Sloupec, na kterém je pole umístěno.
 */
BoardField::BoardField(int rows, int cols)
{
    //ctor
    row = rows;
    col = cols;
    disk = nullptr;
    *border = (Field *)new Field[Field::Direction::COUNT];
}
   
/**
 * Vrací kámen, který je vložen na pole.
 * @return Vložený kámen. Pokud je pole prázdné, vrací null.
 */
Disk *BoardField::getDisk()
{
    return disk;
}
/**
 * Test, zda je možné vložit na pole kámen. Při testu se ověřuje soulad s pravidly hry.
 * @param disk Vkládaný kámen.
 * @return Vrací úspěšnost akce.
 */
bool BoardField::canPutDisk()
{
    return disk == nullptr;
}
/**
 * Přidá sousední pole field v daném směru dirs.
 * @param dirs Směr ve kterém se přidává pole.
 * @param field Přidávané pole.
 */
void BoardField::addNextField(Direction dirs, Field *new_field)
{
    border[dirs] = new_field;
}
/**
 * Vrátí sousední pole v daném směru dirs.
 * @param dirs Směr ve kterém se přidává pole.
 * @return Sousední pole v daném směru dirs.
 */
Field *BoardField::nextField(Direction dirs)
{
    return border[dirs];
}
/**
 * Vloží na pole kámen. Při vkládání se ověřuje soulad s pravidly hry.
 * @param disk Vkládaný kámen.
 * @return Vrací úspěšnost akce.
 */
bool BoardField::putDisk(Disk *new_disk)
{
    if (canPutDisk())
        disk = new_disk;
    else
        return false;

    return true;
}
/**
 * Odstraní disk
 */
void BoardField::removeDisk()
{
    delete[] disk;
    disk = nullptr;
}
/**
 * Vrací pozici row,col
 * @return Vrací pozici 
 */ 
pair<int, int> BoardField::getPosition()
{
    return make_pair(row, col);
}
