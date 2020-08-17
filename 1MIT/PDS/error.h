/**
 * Projekt: PDS - DHCP attacks
 *
 * @file error.h
 *
 * @author David Koleckar - xkolec07
 *
 * @brief Hlavickovy soubor pro error.cpp
 */
#ifndef ERROR_H
#define ERROR_H

#include <iostream>


enum ecodes{
	EARG=0,     // Chyba argumentu
	EUNKNOWN,   // Neznama chyba
	ESOCKET,    // Chyba socketu
	EBIND,      // Chyba pri bind
	EIOCTL,     // Chyba pri IOCTL
	ESEND,      // Chyba pri posilani
	ERECV       // Chyba pri prijmani
};

/**
 * Funkce pro vypis chyby
 *
 * @param ecode Typ chyby.
 */
void printEcode(int ecode);

#endif
