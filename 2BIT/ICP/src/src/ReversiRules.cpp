#include "../include/ReversiRules.h"
/**
 * Třída implementující rozhraní Rules.
 * Reprezentuje pravidla inicializace hry Reversi.
 * @author Ondrej Janota - xjanot02
 */

 /**
  * Inicializace pravidel.
  * @param size Velikost hrací desky.
  */
ReversiRules::ReversiRules(int new_size)
{
    //ctor
    size = new_size;
}
/**
 * @return Vrací velikost desky.
 */     
int ReversiRules::getSize()
{
    return size;
}
/**
 * Vytvoří field na daných souřadnicích
 * @param row Řádek
 * @param col Sloupec
 * @return Vytvořený field.
 */   
Field *ReversiRules::createField(int row, int col)
{
    return (Field *) new BoardField(row, col);
}
