/**
 * Projekt: PDS - DHCP attacks
 *
 * @file dhcprogue.h
 *
 * @author David Koleckar - xkolec07
 *
 * @brief Hlavickovy soubor pro dhcprogue.cpp
 */
#ifndef DHCPROGUE_H
#define DHCPROGUE_H

//vlozeni hlavickovych souboru
#include <iostream>
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
 * Funkce pro parsovani retezce reprezentujici adresni pool.
 *
 * @param input Vstupni retezec s adresnim poolem
 * @return Retezec s prvni adresou.
 */
std::string addresPool(std::string input);

/**
 * Funkce pro parsovani retezce reprezentujici adresni pool.
 *
 * @param input Vstupni retezec s adresnim poolem
 * @return Retezec s druhou adresou.
 */
std::string addresPool2(std::string input);

/**
 * Funkce pro inkrementovani adresy IPV4 adresy
 *
 * @param input Vstupni retezec s IPv4 adresou
 * @return Retezec s IPv4 adresou.
 */
std::string incrementAddress(std::string input);

/**
 * Funkce pro zjisteni velikosti adresniho poolu
 *
 * @param input Vstupni retezec (adresni pool)
 * @return Cislo udavajici pocet adres
 */
int addressPoolSize(std::string input);

/**
 * Funkce pro kontrolu, zda zaslat NAK zpravu
 *
 * @param requestedIP Pozadovana IPv4 adresa
 * @param lastIP Naposled pouzita IPv4 adresa
 * @param adr2 Maximalni mozna IPv4 adresa
 * @return Pravdivostni hodnota true v pripade ze se jedna o NAK,
 * 			v opacnem pripade false.
 */
bool checkNAK(const char *requestedIP,const char *lastIP, const char *adr2);

/**
 * Funkce spusteni serveru (odchytavani a zasilani dat)
 *
 * @param iface Nazev rozhrani
 * @param domain Nazev domain
 * @param pool Adresni pool
 * @param lease Doba vypujcky
 * @param dns Nazev dns serveru
 * @param gateway Adresa gateway
 */
void runServer(std::string iface, std::string domain, std::string pool, int lease, std::string dns, std::string gateway);

#endif
