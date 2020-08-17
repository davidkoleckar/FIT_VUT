/**
 * Projekt: PDS - DHCP attacks
 *
 * @file dhcpstarve.h
 *
 * @author David Koleckar - xkolec07
 *
 * @brief Hlavickovy soubor pro dhcpstarve.cpp
 */
#ifndef DHCPSTARVE_H
#define DHCPSTARVE_H

//vlozeni hlavickovych souboru
#include <unistd.h>
#include <signal.h>
#include "error.h"
#include "network.h"


/**
 * Hlavni telo programu
 *
 * @param argc Pocet parametru.
 * @param argv Hodnota jednotlivych parametru.
 * @return Uspech programu.
 */
int main (int argc, char *argv[]);

/**
 * Funkce pro vypis napovedy.
 */
void help();

/**
 * Funkce pro zachyceni SIGINT signalu.
 * Pokud je zachycen SIGINT signal ukonci se beh aplikace s EXIT_SUCCESS.
 *
 * @param signo Typ signalu.
 */
void signalHandler(int signo);

/**
 * Funkce pro zasilani zprav typu DHCP DISCOVER
 *
 * @param iface Nazev rozhrani.
 */
void sendDiscover(const char *iface);

/**
 * Funkce pro odchytavani zprav DHCP OFFER
 *
 * @param iface Nazev rozhrani.
 */
void checkOffer(const char *iface);

/**
 * Funkce pro zasilani zprav typu DHCP REQUEST
 *
 * @param srv Pozadovana adresa z DHCP OFFER.
 * @param xid ID do DHCP hlavicky.
 * @param yaddr Adresa klienta.
 * @param chaddr MAC adresa.
 * @param socket Socket.
 */
void sendRequest(unsigned int srv, unsigned int xid, unsigned int yaddr, unsigned char *chaddr, int socket);

#endif
