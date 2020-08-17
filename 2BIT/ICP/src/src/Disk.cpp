#include "../include/Disk.h"
/**
 * Třída reprezentuje jeden kámen.
 * Kámen může nabývat dvou barev - bílá nebo černá.
 * 
 * @author Ondrej Janota - xjanot02
 * @author David Koleckar - xkolec07
 */
 
 /**
 * Inicializace kamene.
 * @param isWhite Nastavení barvy - true je bílá, false je černá.
 */
Disk::Disk(bool is_white)
{
    //ctor
    isWhite = is_white;
}
/**
 * Otočení (změna barvy) kamene.
 */
void Disk::turn()
{
    isWhite = !isWhite;
}
/**
 * Test, zda je kámen bílý.
 * @return Vrací true, pokud je kámen bílý.
 */
bool Disk::is_white()
{
    return isWhite;
}
