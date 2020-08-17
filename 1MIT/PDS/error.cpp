/**
 * Projekt: PDS - DHCP attacks
 *
 * @file error.cpp
 *
 * @author David Koleckar - xkolec07
 *
 * @brief Sprava chybovych hlaseni
 */
#include "error.h"

const char *ERRORMSG[] ={
	"Chyba parametru programu",
	"Neznama chyba",
	"Chyba pri tvorbe socketu",
	"Chyba pri bind",
	"Chyba IOCTL",
	"Chyba SEND",
	"Chyba RECV"
};

void printEcode(int ecode){
	std::cerr << ERRORMSG[ecode] << std::endl;
}
