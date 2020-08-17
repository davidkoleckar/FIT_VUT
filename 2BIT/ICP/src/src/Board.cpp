#include "../include/Board.h"

#include <iostream>
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

 /**
  * Inicializuje desku. 
  * Vytvoří a uloží si všechna pole. 
  * Pro všechna pole nastaví jejich okolí. 
  * Na neaktivních polích jsou umístěny instance třídy BorderField.
  * Objekty aktivních polí jsou získány metodou Rules createField(int, int).
  * @param rules Objekt reprezentující pravidla inicializace hry.
  */
Board::Board(Rules *new_rules)
{
    //ctor
    rules = new_rules;

    field.resize(rules->getSize()+2);

    for (auto& i : field)
        i.resize(rules->getSize()+2);



    for (int i = 0; i <= rules->getSize() + 1; i++)
        for (int j = 0; j <= rules->getSize() + 1; j++)
            if (i == 0 || j == 0 || i == rules->getSize() + 1 || j == rules->getSize() + 1)
                field[i][j] = (Field *) new BorderField();
            else
                field[i][j] = (Field *) rules->createField(i, j);

    for (int i = 1; i <= rules->getSize(); i++)
        for (int j = 1; j <= rules->getSize(); j++)
            setBorder(i, j);
}
/**
 * Uložení si okolních polí.
 * @param row Řádek
 * @param col Sloupec
 */
void Board::setBorder(int row, int col)
{
    field[row][col]->addNextField(Field::L, field[row][col - 1]);
    field[row][col]->addNextField(Field::LU, field[row - 1][col - 1]);
    field[row][col]->addNextField(Field::U, field[row - 1][col]);
    field[row][col]->addNextField(Field::RU, field[row - 1][col + 1]);
    field[row][col]->addNextField(Field::R, field[row][col + 1]);
    field[row][col]->addNextField(Field::RD, field[row + 1][col + 1]);
    field[row][col]->addNextField(Field::D, field[row + 1][col]);
    field[row][col]->addNextField(Field::LD, field[row + 1][col - 1]);
}
/**
 * Vrací objekt pravidel.
 * @return Pravidla inicializace hry.
 */
Rules *Board::getRules()
{
    return rules;
}
/**
 * Vrátí pole umístěné na uvedených indexech row a col.
 * @param row Řádek
 * @param col Sloupec
 * @return Pole na pozici [row][col].
 */
Field *Board::getField(int row, int col)
{
    return field[row][col];
}
