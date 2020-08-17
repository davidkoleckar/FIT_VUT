#include "../include/Player.h"
/**
 * Reprezentuje hráče hry. Hráč má bílou nebo černou barvu. 
 * Po vytvoření reprezentuje volného hráče.
 * Součástí hráče je sada kamenů, které má k dispozici pro vkládání na desku.
 * Volný hráč musí být inicializován v rámci hrací desky
 * @author Ondrej Janota - xjanot02
 */

 /**
  * Inicializace hráče.
  * @param isWhite Určuje barvu hráče (true = bílý, false = černý)
  */
Player::Player(bool is_white)
{
    //ctor
    isWhite = is_white;
    disks = 0;
    index = 0;
}
    
/**
 * Inicializace hráče v rámci hrací desky.
 * Vytvoří sadu kamenů o příslušné velikosti a umístí 
 * své počáteční kameny na desku.
 * @param board Deska, v rámci které se hráč inicializuje.
 */
void Player::initialize(Board *new_board)
{
    board = new_board;

    if (!isWhite)
    {
        board->getField(board->getRules()->getSize()/2, board->getRules()->getSize()/2)->putDisk(new Disk(isWhite));
        board->getField(board->getRules()->getSize()/2 + 1, board->getRules()->getSize()/2 + 1)->putDisk(new Disk(isWhite));
    }
    else
    {
        board->getField(board->getRules()->getSize()/2 + 1, board->getRules()->getSize()/2)->putDisk(new Disk(isWhite));
        board->getField(board->getRules()->getSize()/2, board->getRules()->getSize()/2 + 1)->putDisk(new Disk(isWhite));
    }
    disks += 2;
}
/**
 * Test barvy hráče.
 * @return Zda je hráč bílý.
 */
bool Player::is_white()
{
    return isWhite;
}
/**
 * @return Vrací počet disků hráče na desce.
 */     
int Player::getCount()
{
    return disks;
}

bool Player::testBorder(Field *field, Field::Direction dirs, bool canTurn, bool first)
{
    Disk *disk = nullptr;

    if ((disk = field->getDisk()) != nullptr)
    {
        if (disk->is_white() == isWhite){
            if(!first)
                return true;
            }
        else
            if (testBorder(field->nextField(dirs), dirs, canTurn, false))
            {
                if (canTurn)
                {
                    disk->turn();
                    disks++;
                    turned.push_back(field);
                }
                return true;
            }
            else
                return false;
    }
    return false;
}

bool Player::border(Field *field, bool canTurn)
{
    bool can = false;
    bool isTrue = false;

    can = testBorder(field->nextField(Field::R), Field::R, canTurn, true);

    isTrue = testBorder(field->nextField(Field::RD), Field::RD, canTurn, true);

    if (isTrue && !can)
        can = true;

    isTrue = testBorder(field->nextField(Field::D), Field::D, canTurn, true);

    if (isTrue && !can)
        can = true;

    isTrue = testBorder(field->nextField(Field::LD), Field::LD, canTurn, true);

    if (isTrue && !can)
        can = true;

    isTrue = testBorder(field->nextField(Field::L), Field::L, canTurn, true);

    if (isTrue && !can)
        can = true;

    isTrue = testBorder(field->nextField(Field::LU), Field::LU, canTurn, true);

    if (isTrue && !can)
        can = true;

    isTrue = testBorder(field->nextField(Field::U), Field::U, canTurn, true);

    if (isTrue && !can)
        can = true;

    isTrue = testBorder(field->nextField(Field::RU), Field::RU, canTurn, true);

    if (isTrue && !can)
        can = true;

    return can;
}
/**
 * Test, zda je možné vložit nový kámen hráče na dané pole. 
 * Kámen se sady nevybírá ani nevkládá na pole.
 * @param field Pole, na které se má vkládat kámen.
 * @return Zda je možné kámen vložit.
 */
bool Player::canPutDisk(Field *field)
{
    if (field->getDisk() == nullptr)
        if (border(field, false))
            return true;

    return false;
}
/**
 * Vloží nový kámen hráče na dané pole, pokud to pravidla umožňují. 
 * Pokud lze vložit, vybere kámen ze sady a vloží na pole.
 * @param field Pole, na které se vkládá kámen.
 * @return Úspěch akce.
 */
bool Player::putDisk(Field *field)
{
    if(field->getDisk() != nullptr)
        return false;

    if (canPutDisk(field))
    {
        border(field, true);
        field->putDisk(new Disk(isWhite));
        disks++;
        index++;
        undo.push_back(make_pair(field, turned));
        turned.clear();
    }
    else
        return false;

    return true;
}

/**
 * @return Vrací true, pokud hráč má nějaký tah.
 */     
bool Player::checkMoves()
{
    for (int i = 1; i < board->getRules()->getSize() + 1; i++)
        for (int j = 1; j < board->getRules()->getSize() + 1; j++)
            if (canPutDisk(board->getField(i, j)))
                return true;

    undo.push_back(make_pair(nullptr, turned));
    index++;
    return false;
}


/**
 * Funkce provede tah zpět.
 */    
pair<Field *, vector<Field *>> *Player::getUndoMove()
{
    if(index > 0)
    {
        return &undo[--index];
    }
    else
        return nullptr;
}

pair<Field *, vector<Field *>> *Player::getRedoMove()
{

    if(index+1 <= undo.size())
        return &undo[index++];
    else
        return nullptr;
}

vector<pair<Field*, vector<Field*>>> Player::getUndo()
{
    return undo;
}

void Player::setUndo(vector<pair<Field*, vector<Field*>>> new_undo)
{
    undo = new_undo;
}

void Player::removeRedo()
{
    undo.resize(index);
}

int Player::getIndex()
{
    return index;
}

void Player::setIndex(int new_index)
{
    index = new_index;
}